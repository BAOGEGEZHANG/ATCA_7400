/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */

#include "cs_types.h"
#include "arn5_registers.h"

#include "arn5_error.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_stats_priv.h"
#include "arn5_framer.h"
#include "arn5_framer_stats.h"
#include "arn5_framer_priv.h"
#include "arn5_framer_atm.h"
#include "arn5_framer_gfp.h"
#include "arn5_framer_pkt.h"
#include "arn5_framer_soh.h"


static
cs_status  arn5_framer_sel_sts_mode(cs_uint16 port_id,
                        arn5_framer_stsType_t sts_t) ;
static
cs_status  arn5_framer_set_rx_burst_len(cs_uint16 port_id,
                        cs_uint8  burst) ;

/****************************************************************/
/* $_hdr_start    FRAMER INITIALIZATION                         */
/* CATEGORY   : API                                             */
/* ACCESS     : Private                                         */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_init_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg)
/* INPUTS     : o Chip Id                                       */
/*              o Pointer to port configuration summary struct  */
/*              o Pointer to port configuration struct          */
/*                                                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This initializes the port level of the Framer block          */
/* with the given summary.                                      */
/*                                                              */
/* The [pPortCfg] parameter is a pointer to the configuration   */
/* data-structure, which has all the configuration parameters   */
/* set.                                                         */
/* The API expects the data-structure to be already allocated   */
/* and populated with the configuration settings.               */
/* $_hdr_end                                                    */
/****************************************************************/
{
  arn5_port_summ_t      * pSumm = &pPortCfg->summ ;
  arn5_framer_cfg_t     * pCfg  = &pPortCfg->framer ;
  arn5_framer_stsType_t stsType ;

  //CS_PRINT("\t Initializing FRAMER... \n") ;

  /* Configure in SONET or SDH mode. */
  if (arn5_framer_sel_sonet_sdh(port_id, pCfg->sonet_mode)
      != CS_OK) {
    return CS_ERROR ;
  }

  /* Configure sonet streams */
  if (pSumm->line_rate == ARN5_PORT_RATE_OC48C) {
    stsType = ARN5_FR_STS_TYPE_STS48C ;
  }
  else if (pSumm->line_rate == ARN5_PORT_RATE_OC12C) {
    stsType = ARN5_FR_STS_TYPE_STS12C ;
  }
  else {
    stsType = ARN5_FR_STS_TYPE_STS3C ;
  }

  if (arn5_framer_sel_sts_mode(port_id, stsType)
      != CS_OK ) {
    return (CS_ERROR) ;
  }

  arn5_framer_set_rx_burst_len(port_id, 0) ;

  if (arn5_framer_ctl_line_loopback(port_id,
        (pCfg->line_loopback ? CS_ENABLE : CS_DISABLE))
      != CS_OK ) {
    return (CS_ERROR) ;
  }


  if (arn5_framer_set_spe_service_type(port_id, pCfg->svc)
      != CS_OK) {
    return CS_ERROR ;
  }

  if (arn5_framer_ctl_scrambling(port_id, CS_RX_AND_TX,
                        (pCfg->sonet_scrambling ? CS_ENABLE : CS_DISABLE))
      != CS_OK) {
    return CS_ERROR ;
  }

  /*
   * Do POS or PPP specific initialisation.
   */
  if ( (pCfg->svc == ARN5_FR_SVC_POS_PPP ) ||
       (pCfg->svc == ARN5_FR_SVC_POS_HDLC) ||
       (pCfg->svc == ARN5_FR_SVC_LAPS) ) {
    /*
     * Configure the packet min / max values desired and rest will be
     * filtered.
     */
    if (arn5_framer_pkt_cfg_max_min_size(port_id,
                                    pCfg->mode.pos_cfg.max_pktSize,
                                    pCfg->mode.pos_cfg.min_pktSize,
                                    CS_RX_AND_TX)
        != CS_OK) {
      return CS_ERROR ;
    }

    if (arn5_framer_pkt_drop_long(port_id, CS_RX_AND_TX, CS_ENABLE)
        != CS_OK) {
      return CS_ERROR ;
    }

    if (arn5_framer_pkt_drop_short(port_id, CS_RX_AND_TX, CS_ENABLE)
        != CS_OK) {
      return CS_ERROR ;
    }

    if (arn5_framer_pkt_ctl_crc32(port_id, pCfg->mode.pos_cfg.crc32)
        != CS_OK) {
      return CS_ERROR ;
    }

    if (arn5_framer_pkt_ctl_fcs(port_id, pCfg->mode.pos_cfg.pktFCS)
        != CS_OK) {
      return CS_ERROR ;
    }

    if (arn5_framer_pkt_pass_fcs(port_id,
          (pCfg->mode.pos_cfg.passFCS ? CS_ENABLE : CS_DISABLE))
        != CS_OK) {
      return CS_ERROR ;
    }

    if (arn5_framer_pkt_ctl_scrambling(port_id,
          (pCfg->payload_scrambling ? CS_ENABLE : CS_DISABLE))
        != CS_OK) {
      return CS_ERROR ;
    }
  }
  else if ( pCfg->svc == ARN5_FR_SVC_GFP ) { /* GFP Specific init */
    if (arn5_framer_gfp_cfg_mode(port_id, pCfg->mode.gfp_cfg.mode)
        != CS_OK) {
      return CS_ERROR ;
    }

    if (arn5_framer_gfp_set_cut_through_mode(port_id,
         (pCfg->mode.gfp_cfg.cut_through ? CS_ENABLE : CS_DISABLE))
        != CS_OK) {
      return CS_ERROR ;
    }

    if (arn5_framer_pkt_cfg_max_min_size(port_id,
                                    pCfg->mode.gfp_cfg.max_pktSize,
                                    pCfg->mode.gfp_cfg.min_pktSize,
                                    CS_RX_AND_TX)
        != CS_OK) {
      return CS_ERROR ;
    }

    if (arn5_framer_gfp_drop_long(port_id, CS_RX_AND_TX, CS_ENABLE)
        != CS_OK) {
      return CS_ERROR ;
    }

    if (arn5_framer_gfp_drop_short(port_id, CS_RX_AND_TX, CS_ENABLE)
        != CS_OK) {
      return CS_ERROR ;
    }

    if (arn5_framer_gfp_ctl_scrambling(port_id,
         (pCfg->payload_scrambling ? CS_ENABLE : CS_DISABLE))
        != CS_OK) {
      return CS_ERROR ;
    }
  }
  else if ( pCfg->svc == ARN5_FR_SVC_ATM ) { /* ATM specific Init */
    if (arn5_framer_atm_ctl_hec(port_id,
         (pCfg->mode.atm_cfg.check_hec) ? CS_ENABLE : CS_DISABLE)
        != CS_OK) {
      return CS_ERROR ;
    }

    if (arn5_framer_atm_drop_idle(port_id,
         (pCfg->mode.atm_cfg.drop_idle ? CS_ENABLE : CS_DISABLE))
        != CS_OK) {
      return CS_ERROR ;
    }

    if (arn5_framer_atm_ctl_scrambling(port_id,
         (pCfg->payload_scrambling ? CS_ENABLE : CS_DISABLE))
        != CS_OK) {
      return CS_ERROR ;
    }
  }

  /* DCC Port.  */
  if (arn5_framer_cfg_dcc_port(port_id, pCfg->dcc_cfg,
        CS_TX_AND_RX, pCfg->enbl_dcc_prt ? CS_ENABLE : CS_DISABLE)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_framer_set_signal_thresholds(port_id,
         pCfg->line_sd.declare, pCfg->line_sd.clear,
         pCfg->line_sf.declare, pCfg->line_sf.clear)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_framer_set_path_signal_thresholds(port_id,
         pCfg->path_sf.declare, pCfg->path_sf.clear)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_framer_init_stats(port_id, pCfg->svc)
      != CS_OK ) {
    return (CS_ERROR) ;
  }

  /* Init Framer default IRQ */
  if (arn5_framer_init_irq_cfg(port_id, pSumm, pCfg)
      != CS_OK ) {
    return (CS_ERROR) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start    GET DEFAULT FRAMER CONFIGURATION              */
/* CATEGORY   : API                                             */
/* ACCESS     : Private                                         */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_get_default_cfg(cs_uint16 port_id,
                        arn5_port_cfg_t * pPortCfg)
/* INPUTS     : o Device Id                                     */
/*              o Pointer to configuration structure            */
/* OUTPUTS    : o Framer related default config                 */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the port's default Framer configuration based on         */
/* specified summary config info.                               */
/*                                                              */
/* The [pPortCfg] parameter is a pointer to the configuration   */
/* data-structure. The API will fill in this structure with     */
/* default settings based on the config summary info.           */
/*                                                              */
/* The API expects the config summary filled.                   */
/* $_hdr_end                                                    */
/****************************************************************/
{
  arn5_port_summ_t     * pSumm = &pPortCfg->summ ;
  arn5_framer_cfg_t    * pCfg  = &pPortCfg->framer ;

  pCfg->sonet_mode = TRUE ; /* Sonet Mode */
  pCfg->line_sd.declare = ARN5_FR_BER_6 ;
  pCfg->line_sd.clear = ARN5_FR_BER_7 ;
  pCfg->line_sf.declare = ARN5_FR_BER_4 ;
  pCfg->line_sf.clear = ARN5_FR_BER_5 ;
  pCfg->path_sf.declare = ARN5_FR_BER_4 ;
  pCfg->path_sf.clear = ARN5_FR_BER_5 ;

  pCfg->enbl_dcc_prt = FALSE ;
  pCfg->payload_scrambling = TRUE ;
  pCfg->sonet_scrambling = TRUE ;
  pCfg->line_loopback    = FALSE ;

  switch(pSumm->l2_prot) {
    case ARN5_PORT_PROT_POS :
      pCfg->mode.pos_cfg.pktFCS = TRUE ;
      pCfg->mode.pos_cfg.crc32 = TRUE ;
      pCfg->mode.pos_cfg.passFCS = FALSE ;
      pCfg->mode.pos_cfg.max_pktSize = ARN5_FRAMER_PKT_MAX_SIZE_DEF ;
      pCfg->mode.pos_cfg.min_pktSize = ARN5_FRAMER_PKT_MIN_SIZE_DEF ;
      pCfg->svc = ARN5_FR_SVC_POS_HDLC ;
    break ;

    case ARN5_PORT_PROT_PPP :
      pCfg->mode.pos_cfg.pktFCS = TRUE ;
      pCfg->mode.pos_cfg.crc32 = TRUE ;
      pCfg->mode.pos_cfg.passFCS = FALSE ;
      pCfg->mode.pos_cfg.max_pktSize = ARN5_FRAMER_PKT_MAX_SIZE_DEF ;
      pCfg->mode.pos_cfg.min_pktSize = ARN5_FRAMER_PKT_MIN_SIZE_DEF ;
      pCfg->svc = ARN5_FR_SVC_POS_PPP ;
    break ;

    case ARN5_PORT_PROT_LAPS:
      pCfg->mode.pos_cfg.pktFCS = TRUE ;
      pCfg->mode.pos_cfg.crc32 = FALSE ;
      pCfg->mode.pos_cfg.passFCS = FALSE ;
      pCfg->mode.pos_cfg.max_pktSize = ARN5_FRAMER_PKT_MAX_SIZE_DEF ;
      pCfg->mode.pos_cfg.min_pktSize = ARN5_FRAMER_PKT_MIN_SIZE_DEF ;
      pCfg->svc = ARN5_FR_SVC_LAPS;
    break ;

    case ARN5_PORT_PROT_ATM :
      pCfg->mode.atm_cfg.check_hec = TRUE ;
      pCfg->mode.atm_cfg.drop_idle = TRUE ;
      pCfg->svc = ARN5_FR_SVC_ATM ;
    break ;

    case ARN5_PORT_PROT_GFP :
      pCfg->mode.gfp_cfg.mode = ARN5_FR_GFP_NORMAL ;
      pCfg->mode.gfp_cfg.cut_through = FALSE ;
      pCfg->mode.gfp_cfg.max_pktSize = ARN5_FRAMER_PKT_MAX_SIZE_DEF ;
      pCfg->mode.gfp_cfg.min_pktSize = ARN5_FRAMER_PKT_MIN_SIZE_DEF ;
      pCfg->svc = ARN5_FR_SVC_GFP ;
    break ;

    case ARN5_PORT_PROT_CLEAR_CH :
      pCfg->svc = ARN5_FR_SVC_CLR_CH ;
    break ;

    default :
      CS_HNDL_ERROR( port_id, EARN5_PORT_INVALID_USER_ARG,
                     "pSumm->l2_prot = %d\n", pSumm->l2_prot) ;
      return (CS_ERROR) ;
    break ;
  }

  /* Get the Default IRQ Configuration */
  arn5_framer_get_default_irq_cfg(port_id, pSumm, pCfg) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start    GET RUNNING LIF CONFIGURATION                 */
/* CATEGORY   : API                                             */
/* ACCESS     : Private                                         */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_get_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg)
/* INPUTS     : o Device Id                                     */
/*              o Pointer to running configuration structure    */
/* OUTPUTS    : o Framer related running config                 */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Returns the current run-time LIF hardware configuration      */
/* of the specified port.                                       */
/* The [pRunCfg] parameter is a pointer to the port's complete  */
/* configuration data-structure.                                */
/*                                                              */
/* This API will fill in the all the fields in the block        */
/* run-time configuration datastructure.                        */
/* $_hdr_end                                                    */
/****************************************************************/
{
  ARN5_FR_SONET32RX_Configuration_t cfg ;
  ARN5_t                * pDev ;
  cs_uint16             sl_num ;
  cs_reg                * regaddr ;
  arn5_port_summ_t      * pSumm ;
  arn5_framer_runcfg_t  * pCfg ;

  pSumm = &pRunCfg->summ ;
  pCfg = &pRunCfg->framer ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  cfg.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd) ;
  pCfg->sonet_mode = ((cfg.bf.SDH) ? FALSE : TRUE ) ;

  switch (arn5_framer_get_svc_type(port_id)) {
    case ARN5_PORT_PROT_ATM :
    break ;

    case ARN5_PORT_PROT_GFP : {
      ARN5_FR_GFP_TX_GFP_Control_0_t     tx_ctrl0 ;

      tx_ctrl0.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.gfp_tx_GFP_Control_0.wrd) ;

      if (tx_ctrl0.bf.Gen_CRC &&
          tx_ctrl0.bf.Gen_Pld_Header && tx_ctrl0.bf.Gen_THEC ) {
        pCfg->mode.gfp_cfg.mode = ARN5_FR_GFP_NORMAL ;
      }
      else {
        pCfg->mode.gfp_cfg.mode = ARN5_FR_GFP_TRANSPARENT ;
      }
      pCfg->mode.gfp_cfg.cut_through  = tx_ctrl0.bf.Enable_Cut_Through ;
      pCfg->mode.gfp_cfg.payload_scrambling = tx_ctrl0.bf.Dis_Scrambler ;
    }
    break ;

    case ARN5_PORT_PROT_PPP :
    case ARN5_PORT_PROT_POS : {
      ARN5_FR_STREAMRX_PktControl_t            rx_pktctl ;
      regaddr = (cs_reg *)
                  &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd ;
      rx_pktctl.wrd = CS_REG_READ(regaddr) ;
      pCfg->mode.pos_cfg.crc32 = rx_pktctl.bf.crc32 ;
      pCfg->mode.pos_cfg.pktFCS =
          ((rx_pktctl.bf.nullFCS) ? FALSE : TRUE) ;
      pCfg->mode.pos_cfg.pass_fcs = rx_pktctl.bf.passFCS ;
      pCfg->mode.pos_cfg.payload_scrambling = rx_pktctl.bf.disDscr ;
    }
    break ;

    case ARN5_PORT_PROT_LAPS:
    {
      ARN5_FR_STREAMRX_PktControl_t            rx_pktctl ;
      ARN5_FR_STREAMRX_PktX86Ctrl_t            x86Ctl ;
      regaddr = (cs_reg *)
                  &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd ;
      rx_pktctl.wrd = CS_REG_READ(regaddr) ;
      pCfg->mode.pos_cfg.crc32 = rx_pktctl.bf.crc32 ;
      pCfg->mode.pos_cfg.pktFCS =
          ((rx_pktctl.bf.nullFCS) ? FALSE : TRUE) ;
      pCfg->mode.pos_cfg.pass_fcs = rx_pktctl.bf.passFCS ;
      regaddr = (cs_reg *)
                  &pDev->slice[sl_num].Framer.PktX86Ctrl.wrd ;

      pCfg->mode.pos_cfg.payload_scrambling = x86Ctl.bf.X86DisDscr ;
    }
    break ;

    case ARN5_PORT_PROT_CLEAR_CH :
    case ARN5_PORT_PROT_ETHERNET :
    default:
      return (CS_ERROR);
  }

  /* line loopback */
  {
    ARN5_FR_GLOBAL_LoopBack_t      loopback;

    loopback.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.LoopBack.wrd);
    pCfg->line_loopback = (loopback.bf.Line == 1) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start   DUMP RUNNING FRAMER CONFIGURATION              */
/* CATEGORY   : API                                             */
/* ACCESS     : Private                                         */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_dump_running_cfg(cs_uint16 port_id,
                        arn5_port_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to running configuration structure    */
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
/* $_hdr_end                                                    */
/****************************************************************/
{
  arn5_framer_runcfg_t   * pCfg = &pRunCfg->framer ;

  CS_PRINT("Framer Mode             = %s\n",
           (pCfg->sonet_mode ? "SONET" : "SDH")) ;

  CS_PRINT("Line Loopback           = %s\n",
           (pCfg->line_loopback ? "Enabled" : "Disabled")) ;

  switch (arn5_framer_get_svc_type(port_id)) {
    case ARN5_PORT_PROT_ATM :
      CS_PRINT("\n\t*** In ATM Mode ***\n") ;
    break ;

    case ARN5_PORT_PROT_GFP :
      CS_PRINT("\n\t*** In GFP Mode ***\n") ;
      CS_PRINT("GFP Mode                = %s\n",
               ((pCfg->mode.gfp_cfg.mode == ARN5_FR_GFP_NORMAL) ?
                "Normal" : "Transparent")) ;

      CS_PRINT("Cut through             = %s\n",
               (pCfg->mode.gfp_cfg.cut_through ? "Yes" : "No")) ;

      CS_PRINT("Scrambler               = %s\n",
               ((pCfg->mode.gfp_cfg.payload_scrambling) ?
                "DISABLED" : "ENABLED")) ;
    break ;

    case ARN5_PORT_PROT_PPP :
    case ARN5_PORT_PROT_POS :
      CS_PRINT("\n\t*** In POS/PPP Mode ***\n") ;
      CS_PRINT("FCS Mode                = %s\n",
               (pCfg->mode.pos_cfg.pktFCS ? "Normal" : "NULL")) ;

      CS_PRINT("CRC Mode                = %s\n",
               (pCfg->mode.pos_cfg.crc32 ? "CRC32" : "CRC16")) ;

      CS_PRINT("FCS Passed to HOST      = %s\n",
               (pCfg->mode.pos_cfg.pass_fcs ? "YES" : "NO")) ;

      CS_PRINT("Scrambler               = %s\n",
               (pCfg->mode.pos_cfg.payload_scrambling ?
                "DISABLED" : "ENABLED")) ;
    break ;

    case ARN5_PORT_PROT_LAPS :
      CS_PRINT("\n\t*** In LAPS Mode ***\n") ;
      CS_PRINT("FCS Mode                = %s\n",
               (pCfg->mode.pos_cfg.pktFCS ? "Normal" : "NULL")) ;

      CS_PRINT("CRC Mode                = %s\n",
               (pCfg->mode.pos_cfg.crc32 ? "CRC32" : "CRC16")) ;

      CS_PRINT("FCS Passed to HOST      = %s\n",
               (pCfg->mode.pos_cfg.pass_fcs ? "YES" : "NO")) ;

      CS_PRINT("Scrambler               = %s\n",
               (pCfg->mode.pos_cfg.payload_scrambling ?
                "DISABLED" : "ENABLED")) ;
    break ;

    case ARN5_PORT_PROT_CLEAR_CH :
    case ARN5_PORT_PROT_ETHERNET :
    default:
      return (CS_ERROR);
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  SELECTS THE FRAMER MODE (SONET OR SDH)       */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_sel_sonet_sdh(cs_uint16 port_id,
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
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  ARN5_FR_SONET32RX_Configuration_t cfg ;
  ARN5_FR_XMITOVERHEAD_Ctrl_t       tx_ovrhd ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  cfg.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd) ;

  cfg.bf.SDH = (sonet_mode) ? 0 : 1 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd,
               cfg.wrd) ;

  /* TX overhead ctrl . Set the sonet/sdh mode for Z0 byte */
  tx_ovrhd.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd) ;
  tx_ovrhd.bf.SDHMode = (sonet_mode) ? 0 : 1 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd,
               tx_ovrhd.wrd) ;

  //20101120. judd add LOS Control
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.X_LOS_Config.wrd, 0x0);
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLE SONET (DE)SCRAMBLER           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_ctl_scrambling(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_RX_AND_TX                */
/*              o CS_DISABLE (0) or CS_ENABLE (1)               */
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
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_boolean            rx_dir , tx_dir ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if (tx_dir) {
    ARN5_FR_TRANSMIT_SectionControl_t  tx_ctrl ;

    tx_ctrl.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.SectionControl.wrd) ;
    tx_ctrl.bf.DisScr = (ctl == CS_DISABLE) ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.SectionControl.wrd,
                 tx_ctrl.wrd) ;
  }
  if (rx_dir) {
    ARN5_FR_SONET32RX_SectionLineControl_t rx_ctrl ;

    rx_ctrl.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.SectionLineControl.wrd) ;
    rx_ctrl.bf.DisDScr = (ctl == CS_DISABLE) ? 1 : 0 ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.SectionLineControl.wrd,
                 rx_ctrl.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/*                  SELECTS THE SONET STS MODE                  */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
