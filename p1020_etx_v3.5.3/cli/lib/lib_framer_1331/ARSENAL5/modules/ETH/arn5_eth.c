/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2004 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_eth.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 * API's for ETH(TBI) interface
 */

#include "cs_types.h"
#include "cs_rtos.h"
#include "cs_utils.h"

#include "arn5_error.h"
#include "arn5_print.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_gen_data.h"

#include "arn5_eth_data.h"
#include "arn5_eth_api.h"
#include "arn5_eth_priv.h"
#include "arn5_eth_stats.h"


#define ARN5_ETH_HRESET_MSEC_DELAY  (20)

/* the following macro assumes two variables */
/* pDev and sl_num are defined              */
#define ARN5_ETH_COMMON_VALIDATE(PORT_ID, BASE_ADDR, SLICE, RETCODE) \
{                                                               \
  ARN5_INVALID_PORT_EXIT(PORT_ID, RETCODE) ;                    \
  if (ARN5_IS_ETHERNET_MODE((PORT_ID)) == FALSE) {              \
    CS_HNDL_ERROR((PORT_ID), EARN5_INVALID_IN_FRAMER_MODE,      \
                  "\nGigE is powered DOWN in Framer Mode\n") ;  \
    return (RETCODE) ;                                          \
  }                                                             \
  SLICE = ARN5_PORT_ID_TO_SLICE_NUM(PORT_ID) ;                  \
  BASE_ADDR = ARN5_PORT_ID_TO_DEV_BASE(PORT_ID) ;               \
}



/* Forward declarations */
static cs_status arn5_ethnet_mode_cfg(cs_uint16 port_id,
                        cs_boolean promiscuous, cs_boolean tx_addr_insert,
                        cs_boolean len_check, cs_boolean padding_removal,
                        cs_boolean rx_crc_fwd, cs_boolean tx_crc_fwd) ;



/****************************************************************
 *******************   ETH PORT-LEVEL API's    ******************
 ****************************************************************/

