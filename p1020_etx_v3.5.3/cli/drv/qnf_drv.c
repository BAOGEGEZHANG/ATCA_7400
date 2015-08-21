/*
 * Copyright (c) 2010-2015 Qice Technology Ltd, Beijing, China.
 * All rights reserved.
 *
 * This source code is proprietary to Qice Technology Limited and no part
 * of it may be redistributed, published or disclosed except as outlined in
 * the written contract supplied with this product.
 *
 *******************************************************************************
 * File Name  		: 	qnf_drv.c
 * Description		:	main driver file for qnfxxx cards
 *******************************************************************************/


#include <linux/pci.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fs.h>

#ifndef LINUX_VERSION_CODE
#include <linux/version.h>
#endif

#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h> 


#include "qnf_pci.h"
#include "qnf_drv.h"


static int optLoopback = 0;
module_param(optLoopback, int, 0644);
MODULE_PARM_DESC(optLoopback, "Enable/Disable optical loopback");


/* enable the debug message when necessary */
//#define DEBUG

#ifdef DEBUG
# define QNF_DEBUG(MSG) printk MSG
#else
# define QNF_DEBUG(MSG)
#endif

#define  QNF_MSG(MSG) printk MSG


const char* const kqnfReleaseVersion = "2.0";

#define QNFUNIT(X)	      (iminor(X)&QNFMINORMASK)
#define QNFMINOR(X)	      (iminor(X)>>QNFMINORBITS)
#define QNF_MAX_BOARDS		(1<<QNFMINORBITS)

enum {
	QNFF_ATTACHED	= 0x0001,
	QNFF_INITED   = 0x0002,
};

/* Driver global array of data structures holding state information per card */
static qnf_softc_t	qnf_array[QNF_MAX_BOARDS];
static dev_t		    qnf_dev;

extern struct file_operations	qnf_fops;

static int
qnf_find_unattached(void)
{
	int c;
	
	for(c=0; c<QNF_MAX_BOARDS; c++)
		if (!(qnf_array[c].flags & QNFF_ATTACHED))
			return c;
	
	return -1;
}

// PCI configuration save/restore, as configured by BIOS on system startup
static void
qnf_pciconfig_save(qnf_softc_t *sc)
{
	pci_read_config_dword(sc->device, PCI_BASE_ADDRESS_0,
			      &sc->pciconfig.ba0);
	pci_read_config_dword(sc->device, PCI_BASE_ADDRESS_1,
			      &sc->pciconfig.ba1);
	pci_read_config_dword(sc->device, PCI_BASE_ADDRESS_2,
			      &sc->pciconfig.ba2);
	pci_read_config_dword(sc->device, PCI_COMMAND,
			      &sc->pciconfig.com);
	pci_read_config_byte(sc->device, PCI_INTERRUPT_LINE,
			     &sc->pciconfig.lin);
	pci_read_config_byte(sc->device, PCI_LATENCY_TIMER,
			     &sc->pciconfig.lat);
	
	QNF_DEBUG((KERN_DEBUG "qnf%d: ba0 0x%x\n", sc->unit, sc->pciconfig.ba0));
	QNF_DEBUG((KERN_DEBUG "qnf%d: ba1 0x%x\n", sc->unit, sc->pciconfig.ba1));
	QNF_DEBUG((KERN_DEBUG "qnf%d: ba2 0x%x\n", sc->unit, sc->pciconfig.ba2));
	QNF_DEBUG((KERN_DEBUG "qnf%d: com 0x%x\n", sc->unit, sc->pciconfig.com));
	QNF_DEBUG((KERN_DEBUG "qnf%d: lin 0x%x\n", sc->unit, sc->pciconfig.lin));
	QNF_DEBUG((KERN_DEBUG "qnf%d: lat 0x%x\n", sc->unit, sc->pciconfig.lat));
}


extern int hyphy20g_init(struct file *sHandle, int optLoopback);

static int
qnf_open(struct inode *inode, struct file *file)
{
  int		      unit = QNFUNIT(inode);
	qnfminor_t	minor = QNFMINOR(inode);
	qnf_softc_t	*sc;
	
	if(!file->private_data && minor) {
		/* check minor number is valid */
		if (minor >= QNFMINOR_MAX) {
			QNF_MSG( (KERN_ERR "qnf%u: open: illegal minor %u\n", unit, minor) );
			return -ENODEV;
		}
		
		file->f_op = &qnf_fops;
	}
	
	sc = (qnf_softc_t *)file->private_data;
	
	/* if file->private_data is not valid, then we are not using devfs.
	 * This means we need to check that the unit number is valid, and
	 * fill in private_data for other methods
	 */
	if (!sc) {
		/* Unit spans too high */
		if(unit > QNF_MAX_BOARDS ) {
			QNF_MSG( (KERN_WARNING "qnf%u: open: unit number too high, does not exist\n", unit) );
			return -ENODEV;
		}
		
		/* Unit in valid range, but card may has been removed */
		if(!(qnf_array[unit].flags & QNFF_ATTACHED)) {
			QNF_MSG( (KERN_WARNING "qnf%u: open: unit does not exist\n", unit) );
			return -ENODEV;
		}
		
		file->private_data = &qnf_array[unit];
		sc = (qnf_softc_t *)file->private_data;
	}

#if 1 
  	if (!(qnf_array[unit].flags & QNFF_INITED)) {
  		if (sc->devId == PCI_DEVICE_ID_MC20P)
  			hyphy20g_init(file, optLoopback);	
  	
  	qnf_array[unit].flags |= QNFF_INITED;
  }	
#endif
  	
  return 0;
}

