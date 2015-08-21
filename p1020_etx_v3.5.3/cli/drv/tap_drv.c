
#include "tap_drv.h"
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/ioport.h>


/* module "device name" */
#define DEVICE_NAME 		"/dev/bypass_cfgmod"
/* IOCTL MAJOR NUMBER */
#define EX10_MAJOR_NUM		0xcc
#define EX10_IOC_MAGIC	  	'b'
/* IOCTLs */
#define IOCTL_GET_REGS		_IOR(EX10_IOC_MAGIC, 1, unsigned long)
#define IOCTL_SET_REGS		_IOWR(EX10_IOC_MAGIC, 2, unsigned long)


/* 
 * Flag open status to prevent multiple opens.
 */
static int Is_Open = 0;

struct file_operations File_ops = {
	.read = NULL,
	.write = NULL,
	.ioctl = interface_ioctl,
	.open = interface_open,
	.release = interface_close,	
};

void 
interface_get_registers(unsigned long *pRegs)
{
	unsigned long temp = 0x0000dead;
	void *p_mm_addr, *p_io;
  
	/* memory mapped registes/etc */
	p_mm_addr = ioremap(MM_FPGA_BASE, MM_FPGA_LENGTH);
	if(!p_mm_addr) 
	{
		printk("unable to map FPGA register space\n");
	} 
	else 
	{
		p_io = p_mm_addr;
		temp = in_be32(p_io);
	}
	iounmap(p_mm_addr);
	
	copy_to_user(pRegs, &temp, sizeof(unsigned long));
}


void 
interface_set_registers(unsigned long *pRegs)
{
	unsigned long temp = 0x0000dead;
	void *p_mm_addr, *p_io;
	
	
	copy_from_user(&temp, pRegs, sizeof(unsigned long));
	
	/* memory mapped registes/etc */
	p_mm_addr = ioremap(MM_FPGA_BASE, MM_FPGA_LENGTH);
	if(!p_mm_addr) 
	{
		printk("unable to map FPGA register space\n");
	} 
	else 
	{
		p_io = p_mm_addr;
		out_be32(p_io, temp);
		iounmap(p_mm_addr);
	}
}

int 
interface_ioctl (struct inode *inode, struct file *file, 
	unsigned int ioctl_num, unsigned long ioctl_param)
{
	switch (ioctl_num) 
	{
		case IOCTL_GET_REGS:
			interface_get_registers((unsigned long *)ioctl_param); 
			break;
		
		case IOCTL_SET_REGS:
			interface_set_registers((unsigned long *)ioctl_param);
			break;
		
		default:
		printk("tap_ctl: uknown IOCTL\n");
	}

	return(0);
}


/* 
 * interface_open()
 * Handle a file open request
 */
static int 
interface_open (struct inode *inode, struct file *file)
{
	try_module_get(THIS_MODULE);
	
	return(0);
}

/* 
 * interface_close()
 * Handle a file close request
 */
static int 
interface_close (struct inode *inode, struct file *file)
{
	module_put(THIS_MODULE);
	
	return(0);
}

/*
 * module init
 */
static int __init 
interface_init (void) 
{
	int ret_val;

	ret_val = register_chrdev(EX10_MAJOR_NUM, DEVICE_NAME, &File_ops);
	
	return(ret_val);
}

/* 
 * module exit
 */
static void __exit 
interface_cleanup (void) 
{
    unregister_chrdev(EX10_MAJOR_NUM, DEVICE_NAME);
}


MODULE_AUTHOR("Qice Tech");
MODULE_DESCRIPTION("device driver module for Qice QNF inteligent capture cards");
MODULE_SUPPORTED_DEVICE("qnf");
MODULE_LICENSE("GPL");

module_init(interface_init);
module_exit(interface_cleanup);


