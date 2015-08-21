/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_irq.h
 *
 * FRAMER IRQ Tables and APIs
 *
 * This file is released on Mon May  1 00:51:10 2006.
 */

#ifndef __M2R_FRAMER_IRQ_H__
#define __M2R_FRAMER_IRQ_H__

/*****************************************************************
 * Framer IRQ leaf defines
 *****************************************************************/
/* Leaf Interrupt Bit fields for M2R_GRP_FR_GLOBAL_MasterInt */
#define M2R_FR_GLOBAL_MasterInt_TxFrStartI          (0x8000)
#define M2R_FR_GLOBAL_MasterInt_RxFrStartI          (0x0080)
#define M2R_FR_GLOBAL_MasterInt_MbusTimeoutI        (0x0040)
#define M2R_FR_GLOBAL_MasterInt_ResetI              (0x0004)
#define M2R_GRP_FR_GLOBAL_MasterInt_ALL             (0x80C4)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_GLOBAL_ClockInt */
#define M2R_FR_GLOBAL_ClockInt_RxClkI               (0x8000)
#define M2R_FR_GLOBAL_ClockInt_TxClkI               (0x0001)
#define M2R_GRP_FR_GLOBAL_ClockInt_ALL              (0x8001)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_GLOBAL_GPIOInt  */
#define M2R_FR_GLOBAL_GPIOInt_RI0                   (0x0100)
#define M2R_FR_GLOBAL_GPIOInt_RI1                   (0x0200)
#define M2R_FR_GLOBAL_GPIOInt_RI2                   (0x0400)
#define M2R_FR_GLOBAL_GPIOInt_RI3                   (0x0800)
#define M2R_FR_GLOBAL_GPIOInt_RI4                   (0x1000)
#define M2R_FR_GLOBAL_GPIOInt_RI5                   (0x2000)
#define M2R_FR_GLOBAL_GPIOInt_RI6                   (0x4000)
#define M2R_FR_GLOBAL_GPIOInt_RI7                   (0x8000)
#define M2R_FR_GLOBAL_GPIOInt_FI0                   (0x0001)
#define M2R_FR_GLOBAL_GPIOInt_FI1                   (0x0002)
#define M2R_FR_GLOBAL_GPIOInt_FI2                   (0x0004)
#define M2R_FR_GLOBAL_GPIOInt_FI3                   (0x0008)
#define M2R_FR_GLOBAL_GPIOInt_FI4                   (0x0010)
#define M2R_FR_GLOBAL_GPIOInt_FI5                   (0x0020)
#define M2R_FR_GLOBAL_GPIOInt_FI6                   (0x0040)
#define M2R_FR_GLOBAL_GPIOInt_FI7                   (0x0080)
#define M2R_GRP_FR_GLOBAL_GPIOInt_ALL               (0xffff)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_DATACOMRX_StrInt */
#define M2R_FR_DATACOMRX_StrInt_RX_FIFO_Par_Err_I   (0x0004)
#define M2R_GRP_FR_DATACOMRX_StrInt_ALL             (0x0004)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_DATACOMTX_StrInt */
#define M2R_FR_DATACOMTX_StrInt_FIFO_Par_Err_I      (0x0008)
#define M2R_FR_DATACOMTX_StrInt_overflowI           (0x0004)
#define M2R_FR_DATACOMTX_StrInt_badFrameI           (0x0002)
#define M2R_FR_DATACOMTX_StrInt_shortCellI          (0x0001)
#define M2R_GRP_FR_DATACOMTX_StrInt_ALL             (0x000f)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_STREAMRX_CellIntr */
#define M2R_FR_STREAMRX_CellIntr_CellCntI           (0x8000)
#define M2R_FR_STREAMRX_CellIntr_IdleCntI           (0x4000)
#define M2R_FR_STREAMRX_CellIntr_HECDropI           (0x2000)
#define M2R_FR_STREAMRX_CellIntr_HECCorrI           (0x1000)
#define M2R_FR_STREAMRX_CellIntr_OVFCntI            (0x0800)
#define M2R_FR_STREAMRX_CellIntr_SynExI 	    (0x0400)
#define M2R_FR_STREAMRX_CellIntr_OvrFloI            (0x0008)
#define M2R_FR_STREAMRX_CellIntr_HECErrI            (0x0004)
#define M2R_FR_STREAMRX_CellIntr_LCDI               (0x0002)
#define M2R_FR_STREAMRX_CellIntr_OCDI               (0x0001)
#define M2R_GRP_FR_STREAMRX_CellIntr_ALL            (0xfc0f)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_STREAMTX_CellIntr */
#define M2R_FR_STREAMTX_CellIntr_CellCntI           (0x0010)
#define M2R_FR_STREAMTX_CellIntr_IdleCntI           (0x0008)
#define M2R_GRP_FR_STREAMTX_CellIntr_ALL            (0x0018)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_STREAMRX_PktIntr */
#define M2R_FR_STREAMRX_PktIntr_shrtPkt             (0x8000)
#define M2R_FR_STREAMRX_PktIntr_lngPkt              (0x4000)
#define M2R_FR_STREAMRX_PktIntr_Ovrflow             (0x2000)
#define M2R_FR_STREAMRX_PktIntr_FCSErr              (0x1000)
#define M2R_FR_STREAMRX_PktIntr_formatErr           (0x0800)
#define M2R_FR_STREAMRX_PktIntr_abort               (0x0400)
#define M2R_FR_STREAMRX_PktIntr_InvCnt              (0x0200)
#define M2R_FR_STREAMRX_PktIntr_DropCnt             (0x0100)
#define M2R_FR_STREAMRX_PktIntr_FCSECnt             (0x0080)
#define M2R_FR_STREAMRX_PktIntr_pktCnt              (0x0040)
#define M2R_FR_STREAMRX_PktIntr_byteCnt             (0x0020)
#define M2R_FR_STREAMRX_PktIntr_shrtPktCnt          (0x0010)
#define M2R_FR_STREAMRX_PktIntr_longPktCnt          (0x0008)
#define M2R_FR_STREAMRX_PktIntr_OvrPktCnt           (0x0004)
#define M2R_FR_STREAMRX_PktIntr_abortPktCnt         (0x0002)
#define M2R_FR_STREAMRX_PktIntr_GdPktCnt            (0x0001)
#define M2R_FR_STREAMRX_PktIntr_ALL                 (0xffff)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_STREAMTX_PktInt   */
#define M2R_FR_STREAMTX_PktInt_ByteCntI             (0x8000)
#define M2R_FR_STREAMTX_PktInt_TxByteCntI           (0x4000)
#define M2R_FR_STREAMTX_PktInt_PktCntI              (0x2000)
#define M2R_FR_STREAMTX_PktInt_UndCntI              (0x1000)
#define M2R_FR_STREAMTX_PktInt_ErrCntI              (0x0800)
#define M2R_FR_STREAMTX_PktInt_LongCntI             (0x0400)
#define M2R_FR_STREAMTX_PktInt_ShortCntI            (0x0200)
#define M2R_FR_STREAMTX_PktInt_TxShrtPktI           (0x0004)
#define M2R_FR_STREAMTX_PktInt_TxLngPktI            (0x0002)
#define M2R_FR_STREAMTX_PktInt_TxUndFlowI           (0x0001)
#define M2R_GRP_FR_STREAMTX_PktInt_ALL              (0xfe07)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_GFP_RX_GFP_Intr */
#define M2R_FR_GFP_RX_GFP_Intr_Long_Pkt_I           (0x0200)
#define M2R_FR_GFP_RX_GFP_Intr_Short_Pkt_I          (0x0100)
#define M2R_FR_GFP_RX_GFP_Intr_1bit_HEC_Err_I       (0x0080)
#define M2R_FR_GFP_RX_GFP_Intr_Mbit_HEC_Err_I       (0x0040)
#define M2R_FR_GFP_RX_GFP_Intr_OverFlow_I           (0x0020)
#define M2R_FR_GFP_RX_GFP_Intr_Dropped_Pkt_I        (0x0010)
#define M2R_FR_GFP_RX_GFP_Intr_Sync_Err_I           (0x0008)
#define M2R_FR_GFP_RX_GFP_Intr_Loss_Client_I        (0x0004)
#define M2R_FR_GFP_RX_GFP_Intr_Loss_Sync_I          (0x0002)
#define M2R_FR_GFP_RX_GFP_Intr_CRC_Err_I            (0x0001)
#define M2R_GRP_FR_GFP_RX_GFP_Intr_ALL              (0x03ff)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_GFP_TX_GFP_Intr */
#define M2R_FR_GFP_TX_GFP_Intr_Idle_Pkts_MSB_I      (0x0100)
#define M2R_FR_GFP_TX_GFP_Intr_Oversized_Pkts_MSB_I (0x0080)
#define M2R_FR_GFP_TX_GFP_Intr_Short_Pkt_I          (0x0020)
#define M2R_FR_GFP_TX_GFP_Intr_Long_Pkt_I           (0x0010)
#define M2R_FR_GFP_TX_GFP_Intr_Oversized_Pkt_I      (0x0008)
#define M2R_FR_GFP_TX_GFP_Intr_Under_Flow_I         (0x0004)
#define M2R_FR_GFP_TX_GFP_Intr_ECC_Err_Corr_I       (0x0002)
#define M2R_GRP_FR_GFP_TX_GFP_Intr_ALL              (0x01be)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_SONET32RX_PathInt */
#define M2R_FR_SONET32RX_PathInterrupt_NewPtrI      (0x8000)
#define M2R_FR_SONET32RX_PathInterrupt_pathBERI     (0x4000)
#define M2R_FR_SONET32RX_PathInterrupt_PTCapI       (0x2000)
#define M2R_FR_SONET32RX_PathInterrupt_SSI          (0x1000)
#define M2R_FR_SONET32RX_PathInterrupt_PBGEI        (0x0800)
#define M2R_FR_SONET32RX_PathInterrupt_PBIPI        (0x0400)
#define M2R_FR_SONET32RX_PathInterrupt_PPtrEI       (0x0200)
#define M2R_FR_SONET32RX_PathInterrupt_PREII        (0x0100)
#define M2R_FR_SONET32RX_PathInterrupt_PSLUnsI      (0x0080)
#define M2R_FR_SONET32RX_PathInterrupt_PSLMisI      (0x0040)
#define M2R_FR_SONET32RX_PathInterrupt_PTUnsI       (0x0020)
#define M2R_FR_SONET32RX_PathInterrupt_PTMisI       (0x0010)
#define M2R_FR_SONET32RX_PathInterrupt_PUNEQPI      (0x0008)
#define M2R_FR_SONET32RX_PathInterrupt_PRDII        (0x0004)
#define M2R_FR_SONET32RX_PathInterrupt_PAISI        (0x0002)
#define M2R_FR_SONET32RX_PathInterrupt_LOPI         (0x0001)
#define M2R_GRP_FR_SONET32RX_PathInt_ALL            (0xffff)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_SONET32RX_LineInterrupt */
#define M2R_FR_SONET32RX_LineInterrupt_LREII        (0x0040)
#define M2R_FR_SONET32RX_LineInterrupt_LEREIEI      (0x0020)
#define M2R_FR_SONET32RX_LineInterrupt_B2BIPI       (0x0010)
#define M2R_FR_SONET32RX_LineInterrupt_B2BIPEI      (0x0008)
#define M2R_FR_SONET32RX_LineInterrupt_NewZ1S1I     (0x0004)
#define M2R_FR_SONET32RX_LineInterrupt_RDILI        (0x0002)
#define M2R_FR_SONET32RX_LineInterrupt_AISLI        (0x0001)
#define M2R_GRP_FR_SONET32RX_LineInterrupt_ALL      (0x007f)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_SONET32RX_SectionInterrupt */
#define M2R_FR_SONET32RX_SectionInterrupt_X_LOS_raw_I   (0x2000)
#define M2R_FR_SONET32RX_SectionInterrupt_X_LOS_qual_I  (0x1000)
#define M2R_FR_SONET32RX_SectionInterrupt_ParityErrI    (0x0400)
#define M2R_FR_SONET32RX_SectionInterrupt_PRBSErrI      (0x0200)
#define M2R_FR_SONET32RX_SectionInterrupt_SecTrRcvI     (0x0100)
#define M2R_FR_SONET32RX_SectionInterrupt_B1BIPI        (0x0080)
#define M2R_FR_SONET32RX_SectionInterrupt_B1BIPEI       (0x0040)
#define M2R_FR_SONET32RX_SectionInterrupt_SecTrMisI     (0x0020)
#define M2R_FR_SONET32RX_SectionInterrupt_SecTrUnsI     (0x0010)
#define M2R_FR_SONET32RX_SectionInterrupt_SEFI          (0x0008)
#define M2R_FR_SONET32RX_SectionInterrupt_dZ0InconI     (0x0004)
#define M2R_FR_SONET32RX_SectionInterrupt_LOFI          (0x0002)
#define M2R_FR_SONET32RX_SectionInterrupt_LOSI          (0x0001)
#define M2R_GRP_FR_SONET32RX_SectionInterrupt_ALL       (0x37ff)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_SONET32RX_APSInterrupt */
#define M2R_FR_SONET32RX_APSInterrupt_BadF2I            (0x0800)
#define M2R_FR_SONET32RX_APSInterrupt_NewF2I            (0x0400)
#define M2R_FR_SONET32RX_APSInterrupt_BadK1I            (0x0080)
#define M2R_FR_SONET32RX_APSInterrupt_BadK2I            (0x0040)
#define M2R_FR_SONET32RX_APSInterrupt_BadK3I            (0x0020)
#define M2R_FR_SONET32RX_APSInterrupt_NewK1I            (0x0010)
#define M2R_FR_SONET32RX_APSInterrupt_NewK2I            (0x0008)
#define M2R_FR_SONET32RX_APSInterrupt_NewK3I            (0x0004)
#define M2R_FR_SONET32RX_APSInterrupt_SFI               (0x0002)
#define M2R_FR_SONET32RX_APSInterrupt_SDI               (0x0001)
#define M2R_GRP_FR_SONET32RX_APSInterrupt_ALL           (0x0cff)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_RXHDLC_Interrupt */
#define M2R_FR_RXHDLC_Interrupt_UnalignedDataErrorI	(0x0100)
#define M2R_FR_RXHDLC_Interrupt_PacketLengthErrorI	(0x0080)
#define M2R_FR_RXHDLC_Interrupt_EndOfPacketErrorI	(0x0040)
#define M2R_FR_RXHDLC_Interrupt_CRCErrorCountRolloverI	(0x0020)
#define M2R_FR_RXHDLC_Interrupt_CRCFailI		(0x0010)
#define M2R_FR_RXHDLC_Interrupt_FIFOOverflowI		(0x0008)
#define M2R_FR_RXHDLC_Interrupt_FIFOUnderflowI		(0x0004)
#define M2R_FR_RXHDLC_Interrupt_FIFODone1I		(0x0002)
#define M2R_FR_RXHDLC_Interrupt_FIFODone0I		(0x0001)
#define M2R_GRP_FR_RXHDLC_Interrupt_ALL                 (0x01ff)

