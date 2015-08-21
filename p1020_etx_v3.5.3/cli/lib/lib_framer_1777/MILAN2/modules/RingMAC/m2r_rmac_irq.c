/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_rmac_irq.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */

#include "cs_types.h"
#include "cs_rtos.h"

#include "m2r_error.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_irq_priv.h"

#include "m2r_rmac_priv.h"
#include "m2r_rmac_irq.h"

#define M2R_RMAC_IRQ_PORT_FLAGS    ( M2R_IRQ_FLG_RW1C |         \
                                     M2R_IRQ_FLG_1_TO_ENABLE )

/************************************************************************/
/*    IRQ table                                                         */
/************************************************************************/
const m2r_irq_ctl_t  m2r_rmac_irq_tbl[M2R_RMAC_MAX_GRP] = {
  CS_DI([M2R_GRP_RMAC_MICRO_IF_IRQ0]          =) {
        M2R_RMAC_REG_OFST(IRQ0),
        M2R_RMAC_REG_OFST(IRQ_STAT0),
        M2R_RMAC_REG_OFST(IRQ_ENAB0),
        M2R_GRP_RMAC_MICRO_IF_IRQ0_ALL, M2R_GRP_RMAC_MICRO_IF_IRQ0_SPL,
        M2R_RMAC_IRQ_PORT_FLAGS | M2R_IRQ_FLG_SPL_CLR },

  CS_DI([M2R_GRP_RMAC_MICRO_IF_IRQ1]          =) {
        M2R_RMAC_REG_OFST(IRQ1),
        M2R_RMAC_REG_OFST(IRQ_STAT1),
        M2R_RMAC_REG_OFST(IRQ_ENAB1),
        M2R_GRP_RMAC_MICRO_IF_IRQ1_ALL, 0,
        M2R_RMAC_IRQ_PORT_FLAGS },

  CS_DI([M2R_GRP_RMAC_MICRO_IF_IRQ2]          =) {
        M2R_RMAC_REG_OFST(IRQ2),
        M2R_RMAC_REG_OFST(IRQ_STAT2),
        M2R_RMAC_REG_OFST(IRQ_ENAB2),
        M2R_GRP_RMAC_MICRO_IF_IRQ2_ALL, M2R_GRP_RMAC_MICRO_IF_IRQ2_SPL,
        M2R_RMAC_IRQ_PORT_FLAGS | M2R_IRQ_FLG_SPL_INT |
        M2R_IRQ_FLG_SPL_CLR },

  CS_DI([M2R_GRP_RMAC_MICRO_IF_IRQ3]          =) {
        M2R_RMAC_REG_OFST(IRQ3),
        M2R_RMAC_REG_OFST(IRQ_STAT3),
        M2R_RMAC_REG_OFST(IRQ_ENAB3),
        M2R_GRP_RMAC_MICRO_IF_IRQ3_ALL, 0,
        M2R_RMAC_IRQ_PORT_FLAGS },

  CS_DI([M2R_GRP_RMAC_MICRO_IF_IRQ4]          =) {
        M2R_RMAC_REG_OFST(IRQ4),
        M2R_RMAC_REG_OFST(IRQ_STAT4),
        M2R_RMAC_REG_OFST(IRQ_ENAB4),
        M2R_GRP_RMAC_MICRO_IF_IRQ4_ALL, 0,
        M2R_RMAC_IRQ_PORT_FLAGS },

  CS_DI([M2R_GRP_RMAC_MICRO_IF_IRQ5]          =) {
        M2R_RMAC_REG_OFST(IRQ5),
        M2R_RMAC_REG_OFST(IRQ_STAT5),
        M2R_RMAC_REG_OFST(IRQ_ENAB5),
        M2R_GRP_RMAC_MICRO_IF_IRQ5_ALL, M2R_GRP_RMAC_MICRO_IF_IRQ5_SPL,
        (M2R_RMAC_IRQ_PORT_FLAGS | M2R_IRQ_FLG_SPL_INT |
	 M2R_IRQ_FLG_SPL_CLR) }
} ;


const char * m2r_rmac_irq_grp_name_tbl[M2R_RMAC_MAX_GRP] =
{
  CS_DI([M2R_GRP_RMAC_MICRO_IF_IRQ0] =) "DR_MICRO_IF_IRQ0",
  CS_DI([M2R_GRP_RMAC_MICRO_IF_IRQ1] =) "DR_MICRO_IF_IRQ1",
  CS_DI([M2R_GRP_RMAC_MICRO_IF_IRQ2] =) "DR_MICRO_IF_IRQ2",
  CS_DI([M2R_GRP_RMAC_MICRO_IF_IRQ3] =) "DR_MICRO_IF_IRQ3",
  CS_DI([M2R_GRP_RMAC_MICRO_IF_IRQ4] =) "DR_MICRO_IF_IRQ4",
  CS_DI([M2R_GRP_RMAC_MICRO_IF_IRQ5] =) "DR_MICRO_IF_IRQ5"
} ;


