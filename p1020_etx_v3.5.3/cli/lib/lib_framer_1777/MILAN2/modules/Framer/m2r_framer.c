/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */

#include "cs_types.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_stats_priv.h"
#include "m2r_framer.h"
#include "m2r_framer_stats.h"
#include "m2r_framer_irq.h"
#include "m2r_framer_priv.h"
#include "m2r_framer_cmn.h"
#include "m2r_framer_atm.h"
#include "m2r_framer_gfp.h"
#include "m2r_framer_pkt.h"
#include "m2r_framer_soh.h"
#include "m2r_lif.h"


/****************************************************************/
/* $rtn_hdr_start  FRAMER INITIALIZATION                        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_init_cfg(cs_uint16 port_id,
			      m2r_port_summ_t * pSumm,
			      m2r_framer_cfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port configuration summary struct  */
/*              o Pointer to Framer configuration structure     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This initializes the Framer block of the specified port      */
/* with the given configuration.                                */
/* The caller is responsible for allocating memory for the      */
/* configuration structure, populating it.                      */
/* The caller is responsible in putting the framer in logic reset*/
/* before calling this api.                                     */
/* NOTE: if the OC48MD and XGEMD pins dont match the desired    */
/* configuration. Then the user should use XGEMD_Ovride and     */
/* OC48MD_Ovride bits in MPIF register to put the framer in right*/
/* mode.                                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32   err_code ;
  cs_uint8    strm = 0xff ;
  cs_boolean  channelized = FALSE ;

  if ( !(M2R_IS_PORT_VALID(port_id, err_code)) ) {
    CS_HNDL_ERROR(port_id, err_code, NULL) ;
    return(CS_ERROR) ;
  }

  if (pCfg == NULL ) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_CFG_NULL, "\n") ;
    return (CS_ERROR) ;
  }

  if (pSumm == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_SUMM_NULL, "\n") ;
    return (CS_ERROR) ;
  }

  //CS_PRINT("\t Initializing FRAMER... \n") ;
  channelized = ((pSumm->line_rate == M2R_PORT_RATE_OC192) ||
                 (pSumm->line_rate == M2R_PORT_RATE_OC48)) ? TRUE : FALSE ;
  /* Configure in SONET or SDH mode. */
  m2r_framer_sel_sonet_sdh(port_id, pCfg->sonet_mode) ;

  /* Configure sonet streams */
  if (m2r_framer_sel_sts_mode(port_id, pCfg->stsType) != CS_OK ) {
      return (CS_ERROR) ;
  }

  strm = (channelized == TRUE) ? M2R_SONET_STREAM_ALL : M2R_SONET_STREAM0 ;

  m2r_framer_set_rx_burst_len(port_id, strm,
                       (channelized == TRUE) ? pCfg->datacomBus_burst : 0) ;

  if (pCfg->terminal_loopback) {
    if ( m2r_framer_ctl_terminal_loopback(port_id,
					  M2R_ENABLE)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if (pCfg->line_loopback) {
    if ( m2r_framer_ctl_line_loopback(port_id, M2R_ENABLE)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }

  if (pCfg->section_loopback) {
    if ( m2r_framer_ctl_sonet_repeater(port_id, M2R_ENABLE, TRUE)
	 == CS_ERROR ) {
      return (CS_ERROR) ;
    }
  }


  /*
   * Configure the streams. If RMAC is used then the service should be
   * M2R_FR_SVC_POS_HDLC.
   */
  if (pCfg->svc == M2R_FR_SVC_GFP) {
    m2r_framer_set_spe_service_type(port_id, M2R_SONET_STREAM0, pCfg->svc) ;
  }
  else {
    m2r_framer_set_spe_service_type(port_id, strm, pCfg->svc) ;
  }

  m2r_framer_ctl_scrambling(port_id, M2R_RX_AND_TX,
                        (pCfg->sonet_scrambling ? M2R_ENABLE : M2R_DISABLE)) ;

  /*
   * Do POS or PPP specific initialisation.
   */
  if ( (pCfg->svc == M2R_FR_SVC_POS_PPP ) ||
       (pCfg->svc == M2R_FR_SVC_POS_HDLC) ) {
    /*
     * Configure the packet min / max values desired and rest will be
     * filtered.
     */
    m2r_framer_pkt_cfg_max_min_size(port_id, strm,
                                    pCfg->mode.pos_cfg.max_pktSize,
                                    pCfg->mode.pos_cfg.min_pktSize,
                                    M2R_RX_AND_TX) ;

    if ( pCfg->mode.pos_cfg.drop_long) {
      m2r_framer_pkt_drop_long(port_id, strm, M2R_RX_AND_TX, M2R_ENABLE ) ;
    }
    if ( pCfg->mode.pos_cfg.drop_short) {
      m2r_framer_pkt_drop_short(port_id, strm, M2R_RX_AND_TX, M2R_ENABLE ) ;
    }

    m2r_framer_pkt_ctl_crc32(port_id, strm, pCfg->mode.pos_cfg.crc32) ;

    /*
     * NULL FCS. ( For RPR/SRP mode , this should always be passed with
     * value FALSE.
     */
    m2r_framer_pkt_ctl_fcs(port_id, strm, pCfg->mode.pos_cfg.pktFCS ) ;

    m2r_framer_pkt_pass_fcs(port_id, strm,
                      (pCfg->mode.pos_cfg.passFCS ? M2R_ENABLE : M2R_DISABLE)) ;

    if ( pCfg->payload_scrambling == FALSE ) {
      m2r_framer_pkt_ctl_scrambling(port_id, strm, M2R_DISABLE ) ;
    }
  }
  else if ( pCfg->svc == M2R_FR_SVC_GFP ) { /* GFP Specific init */
    m2r_framer_gfp_cfg_mode(port_id, pCfg->mode.gfp_cfg.mode) ;

    if(pCfg->mode.gfp_cfg.cut_through) {
      m2r_framer_gfp_set_cut_through_mode(port_id, M2R_ENABLE) ;
    }

    m2r_framer_pkt_cfg_max_min_size(port_id, strm,
                                    pCfg->mode.gfp_cfg.max_pktSize,
                                    pCfg->mode.gfp_cfg.min_pktSize,
                                    M2R_RX_AND_TX) ;

    if (pCfg->mode.gfp_cfg.drop_long) {
      m2r_framer_gfp_drop_long(port_id, M2R_RX_AND_TX, M2R_ENABLE) ;
    }

    if (pCfg->mode.gfp_cfg.drop_short ) {
      m2r_framer_gfp_drop_short(port_id, M2R_RX_AND_TX, M2R_ENABLE) ;
    }

    if ( pCfg->payload_scrambling == FALSE ) {
      m2r_framer_gfp_ctl_scrambling(port_id, M2R_DISABLE) ;
    }
  }
  else if ( pCfg->svc == M2R_FR_SVC_ATM ) { /* ATM specific Init */
    m2r_framer_atm_ctl_hec(port_id, strm,
                (pCfg->mode.atm_cfg.check_hec) ? M2R_ENABLE : M2R_DISABLE) ;

    if(pCfg->mode.atm_cfg.drop_idle) {
      m2r_framer_atm_drop_idle(port_id, strm, M2R_ENABLE) ;
    }

    if ( pCfg->payload_scrambling == FALSE ) {
      m2r_framer_atm_ctl_scrambling(port_id, strm, M2R_DISABLE) ;
    }
  }

  /* DCC Port.  */
  if ( pCfg->enbl_dcc_prt ) {
    m2r_framer_cfg_dcc_port(port_id, pCfg->dcc_cfg,
                            M2R_TX_AND_RX, M2R_ENABLE) ;
  }

  /* APS port. */
  if ( pCfg->enbl_aps_prt ) {
    m2r_framer_ctl_aps_port(port_id, pCfg->aps_cfg,
                            M2R_TX_AND_RX, M2R_ENABLE) ;
  }

  if ( m2r_framer_init_stats(port_id, strm, pCfg->svc) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  /* Init Framer default IRQ */
  if ( m2r_framer_init_irq(port_id, pSumm, pCfg) == CS_ERROR ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  GET DEFAULT FRAMER CONFIGURATION             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_get_default_cfg(cs_uint16 port_id,
				     m2r_port_summ_t * pSumm,
                                     m2r_framer_cfg_t * pCfg )
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port config summary info           */
/*              o Pointer to Framer configuration structure     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the driver's default Framer configuration for the        */
/* specified port and for the specified summary config info.    */
/* The caller is responsible for allocating memory for the      */
/* configuration structure, and the driver will populate it to  */
/* provide a default configuration. The caller can then update  */
/* the configuration parameters as desired and use it to        */
/* initialize the framer.                                       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint32  strm_start, strm_end ;
  cs_boolean channelized_mode = FALSE ;

  if (pCfg == NULL ) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_CFG_NULL, "\n") ;
    return ( CS_ERROR ) ;
  }

  if (pSumm == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_SUMM_NULL, "\n") ;
    return ( CS_ERROR ) ;
  }

  if ((pSumm->mac_mode != M2R_PORT_IN_BYPASS_MODE) &&
      (pSumm->l2_prot == M2R_PORT_PROT_ATM)) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_CFG_VALIDITY_ERR,
         "In RPR/SRP Mode Framer shouldn't be configured for ATM\n") ;
    return (CS_ERROR) ;
  }

  pCfg->sonet_mode = TRUE ; /* Sonet Mode */
  pCfg->enbl_dcc_prt = FALSE ;
  pCfg->enbl_aps_prt = FALSE ;
  pCfg->payload_scrambling = TRUE ;
  pCfg->sonet_scrambling = TRUE ;

  pCfg->terminal_loopback = FALSE ;

  pCfg->line_loopback    = FALSE ;
  pCfg->section_loopback = FALSE ;

  pCfg->loop_timing      = FALSE ;

  switch(pSumm->line_rate) {
    case M2R_PORT_RATE_OC192  :
      pCfg->stsType = M2R_FR_MODE_OC192 ;
      channelized_mode = TRUE ;
      pCfg->datacomBus_burst = (pSumm->l2_prot == M2R_PORT_PROT_ATM) ?
	M2R_FRAMER_RX_ATM_BURST_DEF : M2R_FRAMER_RX_PKT_BURST_DEF ;
      break ;

    case M2R_PORT_RATE_OC192C :
      pCfg->stsType = M2R_FR_MODE_OC192c ;
      channelized_mode = FALSE ;
      pCfg->datacomBus_burst = 0 ;
      break ;

    case M2R_PORT_RATE_OC48  :
      pCfg->stsType = M2R_FR_MODE_OC48 ;
      channelized_mode = TRUE ;
      pCfg->datacomBus_burst = (pSumm->l2_prot == M2R_PORT_PROT_ATM) ?
	M2R_FRAMER_RX_ATM_BURST_DEF : M2R_FRAMER_RX_PKT_BURST_DEF ;
      break ;

    case M2R_PORT_RATE_OC48C :
      pCfg->stsType = M2R_FR_MODE_OC48c ;
      channelized_mode = FALSE ;
      pCfg->datacomBus_burst = 0 ;
      break ;

    case M2R_PORT_RATE_10G :
      CS_HNDL_ERROR( port_id, EM2R_PORT_CFG_VALIDITY_ERR,
         "Invalid Line rate for SONET Mode\n") ;
      return (CS_ERROR) ;
  }

  if ( channelized_mode == TRUE ) {
    strm_start = M2R_SONET_STREAM0 ;
    strm_end = M2R_SONET_STREAM_ALL ;
  }
  else {
    strm_start = M2R_SONET_STREAM0 ;
    strm_end = M2R_SONET_STREAM1 ;
  }


  switch(pSumm->l2_prot) {
    case M2R_PORT_PROT_POS :
      pCfg->mode.pos_cfg.pktFCS =
          ((pSumm->mac_mode == M2R_PORT_IN_BYPASS_MODE) ? TRUE : FALSE );
      pCfg->mode.pos_cfg.crc32 = TRUE ;
      pCfg->mode.pos_cfg.passFCS = FALSE ;
      pCfg->mode.pos_cfg.drop_long  = TRUE ;
      pCfg->mode.pos_cfg.drop_short = TRUE ;
      if (pSumm->mac_mode == M2R_PORT_IN_BYPASS_MODE) {
	      pCfg->mode.pos_cfg.max_pktSize = M2R_FRAMER_PKT_MAX_SIZE_DEF ;
      }
      else { /* SRP or RPR */
	      pCfg->mode.pos_cfg.max_pktSize = (9 * 1024) ; /* jumbo frame */
      }
      pCfg->mode.pos_cfg.min_pktSize = M2R_FRAMER_PKT_MIN_SIZE_DEF ;
      pCfg->svc = M2R_FR_SVC_POS_HDLC ;
    break ;

    case M2R_PORT_PROT_PPP :
      pCfg->mode.pos_cfg.pktFCS =
          ((pSumm->mac_mode == M2R_PORT_IN_BYPASS_MODE) ? TRUE : FALSE );
      pCfg->mode.pos_cfg.crc32 = TRUE ;
      pCfg->mode.pos_cfg.passFCS = FALSE ;
      pCfg->mode.pos_cfg.drop_long  = TRUE ;
      pCfg->mode.pos_cfg.drop_short = TRUE ;
      pCfg->mode.pos_cfg.max_pktSize = M2R_FRAMER_PKT_MAX_SIZE_DEF ;
      pCfg->mode.pos_cfg.min_pktSize = M2R_FRAMER_PKT_MIN_SIZE_DEF ;
      pCfg->svc = M2R_FR_SVC_POS_PPP ;
    break ;

    case M2R_PORT_PROT_ATM :
      pCfg->mode.atm_cfg.check_hec = TRUE ;
      pCfg->mode.atm_cfg.drop_idle = TRUE ;
      pCfg->svc = M2R_FR_SVC_ATM ;
    break ;

    case M2R_PORT_PROT_GFP :
      pCfg->mode.gfp_cfg.mode = M2R_FR_GFP_NORMAL ;
      pCfg->mode.gfp_cfg.cut_through = FALSE ;
      pCfg->mode.gfp_cfg.drop_long  = TRUE ;
      pCfg->mode.gfp_cfg.drop_short = TRUE ;
      if (pSumm->mac_mode == M2R_PORT_IN_BYPASS_MODE) {
	pCfg->mode.gfp_cfg.max_pktSize = M2R_FRAMER_PKT_MAX_SIZE_DEF ;
      }
      else {
	pCfg->mode.gfp_cfg.max_pktSize = (9 * 1024) ; /* jumbo frame */
      }
      pCfg->mode.gfp_cfg.min_pktSize = M2R_FRAMER_PKT_MIN_SIZE_DEF ;
      pCfg->svc = M2R_FR_SVC_GFP ;
    break ;

    case M2R_PORT_PROT_CLEAR_CH :
      pCfg->svc = M2R_FR_SVC_CLR_CH ;
    break ;

    default :
      CS_HNDL_ERROR( port_id, EM2R_PORT_INVALID_USER_ARG,
		     "pSumm->l2_prot = %d\n", pSumm->l2_prot) ;
      return (CS_ERROR) ;
    break ;
  }

  pCfg->gpio_cfg = 0xf0; /* default out of reset */
  pCfg->gpio_init_out = 0xc0; /* default out of reset */

  /* Get the Default IRQ Configuration */
  m2r_framer_get_default_irq_cfg(port_id, pSumm, pCfg, channelized_mode) ;

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  GET FRAMER RUNNING CONFIGURATION             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_get_running_cfg(cs_uint16 port_id,
				     m2r_port_summ_t * pSumm,
                                     m2r_framer_runcfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port config summary info           */
/*              o Pointer to Framer config structure            */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Returns the current run-time Framer hardware configuration   */
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
  FR_SONET32RX_Configuration_t cfg ;
  FR_DATACOMRX_StrConfig_t     rx_config ;
  M2R_t                      * pDev ;
  cs_uint16                    ii, max_str = 1, sl_num ;
  cs_reg                     * regaddr ;

  if (pCfg == NULL ) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_CFG_NULL, "\n") ;
    return (CS_ERROR) ;
  }

  if (pSumm == NULL) {
    CS_HNDL_ERROR( port_id, EM2R_PORT_SUMM_NULL, "\n") ;
    return (CS_ERROR) ;
  }

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  pCfg->sync_status =  m2r_framer_get_sync_status(port_id) ;
  cfg.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd) ;
  pCfg->sonet_mode = ((cfg.bf.SDH) ? FALSE : TRUE ) ;

  pCfg->channelized_mode = m2r_framer_is_in_channelised_mode(port_id) ;

  if (pCfg->channelized_mode == TRUE) {
    max_str = M2R_SONET_STREAM_ALL ;
  }

  for (ii = 0; ii < max_str; ii++) {
    rx_config.wrd = M2R_REG_READ(
                    &pDev->slice[sl_num].Framer.datacomRx_StrConfig.wrd +
                    (ii * M2R_FR_DATACOMRX_StrConfig_STRIDE)) ;

    pCfg->datacomBus_burst[ii] = rx_config.bf.burstLength ;

    switch (m2r_framer_get_svc_type(port_id, ii)) {
      case M2R_PORT_PROT_ATM :
      break ;

      case M2R_PORT_PROT_GFP : {
        FR_GFP_TX_GFP_Control_0_t     tx_ctrl0 ;

        tx_ctrl0.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.
				   gfp_tx_GFP_Control_0.wrd) ;

        if (tx_ctrl0.bf.Gen_CRC && tx_ctrl0.bf.Gen_Core_Header &&
            tx_ctrl0.bf.Gen_Pld_Header && tx_ctrl0.bf.Gen_THEC ) {
          pCfg->mode[ii].gfp_cfg.mode = M2R_FR_GFP_NORMAL ;
        }
        else {
          pCfg->mode[ii].gfp_cfg.mode = M2R_FR_GFP_TRANSPARENT ;
        }
        pCfg->mode[ii].gfp_cfg.cut_through  = tx_ctrl0.bf.Enable_Cut_Through ;
        pCfg->mode[ii].gfp_cfg.payload_scrambling = tx_ctrl0.bf.Dis_Scrambler ;
      }
      break ;

      case M2R_PORT_PROT_PPP :
      case M2R_PORT_PROT_POS : {
        FR_STREAMRX_PktControl_t            rx_pktctl ;
        regaddr = (cs_reg *)
                    &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd +
                    (ii * M2R_FR_STREAMRX__STRIDE) ;
        rx_pktctl.wrd = M2R_REG_READ(regaddr) ;
        pCfg->mode[ii].pos_cfg.crc32 = rx_pktctl.bf.crc32 ;
        pCfg->mode[ii].pos_cfg.pktFCS =
            ((rx_pktctl.bf.nullFCS) ? FALSE : TRUE) ;
        pCfg->mode[ii].pos_cfg.pass_fcs = rx_pktctl.bf.passFCS ;
        pCfg->mode[ii].pos_cfg.payload_scrambling = rx_pktctl.bf.disDscr ;
      }
      break ;

      case M2R_PORT_PROT_CLEAR_CH :
      case M2R_PORT_PROT_ETHERNET :
      default:
        return (CS_ERROR);
    }
  }

  /* terminal loopback */
  /* line loopback */
  /* loop timing mode */
  {
    FR_GLOBAL_LoopBack_t      loopback;

    loopback.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.LoopBack.wrd);
    pCfg->terminal_loopback = (loopback.bf.Terminal == 1) ;
    pCfg->line_loopback = (loopback.bf.Line == 1) ;
    pCfg->loop_timing = FALSE ; /* obsolete */
  }

  /* section loopback */
  {
    FR_TRANSMIT_Regen_t       regen ;

    regen.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.Regen.wrd);
    pCfg->section_loopback = (regen.bf.regenOn == 1) ;
  }

  {
    /* get gpio config */
    FR_GLOBAL_GPIOControl_t  gpioControl ;

    regaddr = (cs_reg *)&pDev->slice[sl_num].Framer.GPIOControl.wrd ;
    gpioControl.wrd = M2R_REG_READ(regaddr) ;
    pCfg->gpio_cfg = gpioControl.bf.DIR;
  }
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   DUMP RUNNING FRAMER CONFIGURATION           */
/* CATEGORY   : API                                             */
/* ACCESS     : Public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_dump_running_cfg(cs_uint16 port_id,
				   m2r_framer_runcfg_t * pCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to port config summary info           */
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
  cs_uint8                     ii, max_str = 1 ;

  if (pCfg == NULL) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_CFG_NULL, NULL) ;
    return(CS_ERROR) ;
  }

  CS_PRINT("Framer line status: %s\n",
	   (pCfg->sync_status) ? "in-sync(no LOF)" :
	   "out-of-sync!!") ;

  CS_PRINT("Framer Mode       : %s\n",
           (pCfg->sonet_mode ? "SONET" : "SDH")) ;

  CS_PRINT("Line Loopback     : %s\n",
           (pCfg->line_loopback ? "Enabled" : "Disabled")) ;

  CS_PRINT("Terminal Loopback : %s\n",
           (pCfg->terminal_loopback ? "Enabled" : "Disabled")) ;

  CS_PRINT("Section Loopback  : %s\n",
           (pCfg->section_loopback ? "Enabled" : "Disabled")) ;

  if (pCfg->channelized_mode == TRUE) {
    max_str = M2R_SONET_STREAM_ALL ;
  }

  for (ii = 0; ii < max_str; ii++) {
    switch (m2r_framer_get_svc_type(port_id, ii)) {
      case M2R_PORT_PROT_ATM :
        CS_PRINT("\t\tStream-%d is in ATM Mode\n", ii) ;
      break ;

      case M2R_PORT_PROT_GFP :
        CS_PRINT("\t\tStream-%d is in GFP Mode\n", ii) ;
        CS_PRINT("GFP Mode          : %s\n",
                 ((pCfg->mode[ii].gfp_cfg.mode == M2R_FR_GFP_NORMAL) ?
                  "Normal" : "Transparent")) ;

        CS_PRINT("Cut through       : %s\n",
                 (pCfg->mode[ii].gfp_cfg.cut_through ? "Yes" : "No")) ;

        CS_PRINT("Scrambler         : %s\n",
                 ((pCfg->mode[ii].gfp_cfg.payload_scrambling) ?
                  "Disabled" : "Enabled")) ;
      break ;

      case M2R_PORT_PROT_PPP :
      case M2R_PORT_PROT_POS :
        CS_PRINT("\t\tStream-%d is in POS/PPP Mode\n", ii) ;
        CS_PRINT("FCS Mode          : %s\n",
                 (pCfg->mode[ii].pos_cfg.pktFCS ? "Normal" : "NULL")) ;

        CS_PRINT("CRC Mode          : %s\n",
                 (pCfg->mode[ii].pos_cfg.crc32 ? "CRC32" : "CRC16")) ;

        CS_PRINT("FCS Passed to HOST: %s\n",
                 (pCfg->mode[ii].pos_cfg.pass_fcs ? "Yes" : "No")) ;

        CS_PRINT("Scrambler         : %s\n",
                 (pCfg->mode[ii].pos_cfg.payload_scrambling ?
                  "Disabled" : "Enabled")) ;
      break ;

      case M2R_PORT_PROT_CLEAR_CH :
      case M2R_PORT_PROT_ETHERNET :
      default:
        return (CS_ERROR);
    }
    CS_PRINT("Burst Size        : %d\n", pCfg->datacomBus_burst[ii]) ;
  }

  CS_PRINT("GPIO config       : 0x%02x\n", pCfg->gpio_cfg) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SELECTS THE FRAMER MODE (SONET OR SDH)       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_sel_sonet_sdh(cs_uint16 port_id,
                                   cs_boolean sonet_mode)
