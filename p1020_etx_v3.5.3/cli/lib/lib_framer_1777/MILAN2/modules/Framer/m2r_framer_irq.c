/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_irq.c
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */
#include "cs_types.h"
#include "m2r_common.h"
#include "m2r_public.h"
#include "m2r_framer.h"
#include "m2r_framer_priv.h"
#include "m2r_irq_priv.h"
#include "m2r_framer_irq.h"

/************************************************************************/
/*    IRQ table                                                         */
/************************************************************************/
const m2r_irq_ctl_t  m2r_fr_irq_tbl[M2R_FR_MAX_GRP] = {
  CS_DI([M2R_GRP_FR_GLOBAL_MasterInt]          =) {
     M2R_FR_REG_OFFSET(MasterInt, 0, 0), 0,
     M2R_FR_REG_OFFSET(MasterMask, 0, 0),
     M2R_GRP_FR_GLOBAL_MasterInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_GLOBAL_ClockInt] =) {
     M2R_FR_REG_OFFSET(ClockInt, 0, 0),
     M2R_FR_REG_OFFSET(ClockStatus, 0, 0),
     M2R_FR_REG_OFFSET(ClockMask, 0, 0),
     M2R_GRP_FR_GLOBAL_ClockInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_GLOBAL_GPIOInt] =) {
     M2R_FR_REG_OFFSET(GPIOInt, 0, 0),
     M2R_FR_REG_OFFSET(GPIOValue, 0, 0),
     M2R_FR_REG_OFFSET(GPIOMask, 0, 0),
     M2R_GRP_FR_GLOBAL_GPIOInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_DATACOMRX_StrInt_0] =) {
     M2R_FR_REG_OFFSET(datacomRx_StrInt, 0,
		       M2R_FR_DATACOMRX_StrInt_STRIDE),
     M2R_FR_REG_OFFSET(datacomRx_StrStatus, 0,
		       M2R_FR_DATACOMRX_StrInt_STRIDE),
     M2R_FR_REG_OFFSET(datacomRx_StrMask, 0,
		       M2R_FR_DATACOMRX_StrInt_STRIDE),
     M2R_GRP_FR_DATACOMRX_StrInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_DATACOMRX_StrInt_1] =) {
     M2R_FR_REG_OFFSET(datacomRx_StrInt, 1,
		       M2R_FR_DATACOMRX_StrInt_STRIDE),
     M2R_FR_REG_OFFSET(datacomRx_StrStatus, 1,
		       M2R_FR_DATACOMRX_StrInt_STRIDE),
     M2R_FR_REG_OFFSET(datacomRx_StrMask, 1,
		       M2R_FR_DATACOMRX_StrInt_STRIDE),
     M2R_GRP_FR_DATACOMRX_StrInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_DATACOMRX_StrInt_2] =) {
     M2R_FR_REG_OFFSET(datacomRx_StrInt, 2,
		       M2R_FR_DATACOMRX_StrInt_STRIDE),
     M2R_FR_REG_OFFSET(datacomRx_StrStatus, 2,
		       M2R_FR_DATACOMRX_StrInt_STRIDE),
     M2R_FR_REG_OFFSET(datacomRx_StrMask, 2,
         	       M2R_FR_DATACOMRX_StrInt_STRIDE),
     M2R_GRP_FR_DATACOMRX_StrInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_DATACOMRX_StrInt_3] =) {
     M2R_FR_REG_OFFSET(datacomRx_StrInt, 3,
		       M2R_FR_DATACOMRX_StrInt_STRIDE),
     M2R_FR_REG_OFFSET(datacomRx_StrStatus, 3,
		       M2R_FR_DATACOMRX_StrInt_STRIDE),
     M2R_FR_REG_OFFSET(datacomRx_StrMask, 3,
		       M2R_FR_DATACOMRX_StrInt_STRIDE),
     M2R_GRP_FR_DATACOMRX_StrInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_DATACOMTX_StrInt_0] =) {
     M2R_FR_REG_OFFSET(datacomTx_StrInt, 0,
		       M2R_FR_DATACOMTX_StrConfig_STRIDE),
     M2R_FR_REG_OFFSET(datacomTx_StrStatus, 0,
		       M2R_FR_DATACOMTX_StrConfig_STRIDE),
     M2R_FR_REG_OFFSET(datacomTx_StrMask, 0,
		       M2R_FR_DATACOMTX_StrConfig_STRIDE),
     M2R_GRP_FR_DATACOMTX_StrInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_DATACOMTX_StrInt_1] =) {
     M2R_FR_REG_OFFSET(datacomTx_StrInt, 1,
		       M2R_FR_DATACOMTX_StrConfig_STRIDE),
     M2R_FR_REG_OFFSET(datacomTx_StrStatus, 1,
		       M2R_FR_DATACOMTX_StrConfig_STRIDE),
     M2R_FR_REG_OFFSET(datacomTx_StrMask, 1,
		       M2R_FR_DATACOMTX_StrConfig_STRIDE),
     M2R_GRP_FR_DATACOMTX_StrInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_DATACOMTX_StrInt_2] =) {
     M2R_FR_REG_OFFSET(datacomTx_StrInt, 2,
		       M2R_FR_DATACOMTX_StrConfig_STRIDE),
     M2R_FR_REG_OFFSET(datacomTx_StrStatus, 2,
		       M2R_FR_DATACOMTX_StrConfig_STRIDE),
     M2R_FR_REG_OFFSET(datacomTx_StrMask, 2,
		       M2R_FR_DATACOMTX_StrConfig_STRIDE),
     M2R_GRP_FR_DATACOMTX_StrInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_DATACOMTX_StrInt_3] =) {
     M2R_FR_REG_OFFSET(datacomTx_StrInt, 3,
		       M2R_FR_DATACOMTX_StrConfig_STRIDE),
     M2R_FR_REG_OFFSET(datacomTx_StrStatus, 3,
		       M2R_FR_DATACOMTX_StrConfig_STRIDE),
     M2R_FR_REG_OFFSET(datacomTx_StrMask, 3,
		       M2R_FR_DATACOMTX_StrConfig_STRIDE),
     M2R_GRP_FR_DATACOMTX_StrInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_STREAMRX_CellIntr_0] =) {
     M2R_FR_REG_OFFSET(streamRx_CellIntr, 0, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(CellSts, 0, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(CellMask, 0, M2R_FR_STREAMRX__STRIDE),
     M2R_GRP_FR_STREAMRX_CellIntr_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_STREAMRX_CellIntr_1] =) {
     M2R_FR_REG_OFFSET(streamRx_CellIntr, 1, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(CellSts, 1, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(CellMask, 1, M2R_FR_STREAMRX__STRIDE),
     M2R_GRP_FR_STREAMRX_CellIntr_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_STREAMRX_CellIntr_2] =) {
     M2R_FR_REG_OFFSET(streamRx_CellIntr, 2, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(CellSts, 2, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(CellMask, 2, M2R_FR_STREAMRX__STRIDE),
     M2R_GRP_FR_STREAMRX_CellIntr_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_STREAMRX_CellIntr_3] =) {
     M2R_FR_REG_OFFSET(streamRx_CellIntr, 3, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(CellSts, 3, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(CellMask, 3, M2R_FR_STREAMRX__STRIDE),
     M2R_GRP_FR_STREAMRX_CellIntr_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_STREAMTX_CellIntr_0] =) {
     M2R_FR_REG_OFFSET(streamTx_CellIntr, 0, M2R_FR_STREAMTX__STRIDE),
     0,
     M2R_FR_REG_OFFSET(CellIMask, 0, M2R_FR_STREAMTX__STRIDE),
     M2R_GRP_FR_STREAMTX_CellIntr_ALL, M2R_GRP_FR_STREAMTX_CellIntr_ALL,
     (M2R_IRQ_FLG_RW1C | M2R_IRQ_FLG_SPL_INT ) },

  CS_DI([M2R_GRP_FR_STREAMTX_CellIntr_1] =) {
     M2R_FR_REG_OFFSET(streamTx_CellIntr, 1, M2R_FR_STREAMTX__STRIDE),
     0,
     M2R_FR_REG_OFFSET(CellIMask, 1, M2R_FR_STREAMTX__STRIDE),
     M2R_GRP_FR_STREAMTX_CellIntr_ALL, M2R_GRP_FR_STREAMTX_CellIntr_ALL,
     (M2R_IRQ_FLG_RW1C | M2R_IRQ_FLG_SPL_INT ) },

  CS_DI([M2R_GRP_FR_STREAMTX_CellIntr_2] =) {
     M2R_FR_REG_OFFSET(streamTx_CellIntr, 2, M2R_FR_STREAMTX__STRIDE),
     0,
     M2R_FR_REG_OFFSET(CellIMask, 2, M2R_FR_STREAMTX__STRIDE),
     M2R_GRP_FR_STREAMTX_CellIntr_ALL, M2R_GRP_FR_STREAMTX_CellIntr_ALL,
     (M2R_IRQ_FLG_RW1C | M2R_IRQ_FLG_SPL_INT ) },

  CS_DI([M2R_GRP_FR_STREAMTX_CellIntr_3] =) {
     M2R_FR_REG_OFFSET(streamTx_CellIntr, 3, M2R_FR_STREAMTX__STRIDE),
     0,
     M2R_FR_REG_OFFSET(CellIMask, 3, M2R_FR_STREAMTX__STRIDE),
     M2R_GRP_FR_STREAMTX_CellIntr_ALL, M2R_GRP_FR_STREAMTX_CellIntr_ALL,
     (M2R_IRQ_FLG_RW1C | M2R_IRQ_FLG_SPL_INT ) },

  CS_DI([M2R_GRP_FR_STREAMRX_PktIntr_0] =) {
     M2R_FR_REG_OFFSET(PktIntr, 0, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(PktStatus, 0, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(streamRx_PktMask, 0, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_STREAMRX_PktIntr_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_STREAMRX_PktIntr_1] =) {
     M2R_FR_REG_OFFSET(PktIntr, 1, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(PktStatus, 1, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(streamRx_PktMask, 1, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_STREAMRX_PktIntr_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_STREAMRX_PktIntr_2] =) {
     M2R_FR_REG_OFFSET(PktIntr, 2, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(PktStatus, 2, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(streamRx_PktMask, 2, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_STREAMRX_PktIntr_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_STREAMRX_PktIntr_3] =) {
     M2R_FR_REG_OFFSET(PktIntr, 3, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(PktStatus, 3, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_REG_OFFSET(streamRx_PktMask, 3, M2R_FR_STREAMRX__STRIDE),
     M2R_FR_STREAMRX_PktIntr_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_STREAMTX_PktInt_0] =) {
     M2R_FR_REG_OFFSET(PktInt, 0, M2R_FR_STREAMTX__STRIDE),
     M2R_FR_REG_OFFSET(PktSts, 0, M2R_FR_STREAMTX__STRIDE),
     M2R_FR_REG_OFFSET(streamTx_PktMask, 0, M2R_FR_STREAMTX__STRIDE),
     M2R_GRP_FR_STREAMTX_PktInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_STREAMTX_PktInt_1] =) {
     M2R_FR_REG_OFFSET(PktInt, 1, M2R_FR_STREAMTX__STRIDE),
     M2R_FR_REG_OFFSET(PktSts, 1, M2R_FR_STREAMTX__STRIDE),
     M2R_FR_REG_OFFSET(streamTx_PktMask, 1, M2R_FR_STREAMTX__STRIDE),
     M2R_GRP_FR_STREAMTX_PktInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_STREAMTX_PktInt_2] =) {
     M2R_FR_REG_OFFSET(PktInt, 2, M2R_FR_STREAMTX__STRIDE),
     M2R_FR_REG_OFFSET(PktSts, 2, M2R_FR_STREAMTX__STRIDE),
     M2R_FR_REG_OFFSET(streamTx_PktMask, 2, M2R_FR_STREAMTX__STRIDE),
     M2R_GRP_FR_STREAMTX_PktInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_STREAMTX_PktInt_3] =) {
     M2R_FR_REG_OFFSET(PktInt, 3, M2R_FR_STREAMTX__STRIDE),
     M2R_FR_REG_OFFSET(PktSts, 3, M2R_FR_STREAMTX__STRIDE),
     M2R_FR_REG_OFFSET(streamTx_PktMask, 3, M2R_FR_STREAMTX__STRIDE),
     M2R_GRP_FR_STREAMTX_PktInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_GFP_RX_GFP_Intr] =) {
     M2R_FR_REG_OFFSET(gfp_rx_GFP_Intr, 0, 0),
     M2R_FR_REG_OFFSET(gfp_rx_GFP_Status, 0, 0),
     M2R_FR_REG_OFFSET(gfp_rx_GFP_Mask, 0, 0),
     M2R_GRP_FR_GFP_RX_GFP_Intr_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_GFP_TX_GFP_Intr] =) {
     M2R_FR_REG_OFFSET(gfp_tx_GFP_Intr, 0, 0),
     M2R_FR_REG_OFFSET(gfp_tx_GFP_Status, 0, 0),
     M2R_FR_REG_OFFSET(gfp_tx_GFP_Mask, 0, 0),
     M2R_GRP_FR_GFP_TX_GFP_Intr_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_SONET32RX_PathInt_0] =) {
     M2R_FR_REG_OFFSET(PathInt, 0, M2R_FR_SONET32RX_PathInt_STRIDE),
     M2R_FR_REG_OFFSET(PathStatus, 0, M2R_FR_SONET32RX_PathInt_STRIDE),
     M2R_FR_REG_OFFSET(PathMask, 0, M2R_FR_SONET32RX_PathInt_STRIDE),
     M2R_GRP_FR_SONET32RX_PathInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_SONET32RX_PathInt_1] =) {
     M2R_FR_REG_OFFSET(PathInt, 1, M2R_FR_SONET32RX_PathInt_STRIDE),
     M2R_FR_REG_OFFSET(PathStatus, 1, M2R_FR_SONET32RX_PathInt_STRIDE),
     M2R_FR_REG_OFFSET(PathMask, 1, M2R_FR_SONET32RX_PathInt_STRIDE),
     M2R_GRP_FR_SONET32RX_PathInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_SONET32RX_PathInt_2] =) {
     M2R_FR_REG_OFFSET(PathInt, 2, M2R_FR_SONET32RX_PathInt_STRIDE),
     M2R_FR_REG_OFFSET(PathStatus, 2, M2R_FR_SONET32RX_PathInt_STRIDE),
     M2R_FR_REG_OFFSET(PathMask, 2, M2R_FR_SONET32RX_PathInt_STRIDE),
     M2R_GRP_FR_SONET32RX_PathInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_SONET32RX_PathInt_3] =) {
     M2R_FR_REG_OFFSET(PathInt, 3, M2R_FR_SONET32RX_PathInt_STRIDE),
     M2R_FR_REG_OFFSET(PathStatus, 3, M2R_FR_SONET32RX_PathInt_STRIDE),
     M2R_FR_REG_OFFSET(PathMask, 3, M2R_FR_SONET32RX_PathInt_STRIDE),
     M2R_GRP_FR_SONET32RX_PathInt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_SONET32RX_LineInterrupt] =) {
     M2R_FR_REG_OFFSET(LineInterrupt, 0, 0),
     M2R_FR_REG_OFFSET(LineStatus, 0, 0),
     M2R_FR_REG_OFFSET(LineMask, 0, 0),
     M2R_GRP_FR_SONET32RX_LineInterrupt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_SONET32RX_SectionInterrupt] =) {
     M2R_FR_REG_OFFSET(SectionInterrupt, 0, 0),
     M2R_FR_REG_OFFSET(SectionStatus, 0, 0),
     M2R_FR_REG_OFFSET(SectionMask, 0, 0),
     M2R_GRP_FR_SONET32RX_SectionInterrupt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_SONET32RX_APSInterrupt] =) {
     M2R_FR_REG_OFFSET(APSInterrupt, 0, 0),
     0,
     M2R_FR_REG_OFFSET(APSMask, 0, 0),
     M2R_GRP_FR_SONET32RX_APSInterrupt_ALL, 0x08E0,
     (M2R_IRQ_FLG_RW1C | M2R_IRQ_FLG_SPL_INT) },

  CS_DI([M2R_GRP_FR_RXHDLC_Interrupt_Section] =) {
     M2R_FR_REG_OFFSET(rxHDLC_Interrupt, 0, 0),
     0,
     M2R_FR_REG_OFFSET(rxHDLC_Mask, 0, 0),
     M2R_GRP_FR_RXHDLC_Interrupt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_TXHDLC_Interrupt_Section] =) {
     M2R_FR_REG_OFFSET(txHDLC_Interrupt, 0, 0),
     0,
     M2R_FR_REG_OFFSET(txHDLC_Mask, 0, 0),
     M2R_GRP_FR_TXHDLC_Interrupt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_RXHDLC_Interrupt_Line] =) {
     M2R_FR_REG_OFFSET(rxHDLC_Interrupt, 1, M2R_FR_RXHDLC__STRIDE),
     0,
     M2R_FR_REG_OFFSET(rxHDLC_Mask, 1, M2R_FR_RXHDLC__STRIDE),
     M2R_GRP_FR_RXHDLC_Interrupt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_GRP_FR_TXHDLC_Interrupt_Line] =) {
     M2R_FR_REG_OFFSET(txHDLC_Interrupt, 1, M2R_FR_TXHDLC__STRIDE),
     0,
     M2R_FR_REG_OFFSET(txHDLC_Mask, 1, M2R_FR_TXHDLC__STRIDE),
     M2R_GRP_FR_TXHDLC_Interrupt_ALL, 0, M2R_IRQ_FLG_RW1C }
} ;


