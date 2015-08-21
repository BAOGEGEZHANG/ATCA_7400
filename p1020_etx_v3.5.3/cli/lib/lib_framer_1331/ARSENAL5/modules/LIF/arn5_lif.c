/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_lif.c
 *
 * ARN5 driver libraries pertaining to Line Interface(LIF) block
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "arn5_error.h"
#include "arn5_common.h"
#include "arn5_registers.h"
#include "arn5_public.h"
#include "arn5_lif.h"
#include "arn5_lif_priv.h"

/*
 * Internal function prototypes *
 */
static
cs_uint8  arn5_lif_get_STX_CKREF_DIV1(arn5_lif_ref_clk_t refClk) ;
static
cs_uint8  arn5_lif_get_STX_CKREF_DIV2(arn5_lif_ref_clk_t refClk,
                        arn5_lif_tx_clk_mode_t mode) ;
static
cs_status  arn5_lif_ctl_clk(ARN5_t * pDev, cs_uint16 sl_num,
                        cs_uint8 clk, cs_ctl_t ctl) ;


/**********************************************************************/
/*    Macros and defines                                              */
/**********************************************************************/
#define ARN5_LIF_GET_DEVICE_ADDR(PORTID, BASE_ADDR, SLICE, RETCODE) \
{                                                                   \
  cs_uint32 error;                                                  \
  if(ARN5_IS_PORT_VALID(PORTID, &error) != TRUE) {                  \
    CS_HNDL_ERROR((PORTID), error, ": port_id= %d\n", (PORTID));    \
    return (RETCODE) ;                                              \
  }                                                                 \
  (BASE_ADDR) = ARN5_PORT_ID_TO_DEV_BASE((PORTID));                 \
  (SLICE)     = ARN5_PORT_ID_TO_SLICE_NUM((PORTID));                \
}

/*--------------------------------------------------------------------*/
/* Hard coded value and Assumptions :                                 */
/* The freq of STX_CLKREF_P/N is hard coded here. It is a recommended */
/* value, 155.52 MHz for OC48/OC12/OC3,  156.25 MHz for GigE          */
/* The freq of STX_CKREF_LPTIME_P/N is assumed to be the above at all */
/* times.                                                             */
/*--------------------------------------------------------------------*/
#define ARN5_LIF_GET_REF_CLK_VAL(PORTID)     (ARN5_LIF_REF_CLK_155)


/**********************************************************************/
/* $_hdr_start    CONFIGURE LINE INTERFACE(LIF)                       */
/* CATEGORY   : API                                                   */
/* ACCESS     : Private                                               */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status  arn5_lif_init_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg)
/* INPUTS     : o Chip Id                                             */
/*              o Pointer to port configuration struct                */
/*                                                                    */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* This initializes the port level of the LIF block                   */
/* with the given summary.                                            */
/*                                                                    */
/* The [pPortCfg] parameter is a pointer to the configuration         */
/* data-structure, which has all the configuration parameters         */
/* set.                                                               */
/* The API expects the data-structure to be already allocated         */
/* and populated with the configuration settings.                     */
/* $_hdr_end                                                          */
/**********************************************************************/
{
  ARN5_t                * pDev ;
  ARN5_LIF_SERDES_CONFIGURATION_t       serdes ;
  cs_uint8              sl_num ;
  cs_uint32             usec_delay = 100, usec_timeout = 1000 ;
  cs_uint32             usec_elapsed = 0;
  arn5_lif_cfg_t        * pCfg  = &pPortCfg->lif ;

  //CS_PRINT("\t Initializing LIF... \n") ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);

  ARN5_PORT_LOCK(port_id, ARN5_ID_LIF) ;
  serdes.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.SERDES_CONFIGURATION.wrd) ;
  serdes.bf.STX_E_CKREF_SEL =
    (pPortCfg->summ.line_rate == ARN5_PORT_RATE_GIGE) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.SERDES_CONFIGURATION.wrd, serdes.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF) ;

  if (arn5_lif_ctl_rx_clkout(port_id, pCfg->rx_clkout_ctl)
      != CS_OK) {
    return CS_ERROR ;
  }

  if (arn5_lif_sel_rx_clkout_raw(port_id, pCfg->rx_clkout_sel)
      != CS_OK) {
    return CS_ERROR ;
  }

  if (arn5_lif_ctl_tx_clkout(port_id, pCfg->tx_clkout_ctl)
      != CS_OK) {
    return CS_ERROR ;
  }

  if (arn5_lif_sel_tx_clkout(port_id, pCfg->tx_clkout_sel)
      != CS_OK) {
    return CS_ERROR ;
  }

  if (arn5_lif_ctl_loop_timing(port_id, pCfg->tx_clk_mode)
      != CS_OK) {
    return CS_ERROR ;
  }

  if (pCfg->terminal_loopback) {
    if (pCfg->analog_term_lb) {
      arn5_lif_ctl_analog_loopback(port_id, CS_ENABLE);
    }
    else {
      arn5_lif_ctl_terminal_loopback(port_id, CS_ENABLE);
    }
  }
  else
  if (pCfg->line_loopback) {
    arn5_lif_ctl_line_loopback(port_id, CS_ENABLE) ;
  }

  /* Soft-reset LIF before starting traffic */
  if (arn5_lif_set_sreset(port_id)
      != CS_OK) {
    return CS_ERROR ;
  }

  while ( !(arn5_lif_is_sreset_done(port_id)) ) {
    if (usec_elapsed >= usec_timeout) break ;
    CS_UDELAY(usec_delay) ;
    usec_elapsed += usec_delay ;
  }

  /* Init IRQ Tables and enable default IRQs */
  if (arn5_lif_init_irq_cfg(port_id, pCfg)
      != CS_OK ) {
    return (CS_ERROR) ;
  }

  return(CS_OK) ;
}