/****************************************************************/
/* _hdr_start  ETH PORT INITIALIZATION                          */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status arn5_eth_init_cfg(cs_uint16 port_id,
                       arn5_port_cfg_t * pPortCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to device configuration struct        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* This initializes the port level of the ETH block             */
/* with the given summary.                                      */
/*                                                              */
/* The [pCfg] parameter is a pointer to the configuration       */
/* data-structure, which has all the configuration parameters   */
/* set.                                                         */
/* The API expects the data-structure to be already allocated   */
/* and populated with the configuration settings.               */
/* _hdr_end                                                     */
/****************************************************************/
{
  ARN5_GIGE_EI_LINE_MISCCTRL2_t miscCtl2 ;
  cs_uint8                      sl_num ;
  ARN5_t                        * pDev ;
  arn5_eth_cfg_t                * pCfg  = &pPortCfg->eth ;

  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;

  /* set the bit order */
  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;

  miscCtl2.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.MISCCTRL2.wrd) ;
  miscCtl2.bf.RX_BSWAP = CS_IN_SIM_ENV() ? 1 : 0 ;
  miscCtl2.bf.TX_BSWAP = CS_IN_SIM_ENV() ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.MISCCTRL2.wrd, miscCtl2.wrd) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  if (arn5_eth_set_data_line_params(port_id,
        pCfg->rx_data_inv, pCfg->tx_data_inv)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_eth_set_max_frame_len(port_id, pCfg->max_len)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_eth_set_rpr_mode(port_id, pCfg->rpr_mode)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_eth_set_mac_address(port_id, pCfg->ethnet_mode_cfg.mac_addr)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_eth_ethnet_mode_cfg(port_id,
        pCfg->ethnet_mode_cfg.promiscuous,
        pCfg->ethnet_mode_cfg.tx_addr_insert,
        pCfg->ethnet_mode_cfg.len_check,
        pCfg->ethnet_mode_cfg.padding_removal,
        pCfg->ethnet_mode_cfg.rx_crc_fwd,
        pCfg->ethnet_mode_cfg.tx_crc_fwd)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_eth_set_pause_quanta(port_id, pCfg->pause_quanta)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_eth_mac_ctl_frame_cfg(port_id,
        pCfg->ctl_frame_cfg.pause_ignore,
        pCfg->ctl_frame_cfg.pause_frame_forward,
        pCfg->ctl_frame_cfg.user_mac_frame_forward)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_eth_set_ability(port_id,
        pCfg->pause_ability, ARN5_FS_LINK_OK)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_eth_auto_negotiation_cfg(port_id,
        pCfg->auto_neg_cfg.enable,
        pCfg->auto_neg_cfg.link_timer)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_eth_ctl_loopback(port_id, CS_RX, (pCfg->rx2tx_lb == 1))
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_eth_ctl_loopback(port_id, CS_TX, (pCfg->tx2rx_lb == 1))
      != CS_OK) {
    return (CS_ERROR) ;
  }

  if (arn5_eth_sel_status_to_gpio(port_id, pCfg->gpio_sel)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  /*
   * Done with configuring the registers. SRESET(logic reset) the
   * block.
   */
  if (arn5_eth_soft_reset(port_id, ARN5_ETH_SUB_BLOCK_ALL)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  /* soft reset will disable the data path, so let's enable it the last */
  if (arn5_eth_ctl_data_path(port_id, CS_TX_AND_RX, CS_ENABLE)
      != CS_OK) {
    return (CS_ERROR) ;
  }

  /* init port_level statistics */
  if (arn5_eth_init_port_stats(port_id)
      != CS_OK) {
    return (CS_ERROR);
  }

  /* Init default IRQ */
  if (arn5_eth_init_irq_cfg(port_id, pCfg)
      != CS_OK ) {
    return (CS_ERROR) ;
  }

  CS_PRINT("\n\t *** ETH Block Init done! ***\n");

  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start  GET DEFAULT ETH PORT CONFIGURATION              */
/* CATEGORY   : Port                                            */
/* ACCESS     : private                                         */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status arn5_eth_get_default_cfg(cs_uint16 port_id,
                      arn5_port_cfg_t * pPortCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to configuration structure            */
/* OUTPUTS    : o ETH port related default config               */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get the port's default ETH configuration for both host       */
/* and line based on specified summary config info.             */
/*                                                              */
/* The [pPortCfg] parameter is a pointer to the configuration   */
/* data-structure. The API will fill in this structure with     */
/* default settings based on the config summary info.           */
/*                                                              */
/* The API expects the config summary filled.                   */
/* $_hdr_end                                                    */
/****************************************************************/
{
  cs_uint8              sl_num ;
  arn5_eth_cfg_t        * pCfg = &pPortCfg->eth ;

  pCfg->rpr_mode = FALSE ;
  pCfg->ethnet_mode_cfg.promiscuous = TRUE ;
  pCfg->ethnet_mode_cfg.mac_addr.byte5 = 0;
  pCfg->ethnet_mode_cfg.mac_addr.byte4 = 0;
  pCfg->ethnet_mode_cfg.mac_addr.byte3 = 0;
  pCfg->ethnet_mode_cfg.mac_addr.byte2 = 0;
  pCfg->ethnet_mode_cfg.mac_addr.byte1 = 0;
  pCfg->ethnet_mode_cfg.mac_addr.byte0 = 0;

  pCfg->ethnet_mode_cfg.tx_addr_insert = FALSE ;
  pCfg->ethnet_mode_cfg.len_check = FALSE ;
  pCfg->ethnet_mode_cfg.padding_removal = FALSE ;
  pCfg->ethnet_mode_cfg.rx_crc_fwd = TRUE ;
  pCfg->ethnet_mode_cfg.tx_crc_fwd = TRUE ;

  pCfg->ctl_frame_cfg.pause_ignore = FALSE ;
  pCfg->ctl_frame_cfg.pause_frame_forward = FALSE ;
  pCfg->ctl_frame_cfg.user_mac_frame_forward = TRUE ;

  pCfg->max_len = 1522 ;
  pCfg->pause_quanta = 0 ;
  pCfg->pause_ability = ARN5_NO_PAUSE ;

  pCfg->auto_neg_cfg.enable = TRUE ;
  pCfg->auto_neg_cfg.link_timer = 10000 ;

  pCfg->rx_data_inv = FALSE ;
  pCfg->tx_data_inv = FALSE ;

  pCfg->rx2tx_lb = FALSE ;
  pCfg->tx2rx_lb = FALSE ;

  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  pCfg->gpio_sel = ARN5_ETH_STATUS_LINK_STAT ;

  /* Get the Default IRQ Configuration */
  arn5_eth_get_default_irq_cfg(port_id, &pPortCfg->summ, pCfg) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $_hdr_start  GET ETH RUNNING CONFIGURATION                   */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_get_running_cfg(cs_uint16 port_id,
                       arn5_port_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to ETH running config structure       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Returns the current run-time ETH hardware configuration      */
/* of the specified port.                                       */
/* The [pRunCfg] parameter is a pointer to the port's complete  */
/* configuration data-structure.                                */
/*                                                              */
/* This API will fill in the all the fields in the block        */
/* run-time configuration datastructure.                        */
/* $_hdr_end                                                    */
/****************************************************************/
{
  cs_uint8      sl_num ;
  ARN5_t        * pDev ;
  arn5_eth_runcfg_t     * pCfg  = &pRunCfg->eth ;

  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;

  /* need to do lock here to avoid protocol violation on reading */
  /* MAC addresses                                               */
  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;

  /* rpr_mode, crc_forward */
  {
    ARN5_GIGE_MISC_CONFIG_t      cfg ;

    cfg.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.CONFIG.wrd) ;
    pCfg->rpr_mode = (cfg.bf.RPR_MODE == 1) ;
    pCfg->ethnet_mode_cfg.rx_crc_fwd = (cfg.bf.RX_CRC_FWD == 1) ;
    pCfg->ethnet_mode_cfg.tx_crc_fwd = (cfg.bf.TX_CRC_FWD == 1) ;
  }


  /* max_len */
  {
    /* read MSW first then LSW */
    CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.FRM_LENGTH1.wrd) ;
    pCfg->max_len = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.FRM_LENGTH0.wrd) ;
  }

  /* mac_addr */
  {
    cs_uint16 val;

    /* read MSW first then LSW */
    val = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.MAC_ADDR_U1.wrd) ;
    val = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.MAC_ADDR_U0.wrd) ;
    pCfg->ethnet_mode_cfg.mac_addr.byte0 = val >> 8 ;
    pCfg->ethnet_mode_cfg.mac_addr.byte1 = val & 0x00ff ;
    val = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.MAC_ADDR_L1.wrd) ;
    pCfg->ethnet_mode_cfg.mac_addr.byte2 = val >> 8 ;
    pCfg->ethnet_mode_cfg.mac_addr.byte3 = val & 0x00ff ;
    val = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.MAC_ADDR_L0.wrd) ;
    pCfg->ethnet_mode_cfg.mac_addr.byte4 = val >> 8 ;
    pCfg->ethnet_mode_cfg.mac_addr.byte5 = val & 0x00ff ;
  }

  /* pause_quanta */
  {
    /* read MSW first then LSW */
    CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.PAUSE_QUANT1.wrd) ;
    pCfg->pause_quanta = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.PAUSE_QUANT0.wrd) ;
  }

  /* rx_path_enbl, tx_path_enbl, ctl_frame_cfg, ethenet_mode_cfg */
  {
    ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG0_t    cfg0 ;
    ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG1_t    cfg1 ;

    /* must read MSW then LSW */
    cfg1.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG1.wrd) ;
    cfg0.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG0.wrd) ;

    pCfg->rx_path_enbl = (cfg0.bf.RX_ENA == 1) ;
    pCfg->tx_path_enbl = (cfg0.bf.TX_ENA == 1) ;
    pCfg->ctl_frame_cfg.pause_ignore = (cfg0.bf.PAUSE_IGNORE == 1) ;
    pCfg->ctl_frame_cfg.pause_frame_forward = (cfg0.bf.PAUSE_FWD == 1) ;
    pCfg->ctl_frame_cfg.user_mac_frame_forward = (cfg1.bf.CNTL_FRM_ENA == 1) ;
    pCfg->ethnet_mode_cfg.promiscuous = (cfg0.bf.PROMIS_EN == 1) ;
    pCfg->ethnet_mode_cfg.tx_addr_insert = (cfg0.bf.TX_ADDR_INS == 1) ;
    pCfg->ethnet_mode_cfg.padding_removal = (cfg0.bf.PAD_EN == 1) ;
    pCfg->ethnet_mode_cfg.len_check = (cfg1.bf.NO_LGTH_CHECK == 0) ;
  }

  /* loopbacks, data_inv, bit swap */
  {
    ARN5_GIGE_EI_LINE_MISCCTRL2_t     miscCtl2 ;

    miscCtl2.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.MISCCTRL2.wrd) ;
    pCfg->rx2tx_lb = (miscCtl2.bf.RX2TX_LB == 1) ;
    pCfg->tx2rx_lb = (miscCtl2.bf.TX2RX_LB == 1) ;
    pCfg->rx_data_inv = (miscCtl2.bf.RX_DATA_INV == 1) ;
    pCfg->tx_data_inv = (miscCtl2.bf.TX_DATA_INV == 1) ;
    pCfg->rx_bswap = (miscCtl2.bf.RX_BSWAP == 1) ;
    pCfg->tx_bswap = (miscCtl2.bf.TX_BSWAP == 1) ;
  }

  /* pause_ability, fault_state */
  {
    ARN5_GIGE_EI_ECORE_PCS_DEV_ABILITY_t ability ;

    ability.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.DEV_ABILITY.wrd) ;
    pCfg->pause_ability = (arn5_eth_pause_ability_t) ability.bf.PS ;
    pCfg->fault_state = (arn5_eth_fault_state_t) ability.bf.RmtFault ;
  }

  /* auto_neg_cfg */
  {
    cs_uint16 val;
    ARN5_GIGE_EI_ECORE_PCS_CONTROL_t pcsCtl ;

    pcsCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.CONTROL.wrd) ;
    pCfg->auto_neg_cfg.enable = (pcsCtl.bf.AutoNegEn == 1) ;
    pCfg->auto_neg_cfg.link_timer =
      CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.LINK_TIMER_1.wrd) ;
    val = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.LINK_TIMER_0.wrd) ;
    pCfg->auto_neg_cfg.link_timer = (pCfg->auto_neg_cfg.link_timer << 16) | val ;

    /* convert from 8ns step to usec */
    pCfg->auto_neg_cfg.link_timer /= 125 ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;
  return (CS_OK);
}


