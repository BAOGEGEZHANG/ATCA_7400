/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_lif.h
 *
 * LIF: Public structures and exported API's for applications.
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#ifndef __ARN5_LIF_H__
#define __ARN5_LIF_H__

#include "cs_types.h"

#include "arn5_registers.h"


/************************************************************************
 * Reference clock for the VCO.
 ************************************************************************/
typedef enum {
  ARN5_LIF_REF_CLK_622,
  ARN5_LIF_REF_CLK_311,
  ARN5_LIF_REF_CLK_155,
  ARN5_LIF_REF_CLK_78,
} arn5_lif_ref_clk_t ;

/************************************************************************
 * TX CLOCK TIMING MODES
 ************************************************************************/
typedef enum {
  ARN5_LIF_TX_CLK_LOCAL,         /* Clock derived from STX_CKREFP/N  */
  ARN5_LIF_TX_CLK_LOOP_INTERNAL, /* recovered clock from CDR circuit */
  ARN5_LIF_TX_CLK_LOOP_EXTERNAL, /* clock from STX_CKREF_LPTIMEP/N   */
} arn5_lif_tx_clk_mode_t ;

#define  ARN5_LIF_RX_CLK    ( 0x01 )     /* RX CLK FROM SERDES TO LIF */
#define  ARN5_LIF_TX_CLK    ( 0x02 )     /* TX CLK FROM SERDES TO LIF */
#define  ARN5_LIF_LD_CLK    ( 0x04 )     /* LOCK DETECT CLK */


/************************************************************************
 * RX CLOCK OUTPUT SELECTIONS
 ************************************************************************/
typedef enum {
  ARN5_LIF_RX_CLKOUT_15X_MHz,
  ARN5_LIF_RX_CLKOUT_7X_MHz
} arn5_lif_rx_clkout_sel_t ;

/************************************************************************
 * TX CLOCK OUTPUT SELECTIONS
 ************************************************************************/
typedef enum {
  ARN5_LIF_TX_CLKOUT_STX_CLK    = ARN5_STX_CLKOUT_SEL_STX_CKO_STX_CLK,
  ARN5_LIF_TX_CLKOUT_STX_PILOT  = ARN5_STX_CLKOUT_SEL_STX_CKO_STX_PILOT,
  ARN5_LIF_TX_CLKOUT_155_MHz    = ARN5_STX_CLKOUT_SEL_STX_CKO_155_52_MHz,
  ARN5_LIF_TX_CLKOUT_High_Speed = ARN5_STX_CLKOUT_SEL_STX_CKO_High_Speed
} arn5_lif_tx_clkout_sel_t ;

/************************************************************************
 * PRBS PATTERN TYPES
 ************************************************************************/
typedef enum {
  ARN5_LIF_PRBS_PATTERN_31 = ARN5_PRBS_TX_SEL_PRBS_Tx_2exp31,
  ARN5_LIF_PRBS_PATTERN_23 = ARN5_PRBS_TX_SEL_PRBS_Tx_2exp23,
  ARN5_LIF_PRBS_PATTERN_15 = ARN5_PRBS_TX_SEL_PRBS_Tx_2exp15,
  ARN5_LIF_PRBS_PATTERN_7  = ARN5_PRBS_TX_SEL_PRBS_Tx_2exp7
} arn5_lif_prbs_t ;

/************************************************************************
 * PRBS ERROR INJECTION MODES
 ************************************************************************/
typedef enum {
  ARN5_LIF_PRBS_1_BIT_ERR    = 0,
  ARN5_LIF_PRBS_BLOCK_ERR
} arn5_lif_prbs_err_t ;

/************************************************************************
 * LIF STATUS
 ************************************************************************/
typedef enum {
  LIF_PRBS_RX_EFs = 0,
  LIF_PRBS_SYNCs,
  LIF_NO_DATAs,
  LIF_IN_LOCKs,
  LIF_SRX_LOCKDETIs,
  LIF_STATUS_ALL
} arn5_lif_status_t ;


/************************************************************************
 * LIF configuration
 ************************************************************************/