/**********************************************************************/
/* $_hdr_start    GET DEFAULT LIF CONFIGURATION                       */
/* CATEGORY   : API                                                   */
/* ACCESS     : Private                                               */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status  arn5_lif_get_default_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg)
/* INPUTS     : o Device Id                                           */
/*              o Pointer to configuration structure                  */
/* OUTPUTS    : o LIF related default config                          */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Get the port's default LIF configuration based on specified        */
/* summary config info.                                               */
/*                                                                    */
/* The [pPortCfg] parameter is a pointer to the configuration         */
/* data-structure. The API will fill in this structure with           */
/* default settings based on the config summary info.                 */
/*                                                                    */
/* The API expects the config summary filled.                         */
/* $_hdr_end                                                          */
/**********************************************************************/
{
  arn5_port_summ_t      * pSumm = &pPortCfg->summ ;
  arn5_lif_cfg_t        * pCfg  = &pPortCfg->lif ;

  pCfg->ref_clk = ARN5_LIF_GET_REF_CLK_VAL(port_id) ;
  pCfg->tx_clk_mode = ARN5_LIF_TX_CLK_LOCAL;
  pCfg->rx_clkout_ctl = CS_DISABLE;
  pCfg->rx_clkout_sel = 0x13;
  pCfg->tx_clkout_ctl = CS_DISABLE;
  pCfg->tx_clkout_sel = ARN5_LIF_TX_CLKOUT_155_MHz;
  pCfg->line_loopback = FALSE ;
  pCfg->terminal_loopback = FALSE ;
  pCfg->analog_term_lb = FALSE ;

  /* Set the default IRQ Map for LIF */
  arn5_lif_get_default_irq_cfg(port_id, pSumm, pCfg) ;

  return(CS_OK) ;
}


/**********************************************************************/
/* $_hdr_start    GET RUNNING LIF CONFIGURATION                       */
/* CATEGORY   : API                                                   */
/* ACCESS     : Private                                               */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status  arn5_lif_get_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg)
/* INPUTS     : o Device Id                                           */
/*              o Pointer to running configuration structure          */
/* OUTPUTS    : o LIF port related running config                     */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Returns the current run-time LIF hardware configuration            */
/* of the specified port.                                             */
/* The [pRunCfg] parameter is a pointer to the port's complete        */
/* configuration data-structure.                                      */
/*                                                                    */
/* This API will fill in the all the fields in the block              */
/* run-time configuration datastructure.                              */
/* $_hdr_end                                                          */
/**********************************************************************/
{
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;
  ARN5_LIF_GP_CTRL_t    gpCtrl ;
  arn5_port_summ_t      * pSumm ;
  arn5_lif_runcfg_t     * pCfg ;

  pSumm = &pRunCfg->summ ;
  pCfg = &pRunCfg->lif ;
  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);

  pCfg->ref_clk = ARN5_LIF_GET_REF_CLK_VAL(port_id);

  /* tx_clk_mode; look for some signatures to determine the mode */
  {
    gpCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.GP_CTRL.wrd);
    if (gpCtrl.bf.STX_CKREF_SEL == 1) {
      pCfg->tx_clk_mode = ARN5_LIF_TX_CLK_LOOP_EXTERNAL;
    }
    else {
      ARN5_LIF_TEST_CTRL_t      testCtrl;
      testCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.TEST_CTRL.wrd) ;
      pCfg->tx_clk_mode = (testCtrl.bf.STX_TCKI_E == 0) ?
        ARN5_LIF_TX_CLK_LOCAL : ARN5_LIF_TX_CLK_LOOP_INTERNAL;
    }
  }

  /* rx_clkout_ctl, rx_clkout_sel, tx_clkout_ctl, tx_clkout_sel */
  {
    ARN5_LIF_CLKOUT_CTRL_t  clkoutCtrl ;
    clkoutCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.CLKOUT_CTRL.wrd) ;
    pCfg->rx_clkout_ctl = (clkoutCtrl.bf.SRX_CLKOUT_E == 1) ?
                          TRUE : FALSE;
    pCfg->rx_clkout_sel = clkoutCtrl.bf.SRX_CLKOUT_SEL;
    pCfg->tx_clkout_ctl = (clkoutCtrl.bf.STX_CLKOUT_E == 1) ?
                          TRUE : FALSE;
    pCfg->tx_clkout_sel = clkoutCtrl.bf.STX_CLKOUT_SEL;
  }

  /* div1 & div2 */
  {
    ARN5_LIF_TIMING_CTRL_t       tmCtrl ;
    tmCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.TIMING_CTRL.wrd) ;
    pCfg->div1 = tmCtrl.bf.STX_CKREF_DIV1;
    pCfg->div2 = tmCtrl.bf.STX_CKREF_DIV2;
  }

  /* line_loopback, terminal_loopback, analog term loopback */
  {
    pCfg->line_loopback     = (gpCtrl.bf.LINE_LOOP_BK == 1) ?
                              TRUE : FALSE;
    pCfg->terminal_loopback = (gpCtrl.bf.TERM_LP_BK == 1) ?
                              TRUE : FALSE;
    pCfg->analog_term_lb    = (gpCtrl.bf.FE_TERM_LP_BK == 1) ?
                              TRUE : FALSE;
  }
  return(CS_OK) ;
}


