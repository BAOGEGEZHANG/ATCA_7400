/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * arn5_framer_irq.c
 *
 * This file is released on Wed Nov 23 22:30:04 2005.
 */
#include "cs_types.h"
#include "arn5_common.h"
#include "arn5_public.h"
#include "arn5_framer.h"
#include "arn5_framer_priv.h"
#include "arn5_irq_priv.h"
#include "arn5_framer_irq.h"

/*********************************************************************/
/*    IRQ table                                                      */
/*********************************************************************/
const arn5_irq_ctl_t  arn5_fr_irq_tbl[ARN5_FR_MAX_GRP] = {
  [ARN5_GRP_FR_GLOBAL_MasterInt] = {
    ARN5_FR_REG_OFST(MasterInt, 0, 0), 0,
    ARN5_FR_REG_OFST(MasterMask, 0, 0),
    ARN5_GRP_FR_GLOBAL_MasterInt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_GLOBAL_ClockInt] = {
    ARN5_FR_REG_OFST(ClockInt, 0, 0),
    ARN5_FR_REG_OFST(ClockStatus, 0, 0),
    ARN5_FR_REG_OFST(ClockMask, 0, 0),
    ARN5_GRP_FR_GLOBAL_ClockInt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_DATACOMRX_StrInt] = {
    ARN5_FR_REG_OFST(datacomRx_StrInt, 0, 0),
    ARN5_FR_REG_OFST(datacomRx_StrStatus, 0, 0),
    ARN5_FR_REG_OFST(datacomRx_StrMask, 0, 0),
    ARN5_GRP_FR_DATACOMRX_StrInt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_DATACOMTX_StrInt] = {
    ARN5_FR_REG_OFST(datacomTx_StrInt, 0, 0),
    ARN5_FR_REG_OFST(datacomTx_StrStatus, 0, 0),
    ARN5_FR_REG_OFST(datacomTx_StrMask, 0, 0),
    ARN5_GRP_FR_DATACOMTX_StrInt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_STREAMRX_CellIntr] = {
    ARN5_FR_REG_OFST(streamRx_CellIntr, 0, 0),
    ARN5_FR_REG_OFST(CellSts, 0, 0),
    ARN5_FR_REG_OFST(CellMask, 0, 0),
    ARN5_GRP_FR_STREAMRX_CellIntr_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_STREAMTX_CellIntr] = {
    ARN5_FR_REG_OFST(streamTx_CellIntr, 0, 0),
    0,
    ARN5_FR_REG_OFST(CellIMask, 0, 0),
    ARN5_GRP_FR_STREAMTX_CellIntr_ALL,
    ARN5_GRP_FR_STREAMTX_CellIntr_ALL,
    (ARN5_IRQ_FLG_RW1C | ARN5_IRQ_FLG_SPL_INT ), ARN5_MAX_NUM_PORTS, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_STREAMRX_PktIntr] = {
    ARN5_FR_REG_OFST(PktIntr, 0, 0),
    ARN5_FR_REG_OFST(PktStatus, 0, 0),
    ARN5_FR_REG_OFST(streamRx_PktMask, 0, 0),
    ARN5_GRP_STREAMRX_PktIntr_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_STREAMTX_PktInt] = {
    ARN5_FR_REG_OFST(PktInt, 0, 0),
    ARN5_FR_REG_OFST(PktSts, 0, 0),
    ARN5_FR_REG_OFST(streamTx_PktMask, 0, 0),
    ARN5_GRP_FR_STREAMTX_PktInt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_GFP_RX_GFP_Intr] = {
    ARN5_FR_REG_OFST(gfp_rx_GFP_Intr, 0, 0),
    ARN5_FR_REG_OFST(gfp_rx_GFP_Status, 0, 0),
    ARN5_FR_REG_OFST(gfp_rx_GFP_Mask, 0, 0),
    ARN5_GRP_FR_GFP_RX_GFP_Intr_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_GFP_TX_GFP_Intr] = {
    ARN5_FR_REG_OFST(gfp_tx_GFP_Intr, 0, 0),
    ARN5_FR_REG_OFST(gfp_tx_GFP_Status, 0, 0),
    ARN5_FR_REG_OFST(gfp_tx_GFP_Mask, 0, 0),
    ARN5_GRP_FR_GFP_TX_GFP_Intr_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_SONET32RX_PathInt] = {
    ARN5_FR_REG_OFST(PathInt, 0, 0),
    ARN5_FR_REG_OFST(PathStatus, 0, 0),
    ARN5_FR_REG_OFST(PathMask, 0, 0),
    ARN5_GRP_FR_SONET32RX_PathInt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_SONET32RX_LineInterrupt] = {
    ARN5_FR_REG_OFST(LineInterrupt, 0, 0),
    ARN5_FR_REG_OFST(LineStatus, 0, 0),
    ARN5_FR_REG_OFST(LineMask, 0, 0),
    ARN5_GRP_FR_SONET32RX_LineInterrupt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_SONET32RX_SectionInterrupt] = {
    ARN5_FR_REG_OFST(SectionInterrupt, 0, 0),
    ARN5_FR_REG_OFST(SectionStatus, 0, 0),
    ARN5_FR_REG_OFST(SectionMask, 0, 0),
    ARN5_GRP_FR_SONET32RX_SectionInterrupt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE},

  [ARN5_GRP_FR_SONET32RX_APSInterrupt] = {
    ARN5_FR_REG_OFST(APSInterrupt, 0, 0),
    0,
    ARN5_FR_REG_OFST(APSMask, 0, 0),
    ARN5_GRP_FR_SONET32RX_APSInterrupt_ALL, 0x08E0,
    (ARN5_IRQ_FLG_RW1C | ARN5_IRQ_FLG_SPL_INT), ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_RXHDLC_Interrupt_Section] = {
    ARN5_FR_REG_OFST(rxHDLC_Interrupt, 0, 0),
    0,
    ARN5_FR_REG_OFST(rxHDLC_Mask, 0, 0),
    ARN5_GRP_FR_RXHDLC_Interrupt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_TXHDLC_Interrupt_Section] = {
    ARN5_FR_REG_OFST(txHDLC_Interrupt, 0, 0),
    0,
    ARN5_FR_REG_OFST(txHDLC_Mask, 0, 0),
    ARN5_GRP_FR_TXHDLC_Interrupt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_RXHDLC_Interrupt_Line] = {
    ARN5_FR_REG_OFST(rxHDLC_Interrupt, 1, ARN5_FR_RXHDLC__STRIDE),
    0,
    ARN5_FR_REG_OFST(rxHDLC_Mask, 1, ARN5_FR_RXHDLC__STRIDE),
    ARN5_GRP_FR_RXHDLC_Interrupt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_GRP_FR_TXHDLC_Interrupt_Line] = {
    ARN5_FR_REG_OFST(txHDLC_Interrupt, 1, ARN5_FR_TXHDLC__STRIDE),
    0,
    ARN5_FR_REG_OFST(txHDLC_Mask, 1, ARN5_FR_TXHDLC__STRIDE),
    ARN5_GRP_FR_TXHDLC_Interrupt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },
} ;


