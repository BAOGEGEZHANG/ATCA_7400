/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_eth_irq.c
 *
 * Milan driver libraries pertaining to XETH block's IRQ
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_registers.h"
#include "m2r_public.h"
#include "m2r_eth.h"
#include "m2r_irq_priv.h"
#include "m2r_eth_irq.h"

/************************************************************************/
/*    IRQ table                                                         */
/************************************************************************/
const m2r_irq_ctl_t  m2r_eth_irq_tbl[M2R_ETH_MAX_GRP] = {
  CS_DI([M2R_GRP_ETH_MP_Interrupt] =) {
      M2R_ETH_REG_OFFSET(mp_Interrupt),
      0,
      M2R_ETH_REG_OFFSET(mp_Mask),
      M2R_GRP_ETH_MP_Interrupt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_ETH_TX_Interrupt] =) {
      M2R_ETH_REG_OFFSET(tx_Interrupt),
      M2R_ETH_REG_OFFSET(tx_Status),
      M2R_ETH_REG_OFFSET(tx_Mask),
      M2R_GRP_ETH_TX_Interrupt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_ETH_RX_Interrupt_0] =) {
      M2R_ETH_REG_OFFSET(Interrupt_0),
      M2R_ETH_REG_OFFSET(Status_0),
      M2R_ETH_REG_OFFSET(Mask_0),
      M2R_GRP_ETH_RX_Interrupt_0_ALL, 0, M2R_IRQ_FLG_RW1C }

/* Feature 1 */

} ;


const char * m2r_eth_irq_grp_name_tbl[M2R_ETH_MAX_GRP] =
{
  CS_DI([M2R_GRP_ETH_MP_Interrupt]   =) "mp_Interrupt",
  CS_DI([M2R_GRP_ETH_TX_Interrupt]   =) "tx_Interrupt",
  CS_DI([M2R_GRP_ETH_RX_Interrupt_0] =) "Interrupt_0"
/* Feature 1 */
} ;