/**************** IRQ ID NAME TABLES **********************/
const m2r_irq_id_name_t
M2R_GRP_RMAC_MICRO_IF_IRQ0_irq_id_name_tbl[] = {
  {M2R_RMAC_MICRO_IF_IRQ0_RXH_OVERFLOW,         "RXH_OVERFLOW"},
  {M2R_RMAC_MICRO_IF_IRQ0_RXH_GIANT,    	"RXH_GIANT"},
  {M2R_RMAC_MICRO_IF_IRQ0_RXH_PKTDROP,  	"RXH_PKTDROP"},
  {M2R_RMAC_MICRO_IF_IRQ0_RXH_DROPSAT,  	"RXH_DROPSAT"},
  {M2R_RMAC_MICRO_IF_IRQ0_TXH_FIFO_OVFL,        "TXH_FIFO_OVFL"},
  {M2R_RMAC_MICRO_IF_IRQ0_TXH_CTRL_PERR,        "TXH_CTRL_PERR"},
  {M2R_RMAC_MICRO_IF_IRQ0_TXH_SRP_PERR,         "TXH_SRP_PERR"},
  {M2R_RMAC_MICRO_IF_IRQ0_TXH_SHRT_PKT,         "TXH_SHRT_PKT"},
  {M2R_RMAC_MICRO_IF_IRQ0_TXH_PKT_EMPTY,        "TXH_PKT_EMPTY"},
  {M2R_RMAC_MICRO_IF_IRQ0_TXH_PKT_GIANT,        "TXH_PKT_GIANT"},
  {M2R_RMAC_MICRO_IF_IRQ0_TXH_RUNT,     	"TXH_RUNT"},
  {M2R_RMAC_MICRO_IF_IRQ0_TXH_ERR,      	"TXH_ERR"},
  {M2R_RMAC_MICRO_IF_IRQ0_TXH_DPERR,    	"TXH_DPERR"}
} ;

const m2r_irq_id_name_t
M2R_GRP_RMAC_MICRO_IF_IRQ1_irq_id_name_tbl[] = {
  {M2R_RMAC_MICRO_IF_IRQ1_RX_TRAF,      	"RX_TRAF"},
  {M2R_RMAC_MICRO_IF_IRQ1_RX_TIMER,     	"RX_TIMER"},
  {M2R_RMAC_MICRO_IF_IRQ1_RX_CRC_ERR,   	"RX_CRC_ERR"},
  {M2R_RMAC_MICRO_IF_IRQ1_RX_HEC_ERR,   	"RX_HEC_ERR"},
  {M2R_RMAC_MICRO_IF_IRQ1_RXF_IDLE_SA,  	"RXF_IDLE_SA"},
  {M2R_RMAC_MICRO_IF_IRQ1_RXF_FIFO_OVFL,        "RXF_FIFO_OVFL"},
  {M2R_RMAC_MICRO_IF_IRQ1_RXF_CTRL_PERR,        "RXF_CTRL_PERR"},
  {M2R_RMAC_MICRO_IF_IRQ1_RXF_SRP_PERR,         "RXF_SRP_PERR"},
  {M2R_RMAC_MICRO_IF_IRQ1_RXF_SHRT_PKT,         "RXF_SHRT_PKT"},
  {M2R_RMAC_MICRO_IF_IRQ1_RXF_PKT_EMPTY,        "RXF_PKT_EMPTY"},
  {M2R_RMAC_MICRO_IF_IRQ1_RXF_PKT_GIANT,        "RXF_PKT_GIANT"},
  {M2R_RMAC_MICRO_IF_IRQ1_RXF_RUNT,     	"RXF_RUNT"},
  {M2R_RMAC_MICRO_IF_IRQ1_RXF_ERR,      	"RXF_ERR"},
  {M2R_RMAC_MICRO_IF_IRQ1_RXF_DPERR,    	"RXF_DPERR"}
} ;

const m2r_irq_id_name_t
M2R_GRP_RMAC_MICRO_IF_IRQ2_irq_id_name_tbl[] = {
  {M2R_RMAC_MICRO_IF_IRQ2_LKPCAM_MULTIHIT,      "LKPCAM_MULTIHIT"},
  {M2R_RMAC_MICRO_IF_IRQ2_STATCAM_MULTIHIT,     "STATCAM_MULTIHIT"},
  {M2R_RMAC_MICRO_IF_IRQ2_USEQ_FAIR,    	"USEQ_FAIR"},
  {M2R_RMAC_MICRO_IF_IRQ2_BIST_ERR,     	"BIST_ERR"},
  {M2R_RMAC_MICRO_IF_IRQ2_MAC_PERR,     	"MAC_PERR"},
  {M2R_RMAC_MICRO_IF_IRQ2_HW_RESET,     	"HW_RESET"},
  {M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT,     	"USEQ_INT"},
  {M2R_RMAC_MICRO_IF_IRQ2_WDOG,         	"WDOG"},
  {M2R_RMAC_MICRO_IF_IRQ2_WPASS,        	"WPASS"},
  {M2R_RMAC_MICRO_IF_IRQ2_FPASS,        	"FPASS"},
  {M2R_RMAC_MICRO_IF_IRQ2_SPSAT,        	"SPSAT"},
  {M2R_RMAC_MICRO_IF_IRQ2_SBSAT,        	"SBSAT"},
  {M2R_RMAC_MICRO_IF_IRQ2_LPSAT,        	"LPSAT"},
  {M2R_RMAC_MICRO_IF_IRQ2_LBSAT,        	"LBSAT"},
  {M2R_RMAC_MICRO_IF_IRQ2_TTLS,         	"TTLS"},
  {M2R_RMAC_MICRO_IF_IRQ2_WR2_LOCK,     	"WR2_LOCK"}
} ;

