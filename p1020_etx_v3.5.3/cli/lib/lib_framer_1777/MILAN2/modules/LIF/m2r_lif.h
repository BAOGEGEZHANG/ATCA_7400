/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_lif.h
 *
 * LIF: Public structures and exported API's for applications.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#ifndef __M2_LIF_H__
#define __M2_LIF_H__

#include "cs_types.h"
#include "m2r_lif_xgxs.h"
#include "m2r_lif_pcs6.h"


/************************************************************************
 * LIF MODE
 ************************************************************************/
typedef enum {
  M2R_LIF_INTERFACE_SFI41 = 0,
  M2R_LIF_INTERFACE_SFI42,
  M2R_LIF_INTERFACE_XFP
} m2r_lif_sel_t ;

/************************************************************************
 * Reference Clock
 ************************************************************************/
typedef enum {
  M2R_LIF_REF_CLK_78,  /*  77.76  MHz */
  M2R_LIF_REF_CLK_155, /*  155.53 MHz */
  M2R_LIF_REF_CLK_311, /*  311.04 MHz */
  M2R_LIF_REF_CLK_622, /*  622.08 MHz */

  M2R_LIF_REF_CLK_81  = M2R_LIF_REF_CLK_78,  /*  80.57  MHz */
  M2R_LIF_REF_CLK_156 = M2R_LIF_REF_CLK_155, /*  156.25 MHz */
  M2R_LIF_REF_CLK_312 = M2R_LIF_REF_CLK_311, /*  312.5  MHz */
  M2R_LIF_REF_CLK_625 = M2R_LIF_REF_CLK_622, /*  625    MHz */

  M2R_LIF_REF_CLK_644 = M2R_LIF_REF_CLK_622, /* 644.53 MHz  */
  M2R_LIF_REF_CLK_322 = M2R_LIF_REF_CLK_311, /* 322.26 MHz  */
  M2R_LIF_REF_CLK_161 = M2R_LIF_REF_CLK_155  /* 161.13 MHz  */
} m2r_lif_ref_clk_t ;

/************************************************************************
 * Default Reference Clock value for various Modes of LIF
 ************************************************************************/
#define M2R_LIF_SFI4_2_REF_CLK_DEF    (M2R_LIF_REF_CLK_155)  /* SFI4.2 */

#define M2R_LIF_XFP_LCL_TIME_REF_CLK  (M2R_LIF_REF_CLK_622)
#define M2R_LIF_XFP_LP_TIME_REF_CLK   (M2R_LIF_REF_CLK_155)
#define M2R_LIF_XFP_REF_LPCLK_DEF     (M2R_LIF_REF_CLK_622)

#define M2R_LIF_XFP_REF_CLK_OUT       (M2R_LIF_REF_CLK_155)

/************************************************************************
 * RX CLOCK OUTPUT SELECTIONS
 ************************************************************************/
typedef enum {
  M2R_LIF_RX_CLKOUT_15X_MHz = RX_CLKOUT_SEL_SEL_155_52,
  M2R_LIF_RX_CLKOUT_7X_MHz = RX_CLKOUT_SEL_SEL_77_76
} m2r_lif_rx_clkout_sel_t ;

/************************************************************************
 * TX CLOCK OUTPUT SELECTIONS
 ************************************************************************/
typedef enum {
  M2R_LIF_TX_CLKOUT_STX_CLK = TX_CLKOUT_SEL_USE_STX_CLK,
  M2R_LIF_TX_CLKOUT_STX_REF = TX_CLKOUT_SEL_USE_STX_REF,
  M2R_LIF_TX_CLKOUT_155_MHz = TX_CLKOUT_SEL_USE_155_CLK,
  M2R_LIF_TX_CLKOUT_10G     = TX_CLKOUT_SEL_USE_10G_CLK
} m2r_lif_tx_clkout_sel_t ;

/************************************************************************
 * TX Clock Mode
 ************************************************************************/
typedef enum {
  M2R_LIF_TX_CLK_LOCAL,          /* Clock derived from STX_CKREFP/N  */
  M2R_LIF_TX_CLK_LOOP_INTERNAL,  /* recovered clock from CDR circuit */
  M2R_LIF_TX_CLK_LOOP_EXTERNAL   /* clock from STX_CKREF_LPTIMEP/N   */
} m2r_lif_tx_clk_mode_t ;

/************************************************************************
 * SFI4.2 control options
 ************************************************************************/
typedef enum {
  LIF_SFI42_CTRL_PN_INV = 0,
  LIF_SFI42_CTRL_RX_DSCR_EN,
  LIF_SFI42_CTRL_LANE_SWAP,
  LIF_SFI42_CTRL_BIT_SWAP,
  LIF_SFI42_CTRL_TX_SCR_EN,
  LIF_SFI42_CTRL_PBRS_CNT_LAT,
  LIF_SFI42_CTRL_PBRS_EN
} m2r_lif_sfi42_ctrl_t ;

