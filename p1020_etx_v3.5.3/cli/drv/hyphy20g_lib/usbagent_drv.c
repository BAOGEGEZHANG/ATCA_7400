
/*
 * USB Skeleton driver - 0.7
 *
 * Copyright (C) 2001 Greg Kroah-Hartman (greg@kroah.com)
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation; either version 2 of
 *	the License, or (at your option) any later version.
 *
 *
 * This driver is to be used as a skeleton driver to be able to create a
 * USB driver quickly.  The design of it is based on the usb-serial and
 * dc2xx drivers.
 *
 * Thanks to Oliver Neukum and David Brownell for their help in debugging
 * this driver.
 *
 * TODO:
 *	- fix urb->status race condition in write sequence
 *	- move dev_table to a dynamic list.
 *
 * History:
 *
 * 2002_02_12 - 0.7 - zero out dev in probe function for devices that do
 *			not have both a bulk in and bulk out endpoint.
 *			Thanks to Holger Waechtler for the fix.
 * 2001_11_05 - 0.6 - fix minor locking problem in skel_disconnect.
 *			Thanks to Pete Zaitcev for the fix.
 * 2001_09_04 - 0.5 - fix devfs bug in skel_disconnect. Thanks to wim delvaux
 * 2001_08_21 - 0.4 - more small bug fixes.
 * 2001_05_29 - 0.3 - more bug fixes based on review from linux-usb-devel
 * 2001_05_24 - 0.2 - bug fixes based on review from linux-usb-devel people
 * 2001_05_01 - 0.1 - first version
 *
 */

#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/errno.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fcntl.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/smp_lock.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/usb.h>
#include <linux/usbdevice_fs.h>
#include <linux/proc_fs.h>

#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <net/sock.h>

#include "usbagent_drv.h"

#define FXLOAD_NETLINK_TYPE  27

/* Useless variable */
static int debug = 0;

//#define DBG_PRINT(format, args...)   printk(format, ## args)
#define DBG_PRINT(format, args...)

/* Version Information */
#define DRIVER_VERSION "v0.4"
#define DRIVER_AUTHOR "Greg Kroah-Hartman, greg@kroah.com"
#define DRIVER_DESC "USB CY7C68013A Driver"

static const char *const ezusb_proc_dir = "usbagent";
static const char *const usbagent_proc_filename = "usbagent";



/* Module paramaters */
MODULE_PARM (debug, "i");
MODULE_PARM_DESC (debug, "Debug enabled or not");


/* table of devices that work with this driver */
static struct usb_device_id usbagent_table[] = {
    {USB_DEVICE (CYUSB_VENDOR_ID, CYUSB_PRODUCT_ID_HYPHY)},
    {USB_DEVICE (CYUSB_VENDOR_ID, CYUSB_PRODUCT_ID_WIZIRD)},
    {USB_DEVICE (0x4705, 0xedfe)},
    {}                          /* Terminating entry */
};

MODULE_DEVICE_TABLE (usb, usbagent_table);



/* Get a minor range for your devices from the usb maintainer */
#define CYUSB_MINOR_BASE	192
#define USBAGENT_MOD_NAME	"USB Agent"

/* we can have up to this number of device plugged in at once */
#define MAX_DEVICES		1

/* Structure to hold all of our device specific stuff */
struct cyusb
{
    struct usb_device *udev;    /* save off the usb device pointer */
    struct usb_interface *interface; /* the interface for this device */
    devfs_handle_t devfs;       /* devfs device node */
    unsigned char minor;        /* the starting minor number for this device */
    unsigned char num_ports;    /* the number of ports this device has */
    char num_interrupt_in;      /* number of interrupt in endpoints we have */
    char num_bulk_in;           /* number of bulk in endpoints we have */
    char num_bulk_out;          /* number of bulk out endpoints we have */

    unsigned char *bulk_in_buffer; /* the buffer to receive data */
    int bulk_in_size;           /* the size of the receive buffer */
    __u8 bulk_in_endpointAddr;  /* the address of the bulk in endpoint */

    unsigned char *bulk_out_buffer; /* the buffer to send data */
    int bulk_out_size;          /* the size of the send buffer */
    struct urb *write_urb;      /* the urb used to send data */
    __u8 bulk_out_endpointAddr; /* the address of the bulk out endpoint */

    struct tq_struct tqueue;    /* task queue for line discipline waking up */
    int open_count;             /* number of times this port has been opened */
    struct semaphore sem;       /* locks this structure */
};


typedef struct
{
    unsigned char length;
    unsigned short address;
    unsigned char type;
    unsigned char data[MAX_IHEX_RECORD_LEN];
} intel_hex_record_t;

typedef struct _keyword_item
{
    char *keyword;
    int (*fp) ();
    unsigned char argc;
    long argv[6];
} proccmd_t;

typedef struct fxload_msg_t
{
    int busnum;
    int devnum;
} fxload_msg;

/* the global usb devfs handle */
extern devfs_handle_t usb_devfs_handle;
static struct sock *fxload_sock;
static int exit_flag = 0;
static DECLARE_COMPLETION (exit_completion);


/******************************************************************************/
/**  utility functions 										                 **/
/******************************************************************************/
static int char_is_in (char c, char *string);
static int tokenize (char *line, char *delim, char *argv[]);
static char *next_token (char *string, char *delim, char **rest);
static int send_to_fxload_process (void *data);

/******************************************************************************/
/**  PMC FX2 function prototypes							                 **/
/******************************************************************************/
char *i2c_state_string (eI2C_STATE state);
static inline int usbagent_ctrl_msg (__u8 request, __u8 requesttype,
                                     __u16 value, __u16 index, void *data,
                                     __u16 size, int sndrcvFlg);
static inline int usbagnet_bulk_msg (unsigned int ep, void *data, int len,
                                     int *actual_length, int sndrcvFlg);

int pmcFX2_setMuxMode (void);
int pmcFX2_setNonMuxMode (void);
int pmcFX2_pollUntilWriteComplete (void);

int pmcFX2_rdBlockToFile16 (char *page, long address, unsigned long nWords);
int pmcFX2_rdBlockToFile (char *page, long address, unsigned long nBytes);

int
pmcFX2_getMpifStatus (char *buf, unsigned long expectedPacketCount,
                      unsigned long bytesWritten, unsigned long bytesRemaining);
int pmcFX2_getMpifStatusSimple (char *buf);

int pmcFX2_rd (long address, unsigned long nBytes, unsigned char *buf);
int pmcFX2_wr (long address, unsigned long nBytes, unsigned char *buf,
               int loopbackEnable);
int
pmcFX2_twiReadBurst (eTWI_TYPE mode, unsigned char devAddr,
                     unsigned short regAddr, unsigned char len,
                     unsigned char *buf);

int
pmcFX2_twiWriteBurst (eTWI_TYPE mode, unsigned char devAddr,
                      unsigned short regAddr, unsigned char len,
                      unsigned char *buf);
int pmcFX2_rdI2C (unsigned char addr, unsigned char len, unsigned char *buf);
int pmcFX2_wrI2C (unsigned char addr, unsigned char len, unsigned char *buf);


/* local function prototypes */
static ssize_t usbagent_read (struct file *file, char *buffer, size_t count,
                              loff_t * ppos);
static ssize_t usbagent_write (struct file *file, const char *buffer,
                               size_t count, loff_t * ppos);
static int usbagent_ioctl (struct inode *inode, struct file *file,
                           unsigned int cmd, unsigned long arg);
static int usbagent_open (struct inode *inode, struct file *file);
static int usbagent_release (struct inode *inode, struct file *file);

static void *usbagent_probe (struct usb_device *dev, unsigned int ifnum,
                             const struct usb_device_id *id);
static void usbagent_disconnect (struct usb_device *dev, void *ptr);

static void usbagent_write_bulk_callback (struct urb *urb);


static int usbagent_read_proc (char *page, char **start, off_t offset,
                               int count, int *eof, void *data);
static int usbagent_write_proc (struct file *file, const char *buffer,
                                unsigned long count, void *data);

static int gendrv_extract_hex (char *string, char *delim, char **rest,
                               unsigned long *hex);
static int gendrv_extract_dec (char *string, char *delim, char **rest,
                               unsigned long *dec);

/* array of pointers to our devices that are currently connected */
static struct cyusb *dev_table[MAX_DEVICES];

/* array of procfs commands */
static proccmd_t cmdTbl[] = {
    {"hyPhy20gRead", pmcFX2_rd, 2, {0, 0, 0, 0, 0, 0}},
    {"hyPhy20gWrite", pmcFX2_wr, 4, {0, 0, 0, 0, 0, 0}},
    {"twiReadBurst", pmcFX2_twiReadBurst, 4, {0, 0, 0, 0, 0, 0}},
    {"twiWriteBurst", pmcFX2_twiWriteBurst, 5, {0, 0, 0, 0, 0, 0}},
    {"i2cRead", pmcFX2_rdI2C, 2, {0, 0, 0, 0, 0, 0}},
    {"i2cWrite", pmcFX2_wrI2C, 3, {0, 0, 0, 0, 0, 0}},
    {"setMuxMode", pmcFX2_setMuxMode, 0, {0, 0, 0, 0, 0, 0}},
    {"setNonMuxMode", pmcFX2_setNonMuxMode, 0, {0, 0, 0, 0, 0, 0}},
    {"getMpifSS", pmcFX2_getMpifStatusSimple, 0, {0, 0, 0, 0, 0, 0}},
    {"getInfo", 0, 0, {0, 0, 0, 0, 0, 0}},
    {"END", 0, 0, {0, 0, 0, 0, 0, 0}}
};

static proccmd_t *active_cmd_p = NULL;

/* lock to protect the dev_table structure */
static DECLARE_MUTEX (dev_table_mutex);

/*
 * File operations needed when we register this driver.
 * This assumes that this driver NEEDS file operations,
 * of course, which means that the driver is expected
 * to have a node in the /dev directory. If the USB
 * device were for a network interface then the driver
 * would use "struct net_driver" instead, and a serial
 * device would use "struct tty_driver".
 */
static struct file_operations usbagent_fops = {
    /*
     * The owner field is part of the module-locking
     * mechanism. The idea is that the kernel knows
     * which module to increment the use-counter of
     * BEFORE it calls the device's open() function.
     * This also means that the kernel can decrement
     * the use-counter again before calling release()
     * or should the open() function fail.
     *
     * Not all device structures have an "owner" field
     * yet. "struct file_operations" and "struct net_device"
     * do, while "struct tty_driver" does not. If the struct
     * has an "owner" field, then initialize it to the value
     * THIS_MODULE and the kernel will handle all module
     * locking for you automatically. Otherwise, you must
     * increment the use-counter in the open() function
     * and decrement it again in the release() function
     * yourself.
     */
  owner:THIS_MODULE,

  read:usbagent_read,
  write:usbagent_write,
  ioctl:usbagent_ioctl,
  open:usbagent_open,
  release:usbagent_release,
};