/* Leaf Interrupt Bit fields for M2R_GRP_FR_TXHDLC_Interrupt */
#define M2R_FR_TXHDLC_Interrupt_FIFOOverflowI		(0x0008)
#define M2R_FR_TXHDLC_Interrupt_FIFOUnderflowI		(0x0004)
#define M2R_FR_TXHDLC_Interrupt_FIFODone1I		(0x0002)
#define M2R_FR_TXHDLC_Interrupt_FIFODone0I		(0x0001)
#define M2R_GRP_FR_TXHDLC_Interrupt_ALL                 (0x000f)

typedef enum {
  M2R_GRP_FR_GLOBAL_MasterInt    	= 0,
  M2R_GRP_FR_GLOBAL_ClockInt	 	= 1,
  M2R_GRP_FR_GLOBAL_GPIOInt	 	= 2,
  M2R_GRP_FR_DATACOMRX_StrInt_0	 	= 3,
  M2R_GRP_FR_DATACOMRX_StrInt_1  	= 4,
  M2R_GRP_FR_DATACOMRX_StrInt_2  	= 5,
  M2R_GRP_FR_DATACOMRX_StrInt_3  	= 6,
  M2R_GRP_FR_DATACOMTX_StrInt_0  	= 7,
  M2R_GRP_FR_DATACOMTX_StrInt_1  	= 8,
  M2R_GRP_FR_DATACOMTX_StrInt_2  	= 9,
  M2R_GRP_FR_DATACOMTX_StrInt_3  	= 10,
  M2R_GRP_FR_STREAMRX_CellIntr_0 	= 11,
  M2R_GRP_FR_STREAMRX_CellIntr_1 	= 12,
  M2R_GRP_FR_STREAMRX_CellIntr_2 	= 13,
  M2R_GRP_FR_STREAMRX_CellIntr_3 	= 14,
  M2R_GRP_FR_STREAMTX_CellIntr_0 	= 15,
  M2R_GRP_FR_STREAMTX_CellIntr_1 	= 16,
  M2R_GRP_FR_STREAMTX_CellIntr_2 	= 17,
  M2R_GRP_FR_STREAMTX_CellIntr_3 	= 18,
  M2R_GRP_FR_STREAMRX_PktIntr_0         = 19,
  M2R_GRP_FR_STREAMRX_PktIntr_1         = 20,
  M2R_GRP_FR_STREAMRX_PktIntr_2         = 21,
  M2R_GRP_FR_STREAMRX_PktIntr_3         = 22,
  M2R_GRP_FR_STREAMTX_PktInt_0          = 23,
  M2R_GRP_FR_STREAMTX_PktInt_1          = 24,
  M2R_GRP_FR_STREAMTX_PktInt_2          = 25,
  M2R_GRP_FR_STREAMTX_PktInt_3          = 26,
  M2R_GRP_FR_GFP_RX_GFP_Intr            = 27,
  M2R_GRP_FR_GFP_TX_GFP_Intr            = 28,
  M2R_GRP_FR_SONET32RX_PathInt_0        = 29,
  M2R_GRP_FR_SONET32RX_PathInt_1        = 30,
  M2R_GRP_FR_SONET32RX_PathInt_2        = 31,
  M2R_GRP_FR_SONET32RX_PathInt_3        = 32,
  M2R_GRP_FR_SONET32RX_LineInterrupt    = 33,
  M2R_GRP_FR_SONET32RX_SectionInterrupt = 34,
  M2R_GRP_FR_SONET32RX_APSInterrupt     = 35,
  M2R_GRP_FR_RXHDLC_Interrupt_Section   = 36,
  M2R_GRP_FR_TXHDLC_Interrupt_Section   = 37,
  M2R_GRP_FR_RXHDLC_Interrupt_Line      = 38,
  M2R_GRP_FR_TXHDLC_Interrupt_Line      = 39,

  M2R_FR_MAX_GRP
} m2r_framer_irq_grp_t ;

