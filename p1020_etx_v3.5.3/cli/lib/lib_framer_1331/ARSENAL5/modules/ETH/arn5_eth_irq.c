/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_eth_irq.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_irq_priv.h"
#include "arn5_eth_irq.h"
#include "arn5_eth_data.h"
#include "arn5_eth_priv.h"

#define ARN5_ETH_IRQ_FLAGS       ( ARN5_IRQ_FLG_1_TO_ENABLE | \
                                  ARN5_IRQ_FLG_RW1C )

/************************************************************************/
/*    IRQ table                                                         */
/************************************************************************/
const arn5_irq_ctl_t arn5_eth_irq_tbl[ARN5_ETH_MAX_GRP] = {
  [ARN5_GRP_GIGE_EI_ECORE_TOP_INTERRUPT] = {
    ARN5_ETH_EI_REG_OFST(ecore.INTERRUPT),
    ARN5_ETH_EI_REG_OFST(ecore.INTSTATUS),
    ARN5_ETH_EI_REG_OFST(ecore.INTENABLE),
    ARN5_GRP_GIGE_EI_ECORE_TOP_INTERRUPT_ALL,
    0,
    ARN5_ETH_IRQ_FLAGS,ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE},
  [ARN5_GRP_GIGE_EI_LINE_INTERRUPT] = {
    ARN5_ETH_EI_REG_OFST(INTERRUPT),
    ARN5_ETH_EI_REG_OFST(STATUS),
    ARN5_ETH_EI_REG_OFST(MASK),
    ARN5_GRP_GIGE_EI_LINE_INTERRUPT_ALL,
    0,
    ARN5_ETH_IRQ_FLAGS,ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },
  [ARN5_GRP_GIGE_MISC_CONV_INTERRUPT] = {
    ARN5_ETH_REG_OFST(CONV_INTERRUPT),
    0,
    ARN5_ETH_REG_OFST(CONV_INTENABLE),
    ARN5_GRP_GIGE_MISC_CONV_INTERRUPT_ALL,
    0,
    ARN5_ETH_IRQ_FLAGS,ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },
} ;

const arn5_irq_ctl_t arn5_eth_imed_irq_tbl[ARN5_ETH_MAX_IMED] = {
  [ARN5_IMED_GIGE_EI_TOP_ETHERNET_INTERRUPT] = {
    ARN5_ETH_EI_REG_OFST(ETHERNET_INTERRUPT),
    0,
    ARN5_ETH_EI_REG_OFST(ETHERNET_INTENABLE),
    ARN5_IMED_GIGE_EI_TOP_ETHERNET_INTERRUPT_ALL,
    0,
    ARN5_IRQ_FLG_1_TO_ENABLE,ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },
  [ARN5_IMED_GIGE_MISC_INTERRUPT] = {
    ARN5_ETH_REG_OFST(INTERRUPT),
    0,
    ARN5_ETH_REG_OFST(INTENABLE),
    ARN5_IMED_GIGE_MISC_INTERRUPT_ALL,
    0,
    ARN5_IRQ_FLG_1_TO_ENABLE,ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },
} ;


const cs_uint8 * arn5_eth_irq_grp_name_tbl[ARN5_ETH_MAX_GRP] =
{
  [ARN5_GRP_GIGE_EI_ECORE_TOP_INTERRUPT]                = "GIGE EI_ECORE_TOP_INTERRUPT",
  [ARN5_GRP_GIGE_EI_LINE_INTERRUPT]                     = "GIGE EI_LINE_INTERRUPT",
  [ARN5_GRP_GIGE_MISC_CONV_INTERRUPT]                   = "GIGE_MISC_CONV_INTERRUPT",
} ;

const arn5_irq_id_name_t
ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_irq_id_name_tbl[] = {
  {ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_RA_TX_UFLWi,        "RA_TX_UFLWi"},
  {ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_RA_TX_OFLWi,        "RA_TX_OFLWi"},
  {ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_RA_RX_UFLWi,        "RA_RX_UFLWi"},
  {ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_RA_RX_OFLWi,        "RA_RX_OFLWi"},
  {ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_MAC_PROT_ERR_WRi,   "MAC_PROT_ERR_WRi"},
  {ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_MAC_PROT_ERR_RDi,   "MAC_PROT_ERR_RDi"},
  {ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_PCS_AN_DONEi,       "PCS_AN_DONEi"},
  {ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_PCS_LINKi,          "PCS_LINKi"},
} ;

