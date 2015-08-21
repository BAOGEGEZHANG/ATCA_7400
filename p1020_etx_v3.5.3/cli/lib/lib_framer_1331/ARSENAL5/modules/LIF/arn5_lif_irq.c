/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_lif_irq
 *
 * LIF IRQ Tables and APIs
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"

#include "arn5_error.h"
#include "arn5_public.h"
#include "arn5_common.h"
#include "arn5_registers.h"
#include "arn5_lif.h"
#include "arn5_lif_irq.h"
#include "arn5_irq_priv.h"

/**********************************************************************/
/*    IRQ table                                                       */
/**********************************************************************/
const arn5_irq_ctl_t  arn5_lif_irq_tbl[ARN5_LIF_MAX_GRP] = {
  [ARN5_GRP_LIF_INTERRUPT] = {
    ARN5_LIF_REG_OFST(INTERRUPT),
    ARN5_LIF_REG_OFST(STATUS),
    ARN5_LIF_REG_OFST(MASK),
    ARN5_GRP_LIF_INTERRUPT_ALL, 0,
    ARN5_IRQ_FLG_RW1C,ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },
} ;

const cs_uint8 * arn5_lif_irq_grp_name_tbl[ARN5_LIF_MAX_GRP] =
{
  [ARN5_GRP_LIF_INTERRUPT]     = "LIF_INTERRUPT",
} ;


/**************** IRQ ID NAME TABLES **********************/
const arn5_irq_id_name_t
ARN5_GRP_LIF_INTERRUPT_irq_id_name_tbl[] = {
  {ARN5_LIF_INTERRUPT_PRBS_RX_EFi,      "PRBS_RX_EFi"},
  {ARN5_LIF_INTERRUPT_PRBS_SYNCi,       "PRBS_SYNCi"},
  {ARN5_LIF_INTERRUPT_NO_DATAi,         "NO_DATAi"},
  {ARN5_LIF_INTERRUPT_IN_LOCKi,         "IN_LOCKi"},
  {ARN5_LIF_INTERRUPT_SRX_LOCKDETIi,    "SRX_LOCKDETIi"},
} ;


/*********************************************************************
 * GET DEFAULT LIF IRQ CONFIGURATION
 *********************************************************************/
cs_status arn5_lif_get_default_irq_cfg(cs_uint16 port_id,
                                      arn5_port_summ_t * pSumm,
                                      arn5_lif_cfg_t * pCfg)
{
  pCfg->irq = ARN5_GRP_LIF_INTERRUPT_ALL ;
  return (CS_OK) ;
}

/*********************************************************************
 * INIT LIF IRQ
 *********************************************************************/
cs_status arn5_lif_init_irq_cfg(cs_uint16 port_id, arn5_lif_cfg_t * pCfg)
{
  /* Register LIF IRQ table.  */
  ARN5_REGISTER_IRQ_TBL(port_id, ARN5_ID_LIF, arn5_lif_irq_tbl,
                        arn5_lif_irq_grp_name_tbl,
                        ARN5_LIF_MAX_GRP, NULL) ;

  arn5_port_enable_leaf_irq(port_id, ARN5_ID_LIF,
                       ARN5_GRP_LIF_INTERRUPT, pCfg->irq) ;

  return (CS_OK) ;
}

/*********************************************************************/
/*                  ISR ROUTINE FOR LIF BLOCK                        */
/*********************************************************************/
cs_status arn5_lif_isr(cs_uint16 port_id)
{
  ARN5_t              * pDev ;

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id);

  arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_LIF,
                     ARN5_GRP_LIF_INTERRUPT,
                     &arn5_lif_irq_tbl[ARN5_GRP_LIF_INTERRUPT],
                     "LIF_INTERRUPT",
                     ARN5_GRP_LIF_INTERRUPT_irq_id_name_tbl,
                     sizeof(ARN5_GRP_LIF_INTERRUPT_irq_id_name_tbl) /
                     sizeof(arn5_irq_id_name_t)) ;

  return (CS_OK) ;
}





