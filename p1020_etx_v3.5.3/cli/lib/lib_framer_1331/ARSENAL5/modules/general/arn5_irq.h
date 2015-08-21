/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_irq.h
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 *
 * This is a PUBLIC header file.
 * It contains defines, structure and APIs for Interrupt handling
 */
#ifndef __ARN5_IRQ_H__
#define __ARN5_IRQ_H__

#include "cs_types.h"
#include "arn5_public.h"

/*
 * Set to 1 will enable all interrupts by default.  Setting to 0
 * will not affecting the default settings of individual leaves
 */
#define ARN5_OVRD_DEF_IRQ_TO_ENB         0


typedef cs_irq_info_t  arn5_irq_info_t ;

#define ARN5_IRQ_INFO_STATUS_FLAG        (CS_IRQ_INFO_STATUS_FLAG)
#define ARN5_IRQ_INFO_PARAM1_FLAG        (CS_IRQ_INFO_PARAM1_FLAG)
#define ARN5_IRQ_INFO_PARAM2_FLAG        (CS_IRQ_INFO_PARAM2_FLAG)
#define ARN5_IRQ_INFO_SHRD_DEVICE_FLAG   (CS_IRQ_INFO_SHRD_DEVICE_FLAG)


#define ARN5_IRQ_INFO_SET_STATUS(FLAG)   \
         (FLAG) |= ARN5_IRQ_INFO_STATUS_FLAG

#define ARN5_IRQ_INFO_CLR_STATUS(FLAG)   \
         (FLAG) &= ~ARN5_IRQ_INFO_STATUS_FLAG

#define ARN5_IRQ_INFO_SET_PARAM1(FLAG)   \
         (FLAG) |= ARN5_IRQ_INFO_PARAM1_FLAG

#define ARN5_IRQ_INFO_CLR_PARAM1(FLAG)   \
         (FLAG) &= ~ARN5_IRQ_INFO_PARAM1_FLAG

#define ARN5_IRQ_INFO_SET_PARAM2(FLAG)   \
         (FLAG) |= ARN5_IRQ_INFO_PARAM2_FLAG

#define ARN5_IRQ_INFO_CLR_PARAM2(FLAG)   \
         (FLAG) &= ~ARN5_IRQ_INFO_PARAM2_FLAG

/***************************************************************************
 * API
 ***************************************************************************/
extern
cs_status arn5_port_enable_irq(cs_uint16 port_id) ;
extern
cs_status arn5_port_disable_irq(cs_uint16 port_id) ;
extern
cs_status arn5_port_enable_blk_irq(cs_uint16 port_id,
                        cs_uint32 mod_enbl_bits) ;
extern
cs_status arn5_port_disable_blk_irq(cs_uint16 port_id,
                        cs_uint32 mod_disabl_bits) ;
extern
cs_uint32 arn5_port_get_blk_irq_enbl(cs_uint16 port_id) ;

extern
cs_status arn5_port_enable_leaf_irq(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint32 grpId, cs_uint16 irq_enbl_bits) ;
extern
cs_status arn5_port_disable_leaf_irq(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint32 grpId, cs_uint16 irq_disabl_bits) ;

extern
cs_status arn5_port_clear_leaf_irq(cs_uint16 port_id,
                        arn5_module_id_t mod_id,
                        cs_uint32 grpId, cs_uint16 irq_clr_bits) ;
extern
cs_status arn5_dev_enable_blk_irq(cs_uint8 dev_id,
                        cs_uint32 mod_enbl_bits) ;

extern
cs_status arn5_dev_disable_blk_irq(cs_uint8 dev_id,
                        cs_uint32 mod_disabl_bits) ;
extern
cs_uint32 arn5_dev_get_blk_irq_enbl(cs_uint8 dev_id) ;
extern
cs_status arn5_dev_enable_leaf_irq(cs_uint8 dev_id,
                        arn5_module_id_t mod_id,
                        cs_uint32 grpId, cs_uint16 irq_enbl_bits) ;
extern
cs_status arn5_dev_disable_leaf_irq(cs_uint8 dev_id,
                        arn5_module_id_t mod_id,
                        cs_uint32 grpId, cs_uint16 irq_disabl_bits) ;
extern
cs_status arn5_dev_clear_leaf_irq(cs_uint8 dev_id,
                        arn5_module_id_t mod_id,
                        cs_uint32 grpId, cs_uint16 irq_clr_bits) ;
extern
cs_status arn5_isr_handler(cs_uint8 dev_id) ;

#endif /* __ARN5_IRQ_H__ */