static
cs_status  arn5_framer_sel_sts_mode(cs_uint16 port_id,
                        arn5_framer_stsType_t sts_t)
/* INPUTS     : o Port Id                                       */
/*              o STS Mode                                      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the STS mode of the Framer                        */
/*                                                              */
/* The [sts_t] parameter is specified as following:             */
/* ARN5_FR_MODE_STS48C   or   ARN5_FR_MODE_STS12C   or          */
/* ARN5_FR_MODE_STS3C                                           */
/*                                                              */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_TRANSMIT_PathProvisioning_t      tx_prvsn ;
  ARN5_FR_SONET32RX_PathProvision_t        rx_prvsn ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER);

  /* RX PathProvision */
  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathProvision.wrd ;
  rx_prvsn.wrd = CS_REG_READ(regaddr) ;
  rx_prvsn.bf.STSType = sts_t ;
  CS_REG_WRITE(regaddr, rx_prvsn.wrd) ;

  /* TX PathProvision */
  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathProvisioning.wrd ;
  tx_prvsn.wrd = CS_REG_READ(regaddr) ;
  tx_prvsn.bf.STStype = sts_t ;
  CS_REG_WRITE(regaddr, tx_prvsn.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}




/****************************************************************/
/* $rtn_hdr_start  SELECT THE SERVICE TYPE CARRIED BY SONET SPE */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_set_spe_service_type(cs_uint16 port_id,
                        arn5_framer_svc_type_t svc)