/* INPUTS     : o Port Id                                       */
/*              o TRUE ( SONET MODE ) or FALSE ( SDH MODE )     */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Selects the framer mode from either SONET or SDH.            */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                    sl_num ;
  M2R_t                      * pDev ;
  FR_SONET32RX_Configuration_t cfg ;
  FR_XMITOVERHEAD_Ctrl_t       tx_ovrhd ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  cfg.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd) ;

  cfg.bf.SDH = (sonet_mode) ? 0 : 1 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd,
               cfg.wrd) ;

  /* TX overhead ctrl . Set the sonet/sdh mode for Z0 byte */
  tx_ovrhd.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd) ;
  tx_ovrhd.bf.SDHMode = (sonet_mode) ? 0 : 1 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd,
               tx_ovrhd.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE SONET (DE)SCRAMBLER           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_ctl_scrambling(cs_uint16 port_id,
                                    m2r_dir_t dir, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_RX or M2R_TX or M2R_RX_AND_TX             */
/*              o M2R_DISABLE (0) or M2R_ENABLE (1)             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/Disable Framer SONET (De)Scrambler (1 + x^6 + x^7).   */
/* When Disabled for TX Direction, no scrambling is done of the */
/* TX Frame. When Disabled on RX Direction, NO Descrambling is  */
/* is done on the RX Frame.                                     */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_boolean               rx_dir , tx_dir ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  if (tx_dir) {
    FR_TRANSMIT_SectionControl_t  tx_ctrl ;

    tx_ctrl.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.SectionControl.wrd) ;
    tx_ctrl.bf.DisScr = (ctl == M2R_DISABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.SectionControl.wrd,
                 tx_ctrl.wrd) ;
  }
  if (rx_dir) {
    FR_SONET32RX_SectionLineControl_t rx_ctrl ;

    rx_ctrl.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.SectionLineControl.wrd) ;
    rx_ctrl.bf.DisDScr = (ctl == M2R_DISABLE) ? 1 : 0 ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.SectionLineControl.wrd,
                 rx_ctrl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   SELECTS THE SONET STS MODE                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_sel_sts_mode(cs_uint16 port_id,
                                     m2r_framer_stsType_t sts_t)