/**************** IRQ ID NAME TABLES **********************/
const m2r_irq_id_name_t
M2R_GRP_ETH_MP_Interrupt_irq_id_name_tbl[] = {
  {M2R_ETH_MP_Interrupt_TxDataComBadParI,       "TxDataComBadParI"},
  {M2R_ETH_MP_Interrupt_TxDataComBadFrmI,       "TxDataComBadFrmI"},
  {M2R_ETH_MP_Interrupt_TxDataComUndFlwI,       "TxDataComUndFlwI"},
  {M2R_ETH_MP_Interrupt_TxDataComOvrFlwI,       "TxDataComOvrFlwI"},
  {M2R_ETH_MP_Interrupt_RxDataComBadFrmI,       "RxDataComBadFrmI"},
  {M2R_ETH_MP_Interrupt_RxDataComOvrFlwI,       "RxDataComOvrFlwI"},
  {M2R_ETH_MP_Interrupt_RxDataMemParErrI,       "RxDataMemParErrI"},
  {M2R_ETH_MP_Interrupt_TxDataMemParErrI,       "TxDataMemParErrI"},
  {M2R_ETH_MP_Interrupt_RxMessMemParErrI,       "RxMessMemParErrI"},
  {M2R_ETH_MP_Interrupt_TxMessMemParErrI,       "TxMessMemParErrI"},
  {M2R_ETH_MP_Interrupt_RxNoClkI,       	"RxNoClkI"},
  {M2R_ETH_MP_Interrupt_TxNoClkI,       	"TxNoClkI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_ETH_TX_Interrupt_irq_id_name_tbl[] = {
/* Feature 1 */
  {M2R_ETH_TX_Interrupt_GoodPktSatI,    "GoodPktSatI"},
  {M2R_ETH_TX_Interrupt_ErrorPktSatI,   "ErrorPktSatI"},
  {M2R_ETH_TX_Interrupt_ErrorPktI,      "ErrorPktI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_ETH_RX_Interrupt_0_irq_id_name_tbl[] = {
/* Feature 1 */
  {M2R_ETH_RX_Interrupt_0_DropPktSatI,  "DropPktSatI"},
  {M2R_ETH_RX_Interrupt_0_DropPktI,     "DropPktI"},
  {M2R_ETH_RX_Interrupt_0_ErrorCRCSatI, "ErrorCRCSatI"},
  {M2R_ETH_RX_Interrupt_0_ErrorCRCI,    "ErrorCRCI"},
  {M2R_ETH_RX_Interrupt_0_GoodPktSatI,  "GoodPktSatI"},
  {M2R_ETH_RX_Interrupt_0_ErrorPktSatI, "ErrorPktSatI"},
  {M2R_ETH_RX_Interrupt_0_ErrorPktI,    "ErrorPktI"}
} ;

/* Feature 1 */

/* Feature 1 */


/************************************************************************
 * GET DEFAULT XETH IRQ CONFIGURATION
 ***********************************************************************/
cs_status m2r_eth_get_default_irq_cfg(cs_uint16 port_id,
                                      m2r_port_summ_t * pSumm,
                                      m2r_eth_cfg_t * pCfg)
{
  cs_uint16   ii ;

  /* Clear the default IRQ table */
  for (ii = 0; ii < M2R_ETH_MAX_GRP; ii++) {
    pCfg->irq_ctl_tbl[ii] = 0 ;
  }

  pCfg->irq_ctl_tbl[M2R_GRP_ETH_MP_Interrupt] =
                        M2R_GRP_ETH_MP_Interrupt_ALL ;

  pCfg->irq_ctl_tbl[M2R_GRP_ETH_TX_Interrupt] =
                        M2R_GRP_ETH_TX_Interrupt_ALL ;

  pCfg->irq_ctl_tbl[M2R_GRP_ETH_RX_Interrupt_0] = (
                    M2R_ETH_RX_Interrupt_0_DropPktSatI     |
                    M2R_ETH_RX_Interrupt_0_DropPktI        |
                    M2R_ETH_RX_Interrupt_0_ErrorCRCSatI    |
                    M2R_ETH_RX_Interrupt_0_ErrorCRCI       |
                    M2R_ETH_RX_Interrupt_0_GoodPktSatI     |
                    M2R_ETH_RX_Interrupt_0_ErrorPktSatI    |
                    M2R_ETH_RX_Interrupt_0_ErrorPktI   ) ;

/* Feature 1 */

  return(CS_OK) ;
}

/************************************************************************
 * INIT XETH IRQ
 ***********************************************************************/
cs_status m2r_eth_init_irq(cs_uint16 port_id, m2r_eth_cfg_t * pCfg)
{
  cs_uint32     ii ;

  /* Register ETH IRQ table. */
  M2R_REGISTER_IRQ_TBL(port_id, M2R_ID_ETH, m2r_eth_irq_tbl,
                       m2r_eth_irq_grp_name_tbl, M2R_ETH_MAX_GRP, NULL) ;

  for(ii = 0; ii < M2R_ETH_MAX_GRP; ii++) {
    if (pCfg->irq_ctl_tbl[ii]) {
      /* Clear any interrupts */
      m2r_clear_leaf_irq(port_id, M2R_ID_ETH, ii,
                         m2r_eth_irq_tbl[ii].irq_map) ;

      /* Enable The IRQ requested to be enabled by default */
      m2r_enable_leaf_irq(port_id, M2R_ID_ETH, ii,
                          pCfg->irq_ctl_tbl[ii]) ;
    }
  }
  return (CS_OK) ;
}

/************************************************************************/
/*                  ISR ROUTINE FOR FRAMER BLOCK                        */
/************************************************************************/
cs_status m2r_eth_isr(cs_uint16 port_id)
{
  M2R_t              * pDev ;
  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);

  if(M2R_IS_SONET_MODE(port_id) == TRUE) {
    return (CS_OK) ;
  }

  m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_ETH,
                  M2R_GRP_ETH_MP_Interrupt,
                  &m2r_eth_irq_tbl[M2R_GRP_ETH_MP_Interrupt],
		  m2r_eth_irq_grp_name_tbl[M2R_GRP_ETH_MP_Interrupt],
		  M2R_GRP_ETH_MP_Interrupt_irq_id_name_tbl,
		  sizeof(M2R_GRP_ETH_MP_Interrupt_irq_id_name_tbl) /
		  sizeof(m2r_irq_id_name_t)) ;

  m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_ETH,
                  M2R_GRP_ETH_TX_Interrupt,
		  &m2r_eth_irq_tbl[M2R_GRP_ETH_TX_Interrupt],
		  m2r_eth_irq_grp_name_tbl[M2R_GRP_ETH_TX_Interrupt],
		  M2R_GRP_ETH_TX_Interrupt_irq_id_name_tbl,
		  sizeof(M2R_GRP_ETH_TX_Interrupt_irq_id_name_tbl) /
		  sizeof(m2r_irq_id_name_t)) ;

  m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_ETH,
                  M2R_GRP_ETH_RX_Interrupt_0,
		  &m2r_eth_irq_tbl[M2R_GRP_ETH_RX_Interrupt_0],
		  m2r_eth_irq_grp_name_tbl[M2R_GRP_ETH_RX_Interrupt_0],
		  M2R_GRP_ETH_RX_Interrupt_0_irq_id_name_tbl,
		  sizeof(M2R_GRP_ETH_RX_Interrupt_0_irq_id_name_tbl) /
		  sizeof(m2r_irq_id_name_t)) ;


/* Feature 1 */

  return (CS_OK) ;
}