/****************************************************************/
/* $_hdr_start  DUMP ETH RUNNING CONFIGURATION                  */
/* CATEGORY   : Device                                          */
/* ACCESS     : public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_dump_running_cfg(cs_uint16 port_id,
                      arn5_port_runcfg_t * pRunCfg)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to ETH config structure               */
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
  cs_int8               * sTrue = "True" ;
  cs_int8               * sFalse = "False" ;
  cs_int8               * sEnable = "Enabled" ;
  cs_int8               * sDisable = "Disabled" ;
  cs_int8               * sStr ;
  cs_uint16             macU ;
  cs_uint32             macL ;
  arn5_eth_runcfg_t     * pCfg  = &pRunCfg->eth ;

  CS_PRINT("RPR Mode               = %s\n",
    (pCfg->rpr_mode ? sTrue : sFalse)) ;
  CS_PRINT("Max Length             = %d\n", pCfg->max_len) ;

  CS_PRINT("\n\t *** Ethernet Mode Params ***\n");
  CS_PRINT("Promiscuous            = %s\n",
    (pCfg->ethnet_mode_cfg.promiscuous ? sTrue : sFalse)) ;

  macU = (((cs_uint16)(pCfg->ethnet_mode_cfg.mac_addr.byte5)) << 8) |
                ((cs_uint16)pCfg->ethnet_mode_cfg.mac_addr.byte4) ;
  macL = (((cs_uint32)(pCfg->ethnet_mode_cfg.mac_addr.byte3)) << 24) |
         (((cs_uint32)(pCfg->ethnet_mode_cfg.mac_addr.byte2)) << 16) |
         (((cs_uint32)(pCfg->ethnet_mode_cfg.mac_addr.byte1)) << 8 ) |
         (((cs_uint32)(pCfg->ethnet_mode_cfg.mac_addr.byte0))) ;

  CS_PRINT("MAC Address            = 0x%04x%08x\n",
    macU, macL) ;

  CS_PRINT("Tx Address Insert      = %s\n",
    (pCfg->ethnet_mode_cfg.tx_addr_insert ? sTrue : sFalse)) ;
  CS_PRINT("Length Check           = %s\n",
    (pCfg->ethnet_mode_cfg.len_check ? sTrue : sFalse)) ;
  CS_PRINT("Padding Removal        = %s\n",
    (pCfg->ethnet_mode_cfg.padding_removal ? sTrue : sFalse)) ;
  CS_PRINT("Rx CRC Forward         = %s\n",
    (pCfg->ethnet_mode_cfg.rx_crc_fwd ? sTrue : sFalse)) ;
  CS_PRINT("Tx CRC Forward         = %s\n",
    (pCfg->ethnet_mode_cfg.tx_crc_fwd ? sTrue : sFalse)) ;

  CS_PRINT("\n\t *** Control Frame Params ***\n");
  CS_PRINT("Pause Quanta           = %d\n", pCfg->pause_quanta) ;
  CS_PRINT("Ignore Pause Frame     = %s\n",
    (pCfg->ctl_frame_cfg.pause_ignore ? sTrue : sFalse)) ;
  CS_PRINT("Pause Frame Forward    = %s\n",
    (pCfg->ctl_frame_cfg.pause_frame_forward ? sTrue : sFalse)) ;
  CS_PRINT("User Mac Frame Forward = %s\n",
    (pCfg->ctl_frame_cfg.user_mac_frame_forward ? sTrue : sFalse)) ;

  CS_PRINT("\n\t *** My Ability Params ***\n");
  if (pCfg->pause_ability == ARN5_NO_PAUSE) {
    sStr = "No Pause" ;
  }
  else if (pCfg->pause_ability == ARN5_SYMMETRIC_PAUSE) {
    sStr = "Symmetric Pause" ;
  }
  else {
    sStr = "Asymmetric Pause" ;
  }
  CS_PRINT("Pause Ability          = %s\n", sStr) ;

  switch (pCfg->fault_state) {
    case ARN5_FS_LINK_FAIL :
      sStr = "Link Fail" ;
      break ;

    case ARN5_FS_OFFLINE :
      sStr = "Offline" ;
      break ;

    case ARN5_FS_AUTO_NEGOTIATION_ERR :
      sStr = "Auto Negotiation Error" ;
      break ;

    default :
      sStr = "Link OK" ;
  }
  CS_PRINT("Fault State            = %s\n", sStr) ;

  CS_PRINT("\n\t *** Auto Negotiation Params ***\n");
  CS_PRINT("Auto Negotiation       = %s\n",
    (pCfg->auto_neg_cfg.enable ? sEnable : sDisable)) ;
  CS_PRINT("Link Timer (usec)      = %d\n",
    pCfg->auto_neg_cfg.link_timer) ;

  CS_PRINT("\n\t *** Data Line Params ***\n");
  CS_PRINT("Rx Data Line           = %s",
    (pCfg->rx_data_inv ? "Inverted" : "Non-Inverted"));
  if (!pCfg->rx_bswap) {
    CS_PRINT(", Bits Not Swapped") ;
  }
  if (!pCfg->rx_path_enbl) {
    CS_PRINT(", %s", sDisable) ;
  }
  CS_PRINT("\n") ;

  CS_PRINT("Tx Data Line           = %s",
    (pCfg->tx_data_inv ? "Inverted" : "Non-Inverted"));
  if (!pCfg->tx_bswap) {
    CS_PRINT(", Bits Not Swapped") ;
  }
  if (!pCfg->tx_path_enbl) {
    CS_PRINT(", %s", sDisable) ;
  }
  CS_PRINT("\n") ;

  /* Loopbacks */
  CS_PRINT("\n\t *** Loopbacks ***\n") ;
  CS_PRINT("Tx-to-Rx Loopback      = %s\n",
           (pCfg->tx2rx_lb) ? "On" : "Off") ;
  CS_PRINT("Rx-to-Tx Loopback      = %s\n",
           (pCfg->rx2tx_lb) ? "On" : "Off") ;

  return (CS_OK);
}