/* INPUTS     : o Port Id                                       */
/*              o STS Mode                                      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the STS mode of the Framer. If sts_t is           */
/* M2R_FR_MODE_OC192c or M2R_FR_MODE_OC48c , only stream Zero(0)*/
/* is used and user should call the Framer APIs with stream ID  */
/* M2R_SONET_STREAM0. If M2R_FR_MODE_OC192 or M2R_FR_MODE_OC48  */
/* is configured, Framer is in Channelized Mode and stream 0-3  */
/* are valid for the Framer APIs.                               */
/* The [sts_t ] parameter is specified as following for MILAN2: */
/* M2R_FR_MODE_OC192c or M2R_FR_MODE_OC48c or                   */
/* M2R_FR_MODE_OC192  or M2R_FR_MODE_OC48                       */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  volatile cs_reg        * regaddr ;
  cs_uint32                str_end = M2R_SONET_STREAM1, str_iter;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if (((sts_t == M2R_FR_MODE_OC192) || (sts_t == M2R_FR_MODE_OC48)) &&
      (M2R_GET_MAC_MODE(port_id) != M2R_PORT_IN_BYPASS_MODE)) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_IN_RMAC_MODE,
                  "Channelization not supported in RMAC mode\n", sts_t) ;
    return (CS_ERROR) ;
   }

  if ( sts_t > M2R_FR_MODE_OC48 ) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_SPE_BW, "\n", sts_t) ;
    return (CS_ERROR) ;
  }

  switch (sts_t) {
    case M2R_FR_MODE_OC192 :
      sts_t = STSType_STS_48C ;
      str_end = M2R_SONET_STREAM_ALL ;
      break ;

    case M2R_FR_MODE_OC48 :
      sts_t = STSType_STS_12C ;
      str_end = M2R_SONET_STREAM_ALL ;
      break ;

    case M2R_FR_MODE_OC192c :
      sts_t = STSType_STS_192C ;
      str_end = M2R_SONET_STREAM1 ;
      break ;

    case M2R_FR_MODE_OC48c :
      sts_t = STSType_STS_48C ;
      str_end = M2R_SONET_STREAM1 ;
      break ;

    default :
      CS_HNDL_ERROR(port_id, EM2R_INVALID_SPE_BW, "\n", sts_t) ;
      return (CS_ERROR) ;
  }

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);

  for (str_iter = M2R_SONET_STREAM0 ; str_iter < str_end; str_iter++) {
    FR_TRANSMIT_PathProvisioning_t      tx_prvsn ;
    FR_SONET32RX_PathProvision_t        rx_prvsn ;

    /* RX PathProvision */
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathProvision.wrd +
                (str_iter * M2R_FR_SONET32RX_PathProvision_STRIDE) ;
    rx_prvsn.wrd = M2R_REG_READ(regaddr) ;
    rx_prvsn.bf.STSType = sts_t ;
    rx_prvsn.bf.QID     = str_iter ;
    M2R_REG_WRITE(regaddr, rx_prvsn.wrd) ;

    /* TX PathProvision */
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathProvisioning.wrd +
                (str_iter * M2R_FR_TRANSMIT_PathProvisioning_STRIDE) ;
    tx_prvsn.wrd = M2R_REG_READ(regaddr) ;
    tx_prvsn.bf.STStype = sts_t ;
    tx_prvsn.bf.QID     = str_iter ;
    M2R_REG_WRITE(regaddr, tx_prvsn.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SELECT THE SERVICE TYPE CARRIED BY SONET SPE */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_set_spe_service_type(cs_uint16 port_id,
                                      m2r_framer_stream_t channel,
                                      m2r_framer_svc_type_t svc)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o Sonet/SDH service type                        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the SONET service type from POS/PPP/ATM and GFP.  */
/* If the framer is configured in channelized mode, each stream */
/* can be configured independently to carry a different service.*/
/* This API applies to both TX and RX both                      */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* The [svc] parameter is specified as following:               */
/* M2R_FR_SVC_ATM or M2R_FR_SVC_POS_PPP or M2R_FR_SVC_POS_HDLC  */
/* M2R_FR_SVC_GFP or M2R_FR_SVC_CLR_CH                          */
/* NOTE : GFP service can be enabled only on stream 0.          */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16          sl_num ;
  M2R_t            * pDev ;
  cs_uint32          str_start, str_end, str_iter ;
  cs_uint8           strData, bfPOS = 0, bfSvc = 0, bfModeFF03 = 0 ;
  cs_uint8           c2_byte  = 0x00 ;
  volatile cs_reg  * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel);

  switch ( svc ) {
    case M2R_FR_SVC_ATM :
      strData = dataType_CELL ;
      bfSvc = SVC_SRVC_ATM ;
      c2_byte = M2R_SONET_C2_ATM ;
      break ;

    case M2R_FR_SVC_POS_PPP :
      strData = dataType_PACKET ;
      bfPOS = 1 ;
      //2012.05.23: wjd, don't drop FF03 when receive and don't insert FF03 when transmit
//      bfModeFF03 = 1 ; /* NOTE : For RPR/SRP mode dont use PPP */
      bfSvc = SVC_SRVC_PPP ;
      c2_byte = M2R_SONET_C2_HDLC ;
      break ;

    case M2R_FR_SVC_POS_HDLC : /* THIS SHOULD BE USED WHEN RMAC IS USED */
      strData = dataType_PACKET ;
      bfPOS = 1 ;
      bfModeFF03 = 0;
      bfSvc = SVC_SRVC_PPP ;
      c2_byte = M2R_SONET_C2_HDLC ;
      break ;

    case M2R_FR_SVC_GFP :
      strData = dataType_PACKET ;
      bfSvc = SVC_SRVC_GFP ;
      c2_byte = M2R_SONET_C2_GFP ;
      break ;

    case M2R_FR_SVC_PRBS :
      strData = dataType_PACKET ;
      bfSvc = SVC_SRVC_PRBS ;
      c2_byte = M2R_SONET_C2_HDLC ;
      break ;

    case M2R_FR_SVC_CLR_CH :
      strData = dataType_CLEAR ;
      bfPOS = 0 ;
      break ;

    default :
        CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
        return (CS_ERROR) ;
  }

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);

  for (str_iter = str_start; str_iter < str_end; str_iter++) {
    FR_DATACOMTX_StrConfig_t            tx_config ;
    FR_STREAMTX_PktControl_t            tx_pktctl ;
    FR_TRANSMIT_PathProvisioning_t      tx_prvsn ;
    FR_DATACOMRX_StrConfig_t            rx_config ;
    FR_STREAMRX_PktControl_t            rx_pktctl ;
    FR_SONET32RX_PathProvision_t        rx_prvsn ;

    /*
     * Set the expected C2 ( path label ) and the C2 label to be sent.
     */
    m2r_framer_set_overheadByte(port_id, M2R_FR_OVRHD_C2, c2_byte, str_iter) ;
    m2r_framer_set_rx_path_label(port_id, c2_byte, str_iter) ;

    /* RX stream config */
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.datacomRx_StrConfig.wrd +
                (str_iter * M2R_FR_DATACOMRX_StrConfig_STRIDE) ;
    rx_config.wrd = M2R_REG_READ(regaddr) ;
    rx_config.bf.dataType = strData ;
    M2R_REG_WRITE(regaddr, rx_config.wrd) ;

    /* RX PathProvision (modePOS) */
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathProvision.wrd +
                (str_iter * M2R_FR_SONET32RX_PathProvision_STRIDE) ;
    rx_prvsn.wrd = M2R_REG_READ(regaddr) ;
    rx_prvsn.bf.SVC = bfSvc ;
    M2R_REG_WRITE(regaddr, rx_prvsn.wrd) ;

    /* RX Pkt/cell control */
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd +
                (str_iter * M2R_FR_STREAMRX__STRIDE) ;
    rx_pktctl.wrd = M2R_REG_READ(regaddr) ;
    rx_pktctl.bf.modePOS = bfPOS ;
    rx_pktctl.bf.modeDropFF03 = bfModeFF03 ;
    M2R_REG_WRITE(regaddr, rx_pktctl.wrd) ;

    /* TX stream config */
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.datacomTx_StrConfig.wrd +
                (str_iter * M2R_FR_DATACOMTX_StrConfig_STRIDE) ;
    tx_config.wrd = M2R_REG_READ(regaddr) ;
    tx_config.bf.dataType = strData;
    M2R_REG_WRITE(regaddr, tx_config.wrd) ;

    /* TX PathProvision (modePOS) */
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathProvisioning.wrd +
                (str_iter * M2R_FR_TRANSMIT_PathProvisioning_STRIDE) ;
    tx_prvsn.wrd = M2R_REG_READ(regaddr) ;
    tx_prvsn.bf.SVC = bfSvc ;
    M2R_REG_WRITE(regaddr, tx_prvsn.wrd) ;

    /* TX Pkt/cell control */
    regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_PktControl.wrd +
                (str_iter * M2R_FR_STREAMTX__STRIDE) ;
    tx_pktctl.wrd = M2R_REG_READ(regaddr) ;
    tx_pktctl.bf.PosMode = bfPOS ;
    tx_pktctl.bf.InsFF03 = bfModeFF03 ;
    M2R_REG_WRITE(regaddr, tx_pktctl.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   CONFIGURE RX DATACOM BUS BURST LENGTH       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_set_rx_burst_len(cs_uint16 port_id,
                      m2r_framer_stream_t channel, cs_uint8  burst)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o Burst length                                  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configure datacom bus Rx burst length. The stream will attempt*/
/* to send at least burstLength number of words before          */
/* relinquishing the bus to another stream that can send data.  */
/* If Framer is in channelized mode, this API should always be  */
/* called since the default value of burst lengþh is Zero.      */
/* burst field should be set to multiples of seven if the stream*/
/* is configured for cells. It should be set to zero if the     */
/* channel is not provisioned. If all the streams are configured*/
/* to zero then stream zero will always have the bus. The valid */
/* range for this is 1 to 128 words.                            */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint32                str_start, str_end, str_iter;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
    FR_DATACOMRX_StrConfig_t            rx_config ;

    /* RX stream config */
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.datacomRx_StrConfig.wrd +
                (str_iter * M2R_FR_DATACOMRX_StrConfig_STRIDE) ;
    rx_config.wrd = M2R_REG_READ(regaddr) ;
    rx_config.bf.burstLength = burst;
    M2R_REG_WRITE(regaddr, rx_config.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start SET TX DATACOM BUS FIFO FULL THRESHOLD        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_set_tx_fifo_high_mark(cs_uint16 port_id,
                                    m2r_framer_stream_t channel,
                               cs_boolean atm, cs_uint8  threshold)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o TRUE ( if Stream in ATM mode ) else FALSE     */
/*              o Threshold value                               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configure the TX datacom bus fifo full threashold.           */
/* This value is the minimun number of cells (ATM Mode) or words*/
/* ( Packet Mode) in input FIFO before full is asserted.        */
/* IF IN ATM MODE the value of threashold is in number of cells */
/* and the valid range is 1 to 7 . In packet mode the value is  */
/* in words and the valid range is 1 to 63.                     */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint32                str_start, str_end, str_iter;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
    FR_DATACOMTX_StrConfig_t    tx_config ;

    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.datacomTx_StrConfig.wrd +
                (str_iter * M2R_FR_DATACOMTX_StrConfig_STRIDE) ;
    tx_config.wrd = M2R_REG_READ(regaddr) ;
    if ( atm ) {
        tx_config.bf.cellSkid = threshold ;
        tx_config.bf.packetSkid = 0 ;
    }
    else {
        tx_config.bf.packetSkid = threshold ;
        tx_config.bf.cellSkid = 0 ;
    }
    M2R_REG_WRITE(regaddr, tx_config.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   CONFIGURES THE MAX/MIN SIZE OF THE PACKET   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_pkt_cfg_max_min_size(cs_uint16 port_id,
                m2r_framer_stream_t channel, cs_uint32 maxPktSize,
                                cs_uint8 minPktSize, m2r_dir_t dir)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o MAX  Packet Size                              */
/*              o MIN  Packet Size                              */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the size of the maximum and minimum packets. Any  */
/* packet greater than max packet or less than min size will    */
/* be counted in stats. Also user can enable/disable filtering  */
/* of the packets which are not in this range through the       */
/* following APIs :                                             */
/* Packet Mode ( POS/PPP ) :                                    */
/* m2r_framer_pkt_drop_long() API for long packets              */
/* m2r_framer_pkt_drop_short() API for short packets.           */
/*                                                              */
/* For GFP :                                                    */
/* m2r_framer_gfp_drop_long() API for ling packets              */
/* m2r_framer_gfp_drop_short() API for short packets.           */
/* If Stream is configured for GFP Mode, max packet can be      */
/* 0xffff.                                                      */
/*                                                              */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint32                str_start, str_end, str_iter;
  cs_boolean               rx_dir , tx_dir ;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if ( minPktSize > maxPktSize ) {
    CS_HNDL_ERROR(port_id, EM2R_PORT_MIN_GT_MAX, NULL) ;
    return (CS_ERROR);
  }
  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  for ( str_iter = str_start; str_iter < str_end; str_iter++ ) {
    if ( rx_dir ) {
      FR_STREAMRX_MaxLengthMSB_t  rx_maxPkt ;
      FR_STREAMRX_MinLength_t     rx_minPkt ;

      /* set RX pkt maxSize register */
      regaddr = (cs_uint16 *) &pDev->slice[sl_num].Framer.MaxLengthMSB.wrd +
                                      (str_iter * M2R_FR_STREAMRX__STRIDE) ;
      rx_maxPkt.wrd = M2R_REG_READ(regaddr) ;
      rx_maxPkt.bf.maxLength = (maxPktSize >> 16);
      M2R_REG_WRITE(regaddr, rx_maxPkt.wrd);

      regaddr++; /* Point to the LSB register */
      rx_maxPkt.wrd = (maxPktSize & 0x0000FFFF) ;
      M2R_REG_WRITE(regaddr, rx_maxPkt.wrd);

      /* Set the RX Min packet */
      regaddr = (cs_uint16 *) &pDev->slice[sl_num].Framer.MinLength.wrd +
                                      (str_iter * M2R_FR_STREAMRX__STRIDE) ;
      rx_minPkt.wrd = M2R_REG_READ(regaddr) ;
      rx_minPkt.bf.minLength = minPktSize ;
      M2R_REG_WRITE(regaddr, rx_minPkt.wrd);
    }

    if ( tx_dir ) {
      FR_STREAMTX_MaxPktMSB_t  tx_maxPkt ;
      FR_STREAMTX_MinPkt_t     tx_minPkt ;

      /* set RX pkt maxSize register */
      regaddr = (cs_uint16 *) &pDev->slice[sl_num].Framer.MaxPktMSB.wrd +
                                      (str_iter * M2R_FR_STREAMTX__STRIDE) ;
      tx_maxPkt.wrd = M2R_REG_READ(regaddr) ;
      tx_maxPkt.bf.MxLenM = (maxPktSize >> 16);
      M2R_REG_WRITE(regaddr, tx_maxPkt.wrd);

      regaddr++; /* Point to the LSB register */
      tx_maxPkt.bf.MxLenM = (maxPktSize & 0x0000FFFF) ;
      M2R_REG_WRITE(regaddr, tx_maxPkt.wrd);

      /* Set the TX Min packet */
      regaddr = (cs_uint16 *) &pDev->slice[sl_num].Framer.MinPkt.wrd +
                                      (str_iter * M2R_FR_STREAMTX__STRIDE) ;
      tx_minPkt.wrd = M2R_REG_READ(regaddr) ;
      tx_minPkt.bf.MnLenM = minPktSize ;
      M2R_REG_WRITE(regaddr, tx_minPkt.wrd);
    }
  }

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  SETS THE TIMING MODE OF THE FRAMER (OBSOLETE)*/
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_ctl_loop_timing(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Sonet clock loop-timing mode.                 */
/* When enabled, the Sonet Rx clock is used for Sonet Tx also.  */
/* User should always enable Loop Timing for line and section   */
/* loopback.                                                    */
/*                                                              */
/* Note : This API is for internal use only.                    */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  M2R_t                 * pDev ;
  FR_GLOBAL_LoopBack_t  loopback;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);

  loopback.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.LoopBack.wrd);
  loopback.bf.loopTimeMode = (ctl == M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.LoopBack.wrd, loopback.wrd);

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLES TERMINAL LOOPBACK            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_ctl_terminal_loopback(cs_uint16 port_id,
                                           m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Sonet Terminal loopback.                      */
/* When terminal loopback is enabled, Sonet TX Frame is looped  */
/* back. Also the TX clock replaces the RX clock.               */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  M2R_t                 * pDev ;
  FR_GLOBAL_LoopBack_t  loopback;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);
  loopback.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.LoopBack.wrd);
  loopback.bf.Terminal = (ctl == M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.LoopBack.wrd, loopback.wrd);
  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLES LINE LOOPBACK ON THE FRAMER  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_ctl_line_loopback(cs_uint16 port_id,
                                       m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Sonet Line loopback (also called facility LB).*/
/* When line loopback is enabled, Sonet RX frame is looped      */
/* back to TX. It takes places at the interface between LIF and */
/* Sonet Framer.                                                */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  M2R_t                 * pDev ;
  FR_GLOBAL_LoopBack_t  loopback;
  LIF_INTERFACE_t       interf ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);
  loopback.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.LoopBack.wrd);
  loopback.bf.Line = (ctl == M2R_ENABLE) ? 1 : 0 ;
  loopback.bf.loopTimeMode = (ctl == M2R_ENABLE) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.LoopBack.wrd, loopback.wrd);
  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER);

  interf.wrd = M2R_REG_READ(&pDev->LIF.INTERF.wrd) ;

  if (interf.bf.INTERFACE_SEL != INTERFACE_SEL_SFI41_XSBI) {
    m2r_port_cb_t   *ppcb = M2R_PORT_ID_TO_PCB_PTR(port_id) ;
    if (ppcb != NULL) {
      m2r_lif_set_clk_div(port_id,
        (ctl == M2R_ENABLE) ? M2R_LIF_TX_CLK_LOOP_INTERNAL : ppcb->tx_clk_mode) ;
    }
  }

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLES SECTION LOOPBACK             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_ctl_sonet_repeater(cs_uint16 port_id,
                                   m2r_ctl_t ctl, cs_boolean relay)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/*              o TRUE ( Relay J0/E1/F1 and DCC ) else FALSE    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Sonet Section loopback. When Section loopback */