const arn5_irq_id_name_t
ARN5_GIGE_EI_LINE_INTERRUPT_irq_id_name_tbl[] = {
  {ARN5_GIGE_EI_LINE_INTERRUPT_PRBS_RX_EFi,             "PRBS_RX_EFi"},
  {ARN5_GIGE_EI_LINE_INTERRUPT_PRBS_NOSYNCi,            "PRBS_NOSYNCi"},
  {ARN5_GIGE_EI_LINE_INTERRUPT_NO_DATAi,                "NO_DATAi"},
  {ARN5_GIGE_EI_LINE_INTERRUPT_IN_LOCKi,                "IN_LOCKi"},
  {ARN5_GIGE_EI_LINE_INTERRUPT_RX_LOCKDETIi,            "RX_LOCKDETIi"},
} ;

const arn5_irq_id_name_t
ARN5_GIGE_MISC_CONV_INTERRUPT_irq_id_name_tbl[] = {
  {ARN5_GIGE_MISC_CONV_INTERRUPT_TX_SHORT_DROP_MSBi,    "TX_SHORT_DROP_MSBi"},
  {ARN5_GIGE_MISC_CONV_INTERRUPT_TX_SHORT_DROPi,        "TX_SHORT_DROPi"},
  {ARN5_GIGE_MISC_CONV_INTERRUPT_TX_FIFO_UNDERFLOWi,    "TX_FIFO_UNDERFLOWi"},
  {ARN5_GIGE_MISC_CONV_INTERRUPT_TX_FIFO_PAR_ERRi,      "TX_FIFO_PAR_ERRi"},
  {ARN5_GIGE_MISC_CONV_INTERRUPT_TX_BADFRAMEi,          "TX_BADFRAMEi"},
  {ARN5_GIGE_MISC_CONV_INTERRUPT_TX_MEM_PAR_ERRi,       "TX_MEM_PAR_ERRi"},
  {ARN5_GIGE_MISC_CONV_INTERRUPT_RX_DROPi,              "RX_DROPi"},
  {ARN5_GIGE_MISC_CONV_INTERRUPT_RX_ABORTi,             "RX_ABORTi"},
} ;

const cs_uint8 * arn5_eth_irq_imed_name_tbl[ARN5_ETH_MAX_IMED] =
{
  [ARN5_IMED_GIGE_EI_TOP_ETHERNET_INTERRUPT]    = "GIGE_EI_TOP_ETHERNET_INTERRUPT",
  [ARN5_IMED_GIGE_MISC_INTERRUPT]               = "GIGE_MISC_INTERRUPT",
} ;

const arn5_irq_id_name_t
ARN5_GIGE_EI_TOP_ETHERNET_INTERRUPT_irq_id_name_tbl[] = {
  {ARN5_GIGE_EI_TOP_ETHERNET_INTERRUPT_EI_ECOREi,       "EI_ECOREi"},
  {ARN5_GIGE_EI_TOP_ETHERNET_INTERRUPT_EI_LINEi,        "EI_LINEi"},
} ;

const arn5_irq_id_name_t
ARN5_GIGE_MISC_INTERRUPT_irq_id_name_tbl[] = {
  {ARN5_GIGE_MISC_INTERRUPT_EI_CONVi,                   "EI_CONVi"},
  {ARN5_GIGE_MISC_INTERRUPT_EIi,                        "EIi"},
} ;


/************************************************************************
 * GET DEFAULT ETH IRQ CONFIGURATION
 ***********************************************************************/
cs_status arn5_eth_get_default_irq_cfg(cs_uint16 port_id,
                        arn5_port_summ_t * pSumm,
                        arn5_eth_cfg_t * pCfg)
{
  cs_uint16     ii ;

  /* default is to enable all */
  for (ii = 0; ii < ARN5_ETH_MAX_GRP; ii++) {
    pCfg->irq_ctl_tbl[ii] = arn5_eth_irq_tbl[ii].irq_map;
  }

  if (!ARN5_OVRD_DEF_IRQ_TO_ENB) {
    pCfg->irq_ctl_tbl[ARN5_GRP_GIGE_EI_LINE_INTERRUPT] = 0 ;
  }
  return (CS_OK) ;
}


/************************************************************************
 * INIT ETH IRQ
 ***********************************************************************/