/****************************************************************/
/* $rtn_hdr_start  ETH/TBI SOFT RESET                           */
/* CATEGORY   : Port                                            */
/* ACCESS     : public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_soft_reset(cs_uint16 port_id,
                        arn5_eth_sub_block_t sub_block_id)
/* INPUTS     : o Port Id                                       */
/*              o Sub-block ID                                  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Soft reset of a certain sub-block of an ETH slice.           */
/* Most of configuration registers except MAC enable bits TX_ENA*/
/* and RX_ENA are unaffected                                    */
/*                                                              */
/* [sub_block_id] specifies which sub-block to be reset.        */
/*    ARN5_ETH_SUB_BLOCK_PCS                                    */
/*    ARN5_ETH_SUB_BLOCK_MAC                                    */
/*    ARN5_ETH_SUB_BLOCK_LINE                                   */
/*    ARN5_ETH_SUB_BLOCK_ALL                                    */
/*                                                              */
/* The reset bit is SELF CLEARING. This API will block 5 ms     */
/* before returning.                                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t                * pDev ;
  volatile cs_reg       * pReg ;
  cs_uint16             reset ;
  cs_uint32             usec_delay = 5000 ;
  cs_uint8              sl_num ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;

  if ((sub_block_id == ARN5_ETH_SUB_BLOCK_ALL) ||
      (sub_block_id == ARN5_ETH_SUB_BLOCK_PCS))
  {
    reset = 0x8000 ;
    pReg = (cs_reg *)&pDev->slice[sl_num].GigE.EI.ecore.CONTROL ;
    if (CS_IN_TEST_ENV()) {
      reset = 0 ;
    }
    CS_REG_WRITE(pReg, CS_REG_READ(pReg) | reset) ;

    reset = 0x0030 ;
    pReg = (cs_reg *)&pDev->slice[sl_num].GigE.EI.ecore.Reset ;
    if (CS_IN_TEST_ENV()) {
      reset = 0 ;
    }
    CS_REG_WRITE(pReg, CS_REG_READ(pReg) | reset) ;
  }

  if ((sub_block_id == ARN5_ETH_SUB_BLOCK_ALL) ||
      (sub_block_id == ARN5_ETH_SUB_BLOCK_MAC)) {
    ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG0_t    cfg0 ;
    ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG1_t    cfg1 ;

    cfg1.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG1.wrd) ;
    cfg0.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG0.wrd) ;

    reset = 0x2000 ;
    if (CS_IN_TEST_ENV()) {
      reset = 0 ;
    }
    cfg0.wrd |= reset ;
    CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG0.wrd, cfg0.wrd) ;
    CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG1.wrd, cfg1.wrd) ;

    reset = 0x000f ;
    pReg = (cs_reg *)&pDev->slice[sl_num].GigE.EI.ecore.Reset ;
    if (CS_IN_TEST_ENV()) {
      reset = 0 ;
    }
    CS_REG_WRITE(pReg, CS_REG_READ(pReg) | reset) ;
  }

  if ((sub_block_id == ARN5_ETH_SUB_BLOCK_ALL) ||
      (sub_block_id == ARN5_ETH_SUB_BLOCK_LINE)) {
    reset = 0x0003 ;
    pReg = (cs_reg *)&pDev->slice[sl_num].GigE.EI.RESET ;
    if (CS_IN_TEST_ENV()) {
      reset = 0 ;
    }
    CS_REG_WRITE(pReg, CS_REG_READ(pReg) | reset) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;
  CS_UDELAY(usec_delay) ;
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ETH LOOPBACK                                */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_ctl_loopback(cs_uint16 port_id,
                        cs_dir_t dir, cs_boolean enbl)
/* INPUTS     : o Port Id                                       */
/*              o Loopback selection                            */
/*              o TRUE(enable loopback) or FALSE(disable)       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Control(disable/enable) the specified loopback in ETH        */
/* interface.                                                   */
/*                                                              */
/* The [dir] parameter specifies which loopback to control and  */
/* should be one of the following:                              */
/* CS_TX: When enabled, TX outputs are looped back to RX        */
/* CS_RX: When enabled, RX outputs are looped back to TX        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_EI_LINE_MISCCTRL2_t  miscCtl2 ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  miscCtl2.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.MISCCTRL2.wrd) ;

  if (dir == CS_TX) {
    if (enbl) {
      miscCtl2.bf.RX2TX_LB = 0 ;
      miscCtl2.bf.TX2RX_LB = 1 ;
      miscCtl2.bf.TX_CLK_LB_EN = 1 ;
    }
    else {
      miscCtl2.bf.TX2RX_LB = 0 ;
      miscCtl2.bf.TX_CLK_LB_EN = 0 ;
    }
  }
  else {
    if (enbl) {
      miscCtl2.bf.TX2RX_LB = 0 ;
      miscCtl2.bf.TX_CLK_LB_EN = 0 ;
      miscCtl2.bf.RX2TX_LB = 1 ;
    }
    else {
      miscCtl2.bf.RX2TX_LB = 0 ;
    }
  }
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.MISCCTRL2.wrd, miscCtl2.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   ETH DATA PATH CONTROL                       */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_ctl_data_path(cs_uint16 port_id,
                        cs_dir_t dir, cs_ctl_t enbl)