static int
qnf_release(struct inode *inode, struct file * file)
{
  //do nothing
  
  return 0;
}

static long
qnf_ioctl(struct file *fp, u_int cmd, u_long arg)
{	
  int	        error=0;
  reg16_t     reg16_arg;
  reg32_t     reg32_arg; 
  qnf_softc_t *sc=fp->private_data;
  qnfinf_t	  info;	
  
  if (_IOC_TYPE(cmd) != QNF_IOC_MAGIC) 
    return -ENOTTY;
        
  if (_IOC_NR(cmd) > QNF_IOC_MAXNR) 
    return -ENOTTY;
    
  // do some checks	
  if (_IOC_DIR(cmd) & _IOC_READ)
    error = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
  else if (_IOC_DIR(cmd) & _IOC_WRITE)
    error = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
  if (error) return -EFAULT;
  
  
  switch(cmd) {
    case QNFIOCR16:
      if(copy_from_user(&reg16_arg, (reg16_t *)arg, sizeof(reg16_t)))
	    {
	      error = -EFAULT;
	    }
	    
	    if (reg16_arg.offset > sc->mmio_size)
	      return -EFAULT;
	        
	    reg16_arg.value = *(unsigned short *)(sc->mmio + reg16_arg.offset);
              
      if(copy_to_user((void*)arg, &reg16_arg, sizeof(reg16_t)))
		  {
			  error = -EFAULT;
		  }
		  break;

    case QNFIOCW16:
      if(copy_from_user(&reg16_arg, (reg16_t *)arg, sizeof(reg16_t)))
	    {
	      error = -EFAULT;
	    }
	    
	    if (reg16_arg.offset > sc->mmio_size)
	      return -EFAULT;
	    
	    *(unsigned short *)(sc->mmio + reg16_arg.offset) = reg16_arg.value;      
		  break;

    case QNFIOCR32:
      if(copy_from_user(&reg32_arg, (reg32_t *)arg, sizeof(reg32_t)))
	    {
	      error = -EFAULT;
	    }
	     
	    if (reg32_arg.offset > sc->mmio_size)
	      return -EFAULT;
	        
	    reg32_arg.value = *(unsigned int *)(sc->mmio + reg32_arg.offset);
              
      if(copy_to_user((void*)arg, &reg32_arg, sizeof(reg32_t)))
			{
			  error = -EFAULT;
		  }
		  break;

    case QNFIOCW32:
      if(copy_from_user(&reg32_arg, (reg32_t *)arg, sizeof(reg32_t)))
	    {
	      error = -EFAULT;
	    }
	     
	    if (reg32_arg.offset > sc->mmio_size)
	      return -EFAULT;
	    
	    *(unsigned int *)(sc->mmio + reg32_arg.offset) = reg32_arg.value;            
	    break;

    case QNFIOCGINFO:
	 	  info.id = sc->unit;
		  info.device_code = sc->devId;
      
		  if (copy_to_user((void *)arg, &info, sizeof(qnfinf_t)))
			  error = -EFAULT;
		  break;
		
    default:
	 	  error = -ENOTTY;
		  break;
	}
	
	return error;
}