/* INPUTS     : o Port Id                                       */
/*              o Sonet/SDH service type                        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the SONET service type from POS/PPP/ATM and GFP.  */
/* If the framer is configured in channelized mode, each stream */
/* can be configured independently to carry a different service.*/
/* This API applies to both TX and RX both                      */
/*                                                              */
/* The [svc] parameter is specified as following:               */
/* ARN5_FR_SVC_ATM or ARN5_FR_SVC_POS_PPP or ARN5_FR_SVC_POS_HDLC */
/* ARN5_FR_SVC_GFP or ARN5_FR_SVC_LAPS    or ARN5_FR_SVC_CLR_CH */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_uint8              strData, bfPOS = 0, bfSvc = 0, bfModeFF03 = 0 ;
  cs_uint8              c2_byte  = 0x00 ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_DATACOMTX_StrConfig_t         tx_config ;
  ARN5_FR_STREAMTX_PktControl_t         tx_pktctl ;
  ARN5_FR_TRANSMIT_PathProvisioning_t   tx_prvsn ;
  ARN5_FR_DATACOMRX_StrConfig_t         rx_config ;
  ARN5_FR_STREAMRX_PktControl_t         rx_pktctl ;
  ARN5_FR_SONET32RX_PathProvision_t     rx_prvsn ;
  ARN5_FR_STREAMRX_PktX86Ctrl_t         x86Ctl;
  arn5_port_cb_t                        * ppcb ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  ppcb = ARN5_PORT_ID_TO_PCB_PTR(port_id) ;

  switch ( svc ) {
    case ARN5_FR_SVC_ATM :
      if (ppcb->summ.l2_prot != ARN5_PORT_PROT_ATM) {
        CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
          ": Sevice type (svc) conflicts with port summary.\n") ;
        return(CS_ERROR) ;
      }
      strData = ARN5_dataType_CELL ;
      bfSvc = ARN5_SVC_SRVC_ATM ;
      c2_byte = ARN5_SONET_C2_ATM ;
      break ;

    case ARN5_FR_SVC_POS_PPP :
      if (ppcb->summ.l2_prot != ARN5_PORT_PROT_PPP) {
        CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
          ": Sevice type (svc) conflicts with port summary.\n") ;
        return(CS_ERROR) ;
      }
      strData = ARN5_dataType_PACKET ;
      bfPOS = 1 ;
      //bfModeFF03 = 1 ;
      bfModeFF03 = 0;   //even in ppp mode, we keep the ff03 bytes in frame judd@20101117  
      bfSvc = ARN5_SVC_SRVC_PPP ;
      c2_byte = ARN5_SONET_C2_HDLC ;
      break ;

    case ARN5_FR_SVC_POS_HDLC :
      if (ppcb->summ.l2_prot != ARN5_PORT_PROT_POS) {
        CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
          ": Sevice type (svc) conflicts with port summary.\n") ;
        return(CS_ERROR) ;
      }
      strData = ARN5_dataType_PACKET ;
      bfPOS = 1 ;
      bfModeFF03 = 0;
      bfSvc = ARN5_SVC_SRVC_PPP ;
      c2_byte = ARN5_SONET_C2_HDLC ;
      break ;

    case ARN5_FR_SVC_GFP :
      if (ppcb->summ.l2_prot != ARN5_PORT_PROT_GFP) {
        CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
          ": Sevice type (svc) conflicts with port summary.\n") ;
        return(CS_ERROR) ;
      }
      strData = ARN5_dataType_PACKET ;
      bfSvc = ARN5_SVC_SRVC_GFP ;
      c2_byte = ARN5_SONET_C2_GFP ;
      break ;

    case ARN5_FR_SVC_LAPS :
      if (ppcb->summ.l2_prot != ARN5_PORT_PROT_LAPS) {
        CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
          ": Sevice type (svc) conflicts with port summary.\n") ;
        return(CS_ERROR) ;
      }
      strData = ARN5_dataType_PACKET ;
      bfPOS = 1 ;
      bfModeFF03 = 1;
      bfSvc = ARN5_SVC_SRVC_PPP ;
      c2_byte = ARN5_SONET_C2_LAPS;
      break ;

    case ARN5_FR_SVC_PRBS :
      if (ppcb->summ.l2_prot != ARN5_PORT_PROT_POS) {
        CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
          ": Sevice type (svc) conflicts with port summary.\n") ;
        return(CS_ERROR) ;
      }
      strData = ARN5_dataType_PACKET ;
      bfSvc = ARN5_SVC_SRVC_PRBS ;
      c2_byte = ARN5_SONET_C2_HDLC ;
      break ;

    case ARN5_FR_SVC_CLR_CH :
      if (ppcb->summ.l2_prot != ARN5_PORT_PROT_CLEAR_CH) {
        CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
          ": Sevice type (svc) conflicts with port summary.\n") ;
        return(CS_ERROR) ;
      }
      strData = ARN5_dataType_CLEAR ;
      break ;

    default :
        CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG, NULL) ;
        return (CS_ERROR) ;
  }

  /*
   * Set the expected C2 ( path label ) and the C2 label to be sent.
   */
  arn5_framer_set_overheadByte(port_id, ARN5_FR_OVRHD_C2, c2_byte) ;
  arn5_framer_set_rx_path_label(port_id, c2_byte) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER);

  /* RX stream config */
  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.datacomRx_StrConfig.wrd ;
  rx_config.wrd = CS_REG_READ(regaddr) ;
  rx_config.bf.dataType = strData ;
  CS_REG_WRITE(regaddr, rx_config.wrd) ;

  /* RX PathProvision (modePOS) */
  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathProvision.wrd ;
  rx_prvsn.wrd = CS_REG_READ(regaddr) ;
  rx_prvsn.bf.SVC = bfSvc ;
  CS_REG_WRITE(regaddr, rx_prvsn.wrd) ;

  /* RX Pkt/cell control */
  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd ;
  rx_pktctl.wrd = CS_REG_READ(regaddr) ;
  rx_pktctl.bf.modePOS = bfPOS ;
  rx_pktctl.bf.DropAddrCtrl = bfModeFF03 ;
  rx_pktctl.bf.DropSAPI = bfModeFF03 ;
  CS_REG_WRITE(regaddr, rx_pktctl.wrd) ;

  /* TX stream config */
  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.datacomTx_StrConfig.wrd ;
  tx_config.wrd = CS_REG_READ(regaddr) ;
  tx_config.bf.dataType = strData;
  CS_REG_WRITE(regaddr, tx_config.wrd) ;

  /* TX PathProvision (modePOS) */
  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PathProvisioning.wrd ;
  tx_prvsn.wrd = CS_REG_READ(regaddr) ;
  tx_prvsn.bf.SVC = bfSvc ;
  CS_REG_WRITE(regaddr, tx_prvsn.wrd) ;

  /* TX Pkt/cell control */
  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_PktControl.wrd ;
  tx_pktctl.wrd = CS_REG_READ(regaddr) ;
  tx_pktctl.bf.PosMode = bfPOS ;
  tx_pktctl.bf.InsAddrCtrl = bfModeFF03 ;
  tx_pktctl.bf.InsSAPI = bfModeFF03 ;
  CS_REG_WRITE(regaddr, tx_pktctl.wrd) ;

  /* RX pkt X86 control */
  regaddr = (cs_reg *) &pDev->slice[sl_num].Framer.PktX86Ctrl.wrd ;
  x86Ctl.wrd = CS_REG_READ(regaddr) ;
  x86Ctl.bf.X86En = (svc == ARN5_FR_SVC_LAPS) ? 1 : 0 ;
  CS_REG_WRITE(regaddr, x86Ctl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/*                  CONFIGURE RX DATACOM BUS BURST LENGTH       */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
static
cs_status  arn5_framer_set_rx_burst_len(cs_uint16 port_id,
                        cs_uint8  burst)
/* INPUTS     : o Port Id                                       */
/*              o Burst length                                  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configure datacom bus Rx burst length. The stream will attempt*/
/* to send at least burstLength number of words before          */
/* relinquishing the bus to another stream that can send data.  */
/* If Framer is in channelized mode, this API should always be  */
/* called since the default value of burst length is Zero.      */
/* burst field should be set to multiples of seven if the stream*/
/* is configured for cells. It should be set to zero if the     */
/* channel is not provisioned. If all the streams are configured*/
/* to zero then stream zero will always have the bus. The valid */
/* range for this is 1 to 128 words.                            */
/*                                                              */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_DATACOMRX_StrConfig_t            rx_config ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  /* RX stream config */
  regaddr =
      (cs_reg *) &pDev->slice[sl_num].Framer.datacomRx_StrConfig.wrd ;
  rx_config.wrd = CS_REG_READ(regaddr) ;
  rx_config.bf.burstLength = burst;
  CS_REG_WRITE(regaddr, rx_config.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start SET TX DATACOM BUS FIFO FULL THRESHOLD        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_set_tx_fifo_high_mark(cs_uint16 port_id,
                        cs_boolean atm, cs_uint8  threshold)
/* INPUTS     : o Port Id                                       */
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
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  volatile cs_reg       * regaddr ;
  ARN5_FR_DATACOMTX_StrConfig_t    tx_config ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  regaddr =
      (cs_reg *) &pDev->slice[sl_num].Framer.datacomTx_StrConfig.wrd ;
  tx_config.wrd = CS_REG_READ(regaddr) ;
  if ( atm ) {
      tx_config.bf.cellSkid = threshold ;
      tx_config.bf.packetSkid = 0 ;
  }
  else {
      tx_config.bf.packetSkid = threshold ;
      tx_config.bf.cellSkid = 0 ;
  }
  CS_REG_WRITE(regaddr, tx_config.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   CONFIGURES THE MAX/MIN SIZE OF THE PACKET   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_pkt_cfg_max_min_size(cs_uint16 port_id,
                        cs_uint32 maxPktSize,
                        cs_uint8 minPktSize, cs_dir_t dir)
/* INPUTS     : o Port Id                                       */
/*              o MAX  Packet Size                              */
/*              o MIN  Packet Size                              */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configures the size of the maximum and minimum packets. Any  */
/* packet greater than max packet or less than min size will    */
/* be counted in stats. Also user can enable/disable filtering  */
/* of the packets which are not in this range through the       */
/* following APIs :                                             */
/* Packet Mode ( POS/PPP/LAPS ) :                               */
/* arn5_framer_pkt_drop_long() API for long packets             */
/* arn5_framer_pkt_drop_short() API for short packets.          */
/*                                                              */
/* For GFP :                                                    */
/* arn5_framer_gfp_drop_long() API for ling packets             */
/* arn5_framer_gfp_drop_short() API for short packets.          */
/* If Stream is configured for GFP Mode, max packet can be      */
/* 0xffff.                                                      */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_boolean            rx_dir , tx_dir ;
  volatile cs_reg       * regaddr ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  if ( minPktSize > maxPktSize ) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_MIN_GT_MAX, NULL) ;
    return (CS_ERROR);
  }

  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  if ( rx_dir ) {
    ARN5_FR_STREAMRX_MaxLengthMSB_t  rx_maxPkt ;
    ARN5_FR_STREAMRX_MinLength_t     rx_minPkt ;

    /* set RX pkt maxSize register */
    regaddr = (cs_uint16 *) &pDev->slice[sl_num].Framer.MaxLengthMSB.wrd ;
    rx_maxPkt.wrd = CS_REG_READ(regaddr) ;
    rx_maxPkt.bf.maxLength = (maxPktSize >> 16);
    CS_REG_WRITE(regaddr, rx_maxPkt.wrd);

    regaddr++; /* Point to the LSB register */
    rx_maxPkt.wrd = (maxPktSize & 0x0000FFFF) ;
    CS_REG_WRITE(regaddr, rx_maxPkt.wrd);

    /* Set the RX Min packet */
    regaddr = (cs_uint16 *) &pDev->slice[sl_num].Framer.MinLength.wrd ;
    rx_minPkt.wrd = CS_REG_READ(regaddr) ;
    rx_minPkt.bf.minLength = minPktSize ;
    CS_REG_WRITE(regaddr, rx_minPkt.wrd);
  }

  if ( tx_dir ) {
    ARN5_FR_STREAMTX_MaxPktMSB_t  tx_maxPkt ;
    ARN5_FR_STREAMTX_MinPkt_t     tx_minPkt ;

    /* set RX pkt maxSize register */
    regaddr = (cs_uint16 *) &pDev->slice[sl_num].Framer.MaxPktMSB.wrd ;
    tx_maxPkt.wrd = CS_REG_READ(regaddr) ;
    tx_maxPkt.bf.MxLenM = (maxPktSize >> 16);
    CS_REG_WRITE(regaddr, tx_maxPkt.wrd);

    regaddr++; /* Point to the LSB register */
    tx_maxPkt.bf.MxLenM = (maxPktSize & 0x0000FFFF) ;
    CS_REG_WRITE(regaddr, tx_maxPkt.wrd);

    /* Set the TX Min packet */
    regaddr = (cs_uint16 *) &pDev->slice[sl_num].Framer.MinPkt.wrd ;
    tx_minPkt.wrd = CS_REG_READ(regaddr) ;
    tx_minPkt.bf.MnLenM = minPktSize ;
    CS_REG_WRITE(regaddr, tx_minPkt.wrd);
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  ENABLE/DISABLES LINE LOOPBACK ON THE FRAMER  */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_ctl_line_loopback(cs_uint16 port_id,
                        cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Sonet Line loopback (also called facility LB).*/
/* When line loopback is enabled, Sonet RX frame is looped      */
/* back to TX. It takes places at the interface between LIF and */
/* Sonet Framer. Also when enabled, loop timing should be used. */
/* arn5_lif_ctl_loop_timing() API should be used                */
/* to enable loop-timing.                                       */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  ARN5_FR_GLOBAL_LoopBack_t      loopback;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER);
  loopback.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.LoopBack.wrd);
  loopback.bf.Line = (ctl == CS_ENABLE) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.LoopBack.wrd, loopback.wrd);
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER);
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start  FRAMER RESET SEQUENCE CONTROL                */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_sreset_sequence(cs_uint16 port_id, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
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
  arn5_framer_ctl_dcom_stream_sreset(port_id, CS_TX_AND_RX, ctl) ;
  arn5_framer_ctl_mapper_stream_sreset(port_id, CS_TX_AND_RX, ctl) ;
  arn5_framer_ctl_sonet_sreset(port_id, CS_TX_AND_RX, ctl) ;
  arn5_framer_ctl_domain_sreset(port_id, ARN5_FR_ALL_DOMAINS, ctl) ;
  arn5_framer_ctl_overhead_sreset(port_id, CS_TX_AND_RX, ctl) ;

  /* Add delay after deasserting reset */
  if (ctl == CS_DISABLE) {
    CS_MDELAY(ARN5_RESET_MSEC_DELAY) ;
  }

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   FRAMER DOMAIN LOGIC/SOFT RESET              */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_ctl_domain_sreset(cs_uint16 port_id,
                        arn5_framer_domain_t domain, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o Domain selection                              */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Domain(global) Logic reset of the framer      */
/* for the specified clock domain:                              */
/* ARN5_FR_DOMAIN_SONET_TX (311MHz Sonet Transmit) or           */
/* ARN5_FR_DOMAIN_SONET_RX (311MHz Sonet Receive) or            */
/* ARN5_FR_DOMAIN_DATACOM  (200MHz System side) or              */
/* ARN5_FR_ALL_DOMAINS                                          */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  ARN5_FR_GLOBAL_MasterReset_t  rst ;
  cs_uint8              bit_rst ;
  cs_status             status = CS_OK ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  bit_rst = (ctl == CS_DISABLE) ? 0 : 1 ;
  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  rst.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.MasterReset.wrd) ;

  switch (domain) {
    case ARN5_FR_DOMAIN_SONET_TX :
      rst.bf.TxLogicReset = bit_rst ; break ;
    case ARN5_FR_DOMAIN_SONET_RX :
      rst.bf.RxLogicReset = bit_rst ; break ;
    case ARN5_FR_DOMAIN_DATACOM :
      rst.bf.DtLogicReset = bit_rst ; break ;
    case ARN5_FR_ALL_DOMAINS :
      rst.bf.TxLogicReset = rst.bf.RxLogicReset = rst.bf.DtLogicReset =
                            bit_rst ;
      break ;
    default :
      CS_HNDL_ERROR(port_id, EARN5_INVALID_FR_DOMAIN, NULL) ;
      status = CS_ERROR ;
      break ;
  }

  CS_REG_WRITE(&pDev->slice[sl_num].Framer.MasterReset.wrd, rst.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return status ;
}


/****************************************************************/
/* $rtn_hdr_start   FRAMER SONET OVERHEAD SOFT(LOGIC) RESET     */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_ctl_overhead_sreset(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/*                                                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Sonet Overhead sub-block logic reset of framer*/
/* for the specified datapath direction.                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_uint8              bit_rst ;
  cs_boolean            rx_dir , tx_dir ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  bit_rst = (ctl == CS_DISABLE) ? 0 : 1 ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ( tx_dir ) {
    ARN5_FR_XMITOVERHEAD_Ctrl_t  tx_rst ;
    tx_rst.wrd = CS_REG_READ(
                  &pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd) ;
    tx_rst.bf.reset = bit_rst ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.xmitOverhead_Ctrl.wrd,
                 tx_rst.wrd) ;
  }

  if ( rx_dir ) {
    ARN5_FR_RECVOVERHEAD_Ctrl_t  rx_rst ;
    rx_rst.wrd =
      CS_REG_READ(&pDev->slice[sl_num].Framer.recvOverhead_Ctrl.wrd) ;
    rx_rst.bf.reset = bit_rst ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.recvOverhead_Ctrl.wrd,
                 rx_rst.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   FRAMER SONET SOFT(LOGIC) RESET              */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_framer_ctl_sonet_sreset(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/*                                                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Sonet sub-block logic reset of the framer     */
/* for the specified datapath direction:                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_uint8              bit_rst ;
  cs_boolean            rx_dir , tx_dir ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  bit_rst = (ctl == CS_DISABLE) ? 0 : 1 ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ( rx_dir ) {
    ARN5_FR_SONET32RX_Configuration_t rx_rst ;
    rx_rst.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd) ;
    rx_rst.bf.reset = bit_rst ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.sonet32Rx_Configuration.wrd,
                 rx_rst.wrd) ;
  }

  if ( tx_dir ) {
    ARN5_FR_TRANSMIT_Configuration_t tx_rst ;
    tx_rst.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.transmit_Configuration.wrd) ;
    tx_rst.bf.Reset = bit_rst ;
    CS_REG_WRITE(&pDev->slice[sl_num].Framer.transmit_Configuration.wrd,
                 tx_rst.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   FRAMER MAPPER STREAM SOFT(LOGIC) RESET      */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_ctl_mapper_stream_sreset(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/*                                                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Mapper's stream logic reset of framer.        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_uint8              bit_rst ;
  cs_boolean            rx_dir , tx_dir ;
  volatile cs_reg       * regaddr ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;

  bit_rst = (ctl == CS_DISABLE) ? 0 : 1 ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ( tx_dir ) {
    ARN5_FR_STREAMTX_PktControl_t    tx_rst ;
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamTx_PktControl.wrd ;

    tx_rst.wrd = CS_REG_READ(regaddr) ;
    tx_rst.bf.Reset = bit_rst ;
    CS_REG_WRITE(regaddr, tx_rst.wrd) ;
  }

  if ( rx_dir ) {
    ARN5_FR_STREAMRX_PktControl_t     rx_rst ;
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd ;
    rx_rst.wrd = CS_REG_READ(regaddr) ;
    rx_rst.bf.reset = bit_rst ;
    CS_REG_WRITE(regaddr, rx_rst.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   FRAMER DATACOM STREAM SOFT(LOGIC) RESET     */
/* CATEGORY   : API                                             */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_status arn5_framer_ctl_dcom_stream_sreset(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_RX or CS_TX or CS_TX_AND_RX                */
/*              o CS_DISABLE(0) or CS_ENABLE(1)                 */
/*                                                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Disable/Enable Mapper's stream logic reset of the framer.    */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_uint8              bit_rst ;
  cs_boolean            rx_dir , tx_dir ;
  volatile cs_reg       * regaddr ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_FR_PARSE_DIR_CTL(rx_dir, tx_dir, dir) ;
  bit_rst = (ctl == CS_DISABLE) ? 0 : 1 ;
  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  if ( tx_dir ) {
    ARN5_FR_DATACOMTX_StrConfig_t    tx_rst ;
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.datacomTx_StrConfig.wrd ;
    tx_rst.wrd = CS_REG_READ(regaddr) ;
    tx_rst.bf.reset = bit_rst ;
    CS_REG_WRITE(regaddr, tx_rst.wrd) ;
  }

  if ( rx_dir ) {
    ARN5_FR_DATACOMRX_StrConfig_t     rx_rst ;
    regaddr =
        (cs_reg *) &pDev->slice[sl_num].Framer.datacomRx_StrConfig.wrd ;
    rx_rst.wrd = CS_REG_READ(regaddr) ;
    rx_rst.bf.reset = bit_rst ;
    CS_REG_WRITE(regaddr, rx_rst.wrd) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   GET FRAMERS RX DATA PATH STATUS             */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_boolean arn5_framer_get_rx_data_path_status(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : TRUE (path ready) or FALSE (path not ready)     */
/* DESCRIPTION:                                                 */
/* Get the Framer's rx data path status.                        */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;

  ARN5_FR_SONET32RX_SectionStatus_t     status ;
  ARN5_FR_SONET32RX_PathStatus_t        pathStatus ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, FALSE) ;
  status.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.SectionStatus.wrd) ;

  if (!status.bf.InFrameS) return FALSE ;
  if (status.bf.LOFS) return FALSE ;

  pathStatus.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.PathStatus.wrd) ;
  if (pathStatus.bf.LOPS) return FALSE ;
  return (TRUE) ;
}


/****************************************************************/
/* $rtn_hdr_start   GET FRAMERS SYNC STATUS                     */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_boolean arn5_framer_get_sync_status(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : TRUE (In Sync) or FALSE (out of Sync)           */
/* DESCRIPTION:                                                 */
/* Get the Framer's sync status.                                */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  cs_boolean            sync_status ;
  ARN5_FR_SONET32RX_SectionStatus_t status ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, FALSE) ;
  status.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.SectionStatus.wrd) ;

  sync_status = (status.bf.InFrameS ? TRUE : FALSE ) ;
  return (sync_status) ;
}

/****************************************************************/
/* $rtn_hdr_start   GET SERVICE TYPE CONFIGURED FOR PATH        */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
arn5_port_prot_t arn5_framer_get_svc_type(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Return Service type configuration of the SONET Path.         */
/* Note : This API tries to map the current configuration to    */
/* one of the supported service (protocol) type ; i.e. it does  */
/* not return the type directly from FR_SONET32RX_PathProvision */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint16     sl_num ;
  ARN5_FR_SONET32RX_PathProvision_t     rx_prvsn ;
  ARN5_FR_STREAMRX_PktControl_t         rx_pktctl ;
  ARN5_FR_DATACOMRX_StrConfig_t         rx_config ;
  ARN5_FR_STREAMRX_PktX86Ctrl_t         x86Ctl;
  arn5_port_prot_t                      svc_type = ARN5_PORT_PROT_POS ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, ARN5_PORT_PROT_CLEAR_CH) ;

  rx_config.wrd = CS_REG_READ(
        &pDev->slice[sl_num].Framer.datacomRx_StrConfig.wrd) ;

  if (rx_config.bf.dataType == ARN5_dataType_CLEAR) {
      svc_type = ARN5_PORT_PROT_CLEAR_CH ;
  }
  else {
    rx_prvsn.wrd = CS_REG_READ(
          &pDev->slice[sl_num].Framer.PathProvision.wrd) ;

    switch (rx_prvsn.bf.SVC) {
      case ARN5_SVC_SRVC_ATM :
        svc_type = ARN5_PORT_PROT_ATM ;
      break ;

      case ARN5_SVC_SRVC_PPP :
        rx_pktctl.wrd = CS_REG_READ(
                &pDev->slice[sl_num].Framer.streamRx_PktControl.wrd) ;
        if (rx_pktctl.bf.DropAddrCtrl == 0) {
          svc_type = ARN5_PORT_PROT_POS ;
        }
        else {
          x86Ctl.wrd = CS_REG_READ(
                &pDev->slice[sl_num].Framer.PktX86Ctrl.wrd) ;
          svc_type = (x86Ctl.bf.X86En == 1) ?
                ARN5_PORT_PROT_LAPS : ARN5_PORT_PROT_PPP ;
        }
      break ;

      case ARN5_SVC_SRVC_GFP :
        svc_type = ARN5_PORT_PROT_GFP ;
      break ;
    }
  }

  return svc_type ;
}