const m2r_irq_id_name_t
M2R_GRP_RMAC_MICRO_IF_IRQ3_irq_id_name_tbl[] = {
  {M2R_RMAC_MICRO_IF_IRQ3_IPS_RI_TTL_MAX,       "IPS_RI_TTL_MAX"},
  {M2R_RMAC_MICRO_IF_IRQ3_EXTTB_DPERR,  	"EXTTB_DPERR"},
  {M2R_RMAC_MICRO_IF_IRQ3_EXTTB_OVFL,   	"EXTTB_OVFL"},
  {M2R_RMAC_MICRO_IF_IRQ3_PTQ_KILL_DONE,        "PTQ_KILL_DONE"},
  {M2R_RMAC_MICRO_IF_IRQ3_STQ_KILL_DONE,        "STQ_KILL_DONE"},
  {M2R_RMAC_MICRO_IF_IRQ3_TBD_IPS,      	"TBD_IPS"},
  {M2R_RMAC_MICRO_IF_IRQ3_RING_ERR,     	"RING_ERR"},
  {M2R_RMAC_MICRO_IF_IRQ3_LTBO,         	"LTBO"},
  {M2R_RMAC_MICRO_IF_IRQ3_HTBO,         	"HTBO"},
  {M2R_RMAC_MICRO_IF_IRQ3_TBD_RUNT_USG,         "TBD_RUNT_USG"},
  {M2R_RMAC_MICRO_IF_IRQ3_LTB_DPERR,    	"LTB_DPERR"},
  {M2R_RMAC_MICRO_IF_IRQ3_HTB_DPERR,    	"HTB_DPERR"},
  {M2R_RMAC_MICRO_IF_IRQ3_CTL_RCV_INT,  	"CTL_RCV_INT"},
  {M2R_RMAC_MICRO_IF_IRQ3_CTL_SND_INT,  	"CTL_SND_INT"},
  {M2R_RMAC_MICRO_IF_IRQ3_IPS_RCV_INT,  	"IPS_RCV_INT"},
  {M2R_RMAC_MICRO_IF_IRQ3_IPS_SND_INT,  	"IPS_SND_INT"}
} ;

const m2r_irq_id_name_t
M2R_GRP_RMAC_MICRO_IF_IRQ4_irq_id_name_tbl[] = {
  {M2R_RMAC_MICRO_IF_IRQ4_FRM_HEC_ERR,  	"FRM_HEC_ERR"},
  {M2R_RMAC_MICRO_IF_IRQ4_TOM_UNDERFLOW,        "TOM_UNDERFLOW"},
  {M2R_RMAC_MICRO_IF_IRQ4_MATE_WRAP,    	"MATE_WRAP"},
  {M2R_RMAC_MICRO_IF_IRQ4_FRM_CTRL_SQNC_ERR,    "FRM_CTRL_SQNC_ERR"},
  {M2R_RMAC_MICRO_IF_IRQ4_FRM_OVERFLOW,         "FRM_OVERFLOW"},
  {M2R_RMAC_MICRO_IF_IRQ4_FRM_UNDERFLOW,        "FRM_UNDERFLOW"},
  {M2R_RMAC_MICRO_IF_IRQ4_FRM_SERDES,   	"FRM_SERDES"},
  {M2R_RMAC_MICRO_IF_IRQ4_FRM_CRC_ERR,  	"FRM_CRC_ERR"},
  {M2R_RMAC_MICRO_IF_IRQ4_FRM_CRC_ERR_SAT,      "FRM_CRC_ERR_SAT"},
  {M2R_RMAC_MICRO_IF_IRQ4_FRM_GIANT,    	"FRM_GIANT"},
  {M2R_RMAC_MICRO_IF_IRQ4_FRM_GIANT_SAT,        "FRM_GIANT_SAT"},
  {M2R_RMAC_MICRO_IF_IRQ4_FRM_LOS,      	"FRM_LOS"},
  {M2R_RMAC_MICRO_IF_IRQ4_FRM_LOS_SAT,  	"FRM_LOS_SAT"},
  {M2R_RMAC_MICRO_IF_IRQ4_FRM_RUNT,     	"FRM_RUNT"},
  {M2R_RMAC_MICRO_IF_IRQ4_FRM_RUNT_SAT,         "FRM_RUNT_SAT"}
} ;

