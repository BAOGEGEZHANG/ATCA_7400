/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_spi42.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 * Milan2 only API's for SPI4.2
 */

#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"

#include "m2r_spi42.h"
#include "m2r_spi42_irq.h"
#include "m2r_spi42_priv.h"


extern const m2r_irq_ctl_t  m2r_spi42_irq_tbl[M2R_SPI42_MAX_GRP] ;

/****************************************************************/
/* $rtn_hdr_start  SPI42 INITIALIZATION     			*/
/* CATEGORY   : Port						*/
/* ACCESS     : public						*/
/* BLOCK      : SPI42                                           */
/* CHIP	      : Milan2                                          */
cs_status m2r_spi42_init_cfg(cs_uint16 port_id,
			     m2r_port_summ_t * pSumm,
			     m2r_spi42_cfg_t * pCfg)
/* INPUTS     : o Port Id					*/
/* 	        o Pointer to port configuration summary struct  */
/* 		o Pointer to SPI42 configuration structure      */
/* OUTPUTS    : ----						*/
/* RETURNS    : CS_OK or CS_ERROR				*/
/* DESCRIPTION:						        */
/* This initializes the SPI42 block of the specified port       */
/* with the given configuration.                                */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure, which has all the configuration parameters   */
/* set.                                                         */
/* The API expects the data-structure to be already allocated   */
/* and populated with the configuration settings.               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	ii ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if (pCfg == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, ": SPI42 cfg\n") ;
    return(CS_ERROR) ;
  }

  //CS_PRINT("\t Initializing SPI4.2... \n") ;
  if ( m2r_spi42_set_gen_char(port_id, pCfg->gen_char.stat_lvds,
			      pCfg->gen_char.dynalign,
			      pCfg->gen_char.quarter_rate)
       == CS_ERROR ) {
    return(CS_ERROR) ;
  }

  if ( m2r_spi42_set_rdclk_mult(port_id, pCfg->rdclk_mult)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  for ( ii = 0; ii < 4; ii++) {
    if ( m2r_spi42_atm_mode(port_id, ii, pCfg->atm_mode[ii])
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if ( m2r_spi42_set_burst_params(port_id, pCfg->burst_params.burst_size,
				  pCfg->burst_params.maxburst1,
				  pCfg->burst_params.maxburst2)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_spi42_set_training_params(port_id, pCfg->training.alpha,
				    pCfg->training.datamaxt)
      == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_spi42_set_dip2_tholds(port_id, pCfg->dip2.error_thold,
                                 pCfg->dip2.match_thold)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_spi42_set_dip4_tholds(port_id, pCfg->dip4.error_thold,
                                 pCfg->dip4.match_thold)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_spi42_set_tx_sched_mode(port_id, pCfg->tx_sched.mode,
				   pCfg->tx_sched.threshlo,
				   pCfg->tx_sched.cal_burst)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_spi42_set_tx_fifo_wmarks(port_id, pCfg->tx_fifo_wmarks.low,
				    pCfg->tx_fifo_wmarks.high)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_spi42_set_rx_fifo_wmarks(port_id, pCfg->rx_fifo_wmarks.high)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_spi42_set_cal_params(port_id, M2R_TX, pCfg->tx_cal.m,
				pCfg->tx_cal.len, pCfg->tx_cal.ent0,
				pCfg->tx_cal.ent1, pCfg->tx_cal.ent2,
				pCfg->tx_cal.ent3)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if ( m2r_spi42_set_cal_params(port_id, M2R_RX, pCfg->rx_cal.m,
				pCfg->rx_cal.len, pCfg->rx_cal.ent0,
				pCfg->rx_cal.ent1, pCfg->rx_cal.ent2,
				pCfg->rx_cal.ent3)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  if (pCfg->rx_lpbk) { /* LB loopback */
    if ( m2r_spi42_set_loopback(port_id, M2R_RX, TRUE)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  /*
   * Done with configuring the registers. SRESET(logic reset) the
   * block.
   */
  if ( m2r_spi42_sreset(port_id) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* Init IRQ's - register table and selectively enable leaf IRQ's */
  if ( m2r_spi42_init_irq(port_id, pCfg->irq_ctl_tbl)
       == CS_ERROR ) {
    return (CS_ERROR) ;
  }


  /* Init Stats */
  {
    cs_uint16 num_channels ;

    num_channels = ( (pCfg->rx_cal.len > pCfg->tx_cal.len) ?
		     (pCfg->rx_cal.len + 1) :
		     (pCfg->tx_cal.len + 1) ) ;
    if ( m2r_spi42_init_stats(port_id, num_channels) ==
	 CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  GET DEFAULT SPI42 CONFIGURATION           	*/
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP	      : Milan2                                          */
cs_status m2r_spi42_get_default_cfg(cs_uint16 port_id,
				    m2r_port_summ_t * pSumm,
				    m2r_spi42_cfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port config summary info           */
/* 		o Pointer to SPI42 configuration structure    	*/
/* OUTPUTS    : o SPI4.2 default config                         */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the driver's default SPI42 configuration for the          */
/* specified port and for the specified summary config info.    */
/*                                                              */
/* The [pSumm] parameter is a pointer to the configuration      */
/* Summary section, which has the caller provided config summary*/
/* info. This is used in returning the appropriate default      */
/* configuration for the SPI42 block.                           */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure. The API will fill in this structure with     */
/* default settings based on the config summary info.           */
/*                                                              */
/* The API expects both datastrutues(pSumm and pCfg) already    */
/* allocated, and the config summary filled in pSumm.           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32	ii ;

  if (pCfg == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, ": SPI42 cfg\n") ;
    return(CS_ERROR) ;
  }

  /* general(electrical) characterstics */
  pCfg->gen_char.stat_lvds    = TRUE ;
  pCfg->gen_char.dynalign     = TRUE ; /* ALWAYS use dynamic alignmt */

  if ( (pSumm->line_rate == M2R_PORT_RATE_OC48C) ||
       (pSumm->line_rate == M2R_PORT_RATE_OC48) ) {
    pCfg->gen_char.quarter_rate = TRUE ;
  }
  else {
    pCfg->gen_char.quarter_rate = FALSE ;
  }

  pCfg->rdclk_mult = 4 ; /* RDCLK = 4RDREFCLK */

  /* ATM mode */
  for (ii = 0; ii < 4; ii++) {
    pCfg->atm_mode[ii] = (pSumm->l2_prot == M2R_PORT_PROT_ATM) ? TRUE :
                        FALSE ;
  }

  /* burst parameters */
  pCfg->burst_params.burst_size   = 3 ;  /* (3+1) * 16 = 64 bytes */
  pCfg->burst_params.maxburst1    = 2 ;  /* (2+1) * burst_size */
  pCfg->burst_params.maxburst2    = 2 ;  /* (2+1) * burst_size */

  /* Training */
  pCfg->training.alpha        = 3 ;
  pCfg->training.datamaxt     = (1000 * pCfg->training.alpha);

  /* DIP-2 and DIP-4 thresholds */
  pCfg->dip2.error_thold = 1 ;
  pCfg->dip2.match_thold = 4 ;
  pCfg->dip4.error_thold = 1 ;
  pCfg->dip4.match_thold = 0x0c80 ;

  /* If framer in channelized mode, then use 4 channel RR mode */
  if ( (pSumm->line_rate == M2R_PORT_RATE_OC192) ||
       (pSumm->line_rate == M2R_PORT_RATE_OC48) ) { /* channelized */
    pCfg->tx_sched.mode = M2R_SPI42_4CH_RR_CUT_THRU ;
  }

  /* If in SRP/RPR mode, then use 4 channel prio mode */
  else if ( (pSumm->mac_mode == M2R_PORT_IN_SRP_MODE) ||
	    (pSumm->mac_mode == M2R_PORT_IN_RPR_MODE) ) {
    pCfg->tx_sched.mode = M2R_SPI42_4CH_PRIO_ST_FWD ;
  }

  /* otherwise, use 1 ch cut-thru as default */
  else {
    pCfg->tx_sched.mode = M2R_SPI42_1CH_CUT_THRU ;
  }

  /*
   * The threshlo value is set to a high value so as not to underflow
   * the framer FIFO, when operating in cut-thru mode(this value is
   * not used in store & forward mode). Also, set cal_burst
   * should be non-zero when in 4 channel cut-thru mode. Since these
   * settings are ignored for other modes, set them always.
   */
  pCfg->tx_sched.threshlo = 0x0100 ;
  pCfg->tx_sched.cal_burst = 6 ;

  pCfg->tx_fifo_wmarks.low  = 0x0200 ;
  pCfg->tx_fifo_wmarks.high = 0x0400 ;
  /* only high watermark is valid for Rx FIFO */
  pCfg->rx_fifo_wmarks.high = 100 ;

  /* Tx calendar */
  pCfg->tx_cal.m    = 0 ;
  if ( (pCfg->tx_sched.mode == M2R_SPI42_4CH_RR_CUT_THRU) ||
       (pCfg->tx_sched.mode == M2R_SPI42_4CH_PRIO_ST_FWD) ) {
    pCfg->tx_cal.len  = 3 ;	/* 4 channels */
  }
  else {
    pCfg->tx_cal.len  = 0 ; /* single channel */
  }
  pCfg->tx_cal.ent0 = 0 ;
  pCfg->tx_cal.ent1 = 1 ;
  pCfg->tx_cal.ent2 = 2 ;
  pCfg->tx_cal.ent3 = 3 ;

  /* Rx calendar */
  pCfg->rx_cal = pCfg->tx_cal ;

  /* Loopbacks */
  pCfg->tx_lpbk = FALSE ; /* BERT */
  pCfg->rx_lpbk = FALSE ; /* LB   */

  /* Set the default IRQs */
  pCfg->irq_ctl_tbl[M2R_GRP_SPI42_RXINTRPTS] =
    M2R_GRP_SPI42_RXINTRPTS_ALL ;
  pCfg->irq_ctl_tbl[M2R_GRP_SPI42_TXINTRPTS] =
    M2R_GRP_SPI42_TXINTRPTS_ALL ;
  pCfg->irq_ctl_tbl[M2R_GRP_SPI42_TXFIFOINTRPTS] =
    M2R_GRP_SPI42_TXFIFOINTRPTS_ALL ;
  pCfg->irq_ctl_tbl[M2R_GRP_SPI42_RXINTRPTS2] =
    M2R_GRP_SPI42_RXINTRPTS2_ALL ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  GET SPI42 RUNNING CONFIGURATION           	*/
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP	      : Milan2                                          */
cs_status  m2r_spi42_get_running_cfg(cs_uint16 port_id,
				     m2r_port_summ_t * pRunSumm,
				     m2r_spi42_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port running config summary info   */
/*		o Pointer to SPI42 running config structure     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Returns the current run-time SPI42 hardware configuration    */
/* of the specified port.                                       */
/*                                                              */
/* The [pRunSumm] parameter is a pointer to the running config  */
/* summary section.                                             */
/*                                                              */
/* The [pRunCfg] parameter is a pointer to the block's          */
/* configuration data-structure.                                */
/*                                                              */
/* This API will fill in the all the fields in the block        */
/* run-time configuration datastructure.                        */
/* The caller is responsible for allocating memory for the      */
/* configuration structures.                                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;

  if ( (pRunSumm == NULL) || (pRunCfg == NULL) ) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_INVALID_USER_ARG,
		   "pRunSumm = 0x%x, pRunCfg = 0x%x\n",
		   pRunSumm, pRunCfg) ;
    return (CS_ERROR) ;
  }
  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;

  /* Interface sync status */
  pRunCfg->tx_sync_state = m2r_spi42_get_sync_state(port_id, CS_TX) ;
  pRunCfg->rx_sync_state = m2r_spi42_get_sync_state(port_id, CS_RX) ;

  /* RDCLK scale factor */
  {
    SPI4P2_RXPLLCONTROL1_t  rxpllcontrol1 ;

    rxpllcontrol1.wrd = M2R_REG_READ(&pDev->SPI4P2.RXPLLCONTROL1.wrd) ;
    switch (rxpllcontrol1.bf.MULTSEL) {
      default :
      case 0 : pRunCfg->rdclk_mult = 1 ; break ;
      case 1 : pRunCfg->rdclk_mult = 2 ; break ;
      case 2 : pRunCfg->rdclk_mult = 4 ; break ;
    }
  }

  /* Burst parameters */
  {
    m2r_spi42_burst_params_t  * p = &pRunCfg->burst_params ;

    p->burst_size = M2R_REG_READ(&pDev->SPI4P2.BURSTSIZE.wrd) ;
    p->maxburst1  = M2R_REG_READ(&pDev->SPI4P2.MAXBURST1.wrd) ;
    p->maxburst2  = M2R_REG_READ(&pDev->SPI4P2.MAXBURST2.wrd) ;
  }

  /* Training parameters */
  {
    m2r_spi42_training_t  * p = &pRunCfg->training ;

    p->alpha    = M2R_REG_READ(&pDev->SPI4P2.ALPHA.wrd) ;
    p->datamaxt = M2R_REG_READ(&pDev->SPI4P2.DATAMAXT.wrd) ;
  }

  /* Tx Scheduler mode */
  {
    m2r_spi42_tx_sched_t  * p = &pRunCfg->tx_sched ;
    cs_reg		  bits ;

    bits = M2R_REG_READ(&pDev->SPI4P2.TXCH_MODE.wrd) ;
    switch (bits) {
      default                       :
      case VALUE_mode_onech: p->mode = M2R_SPI42_1CH_CUT_THRU;    break ;
      case VALUE_mode_stfwd: p->mode = M2R_SPI42_1CH_ST_FWD;      break ;
      case VALUE_mode_oc48 : p->mode = M2R_SPI42_4CH_RR_CUT_THRU; break ;
      case VALUE_mode_stops: p->mode = M2R_SPI42_4CH_PRIO_ST_FWD; break ;
    }
    p->threshlo = M2R_REG_READ(&pDev->SPI4P2.TXCH_THRESHLO.wrd) ;
    p->cal_burst= M2R_REG_READ(&pDev->SPI4P2.TXCAL_BURST.wrd) ;
  }

  /*  Tx and Rx FIFO watermarks(thresholds) */
  {
    pRunCfg->tx_fifo_wmarks.low = M2R_REG_READ(&pDev->
					      SPI4P2.TXFIFOALMOSTEMPTY.wrd) ;
    pRunCfg->tx_fifo_wmarks.high =  M2R_REG_READ(&pDev->
						SPI4P2.TXFIFOALMOSTFULL.wrd) ;
    /* only almost full for Rx */
    pRunCfg->rx_fifo_wmarks.high = M2R_REG_READ(&pDev->
					       SPI4P2.RXFIFOALMOSTFULL.wrd) ;
  }

  /* Tx calendar */
  {
    m2r_spi42_cal_params_t   * p = &pRunCfg->tx_cal ;
    SPI4P2_TXCAL_t           txcal ;

    p->m   = M2R_REG_READ(&pDev->SPI4P2.TXCAL_M.wrd) ;
    p->len = M2R_REG_READ(&pDev->SPI4P2.TXCAL_LEN.wrd) ;
    txcal.wrd = M2R_REG_READ(&pDev->SPI4P2.TXCAL.wrd) ;
    p->ent0 = txcal.bf.ENT0 ;
    p->ent1 = txcal.bf.ENT1 ;
    p->ent2 = txcal.bf.ENT2 ;
    p->ent3 = txcal.bf.ENT3 ;
  }

  /* Rx calendar */
  {
    m2r_spi42_cal_params_t   * p = &pRunCfg->rx_cal ;
    SPI4P2_RXCAL_t           rxcal ;

    p->m   = M2R_REG_READ(&pDev->SPI4P2.RXCAL_M.wrd) ;
    p->len = M2R_REG_READ(&pDev->SPI4P2.RXCAL_LEN.wrd) ;
    rxcal.wrd = M2R_REG_READ(&pDev->SPI4P2.RXCAL.wrd) ;
    p->ent0 = rxcal.bf.ENT0 ;
    p->ent1 = rxcal.bf.ENT1 ;
    p->ent2 = rxcal.bf.ENT2 ;
    p->ent3 = rxcal.bf.ENT3 ;
  }

  /* Loopbacks */
  {
    SPI4P2_LOOPBACKS_t    lpbk ;

    lpbk.wrd = M2R_REG_READ(&pDev->SPI4P2.LOOPBACKS.wrd) ;
    pRunCfg->tx_lpbk =
      (lpbk.bf.BERTLB) ? TRUE : FALSE ;
    pRunCfg->rx_lpbk =
      (lpbk.bf.LB) ? TRUE : FALSE ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  DUMP SPI4.2 RUNNING CONFIGURATION 	       	*/
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP	      : MILAN2                                          */
cs_status m2r_spi42_dump_running_cfg(cs_uint16 port_id,
				     m2r_spi42_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                       */
/*		o Pointer to SPI42 config structure             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Prints the current run-time hardware configuration for the   */
/* this block.                                                  */
/*                                                              */
/* The [pRunCfg] parameter is a pointer to the running          */
/* configuration data-structure allocated by the caller with    */
/* the configuration data from HW already retrieved and filled  */
/* in, prior to calling this API.                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  if ( pRunCfg == NULL ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, NULL) ;
    return (CS_ERROR) ;
  }

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  /* Interface sync status */
  CS_PRINT("SPI interface status    = Tx: %s,  Rx: %s\n",
	   (pRunCfg->tx_sync_state == M2R_SPI42_IN_SYNC) ? "in-sync" :
	   "no sync!!",
	   (pRunCfg->rx_sync_state == M2R_SPI42_IN_SYNC) ? "in-sync" :
	   "no sync!!") ;

  /* RDCLK */
  CS_PRINT("RDCLK                   = %d * RDREFCLK\n",
	   pRunCfg->rdclk_mult) ;

  /* Burst parameters */
  {
    m2r_spi42_burst_params_t  * p = &pRunCfg->burst_params ;

    CS_PRINT("\n\t *** Burst Parameters ***\n") ;
    CS_PRINT("Burst Size             = 0x%x (or 0x%x bytes)\n",
	     p->burst_size, (p->burst_size + 1) * 16) ;
    CS_PRINT("Maxburst1              = 0x%x (%d x Burst Size)\n",
	     p->maxburst1, p->maxburst1 + 1) ;
    CS_PRINT("Maxburst2              = 0x%x (%d x Burst Size)\n",
	     p->maxburst2, p->maxburst2 + 1) ;
  }

  /* Training parameters */
  {
    m2r_spi42_training_t * p = &pRunCfg->training ;

    CS_PRINT("\n\t *** Training Parameters ***\n") ;
    CS_PRINT("Alpha                  = 0x%x (num repetitions)\n",
	     p->alpha) ;
    CS_PRINT("DataMaxt               = 0x%x (interval)\n",
	     p->datamaxt) ;
  }

  /* Tx Scheduler mode */
  {
    m2r_spi42_tx_sched_t  * p = &pRunCfg->tx_sched ;
    char * str ;

    CS_PRINT("\n\t *** Tx Scheduler ***\n") ;
    switch (p->mode) {
      default                       :
      case M2R_SPI42_1CH_CUT_THRU   :
	str = "Cut-thru(single-channel)" ;        break ;
      case M2R_SPI42_1CH_ST_FWD     :
	str = "Store-n-Forward(single-channel)" ; break ;
      case M2R_SPI42_4CH_RR_CUT_THRU:
	str = "Cut-thru(4-channel)" ;        break ;
      case M2R_SPI42_4CH_PRIO_ST_FWD:
	str = "Store-n-Forward(4-channel)" ; break ;
    }
    CS_PRINT("Mode                   = %s\n", str) ;
    CS_PRINT("Cut-thru low threshold = 0x%x\n",
	     p->threshlo) ;
    CS_PRINT("Tx cal burst           = 0x%x (valid only for 4ch cut-thru)\n",
	     p->cal_burst) ;
  }

  CS_PRINT("\n\t *** FIFO Watermarks(Thresholds) ***\n") ;
  CS_PRINT("Tx FIFO Almost Empty   = 0x%x (or 0x%x Bytes)\n",
	   pRunCfg->tx_fifo_wmarks.low, 
	   pRunCfg->tx_fifo_wmarks.low * 16) ; /* 128-bit boundary */ 
  CS_PRINT("Tx FIFO Almost Full    = 0x%x (or 0x%x Bytes)\n",
	   pRunCfg->tx_fifo_wmarks.high, 
	   pRunCfg->tx_fifo_wmarks.high * 16) ; /* 128-bit boundary */ 
  CS_PRINT("Rx FIFO Almost Full    = 0x%x (or 0x%x Bytes)\n",
	   pRunCfg->rx_fifo_wmarks.high,
	   pRunCfg->rx_fifo_wmarks.high * 8) ; /* 64-bit boundary */  

  /* Tx calendar */
  {
    m2r_spi42_cal_params_t  * p = &pRunCfg->tx_cal ;

    CS_PRINT("\n\t *** Tx Calendar ***\n") ;
    CS_PRINT("m (num of repetitions) = %d (reg value= %d)\n",
	     p->m + 1, p->m) ;
    CS_PRINT("Calendar length        = %d (reg value= %d)\n",
	     p->len + 1, p->len) ;
    CS_PRINT("Calendar entries       = {%d, %d, %d, %d}\n",
	     p->ent0, p->ent1, p->ent2, p->ent3) ;
  }

  /* Rx calendar */
  {
    m2r_spi42_cal_params_t  * p = &pRunCfg->rx_cal ;

    CS_PRINT("\n\t *** Rx Calendar ***\n") ;
    CS_PRINT("m (num of repetitions) = %d (reg value= %d)\n",
	     p->m + 1, p->m) ;
    CS_PRINT("Calendar length        = %d (reg value= %d)\n",
	     p->len + 1, p->len) ;
    CS_PRINT("Calendar entries       = {%d, %d, %d, %d}\n",
	     p->ent0, p->ent1, p->ent2, p->ent3) ;
  }

  /* Loopbacks */
  CS_PRINT("\n\t *** Loopbacks ***\n") ;
  CS_PRINT("Tx-to-Rx loopback      = %s\n",
           (pRunCfg->tx_lpbk) ? "ON" : "OFF") ;
  CS_PRINT("Rx-to-Tx loopback      = %s\n",
           (pRunCfg->rx_lpbk) ? "ON" : "OFF") ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SPI42 SRESET                                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP       : Milan2                                          */
cs_status  m2r_spi42_sreset(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Soft reset the entire SPI4.2 block. All the configuration    */
/* registers are unaffected.                                    */
/*                                                              */
/* The reset bit is SELF CLEARING. This API will poll for the   */
/* the reset bit to clear before returning.                     */
/* A timeout of 20 ms is provided to prevent any indefinite     */
/* blocking by this API.                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;
  SPI4P2_CONTROL_t	ctl ;
  cs_uint32             usec_delay = 100, usec_timeout = 20000 ;
  cs_uint32             usec_elapsed = 0;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  M2R_PORT_LOCK(port_id, M2R_ID_SPI42) ;

  ctl.wrd = M2R_REG_READ(&pDev->SPI4P2.CONTROL.wrd) ;

#ifndef RELEASE_PLATFORM
  if (!IN_TEST_ENV()) {
    ctl.bf.RESET = 1 ;
  }
#else
  ctl.bf.RESET = 1 ;
#endif

  M2R_REG_WRITE(&pDev->SPI4P2.CONTROL.wrd, ctl.wrd) ;

  ctl.wrd = M2R_REG_READ(&pDev->SPI4P2.CONTROL.wrd) ;

  while (ctl.bf.RESET) { /* poll for reset bit to clear */
    if (usec_elapsed >= usec_timeout) break ;
    CS_PRINT("Port-%d: SPI42 - waiting for SRESET to clear...\n",
	     port_id) ;
    CS_UDELAY(usec_delay) ;
    usec_elapsed += usec_delay ;
    ctl.wrd = M2R_REG_READ(&pDev->SPI4P2.CONTROL.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_SPI42) ;

  if (ctl.bf.RESET) {
    CS_HNDL_ERROR(port_id, EM2R_RESET_TIMEOUT, ": SPI42 sreset\n") ;
    return (CS_ERROR) ;
  }
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SPI42 TX/RX SYNC SRESET                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP       : Milan2                                          */
cs_status  m2r_spi42_sync_sreset(cs_uint16 port_id, m2r_dir_t dir,
				 m2r_reset_action_t act)
/* INPUTS     : o Port Id                                       */
/*              o State machine selection(Tx, Rx or both)       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Reset control of the specified sync state machine.           */
/* This is a soft reset and the configuration registers are     */
/* unaffected.                                                  */
/*                                                              */
/* The [dir] paramter is specified as one of the following:     */
/* M2R_TX, M2R_RX, or M2R_TX_AND_RX.                            */
/* The [act] parameter is specified to be:                      */
/* M2R_RESET_DEASSERT, M2R_RESET_ASSERT, or                     */
/* M2R_RESET_TOGGLE.                                            */
/* The M2R_RESET_TOGGLE option will assert reset, hold the      */
/* reset for a while and then de-assert the reset bit(s).       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;
  SPI4P2_CONTROL_t	ctl, bits ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;

  bits.wrd = 0 ;
  if (dir == M2R_TX || dir == M2R_TX_AND_RX) {
    bits.bf.TXSYNRESET = 1 ;
  }
  if (dir == M2R_RX || dir == M2R_TX_AND_RX) {
    bits.bf.RXSYNRESET = 1 ;
  }

  M2R_PORT_LOCK(port_id, M2R_ID_SPI42) ;
  ctl.wrd = M2R_REG_READ(&pDev->SPI4P2.CONTROL.wrd) ;
  switch (act) {
    case M2R_RESET_DEASSERT :
      ctl.wrd &= ~bits.wrd ;
      M2R_REG_WRITE(&pDev->SPI4P2.CONTROL.wrd, ctl.wrd) ;
      break ;

    case M2R_RESET_ASSERT :
      ctl.wrd |= bits.wrd ;
      M2R_REG_WRITE(&pDev->SPI4P2.CONTROL.wrd, ctl.wrd) ;
      break ;

    case M2R_RESET_TOGGLE :
      /* set, wait for a few msecs, and clear hard reset bit(s) */
      ctl.wrd |= bits.wrd ;
      M2R_REG_WRITE(&pDev->SPI4P2.CONTROL.wrd, ctl.wrd) ;

#ifndef RELEASE_PLATFORM
      if (IN_SIM_ENV()) {
        m2r_sim_hreset_delay( M2R_PORT_ID_TO_DEV_ID(port_id) ,
                              M2R_RESET_MSEC_DELAY ) ;
      } else {
        CS_MDELAY( M2R_RESET_MSEC_DELAY ) ;
      }
#else
      CS_MDELAY( M2R_RESET_MSEC_DELAY ) ;
#endif

      ctl.wrd &= ~bits.wrd ;
      M2R_REG_WRITE(&pDev->SPI4P2.CONTROL.wrd, ctl.wrd) ;
      break ;
  }

#ifndef RELEASE_PLATFORM
  if (IN_SIM_ENV()) {
    m2r_sim_hreset_delay( M2R_PORT_ID_TO_DEV_ID(port_id) ,
                          M2R_RESET_MSEC_DELAY ) ;
  } else {
    CS_MDELAY( M2R_RESET_MSEC_DELAY ) ;
  }
#else
  CS_MDELAY( M2R_RESET_MSEC_DELAY ) ;
#endif

  M2R_PORT_UNLOCK(port_id, M2R_ID_SPI42) ;
  return (CS_OK) ;
}



/****************************************************************/
/* $rtn_hdr_start  SET SPI42 GENERAL CHARACTERSTICS             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP	      : Milan2                                          */
cs_status  m2r_spi42_set_gen_char(cs_uint16 port_id, cs_boolean stat_lvds,
				  cs_boolean dynalign,
				  cs_boolean quarter_rate)
/* INPUTS     : o Port Id                                       */
/*		o TRUE(if LVDS status channels) or FALSE       	*/
/*		o TRUE(if Dynamic alignment) or FALSE       	*/
/*              o TRUE(if quarter rate mode) or FALSE           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the general/electrical characterstics for the SPI4.2    */
/* interface.                                                   */
/*                                                              */
/* The [stat_lvds] parameter specifies whether LVDS or          */
/* LV-TTL I/Os are used for the status channels.                */
/*                                                              */
/* The [dynalign] parameter specifies whether dynamic or        */
/* static alignment will be done on the Tx path.                */
/* It is recommended to always use dynamic alignment(default    */
/* setting).                                                    */
/*                                                              */
/* The [quarter_rate] parameter specifies whether the Quarter   */
/* Rate (OC48) mode clocking is to be enabled or not.           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;

  M2R_REG_WRITE(&pDev->SPI4P2.STATLVDS.wrd, (stat_lvds) ? 1 : 0) ;
  M2R_REG_WRITE(&pDev->SPI4P2.DYNALIGN.wrd, (dynalign) ? 1 : 0) ;
  M2R_REG_WRITE(&pDev->SPI4P2.QTR_RATE.wrd, (quarter_rate) ? 1 : 0) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SELECT SPI42 RDCLK MULTIPLICATION FACTOR     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP       : Milan2                                          */
cs_status m2r_spi42_set_rdclk_mult(cs_uint16 port_id,
				   cs_uint8 mult)
/* INPUTS     : o Port Id                                       */
/*              o Multiplication factor(1, 2, or 4)             */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Selects the multiplication factor to scale up RDCLK          */
/* frequency with respect to the RDREFCLK. By default, the      */
/* factor is 1, ie. RDCLK is same as RDREFCLK.                  */
/* Note that the PLL center(or VCO) frequency runs at twice the */
/* speed of RDCLK.                                              */
/* If lower frequency reference clock is available only, then   */
/* this factor can be appropriately selected to get x2 or x4    */
/* to get the higher clock speeds for RDCLK.                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                   * pDev ;
  SPI4P2_RXPLLCONTROL1_t  rxpllcontrol1 ;

  if (mult != 1 && mult != 2 && mult != 4) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ":mult(%d) should be 2, 4 or 8\n", mult) ;
    return (CS_ERROR) ;
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;

  M2R_PORT_LOCK(port_id, M2R_ID_SPI42) ;
  rxpllcontrol1.wrd = M2R_REG_READ(&pDev->SPI4P2.RXPLLCONTROL1.wrd) ;
  switch (mult) {
    default :
    case 1 : rxpllcontrol1.bf.MULTSEL = 0 ; break ;
    case 2 : rxpllcontrol1.bf.MULTSEL = 1 ; break ;
    case 4 : rxpllcontrol1.bf.MULTSEL = 2 ; break ;
  }
  M2R_REG_WRITE(&pDev->SPI4P2.RXPLLCONTROL1.wrd, rxpllcontrol1.wrd) ;
  M2R_PORT_UNLOCK(port_id, M2R_ID_SPI42) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  ENABLING OF ATM MODE FOR SPI4.2              */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP	      : Milan2                                          */
cs_status  m2r_spi42_atm_mode(cs_uint16 port_id, cs_uint8 chan_id,
			      cs_boolean atm_mode)
/* INPUTS     : o Port Id                                       */
/*              o Channel Id(0...4)                             */
/*              o TRUE(if ATM cell mode) or FALSE               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Controls the enabling of ATM cell mode for the specified     */
/* SPI4.2 channel.                                              */
/*                                                              */
/* The [chan_id] parameter can be either 0...3 to specify a     */
/* particular channel or 4 to specify all 4 channels.           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;
  SPI4P2_CONTROL_t	control ;
  cs_uint8		bits = 0 ;
  m2r_port_mac_mode_t	mac_mode ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  mac_mode = M2R_GET_MAC_MODE(port_id) ;
  if ( (mac_mode != M2R_PORT_IN_BYPASS_MODE) && (atm_mode) ) {
    CS_HNDL_ERROR(port_id,
		  (mac_mode == M2R_PORT_IN_SRP_MODE) ?
		  EM2R_RMAC_INVALID_IN_SRP : EM2R_RMAC_INVALID_IN_RPR,
		  NULL) ;
    return (CS_ERROR) ;
  }

  if (chan_id > 4) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  " : chan_id(%d) s/b 0...4\n", chan_id) ;
    return (CS_ERROR) ;
  }

  if (chan_id == 4) { /* all 4 channels */
    bits = 0x0f ;
  }
  else {
    bits = 1 << chan_id ;
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  M2R_PORT_LOCK(port_id, M2R_ID_SPI42) ;

  control.wrd = M2R_REG_READ(&pDev->SPI4P2.CONTROL.wrd) ;
  if (atm_mode) {
    control.bf.RXATMCELLENABLE |= bits ;
    control.bf.TXATMCELLENABLE |= bits ;
  }
  else {
    control.bf.RXATMCELLENABLE &= ~bits ;
    control.bf.TXATMCELLENABLE &= ~bits ;
  }
  M2R_REG_WRITE(&pDev->SPI4P2.CONTROL.wrd, control.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_SPI42) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET BURST PARAMETERS                         */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP	      : Milan2                                          */
cs_status  m2r_spi42_set_burst_params(cs_uint16 port_id,
				      cs_uint8 burst_size,
				      cs_uint8 maxburst1,
				      cs_uint8 maxburst2)
/* INPUTS     : o Port Id                                       */
/*              o BURST_SIZE parameter(0...31)                  */
/*              o MAXBURST1 parameter(0...31)                   */
/*              o MAXBURST2 parameter(0...31)                   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets BURST_SIZE, MAXBURST1 and MAXBURST2 parameters.         */
/*                                                              */
/* BURST_SIZE is a number of 16-byte blocks, and it forms the   */
/* basic unit for configuring MAXBURST1 and MAXBURST2           */
/* parameters.                                                  */
/*                                                              */
/* MAX_BURST1 is the data burst credit in units of BURST_SIZE,  */
/* that is granted when RSTAT indicates "Starving" status.      */
/*                                                              */
/* MAX_BURST2 is the data burst credit in units of BURST_SIZE,  */
/* that is granted when RSTAT indicates "Hungry" status.        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if ( (burst_size >= 32) || (maxburst1 >= 32) ||
       (maxburst2 >= 32) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
	":burst_size(%d), max_burst1(%d), max_burst2(%d) s/b 0..31\n",
	burst_size, maxburst1, maxburst2) ;
    return (CS_ERROR) ;
  }

  if (maxburst2 > maxburst1) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  " : maxburst1(%d) s/b >= maxburst2(%d)\n",
		  maxburst1, maxburst2) ;
    return (CS_ERROR) ;
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;

  M2R_REG_WRITE(&pDev->SPI4P2.BURSTSIZE.wrd, burst_size) ;
  M2R_REG_WRITE(&pDev->SPI4P2.MAXBURST1.wrd, maxburst1) ;
  M2R_REG_WRITE(&pDev->SPI4P2.MAXBURST2.wrd, maxburst2) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SET TRAINING PARAMETERS                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP	      : Milan2                                          */
cs_status  m2r_spi42_set_training_params(cs_uint16 port_id,
					 cs_uint8 alpha,
					 cs_uint16 datamaxt)
/* INPUTS     : o Port Id                                       */
/*              o alpha (0...15)                                */
/*              o datamaxt(0...65535)                           */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the data training parameters for SPI4.2 interface.      */
/*                                                              */
/* alpha -                                                      */
/* This is ALPHA parameter and it specifies the number of       */
/* repetitions that must be scheduled every DATA_MAX_T cycles.  */
/*                                                              */
/* datamaxt -                                                   */
/* This is DATA_MAX_T parameter and it specifies the max        */
/* interval between training sequences.                         */
/* The training interval specified is counted at 1/2 the SPI Rx */
/* frequency. It can be DISABLED by specifying a value of 0.    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if (alpha > 15) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  " : alpha(%d) s/b 0...15\n", alpha) ;
    return (CS_ERROR) ;
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;

  M2R_REG_WRITE(&pDev->SPI4P2.ALPHA.wrd, alpha) ;
  M2R_REG_WRITE(&pDev->SPI4P2.DATAMAXT.wrd, datamaxt) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start    SET DIP-2 THRESHOLDS                       */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP	      : Milan2                                          */
cs_status  m2r_spi42_set_dip2_tholds(cs_uint16 port_id,
				     cs_uint8 error_thold,
				     cs_uint8 match_thold)
/* INPUTS     : o Port Id                                       */
/*              o Error Threshold(0...15)                       */
/*              o Match Threshold(0...15)                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* DIP-2 is sent with RSTAT from Host and is checked by chip.   */
/* These thresholds decide the transition of Rx state machine to*/
/* out-of-sync or in-sync states respectively.                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if (error_thold > 15) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  " : error_thold(%d) s/b 0...15\n", error_thold) ;
    return (CS_ERROR) ;
  }

  if (match_thold > 15) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  " : match_thold(%d) s/b 0...15\n", match_thold) ;
    return (CS_ERROR) ;
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;

  M2R_REG_WRITE(&pDev->SPI4P2.NUMDIP2ERR.wrd, error_thold) ;
  M2R_REG_WRITE(&pDev->SPI4P2.NUMDIP2MATCH.wrd, match_thold) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start    SET DIP-4 THRESHOLDS                       */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP	      : Milan2                                          */
cs_status  m2r_spi42_set_dip4_tholds(cs_uint16 port_id,
                   cs_uint8 error_thold, cs_uint16 match_thold)
/* INPUTS     : o Port Id                                       */
/*              o Error Threshold(0...15)                       */
/*              o Match Threshold(0...65535)                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* DIP-4 is sent with TDAT from Host and is checked by chip.    */
/* These thresholds decide the transition of Tx state machine to*/
/* out-of-sync or in-sync states respectively.                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if (error_thold > 15) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  " : error_thold(%d) s/b 0...15\n", error_thold) ;
    return (CS_ERROR) ;
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;

  M2R_REG_WRITE(&pDev->SPI4P2.NUMDIP4ERR.wrd, error_thold) ;
  M2R_REG_WRITE(&pDev->SPI4P2.NUMDIP4MATCH.wrd, match_thold) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   GET RX or TX SYNC STATE                     */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP	      : Milan2                                          */
m2r_spi42_sync_t  m2r_spi42_get_sync_state(cs_uint16 port_id,
					   m2r_dir_t dir)
/* INPUTS     : o Port Id                                       */
/*              o M2R_TX or M2R_RX                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Tx or Rx Sync Status                            */
/* DESCRIPTION:                                                 */
/* Indicates the current sync state of Tx or Rx state machine   */
/* The return value is:                                         */
/* M2R_SPI42_INIT_STATE      : No framing acquired yet          */
/* M2R_SPI42_OUT_OF_SYNC     : Waiting for DIP2 codeword match  */
/* M2R_SPI42_IN_SYNC         : In-sync and ready                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;
  SPI4P2_STATUS_t       status ;
  cs_uint8		bits ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if ( (dir != M2R_TX) && (dir != M2R_RX) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  " : dir(%d) s/b M2R_TX(%d) or M2R_RX(%d)",
		  dir, M2R_TX, M2R_RX) ;
    return (CS_ERROR) ;
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  status.wrd = M2R_REG_READ(&pDev->SPI4P2.STATUS.wrd) ;

  bits = ( (dir == M2R_TX) ? status.bf.SPI4P2TXSTATE :
	   status.bf.SPI4P2RXSTATE ) ;
  switch (bits) {
    default :
    case SPI4P2TXSTATE_INIT        : return (M2R_SPI42_INIT_STATE) ;
    case SPI4P2TXSTATE_OUT_OF_SYNC : return (M2R_SPI42_OUT_OF_SYNC) ;
    case SPI4P2TXSTATE_SYNC        : return (M2R_SPI42_IN_SYNC) ;
  }
}


/****************************************************************/
/* $rtn_hdr_start   SET TX SCHEDULER MODE                       */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP	      : Milan2                                          */
cs_status  m2r_spi42_set_tx_sched_mode(cs_uint16 port_id,
				       m2r_spi42_sched_mode_t mode,
				       cs_uint16 threshlo,
				       cs_uint16 txcal_burst)
/* INPUTS     : o Port Id                                       */
/*              o Mode selection                                */
/*              o Low threshold(0...4095) for cut-through modes */
/*              o Max Tx burst(0...4095) for 4-ch RR cut-through*/
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                           i     */
/* Select one of the following Tx scheduler modes and sets the  */
/* cut-through low threshold value.                             */
/* The [mode] parameter specifies one of the following modes    */
/* that the Tx datapath will operate in:                        */
/* M2R_SPI42_1CH_CUT_THRU    : 1 ch Cut-Through                 */
/* M2R_SPI42_1CH_ST_FWD      : 1 ch Store & Forward             */
/* M2R_SPI42_4CH_RR_CUT_THRU : 4 ch Roun-Robin Cut-Through      */
/* M2R_SPI42_4CH_PRIO_ST_FWD : 4 ch Priority Store & Forward    */
/*                                                              */
/* The [threshlo] parameter is valid only in *cut-through* modes*/
/* and it specifies how many 128-bit words need to be in the    */
/* FIFO before it is read. A value of 0 makes the Tx logic      */
/* read from FIFO only after EOP of packet has been written(ie. */
/* like in store & forward mode).                               */
/* This parameter is not used in store&forward modes. The       */
/* default(and recommended) value is 3.                         */
/*                                                              */
/* The [txcal_burst] is valid only for 4 ch Round-Robin Cut-Thru*/
/* and it specifies the maximum (N+1) 64-bit data that can      */
/* written to FIFO before channel switching of Tx scheduler.    */
/* This parameter is not used in other modes. The default(and   */
/* recommended) value is 6.                                     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;
  cs_reg		bits = 0 ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;

  switch (mode) {
    default                       :
    case M2R_SPI42_1CH_CUT_THRU   : bits = VALUE_mode_onech ; break ;
    case M2R_SPI42_1CH_ST_FWD     : bits = VALUE_mode_stfwd ; break ;
    case M2R_SPI42_4CH_RR_CUT_THRU: bits = VALUE_mode_oc48 ; break ;
    case M2R_SPI42_4CH_PRIO_ST_FWD: bits = VALUE_mode_stops ; break ;
  }

  M2R_REG_WRITE(&pDev->SPI4P2.TXCH_MODE.wrd, bits) ;

  if ( ((mode == M2R_SPI42_1CH_CUT_THRU) ||
	(mode == M2R_SPI42_4CH_RR_CUT_THRU))
       &&
       (threshlo == 0) ) {
    CS_PRINT("port-%d: WARNING- threshlo must be > 0 \n", port_id) ;
  }
  M2R_REG_WRITE(&pDev->SPI4P2.TXCH_THRESHLO.wrd, threshlo) ;

  if ( (mode == M2R_SPI42_4CH_RR_CUT_THRU) &&
       (txcal_burst == 0) ) {
    CS_PRINT("port-%d: WARNING- txcal_burst must be > 0\n", port_id) ;
  }
  M2R_REG_WRITE(&pDev->SPI4P2.TXCAL_BURST.wrd, txcal_burst) ;

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   SET TX FIFO LOW/HIGH WATERMARKS             */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP       : Milan2                                          */
cs_status m2r_spi42_set_tx_fifo_wmarks(cs_uint16 port_id,
				       cs_uint16 low_wmark,
				       cs_uint16 high_wmark)
/* INPUTS     : o Port Id                                       */
/*              o Tx FIFO low watermark(12-bits)                */
/*              o Tx FIFO high watermark(12-bits)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the FIFO parameters for the specified port.             */
/*                                                              */
/* The [low_wmark] parameter specifies ALMOST EMPTY condition.  */
/* The TSTAT signal(to Host) indicates STARVING when the        */
/* Tx FIFO is below this watermark. If the FIFO depth exceeds   */
/* this watermark then HUNGRY state is indicated.               */
/* This operates on 128-bit boundary.                           */
/*                                                              */
/* The [high_wmark] parameter specifies ALMOST FULL condition.  */
/* The TSTAT signal(to Host) indicates SATISFIED when the       */
/* Tx FIFO depth reaches this threshold.                        */
/* This operates on 128-bit boundary.                           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if ( (low_wmark > 0x0FFF) || (high_wmark > 0x0FFF) ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ":low_wmark(0x%x), high_wmark(0x%x) s/b <= 0x0FFF\n",
		  low_wmark, high_wmark) ;
    return (CS_ERROR) ;
  }

  if ( low_wmark > high_wmark ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ":low_wmark(0x%x) > high_wmark(0x%x)\n",
		  low_wmark, high_wmark) ;
    return (CS_ERROR) ;
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  M2R_REG_WRITE(&pDev->SPI4P2.TXFIFOALMOSTEMPTY.wrd, low_wmark) ;
  M2R_REG_WRITE(&pDev->SPI4P2.TXFIFOALMOSTFULL.wrd, high_wmark) ;

  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET RX FIFO WATERMARKS                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP       : Milan2                                          */
cs_status m2r_spi42_set_rx_fifo_wmarks(cs_uint16 port_id,
				       cs_uint16 high_wmark)
/* INPUTS     : o Port Id                                       */
/*              o Tx FIFO high watermark(12-bits)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the FIFO high-watermark for the specified port.         */
/*                                                              */
/* The [high_wmark] parameter specifies ALMOST FULL condition.  */
/* The TSTAT signal(to Host) indicates SATISFIED when the       */
/* Tx FIFO depth reaches this threshold.                        */
/* This operates on 128-bit boundary.                           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if ( high_wmark > 0x0FFF ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_INVALID_USER_ARG,
		  ": high_wmark(0x%x) s/b <= 0x0FFF\n",
		  high_wmark) ;
    return (CS_ERROR) ;
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  M2R_REG_WRITE(&pDev->SPI4P2.RXFIFOALMOSTFULL.wrd, high_wmark) ;

  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET CALENDAR PARAMETERS                     */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP	      : Milan2                                          */
cs_status  m2r_spi42_set_cal_params(cs_uint16 port_id,
		          m2r_dir_t dir, cs_uint16 m, cs_uint8  len,
				     cs_uint8  ent0, cs_uint8  ent1,
				     cs_uint8  ent2, cs_uint8  ent3)
/* INPUTS     : o Port Id                                       */
/*              o M2R_TX, M2R_RX or M2R_TX_AND_RX               */
/*              o Calendar Seq repetition number (0...65535)    */
/*              o Calendar Length (0...3)                       */
/*              o Calendar Entry 0 (0...3)                      */
/*              o Calendar Entry 1 (0...3)                      */
/*              o Calendar Entry 2 (0...3)                      */
/*              o Calendar Entry 3 (0...3)                      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Sets the various calendar parameters for the specified path. */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  if ( (len > 3) ||
       (ent0 > 3) || (ent1 > 3) || (ent2 > 3) || (ent3 > 3) ) {
    CS_HNDL_ERROR(port_id,  EM2R_PORT_INVALID_USER_ARG, NULL) ;
    return (CS_ERROR) ;
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;

  if ( (dir == M2R_TX) || (dir == M2R_TX_AND_RX) ) {
    SPI4P2_TXCAL_t  txcal ;

    M2R_REG_WRITE(&pDev->SPI4P2.TXCAL_M.wrd, m) ;
    M2R_REG_WRITE(&pDev->SPI4P2.TXCAL_LEN.wrd, len) ;

    txcal.wrd = 0 ;
    txcal.bf.ENT0 = ent0 ;
    txcal.bf.ENT1 = ent1 ;
    txcal.bf.ENT2 = ent2 ;
    txcal.bf.ENT3 = ent3 ;
    M2R_REG_WRITE(&pDev->SPI4P2.TXCAL.wrd, txcal.wrd) ;
  }

  if ( (dir == M2R_RX) || (dir == M2R_TX_AND_RX) ) {
    SPI4P2_RXCAL_t  rxcal ;

    M2R_REG_WRITE(&pDev->SPI4P2.RXCAL_M.wrd, m) ;
    M2R_REG_WRITE(&pDev->SPI4P2.RXCAL_LEN.wrd, len) ;

    rxcal.wrd = 0 ;
    rxcal.bf.ENT0 = ent0 ;
    rxcal.bf.ENT1 = ent1 ;
    rxcal.bf.ENT2 = ent2 ;
    rxcal.bf.ENT3 = ent3 ;
    M2R_REG_WRITE(&pDev->SPI4P2.RXCAL.wrd, rxcal.wrd) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SPI4.2 LOOPBACK                             */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : SPI42                                           */
/* CHIP	      : Milan2                                          */
cs_status  m2r_spi42_set_loopback(cs_uint16 port_id,
				  m2r_dir_t dir, cs_boolean enbl)
/* INPUTS     : o Port Id                                       */
/*              o Loopback selection (M2R_RX only!)             */
/*              o TRUE(enable loopback) or FALSE(disable)       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Control(disable/enable) the specified loopback in SPI4.2     */
/* interface.                                                   */
/*                                                              */
/* The [dir] parameter specifies which loopback to control and  */
/* should be one of the following:                              */
/* M2R_TX: When enabled, TDAT and TCTL outputs are looped back  */
/*         to RDAT and RCTL to form host-side(BERT) loopback.   */
/*         Do NOT select this option!!!                         */
/*                                                              */
/* M2R_RX: When enabled, RDAT and RCTL outputs are looped back  */
/*         to TDAT and TCTL to form line-side(LB) loopback.     */
/*                                                              */
/* Note: M2R_TX (BERT) loopback is meant to used for lab debug  */
/* only. Do NOT use this for looping back packets!              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  M2R_t                 * pDev ;
  SPI4P2_LOOPBACKS_t    lpbk ;

  M2R_SPI42_INVALID_PORT_EXIT(port_id, CS_ERROR) ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  M2R_PORT_LOCK(port_id, M2R_ID_SPI42) ;

  lpbk.wrd = M2R_REG_READ(&pDev->SPI4P2.LOOPBACKS.wrd) ;

  if (dir == M2R_TX) {
    if (enbl) {
      CS_PRINT("SPI BERT loopback(ROMA_TX) is not SUPPORTED!!\n") ;
    }
    else {
      lpbk.bf.BERTLB = 0 ;
    }
  }
  else { /* M2R_RX */
    lpbk.bf.LB = (enbl) ? 1 : 0 ;
  }
  M2R_REG_WRITE(&pDev->SPI4P2.LOOPBACKS.wrd, lpbk.wrd) ;
  M2R_PORT_UNLOCK(port_id, M2R_ID_SPI42) ;

  return (CS_OK) ;
}

