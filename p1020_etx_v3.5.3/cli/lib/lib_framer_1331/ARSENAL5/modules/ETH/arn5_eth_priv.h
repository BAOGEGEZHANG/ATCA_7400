/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_eth_priv.h
 *
 * ETH IRQ Tables and APIs
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#ifndef __ARN5_ETH_PRIV_H__
#define __ARN5_ETH_PRIV_H__

#include "arn5_eth_data.h"


/*****************************************************************
 * ETH Port Config
 *****************************************************************/
extern
cs_status  arn5_eth_init_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg) ;
extern
cs_status  arn5_eth_get_default_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg) ;
extern
cs_status  arn5_eth_get_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg) ;
extern
cs_status  arn5_eth_dump_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg) ;

/*****************************************************************
 * ETH IRQ API
 *****************************************************************/
extern
cs_status  arn5_eth_get_default_irq_cfg(cs_uint16 port_id,
                        arn5_port_summ_t * pSumm,
                        arn5_eth_cfg_t * pCfg) ;

extern
cs_status  arn5_eth_init_irq_cfg(cs_uint16 port_id,
                        arn5_eth_cfg_t * pCfg) ;

extern cs_status  arn5_eth_isr(cs_uint16 port_id) ;


/*****************************************************************
 * ETH STATS API
 *****************************************************************/
extern
cs_status  arn5_eth_init_port_stats(cs_uint16 port_id) ;


#endif /* __ARN5_ETH_PRIV_H__ */