const m2r_irq_id_name_t
M2R_GRP_RMAC_MICRO_IF_IRQ5_irq_id_name_tbl[] = {
  {M2R_RMAC_MICRO_IF_IRQ5_TBD_WE,       	"TBD_WE"},
  {M2R_RMAC_MICRO_IF_IRQ5_TBD_NOIPS,    	"TBD_NOIPS"},
  {M2R_RMAC_MICRO_IF_IRQ5_RXF_MCASTSA,  	"RXF_MCASTSA"},
  {M2R_RMAC_MICRO_IF_IRQ5_DQTA_TO,      	"DQTA_TO"},
  {M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1,      	"MEMPAR1"},
  {M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0,      	"MEMPAR0"},
  {M2R_RMAC_MICRO_IF_IRQ5_NEW_TTL_CONGEST,      "NEW_TTL_CONGEST"},
  {M2R_RMAC_MICRO_IF_IRQ5_OLD_TSN_FLUSHED,      "OLD_TSN_FLUSHED"},
  {M2R_RMAC_MICRO_IF_IRQ5_MAC_PKTLEN_ERR,       "MAC_PKTLEN_ERR"},
  {M2R_RMAC_MICRO_IF_IRQ5_TBH_ECC,      	"TBH_ECC"},
  {M2R_RMAC_MICRO_IF_IRQ5_TBL_ECC,      	"TBL_ECC"},
  {M2R_RMAC_MICRO_IF_IRQ5_EXTTB_ECC,    	"EXTTB_ECC"}
} ;


/************************************************************************/
/*    XGXS IRQ TABLE                                                    */
/************************************************************************/
const m2r_irq_ctl_t  m2r_rmac_xgxs_irq_tbl[M2R_XGXS_MAX_GRP] =
{
  CS_DI([M2R_GRP_XGXS_MP_MnInt0] =) {
      M2R_XGXS_REG_OFFSET(MnInt0),
      M2R_XGXS_REG_OFFSET(MnSta0),
      M2R_XGXS_REG_OFFSET(MnIntMask0),
      M2R_GRP_XGXS_MP_MnInt0_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_XGXS_MP_MnInt1] =) {
      M2R_XGXS_REG_OFFSET(MnInt1),
      M2R_XGXS_REG_OFFSET(MnSta1),
      M2R_XGXS_REG_OFFSET(MnIntMask1),
      M2R_GRP_XGXS_MP_MnInt1_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_XGXS_TX_TxInt]  =) {
      M2R_XGXS_REG_OFFSET(TxInt),
      M2R_XGXS_REG_OFFSET(TxSta),
      M2R_XGXS_REG_OFFSET(TxIntMask),
      M2R_GRP_XGXS_TX_TxInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_XGXS_RX_RxInt]  =) {
      M2R_XGXS_REG_OFFSET(RxInt),
      M2R_XGXS_REG_OFFSET(RxSta),
      M2R_XGXS_REG_OFFSET(RxIntMask),
      M2R_GRP_XGXS_RX_RxInt_ALL, 0, M2R_IRQ_FLG_RW1C }
} ;


const char * m2r_rmac_xgxs_irq_grp_name_tbl[M2R_RMAC_MAX_GRP] =
{
  CS_DI([M2R_GRP_XGXS_MP_MnInt0]  =) "XGXS_MP_MnInt0",
  CS_DI([M2R_GRP_XGXS_MP_MnInt1]  =) "XGXS_MP_MnInt1",
  CS_DI([M2R_GRP_XGXS_TX_TxInt]   =) "XGXS_TX_TxInt",
  CS_DI([M2R_GRP_XGXS_RX_RxInt]   =) "XGXS_RX_RxInt"
} ;

/**************** IRQ ID NAME TABLES **********************/
const m2r_irq_id_name_t
M2R_GRP_XGXS_MP_MnInt0_irq_id_name_tbl[] = {
  {M2R_XGXS_MP_MnInt0_LckDet3I,         "LckDet3I"},
  {M2R_XGXS_MP_MnInt0_LckDet2I,         "LckDet2I"},
  {M2R_XGXS_MP_MnInt0_LckDet1I,         "LckDet1I"},
  {M2R_XGXS_MP_MnInt0_LckDet0I,         "LckDet0I"},
  {M2R_XGXS_MP_MnInt0_DatDet3I,         "DatDet3I"},
  {M2R_XGXS_MP_MnInt0_DatDet2I,         "DatDet2I"},
  {M2R_XGXS_MP_MnInt0_DatDet1I,         "DatDet1I"},
  {M2R_XGXS_MP_MnInt0_DatDet0I,         "DatDet0I"},
  {M2R_XGXS_MP_MnInt0_NoClk3I,  	"NoClk3I"},
  {M2R_XGXS_MP_MnInt0_NoClk2I,  	"NoClk2I"},
  {M2R_XGXS_MP_MnInt0_NoClk1I,  	"NoClk1I"},
  {M2R_XGXS_MP_MnInt0_NoClk0I,  	"NoClk0I"}
} ;