/* usb specific object needed to register this driver with the usb subsystem */
static struct usb_driver usbagent_drv = {
  name:"HyPHY USB Agent Drv",
  probe:usbagent_probe,
  disconnect:usbagent_disconnect,
  fops:&usbagent_fops,
  minor:CYUSB_MINOR_BASE,
  id_table:usbagent_table,
};

/******************************************************************************/
/**  utility functions 										                 **/
/******************************************************************************/
#ifndef tolower
#define tolower(x) (((x) >= 'A' && (x) <= 'Z') ? ((x) - 'A' + 'a') : (x))
#endif

static int
char_is_in (char c, char *string)
{
    int i, len = strlen (string);

    for (i = 0; i < len && c != string[i]; i++);
    return (i < len);
}

static int
tokenize (char *line, char *delim, char *argv[])
{
    char *rline = line;
    int argc = 0;

    while ((argv[argc] = next_token (rline, delim, &rline)) != NULL)
        argc++;
    return argc;
}

static char *
next_token (char *string, char *delim, char **rest)
{
    char *token = string;
    char *tmp = string;

    *rest = NULL;
    if (string == NULL || *string == '\0')
        return NULL;

    /* ignore leading delimiters */
    while (char_is_in (*tmp, delim) && *tmp != '\0')
    {
        *tmp = '\0';
        tmp++;
        token++;
    }
    if (*tmp == '\0')           /* all delimiters */
    {
        *rest = NULL;
        return NULL;
    }

    /* find the first delimiter */
    while (!char_is_in (*tmp, delim) && *tmp != '\0')
        tmp++;
    if (*tmp == '\0')
    {
        /* no delimiters found */
        *rest = NULL;
        return token;
    }

    /* replace trailing delimiters with zeros */
    while (char_is_in (*tmp, delim) && *tmp != '\0')
    {
        *tmp = '\0';
        tmp++;
    }
    if (*tmp != '\0')
        *rest = tmp;
    else
        *rest = NULL;

    return token;
}


static int
gendrv_extract_hex (char *string, char *delim, char **rest, unsigned long *hex)
{
    char *num;

    *hex = 0;
    num = next_token (string, delim, rest);
    if (num == NULL)
    {
        return -1;
    }
    if (strlen (num) > 2 && num[0] == '0' && tolower (num[1]) == 'x')
        num += 2;
    if (strspn (num, "0123456789abcdefABCDEF") != strlen (num))
        return -1;
    *hex = simple_strtoul (num, NULL, 16);
    return 0;
}

static int
gendrv_extract_dec (char *string, char *delim, char **rest, unsigned long *dec)
{
    char *num;

    num = next_token (string, delim, rest);
    if (num == NULL)
    {
        *dec = 0;
        return -1;
    }
    if (strspn (num, "0123456789") != strlen (num))
        return -1;
    *dec = simple_strtoul (num, NULL, 10);
    return 0;
}


/******************************************************************************/

/**  PMC FX2 functions 										                 **/

/******************************************************************************/
char *
i2c_state_string (eI2C_STATE state)
{
    char *s;

    switch (state)
    {
    case I2C_IDLE:
        s = "I2C_IDLE";
        break;
    case I2C_SENDING:
        s = "I2C_SENDING";
        break;
    case I2C_RECEIVING:
        s = "I2C_RECEIVING";
        break;
    case I2C_PRIME:
        s = "I2C_PRIME";
        break;
    case I2C_STOP:
        s = "I2C_STOP";
        break;
    case I2C_BERROR:
        s = "I2C_BERROR";
        break;
    case I2C_NACK:
        s = "I2C_NACK";
        break;
    case I2C_OK:
        s = "I2C_OK";
        break;
    case I2C_WAITSTOP:
        s = "I2C_WAITSTOP";
        break;
    case I2C_INVAL:
        s = "I2C_INVAL";
        break;
    case I2C_BUSERROR:
        s = "I2C_BUSERROR";
        break;
    case I2C_BAD_TWI_TYPE:
        s = "I2C_BAD_TWI_TYPE";
        break;
    default:
        s = "unknown";
        break;
    }
    return s;
}


//! Instruct the firmware to use multiplexed address and data when accessing the ECBI/MPIF interface

/*! The firmware and CPLD on the USB 2.0 interface boards support ECBI/MPIF
 *  accesses in multiplexed and non-multiplexed modes.
 *
 *  In multiplexed mode (the default), the ECBI address and data lines are shared.
 *  In non-multiplexed mode, separate lines are used for the ECBI address and data.
 *  The firmware documentation includes timing diagrams for both modes.
 *
 *  This command instructs the firmware to use multiplexed address and data lines when
 *  reading or writing the ECBI/MPIF interface.
 *
 *  The non-multiplexed mode has not been tested with real hardware.  Consequently
 *  we decided not to expose setMuxMode() and setNonMuxMode() in the USBtwo class.
 *
 *  The firmware defaults to multiplexed mode.
 */
int
pmcFX2_setMuxMode (void)
{
    unsigned char buf = 0;
    int retval = 0;
    retval = usbagent_ctrl_msg (PMC_USB2_MUX_MODE, REQ_VENDOR, 0, 0, (void *) &buf, 0, SND_PIPE_FLAG);
    DBG_PRINT(":retval=%d %s,%d", retval, __FILE__,__LINE__);
    if (retval < 0)
    {
        DBG_PRINT(": failed to read from EP0. retval=%d %s,%d", retval, __FILE__,
             __LINE__);
    }

    return retval;
}

//! Instruct the firmware to use separate address and data lines when accesing the ECBI/MPIF interface

/*  See the documentation on setMuxMode().
 */
int
pmcFX2_setNonMuxMode (void)
{
    unsigned char buf = 0;
    int retval = 0;

    retval = usbagent_ctrl_msg (PMC_USB2_NONMUX_MODE, REQ_VENDOR, 0, 0, (void *) &buf,
                       0, SND_PIPE_FLAG);
    DBG_PRINT(":retval=%d %s,%d", retval, __FILE__,__LINE__);
    if (retval < 0)
    {
        DBG_PRINT(": failed to read from EP0. retval=%d %s,%d", retval, __FILE__,
             __LINE__);
    }

    return retval;
}

//! read nBytes from the MPIF interface of the USB device starting from address and write the result to a text file named fileName.

/*! The file format is simple:
 *    - the first line contains the ECBI address as "address 0xYYYYYY"
 *    - each subsequent line contains a hex value for one byte of returned data
 */

int
pmcFX2_rdBlockToFile (char *buf, long address, unsigned long nBytes)
{
    unsigned char *memblock;

    /* NOTE: len set to 0, and it means we will overwrite the previous buf */
    int len = 0, retval = -1;
    int i;

    if (buf == NULL)
    {
        DBG_PRINT("buf is NULL. %s,%d", __FILE__, __LINE__);
        return retval;
    }

    // if nBytes is odd, add an extra byte to make it even
    if (nBytes & 1)
    {
        ++nBytes;
    }

    // allocate space for the data
    memblock = kmalloc (nBytes * sizeof (unsigned char), GFP_KERNEL);
    if (memblock == NULL)
    {
        DBG_PRINT("memblock: kmalloc failed. %s,%d", __FILE__,
             __LINE__);
        return retval;
    }
    // read the data from the USB device
    pmcFX2_rd (address, nBytes, memblock);


    // write each byte as two hex digits on a separate line.

    len += sprintf (buf + len, "address 0x%8.8lx\n", address);
    for (i = 0; i < nBytes; ++i)
    {
        len += sprintf (buf + len, "%2.2x\n", memblock[i]);
    }
    len += sprintf (buf + len, "\n");

    // release allocated memory
    kfree (memblock);
    return (retval = 0);
}

//! read nWords of 16 bits each from the MPIF interface of the USB device starting from address and write the result to a text file named fileName.

/*! The file format is simple:
 *    - the first line contains the ECBI address as "address 0xYYYYYY"
 *    - each subsequent line contains a 16-bit hex value for two bytes of returned data
 */

int
pmcFX2_rdBlockToFile16 (char *buf, long address, unsigned long nWords)
{
    unsigned char *memblock;

    // write each byte as two hex digits on a separate line.
    int i;

    /* NOTE: len set to 0, and it means we will overwrite the previous buf */
    int len = 0;
    int retval = -1;

    if (buf == NULL)
    {
        DBG_PRINT("buf is NULL. %s,%d", __FILE__, __LINE__);
        return retval;
    }


    // double the number of 16-bit words to get number of bytes
    nWords <<= 1;

    // allocate space for the data
    memblock = kmalloc (nWords * sizeof (unsigned char), GFP_KERNEL);
    if (memblock == NULL)
    {
        DBG_PRINT("memblock: kmalloc failed. %s,%d", __FILE__,
             __LINE__);
        return retval;
    }

    // read the data from the USB device
    pmcFX2_rd (address, nWords, memblock);


    len += sprintf (buf + len, "address 0x%8.8lx\n", address);
    for (i = 0; i < nWords; i += 2)
    {
        len +=
            sprintf (buf + len, "%4.4x\n",
                     ((memblock[i + 1] << 8) | memblock[i]));
    }
    len += sprintf (buf + len, "\n");

    // release allocated memory
    kfree (memblock);
    return (retval = 0);
}


int
pmcFX2_pollUntilWriteComplete (void)
{

    int i;
    long len = 64;

    // We get the status in the data segment of the message
    unsigned char packet[64];

    // read the status until writeLen is zero, indicating that the write operation has completed
    unsigned long packetCount, readLen, readAddr, writeLen = 0, writeAddr = 0;

    for (i = 0; i < 10; ++i)
    {
        unsigned long *p;
        unsigned long index = 0;
        int retval;

        retval = usbagent_ctrl_msg (PMC_USB2_MPIF_STATUS, REQ_VENDOR, 0, 0,
                                        (void *) packet, len, RCV_PIPE_FLAG);
        if (retval != len)
        {
            DBG_PRINT(": failed to read from EP0. retval=%d %s,%d", retval, __FILE__,
                 __LINE__);
            continue;
        }


        // Now decode the results and put them into a char *
        p = (unsigned long *) (packet + index);
        packetCount = *p;
        index += 4;

        p = (unsigned long *) (packet + index);
        readLen = *p;
        index += 4;

        p = (unsigned long *) (packet + index);
        readAddr = *p;
        index += 4;

        p = (unsigned long *) (packet + index);
        writeLen = *p;
        index += 4;

        p = (unsigned long *) (packet + index);
        writeAddr = *p;
        index += 4;

        if (writeLen == 0)
        {
            return 0;
        }
    }

    // if we got here, we failed.
    DBG_PRINT(
         "ERROR: timeout waiting for write to complete, writeLen =%ld  writeAddr = 0x%lx %s,%d",
         writeLen, writeAddr, __FILE__, __LINE__);

    return -1;
}


