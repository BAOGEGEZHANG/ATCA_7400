/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_pprbs_irq.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_irq_priv.h"
#include "arn5_pprbs_irq.h"
#include "arn5_pprbs.h"
#include "arn5_pprbs_priv.h"

#define ARN5_PPRBS_IRQ_FLAGS     ( ARN5_IRQ_FLG_RW1C )

/************************************************************************/
/*    IRQ table                                                         */
/************************************************************************/
const arn5_irq_ctl_t arn5_pprbs_irq_tbl[ARN5_PPRBS_MAX_GRP] = {
  [ARN5_GRP_PPRBS_Interrupt] = {
    ARN5_PPRBS_REG_OFST(Interrupt),
    0,
    ARN5_PPRBS_REG_OFST(IntMask),
    ARN5_GRP_PPRBS_Interrupt_ALL,
    0,
    ARN5_PPRBS_IRQ_FLAGS,
    ARN5_PPRBS_COUNT, ARN5_PPRBS_STRIDE * sizeof(cs_reg) },
} ;

/**************** IRQ ID NAME TABLES **********************/
const cs_uint8 * arn5_pprbs_irq_grp_name_tbl[ARN5_PPRBS_MAX_GRP] =
{
  [ARN5_GRP_PPRBS_Interrupt]                             = "PPRBS_Interrupt",
} ;

const arn5_irq_id_name_t
ARN5_PPRBS_Interrupt_irq_id_name_tbl[] = {
  {ARN5_PPRBS_Interrupt_RolloverTxPacket1I,              "RolloverTxPacket1I"},
  {ARN5_PPRBS_Interrupt_RolloverTxByte1I,                "RolloverTxByte1I"},
  {ARN5_PPRBS_Interrupt_SentPackets1I,                   "SentPackets1I"},
  {ARN5_PPRBS_Interrupt_RolloverPacket1I,                "RolloverPacket1I"},
  {ARN5_PPRBS_Interrupt_RolloverByte1I,                  "RolloverByte1I"},
  {ARN5_PPRBS_Interrupt_Rollover1I,                      "Rollover1I"},
  {ARN5_PPRBS_Interrupt_BitError1I,                      "BitError1I"},
  {ARN5_PPRBS_Interrupt_SyncState1I,                     "SyncState1I"},
  {ARN5_PPRBS_Interrupt_RolloverTxPacket0I,              "RolloverTxPacket0I"},
  {ARN5_PPRBS_Interrupt_RolloverTxByte0I,                "RolloverTxByte0I"},
  {ARN5_PPRBS_Interrupt_SentPackets0I,                   "SentPackets0I"},
  {ARN5_PPRBS_Interrupt_RolloverPacket0I,                "RolloverPacket0I"},
  {ARN5_PPRBS_Interrupt_RolloverByte0I,                  "RolloverByte0I"},
  {ARN5_PPRBS_Interrupt_Rollover0I,                      "Rollover0I"},
  {ARN5_PPRBS_Interrupt_BitError0I,                      "BitError0I"},
  {ARN5_PPRBS_Interrupt_SyncState0I,                     "SyncState0I"},
} ;


/************************************************************************
 * GET DEFAULT PPRBS IRQ CONFIGURATION
 ***********************************************************************/
cs_status arn5_pprbs_get_default_irq_cfg(cs_uint16 port_id,
                                        arn5_port_summ_t * pSumm,
                                        arn5_pprbs_cfg_t * pCfg)
{
  /* mask register, so this means disable all PPRBS interrupts */
  pCfg->irq_ctl_tbl[ARN5_GRP_PPRBS_Interrupt] =
    ARN5_OVRD_DEF_IRQ_TO_ENB ? ARN5_GRP_PPRBS_Interrupt_ALL : 0 ;
  return (CS_OK) ;
}

/************************************************************************
 * INIT PPRBS IRQ
 ***********************************************************************/
cs_status arn5_pprbs_init_irq(cs_uint16 port_id, arn5_pprbs_cfg_t * pCfg)
{
  cs_uint32 ii ;

  /* Register PPRBS IRQ table. */
  ARN5_IRQ_RET_IF_ERROR(ARN5_REGISTER_IRQ_TBL(port_id, ARN5_ID_PPRBS,
                       arn5_pprbs_irq_tbl, arn5_pprbs_irq_grp_name_tbl,
                       ARN5_PPRBS_MAX_GRP, NULL), CS_ERROR) ;

  for (ii = 0; ii < ARN5_PPRBS_MAX_GRP; ii++) {
    if (pCfg->irq_ctl_tbl[ii]) {
      /* Clear any interrupts */
      ARN5_IRQ_RET_IF_ERROR(arn5_port_clear_leaf_irq(port_id, ARN5_ID_PPRBS, ii,
                         arn5_pprbs_irq_tbl[ii].irq_map), CS_ERROR) ;

      /* Enable The IRQ requested to be enabled by default */
      ARN5_IRQ_RET_IF_ERROR(arn5_port_enable_leaf_irq(port_id, ARN5_ID_PPRBS, ii,
                          pCfg->irq_ctl_tbl[ii]), CS_ERROR) ;
    }
  }
  return (CS_OK) ;
}

/************************************************************************
 * ISR ROUTINE FOR PPRBS BLOCK
 ***********************************************************************/
cs_status arn5_pprbs_isr(cs_uint16 port_id)
{
  ARN5_t * pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;

  ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_PPRBS,
                ARN5_GRP_PPRBS_Interrupt,
                &arn5_pprbs_irq_tbl[ARN5_GRP_PPRBS_Interrupt],
                arn5_pprbs_irq_grp_name_tbl[ARN5_GRP_PPRBS_Interrupt],
                ARN5_PPRBS_Interrupt_irq_id_name_tbl,
                sizeof(ARN5_PPRBS_Interrupt_irq_id_name_tbl) /
                sizeof(arn5_irq_id_name_t)), CS_ERROR) ;

  return (CS_OK) ;
}