const m2r_irq_id_name_t
M2R_GRP_XGXS_MP_MnInt1_irq_id_name_tbl[] = {
  {M2R_XGXS_MP_MnInt1_RxNoClkI,         "RxNoClkI"},
  {M2R_XGXS_MP_MnInt1_TxNoClkI,         "TxNoClkI"},
  {M2R_XGXS_MP_MnInt1_RxPCPAI,  	"RxPCPAI"},
  {M2R_XGXS_MP_MnInt1_RxLBPAI,  	"RxLBPAI"},
  {M2R_XGXS_MP_MnInt1_TxLnPAI,  	"TxLnPAI"},
  {M2R_XGXS_MP_MnInt1_TxLBPAI,  	"TxLBPAI"},
  {M2R_XGXS_MP_MnInt1_RxL3PAI,  	"RxL3PAI"},
  {M2R_XGXS_MP_MnInt1_RxL2PAI,  	"RxL2PAI"},
  {M2R_XGXS_MP_MnInt1_RxL1PAI,  	"RxL1PAI"},
  {M2R_XGXS_MP_MnInt1_RxL0PAI,  	"RxL0PAI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_XGXS_TX_TxInt_irq_id_name_tbl[] = {
  {M2R_XGXS_TX_TxInt_GoodPktSatI,       "GoodPktSatI"},
  {M2R_XGXS_TX_TxInt_ErrorPktSatI,      "ErrorPktSatI"},
  {M2R_XGXS_TX_TxInt_ErrorPktI,         "ErrorPktI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_XGXS_RX_RxInt_irq_id_name_tbl[] = {
  {M2R_XGXS_RX_RxInt_PatLockDetI,       "PatLockDetI"},
  {M2R_XGXS_RX_RxInt_AlignDetI,         "AlignDetI"},
  {M2R_XGXS_RX_RxInt_SyncDet3I,         "SyncDet3I"},
  {M2R_XGXS_RX_RxInt_SyncDet2I,         "SyncDet2I"},
  {M2R_XGXS_RX_RxInt_SyncDet1I,         "SyncDet1I"},
  {M2R_XGXS_RX_RxInt_SyncDet0I,         "SyncDet0I"},
  {M2R_XGXS_RX_RxInt_PatErrSatI,        "PatErrSatI"},
  {M2R_XGXS_RX_RxInt_PatErrI,   	"PatErrI"},
  {M2R_XGXS_RX_RxInt_InvCodeSatI,       "InvCodeSatI"},
  {M2R_XGXS_RX_RxInt_InvCodeI,  	"InvCodeI"},
  {M2R_XGXS_RX_RxInt_GoodPktSatI,       "GoodPktSatI"},
  {M2R_XGXS_RX_RxInt_ErrorPktSatI,      "ErrorPktSatI"},
  {M2R_XGXS_RX_RxInt_ErrorPktI,         "ErrorPktI"}
} ;

/************************************************************************
 * RINGMAC IRQ SPECIAL HANDLER
 ***********************************************************************/
cs_uint16 m2r_rmac_irq_spl_hndlr(m2r_irq_spl_info_t * irq_spl_info)
{
  cs_uint16   status = 0 ;
  cs_uint16   sl_num ;
  M2R_t     * pDev ;
  cs_uint16   port_id ;
  cs_uint32   grp_id ;
  cs_uint16   irq_id ;
  cs_uint16   spl_clr_bits ;

  port_id = irq_spl_info->port_id;
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);
  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);

  switch (irq_spl_info->irq_oper) {
    case M2R_IRQ_SPL_INT :
        grp_id = irq_spl_info->op_mode.irq_hndl.irq_info->grp_id ;
        irq_id = irq_spl_info->op_mode.irq_hndl.irq_info->irq_id ;

        irq_spl_info->op_mode.irq_hndl.irq_info->flags |=
	  M2R_IRQ_INFO_PARAM1_FLAG ;

        if ( grp_id == M2R_GRP_RMAC_MICRO_IF_IRQ2 ) {
          /* USEQ_INT */
          irq_spl_info->op_mode.irq_hndl.irq_info->param1 =
              m2r_ms_read_int_bitmap( port_id, TRUE ) ;
          if ( irq_spl_info->op_mode.irq_hndl.irq_info->param1 & 0xff ) {
            cs_uint16 ms_int = 0 ;
            cs_uint16 times = 1 ;
            cs_int16 i;

            /* because clearing SEMAphore will raise up MS interrupt, we clear */
            /* MS interrupt here and then check again to see if there is another*/
            /* SEMAphore interrupt was happening.                              */
            for ( i=times; i>0; i-- ) {
               M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.IRQ2.wrd,
                            M2R_RMAC_MICRO_IF_IRQ2_USEQ_INT);
               ms_int = m2r_ms_read_int_bitmap( port_id, TRUE ) ;
               irq_spl_info->op_mode.irq_hndl.irq_info->param1 |= ms_int ;
               if ( (ms_int & 0xff) == 0 ) {
                 break ;
               }
            }

            /* retrieve MS-SEMA status flags */
            irq_spl_info->op_mode.irq_hndl.irq_info->param2 =
                M2R_REG_READ(&pDev->slice[sl_num].RingMAC.SPARE_REG5.wrd);
            irq_spl_info->op_mode.irq_hndl.irq_info->flags |=
                    M2R_IRQ_INFO_PARAM2_FLAG ;
          }

        } else {
          /* must be group M2R_GRP_RMAC_MICRO_IF_IRQ5 */
          switch (irq_id) {
            case M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1 :
                  irq_spl_info->op_mode.irq_hndl.irq_info->param1 =
                      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MEMPAR1.wrd);
                  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MEMPAR1.wrd,
          	               M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_ALL);
          	  break;
            case M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0 :
          	  irq_spl_info->op_mode.irq_hndl.irq_info->param1 =
          	      M2R_REG_READ(&pDev->slice[sl_num].RingMAC.MEMPAR0.wrd);
          	  M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MEMPAR0.wrd,
          	               M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_ALL);
          	  break;
          }
        }

    	break;

    case M2R_IRQ_SPL_CLR :
    	spl_clr_bits = irq_spl_info->op_mode.irq_clr.spl_clr_bits ;
        grp_id = irq_spl_info->op_mode.irq_clr.grp_id ;

        if ( grp_id == M2R_GRP_RMAC_MICRO_IF_IRQ0 ) {
          m2r_rmac_clear_host_drop_sat(port_id);
        } else { /* must be IRQ2 or IRQ5 */

	  /* This is a dummy call for IRQ2 so that the grp handler will
           * handle it correctly. */

    	  if (spl_clr_bits & M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1) {
              M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MEMPAR1.wrd,
          	             M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1_ALL);
    	  }

    	  if (spl_clr_bits & M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0) {
              M2R_REG_WRITE(&pDev->slice[sl_num].RingMAC.MEMPAR0.wrd,
          	             M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0_ALL);
    	  }
        }
    	break;

    default :
        return (0) ;
  }

  return (status) ;
}