/**********************************************************************/
/* $_hdr_start   DUMP RUNNING LIF CONFIGURATION                       */
/* CATEGORY   : API                                                   */
/* ACCESS     : Private                                               */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status  arn5_lif_dump_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                             */
/*              o Pointer to running configuration structure          */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Prints the current run-time hardware configuration for the         */
/* this block.                                                        */
/*                                                                    */
/* The [pRunCfg] parameter is a pointer to the running                */
/* configuration data-structure allocated by the caller with          */
/* the configuration data from HW already retrieved and filled        */
/* in, prior to calling this API.                                     */
/* $_hdr_end                                                          */
/**********************************************************************/
{
  char                  * pS ;
  arn5_lif_runcfg_t     * pCfg = &pRunCfg->lif ;

  switch (pCfg->ref_clk) {
    case ARN5_LIF_REF_CLK_155 :
      pS = "155.52 MHz";
      break;

    case ARN5_LIF_REF_CLK_78 :
      pS = "77.76 MHz";
      break;

    case ARN5_LIF_REF_CLK_311 :
      pS = "311.04 MHz";
      break;

    case ARN5_LIF_REF_CLK_622 :
      pS = "622.08 MHz";
      break;

    default :
      pS = "Unknown";
      break;
  }
  CS_PRINT("Ref Clk                 = %s\n",
           pS);

  switch (pCfg->tx_clk_mode) {
    case ARN5_LIF_TX_CLK_LOCAL :
      pS = "Local";
      break;

    case ARN5_LIF_TX_CLK_LOOP_INTERNAL :
      pS = "Internal Loop";
      break;

    case ARN5_LIF_TX_CLK_LOOP_EXTERNAL :
      pS = "External Loop";
      break;

    default :
      pS = "Unknown";
      break;
  }
  CS_PRINT("Tx Clock Mode           = %s\n",
           pS);

  CS_PRINT("Clk Div1/2              = 0x%x/0x%x\n",
           pCfg->div1, pCfg->div2);

  CS_PRINT("Rx Clkout Ctrl          = %s\n",
           pCfg->rx_clkout_ctl ? "Enabled" : "Disabled");

  CS_PRINT("Rx Clkout Sel           = 0x%x\n",
           pCfg->rx_clkout_sel);

  CS_PRINT("Tx Clkout Ctrl          = %s\n",
           pCfg->tx_clkout_ctl ? "Enabled" : "Disabled");

  CS_PRINT("Tx Clkout Sel           = 0x%x\n",
           pCfg->tx_clkout_sel);

  CS_PRINT("Analog Terminal Lpbk    = %s\n",
           pCfg->analog_term_lb ? "Enabled" : "Disabled");

  CS_PRINT("Line Lpbk               = %s\n",
           pCfg->line_loopback ? "Enabled" : "Disabled");

  CS_PRINT("Terminal Lpbk           = %s\n",
           pCfg->terminal_loopback ? "Enabled" : "Disabled");

  return(CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start    LIF SOFT RESET                                   */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status  arn5_lif_set_sreset(cs_uint16 port_id)
/* INPUTS     : o Port Id                                             */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Put Line Interface(LIF) block in Soft/Logic Reset.                 */
/* The CPU access registers are unaffected.                           */
/*                                                                    */
/* The reset bit in LIF register is self-clearing and will be         */
/* cleared after all the LIF logic reset has been done.               */
/*                                                                    */
/* The caller is responsible to ensure that soft reset is             */
/* complete by calling arn5_lif_is_sreset_done() and checking          */
/* for a TRUE return value before proceeding further in               */
/* configuring LIF registers.                                         */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint16     sl_num ;
  ARN5_LIF_RESET_t      reset ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  reset.wrd = 0 ;

  if (!CS_IN_TEST_ENV()) {
    reset.bf.TX_RST = 1 ;
    reset.bf.RX_RST = 1 ;
  }
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.RESET.wrd, reset.wrd) ;

  return(CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start    CHECK LIF SOFT RESET STATUS                      */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_boolean  arn5_lif_is_sreset_done(cs_uint16 port_id)
/* INPUTS     : o Port Id                                             */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : TRUE(reset done) or FALSE(still in reset)             */
/* DESCRIPTION:                                                       */
/* Returns TRUE if reset of LIF logic is complete or FALSE if         */
/* LIF logic is still in reset state.                                 */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;
  ARN5_LIF_RESET_t      srst ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);

  srst.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.RESET.wrd) ;

  if ( srst.bf.TX_RST || srst.bf.RX_RST ) return (FALSE) ;
  else return (TRUE) ;
}