/* Intermediate Interrupt Bit fields for M2R_IMED_FR_GLOBAL_MasterInt */
#define M2R_FR_GLOBAL_MasterInt_APSI                (0x4000)
#define M2R_FR_GLOBAL_MasterInt_ClkFailI            (0x2000)
#define M2R_FR_GLOBAL_MasterInt_GPIOI               (0x1000)
#define M2R_FR_GLOBAL_MasterInt_TxI                 (0x0800)
#define M2R_FR_GLOBAL_MasterInt_DatacomTxI          (0x0400)
#define M2R_FR_GLOBAL_MasterInt_TxCellPI            (0x0200)
#define M2R_FR_GLOBAL_MasterInt_TxPktPI             (0x0100)
#define M2R_FR_GLOBAL_MasterInt_RxSI                (0x0020)
#define M2R_FR_GLOBAL_MasterInt_RxLI                (0x0010)
#define M2R_FR_GLOBAL_MasterInt_RxPI                (0x0008)
#define M2R_FR_GLOBAL_MasterInt_RxCellPI            (0x0002)
#define M2R_FR_GLOBAL_MasterInt_RxPktPI             (0x0001)
#define M2R_IMED_FR_GLOBAL_MasterInt_ALL            (0x7f3b)

/* Intermediate Interrupt Bit fields for M2R_IMED_FR_GLOBAL_MasterInt2 */
#define M2R_FR_GLOBAL_MasterInt2_StreamRx_GFP_I     (0x0004)
#define M2R_FR_GLOBAL_MasterInt2_StreamTx_GFP_I     (0x0002)
#define M2R_FR_GLOBAL_MasterInt2_DatacomRxI         (0x0001)
#define M2R_IMED_FR_GLOBAL_MasterInt2_ALL           (0x0007)