const m2r_irq_ctl_t  m2r_fr_imed_irq_tbl[M2R_FR_MAX_IMED] = {
  CS_DI([M2R_IMED_FR_GLOBAL_MasterInt] =) {
     M2R_FR_REG_OFFSET(MasterInt, 0, 0),
     0,
     M2R_FR_REG_OFFSET(MasterMask, 0, 0),
     M2R_IMED_FR_GLOBAL_MasterInt_ALL, 0, 0 },

  CS_DI([M2R_IMED_FR_GLOBAL_MasterInt2] =) {
     M2R_FR_REG_OFFSET(MasterInt2, 0, 0),
     0,
     M2R_FR_REG_OFFSET(MasterMask2, 0, 0),
     M2R_IMED_FR_GLOBAL_MasterInt2_ALL, 0, 0 },

  CS_DI([M2R_IMED_FR_GLOBAL_CellRxInt] =) {
     M2R_FR_REG_OFFSET(CellRxInt, 0, 0),
     0,
     M2R_FR_REG_OFFSET(CellRxMask, 0, 0),
     M2R_IMED_FR_GLOBAL_CellRxInt_ALL, 0, 0 },

  CS_DI([M2R_IMED_FR_GLOBAL_CellTxInt] =) {
     M2R_FR_REG_OFFSET(CellTxInt, 0, 0),
     0,
     M2R_FR_REG_OFFSET(CellTxMask, 0, 0),
     M2R_IMED_FR_GLOBAL_CellTxInt_ALL, 0, 0 },

  CS_DI([M2R_IMED_FR_GLOBAL_PacketRxInt] =) {
     M2R_FR_REG_OFFSET(PacketRxInt, 0, 0),
     0,
     M2R_FR_REG_OFFSET(PacketRxMask, 0, 0),
     M2R_IMED_FR_GLOBAL_PacketRxInt_ALL, 0, 0 },

  CS_DI([M2R_IMED_FR_GLOBAL_PacketTxInt] =) {
     M2R_FR_REG_OFFSET(PacketTxInt, 0, 0),
     0,
     M2R_FR_REG_OFFSET(PacketTxMask, 0, 0),
     M2R_IMED_FR_GLOBAL_PacketTxInt_ALL, 0, 0 },

  CS_DI([M2R_IMED_FR_DATACOMRX_Interrupt] =) {
     M2R_FR_REG_OFFSET(datacomRx_Interrupt, 0, 0),
     0,
     M2R_FR_REG_OFFSET(datacomRx_Mask, 0, 0),
     M2R_IMED_FR_DATACOMRX_Interrupt_ALL, 0, 0 },

  CS_DI([M2R_IMED_FR_DATACOMTX_Interrupt] =) {
     M2R_FR_REG_OFFSET(datacomTx_Interrupt, 0, 0),
     0,
     M2R_FR_REG_OFFSET(datacomTx_Mask, 0, 0),
     M2R_IMED_FR_DATACOMTX_Interrupt_ALL, 0, 0 },

  CS_DI([M2R_IMED_FR_SONET32RX_SectionInterrupt] =) {
     M2R_FR_REG_OFFSET(SectionInterrupt, 0, 0),
     0,
     M2R_FR_REG_OFFSET(SectionMask, 0, 0),
     M2R_IMED_FR_SONET32RX_SectionInterrupt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_IMED_FR_SONET32RX_LineInterrupt] =) {
     M2R_FR_REG_OFFSET(LineInterrupt, 0, 0),
     0,
     M2R_FR_REG_OFFSET(LineMask, 0, 0),
     M2R_IMED_FR_SONET32RX_LineInterrupt_ALL, 0, M2R_IRQ_FLG_RW1C },

  CS_DI([M2R_IMED_FR_SONET32RX_PathInterrupts] =) {
     M2R_FR_REG_OFFSET(PathInterrupts, 0, 0),
     0,
     M2R_FR_REG_OFFSET(PathMasks, 0, 0),
     M2R_IMED_FR_SONET32RX_PathInterrupts_ALL, 0, 0 },

  CS_DI([M2R_IMED_FR_TRANSMIT_TxInterrupt] =) {
     M2R_FR_REG_OFFSET(TxInterrupt, 0, 0),
     0,
     M2R_FR_REG_OFFSET(TxMask, 0, 0),
     M2R_IMED_FR_TRANSMIT_TxInterrupt_ALL, 0, M2R_IRQ_FLG_RW1C }
} ;