const arn5_irq_ctl_t  arn5_fr_imed_irq_tbl[ARN5_FR_MAX_IMED] = {
  [ARN5_IMED_FR_GLOBAL_MasterInt] = {
    ARN5_FR_REG_OFST(MasterInt, 0, 0),
    0,
    ARN5_FR_REG_OFST(MasterMask, 0, 0),
    ARN5_IMED_FR_GLOBAL_MasterInt_ALL, 0,
    0, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_IMED_FR_GLOBAL_MasterInt2] = {
    ARN5_FR_REG_OFST(MasterInt2, 0, 0),
    0,
    ARN5_FR_REG_OFST(MasterMask2, 0, 0),
    ARN5_IMED_FR_GLOBAL_MasterInt2_ALL, 0,
    0, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_IMED_FR_GLOBAL_CellRxInt] = {
    ARN5_FR_REG_OFST(CellRxInt, 0, 0),
    0,
    ARN5_FR_REG_OFST(CellRxMask, 0, 0),
    ARN5_IMED_FR_GLOBAL_CellRxInt_ALL, 0,
    0,ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_IMED_FR_GLOBAL_CellTxInt] = {
    ARN5_FR_REG_OFST(CellTxInt, 0, 0),
    0,
    ARN5_FR_REG_OFST(CellTxMask, 0, 0),
    ARN5_IMED_FR_GLOBAL_CellTxInt_ALL, 0,
    0, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_IMED_FR_GLOBAL_PacketRxInt] = {
    ARN5_FR_REG_OFST(PacketRxInt, 0, 0),
    0,
    ARN5_FR_REG_OFST(PacketRxMask, 0, 0),
    ARN5_IMED_FR_GLOBAL_PacketRxInt_ALL, 0,
    0, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_IMED_FR_GLOBAL_PacketTxInt] = {
    ARN5_FR_REG_OFST(PacketTxInt, 0, 0),
    0,
    ARN5_FR_REG_OFST(PacketTxMask, 0, 0),
    ARN5_IMED_FR_GLOBAL_PacketTxInt_ALL, 0,
    0, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_IMED_FR_DATACOMRX_Interrupt] = {
    ARN5_FR_REG_OFST(datacomRx_Interrupt, 0, 0),
    0,
    ARN5_FR_REG_OFST(datacomRx_Mask, 0, 0),
    ARN5_IMED_FR_DATACOMRX_Interrupt_ALL, 0,
    0, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_IMED_FR_DATACOMTX_Interrupt] = {
    ARN5_FR_REG_OFST(datacomTx_Interrupt, 0, 0),
    0,
    ARN5_FR_REG_OFST(datacomTx_Mask, 0, 0),
    ARN5_IMED_FR_DATACOMTX_Interrupt_ALL, 0,
    0, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_IMED_FR_SONET32RX_SectionInterrupt] = {
    ARN5_FR_REG_OFST(SectionInterrupt, 0, 0),
    0,
    ARN5_FR_REG_OFST(SectionMask, 0, 0),
    ARN5_IMED_FR_SONET32RX_SectionInterrupt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_IMED_FR_SONET32RX_LineInterrupt] = {
    ARN5_FR_REG_OFST(LineInterrupt, 0, 0),
    0,
    ARN5_FR_REG_OFST(LineMask, 0, 0),
    ARN5_IMED_FR_SONET32RX_LineInterrupt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_IMED_FR_SONET32RX_PathInterrupts] = {
    ARN5_FR_REG_OFST(PathInterrupts, 0, 0),
    0,
    ARN5_FR_REG_OFST(PathMasks, 0, 0),
    ARN5_IMED_FR_SONET32RX_PathInterrupts_ALL, 0,
    0, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },

  [ARN5_IMED_FR_TRANSMIT_TxInterrupt] = {
    ARN5_FR_REG_OFST(TxInterrupt, 0, 0),
    0,
    ARN5_FR_REG_OFST(TxMask, 0, 0),
    ARN5_IMED_FR_TRANSMIT_TxInterrupt_ALL, 0,
    ARN5_IRQ_FLG_RW1C, ARN5_NUM_OF_SLICES, ARN5_SLICE_SIZE },
} ;


const cs_uint8 * arn5_fr_irq_grp_name_tbl[ARN5_FR_MAX_GRP] =
{
  [ARN5_GRP_FR_GLOBAL_MasterInt]    = "FR_GLOBAL_MasterInt",
  [ARN5_GRP_FR_GLOBAL_ClockInt]     = "FR_GLOBAL_ClockInt",
  [ARN5_GRP_FR_DATACOMRX_StrInt]    = "FR_DATACOMRX_StrInt",
  [ARN5_GRP_FR_DATACOMTX_StrInt]    = "FR_DATACOMTX_StrInt",
  [ARN5_GRP_FR_STREAMRX_CellIntr]   = "FR_STREAMRX_CellIntr",
  [ARN5_GRP_FR_STREAMTX_CellIntr]   = "FR_STREAMTX_CellIntr",
  [ARN5_GRP_FR_STREAMRX_PktIntr]    = "FR_STREAMRX_PktIntr",
  [ARN5_GRP_FR_STREAMTX_PktInt]     = "FR_STREAMTX_PktInt",
  [ARN5_GRP_FR_GFP_RX_GFP_Intr]     = "FR_GFP_RX_GFP_Intr",
  [ARN5_GRP_FR_GFP_TX_GFP_Intr]     = "FR_GFP_TX_GFP_Intr",
  [ARN5_GRP_FR_SONET32RX_PathInt]   = "FR_SONET32RX_PathInt",
  [ARN5_GRP_FR_SONET32RX_LineInterrupt] = "FR_SONET32RX_LineInterrupt",
  [ARN5_GRP_FR_SONET32RX_SectionInterrupt] =
                              "FR_SONET32RX_SectionInterrupt",
  [ARN5_GRP_FR_SONET32RX_APSInterrupt]   = "FR_SONET32RX_APSInterrupt",
  [ARN5_GRP_FR_RXHDLC_Interrupt_Section] = "FR_RXHDLC_Interrupt_Section",
  [ARN5_GRP_FR_TXHDLC_Interrupt_Section] = "FR_TXHDLC_Interrupt_Section",
  [ARN5_GRP_FR_RXHDLC_Interrupt_Line]    = "FR_RXHDLC_Interrupt_Line",
  [ARN5_GRP_FR_TXHDLC_Interrupt_Line]    = "FR_TXHDLC_Interrupt_Line",
} ;


/**************** IRQ ID NAME TABLES **********************/
const arn5_irq_id_name_t
ARN5_GRP_FR_GLOBAL_MasterInt_irq_id_name_tbl[] = {
  {ARN5_FR_GLOBAL_MasterInt_TxFrStartI,         "TxFrStartI"},
  {ARN5_FR_GLOBAL_MasterInt_APSI,               "APSI"},
  {ARN5_FR_GLOBAL_MasterInt_ClkFailI,           "ClkFailI"},
  {ARN5_FR_GLOBAL_MasterInt_TxI,                "TxI"},
  {ARN5_FR_GLOBAL_MasterInt_DatacomTxI,         "DatacomTxI"},
  {ARN5_FR_GLOBAL_MasterInt_TxCellPI,           "TxCellPI"},
  {ARN5_FR_GLOBAL_MasterInt_TxPktPI,            "TxPktPI"},
  {ARN5_FR_GLOBAL_MasterInt_RxFrStartI,         "RxFrStartI"},
  {ARN5_FR_GLOBAL_MasterInt_MbusTimeoutI,       "MbusTimeoutI"},
  {ARN5_FR_GLOBAL_MasterInt_RxSI,               "RxSI"},
  {ARN5_FR_GLOBAL_MasterInt_RxLI,               "RxLI"},
  {ARN5_FR_GLOBAL_MasterInt_RxPI,               "RxPI"},
  {ARN5_FR_GLOBAL_MasterInt_ResetI,             "ResetI"},
  {ARN5_FR_GLOBAL_MasterInt_RxCellPI,           "RxCellPI"},
  {ARN5_FR_GLOBAL_MasterInt_RxPktPI,            "RxPktPI"},
} ;