int
pmcFX2_getMpifStatus (char *buf, unsigned long expectedPacketCount,
                      unsigned long bytesWritten, unsigned long bytesRemaining)
{

    int i, cnt = 0;
    int retval = -1;
    long len = 64;

    // We get the status in the data segment of the message
    unsigned char packet[64];

    unsigned long packetCount, readLen, readAddr, writeLen, writeAddr;
    unsigned long *p;
    unsigned long index;

    if (buf == NULL)
    {
        DBG_PRINT("buf is NULL. %s,%d", __FILE__, __LINE__);
        return retval;
    }

    // define the contents of the control message
    retval =
        usbagent_ctrl_msg (PMC_USB2_MPIF_STATUS, REQ_VENDOR, 0, 0,
                           (void *) packet, len, RCV_PIPE_FLAG);

    if (retval != len)
    {
        DBG_PRINT(": failed to read from EP0. retval=%d %s,%d", retval, __FILE__,
             __LINE__);
        return retval;
    }

    // Now decode the results and put them into a char *

    index = 0;

    p = (unsigned long *) (packet + index);
    packetCount = *p;
    index += 4;

    p = (unsigned long *) (packet + index);
    readLen = *p;
    index += 4;

    p = (unsigned long *) (packet + index);
    readAddr = *p;
    index += 4;

    p = (unsigned long *) (packet + index);
    writeLen = *p;
    index += 4;

    p = (unsigned long *) (packet + index);
    writeAddr = *p;
    index += 4;

    cnt +=
        sprintf (buf + cnt, "(%s, %d)\nlen=\t\t%ld\n\n", __FILE__, __LINE__,
                 len);

    cnt +=
        sprintf (buf + cnt,
                 "(expectedPacketCount=%ld,packetCount=%ld\nreadLen=%ld\nreadAddr=0x%lx\n",
                 expectedPacketCount, packetCount, readLen, readAddr);
    cnt +=
        sprintf (buf + cnt,
                 "(bytesRemaining=%ld,writeLen=%ld\nwriteAddr0x%lx\n",
                 bytesRemaining, writeLen, writeAddr);

    for (i = 0; i < 10; ++i)
    {
        cnt += sprintf (buf + cnt, "(stall[%d]=%d\n", i, (int) (packet[index]));
    }

    if (packetCount != expectedPacketCount)
    {

    }

    return retval;
}

int
pmcFX2_getMpifStatusSimple (char *buf)
{

    int i, cnt = 0;
    int retval = -1;
    long len = 64;

    // We get the status in the data segment of the message
    unsigned char packet[64];

    unsigned long packetCount = 0, readLen = 0, readAddr = 0, writeLen = 0, writeAddr = 0;
    unsigned long *p;
    unsigned long index;

    if (buf == NULL)
    {
        DBG_PRINT("buf is NULL. %s,%d", __FILE__, __LINE__);
        return retval;
    }
    memset (packet, 0x00, 64* sizeof (char));
    // define the contents of the control message
    retval =
        usbagent_ctrl_msg (PMC_USB2_MPIF_STATUS, REQ_VENDOR, 0, 0,
                           (void *) packet, len, RCV_PIPE_FLAG);

    DBG_PRINT(":retval=%d %s,%d", retval, __FILE__,__LINE__);
    if (retval != len)
    {
        DBG_PRINT(": failed to read from EP0. retval=%d. %s,%d", retval, __FILE__,
             __LINE__);
    }

    // Now decode the results and put them into a char *

    index = 0;

    p = (unsigned long *) (packet + index);
    packetCount = *p;
    index += 4;

    p = (unsigned long *) (packet + index);
    readLen = *p;
    index += 4;

    p = (unsigned long *) (packet + index);
    readAddr = *p;
    index += 4;

    p = (unsigned long *) (packet + index);
    writeLen = *p;
    index += 4;

    p = (unsigned long *) (packet + index);
    writeAddr = *p;
    index += 4;

    cnt +=
        sprintf (buf + cnt, "(%s, %d)\nlen=\t\t%ld\n\n", __FILE__, __LINE__,
                 len);

    cnt +=
        sprintf (buf + cnt,
                 "packetCount=%ld\nreadLen=%ld\nreadAddr=0x%lx\n",
                 packetCount, readLen, readAddr);
    cnt +=
        sprintf (buf + cnt, "writeLen=%ld\nwriteAddr0x%lx\n", writeLen,
                 writeAddr);

    for (i = 0; i < 10; ++i)
    {
        cnt += sprintf (buf + cnt, "stall[%d]=%d\n", i, (int) (packet[i]));
    }
    for (i=0; i<10; i++)
    cnt +=
        sprintf (buf + cnt, "packet[%d]=0x%x ", i,packet[i]);

    return retval;
}


//! read up to 63 bytes of data from the I2C interface

/*! Note that individual I2C devices may impose their own limits on
 *  burst size.  In the extreme case, a device may require a register
 *  address for each byte transferred, limiting the burst size may be limited to a single byte.
 */

int
pmcFX2_twiReadBurst (eTWI_TYPE mode, unsigned char devAddr,
                     unsigned short regAddr, unsigned char len,
                     unsigned char *buf)
{
    unsigned char packet[64];
    int retval = 0;

    // send the packet to request the read
    unsigned char pktLen = 6;
    int count;
    int status = 0;             // urb status:

    if (dev_table[0] == NULL)
    {
        DBG_PRINT(" No USB Device attached %s %d", __FILE__, __LINE__);
        return -1;
    }

    // check that the data will fit in a packet
    if ((len > 63) || (len == 0))
    {
        DBG_PRINT("i2c max transfer size is 61 bytes, not %d. %s,%d",
             len, __FILE__, __LINE__);
        return retval = -1;
    }


    // The first byte of the packet indicates the command class
    packet[0] = PMC_USB2_PMC_TWI_BURST_RD;

    // The second byte of the packet indicates whether we're using 8 or 16 bit register addresses
    packet[1] = mode;

    // the third byte of the packet specifies the I2C address
    packet[2] = devAddr;

    // the fourth byte of the packet is the register address MSB
    packet[3] = (unsigned char) (regAddr >> 8);

    // the fifth byte of the packet is the register address LSB
    packet[4] = (unsigned char) regAddr;

    // the sixth byte of the packet indicates the number of bytes to read.
    // The return packet can hold max 63 bytes of data (plus a one byte status code).
    packet[5] = len;
    status =
        usbagnet_bulk_msg (I2C_WR_ENDPOINT, (void *) packet, pktLen, &count,
                           SND_PIPE_FLAG);
    DBG_PRINT(
         "ubr return status is %d %s,%d", status, __FILE__, __LINE__);

    if (status != 0)
    {
        DBG_PRINT(
             "ERROR: failed to send I2C read request. urb status is %d %s,%d",
             status, __FILE__, __LINE__);
        return retval = -1;
    }

    // now read the returned data
    pktLen = len + 1;
    status =
        usbagnet_bulk_msg (I2C_RD_ENDPOINT, (void *) packet, pktLen, &count,
                           RCV_PIPE_FLAG);

    if (status != 0)
    {
        DBG_PRINT(
             "ERROR: failed to send I2C read request. urb status is %d %s,%d",
             status, __FILE__, __LINE__);
        return retval = -1;
    }
#ifdef DEBUG_USELESS
    {
        int i;

        cout << "Received twi read response packet of " << dec << pktLen <<
            " bytes\n";
        for (i = 0; i < pktLen; ++i)
        {
            cout << "0x" << hex << (unsigned int) (packet[i]) << "\n";
        }
    }
#endif

    // confirm that we got the expected number of bytes.  First byte is status.
    if (pktLen != len + 1)
    {
        DBG_PRINT(
             "ERROR: I2C read request expected %d bytes but got %d. %s,%d", len,
             pktLen, __FILE__, __LINE__);
        DBG_PRINT("Status code is : %s. %s,%d",
             i2c_state_string ((eI2C_STATE) packet[0]), __FILE__, __LINE__);
        return retval = -1;
    }

    if (packet[0] != I2C_OK)
    {
        DBG_PRINT("ERROR: I2C read returned bad status : %s. %s,%d",
             i2c_state_string ((eI2C_STATE) packet[0]), __FILE__, __LINE__);
        return retval = -1;
    }

    // copy the data into the return buffer
    memcpy (buf, packet + 1, len);

    return retval;
}

//! write up to 58 bytes of data to the I2C interface.

/*! Note that individual I2C devices may impose their own limits on
 *  burst size.  For example, the MICROCHIP 24LC64 SEEPROM has a max write page size of 31 bytes.
 *  Other devices may require a register address for each byte transferred, i.e. the
 *  burst size may be limited to a single byte.
 */
int
pmcFX2_twiWriteBurst (eTWI_TYPE mode, unsigned char devAddr,
                      unsigned short regAddr, unsigned char len,
                      unsigned char *buf)
{
    unsigned char packet[64];
    unsigned char pktLen;
    int status;                 // urb status
    int count;
    int retval = 0;

    if (dev_table[0] == NULL)
    {
        DBG_PRINT(" No USB Device attached %s %d", __FILE__, __LINE__);
        return -1;
    }

    // check that the data will fit in a packet.  We have six bytes of overhead,
    // so the payload is max 58 bytes.
    if ((len > 58) || (len == 0))
    {
        DBG_PRINT(
             "max transfer size is 58 bytes, not %d. %s,%d", len,
             __FILE__, __LINE__);
        return retval = -1;
    }

    // The first byte of the packet indicates the command class
    packet[0] = PMC_USB2_PMC_TWI_BURST_WR;

    // The second byte of the packet indicates whether we're using 8 or 16 bit register addresses
    //
    packet[1] = mode;

    // the third byte of the packet specifies the I2C address
    packet[2] = devAddr;

    // the fourth byte of the packet is the register address MSB
    packet[3] = (unsigned char) (regAddr >> 8);

    // the fifth byte of the packet is the register address LSB
    packet[4] = (unsigned char) regAddr;

    // the sixth byte of the packet indicates the number of bytes to read.
    // The return packet can hold max 63 bytes of data (plus a one byte status code).
    packet[5] = len;

    // copy the data into the rest of the packet
    memcpy (packet + 6, buf, len);

    DBG_PRINT(
         "cmd %x mode %x devAddr %x msb(reg) %x lsb(reg) %x len %x dat[0] %x %s,%d",
         packet[0], packet[1], packet[2], packet[3], packet[4], packet[5],
         packet[6], __FILE__, __LINE__);

    // send the packet to request the write
    pktLen = len + 6;

    status =
        usbagnet_bulk_msg (I2C_WR_ENDPOINT, (void *) packet, pktLen,
                           &count, SND_PIPE_FLAG);
    if (status != 0)
    {
        DBG_PRINT(
             "ERROR: failed to send I2C write request. urb status is %d %s,%d",
             status, __FILE__, __LINE__);
        return retval = -1;
    }

    // now read the status from EP1IN
    pktLen = len + 2;
    status =
        usbagnet_bulk_msg (I2C_RD_ENDPOINT, (void *) packet, pktLen,
                           &count, RCV_PIPE_FLAG);
    if (status != 0)
    {
        DBG_PRINT(
             "ERROR: failed to send I2C write request. urb status is %d %s,%d",
             status, __FILE__, __LINE__);
        return retval = -1;
    }

    if (packet[0] != I2C_OK)
    {
        DBG_PRINT("ERROR: I2C write returned bad status : %s. %s,%d",
             i2c_state_string ((eI2C_STATE) packet[0]), __FILE__, __LINE__);
        return retval = -1;
    }

    return retval;
}


