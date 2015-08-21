/*
 * ex10 tap bypass config IOCTL definitions
 * 
 */

#include <linux/ioctl.h>


/* module "device name" */
#define DEVICE_NAME 		"/dev/bypass_cfgmod"

/* IOCTL MAJOR NUMBER */
#define EX10_MAJOR_NUM		0xcc

#define EX10_IOC_MAGIC	  	'b'

/* IOCTLs */
#define IOCTL_GET_REGS		_IOR(EX10_IOC_MAGIC, 1, unsigned long)
#define IOCTL_SET_REGS		_IOWR(EX10_IOC_MAGIC, 2, unsigned long)