/* Intermediate Interrupt Bit fields for M2R_IMED_FR_DATACOMRX_Interrupt */
#define M2R_FR_DATACOMRX_Interrupt_Stream_RxS3      (0x0008)
#define M2R_FR_DATACOMRX_Interrupt_Stream_RxS2      (0x0004)
#define M2R_FR_DATACOMRX_Interrupt_Stream_RxS1      (0x0002)
#define M2R_FR_DATACOMRX_Interrupt_Stream_RxS0      (0x0001)
#define M2R_IMED_FR_DATACOMRX_Interrupt_ALL         (0x000f)

/* Intermediate Interrupt Bit fields for M2R_IMED_FR_DATACOMTX_Interrupt */
#define M2R_FR_DATACOMTX_Interrupt_Stream_RxS3      (0x0008)
#define M2R_FR_DATACOMTX_Interrupt_Stream_RxS2      (0x0004)
#define M2R_FR_DATACOMTX_Interrupt_Stream_RxS1      (0x0002)
#define M2R_FR_DATACOMTX_Interrupt_Stream_RxS0      (0x0001)
#define M2R_IMED_FR_DATACOMTX_Interrupt_ALL         (0x000f)

/* Intermediate Interrupt Bit fields for M2R_IMED_FR_GLOBAL_CellRxInt */
#define M2R_FR_GLOBAL_CellRxInt_CellRxI3            (0x0008)
#define M2R_FR_GLOBAL_CellRxInt_CellRxI2            (0x0004)
#define M2R_FR_GLOBAL_CellRxInt_CellRxI1            (0x0002)
#define M2R_FR_GLOBAL_CellRxInt_CellRxI0            (0x0001)
#define M2R_IMED_FR_GLOBAL_CellRxInt_ALL            (0x000f)