/* is enabled, chip is configured in the regenerator mode i.e.  */
/* Sonet RX Frame is looped to TX at the section level.         */
/* When relay is enabled, J0/E1/F1 and DCC bytes as transmitted */
/* as they are received, else they are taken from the taken     */
/* either from framer register or TOH or trace buffer.          */
/* Not all options are avalibale for all the section overhead   */
/* bytes. For example J0 can be sourced from trace buffer or    */
/* TOH. User should select  the source of these bytes           */
/* by calling m2r_framer_set_tx_ovrhd_src() API.                */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                 sl_num ;
  M2R_t                   * pDev ;
  cs_uint8                  bfSec ;
  FR_TRANSMIT_Regen_t       regen ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  bfSec = (ctl == M2R_ENABLE) ? 1 : 0 ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);

  regen.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.Regen.wrd);
  /* relay J0/DCC/E1/F1 bytes */
  regen.bf.regenOn = bfSec ;
  regen.bf.DCCRel  = (relay) ? 1 : 0 ;
  regen.bf.J0Rel   = (relay) ? 1 : 0 ;
  regen.bf.E1Rel   = (relay) ? 1 : 0 ;
  regen.bf.F1Rel   = (relay) ? 1 : 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.Regen.wrd, regen.wrd);
  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER);
  return (CS_OK) ;
}

