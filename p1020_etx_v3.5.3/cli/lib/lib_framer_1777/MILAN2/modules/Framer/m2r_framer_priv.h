/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * m2r_framer_priv.h
 *
 * This file is released on Mon May  1 00:51:10 2006.
 *
 * This is a PRIVATE header file.
 */
#ifndef __M2R_FRAMER_PRIV_H__
#define __M2R_FRAMER_PRIV_H__

#include "cs_types.h"

/*Fill device base address and slice number for the given port. */
#define M2R_FR_GET_DEVICE_ADDR(PORTID, BASE_ADDR, SLICE, RETCODE) \
{                                                                 \
  cs_uint32 error;                                                \
  if(M2R_IS_PORT_VALID(PORTID, error) != TRUE) {                  \
    CS_HNDL_ERROR((PORTID), error, ": port_id = %d\n", (PORTID)); \
    return (RETCODE) ;                                            \
  }                                                               \
  if(M2R_IS_SONET_MODE((PORTID)) == FALSE) {                      \
    CS_HNDL_ERROR((PORTID), EM2R_INVALID_IN_ETH_MODE,             \
                  "Framer is powered DOWN in ethernet Mode\n") ;  \
    return (RETCODE) ;                                            \
  }                                                               \
  (BASE_ADDR) = M2R_PORT_ID_TO_DEV_BASE((PORTID));                \
  (SLICE)     = M2R_PORT_ID_TO_SLICE_NUM((PORTID));               \
}

/* Fill the stream ID bounds.  */
#define M2R_FR_GET_CHANNEL_BOUNDS(CHAN_LO, CHAN_HI, STREAM_ID)  \
    if ((STREAM_ID) != M2R_SONET_STREAM_ALL) {                  \
        (CHAN_LO) = (STREAM_ID) ;                               \
        (CHAN_HI) = (STREAM_ID) + 1 ;                           \
    }                                                           \
    else {                                                      \
        (CHAN_LO) = M2R_SONET_STREAM0;                          \
        (CHAN_HI) = M2R_SONET_STREAM_ALL;                       \
    }

/* Set rx_dir and tx_dir depending upon the dir_ctl passed. */
#define M2R_FR_PARSE_DIR_CTL(RX_DIR, TX_DIR, DIR_CTL)           \
  (RX_DIR) = TRUE ;                                             \
  (TX_DIR) = TRUE ;                                             \
  if ((DIR_CTL) == M2R_TX) {                                    \
      (RX_DIR) = FALSE;                                         \
  }                                                             \
  else                                                          \
  if ((DIR_CTL) == M2R_RX) {                                    \
      (TX_DIR) = FALSE;                                         \
  }


#define M2R_FR_DATACOMRX_StrConfig_STRIDE                \
		FR_DATACOMRX_StrConfig_STRIDE

#define M2R_FR_DATACOMRX_StrInt_STRIDE                   \
		FR_DATACOMRX_StrInt_STRIDE

#define M2R_FR_DATACOMRX_StrMask_STRIDE                  \
		FR_DATACOMRX_StrMask_STRIDE

#define M2R_FR_DATACOMRX_StrStatus_STRIDE                \
		FR_DATACOMRX_StrStatus_STRIDE

#define M2R_FR_STREAMRX__STRIDE                          \
		FR_STREAMRX_STRIDE

#define M2R_FR_DATACOMTX_StrConfig_STRIDE                \
		FR_DATACOMTX_StrConfig_STRIDE

#define M2R_FR_DATACOMTX_StrInt_STRIDE                   \
		FR_DATACOMTX_StrInt_STRIDE

#define M2R_FR_DATACOMTX_StrMask_STRIDE                  \
		FR_DATACOMTX_StrMask_STRIDE

#define M2R_FR_DATACOMTX_StrStatus_STRIDE                \
		FR_DATACOMTX_StrStatus_STRIDE

#define M2R_FR_STREAMTX__STRIDE                          \
		FR_STREAMTX_STRIDE

#define M2R_FR_SONET32RX_PathProvision_STRIDE            \
		FR_SONET32RX_PathProvision_STRIDE

#define M2R_FR_SONET32RX_FilteredG1C2_STRIDE             \
		FR_SONET32RX_FilteredG1C2_STRIDE

#define M2R_FR_SONET32RX_PRDITxInsert_STRIDE             \
		FR_SONET32RX_PRDITxInsert_STRIDE

#define M2R_FR_SONET32RX_PathLabel_STRIDE                \
		FR_SONET32RX_PathLabel_STRIDE

#define M2R_FR_SONET32RX_APSPathTxInsert_STRIDE          \
		FR_SONET32RX_APSPathTxInsert_STRIDE

#define M2R_FR_SONET32RX_APSPathB3TxInsert_STRIDE        \
		FR_SONET32RX_APSPathB3TxInsert_STRIDE

#define M2R_FR_SONET32RX_PathPointer_STRIDE              \
		FR_SONET32RX_PathPointer_STRIDE