const cs_uint8 * arn5_fr_irq_imed_name_tbl[ARN5_FR_MAX_IMED] =
{
  [ARN5_IMED_FR_GLOBAL_MasterInt]           = "FR_GLOBAL_MasterInt",
  [ARN5_IMED_FR_GLOBAL_MasterInt2]          = "FR_GLOBAL_MasterInt2",
  [ARN5_IMED_FR_GLOBAL_CellRxInt]           = "FR_GLOBAL_CellRxInt",
  [ARN5_IMED_FR_GLOBAL_CellTxInt]           = "FR_GLOBAL_CellTxInt",
  [ARN5_IMED_FR_GLOBAL_PacketRxInt]         = "FR_GLOBAL_PacketRxInt",
  [ARN5_IMED_FR_GLOBAL_PacketTxInt]         = "FR_GLOBAL_PacketTxInt",
  [ARN5_IMED_FR_DATACOMRX_Interrupt]        = "FR_DATACOMRX_Interrupt",
  [ARN5_IMED_FR_DATACOMTX_Interrupt]        = "FR_DATACOMTX_Interrupt",
  [ARN5_IMED_FR_SONET32RX_SectionInterrupt] = "FR_SONET32RX_SectionInterrupt",
  [ARN5_IMED_FR_SONET32RX_LineInterrupt]    = "FR_SONET32RX_LineInterrupt",
  [ARN5_IMED_FR_SONET32RX_PathInterrupts]   = "FR_SONET32RX_PathInterrupts",
  [ARN5_IMED_FR_TRANSMIT_TxInterrupt]       = "FR_TRANSMIT_TxInterrupt",
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_IRQ_GBL_MasterInt2_irq_id_name_tbl[] = {
  {ARN5_FR_GLOBAL_MasterInt2_StreamRx_GFP_I,    "StreamRx_GFP_I"},
  {ARN5_FR_GLOBAL_MasterInt2_StreamTx_GFP_I,    "StreamTx_GFP_I"},
  {ARN5_FR_GLOBAL_MasterInt2_DatacomRxI,        "DatacomRxI"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_GLOBAL_ClockInt_irq_id_name_tbl[] = {
  {ARN5_FR_GLOBAL_ClockInt_RxClkI,              "RxClkI"},
  {ARN5_FR_GLOBAL_ClockInt_TxClkI,              "TxClkI"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_DATACOMRX_StrInt_irq_id_name_tbl[] = {
  {ARN5_FR_DATACOMRX_StrInt_RX_FIFO_Par_Err_I,  "RX_FIFO_Par_Err_I"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_DATACOMTX_StrInt_irq_id_name_tbl[] = {
  {ARN5_FR_DATACOMTX_StrInt_FIFO_Par_Err_I,     "FIFO_Par_Err_I"},
  {ARN5_FR_DATACOMTX_StrInt_overflowI,          "overflowI"},
  {ARN5_FR_DATACOMTX_StrInt_badFrameI,          "badFrameI"},
  {ARN5_FR_DATACOMTX_StrInt_shortCellI,         "shortCellI"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_STREAMRX_CellIntr_irq_id_name_tbl[] = {
  {ARN5_FR_STREAMRX_CellIntr_CellCntI,          "CellCntI"},
  {ARN5_FR_STREAMRX_CellIntr_IdleCntI,          "IdleCntI"},
  {ARN5_FR_STREAMRX_CellIntr_HECDropI,          "HECDropI"},
  {ARN5_FR_STREAMRX_CellIntr_HECCorrI,          "HECCorrI"},
  {ARN5_FR_STREAMRX_CellIntr_OVFCntI,           "OVFCntI"},
  {ARN5_FR_STREAMRX_CellIntr_SynExI,            "SynExI"},
  {ARN5_FR_STREAMRX_CellIntr_X86AdaptByteI,     "X86AdaptByteI"},
  {ARN5_FR_STREAMRX_CellIntr_OvrFloI,           "OvrFloI"},
  {ARN5_FR_STREAMRX_CellIntr_HECErrI,           "HECErrI"},
  {ARN5_FR_STREAMRX_CellIntr_LCDI,              "LCDI"},
  {ARN5_FR_STREAMRX_CellIntr_OCDI,              "OCDI"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_STREAMTX_CellIntr_irq_id_name_tbl[] = {
  {ARN5_FR_STREAMTX_CellIntr_CellCntI,          "CellCntI"},
  {ARN5_FR_STREAMTX_CellIntr_IdleCntI,          "IdleCntI"},
} ;

const arn5_irq_id_name_t
ARN5_FR_STREAMRX_PktIntr_irq_id_name_tbl[] = {
  {ARN5_FR_STREAMRX_PktIntr_shrtPkt,            "shrtPkt"},
  {ARN5_FR_STREAMRX_PktIntr_lngPkt,             "lngPkt"},
  {ARN5_FR_STREAMRX_PktIntr_Ovrflow,            "Ovrflow"},
  {ARN5_FR_STREAMRX_PktIntr_FCSErr,             "FCSErr"},
  {ARN5_FR_STREAMRX_PktIntr_formatErr,          "formatErr"},
  {ARN5_FR_STREAMRX_PktIntr_abort,              "abort"},
  {ARN5_FR_STREAMRX_PktIntr_InvCnt,             "InvCnt"},
  {ARN5_FR_STREAMRX_PktIntr_DropCnt,            "DropCnt"},
  {ARN5_FR_STREAMRX_PktIntr_FCSECnt,            "FCSECnt"},
  {ARN5_FR_STREAMRX_PktIntr_pktCnt,             "pktCnt"},
  {ARN5_FR_STREAMRX_PktIntr_byteCnt,            "byteCnt"},
  {ARN5_FR_STREAMRX_PktIntr_shrtPktCnt,         "shrtPktCnt"},
  {ARN5_FR_STREAMRX_PktIntr_longPktCnt,         "longPktCnt"},
  {ARN5_FR_STREAMRX_PktIntr_OvrPktCnt,          "OvrPktCnt"},
  {ARN5_FR_STREAMRX_PktIntr_abortPktCnt,        "abortPktCnt"},
  {ARN5_FR_STREAMRX_PktIntr_GdPktCnt,           "GdPktCnt"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_STREAMTX_PktInt_irq_id_name_tbl[] = {
  {ARN5_FR_STREAMTX_PktInt_ByteCntI,            "ByteCntI"},
  {ARN5_FR_STREAMTX_PktInt_TxByteCntI,          "TxByteCntI"},
  {ARN5_FR_STREAMTX_PktInt_PktCntI,             "PktCntI"},
  {ARN5_FR_STREAMTX_PktInt_UndCntI,             "UndCntI"},
  {ARN5_FR_STREAMTX_PktInt_ErrCntI,             "ErrCntI"},
  {ARN5_FR_STREAMTX_PktInt_LongCntI,            "LongCntI"},
  {ARN5_FR_STREAMTX_PktInt_ShortCntI,           "ShortCntI"},
  {ARN5_FR_STREAMTX_PktInt_TxShrtPktI,          "TxShrtPktI"},
  {ARN5_FR_STREAMTX_PktInt_TxLngPktI,           "TxLngPktI"},
  {ARN5_FR_STREAMTX_PktInt_TxUndFlowI,          "TxUndFlowI"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_GFP_RX_GFP_Intr_irq_id_name_tbl[] = {
  {ARN5_FR_GFP_RX_GFP_Intr_Long_Pkt_I,          "Long_Pkt_I"},
  {ARN5_FR_GFP_RX_GFP_Intr_Short_Pkt_I,         "Short_Pkt_I"},
  {ARN5_FR_GFP_RX_GFP_Intr_1bit_HEC_Err_I,      "1bit_HEC_Err_I"},
  {ARN5_FR_GFP_RX_GFP_Intr_Mbit_HEC_Err_I,      "Mbit_HEC_Err_I"},
  {ARN5_FR_GFP_RX_GFP_Intr_OverFlow_I,          "OverFlow_I"},
  {ARN5_FR_GFP_RX_GFP_Intr_Dropped_Pkt_I,       "Dropped_Pkt_I"},
  {ARN5_FR_GFP_RX_GFP_Intr_Sync_Err_I,          "Sync_Err_I"},
  {ARN5_FR_GFP_RX_GFP_Intr_Loss_Client_I,       "Loss_Client_I"},
  {ARN5_FR_GFP_RX_GFP_Intr_Loss_Sync_I,         "Loss_Sync_I"},
  {ARN5_FR_GFP_RX_GFP_Intr_CRC_Err_I,           "CRC_Err_I"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_GFP_TX_GFP_Intr_irq_id_name_tbl[] = {
  {ARN5_FR_GFP_TX_GFP_Intr_Idle_Pkts_MSB_I,     "Idle_Pkts_MSB_I"},
  {ARN5_FR_GFP_TX_GFP_Intr_Short_Pkt_I,         "Short_Pkt_I"},
  {ARN5_FR_GFP_TX_GFP_Intr_Long_Pkt_I,          "Long_Pkt_I"},
  {ARN5_FR_GFP_TX_GFP_Intr_Under_Flow_I,        "Under_Flow_I"},
  {ARN5_FR_GFP_TX_GFP_Intr_AOP_Pkt_I,           "AOP_Pkt_I"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_SONET32RX_PathInt_irq_id_name_tbl[] = {
  {ARN5_FR_SONET32RX_PathInterrupt_NewPtrI,     "NewPtrI"},
  {ARN5_FR_SONET32RX_PathInterrupt_pathBERI,    "pathBERI"},
  {ARN5_FR_SONET32RX_PathInterrupt_PTCapI,      "PTCapI"},
  {ARN5_FR_SONET32RX_PathInterrupt_SSI,         "SSI"},
  {ARN5_FR_SONET32RX_PathInterrupt_PBGEI,       "PBGEI"},
  {ARN5_FR_SONET32RX_PathInterrupt_PBIPI,       "PBIPI"},
  {ARN5_FR_SONET32RX_PathInterrupt_PPtrEI,      "PPtrEI"},
  {ARN5_FR_SONET32RX_PathInterrupt_PREII,       "PREII"},
  {ARN5_FR_SONET32RX_PathInterrupt_PSLUnsI,     "PSLUnsI"},
  {ARN5_FR_SONET32RX_PathInterrupt_PSLMisI,     "PSLMisI"},
  {ARN5_FR_SONET32RX_PathInterrupt_PTUnsI,      "PTUnsI"},
  {ARN5_FR_SONET32RX_PathInterrupt_PTMisI,      "PTMisI"},
  {ARN5_FR_SONET32RX_PathInterrupt_PUNEQPI,     "PUNEQPI"},
  {ARN5_FR_SONET32RX_PathInterrupt_PRDII,       "PRDII"},
  {ARN5_FR_SONET32RX_PathInterrupt_PAISI,       "PAISI"},
  {ARN5_FR_SONET32RX_PathInterrupt_LOPI,        "LOPI"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_SONET32RX_LineInterrupt_irq_id_name_tbl[] = {
  {ARN5_FR_SONET32RX_LineInterrupt_LREII,       "LREII"},
  {ARN5_FR_SONET32RX_LineInterrupt_LEREIEI,     "LEREIEI"},
  {ARN5_FR_SONET32RX_LineInterrupt_B2BIPI,      "B2BIPI"},
  {ARN5_FR_SONET32RX_LineInterrupt_B2BIPEI,     "B2BIPEI"},
  {ARN5_FR_SONET32RX_LineInterrupt_NewZ1S1I,    "NewZ1S1I"},
  {ARN5_FR_SONET32RX_LineInterrupt_RDILI,       "RDILI"},
  {ARN5_FR_SONET32RX_LineInterrupt_AISLI,       "AISLI"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_SONET32RX_SectionInterrupt_irq_id_name_tbl[] = {
  {ARN5_FR_SONET32RX_SectionInterrupt_X_LOS_raw_I,      "X_LOS_raw_I"},
  {ARN5_FR_SONET32RX_SectionInterrupt_X_LOS_qual_I,     "X_LOS_qual_I"},
  {ARN5_FR_SONET32RX_SectionInterrupt_ParityErrI,       "ParityErrI"},
  {ARN5_FR_SONET32RX_SectionInterrupt_PRBSErrI,         "PRBSErrI"},
  {ARN5_FR_SONET32RX_SectionInterrupt_SecTrRcvI,        "SecTrRcvI"},
  {ARN5_FR_SONET32RX_SectionInterrupt_B1BIPI,           "B1BIPI"},
  {ARN5_FR_SONET32RX_SectionInterrupt_B1BIPEI,          "B1BIPEI"},
  {ARN5_FR_SONET32RX_SectionInterrupt_SecTrMisI,        "SecTrMisI"},
  {ARN5_FR_SONET32RX_SectionInterrupt_SecTrUnsI,        "SecTrUnsI"},
  {ARN5_FR_SONET32RX_SectionInterrupt_SEFI,             "SEFI"},
  {ARN5_FR_SONET32RX_SectionInterrupt_dZ0InconI,        "dZ0InconI"},
  {ARN5_FR_SONET32RX_SectionInterrupt_LOFI,             "LOFI"},
  {ARN5_FR_SONET32RX_SectionInterrupt_LOSI,             "LOSI"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_SONET32RX_APSInterrupt_irq_id_name_tbl[] = {
  {ARN5_FR_SONET32RX_APSInterrupt_BadF2I,       "BadF2I"},
  {ARN5_FR_SONET32RX_APSInterrupt_NewF2I,       "NewF2I"},
  {ARN5_FR_SONET32RX_APSInterrupt_BadK1I,       "BadK1I"},
  {ARN5_FR_SONET32RX_APSInterrupt_BadK2I,       "BadK2I"},
  {ARN5_FR_SONET32RX_APSInterrupt_BadK3I,       "BadK3I"},
  {ARN5_FR_SONET32RX_APSInterrupt_NewK1I,       "NewK1I"},
  {ARN5_FR_SONET32RX_APSInterrupt_NewK2I,       "NewK2I"},
  {ARN5_FR_SONET32RX_APSInterrupt_NewK3I,       "NewK3I"},
  {ARN5_FR_SONET32RX_APSInterrupt_SFI,          "SFI"},
  {ARN5_FR_SONET32RX_APSInterrupt_SDI,          "SDI"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_TRANSMIT_TxInterrupt_irq_id_name_tbl[] = {
  {ARN5_FR_TRANSMIT_TxInterrupt_hdlcTxSDCCInterrupt,
   "hdlcTxSDCCInterrupt"},
  {ARN5_FR_TRANSMIT_TxInterrupt_hdlcTxLDCCInterrupt,
   "hdlcTxLDCCInterrupt"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_RXHDLC_Interrupt_irq_id_name_tbl[] = {
  {ARN5_FR_RXHDLC_Interrupt_FIFODone0I,         "FIFODone0I"},
  {ARN5_FR_RXHDLC_Interrupt_FIFODone1I,         "FIFODone1I"},
  {ARN5_FR_RXHDLC_Interrupt_FIFOUnderflowI,     "FIFOUnderflowI"},
  {ARN5_FR_RXHDLC_Interrupt_FIFOOverflowI,      "FIFOOverflowI"},
  {ARN5_FR_RXHDLC_Interrupt_CRCFailI,           "CRCFailI"},
  {ARN5_FR_RXHDLC_Interrupt_CRCErrorCountRolloverI,
   "CRCErrorCountRolloverI"},
  {ARN5_FR_RXHDLC_Interrupt_EndOfPacketErrorI,  "EndOfPacketErrorI"},
  {ARN5_FR_RXHDLC_Interrupt_PacketLengthErrorI, "PacketLengthErrorI"},
  {ARN5_FR_RXHDLC_Interrupt_UnalignedDataErrorI,"UnalignedDataErrorI"},
} ;

const arn5_irq_id_name_t
ARN5_GRP_FR_TXHDLC_Interrupt_irq_id_name_tbl[] = {
  {ARN5_FR_TXHDLC_Interrupt_FIFODone0I,         "FIFODone0I"},
  {ARN5_FR_TXHDLC_Interrupt_FIFODone1I,         "FIFODone1I"},
  {ARN5_FR_TXHDLC_Interrupt_FIFOUnderflowI,     "FIFOUnderflowI"},
  {ARN5_FR_TXHDLC_Interrupt_FIFOOverflowI,      "FIFOOverflowI"},
} ;


/****************************************************************
 * Internal FRAMER functions declarations
 ****************************************************************/
/* this function is to take care of a few intermediate interrutps that
   also need write to clear, no validation to the group */
static
cs_status arn5_framer_clear_imed_irq(cs_uint16 port_id,
            const arn5_irq_ctl_t * imed_tbl, cs_uint32 imed_id,
            cs_uint16 irq_clr_bits);

/**********************************************************************
 *         FRAMER IRQ SPECIAL HANDLER
 **********************************************************************/
cs_uint16 arn5_framer_irq_spl_hndlr(arn5_irq_spl_info_t * irq_spl_info)
{
  arn5_irq_info_t    * irq_info ;
  cs_uint16           status = 0, grp_id, irq_reg_val, irq_id ;

  if (irq_spl_info->irq_oper != ARN5_IRQ_SPL_INT) {
    return (0) ;
  }

  irq_info = irq_spl_info->op_mode.irq_hndl.irq_info ;
  irq_reg_val = irq_spl_info->op_mode.irq_hndl.irq_reg_val ;
  irq_id = irq_info->irq_id ;
  grp_id = irq_info->grp_id ;

  switch (grp_id) {
    case ARN5_GRP_FR_STREAMTX_CellIntr :
    {
      irq_info->flags |= ARN5_IRQ_INFO_STATUS_FLAG ;
      irq_info->status = ((irq_reg_val >> 8) & irq_id) ? 1 : 0 ;
    }
    break ;

    case ARN5_GRP_FR_SONET32RX_APSInterrupt :
    {
      switch (irq_id) {
        case ARN5_FR_SONET32RX_APSInterrupt_BadK3I :
        case ARN5_FR_SONET32RX_APSInterrupt_BadK2I :
        case ARN5_FR_SONET32RX_APSInterrupt_BadK1I :
          irq_info->flags |= ARN5_IRQ_INFO_STATUS_FLAG ;
          irq_info->status = ((irq_reg_val >> 8) & irq_id) ? 1 : 0 ;
        break ;

        case ARN5_FR_SONET32RX_APSInterrupt_BadF2I :
          irq_info->flags |= ARN5_IRQ_INFO_STATUS_FLAG ;
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
cs_status arn5_framer_get_default_irq_cfg(cs_uint16 port_id,
                        arn5_port_summ_t * pSumm,
                        arn5_framer_cfg_t * pCfg)
{
  /*******************************************************************/
  /* Set the default IRQs for SONET Section/Line and path            */
  /*******************************************************************/
  /* Enable MBUS timeout IRQ and Reset IRQ ON */
  pCfg->irq_ctl_tbl[ARN5_GRP_FR_GLOBAL_MasterInt] =
                        (ARN5_FR_GLOBAL_MasterInt_MbusTimeoutI |
                         ARN5_FR_GLOBAL_MasterInt_ResetI ) ;

  /* Enable RX/TX clock activity change IRQ */
  pCfg->irq_ctl_tbl[ARN5_GRP_FR_GLOBAL_ClockInt] =
    (ARN5_GRP_FR_GLOBAL_ClockInt_ALL) ;

  /* Set the default IRQ for SONET */
  pCfg->irq_ctl_tbl[ARN5_GRP_FR_SONET32RX_LineInterrupt] =
                                (ARN5_FR_SONET32RX_LineInterrupt_LREII |
                                 ARN5_FR_SONET32RX_LineInterrupt_RDILI |
                                 ARN5_FR_SONET32RX_LineInterrupt_AISLI) ;

  /* Section default IRQs */
  pCfg->irq_ctl_tbl[ARN5_GRP_FR_SONET32RX_SectionInterrupt] =
    (ARN5_FR_SONET32RX_SectionInterrupt_LOSI       |
     ARN5_FR_SONET32RX_SectionInterrupt_LOFI       |
     ARN5_FR_SONET32RX_SectionInterrupt_SEFI       |
     ARN5_FR_SONET32RX_SectionInterrupt_SecTrUnsI  |
     ARN5_FR_SONET32RX_SectionInterrupt_dZ0InconI  |
     ARN5_FR_SONET32RX_SectionInterrupt_SecTrRcvI  |
     ARN5_FR_SONET32RX_SectionInterrupt_SecTrMisI  ) ;

  /* SONET Path defaut IRQs */
  pCfg->irq_ctl_tbl[ARN5_GRP_FR_SONET32RX_PathInt] =
                  (ARN5_FR_SONET32RX_PathInterrupt_LOPI    |
                   ARN5_FR_SONET32RX_PathInterrupt_PAISI   |
                   ARN5_FR_SONET32RX_PathInterrupt_PRDII   |
                   ARN5_FR_SONET32RX_PathInterrupt_PUNEQPI |
                   ARN5_FR_SONET32RX_PathInterrupt_PREII   |
                   ARN5_FR_SONET32RX_PathInterrupt_PSLUnsI |
                   ARN5_FR_SONET32RX_PathInterrupt_PSLMisI |
                   ARN5_FR_SONET32RX_PathInterrupt_PTUnsI  |
                   ARN5_FR_SONET32RX_PathInterrupt_PTMisI  ) ;

  pCfg->irq_ctl_tbl[ARN5_GRP_FR_DATACOMRX_StrInt] =
    ARN5_GRP_FR_DATACOMRX_StrInt_ALL ;

  pCfg->irq_ctl_tbl[ARN5_GRP_FR_DATACOMTX_StrInt] =
    ARN5_GRP_FR_DATACOMTX_StrInt_ALL ;

  switch(pSumm->l2_prot) {
    case ARN5_PORT_PROT_POS :
      /* Set the Default IRQ for POS Mode */
      pCfg->irq_ctl_tbl[ARN5_GRP_FR_STREAMRX_PktIntr] =
        (ARN5_GRP_STREAMRX_PktIntr_ALL) ;
      pCfg->irq_ctl_tbl[ARN5_GRP_FR_STREAMTX_PktInt] =
        (ARN5_GRP_FR_STREAMTX_PktInt_ALL) ;
    break ;

    case ARN5_PORT_PROT_PPP :
      /* Set the Default IRQ for POS Mode */
        pCfg->irq_ctl_tbl[ARN5_GRP_FR_STREAMRX_PktIntr] =
          (ARN5_GRP_STREAMRX_PktIntr_ALL) ;
        pCfg->irq_ctl_tbl[ARN5_GRP_FR_STREAMTX_PktInt] =
          (ARN5_GRP_FR_STREAMTX_PktInt_ALL) ;
    break ;

    case ARN5_PORT_PROT_LAPS:
      /* Set the Default IRQ for LAPS Mode */
      pCfg->irq_ctl_tbl[ARN5_GRP_FR_STREAMRX_PktIntr] =
        (ARN5_GRP_STREAMRX_PktIntr_ALL) ;
      pCfg->irq_ctl_tbl[ARN5_GRP_FR_STREAMTX_PktInt] =
        (ARN5_GRP_FR_STREAMTX_PktInt_ALL) ;
      /* X86 interrupt is located in STREAMRX_CellIntr */
      pCfg->irq_ctl_tbl[ARN5_GRP_FR_STREAMRX_CellIntr] =
        (ARN5_FR_STREAMRX_CellIntr_X86AdaptByteI) ;
    break ;

    case ARN5_PORT_PROT_ATM :
      /* Set the Default IRQ for ATM Mode */
      pCfg->irq_ctl_tbl[ARN5_GRP_FR_STREAMRX_CellIntr] =
        (ARN5_GRP_FR_STREAMRX_CellIntr_ALL &
         (~ARN5_FR_STREAMRX_CellIntr_X86AdaptByteI)) ;
      pCfg->irq_ctl_tbl[ARN5_GRP_FR_STREAMTX_CellIntr] =
        (ARN5_GRP_FR_STREAMTX_CellIntr_ALL) ;
    break ;

    case ARN5_PORT_PROT_GFP :
      pCfg->svc = ARN5_FR_SVC_GFP ;
      /* Set the Default IRQ for GFP Mode */
      pCfg->irq_ctl_tbl[ARN5_GRP_FR_GFP_RX_GFP_Intr] =
        (ARN5_GRP_FR_GFP_RX_GFP_Intr_ALL) ;
      pCfg->irq_ctl_tbl[ARN5_GRP_FR_GFP_TX_GFP_Intr] =
        (ARN5_GRP_FR_GFP_TX_GFP_Intr_ALL) ;
    break ;

    case ARN5_PORT_PROT_CLEAR_CH :
    default :
    break ;
  }
  return (CS_OK) ;
}


/************************************************************************
 * INIT FRAMER IRQ
 ***********************************************************************/
cs_status arn5_framer_init_irq_cfg(cs_uint16 port_id,
                               arn5_port_summ_t * pSumm,
                               arn5_framer_cfg_t * pCfg)
{
  ARN5_t                * pDev ;
  cs_uint16             sl_num, ii ;

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id);

  /* Register IRQ Table */
  ARN5_REGISTER_IRQ_TBL(port_id, ARN5_ID_FRAMER, arn5_fr_irq_tbl,
                        arn5_fr_irq_grp_name_tbl, ARN5_FR_MAX_GRP,
                        arn5_framer_irq_spl_hndlr) ;

  ARN5_REGISTER_IMED_TBL(port_id, ARN5_ID_FRAMER, arn5_fr_imed_irq_tbl,
                        arn5_fr_irq_imed_name_tbl, ARN5_FR_MAX_IMED,
                        NULL) ;

  /* Enable the IRQs which are requested to be enabled by default */
  /* first, clear intermediate interrupts that needs write to clear */
  arn5_framer_clear_imed_irq(port_id, arn5_fr_imed_irq_tbl,
    ARN5_IMED_FR_SONET32RX_SectionInterrupt,
    ARN5_IMED_FR_SONET32RX_SectionInterrupt_ALL);

  arn5_framer_clear_imed_irq(port_id, arn5_fr_imed_irq_tbl,
    ARN5_IMED_FR_SONET32RX_LineInterrupt,
    ARN5_IMED_FR_SONET32RX_LineInterrupt_ALL);

  arn5_framer_clear_imed_irq(port_id, arn5_fr_imed_irq_tbl,
    ARN5_IMED_FR_TRANSMIT_TxInterrupt,
    ARN5_IMED_FR_TRANSMIT_TxInterrupt_ALL);

  for (ii = 0; ii < ARN5_FR_MAX_GRP; ii++) {
    /* Clear any interrupts */
    arn5_port_clear_leaf_irq(port_id, ARN5_ID_FRAMER, ii,
                        arn5_fr_irq_tbl[ii].irq_map) ;

    if (pCfg->irq_ctl_tbl[ii]) {
      arn5_port_enable_leaf_irq(port_id, ARN5_ID_FRAMER, ii,
                        pCfg->irq_ctl_tbl[ii]) ;
    }
  }

  /* Enable all the intermediate interrupt controls by default */
  for (ii = 0; ii < ARN5_FR_MAX_IMED; ii++) {
    arn5_enable_imed_irq(port_id, ARN5_ID_FRAMER, ii,
                        arn5_fr_imed_irq_tbl[ii].irq_map) ;
  }

  return (CS_OK) ;
}

/********************************************************************/
/*              ISR ROUTINE FOR FRAMER BLOCK                        */
/********************************************************************/
cs_status arn5_framer_isr(cs_uint16 port_id)
{
  ARN5_t                        * pDev ;
  cs_uint16                     sl_num ;
  ARN5_FR_GLOBAL_MasterInt_t    masterInt ;
  ARN5_FR_GLOBAL_MasterInt2_t   masterInt2 ;

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id) ;
  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id);

  masterInt.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.
                              MasterInt.wrd) ;
  masterInt2.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.
                               MasterInt2.wrd) ;

  if (masterInt.wrd & ARN5_GRP_FR_GLOBAL_MasterInt_ALL) {
    arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
              ARN5_GRP_FR_GLOBAL_MasterInt,
              &arn5_fr_irq_tbl[ARN5_GRP_FR_GLOBAL_MasterInt],
              arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_GLOBAL_MasterInt],
              ARN5_GRP_FR_GLOBAL_MasterInt_irq_id_name_tbl,
              sizeof(ARN5_GRP_FR_GLOBAL_MasterInt_irq_id_name_tbl) /
              sizeof(arn5_irq_id_name_t)) ;
  }

  if(masterInt.bf.APSI) {
    arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
         ARN5_GRP_FR_SONET32RX_APSInterrupt,
         &arn5_fr_irq_tbl[ARN5_GRP_FR_SONET32RX_APSInterrupt],
         arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_SONET32RX_APSInterrupt],
         ARN5_GRP_FR_SONET32RX_APSInterrupt_irq_id_name_tbl,
         sizeof(ARN5_GRP_FR_SONET32RX_APSInterrupt_irq_id_name_tbl) /
         sizeof(arn5_irq_id_name_t)) ;

  }

  if(masterInt.bf.RxPI) {
    ARN5_FR_SONET32RX_PathInterrupts_t  pathInt ;

    pathInt.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.
                              PathInterrupts.wrd) ;

    if(pathInt.bf.PathI & 1) {
      arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
         (ARN5_GRP_FR_SONET32RX_PathInt),
         &arn5_fr_irq_tbl[ARN5_GRP_FR_SONET32RX_PathInt],
         arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_SONET32RX_PathInt],
         ARN5_GRP_FR_SONET32RX_PathInt_irq_id_name_tbl,
         sizeof(ARN5_GRP_FR_SONET32RX_PathInt_irq_id_name_tbl) /
         sizeof(arn5_irq_id_name_t)) ;
    }
  }

  if(masterInt.bf.RxLI) {
    ARN5_FR_SONET32RX_LineInterrupt_t lineInt;
    lineInt.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.LineInterrupt.wrd) ;

    arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
         ARN5_GRP_FR_SONET32RX_LineInterrupt,
         &arn5_fr_irq_tbl[ARN5_GRP_FR_SONET32RX_LineInterrupt],
         arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_SONET32RX_LineInterrupt],
         ARN5_GRP_FR_SONET32RX_LineInterrupt_irq_id_name_tbl,
         sizeof(ARN5_GRP_FR_SONET32RX_LineInterrupt_irq_id_name_tbl) /
         sizeof(arn5_irq_id_name_t)) ;

    if (lineInt.bf.SerialHDLCI) {
      arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
              ARN5_GRP_FR_RXHDLC_Interrupt_Line,
              &arn5_fr_irq_tbl[ARN5_GRP_FR_RXHDLC_Interrupt_Line],
              arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_RXHDLC_Interrupt_Line],
              ARN5_GRP_FR_RXHDLC_Interrupt_irq_id_name_tbl,
              sizeof(ARN5_GRP_FR_RXHDLC_Interrupt_irq_id_name_tbl) /
              sizeof(arn5_irq_id_name_t)) ;
    }

    /* clear sticky imed interrupt bit */
    arn5_framer_clear_imed_irq(port_id, arn5_fr_imed_irq_tbl,
        ARN5_IMED_FR_SONET32RX_LineInterrupt,
        ARN5_IMED_FR_SONET32RX_LineInterrupt_ALL);
  }

  if(masterInt.bf.RxSI) {
    ARN5_FR_SONET32RX_SectionInterrupt_t sectionInt;
    sectionInt.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.SectionInterrupt.wrd) ;

    arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
       ARN5_GRP_FR_SONET32RX_SectionInterrupt,
       &arn5_fr_irq_tbl[ARN5_GRP_FR_SONET32RX_SectionInterrupt],
       arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_SONET32RX_SectionInterrupt],
       ARN5_GRP_FR_SONET32RX_SectionInterrupt_irq_id_name_tbl,
       sizeof(ARN5_GRP_FR_SONET32RX_SectionInterrupt_irq_id_name_tbl) /
       sizeof(arn5_irq_id_name_t)) ;

    if (sectionInt.bf.SerialHDLCI) {
      arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
              ARN5_GRP_FR_RXHDLC_Interrupt_Section,
              &arn5_fr_irq_tbl[ARN5_GRP_FR_RXHDLC_Interrupt_Section],
              arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_RXHDLC_Interrupt_Section],
              ARN5_GRP_FR_RXHDLC_Interrupt_irq_id_name_tbl,
              sizeof(ARN5_GRP_FR_RXHDLC_Interrupt_irq_id_name_tbl) /
              sizeof(arn5_irq_id_name_t)) ;
    }

    /* clear sticky imed interrupt bit */
    arn5_framer_clear_imed_irq(port_id, arn5_fr_imed_irq_tbl,
        ARN5_IMED_FR_SONET32RX_SectionInterrupt,
        ARN5_IMED_FR_SONET32RX_SectionInterrupt_ALL);
  }

  if(masterInt2.bf.DatacomRxI) {
    ARN5_FR_DATACOMRX_Interrupt_t  rxDcomInt ;
    rxDcomInt.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.datacomRx_Interrupt.wrd);
     if(rxDcomInt.bf.streamRxS & 1) {
      arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
        ARN5_GRP_FR_DATACOMRX_StrInt,
        &arn5_fr_irq_tbl[ARN5_GRP_FR_DATACOMRX_StrInt],
        arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_DATACOMRX_StrInt],
        ARN5_GRP_FR_DATACOMRX_StrInt_irq_id_name_tbl,
        sizeof(ARN5_GRP_FR_DATACOMRX_StrInt_irq_id_name_tbl) /
        sizeof(arn5_irq_id_name_t)) ;
    }
  }

  if(masterInt2.bf.StreamRx_GFP_I) {
    arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
          ARN5_GRP_FR_GFP_RX_GFP_Intr,
          &arn5_fr_irq_tbl[ARN5_GRP_FR_GFP_RX_GFP_Intr],
          arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_GFP_RX_GFP_Intr],
          ARN5_GRP_FR_GFP_RX_GFP_Intr_irq_id_name_tbl,
          sizeof(ARN5_GRP_FR_GFP_RX_GFP_Intr_irq_id_name_tbl) /
          sizeof(arn5_irq_id_name_t)) ;
  }

  if(masterInt.bf.RxPktPI) {
    ARN5_FR_GLOBAL_PacketRxInt_t     rxPktInt ;

    rxPktInt.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.
                               PacketRxInt.wrd) ;

     if(rxPktInt.bf.PacketRxI & 1) {
      arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
          ARN5_GRP_FR_STREAMRX_PktIntr,
          &arn5_fr_irq_tbl[ARN5_GRP_FR_STREAMRX_PktIntr],
          arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_STREAMRX_PktIntr],
          ARN5_FR_STREAMRX_PktIntr_irq_id_name_tbl,
          sizeof(ARN5_FR_STREAMRX_PktIntr_irq_id_name_tbl) /
          sizeof(arn5_irq_id_name_t)) ;
    }
  }

  if(masterInt.bf.RxCellPI) {
    ARN5_FR_GLOBAL_CellRxInt_t       rxCellInt ;
    rxCellInt.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.
                                CellRxInt.wrd) ;
     if (rxCellInt.bf.CellRxI  & 1) {
      arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
           ARN5_GRP_FR_STREAMRX_CellIntr,
           &arn5_fr_irq_tbl[ARN5_GRP_FR_STREAMRX_CellIntr],
           arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_STREAMRX_CellIntr],
           ARN5_GRP_FR_STREAMRX_CellIntr_irq_id_name_tbl,
           sizeof(ARN5_GRP_FR_STREAMRX_CellIntr_irq_id_name_tbl) /
           sizeof(arn5_irq_id_name_t)) ;
    }
  }

  if(masterInt.bf.TxI) {
    ARN5_FR_TRANSMIT_TxInterrupt_t txInt;
    txInt.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.TxInterrupt.wrd) ;

    if (txInt.bf.hdlcTxLDCCInterrupt) {
      arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
              ARN5_GRP_FR_TXHDLC_Interrupt_Line,
              &arn5_fr_irq_tbl[ARN5_GRP_FR_TXHDLC_Interrupt_Line],
              arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_TXHDLC_Interrupt_Line],
              ARN5_GRP_FR_TXHDLC_Interrupt_irq_id_name_tbl,
              sizeof(ARN5_GRP_FR_TXHDLC_Interrupt_irq_id_name_tbl) /
              sizeof(arn5_irq_id_name_t)) ;
    }

    if (txInt.bf.hdlcTxSDCCInterrupt) {
      arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
              ARN5_GRP_FR_TXHDLC_Interrupt_Section,
              &arn5_fr_irq_tbl[ARN5_GRP_FR_TXHDLC_Interrupt_Section],
              arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_TXHDLC_Interrupt_Section],
              ARN5_GRP_FR_TXHDLC_Interrupt_irq_id_name_tbl,
              sizeof(ARN5_GRP_FR_TXHDLC_Interrupt_irq_id_name_tbl) /
              sizeof(arn5_irq_id_name_t)) ;
    }

    /* clear sticky imed interrupt bit */
    arn5_framer_clear_imed_irq(port_id, arn5_fr_imed_irq_tbl,
      ARN5_IMED_FR_TRANSMIT_TxInterrupt,
      ARN5_IMED_FR_TRANSMIT_TxInterrupt_ALL);
  }

  if(masterInt.bf.DatacomTxI) {
    ARN5_FR_DATACOMTX_Interrupt_t  txDcomInt ;
    txDcomInt.wrd =
        CS_REG_READ(&pDev->slice[sl_num].Framer.datacomTx_Interrupt.wrd);

     if (txDcomInt.bf.streamTxS & 1) {
      arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
           ARN5_GRP_FR_DATACOMTX_StrInt,
           &arn5_fr_irq_tbl[ARN5_GRP_FR_DATACOMTX_StrInt],
           arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_DATACOMTX_StrInt],
           ARN5_GRP_FR_DATACOMTX_StrInt_irq_id_name_tbl,
           sizeof(ARN5_GRP_FR_DATACOMTX_StrInt_irq_id_name_tbl) /
           sizeof(arn5_irq_id_name_t)) ;
    }
  }

  if(masterInt.bf.TxPktPI) {
    ARN5_FR_GLOBAL_PacketTxInt_t  txPktInt ;
    txPktInt.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.
                               PacketTxInt.wrd) ;
    if (txPktInt.bf.PacketTxI & 1) {
      arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
        ARN5_GRP_FR_STREAMTX_PktInt,
        &arn5_fr_irq_tbl[ARN5_GRP_FR_STREAMTX_PktInt],
        arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_STREAMTX_PktInt],
        ARN5_GRP_FR_STREAMTX_PktInt_irq_id_name_tbl,
        sizeof(ARN5_GRP_FR_STREAMTX_PktInt_irq_id_name_tbl) /
        sizeof(arn5_irq_id_name_t)) ;
    }
  }

  if(masterInt2.bf.StreamTx_GFP_I) {
    arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
            ARN5_GRP_FR_GFP_TX_GFP_Intr,
            &arn5_fr_irq_tbl[ARN5_GRP_FR_GFP_TX_GFP_Intr],
            arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_GFP_TX_GFP_Intr],
            ARN5_GRP_FR_GFP_TX_GFP_Intr_irq_id_name_tbl,
            sizeof(ARN5_GRP_FR_GFP_TX_GFP_Intr_irq_id_name_tbl) /
            sizeof(arn5_irq_id_name_t)) ;
  }

  if(masterInt.bf.TxCellPI) {
    ARN5_FR_GLOBAL_CellTxInt_t       txCellInt ;

    txCellInt.wrd = CS_REG_READ(&pDev->slice[sl_num].Framer.
                                CellTxInt.wrd) ;
    if (txCellInt.bf.CellTxI & 1) {
      arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
           ARN5_GRP_FR_STREAMTX_CellIntr,
           &arn5_fr_irq_tbl[ARN5_GRP_FR_STREAMTX_CellIntr],
           arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_STREAMTX_CellIntr],
           ARN5_GRP_FR_STREAMTX_CellIntr_irq_id_name_tbl,
           sizeof(ARN5_GRP_FR_STREAMTX_CellIntr_irq_id_name_tbl) /
           sizeof(arn5_irq_id_name_t)) ;
    }
  }

  if(masterInt.bf.ClkFailI) {
    arn5_irq_grp_hndlr(port_id, (cs_uint32)pDev, ARN5_ID_FRAMER,
             ARN5_GRP_FR_GLOBAL_ClockInt,
             &arn5_fr_irq_tbl[ARN5_GRP_FR_GLOBAL_ClockInt],
             arn5_fr_irq_grp_name_tbl[ARN5_GRP_FR_GLOBAL_ClockInt],
             ARN5_GRP_FR_GLOBAL_ClockInt_irq_id_name_tbl,
             sizeof(ARN5_GRP_FR_GLOBAL_ClockInt_irq_id_name_tbl) /
             sizeof(arn5_irq_id_name_t)) ;
  }

  return(CS_OK) ;
}


