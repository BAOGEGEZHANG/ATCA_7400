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
**  FILE        :  hyPhy20g_opi.h
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :  This is the header file for hyPhy20g_opi.c file.
**
**  NOTES       :
**
*******************************************************************************/

#ifndef _HYPHY20G_OPI_H
#define _HYPHY20G_OPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_typs.h"
#include "hyphy20g_defs.h"
#include "hyphy20g_idefs.h"
#include "hyphy20g_err.h"
#include "hyphy20g_hw.h"
#include "hyphy20g_util_intlkn.h"
#include "hyphy20g_util_scbs.h"

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/
#define HYPHY20G_OPI_CBRI_NUM_SLICE                                            2

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gCbrbInit(struct file *fileHndl, UINT4 enable);

INT4 hyPhy20gCbrbVcatIntfInit(struct file *fileHndl, UINT4 slice, UINT4 payload);

INT4 hyPhy20gCbrbVcatIntfSrcCfg(struct file *fileHndl, UINT4 chnl, UINT4 source);

INT4 hyPhy20gCbriIntfInit(struct file *fileHndl, UINT4 slice, UINT4 enable);

INT4 hyPhy20gCbriIntfEnable(struct file *fileHndl, UINT4 slice, UINT4 enable);

INT4 hyPhy20gCbriTxRateLimitCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 rateLimit, UINT4 burstLimit,
                                UINT4 byteGran);

INT4 hyPhy20gCbriTxFcResponseCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 xoffMode, UINT4 xoffHoldOff,
                                 UINT4 linkMode, UINT4 xoffIgnore);

INT4 hyPhy20gCbriLaneDisable(struct file *fileHndl, UINT4 slice, 
                             UINT4 disableMsk);

INT4 hyPhy20gCbriFcModeCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                           UINT4 mode, UINT4 ctxt);

INT4 hyPhy20gCbriFcCalCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                          UINT4 calAddr, UINT4 fcPort);

INT4 hyPhy20gCbrbOtnZoneChnlCfg(struct file *fileHndl, UINT4 rdChnl,
                                UINT4 zoneLow, UINT4 zoneMid, UINT4 zoneHigh,
                                UINT4 fifoCentEn);                                

INT4 hyPhy20gCbrbVcatOtnChnlCfg(struct file *fileHndl, UINT4 frmr,
                                UINT4 enable);

INT4 hyPhy20gCbrbReaderSrcChnlCfg(struct file *fileHndl, UINT4 rdChnl,
                                  UINT4 wrChnlIntf, UINT4 wrChnl);

INT4 hyPhy20gCbrbCbriFlowControlCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 fcPort, UINT4 rdChnl, UINT4 trigLow,
                                    UINT4 trigHigh);

INT4 hyPhy20gCbrbFifoReaderChnlEnable(struct file *fileHndl, UINT4 rdChnl,
                                      UINT4 rate, UINT4 enable);

INT4 hyPhy20gCbrbFifoWriterChnlEnable(struct file *fileHndl, UINT4 wrChnl,
                                      UINT4 enable);

INT4 hyPhy20gCbrbCbriFlowControlEnable(struct file *fileHndl, UINT4 slice,
                                       UINT4 fcPort, UINT4 enable);

INT4 hyPhy20gCbrbCbriRxSerdesSrcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 rxSrc);

INT4 hyPhy20gCbriCreateCtxt(struct file *fileHndl, UINT4 slice);

INT4 hyPhy20gCbriDestroyCtxt(struct file *fileHndl, UINT4 slice);

INT4 hyPhy20gCbrbPktSizeTypeCfg(struct file *fileHndl, UINT4 type);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                 */
/******************************************************************************/ 
UINT4 cbriHyPhy20gCbriGetCtxtSize(struct file *fileHndl);
INT4 cbriHyPhy20gCbriExtractCtxt(struct file *fileHndl, 
                                 void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 cbriHyPhy20gCbriRestoreCtxt(struct file *fileHndl, 
                                 void *pCtxtBuf, UINT4 *pParsedSize);
INT4 cbriHyPhy20gCbriAuditCtxt(struct file *fileHndl);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_OPI_H */
/* end of file */