//! read up to 61 bytes from an I2C/TWI device

/*! \param addr a 7 bit I2C address
 *  \param len  the number of bytes to transfer (must be < 61)
 *  \param buf  buffer to contain the returned data (size must be >= len)
 */
int
pmcFX2_rdI2C (unsigned char addr, unsigned char len, unsigned char *buf)
{
    unsigned char packet[64];
    long pktLen = 3;
    int actual_length = 0;
    int status;

    if (dev_table[0] == NULL)
    {
        DBG_PRINT(" No USB Device attached %s %d", __FILE__, __LINE__);
        return -1;
    }

    // check that the data will fit in a packet
    if ((len > 61) || (len == 0))
    {
        DBG_PRINT("i2c max transfer size is 61 bytes, not %d. %s,%d",
             len, __FILE__, __LINE__);
        return -1;
    }


    // The first byte of the packet indicates the command class
    packet[0] = PMC_USB2_I2C_CMD_XFER;

    // The second byte of the packet holds the address info in
    // bits [7:1]. We use bit 0 of this byte to indicate
    // whether we're performing a read or a write.
    //
    packet[1] = (addr << 1) | 0x1; // indicate a read operation

    // the third byte of the packet indicates the number of data bytes
    packet[2] = len;

    // send the packet to request the read
    status =
        usbagnet_bulk_msg (I2C_WR_ENDPOINT, (void *) packet, pktLen,
                           &actual_length, SND_PIPE_FLAG);
    if (status != 0)
    {
        DBG_PRINT(
             "ERROR: failed to send I2C read request. urb status is %d %s,%d",
             status, __FILE__, __LINE__);
        return -1;
    }


    // now read the returned data
    pktLen = len + 1;

    status =
        usbagnet_bulk_msg (I2C_RD_ENDPOINT, (void *) buf, pktLen,
                           &actual_length, RCV_PIPE_FLAG);
    if (status != 0)
    {
        DBG_PRINT(
             "ERROR: failed to send I2C read request. urb status is %d %s,%d",
             status, __FILE__, __LINE__);
        return -1;
    }


    // confirm that we got the expected number of bytes.  First byte is status.
    if (pktLen != actual_length)
    {
        DBG_PRINT(
             "ERROR: I2C read request expected %d bytes but got %d. %s,%d", len,
             (int)pktLen, __FILE__, __LINE__);
        return -1;
    }

    if (packet[0] != I2C_OK)
    {
        DBG_PRINT("ERROR: I2C read returned bad status : %s. %s,%d",
             i2c_state_string ((eI2C_STATE) packet[0]), __FILE__, __LINE__);
        return -1;
    }

    return 0;
}


//! write up to 61 bytes to an I2C/TWI device

/*! \param addr a 7 bit I2C address
 *  \param len  the number of bytes to transfer (must be < 61)
 *  \param buf  buffer to contain the returned data (size must be >= len)
 */
int
pmcFX2_wrI2C (unsigned char addr, unsigned char len, unsigned char *buf)
{
    unsigned char packet[64];
    long pktLen;
    int status, actual_length = 0;

    if (dev_table[0] == NULL)
    {
        DBG_PRINT(" No USB Device attached %s %d", __FILE__, __LINE__);
        return -1;
    }

    // check that the data will fit in a packet
    if ((len > 61) || (len == 0))
    {

        DBG_PRINT(
             "i2c max transfer size is 58 bytes, not %d. %s,%d", len,
             __FILE__, __LINE__);
        return -1;
    }


    // The first byte of the packet indicates the command class
    packet[0] = PMC_USB2_I2C_CMD_XFER;

    // The second byte of the packet holds the address info in
    // bits [7:1]. We use bit 0 of this byte to indicate
    // whether we're performing a read or a write.
    //
    packet[1] = (addr << 1) & 0xfe; // indicate a write operation

    // the third byte of the packet indicates the number of data bytes
    packet[2] = len;

    // copy the data into the packet buffer
    memcpy (packet + 3, buf, len); // we checked earlier that len < 61

    // send the write request
    pktLen = len + 3;

    status =
        usbagnet_bulk_msg (I2C_WR_ENDPOINT, (void *) packet, pktLen,
                           &actual_length, SND_PIPE_FLAG);
    if (status != 0)
    {
        DBG_PRINT(
             "ERROR: failed to send I2C write request. urb status is %d %s,%d",
             status, __FILE__, __LINE__);
        return -1;
    }

    // grab a packet from EP1IN to see whether the write succeeded
    pktLen = len;
    status =
        usbagnet_bulk_msg (I2C_RD_ENDPOINT, (void *) packet, pktLen,
                           &actual_length, RCV_PIPE_FLAG);
    if (status != 0)
    {
        DBG_PRINT(
             "ERROR: failed to send I2C write request. urb status is %d %s,%d",
             status, __FILE__, __LINE__);
        return -1;
    }


    if (packet[0] != I2C_OK)
    {
        DBG_PRINT("ERROR: I2C write returned bad status : %s. %s,%d",
             i2c_state_string ((eI2C_STATE) packet[0]), __FILE__, __LINE__);
        return -1;
    }

    return 0;
}



//! Read nBytes starting at the specified ECBI/MPIF address

/*! \param address The ECBI/MPIF address of the first word to read.  Note that
 *                 the firmware supports a maximum 24 bit address space, so address
 *                 should not be larger than 0xFFFFFF (16MB). This is not enforced.
 *  \param nBytes  The number of bytes to read.  _NOTE_: nBytes _MUST_ be an even number,
 *                 since each ECBI/MPIF address specifies a 16 bit word.  An exception is
 *                 thrown if nBytes is odd.  nBytes must not be so large as to
 *                 overflow the 24 bit address space.  This is not enforced, but the
 *                 result of reading past the end of the address space is undefined.
 *  \param buf     Pointer to the buffer into which the data is read.  Must be at least nBytes in size.
 *                 Bytes are read from the ELB as 16 bit words and are put into buf
 *
 *
 *
 * The firmware is always processing its event loop.  It has nothing to do until
 * it receives a control packet.  When it gets a PMC_USB2_RD_16_MPIF control packet,
 * the firmware starts reading data into the endpoint 6 buffers.   EP6 has four
 * 512-byte buffers.  When these are full, the firmware blocks waiting for
 * the host to read packets from EP6.  As soon as a buffer comes available (because
 * the host has read a packet), the firmware resumes reading data.  This continues
 * until the requested number of bytes has been read.
 *
 * From the host's perspective, we send the PMC_USB2_RD_16_MPIF control packet,
 * then read bulk packets from EP6 until we have all the data we requested.
 *
 * We must not send any other control packets until we have read back all of the
 * expected data. Doing so could cause the firmware to not read all of the requested data.
 * This means that it is not safe to have multiple threads or multiple programs
 * accessing the same USB device.
 *
 */