#define M2R_FR_SRESET_MSEC_DELAY           10
/****************************************************************/
/* $rtn_hdr_start  FRAMER RESET SEQUENCE CONTROL                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status m2r_framer_sreset_sequence(cs_uint16 port_id, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Logic reset enable/disable sequence for the framer.          */
/* Before configuring the framer, the application should        */
/* ideally call this to enable the reset sequence and again     */
/* call it after configuration to take it out of reset.         */
/* Note, that also when configuring other blocks for this port, */
/* it may be neccessary to keep the framer in logic reset.      */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  m2r_framer_ctl_dcom_stream_sreset(port_id, M2R_SONET_STREAM_ALL,
                                    M2R_TX_AND_RX, ctl) ;
  m2r_framer_ctl_mapper_stream_sreset(port_id, M2R_SONET_STREAM_ALL,
                                    M2R_TX_AND_RX, ctl) ;
  m2r_framer_ctl_sonet_sreset(port_id, M2R_TX_AND_RX, ctl) ;
  m2r_framer_ctl_domain_sreset(port_id, M2R_FR_ALL_DOMAINS, ctl) ;
  m2r_framer_ctl_overhead_sreset(port_id, M2R_TX_AND_RX, ctl) ;

  /* Add delay after deasserting reset */
  if (ctl == M2R_DISABLE) {
    CS_MDELAY(M2R_RESET_MSEC_DELAY) ;
  }

  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   FRAMER DOMAIN LOGIC/SOFT RESET              */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_ctl_domain_sreset(cs_uint16 port_id,
                                        m2r_framer_domain_t domain,
                                        m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Domain selection                              */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Domain(global) Logic reset of the framer      */
/* for the specified clock domain:                              */
/* M2R_FR_DOMAIN_SONET_TX (311MHz Sonet Transmit) or            */
/* M2R_FR_DOMAIN_SONET_RX (311MHz Sonet Receive) or             */
/* M2R_FR_DOMAIN_DATACOM  (200MHz System side) or               */
/* M2R_FR_ALL_DOMAINS                                           */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  FR_GLOBAL_MasterReset_t  rst ;
  cs_uint8                 bit_rst ;
  cs_status                status = CS_OK ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  bit_rst = (ctl == M2R_DISABLE) ? 0 : 1 ;
  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  rst.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.MasterReset.wrd) ;

  switch (domain) {
    case M2R_FR_DOMAIN_SONET_TX :
      rst.bf.TxLogicReset = bit_rst ; break ;
    case M2R_FR_DOMAIN_SONET_RX :
      rst.bf.RxLogicReset = bit_rst ; break ;
    case M2R_FR_DOMAIN_DATACOM :
      rst.bf.DtLogicReset = bit_rst ; break ;
    case M2R_FR_ALL_DOMAINS :
      rst.bf.TxLogicReset = rst.bf.RxLogicReset = rst.bf.DtLogicReset =
                            bit_rst ;
      break ;
    default :
      CS_HNDL_ERROR(port_id, EM2R_INVALID_FR_DOMAIN, NULL) ;
      status = CS_ERROR ;
      break ;
  }

  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.MasterReset.wrd, rst.wrd) ;

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return status ;
}