const char * m2r_fr_irq_grp_name_tbl[M2R_FR_MAX_GRP] =
{
  CS_DI([M2R_GRP_FR_GLOBAL_MasterInt]    =) "FR_GLOBAL_MasterInt",
  CS_DI([M2R_GRP_FR_GLOBAL_ClockInt]     =) "FR_GLOBAL_ClockInt",
  CS_DI([M2R_GRP_FR_GLOBAL_GPIOInt]      =) "FR_GLOBAL_GPIOInt",
  CS_DI([M2R_GRP_FR_DATACOMRX_StrInt_0]  =) "FR_DATACOMRX_StrInt_0",
  CS_DI([M2R_GRP_FR_DATACOMRX_StrInt_1]  =) "FR_DATACOMRX_StrInt_1",
  CS_DI([M2R_GRP_FR_DATACOMRX_StrInt_2]  =) "FR_DATACOMRX_StrInt_2",
  CS_DI([M2R_GRP_FR_DATACOMRX_StrInt_3]  =) "FR_DATACOMRX_StrInt_3",
  CS_DI([M2R_GRP_FR_DATACOMTX_StrInt_0]  =) "FR_DATACOMTX_StrInt_0",
  CS_DI([M2R_GRP_FR_DATACOMTX_StrInt_1]  =) "FR_DATACOMTX_StrInt_1",
  CS_DI([M2R_GRP_FR_DATACOMTX_StrInt_2]  =) "FR_DATACOMTX_StrInt_2",
  CS_DI([M2R_GRP_FR_DATACOMTX_StrInt_3]  =) "FR_DATACOMTX_StrInt_3",
  CS_DI([M2R_GRP_FR_STREAMRX_CellIntr_0] =) "FR_STREAMRX_CellIntr_0",
  CS_DI([M2R_GRP_FR_STREAMRX_CellIntr_1] =) "FR_STREAMRX_CellIntr_1",
  CS_DI([M2R_GRP_FR_STREAMRX_CellIntr_2] =) "FR_STREAMRX_CellIntr_2",
  CS_DI([M2R_GRP_FR_STREAMRX_CellIntr_3] =) "FR_STREAMRX_CellIntr_3",
  CS_DI([M2R_GRP_FR_STREAMTX_CellIntr_0] =) "FR_STREAMTX_CellIntr_0",
  CS_DI([M2R_GRP_FR_STREAMTX_CellIntr_1] =) "FR_STREAMTX_CellIntr_1",
  CS_DI([M2R_GRP_FR_STREAMTX_CellIntr_2] =) "FR_STREAMTX_CellIntr_2",
  CS_DI([M2R_GRP_FR_STREAMTX_CellIntr_3] =) "FR_STREAMTX_CellIntr_3",
  CS_DI([M2R_GRP_FR_STREAMRX_PktIntr_0]  =) "FR_STREAMRX_PktIntr_0",
  CS_DI([M2R_GRP_FR_STREAMRX_PktIntr_1]  =) "FR_STREAMRX_PktIntr_1",
  CS_DI([M2R_GRP_FR_STREAMRX_PktIntr_2]  =) "FR_STREAMRX_PktIntr_2",
  CS_DI([M2R_GRP_FR_STREAMRX_PktIntr_3]  =) "FR_STREAMRX_PktIntr_3",
  CS_DI([M2R_GRP_FR_STREAMTX_PktInt_0]   =) "FR_STREAMTX_PktInt_0",
  CS_DI([M2R_GRP_FR_STREAMTX_PktInt_1]   =) "FR_STREAMTX_PktInt_1",
  CS_DI([M2R_GRP_FR_STREAMTX_PktInt_2]   =) "FR_STREAMTX_PktInt_2",
  CS_DI([M2R_GRP_FR_STREAMTX_PktInt_3]   =) "FR_STREAMTX_PktInt_3",
  CS_DI([M2R_GRP_FR_GFP_RX_GFP_Intr]     =) "FR_GFP_RX_GFP_Intr",
  CS_DI([M2R_GRP_FR_GFP_TX_GFP_Intr]     =) "FR_GFP_TX_GFP_Intr",
  CS_DI([M2R_GRP_FR_SONET32RX_PathInt_0] =) "FR_SONET32RX_PathInt_0",
  CS_DI([M2R_GRP_FR_SONET32RX_PathInt_1] =) "FR_SONET32RX_PathInt_1",
  CS_DI([M2R_GRP_FR_SONET32RX_PathInt_2] =) "FR_SONET32RX_PathInt_2",
  CS_DI([M2R_GRP_FR_SONET32RX_PathInt_3] =) "FR_SONET32RX_PathInt_3",
  CS_DI([M2R_GRP_FR_SONET32RX_LineInterrupt]  =) "FR_SONET32RX_LineInterrupt",
  CS_DI([M2R_GRP_FR_SONET32RX_SectionInterrupt] =)
                              "FR_SONET32RX_SectionInterrupt",
  CS_DI([M2R_GRP_FR_SONET32RX_APSInterrupt]   =) "FR_SONET32RX_APSInterrupt",
  CS_DI([M2R_GRP_FR_RXHDLC_Interrupt_Section] =) "FR_RXHDLC_Interrupt_Section",
  CS_DI([M2R_GRP_FR_TXHDLC_Interrupt_Section] =) "FR_TXHDLC_Interrupt_Section",
  CS_DI([M2R_GRP_FR_RXHDLC_Interrupt_Line]    =) "FR_RXHDLC_Interrupt_Line",
  CS_DI([M2R_GRP_FR_TXHDLC_Interrupt_Line]    =) "FR_TXHDLC_Interrupt_Line"
} ;

const char * m2r_fr_irq_imed_name_tbl[M2R_FR_MAX_IMED] =
{
  CS_DI([M2R_IMED_FR_GLOBAL_MasterInt]           =) "FR_GLOBAL_MasterInt",
  CS_DI([M2R_IMED_FR_GLOBAL_MasterInt2]          =) "FR_GLOBAL_MasterInt2",
  CS_DI([M2R_IMED_FR_GLOBAL_CellRxInt]           =) "FR_GLOBAL_CellRxInt",
  CS_DI([M2R_IMED_FR_GLOBAL_CellTxInt]           =) "FR_GLOBAL_CellTxInt",
  CS_DI([M2R_IMED_FR_GLOBAL_PacketRxInt]         =) "FR_GLOBAL_PacketRxInt",
  CS_DI([M2R_IMED_FR_GLOBAL_PacketTxInt]         =) "FR_GLOBAL_PacketTxInt",
  CS_DI([M2R_IMED_FR_DATACOMRX_Interrupt]        =) "FR_DATACOMRX_Interrupt",
  CS_DI([M2R_IMED_FR_DATACOMTX_Interrupt]        =) "FR_DATACOMTX_Interrupt",
  CS_DI([M2R_IMED_FR_SONET32RX_SectionInterrupt] =) "FR_SONET32RX_SectionInterrupt",
  CS_DI([M2R_IMED_FR_SONET32RX_LineInterrupt]    =) "FR_SONET32RX_LineInterrupt",
  CS_DI([M2R_IMED_FR_SONET32RX_PathInterrupts]   =) "FR_SONET32RX_PathInterrupts",
  CS_DI([M2R_IMED_FR_TRANSMIT_TxInterrupt]       =) "FR_TRANSMIT_TxInterrupt"
} ;

