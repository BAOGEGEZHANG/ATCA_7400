/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_lif
 *
 * LIF: Public structures and exported API's for applications.
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#ifndef __M2_LIF_XGXS_H__
#define __M2_LIF_XGXS_H__

#include "cs_types.h"
#include "m2r_xgxs.h"

/***********************************************************************
 * LIF XGXS IRQ LEAF DEFINITIONS
 ***********************************************************************/
#define M2R_LIF_XGXS_MP_MnInt0_LckDet3I    (0x0800)
#define M2R_LIF_XGXS_MP_MnInt0_LckDet2I    (0x0400)
#define M2R_LIF_XGXS_MP_MnInt0_LckDet1I    (0x0200)
#define M2R_LIF_XGXS_MP_MnInt0_LckDet0I    (0x0100)
#define M2R_LIF_XGXS_MP_MnInt0_DatDet3I    (0x0080)
#define M2R_LIF_XGXS_MP_MnInt0_DatDet2I    (0x0040)
#define M2R_LIF_XGXS_MP_MnInt0_DatDet1I    (0x0020)
#define M2R_LIF_XGXS_MP_MnInt0_DatDet0I    (0x0010)
#define M2R_LIF_XGXS_MP_MnInt0_NoClk3I     (0x0008)
#define M2R_LIF_XGXS_MP_MnInt0_NoClk2I     (0x0004)
#define M2R_LIF_XGXS_MP_MnInt0_NoClk1I     (0x0002)
#define M2R_LIF_XGXS_MP_MnInt0_NoClk0I     (0x0001)
#define M2R_GRP_LIF_XGXS_MP_MnInt0_ALL     (0x0fff)

#define M2R_LIF_XGXS_MP_MnInt1_RxNoClkI    (0x0200)
#define M2R_LIF_XGXS_MP_MnInt1_TxNoClkI    (0x0100)
#define M2R_LIF_XGXS_MP_MnInt1_RxPCPAI     (0x0080)
#define M2R_LIF_XGXS_MP_MnInt1_RxLBPAI     (0x0040)
#define M2R_LIF_XGXS_MP_MnInt1_TxLnPAI     (0x0020)
#define M2R_LIF_XGXS_MP_MnInt1_TxLBPAI     (0x0010)
#define M2R_LIF_XGXS_MP_MnInt1_RxL3PAI     (0x0008)
#define M2R_LIF_XGXS_MP_MnInt1_RxL2PAI     (0x0004)
#define M2R_LIF_XGXS_MP_MnInt1_RxL1PAI     (0x0002)
#define M2R_LIF_XGXS_MP_MnInt1_RxL0PAI     (0x0001)
#define M2R_GRP_LIF_XGXS_MP_MnInt1_ALL     (0x03ff)

#define M2R_LIF_XGXS_TX_TxInt_GoodPktSatI  (0x0004)
#define M2R_LIF_XGXS_TX_TxInt_ErrorPktSatI (0x0002)
#define M2R_LIF_XGXS_TX_TxInt_ErrorPktI    (0x0001)
#define M2R_GRP_LIF_XGXS_TX_TxInt_ALL      (0x0007)

#define M2R_LIF_XGXS_RX_RxInt_PatLockDetI  (0x2000)
#define M2R_LIF_XGXS_RX_RxInt_AlignDetI    (0x1000)
#define M2R_LIF_XGXS_RX_RxInt_SyncDet3I    (0x0800)
#define M2R_LIF_XGXS_RX_RxInt_SyncDet2I    (0x0400)
#define M2R_LIF_XGXS_RX_RxInt_SyncDet1I    (0x0200)
#define M2R_LIF_XGXS_RX_RxInt_SyncDet0I    (0x0100)
#define M2R_LIF_XGXS_RX_RxInt_PatErrSatI   (0x0040)
#define M2R_LIF_XGXS_RX_RxInt_PatErrI      (0x0020)
#define M2R_LIF_XGXS_RX_RxInt_InvCodeSatI  (0x0010)
#define M2R_LIF_XGXS_RX_RxInt_InvCodeI     (0x0008)
#define M2R_LIF_XGXS_RX_RxInt_GoodPktSatI  (0x0004)
#define M2R_LIF_XGXS_RX_RxInt_ErrorPktSatI (0x0002)
#define M2R_LIF_XGXS_RX_RxInt_ErrorPktI    (0x0001)
#define M2R_GRP_LIF_XGXS_RX_RxInt_ALL      (0x3f7f)

/***********************************************************************
 * LIF XGXS APIs
 ***********************************************************************/
extern  cs_status m2r_lif_xgxs_get_clk_status(cs_uint16 port_id) ;

extern  cs_uint16 m2r_lif_xgxs_get_status(cs_uint16 port_id,
                                          m2r_xgxs_status_t sel) ;
extern
cs_boolean m2r_lif_xgxs_is_align_detected(cs_uint16 port_id) ;
extern  cs_status m2r_lif_xgxs_reset_estore(cs_uint16 port_id) ;

extern  cs_status m2r_lif_xgxs_ctl_prbs(cs_uint16 port_id, m2r_ctl_t ctl,
                                m2r_xgxs_prbs_t pattern) ;

extern  cs_status m2r_lif_xgxs_inj_error(cs_uint16 port_id,
                                         m2r_xgxs_xaui_lane_t lane) ;

extern  cs_status m2r_lif_ctl_data_detect(cs_uint16 port_id, m2r_ctl_t ctl) ;

extern  cs_status m2r_lif_xgxs_cfg_prbs_reg_data(cs_uint16 port_id,
                                         m2r_xgxs_xaui_lane_t lane,
                                         cs_boolean ctrl_data, cs_uint8 data) ;

#endif /* __M2_LIF_XGXS_H__ */

