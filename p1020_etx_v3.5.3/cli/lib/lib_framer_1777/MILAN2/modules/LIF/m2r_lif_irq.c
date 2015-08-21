/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_lif_irq
 *
 * LIF IRQ Tables and APIs
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */

#include "cs_types.h"

#include "m2r_error.h"
#include "m2r_public.h"
#include "m2r_common.h"
#include "m2r_registers.h"
#include "m2r_lif.h"
#include "m2r_lif_irq.h"
#include "m2r_irq_priv.h"

/************************************************************************/
/*    IRQ table                                                         */
/************************************************************************/
const m2r_irq_ctl_t  m2r_lif_irq_tbl[M2R_LIF_MAX_GRP] = {
  CS_DI([M2R_GRP_LIF_INTERRUPT1] =) {
                         M2R_LIF_REG_OFFSET(INTERRUPT1),
                         M2R_LIF_REG_OFFSET(STATUS1),
                         M2R_LIF_REG_OFFSET(MASK1),
                         M2R_GRP_LIF_INTERRUPT1_ALL,
                         0,
                         M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_LIF_INTERRUPT2] =) {
                         M2R_LIF_REG_OFFSET(INTERRUPT2),
                         M2R_LIF_REG_OFFSET(STATUS2),
                         M2R_LIF_REG_OFFSET(MASK2),
                         M2R_GRP_LIF_INTERRUPT2_ALL,
                         0,
                         M2R_IRQ_FLG_RW1C },

   /* PCS6 IRQ */
  CS_DI([M2R_GRP_LIF_PCS6_TX_TxInt] =) {
      M2R_LIF_PCS_REG_OFFSET(TxInt),
      M2R_LIF_PCS_REG_OFFSET(TxSta),
      M2R_LIF_PCS_REG_OFFSET(TxIntMask),
      M2R_GRP_LIF_PCS6_TX_TxInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_LIF_PCS6_RX_RxInt] =) {
      M2R_LIF_PCS_REG_OFFSET(RxInt),
      M2R_LIF_PCS_REG_OFFSET(RxStat),
      M2R_LIF_PCS_REG_OFFSET(RxIntMask),
      M2R_GRP_LIF_PCS6_RX_RxInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  /* LIF XGXS IRQ */
  CS_DI([M2R_GRP_LIF_XGXS_MP_MnInt0] =) {
      M2R_LIF_XGXS_REG_OFFSET(MnInt0),
      M2R_LIF_XGXS_REG_OFFSET(MnSta0),
      M2R_LIF_XGXS_REG_OFFSET(MnIntMask0),
      M2R_GRP_LIF_XGXS_MP_MnInt0_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_LIF_XGXS_MP_MnInt1] =) {
      M2R_LIF_XGXS_REG_OFFSET(MnInt1),
      M2R_LIF_XGXS_REG_OFFSET(MnSta1),
      M2R_LIF_XGXS_REG_OFFSET(MnIntMask1),
      M2R_GRP_LIF_XGXS_MP_MnInt1_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_LIF_XGXS_TX_TxInt]  =) {
      M2R_LIF_XGXS_REG_OFFSET(TxInt),
      M2R_LIF_XGXS_REG_OFFSET(TxSta),
      M2R_LIF_XGXS_REG_OFFSET(TxIntMask),
      M2R_GRP_LIF_XGXS_TX_TxInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_LIF_XGXS_RX_RxInt]  =) {
      M2R_LIF_XGXS_REG_OFFSET(RxInt),
      M2R_LIF_XGXS_REG_OFFSET(RxSta),
      M2R_LIF_XGXS_REG_OFFSET(RxIntMask),
      M2R_GRP_LIF_XGXS_RX_RxInt_ALL, 0, M2R_IRQ_FLG_RW1C }
} ;