/* Intermediate Interrupt Bit fields for M2R_IMED_FR_GLOBAL_CellTxInt */
#define M2R_FR_GLOBAL_CellTxInt_CellTxI3            (0x0008)
#define M2R_FR_GLOBAL_CellTxInt_CellTxI2            (0x0004)
#define M2R_FR_GLOBAL_CellTxInt_CellTxI1            (0x0002)
#define M2R_FR_GLOBAL_CellTxInt_CellTxI0            (0x0001)
#define M2R_IMED_FR_GLOBAL_CellTxInt_ALL            (0x000f)

/* Intermediate Interrupt Bit fields for M2R_IMED_FR_GLOBAL_PacketRxInt */
#define M2R_FR_GLOBAL_PacketRxInt_PacketRxI3        (0x0008)
#define M2R_FR_GLOBAL_PacketRxInt_PacketRxI2        (0x0004)
#define M2R_FR_GLOBAL_PacketRxInt_PacketRxI1        (0x0002)
#define M2R_FR_GLOBAL_PacketRxInt_PacketRxI0        (0x0001)
#define M2R_IMED_FR_GLOBAL_PacketRxInt_ALL          (0x000f)

/* Intermediate Interrupt Bit fields for M2R_IMED_FR_GLOBAL_PacketTxInt */
#define M2R_FR_GLOBAL_PacketTxInt_PacketTxI3        (0x0008)
#define M2R_FR_GLOBAL_PacketTxInt_PacketTxI2        (0x0004)
#define M2R_FR_GLOBAL_PacketTxInt_PacketTxI1        (0x0002)
#define M2R_FR_GLOBAL_PacketTxInt_PacketTxI0        (0x0001)
#define M2R_IMED_FR_GLOBAL_PacketTxInt_ALL          (0x000f)

