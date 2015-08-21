/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_eth
 *
 * XETH: Public structures and exported API's for applications.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#ifndef __M2R_ETH_H__
#define __M2R_ETH_H__

/* Feature 1 */

/***********************************************************************
 * Defines for default values
 **********************************************************************/
#define M2R_XETH_FIFO_FULL_THRSHOLD_DEF         (0x10)
#define M2R_XETH_BURST_LEN_DEF                  (0x40)

/***********************************************************************
 * Default config structure for XETH block
 **********************************************************************/
typedef struct {
/* Feature 1 */
  cs_uint16             burstLen ;
  cs_uint16             fifo_high_thrshold ;

  cs_uint16             irq_ctl_tbl[8] ;
} m2r_eth_cfg_t ;

/***********************************************************************
 * Default running config structure for XETH block
 **********************************************************************/
typedef struct {
/* Feature 1 */

  cs_uint16         burst ;
  cs_uint16         fifo_thrshold ;
} m2r_eth_runcfg_t ;

/***********************************************************************
 * XETH APIs
 **********************************************************************/
extern  cs_status  m2r_eth_init_cfg(cs_uint16 port_id,
                   m2r_port_summ_t * pSumm, m2r_eth_cfg_t * pCfg) ;

extern  cs_status  m2r_eth_get_default_cfg(cs_uint16 port_id,
                   m2r_port_summ_t * pSumm, m2r_eth_cfg_t * pCfg) ;

extern  cs_status  m2r_eth_get_running_cfg(cs_uint16 port_id,
				   m2r_port_summ_t * pSumm,
				   m2r_eth_runcfg_t * pCfg) ;

extern  cs_status  m2r_eth_dump_running_cfg(cs_uint16 port_id,
				   m2r_eth_runcfg_t * pCfg) ;

/* Feature 1 */

extern  cs_status m2r_eth_set_fifo_full_threshold(cs_uint16 port_id,
                                                   cs_uint8 threshold) ;

extern  cs_status m2r_eth_set_burst_threshold(cs_uint16 port_id,
                                              cs_uint8 threshold) ;

extern  cs_status m2r_eth_set_drop_runt_pkt(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern  cs_status m2r_eth_get_clock_status(cs_uint16 port_id) ;

/* Feature 1 */

/* Feature 1 */

#endif /* __M2R_ETH_H__ */