const char * m2r_lif_irq_grp_name_tbl[M2R_LIF_MAX_GRP] =
{
  CS_DI([M2R_GRP_LIF_INTERRUPT1]     =) "LIF_INTERRUPT1",
  CS_DI([M2R_GRP_LIF_INTERRUPT2]     =) "LIF_INTERRUPT2",
  CS_DI([M2R_GRP_LIF_PCS6_TX_TxInt]  =) "PCS6_TX_TxInt",
  CS_DI([M2R_GRP_LIF_PCS6_RX_RxInt]  =) "PCS6_RX_RxInt",
  CS_DI([M2R_GRP_LIF_XGXS_MP_MnInt0] =) "LIF_XGXS_MP_MnInt0",
  CS_DI([M2R_GRP_LIF_XGXS_MP_MnInt1] =) "LIF_XGXS_MP_MnInt1",
  CS_DI([M2R_GRP_LIF_XGXS_TX_TxInt]  =) "LIF_XGXS_TX_TxInt",
  CS_DI([M2R_GRP_LIF_XGXS_RX_RxInt]  =) "LIF_XGXS_RX_RxInt"
} ;


/**************** IRQ ID NAME TABLES **********************/
const m2r_irq_id_name_t
M2R_GRP_LIF_INTERRUPT1_irq_id_name_tbl[] = {
  {M2R_LIF_INTERRUPT1_DATA_DT0,         "DATA_DT0"},
  {M2R_LIF_INTERRUPT1_DATA_DT1,         "DATA_DT1"},
  {M2R_LIF_INTERRUPT1_DATA_DT2,         "DATA_DT2"},
  {M2R_LIF_INTERRUPT1_DATA_DT3,         "DATA_DT3"},
  {M2R_LIF_INTERRUPT1_SYNC0,    	"SYNC0"},
  {M2R_LIF_INTERRUPT1_SYNC1,    	"SYNC1"},
  {M2R_LIF_INTERRUPT1_SYNC2,    	"SYNC2"},
  {M2R_LIF_INTERRUPT1_SYNC3,    	"SYNC3"},
  {M2R_LIF_INTERRUPT1_BAD_VCOCLK0,      "BAD_VCOCLK0"},
  {M2R_LIF_INTERRUPT1_BAD_VCOCLK1,      "BAD_VCOCLK1"},
  {M2R_LIF_INTERRUPT1_BAD_VCOCLK2,      "BAD_VCOCLK2"},
  {M2R_LIF_INTERRUPT1_BAD_VCOCLK3,      "BAD_VCOCLK3"},
  {M2R_LIF_INTERRUPT1_LOCK0,    	"LOCK0"},
  {M2R_LIF_INTERRUPT1_LOCK1,    	"LOCK1"},
  {M2R_LIF_INTERRUPT1_LOCK2,    	"LOCK2"},
  {M2R_LIF_INTERRUPT1_LOCK3,    	"LOCK3"}
} ;

