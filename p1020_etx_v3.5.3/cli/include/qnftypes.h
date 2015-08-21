/*
 * Copyright (c) 2010-2015 Qice Technology Ltd, Beijing, China.
 * All rights reserved.
 *
 * This source code is proprietary to Qice Technology Limited and no part
 * of it may be redistributed, published or disclosed except as outlined in
 * the written contract supplied with this product.
 *
 *******************************************************************************
 * File Name   	: 	qnftype.h
 * Description	:	header file for qnfxxx card structure types
 *******************************************************************************/

#ifndef __QNFTYPE_H__
#define __QNFTYPE_H__

#include <inttypes.h>
#include <time.h>
#include <stdlib.h>



// same as the one defined in qnf_drv.h
typedef enum framerminor
{
	FRAMERMINORBITS  	= 3,
	FRAMERMINORMASK  	= (1<<FRAMERMINORBITS)-1,
	FRAMERMINOR_QNF  	= 0,
	FRAMERMINOR_MAX  	= 3
} framerminor_t;

typedef enum qnfminor
{
	QNFMINORBITS  		= 3,
	QNFMINORMASK  		= (1<<QNFMINORBITS)-1,
	QNFMINOR_QNF  		= 0,
	QNFMINOR_MAX  		= 3						
} qnfminor_t;

typedef struct qnfinf
{
	uint32_t		id;
	uint16_t		device_code;
} qnfinf_t;

typedef struct sheep 
{
	char			qnfname[32];   		// QNF card device name
	qnfinf_t          	qnfinf;
	uint32_t		opened:1;
} sheep_t;

typedef struct rule_tuple{
	uint8_t proto;
	uint32_t sip;
	uint32_t dip;
	uint16_t sport;
	uint16_t dport;
	uint16_t tcpflag;
	uint16_t pktlen;
} rule_tuple_t;
 
/* mask 0 means active
   mask 1 means don't care */
typedef struct qnf_rule_tuple{	
	rule_tuple_t	data;
	rule_tuple_t	mask;
} qnf_rule_tuple_t;

/* mask 0 means active
   mask 1 means don't care */
typedef struct qnf_rule_payload{
	unsigned char data[20];
	unsigned char mask[20];
} qnf_rule_payload_t;

typedef struct qnf_action {
	unsigned short cmd;
	unsigned short outch;
} qnf_action_t;


// flash update
typedef volatile union status_reg{
	struct {
#ifdef QNF_BIG_ENDIAN	
		uint16_t sr15	:1;	//bit15
		uint16_t sr14	:1;
		uint16_t sr13	:1;
		uint16_t sr12	:1;
		uint16_t sr11	:1;
		uint16_t sr10	:1;
		uint16_t sr9	:1;
		uint16_t sr8	:1;
		uint16_t sr7	:1;
		uint16_t sr6	:1;
		uint16_t sr5	:1;
		uint16_t sr4	:1;
		uint16_t sr3	:1;
		uint16_t sr2	:1;
		uint16_t sr1	:1;
		uint16_t sr0	:1;	//bit0
#else
		uint16_t sr0	:1;	//bit0
		uint16_t sr1	:1;
		uint16_t sr2	:1;
		uint16_t sr3	:1;
		uint16_t sr4	:1;
		uint16_t sr5	:1;
		uint16_t sr6	:1;
		uint16_t sr7	:1;
		uint16_t sr8	:1;
		uint16_t sr9	:1;
		uint16_t sr10	:1;
		uint16_t sr11	:1;
		uint16_t sr12	:1;
		uint16_t sr13	:1;
		uint16_t sr14	:1;
		uint16_t sr15	:1;	//bit16
#endif
		}SR_t;
	uint16_t value;
} status_reg_t ;


typedef enum{
	UNLOCKED = 0X0000,
	LOCKED = 0X0001,
	UNLOCKED_AND_LOCKEDDOWN = 0X0002,
	LOCKED_AND_LOCKEDDOWN = 0X0003
} lock_stat_t;


typedef struct{
    unsigned int line_rate;
    unsigned int frame;
    unsigned int encap;
    unsigned int framer_scramble;
    unsigned int mapper_scramble;
    unsigned int fcs;
    unsigned int    pass_crc;
    unsigned int    max_pkt_size;
    unsigned int min_pkt_size;
}conf_t;


typedef struct{
    conf_t configs[2];
}confs_t;

typedef struct {
    conf_t cfg[4];
} configs_t;


// IOCTRL interface
typedef struct reg16 
{
    unsigned int   offset;
    unsigned short value;
} reg16_t;

typedef struct reg32
{
    unsigned int   offset;
    unsigned int   value;
} reg32_t;


#define FRAMER_IOC_MAGIC	'f'
#define FRAMER_IOC_MAXNR	3
#define FRAMERIOCR32     		_IOR(FRAMER_IOC_MAGIC, 1, reg32_t)
#define FRAMERIOCW32     	_IOW(FRAMER_IOC_MAGIC, 2, reg32_t)
#define FRAMERIOCDUMP     	_IOW(FRAMER_IOC_MAGIC, 3, reg32_t)


#define QNF_IOC_MAGIC	    	'q'
#define QNF_IOC_MAXNR	    	5
#define QNFIOCR16     		_IOR(QNF_IOC_MAGIC,  1, reg16_t)
#define QNFIOCW16     	    	_IOW(QNF_IOC_MAGIC, 2, reg16_t)
#define QNFIOCR32     		_IOR(QNF_IOC_MAGIC,  3, reg32_t)
#define QNFIOCW32     	    	_IOW(QNF_IOC_MAGIC, 4, reg32_t)
#define QNFIOCGINFO              _IOR(QNF_IOC_MAGIC, 5, qnfinf_t)


#endif /* __QNFTYPE_H__ */