/************************************************************************
 * XAUI/SFI4.2 LANE
 ************************************************************************/
typedef enum {
 M2R_LIF_SFI42_LANE0     = 0x01,
 M2R_LIF_SFI42_LANE1     = 0x02,
 M2R_LIF_SFI42_LANE2     = 0x04,
 M2R_LIF_SFI42_LANE3     = 0x08
} m2r_lif_sfi42_lane_t ;

/************************************************************************
 * PRBS PATTERN MODES FOR SONET PRBS
 ************************************************************************/
typedef enum {
  M2R_LIF_PRBS_PATTERN_31 = RXPRBS_MODE_PRBS_31,
  M2R_LIF_PRBS_PATTERN_23 = RXPRBS_MODE_PRBS_23,
  M2R_LIF_PRBS_PATTERN_15 = RXPRBS_MODE_PRBS_15,
  M2R_LIF_PRBS_PATTERN_7  = RXPRBS_MODE_PRBS_7
} m2r_lif_sonet_prbs_t ;

/************************************************************************
 * PRBS ERROR INJECTION MODES
 ************************************************************************/
typedef enum {
  M2R_LIF_PRBS_1_BIT_ERR    = 0,    /* Single Bit Error injection   */
  M2R_LIF_PRBS_BLOCK_ERR            /* Block Error injection        */
} m2r_lif_prbs_err_t ;

/************************************************************************
 * LIF default configuration structure
 ************************************************************************/
typedef struct {
  m2r_lif_sel_t     interface_sel ;     /* SFI4.1 or SFI4.2 or XFP  */
  m2r_lif_ref_clk_t tx_ckref ;          /* TX REF CLK               */
  m2r_lif_ref_clk_t tx_ckref_lptime ;   /* TX REF LPTIME CLK        */
  m2r_lif_tx_clk_mode_t tx_clk_mode ;   /* TX CLK Mode              */
  cs_boolean        rx_clkout_ctl ;     /* RX_CLKOUT enable or disable */
  cs_boolean        rx_clkout_invert ;  /* RX_CLKOUT inverted       */
  cs_uint16         rx_clkout_sel ;     /* RX_CLKOUT selection      */
  cs_boolean        tx_clkout_ctl ;     /* TX_CLKOUT enable or disable */
  m2r_lif_tx_clkout_sel_t tx_clkout_sel;/* TX_CLKOUT selection      */
  cs_boolean        line_loopback ;     /* LIF line loopback        */
  cs_boolean        terminal_loopback ; /* terminal Loopback enabled*/
  cs_boolean        analog_term_lb ;    /* Analog loopback required */
  cs_ctl_t	    clk_bkp_los ;	/* Ctl of bkplane clk on LOS */
  cs_ctl_t	    clk_bkp_lof ;       /* Ctl of bkplane clk on LOF */
  cs_ctl_t	    clk_bkp_ais ;       /* Ctl of bkplane clk on AIS */
  cs_boolean        frclk_is_external ; /* external FRCLK present ? */
  cs_uint8          tx_level ;          /* Transmit driver level for XFP/SFP only */
  cs_uint16         irq_ctl_tbl[16] ;   /* Default IRQ   */
} m2r_lif_cfg_t ;

/************************************************************************
 * LIF running configuration structure
 ************************************************************************/
typedef struct {
  cs_boolean        xgxs_alignment_detected ;  /*do XGXS status only for now*/
  cs_boolean        sonet_mode ;
  m2r_lif_sel_t     interface_sel ;     /* SFI4.1 or SFI4.2 or XFP  */
  m2r_lif_tx_clk_mode_t tx_clk_mode ;   /* TX CLK Mode              */
  cs_boolean        rx_clkout_ctl;      /* RX_CLKOUT enable or disable */
  cs_boolean        rx_clkout_invert;   /* RX_CLKOUT inverted       */
  cs_uint16         rx_clkout_sel;      /* RX_CLKOUT selection      */
  cs_boolean        tx_clkout_ctl;      /* TX_CLKOUT enable or disable */
  cs_uint16         tx_clkout_sel;      /* TX_CLKOUT selection      */
  cs_uint16         div1 ;
  cs_uint16         div2 ;
  cs_boolean        line_loopback ;     /* LIF line loopback        */
  cs_boolean        terminal_loopback ; /* terminal Loopback enabled*/
  cs_boolean        analog_term_lb ;    /* Analog loopback required */
} m2r_lif_runcfg_t ;


/************************************************************************
 * LIF API
 ************************************************************************/
extern cs_status  m2r_lif_init_cfg(cs_uint16 port_id,
                                   m2r_port_summ_t * pSumm,
                                   m2r_lif_cfg_t * pCfg) ;

extern cs_status  m2r_lif_get_default_cfg(cs_uint16 port_id,
                                          m2r_port_summ_t * pSumm,
                                          m2r_lif_cfg_t * pCfg) ;

