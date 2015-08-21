
/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_mpif_irq.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_irq_priv.h"
#include "arn5_mpif_irq.h"
#include "arn5_mpif_priv.h"

#define ARN5_MPIF_IRQ_FLAGS      ( ARN5_IRQ_FLG_1_TO_ENABLE )

/************************************************************************/
/*    IRQ table                                                         */
/************************************************************************/
const arn5_irq_ctl_t arn5_mpif_irq_tbl[ARN5_MPIF_MAX_GRP] = {
  [ARN5_GRP_MPIF_MPIF_INTERRUPT] = {
    ARN5_MPIF_REG_OFST(MPIF_INTERRUPT),
    0,
    ARN5_MPIF_REG_OFST(MPIF_INTENABLE),
    ARN5_GRP_MPIF_MPIF_INTERRUPT_ALL,
    ARN5_GRP_MPIF_MPIF_INTERRUPT_SPL,
    ARN5_MPIF_IRQ_FLAGS | ARN5_IRQ_FLG_RW1C | ARN5_IRQ_FLG_SPL_INT,
    0, 0 },

  [ARN5_GRP_MPIF_GPIORInt0] = {
    ARN5_MPIF_REG_OFST(GPIORInt0),
    0,
    ARN5_MPIF_REG_OFST(GPIORMask0),
    ARN5_GRP_MPIF_GPIORInt0_ALL,
    0,
    ARN5_IRQ_FLG_RW1C,
    0, 0 },

  [ARN5_GRP_MPIF_GPIORInt1] = {
    ARN5_MPIF_REG_OFST(GPIORInt1),
    0,
    ARN5_MPIF_REG_OFST(GPIORMask1),
    ARN5_GRP_MPIF_GPIORInt1_ALL,
    0,
    ARN5_IRQ_FLG_RW1C,
    0, 0 },

  [ARN5_GRP_MPIF_GPIOFInt0] = {
    ARN5_MPIF_REG_OFST(GPIOFInt0),
    0,
    ARN5_MPIF_REG_OFST(GPIOFMask0),
    ARN5_GRP_MPIF_GPIORInt0_ALL,
    0,
    ARN5_IRQ_FLG_RW1C,
    0, 0 },

  [ARN5_GRP_MPIF_GPIOFInt1] = {
    ARN5_MPIF_REG_OFST(GPIOFInt1),
    0,
    ARN5_MPIF_REG_OFST(GPIOFMask1),
    ARN5_GRP_MPIF_GPIOFInt1_ALL,
    0,
    ARN5_IRQ_FLG_RW1C,
    0, 0 },
} ;

/**************** IRQ ID NAME TABLES **********************/
const cs_uint8 * arn5_mpif_irq_grp_name_tbl[ARN5_MPIF_MAX_GRP] =
{
  [ARN5_GRP_MPIF_MPIF_INTERRUPT]                 = "MPIF_INTERRUPT",
  [ARN5_GRP_MPIF_GPIORInt0]                      = "GPIORInt0",
  [ARN5_GRP_MPIF_GPIORInt1]                      = "GPIORInt1",
  [ARN5_GRP_MPIF_GPIOFInt0]                      = "GPIOFInt0",
  [ARN5_GRP_MPIF_GPIOFInt1]                      = "GPIOFInt1",
} ;

const arn5_irq_id_name_t
ARN5_MPIF_MPIF_INTERRUPT_irq_id_name_tbl[] = {
  {ARN5_MPIF_MPIF_INTERRUPT_buserrI,            "buserrI"},
} ;

const arn5_irq_id_name_t
ARN5_MPIF_GPIORInt0_irq_id_name_tbl[] = {
  {ARN5_MPIF_GPIORInt0_RI0_7,                   "RI0_7"},
  {ARN5_MPIF_GPIORInt0_RI0_6,                   "RI0_6"},
  {ARN5_MPIF_GPIORInt0_RI0_5,                   "RI0_5"},
  {ARN5_MPIF_GPIORInt0_RI0_4,                   "RI0_4"},
  {ARN5_MPIF_GPIORInt0_RI0_3,                   "RI0_3"},
  {ARN5_MPIF_GPIORInt0_RI0_2,                   "RI0_2"},
  {ARN5_MPIF_GPIORInt0_RI0_1,                   "RI0_1"},
  {ARN5_MPIF_GPIORInt0_RI0_0,                   "RI0_0"},
} ;

const arn5_irq_id_name_t
ARN5_MPIF_GPIORInt1_irq_id_name_tbl[] = {
  {ARN5_MPIF_GPIORInt0_RI1_7,                   "RI1_7"},
  {ARN5_MPIF_GPIORInt0_RI1_6,                   "RI1_6"},
  {ARN5_MPIF_GPIORInt0_RI1_5,                   "RI1_5"},
  {ARN5_MPIF_GPIORInt0_RI1_4,                   "RI1_4"},
  {ARN5_MPIF_GPIORInt0_RI1_3,                   "RI1_3"},
  {ARN5_MPIF_GPIORInt0_RI1_2,                   "RI1_2"},
  {ARN5_MPIF_GPIORInt0_RI1_1,                   "RI1_1"},
  {ARN5_MPIF_GPIORInt0_RI1_0,                   "RI1_0"},
} ;

