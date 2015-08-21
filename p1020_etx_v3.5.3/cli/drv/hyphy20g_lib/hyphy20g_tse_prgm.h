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
**  FILE        :  hyphy20g_tse_prgm.h
**
**  $Date: 2011-01-20 03:50:23 +0800 (Thu, 20 Jan 2011) $
**
**  $Revision: 10601 $
**
**  DESCRIPTION :  Contains hardware specific definitions
**
**  NOTES       :  The user check the contents of this file and modify
**                 the contents as per their system's hardware characteristics
**
*******************************************************************************/

#ifndef _HYPHY20G_TSE_PRGM_H
#define _HYPHY20G_TSE_PRGM_H

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
INT4 hyPhy20gTsePrgmInit(struct file *fileHndl, UINT4 enable);
INT4 hyPhy20gTsePrgmTxSts48PayloadCfg(struct file *fileHndl, UINT4 pldType);
INT4 hyPhy20gTsePrgmTxSts12PayloadCfg(struct file *fileHndl, UINT4 prgmNum, 
                                      UINT4 pldType);
INT4 hyPhy20gTsePrgmTxSts3PayloadCfg(struct file *fileHndl, UINT4 prgmNum,
                                     UINT4 sts3Num, UINT4 pldType);
INT4 hyPhy20gTsePrgmRxSts48PayloadCfg(struct file *fileHndl, UINT4 pldType);
INT4 hyPhy20gTsePrgmRxSts12PayloadCfg(struct file *fileHndl, UINT4 prgmNum, 
                                      UINT4 pldType);
INT4 hyPhy20gTsePrgmRxSts3PayloadCfg(struct file *fileHndl, UINT4 prgmNum, 
                                     UINT4 sts3Num, UINT4 pldType);
INT4 hyPhy20gTsePrgmTxPatternCfg(struct file *fileHndl, UINT4 prgmNum,
                                 UINT4 path, UINT4 rate, UINT4 patternType);
INT4 hyPhy20gTsePrgmRxPatternCfg(struct file *fileHndl, UINT4 prgmNum,
                                 UINT4 path, UINT4 rate, UINT4 patternType);
INT4 hyPhy20gTsePrgmRxErrCnt(struct file *fileHndl, UINT4 prgmNum, 
                             UINT4 path, UINT4 accessMode, UINT4 *errCnt);
INT4 hyPhy20gTsePrgmRxResync(struct file *fileHndl, UINT4 prgmNum, UINT4 path);
INT4 hyPhy20gTsePrgmTxErrIns(struct file *fileHndl, UINT4 prgmNum, UINT4 path);
INT4 hyPhy20gTsePrgmRxPortSel(struct file *fileHndl, UINT4 prgmSrc);
INT4 hyPhy20gTsePrgmRxStatus(struct file *fileHndl, UINT4 prgmNum, 
                             UINT4 intClear, UINT4 *statusInts, 
                             UINT4 *statusVals);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_TSE_PRGM_H */         
/* end of file */