const m2r_irq_id_name_t
M2R_GRP_LIF_INTERRUPT2_irq_id_name_tbl[] = {
  {M2R_LIF_INTERRUPT2_SFI41PERR,        "SFI41PERR"},
  {M2R_LIF_INTERRUPT2_PRBSCNT31,        "PRBSCNT31"},
  {M2R_LIF_INTERRUPT2_PRBSERR,  	"PRBSERR"},
  {M2R_LIF_INTERRUPT2_XGDATA,   	"XGDATA"},
  {M2R_LIF_INTERRUPT2_XGBAD_VCOCLK,     "XGBAD_VCOCLK"},
  {M2R_LIF_INTERRUPT2_XGLOCK,   	"XGLOCK"},
  {M2R_LIF_INTERRUPT2_TXCK_MISS,        "TXCK_MISS"},
  {M2R_LIF_INTERRUPT2_RXCK_MISS,        "RXCK_MISS"},
  {M2R_LIF_INTERRUPT2_TxLnPAI,  	"TxLnPAI"},
  {M2R_LIF_INTERRUPT2_TxLBPAI,  	"TxLBPAI"},
  {M2R_LIF_INTERRUPT2_RxPCPAI,  	"RxPCPAI"},
  {M2R_LIF_INTERRUPT2_RxLBPAI,  	"RxLBPAI"},
  {M2R_LIF_INTERRUPT2_RxL3PAI,  	"RxL3PAI"},
  {M2R_LIF_INTERRUPT2_RxL2PAI,  	"RxL2PAI"},
  {M2R_LIF_INTERRUPT2_RxL1PAI,  	"RxL1PAI"},
  {M2R_LIF_INTERRUPT2_RxL0PAI,  	"RxL0PAI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_LIF_PCS6_TX_TxInt_irq_id_name_tbl[] = {
  {M2R_LIF_PCS6_TX_TxInt_GoodPktSatI,   "GoodPktSatI"},
  {M2R_LIF_PCS6_TX_TxInt_ErrorPktSatI,  "ErrorPktSatI"},
  {M2R_LIF_PCS6_TX_TxInt_ErrorPktI,     "ErrorPktI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_LIF_PCS6_RX_RxInt_irq_id_name_tbl[] = {
  {M2R_LIF_PCS6_RX_RxInt_PatSyncI,      "PatSyncI"},
  {M2R_LIF_PCS6_RX_RxInt_RxSOSI,        "RxSOSI"},
  {M2R_LIF_PCS6_RX_RxInt_BerHighI,      "BerHighI"},
  {M2R_LIF_PCS6_RX_RxInt_SyncDetI,      "SyncDetI"},
  {M2R_LIF_PCS6_RX_RxInt_ErrorBERSatI,  "ErrorBERSatI"},
  {M2R_LIF_PCS6_RX_RxInt_ErrorPatSatI,  "ErrorPatSatI"},
  {M2R_LIF_PCS6_RX_RxInt_ErrorPatI,     "ErrorPatI"},
  {M2R_LIF_PCS6_RX_RxInt_ErrorDecSatI,  "ErrorDecSatI"},
  {M2R_LIF_PCS6_RX_RxInt_ErrorDecI,     "ErrorDecI"},
  {M2R_LIF_PCS6_RX_RxInt_GoodPktSatI,   "GoodPktSatI"},
  {M2R_LIF_PCS6_RX_RxInt_ErrorPktSatI,  "ErrorPktSatI"},
  {M2R_LIF_PCS6_RX_RxInt_ErrorPktI,     "ErrorPktI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_LIF_XGXS_MP_MnInt0_irq_id_name_tbl[] = {
  {M2R_LIF_XGXS_MP_MnInt0_LckDet3I,     "LckDet3I"},
  {M2R_LIF_XGXS_MP_MnInt0_LckDet2I,     "LckDet2I"},
  {M2R_LIF_XGXS_MP_MnInt0_LckDet1I,     "LckDet1I"},
  {M2R_LIF_XGXS_MP_MnInt0_LckDet0I,     "LckDet0I"},
  {M2R_LIF_XGXS_MP_MnInt0_DatDet3I,     "DatDet3I"},
  {M2R_LIF_XGXS_MP_MnInt0_DatDet2I,     "DatDet2I"},
  {M2R_LIF_XGXS_MP_MnInt0_DatDet1I,     "DatDet1I"},
  {M2R_LIF_XGXS_MP_MnInt0_DatDet0I,     "DatDet0I"},
  {M2R_LIF_XGXS_MP_MnInt0_NoClk3I,      "NoClk3I"},
  {M2R_LIF_XGXS_MP_MnInt0_NoClk2I,      "NoClk2I"},
  {M2R_LIF_XGXS_MP_MnInt0_NoClk1I,      "NoClk1I"},
  {M2R_LIF_XGXS_MP_MnInt0_NoClk0I,      "NoClk0I"}
} ;

const m2r_irq_id_name_t
M2R_GRP_LIF_XGXS_MP_MnInt1_irq_id_name_tbl[] = {
  {M2R_LIF_XGXS_MP_MnInt1_RxNoClkI,     "RxNoClkI"},
  {M2R_LIF_XGXS_MP_MnInt1_TxNoClkI,     "TxNoClkI"},
  {M2R_LIF_XGXS_MP_MnInt1_RxPCPAI,      "RxPCPAI"},
  {M2R_LIF_XGXS_MP_MnInt1_RxLBPAI,      "RxLBPAI"},
  {M2R_LIF_XGXS_MP_MnInt1_TxLnPAI,      "TxLnPAI"},
  {M2R_LIF_XGXS_MP_MnInt1_TxLBPAI,      "TxLBPAI"},
  {M2R_LIF_XGXS_MP_MnInt1_RxL3PAI,      "RxL3PAI"},
  {M2R_LIF_XGXS_MP_MnInt1_RxL2PAI,      "RxL2PAI"},
  {M2R_LIF_XGXS_MP_MnInt1_RxL1PAI,      "RxL1PAI"},
  {M2R_LIF_XGXS_MP_MnInt1_RxL0PAI,      "RxL0PAI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_LIF_XGXS_TX_TxInt_irq_id_name_tbl[] = {
  {M2R_LIF_XGXS_TX_TxInt_GoodPktSatI,   "GoodPktSatI"},
  {M2R_LIF_XGXS_TX_TxInt_ErrorPktSatI,  "ErrorPktSatI"},
  {M2R_LIF_XGXS_TX_TxInt_ErrorPktI,     "ErrorPktI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_LIF_XGXS_RX_RxInt_irq_id_name_tbl[] = {
  {M2R_LIF_XGXS_RX_RxInt_PatLockDetI,   "PatLockDetI"},
  {M2R_LIF_XGXS_RX_RxInt_AlignDetI,     "AlignDetI"},
  {M2R_LIF_XGXS_RX_RxInt_SyncDet3I,     "SyncDet3I"},
  {M2R_LIF_XGXS_RX_RxInt_SyncDet2I,     "SyncDet2I"},
  {M2R_LIF_XGXS_RX_RxInt_SyncDet1I,     "SyncDet1I"},
  {M2R_LIF_XGXS_RX_RxInt_SyncDet0I,     "SyncDet0I"},
  {M2R_LIF_XGXS_RX_RxInt_PatErrSatI,    "PatErrSatI"},
  {M2R_LIF_XGXS_RX_RxInt_PatErrI,       "PatErrI"},
  {M2R_LIF_XGXS_RX_RxInt_InvCodeSatI,   "InvCodeSatI"},
  {M2R_LIF_XGXS_RX_RxInt_InvCodeI,      "InvCodeI"},
  {M2R_LIF_XGXS_RX_RxInt_GoodPktSatI,   "GoodPktSatI"},
  {M2R_LIF_XGXS_RX_RxInt_ErrorPktSatI,  "ErrorPktSatI"},
  {M2R_LIF_XGXS_RX_RxInt_ErrorPktI,     "ErrorPktI"}
} ;




/************************************************************************
 * GET DEFAULT LIF IRQ CONFIGURATION
 ***********************************************************************/
cs_status m2r_lif_get_default_irq_cfg(cs_uint16 port_id,
                                      m2r_port_summ_t * pSumm,
                                      m2r_lif_cfg_t * pCfg)
{
  cs_boolean  sonet_mode ;
  cs_uint16   ii ;

  /* Clear the default IRQ table */
  for (ii = 0; ii < M2R_LIF_MAX_GRP; ii++) {
    pCfg->irq_ctl_tbl[ii] = 0 ;
  }

  sonet_mode = ((pSumm->line_rate == M2R_PORT_RATE_10G) ? FALSE : TRUE ) ;

  switch ( pSumm->lif_mode ) {
    case M2R_PORT_IF_SFI4_1 :
      pCfg->irq_ctl_tbl[M2R_GRP_LIF_INTERRUPT2] =
                            ( M2R_LIF_INTERRUPT2_PRBSCNT31  |
                              M2R_LIF_INTERRUPT2_PRBSERR    |
                              M2R_LIF_INTERRUPT2_TXCK_MISS  |
                              M2R_LIF_INTERRUPT2_RXCK_MISS  ) ;

      if ( sonet_mode == FALSE ) { /* 10 GIG Ethernet Mode, PCS is ON */
        pCfg->irq_ctl_tbl[M2R_GRP_LIF_PCS6_TX_TxInt] =
                                M2R_GRP_LIF_PCS6_TX_TxInt_ALL ;
        pCfg->irq_ctl_tbl[M2R_GRP_LIF_PCS6_RX_RxInt] =
                                M2R_GRP_LIF_PCS6_RX_RxInt_ALL ;
      }
    break ;

    case M2R_PORT_IF_SFI4_2 :
      /* Set the default IRQ to be enabled */
      pCfg->irq_ctl_tbl[M2R_GRP_LIF_INTERRUPT1] =
                                        M2R_GRP_LIF_INTERRUPT1_ALL ;

      pCfg->irq_ctl_tbl[M2R_GRP_LIF_INTERRUPT2] =
                            ( M2R_LIF_INTERRUPT2_TXCK_MISS  |
                              M2R_LIF_INTERRUPT2_RXCK_MISS  ) ;

      if (sonet_mode == FALSE) { /* XAUI mode */
        pCfg->irq_ctl_tbl[M2R_GRP_LIF_XGXS_MP_MnInt0] =
                                M2R_GRP_LIF_XGXS_MP_MnInt0_ALL ;
        pCfg->irq_ctl_tbl[M2R_GRP_LIF_XGXS_MP_MnInt1] =
                                M2R_GRP_LIF_XGXS_MP_MnInt1_ALL ;
        pCfg->irq_ctl_tbl[M2R_GRP_LIF_XGXS_TX_TxInt]  =
                                M2R_GRP_LIF_XGXS_TX_TxInt_ALL ;
        pCfg->irq_ctl_tbl[M2R_GRP_LIF_XGXS_RX_RxInt]  =
                                M2R_GRP_LIF_XGXS_RX_RxInt_ALL ;
      }
    break ;

    case M2R_PORT_IF_XFP :
      /* Set the default IRQ to be enabled */
      pCfg->irq_ctl_tbl[M2R_GRP_LIF_INTERRUPT2] =
                   ( M2R_LIF_INTERRUPT2_XGDATA      |
                     M2R_LIF_INTERRUPT2_XGLOCK      |
                     M2R_LIF_INTERRUPT2_TXCK_MISS   |
                     M2R_LIF_INTERRUPT2_RXCK_MISS   |
                     M2R_LIF_INTERRUPT2_XGBAD_VCOCLK ) ;
      if ( sonet_mode == FALSE ) { /* 10 GIG Ethernet Mode, PCS is ON */
        pCfg->irq_ctl_tbl[M2R_GRP_LIF_PCS6_TX_TxInt] =
                                M2R_GRP_LIF_PCS6_TX_TxInt_ALL ;
        pCfg->irq_ctl_tbl[M2R_GRP_LIF_PCS6_RX_RxInt] =
                                M2R_GRP_LIF_PCS6_RX_RxInt_ALL ;
      }
    break ;
  }

  return (CS_OK) ;
}

/************************************************************************
 * INIT LIF IRQ
 ***********************************************************************/
cs_status m2r_lif_init_irq(cs_uint16 port_id, m2r_lif_cfg_t * pCfg)
{
  cs_uint32     ii ;

  /* Register LIF IRQ table. */
  M2R_REGISTER_IRQ_TBL(port_id, M2R_ID_LIF, m2r_lif_irq_tbl,
                       m2r_lif_irq_grp_name_tbl, M2R_LIF_MAX_GRP, NULL) ;

  for(ii = 0; ii < M2R_LIF_MAX_GRP; ii++) {
    if (pCfg->irq_ctl_tbl[ii]) {
      /* Clear any interrupts */
      m2r_clear_leaf_irq(port_id, M2R_ID_LIF, ii,
                         m2r_lif_irq_tbl[ii].irq_map) ;

      /* Enable The IRQ requested to be enabled by default */
      m2r_enable_leaf_irq(port_id, M2R_ID_LIF, ii,
                          pCfg->irq_ctl_tbl[ii]) ;
    }
  }
  return (CS_OK) ;
}

/************************************************************************/
/*                  ISR ROUTINE FOR LIF BLOCK                           */
/************************************************************************/
cs_status m2r_lif_isr(cs_uint16 port_id)
{
  M2R_t              * pDev ;
  LIF_INT_SOURCE_t     lif_irq_src;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);

  lif_irq_src.wrd = M2R_REG_READ(&pDev->LIF.INT_SOURCE.wrd);

  if (lif_irq_src.bf.LIF_INT) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_LIF,
                  M2R_GRP_LIF_INTERRUPT1,
                  &m2r_lif_irq_tbl[M2R_GRP_LIF_INTERRUPT1],
		  m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_INTERRUPT1],
		  M2R_GRP_LIF_INTERRUPT1_irq_id_name_tbl,
		  sizeof(M2R_GRP_LIF_INTERRUPT1_irq_id_name_tbl) /
		  sizeof(m2r_irq_id_name_t)) ;

    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_LIF,
                  M2R_GRP_LIF_INTERRUPT2,
                  &m2r_lif_irq_tbl[M2R_GRP_LIF_INTERRUPT2],
		  m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_INTERRUPT2],
		  M2R_GRP_LIF_INTERRUPT2_irq_id_name_tbl,
		  sizeof(M2R_GRP_LIF_INTERRUPT2_irq_id_name_tbl) /
		  sizeof(m2r_irq_id_name_t)) ;
  }

  if (lif_irq_src.bf.XGXS_INT) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_LIF,
                  M2R_GRP_LIF_XGXS_MP_MnInt0,
                  &m2r_lif_irq_tbl[M2R_GRP_LIF_XGXS_MP_MnInt0],
		  m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_XGXS_MP_MnInt0],
		  M2R_GRP_LIF_XGXS_MP_MnInt0_irq_id_name_tbl,
		  sizeof(M2R_GRP_LIF_XGXS_MP_MnInt0_irq_id_name_tbl) /
		  sizeof(m2r_irq_id_name_t)) ;

    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_LIF,
                  M2R_GRP_LIF_XGXS_MP_MnInt1,
                  &m2r_lif_irq_tbl[M2R_GRP_LIF_XGXS_MP_MnInt1],
		  m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_XGXS_MP_MnInt1],
		  M2R_GRP_LIF_XGXS_MP_MnInt1_irq_id_name_tbl,
		  sizeof(M2R_GRP_LIF_XGXS_MP_MnInt1_irq_id_name_tbl) /
		  sizeof(m2r_irq_id_name_t)) ;

    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_LIF,
                  M2R_GRP_LIF_XGXS_TX_TxInt,
                  &m2r_lif_irq_tbl[M2R_GRP_LIF_XGXS_TX_TxInt],
		  m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_XGXS_TX_TxInt],
		  M2R_GRP_LIF_XGXS_TX_TxInt_irq_id_name_tbl,
		  sizeof(M2R_GRP_LIF_XGXS_TX_TxInt_irq_id_name_tbl) /
		  sizeof(m2r_irq_id_name_t)) ;

    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_LIF,
                  M2R_GRP_LIF_XGXS_RX_RxInt,
		  &m2r_lif_irq_tbl[M2R_GRP_LIF_XGXS_RX_RxInt],
		  m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_XGXS_RX_RxInt],
		  M2R_GRP_LIF_XGXS_RX_RxInt_irq_id_name_tbl,
		  sizeof(M2R_GRP_LIF_XGXS_RX_RxInt_irq_id_name_tbl) /
		  sizeof(m2r_irq_id_name_t)) ;
  }

  if (lif_irq_src.bf.PCS_INT) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_LIF,
                  M2R_GRP_LIF_PCS6_TX_TxInt,
                  &m2r_lif_irq_tbl[M2R_GRP_LIF_PCS6_TX_TxInt],
		  m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_PCS6_TX_TxInt],
		  M2R_GRP_LIF_PCS6_TX_TxInt_irq_id_name_tbl,
		  sizeof(M2R_GRP_LIF_PCS6_TX_TxInt_irq_id_name_tbl) /
		  sizeof(m2r_irq_id_name_t)) ;

    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_LIF,
                  M2R_GRP_LIF_PCS6_RX_RxInt,
                  &m2r_lif_irq_tbl[M2R_GRP_LIF_PCS6_RX_RxInt],
		  m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_PCS6_RX_RxInt],
		  M2R_GRP_LIF_PCS6_RX_RxInt_irq_id_name_tbl,
		  sizeof(M2R_GRP_LIF_PCS6_RX_RxInt_irq_id_name_tbl) /
		  sizeof(m2r_irq_id_name_t)) ;
  }

  return(CS_OK) ;
}