int
pmcFX2_rd (long address, unsigned long nBytes, unsigned char *buf)
{
    int retval = 0;

    // define the contents of the control message
//    int reqType;                // vendor request
//    int reqCode;                // specific command

    // the number of bytes to transfer is sent in the data segment of the
    // control message.
    long opCount;
//    int timeout = 1000;         // milliseconds
    unsigned char dataSeg[2 * sizeof (unsigned long)];
    long len = 2 * sizeof (unsigned long);
    long pktLen;
    long iteration = 0;
    unsigned char pktBuf[BULK_MAX_PKT_SZ];

    // read a debug packet off EP1IN
    unsigned char debug[64];
    int debugLen, rcvPktLen;


    if (dev_table[0] == NULL)
    {
        DBG_PRINT(" No USB Device attached %s %d", __FILE__, __LINE__);
        return -1;
    }

    DBG_PRINT(" - address=0x%lx, nBytes=%ld, %s,%d",
         address, nBytes, __FILE__, __LINE__);

    // nBytes must be even, since each read accesses a 16 bit word
    if (nBytes & 0x1)
    {
        DBG_PRINT(
             "ERROR: ): nBytes must be even, but nBytes == %ld %s,%d",
             nBytes, __FILE__, __LINE__);
        return -1;
    }
    opCount = nBytes / 2;       // each read accesses a 16 bit word.

    // put the address into the data segment
    dataSeg[0] = (unsigned char) address & 0x000000ff;
    dataSeg[1] = (unsigned char) ((address & 0x0000ff00) >> 8);
    dataSeg[2] = (unsigned char) ((address & 0x00ff0000) >> 16);
    dataSeg[3] = (unsigned char) ((address & 0xff000000) >> 24); // the GPIF only supports a 24 bit address, but we don't enforce that here
    // put the number of operations into the data segment
    dataSeg[4] = (unsigned char) opCount & 0x000000ff;
    dataSeg[5] = (unsigned char) ((opCount & 0x0000ff00) >> 8);
    dataSeg[6] = (unsigned char) ((opCount & 0x00ff0000) >> 16);
    dataSeg[7] = (unsigned char) ((opCount & 0xff000000) >> 24); // the GPIF only supports a 24 bit address, but we don't enforce that here


    retval =
        usbagent_ctrl_msg (PMC_USB2_RD_16_MPIF, REQ_VENDOR, 0, 0,
                           (void *) dataSeg, len, SND_PIPE_FLAG);
    DBG_PRINT(":retval=%d %s,%d", retval, __FILE__,__LINE__);

    if (retval != len)
    {
        DBG_PRINT(
             "ERROR:failed to send the control packet.\n\tnBytes=%ld opCount=%d address=0x%lx %s,%d",
             nBytes, opCount, address, __FILE__, __LINE__);
        return -1;
    }


    // Get the data from the device.
    // The firmware starts reading as soon as it receives the control packet,
    // so the first data packet should be ready by the time we ask for the data.
    //
    // We can't receive more than BULK_MAX_PKT_SZ bytes in a bulk packet.
    //
    while (nBytes > 0)
    {
        int maxAddr;
        int maxBytes;

        // determine how many bytes to ask for.  We have three constraints:
        // - No single read can cross a 512 address boundary.
        // - No single read can exceed BULK_MAX_PKT_SZ.
        // - The last read should not request more bytes than necessary to fulfill
        //   the request
        //
        pktLen = (BULK_MAX_PKT_SZ > nBytes) ? nBytes : BULK_MAX_PKT_SZ;
        maxAddr = (address & 0xfffffe00) + 512;
        maxBytes = (maxAddr - address) * 2; // bytes to 512 address boundary (2 bytes per address)
        pktLen = (pktLen > maxBytes) ? maxBytes : pktLen;

        if ( 0 != usbagnet_bulk_msg (MPIF_RD_ENDPOINT, (void *) pktBuf, pktLen, &pktLen,RCV_PIPE_FLAG))
        {

            DBG_PRINT(" ERROR: failed to read expected data. nBytes=%ld,opCount=%d,address=0x%lx,iteration=%d. %s %d",
                nBytes, opCount, address, iteration, __FILE__, __LINE__);

        }

        // read a debug packet off EP1IN
        debugLen = 64;

        if ( 0 != usbagnet_bulk_msg (I2C_RD_ENDPOINT, (void *) debug, debugLen, &rcvPktLen, RCV_PIPE_FLAG))
        {
            DBG_PRINT(" ERROR: failed to read expected data. nBytes=%ld,opCount=%d,address=0x%lx,iteration=%d. %s %d",
                nBytes, opCount, address, iteration, __FILE__, __LINE__);
        }

        // put the debug info into debugStr
        {

            long tmp;
            int i = 0, len = 0;
            char debugStr[512];

            tmp = ((long) ((buf[i + 1])) << 8) + buf[i];
            i += 2;
            len += sprintf(debugStr+len, "maxOps=%ld, ", tmp);
            tmp = ((long) ((buf[i + 1])) << 8) + buf[i];
            i += 2;
            len += sprintf(debugStr+len, "xferSize=%ld, ", tmp);

            tmp =
                ((long) ((buf[i + 3])) << 24) + ((long) ((buf[i + 2])) << 16) +
                ((long) ((buf[i + 1])) << 8) + buf[i];
            i += 4;
            len += sprintf(debugStr+len, "startAddr=0x%lx, ", tmp);
            tmp =
                ((long) ((buf[i + 3])) << 24) + ((long) ((buf[i + 2])) << 16) +
                ((long) ((buf[i + 1])) << 8) + buf[i];
            i += 4;
            len += sprintf(debugStr+len, "pageAddr=0x%lx, ", tmp);

            tmp =
                ((long) ((buf[i + 3])) << 24) + ((long) ((buf[i + 2])) << 16) +
                ((long) ((buf[i + 1])) << 8) + buf[i];
            i += 4;
            len += sprintf(debugStr+len, "maxAddr=0x%lx, ", tmp);

            tmp =
                ((long) ((buf[i + 3])) << 24) + ((long) ((buf[i + 2])) << 16) +
                ((long) ((buf[i + 1])) << 8) + buf[i];
            i += 4;
            len += sprintf(debugStr+len, "readLen=%ld, ", tmp);

            tmp =
                ((long) ((buf[i + 3])) << 24) + ((long) ((buf[i + 2])) << 16) +
                ((long) ((buf[i + 1])) << 8) + buf[i];
            i += 4;
            len += sprintf(debugStr+len, "junk=0x%lx, ", tmp);

            tmp =
                ((long) ((buf[i + 3])) << 24) + ((long) ((buf[i + 2])) << 16) +
                ((long) ((buf[i + 1])) << 8) + buf[i];
            i += 4;
            len += sprintf(debugStr+len, "newLen=%ld, ", tmp);

            tmp =
                ((long) ((buf[i + 3])) << 24) + ((long) ((buf[i + 2])) << 16) +
                ((long) ((buf[i + 1])) << 8) + buf[i];
            i += 4;
            len += sprintf(debugStr+len, "newAddr=0x%lx", tmp);

            DBG_PRINT(" debug info: %s. %s %d", debugStr, __FILE__, __LINE__);

        }

        // pktLen should always be an even number on return, since we are
        // reading 16 bit words.
        if (pktLen & 0x1)
        {
            DBG_PRINT(" ERROR: received an odd number of bytes. %s %d", __FILE__, __LINE__);
            return -1;
        }

        // pktLen had better not be zero.  If it is, we have failed our read.
        if (pktLen == 0)
        {
            DBG_PRINT(" ERROR: pktLen had better not be zero. %s %d", __FILE__, __LINE__);
            return -1;
        }

        // We assume that buf is only nBytes long.  If we got more bytes
        // than were requested, then we have a problem since we just overwrote
        // some space presumably occupied by other variables.  Darn.
        //
        // It would be safer to receive the data into a packet buffer, then transfer
        // it to the request buffer
        //
        if (pktLen > nBytes)
        {
            DBG_PRINT(" ERROR: received more data than requested, pktLen=%d. %s %d",
                pktLen, __FILE__, __LINE__);
        }

        // copy the data to buf
        memcpy (buf, pktBuf, pktLen);

        // debug: dump buf
        {
            int i = 0, len = 0;
            char debugStr[512];
            for (i=0; i< pktLen; i++)
                len += sprintf(debugStr+len, "buf[%d]=0x%x ",i, buf[i]);

            DBG_PRINT(" debug info: i=%d,pktLen=%d, %s. %s %d", i, pktLen, debugStr, __FILE__, __LINE__);

        }

        // update the number of bytes remaining and move the buf pointer to the next packet location

        buf += pktLen;
        nBytes -= pktLen;
        ++iteration;
//        rcvPktLen.push_back (pktLen);
//        bytesRemaining.push_back (nBytes);
    }

    // If something went wrong.  Return false.
    if (nBytes != 0)
    {
        DBG_PRINT(
             "ERROR:%d fewer bytes read than expected. %s,%d",
             nBytes, __FILE__, __LINE__);
        return -1;
    }

    return 0;
}



//! Write nBytes from buf to the specified ECBI/MPIF address

/*! \param address The ECBI/MPIF address of the first word to write.  Note that
 *                 the firmware supports a maximum 24 bit address space, so address
 *                 should not be larger than 0xFFFFFF (16MB). This is not enforced.
 *  \param nBytes  The number of bytes to write.  _NOTE_: nBytes _MUST_ be an even number,
 *                 since each ECBI/MPIF address specifies a 16 bit word.  An exception is
 *                 thrown if nBytes is odd.  nBytes must not be so large as to
 *                 overflow the 24 bit address space.  This is not enforced, but the
 *                 result of reading past the end of the address space is undefined.
 *  \param buf     Pointer to the buffer containing the data to be written.  Must be at least nBytes in size.
 *
 * The firmware is always processing its event loop.  It has nothing to do until
 * it receives a control packet.  When it gets a PMC_USB2_WR_16_MPIF control packet,
 * the firmware resets some internal state variables that tell it to copy to the ECBI/MPIF
 * interface the first nBytes of data it receives on endpoint 2.   The data from
 * each packet on EP2 is written to the ECBI interface as soon as it arrives.
 *
 * From the host's perspective, we send the PMC_USB2_WR_16_MPIF control packet,
 * then write bulk packets to EP2 until we have sent all the data.  Each bulk
 * packet has a max of 512 bytes.  The firmware imposes the additional limitation
 * that no single packet may contain writes that cross a 512 address (9 bit) boundary,
 * so the data must be broken up appropriately.
 *
 * We must not send any other control packets until we have finished sending all of the
 * data to write. Doing so would cause the firmware to fail to write some of the data.
 *
 * We must also avoid writing packets to EP2 when the firmware is not expecting data.
 * The firmware should refuse to accept (NAK) such packets, but if it doesn't, you may
 * find that your next write command uses this unsolicited data.  There are some
 * safeguards in the firmware that attempt to prevent this, but best not to make the attempt.
 *
 * For this reason, it is not safe to allow multiple threads or processes to
 * simultaneously access a single USB device.
 */