/**********************************************************************/
/* $rtn_hdr_start    CONFIGURES THE TX CLOCK TIMING                   */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status  arn5_lif_ctl_loop_timing(cs_uint16 port_id,
                        arn5_lif_tx_clk_mode_t tx_clk_mode)
/* INPUTS     : o Port Id                                             */
/*              o TX Clock Mode                                       */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Configures the TX clock timing.                                    */
/*                                                                    */
/* The [tx_clk_mode] parameter is one of the following:               */
/* ARN5_LIF_TX_CLK_LOCAL or ARN5_LIF_TX_CLK_LOOP_INTERNAL or          */
/* ARN5_LIF_TX_CLK_LOOP_EXTERNAL                                      */
/*                                                                    */
/* While selecting ARN5_LIF_TX_CLK_LOOP_EXTERNAL, user might need to  */
/* select and enable RX_CLKOUT first.                                 */
/* See also arn5_lif_ctl_rx_clkout, arn5_lif_sel_rx_clkout_commmon    */
/*          arn5_lif_sel_rx_clkout_raw                                */
/*                                                                    */
/* LIF Soft-reset will be hit in this API.                            */
/* It is also required to hit Framer logic reset whenever loop_timing */
/* mode is changed.                                                   */
/* For Framer reset, use API arn5_framer_ctl_domain_sreset(), and     */
/* select all domains, ARN5_FR_ALL_DOMAINS.  Toggle the reset control */
/* with minimum of 10us in between.                                   */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_t                        * pDev ;
  arn5_lif_ref_clk_t            refClk ;
  cs_uint16                     sl_num ;
  cs_uint8                      tx_clk_div1, tx_clk_div2, clk_mask = 0;
  ARN5_LIF_TIMING_CTRL_t        tmCtrl ;
  ARN5_LIF_GP_CTRL_t            gpCtrl ;
  cs_uint16                     rxVcoTune2, rxLoopFilter ;
  cs_uint16                     rxChargePump, testCtrl ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);

  if (arn5_mpif_is_in_gige_mode(port_id) &&
      (tx_clk_mode != ARN5_LIF_TX_CLK_LOCAL)) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
        ": GIGE only works with local timing mode.\n") ;
    return (CS_ERROR) ;
  }

  ARN5_PORT_LOCK(port_id, ARN5_ID_LIF);

  /*
   * Get the current configured value of the reference clock.
   */
  refClk = ARN5_LIF_GET_REF_CLK_VAL(port_id) ;

  rxVcoTune2 = 0x735 ;
  testCtrl = 0 ;
  rxLoopFilter = 0xa3 ;
  rxChargePump = 0xf2f ;

  if (arn5_mpif_is_in_oc12_mode(port_id)) {
    if (tx_clk_mode == ARN5_LIF_TX_CLK_LOOP_INTERNAL) {
      rxVcoTune2 = 0xf35 ;
      testCtrl = 1 ;
      rxLoopFilter = 0x93 ;
      rxChargePump = 0xfbf ;
    }
  }
  else if (arn5_mpif_is_in_oc48_mode(port_id)) {
    if (tx_clk_mode == ARN5_LIF_TX_CLK_LOOP_INTERNAL) {
      rxVcoTune2 = 0xf34 ;
      testCtrl = 1 ;
      rxLoopFilter = 0x13 ;
      rxChargePump = 0xfbf ;
    }
  }
  else if (arn5_mpif_is_in_oc3_mode(port_id)) {
    rxLoopFilter = 0xd3 ;
    rxChargePump = 0xfbf ;
    if (tx_clk_mode == ARN5_LIF_TX_CLK_LOOP_INTERNAL) {
      rxVcoTune2 = 0xf35 ;
      testCtrl = 1 ;
    }
  }
  else {
    /* GigeE */
  }

  CS_REG_WRITE(&pDev->slice[sl_num].LIF.RX_VCO_TUNE2.wrd, rxVcoTune2) ;
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.TEST_CTRL.wrd, testCtrl) ;
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.RX_LOOP_FILTER.wrd, rxLoopFilter) ;
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.RX_CHARGE_PUMP.wrd, rxChargePump) ;

  /*
   * Check if div1 or div2 needs to be changed. If so, modify them.
   */
  tx_clk_div1 = arn5_lif_get_STX_CKREF_DIV1(refClk) ;
  tx_clk_div2 = arn5_lif_get_STX_CKREF_DIV2(refClk, tx_clk_mode) ;
  tmCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.TIMING_CTRL.wrd) ;
  if (tmCtrl.bf.STX_CKREF_DIV1 != tx_clk_div1) {
    clk_mask = (ARN5_LIF_TX_CLK | ARN5_LIF_LD_CLK) ;
  }
  else if (tmCtrl.bf.STX_CKREF_DIV2 != tx_clk_div2) {
    clk_mask = (ARN5_LIF_TX_CLK) ;
  }

  if ( clk_mask ) {
    arn5_lif_ctl_clk(pDev, sl_num, clk_mask, CS_DISABLE ) ;
    tmCtrl.bf.STX_CKREF_DIV1 = tx_clk_div1 ;
    tmCtrl.bf.STX_CKREF_DIV2 = tx_clk_div2 ;
    CS_REG_WRITE(&pDev->slice[sl_num].LIF.TIMING_CTRL.wrd, tmCtrl.wrd) ;
    arn5_lif_ctl_clk(pDev, sl_num, clk_mask, CS_ENABLE ) ;
  }

  gpCtrl.wrd  = CS_REG_READ(&pDev->slice[sl_num].LIF.GP_CTRL.wrd) ;
  gpCtrl.bf.STX_CKREF_SEL =
      (tx_clk_mode == ARN5_LIF_TX_CLK_LOOP_EXTERNAL) ? 1 : 0 ;
  gpCtrl.bf.STX_LPTIME_E = 0 ; /* always */
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.GP_CTRL.wrd, gpCtrl.wrd) ;

  /* set this value to TX_OUTPUT_CTRL regardless mode selection */
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.TX_OUTPUT_CTRL.wrd, 0x43) ;

  /* hit soft reset to let LIF start a new life */
  arn5_lif_set_sreset(port_id) ;
  CS_UDELAY(10) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF);

  return(CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start  LIF LINE LOOPBACK                                  */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status  arn5_lif_ctl_line_loopback(cs_uint16 port_id, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                             */
/*              o Loopback control(CS_ENABLE or CS_DISABLE)           */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Enable or disable line loopback at the Line Interface. The         */
/* Rx data is looped back to Tx.                                      */
/* User should enable the loop time mode by calling                   */
/* arn5_lif_ctl_loop_timing API().                                    */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_LIF_GP_CTRL_t    gpctrl ;
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PORT_LOCK(port_id, ARN5_ID_LIF);
  gpctrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.GP_CTRL.wrd) ;
  gpctrl.bf.LINE_LOOP_BK = (ctl == CS_ENABLE) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.GP_CTRL.wrd, gpctrl.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF);

  return(CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start  LIF TERMINAL LOOPBACK                              */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status  arn5_lif_ctl_terminal_loopback(cs_uint16 port_id, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                             */
/*              o Loopback control(CS_ENABLE or CS_DISABLE)           */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Enable or disable digital terminal loopback at the Line Interface. */
/* NOTE : before enabling terminal loopback, loop timing should be    */
/* disabled by calling the arn5_lif_ctl_loop_timing() API.            */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_LIF_GP_CTRL_t    gpctrl ;
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PORT_LOCK(port_id, ARN5_ID_LIF);
  gpctrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.GP_CTRL.wrd) ;
  gpctrl.bf.TERM_LP_BK = (ctl == CS_ENABLE) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.GP_CTRL.wrd, gpctrl.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF);
  return (CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start  LIF ANALOG TERMINAL LOOPBACK                       */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status  arn5_lif_ctl_analog_loopback(cs_uint16 port_id, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                             */
/*              o Loopback control(CS_ENABLE or CS_DISABLE)           */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Enable or disable analog terminal loopback at the Line Interface.  */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_LIF_GP_CTRL_t    gpctrl ;
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PORT_LOCK(port_id, ARN5_ID_LIF);
  gpctrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.GP_CTRL.wrd) ;
  gpctrl.bf.FE_TERM_LP_BK = (ctl == CS_ENABLE) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.GP_CTRL.wrd, gpctrl.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF);
  return (CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start   ENABLE/DISABLE SONET UNFRAMERD PRBS               */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status  arn5_lif_ctl_prbs(cs_uint16 port_id, cs_ctl_t tx_ctl,
                        cs_ctl_t rx_ctl, cs_boolean tx_invert,
                        cs_boolean rx_invert, arn5_lif_prbs_t tx_ptrn,
                        arn5_lif_prbs_t rx_ptrn)
/* INPUTS     : o Port Id                                             */
/*              o CS_DISABLE (default) or CS_ENABLE                   */
/*              o CS_DISABLE (default) or CS_ENABLE                   */
/*              o FALSE or TRUE                                       */
/*              o FALSE or TRUE                                       */
/*              o TX PRBS pattern                                     */
/*              o RX PRBS pattern                                     */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Enable/Disable the PRBS on LIF interfaces.                         */
/*                                                                    */
/* [tx_invert]/[rx_invert] indicates whether to invert the transmit or*/
/* receive PRBS pattern.                                              */
/*                                                                    */
/* The [tx_ptrn]/[rx_ptrn] parameter is one of the following:         */
/* ARN5_LIF_PRBS_PATTERN_31 or ARN5_LIF_PRBS_PATTERN_23 or            */
/* ARN5_LIF_PRBS_PATTERN_15 or ARN5_LIF_PRBS_PATTERN_7                */
/*                                                                    */
/* NOTE : UNFRAMERD PRBS feature can be used for internal/external    */
/* loopback diagnostics and SONET compliance test as well.            */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_LIF_PRBS_PROV_t  prbs_prvsn ;
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PORT_LOCK(port_id, ARN5_ID_LIF);
  prbs_prvsn.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.PRBS_PROV.wrd) ;
  prbs_prvsn.bf.PRBS_TX_SEL = tx_ptrn ;
  prbs_prvsn.bf.PRBS_RX_SEL = rx_ptrn ;
  prbs_prvsn.bf.PRBS_TX_INVERT = ( tx_invert ) ? 1 : 0 ;
  prbs_prvsn.bf.PRBS_RX_INVERT = ( rx_invert ) ? 1 : 0 ;
  prbs_prvsn.bf.PRBS_TX_ENABLE = ( tx_ctl == CS_ENABLE ) ? 1 : 0 ;
  prbs_prvsn.bf.PRBS_RX_ENABLE = ( rx_ctl == CS_ENABLE ) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.PRBS_PROV.wrd, prbs_prvsn.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF);
  return (CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE RX_CLKOUT                           */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status arn5_lif_ctl_rx_clkout(cs_uint16 port_id, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                             */
/*              o RX_CLKOUT control(CS_ENABLE or CS_DISABLE)          */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Enable/Disable the RX clock output                                 */
/*                                                                    */
/* Use API arn5_lif_sel_rx_clkout_common() or                         */
/* arn5_lif_sel_rx_clkout_raw() to select frequency before enable the */
/* output.                                                            */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_LIF_CLKOUT_CTRL_t clkoutCtrl ;
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);

  ARN5_PORT_LOCK(port_id, ARN5_ID_LIF);
  clkoutCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.CLKOUT_CTRL.wrd) ;
  clkoutCtrl.bf.SRX_CLKOUT_E = (ctl == CS_ENABLE) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.CLKOUT_CTRL.wrd, clkoutCtrl.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF);
  return (CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start  SELECT RX_CLKOUT FREQUENCY                         */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status arn5_lif_sel_rx_clkout_common(cs_uint16 port_id,
                                      arn5_lif_rx_clkout_sel_t sel)
/* INPUTS     : o Port Id                                             */
/*              o Selection, ARN5_LIF_RX_CLKOUT_15X_MHz or            */
/*                           ARN5_LIF_RX_CLKOUT_7X_MHz                */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Select commonly used clock frequency outputting to RX_CLKOUT       */
/* Use API arn5_lif_sel_rx_clkout_raw() for all other selections      */
/* Use API arn5_lif_ctl_rx_clkout() to enable or disable the selected */
/* output                                                             */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_LIF_CLKOUT_CTRL_t clkoutCtrl ;
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);

  ARN5_PORT_LOCK(port_id, ARN5_ID_LIF);
  clkoutCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.CLKOUT_CTRL.wrd) ;
  clkoutCtrl.bf.SRX_CLKOUT_SEL = (sel == ARN5_LIF_RX_CLKOUT_15X_MHz) ?
                                           0x13 : 0x1b;
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.CLKOUT_CTRL.wrd, clkoutCtrl.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF);
  return (CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start  SELECT RX_CLKOUT FREQUENCY AMONG ALL SELECTIONS    */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status arn5_lif_sel_rx_clkout_raw(cs_uint16 port_id,
                                     cs_uint16 sel)
/* INPUTS     : o Port Id                                             */
/*              o Selection, see table below                          */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Select one of all possible clock frequency outputting to RX_CLKOUT */
/* Use with care, no validation.                                      */
/* Use API arn5_lif_ctl_rx_clkout() to enable or disable the selected */
/* output.                                                            */
/*                                                                    */
/* [sel], the value is mode dependent as listed below.                */
/* sel value    OC48 mode     OC12 mode     OC3 mode      1GE mode    */
/* -------------------------------------------------------------------*/
/* 0x03        622.08  MHz   622.08  MHz   622.08  MHz   625      MHz */
/* 0x0b        311.04  MHz   311.04  MHz   311.04  MHz   312.5    MHz */
/* 0x13 (*)    155.52  MHz   155.52  MHz   155.52  MHz   156.25   MHz */
/* 0x1b         77.76  MHz    77.76  MHz    77.76  MHz    78.125  MHz */
/* 0x33         38.88  MHz    38.88  MHz    38.88  MHZ    39.0625 MHz */
/* 0x3b SRX_CLK(77.76  MHz)  (19.44  MHz)   (4.86  MHz) (125      MHz)*/
/* 0x3d          2.488 GHz     2.488 GHz     2.488 GHz     2.5    GHz */
/* 0x3e          2.5   Gb/s  622     Mb/s  155     Mb/s    1.25   Gb/s*/
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_LIF_CLKOUT_CTRL_t  clkoutCtrl ;
  ARN5_t                  * pDev ;
  cs_uint16               sl_num ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);

  ARN5_PORT_LOCK(port_id, ARN5_ID_LIF);
  clkoutCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.CLKOUT_CTRL.wrd) ;
  clkoutCtrl.bf.SRX_CLKOUT_SEL = sel;
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.CLKOUT_CTRL.wrd, clkoutCtrl.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF);
  return (CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE TX_CLKOUT                           */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status arn5_lif_ctl_tx_clkout(cs_uint16 port_id, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                             */
/*              o TX_CLKOUT control(CS_ENABLE or CS_DISABLE)          */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Enable/Disable the TX clock output                                 */
/*                                                                    */
/* Use API arn5_lif_sel_tx_clkout() to select frequency before enable */
/* the output.                                                        */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_LIF_CLKOUT_CTRL_t clkoutCtrl ;
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);

  ARN5_PORT_LOCK(port_id, ARN5_ID_LIF);
  clkoutCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.CLKOUT_CTRL.wrd) ;
  clkoutCtrl.bf.STX_CLKOUT_E = (ctl == CS_ENABLE) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.CLKOUT_CTRL.wrd, clkoutCtrl.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF);
  return (CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start  SELECT TX_CLKOUT FREQUENCY                         */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status arn5_lif_sel_tx_clkout(cs_uint16 port_id,
                                 arn5_lif_tx_clkout_sel_t sel)
/* INPUTS     : o Port Id                                             */
/*              o Selection, see table below                          */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Select one of the possible clock frequency outputting to TX_CLKOUT */
/* Use with care, no validation.                                      */
/* Use API arn5_lif_ctl_tx_clkout() to enable or disable the selected */
/* output                                                             */
/*                                                                    */
/* [sel]                            description                       */
/* ------------------------------------------------------------------ */
/* ARN5_LIF_TX_CLKOUT_STX_CLK       stx_clk                           */
/* ARN5_LIF_TX_CLKOUT_STX_PILOT     stx_pilot                         */
/* ARN5_LIF_TX_CLKOUT_155_MHz (*)   155.52 MHz(SONET)/156.25 MHz(ETH) */
/* ARN5_LIF_TX_CLKOUT_High_Speed    high speed clock                  */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_LIF_CLKOUT_CTRL_t clkoutCtrl ;
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);

  ARN5_PORT_LOCK(port_id, ARN5_ID_LIF);
  clkoutCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.CLKOUT_CTRL.wrd) ;
  clkoutCtrl.bf.STX_CLKOUT_SEL = sel & 0x3;
  CS_REG_WRITE(&pDev->slice[sl_num].LIF.CLKOUT_CTRL.wrd, clkoutCtrl.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF);
  return (CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start   INJECT ERRORS IN PRBS                             */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_status arn5_lif_inj_error(cs_uint16 port_id, arn5_lif_prbs_err_t err,
                             cs_boolean once, cs_uint16 duration)
/* INPUTS     : o Port Id                                             */
/*              o ERROR TYPE ( 1 BIT ) OR BLOCK ERROR                 */
/*              o Once ( TRUE ) or continuously ( FALSE )             */
/*              o Durtaion of error injection(if doing continously)   */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : CS_OK or CS_ERROR                                     */
/* DESCRIPTION:                                                       */
/* Injects error in the PRBS stream.                                  */
/* The [err] parameter is one of the following:                       */
/* ARN5_LIF_PRBS_1_BIT_ERR or ARN5_LIF_PRBS_BLOCK_ERR                 */
/* Note : This function might block for "duration" if continuous      */
/*  errors need to be generated.                                      */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;
  ARN5_LIF_PRBS_CTRL_t  prbsCtrl ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  prbsCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.PRBS_CTRL.wrd) ;
  ARN5_PORT_LOCK(port_id, ARN5_ID_LIF);

  switch ( err ) {
    case ARN5_LIF_PRBS_BLOCK_ERR:
      if(once) {
        prbsCtrl.bf.PRBS_WORD_ERR_INJ_ONCE = 1 ;
      }
      else {
        prbsCtrl.bf.PRBS_WORD_ERR_INJ = 1 ;
      }
      break ;

    case ARN5_LIF_PRBS_1_BIT_ERR:
      if(once) {
        prbsCtrl.bf.PRBS_BIT_ERR_INJ_ONCE = 1 ;
      }
      else {
        prbsCtrl.bf.PRBS_BIT_ERR_INJ = 1 ;
      }
      break ;

    default :
      ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF);
      CS_HNDL_ERROR(port_id, EARN5_LIF_INVALID_ERROR_OP, "\n" ) ;
      return (CS_ERROR) ;
  }

  CS_REG_WRITE(&pDev->slice[sl_num].LIF.PRBS_CTRL.wrd, prbsCtrl.wrd) ;

  if(!once) {
    CS_UDELAY(duration) ;
    if ( err == ARN5_LIF_PRBS_BLOCK_ERR ) {
      prbsCtrl.bf.PRBS_WORD_ERR_INJ = 0 ;
    }
    else {
      prbsCtrl.bf.PRBS_BIT_ERR_INJ = 0 ;
    }
    CS_REG_WRITE(&pDev->slice[sl_num].LIF.PRBS_CTRL.wrd, prbsCtrl.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF);
  return (CS_OK) ;
}