/****************************************************************/
/* $rtn_hdr_start   FRAMER SONET OVERHEAD SOFT(LOGIC) RESET     */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_ctl_overhead_sreset(cs_uint16 port_id,
                                     m2r_dir_t dir, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/*                                                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Sonet Overhead sub-block logic reset of framer*/
/* for the specified datapath direction.                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint8                 bit_rst ;
  cs_boolean               rx_dir , tx_dir ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  bit_rst = (ctl == M2R_DISABLE) ? 0 : 1 ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  if ( tx_dir ) {
    FR_XMITOVERHEAD_Ctrl_t  tx_rst ;
    tx_rst.wrd = M2R_REG_READ(
                  &pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd) ;
    tx_rst.bf.reset = bit_rst ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd,
                 tx_rst.wrd) ;
  }

  if ( rx_dir ) {
    FR_RECVOVERHEAD_Ctrl_t  rx_rst ;
    rx_rst.wrd =
      M2R_REG_READ(&pDev->slice[sl_num].Framer.recvOverhead_Ctrl.wrd) ;
    rx_rst.bf.reset = bit_rst ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.recvOverhead_Ctrl.wrd,
                 rx_rst.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   FRAMER SONET SOFT(LOGIC) RESET              */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_ctl_sonet_sreset(cs_uint16 port_id,
                                      m2r_dir_t dir, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/*                                                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Sonet sub-block logic reset of the framer     */
/* for the specified datapath direction:                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  cs_uint8                 bit_rst ;
  cs_boolean               rx_dir , tx_dir ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  bit_rst = (ctl == M2R_DISABLE) ? 0 : 1 ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  if ( rx_dir ) {
    FR_SONET32RX_Configuration_t rx_rst ;
    rx_rst.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd) ;
    rx_rst.bf.reset = bit_rst ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd,
                 rx_rst.wrd) ;
  }

  if ( tx_dir ) {
    FR_TRANSMIT_Configuration_t tx_rst ;
    tx_rst.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.transmit_Configuration.wrd) ;
    tx_rst.bf.Reset = bit_rst ;
    M2R_REG_WRITE(&pDev->slice[sl_num].Framer.transmit_Configuration.wrd,
                 tx_rst.wrd) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   FRAMER MAPPER STREAM SOFT(LOGIC) RESET      */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_ctl_mapper_stream_sreset(cs_uint16 port_id,
                                      m2r_framer_stream_t channel,
                                      m2r_dir_t dir, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/*                                                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Mapper's stream logic reset of framer.        */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                sl_num ;
  cs_uint32                str_start, str_end, str_iter;
  M2R_t                  * pDev ;
  cs_uint8                 bit_rst ;
  cs_boolean               rx_dir , tx_dir ;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;

  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  bit_rst = (ctl == M2R_DISABLE) ? 0 : 1 ;

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for (str_iter = str_start; str_iter < str_end; str_iter++) {
    if ( tx_dir ) {
      FR_STREAMTX_PktControl_t    tx_rst ;
      regaddr =
          (cs_reg *) (&pDev->slice[sl_num].Framer.streamTx_PktControl.wrd +
                (str_iter * M2R_FR_STREAMTX__STRIDE)) ;

      tx_rst.wrd = M2R_REG_READ(regaddr) ;
      tx_rst.bf.Reset = bit_rst ;
      M2R_REG_WRITE(regaddr, tx_rst.wrd) ;
    }

    if ( rx_dir ) {
      FR_STREAMRX_PktControl_t     rx_rst ;
      regaddr =
          (cs_reg *) (&pDev->slice[sl_num].Framer.streamRx_PktControl.wrd +
                (str_iter * M2R_FR_STREAMRX__STRIDE)) ;
      rx_rst.wrd = M2R_REG_READ(regaddr) ;
      rx_rst.bf.reset = bit_rst ;
      M2R_REG_WRITE(regaddr, rx_rst.wrd) ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   FRAMER DATACOM STREAM SOFT(LOGIC) RESET     */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_status  m2r_framer_ctl_dcom_stream_sreset(cs_uint16 port_id,
                                      m2r_framer_stream_t channel,
                                      m2r_dir_t dir, m2r_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Stream ID                                     */
/*              o M2R_RX or M2R_TX or M2R_TX_AND_RX             */
/*              o M2R_DISABLE(0) or M2R_ENABLE(1)               */
/*                                                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Mapper's stream logic reset of the framer.    */
/* The [channel] parameter is specified as following:           */
/* M2R_SONET_STREAM0 or M2R_SONET_STREAM1 or M2R_SONET_STREAM2  */
/* M2R_SONET_STREAM3 or M2R_SONET_STREAM_ALL                    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16                sl_num ;
  cs_uint32                str_start, str_end, str_iter;
  M2R_t                  * pDev ;
  cs_uint8                 bit_rst ;
  cs_boolean               rx_dir , tx_dir ;
  volatile cs_reg        * regaddr ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  M2R_FR_GET_CHANNEL_BOUNDS(str_start, str_end, channel) ;
  M2R_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;
  bit_rst = (ctl == M2R_DISABLE) ? 0 : 1 ;
  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER) ;

  for (str_iter = str_start; str_iter < str_end; str_iter++) {
    if ( tx_dir ) {
      FR_DATACOMTX_StrConfig_t    tx_rst ;
      regaddr =
          (cs_reg *) &pDev->slice[sl_num].Framer.datacomTx_StrConfig.wrd +
                (str_iter * M2R_FR_DATACOMTX_StrConfig_STRIDE) ;
      tx_rst.wrd = M2R_REG_READ(regaddr) ;
      tx_rst.bf.reset = bit_rst ;
      M2R_REG_WRITE(regaddr, tx_rst.wrd) ;
    }

    if ( rx_dir ) {
      FR_DATACOMRX_StrConfig_t     rx_rst ;
      regaddr =
          (cs_reg *) &pDev->slice[sl_num].Framer.datacomRx_StrConfig.wrd +
                (str_iter * M2R_FR_DATACOMRX_StrConfig_STRIDE) ;
      rx_rst.wrd = M2R_REG_READ(regaddr) ;
      rx_rst.bf.reset = bit_rst ;
      M2R_REG_WRITE(regaddr, rx_rst.wrd) ;
    }
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER) ;
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   GET FRAMERS SYNC STATUS                     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_boolean m2r_framer_get_sync_status(cs_uint16 port_id)
/* INPUTS     : o port id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : TRUE (In Sync) or FALSE (out of Sync)           */
/* DESCRIPTION:                                                 */
/* Get the Framer's sync status.                                */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  M2R_t                 * pDev ;
  cs_boolean            sync_status ;
  FR_SONET32RX_SectionStatus_t status ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, FALSE) ;
  status.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.SectionStatus.wrd) ;

  sync_status = (status.bf.InFrameS ? TRUE : FALSE ) ;
  return (sync_status) ;
}


/****************************************************************/
/* $rtn_hdr_start   GET SERVICE TYPE CONFIGURED FOR PATH        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
m2r_port_prot_t  m2r_framer_get_svc_type(cs_uint16 port_id,
                                        m2r_framer_stream_t channel)
/* INPUTS     : o port id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Return Service type configuration of the SONET Path.         */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  M2R_t                             * pDev ;
  cs_uint16                           sl_num ;
  FR_SONET32RX_PathProvision_t        rx_prvsn ;
  FR_STREAMRX_PktControl_t            rx_pktctl ;
  m2r_port_prot_t                     svc_type = M2R_PORT_PROT_POS ;

  if(channel > M2R_SONET_STREAM_ALL) {
    CS_HNDL_ERROR(port_id, EM2R_INVALID_STREAM, NULL) ;
    return (CS_ERROR);
  }

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  rx_prvsn.wrd = M2R_REG_READ(
                    &pDev->slice[sl_num].Framer.PathProvision.wrd +
                   (channel * M2R_FR_SONET32RX_PathProvision_STRIDE)) ;

  switch (rx_prvsn.bf.SVC) {
    case SVC_SRVC_ATM :
      svc_type = M2R_PORT_PROT_ATM ;
    break ;

    case SVC_SRVC_PPP :
      rx_pktctl.wrd = M2R_REG_READ(
                 &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd +
                (channel * M2R_FR_STREAMRX__STRIDE)) ;
      svc_type = ((rx_pktctl.bf.modeDropFF03) ?
                      M2R_PORT_PROT_PPP : M2R_PORT_PROT_POS) ;
    break ;

    case SVC_SRVC_GFP :
      svc_type = M2R_PORT_PROT_GFP ;
    break ;
  }

  return svc_type ;
}

/****************************************************************/
/* $rtn_hdr_start   GET CHANNELIZED MODE STATUS OF FRAMER       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Both                                            */
cs_boolean m2r_framer_is_in_channelised_mode(cs_uint16 port_id)
/* INPUTS     : o port id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Return TURE if framer is configure in channelized mode else  */
/* return FALSE.                                                */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_boolean                      channelized = FALSE ;
  FR_SONET32RX_PathProvision_t    path_prvsn ;
  M2R_t                         * pDev ;
  cs_uint16                       sl_num ;
  cs_boolean                      oc48_mode ;
  extern cs_boolean m2r_mpif_is_in_oc48_mode(cs_uint16 port_id) ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  oc48_mode = m2r_mpif_is_in_oc48_mode(port_id) ;

  /* Read Path 0 provision state */
  path_prvsn.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.PathProvision.wrd) ;

  if (((oc48_mode == FALSE) && (path_prvsn.bf.STSType == STSType_STS_48C)) ||
      ((oc48_mode == TRUE) && (path_prvsn.bf.STSType == STSType_STS_12C))) {
    channelized = TRUE ;
  }

  return (channelized) ;
}