const arn5_irq_id_name_t
ARN5_MPIF_GPIOFInt0_irq_id_name_tbl[] = {
  {ARN5_MPIF_GPIOFInt0_FI0_7,                   "FI0_7"},
  {ARN5_MPIF_GPIOFInt0_FI0_6,                   "FI0_6"},
  {ARN5_MPIF_GPIOFInt0_FI0_5,                   "FI0_5"},
  {ARN5_MPIF_GPIOFInt0_FI0_4,                   "FI0_4"},
  {ARN5_MPIF_GPIOFInt0_FI0_3,                   "FI0_3"},
  {ARN5_MPIF_GPIOFInt0_FI0_2,                   "FI0_2"},
  {ARN5_MPIF_GPIOFInt0_FI0_1,                   "FI0_1"},
  {ARN5_MPIF_GPIOFInt0_FI0_0,                   "FI0_0"},
} ;

const arn5_irq_id_name_t
ARN5_MPIF_GPIOFInt1_irq_id_name_tbl[] = {
  {ARN5_MPIF_GPIOFInt0_FI1_7,                   "FI1_7"},
  {ARN5_MPIF_GPIOFInt0_FI1_6,                   "FI1_6"},
  {ARN5_MPIF_GPIOFInt0_FI1_5,                   "FI1_5"},
  {ARN5_MPIF_GPIOFInt0_FI1_4,                   "FI1_4"},
  {ARN5_MPIF_GPIOFInt0_FI1_3,                   "FI1_3"},
  {ARN5_MPIF_GPIOFInt0_FI1_2,                   "FI1_2"},
  {ARN5_MPIF_GPIOFInt0_FI1_1,                   "FI1_1"},
  {ARN5_MPIF_GPIOFInt0_FI1_0,                   "FI1_0"},
} ;


/************************************************************************
 * MPIF IRQ SPECIAL HANDLER
 ***********************************************************************/
static
cs_uint16 arn5_mpif_irq_spl_hndlr(arn5_irq_spl_info_t * irq_spl_info)
{
  cs_uint8 dev_id = (cs_uint8)(irq_spl_info->dpid) ;
  ARN5_t * pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;
  cs_uint16 irq_id = irq_spl_info->op_mode.irq_hndl.irq_info->irq_id ;
  cs_uint16 status = 0;

  if (irq_spl_info->irq_oper != ARN5_IRQ_SPL_INT) {
    return (0) ;
  }

  if (irq_id == ARN5_MPIF_MPIF_INTERRUPT_buserrI) {
    irq_spl_info->op_mode.irq_hndl.irq_info->flags |=
      (ARN5_IRQ_INFO_STATUS_FLAG |
       ARN5_IRQ_INFO_PARAM1_FLAG | ARN5_IRQ_INFO_PARAM2_FLAG) ;
    /* Status stores PREV_DATA */
    irq_spl_info->op_mode.irq_hndl.irq_info->status =
      CS_REG_READ(&pDev->MicroIF.PREV_DATA.wrd) ;
    /* Param1 stores the PREV_ADDRESS, bit 31 stores the PREV_TRANSACTION */
    irq_spl_info->op_mode.irq_hndl.irq_info->param1 =
      (CS_REG_READ(&pDev->MicroIF.PREV_TRANSACTION.wrd) << 31) |
      (CS_REG_READ(&pDev->MicroIF.PREV_ADDRESS1.wrd) << 16) |
       CS_REG_READ(&pDev->MicroIF.PREV_ADDRESS0.wrd) ;
    /* Param2 stores the TIMEOUT_ADDRESS, bit 31 stores the TIMEOUT_TRANSACTION */
    irq_spl_info->op_mode.irq_hndl.irq_info->param2 =
      (CS_REG_READ(&pDev->MicroIF.TIMEOUT_TRANSACTION.wrd) << 31) |
      (CS_REG_READ(&pDev->MicroIF.TIMEOUT_ADDRESS1.wrd) << 16) |
       CS_REG_READ(&pDev->MicroIF.TIMEOUT_ADDRESS0.wrd) ;

    return (status) ;
  }
  return (0) ;
}


/************************************************************************
 * GET DEFAULT MPIF IRQ CONFIGURATION
 ***********************************************************************/
cs_status arn5_mpif_dev_get_default_irq_cfg(cs_uint8 dev_id,
                        arn5_dev_summ_t * pSumm,
                        arn5_mpif_cfg_t * pCfg)
{
  /* disable buserrI by default */
  pCfg->irq_ctl_tbl[ARN5_GRP_MPIF_MPIF_INTERRUPT] = 0 ;
  pCfg->irq_ctl_tbl[ARN5_GRP_MPIF_GPIORInt0] = 0 ;
  pCfg->irq_ctl_tbl[ARN5_GRP_MPIF_GPIORInt1] = 0 ;
  pCfg->irq_ctl_tbl[ARN5_GRP_MPIF_GPIOFInt0] = 0 ;
  pCfg->irq_ctl_tbl[ARN5_GRP_MPIF_GPIOFInt1] = 0 ;

  return (CS_OK) ;
}