/* INPUTS     : o Port Id                                       */
/*              o data path selection                           */
/*              o CS_DISABLE or CS_ENABLE                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Control(disable/enable) the specified data path              */
/*                                                              */
/* [dir] parameter should be one of the following:              */
/* CS_TX, CS_RX or CS_TX_AND_RX                                 */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG0_t   cfg0 ;
  ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG1_t   cfg1 ;
  ARN5_GIGE_EI_LINE_MISCCTRL2_t              miscCtl2 ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  /* read both (otherwise protocol violation) despite use only one of the two */
  cfg1.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG1.wrd) ;
  cfg0.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG0.wrd) ;

  miscCtl2.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.MISCCTRL2.wrd) ;

  if ((dir == CS_TX) || (dir == CS_TX_AND_RX)) {
    cfg0.bf.TX_ENA = (enbl) ? 1 : 0 ;
    miscCtl2.bf.TX_CLK_EN = cfg0.bf.TX_ENA ;
  }

  if ((dir == CS_RX) || (dir == CS_TX_AND_RX)) {
    cfg0.bf.RX_ENA = (enbl) ? 1 : 0 ;
    miscCtl2.bf.RX_CLK_EN = cfg0.bf.RX_ENA ;
  }

  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.MISCCTRL2.wrd, miscCtl2.wrd) ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG0.wrd, cfg0.wrd) ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG1.wrd, cfg1.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET ETH DATA LINE PARAMS                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_set_data_line_params(cs_uint16 port_id,
                        cs_boolean rx_data_inv, cs_boolean tx_data_inv)
/* INPUTS     : o Port Id                                       */
/*              o Rx Data Invert (FALSE op TRUE)                */
/*              o Tx Data Invert (FALSE op TRUE)                */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Allow to set the polarity of rx and tx data lines            */
/*                                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_EI_LINE_MISCCTRL2_t  miscCtl2 ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  miscCtl2.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.MISCCTRL2.wrd) ;

  miscCtl2.bf.RX_DATA_INV = rx_data_inv ? 1 : 0 ;
  miscCtl2.bf.TX_DATA_INV = tx_data_inv ? 1 : 0 ;

  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.MISCCTRL2.wrd, miscCtl2.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   CONFIG MAC CONTROL FRAMES HANDLING          */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_mac_ctl_frame_cfg(cs_uint16 port_id,
                        cs_boolean pause_ignore,
                        cs_boolean pause_frame_forward,
                        cs_boolean user_mac_frame_forward)
/* INPUTS     : o Port Id                                       */
/*              o FALSE (process) or TRUE (ignore)              */
/*              o FALSE (terminate) or TRUE (forward)           */
/*              o FALSE or TRUE                                 */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configure various controls over mac control frames           */
/* MAC control frame with Opcode == 0x0001 is pause frame, which*/
/* can be terminated, processed or forwarded.                   */
/* Others (Opcode != 0x0001) are considered as user defined     */
/* mac control frame and can only be forwarded or discarded     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG0_t   cfg0 ;
  ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG1_t   cfg1 ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  cfg1.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG1.wrd) ;
  cfg0.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG0.wrd) ;

  cfg0.bf.PAUSE_IGNORE = (pause_ignore) ? 1 : 0 ;
  cfg0.bf.PAUSE_FWD = (pause_frame_forward) ? 1 : 0 ;
  cfg1.bf.CNTL_FRM_ENA = (user_mac_frame_forward) ? 1 : 0 ;

  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG0.wrd, cfg0.wrd) ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG1.wrd, cfg1.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET ETH DATA LINE PARAMS                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status arn5_eth_set_max_frame_len(cs_uint16 port_id,
                        cs_uint16 max_len)
/* INPUTS     : o Port Id                                       */
/*              o Maxinum Frame Length [13:0]                   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Specify Max frame length, a 14 bit maxinum value for MAC     */
/* lengh check.                                                 */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_EI_ECORE_MAC_FRM_LENGTH0_t   len0 ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  len0.wrd = 0 ;
  len0.bf.FRM_LENGTH = max_len & 0x3fff ;
  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  /* write LSW then MSW */
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.FRM_LENGTH0.wrd, len0.wrd) ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.FRM_LENGTH1.wrd, 0) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET ETH MAC ADDRESS                         */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_set_mac_address(cs_uint16 port_id,
                        cs_mac mac_addr)
/* INPUTS     : o Port Id                                       */
/*              o MAC Address                                   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Specify MAC Address, a 48 bit unicast address can be used    */
/* for address filtering and/or tx source address overide in    */
/* Ethernet mode application.                                   */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  cs_uint16     val ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  /* write LSW then MSW */
  val = (((cs_uint16)mac_addr.byte4) << 8) | mac_addr.byte5 ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.MAC_ADDR_L0.wrd, val) ;
  val = (((cs_uint16)mac_addr.byte2) << 8) | mac_addr.byte3 ; ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.MAC_ADDR_L1.wrd, val) ;
  val = (((cs_uint16)mac_addr.byte0) << 8) | mac_addr.byte1 ; ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.MAC_ADDR_U0.wrd, val) ;
  val = 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.MAC_ADDR_U1.wrd, val) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET ETH MAC ADDRESS                         */
/* CATEGORY   : Port                                            */
/* ACCESS     : Private                                         */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_set_mac_address_bytes(cs_uint16 port_id,
                        cs_uint16 mac5_4, cs_uint16 mac3_2, cs_uint16 mac1_0)