int
pmcFX2_wr (long address, unsigned long nBytes, unsigned char *buf,
           int loopbackEnable)
{
    // the number of bytes to transfer is sent in the data segment of the
    // control message.
    long opCount = nBytes / 2;  // each read accesses a 16 bit word.
    unsigned char dataSeg[3 * sizeof (unsigned long)];
    long len = 3 * sizeof (unsigned long);

    // define the contents of the control message
//    int reqType = REQ_VENDOR;   // vendor request
//    int reqCode = PMC_USB2_WR_16_MPIF; // specific command

    int pktLen, requestLen, retval;
    long packetCount = 0;

//    unsigned char *p = buf;     // record our start position
    long bytesWritten = 0;
    unsigned char debugPacket[512];
    long debugPacketLen;
//    long timeout = 50000;       // myWrEndPoint->TimeOut = 50000;  // milliseconds

    if (dev_table[0] == NULL)
    {
        DBG_PRINT(" No USB Device attached %s %d", __FILE__, __LINE__);
        return -1;
    }

    // nBytes must be even, since each read accesses a 16 bit word
    if (nBytes & 0x1)
    {
        DBG_PRINT(
             "ERROR: nBytes must be even, but nBytes == %ld %s,%d",
             nBytes, __FILE__, __LINE__);
        return -1;
    }

    DBG_PRINT(" - address=0x%lx, nBytes=%ld, loopbackEnable=%d %s,%d",
         address, nBytes, loopbackEnable, __FILE__, __LINE__);

    // put the address into the data segment
    dataSeg[0] = (unsigned char) address & 0x000000ff;
    dataSeg[1] = (unsigned char) ((address & 0x0000ff00) >> 8);
    dataSeg[2] = (unsigned char) ((address & 0x00ff0000) >> 16);
    dataSeg[3] = (unsigned char) ((address & 0xff000000) >> 24); // the GPIF only supports a 24 bit address, but we don't enforce that here

    // put the number of operations into the data segment
    dataSeg[4] = (unsigned char) opCount & 0x000000ff;
    dataSeg[5] = (unsigned char) ((opCount & 0x0000ff00) >> 8);
    dataSeg[6] = (unsigned char) ((opCount & 0x00ff0000) >> 16);
    dataSeg[7] = (unsigned char) ((opCount & 0xff000000) >> 24); // the GPIF only supports a 24 bit address, but we don't enforce that here

    // indicate whether we want the firmware to loop back all data we send
    if (loopbackEnable)
    {
        dataSeg[8] = 1;
    }
    else
    {
        dataSeg[8] = 0;
    }
    dataSeg[9] = 0;             // unused
    dataSeg[10] = 0;            // unused
    dataSeg[11] = 0;            // unused

    // send the control packet to set up the write operation
    retval = usbagent_ctrl_msg (PMC_USB2_WR_16_MPIF, REQ_VENDOR, 0, 0,
        (void *) dataSeg, len, SND_PIPE_FLAG);
    DBG_PRINT(":retval=%d %s,%d", retval, __FILE__,__LINE__);
    if (len != retval)
    {
        DBG_PRINT(
             "ERROR:failed to send the control packet.\n\tnBytes=%ld opCount=%ld address=0x%lx %s,%d",
             nBytes, opCount, address, __FILE__, __LINE__);
        return -1;
    }

    // Send the data to the device..
    //
    // We can't send more than BULK_MAX_PKT_SZ bytes in a bulk packet.
    // Also, the address range for a single packet cannot cross a 512 address
    // boundary (the GPIF controls 9 address lines).
    //
    while (nBytes > 0)
    {
        int maxAddr;
        int maxBytes;

        // determine how many bytes to send.  We have three constraints:
        // - Addresses in the packet cannot cross a 512 address boundary.
        // - No single packet can exceed BULK_MAX_PKT_SZ.
        // - The last packet should contain only enough bytes to finish writing the buffer.
        //
        pktLen = (BULK_MAX_PKT_SZ > nBytes) ? nBytes : BULK_MAX_PKT_SZ;
        maxAddr = (address & 0xfffffe00) + 512;
        maxBytes = (maxAddr - address) * 2; // bytes to 512 address boundary (2 bytes per address)
        pktLen = (pktLen > maxBytes) ? maxBytes : pktLen;
        requestLen = pktLen;    // temp variable so we can check that the complete packet was sent

        // send one packet of data
        {
            int i;
            int rc;


            bytesWritten += pktLen;
            for (i = 0; i < 5; ++i)
            {
                rc =
                    usbagnet_bulk_msg (MPIF_WR_ENDPOINT, (void *) buf, pktLen, &requestLen,
                                       SND_PIPE_FLAG);

                if (rc == 0)
                {
                    DBG_PRINT(": sent %d bytes. rc=%d. %s %d", pktLen, rc, __FILE__, __LINE__);
                    break;
                }

                DBG_PRINT(": ERROR WARNING: failed attempt %d to write packet %ld pktLen=%d, %d bytes remaining to write. %d bytes written %s %d",
                    i, packetCount, pktLen, (int)(buf-p), bytesWritten, __FILE__, __LINE__);


                // try resetting the endpoint
                if ((i % 2) == 0)
                {
                    char tmpBuf[32];
                    DBG_PRINT(" Resetting endpoint. %s %d", __FILE__, __LINE__);
                    if (0 != usbagent_ctrl_msg (PMC_USB2_RESET_MPIF_ENDPOINTS, REQ_VENDOR, 0, 0,
                                       (void *) tmpBuf, 0, SND_PIPE_FLAG))
                    {

                        DBG_PRINT(
                             "ERROR:failed to send the control packet to reset endpoint.nBytes=%ld opCount=%ld address=0x%lx %s,%d",
                             nBytes, opCount, address, __FILE__, __LINE__);
                        return -1;
                    }

                    //myWrEndPoint->Abort ();
                    //dbg print myWrEndPoint->UsbdStatus
                    //DBG_PRINT(" Driver status: 0x%x. %s %d", 0, __FILE__, __LINE__);
                    //myWrEndPoint->Reset ();
                    //DBG_PRINT(" Driver status: 0x%x. %s %d", 0, __FILE__, __LINE__);
                }
                else
                {
                    // try closing and reopening the device.  Assumes only one device is connected.
                    DBG_PRINT(" Closing and reopening device. %s %d", __FILE__, __LINE__);
                    //this->close ();
                    //this->open ();
                }
                udelay (1);
            }

            if (rc != 0)
            {
                DBG_PRINT(" ERROR:failed to write data packet in five tries, pktLen=%d, %ld bytes remaining to write. %s %d",
                    pktLen, nBytes, __FILE__, __LINE__);
            }
        }

        // pktLen had better match requestLen.  If not, some bytes didn't get written.
        if (pktLen != requestLen)
        {
            DBG_PRINT(" ERROR:wrote %d bytes instead of expected %d. %s %d",
                pktLen, requestLen, __FILE__, __LINE__);

        }

        // This is just a sanity check.  It shouldn't be possible for us to send more than
        // the number of bytes outstanding.
        //
        if (pktLen > nBytes)
        {
            DBG_PRINT(" ERROR:wrote %d bytes when nBytes == %d. %s %d",
                pktLen, nBytes, __FILE__, __LINE__);
        }

        // Fetch the debug copy of the write packet from the USB device and compare it
        // to the data we sent.  Log an error if there's a problem.
        if (loopbackEnable)
        {
            int bc, rc;
            debugPacketLen = BULK_MAX_PKT_SZ;

            udelay (100);
            rc = usbagnet_bulk_msg (MPIF_RD_ENDPOINT, (void *) debugPacket, debugPacketLen, &pktLen,
                               RCV_PIPE_FLAG);

            if ( 0 != rc)
            {
                DBG_PRINT(" ERROR:failed to read debug packet. %s %d",
                    __FILE__, __LINE__);
            }
            DBG_PRINT(" pktLen = %d. %s %d", pktLen, __FILE__, __LINE__);

            for (bc = 0; bc < pktLen; ++bc)
            {
                DBG_PRINT(" 0x%x 0x%x  %s %d", (int) (debugPacket[bc]),(int) (buf[bc]),__FILE__, __LINE__);
                if (debugPacket[bc] != buf[bc])
                {
                    int kk;
                    DBG_PRINT(" ERROR: data mismatch starting at byte %d, pktLen = %d.  %s %d",
                        bc, pktLen, __FILE__, __LINE__);

                    for (kk = bc; kk < pktLen; ++kk)
                    {
                        DBG_PRINT(" buf[%d]=0x%x , read back 0x%x.  %s %d",
                            kk, (int) (buf[kk]), (int) (debugPacket[kk]), __FILE__, __LINE__);
                    }
                }
            }
        }

        // update the number of bytes remaining and move the buf pointer to the next packet location
        buf += pktLen;
        nBytes -= pktLen;

        // make sure the firmware counts packets the same way we do
        ++packetCount;
        DBG_PRINT(" pktLen = %d, nBytes = %d,  packetCount = %d. %s %d",
        pktLen, nBytes, packetCount, __FILE__, __LINE__);
    }

    DBG_PRINT(" successfully wrote %d packets. %s %d", packetCount, __FILE__, __LINE__);

    if (nBytes != 0)
    {
        // something went wrong
        DBG_PRINT(" ERROR: fewer bytes read than expected. nBytes == %ld. %s %d", nBytes, __FILE__, __LINE__);
        return -1;
   }

    // If we got here, all is well.
    return 0;
}


/******************************************************************************/
/**  USB Device Driver functions							                 **/
/******************************************************************************/

/**
 *	cyusb_debug_data
 */
static inline void
cyusb_debug_data (const char *function, int size, const unsigned char *data)
{
    int i;

    if (!debug)
        return;

    DBG_PRINT(KERN_DEBUG __FILE__ ": %s - length = %d, data = ", function, size);
    for (i = 0; i < size; ++i)
    {
        DBG_PRINT("%.2x ", data[i]);
    }
    DBG_PRINT("\n");
}

static inline int
usbagnet_bulk_msg (unsigned int ep, void *data, int len, int *actual_length,
                   int sndrcvFlg)
{
    int retval = -1;
    struct cyusb *dev;
    struct usb_device *udev;
    int timeout_value = 1000;


    if (dev_table[0] == NULL)
    {
        DBG_PRINT("No USB Device attached %s,%d", __FILE__, __LINE__);
        return retval;
    }

    dev = dev_table[0];
    udev = dev->udev;

    if (sndrcvFlg == SND_PIPE_FLAG)
    {
        retval = usb_bulk_msg (udev,
                               usb_sndbulkpipe (udev, ep),
                               data, len, actual_length, timeout_value);
    }
    else if (sndrcvFlg == RCV_PIPE_FLAG)
    {
        retval = usb_bulk_msg (udev,
                               usb_rcvbulkpipe (udev, ep),
                               data, len, actual_length, timeout_value);
    }
    else
    {
        DBG_PRINT(
             "usbagnet_bulk_msg: Give me the correct sndrcvFlg! %s,%d",
             __FILE__, __LINE__);
    }

    if (retval < 0)
    {
        DBG_PRINT(
             "usb_bulk_msg FAIL! - ep:%d, len:%d *actual_length:%d retval=%d %s,%d",
             ep, len, *actual_length, retval, __FILE__, __LINE__);
        return retval;
    }

    return retval;
}


static inline int
usbagent_ctrl_msg (__u8 request, __u8 requesttype, __u16 value, __u16 index,
                   void *data, __u16 size, int sndrcvFlg)
{
    int retval = -1;
    struct cyusb *dev;
    struct usb_device *udev;

    if (dev_table[0] == NULL)
    {
        DBG_PRINT("No USB Device attached %s,%d", __FILE__, __LINE__);
        return retval;
    }

    dev = dev_table[0];
    udev = dev->udev;

    if (sndrcvFlg == SND_PIPE_FLAG)
    {
        retval = usb_control_msg (udev,
                                usb_sndctrlpipe (udev, 0),
                                  request, requesttype,
                                  value, index, data, size, 5000);
    }
    else if (sndrcvFlg == RCV_PIPE_FLAG)
    {
        retval = usb_control_msg (udev,
                                  usb_rcvctrlpipe (udev, 0),
                                  request, requesttype,
                                  value, index, data, size, 5000);
    }
    else
    {
        DBG_PRINT(
             "usb_control_msg: Give me the correct sndrcvFlg! %s,%d",
             __FILE__, __LINE__);
    }

    if (retval < 0)
    {
        DBG_PRINT(
             "FAIL! - REQUEST:0x%x, REQUEST_TYPE:0x%x, retval=%d %s,%d",
             request, requesttype, retval, __FILE__, __LINE__);
        return retval;
    }

    return retval;
}



static void
recv_from_fxload (struct sock *sk, int len)
{
    DBG_PRINT(":Rev a msg\n");

    /* Do nothing.. */
    wake_up (sk->sleep);
    return;
}

static int
send_to_fxload_process (void *data)
{
    struct sk_buff *skb = NULL;
    struct nlmsghdr *nlhdr = NULL;

    daemonize ();
    // Setup a nice name
    strcpy (current->comm, "send_to_fxload");

    while (exit_flag == 0)
    {
        interruptible_sleep_on (fxload_sock->sleep);

        DBG_PRINT(":Wake up from sleeping");
        if (exit_flag == 1)
        {
            goto process_complete;
        }
        else
        {

            struct usb_device *udev;
            fxload_msg *msg = NULL;
            unsigned char *old_tail;

            if (dev_table[0] == NULL)
            {
                DBG_PRINT(":No USB Device attached %s,%d", __FILE__,
                     __LINE__);
                goto process_complete;
            }
            udev = (dev_table[0])->udev;

            skb = alloc_skb (50, GFP_KERNEL);

            if (!skb)
            {
                DBG_PRINT(":alloc_skb fail !");
                return -1;
            }

            old_tail = skb->tail;
            nlhdr =
                NLMSG_PUT (skb, 0, 0, FXLOAD_NETLINK_TYPE, sizeof (fxload_msg));
            msg = NLMSG_DATA (nlhdr);
            // then fill the data to msg pointer.
            msg->busnum = (udev->bus)->busnum;
            msg->devnum = udev->devnum;

            nlhdr->nlmsg_len = skb->tail - old_tail;

            NETLINK_CB (skb).dst_groups = 1;

            // we don't need to free skb, netlink_broadcast will do this.
            netlink_broadcast (fxload_sock, skb, 0, 1, GFP_KERNEL);
        }

    }

  process_complete:
    complete (&exit_completion);
    return 0;

  nlmsg_failure:
    return -1;

}


