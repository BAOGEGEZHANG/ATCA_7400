/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_lif_priv.h
 *
 * LIF: This is a PRIVATE header file.
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#ifndef __ARN5_LIF_PRIV_H__
#define __ARN5_LIF_PRIV_H__

#include "cs_types.h"


/********************************************************************
 * LINE INTERFACE (LIF) Private API's
 ********************************************************************/
extern
cs_status  arn5_lif_init_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg) ;
extern
cs_status  arn5_lif_get_default_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg) ;
extern
cs_status  arn5_lif_get_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg) ;
extern
cs_status  arn5_lif_dump_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg) ;

/********************************************************************
 * LIF IRQ APIs
 ********************************************************************/
extern
cs_status  arn5_lif_get_default_irq_cfg(cs_uint16 port_id,
                        arn5_port_summ_t * pSumm,
                        arn5_lif_cfg_t * pCfg) ;
extern
cs_status  arn5_lif_init_irq_cfg(cs_uint16 port_id, arn5_lif_cfg_t * pCfg) ;

extern cs_status  arn5_lif_isr(cs_uint16 port_id) ;

#endif /* __ARN5_LIF_PRIV_H__ */