/* INPUTS     : o Port Id                                       */
/*              o MAC address bytes(5:4)                        */
/*              o MAC address bytes(3:2)                        */
/*              o MAC address bytes(1:0)                        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Specify MAC Address, a 48 bit unicast address can be used    */
/* for address filtering and/or tx source address overide in    */
/* Ethernet mode application.                                   */
/*                                                              */
/* This is a private API - mainly provided for lab use.         */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  cs_uint16     macBuf ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  /* write LSW then MSW */
  /* multicast bit (bit 40) is at lower byte of L0 */
  macBuf  = ((mac5_4 & 0x00ff) << 8) | ((mac5_4 & 0xff00) >> 8) ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.MAC_ADDR_L0.wrd, macBuf) ;
  macBuf  = ((mac3_2 & 0x00ff) << 8) | ((mac3_2 & 0xff00) >> 8) ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.MAC_ADDR_L1.wrd, macBuf) ;
  macBuf  = ((mac1_0 & 0x00ff) << 8) | ((mac1_0 & 0xff00) >> 8) ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.MAC_ADDR_U0.wrd, macBuf) ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.MAC_ADDR_U1.wrd, 0) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET INTERFACE IN RPR or ETHERNET MODE       */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_set_rpr_mode(cs_uint16 port_id,
                        cs_boolean rpr)
/* INPUTS     : o Port Id                                       */
/*              o FALSE (Ethernet Mode) or TRUE (RPR Mode)      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the inferface to accept rpr or normal ethernet frame     */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_MISC_CONFIG_t  cfg ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  cfg.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.CONFIG.wrd) ;
  cfg.bf.RPR_MODE = (rpr) ? 1 : 0 ;

  CS_REG_WRITE(&pDev->slice[sl_num].GigE.CONFIG.wrd, cfg.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET CONFIGRATION SPECIFIC TO ETHERNET MODE  */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_ethnet_mode_cfg(cs_uint16 port_id,
                        cs_boolean promiscuous, cs_boolean tx_addr_insert,
                        cs_boolean len_check, cs_boolean padding_removal,
                        cs_boolean rx_crc_fwd, cs_boolean tx_crc_fwd)
/* INPUTS     : o Port Id                                       */
/*              o FALSE (MAC address filtering) or TRUE         */
/*              o FALSE or TRUE                                 */
/*              o FALSE (no length check) or TRUE               */
/*              o FALSE (Rx CRC termination) or TRUE            */
/*              o FALSE (No CRC gen) or TRUE (Tx CRC forwarding)*/
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configure various controls specific to ethernet mode.        */
/* [promiscuous] - Controls unicast address filtering or not    */
/* [tx_addr_insert] - Allows source address insertion on TX     */
/* [len_check] - Allows MAC checks the frame's payload length   */
/*               with the Frame Length/Type field on RX         */
/* [padding_removal] - Allows padding to be removed on RX       */
/* [rx_crc_forward] - Allows RX CRC field to be passed on       */
/* [tx_crc_forward] - Allows TX CRC field to be passed on       */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev = 0 ;
  cs_uint8      sl_num ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;
  return arn5_ethnet_mode_cfg(port_id, promiscuous, tx_addr_insert,
                        len_check, padding_removal, rx_crc_fwd,
                        tx_crc_fwd) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET MY ABILITY TO ADVERTIZE                 */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_set_ability(cs_uint16 port_id,
                        arn5_eth_pause_ability_t pause_ability,
                        arn5_eth_fault_state_t fault_state)
/* INPUTS     : o Port Id                                       */
/*              o What ability for pause frame support          */
/*              o Current link fault state                      */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set ability attributes to advertize during auto negotiation  */
/* Full Duplex only is always set by this API.                  */
/* [pause_ability] is one of the following                      */
/*    ARN5_NO_PAUSE                                             */
/*    ARN5_SYMMETRIC_PAUSE                                      */
/*    ARN5_ASYMMETRIC_PAUSE                                     */
/* [fault_state] is one of the following                        */
/*    ARN5_FS_LINK_OK                                           */
/*    ARN5_FS_LINK_FAIL                                         */
/*    ARN5_FS_OFFLINE                                           */
/*    ARN5_FS_AUTO_NEGOTIATION_ERR                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_EI_ECORE_PCS_DEV_ABILITY_t ability ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  if ((pause_ability > ARN5_ASYMMETRIC_PAUSE) ||
      (fault_state > ARN5_FS_AUTO_NEGOTIATION_ERR)) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG, NULL) ;
    return (CS_ERROR);
  }

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  ability.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.DEV_ABILITY.wrd) ;
  ability.bf.RmtFault = fault_state ;
  ability.bf.PS = pause_ability ;
  ability.bf.HalfDuplex = 0 ;
  ability.bf.FullDuplex = 1 ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.DEV_ABILITY.wrd, ability.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK);
}


/****************************************************************/
/* $rtn_hdr_start   GET PARTNER ABILITY                         */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_get_partner_ability(cs_uint16 port_id,
                        arn5_eth_ability_t * pAbility)
/* INPUTS     : o Port Id                                       */
/*              o Pointer to ability struct                     */
/* OUTPUTS    : Updated partner ability filled in struct        */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Get partner ability attributes                               */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_EI_ECORE_PCS_PARTNER_ABILITY_t ability ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  if (pAbility == NULL) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_NULL_PTR, NULL) ;
    return (CS_ERROR);
  }

  ability.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.PARTNER_ABILITY.wrd) ;
  pAbility->next_page     = (ability.bf.NextPage == 1) ;
  pAbility->ack           = (ability.bf.ACK == 1) ;
  pAbility->fault_state   = (arn5_eth_fault_state_t)ability.bf.RmtFault ;
  pAbility->pause_ability = (arn5_eth_pause_ability_t)ability.bf.PS ;
  pAbility->half_duplex   = (ability.bf.HalfDuplex == 1) ;
  pAbility->full_duplex   = (ability.bf.FullDuplex == 1) ;

  return (CS_OK);
}


/****************************************************************/
/* $rtn_hdr_start   GET ETHNET LINK STATUS                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_boolean  arn5_eth_get_link_status(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Link Status (TRUE - Valid link is established,  */
/*              or FALSE)                                       */
/* DESCRIPTION:                                                 */
/* Get link status                                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_EI_ECORE_PCS_STATUS_t       status ;
  ARN5_GIGE_EI_ECORE_TOP_INTSTATUS_t    intStatus ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, FALSE) ;

  /* read twice to clear latch old link failure */
  status.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.STATUS.wrd) ;
  status.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.STATUS.wrd) ;
  /* intstatus is the choice for the status */
  intStatus.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.INTSTATUS.wrd) ;

  return (intStatus.bf.PCS_LINKs == 1);
}


