/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_lif
 *
 * Milan driver libraries pertaining to Line Interface(LIF) block
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */

#include "cs_types.h"

#include "m2r_error.h"
#include "m2r_public.h"
#include "m2r_common.h"
#include "m2r_registers.h"
#include "m2r_lif.h"
#include "m2r_lif_pcs6.h"
#include "m2r_lif_xgxs.h"
#include "m2r_lif_irq.h"
#include "m2r_framer_priv.h"
#include "m2r_framer.h"

/* do not use this flag. Cortina internal use only */
static  cs_boolean  xfp_ref_clk_is_155 = FALSE ;

/************************************************************************/
/*    Macros and defines                                                */
/************************************************************************/
#define M2R_LIF_GET_DEV_BASEADDR(PORTID, BASE_ADDR, RETCODE)      \
{                                                                 \
  cs_uint32 error;                                                \
  if(M2R_IS_PORT_VALID(PORTID, error) != TRUE) {                  \
    CS_HNDL_ERROR((PORTID), error, ": port_id = %d\n", (PORTID)); \
    return (RETCODE) ;                                            \
  }                                                               \
  (BASE_ADDR) = M2R_PORT_ID_TO_DEV_BASE((PORTID));                \
}

/****************************************************************
 * Internal LIF functions declarations
 ****************************************************************/
static cs_uint8 m2r_lif_calc_xfp_clk_div(m2r_lif_ref_clk_t refClk) ;

static cs_status m2r_lif_set_xfp_clk_div(cs_uint16 port_id, M2R_t * pDev,
                                  m2r_lif_tx_clk_mode_t tx_clk_mode) ;

static cs_status m2r_lif_set_sfi42_clk_div(cs_uint16 port_id, M2R_t * pDev,
                                     m2r_lif_tx_clk_mode_t tx_clk_mode) ;

static cs_status m2r_lif_xfp_analog_init(cs_uint16 port_id, M2R_t * pDev) ;

/************************************************************************/
/* $rtn_hdr_start    CONFIGURE  LINE INTERFACE(LIF)                     */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_init_cfg(cs_uint16 port_id, m2r_port_summ_t * pSumm,
                            m2r_lif_cfg_t * pCfg)