/* Intermediate Interrupt Bit fields for M2R_IMED_FR_SONET32RX_LineInterrupt */
#define M2R_FR_SONET32RX_LineInterrupt_SerialHDLCI      (0x8000)
#define M2R_IMED_FR_SONET32RX_LineInterrupt_ALL         (0x8000)

/* Intermediate Interrupt Bit fields for M2R_GRP_FR_SONET32RX_SectionInterrupt */
#define M2R_FR_SONET32RX_SectionInterrupt_SerialHDLCI   (0x0800)
#define M2R_IMED_FR_SONET32RX_SectionInterrupt_ALL      (0x0800)

/* Intermediate Interrupt Bit fields for M2R_IMED_FR_SONET32RX_PathInterrupts */
#define M2R_FR_SONET32RX_PathInterrupts_PathI3          (0x0008)
#define M2R_FR_SONET32RX_PathInterrupts_PathI2          (0x0004)
#define M2R_FR_SONET32RX_PathInterrupts_PathI1          (0x0002)
#define M2R_FR_SONET32RX_PathInterrupts_PathI0          (0x0001)
#define M2R_IMED_FR_SONET32RX_PathInterrupts_ALL        (0x000f)

/* Intermediate Interrupt Bit fields for M2R_IMED_FR_TRANSMIT_TxInterrupt */
#define M2R_FR_TRANSMIT_TxInterrupt_hdlcTxLDCCInterrupt (0x8000)
#define M2R_FR_TRANSMIT_TxInterrupt_hdlcTxSDCCInterrupt (0x4000)
#define M2R_IMED_FR_TRANSMIT_TxInterrupt_ALL            (0xc000)