typedef struct {
  arn5_lif_ref_clk_t ref_clk ;          /* reference clock         */
  arn5_lif_tx_clk_mode_t tx_clk_mode;   /* tx clock mode           */
  cs_boolean        rx_clkout_ctl;      /* RX_CLKOUT enable or disable */
  cs_uint16         rx_clkout_sel;      /* RX_CLKOUT selection     */
  cs_boolean        tx_clkout_ctl;      /* TX_CLKOUT enable or disable */
  cs_uint16         tx_clkout_sel;      /* TX_CLKOUT selection     */
  cs_boolean        line_loopback ;     /* LIF line loopback       */
  cs_boolean        terminal_loopback ;
  cs_boolean        analog_term_lb ;
  cs_uint16         irq ;
} arn5_lif_cfg_t ;

/************************************************************************
 * LIF running configuration structure
 ************************************************************************/
typedef struct {
  arn5_lif_ref_clk_t ref_clk ;           /* reference clock         */
  arn5_lif_tx_clk_mode_t tx_clk_mode;    /* TX CLK Mode             */
  cs_boolean        rx_clkout_ctl;
  cs_uint16         rx_clkout_sel;
  cs_boolean        tx_clkout_ctl;
  cs_uint16         tx_clkout_sel;
  cs_uint16         div1 ;
  cs_uint16         div2 ;
  cs_boolean        line_loopback ;     /* LIF line loopback          */
  cs_boolean        terminal_loopback ; /* terminal Loopback          */
  /* if analog terminal loopback is needed, user needs to set         */
  /* terminal_loopback to TURE as well.                               */
  /* terminal_loopback == TRUE, analog_term_lb == FALSE -> digital LB */
  /* terminal_loopback == TRUE, analog_term_lb == TRUE  -> analog LB  */
  cs_boolean        analog_term_lb ;
} arn5_lif_runcfg_t ;

/********************************************************************
 * LINE INTERFACE (LIF) APIs
 ********************************************************************/
extern
cs_status  arn5_lif_set_sreset(cs_uint16 port_id) ;
extern
cs_boolean  arn5_lif_is_sreset_done(cs_uint16 port_id) ;
extern
cs_status  arn5_lif_ctl_loop_timing(cs_uint16 port_id,
                        arn5_lif_tx_clk_mode_t tx_clk_mode) ;
extern
cs_status  arn5_lif_ctl_line_loopback(cs_uint16 port_id, cs_ctl_t ctl) ;
extern
cs_status  arn5_lif_ctl_terminal_loopback(cs_uint16 port_id, cs_ctl_t ctl) ;
extern
cs_status  arn5_lif_ctl_analog_loopback(cs_uint16 port_id, cs_ctl_t ctl) ;
extern
cs_status  arn5_lif_ctl_prbs(cs_uint16 port_id, cs_ctl_t tx_ctl,
                        cs_ctl_t rx_ctl, cs_boolean tx_invert,
                        cs_boolean rx_invert, arn5_lif_prbs_t tx_ptrn,
                        arn5_lif_prbs_t rx_ptrn) ;
extern
cs_status  arn5_lif_ctl_rx_clkout(cs_uint16 port_id, cs_ctl_t ctl);
extern
cs_status  arn5_lif_sel_rx_clkout_common(cs_uint16 port_id,
                        arn5_lif_rx_clkout_sel_t sel);
extern
cs_status  arn5_lif_sel_rx_clkout_raw(cs_uint16 port_id,
                        cs_uint16 sel);
extern
cs_status  arn5_lif_ctl_tx_clkout(cs_uint16 port_id, cs_ctl_t ctl);
extern
cs_status  arn5_lif_sel_tx_clkout(cs_uint16 port_id,
                        arn5_lif_tx_clkout_sel_t sel);
extern
cs_status  arn5_lif_inj_error(cs_uint16 port_id, arn5_lif_prbs_err_t err,
                        cs_boolean once, cs_uint16 duration) ;
extern
cs_uint32  arn5_lif_get_prbs_errors(cs_uint16 port_id) ;
extern
cs_uint32  arn5_lif_get_status(cs_uint16 port_id, arn5_lif_status_t sel) ;

#endif /* __ARN5_LIF_H__ */


