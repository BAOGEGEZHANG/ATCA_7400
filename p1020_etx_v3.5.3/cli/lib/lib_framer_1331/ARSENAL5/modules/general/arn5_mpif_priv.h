/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_mpif_priv.h
 *
 * MPIF: This is a PRIVATE header file.
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#ifndef __ARN5_MPIF_PRIV_H__
#define __ARN5_MPIF_PRIV_H__

#include "cs_types.h"
#include "arn5_public.h"
#include "arn5_mpif_data.h"

extern
cs_status  arn5_slice_hard_reset(cs_uint8 dev_id, cs_uint8 slice_num,
                        arn5_module_id_t mid,
                        cs_reset_action_t act) ;
extern
cs_status  arn5_mpif_set_line_rate(cs_uint16 port_id,
                        arn5_port_line_rate_t line_rate) ;

/*****************************************************************
 * MPIF IRQ API
 *****************************************************************/
extern
cs_status  arn5_mpif_dev_get_default_irq_cfg(cs_uint8 dev_id,
                        arn5_dev_summ_t * pSumm,
                        arn5_mpif_cfg_t * pCfg) ;

extern
cs_status  arn5_mpif_dev_init_irq_cfg(cs_uint8 dev_id,
                        arn5_mpif_cfg_t * pCfg) ;

extern cs_status  arn5_mpif_dev_isr(cs_uint8 dev_id) ;

#endif /* __ARN5_MPIF_PRIV_H__ */

