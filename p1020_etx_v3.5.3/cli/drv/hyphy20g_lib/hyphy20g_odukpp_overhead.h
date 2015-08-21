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
**  FILE        :  hyphy20g_odukpp_overhead.h
**
**  $Date: 2010-01-13 16:09:30 -0800 (Wed, 13 Jan 2010) $
**
**  $Revision: 6161 $
**
**  DESCRIPTION :  Contains hardware specific definitions
**
**  NOTES       :  The user check the contents of this file and modify
**                 the contents as per their system's hardware characteristics
**
*******************************************************************************/

#ifndef _HYPHY20G_ODUKPP_OVERHEAD_H
#define _HYPHY20G_ODUKPP_OVERHEAD_H

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
INT4 hyPhy20gOdukppOtukRxSmTtiExpCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 sapi[16], UINT4 dapi[16],
                                     UINT4 opSpec[32]);
INT4 hyPhy20gOdukppOtukRxSmTtiProcCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 acptCnt, UINT4 inconCnt, 
                                      UINT4 cmpSapi, UINT4 cmpDapi, UINT4 cmpOs);
INT4 hyPhy20gOdukppOtukRxSmTtiAcptExt(struct file *fileHndl, UINT4 slice,
                                      UINT4 *pSapi, UINT4 *pDapi, 
                                      UINT4 *pOpSpec);
INT4 hyPhy20gOdukppOtukRxSmResProcCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 acptCnt, UINT4 inconCnt);
INT4 hyPhy20gOdukppOtukRxSmResAcptExt(struct file *fileHndl, UINT4 slice,
                                      UINT4 *pSmRes);
INT4 hyPhy20gOdukppOtukTxSmBeiBiaeSrcCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOtukTxSmBdiAutoProcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 losEnbl, UINT4 lofEnbl, 
                                          UINT4 lomEnbl, UINT4 otukAisEnbl, 
                                          UINT4 timEnbl);
INT4 hyPhy20gOdukppOtukTxSmBdiSrcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOtukTxSmIaeSrcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOtukTxSmResSrcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOtukRxSmBiaeBdiIaeProcCfg(struct file *fileHndl, UINT4 slice,
                                             UINT4 biaeFilter5, 
                                             UINT4 bdiFilter5, UINT4 iaeFilter5);
INT4 hyPhy20gOdukppOtukTxSmTtiMsgCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 sapi[16], UINT4 dapi[16],
                                     UINT4 opSpec[32]);
INT4 hyPhy20gOdukppOtukTxSmTtiMsgSrcCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 source);
INT4 hyPhy20gOdukppOtukRxResProcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 acptCnt, UINT4 inconCnt);
INT4 hyPhy20gOdukppOtukRxResAcptExt(struct file *fileHndl, UINT4 slice, 
                                    UINT4 *pRes);
INT4 hyPhy20gOdukppOtukTxResSrcCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOtukRxGcc0ProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 acptCnt, UINT4 inconCnt);
INT4 hyPhy20gOdukppOtukRxGcc0AcptExt(struct file *fileHndl, UINT4 slice, 
                                     UINT4 *pGcc0);
INT4 hyPhy20gOdukppOtukTxGcc0SrcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOdukRxRes1ProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 acptCnt, UINT4 inconCnt);
INT4 hyPhy20gOdukppOdukRxRes1AcptExt(struct file *fileHndl, UINT4 slice, 
                                     UINT4 *pRes1);
INT4 hyPhy20gOdukppOdukRxRes2ProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 acptCnt, UINT4 inconCnt);
INT4 hyPhy20gOdukppOdukRxRes2AcptExt(struct file *fileHndl, UINT4 slice,
                                     UINT4 *pRes2Hi, UINT4 *pRes2Low);
INT4 hyPhy20gOdukppOdukRxGcc1ProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 acptCnt, UINT4 inconCnt);
INT4 hyPhy20gOdukppOdukRxGcc1AcptExt(struct file *fileHndl, UINT4 slice, 
                                     UINT4 *pGcc1);
INT4 hyPhy20gOdukppOdukRxGcc2ProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 acptCnt, UINT4 inconCnt);
INT4 hyPhy20gOdukppOdukRxGcc2AcptExt(struct file *fileHndl, UINT4 slice, 
                                     UINT4 *pGcc2);
INT4 hyPhy20gOdukppOdukRxTcmActProcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 acptCnt, UINT4 inconCnt);
INT4 hyPhy20gOdukppOdukRxTcmActAcptExt(struct file *fileHndl, UINT4 slice,
                                       UINT4 *pTcmAct);
INT4 hyPhy20gOdukppOdukRxExpProcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 acptCnt, UINT4 inconCnt);
INT4 hyPhy20gOdukppOdukRxExpAcptExt(struct file *fileHndl, UINT4 slice, 
                                    UINT4 *pExp);
INT4 hyPhy20gOdukppOdukRxFtflProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 acptCnt, UINT4 inconCnt);
INT4 hyPhy20gOdukppOdukRxFtflAcptExt(struct file *fileHndl, UINT4 slice,
                                     UINT4 *pFwdIdFld, UINT4 *pFwdOpId,
                                     UINT4 *pFwdOpSp, UINT4 *pBwdIdFld,
                                     UINT4 *pBwdOpId, UINT4 *pBwdOpSp);