/************************************************************************
 * GET DEFAULT RINGMAC IRQ CONFIGURATION
 ***********************************************************************/
cs_status m2r_rmac_get_default_irq_cfg(cs_uint16 port_id,
                                         m2r_port_summ_t * pSumm,
                                         m2r_rmac_cfg_t * pCfg)
{
  switch (pSumm->mac_mode) {
    case M2R_PORT_IN_SRP_MODE :
      break;
    case M2R_PORT_IN_RPR_MODE :
      break;
    case M2R_PORT_IN_BYPASS_MODE :
      CS_HNDL_ERROR(port_id, EM2R_RMAC_INVALID_IN_BYPASS, NULL) ;
      return(CS_ERROR) ;
  }

  pCfg->irq_ctl_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ0]
        = M2R_GRP_RMAC_MICRO_IF_IRQ0_ALL;
  pCfg->irq_ctl_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ1]
        = M2R_GRP_RMAC_MICRO_IF_IRQ1_ALL;
  /* Do not enable useq_fair interrupt by default */
  pCfg->irq_ctl_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ2]
        = (M2R_GRP_RMAC_MICRO_IF_IRQ2_ALL &
	   (~M2R_RMAC_MICRO_IF_IRQ2_USEQ_FAIR));
  pCfg->irq_ctl_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ3]
        = M2R_GRP_RMAC_MICRO_IF_IRQ3_ALL;
  pCfg->irq_ctl_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ4]
        = M2R_GRP_RMAC_MICRO_IF_IRQ4_ALL;
  pCfg->irq_ctl_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ5]
        = M2R_GRP_RMAC_MICRO_IF_IRQ5_ALL;

  if ( !(m2r_rmac_mems_must_be_cleared()) ) {
    pCfg->irq_ctl_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ5]
      &= ~(M2R_RMAC_MICRO_IF_IRQ5_MEMPAR0 |
	   M2R_RMAC_MICRO_IF_IRQ5_MEMPAR1) ;
  }

  pCfg->mate.xgxs_irq_ctl_tbl[M2R_GRP_XGXS_MP_MnInt0] =
                          M2R_GRP_XGXS_MP_MnInt0_ALL ;
  pCfg->mate.xgxs_irq_ctl_tbl[M2R_GRP_XGXS_MP_MnInt1] =
                       ( M2R_GRP_XGXS_MP_MnInt1_ALL &
			 ~(M2R_XGXS_MP_MnInt1_RxLBPAI |
			   M2R_XGXS_MP_MnInt1_TxLBPAI) ) ;
  pCfg->mate.xgxs_irq_ctl_tbl[M2R_GRP_XGXS_TX_TxInt]  =
                          M2R_GRP_XGXS_TX_TxInt_ALL ;
  pCfg->mate.xgxs_irq_ctl_tbl[M2R_GRP_XGXS_RX_RxInt]  =
                          M2R_GRP_XGXS_RX_RxInt_ALL ;

  return (CS_OK) ;
}


