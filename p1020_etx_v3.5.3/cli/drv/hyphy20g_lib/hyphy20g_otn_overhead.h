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
**  FILE        :  hyphy20g_otn_overhead.h
**
**  $Date: 2011-11-27 17:25:10 +0800 (Sun, 27 Nov 2011) $
**
**  $Revision: 17466 $
**
**  DESCRIPTION :  Contains hardware specific definitions
**
**  NOTES       :  The user check the contents of this file and modify
**                 the contents as per their system's hardware characteristics
**
*******************************************************************************/

#ifndef _HYPHY20G_OTN_OVERHEAD_H
#define _HYPHY20G_OTN_OVERHEAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gOtukRxSmTtiExpCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 sapi[16], UINT4 dapi[16],
                               UINT4 opSpec[32]);

INT4 hyPhy20gOtukRxSmTtiProcCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 acptCnt, UINT4 inconCnt, UINT4 cmpSapi,
                                UINT4 cmpDapi, UINT4 cmpOs);

INT4 hyPhy20gOtukRxSmTtiAcptExt(struct file *fileHndl, UINT4 slice,
                                UINT4 *pSapi, UINT4 *pDapi, UINT4 *pOpSpec);

INT4 hyPhy20gOtukRxSmResProcCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 acptCnt, UINT4 inconCnt);

INT4 hyPhy20gOtukRxSmResAcptExt(struct file *fileHndl, UINT4 slice,
                                UINT4 *pSmRes);

INT4 hyPhy20gOtukTxSmBeiBiaeSrcCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 source, UINT4 forceVal);

INT4 hyPhy20gOtukTxSmBdiAutoProcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 losEnbl, UINT4 lofEnbl, UINT4 lomEnbl,
                                    UINT4 otukAisEnbl, UINT4 timEnbl);

INT4 hyPhy20gOtukTxSmBdiSrcCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 source, UINT4 forceVal);

INT4 hyPhy20gOtukTxSmIaeSrcCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 source, UINT4 forceVal);

INT4 hyPhy20gOtukTxSmResSrcCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 source, UINT4 forceVal);

INT4 hyPhy20gOtukRxSmBiaeBdiIaeProcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 biaeFilter5, UINT4 bdiFilter5,
                                       UINT4 iaeFilter5);

INT4 hyPhy20gOtukTxSmTtiMsgCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 sapi[16], UINT4 dapi[16],
                               UINT4 opSpec[32]);

INT4 hyPhy20gOtukTxSmTtiMsgSrcCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 source);

INT4 hyPhy20gOtukRxResProcCfg(struct file *fileHndl, UINT4 slice,
                              UINT4 acptCnt, UINT4 inconCnt);

INT4 hyPhy20gOtukRxResAcptExt(struct file *fileHndl, UINT4 slice, UINT4 *pRes);

INT4 hyPhy20gOtukTxResSrcCfg(struct file *fileHndl, UINT4 slice,
                             UINT4 source, UINT4 forceVal);

INT4 hyPhy20gOtukRxGcc0ProcCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 acptCnt, UINT4 inconCnt);

INT4 hyPhy20gOtukRxGcc0AcptExt(struct file *fileHndl, UINT4 slice, UINT4 *pGcc0);

INT4 hyPhy20gOtukTxGcc0SrcCfg(struct file *fileHndl, UINT4 slice,
                              UINT4 source, UINT4 forceVal);

INT4 hyPhy20gOdukRxRes1ProcCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 acptCnt, UINT4 inconCnt);

INT4 hyPhy20gOdukRxRes1AcptExt(struct file *fileHndl, UINT4 slice, UINT4 *pRes1);

INT4 hyPhy20gOdukRxRes2ProcCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 acptCnt, UINT4 inconCnt);

INT4 hyPhy20gOdukRxRes2AcptExt(struct file *fileHndl, UINT4 slice,
                               UINT4 *pRes2Hi, UINT4 *pRes2Low);

INT4 hyPhy20gOdukRxGcc1ProcCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 acptCnt, UINT4 inconCnt);

INT4 hyPhy20gOdukRxGcc1AcptExt(struct file *fileHndl, UINT4 slice, UINT4 *pGcc1);

INT4 hyPhy20gOdukRxGcc2ProcCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 acptCnt, UINT4 inconCnt);

INT4 hyPhy20gOdukRxGcc2AcptExt(struct file *fileHndl, UINT4 slice, UINT4 *pGcc2);

INT4 hyPhy20gOdukRxTcmActProcCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 acptCnt, UINT4 inconCnt);

INT4 hyPhy20gOdukRxTcmActAcptExt(struct file *fileHndl, UINT4 slice,
                                 UINT4 *pTcmAct);

INT4 hyPhy20gOdukRxExpProcCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 acptCnt, UINT4 inconCnt);

INT4 hyPhy20gOdukRxExpAcptExt(struct file *fileHndl, UINT4 slice, UINT4 *pExp);

INT4 hyPhy20gOdukRxFtflProcCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 acptCnt, UINT4 inconCnt);