static int
usbagent_read_proc (char *page, char **start, off_t offset, int count,
                    int *eof, void *data)
{
    int len = 0;
    struct cyusb *dev;
    struct usb_device *udev;

    if (dev_table[0] == NULL)
    {
        len += sprintf (page + len, "No USB Device attached\n");
        return len;
    }

    if ((active_cmd_p == NULL) || 0 == strcmp (active_cmd_p->keyword, "NULL"))
    {
        len += sprintf (page + len, "Invalid acitve command\n");
        return len;
    }

    dev = dev_table[0];
    udev = dev->udev;

    if (0 == strcmp (active_cmd_p->keyword, "hyPhy20gRead"))
    {
        unsigned char tmp[10];

        active_cmd_p->fp (active_cmd_p->argv[0], active_cmd_p->argv[1], &tmp);
        len += sprintf (page + len, "0x%x 0x%x\n", tmp[0], tmp[1]);
    }
    else if (0 == strcmp (active_cmd_p->keyword, "hyPhy20gWrite"))
    {
        active_cmd_p->fp (active_cmd_p->argv[0], active_cmd_p->argv[1],
                          &(active_cmd_p->argv[2]), active_cmd_p->argv[3]);
        len +=
            sprintf (page + len, "OK\nhyPhy20gWrite:%ld\n",
                     active_cmd_p->argv[2]);
    }
    else if (0 == strcmp (active_cmd_p->keyword, "twiReadBurst"))
    {
        unsigned char tmp = 0;

        active_cmd_p->fp (active_cmd_p->argv[0], active_cmd_p->argv[1],
                          active_cmd_p->argv[2], active_cmd_p->argv[3], &tmp);
        len += sprintf (page + len, "OK\ntwiReadBurst:0x%x\n", tmp);
    }
    else if (0 == strcmp (active_cmd_p->keyword, "twiWriteBurst"))
    {
        unsigned char tmp = (unsigned char) active_cmd_p->argv[4];

        active_cmd_p->fp (active_cmd_p->argv[0], active_cmd_p->argv[1],
                          active_cmd_p->argv[2], active_cmd_p->argv[3], &tmp);
        len += sprintf (page + len, "OK\ntwiWriteBurst:0x%x\n", tmp);
    }
    else if (0 == strcmp (active_cmd_p->keyword, "setNonMuxMode") ||
             0 == strcmp (active_cmd_p->keyword, "setMuxMode"))
    {
        active_cmd_p->fp ();
    }
    else if (0 == strcmp (active_cmd_p->keyword, "getInfo"))
    {
        len +=
            sprintf (page + len,
                     "Device Info\ndevnum:\t\t\t0x%x\ndevpath:\t\t%s\nspeed:\t\t\t0x%x\n",
                     udev->devnum, udev->devpath, udev->speed);

        len +=
            sprintf (page + len,
                     "idVendor:\t\t0x%x\nidProduct:\t\t0x%x\nbcdDevice:\t\t0x%lx\nbDeviceClass:\t\t0x%x\nbDeviceSubClass:\t0x%x\nbDeviceProtocol:\t0x%x\niSerialNumber:\t\t0x%x\n",
                     udev->descriptor.idVendor, udev->descriptor.idProduct,
                     udev->descriptor.bcdDevice,
                     udev->descriptor.bDeviceClass,
                     udev->descriptor.bDeviceSubClass,
                     udev->descriptor.bDeviceProtocol,
                     udev->descriptor.iSerialNumber);
    }
    else if (0 == strcmp (active_cmd_p->keyword, "help"))
    {
        len +=
            sprintf (page + len,
                     "Usage:\n\techo command > /proc/%s then cat /proc/%s\n",
                     ezusb_proc_dir, ezusb_proc_dir);
        len += sprintf (page + len, "Command\t\tDescription\n");
    }
    else if (0 == strcmp (active_cmd_p->keyword, "getMpifSS"))
    {
        unsigned char tmp[512];
        pmcFX2_getMpifStatusSimple(tmp);
        len += sprintf (page + len, "%s", tmp);
    }
    else if (0 == strcmp (active_cmd_p->keyword, "i2cWrite"))
    {
        unsigned char tmp = (unsigned char) active_cmd_p->argv[2];
        active_cmd_p->fp (active_cmd_p->argv[0], active_cmd_p->argv[1],&tmp);
        len += sprintf (page + len, "OK\ni2cWrite:0x%x\n", tmp);
    }
    else if (0 == strcmp (active_cmd_p->keyword, "i2cRead"))
    {
        unsigned char tmp[10];
        active_cmd_p->fp (active_cmd_p->argv[0], active_cmd_p->argv[1],&tmp);
        len += sprintf (page + len, "OK\ni2cRead:0x%x 0x%x\n", tmp[0], tmp[0]);
    }
    else
    {
        len += sprintf (page + len, "Unknow command.\n");
    }

    return len;
}



static int
usbagent_write_proc (struct file *file, const char *buffer,
                     unsigned long count, void *data)
{
    int len = strlen (buffer), i, retval = 0;
    char *local_buf = kmalloc ((len + 1) * sizeof (char), GFP_KERNEL);
    char *cname, *argstr, *rest;

    active_cmd_p = NULL;
    if (dev_table[0] == NULL)
    {
        DBG_PRINT(" No USB Device attached %s %d", __FILE__, __LINE__);
        return count;
    }
    strcpy (local_buf, buffer);

    /* Make the string parser happy! */
    local_buf[len - 1] = 32;
    local_buf[len] = '\0';

    DBG_PRINT(" - local_buf is %s, %s,%d", local_buf, __FILE__,
         __LINE__);
    argstr = local_buf;
    if ((cname = next_token (argstr, " ", &rest)) == NULL)
    {
        retval = -1;
    }

    for (i = 0; strcmp (cmdTbl[i].keyword, "END") != 0; i++)
    {

        if (0 == strcmp (cmdTbl[i].keyword, cname))
        {
            int j = 0;

            active_cmd_p = &cmdTbl[i];

            DBG_PRINT(" - Found cmd: %s i:%d argc:%d %s,%d",
                 active_cmd_p->keyword, i, active_cmd_p->argc, __FILE__,
                 __LINE__);

            for (j = active_cmd_p->argc; (active_cmd_p != NULL && (j > 0)); j--)
            {
                argstr = rest;

                if (argstr[0] == '0' && (argstr[1] == 'x' || argstr[1] == 'X'))
                {
                    if (gendrv_extract_hex
                        (argstr, " ", &rest,
                         (unsigned long *) &(active_cmd_p->
                                             argv[active_cmd_p->argc - j])) !=
                        0)
                    {
                        DBG_PRINT(" - Found argv[%d]: 0x%lx %s,%d",
                             active_cmd_p->argc - j,
                             active_cmd_p->argv[active_cmd_p->argc - j],
                             __FILE__, __LINE__);
                        retval = -1;
                    }
                }
                else
                {
                    if (gendrv_extract_dec
                        (argstr, " ", &rest,
                         (unsigned long *) &(active_cmd_p->
                                             argv[active_cmd_p->argc - j])) !=
                        0)
                    {
                        DBG_PRINT(" - Found argv[%d]: 0x%lx %s,%d",
                             active_cmd_p->argc - j,
                             active_cmd_p->argv[active_cmd_p->argc - j],
                             __FILE__, __LINE__);
                        retval = -1;
                    }

                }

            }

            return count;
        }
    }

    return count;
}


/**
 *	usbagent_delete
 */
static inline void
usbagent_delete (struct cyusb *dev)
{
    dev_table[dev->minor] = NULL;
    if (dev->bulk_in_buffer != NULL)
        kfree (dev->bulk_in_buffer);
    if (dev->bulk_out_buffer != NULL)
        kfree (dev->bulk_out_buffer);
    if (dev->write_urb != NULL)
        usb_free_urb (dev->write_urb);
    kfree (dev);
}


/**
 *	usbagent_open
 */
static int
usbagent_open (struct inode *inode, struct file *file)
{
    struct cyusb *dev = NULL;
    int subminor;
    int retval = 0;

    subminor = MINOR (inode->i_rdev) - CYUSB_MINOR_BASE;
    if ((subminor < 0) || (subminor >= MAX_DEVICES))
    {
        return -ENODEV;
    }

    /* Increment our usage count for the module.
     * This is redundant here, because "struct file_operations"
     * has an "owner" field. This line is included here soley as
     * a reference for drivers using lesser structures... ;-)
     */
    MOD_INC_USE_COUNT;

    /* lock our minor table and get our local data for this minor */
    down (&dev_table_mutex);
    dev = dev_table[subminor];
    if (dev == NULL)
    {
        up (&dev_table_mutex);
        MOD_DEC_USE_COUNT;
        return -ENODEV;
    }

    /* lock this device */
    down (&dev->sem);

    /* unlock the minor table */
    up (&dev_table_mutex);

    /* increment our usage count for the driver */
    ++dev->open_count;

    /* save our object in the file's private structure */
    file->private_data = dev;

    /* unlock this device */
    up (&dev->sem);

    return retval;
}


/**
 *	usbagent_release
 */
static int
usbagent_release (struct inode *inode, struct file *file)
{
    struct cyusb *dev;
    int retval = 0;

    dev = (struct cyusb *) file->private_data;
    if (dev == NULL)
    {
        DBG_PRINT(" - object is NULL");
        return -ENODEV;
    }

    DBG_PRINT(" - minor %d", dev->minor);

    /* lock our minor table */
    down (&dev_table_mutex);

    /* lock our device */
    down (&dev->sem);

    if (dev->open_count <= 0)
    {
        DBG_PRINT(" - device not opened");
        retval = -ENODEV;
        goto exit_not_opened;
    }

    if (dev->udev == NULL)
    {
        /* the device was unplugged before the file was released */
        up (&dev->sem);
        usbagent_delete (dev);
        up (&dev_table_mutex);
        MOD_DEC_USE_COUNT;
        return 0;
    }

    /* decrement our usage count for the device */
    --dev->open_count;
    if (dev->open_count <= 0)
    {
        /* shutdown any bulk writes that might be going on */
        usb_unlink_urb (dev->write_urb);
        dev->open_count = 0;
    }

    /* decrement our usage count for the module */
    MOD_DEC_USE_COUNT;

  exit_not_opened:
    up (&dev->sem);
    up (&dev_table_mutex);

    return retval;
}


/**
 *	usbagent_read
 */
static ssize_t
usbagent_read (struct file *file, char *buffer, size_t count, loff_t * ppos)
{
    struct cyusb *dev;
    int retval = 0;

    dev = (struct cyusb *) file->private_data;

    DBG_PRINT(" - minor %d, count = %d", dev->minor, count);

    /* lock this object */
    down (&dev->sem);

    /* verify that the device wasn't unplugged */
    if (dev->udev == NULL)
    {
        up (&dev->sem);
        return -ENODEV;
    }

    /* do an immediate bulk read to get data from the device */
    retval = usb_bulk_msg (dev->udev,
                           usb_rcvbulkpipe (dev->udev,
                                            dev->bulk_in_endpointAddr),
                           dev->bulk_in_buffer, dev->bulk_in_size,
                           &count, HZ * 10);

    /* if the read was successful, copy the data to userspace */
    if (!retval)
    {
        if (copy_to_user (buffer, dev->bulk_in_buffer, count))
            retval = -EFAULT;
        else
            retval = count;
    }

    /* unlock the device */
    up (&dev->sem);
    return retval;
}


/**
 *	usbagent_write
 */