/**************** IRQ ID NAME TABLES **********************/
const m2r_irq_id_name_t
M2R_GRP_FR_GLOBAL_MasterInt_irq_id_name_tbl[] = {
  {M2R_FR_GLOBAL_MasterInt_TxFrStartI,  	"TxFrStartI"},
  {M2R_FR_GLOBAL_MasterInt_APSI,        	"APSI"},
  {M2R_FR_GLOBAL_MasterInt_ClkFailI,    	"ClkFailI"},
  {M2R_FR_GLOBAL_MasterInt_GPIOI,       	"GPIOI"},
  {M2R_FR_GLOBAL_MasterInt_TxI,         	"TxI"},
  {M2R_FR_GLOBAL_MasterInt_DatacomTxI,  	"DatacomTxI"},
  {M2R_FR_GLOBAL_MasterInt_TxCellPI,    	"TxCellPI"},
  {M2R_FR_GLOBAL_MasterInt_TxPktPI,     	"TxPktPI"},
  {M2R_FR_GLOBAL_MasterInt_RxFrStartI,  	"RxFrStartI"},
  {M2R_FR_GLOBAL_MasterInt_MbusTimeoutI,        "MbusTimeoutI"},
  {M2R_FR_GLOBAL_MasterInt_RxSI,        	"RxSI"},
  {M2R_FR_GLOBAL_MasterInt_RxLI,        	"RxLI"},
  {M2R_FR_GLOBAL_MasterInt_RxPI,        	"RxPI"},
  {M2R_FR_GLOBAL_MasterInt_ResetI,      	"ResetI"},
  {M2R_FR_GLOBAL_MasterInt_RxCellPI,    	"RxCellPI"},
  {M2R_FR_GLOBAL_MasterInt_RxPktPI,     	"RxPktPI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_IRQ_GBL_MasterInt2_irq_id_name_tbl[] = {
  {M2R_FR_GLOBAL_MasterInt2_StreamRx_GFP_I,     "StreamRx_GFP_I"},
  {M2R_FR_GLOBAL_MasterInt2_StreamTx_GFP_I,     "StreamTx_GFP_I"},
  {M2R_FR_GLOBAL_MasterInt2_DatacomRxI,         "DatacomRxI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_GLOBAL_ClockInt_irq_id_name_tbl[] = {
  {M2R_FR_GLOBAL_ClockInt_RxClkI,       "RxClkI"},
  {M2R_FR_GLOBAL_ClockInt_TxClkI,       "TxClkI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_GLOBAL_GPIOInt_irq_id_name_tbl[] = {
  {M2R_FR_GLOBAL_GPIOInt_RI0,   "RI0"},
  {M2R_FR_GLOBAL_GPIOInt_RI1,   "RI1"},
  {M2R_FR_GLOBAL_GPIOInt_RI2,   "RI2"},
  {M2R_FR_GLOBAL_GPIOInt_RI3,   "RI3"},
  {M2R_FR_GLOBAL_GPIOInt_RI4,   "RI4"},
  {M2R_FR_GLOBAL_GPIOInt_RI5,   "RI5"},
  {M2R_FR_GLOBAL_GPIOInt_RI6,   "RI6"},
  {M2R_FR_GLOBAL_GPIOInt_RI7,   "RI7"},
  {M2R_FR_GLOBAL_GPIOInt_FI0,   "FI0"},
  {M2R_FR_GLOBAL_GPIOInt_FI1,   "FI1"},
  {M2R_FR_GLOBAL_GPIOInt_FI2,   "FI2"},
  {M2R_FR_GLOBAL_GPIOInt_FI3,   "FI3"},
  {M2R_FR_GLOBAL_GPIOInt_FI4,   "FI4"},
  {M2R_FR_GLOBAL_GPIOInt_FI5,   "FI5"},
  {M2R_FR_GLOBAL_GPIOInt_FI6,   "FI6"},
  {M2R_FR_GLOBAL_GPIOInt_FI7,   "FI7"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_DATACOMRX_StrInt_irq_id_name_tbl[] = {
  {M2R_FR_DATACOMRX_StrInt_RX_FIFO_Par_Err_I,   "RX_FIFO_Par_Err_I"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_DATACOMTX_StrInt_irq_id_name_tbl[] = {
  {M2R_FR_DATACOMTX_StrInt_FIFO_Par_Err_I,      "FIFO_Par_Err_I"},
  {M2R_FR_DATACOMTX_StrInt_overflowI,   	"overflowI"},
  {M2R_FR_DATACOMTX_StrInt_badFrameI,   	"badFrameI"},
  {M2R_FR_DATACOMTX_StrInt_shortCellI,  	"shortCellI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_STREAMRX_CellIntr_irq_id_name_tbl[] = {
  {M2R_FR_STREAMRX_CellIntr_CellCntI,   "CellCntI"},
  {M2R_FR_STREAMRX_CellIntr_IdleCntI,   "IdleCntI"},
  {M2R_FR_STREAMRX_CellIntr_HECDropI,   "HECDropI"},
  {M2R_FR_STREAMRX_CellIntr_HECCorrI,   "HECCorrI"},
  {M2R_FR_STREAMRX_CellIntr_OVFCntI,    "OVFCntI"},
  {M2R_FR_STREAMRX_CellIntr_SynExI,     "SynExI"},
  {M2R_FR_STREAMRX_CellIntr_OvrFloI,    "OvrFloI"},
  {M2R_FR_STREAMRX_CellIntr_HECErrI,    "HECErrI"},
  {M2R_FR_STREAMRX_CellIntr_LCDI,       "LCDI"},
  {M2R_FR_STREAMRX_CellIntr_OCDI,       "OCDI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_STREAMTX_CellIntr_irq_id_name_tbl[] = {
  {M2R_FR_STREAMTX_CellIntr_CellCntI,   "CellCntI"},
  {M2R_FR_STREAMTX_CellIntr_IdleCntI,   "IdleCntI"}
} ;

const m2r_irq_id_name_t
M2R_FR_STREAMRX_PktIntr_irq_id_name_tbl[] = {
  {M2R_FR_STREAMRX_PktIntr_shrtPkt,     "shrtPkt"},
  {M2R_FR_STREAMRX_PktIntr_lngPkt,      "lngPkt"},
  {M2R_FR_STREAMRX_PktIntr_Ovrflow,     "Ovrflow"},
  {M2R_FR_STREAMRX_PktIntr_FCSErr,      "FCSErr"},
  {M2R_FR_STREAMRX_PktIntr_formatErr,   "formatErr"},
  {M2R_FR_STREAMRX_PktIntr_abort,       "abort"},
  {M2R_FR_STREAMRX_PktIntr_InvCnt,      "InvCnt"},
  {M2R_FR_STREAMRX_PktIntr_DropCnt,     "DropCnt"},
  {M2R_FR_STREAMRX_PktIntr_FCSECnt,     "FCSECnt"},
  {M2R_FR_STREAMRX_PktIntr_pktCnt,      "pktCnt"},
  {M2R_FR_STREAMRX_PktIntr_byteCnt,     "byteCnt"},
  {M2R_FR_STREAMRX_PktIntr_shrtPktCnt,  "shrtPktCnt"},
  {M2R_FR_STREAMRX_PktIntr_longPktCnt,  "longPktCnt"},
  {M2R_FR_STREAMRX_PktIntr_OvrPktCnt,   "OvrPktCnt"},
  {M2R_FR_STREAMRX_PktIntr_abortPktCnt, "abortPktCnt"},
  {M2R_FR_STREAMRX_PktIntr_GdPktCnt,    "GdPktCnt"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_STREAMTX_PktInt_irq_id_name_tbl[] = {
  {M2R_FR_STREAMTX_PktInt_ByteCntI,     "ByteCntI"},
  {M2R_FR_STREAMTX_PktInt_TxByteCntI,   "TxByteCntI"},
  {M2R_FR_STREAMTX_PktInt_PktCntI,      "PktCntI"},
  {M2R_FR_STREAMTX_PktInt_UndCntI,      "UndCntI"},
  {M2R_FR_STREAMTX_PktInt_ErrCntI,      "ErrCntI"},
  {M2R_FR_STREAMTX_PktInt_LongCntI,     "LongCntI"},
  {M2R_FR_STREAMTX_PktInt_ShortCntI,    "ShortCntI"},
  {M2R_FR_STREAMTX_PktInt_TxShrtPktI,   "TxShrtPktI"},
  {M2R_FR_STREAMTX_PktInt_TxLngPktI,    "TxLngPktI"},
  {M2R_FR_STREAMTX_PktInt_TxUndFlowI,   "TxUndFlowI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_GFP_RX_GFP_Intr_irq_id_name_tbl[] = {
  {M2R_FR_GFP_RX_GFP_Intr_Long_Pkt_I,   	"Long_Pkt_I"},
  {M2R_FR_GFP_RX_GFP_Intr_Short_Pkt_I,  	"Short_Pkt_I"},
  {M2R_FR_GFP_RX_GFP_Intr_1bit_HEC_Err_I,       "1bit_HEC_Err_I"},
  {M2R_FR_GFP_RX_GFP_Intr_Mbit_HEC_Err_I,       "Mbit_HEC_Err_I"},
  {M2R_FR_GFP_RX_GFP_Intr_OverFlow_I,   	"OverFlow_I"},
  {M2R_FR_GFP_RX_GFP_Intr_Dropped_Pkt_I,        "Dropped_Pkt_I"},
  {M2R_FR_GFP_RX_GFP_Intr_Sync_Err_I,   	"Sync_Err_I"},
  {M2R_FR_GFP_RX_GFP_Intr_Loss_Client_I,        "Loss_Client_I"},
  {M2R_FR_GFP_RX_GFP_Intr_Loss_Sync_I,  	"Loss_Sync_I"},
  {M2R_FR_GFP_RX_GFP_Intr_CRC_Err_I,    	"CRC_Err_I"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_GFP_TX_GFP_Intr_irq_id_name_tbl[] = {
  {M2R_FR_GFP_TX_GFP_Intr_Idle_Pkts_MSB_I,      "Idle_Pkts_MSB_I"},
  {M2R_FR_GFP_TX_GFP_Intr_Oversized_Pkts_MSB_I, "Oversized_Pkts_MSB_I"},
  {M2R_FR_GFP_TX_GFP_Intr_Short_Pkt_I,  	"Short_Pkt_I"},
  {M2R_FR_GFP_TX_GFP_Intr_Long_Pkt_I,   	"Long_Pkt_I"},
  {M2R_FR_GFP_TX_GFP_Intr_Oversized_Pkt_I,      "Oversized_Pkt_I"},
  {M2R_FR_GFP_TX_GFP_Intr_Under_Flow_I,         "Under_Flow_I"},
  {M2R_FR_GFP_TX_GFP_Intr_ECC_Err_Corr_I,       "ECC_Err_Corr_I"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_SONET32RX_PathInt_irq_id_name_tbl[] = {
  {M2R_FR_SONET32RX_PathInterrupt_NewPtrI,      "NewPtrI"},
  {M2R_FR_SONET32RX_PathInterrupt_pathBERI,     "pathBERI"},
  {M2R_FR_SONET32RX_PathInterrupt_PTCapI,       "PTCapI"},
  {M2R_FR_SONET32RX_PathInterrupt_SSI,  	"SSI"},
  {M2R_FR_SONET32RX_PathInterrupt_PBGEI,        "PBGEI"},
  {M2R_FR_SONET32RX_PathInterrupt_PBIPI,        "PBIPI"},
  {M2R_FR_SONET32RX_PathInterrupt_PPtrEI,       "PPtrEI"},
  {M2R_FR_SONET32RX_PathInterrupt_PREII,        "PREII"},
  {M2R_FR_SONET32RX_PathInterrupt_PSLUnsI,      "PSLUnsI"},
  {M2R_FR_SONET32RX_PathInterrupt_PSLMisI,      "PSLMisI"},
  {M2R_FR_SONET32RX_PathInterrupt_PTUnsI,       "PTUnsI"},
  {M2R_FR_SONET32RX_PathInterrupt_PTMisI,       "PTMisI"},
  {M2R_FR_SONET32RX_PathInterrupt_PUNEQPI,      "PUNEQPI"},
  {M2R_FR_SONET32RX_PathInterrupt_PRDII,        "PRDII"},
  {M2R_FR_SONET32RX_PathInterrupt_PAISI,        "PAISI"},
  {M2R_FR_SONET32RX_PathInterrupt_LOPI,         "LOPI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_SONET32RX_LineInterrupt_irq_id_name_tbl[] = {
  {M2R_FR_SONET32RX_LineInterrupt_SerialHDLCI,  "SerialHDLCI"},
  {M2R_FR_SONET32RX_LineInterrupt_LREII,        "LREII"},
  {M2R_FR_SONET32RX_LineInterrupt_LEREIEI,      "LEREIEI"},
  {M2R_FR_SONET32RX_LineInterrupt_B2BIPI,       "B2BIPI"},
  {M2R_FR_SONET32RX_LineInterrupt_B2BIPEI,      "B2BIPEI"},
  {M2R_FR_SONET32RX_LineInterrupt_NewZ1S1I,     "NewZ1S1I"},
  {M2R_FR_SONET32RX_LineInterrupt_RDILI,        "RDILI"},
  {M2R_FR_SONET32RX_LineInterrupt_AISLI,        "AISLI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_SONET32RX_SectionInterrupt_irq_id_name_tbl[] = {
  {M2R_FR_SONET32RX_SectionInterrupt_X_LOS_raw_I,       "X_LOS_raw_I"},
  {M2R_FR_SONET32RX_SectionInterrupt_X_LOS_qual_I,      "X_LOS_qual_I"},
  {M2R_FR_SONET32RX_SectionInterrupt_ParityErrI,        "ParityErrI"},
  {M2R_FR_SONET32RX_SectionInterrupt_PRBSErrI,  	"PRBSErrI"},
  {M2R_FR_SONET32RX_SectionInterrupt_SecTrRcvI,         "SecTrRcvI"},
  {M2R_FR_SONET32RX_SectionInterrupt_B1BIPI,    	"B1BIPI"},
  {M2R_FR_SONET32RX_SectionInterrupt_B1BIPEI,   	"B1BIPEI"},
  {M2R_FR_SONET32RX_SectionInterrupt_SecTrMisI,         "SecTrMisI"},
  {M2R_FR_SONET32RX_SectionInterrupt_SecTrUnsI,         "SecTrUnsI"},
  {M2R_FR_SONET32RX_SectionInterrupt_SEFI,      	"SEFI"},
  {M2R_FR_SONET32RX_SectionInterrupt_dZ0InconI,         "dZ0InconI"},
  {M2R_FR_SONET32RX_SectionInterrupt_LOFI,      	"LOFI"},
  {M2R_FR_SONET32RX_SectionInterrupt_LOSI,      	"LOSI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_SONET32RX_APSInterrupt_irq_id_name_tbl[] = {
  {M2R_FR_SONET32RX_APSInterrupt_BadF2I,        "BadF2I"},
  {M2R_FR_SONET32RX_APSInterrupt_NewF2I,        "NewF2I"},
  {M2R_FR_SONET32RX_APSInterrupt_BadK1I,        "BadK1I"},
  {M2R_FR_SONET32RX_APSInterrupt_BadK2I,        "BadK2I"},
  {M2R_FR_SONET32RX_APSInterrupt_BadK3I,        "BadK3I"},
  {M2R_FR_SONET32RX_APSInterrupt_NewK1I,        "NewK1I"},
  {M2R_FR_SONET32RX_APSInterrupt_NewK2I,        "NewK2I"},
  {M2R_FR_SONET32RX_APSInterrupt_NewK3I,        "NewK3I"},
  {M2R_FR_SONET32RX_APSInterrupt_SFI,   	"SFI"},
  {M2R_FR_SONET32RX_APSInterrupt_SDI,   	"SDI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_TRANSMIT_TxInterrupt_irq_id_name_tbl[] = {
  {M2R_FR_TRANSMIT_TxInterrupt_hdlcTxSDCCInterrupt,
   "hdlcTxSDCCInterrupt"},
  {M2R_FR_TRANSMIT_TxInterrupt_hdlcTxLDCCInterrupt,
   "hdlcTxLDCCInterrupt"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_RXHDLC_Interrupt_irq_id_name_tbl[] = {
  {M2R_FR_RXHDLC_Interrupt_FIFODone0I,  	"FIFODone0I"},
  {M2R_FR_RXHDLC_Interrupt_FIFODone1I,  	"FIFODone1I"},
  {M2R_FR_RXHDLC_Interrupt_FIFOUnderflowI,      "FIFOUnderflowI"},
  {M2R_FR_RXHDLC_Interrupt_FIFOOverflowI,       "FIFOOverflowI"},
  {M2R_FR_RXHDLC_Interrupt_CRCFailI,    	"CRCFailI"},
  {M2R_FR_RXHDLC_Interrupt_CRCErrorCountRolloverI,
   "CRCErrorCountRolloverI"},
  {M2R_FR_RXHDLC_Interrupt_EndOfPacketErrorI,   "EndOfPacketErrorI"},
  {M2R_FR_RXHDLC_Interrupt_PacketLengthErrorI,  "PacketLengthErrorI"},
  {M2R_FR_RXHDLC_Interrupt_UnalignedDataErrorI, "UnalignedDataErrorI"}
} ;

const m2r_irq_id_name_t
M2R_GRP_FR_TXHDLC_Interrupt_irq_id_name_tbl[] = {
  {M2R_FR_TXHDLC_Interrupt_FIFODone0I,  	"FIFODone0I"},
  {M2R_FR_TXHDLC_Interrupt_FIFODone1I,  	"FIFODone1I"},
  {M2R_FR_TXHDLC_Interrupt_FIFOUnderflowI,      "FIFOUnderflowI"},
  {M2R_FR_TXHDLC_Interrupt_FIFOOverflowI,       "FIFOOverflowI"}
} ;


/****************************************************************
 * Internal FRAMER functions declarations
 ****************************************************************/
/* this function is to take care of a few intermediate interrutps that
   also need write to clear, no validation to the group */
static
cs_status m2r_framer_clear_imed_irq(cs_uint16 port_id,
            const m2r_irq_ctl_t * imed_tbl, cs_uint32 imed_id,
	    cs_uint16 irq_clr_bits);

/************************************************************************
 * FRAMER IRQ SPECIAL HANDLER
 ***********************************************************************/
cs_uint16 m2r_framer_irq_spl_hndlr(m2r_irq_spl_info_t * irq_spl_info)
{
  m2r_irq_info_t    * irq_info ;
  cs_uint16           status = 0, grp_id, irq_reg_val, irq_id ;

  if (irq_spl_info->irq_oper != M2R_IRQ_SPL_INT) {
    return (0) ;
  }

  irq_info = irq_spl_info->op_mode.irq_hndl.irq_info ;
  irq_reg_val = irq_spl_info->op_mode.irq_hndl.irq_reg_val ;
  irq_id = irq_info->irq_id ;
  grp_id = irq_info->grp_id ;

  switch (grp_id) {
    case M2R_GRP_FR_STREAMTX_CellIntr_0 :
    case M2R_GRP_FR_STREAMTX_CellIntr_1 :
    case M2R_GRP_FR_STREAMTX_CellIntr_2 :
    case M2R_GRP_FR_STREAMTX_CellIntr_3 :
    {
      irq_info->flags |= M2R_IRQ_INFO_STATUS_FLAG ;
      irq_info->status = ((irq_reg_val >> 8) & irq_id) ? 1 : 0 ;
    }
    break ;

    case M2R_GRP_FR_SONET32RX_APSInterrupt :
    {
      switch (irq_id) {
        case M2R_FR_SONET32RX_APSInterrupt_BadK3I :
        case M2R_FR_SONET32RX_APSInterrupt_BadK2I :
        case M2R_FR_SONET32RX_APSInterrupt_BadK1I :
          irq_info->flags |= M2R_IRQ_INFO_STATUS_FLAG ;
          irq_info->status = ((irq_reg_val >> 8) & irq_id) ? 1 : 0 ;
        break ;

        case M2R_FR_SONET32RX_APSInterrupt_BadF2I :
          irq_info->flags |= M2R_IRQ_INFO_STATUS_FLAG ;
          irq_info->status = (irq_reg_val >> 11) ;
        break ;
      }
    }
    break ;
  }

  return (status) ;
}

/************************************************************************
 * GET DEFAULT FAMER IRQ CONFIGURATION
 ***********************************************************************/
cs_status m2r_framer_get_default_irq_cfg(cs_uint16 port_id,
                                         m2r_port_summ_t * pSumm,
                                         m2r_framer_cfg_t * pCfg,
                                         cs_boolean channelized_mode)
{
  cs_uint32  strm_start, strm_end, ii ;

  if ( channelized_mode == TRUE ) {
    strm_start = M2R_SONET_STREAM0 ;
    strm_end = M2R_SONET_STREAM_ALL ;
  }
  else {
    strm_start = M2R_SONET_STREAM0 ;
    strm_end = M2R_SONET_STREAM1 ;
  }

  /************************************************************************/
  /* Set the default IRQs for SONET Section/Line and path                 */
  /************************************************************************/
  /* Enable MBUS timeout IRQ and Reset IRQ ON */
  pCfg->irq_ctl_tbl[M2R_GRP_FR_GLOBAL_MasterInt] =
                        (M2R_FR_GLOBAL_MasterInt_MbusTimeoutI |
                         M2R_FR_GLOBAL_MasterInt_ResetI ) ;

  /* Enable RX/TX clock activity change IRQ */
  pCfg->irq_ctl_tbl[M2R_GRP_FR_GLOBAL_ClockInt] =
                                        (M2R_GRP_FR_GLOBAL_ClockInt_ALL) ;

  /* Set the default IRQ for SONET */
  pCfg->irq_ctl_tbl[M2R_GRP_FR_SONET32RX_LineInterrupt] =
                                (M2R_FR_SONET32RX_LineInterrupt_LREII |
                                 M2R_FR_SONET32RX_LineInterrupt_RDILI |
                                 M2R_FR_SONET32RX_LineInterrupt_AISLI) ;

  /* Section default IRQs */
  pCfg->irq_ctl_tbl[M2R_GRP_FR_SONET32RX_SectionInterrupt] =
                            (M2R_FR_SONET32RX_SectionInterrupt_LOSI       |
                             M2R_FR_SONET32RX_SectionInterrupt_LOFI       |
                             M2R_FR_SONET32RX_SectionInterrupt_SEFI       |
                             M2R_FR_SONET32RX_SectionInterrupt_SecTrUnsI  |
                             M2R_FR_SONET32RX_SectionInterrupt_dZ0InconI  |
                             M2R_FR_SONET32RX_SectionInterrupt_SecTrRcvI  |
                             M2R_FR_SONET32RX_SectionInterrupt_SecTrMisI  ) ;

  /* SONET Path defaut IRQs */
  for(ii = strm_start; ii < strm_end; ii++) {
    pCfg->irq_ctl_tbl[M2R_GRP_FR_SONET32RX_PathInt_0 + ii] =
                    (M2R_FR_SONET32RX_PathInterrupt_LOPI    |
                     M2R_FR_SONET32RX_PathInterrupt_PAISI   |
                     M2R_FR_SONET32RX_PathInterrupt_PRDII   |
                     M2R_FR_SONET32RX_PathInterrupt_PUNEQPI |
                     M2R_FR_SONET32RX_PathInterrupt_PREII   |
                     M2R_FR_SONET32RX_PathInterrupt_PSLUnsI |
                     M2R_FR_SONET32RX_PathInterrupt_PSLMisI |
                     M2R_FR_SONET32RX_PathInterrupt_PTUnsI  |
                     M2R_FR_SONET32RX_PathInterrupt_PTMisI  ) ;

    pCfg->irq_ctl_tbl[M2R_GRP_FR_DATACOMRX_StrInt_0 + ii] =
                                        M2R_GRP_FR_DATACOMRX_StrInt_ALL ;

    pCfg->irq_ctl_tbl[M2R_GRP_FR_DATACOMTX_StrInt_0 + ii] =
                                        M2R_GRP_FR_DATACOMTX_StrInt_ALL ;
  }

  switch(pSumm->l2_prot) {
    case M2R_PORT_PROT_POS :
      /* Set the Default IRQ for POS Mode */
      for(ii = strm_start; ii < strm_end; ii++ ) {
        pCfg->irq_ctl_tbl[M2R_GRP_FR_STREAMRX_PktIntr_0 + ii] =
                                            (M2R_FR_STREAMRX_PktIntr_ALL) ;
        pCfg->irq_ctl_tbl[M2R_GRP_FR_STREAMTX_PktInt_0 + ii] =
                                         (M2R_GRP_FR_STREAMTX_PktInt_ALL) ;
      }
    break ;

    case M2R_PORT_PROT_PPP :
      /* Set the Default IRQ for POS Mode */
      for(ii = strm_start; ii < strm_end; ii++ ) {
        pCfg->irq_ctl_tbl[M2R_GRP_FR_STREAMRX_PktIntr_0 + ii] =
                                            (M2R_FR_STREAMRX_PktIntr_ALL) ;
        pCfg->irq_ctl_tbl[M2R_GRP_FR_STREAMTX_PktInt_0 + ii] =
                                         (M2R_GRP_FR_STREAMTX_PktInt_ALL) ;
      }
    break ;

    case M2R_PORT_PROT_ATM :
      /* Set the Default IRQ for ATM Mode */
      for(ii = strm_start; ii < strm_end; ii++ ) {
        pCfg->irq_ctl_tbl[M2R_GRP_FR_STREAMRX_CellIntr_0 + ii] =
                                    (M2R_GRP_FR_STREAMRX_CellIntr_ALL) ;
        pCfg->irq_ctl_tbl[M2R_GRP_FR_STREAMTX_CellIntr_0 + ii] =
                                    (M2R_GRP_FR_STREAMTX_CellIntr_ALL) ;
      }
    break ;

    case M2R_PORT_PROT_GFP :
      pCfg->svc = M2R_FR_SVC_GFP ;
      /* Set the Default IRQ for GFP Mode */
      pCfg->irq_ctl_tbl[M2R_GRP_FR_GFP_RX_GFP_Intr] =
                                      (M2R_GRP_FR_GFP_RX_GFP_Intr_ALL) ;
      pCfg->irq_ctl_tbl[M2R_GRP_FR_GFP_TX_GFP_Intr] =
                                      (M2R_GRP_FR_GFP_TX_GFP_Intr_ALL) ;
    break ;

    case M2R_PORT_PROT_CLEAR_CH :
    default :
    break ;
  }
  return (CS_OK) ;
}


/************************************************************************
 * INIT FRAMER IRQ
 ***********************************************************************/
cs_status m2r_framer_init_irq(cs_uint16 port_id,
                              m2r_port_summ_t * pSumm,
                              m2r_framer_cfg_t * pCfg)
{
  M2R_t                   * pDev ;
  cs_uint16                 sl_num, ii ;
/*
  cs_reg                  * regaddr ;
  FR_SONET32RX_PathMasks_t  pathMask ;
  FR_GLOBAL_MasterMask_t    masterMask ;
  FR_GLOBAL_MasterMask2_t   master2Mask ;
  FR_DATACOMRX_Mask_t       datacomRxMask ;
  FR_DATACOMTX_Mask_t       datacomTxMask ;
*/
  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  /* Register IRQ Table */
  M2R_REGISTER_IRQ_TBL(port_id, M2R_ID_FRAMER, m2r_fr_irq_tbl,
                       m2r_fr_irq_grp_name_tbl, M2R_FR_MAX_GRP,
		       m2r_framer_irq_spl_hndlr) ;

  M2R_REGISTER_IMED_TBL(port_id, M2R_ID_FRAMER, m2r_fr_imed_irq_tbl,
                        m2r_fr_irq_imed_name_tbl, M2R_FR_MAX_IMED,
			NULL) ;

  /* Enable the IRQs which are requested to be enabled by default */
  /* first, clear intermediate interrupts that needs write to clear */
  m2r_framer_clear_imed_irq(port_id, m2r_fr_imed_irq_tbl,
    M2R_IMED_FR_SONET32RX_SectionInterrupt,
    M2R_IMED_FR_SONET32RX_SectionInterrupt_ALL);

  m2r_framer_clear_imed_irq(port_id, m2r_fr_imed_irq_tbl,
    M2R_IMED_FR_SONET32RX_LineInterrupt,
    M2R_IMED_FR_SONET32RX_LineInterrupt_ALL);

  m2r_framer_clear_imed_irq(port_id, m2r_fr_imed_irq_tbl,
    M2R_IMED_FR_TRANSMIT_TxInterrupt,
    M2R_IMED_FR_TRANSMIT_TxInterrupt_ALL);

  for (ii = 0; ii < M2R_FR_MAX_GRP; ii++) {
    /* Clear any interrupts */
    m2r_clear_leaf_irq(port_id, M2R_ID_FRAMER, ii,
                         m2r_fr_irq_tbl[ii].irq_map) ;

    if (pCfg->irq_ctl_tbl[ii]) {
      m2r_enable_leaf_irq(port_id, M2R_ID_FRAMER, ii,
                          pCfg->irq_ctl_tbl[ii]) ;
    }
  }

  /* Enable all the intermediate interrupt controls by default */
  for (ii = 0; ii < M2R_FR_MAX_IMED; ii++) {
    m2r_enable_imed_irq(port_id, M2R_ID_FRAMER, ii,
                        m2r_fr_imed_irq_tbl[ii].irq_map) ;
  }

#if 0
  /* Enable path IRQ groups. If Normal mode, only stream-0 Path IRQ
   * group is enabled else all the streams are enabled.
   */
  irq_map = &pCfg->irq_ctl_tbl[M2R_GRP_FR_SONET32RX_PathInt_0] ;

  regaddr = (cs_reg*) &pDev->slice[sl_num].Framer.PathMasks.wrd ;
  pathMask.wrd = M2R_REG_READ(regaddr) ;

  for(ii = 0; ii < M2R_SONET_STREAM_ALL; ii++ ) {
    pathMask.bf.PathM &= ((*irq_map) ?  (~(1 << ii)) : 0xffff) ;
    irq_map++ ;
  }
  M2R_REG_WRITE(regaddr, pathMask.wrd) ;

  /* Datacom Stream Group */
  irq_map = &pCfg->irq_ctl_tbl[M2R_GRP_FR_DATACOMRX_StrInt_0] ;

  regaddr = (cs_reg*) &pDev->slice[sl_num].Framer.datacomRx_Mask.wrd ;
  datacomRxMask.wrd = M2R_REG_READ(regaddr) ;

  for(ii = 0; ii < M2R_SONET_STREAM_ALL; ii++ ) {
    datacomRxMask.bf.streamRxM &= ((*irq_map) ?  (~(1 << ii)) : 0xffff) ;
    irq_map++ ;
  }
  M2R_REG_WRITE(regaddr, datacomRxMask.wrd) ;

  /* DataCom TX Groups IRQ */
  irq_map = &pCfg->irq_ctl_tbl[M2R_GRP_FR_DATACOMTX_StrInt_0] ;

  regaddr = (cs_reg*) &pDev->slice[sl_num].Framer.datacomTx_Mask.wrd ;
  datacomTxMask.wrd = M2R_REG_READ(regaddr) ;

  for(ii = 0; ii < M2R_SONET_STREAM_ALL; ii++ ) {
    datacomTxMask.bf.streamTxM &= ((*irq_map) ?  (~(1 << ii)) : 0xffff) ;
    irq_map++ ;
  }
  M2R_REG_WRITE(regaddr, datacomTxMask.wrd) ;

  /* Enable mapper IRQs groups based on channelized Vs Normal Mode */
  switch(pSumm->l2_prot) {
    case M2R_PORT_PROT_POS :
    case M2R_PORT_PROT_PPP : {
      FR_GLOBAL_PacketRxMask_t pktRxMask ;
      FR_GLOBAL_PacketTxMask_t pktTxMask ;

      irq_map = &pCfg->irq_ctl_tbl[M2R_GRP_FR_STREAMRX_PktIntr_0] ;
      regaddr = (cs_reg*) &pDev->slice[sl_num].Framer.PacketRxMask.wrd ;
      pktRxMask.wrd = M2R_REG_READ(regaddr) ;

      for(ii = 0; ii < M2R_SONET_STREAM_ALL; ii++ ) {
        pktRxMask.bf.PacketRxM &= ((*irq_map) ?  (~(1 << ii)) : 0xffff) ;
        irq_map++ ;
      }
      M2R_REG_WRITE(regaddr, pktRxMask.wrd) ;

      /* Pkt TX group */
      irq_map = &pCfg->irq_ctl_tbl[M2R_GRP_FR_STREAMTX_PktInt_0] ;
      regaddr = (cs_reg*) &pDev->slice[sl_num].Framer.PacketTxMask.wrd ;
      pktTxMask.wrd = M2R_REG_READ(regaddr) ;

      for(ii = 0; ii < M2R_SONET_STREAM_ALL; ii++ ) {
        pktTxMask.bf.PacketTxM &= ((*irq_map) ?  (~(1 << ii)) : 0xffff) ;
        irq_map++ ;
      }
      M2R_REG_WRITE(regaddr, pktTxMask.wrd) ;
    }
    break ;

    case M2R_PORT_PROT_ATM : {
      FR_GLOBAL_CellRxMask_t cellRxMask ;
      FR_GLOBAL_CellTxMask_t cellTxMask ;

      irq_map = &pCfg->irq_ctl_tbl[M2R_GRP_FR_STREAMRX_CellIntr_0] ;
      regaddr = (cs_reg*) &pDev->slice[sl_num].Framer.CellRxMask.wrd ;
      cellRxMask.wrd = M2R_REG_READ(regaddr) ;

      for(ii = 0; ii < M2R_SONET_STREAM_ALL; ii++ ) {
        cellRxMask.bf.CellRxM &= ((*irq_map) ?  (~(1 << ii)) : 0xffff) ;
        irq_map++ ;
      }
      M2R_REG_WRITE(regaddr, cellRxMask.wrd) ;

      /* Enable TX Cell Group IRQs */
      irq_map = &pCfg->irq_ctl_tbl[M2R_GRP_FR_STREAMTX_CellIntr_0] ;
      regaddr = (cs_reg*) &pDev->slice[sl_num].Framer.CellTxMask.wrd ;
      cellTxMask.wrd = M2R_REG_READ(regaddr) ;

      for(ii = 0; ii < M2R_SONET_STREAM_ALL; ii++ ) {
        cellTxMask.bf.CellTxM &= ((*irq_map) ?  (~(1 << ii)) : 0xffff) ;
        irq_map++ ;
      }
      M2R_REG_WRITE(regaddr, cellTxMask.wrd) ;
    }
    break ;

    case M2R_PORT_PROT_GFP :
    case M2R_PORT_PROT_CLEAR_CH :
    default :
    break ;
  }

  masterMask.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.MasterMask.wrd) ;
  masterMask.wrd &= (M2R_GRP_FR_GLOBAL_MasterInt_ALL) ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.MasterMask.wrd, masterMask.wrd) ;

  master2Mask.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.MasterMask2.wrd) ;
  master2Mask.wrd = 0 ;
  M2R_REG_WRITE(&pDev->slice[sl_num].Framer.MasterMask2.wrd, master2Mask.wrd) ;

#endif

  return (CS_OK) ;
}

/************************************************************************/
/*                  ISR ROUTINE FOR FRAMER BLOCK                        */
/************************************************************************/
cs_status m2r_framer_isr(cs_uint16 port_id)
{
  M2R_t                  * pDev ;
  cs_uint16                sl_num ;
  FR_GLOBAL_MasterInt_t    masterInt ;
  FR_GLOBAL_MasterInt2_t   masterInt2 ;
  cs_uint8                 strm ;

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id) ;
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  masterInt.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.MasterInt.wrd) ;
  masterInt2.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.MasterInt2.wrd) ;

  if (masterInt.wrd & M2R_GRP_FR_GLOBAL_MasterInt_ALL) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
          M2R_GRP_FR_GLOBAL_MasterInt,
          &m2r_fr_irq_tbl[M2R_GRP_FR_GLOBAL_MasterInt],
          m2r_fr_irq_grp_name_tbl[M2R_GRP_FR_GLOBAL_MasterInt],
	  M2R_GRP_FR_GLOBAL_MasterInt_irq_id_name_tbl,
	  sizeof(M2R_GRP_FR_GLOBAL_MasterInt_irq_id_name_tbl) /
	  sizeof(m2r_irq_id_name_t)) ;
  }

  if(masterInt.bf.APSI) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
          M2R_GRP_FR_SONET32RX_APSInterrupt,
          &m2r_fr_irq_tbl[M2R_GRP_FR_SONET32RX_APSInterrupt],
	  m2r_fr_irq_grp_name_tbl[M2R_GRP_FR_SONET32RX_APSInterrupt],
	  M2R_GRP_FR_SONET32RX_APSInterrupt_irq_id_name_tbl,
	  sizeof(M2R_GRP_FR_SONET32RX_APSInterrupt_irq_id_name_tbl) /
	  sizeof(m2r_irq_id_name_t)) ;
  }

  if(masterInt.bf.RxPI) {
    FR_SONET32RX_PathInterrupts_t  pathInt ;

    pathInt.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.PathInterrupts.wrd) ;

    for(strm = M2R_SONET_STREAM0; strm < M2R_SONET_STREAM_ALL; strm++) {
      if(pathInt.bf.PathI & (1 << strm)) {
        m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
          (M2R_GRP_FR_SONET32RX_PathInt_0 + strm),
	  &m2r_fr_irq_tbl[(M2R_GRP_FR_SONET32RX_PathInt_0 +
			   strm)],
          m2r_fr_irq_grp_name_tbl[(M2R_GRP_FR_SONET32RX_PathInt_0 +
				   strm)],
	  M2R_GRP_FR_SONET32RX_PathInt_irq_id_name_tbl,
	  sizeof(M2R_GRP_FR_SONET32RX_PathInt_irq_id_name_tbl) /
	  sizeof(m2r_irq_id_name_t)) ;
      }
    }

    /* clear sticky imed interrupt bit */
    m2r_framer_clear_imed_irq(port_id, m2r_fr_imed_irq_tbl,
        M2R_IMED_FR_SONET32RX_PathInterrupts,
        M2R_IMED_FR_SONET32RX_PathInterrupts_ALL);
  }

  if(masterInt.bf.RxLI) {
    FR_SONET32RX_LineInterrupt_t lineInt;
    lineInt.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.LineInterrupt.wrd) ;

    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
        M2R_GRP_FR_SONET32RX_LineInterrupt,
	&m2r_fr_irq_tbl[M2R_GRP_FR_SONET32RX_LineInterrupt],
	m2r_fr_irq_grp_name_tbl[M2R_GRP_FR_SONET32RX_LineInterrupt],
	M2R_GRP_FR_SONET32RX_LineInterrupt_irq_id_name_tbl,
	sizeof(M2R_GRP_FR_SONET32RX_LineInterrupt_irq_id_name_tbl) /
	sizeof(m2r_irq_id_name_t)) ;

    if (lineInt.bf.SerialHDLCI) {
      m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
              M2R_GRP_FR_RXHDLC_Interrupt_Line,
              &m2r_fr_irq_tbl[M2R_GRP_FR_RXHDLC_Interrupt_Line],
	      m2r_fr_irq_grp_name_tbl[M2R_GRP_FR_RXHDLC_Interrupt_Line],
	      M2R_GRP_FR_RXHDLC_Interrupt_irq_id_name_tbl,
	      sizeof(M2R_GRP_FR_RXHDLC_Interrupt_irq_id_name_tbl) /
	      sizeof(m2r_irq_id_name_t)) ;
    }

    /* clear sticky imed interrupt bit */
    m2r_framer_clear_imed_irq(port_id, m2r_fr_imed_irq_tbl,
        M2R_IMED_FR_SONET32RX_LineInterrupt,
        M2R_IMED_FR_SONET32RX_LineInterrupt_ALL);
  }

  if(masterInt.bf.RxSI) {
    FR_SONET32RX_SectionInterrupt_t sectionInt;
    sectionInt.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.SectionInterrupt.wrd) ;

    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
        M2R_GRP_FR_SONET32RX_SectionInterrupt,
        &m2r_fr_irq_tbl[M2R_GRP_FR_SONET32RX_SectionInterrupt],
	m2r_fr_irq_grp_name_tbl[M2R_GRP_FR_SONET32RX_SectionInterrupt],
	M2R_GRP_FR_SONET32RX_SectionInterrupt_irq_id_name_tbl,
	sizeof(M2R_GRP_FR_SONET32RX_SectionInterrupt_irq_id_name_tbl) /
	sizeof(m2r_irq_id_name_t)) ;

    if (sectionInt.bf.SerialHDLCI) {
      m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
              M2R_GRP_FR_RXHDLC_Interrupt_Section,
              &m2r_fr_irq_tbl[M2R_GRP_FR_RXHDLC_Interrupt_Section],
	      m2r_fr_irq_grp_name_tbl[M2R_GRP_FR_RXHDLC_Interrupt_Section],
	      M2R_GRP_FR_RXHDLC_Interrupt_irq_id_name_tbl,
	      sizeof(M2R_GRP_FR_RXHDLC_Interrupt_irq_id_name_tbl) /
	      sizeof(m2r_irq_id_name_t)) ;
    }

    /* clear sticky imed interrupt bit */
    m2r_framer_clear_imed_irq(port_id, m2r_fr_imed_irq_tbl,
        M2R_IMED_FR_SONET32RX_SectionInterrupt,
        M2R_IMED_FR_SONET32RX_SectionInterrupt_ALL);
  }

  if(masterInt2.bf.DatacomRxI) {
    FR_DATACOMRX_Interrupt_t  rxDcomInt ;
    rxDcomInt.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.datacomRx_Interrupt.wrd) ;
    for(strm = M2R_SONET_STREAM0; strm < M2R_SONET_STREAM_ALL; strm++) {
      if(rxDcomInt.bf.streamRxS & (1 << strm)) {
        m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
          (M2R_GRP_FR_DATACOMRX_StrInt_0 + strm),
	&m2r_fr_irq_tbl[(M2R_GRP_FR_DATACOMRX_StrInt_0 +
			 strm)],
        m2r_fr_irq_grp_name_tbl[(M2R_GRP_FR_DATACOMRX_StrInt_0 +
				 strm)],
        M2R_GRP_FR_DATACOMRX_StrInt_irq_id_name_tbl,
	sizeof(M2R_GRP_FR_DATACOMRX_StrInt_irq_id_name_tbl) /
	sizeof(m2r_irq_id_name_t)) ;
      }
    }
  }

  if(masterInt2.bf.StreamRx_GFP_I) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
                 M2R_GRP_FR_GFP_RX_GFP_Intr,
                 &m2r_fr_irq_tbl[M2R_GRP_FR_GFP_RX_GFP_Intr],
		 m2r_fr_irq_grp_name_tbl[M2R_GRP_FR_GFP_RX_GFP_Intr],
		 M2R_GRP_FR_GFP_RX_GFP_Intr_irq_id_name_tbl,
		 sizeof(M2R_GRP_FR_GFP_RX_GFP_Intr_irq_id_name_tbl) /
		 sizeof(m2r_irq_id_name_t)) ;
  }

  if(masterInt.bf.RxPktPI) {
    FR_GLOBAL_PacketRxInt_t     rxPktInt ;

    rxPktInt.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.PacketRxInt.wrd) ;

    for(strm = M2R_SONET_STREAM0; strm < M2R_SONET_STREAM_ALL; strm++) {
      if(rxPktInt.bf.PacketRxI & (1 << strm)) {
        m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
              (M2R_GRP_FR_STREAMRX_PktIntr_0 + strm),
	      &m2r_fr_irq_tbl[(M2R_GRP_FR_STREAMRX_PktIntr_0 +
			       strm)],
              m2r_fr_irq_grp_name_tbl[(M2R_GRP_FR_STREAMRX_PktIntr_0
				       + strm)],
	      M2R_FR_STREAMRX_PktIntr_irq_id_name_tbl,
	      sizeof(M2R_FR_STREAMRX_PktIntr_irq_id_name_tbl) /
	      sizeof(m2r_irq_id_name_t)) ;
      }
    }
  }

  if(masterInt.bf.RxCellPI) {
    FR_GLOBAL_CellRxInt_t       rxCellInt ;
    rxCellInt.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.CellRxInt.wrd) ;

    for(strm = M2R_SONET_STREAM0; strm < M2R_SONET_STREAM_ALL; strm++) {
      if(rxCellInt.bf.CellRxI  & (1 << strm)) {
        m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
               (M2R_GRP_FR_STREAMRX_CellIntr_0 + strm),
               &m2r_fr_irq_tbl[(M2R_GRP_FR_STREAMRX_CellIntr_0 +
				strm)],
               m2r_fr_irq_grp_name_tbl[(M2R_GRP_FR_STREAMRX_CellIntr_0 +
					strm)],
	       M2R_GRP_FR_STREAMRX_CellIntr_irq_id_name_tbl,
	       sizeof(M2R_GRP_FR_STREAMRX_CellIntr_irq_id_name_tbl) /
	       sizeof(m2r_irq_id_name_t)) ;
      }
    }
  }

  if(masterInt.bf.TxI) {
    FR_TRANSMIT_TxInterrupt_t txInt;
    txInt.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.TxInterrupt.wrd) ;

    if (txInt.bf.hdlcTxLDCCInterrupt) {
      m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
              M2R_GRP_FR_TXHDLC_Interrupt_Line,
              &m2r_fr_irq_tbl[M2R_GRP_FR_TXHDLC_Interrupt_Line],
	      m2r_fr_irq_grp_name_tbl[M2R_GRP_FR_TXHDLC_Interrupt_Line],
	      M2R_GRP_FR_TXHDLC_Interrupt_irq_id_name_tbl,
	      sizeof(M2R_GRP_FR_TXHDLC_Interrupt_irq_id_name_tbl) /
	      sizeof(m2r_irq_id_name_t)) ;
    }

    if (txInt.bf.hdlcTxSDCCInterrupt) {
      m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
              M2R_GRP_FR_TXHDLC_Interrupt_Section,
              &m2r_fr_irq_tbl[M2R_GRP_FR_TXHDLC_Interrupt_Section],
	      m2r_fr_irq_grp_name_tbl[M2R_GRP_FR_TXHDLC_Interrupt_Section],
	      M2R_GRP_FR_TXHDLC_Interrupt_irq_id_name_tbl,
	      sizeof(M2R_GRP_FR_TXHDLC_Interrupt_irq_id_name_tbl) /
	      sizeof(m2r_irq_id_name_t)) ;
    }

    /* clear sticky imed interrupt bit */
    m2r_framer_clear_imed_irq(port_id, m2r_fr_imed_irq_tbl,
      M2R_IMED_FR_TRANSMIT_TxInterrupt,
      M2R_IMED_FR_TRANSMIT_TxInterrupt_ALL);
  }

  if(masterInt.bf.DatacomTxI) {
    FR_DATACOMTX_Interrupt_t  txDcomInt ;
    txDcomInt.wrd =
        M2R_REG_READ(&pDev->slice[sl_num].Framer.datacomTx_Interrupt.wrd) ;
    for(strm = M2R_SONET_STREAM0; strm < M2R_SONET_STREAM_ALL; strm++) {
      if(txDcomInt.bf.streamTxS & (1 << strm)) {
        m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
              (M2R_GRP_FR_DATACOMTX_StrInt_0 + strm),
	      &m2r_fr_irq_tbl[(M2R_GRP_FR_DATACOMTX_StrInt_0 +
			       strm)],
	      m2r_fr_irq_grp_name_tbl[(M2R_GRP_FR_DATACOMTX_StrInt_0 +
				       strm)],
	      M2R_GRP_FR_DATACOMTX_StrInt_irq_id_name_tbl,
	      sizeof(M2R_GRP_FR_DATACOMTX_StrInt_irq_id_name_tbl) /
	      sizeof(m2r_irq_id_name_t)) ;
      }
    }
  }

  if(masterInt.bf.TxPktPI) {
    FR_GLOBAL_PacketTxInt_t  txPktInt ;
    txPktInt.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.PacketTxInt.wrd) ;
    for(strm = M2R_SONET_STREAM0; strm < M2R_SONET_STREAM_ALL; strm++) {
      if(txPktInt.bf.PacketTxI & (1 << strm)) {
        m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
              (M2R_GRP_FR_STREAMTX_PktInt_0 + strm),
              &m2r_fr_irq_tbl[(M2R_GRP_FR_STREAMTX_PktInt_0
			       + strm)],
              m2r_fr_irq_grp_name_tbl[(M2R_GRP_FR_STREAMTX_PktInt_0 +
				       strm)],
	      M2R_GRP_FR_STREAMTX_PktInt_irq_id_name_tbl,
	      sizeof(M2R_GRP_FR_STREAMTX_PktInt_irq_id_name_tbl) /
	      sizeof(m2r_irq_id_name_t)) ;
      }
    }
  }

  if(masterInt2.bf.StreamTx_GFP_I) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
                M2R_GRP_FR_GFP_TX_GFP_Intr,
                &m2r_fr_irq_tbl[M2R_GRP_FR_GFP_TX_GFP_Intr],
		m2r_fr_irq_grp_name_tbl[M2R_GRP_FR_GFP_TX_GFP_Intr],
		M2R_GRP_FR_GFP_TX_GFP_Intr_irq_id_name_tbl,
		sizeof(M2R_GRP_FR_GFP_TX_GFP_Intr_irq_id_name_tbl) /
		sizeof(m2r_irq_id_name_t)) ;
  }

  if(masterInt.bf.TxCellPI) {
    FR_GLOBAL_CellTxInt_t       txCellInt ;

    txCellInt.wrd = M2R_REG_READ(&pDev->slice[sl_num].Framer.CellTxInt.wrd) ;

    for(strm = M2R_SONET_STREAM0; strm < M2R_SONET_STREAM_ALL; strm++) {
      if(txCellInt.bf.CellTxI & (1 << strm)) {
        m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
               (M2R_GRP_FR_STREAMTX_CellIntr_0 + strm),
	       &m2r_fr_irq_tbl[(M2R_GRP_FR_STREAMTX_CellIntr_0 +
				strm)],
	       m2r_fr_irq_grp_name_tbl[(M2R_GRP_FR_STREAMTX_CellIntr_0 +
					strm)],
	       M2R_GRP_FR_STREAMTX_CellIntr_irq_id_name_tbl,
	       sizeof(M2R_GRP_FR_STREAMTX_CellIntr_irq_id_name_tbl) /
	       sizeof(m2r_irq_id_name_t)) ;
      }
    }
  }

  if(masterInt.bf.GPIOI) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
               M2R_GRP_FR_GLOBAL_GPIOInt,
               &m2r_fr_irq_tbl[M2R_GRP_FR_GLOBAL_GPIOInt],
	       m2r_fr_irq_grp_name_tbl[M2R_GRP_FR_GLOBAL_GPIOInt],
	       M2R_GRP_FR_GLOBAL_GPIOInt_irq_id_name_tbl,
	       sizeof(M2R_GRP_FR_GLOBAL_GPIOInt_irq_id_name_tbl) /
	       sizeof(m2r_irq_id_name_t)) ;

  }

  if(masterInt.bf.ClkFailI) {
    m2r_irq_grp_hndlr(port_id, (cs_uint64)pDev, M2R_ID_FRAMER,
               M2R_GRP_FR_GLOBAL_ClockInt,
	       &m2r_fr_irq_tbl[M2R_GRP_FR_GLOBAL_ClockInt],
	       m2r_fr_irq_grp_name_tbl[M2R_GRP_FR_GLOBAL_ClockInt],
	       M2R_GRP_FR_GLOBAL_ClockInt_irq_id_name_tbl,
	       sizeof(M2R_GRP_FR_GLOBAL_ClockInt_irq_id_name_tbl) /
	       sizeof(m2r_irq_id_name_t)) ;
  }

  return(CS_OK) ;
}