/************************************************************************
 * INIT RINGMAC IRQ
 ***********************************************************************/
cs_status m2r_rmac_init_irq(cs_uint16 port_id, m2r_rmac_cfg_t * pCfg)
{
  cs_uint16     ii ;

  /* Register IRQ Table */
  M2R_REGISTER_IRQ_TBL(port_id, M2R_ID_RMAC, m2r_rmac_irq_tbl,
                       m2r_rmac_irq_grp_name_tbl, M2R_RMAC_MAX_GRP,
		       m2r_rmac_irq_spl_hndlr) ;

  /* Enable the IRQs which are requested to be enabled by default */
  for (ii = 0; ii < M2R_RMAC_MAX_GRP; ii++) {

    /* Clear any interrupts */
    m2r_clear_leaf_irq(port_id, M2R_ID_RMAC, ii,
                         m2r_rmac_irq_tbl[ii].irq_map) ;

    /* Adam on 12/21/03 -> disable 0x8000 and 0x4000 from RMAC IRQ1 by default */
    if ( ii == M2R_GRP_RMAC_MICRO_IF_IRQ1 ) {
      cs_uint16 irq1_en_bits;

      irq1_en_bits = pCfg->irq_ctl_tbl[ii]
                     & ((~(M2R_RMAC_MICRO_IF_IRQ1_RX_TRAF |
			   M2R_RMAC_MICRO_IF_IRQ1_RX_TIMER)) & 0xffff);
      m2r_enable_leaf_irq(port_id, M2R_ID_RMAC, ii, irq1_en_bits) ;
    } else {
      m2r_enable_leaf_irq(port_id, M2R_ID_RMAC, ii, pCfg->irq_ctl_tbl[ii]) ;
    }
  }

  /* Mate XGXS */
  M2R_REGISTER_IRQ_TBL(port_id, M2R_ID_XGXS, m2r_rmac_xgxs_irq_tbl,
                       m2r_rmac_xgxs_irq_grp_name_tbl, M2R_XGXS_MAX_GRP,
		       NULL) ;

  for(ii = 0; ii < M2R_XGXS_MAX_GRP; ii++) {
    if (pCfg->mate.xgxs_irq_ctl_tbl[ii]) {
      /* Clear any interrupts */
      m2r_clear_leaf_irq(port_id, M2R_ID_XGXS, ii,
                         m2r_rmac_xgxs_irq_tbl[ii].irq_map) ;

      /* Enable The IRQ requested to be enabled by default */
      m2r_enable_leaf_irq(port_id, M2R_ID_XGXS, ii,
                          pCfg->mate.xgxs_irq_ctl_tbl[ii]) ;
    }
  }
  return (CS_OK) ;
}

