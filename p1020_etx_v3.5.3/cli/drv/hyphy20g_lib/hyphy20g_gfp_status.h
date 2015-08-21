/******************************************************************************/
/**  COPYRIGHT (C) 2008 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
/**--------------------------------------------------------------------------**/
/** This software embodies materials and concepts which are proprietary and  **/
/** confidential to PMC-Sierra, Inc. PMC-Sierra distributes this software    **/
/** to its customers pursuant to the terms and conditions of the Software    **/
/** License Agreement contained in the text file software.lic that is        **/
/** distributed along with the software. This software can only be utilized  **/
/** if all terms and conditions of the Software License Agreement are        **/
/** accepted. If there are any questions, concerns, or if the                **/
/** Software License Agreement text file, software.lic, is missing please    **/
/** contact PMC-Sierra for assistance.                                       **/
/**--------------------------------------------------------------------------**/
/**                                                                          **/
/******************************************************************************/
/*******************************************************************************
**  MODULE      : HyPHY 20G Configuration Code
**
**  FILE        :  hyphy20g_gfp_status.h
**
**  $Date: 2010-10-23 01:00:36 +0800 (Sat, 23 Oct 2010) $
**
**  $Revision: 9876 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_GFP_STATUS_H
#define _HYPHY20G_GFP_STATUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_indirect.h"


/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* TEMPORARY DEFINITIONS                                                      */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gGfpTxPmonGfpFHdlcCntrs(struct file *fileHndl, UINT4 slice,
                                		UINT4 grpId, UINT4 trigger,
                                		UINT4 *txByteCntMsb, UINT4 *txByteCntLsb,
                                		UINT4 *txFrmCnt, UINT4 *txErrFrmCnt,
                                		UINT4 *txCmfFrmCnt, UINT4 *txIdleFrmCnt);

INT4 hyPhy20gGfpRxPmonGrpFrmCntrs(struct file *fileHndl, UINT4 slice,
                                  UINT4 grpId, UINT4 *rxByteCntMsb,
                                  UINT4 *rxByteCntLsb, UINT4 *rxCtrlFrmCnt,
                                  UINT4 *rxFrmCnt, UINT4 *rxIdleFrmCnt);

INT4 hyPhy20gGfpRxPmonGrpErrCntrs(struct file *fileHndl, UINT4 slice,
                                  UINT4 grpId, UINT4 *rxChecAbrt,
                                  UINT4 *rxThecMinfl, UINT4 *rxEhecMaxfl,
                                  UINT4 *rxEcorrCnt, UINT4 *rxDropFrmCnt,
                                  UINT4 *rxExiFltDrop);

INT4 hyPhy20gGfpRxPmonGfpFCpbidCntrs(struct file *fileHndl, UINT4 slice,
                                     UINT4 cpbId, UINT4 *rxFdfOvfCnt,
                                     UINT4 *rxCsfFrmCnt, UINT4 *rxErrFcsCnt,
                                     UINT4 *rxPauseFrmCnt, UINT4 *rxPtiUpiDropCnt);

INT4 hyPhy20gGfpRxGetPauseTime(struct file *fileHndl, UINT4 slice,
                               UINT4 cpbId, UINT4 *rxPauseTime);

INT4 hyPhy20gGfpRxGrpStatus(struct file *fileHndl, UINT4 slice,
                                  UINT4 grpId, UINT4 intClear,
                                  UINT4 *statusBits);

INT4 hyPhy20gGfpRxCpbidStatus(struct file *fileHndl, UINT4 slice,
                              UINT4 cpbId, UINT4 intClear,
                              UINT4 *statusBits);

INT4 hyPhy20gGfpRxCsfFifoStatus(struct file *fileHndl, UINT4 slice,
                             		UINT4 intClear, UINT4 fifoClear,
                             		UINT4 *statusBits);

INT4 hyPhy20gGfpRxCsfFifoRead(struct file *fileHndl, UINT4 slice,
                              UINT4 *cpbId, UINT4 *upi, UINT4 *valid);

INT4 hyPhy20gGfpTxFifoStatus(struct file *fileHndl, UINT4 slice,
                             UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gGfpTxGrpStatus(struct file *fileHndl, UINT4 slice,
                            UINT4 grpId, UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gGfpTranspTxGrpStatus(struct file *fileHndl, UINT4 slice,
                                  UINT4 grpId, UINT4 intClear,
                                  UINT4 *statusBits);

INT4 hyPhy20gGfpTranspRxGrpStatus(struct file *fileHndl, UINT4 slice,
                                  UINT4 grpId, UINT4 intClear,
                                  UINT4 *statusBits);

INT4 hyPhy20gGfpTranspFifoStatus(struct file *fileHndl, UINT4 slice,
                                    UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gGfpRxPmonGfpTCpbidCntrs(struct file *fileHndl, UINT4 slice,
                                     UINT4 cpbId, UINT4 *rxCorCrcCnt,
                                     UINT4 *rxUncCrcCnt, UINT4 *rxErrFreeSbCnt,
                                     UINT4 *rxProtErrCnt);

INT4 hyPhy20gGfpTxPmonGfpTCntrs(struct file *fileHndl, UINT4 slice,
                                UINT4 cpbId, UINT4 *txByteCntMsb,
                                UINT4 *txByteCntLsb, UINT4 *tx65bPadCntMsb,
                                UINT4 *tx65bPadCntLsb);

INT4 hyPhy20gGfpRxdpPmonTrig(struct file *fileHndl, UINT4 slice);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_GFP_STATUS_H */
/* end of file */