typedef enum {
  M2R_IMED_FR_GLOBAL_MasterInt          = 0,
  M2R_IMED_FR_GLOBAL_MasterInt2         = 1,
  M2R_IMED_FR_GLOBAL_CellRxInt          = 2,
  M2R_IMED_FR_GLOBAL_CellTxInt          = 3,
  M2R_IMED_FR_GLOBAL_PacketRxInt        = 4,
  M2R_IMED_FR_GLOBAL_PacketTxInt        = 5,
  M2R_IMED_FR_DATACOMRX_Interrupt       = 6,
  M2R_IMED_FR_DATACOMTX_Interrupt       = 7,
  M2R_IMED_FR_SONET32RX_SectionInterrupt= 8,
  M2R_IMED_FR_SONET32RX_LineInterrupt   = 9,
  M2R_IMED_FR_SONET32RX_PathInterrupts  = 10,
  M2R_IMED_FR_TRANSMIT_TxInterrupt      = 11,

  M2R_FR_MAX_IMED
} m2r_framer_imed_irq_grp_t ;

/*****************************************************************
 * Framer IRQ API
 *****************************************************************/
extern
cs_status m2r_framer_get_default_irq_cfg(cs_uint16 port_id,
                                         m2r_port_summ_t * pSumm,
                                         m2r_framer_cfg_t * pCfg,
                                         cs_boolean channelized_mode) ;

extern
cs_status m2r_framer_init_irq(cs_uint16 port_id,
                              m2r_port_summ_t * pSumm,
                              m2r_framer_cfg_t * pCfg) ;

extern cs_status m2r_framer_isr(cs_uint16 port_id) ;


#endif /* __M2R_FRAMER_IRQ_H__ */