INT4 hyPhy20gOdukRxFtflAcptExt(struct file *fileHndl, UINT4 slice,
                               UINT4 *pFwdIdFld, UINT4 *pFwdOpId,
                               UINT4 *pFwdOpSp, UINT4 *pBwdIdFld,
                               UINT4 *pBwdOpId, UINT4 *pBwdOpSp);

INT4 hyPhy20gOdukRxApsPccProcCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 acptCnt, UINT4 inconCnt);

INT4 hyPhy20gOdukRxApsPccAcptExt(struct file *fileHndl, UINT4 slice,
                                 UINT4 mFrame, UINT4 *pApsPcc);

INT4 hyPhy20gOdukTxFtflSrcCfg(struct file *fileHndl, UINT4 slice, UINT4 source);

INT4 hyPhy20gOdukTxFtflMsgCfg(struct file *fileHndl, UINT4 slice,
                              UINT4 fwdIdFld, UINT4 fwdOpId[9],
                              UINT4 fwdOpSp[118], UINT4 bwdIdFld,
                              UINT4 bwdOpId[9], UINT4 bwdOpSp[118]);

INT4 hyPhy20gOdukTxApsPccSrcCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 mFrame, UINT4 source, UINT4 forceVal);

INT4 hyPhy20gOdukTxRes1SrcCfg(struct file *fileHndl, UINT4 slice, UINT4 source,
                              UINT4 forceVal);

INT4 hyPhy20gOdukTxRes2SrcCfg(struct file *fileHndl, UINT4 slice, UINT4 source,
                              UINT4 res2Hi, UINT4 res2Low);

INT4 hyPhy20gOdukTxGcc1SrcCfg(struct file *fileHndl, UINT4 slice, UINT4 source,
                              UINT4 forceVal);

INT4 hyPhy20gOdukTxGcc2SrcCfg(struct file *fileHndl, UINT4 slice, UINT4 source,
                              UINT4 forceVal);

INT4 hyPhy20gOdukTxTcmActSrcCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 source, UINT4 forceVal);

INT4 hyPhy20gOdukTxExpSrcCfg(struct file *fileHndl, UINT4 slice, UINT4 source,
                             UINT4 forceVal);

INT4 hyPhy20gOdukRxPmTcmiTtiExpCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 pmTcmNum, UINT4 sapi[16],
                                   UINT4 dapi[16], UINT4 opSpec[32]);

INT4 hyPhy20gOdukRxPmTcmiTtiProcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 pmTcmNum, UINT4 acptCnt,
                                    UINT4 inconCnt, UINT4 cmpSapi,
                                    UINT4 cmpDapi, UINT4 cmpOs);

INT4 hyPhy20gOdukRxPmTcmiTtiAcptExt(struct file *fileHndl, UINT4 slice,
                                    UINT4 pmTcmNum, UINT4 *pSapi, UINT4 *pDapi,
                                    UINT4 *pOpSpec);

INT4 hyPhy20gOdukTxTcmiBeiBipProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 accuMode);

INT4 hyPhy20gOdukTxPmTcmiBeiBiaeSrcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 pmTcmNum, UINT4 source,
                                       UINT4 forceVal);

INT4 hyPhy20gOdukTxPmBdiAutoProcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 losEnbl, UINT4 lofEnbl, UINT4 lomEnbl,
                                    UINT4 otukAisEnbl, UINT4 odukAisEnbl,
                                    UINT4 ociEnbl, UINT4 lckEnbl,
                                    UINT4 pmTimEnbl, UINT4 smTimEnbl,
                                    UINT4 tcmiTimOciLckEnblMsk);

INT4 hyPhy20gOdukTxPmBdiSrcCfg(struct file *fileHndl, UINT4 slice, UINT4 source,
                               UINT4 forceVal);

INT4 hyPhy20gOdukTxTcmiBdiAutoProcCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 losEnbl, UINT4 lofEnbl,
                                      UINT4 lomEnbl, UINT4 otukAisEnbl,
                                      UINT4 odukAisEnbl, UINT4 ltcEnbl,
                                      UINT4 ociEnbl, UINT4 lckEnbl,
                                      UINT4 tcmiTimEnbl, UINT4 smTimEnbl);

INT4 hyPhy20gOdukTxTcmiBdiSrcCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 tcmNum, UINT4 source, UINT4 forceVal);

INT4 hyPhy20gOdukTxPmTcmiStatSrcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 pmTcmNum, UINT4 source,
                                    UINT4 forceVal);

INT4 hyPhy20gOdukRxPmTcmiStatProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 pmTcmNum, UINT4 acptCnt,
                                     UINT4 iaeInsCnt);

INT4 hyPhy20gOdukRxPmTcmiStatAcptExt(struct file *fileHndl, UINT4 slice,
                                     UINT4 pmTcmNum, UINT4 *pPmTcmiStat);

INT4 hyPhy20gOdukTxPmTcmiTtiMsgCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 pmTcmNum, UINT4 sapi[16],
                                   UINT4 dapi[16], UINT4 opSpec[32]);

INT4 hyPhy20gOdukTxPmTcmiTtiMsgSrcCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 pmTcmNum, UINT4 source);