static ssize_t
usbagent_write (struct file *file, const char *buffer, size_t count,
                loff_t * ppos)
{
    struct cyusb *dev;
    ssize_t bytes_written = 0;
    int retval = 0;

    dev = (struct cyusb *) file->private_data;

    DBG_PRINT(" - minor %d, count = %d", dev->minor, count);

    /* lock this object */
    down (&dev->sem);

    /* verify that the device wasn't unplugged */
    if (dev->udev == NULL)
    {
        retval = -ENODEV;
        goto exit;
    }

    /* verify that we actually have some data to write */
    if (count == 0)
    {
        DBG_PRINT(" - write request of 0 bytes");
        goto exit;
    }

    /* see if we are already in the middle of a write */
    if (dev->write_urb->status == -EINPROGRESS)
    {
        DBG_PRINT(" - already writing");
        goto exit;
    }

    /* we can only write as much as 1 urb will hold */
    bytes_written = (count > dev->bulk_out_size) ? dev->bulk_out_size : count;

    /* copy the data from userspace into our urb */
    if (copy_from_user (dev->write_urb->transfer_buffer, buffer, bytes_written))
    {
        retval = -EFAULT;
        goto exit;
    }

    cyusb_debug_data (__FUNCTION__, bytes_written,
                      dev->write_urb->transfer_buffer);

    /* set up our urb */
    FILL_BULK_URB (dev->write_urb, dev->udev,
                   usb_sndbulkpipe (dev->udev, dev->bulk_out_endpointAddr),
                   dev->write_urb->transfer_buffer, bytes_written,
                   usbagent_write_bulk_callback, dev);

    /* send the data out the bulk port */
    retval = usb_submit_urb (dev->write_urb);
    if (retval)
    {
        DBG_PRINT(" - failed submitting write urb, error %d", retval);
    }
    else
    {
        retval = bytes_written;
    }

  exit:
    /* unlock the device */
    up (&dev->sem);

    return retval;
}


/**
 *	usbagent_ioctl
 */
static int
usbagent_ioctl (struct inode *inode, struct file *file, unsigned int cmd,
                unsigned long arg)
{
    struct cyusb *dev;
    struct usb_device *usbdev;
    int retval = -ENOTTY;

    dev = (struct cyusb *) file->private_data;

    /* lock this object */
    down (&dev->sem);

    /* verify that the device wasn't unplugged */
    if (dev->udev == NULL)
    {
        up (&dev->sem);
        return retval;
    }
    usbdev = dev->udev;
    DBG_PRINT(" - minor %d, cmd 0x%.4x, arg %ld", dev->minor, cmd, arg);


    /* fill in your device specific stuff here */

    /* unlock the device */
    up (&dev->sem);

    /* return that we did not understand this ioctl call */
    return retval;
}


/**
 *	usbagent_write_bulk_callback
 */
static void
usbagent_write_bulk_callback (struct urb *urb)
{
    struct cyusb *dev = (struct cyusb *) urb->context;

    DBG_PRINT(" - minor %d", dev->minor);

    if ((urb->status != -ENOENT) && (urb->status != -ECONNRESET))
    {
        DBG_PRINT(" - nonzero write bulk status received: %d",
             urb->status);
        return;
    }

    return;
}


/**
 *	usbagent_probe
 *
 *	Called by the usb core when a new device is connected that it thinks
 *	this driver might be interested in.
 */
static void *
usbagent_probe (struct usb_device *udev, unsigned int ifnum,
                const struct usb_device_id *id)
{
    struct cyusb *dev = NULL;
    struct usb_interface *interface;
    struct usb_interface_descriptor *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int minor;
    int buffer_size;
    int i;
    char name[10];


    /* See if the device offered us matches what we can accept */
    if ((udev->descriptor.idVendor != CYUSB_VENDOR_ID)
        && (udev->descriptor.idVendor != 0x4705))
    {
        return NULL;
    }
    if ((udev->descriptor.idProduct != CYUSB_PRODUCT_ID_HYPHY) &&
        (udev->descriptor.idProduct != CYUSB_PRODUCT_ID_WIZIRD) &&
        (udev->descriptor.idProduct != 0xedfe))
    {
        return NULL;
    }

    /* select a "subminor" number (part of a minor number) */
    down (&dev_table_mutex);
    for (minor = 0; minor < MAX_DEVICES; ++minor)
    {
        if (dev_table[minor] == NULL)
            break;
    }
    if (minor >= MAX_DEVICES)
    {
        info ("Too many devices plugged in, can not handle this device.");
        goto exit;
    }

    /* allocate memory for our device state and intialize it */
    dev = kmalloc (sizeof (struct cyusb), GFP_KERNEL);
    if (dev == NULL)
    {
        err ("Out of memory");
        goto exit;
    }
    memset (dev, 0x00, sizeof (*dev));
    dev_table[minor] = dev;

    interface = &udev->actconfig->interface[ifnum];

    init_MUTEX (&dev->sem);
    dev->udev = udev;
    dev->interface = interface;
    dev->minor = minor;

    /* set up the endpoint information */
    /* check out the endpoints */
    iface_desc = &interface->altsetting[0];
    for (i = 0; i < iface_desc->bNumEndpoints; ++i)
    {
        endpoint = &iface_desc->endpoint[i];

        if ((endpoint->bEndpointAddress & 0x80) &&
            ((endpoint->bmAttributes & 3) == 0x02))
        {
            /* we found a bulk in endpoint */
            buffer_size = endpoint->wMaxPacketSize;
            dev->bulk_in_size = buffer_size;
            dev->bulk_in_endpointAddr = endpoint->bEndpointAddress;
            dev->bulk_in_buffer = kmalloc (buffer_size, GFP_KERNEL);
            if (!dev->bulk_in_buffer)
            {
                err ("Couldn't allocate bulk_in_buffer");
                goto error;
            }
        }

        if (((endpoint->bEndpointAddress & 0x80) == 0x00) &&
            ((endpoint->bmAttributes & 3) == 0x02))
        {
            /* we found a bulk out endpoint */
            dev->write_urb = usb_alloc_urb (0);
            if (!dev->write_urb)
            {
                err ("No free urbs available");
                goto error;
            }
            buffer_size = endpoint->wMaxPacketSize;
            dev->bulk_out_size = buffer_size;
            dev->bulk_out_endpointAddr = endpoint->bEndpointAddress;
            dev->bulk_out_buffer = kmalloc (buffer_size, GFP_KERNEL);
            if (!dev->bulk_out_buffer)
            {
                err ("Couldn't allocate bulk_out_buffer");
                goto error;
            }
            FILL_BULK_URB (dev->write_urb, udev,
                           usb_sndbulkpipe (udev,
                                            endpoint->bEndpointAddress),
                           dev->bulk_out_buffer, buffer_size,
                           usbagent_write_bulk_callback, dev);
        }
    }

    /* initialize the devfs node for this device and register it */
    sprintf (name, "agent %d", dev->minor);

    dev->devfs = devfs_register (usb_devfs_handle, name,
                                 DEVFS_FL_DEFAULT, USB_MAJOR,
                                 CYUSB_MINOR_BASE + dev->minor,
                                 S_IFCHR | S_IRUSR | S_IWUSR |
                                 S_IRGRP | S_IWGRP | S_IROTH,
                                 &usbagent_fops, NULL);

    /* let the user know what node this device is now attached to */
    info ("CY7C68013A USB Device device now attached to usbagent %d",
          dev->minor);

    if (udev->descriptor.idVendor == CYUSB_VENDOR_ID &&
        udev->descriptor.idProduct == CYUSB_PRODUCT_ID_WIZIRD)
    {
        if (fxload_sock == 0)
        {
            fxload_sock =
                netlink_kernel_create (FXLOAD_NETLINK_TYPE, recv_from_fxload);
            if (!fxload_sock)
            {
                info ("can not create a netlink socket");
                goto error;
            }
            kernel_thread (send_to_fxload_process, NULL,
                           (CLONE_FS | CLONE_FILES | CLONE_SIGHAND));

        }
        udelay (500);
        info ("wake_up sock id 0x%x", fxload_sock);
        wake_up (fxload_sock->sleep);
    }

    goto exit;

  error:
    usbagent_delete (dev);
    dev = NULL;

  exit:
    up (&dev_table_mutex);
    return dev;
}


/**
 *	usbagent_disconnect
 *
 *	Called by the usb core when the device is removed from the system.
 */
static void
usbagent_disconnect (struct usb_device *udev, void *ptr)
{
    struct cyusb *dev;
    int minor;

    dev = (struct cyusb *) ptr;

    down (&dev_table_mutex);
    down (&dev->sem);

    minor = dev->minor;

    /* remove our devfs node */
    devfs_unregister (dev->devfs);

    /* if the device is not opened, then we clean up right now */
    if (!dev->open_count)
    {
        up (&dev->sem);
        usbagent_delete (dev);
    }
    else
    {
        dev->udev = NULL;
        up (&dev->sem);
    }

    info ("CY7C68013A USB Device #%d now disconnected", minor);
    up (&dev_table_mutex);
}



/**
 *	usbagent_init
 */
static int __init
usbagent_init (void)
{
    int result;
    struct proc_dir_entry *res;

    if (fxload_sock == 0)
    {

        fxload_sock =
            netlink_kernel_create (FXLOAD_NETLINK_TYPE, recv_from_fxload);
        if (!fxload_sock)
        {
            err ("can not create a netlink socket\n");
            return -1;
        }
        kernel_thread (send_to_fxload_process, NULL,
                       (CLONE_FS | CLONE_FILES | CLONE_SIGHAND));
        info ("fxload_sock 0x%x is created in usbagent_init", fxload_sock);

    }
    /* register this driver with the USB subsystem */
    result = usb_register (&usbagent_drv);
    if (result < 0)
    {
        err ("usb_register failed for the " __FILE__ " driver. Error number %d",
             result);
        return -1;
    }

    res = create_proc_entry (usbagent_proc_filename, 0, NULL);
    if (res)
    {
        res->read_proc = usbagent_read_proc;
        res->write_proc = usbagent_write_proc;
    }

    info (DRIVER_DESC " " DRIVER_VERSION);
    return 0;
}


/**
 *	usbagent_exit
 */
static void __exit
usbagent_exit (void)
{
    remove_proc_entry (usbagent_proc_filename, 0);
    exit_flag = 1;
    wake_up (fxload_sock->sleep);
    wait_for_completion (&exit_completion);

    // remove netlink socket
    if (fxload_sock)
    {
        sock_release (fxload_sock->socket);
    }

    /* deregister this driver with the USB subsystem */
    usb_deregister (&usbagent_drv);
}

EXPORT_SYMBOL (pmcFX2_rd);
EXPORT_SYMBOL (pmcFX2_wr);
EXPORT_SYMBOL (pmcFX2_twiWriteBurst);
EXPORT_SYMBOL (pmcFX2_twiReadBurst);

module_init (usbagent_init);
module_exit (usbagent_exit);

MODULE_AUTHOR (DRIVER_AUTHOR);
MODULE_DESCRIPTION (DRIVER_DESC);
MODULE_LICENSE ("GPL");