/**********************************************************************/
/* $rtn_hdr_start   GET PRBS ERROR COUNT                              */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_uint32 arn5_lif_get_prbs_errors(cs_uint16 port_id)
/* INPUTS     : o Port Id                                             */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : PRBS ERROR COUNT                                      */
/* DESCRIPTION:                                                       */
/* Gets the current PRBS error count since the time PRBS was enabled. */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint16     sl_num ;
  cs_uint16     cnt_lsb, cnt_msb ;

  ARN5_LIF_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR);
  ARN5_PORT_LOCK(port_id, ARN5_ID_LIF);

  cnt_msb = CS_REG_READ(&pDev->slice[sl_num].LIF.PRBS_CNT_MSB.wrd) ;
  cnt_lsb = CS_REG_READ(&pDev->slice[sl_num].LIF.PRBS_CNT_LSB.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_LIF);
  return ((cnt_msb << 16) | cnt_lsb) ;
}


/**********************************************************************/
/* $rtn_hdr_start  GET THE LIF STATUS                                 */
/* CATEGORY   : API                                                   */
/* ACCESS     : Public                                                */
/* BLOCK      : LIF                                                   */
/* CHIP       : Arsenal5                                              */
cs_uint32 arn5_lif_get_status(cs_uint16 port_id, arn5_lif_status_t sel)
/* INPUTS     : o Port Id                                             */
/*              o Status selection                                    */
/* OUTPUTS    : ----                                                  */
/* RETURNS    : Bitmap status                                         */
/* DESCRIPTION:                                                       */
/* Get the current status of the LIF interface.                       */
/* The [sel] parameter is one of the following:                       */
/* LIF_PRBS_RX_EFs ,   LIF_PRBS_SYNCs    ,   LIF_NO_DATAs ,           */
/* LIF_IN_LOCKs    ,   LIF_SRX_LOCKDETIs ,   LIF_STATUS_ALL           */
/*                                                                    */
/* LIF_STATUS_ALL : Read the whole status register                    */
/* $rtn_hdr_end                                                       */
/**********************************************************************/
{
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;
  ARN5_LIF_STATUS_t     lif_status ;
  cs_uint32             status = 0 ;

  lif_status.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.STATUS.wrd) ;

  switch ( sel ) {
    case LIF_PRBS_RX_EFs:
      status = lif_status.bf.PRBS_RX_EFs ;
      break ;

    case LIF_PRBS_SYNCs:
      status = lif_status.bf.PRBS_SYNCs ;
      break ;

    case LIF_NO_DATAs:
      status = lif_status.bf.NO_DATAs ;
      break ;

    case LIF_IN_LOCKs:
      status = lif_status.bf.IN_LOCKs ;
      break ;

    case LIF_SRX_LOCKDETIs:
      status = lif_status.bf.SRX_LOCKDETIs ;
      break ;

    case LIF_STATUS_ALL:
      status = lif_status.wrd ;
      break ;
  }
  return (status) ;
}