int __devinit
qnf_init_one(struct pci_dev *dev, const struct pci_device_id *ent)
{
	char				rev;
	int					rc;
	int					unit, error;
	uint8_t			lat;
	unsigned long	iom;
	uint32_t		iom_size;
	qnf_softc_t *sc=NULL;
	uint32_t		devId;	
	
	QNF_DEBUG( (KERN_DEBUG "qnf: qnf_init_one entry\n") );
	
	unit = qnf_find_unattached();
	QNF_DEBUG( (KERN_DEBUG "qnf: discovered unit %d\n", unit) );
	
	if (unit < 0) {
		QNF_MSG( (KERN_WARNING "Too many QNF devices loaded (>%d)\n", QNF_MAX_BOARDS) );
		return -ENOMEM;
	}
	
	sc = &qnf_array[unit];
	sc->unit = unit;
	sc->flags |= QNFF_ATTACHED;
	sc->device = dev;
	pci_set_drvdata(dev, sc);
	
	rc = pci_enable_device(dev);
	if (rc) return rc;
	
	pci_set_master(dev); /* returns void */
	
	pci_read_config_dword(dev, PCI_VENDOR_ID, &devId);
	sc->devId = (uint16_t) ((devId >> 16) & 0xffff);
	QNF_DEBUG((KERN_DEBUG "qnf%d: devId is 0x%04x\n", unit, sc->devId));
	
	pci_read_config_byte(dev, PCI_LATENCY_TIMER, &lat);
	if (lat < QNF_MIN_LATENCY) {
		QNF_DEBUG( (KERN_DEBUG "qnf%d: Setting latency timer to %d\n", unit, QNF_MIN_LATENCY) );
		pci_write_config_byte(dev, PCI_LATENCY_TIMER, 64);
	}
	
	/* PCI IO memory */
	iom = pci_resource_start(dev, 0);
	iom_size = pci_resource_len(dev, 0);
	
	if(check_mem_region(iom, iom_size)) {
		QNF_MSG( (KERN_WARNING "qnf%d: Fatal - IO memory 0x%08lx already in use\n", unit, iom) );
		return -EBUSY;
	}
	
	request_mem_region(iom, iom_size, "qnf");
	QNF_DEBUG( (KERN_DEBUG "qnf%d: iom requested 0x%08lx\n", unit, iom) );
	
	sc->mmio = ioremap_nocache(iom, iom_size);
	sc->mmio_size = iom_size;
	QNF_DEBUG( (KERN_DEBUG "qnf%d: iom mmapped from 0x%08lx\n", unit, (long)sc->mmio) );
	
	// don't care irq
	qnf_pciconfig_save(sc);
	pci_read_config_byte(dev, PCI_REVISION_ID, &rev);
	
	cdev_init(&sc->cdev_qnf, &qnf_fops);
       sc->cdev_qnf.owner = THIS_MODULE;
	kobject_set_name(&sc->cdev_qnf.kobj, "qnf%d", unit);
	
	if((error=cdev_add(&sc->cdev_qnf, qnf_dev|(QNFMINOR_QNF*QNF_MAX_BOARDS+unit), 1))) {
		QNF_MSG( (KERN_WARNING "qnf%d: cdev qnf register failed (%d)\n",
		       unit, error) );		
	}
	
	return 0;
}

void __devexit
qnf_remove_one(struct pci_dev *dev)
{
	qnf_softc_t *sc = (qnf_softc_t *)pci_get_drvdata(dev);
	
	if(!sc) return;
	
	cdev_del(&sc->cdev_qnf);
	
	/* PCI IO memory */
	if(sc->mmio) {
		iounmap(sc->mmio);
		release_mem_region(pci_resource_start(dev,0), pci_resource_len(dev,0));
	}
	
	/* clear stored parameters */
	memset(sc, 0, sizeof(qnf_softc_t));
}

struct file_operations qnf_fops = {
	owner:  	THIS_MODULE,
	read:			NULL, 
	write:		NULL,
	unlocked_ioctl: qnf_ioctl,
	compat_ioctl:   NULL,
	mmap:			NULL,
	open:			qnf_open,
	release: 	qnf_release
};

static struct pci_driver qnf_pci_driver = {
  name:     "qnf",
  probe:    qnf_init_one,
  remove:	  __devexit_p(qnf_remove_one),
  id_table: qnf_pci_tbl,
};

void __exit
qnf_cleanup(void)
{
	if(qnf_dev)
		unregister_chrdev_region(qnf_dev, QNF_MAX_BOARDS*QNFMINOR_MAX);
  
	pci_unregister_driver(&qnf_pci_driver);
}

int __init
qnf_init(void)
{
	int	err=0;
	
	QNF_MSG( (KERN_INFO "qnf: Version %s\n", kqnfReleaseVersion) );
	
	if (alloc_chrdev_region( &qnf_dev, 0, QNF_MAX_BOARDS*QNFMINOR_MAX, "qnf" ) < 0) {
		QNF_MSG( (KERN_WARNING "qnf: Couldn't get major error %d\n", err) );
		goto fail_ch;
  }
	
	err = pci_register_driver(&qnf_pci_driver);
	
	if (err) {
		QNF_MSG( (KERN_WARNING "qnf: Init failure %d\n", err) );
		goto fail_pci;
	}
	
	return 0;
	
fail_ch:
	qnf_cleanup();
 	
fail_pci:
	return err;
}


MODULE_AUTHOR("Qice Tech");
MODULE_DESCRIPTION("device driver module for Qice QNF inteligent capture cards");
MODULE_SUPPORTED_DEVICE("qnf");
MODULE_LICENSE("GPL");

module_init(qnf_init);
module_exit(qnf_cleanup);