/************************************************************************/
/*                  ISR ROUTINE FOR RINGMAC BLOCK                       */
/************************************************************************/
cs_status m2r_rmac_isr(cs_uint16 port_id)
{
  M2R_t                  * pDev ;
  cs_uint16                sl_num ;
  DR_MICRO_IF_IRQMAP_t     irqmap ;

  M2R_RMAC_INVALID_PORT_EXIT(port_id, CS_ERROR) ;
  M2R_RMAC_INVALID_MODE_EXIT(port_id, M2R_INVALID_IN_BYPASS, CS_ERROR) ;

  pDev   = M2R_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  irqmap.wrd = M2R_REG_READ(&pDev->slice[sl_num].RingMAC.IRQMAP.wrd) ;

  if(irqmap.bf.IRQ0) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_RMAC,
                M2R_GRP_RMAC_MICRO_IF_IRQ0,
                &m2r_rmac_irq_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ0],
	        m2r_rmac_irq_grp_name_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ0],
		M2R_GRP_RMAC_MICRO_IF_IRQ0_irq_id_name_tbl,
		sizeof(M2R_GRP_RMAC_MICRO_IF_IRQ0_irq_id_name_tbl) /
		sizeof(m2r_irq_id_name_t)) ;
  }

  if(irqmap.bf.IRQ1) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_RMAC,
                M2R_GRP_RMAC_MICRO_IF_IRQ1,
                &m2r_rmac_irq_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ1],
		m2r_rmac_irq_grp_name_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ1],
                M2R_GRP_RMAC_MICRO_IF_IRQ1_irq_id_name_tbl,
		sizeof(M2R_GRP_RMAC_MICRO_IF_IRQ1_irq_id_name_tbl) /
		sizeof(m2r_irq_id_name_t)) ;
  }

  if(irqmap.bf.IRQ2) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_RMAC,
                M2R_GRP_RMAC_MICRO_IF_IRQ2,
                &m2r_rmac_irq_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ2],
		m2r_rmac_irq_grp_name_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ2],
                M2R_GRP_RMAC_MICRO_IF_IRQ2_irq_id_name_tbl,
		sizeof(M2R_GRP_RMAC_MICRO_IF_IRQ2_irq_id_name_tbl) /
		sizeof(m2r_irq_id_name_t)) ;
  }

  if(irqmap.bf.IRQ3) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_RMAC,
                M2R_GRP_RMAC_MICRO_IF_IRQ3,
                &m2r_rmac_irq_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ3],
		m2r_rmac_irq_grp_name_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ3],
                M2R_GRP_RMAC_MICRO_IF_IRQ3_irq_id_name_tbl,
		sizeof(M2R_GRP_RMAC_MICRO_IF_IRQ3_irq_id_name_tbl) /
		sizeof(m2r_irq_id_name_t)) ;
  }

  if(irqmap.bf.IRQ4) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_RMAC,
                M2R_GRP_RMAC_MICRO_IF_IRQ4,
                &m2r_rmac_irq_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ4],
		m2r_rmac_irq_grp_name_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ4],
                M2R_GRP_RMAC_MICRO_IF_IRQ4_irq_id_name_tbl,
		sizeof(M2R_GRP_RMAC_MICRO_IF_IRQ4_irq_id_name_tbl) /
		sizeof(m2r_irq_id_name_t)) ;
  }

  if (irqmap.bf.IRQ5) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_RMAC,
                M2R_GRP_RMAC_MICRO_IF_IRQ5,
                &m2r_rmac_irq_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ5],
		m2r_rmac_irq_grp_name_tbl[M2R_GRP_RMAC_MICRO_IF_IRQ5],
                M2R_GRP_RMAC_MICRO_IF_IRQ5_irq_id_name_tbl,
		sizeof(M2R_GRP_RMAC_MICRO_IF_IRQ5_irq_id_name_tbl) /
		sizeof(m2r_irq_id_name_t)) ;
  }

  return(CS_OK) ;
}


/************************************************************************/
/*                  ISR ROUTINE FOR RMAC XGXS BLOCK                     */
/************************************************************************/
cs_status m2r_rmac_xgxs_isr(cs_uint16 port_id)
{
  M2R_t              * pDev ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);

  m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_XGXS,
           M2R_GRP_XGXS_MP_MnInt0,
           &m2r_rmac_xgxs_irq_tbl[M2R_GRP_XGXS_MP_MnInt0],
	   m2r_rmac_xgxs_irq_grp_name_tbl[M2R_GRP_XGXS_MP_MnInt0],
	   M2R_GRP_XGXS_MP_MnInt0_irq_id_name_tbl,
	   sizeof(M2R_GRP_XGXS_MP_MnInt0_irq_id_name_tbl) /
	   sizeof(m2r_irq_id_name_t)) ;

  m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_XGXS,
           M2R_GRP_XGXS_MP_MnInt1,
           &m2r_rmac_xgxs_irq_tbl[M2R_GRP_XGXS_MP_MnInt1],
	   m2r_rmac_xgxs_irq_grp_name_tbl[M2R_GRP_XGXS_MP_MnInt1],
	   M2R_GRP_XGXS_MP_MnInt1_irq_id_name_tbl,
	   sizeof(M2R_GRP_XGXS_MP_MnInt1_irq_id_name_tbl) /
	   sizeof(m2r_irq_id_name_t)) ;

  m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_XGXS,
           M2R_GRP_XGXS_TX_TxInt,
           &m2r_rmac_xgxs_irq_tbl[M2R_GRP_XGXS_TX_TxInt],
	   m2r_rmac_xgxs_irq_grp_name_tbl[M2R_GRP_XGXS_TX_TxInt],
	   M2R_GRP_XGXS_TX_TxInt_irq_id_name_tbl,
	   sizeof(M2R_GRP_XGXS_TX_TxInt_irq_id_name_tbl) /
	   sizeof(m2r_irq_id_name_t)) ;

  m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_XGXS,
           M2R_GRP_XGXS_RX_RxInt,
	   &m2r_rmac_xgxs_irq_tbl[M2R_GRP_XGXS_RX_RxInt],
	   m2r_rmac_xgxs_irq_grp_name_tbl[M2R_GRP_XGXS_RX_RxInt],
	   M2R_GRP_XGXS_RX_RxInt_irq_id_name_tbl,
	   sizeof(M2R_GRP_XGXS_RX_RxInt_irq_id_name_tbl) /
	   sizeof(m2r_irq_id_name_t)) ;

  return(CS_OK) ;
}