/***********************************************************
 *        INTERNAL UTILITIES AND FUNCTIONS                 *
 ***********************************************************/
/*
 * Get the value of STX_CKREF_DIV1 for the given reference clock.
 */
static
cs_uint8  arn5_lif_get_STX_CKREF_DIV1(arn5_lif_ref_clk_t refClk)
{
  cs_uint8  div1 ;

  switch (refClk) {
    case ARN5_LIF_REF_CLK_155 :
      div1 = 0x01 ;
      break ;

    default :
      div1 = 0x01 ;
      break ;
  }
  return div1 ;
}


/*
 * Get the value of STX_CKREF_DIV2 for the given reference clock.
 */
static
cs_uint8  arn5_lif_get_STX_CKREF_DIV2(arn5_lif_ref_clk_t refClk,
                                      arn5_lif_tx_clk_mode_t mode )
{
  cs_uint8  div2 ;

  switch (refClk) {
    case ARN5_LIF_REF_CLK_155 :
      div2 = 0x01 ;
      break ;

    default :
      div2 = 0x01 ;
      break ;
  }
  return div2 ;
}


static
cs_status arn5_lif_ctl_clk(ARN5_t * pDev, cs_uint16 sl_num,
                        cs_uint8 clk, cs_ctl_t ctl)
{
  ARN5_LIF_CLOCK_ENABLE_t   clkCtrl ;

  clkCtrl.wrd = CS_REG_READ(&pDev->slice[sl_num].LIF.CLOCK_ENABLE.wrd) ;
  if (clk & ARN5_LIF_RX_CLK) {
    clkCtrl.bf.RX_CLK_E = ( ctl == CS_DISABLE ) ? 0 : 1 ;
  }

  if (clk & ARN5_LIF_TX_CLK) {
    clkCtrl.bf.TX_CLK_E = ( ctl == CS_DISABLE ) ? 0 : 1 ;
  }

  if (clk & ARN5_LIF_LD_CLK) {
      clkCtrl.bf.LD_CLK_E = ( ctl == CS_DISABLE ) ? 0 : 1 ;
  }

  CS_REG_WRITE(&pDev->slice[sl_num].LIF.CLOCK_ENABLE.wrd, clkCtrl.wrd) ;
  return (CS_OK) ;
}

