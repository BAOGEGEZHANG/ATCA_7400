
#ifndef __TAPDRV_H__
#define __TAPDRV_H__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <asm/dcr-native.h>
#include <asm/dcr-regs.h>
#include <asm/reg_booke.h>
#include <asm/uaccess.h>


/* prototypes */
int interface_ioctl (struct inode *inode, struct file *file, 
		unsigned int ioctl_num, unsigned long ioctl_param);
static int interface_open (struct inode *inode, struct file *file);
static int interface_close (struct inode *inode, struct file *file);

static int __init interface_init (void);
static void __exit interface_cleanup (void);


/* memory mapped address*/
#define MM_FPGA_BASE		0xF0000000
#define MM_FPGA_LENGTH		0x4

#endif

