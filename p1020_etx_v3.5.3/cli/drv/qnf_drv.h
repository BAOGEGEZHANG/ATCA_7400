/*
 * Copyright (c) 2010-2015 Qice Technology Ltd, Beijing, China.
 * All rights reserved.
 *
 * This source code is proprietary to Qice Technology Limited and no part
 * of it may be redistributed, published or disclosed except as outlined in
 * the written contract supplied with this product.
 *
 *******************************************************************************
 * File Name   		: 	qnfdrv.h
 * Description		:	header file for qnfxxx card driver
 *******************************************************************************/

#ifndef __QNFDRV_H__
#define __QNFDRV_H__


# include <linux/cdev.h>



#define QNF_MIN_LATENCY	64

// pciconfig structure
typedef struct pciconfig {
	uint32_t	    ba0;	/* base address 0 */
	uint32_t	    ba1;	/* base address 1 */
	uint32_t	    ba2;	/* base address 2 */
	uint32_t	    com;	/* command */
	uint8_t	    lin;	/* interrupt line */
	uint8_t	    lat;	/* latency timer */
} pciconfig_t;

// qnf_soft controller structure
typedef struct qnf_softc {
	int				unit;
	int				flags;
	uint16_t			devId;
	struct semaphore	lock_sem;
	struct pci_dev   	*device;
	uint8_t          		*mmio;
	uint32_t              	mmio_size;
	struct cdev          	cdev_qnf;
	pciconfig_t	      	pciconfig;
}qnf_softc_t;

typedef struct qnfinf
{
	uint32_t			id;
	uint16_t			device_code;
} qnfinf_t;


typedef enum qnfminor
{
	QNFMINORBITS  		= 3,
	QNFMINORMASK  		= (1<<QNFMINORBITS)-1,
	QNFMINOR_QNF  		= 0,
	QNFMINOR_MAX  		= 3						
} qnfminor_t;

typedef struct reg16 
{
    unsigned int    offset;
    unsigned short value;
} reg16_t;

typedef struct reg32
{
    unsigned int    offset;
    unsigned int    value;
} reg32_t;



// two IOCTRL interface between driver and api function
#define QNF_IOC_MAGIC	    	'q'
#define QNF_IOC_MAXNR	    	5
#define QNFIOCR16     		_IOR(QNF_IOC_MAGIC,  1, reg16_t)
#define QNFIOCW16     	    	_IOW(QNF_IOC_MAGIC, 2, reg16_t)
#define QNFIOCR32     		_IOR(QNF_IOC_MAGIC,  3, reg32_t)
#define QNFIOCW32     	    	_IOW(QNF_IOC_MAGIC, 4, reg32_t)
#define QNFIOCGINFO			_IOR(QNF_IOC_MAGIC, 5, qnfinf_t)


#endif /* __QNFDRV_H__ */


