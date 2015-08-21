/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_irq.h
 * 
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PUBLIC header file.
 * It contains defines, structure and APIs for MILAN2 Interrupt handling
 */
#ifndef __M2R_IRQ_H__
#define __M2R_IRQ_H__

#include "cs_types.h"
#include "m2r_public.h"

typedef cs_irq_info_t  m2r_irq_info_t ;

#define M2R_IRQ_INFO_STATUS_FLAG        (CS_IRQ_INFO_STATUS_FLAG)
#define M2R_IRQ_INFO_PARAM1_FLAG        (CS_IRQ_INFO_PARAM1_FLAG)
#define M2R_IRQ_INFO_PARAM2_FLAG        (CS_IRQ_INFO_PARAM2_FLAG)
#define M2R_IRQ_INFO_SHRD_DEVICE_FLAG   (CS_IRQ_INFO_SHRD_DEVICE_FLAG)


#define M2R_IRQ_INFO_SET_STATUS(FLAG)   \
         (FLAG) |= M2R_IRQ_INFO_STATUS_FLAG

#define M2R_IRQ_INFO_CLR_STATUS(FLAG)   \
         (FLAG) &= ~M2R_IRQ_INFO_STATUS_FLAG

#define M2R_IRQ_INFO_SET_PARAM1(FLAG)   \
         (FLAG) |= M2R_IRQ_INFO_PARAM1_FLAG

#define M2R_IRQ_INFO_CLR_PARAM1(FLAG)   \
         (FLAG) &= ~M2R_IRQ_INFO_PARAM1_FLAG

#define M2R_IRQ_INFO_SET_PARAM2(FLAG)   \
         (FLAG) |= M2R_IRQ_INFO_PARAM2_FLAG

#define M2R_IRQ_INFO_CLR_PARAM2(FLAG)   \
         (FLAG) &= ~M2R_IRQ_INFO_PARAM2_FLAG

/***************************************************************************
 * API
 ***************************************************************************/
extern cs_status m2r_port_enable_blk_irq(cs_uint16 port_id,
                                      cs_uint32 mod_enbl_bits) ;

extern cs_status m2r_port_disable_blk_irq(cs_uint16 port_id,
                                       cs_uint32 mod_disabl_bits) ;

extern cs_uint32 m2r_port_get_blk_irq_enbl(cs_uint16 port_id) ;

extern cs_status m2r_enable_leaf_irq(cs_uint16 port_id, m2r_module_id_t mod_id,
                                cs_uint32 grpId, cs_uint16 irq_enbl_bits) ;

extern cs_status m2r_disable_leaf_irq(cs_uint16 port_id, m2r_module_id_t mod_id,
                                 cs_uint32 grpId, cs_uint16 irq_disabl_bits) ;

extern cs_status m2r_clear_leaf_irq(cs_uint16 port_id, m2r_module_id_t mod_id,
                                 cs_uint32 grpId, cs_uint16 irq_clr_bits) ;

extern cs_status m2r_isr_handler(cs_uint16 dev_id) ;

#endif /* __M2R_IRQ_H__ */