/* INPUTS     : o Port Id                                               */
/*              o Pointer to port configuration summary struct          */
/*              o Pointer to LIF configuration data structure           */
/*                                                                      */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Configures the Line Interface(LIF) as per the parameters in the      */
/* configuration structure(pCfg is the pointer to the structure).       */
/*                                                                      */
/* Here are the needed LIF configuration changes from driver's defaults */
/* for the various options mentioned in the MILAN2 general spec.        */
/*                                                                      */
/* XFI Clock Distribution, Option 1:                                    */
/*   pCfg->lif.tx_ckref = M2R_LIF_REF_CLK_155;                          */
/*   pCfg->lif.tx_ckref_lptime = M2R_LIF_REF_CLK_622;                   */
/*   pCfg->lif.tx_clk_mode = M2R_LIF_TX_CLK_LOOP_EXTERNAL;              */
/*   pCfg->lif.rx_clkout_sel = M2R_LIF_RX_CLKOUT_7X_MHz;                */
/*   pCfg->lif.rx_clkout_ctl = TRUE;                                    */
/*                                                                      */
/* XFI Clock Distribution, Option 2:                                    */
/*   pCfg->lif.rx_clkout_sel = M2R_LIF_RX_CLKOUT_7X_MHz;                */
/*   pCfg->lif.rx_clkout_ctl = TRUE;                                    */
/*   pCfg->lif.tx_clkout_sel = M2R_LIF_TX_CLKOUT_155_MHz;               */
/*   pCfg->lif.tx_clkout_ctl = TRUE;                                    */
/*                                                                      */
/* XFI Clock Distribution, Option 3 (10GE mode only):                   */
/*   pCfg->lif.tx_ckref = M2R_LIF_REF_CLK_161;                          */
/*                                                                      */
/* XFI/SFI OC-48 Clock Distribution, Option 1:                          */
/*   pCfg->lif.tx_ckref = M2R_LIF_REF_CLK_155;                          */
/*   pCfg->lif.tx_ckref_lptime = M2R_LIF_REF_CLK_622;                   */
/*   pCfg->lif.tx_clk_mode = M2R_LIF_TX_CLK_LOOP_EXTERNAL;              */
/*   pCfg->lif.rx_clkout_sel = M2R_LIF_RX_CLKOUT_7X_MHz;                */
/*   pCfg->lif.rx_clkout_ctl = TRUE;                                    */
/*                                                                      */
/* XFI/SFI OC-48 Clock Distribution, Option 2:                          */
/*   pCfg->lif.rx_clkout_sel = M2R_LIF_RX_CLKOUT_7X_MHz;                */
/*   pCfg->lif.rx_clkout_ctl = TRUE;                                    */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32     usec_delay = 100, usec_timeout = 1000 ;
  cs_uint32     usec_elapsed = 0 ;
  cs_boolean    sonet_mode ;
  cs_uint32     error;

  if ((pCfg == NULL) || (pSumm == NULL)) {
    CS_HNDL_ERROR(port_id,
                  ((!pSumm) ? EM2R_PORT_SUMM_NULL : EM2R_PORT_CFG_NULL),
                  NULL) ;
    return(CS_ERROR) ;
  }

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ": port_id = %d\n", port_id);
    return (CS_ERROR) ;
  }

  //CS_PRINT("\t Initializing LIF... \n") ;

  /* software workaround in case of some registers are not start*/
  /* with defaults.                                             */
  m2r_lif_setup_default(port_id, pSumm);

  /* save some config valuse to pcb for later use               */
  /* do this first, since the rest of API might reference these */
  /* implicitely.                                               */
  {
    m2r_port_cb_t * ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;
    if (!ppcb->tx_ckref_custom) {
      ppcb->tx_ckref = pCfg->tx_ckref;
      ppcb->tx_ckref_lptime = pCfg->tx_ckref_lptime;
    }
  }

  sonet_mode = ((pSumm->line_rate == M2R_PORT_RATE_10G) ? FALSE : TRUE ) ;

  if( m2r_lif_select_interface(port_id, pCfg->interface_sel) == CS_ERROR ) {
      return ( CS_ERROR ) ;
  }

  if (pCfg->interface_sel == M2R_LIF_INTERFACE_SFI42 ) {
    m2r_lif_set_clk_div(port_id, pCfg->tx_clk_mode) ;
  }
  else if (pCfg->interface_sel == M2R_PORT_IF_XFP) {
    M2R_t  * pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
    /* set tx_level directly */
    LIF_TXADJUST3_t txAdj3 ;
    M2R_PORT_LOCK(port_id, M2R_ID_LIF);
    txAdj3.wrd = M2R_REG_READ(&pDev->LIF.TXADJUST3.wrd) ;
    txAdj3.bf.TX_LEVEL = pCfg->tx_level ;
    M2R_REG_WRITE(&pDev->LIF.TXADJUST3.wrd, txAdj3.wrd) ;
    M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);

    m2r_lif_xfp_set_clk_mode(port_id, pCfg->tx_clk_mode) ;
    m2r_lif_sel_rx_clkout_raw(port_id, pCfg->rx_clkout_sel) ;
    m2r_lif_ctl_rx_clkout(port_id, pCfg->rx_clkout_ctl,
			  pCfg->rx_clkout_invert ) ;
    m2r_lif_sel_tx_clkout(port_id, pCfg->tx_clkout_sel) ;
    m2r_lif_ctl_tx_clkout(port_id, pCfg->tx_clkout_ctl) ;
  }

  if (pCfg->terminal_loopback) {
    if (pCfg->analog_term_lb) {
      m2r_lif_ctl_analog_loopback(port_id, M2R_ENABLE);
    }
    else {
      m2r_lif_ctl_terminal_loopback(port_id, M2R_ENABLE);
    }
  }
  else
  if (pCfg->line_loopback) {
    m2r_lif_ctl_line_loopback(port_id, M2R_ENABLE) ;
  }

  if ( m2r_lif_ctl_clk_to_backplane(port_id, pCfg->clk_bkp_los,
				    pCfg->clk_bkp_lof,
				    pCfg->clk_bkp_ais)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* Soft-reset LIF before starting traffic */
  m2r_lif_set_sreset(port_id) ;

  while ( !(m2r_lif_is_sreset_done(port_id)) ) {
    if (usec_elapsed >= usec_timeout) break ;
    CS_UDELAY(usec_delay) ;
    usec_elapsed += usec_delay ;
  }

  /* Init Stats */
  if ( m2r_lif_init_stats(port_id, pCfg->interface_sel, sonet_mode)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* Initialise LIF IRQ Tables and enable default IRQs */
  if ( m2r_lif_init_irq(port_id, pCfg) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return(CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start    GET DEFAULT LIF CONFIGURATION                      */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_get_default_cfg(cs_uint16 port_id,
				   m2r_port_summ_t * pSumm,
				   m2r_lif_cfg_t * pCfg)
/* INPUTS     : o Port Id                                               */
/*              o Pointer to port config summary info                   */
/*              o Pointer to configuration data structure               */
/* OUTPUTS    : Driver default LIF configuration settings               */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Get the driver's default LIF configuration for the                   */
/* specified port and for the specified summary config info.            */
/* The caller is responsible for allocating memory for the              */
/* configuration structure, and the driver will populate it to          */
/* provide a default configuration. The caller can then update          */
/* the configuration parameters as desired and use it to                */
/* initialize the LIF.                                                  */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_boolean  sonet_mode ;

  if ((pCfg == NULL) || (pSumm == NULL)) {
    CS_HNDL_ERROR(port_id,
                  ((!pSumm) ? EM2R_PORT_SUMM_NULL : EM2R_PORT_CFG_NULL),
                  NULL) ;
    return(CS_ERROR) ;
  }

  pCfg->line_loopback = FALSE ;
  pCfg->terminal_loopback = FALSE ;
  pCfg->analog_term_lb = FALSE ;
  /* pCfg->tx_clk_mode = M2R_LIF_TX_CLK_LOOP_EXTERNAL ; */
  pCfg->tx_clk_mode = M2R_LIF_TX_CLK_LOCAL;

  /* default - use 622MHz  */
  pCfg->tx_ckref = M2R_LIF_REF_CLK_155 ;
  pCfg->tx_ckref_lptime = M2R_LIF_REF_CLK_622 ;

  sonet_mode = ((pSumm->line_rate == M2R_PORT_RATE_10G) ? FALSE : TRUE ) ;
  switch ( pSumm->lif_mode ) {
    case M2R_PORT_IF_SFI4_1 :
       pCfg->tx_ckref = M2R_LIF_REF_CLK_622 ;
       pCfg->tx_ckref_lptime = pCfg->tx_ckref ;
       pCfg->interface_sel = M2R_LIF_INTERFACE_SFI41 ;
    break ;

    case M2R_PORT_IF_SFI4_2 :
      pCfg->tx_ckref = M2R_LIF_SFI4_2_REF_CLK_DEF ;
      pCfg->tx_ckref_lptime = pCfg->tx_ckref ;
      pCfg->interface_sel = M2R_LIF_INTERFACE_SFI42 ;
    break ;

    case M2R_PORT_IF_XFP :
     pCfg->tx_ckref = M2R_LIF_REF_CLK_155 ;
     pCfg->tx_ckref_lptime = M2R_LIF_REF_CLK_622 ;
     pCfg->interface_sel = M2R_LIF_INTERFACE_XFP ;
    break ;
  }

  pCfg->rx_clkout_ctl = FALSE;
  pCfg->rx_clkout_invert = TRUE;
  pCfg->rx_clkout_sel = 0x1e;
  pCfg->tx_clkout_sel = M2R_LIF_TX_CLKOUT_STX_REF;
  pCfg->tx_clkout_ctl = FALSE;

  pCfg->clk_bkp_los = CS_DISABLE ;
  pCfg->clk_bkp_lof = CS_DISABLE ;
  pCfg->clk_bkp_ais = CS_DISABLE ;
  pCfg->frclk_is_external = TRUE;

  if ((pSumm->line_rate == M2R_PORT_RATE_OC48C) ||
      (pSumm->line_rate == M2R_PORT_RATE_OC48)) {
    pCfg->tx_level = 0x16 ;
  }
  else {
    pCfg->tx_level = 0x10 ;
  }

  m2r_lif_get_default_irq_cfg(port_id, pSumm, pCfg) ;

  return(CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start    GET RUNNING LIF CONFIGURATION                      */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_get_running_cfg(cs_uint16 port_id,
				   m2r_port_summ_t * pSumm,
				   m2r_lif_runcfg_t * pCfg)
/* INPUTS     : o Port Id                                               */
/*              o Pointer to port config summary info                   */
/*              o Pointer to configuration data structure               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Returns the current run-time LIF hardware configuration              */
/* of the specified port.                                               */
/*                                                                      */
/* The [pRunSumm] parameter is a pointer to the running config          */
/* summary section.                                                     */
/*                                                                      */
/* The [pRunCfg] parameter is a pointer to the block's                  */
/* configuration data-structure.                                        */
/*                                                                      */
/* This API will fill in the all the fields in the block                */
/* run-time configuration datastructure.                                */
/* The caller is responsible for allocating memory for the              */
/* configuration structures.                                            */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  LIF_INTERFACE_t      interf ;
  LIF_LBSWAP_t         lbswap ;
  M2R_t              * pDev ;

  if ((pCfg == NULL) || (pSumm == NULL)) {
    CS_HNDL_ERROR(port_id,
                  ((!pSumm) ? EM2R_PORT_SUMM_NULL : EM2R_PORT_CFG_NULL),
                  NULL) ;
    return(CS_ERROR) ;
  }

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  pCfg->sonet_mode = M2R_GET_SONET_MODE_FROM_HW(port_id) ;

  lbswap.wrd = M2R_REG_READ(&pDev->LIF.LBSWAP.wrd) ;
  interf.wrd = M2R_REG_READ(&pDev->LIF.INTERF.wrd) ;

  switch (interf.bf.INTERFACE_SEL) {
    case INTERFACE_SEL_SFI41_XSBI : {
      pCfg->interface_sel = M2R_LIF_INTERFACE_SFI41 ;
      pCfg->tx_clk_mode = M2R_LIF_TX_CLK_LOCAL ; /* for completeness */
      pCfg->line_loopback = lbswap.bf.RX2TX_LB ;
      pCfg->terminal_loopback = lbswap.bf.TX2RX_LB ;
      pCfg->analog_term_lb = 0 ; /* for completeness */
    }
    break ;

    case INTERFACE_SEL_SFI42_XAUI : {
      XGXS_MP_XAUI0_t       xauiCtrl ;
      LIF_SFI42CNTL_t       sfi4_2_ctl;
      LIF_GPCTRL_t          gpctrl ;

      pCfg->interface_sel = M2R_LIF_INTERFACE_SFI42 ;
      pCfg->tx_clk_mode = M2R_LIF_TX_CLK_LOCAL ;

      if (pCfg->sonet_mode) {
        sfi4_2_ctl.wrd = M2R_REG_READ(&pDev->LIF.SFI42CNTL.wrd) ;
        pCfg->div1 = sfi4_2_ctl.bf.TXCKREF_DIV ;
        pCfg->analog_term_lb = sfi4_2_ctl.bf.TX2RX_LB ;
        gpctrl.wrd = M2R_REG_READ(&pDev->LIF.GPCTRL.wrd) ;
        pCfg->line_loopback = gpctrl.bf.LANELB_EN ;
      }
      else {
        xauiCtrl.wrd = M2R_REG_READ(&pDev->LIF.XGXS.XAUI0.wrd) ;
        pCfg->div1 = xauiCtrl.bf.xaui_tx_ckref_d ;
        pCfg->analog_term_lb = xauiCtrl.bf.xaui_rx_loopbk ;
        pCfg->line_loopback = lbswap.bf.RX2TX_LB ;
        pCfg->terminal_loopback = lbswap.bf.TX2RX_LB ;
      }
    }
    break ;

    case INTERFACE_SEL_10G : {
      LIF_TXADJUST3_t       txAdj3 ;
      LIF_RXADJUST4_t       rxAdj4 ;
      LIF_XGCNTL1_t         xgctl ;

      pCfg->interface_sel = M2R_LIF_INTERFACE_XFP ;

      txAdj3.wrd = M2R_REG_READ(&pDev->LIF.TXADJUST3.wrd) ;
      rxAdj4.wrd = M2R_REG_READ(&pDev->LIF.RXADJUST4.wrd) ;

      if (txAdj3.bf.TX_CLKREF_SEL == 0) {
        if (M2R_IS_SONET_MODE(port_id)) {
          cs_uint16             sl_num ;
          FR_GLOBAL_LoopBack_t  loopback;

          M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
          loopback.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.LoopBack.wrd);
          pCfg->tx_clk_mode = loopback.bf.loopTimeMode == 1 ? 
                              M2R_LIF_TX_CLK_LOOP_INTERNAL :
                              M2R_LIF_TX_CLK_LOCAL ;
        }
        else {
          pCfg->tx_clk_mode = lbswap.bf.XGE_LPTIME_EN == 1 ?
                              M2R_LIF_TX_CLK_LOOP_INTERNAL :
                              M2R_LIF_TX_CLK_LOCAL ;
        }
      }
      else {
         pCfg->tx_clk_mode = M2R_LIF_TX_CLK_LOOP_EXTERNAL ;
      }

      xgctl.wrd = M2R_REG_READ(&pDev->LIF.XGCNTL1.wrd) ;
      pCfg->div1 = xgctl.bf.TX_CKREF_DIV1 ;
      pCfg->div2 = xgctl.bf.TX_CKREF_DIV2 ;

      pCfg->analog_term_lb = xgctl.bf.RX_LPBK ;
      pCfg->terminal_loopback = lbswap.bf.TX2RX_LB ;
      pCfg->line_loopback = lbswap.bf.RX2TX_LB ;

      pCfg->rx_clkout_ctl = (rxAdj4.bf.RX_CLKOUT_SEL != 0x3f) ;
      pCfg->rx_clkout_invert = rxAdj4.bf.RX_CLK_INV;
      pCfg->rx_clkout_sel = rxAdj4.bf.RX_CLKOUT_SEL;
      pCfg->tx_clkout_sel = txAdj3.bf.TX_CLKOUT_SEL;
      pCfg->tx_clkout_ctl = txAdj3.bf.TX_CKOUT_EN;
    }
    break ;
  }

  if ( !(pCfg->sonet_mode) &&
       (pCfg->interface_sel == M2R_LIF_INTERFACE_SFI42) ) { /* XGXS */
    pCfg->xgxs_alignment_detected = m2r_lif_xgxs_is_align_detected(port_id) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);

  return(CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start   DUMP RUNNING LIF CONFIGURATION                      */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_dump_running_cfg(cs_uint16 port_id,
				   m2r_lif_runcfg_t * pCfg)
/* INPUTS     : o Port Id                                               */
/*              o Pointer to port config summary info                   */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Prints the current run-time hardware configuration for the           */
/* this block.                                                          */
/*                                                                      */
/* The [pRunCfg] parameter is a pointer to the running                  */
/* configuration data-structure allocated by the caller with            */
/* the configuration data from HW already retrieved and filled          */
/* in, prior to calling this API.                                       */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32     error;
  cs_int8       * refClk = "Unknown";

  if (pCfg == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, NULL) ;
    return (CS_ERROR) ;
  }

  if(M2R_IS_PORT_VALID(port_id, error) != TRUE) {
    CS_HNDL_ERROR(port_id, error, ": port_id = %d\n", port_id);
    return (CS_ERROR) ;
  }


  switch (pCfg->interface_sel) {
    case M2R_LIF_INTERFACE_SFI41 : {
      CS_PRINT("Lif Mode          : %s\n",
               ((pCfg->sonet_mode) ? "SFI4.1": "XSBI")) ;
      CS_PRINT("Line Loopback     : %s\n",
               ((pCfg->line_loopback) ? "Enabled": "Disabled"));
      CS_PRINT("Terminal Loopback : %s\n",
               ((pCfg->terminal_loopback) ? "Enabled": "Disabled"));
    }
    break ;

    case M2R_LIF_INTERFACE_SFI42 : {
      CS_PRINT("Lif Mode          : %s\n",
               ((pCfg->sonet_mode) ? "SFI4.2": "XAUI")) ;
      if ( !(pCfg->sonet_mode) ) {
	CS_PRINT("XAUI Interface status = %s\n",
		 (pCfg->xgxs_alignment_detected) ?
		 "in-sync(aligned)" :
		 "no sync(un-aligned)!!") ;
      }

      if (pCfg->sonet_mode) {
        switch (pCfg->div1) {
          case TXCKREF_DIV_CK77_76 :
            refClk = "77.76 Mhz" ;
          break ;
          case TXCKREF_DIV_CK155_52 :
            refClk = "155.52 Mhz" ;
          break ;
          case TXCKREF_DIV_CK311 :
            refClk = "311 Mhz" ;
          break ;
          case TXCKREF_DIV_CK622 :
            refClk = "622 Mhz" ;
          break ;
        }
        CS_PRINT("Refernce Clock    : %s\n", refClk ) ;
        CS_PRINT("Analog Loopback : %s\n",
               ((pCfg->analog_term_lb) ? "Enabled": "Disabled"));
        CS_PRINT("Line Loopback   : %s\n",
               ((pCfg->line_loopback) ? "Enabled": "Disabled"));
      }
      else {
        switch (pCfg->div1) {
          case 0 :
            refClk = "156.25 Mhz" ;
          break ;
          case 1 :
            refClk = "312.5 Mhz" ;
          break ;
          case 2 :
            refClk = "625 Mhz" ;
          break ;
          default :
            refClk = "Unknown" ;
          break ;
        }
        CS_PRINT("Refernce Clock    : %s\n", refClk ) ;
        CS_PRINT("Analog Loopback   : %s\n",
               ((pCfg->analog_term_lb) ? "Enabled": "Disabled"));
        CS_PRINT("Line Loopback     : %s\n",
               ((pCfg->line_loopback) ? "Enabled": "Disabled"));
        CS_PRINT("Terminal Loopback : %s\n",
               ((pCfg->terminal_loopback) ? "Enabled": "Disabled"));
      }
    }
    break ;

    case M2R_LIF_INTERFACE_XFP : {
      CS_PRINT("Lif Mode          : %s\n",
               ((pCfg->sonet_mode) ? "XFP": "XFP")) ;

      switch (pCfg->tx_clk_mode) {
        case M2R_LIF_TX_CLK_LOCAL :
          CS_PRINT("TX Clock Mode     : %s\n", "Local") ;
        break ;
        case M2R_LIF_TX_CLK_LOOP_INTERNAL :
          CS_PRINT("TX Clock Mode     : %s\n", "Internal Loop timing") ;
        break ;
        case M2R_LIF_TX_CLK_LOOP_EXTERNAL :
          CS_PRINT("TX Clock Mode     : %s\n", "External Loop timing") ;
        break ;
      }

      CS_PRINT("Clk Div1/2        : %d/%d\n", pCfg->div1, pCfg->div2) ;
      CS_PRINT("Rx Clkout Ctrl/Inv: %s/%s\n",
	     ((pCfg->rx_clkout_ctl) ? "Enabled": "Disabled"),
	     ((pCfg->rx_clkout_invert) ? "True": "False"));
      CS_PRINT("Rx Clkout Sel     : 0x%x\n", pCfg->rx_clkout_sel);
      CS_PRINT("Tx Clkout Ctrl    : %s\n",
	     ((pCfg->tx_clkout_ctl) ? "Enabled": "Disabled"));
      CS_PRINT("Tx Clkout Sel     : 0x%x\n", pCfg->tx_clkout_sel);
      CS_PRINT("Analog Loopback   : %s\n",
             ((pCfg->analog_term_lb) ? "Enabled": "Disabled"));
      CS_PRINT("Line Loopback     : %s\n",
             ((pCfg->line_loopback) ? "Enabled": "Disabled"));
      CS_PRINT("Terminal Loopback : %s\n",
             ((pCfg->terminal_loopback) ? "Enabled": "Disabled"));
    }
    break ;
  }

  return(CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start    LIF SOFT RESET                                     */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_set_sreset(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Put Line Interface(LIF) block in Soft/Logic Reset.                   */
/* The CPU access registers are unaffected.                             */
/*                                                                      */
/* The reset bit in LIF register is self-clearing and will be           */
/* cleared after all the LIF logic reset has been done.                 */
/*                                                                      */
/* The caller is responsible to ensure that soft reset is               */
/* complete by calling m2r_lif_is_sreset_done() and checking            */
/* for a TRUE return value before proceeding further in                 */
/* configuring LIF registers.                                           */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  M2R_REG_WRITE(&pDev->LIF.SRST.wrd, 0x01) ;

  return(CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start    CHECK LIF SOFT RESET STATUS                        */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_boolean  m2r_lif_is_sreset_done(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : TRUE(reset done) or FALSE(still in reset)               */
/* DESCRIPTION:                                                         */
/* Returns TRUE if reset of LIF logic is complete or FALSE if           */
/* LIF logic is still in reset state.                                   */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  LIF_SRST_t           srst ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, FALSE);

  srst.wrd = M2R_REG_READ(&pDev->LIF.SRST.wrd) ;

  if (srst.bf.SRST) return(FALSE) ;
  else return(TRUE) ;
}

/************************************************************************/
/* $rtn_hdr_start  LIF INTERFACE SELECTION                              */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_select_interface(cs_uint16 port_id, m2r_lif_sel_t sel)
/* INPUTS     : o Port Id                                               */
/*              o Line Interface selection                              */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Selects the Line Interface Mode from SFI4.1 or SFI4.2 or XFP if MILAN2*/
/* is operating in SONET mode or from XSBI or XAUI or XFP if MILAN2     */
/* is operating in ETHERNET mode. In case SFI4.1/XSBI is selected,      */
/* it powers down SFI4.1/XAUI and XFP blocks.                           */
/*                                                                      */
/* The [sel] parameter is specified as one of the following:            */
/* M2R_LIF_INTERFACE_SFI41 or                                           */
/* M2R_LIF_INTERFACE_SFI42, or M2R_LIF_INTERFACE_XFP                    */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  LIF_INTERFACE_t      interf ;
  M2R_t              * pDev ;
  LIF_GPCTRL_t         gpctrl ;
  LIF_VCOFREQ_t        vcofreq ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  switch(sel) {
      case M2R_LIF_INTERFACE_SFI41:
        m2r_lif_ctl_power(port_id, M2R_LIF_INTERFACE_XFP, M2R_DISABLE);
        m2r_lif_ctl_power(port_id, M2R_LIF_INTERFACE_SFI42, M2R_DISABLE);
        break;

      case M2R_LIF_INTERFACE_SFI42:
        m2r_lif_ctl_power(port_id, M2R_LIF_INTERFACE_SFI42, M2R_ENABLE);
        m2r_lif_ctl_power(port_id, M2R_LIF_INTERFACE_XFP, M2R_DISABLE);
        break;

      case M2R_LIF_INTERFACE_XFP:
        m2r_lif_ctl_power(port_id, M2R_LIF_INTERFACE_XFP, M2R_ENABLE);
        m2r_lif_ctl_power(port_id, M2R_LIF_INTERFACE_SFI42, M2R_DISABLE);
        break;

      default :
        CS_HNDL_ERROR(port_id, EM2R_LIF_INVALID_IF, "\n") ;
        return (CS_ERROR) ;
  }

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  interf.wrd = M2R_REG_READ(&pDev->LIF.INTERF.wrd) ;
  switch(sel) {
      case M2R_LIF_INTERFACE_SFI41:
      {
        interf.bf.INTERFACE_SEL = INTERFACE_SEL_SFI41_XSBI ;
        M2R_REG_WRITE(&pDev->LIF.INTERF.wrd, interf.wrd) ;
      }
      break ;

      case M2R_LIF_INTERFACE_SFI42:
      {
        interf.bf.INTERFACE_SEL = INTERFACE_SEL_SFI42_XAUI ;
        /* Select XAUI Vs SFI4.2.  */
        gpctrl.wrd = M2R_REG_READ(&pDev->LIF.GPCTRL.wrd) ;
        gpctrl.bf.SFI42_XAUI = M2R_IS_SONET_MODE(port_id) ? 1 : 0;
	vcofreq.wrd = M2R_REG_READ(&pDev->LIF.VCOFREQ.wrd) ;
	/* 0xc838 is VCOFREQ default. Set to 0xcc3c in XAUI mode */
	vcofreq.wrd = (gpctrl.bf.SFI42_XAUI == 0) ? 0xcc3c : 0xc838;
        M2R_REG_WRITE(&pDev->LIF.GPCTRL.wrd, gpctrl.wrd) ;
        M2R_REG_WRITE(&pDev->LIF.INTERF.wrd, interf.wrd) ;
	M2R_REG_WRITE(&pDev->LIF.VCOFREQ.wrd, vcofreq.wrd) ;
      }
      break ;

      case M2R_LIF_INTERFACE_XFP:
      {
        interf.bf.INTERFACE_SEL = INTERFACE_SEL_10G ;
        M2R_REG_WRITE(&pDev->LIF.INTERF.wrd, interf.wrd) ;

        /* Init Analog Section */
        m2r_lif_xfp_analog_init(port_id, pDev) ;
      }
      break ;
  }
  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
  return(CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start   CONTROL POWER ON/OFF ON SFI42/XAUI AND XFP          */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_ctl_power(cs_uint16 port_id, m2r_lif_sel_t sel,
                             cs_uint8 ctrl)
/* INPUTS     : o Port Id                                               */
/*              o Line Interface selection                              */
/*              o control(M2R_ENABLE or M2R_DISABLE)                    */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Powers Down or Up the LIF SFI42/XAUI  OR XFP interface.              */
/* The [sel] parameter is specified as one of the following:            */
/* M2R_LIF_INTERFACE_SFI42, or M2R_LIF_INTERFACE_XFP                    */
/* Returns CS_OK if the POWER_DOWN or POWER_UP OPERATION                */
/* is SUCCESS.                                                          */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_uint32            status = CS_OK ;
  LIF_PWRDWN_t         pwrctrl ;
  cs_uint8             pwr_mode ;
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);
  if (( sel != M2R_LIF_INTERFACE_SFI42) &&
      ( sel != M2R_LIF_INTERFACE_XFP)) {
    CS_HNDL_ERROR(port_id, EM2R_LIF_INVALID_IF,
                  "Interface selection s/b SFI4.2 or XFP. Secection = %d\n",
                  (int)sel) ;
    status = CS_ERROR;
    goto EXIT;
  }
  M2R_PORT_LOCK(port_id, M2R_ID_LIF);
  pwrctrl.wrd = M2R_REG_READ(&pDev->LIF.PWRDWN.wrd) ;
  pwr_mode = (ctrl == M2R_ENABLE ) ? 0 : 1;
  if (sel == M2R_LIF_INTERFACE_SFI42) {
    pwrctrl.bf.STX_PD = pwr_mode;
    pwrctrl.bf.SRX_PD = pwr_mode;
  }
  else {
    pwrctrl.bf.STX_XGPD = pwr_mode;
    pwrctrl.bf.SRX_XGPD = pwr_mode;
  }
  M2R_REG_WRITE(&pDev->LIF.PWRDWN.wrd, pwrctrl.wrd);
  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);

EXIT:
  return(status) ;
}

/************************************************************************/
/* $rtn_hdr_start  SET THE CLOCK DIVIDER VALUES                         */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_set_clk_div(cs_uint16 port_id,
                               m2r_lif_tx_clk_mode_t tx_clk_mode)
/* INPUTS     : o Port Id                                               */
/*              o TX Clock Mode                                         */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Set the clock divider values for  SFI4.2 or XFP interface.           */
/* NOTE : This API is only valid when the LIF is configured for         */
/* SFI4.2/XAUI or XFP mode.                                             */
/*                                                                      */
/* The [tx_clk_mode] parameter is specified as one of the following:    */
/* M2R_LIF_TX_CLK_LOCAL or M2R_LIF_TX_CLK_LOOP_INTERNAL or              */
/* M2R_LIF_TX_CLK_LOOP_EXTERNAL                                         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t             * pDev ;
  LIF_INTERFACE_t   interf ;
  m2r_port_cb_t     *ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;


  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  interf.wrd = M2R_REG_READ(&pDev->LIF.INTERF.wrd) ;

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  switch (interf.bf.INTERFACE_SEL) {
    case INTERFACE_SEL_SFI42_XAUI :
      m2r_lif_set_sfi42_clk_div(port_id, pDev, tx_clk_mode) ;
      break ;

    case INTERFACE_SEL_10G :
      m2r_lif_set_xfp_clk_div(port_id, pDev, tx_clk_mode) ;
      break ;

    default :
    CS_HNDL_ERROR(port_id, EM2R_LIF_INVALID_IF, "\n") ;
    return (CS_ERROR) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
   
  if ((ppcb != NULL) && (tx_clk_mode != M2R_LIF_TX_CLK_LOOP_INTERNAL))  {
    ppcb->tx_clk_mode = tx_clk_mode ;
  }
  return(CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  SELECT TX CLOCK MODE FOR XFP                         */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_xfp_set_clk_mode(cs_uint16 port_id,
                                   m2r_lif_tx_clk_mode_t clkMode)
/* INPUTS     : o Port Id                                               */
/*              o TX Clock Mode                                         */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Select TX Clock mode.                                                */
/* NOTE : only applicable when LIF is in XFP Mode.                      */
/*                                                                      */
/* [clkMode] M2R_LIF_TX_CLK_LOCAL or M2R_LIF_TX_CLK_LOOP_EXTERNAL       */
/*           (M2R_LIF_TX_CLK_INTERNAL, for debugging only)              */
/*                                                                      */
/* clock distribution option 1   --> M2R_LIF_TX_CLK_LOOP_EXTERNAL       */
/* clock distribution option 2,3 --> M2R_LIF_TX_CLK_LOOP_LOCAL          */
/* Prior to this, you may need to use API m2r_lif_xfp_use_clk_ref()     */
/* to specify ref clk first.                                            */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  LIF_LBSWAP_t          lbswap ;
  M2R_t               * pDev ;
  LIF_INTERFACE_t       interf ;
  LIF_TXADJUST3_t       txAdj3 ;
  LIF_RXADJUST5_t       rxAdj5 ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  if (clkMode > M2R_LIF_TX_CLK_LOOP_EXTERNAL) {
    CS_HNDL_ERROR(port_id, EM2R_LIF_INVALID_REF_CLK, "\n") ;
    return (CS_ERROR) ;
  }

  interf.wrd = M2R_REG_READ(&pDev->LIF.INTERF.wrd) ;
  if (interf.bf.INTERFACE_SEL != INTERFACE_SEL_10G) {
    CS_HNDL_ERROR(port_id, EM2R_LIF_INVALID_LPTIME_MODE, "\n") ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  /* Set the Clock dividers */
  m2r_lif_set_clk_div(port_id, clkMode) ;

  lbswap.wrd = M2R_REG_READ(&pDev->LIF.LBSWAP.wrd) ;
  txAdj3.wrd = M2R_REG_READ(&pDev->LIF.TXADJUST3.wrd) ;
  rxAdj5.wrd = M2R_REG_READ(&pDev->LIF.RXADJUST5.wrd) ;

  switch (clkMode) {
    case M2R_LIF_TX_CLK_LOCAL :
      lbswap.bf.XGE_LPTIME_EN = 0 ;
      txAdj3.bf.TX_CLKREF_SEL = 0 ;

      /* analog adjustment based on clk mode */
      if (m2r_mpif_is_in_oc48_mode(port_id)) {
        rxAdj5.bf.RX_PHSEL = 0x0f;
      }
      else if (m2r_mpif_is_in_eth_mode(port_id)) {
        rxAdj5.bf.RX_PHSEL = 0x0d;
      }
      else {
        rxAdj5.bf.RX_PHSEL = 0x0c;
      }
    break ;

    case M2R_LIF_TX_CLK_LOOP_INTERNAL :
      lbswap.bf.XGE_LPTIME_EN = 1 ;
      txAdj3.bf.TX_CLKREF_SEL = 0 ;

      /* analog adjustment based on clk mode */
      if (m2r_mpif_is_in_oc48_mode(port_id)) {
        rxAdj5.bf.RX_PHSEL = 0x0f;
      }
      else if (m2r_mpif_is_in_eth_mode(port_id)) {
        rxAdj5.bf.RX_PHSEL = 0x0d;
      }
      else {
        rxAdj5.bf.RX_PHSEL = 0x0c;
      }
    break ;

    case M2R_LIF_TX_CLK_LOOP_EXTERNAL :
      lbswap.bf.XGE_LPTIME_EN = 0 ;
      txAdj3.bf.TX_CLKREF_SEL = 1 ;

      /* analog adjustment based on clk mode */
      if (m2r_mpif_is_in_oc48_mode(port_id)) {
        rxAdj5.bf.RX_PHSEL = 0x0f;
      }
      else {
        rxAdj5.bf.RX_PHSEL = 0x0d;
      }
    break ;
  }

  M2R_REG_WRITE(&pDev->LIF.LBSWAP.wrd, lbswap.wrd) ;
  M2R_REG_WRITE(&pDev->LIF.TXADJUST3.wrd, txAdj3.wrd) ;
  M2R_REG_WRITE(&pDev->LIF.RXADJUST5.wrd, rxAdj5.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
  return(CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  LIF LINE LOOPBACK                                    */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_ctl_line_loopback(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)                       */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable or disable line loopback at the Line Interface. The           */
/* Rx data is looped back to Tx.                                        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  LIF_GPCTRL_t      gpctrl ;
  LIF_LBSWAP_t      lbswap ;
  LIF_INTERFACE_t   interf ;
  M2R_t             * pDev ;
  cs_boolean        ethMode ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  interf.wrd = M2R_REG_READ(&pDev->LIF.INTERF.wrd) ;

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  ethMode = M2R_IS_ETHERNET_MODE(port_id) ;
  if ((!ethMode) &&
      (interf.bf.INTERFACE_SEL == INTERFACE_SEL_SFI42_XAUI)) {

    gpctrl.wrd = M2R_REG_READ(&pDev->LIF.GPCTRL.wrd) ;
    gpctrl.bf.LANELB_EN = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->LIF.GPCTRL.wrd, gpctrl.wrd) ;
  }
  else {
    lbswap.wrd = M2R_REG_READ(&pDev->LIF.LBSWAP.wrd) ;
    lbswap.bf.RX2TX_LB = (ctl == M2R_ENABLE) ? 1 : 0 ; 
    if (ethMode &&
        (interf.bf.INTERFACE_SEL != INTERFACE_SEL_SFI42_XAUI)) {
      lbswap.bf.XGE_LPTIME_EN = (ctl == M2R_ENABLE) ? 1 : 0 ; 
    }
    M2R_REG_WRITE(&pDev->LIF.LBSWAP.wrd, lbswap.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);

  if (interf.bf.INTERFACE_SEL != INTERFACE_SEL_SFI41_XSBI) {
    m2r_port_cb_t   *ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;
    if (ppcb != NULL) {
      m2r_lif_set_clk_div(port_id,
        (ctl == M2R_ENABLE) ? M2R_LIF_TX_CLK_LOOP_INTERNAL : ppcb->tx_clk_mode) ;
    }
  }

  if (!ethMode) {
    return m2r_framer_ctl_loop_timing(port_id, ctl) ;
  }
  return(CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  LIF TERMINAL LOOPBACK                                */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_ctl_terminal_loopback(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Loopback control(M2R_ENABLE or M2R_DISABLE)           */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable terminal loopback. Loopback takes place at the digital*/
/* portion of the LIF.                                                  */
/* Terminal loopback takes place at the digital section of the LIF and  */
/* is not VALID for SFI4.2 mode of LIF.                                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  LIF_LBSWAP_t         lbswap ;
  LIF_INTERFACE_t      interf ;
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  interf.wrd = M2R_REG_READ(&pDev->LIF.INTERF.wrd) ;

  if ((M2R_IS_SONET_MODE(port_id)) &&
      (interf.bf.INTERFACE_SEL == INTERFACE_SEL_SFI42_XAUI)) {

    CS_HNDL_ERROR(port_id, EM2R_LIF_INVALID_DIGITAL_LB,
                      "Digital LB not supported \n") ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK(port_id, M2R_ID_LIF) ;

  lbswap.wrd = M2R_REG_READ(&pDev->LIF.LBSWAP.wrd) ;
  lbswap.bf.TX2RX_LB = (ctl == M2R_DISABLE) ? 0 : 1 ;
  M2R_REG_WRITE(&pDev->LIF.LBSWAP.wrd, lbswap.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start  LIF TERMINAL ANALOG LOOPBACK                         */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status  m2r_lif_ctl_analog_loopback(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o Loopback control(M2R_ENABLE or M2R_DISABLE)           */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable terminal loopback. Loopback takes place at the analog */
/* portion of the LIF. Analog Loopback is not available for SFI4.1/XSBI */
/* modes of LIF.                                                        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev ;
  LIF_INTERFACE_t      interf ;
  LIF_XGCNTL1_t        xgctl;
  LIF_SFI42CNTL_t      sfi4_2_ctl;
  XGXS_MP_XAUI0_t      xauiCtrl ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  interf.wrd = M2R_REG_READ(&pDev->LIF.INTERF.wrd) ;

  if (interf.bf.INTERFACE_SEL == INTERFACE_SEL_SFI41_XSBI) {
    CS_HNDL_ERROR(port_id, EM2R_LIF_INVALID_DIGITAL_LB,
                      "Analog LB not supported for XSBI/SFI4.1\n") ;
    return (CS_ERROR) ;
  }

  M2R_PORT_LOCK(port_id, M2R_ID_LIF) ;

  if (interf.bf.INTERFACE_SEL == INTERFACE_SEL_10G) {
    xgctl.wrd = M2R_REG_READ(&pDev->LIF.XGCNTL1.wrd) ;
    xgctl.bf.RX_LPBK = (ctl == M2R_ENABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->LIF.XGCNTL1.wrd, xgctl.wrd) ;
  }
  else {
    if (M2R_IS_SONET_MODE(port_id)) {
      sfi4_2_ctl.wrd = M2R_REG_READ(&pDev->LIF.SFI42CNTL.wrd) ;
      sfi4_2_ctl.bf.TX2RX_LB = (ctl == M2R_ENABLE) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->LIF.SFI42CNTL.wrd, sfi4_2_ctl.wrd) ;
    }
    else {
      xauiCtrl.wrd = M2R_REG_READ(&pDev->LIF.XGXS.XAUI0.wrd) ;
      xauiCtrl.bf.xaui_rx_loopbk = (ctl == M2R_ENABLE) ? 1 : 0 ;
      M2R_REG_WRITE(&pDev->LIF.XGXS.XAUI0.wrd, xauiCtrl.wrd) ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE SONET UNFRAMED PRBS                  */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_ctl_sonet_prbs(cs_uint16 port_id, m2r_ctl_t ctl,
                                            m2r_lif_sonet_prbs_t ptrn)
/* INPUTS     : o Port Id                                               */
/*              o M2R_DISABLE(default) or M2R_ENABLE                    */
/*              o PRBS pattern type                                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the PRBS on LIF interfaces. It is applicable for all  */
/* SFI41, SFI42 AND XFP interfaces.                                     */
/* The [ptrn] parameter is specified as one of the following:           */
/* M2R_LIF_PRBS_PATTERN_31 or M2R_LIF_PRBS_PATTERN_23                   */
/* M2R_LIF_PRBS_PATTERN_15 or M2R_LIF_PRBS_PATTERN_7                    */
/* NOTE : unless external loopback is used , user is responsible for    */
/* putting the LIF interface into the internal Loopback.                */
/* user should call m2r_lif_ctl_terminal_loopback to put the LIF in     */
/* internal loopback.                                                   */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev  ;
  cs_uint8             bit_ctl ;
  LIF_PRBSPROV_t       prbs_prvsn ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  if(!M2R_IS_SONET_MODE(port_id)) {
    CS_HNDL_ERROR(port_id, EM2R_LIF_INVALID_IN_ETH_MODE, "\n" ) ;
    return (CS_ERROR) ;
  }

  bit_ctl = (ctl == M2R_ENABLE) ? 1 : 0 ;

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  if ( ctl == M2R_ENABLE ) {
    M2R_REG_WRITE(&pDev->LIF.PRBSCNT_LSB.wrd, 0) ;
    M2R_REG_WRITE(&pDev->LIF.PRBSCNT_MSB.wrd, 0) ;
  }

  prbs_prvsn.wrd = M2R_REG_READ(&pDev->LIF.PRBSPROV.wrd) ;

  prbs_prvsn.bf.TXPRBS_EN = bit_ctl ;
  prbs_prvsn.bf.RXPRBS_EN = bit_ctl ;
  prbs_prvsn.bf.TXPRBS_MODE = prbs_prvsn.bf.RXPRBS_MODE = ptrn ;

  M2R_REG_WRITE(&pDev->LIF.PRBSPROV.wrd, prbs_prvsn.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
  return(CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE PRBS IN LIF                          */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_ctl_prbs(cs_uint16 port_id, m2r_ctl_t ctl,
                           cs_uint32 prbs_pattern)
/* INPUTS     : o Port Id                                               */
/*              o M2R_DISABLE(default) or M2R_ENABLE                    */
/*              o PRBS pattern Type                                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the PRBS on LIF interfaces. It is applicable for all  */
/* SFI41, SFI42 AND XFP interfaces.                                     */
/*                                                                      */
/* The [prbs_pattern] parameter one of following ( SONET MODE )         */
/* M2R_LIF_PRBS_PATTERN_31 or M2R_LIF_PRBS_PATTERN_23                   */
/* M2R_LIF_PRBS_PATTERN_15 or M2R_LIF_PRBS_PATTERN_7                    */
/*                                                                      */
/* The [prbs_pattern] parameter one of following ( ETH MODE using XAUI) */
/* M2R_XGXS_REG_PATTERN or M2R_XGXS_HFPAT or M2R_XGXS_LFPAT or          */
/* M2R_XGXS_MFPAT or M2R_XGXS_CRPAT_0 or M2R_XGXS_CRPAT_1 or            */
/* M2R_XGXS_CJPAT                                                       */
/*                                                                      */
/* The [prbs_pattern] parameter one of following ( ETH MODE using XFP ) */
/* M2R_LIF_PCS_DAT_LOW or M2R_LIF_PCS_SQ1 or M2R_LIF_PCS_SQ2  or        */
/* M2R_LIF_PCS_SQ4 or M2R_LIF_PCS_SQ8 or M2R_LIF_PCS_PULSE0 or          */
/* M2R_LIF_PCS_PULSE1 or M2R_LIF_PCS_DAT_HIGH or M2R_LIF_PCS_PRBS31     */
/*                                                                      */
/* NOTE : unless external loopback is used , user is responsible for    */
/* putting the LIF interface into the internal Loopback.                */
/* user should call m2r_lif_ctl_terminal_loopback to put the LIF in     */
/* internal loopback.                                                   */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev  ;
  LIF_INTERFACE_t      interf ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  if (M2R_IS_SONET_MODE(port_id)) {
    m2r_lif_ctl_sonet_prbs(port_id, ctl, prbs_pattern) ;
  }
  else {
    interf.wrd = M2R_REG_READ(&pDev->LIF.INTERF.wrd) ;
    if (interf.bf.INTERFACE_SEL == INTERFACE_SEL_SFI42_XAUI) {
      m2r_lif_xgxs_ctl_prbs(port_id, ctl, prbs_pattern) ;
    }
    else {
      m2r_lif_pcs6_ctl_test_pattern(port_id, M2R_LIF_PCS_PRBS_31,
                                    M2R_RX_AND_TX, ctl) ;
    }
  }

  return(CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start   INJECT ERRORS IN PRBS (SONET MODE)                  */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_sonet_inj_error(cs_uint16 port_id, m2r_lif_prbs_err_t err,
                            cs_boolean once, cs_uint16 duration)
/* INPUTS     : o Port Id                                               */
/*              o ERROR TYPE ( 1 BIT ) OR BLOCK ERROR                   */
/*              o Once ( TRUE ) or continuously ( FALSE )               */
/*              o Durtaion of error injection(if doing continously)     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Injects error in the PRBS stream. Errors can be injected once or     */
/* continuously for a period of time. If parameter [once] is set to TRUE*/
/* Single error is injected else for the period specified by duration.  */
/*                                                                      */
/* The [err] parameter is one of the following:                         */
/* M2R_LIF_PRBS_1_BIT_ERR or M2R_LIF_PRBS_BLOCK_ERR                     */
/*                                                                      */
/* Note : This function might block for "duration" if continuous errors */
/* need to be generated. This API is valid only in SONET MODE.          */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev  ;
  LIF_PRBS_CNTL_t      prbsCtrl ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  prbsCtrl.wrd = M2R_REG_READ(&pDev->LIF.PRBS_CNTL.wrd) ;

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  switch ( err ) {
    case M2R_LIF_PRBS_BLOCK_ERR:
      if(once) {
        prbsCtrl.bf.INSERR_ONCE = 1 ;
      }
      else {
        prbsCtrl.bf.INS_ERR = 1 ;
      }
      break ;

    case M2R_LIF_PRBS_1_BIT_ERR:
      if(once) {
        prbsCtrl.bf.INSONEBITERR_ONCE = 1 ;
      }
      else {
        prbsCtrl.bf.INSONEBIT_ERR = 1 ;
      }
      break ;

    default :
      M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
      CS_HNDL_ERROR(port_id, EM2R_LIF_INVALID_ERROR_OP, "\n" ) ;
      return (CS_ERROR) ;
  }

  M2R_REG_WRITE(&pDev->LIF.PRBS_CNTL.wrd, prbsCtrl.wrd) ;

  if(!once) {
    CS_UDELAY(duration) ;
    if ( err == M2R_LIF_PRBS_BLOCK_ERR ) {
      prbsCtrl.bf.INS_ERR = 0 ;
    }
    else {
      prbsCtrl.bf.INSONEBIT_ERR = 0 ;
    }
    M2R_REG_WRITE(&pDev->LIF.PRBS_CNTL.wrd, prbsCtrl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start   INJECT ERRORS IN PRBS                               */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_inj_error(cs_uint16 port_id, m2r_lif_prbs_err_t err,
                            cs_boolean once, cs_uint16 duration)
/* INPUTS     : o Port Id                                               */
/*              o M2R_LIF_PRBS_1_BIT_ERR or M2R_LIF_PRBS_BLOCK_ERR      */
/*              o Once ( TRUE ) or continuously ( FALSE )               */
/*              o Durtaion of error injection(if doing continously)     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Injects error in the PRBS stream. Errors can be injected once or     */
/* continuously for a period of time. If parameter [once] is set to TRUE*/
/* Single error is injected else for the period specified by duration.  */
/* This API enables SONET PRBS or PCS PRBS or XGXS PRBS, depending      */
/* on the current mode MILAN2 is configured for.                        */
/*                                                                      */
/* Note : This function might block for time specified by [duration]    */
/* if parameter [once] is FALSE.                                        */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev  ;
  LIF_INTERFACE_t      interf ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  if(M2R_IS_SONET_MODE(port_id)) {
    m2r_lif_sonet_inj_error(port_id, err, once, duration) ;
  }
  else {
    interf.wrd = M2R_REG_READ(&pDev->LIF.INTERF.wrd) ;
    if (interf.bf.INTERFACE_SEL == INTERFACE_SEL_SFI42_XAUI) {
      m2r_lif_xgxs_inj_error(port_id, M2R_XGXS_XAUI_LANE0) ;
    }
    else {
      m2r_lif_pcs6_inj_error(port_id) ;
    }
  }
  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start   GET PRBS ERROR COUNT                                */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_uint32 m2r_lif_get_prbs_errors(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : PRBS ERROR COUNT                                        */
/* DESCRIPTION:                                                         */
/* Gets the current PRBS error count. This is cumalative error count    */
/* since the chip was taken out of reset.                               */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev  ;
  cs_uint16            cnt_lsb, cnt_msb ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  cnt_msb = M2R_REG_READ(&pDev->LIF.PRBSCNT_MSB.wrd) ;
  cnt_lsb = M2R_REG_READ(&pDev->LIF.PRBSCNT_LSB.wrd) ;

  return ((cnt_msb << 16) | cnt_lsb) ;
}

/************************************************************************/
/* $rtn_hdr_start    CONTROL SFI4.2 INTERFACE PROPERTIES                */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_sfi42_ctl(cs_uint16 port_id, m2r_dir_t direction,
                            m2r_lif_sfi42_ctrl_t sel, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX                     */
/*              o Control selection                                     */
/*              o M2R_DISABLE(default) or M2R_ENABLE                    */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disables/configure SFI4.2 interface's properties.             */
/*                                                                      */
/* [sel] parameters is one of the following :                           */
/* LIF_SFI42_CTRL_PN_INV or LIF_SFI42_CTRL_RX_DSCR_EN                   */
/* LIF_SFI42_CTRL_LANE_SWAP or LIF_SFI42_CTRL_BIT_SWAP or               */
/* LIF_SFI42_CTRL_TX_SCR_EN or LIF_SFI42_CTRL_PBRS_CNT_LAT or           */
/* LIF_SFI42_CTRL_PBRS_EN                                               */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  cs_boolean           tx_dir = FALSE;
  cs_boolean           rx_dir = FALSE;
  LIF_GPCTRL_t         gpctrl;
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  switch ( direction ) {
      case M2R_RX : rx_dir = TRUE; break;
      case M2R_TX : tx_dir = TRUE; break;
      case M2R_RX_AND_TX: rx_dir = TRUE; tx_dir = TRUE; break;
      default : CS_HNDL_ERROR(direction, EM2R_PORT_INVALID_DIR, NULL);
           return (CS_ERROR);
  }

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  gpctrl.wrd = M2R_REG_READ(&pDev->LIF.GPCTRL.wrd);

  switch (sel) {
    case LIF_SFI42_CTRL_PN_INV :
        if (rx_dir) gpctrl.bf.RXBIT_INV = (ctl == M2R_ENABLE) ? 1 : 0 ;
        if (tx_dir) gpctrl.bf.TXBIT_INV = (ctl == M2R_ENABLE) ? 1 : 0 ;
        break;

    case LIF_SFI42_CTRL_RX_DSCR_EN :
        if (rx_dir) gpctrl.bf.RXDSRC_EN = (ctl == M2R_ENABLE) ? 1 : 0 ;
        break;

    case LIF_SFI42_CTRL_LANE_SWAP :
        if (rx_dir) gpctrl.bf.RXLANE_SWAP = (ctl == M2R_ENABLE) ? 1 : 0 ;
        if (tx_dir) gpctrl.bf.TXLANE_SWAP = (ctl == M2R_ENABLE) ? 1 : 0 ;
        break;

    case LIF_SFI42_CTRL_BIT_SWAP :
        if (rx_dir) gpctrl.bf.RXBIT_SWAP = (ctl == M2R_ENABLE) ? 1 : 0 ;
        if (tx_dir) gpctrl.bf.TXBIT_SWAP = (ctl == M2R_ENABLE) ? 1 : 0 ;
        break;

    case LIF_SFI42_CTRL_TX_SCR_EN :
        if (tx_dir) gpctrl.bf.TXSCR_EN = (ctl == M2R_ENABLE) ? 1 : 0 ;
        break;

    case LIF_SFI42_CTRL_PBRS_CNT_LAT :
        gpctrl.bf.PRBSCNT_LAT = (ctl == M2R_ENABLE) ? 1 : 0 ;
        break;

    case LIF_SFI42_CTRL_PBRS_EN :
        gpctrl.bf.PRBS_EN = (ctl == M2R_ENABLE) ? 1 : 0 ;
        break;

    default:
      CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_OP, NULL) ;
  }
  M2R_REG_WRITE(&pDev->LIF.GPCTRL.wrd, gpctrl.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
  return(CS_OK) ;
}


/************************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE SONET SFI4.2 PRBS                    */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_ctl_sfi42_prbs(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o M2R_DISABLE(default) or M2R_ENABLE                    */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Enable/Disable the PRBS on LIF SFI4.2 interface. PRBS is applicable  */
/* when LIF is configured for SFI4.2 interface. This is solely for      */
/* testing the analog sfi4.2 interface.                                 */
/* NOTE: before enabling PRBS, SFI interface should have been put into  */
/* Loopback, either external or internal.                               */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev  ;
  LIF_GPCTRL_t         gpctrl ;
  cs_uint8             bit_ctl ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  bit_ctl  = (ctl == M2R_DISABLE) ? 0 : 1 ;

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  gpctrl.wrd = M2R_REG_READ(&pDev->LIF.GPCTRL.wrd) ;

  /*
   * Clear any previous eror counters. Get fresh error values.
   */
  if (ctl == M2R_ENABLE) {
    gpctrl.bf.PRBSCNT_LAT = 1 ;
    M2R_REG_WRITE(&pDev->LIF.GPCTRL.wrd, gpctrl.wrd) ;
  }

  gpctrl.bf.PRBS_EN     = bit_ctl ;
  M2R_REG_WRITE(&pDev->LIF.GPCTRL.wrd, gpctrl.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
  return(CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start   SET THE SFI4.2 IN SYNC THRESHOLD VALUE              */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_sfi42_set_in_sync_threshold(cs_uint16 port_id,
                                              cs_uint8  thrshold)
/* INPUTS     : o Port Id                                               */
/*              o Threshold Value (0 - 127)                             */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Set the In-sync threshold value. This is the number of consecutive   */
/* 66-bit data frames during which the sync bit pattern is observed in  */
/* the same relative position before declaring that the data lane is    */
/* "in-sync".                                                           */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev  ;
  LIF_RXCTRL4_t        sfi42_rx_ctl ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  sfi42_rx_ctl.wrd = M2R_REG_READ(&pDev->LIF.RXCTRL4.wrd) ;
  sfi42_rx_ctl.bf.IS_THR = thrshold ;
  M2R_REG_WRITE(&pDev->LIF.RXCTRL4.wrd, sfi42_rx_ctl.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
  return (CS_OK);
}


/************************************************************************/
/* $rtn_hdr_start   SET THE SFI4.2 OUT OF SYNC THRESHOLD VALUE          */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_sfi42_set_oos_threshold(cs_uint16 port_id,
                                          cs_uint8  thrshold)
/* INPUTS     : o Port Id                                               */
/*              o Threshold Value (1 - 63)                              */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Set the out of sync threshold value. This is the number of times,    */
/* within a window size established by IS_THR, that the sync bit pattern*/
/* is in error before declaring that the data lane is "out-of-sync".    */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  M2R_t              * pDev  ;
  LIF_RXCTRL4_t        sfi42_rx_ctl ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  sfi42_rx_ctl.wrd = M2R_REG_READ(&pDev->LIF.RXCTRL4.wrd) ;
  sfi42_rx_ctl.bf.OOS_THR = thrshold ;
  M2R_REG_WRITE(&pDev->LIF.RXCTRL4.wrd, sfi42_rx_ctl.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
  return (CS_OK);
}


/************************************************************************/
/* $rtn_hdr_start   GET LIF MODE CONFIGURATION                          */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
m2r_port_lif_mode_t m2r_lif_get_lif_mode(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : LIF mode                                                */
/* DESCRIPTION:                                                         */
/* Return the current lif mode selected which is one of the following   */
/* The returned value is one of the following enumerated values,        */
/* M2R_PORT_IF_SFI4_1, M2R_PORT_IF_SFI4_2, or M2R_PORT_IF_XFP.          */
/*                                                                      */
/*                                                                      */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  LIF_INTERFACE_t      interf ;
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  interf.wrd = M2R_REG_READ(&pDev->LIF.INTERF.wrd) ;

  switch (interf.bf.INTERFACE_SEL) {
    default :
    case INTERFACE_SEL_SFI41_XSBI :
      return M2R_PORT_IF_SFI4_1 ;
    break ;

    case INTERFACE_SEL_SFI42_XAUI :
      return M2R_PORT_IF_SFI4_2 ;
    break ;

    case INTERFACE_SEL_10G :
      return M2R_PORT_IF_XFP ;
    break ;
  }
}


/************************************************************************/
/* $rtn_hdr_start   GET THE CLOCKS STATUS                               */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_boolean m2r_lif_check_clk_status(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : TRUE or FALSE                                           */
/* DESCRIPTION:                                                         */
/* Get the Clock status in LIF. If either RX or TX or BOTH the clocks   */
/* are missing from the LIF block, False is returned else True is       */
/* returned.                                                            */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  LIF_STATUS2_t        status2 ;
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, 0);

  status2.wrd = M2R_REG_READ(&pDev->LIF.STATUS2.wrd) ;

  if (status2.bf.sTXCK_MISS || status2.bf.sRXCK_MISS ) {
    return ( FALSE ) ;
  }
  else {
    return ( TRUE ) ;
  }
}

/************************************************************************/
/* $rtn_hdr_start   RETURN LOCK DETECTOR'S STATUS                       */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_boolean m2r_lif_xfp_is_locked(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : TRUE or FALSE                                           */
/* DESCRIPTION:                                                         */
/* Return whether XFP receiver has acquired lock or not.                */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  LIF_STATUS2_t        status2 ;
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, 0);

  status2.wrd = M2R_REG_READ(&pDev->LIF.STATUS2.wrd) ;

  if (status2.bf.sXGLOCK) {
    return ( TRUE ) ;
  }
  else {
    return ( FALSE ) ;
  }
}

/************************************************************************/
/* $rtn_hdr_start   RETURN DATA DETECTOR'S STATUS                       */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_boolean m2r_lif_xfp_is_data_detected(cs_uint16 port_id)
/* INPUTS     : o Port Id                                               */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : TRUE or FALSE                                           */
/* DESCRIPTION:                                                         */
/* Return Status of the presence of data at equalizer input in 10G      */
/* serial interface.                                                    */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  LIF_STATUS2_t        status2 ;
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, 0);

  status2.wrd = M2R_REG_READ(&pDev->LIF.STATUS2.wrd) ;

  if (status2.bf.sXGDATA_DET) {
    return ( TRUE ) ;
  }
  else {
    return ( FALSE ) ;
  }
}

/************************************************************************/
/* $rtn_hdr_start   CONTROL LOCK DETECT MODE ALGORITHM                  */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_xfp_use_data_detect(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                               */
/*              o M2R_ENABLE (1) or M2R_DISABLE (0)                     */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Select lock detection mode. When ENABLED, SRX_LOCKDETI uses both     */
/* data detect and lock detect to determine it's value.                 */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  LIF_XGCNTL1_t        xgctl ;
  M2R_t              * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, 0);

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  xgctl.wrd = M2R_REG_READ(&pDev->LIF.XGCNTL1.wrd) ;
  xgctl.bf.USE_DATA_DET = (ctl == M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->LIF.XGCNTL1.wrd, xgctl.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);

  return (CS_OK) ;
}

/************************************************************************/
/* $rtn_hdr_start   SPECIFY TX REF CLK                                  */
/* CATEGORY   : API                                                     */
/* ACCESS     : Public                                                  */
/* BLOCK      : LIF                                                     */
/* CHIP       : MILAN2                                                  */
cs_status m2r_lif_xfp_use_clk_ref(cs_uint16 port_id,
			          m2r_lif_ref_clk_t tx_ckref,
			          m2r_lif_ref_clk_t tx_ckref_lptime)
/* INPUTS     : o Port Id                                               */
/*              o clk supplying to TX_CKREF_P/N, see table below        */
/*              o clk supplying to TX_CKREF_LPTIME_P/N, see table below */
/* OUTPUTS    : ----                                                    */
/* RETURNS    : CS_OK or CS_ERROR                                       */
/* DESCRIPTION:                                                         */
/* Tell driver what ref clk is used here so the clk divider can be set  */
/* accordingly. tx_ckref_lptime may be used for SONET only.             */
/* For SFI4.2/XAUI interface, ref clk should only be 155.52/156.25 MHz  */
/* respectively. No API call is necessary if SFI4.2/XAUI is selected.   */
/*                                                                      */
/* Use API m2r_lif_xfp_set_clk_mode() after this for the settings to    */
/* take effective.                                                      */
/*                                                                      */
/* [tx_ckref, tx_ckref_lptime]         SONET          10GE              */
/* ---------------------------------------------------------------      */
/* M2R_LIF_REF_CLK_622 (*)            622.08 MHz                        */
/* M2R_LIF_REF_CLK_155                155.52 MHz                        */
/* M2R_LIF_REF_CLK_644                               644.53 MHz         */
/* M2R_LIF_REF_CLK_161                               161.13 MHz         */
/* $rtn_hdr_end                                                         */
/************************************************************************/
{
  m2r_port_cb_t         *ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  if (ppcb == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_UNINITIALIZED, NULL) ;
    return (CS_ERROR) ;
  }

  ppcb->tx_ckref = tx_ckref;
  ppcb->tx_ckref_lptime = tx_ckref_lptime;
  ppcb->tx_ckref_custom = TRUE;
  return (CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start  RX_CLKOUT OUTPUT CONTROL                           */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : MILAN2                                                */
cs_status m2r_lif_ctl_rx_clkout(cs_uint16 port_id, m2r_ctl_t ctl,
				cs_boolean inverted)
/* INPUTS     : o Port Id                                             */
/*              o RX_CLKOUT control, M2R_ENABLE (1) or M2R_DISABLE (0)*/
/*              o Inverted output, TRUE (1*) or FALSE (0)             */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* RX clock output control                                            */
/* Setting ctl to M2R_ENABLE doesn't really enable the output,        */
/* Select valid rx_clkout will enable the output automatically.       */
/* With ctl == M2R_DISABLE, rx_clkout sel is set to all 1's(0x3f)     */
/*                                                                    */
/* Use API m2r_lif_sel_rx_clkout_common() or                          */
/* m2r_lif_sel_rx_clkout_raw() to select frequency before enable the  */
/* output.                                                            */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  LIF_RXADJUST4_t       rxAdj4 ;
  M2R_t                 * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  rxAdj4.wrd = M2R_REG_READ(&pDev->LIF.RXADJUST4.wrd) ;

  /* setting sel bits all 1's disable the RX output */
  if (ctl == M2R_DISABLE) rxAdj4.bf.RX_CLKOUT_SEL = 0x3f ;
  rxAdj4.bf.RX_CLK_INV = (inverted == TRUE) ? 1 : 0 ;

  M2R_REG_WRITE(&pDev->LIF.RXADJUST4.wrd, rxAdj4.wrd) ;
  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
  return (CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start  SELECT RX_CLKOUT FREQUENCY                         */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : MILAN2                                                */
cs_status m2r_lif_sel_rx_clkout_common(cs_uint16 port_id,
                                       m2r_lif_rx_clkout_sel_t sel)
/* INPUTS     : o Port Id                                             */
/*              o Selection, see table below                          */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Select commonly used clock frequency outputting to RX_CLKOUT       */
/* Use API m2r_lif_sel_rx_clkout_raw for all other selections         */
/* Use API m2r_lif_ctl_rx_clkout() to enable or disable the selected  */
/* output                                                             */
/*                                                                    */
/* [sel]                             description                      */
/* -----------------------------------------------------------------  */
/* M2R_LIF_RX_CLKOUT_15X_MHz         155.52 MHz for OC192 or          */
/*                                   161.13 MHz for 10GE              */
/* M2R_LIF_RX_CLKOUT_7X_MHz          77.76 MHz for OC192 or           */
/*                                   80.57 MHz for 10G)               */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  if (sel == M2R_LIF_RX_CLKOUT_15X_MHz) {
    return m2r_lif_sel_rx_clkout_raw(port_id, RX_CLKOUT_SEL_SEL_155_52);
  }
  else {
    return m2r_lif_sel_rx_clkout_raw(port_id, RX_CLKOUT_SEL_SEL_77_76);
  }
}


/**********************************************************************/
/* $rtn_hdr_start  SELECT RX_CLKOUT FREQUENCY AMONG ALL SELECTIONS    */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : MILAN2                                                */
cs_status m2r_lif_sel_rx_clkout_raw(cs_uint16 port_id,
                                    cs_uint16 sel)
/* INPUTS     : o Port Id                                             */
/*              o Selection, see Analog Spec for valid selections     */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Select one of all possible clock frequency outputting to RX_CLKOUT */
/* Use with care, no validation                                       */
/* Use API m2r_lif_ctl_rx_clkout() to enable or disable the selected  */
/* output                                                             */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  LIF_RXADJUST4_t       rxAdj4 ;
  M2R_t                  * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  rxAdj4.wrd = M2R_REG_READ(&pDev->LIF.RXADJUST4.wrd) ;

  rxAdj4.bf.RX_CLKOUT_SEL = sel;

  M2R_REG_WRITE(&pDev->LIF.RXADJUST4.wrd, rxAdj4.wrd) ;
  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
  return (CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start  TX_CLKOUT OUTPUT CONTROL                           */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : MILAN2                                                */
cs_status m2r_lif_ctl_tx_clkout(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                             */
/*              o M2R_ENABLE (1) or M2R_DISABLE (0)                   */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* TX clock output control                                            */
/*                                                                    */
/* Use API m2r_lif_sel_tx_clkout() to select frequency before enable  */
/* the output.                                                        */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  LIF_TXADJUST3_t        txAdj3 ;
  M2R_t                  * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  txAdj3.wrd = M2R_REG_READ(&pDev->LIF.TXADJUST3.wrd) ;

  txAdj3.bf.TX_CKOUT_EN = (ctl == M2R_ENABLE) ? 1 : 0 ;

  M2R_REG_WRITE(&pDev->LIF.TXADJUST3.wrd, txAdj3.wrd) ;
  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
  return (CS_OK) ;
}

/**********************************************************************/
/* $rtn_hdr_start  SELECT TX_CLKOUT FREQUENCY                         */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : MILAN2                                                */
cs_status m2r_lif_sel_tx_clkout(cs_uint16 port_id,
                                m2r_lif_tx_clkout_sel_t sel)
/* INPUTS     : o Port Id                                             */
/*              o Selection, see table below                          */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Select one of the possible clock frequency outputting to TX_CLKOUT */
/* Use with care, no validation.                                      */
/* Use API m2r_lif_ctl_tx_clkout() to enable or disable the selected  */
/* output                                                             */
/*                                                                    */
/* [sel]                           description                        */
/* ----------------------------------------------------------         */
/* M2R_LIF_TX_CLKOUT_STX_CLK         stx_clk (10G/32 = 311.04 MHz)    */
/* M2R_LIF_TX_CLKOUT_STX_REF (*)     stx_ref                          */
/* M2R_LIF_TX_CLKOUT_155_MHz         155.52 MHz clock                 */
/* M2R_LIF_TX_CLKOUT_10G             10G clock                        */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  LIF_TXADJUST3_t        txAdj3 ;
  M2R_t                  * pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR);

  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  txAdj3.wrd = M2R_REG_READ(&pDev->LIF.TXADJUST3.wrd) ;

  txAdj3.bf.TX_CLKOUT_SEL = sel;

  M2R_REG_WRITE(&pDev->LIF.TXADJUST3.wrd, txAdj3.wrd) ;
  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);
  return (CS_OK) ;
}


/*********************************************************************/
/* $rtn_hdr_start  CONTROL RECOVERED CLOCK TO BACK-PLANE             */
/* CATEGORY   : API                                                  */
/* ACCESS     : Public                                               */
/* BLOCK      : LIF                                                  */
/* CHIP       : MILAN2                                               */
cs_status m2r_lif_ctl_clk_to_backplane(cs_uint16 port_id,
				       cs_ctl_t los,
				       cs_ctl_t lof,
				       cs_ctl_t ais)
/* INPUTS     : o Port Id                                            */
/*              o Enable or disable clock on LOS condition           */
/*              o Enable or disable clock on LOF condition           */
/*              o Enable or disable clock on AIS condition           */
/* OUTPUTS    : ----                                                 */
/* RETURNS    : CS_OK or CS_ERROR                                    */
/* DESCRIPTION:                                                      */
/* Enable or disable the recovered clock(19MHz/78MHz) clock going    */
/* to the backplane WHEN an alarm condition(AIS, LOS, LOF) is        */
/* detected by the Framer.                                           */
/* This control is effective only when there is an alarm condition.  */
/* The clock is not squelched when there are no alarms.              */
/*                                                                   */
/* The 78MHz clock is sent to FRCLK78_N/P (LVDS) pins.               */
/* The 19MHz clock is sent to FRCLK19 (LVCMOS) pin.                  */
/* $rtn_hdr_end                                                      */
/*********************************************************************/
{
  LIF_CKMASK_t	ckmask ;
  M2R_t		* pDev ;

  M2R_LIF_GET_DEV_BASEADDR(port_id, pDev, CS_ERROR) ;
  M2R_PORT_LOCK(port_id, M2R_ID_LIF);

  ckmask.wrd = 0 ;
  ckmask.bf.MASK_LOS = (los == CS_ENABLE) ? 1 : 0 ;
  ckmask.bf.MASK_LOF = (lof == CS_ENABLE) ? 1 : 0 ;
  ckmask.bf.MASK_AIS = (ais == CS_ENABLE) ? 1 : 0 ;

  M2R_REG_WRITE(&pDev->LIF.CKMASK.wrd, ckmask.wrd) ;
  M2R_PORT_UNLOCK(port_id, M2R_ID_LIF);

  return (CS_OK) ;
}




/****************************************************************
 * Internal LIF functions
 ****************************************************************/

/****************************************************************
 * Internal function to get the Clock Divider values for XFP I/F.
 ****************************************************************/
static cs_uint8 m2r_lif_calc_xfp_clk_div(m2r_lif_ref_clk_t refClk)
{
  cs_uint8 div = 0 ;

  switch ( refClk ) {
    case M2R_LIF_REF_CLK_311 :
      div = 0x01 ;
      break ;
    case M2R_LIF_REF_CLK_155 :
      div = 0x00 ;
      break ;
    case M2R_LIF_REF_CLK_622 :
    default:
      div = 0x02 ;
      break ;
  }
  return (div) ;
}


/****************************************************************
 * This internal API sets the clock divider values based on
 * TX CLk Mode.
 ****************************************************************/
static cs_status m2r_lif_set_xfp_clk_div(cs_uint16 port_id, M2R_t * pDev,
                                  m2r_lif_tx_clk_mode_t tx_clk_mode)
{
  LIF_XGCNTL1_t         xgctl ;
  cs_uint8              div1, div2;
  m2r_lif_ref_clk_t     refClk ;
  m2r_port_cb_t         *ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  div1 = div2 = 2;
  refClk = ppcb->tx_ckref ;

  switch (tx_clk_mode) {
    case M2R_LIF_TX_CLK_LOCAL :
      if (xfp_ref_clk_is_155) { /* internal hack, cortina use only */
	refClk = M2R_LIF_REF_CLK_155 ;
      }
      div1 = div2 = m2r_lif_calc_xfp_clk_div(refClk) ;
    break ;

    case M2R_LIF_TX_CLK_LOOP_INTERNAL :
      if (xfp_ref_clk_is_155) { /* internal hack, cortina use only */
	refClk = M2R_LIF_REF_CLK_155 ;
      }
      div1   = m2r_lif_calc_xfp_clk_div(refClk) ;
      div2   = 0x2 ;
    break ;

    case M2R_LIF_TX_CLK_LOOP_EXTERNAL :
      div1   = m2r_lif_calc_xfp_clk_div(refClk) ;
      refClk = ppcb->tx_ckref_lptime ;
      div2   = m2r_lif_calc_xfp_clk_div(refClk) ;

    break ;
  }

  xgctl.wrd = M2R_REG_READ(&pDev->LIF.XGCNTL1.wrd) ;
  xgctl.bf.TX_CKREF_DIV1 = div1 ;
  xgctl.bf.TX_CKREF_DIV2 = div2 ;
  M2R_REG_WRITE(&pDev->LIF.XGCNTL1.wrd, xgctl.wrd) ;
  return (CS_OK) ;
}

/****************************************************************
 * Internal function to set the Clock Divider values for SFI4.2 I/F.
 ****************************************************************/
static cs_status m2r_lif_set_sfi42_clk_div(cs_uint16 port_id,
                       M2R_t * pDev, m2r_lif_tx_clk_mode_t tx_clk_mode)
{
  cs_uint8              div ;
  XGXS_MP_XAUI0_t       xauiCtrl ;
  LIF_SFI42CNTL_t       sfi4_2_ctl;
  m2r_lif_ref_clk_t     refClk ;
  m2r_port_cb_t         *ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;

  refClk = ppcb->tx_ckref;

  if ( M2R_IS_SONET_MODE(port_id) == FALSE ) {
    /* XAUI */
    switch ( refClk ) {
      case M2R_LIF_REF_CLK_156 :
        div = 0x00 ; break ;

      case M2R_LIF_REF_CLK_312 :
        div = 0x01 ; break ;

      case M2R_LIF_REF_CLK_625 :
        div = 0x02 ; break ;

      default :
        CS_HNDL_ERROR(port_id, EM2R_LIF_INVALID_REF_CLK, NULL ) ;
        return ( CS_ERROR ) ;
    }

    xauiCtrl.wrd = M2R_REG_READ(&pDev->LIF.XGXS.XAUI0.wrd) ;
    xauiCtrl.bf.xaui_tx_ckref_d = div ;
    M2R_REG_WRITE(&pDev->LIF.XGXS.XAUI0.wrd, xauiCtrl.wrd) ;
  }
  else {
    /* SFI42 */
    switch ( refClk ) {
      case M2R_LIF_REF_CLK_78 :
        div = 0x00 ; break ;

      case M2R_LIF_REF_CLK_155 :
        div = 0x01 ; break ;

      case M2R_LIF_REF_CLK_311 :
        div = 0x02 ; break ;

      case M2R_LIF_REF_CLK_622 :
        div = 0x03 ; break ;
      default :
        CS_HNDL_ERROR(port_id, EM2R_LIF_INVALID_REF_CLK, NULL ) ;
        return ( CS_ERROR ) ;
    }
    sfi4_2_ctl.wrd = M2R_REG_READ(&pDev->LIF.SFI42CNTL.wrd) ;
    sfi4_2_ctl.bf.TXCKREF_DIV = div ;
    M2R_REG_WRITE(&pDev->LIF.SFI42CNTL.wrd, sfi4_2_ctl.wrd) ;
  }
  return (CS_OK) ;
}

static cs_status m2r_lif_xfp_analog_init(cs_uint16 port_id, M2R_t * pDev)
{
  LIF_TXADJUST3_t txAdj3 ;
  LIF_TXADJUST4_t txAdj4 ;
  LIF_TXADJUST5_t txAdj5 ;
  LIF_RXADJUST5_t rxAdj5 ;
  LIF_RXADJUST6_t rxAdj6 ;
  LIF_RXADJUST7_t rxAdj7 ;

  txAdj3.wrd = M2R_REG_READ(&pDev->LIF.TXADJUST3.wrd) ;
  txAdj4.wrd = M2R_REG_READ(&pDev->LIF.TXADJUST4.wrd) ;
  txAdj5.wrd = M2R_REG_READ(&pDev->LIF.TXADJUST5.wrd) ;
  rxAdj5.wrd = M2R_REG_READ(&pDev->LIF.RXADJUST5.wrd) ;
  rxAdj6.wrd = M2R_REG_READ(&pDev->LIF.RXADJUST6.wrd) ;
  rxAdj7.wrd = M2R_REG_READ(&pDev->LIF.RXADJUST7.wrd) ;


  txAdj4.bf.TX_FREQ_THRESH = 0x2a;
  rxAdj7.bf.RX_FREQ_THRESH = 0x2a;

  if (m2r_mpif_is_in_oc48_mode(port_id) == TRUE) {
    txAdj3.bf.TX_LEVEL   = 0x16 ;
    /* For CORTINA ATE use
    txAdj3.bf.TX_PEAK    = 0 ;
     */

    txAdj5.bf.TX_LOWERCM = 0 ;

    rxAdj6.bf.RX_DIVSEL  = 0 ;

    rxAdj5.bf.RX_PHSEL   = 0x0f ;
    rxAdj5.bf.RX_CPCURR  = 0x5 ;
  }
  else { /* OC192 or 10 GE Mode */
    /* new TX_LEVEL default according to Kenji, 1/12/05 */
	txAdj3.bf.TX_LEVEL   = 0x0c ;
	
    /* For CORTINA ATE use
    txAdj3.bf.TX_PEAK    = 0x2 ;
     */
	 
    rxAdj5.bf.RX_PHSEL   = 0x0c ;
  }
  rxAdj5.bf.RX_FILTINT = 1 ;
  rxAdj5.bf.RX_FILTEN  = 0 ;

  M2R_REG_WRITE(&pDev->LIF.TXADJUST3.wrd, txAdj3.wrd) ;
  M2R_REG_WRITE(&pDev->LIF.TXADJUST4.wrd, txAdj4.wrd) ;
  M2R_REG_WRITE(&pDev->LIF.TXADJUST5.wrd, txAdj5.wrd) ;
  M2R_REG_WRITE(&pDev->LIF.RXADJUST5.wrd, rxAdj5.wrd) ;
  M2R_REG_WRITE(&pDev->LIF.RXADJUST6.wrd, rxAdj6.wrd) ;
  M2R_REG_WRITE(&pDev->LIF.RXADJUST7.wrd, rxAdj7.wrd) ;

  return (CS_OK) ;
}

/*
 * Set some LIF registers to default values for Milan2
 * according to spec 4.9.2.4
 */
cs_status  m2r_lif_setup_default(cs_uint16 port_id,
				 m2r_port_summ_t * pSumm)
{
  M2R_t                 * pDev ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;

  switch (pSumm->lif_mode) {
    case M2R_PORT_IF_XFP:
      m2r_lif_select_interface(port_id, M2R_LIF_INTERFACE_XFP) ;
      break;

    case M2R_PORT_IF_SFI4_2:
      m2r_lif_select_interface(port_id, M2R_LIF_INTERFACE_SFI42) ;
      break;

    default:
      m2r_lif_select_interface(port_id, M2R_LIF_INTERFACE_SFI41) ;
      break;
  }

  CS_MDELAY(2) ;

  /* write defaults to RW registers */
  /* read to clear RW1C registers   */
  M2R_REG_WRITE(&pDev->LIF.PRBS_CNTL.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PRBSPROV.wrd, 0) ;
  M2R_REG_READ(&pDev->LIF.PRBSCNT_MSB.wrd) ;
  M2R_REG_READ(&pDev->LIF.PRBSCNT_LSB.wrd) ;

  M2R_REG_WRITE(&pDev->LIF.PCS6.TxCntrl.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.TxInt.wrd, 0x7) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.TxIntMask.wrd, 0x7) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_SOS_A.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_SOS_B.wrd, 0) ;
  M2R_REG_READ(&pDev->LIF.PCS6.tx_GoodPkt.wrd) ;
  M2R_REG_READ(&pDev->LIF.PCS6.tx_ErrPkt.wrd) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_A_3.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_A_2.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_A_1.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_A_0.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_B_3.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_B_2.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_B_1.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_B_0.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_D_3.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_D_2.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_D_1.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.tx_Seed_D_0.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.FMRO.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.RxCntrl.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.RxInt.wrd, 0xfff) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.RxIntMask.wrd, 0xfff) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_SOS_A.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_SOS_B.wrd, 0) ;
  M2R_REG_READ(&pDev->LIF.PCS6.rx_GoodPkt.wrd) ;
  M2R_REG_READ(&pDev->LIF.PCS6.rx_ErrPkt.wrd) ;
  M2R_REG_READ(&pDev->LIF.PCS6.ErrPat.wrd) ;
  M2R_REG_READ(&pDev->LIF.PCS6.ErrDec.wrd) ;
  M2R_REG_READ(&pDev->LIF.PCS6.ErrBer.wrd) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.SynOff.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_A_3.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_A_2.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_A_1.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_A_0.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_B_3.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_B_2.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_B_1.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_B_0.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_D_3.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_D_2.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_D_1.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.PCS6.rx_Seed_D_0.wrd, 0) ;

  M2R_REG_WRITE(&pDev->LIF.XGXS.TxCntrl0.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.TxCntrl1.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.TxCntrl2.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.TxCntrl3.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.TxCntrl4.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.TxCntrl5.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.TxCntrl6.wrd, 0x3) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.TxCntrl7.wrd, 0x2111) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.TxInt.wrd, 0x7) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.TxIntMask.wrd, 0x7) ;
  M2R_REG_READ(&pDev->LIF.XGXS.tx_GoodPkt.wrd) ;
  M2R_REG_READ(&pDev->LIF.XGXS.tx_ErrPkt.wrd) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.RxCntrl0.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.RxCntrl1.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.RxCntrl2.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.RxCntrl3.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.RxCntrl4.wrd, 0) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.RxInt.wrd, 0x3f7f) ;
  M2R_REG_WRITE(&pDev->LIF.XGXS.RxIntMask.wrd, 0x3f7f) ;
  M2R_REG_READ(&pDev->LIF.XGXS.rx_GoodPkt.wrd) ;
  M2R_REG_READ(&pDev->LIF.XGXS.rx_ErrPkt.wrd) ;
  M2R_REG_READ(&pDev->LIF.XGXS.ICC0.wrd) ;
  M2R_REG_READ(&pDev->LIF.XGXS.ICC1.wrd) ;
  M2R_REG_READ(&pDev->LIF.XGXS.ICC2.wrd) ;
  M2R_REG_READ(&pDev->LIF.XGXS.ICC3.wrd) ;
  M2R_REG_READ(&pDev->LIF.XGXS.PEC0.wrd) ;
  M2R_REG_READ(&pDev->LIF.XGXS.PEC1.wrd) ;
  M2R_REG_READ(&pDev->LIF.XGXS.PEC2.wrd) ;
  M2R_REG_READ(&pDev->LIF.XGXS.PEC3.wrd) ;

  return (CS_OK) ;
}