/****************************************************************/
/* $rtn_hdr_start   CONFIGURE GPIO FOR INPUTS OR OUTPUTS        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : MILAN2                                          */
cs_status m2r_framer_cfg_gpio(cs_uint16 port_id,
			      cs_uint8 config,
			      cs_uint8 mask)
/* INPUTS     : o port id                                       */
/*            : o config, one bit for each channel;             */
/*                bit 7..0 <-> channel 7..0                     */
/*            : o mask, one bit for each channel;               */
/*                0 - don't care, 1 - configure it              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Selectively configure 8 of gpio channels as inputs or outputs*/
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  FR_GLOBAL_GPIOControl_t  gpioControl ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  gpioControl.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.GPIOControl.wrd) ;
  gpioControl.bf.DIR = (gpioControl.bf.DIR & ~mask) |
                     (config & mask);
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.GPIOControl.wrd, gpioControl.wrd);
  return (CS_OK) ;
}

/****************************************************************/
/* $rtn_hdr_start   READ GPIO INPUTS                            */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : MILAN2                                          */
cs_uint8 m2r_framer_get_gpio(cs_uint16 port_id,
			     cs_uint8 mask)
/* INPUTS     : o port id                                       */
/*            : o mask, one bit for each channel;               */
/*                bit 7..0 <-> channel 7..0                     */
/*                0 - don't care, 1 - get it                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    :                           */
/* DESCRIPTION:                                                 */
/* Selectively reads 8 of gpio channels                         */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  FR_GLOBAL_GPIOValue_t    gpioValue ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  gpioValue.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.GPIOValue.wrd) ;
  return (gpioValue.bf.I & mask) ;
}

/****************************************************************/
/* $rtn_hdr_start   WIRTE GPIO OUTPUTS                          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : MILAN2                                          */
cs_status m2r_framer_set_gpio(cs_uint16 port_id,
                              cs_uint8 data,
			      cs_uint8 mask)
/* INPUTS     : o port id                                       */
/*            : o output data, one bit for each channel;        */
/*                bit 7..0 <-> channel 7..0                     */
/*            : o mask, one bit for each channel;               */
/*                0 - don't change, 1 - set it                  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Selectively output 8 of gpio output channels                 */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16                sl_num ;
  M2R_t                  * pDev ;
  FR_GLOBAL_GPIOControl_t  gpioControl ;

  M2R_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  gpioControl.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.GPIOControl.wrd) ;
  gpioControl.bf.O = (gpioControl.bf.O & ~mask) |
                   (data & mask);
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.GPIOControl.wrd, gpioControl.wrd);
  return (CS_OK) ;
}


/************************* for debug only ************************/