/****************************************************************/
/* $rtn_hdr_start   SELECT PAYLOAD SCARMBLER                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : Framer                                          */
/* CHIP       : Arsenal5                                        */
cs_boolean arn5_framer_sel_payload_scrambler(cs_uint16 port_id,
                        arn5_framer_data_scrambler_sel_t sel)
/* INPUTS     : o Port Id                                       */
/*              o (De)Scrambler Selection                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Select one of the scrambler and descrambler to use in SONET  */
/* payload.  This selection only applies to non-ATM payload.    */
/*                                                              */
/* [sel] : ARN5_FR_SCRM_X43 (x^43 + 1) or                       */
/*         ARN5_FR_SCRM_X47 (x^47 + x^42 + 1)                   */
/* $rtn_hdr_end.                                                */
/****************************************************************/
{
  cs_uint16             sl_num ;
  ARN5_t                * pDev ;
  ARN5_FR_STREAMTX_MinPkt_t      txMinPkt ;
  ARN5_FR_STREAMRX_MinLength_t   rxMinLen ;
  ARN5_FR_STREAMRX_PktX86Ctrl_t  x86Ctl ;

  ARN5_FR_GET_DEVICE_ADDR(port_id, pDev, sl_num, CS_ERROR) ;
  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER) ;

  txMinPkt.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.MinPkt.wrd) ;
  txMinPkt.bf.Tx_Alt_Scramble = (sel == ARN5_FR_SCRM_X47) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.MinPkt.wrd, txMinPkt.wrd) ;

  rxMinLen.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.MinLength.wrd) ;
  rxMinLen.bf.Rx_Alt_Scramble = (sel == ARN5_FR_SCRM_X47) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.MinLength.wrd, rxMinLen.wrd) ;

  x86Ctl.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.PktX86Ctrl.wrd) ;
  x86Ctl.bf.X86Sel43 = (sel == ARN5_FR_SCRM_X47) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].Framer.PktX86Ctrl.wrd, x86Ctl.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER) ;
  return (CS_OK) ;
}