/****************************************************************/
/* $rtn_hdr_start   GET AUTO NEGOTIATION DONE STATUS            */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_boolean  arn5_eth_get_auto_negotiation_status(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : Auto Negotiation Done Status (FALSE or TRUE)    */
/* DESCRIPTION:                                                 */
/* Get auto negothiation done status                            */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_EI_ECORE_PCS_STATUS_t status ;
  ARN5_GIGE_EI_ECORE_TOP_INTSTATUS_t    intStatus ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, FALSE) ;

  status.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.STATUS.wrd) ;
  /* intstatus is the choice for the status */
  intStatus.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.INTSTATUS.wrd) ;

  return (intStatus.bf.PCS_AN_DONEs == 1) ;
}


/****************************************************************/
/* $rtn_hdr_start   CONFIG AUTO NEGOTIATION                     */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_auto_negotiation_cfg(cs_uint16 port_id,
                        cs_ctl_t ctl, cs_uint32 link_timer)
/* INPUTS     : o Port Id                                       */
/*              o CS_DISABLE or CS_ENABLE                       */
/*              o Link timer in microseconds (0...16000)        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Configure auto negotiation related attributes                */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  cs_uint16     val ;
  ARN5_GIGE_EI_ECORE_PCS_CONTROL_t pcsCtl ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  if (link_timer > 16000) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
                  ": link_timer %d s/b 0...16000\n", link_timer) ;
    return (CS_ERROR) ;
  }

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  pcsCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.CONTROL.wrd) ;
  pcsCtl.bf.AutoNegEn = (ctl == CS_ENABLE) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.CONTROL.wrd, pcsCtl.wrd) ;

  link_timer *= 125 ;
  val = link_timer & 0x0000fffe ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.LINK_TIMER_0.wrd, val) ;
  val = (link_timer & 0x001f0000) >> 16 ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.LINK_TIMER_1.wrd, val) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK);
}


/****************************************************************/
/* $rtn_hdr_start   RESTART AUTO NEGOTIATION                    */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_restart_auto_negotiation(cs_uint16 port_id)
/* INPUTS     : o Port Id                                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Restart auto negotiation process.                            */
/*                                                              */
/* The retart bit is SELF CLEARING. This API will poll for the  */
/* the retart bit to clear before returning.                    */
/* A timeout of 1 sec is provided to prevent any indefinite     */
/* blocking by this API.                                        */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  cs_uint32     usec_delay = 100, usec_timeout = 20000 ;
  cs_uint32     usec_elapsed = 0;
  ARN5_GIGE_EI_ECORE_PCS_CONTROL_t pcsCtl ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  pcsCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.CONTROL.wrd) ;
  pcsCtl.bf.AutoNegRestart = 1 ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.CONTROL.wrd, pcsCtl.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  pcsCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.CONTROL.wrd) ;
  while (pcsCtl.bf.AutoNegRestart) {
    if (usec_elapsed >= usec_timeout) break ;
    CS_UDELAY(usec_delay) ;
    usec_elapsed += usec_delay ;
    pcsCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.CONTROL.wrd) ;
  }

  if (pcsCtl.bf.AutoNegRestart) {
    CS_HNDL_ERROR(port_id, EARN5_ETH_RESTART_TIMEOUT, ": Auto Negotiation Restart\n") ;
    return (CS_ERROR) ;
  }
  return(CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET PAUSE QUANTA                            */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_set_pause_quanta(cs_uint16 port_id,
                        cs_uint16 pause_quanta)
/* INPUTS     : o Port Id                                       */
/*              o Pause Quanta                                  */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set the pause quanta which is used in each pause frame sent  */
/* to remote Ethernet device.                                   */
/*                                                              */
/* [pause_quanta] is a 16 bit value expressed in increment of   */
/* 512 ethernet bit times.                                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  /* write LSW then MSW */
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.PAUSE_QUANT0.wrd, pause_quanta) ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.PAUSE_QUANT1.wrd, 0) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   GENERATE A PAUSE FRAME                      */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_generate_pause_frame(cs_uint16 port_id,
                        cs_boolean xon)
/* INPUTS     : o Port Id                                       */
/*              o FALSE (XOFF) or TRUE (XON)                    */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Generate pause frame from microporcessor interface. Usually  */
/* pause frame is generated automatically by Arsenal5. This API */
/* function may be used for debug purpose.                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  cs_uint32     usec_delay = 1000 ;  /* 1 ms */
  ARN5_GIGE_EI_ECORE_TOP_Control_t topCtl ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  topCtl.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.Control.wrd) ;
  topCtl.bf.XON_XOFF = (xon) ? 1 : 0 ;

  /* toggle the gen bit to generate pause frame */
  topCtl.bf.X_GEN ^= 1 ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.Control.wrd, topCtl.wrd) ;

  /* delay at least 20 microprocessor clock cycles */
  CS_UDELAY(usec_delay) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK);
}


/****************************************************************/
/* $rtn_hdr_start   ETH STATUS TO GP OUTPUT MAPPING             */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_sel_status_to_gpio(cs_uint16 port_id,
                        arn5_eth_status_sel_t sel)
/* INPUTS     : o Port Id                                       */
/*              o Status Selection                              */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Select one of the status to output to GPIO.                  */
/*                                                              */
/* [sel] :                                                      */
/*   ARN5_ETH_STATUS_LINK_STAT  or                              */
/*   ARN5_ETH_STATUS_TX_ACTIVITY  or ARN5_ETH_STATUS_RX_ACTIVITY*/
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_MISC_CONFIG_t  miscCfg ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;

  miscCfg.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.CONFIG.wrd) ;
  miscCfg.bf.GPIO_ALARM_STAT_SEL = sel ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.CONFIG.wrd, miscCfg.wrd ) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET MIN PACKET SIZE ON TX CONV              */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_set_conv_tx_min_pkt_size(cs_uint16 port_id,
                        cs_uint8 min_size)