INT4 hyPhy20gOdukppOdukRxApsPccProcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 acptCnt, UINT4 inconCnt);
INT4 hyPhy20gOdukppOdukRxApsPccAcptExt(struct file *fileHndl, UINT4 slice,
                                       UINT4 mFrame, UINT4 *pApsPcc);
INT4 hyPhy20gOdukppOdukTxFtflSrcCfg(struct file *fileHndl, UINT4 slice, 
                                    UINT4 source);
INT4 hyPhy20gOdukppOdukTxFtflMsgCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 fwdIdFld, UINT4 fwdOpId[9],
                                    UINT4 fwdOpSp[118], UINT4 bwdIdFld,
                                    UINT4 bwdOpId[9], UINT4 bwdOpSp[118]);
INT4 hyPhy20gOdukppOdukTxApsPccSrcCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 mFrame, UINT4 source, 
                                      UINT4 forceVal);
INT4 hyPhy20gOdukppOdukTxRes1SrcCfg(struct file *fileHndl, UINT4 slice, 
                                    UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOdukTxRes2SrcCfg(struct file *fileHndl, UINT4 slice, 
                                    UINT4 source, UINT4 res2Hi, UINT4 res2Low);
INT4 hyPhy20gOdukppOdukTxGcc1SrcCfg(struct file *fileHndl, UINT4 slice, 
                                    UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOdukTxGcc2SrcCfg(struct file *fileHndl, UINT4 slice, 
                                    UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOdukTxTcmActSrcCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOdukTxExpSrcCfg(struct file *fileHndl, UINT4 slice, 
                                   UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOdukRxPmTcmiTtiExpCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 pmTcmNum, UINT4 sapi[16],
                                         UINT4 dapi[16], UINT4 opSpec[32]);
INT4 hyPhy20gOdukppOdukRxPmTcmiTtiProcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 pmTcmNum, UINT4 acptCnt,
                                          UINT4 inconCnt, UINT4 cmpSapi,
                                          UINT4 cmpDapi, UINT4 cmpOs);
INT4 hyPhy20gOdukppOdukRxPmTcmiTtiAcptExt(struct file *fileHndl, UINT4 slice,
                                          UINT4 pmTcmNum, UINT4 *pSapi, 
                                          UINT4 *pDapi, UINT4 *pOpSpec);
INT4 hyPhy20gOdukppOdukTxTcmiBeiBipProcCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 accuMode);
INT4 hyPhy20gOdukppOdukTxPmTcmiBeiBiaeSrcCfg(struct file *fileHndl, UINT4 slice,
                                             UINT4 pmTcmNum, UINT4 source,
                                             UINT4 forceVal);
INT4 hyPhy20gOdukppOdukTxPmBdiAutoProcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 losEnbl, UINT4 lofEnbl, 
                                          UINT4 lomEnbl, UINT4 otukAisEnbl, 
                                          UINT4 odukAisEnbl, UINT4 ociEnbl, 
                                          UINT4 lckEnbl, UINT4 pmTimEnbl, 
                                          UINT4 smTimEnbl, 
                                          UINT4 tcmiTimOciLckEnblMsk);
INT4 hyPhy20gOdukppOdukTxPmBdiSrcCfg(struct file *fileHndl, UINT4 slice, 
                                     UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOdukTxTcmiBdiAutoProcCfg(struct file *fileHndl, UINT4 slice,
                                            UINT4 losEnbl, UINT4 lofEnbl,
                                            UINT4 lomEnbl, UINT4 otukAisEnbl,
                                            UINT4 odukAisEnbl, UINT4 ltcEnbl,
                                            UINT4 ociEnbl, UINT4 lckEnbl,
                                            UINT4 tcmiTimEnbl, UINT4 smTimEnbl);
INT4 hyPhy20gOdukppOdukTxTcmiBdiSrcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 tcmNum, UINT4 source, 
                                       UINT4 forceVal);
INT4 hyPhy20gOdukppOdukTxPmTcmiStatSrcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 pmTcmNum, UINT4 source,
                                          UINT4 forceVal);
INT4 hyPhy20gOdukppOdukRxPmTcmiStatProcCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 pmTcmNum, UINT4 acptCnt,
                                           UINT4 iaeInsCnt);
INT4 hyPhy20gOdukppOdukRxPmTcmiStatAcptExt(struct file *fileHndl, UINT4 slice,
                                           UINT4 pmTcmNum, UINT4 *pPmTcmiStat);
INT4 hyPhy20gOdukppOdukTxPmTcmiTtiMsgCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 pmTcmNum, UINT4 sapi[16],
                                         UINT4 dapi[16], UINT4 opSpec[32]);
INT4 hyPhy20gOdukppOdukTxPmTcmiTtiMsgSrcCfg(struct file *fileHndl, UINT4 slice,
                                            UINT4 pmTcmNum, UINT4 source);