/* caller doesn't need to worry about whether the bits to clear are there to
 * begin with. The actual clear only applies to the active one.  No chance of
 * clear int(s) by accident.
 */
static
cs_status m2r_framer_clear_imed_irq(cs_uint16 port_id,
	    const m2r_irq_ctl_t * imed_tbl,
            cs_uint32 imed_id, cs_uint16 irq_clr_bits)
{
  M2R_t             * pDev ;
  cs_uint8            sl_num ;
  volatile cs_reg   * irq_reg ;
  volatile cs_reg     clear_irq ;
  const m2r_irq_ctl_t * irq_cb = &imed_tbl[imed_id];
  /*
   * Get the pointer to m2r_irq_ctl_t in the table using grp_id as index into
   * blk_irq_cb table.
   */

  pDev = M2R_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = M2R_PORT_ID_TO_SLICE_NUM(port_id);

  M2R_PORT_LOCK(port_id, M2R_ID_FRAMER);

  irq_reg = (volatile cs_reg *)(((cs_uint64) pDev) +
                 (sl_num * M2R_SLICE_SIZE) + irq_cb->offset_intr_reg) ;

  if ( irq_cb->flags & M2R_IRQ_FLG_RW1C ) { /* RWIC */
    clear_irq = (irq_clr_bits & irq_cb->irq_map) ;
    clear_irq &= M2R_REG_READ(irq_reg);
    M2R_REG_WRITE(irq_reg, clear_irq) ;
  }

  M2R_PORT_UNLOCK(port_id, M2R_ID_FRAMER);
  return (CS_OK) ;
}