/* INPUTS     : o Port Id                                       */
/*              o minimum packet size (0..25)                   */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set minimum packet size for packets in non-RPR Mode. Packets */
/* of a size less than the programmed value will be dropped.    */
/*                                                              */
/* Note: A value of 0 indicates that the Min Packet Size filter */
/* is disabled.                                                 */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_MISC_CONV_TX_MINPKTSIZE_t  minSize ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  if (min_size > 25) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
                  ": min_size(%d) s/b <= 25\n", min_size) ;
    return (CS_ERROR) ;
  }

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;

  minSize.wrd = CS_REG_READ(
        &pDev->slice[sl_num].GigE.CONV_TX_MINPKTSIZE.wrd) ;

  minSize.bf.value = min_size ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.CONV_TX_MINPKTSIZE.wrd,
        minSize.wrd ) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SELECT THE SOURCE OF TX PAUSE FRAME REQUEST */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_sel_tx_pause_src(cs_uint16 port_id,
                        arn5_tx_pause_src_t src)
/* INPUTS     : o Port Id                                       */
/*              o Pause Source Selection                        */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Select the source for the transmit Pause frame request.      */
/*                                                              */
/* [src] : ARN5_PAUSE_SRC_SYS_INF_BACKPRESSURE or               */
/*         ARN5_PAUSE_SRC_PAUSE_CONTROL_INF    or               */
/*         ARN5_PAUSE_SRC_NO_PAUSE                              */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_MISC_CONFIG_t  miscCfg ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;

  miscCfg.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.CONFIG.wrd) ;
  miscCfg.bf.TX_PAUSE_SRC_SEL = src ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.CONFIG.wrd, miscCfg.wrd ) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   CONTROL THE RX PACKET DROP UPON BACKPRESSURE*/
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_ctl_conv_rx_drop_on_backpressure(cs_uint16 port_id,
                        cs_ctl_t ctl)
/* INPUTS     : o Port Id                                       */
/*              o CS_ENABLE or CS_DISABLE                       */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Enable/disable packet drop/abort upon System Interface       */
/* backpressure assertion.                                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_MISC_CONFIG_t  miscCfg ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;

  miscCfg.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.CONFIG.wrd) ;
  miscCfg.bf.RX_CONV_DROP_EN = (ctl == CS_ENABLE) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.CONFIG.wrd, miscCfg.wrd ) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************/
/* $rtn_hdr_start   SET MIN PACKET SIZE ON TX CONV              */
/* CATEGORY   : Port                                            */
/* ACCESS     : Public                                          */
/* BLOCK      : ETH                                             */
/* CHIP       : Arsenal5                                        */
cs_status  arn5_eth_set_conv_tx_fifo_params(cs_uint16 port_id,
                        cs_uint8 pkt_skid, cs_uint8 threshold)
/* INPUTS     : o Port Id                                       */
/*              o Flow Control Skid in 8-byte words (0...63)    */
/*              o Threshold in 8-byte words (0...63)            */
/* OUTPUTS    : ----                                            */
/* RETURNS    : CS_OK or CS_ERROR                               */
/* DESCRIPTION:                                                 */
/* Set two params regarding to converter's tx fifo.             */
/*                                                              */
/* [pkt_skid] is the minimum number of the 8-byte words in the  */
/* input FIFO before full is asserted.                          */
/*                                                              */
/* [threshold] specifies the number of 8-byte words to be       */
/* available in the SPI4.1 bus input FIFO before the data is    */
/* forwarded to the MAC module.                                 */
/* $rtn_hdr_end                                                 */
/****************************************************************/
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;
  ARN5_GIGE_MISC_CONV_TX_CONFIG_t  txCfg ;

  ARN5_ETH_COMMON_VALIDATE(port_id, pDev, sl_num, CS_ERROR) ;

  if (pkt_skid >= 64) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
                  ": pkt_skid(%d) s/b < 64\n", pkt_skid) ;
    return (CS_ERROR) ;
  }

  if (threshold >= 64) {
    CS_HNDL_ERROR(port_id, EARN5_PORT_INVALID_USER_ARG,
                  ": threshold(%d) s/b < 64\n", threshold) ;
    return (CS_ERROR) ;
  }

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;

  txCfg.wrd = CS_REG_READ(
        &pDev->slice[sl_num].GigE.CONV_TX_CONFIG.wrd) ;

  txCfg.bf.packetSkid = pkt_skid ;
  txCfg.bf.Thr = threshold ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.CONV_TX_CONFIG.wrd,
        txCfg.wrd ) ;

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK) ;
}


/****************************************************************
 *****************     Internal functions       *****************
 ****************************************************************/

/*--------------------------------------------------------------*/
/* common function to set various ethenet cfg bits              */
/*--------------------------------------------------------------*/
static cs_status  arn5_ethnet_mode_cfg(cs_uint16 port_id,
                        cs_boolean promiscuous, cs_boolean tx_addr_insert,
                        cs_boolean len_check, cs_boolean padding_removal,
                        cs_boolean rx_crc_fwd, cs_boolean tx_crc_fwd)
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num, devId ;
  ARN5_GIGE_MISC_CONFIG_t      miscCfg ;
  ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG0_t    cfg0 ;
  ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG1_t    cfg1 ;

  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;
  devId = ARN5_PORT_ID_TO_DEV_ID(port_id) ;
  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;

  ARN5_PORT_LOCK(port_id, ARN5_ID_ETH) ;
  cfg1.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG1.wrd) ;
  cfg0.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG0.wrd) ;

  cfg0.bf.PROMIS_EN = (promiscuous) ? 1 : 0 ;
  cfg0.bf.TX_ADDR_INS = (tx_addr_insert) ? 1 : 0 ;
  cfg0.bf.PAD_EN = (padding_removal) ? 1 : 0 ;
  cfg1.bf.NO_LGTH_CHECK = (len_check) ? 0 : 1 ;

  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG0.wrd, cfg0.wrd) ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.EI.ecore.COMMAND_CONFIG1.wrd, cfg1.wrd) ;

  miscCfg.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.CONFIG.wrd) ;
  miscCfg.bf.RX_CRC_FWD = (rx_crc_fwd) ? 1 : 0 ;
  miscCfg.bf.TX_CRC_FWD = (tx_crc_fwd) ? 1 : 0 ;
  CS_REG_WRITE(&pDev->slice[sl_num].GigE.CONFIG.wrd, miscCfg.wrd) ;
  ARN5_PORT_UNLOCK(port_id, ARN5_ID_ETH) ;

  return (CS_OK);
}