cs_status arn5_eth_init_irq_cfg(cs_uint16 port_id, arn5_eth_cfg_t * pCfg)
{
  cs_uint32 ii ;

  /* Register ETH IRQ table. */
  ARN5_IRQ_RET_IF_ERROR(ARN5_REGISTER_IRQ_TBL(port_id, ARN5_ID_ETH,
                       arn5_eth_irq_tbl, arn5_eth_irq_grp_name_tbl,
                       ARN5_ETH_MAX_GRP, NULL), CS_ERROR) ;
  ARN5_IRQ_RET_IF_ERROR(ARN5_REGISTER_IMED_TBL(port_id, ARN5_ID_ETH,
                       arn5_eth_imed_irq_tbl, arn5_eth_irq_imed_name_tbl,
                       ARN5_ETH_MAX_IMED, NULL), CS_ERROR) ;

  for (ii = 0; ii < ARN5_ETH_MAX_GRP; ii++) {
    if (pCfg->irq_ctl_tbl[ii]) {
      /* Clear any interrupts */
      ARN5_IRQ_RET_IF_ERROR(arn5_port_clear_leaf_irq(port_id, ARN5_ID_ETH, ii,
                         arn5_eth_irq_tbl[ii].irq_map), CS_ERROR) ;

      /* Enable The IRQ requested to be enabled by default */
      ARN5_IRQ_RET_IF_ERROR(arn5_port_enable_leaf_irq(port_id, ARN5_ID_ETH, ii,
                          pCfg->irq_ctl_tbl[ii]), CS_ERROR) ;
    }
  }
  /* Enable all the intermediate interrupt controls by default */
  for (ii = 0; ii < ARN5_ETH_MAX_IMED; ii++) {
    ARN5_IRQ_RET_IF_ERROR(arn5_enable_imed_irq(port_id, ARN5_ID_ETH, ii,
                        arn5_eth_imed_irq_tbl[ii].irq_map), CS_ERROR) ;
  }
  return (CS_OK) ;
}

/************************************************************************
 * ISR ROUTINE FOR ETH BLOCK
 ***********************************************************************/
cs_status arn5_eth_isr(cs_uint16 port_id)
{
  ARN5_t        * pDev ;
  cs_uint8      sl_num ;

  ARN5_GIGE_MISC_INTERRUPT_t    misc_intrpt ;

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id) ;

  misc_intrpt.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.INTERRUPT.wrd) ;

  if (misc_intrpt.bf.EI_CONVi) {
    ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_ETH,
                  ARN5_GRP_GIGE_MISC_CONV_INTERRUPT,
                  &arn5_eth_irq_tbl[ARN5_GRP_GIGE_MISC_CONV_INTERRUPT],
                  arn5_eth_irq_grp_name_tbl[ARN5_GRP_GIGE_MISC_CONV_INTERRUPT],
                  ARN5_GIGE_MISC_CONV_INTERRUPT_irq_id_name_tbl,
                  sizeof(ARN5_GIGE_MISC_CONV_INTERRUPT_irq_id_name_tbl) /
                  sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
  }
  if (misc_intrpt.bf.EIi) {
    ARN5_GIGE_EI_TOP_ETHERNET_INTERRUPT_t  eth_intrpt ;

    eth_intrpt.wrd = CS_REG_READ(&pDev->slice[sl_num].GigE.EI.ETHERNET_INTERRUPT.wrd) ;

    if (eth_intrpt.bf.EI_ECOREi) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_ETH,
                  ARN5_GRP_GIGE_EI_ECORE_TOP_INTERRUPT,
                  &arn5_eth_irq_tbl[ARN5_GRP_GIGE_EI_ECORE_TOP_INTERRUPT],
                  arn5_eth_irq_grp_name_tbl[ARN5_GRP_GIGE_EI_ECORE_TOP_INTERRUPT],
                  ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_irq_id_name_tbl,
                  sizeof(ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_irq_id_name_tbl) /
                  sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
    if (eth_intrpt.bf.EI_LINEi) {
      ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_ETH,
                  ARN5_GRP_GIGE_EI_LINE_INTERRUPT,
                  &arn5_eth_irq_tbl[ARN5_GRP_GIGE_EI_LINE_INTERRUPT],
                  arn5_eth_irq_grp_name_tbl[ARN5_GRP_GIGE_EI_LINE_INTERRUPT],
                  ARN5_GIGE_EI_LINE_INTERRUPT_irq_id_name_tbl,
                  sizeof(ARN5_GIGE_EI_LINE_INTERRUPT_irq_id_name_tbl) /
                  sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
    }
  }
  return (CS_OK) ;
}