/************************************************************************
 * INIT MPIF IRQ
 ***********************************************************************/
cs_status arn5_mpif_dev_init_irq_cfg(cs_uint8 dev_id,
                        arn5_mpif_cfg_t * pCfg)
{
  cs_uint32 ii ;

  /* Register MPIF IRQ table. */
  ARN5_IRQ_RET_IF_ERROR(ARN5_REGISTER_IRQ_TBL(dev_id, ARN5_ID_MPIF,
                       arn5_mpif_irq_tbl, arn5_mpif_irq_grp_name_tbl,
                       ARN5_MPIF_MAX_GRP, arn5_mpif_irq_spl_hndlr), CS_ERROR) ;

  for (ii = 0; ii < ARN5_MPIF_MAX_GRP; ii++) {
    if (pCfg->irq_ctl_tbl[ii]) {
      /* Clear any interrupts */
      ARN5_IRQ_RET_IF_ERROR(arn5_dev_clear_leaf_irq(dev_id, ARN5_ID_MPIF, ii,
                         arn5_mpif_irq_tbl[ii].irq_map), CS_ERROR) ;

      /* Enable The IRQ requested to be enabled by default */
      ARN5_IRQ_RET_IF_ERROR(arn5_dev_enable_leaf_irq(dev_id, ARN5_ID_MPIF, ii,
                          pCfg->irq_ctl_tbl[ii]), CS_ERROR) ;
    }
  }

  return (CS_OK) ;
}

/************************************************************************
 * ISR ROUTINE FOR MPIF BLOCK
 ***********************************************************************/
cs_status arn5_mpif_dev_isr(cs_uint8 dev_id)
{
  ARN5_MPIF_GlobalInt_t  globalInt ;

  ARN5_t * pDev = ARN5_DEV_ID_TO_DEV_BASE(dev_id) ;

  globalInt.wrd = CS_REG_READ(&pDev->MicroIF.GlobalInt.wrd) ;

  if (globalInt.bf.MPIFI) {
    ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev, ARN5_ID_MPIF,
                ARN5_GRP_MPIF_MPIF_INTERRUPT,
                &arn5_mpif_irq_tbl[ARN5_GRP_MPIF_MPIF_INTERRUPT],
                arn5_mpif_irq_grp_name_tbl[ARN5_GRP_MPIF_MPIF_INTERRUPT],
                ARN5_MPIF_MPIF_INTERRUPT_irq_id_name_tbl,
                sizeof(ARN5_MPIF_MPIF_INTERRUPT_irq_id_name_tbl) /
                sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
  }

  if (globalInt.bf.GPIOI) {
    ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev, ARN5_ID_MPIF,
                ARN5_GRP_MPIF_GPIORInt0,
                &arn5_mpif_irq_tbl[ARN5_GRP_MPIF_GPIORInt0],
                arn5_mpif_irq_grp_name_tbl[ARN5_GRP_MPIF_GPIORInt0],
                ARN5_MPIF_GPIORInt0_irq_id_name_tbl,
                sizeof(ARN5_MPIF_GPIORInt0_irq_id_name_tbl) /
                sizeof(arn5_irq_id_name_t)), CS_ERROR) ;

    ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev, ARN5_ID_MPIF,
                ARN5_GRP_MPIF_GPIORInt1,
                &arn5_mpif_irq_tbl[ARN5_GRP_MPIF_GPIORInt1],
                arn5_mpif_irq_grp_name_tbl[ARN5_GRP_MPIF_GPIORInt1],
                ARN5_MPIF_GPIORInt1_irq_id_name_tbl,
                sizeof(ARN5_MPIF_GPIORInt1_irq_id_name_tbl) /
                sizeof(arn5_irq_id_name_t)), CS_ERROR) ;

    ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev, ARN5_ID_MPIF,
                ARN5_GRP_MPIF_GPIOFInt0,
                &arn5_mpif_irq_tbl[ARN5_GRP_MPIF_GPIOFInt0],
                arn5_mpif_irq_grp_name_tbl[ARN5_GRP_MPIF_GPIOFInt0],
                ARN5_MPIF_GPIOFInt0_irq_id_name_tbl,
                sizeof(ARN5_MPIF_GPIOFInt0_irq_id_name_tbl) /
                sizeof(arn5_irq_id_name_t)), CS_ERROR) ;

    ARN5_IRQ_RET_IF_ERROR(arn5_irq_grp_hndlr(dev_id, (cs_uint32)pDev, ARN5_ID_MPIF,
                ARN5_GRP_MPIF_GPIOFInt1,
                &arn5_mpif_irq_tbl[ARN5_GRP_MPIF_GPIOFInt1],
                arn5_mpif_irq_grp_name_tbl[ARN5_GRP_MPIF_GPIOFInt1],
                ARN5_MPIF_GPIOFInt1_irq_id_name_tbl,
                sizeof(ARN5_MPIF_GPIOFInt1_irq_id_name_tbl) /
                sizeof(arn5_irq_id_name_t)), CS_ERROR) ;
  }

  return (CS_OK) ;
}