INT4 hyPhy20gOpukRxPayloadByteExpCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 payloadVal);

INT4 hyPhy20gOpukTxPayloadSrcCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 source, UINT4 forceVal);

INT4 hyPhy20gOpukTxPtSrcCfg(struct file *fileHndl, UINT4 slice,
                            UINT4 source, UINT4 forceVal);

INT4 hyPhy20gOpukRxPtExpCfg(struct file *fileHndl, UINT4 slice,
                            UINT4 pt);

INT4 hyPhy20gOpukRxPtAcptExt(struct file *fileHndl, UINT4 slice, UINT4 *pPt);

INT4 hyPhy20gOpukRxPtProcCfg(struct file *fileHndl, UINT4 slice,
                             UINT4 acptCnt, UINT4 inconCnt);

INT4 hyPhy20gOtukRxFrameProcCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 nLomLomCnt, UINT4 imOomCnt,
                                UINT4 lomNlomCnt, UINT4 nLofLofCnt,
                                UINT4 ifOofCnt, UINT4 lofNlofCnt);

INT4 hyPhy20gOdukTxPmTcmiBipSrcCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 pmTcmNum, UINT4 source);

INT4 hyPhy20gOtukTxSmBipSrcCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 source);

INT4 hyPhy20gOtukRxPmTcmiBdiProcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 pmTcmNum, UINT4 bdiAcpt);

INT4 hyPhy20gOtukTxTcmiBdiProcCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 tcmNum, UINT4 bdiAcpt);

INT4 hyPhy20gOdukRxTcmiBipSrcCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 tcmNum, UINT4 source);

INT4 hyPhy20gOdukTxTcmiTtiExpCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 tcmNum, UINT4 sapi[16],
                                 UINT4 dapi[16], UINT4 opSpec[32]);

INT4 hyPhy20gOdukTxTcmiTtiProcCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 tcmNum, UINT4 acptCnt,
                                  UINT4 inconCnt, UINT4 cmpSapi,
                                  UINT4 cmpDapi, UINT4 cmpOs);

INT4 hyPhy20gOdukTxTcmiTtiAcptExt(struct file *fileHndl, UINT4 slice,
                                  UINT4 tcmNum, UINT4 *pSapi, UINT4 *pDapi,
                                  UINT4 *pOpSpec);

INT4 hyPhy20gOdukRxTcmiBeiBipProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 accuMode);

INT4 hyPhy20gOtukRxSmBipProcCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 accuMode);

INT4 hyPhy20gOdukRxTcmiBeiBiaeSrcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 tcmNum, UINT4 source,
                                     UINT4 forceVal);

INT4 hyPhy20gOdukRxTcmiBdiAutoProcCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 odukAisEnbl, UINT4 ltcEnbl,
                                      UINT4 ociEnbl, UINT4 lckEnbl,
                                      UINT4 timEnbl);

INT4 hyPhy20gOdukRxTcmiBdiSrcCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 tcmNum, UINT4 source, UINT4 forceVal);

INT4 hyPhy20gOdukRxTcmiStatSrcCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 tcmNum, UINT4 source,
                                  UINT4 forceVal);

INT4 hyPhy20gOdukTxTcmiStatProcCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 tcmNum, UINT4 acptCnt,
                                   UINT4 iaeInsCnt);

INT4 hyPhy20gOdukTxTcmiStatAcptExt(struct file *fileHndl, UINT4 slice,
                                   UINT4 tcmNum, UINT4 *pTcmiStat);

INT4 hyPhy20gOdukRxTcmiTtiMsgCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 tcmNum, UINT4 sapi[16],
                                 UINT4 dapi[16], UINT4 opSpec[32]);

INT4 hyPhy20gOdukRxTcmiTtiMsgSrcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 tcmNum, UINT4 source);

INT4 hyPhy20gOtukFrameTxDiagCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 fasCrpt, UINT4 mfasCrpt, UINT4 scmbCrpt);

INT4 hyPhy20gOdukTxDiagCfg(struct file *fileHndl, UINT4 slice,
                           UINT4 forceAlarmSig);

INT4 hyPhy20gOpukTxDiagCfg(struct file *fileHndl, UINT4 slice, UINT4 jc1Crpt,
                           UINT4 jc2Crpt, UINT4 jc3Crpt, UINT4 forcePj,
                           UINT4 forceNj);

INT4 hyPhy20gOtukFrameRxDiagCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 scmbCrpt);

INT4 hyPhy20gOtukTxDiagCfg(struct file *fileHndl, UINT4 slice, UINT4 forceAis);

INT4 hyPhy20gOdukRxDiagCfg(struct file *fileHndl, UINT4 slice,
                           UINT4 forceAlarmSig);

INT4 hyPhy20gOpukRxDiagCfg(struct file *fileHndl, UINT4 slice, UINT4 forceAis);

INT4 hyPhy20gOtuEfecTxSetBitError(struct file *fileHndl, UINT4 slice,
                                  UINT4 fecType);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_OTN_OVERHEAD_H */
/* end of file */
