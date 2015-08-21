/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_pprbs_priv.h
 *
 * PPRBS private prototypes.
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#ifndef __ARN5_PPRBS_PRIV_H__
#define __ARN5_PPRBS_PRIV_H__

#include "arn5_common.h"
#include "arn5_pprbs.h"

/*****************************************************************
 * PPRBS API
 *****************************************************************/
extern
cs_status arn5_pprbs_get_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pCfg) ;

extern
cs_status arn5_pprbs_dump_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pCfg) ;


/*****************************************************************
 * PPRBS IRQ API
 *****************************************************************/
extern
cs_status arn5_pprbs_get_default_irq_cfg(cs_uint16 port_id,
                        arn5_port_summ_t * pSumm,
                        arn5_pprbs_cfg_t * pCfg) ;

extern
cs_status arn5_pprbs_init_irq(cs_uint16 port_id,
                        arn5_pprbs_cfg_t * pCfg) ;

extern cs_status arn5_pprbs_isr(cs_uint16 port_id) ;

#endif /* __ARN5_PPRBS_PRIV_H__ */