/* caller doesn't need to worry about whether the bits to clear are there to
 * begin with. The actual clear only applies to the active one.  No chance of
 * clear int(s) by accident.
 */
static
cs_status arn5_framer_clear_imed_irq(cs_uint16 port_id,
            const arn5_irq_ctl_t * imed_tbl,
            cs_uint32 imed_id, cs_uint16 irq_clr_bits)
{
  ARN5_t                * pDev ;
  cs_uint8              sl_num ;
  volatile cs_reg       * irq_reg ;
  volatile cs_reg       clear_irq ;
  const arn5_irq_ctl_t  * irq_cb = &imed_tbl[imed_id];
  /*
   * Get the pointer to arn5_irq_ctl_t in the table using grp_id as index into
   * blk_irq_cb table.
   */

  pDev = ARN5_PORT_ID_TO_DEV_BASE(port_id);
  sl_num = ARN5_PORT_ID_TO_SLICE_NUM(port_id);

  ARN5_PORT_LOCK(port_id, ARN5_ID_FRAMER);

  irq_reg = (volatile cs_reg *)(((cs_uint32) pDev) +
                 (sl_num * ARN5_SLICE_SIZE) + irq_cb->offset_intr_reg) ;

  if ( irq_cb->flags & ARN5_IRQ_FLG_RW1C ) { /* RWIC */
    clear_irq = (irq_clr_bits & irq_cb->irq_map) ;
    clear_irq &= CS_REG_READ(irq_reg);
    CS_REG_WRITE(irq_reg, clear_irq) ;
  }

  ARN5_PORT_UNLOCK(port_id, ARN5_ID_FRAMER);
  return (CS_OK) ;
}