extern cs_status  m2r_lif_get_running_cfg(cs_uint16 port_id,
                                          m2r_port_summ_t * pSumm,
                                          m2r_lif_runcfg_t * pCfg) ;

extern cs_status  m2r_lif_dump_running_cfg(cs_uint16 port_id,
                                           m2r_lif_runcfg_t * pCfg) ;

extern cs_status  m2r_lif_set_sreset(cs_uint16 port_id) ;

extern cs_boolean  m2r_lif_is_sreset_done(cs_uint16 port_id) ;

extern cs_status  m2r_lif_select_interface(cs_uint16 port_id,
                                           m2r_lif_sel_t sel) ;

extern cs_status  m2r_lif_ctl_power(cs_uint16 port_id,
                                    m2r_lif_sel_t sel,
                                    cs_uint8 ctrl) ;

extern  cs_status  m2r_lif_set_clk_div(cs_uint16 port_id,
                                       m2r_lif_tx_clk_mode_t tx_clk_mode) ;

extern cs_status  m2r_lif_xfp_set_clk_mode(cs_uint16 port_id,
                                          m2r_lif_tx_clk_mode_t clkMode) ;

extern cs_status  m2r_lif_ctl_line_loopback(cs_uint16 port_id,
                                            m2r_ctl_t ctl) ;

extern cs_status  m2r_lif_ctl_terminal_loopback(cs_uint16 port_id,
                                                m2r_ctl_t ctl) ;

extern cs_status  m2r_lif_ctl_analog_loopback(cs_uint16 port_id,
                                              m2r_ctl_t ctl) ;

extern cs_status m2r_lif_ctl_sonet_prbs(cs_uint16 port_id,
                                        m2r_ctl_t ctl,
                                        m2r_lif_sonet_prbs_t ptrn) ;

extern  cs_status m2r_lif_ctl_prbs(cs_uint16 port_id,
                                   m2r_ctl_t ctl,
                                   cs_uint32 prbs_pattern) ;

extern cs_status m2r_lif_sonet_inj_error(cs_uint16 port_id,
                                         m2r_lif_prbs_err_t err,
                                         cs_boolean once,
                                         cs_uint16 duration) ;

extern cs_status m2r_lif_inj_error(cs_uint16 port_id,
                                   m2r_lif_prbs_err_t err,
                                   cs_boolean once,
                                   cs_uint16 duration) ;

extern cs_uint32 m2r_lif_get_prbs_errors(cs_uint16 port_id) ;

extern cs_boolean m2r_lif_check_clk_status(cs_uint16 port_id) ;

extern cs_status m2r_lif_sfi42_ctl(cs_uint16 port_id,
                                   m2r_dir_t direction,
                                   m2r_lif_sfi42_ctrl_t sel,
                                   m2r_ctl_t ctl);

extern cs_status m2r_lif_ctl_sfi42_prbs(cs_uint16 port_id,
                                         m2r_ctl_t ctl) ;

extern cs_status m2r_lif_sfi42_set_in_sync_threshold(cs_uint16 port_id,
                                              cs_uint8  thrshold) ;

extern cs_status m2r_lif_sfi42_set_oos_threshold(cs_uint16 port_id,
                                          cs_uint8  thrshold) ;

extern m2r_port_lif_mode_t m2r_lif_get_lif_mode(cs_uint16 port_id) ;

extern cs_boolean m2r_lif_xfp_is_locked(cs_uint16 port_id) ;

extern cs_boolean m2r_lif_xfp_is_data_detected(cs_uint16 port_id) ;

extern cs_status m2r_lif_xfp_use_data_detect(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern cs_status m2r_lif_xfp_use_clk_ref(cs_uint16 port_id,
			                 m2r_lif_ref_clk_t tx_ckref,
			                 m2r_lif_ref_clk_t tx_ckref_lptime) ;

extern cs_status m2r_lif_ctl_rx_clkout(cs_uint16 port_id, m2r_ctl_t ctl,
				       cs_boolean inverted) ;

extern cs_status m2r_lif_sel_rx_clkout_common(cs_uint16 port_id,
					      m2r_lif_rx_clkout_sel_t sel) ;

extern cs_status m2r_lif_sel_rx_clkout_raw(cs_uint16 port_id,
                                           cs_uint16 sel) ;

extern cs_status m2r_lif_ctl_tx_clkout(cs_uint16 port_id, m2r_ctl_t ctl) ;
extern cs_status m2r_lif_sel_tx_clkout(cs_uint16 port_id,
                                       m2r_lif_tx_clkout_sel_t sel);
extern
cs_status m2r_lif_ctl_clk_to_backplane(cs_uint16 port_id,
				       cs_ctl_t los,
				       cs_ctl_t lof,
				       cs_ctl_t ais) ;

#endif /* __M2_LIF_H__ */