cs_status m2r_lif_dump_irq(cs_uint16 port_id)
{
  cs_boolean  sonet_mode ;
  cs_uint64 dev_addr = (cs_uint64) M2R_PORT_ID_TO_DEV_BASE(port_id);
  m2r_port_summ_t * pSumm = &(M2R_PORT_ID_TO_PCB_PTR(port_id)->summ) ;

  sonet_mode = M2R_IS_SONET_MODE(port_id) ;

  CS_PRINT("%-30s ", m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_INTERRUPT1]) ;
  m2r_irq_dump_tbl_entry(port_id, dev_addr,
                         &m2r_lif_irq_tbl[M2R_GRP_LIF_INTERRUPT1]) ;
  CS_PRINT("%-30s ", m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_INTERRUPT2]) ;
  m2r_irq_dump_tbl_entry(port_id, dev_addr,
                         &m2r_lif_irq_tbl[M2R_GRP_LIF_INTERRUPT2]) ;

  if ( sonet_mode == FALSE ) { /* 10 GIG Ethernet Mode, PCS is ON */
    switch ( pSumm->lif_mode ) {
      case M2R_PORT_IF_SFI4_1 :
      case M2R_PORT_IF_XFP :
        CS_PRINT("%-30s ", m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_PCS6_TX_TxInt]) ;
	m2r_irq_dump_tbl_entry(port_id, dev_addr,
                                 &m2r_lif_irq_tbl[M2R_GRP_LIF_PCS6_TX_TxInt]) ;
        CS_PRINT("%-30s ", m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_PCS6_RX_RxInt]) ;
	m2r_irq_dump_tbl_entry(port_id, dev_addr,
                                 &m2r_lif_irq_tbl[M2R_GRP_LIF_PCS6_RX_RxInt]) ;
      break ;

      case M2R_PORT_IF_SFI4_2 :
        CS_PRINT("%-30s ", m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_XGXS_MP_MnInt0]) ;
	m2r_irq_dump_tbl_entry(port_id, dev_addr,
                              &m2r_lif_irq_tbl[M2R_GRP_LIF_XGXS_MP_MnInt0]) ;
        CS_PRINT("%-30s ", m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_XGXS_MP_MnInt1]) ;
	m2r_irq_dump_tbl_entry(port_id, dev_addr,
                              &m2r_lif_irq_tbl[M2R_GRP_LIF_XGXS_MP_MnInt1]) ;
        CS_PRINT("%-30s ", m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_XGXS_TX_TxInt]) ;
        m2r_irq_dump_tbl_entry(port_id, dev_addr,
                              &m2r_lif_irq_tbl[M2R_GRP_LIF_XGXS_TX_TxInt]) ;
        CS_PRINT("%-30s ", m2r_lif_irq_grp_name_tbl[M2R_GRP_LIF_XGXS_RX_RxInt]) ;
        m2r_irq_dump_tbl_entry(port_id, dev_addr,
                              &m2r_lif_irq_tbl[M2R_GRP_LIF_XGXS_RX_RxInt]) ;
      break ;
    }
  }

  return (CS_OK) ;
}