#define M2R_FR_SONET32RX_PathBIPCount_STRIDE             \
		FR_SONET32RX_PathBIPCount_STRIDE

#define M2R_FR_SONET32RX_PathREICount_STRIDE             \
		FR_SONET32RX_PathREICount_STRIDE

#define M2R_FR_SONET32RX_PathPositiveCount_STRIDE        \
		FR_SONET32RX_PathPositiveCount_STRIDE

#define M2R_FR_SONET32RX_PathNegativeCount_STRIDE        \
		FR_SONET32RX_PathNegativeCount_STRIDE

#define M2R_FR_SONET32RX_PathNewPtrCount_STRIDE          \
		FR_SONET32RX_PathNewPtrCount_STRIDE

#define M2R_FR_SONET32RX_PathInt_STRIDE                  \
		FR_SONET32RX_PathInt_STRIDE

#define M2R_FR_SONET32RX_PathMask_STRIDE                 \
		FR_SONET32RX_PathMask_STRIDE

#define M2R_FR_SONET32RX_PathStatus_STRIDE               \
		FR_SONET32RX_PathStatus_STRIDE

#define M2R_FR_SONET32RX_RxSectionTraceExp_STRIDE        \
		FR_SONET32RX_RxSectionTraceExp_STRIDE

#define M2R_FR_SONET32RX_RxPathTraceExp_STRIDE           \
		FR_SONET32RX_RxPathTraceExp_STRIDE

#define M2R_FR_RECVOVERHEAD_alarm3Path_STRIDE            \
		FR_RECVOVERHEAD_alarm3Path_STRIDE

#define M2R_FR_RECVOVERHEAD_alarm4Path_STRIDE            \
		FR_RECVOVERHEAD_alarm4Path_STRIDE

#define M2R_FR_RECVOVERHEAD_pathOverheadA_STRIDE         \
		FR_RECVOVERHEAD_pathOverheadA_STRIDE

#define M2R_FR_RECVOVERHEAD_pathOverheadB_STRIDE         \
		FR_RECVOVERHEAD_pathOverheadB_STRIDE

#define M2R_FR_RECVOVERHEAD_pathOverheadC_STRIDE         \
		FR_RECVOVERHEAD_pathOverheadC_STRIDE

#define M2R_FR_RXHDLC__STRIDE                            \
		FR_RXHDLC_STRIDE

#define M2R_FR_TRANSMIT_PathSPEPointerControl_STRIDE     \
		FR_TRANSMIT_PathSPEPointerControl_STRIDE

#define M2R_FR_TRANSMIT_PathProvisioning_STRIDE          \
		FR_TRANSMIT_PathProvisioning_STRIDE

#define M2R_FR_TRANSMIT_PathSPEForceControl_STRIDE       \
		FR_TRANSMIT_PathSPEForceControl_STRIDE

#define M2R_FR_TRANSMIT_PathNewPointer_STRIDE            \
		FR_TRANSMIT_PathNewPointer_STRIDE

#define M2R_FR_TRANSMIT_PathConcat_STRIDE                \
		FR_TRANSMIT_PathConcat_STRIDE

#define M2R_FR_XMITOVERHEAD_PathC2Z5_STRIDE              \
		FR_XMITOVERHEAD_PathC2Z5_STRIDE

#define M2R_FR_XMITOVERHEAD_PathUserInd_STRIDE           \
		FR_XMITOVERHEAD_PathUserInd_STRIDE

#define M2R_FR_XMITOVERHEAD_PathGrowth_STRIDE            \
		FR_XMITOVERHEAD_PathGrowth_STRIDE

#define M2R_FR_XMITOVERHEAD_PathAlarmDefectCtrl_STRIDE   \
		FR_XMITOVERHEAD_PathAlarmDefectCtrl_STRIDE

#define M2R_FR_XMITOVERHEAD_alarm3Path_STRIDE            \
		FR_XMITOVERHEAD_alarm3Path_STRIDE

#define M2R_FR_XMITOVERHEAD_alarm4Path_STRIDE            \
		FR_XMITOVERHEAD_alarm4Path_STRIDE

#define M2R_FR_XMITOVERHEAD_MonitorAlarm3Path_STRIDE     \
		FR_XMITOVERHEAD_MonitorAlarm3Path_STRIDE

#define M2R_FR_XMITOVERHEAD_MonitorAlarm4Path_STRIDE     \
		FR_XMITOVERHEAD_MonitorAlarm4Path_STRIDE

#define M2R_FR_XMITOVERHEAD_SectionTrace_STRIDE          \
		FR_XMITOVERHEAD_SectionTrace_STRIDE

#define M2R_FR_XMITOVERHEAD_PathTrace_STRIDE             \
		FR_XMITOVERHEAD_PathTrace_STRIDE

#define M2R_FR_TXHDLC__STRIDE                            \
		FR_TXHDLC_STRIDE


#endif /* __M2R_FRAMER_PRIV_H__ */