INT4 hyPhy20gOdukppOpukRxPayloadByteExpCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 payloadVal);
INT4 hyPhy20gOdukppOpukTxPayloadSrcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOpukTxPtSrcCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 source, UINT4 forceVal);
INT4 hyPhy20gOdukppOpukRxPtExpCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 pt);
INT4 hyPhy20gOdukppOpukRxPtAcptExt(struct file *fileHndl, UINT4 slice,                      
                                   UINT4 *pPt);
INT4 hyPhy20gOdukppOpukRxPtProcCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 acptCnt, UINT4 inconCnt);
INT4 hyPhy20gOdukppOtukRxFrameProcCfg(struct file *fileHndl, UINT4 slice, 
                                      UINT4 nLomLomCnt, UINT4 imOomCnt, 
                                      UINT4 lomNlomCnt, UINT4 nLofLofCnt,
                                      UINT4 ifOofCnt, UINT4 lofNlofCnt);
INT4 hyPhy20gOdukppOdukTxPmTcmiBipSrcCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 pmTcmNum, UINT4 source);
INT4 hyPhy20gOdukppOtukTxSmBipSrcCfg(struct file *fileHndl, UINT4 slice, 
                                     UINT4 source);
INT4 hyPhy20gOdukppOtukRxPmTcmiBdiProcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 pmTcmNum, UINT4 bdiAcpt);
INT4 hyPhy20gOdukppOtukTxTcmiBdiProcCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 tcmNum, UINT4 bdiAcpt);
INT4 hyPhy20gOdukppOdukRxTcmiBipSrcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 tcmNum, UINT4 source);
INT4 hyPhy20gOdukppOdukTxTcmiTtiExpCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 tcmNum, UINT4 sapi[16],
                                       UINT4 dapi[16], UINT4 opSpec[32]);
INT4 hyPhy20gOdukppOdukTxTcmiTtiProcCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 tcmNum, UINT4 acptCnt,
                                        UINT4 inconCnt, UINT4 cmpSapi,
                                        UINT4 cmpDapi, UINT4 cmpOs);
INT4 hyPhy20gOdukppOdukTxTcmiTtiAcptExt(struct file *fileHndl, UINT4 slice,
                                        UINT4 tcmNum, UINT4 *pSapi, 
                                        UINT4 *pDapi, UINT4 *pOpSpec);
INT4 hyPhy20gOdukppOdukRxTcmiBeiBipProcCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 accuMode);
INT4 hyPhy20gOdukppOtukRxSmBipProcCfg(struct file *fileHndl, UINT4 slice, 
                                      UINT4 accuMode);
INT4 hyPhy20gOdukppOdukRxTcmiBeiBiaeSrcCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 tcmNum, UINT4 source,
                                           UINT4 forceVal);
INT4 hyPhy20gOdukppOdukRxTcmiBdiAutoProcCfg(struct file *fileHndl, UINT4 slice,
                                            UINT4 odukAisEnbl, UINT4 ltcEnbl, 
                                            UINT4 ociEnbl, UINT4 lckEnbl, 
                                            UINT4 timEnbl);
INT4 hyPhy20gOdukppOdukRxTcmiBdiSrcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 tcmNum, UINT4 source, 
                                       UINT4 forceVal);
INT4 hyPhy20gOdukppOdukRxTcmiStatSrcCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 tcmNum, UINT4 source,
                                        UINT4 forceVal);
INT4 hyPhy20gOdukppOdukTxTcmiStatProcCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 tcmNum, UINT4 acptCnt,
                                         UINT4 iaeInsCnt);
INT4 hyPhy20gOdukppOdukTxTcmiStatAcptExt(struct file *fileHndl, UINT4 slice,
                                         UINT4 tcmNum, UINT4 *pTcmiStat);
INT4 hyPhy20gOdukppOdukRxTcmiTtiMsgCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 tcmNum, UINT4 sapi[16],
                                       UINT4 dapi[16], UINT4 opSpec[32]);
INT4 hyPhy20gOdukppOdukRxTcmiTtiMsgSrcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 tcmNum, UINT4 source);
INT4 hyPhy20gOdukppOtukFrameTxDiagCfg(struct file *fileHndl, UINT4 slice, 
                                      UINT4 fasCrpt, UINT4 mfasCrpt, 
                                      UINT4 scmbCrpt);
INT4 hyPhy20gOdukppOdukTxDiagCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 forceAlarmSig);
INT4 hyPhy20gOdukppOpukTxDiagCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 jc1Crpt, UINT4 jc2Crpt, UINT4 jc3Crpt, 
                                 UINT4 forcePj, UINT4 forceNj);
INT4 hyPhy20gOdukppOtukFrameRxDiagCfg(struct file *fileHndl, UINT4 slice, 
                                      UINT4 scmbCrpt);
INT4 hyPhy20gOdukppOtukTxDiagCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 forceAis);
INT4 hyPhy20gOdukppOdukRxDiagCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 forceAlarmSig);
INT4 hyPhy20gOdukppOpukRxDiagCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 forceAis);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_ODUKPP_OVERHEAD_H */         
/* end of file */ 
