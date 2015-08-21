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
**  FILE        :  hyphy20g_opi_features.h
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_OPI_FEATURES_H
#define _HYPHY20G_OPI_FEATURES_H

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
INT4 hyPhy20gCbrbFailSrcCfg(struct file *fileHndl, UINT4 port, UINT4 oofEnbl,
                            UINT4 aisEnbl, UINT4 ptmEnbl, UINT4 vcPtmEnbl,
                            UINT4 ociEnbl, UINT4 soakEnbl, UINT4 lckEnbl);

INT4 hyPhy20gCbrbFailProcCfg(struct file *fileHndl, UINT4 port,
                             UINT4 ptExp, UINT4 vcPtExp, UINT4 soakOofTmr);

INT4 hyPhy20gCbrbMiniFrmrGetStatus(struct file *fileHndl, UINT4 *pOofStat,
                                   UINT4 *pAisStat, UINT4 *pLckStat,
                                   UINT4 *pOciStat, UINT4 *pPtmStat,
                                   UINT4 *pVcPtmStat);

INT4 hyPhy20gCbrbPktMonGetStatus(struct file *fileHndl, UINT4 intClear,
                                 UINT4 *pStatus0, UINT4 *pStatus1,
                                 UINT4 *pStatus2, UINT4 *pStatus3, 
                                 UINT4 *pStatus4, UINT4 *pStatus5);

INT4 hyPhy20gCbrbPktMonGetRxCsiValue(struct file *fileHndl, UINT4 pmm,
                                     UINT4 *pMonCsiOut);

INT4 hyPhy20gCbrbPktMkrGetStatus(struct file *fileHndl, UINT4 intClear,
                                 UINT4 *pStatus0, UINT4 *pStatus1,
                                 UINT4 *pStatus2);

INT4 hyPhy20gCbrbFcGetStatus(struct file *fileHndl, UINT4 intClear,
                             UINT4 *pStatus0, UINT4 *pStatus1);

INT4 hyPhy20gCbrbFifoGetStatus(struct file *fileHndl, UINT4 intClear,
                               UINT4 *pStatus0, UINT4 *pStatus1);

INT4 hyPhy20gCbriShimGetStatus(struct file *fileHndl, UINT4 slice,
                               UINT4 *pStatusBits);

INT4 hyPhy20gCbriCoreRxIntGetStatus(struct file *fileHndl, UINT4 slice,
                                    UINT4 intClear, UINT4 *pStatusBits);

INT4 hyPhy20gCbriCoreTxIntGetStatus(struct file *fileHndl, UINT4 slice,
                                    UINT4 intClear, UINT4 *pStatusBits);

INT4 hyPhy20gCbriCoreTxCounterGetStatus(struct file *fileHndl, UINT4 slice,
                                        UINT4 intClear, UINT4 *pStatusBits);

INT4 hyPhy20gCbriCoreRxCounterGetStatus(struct file *fileHndl, UINT4 slice, 
                                        UINT4 intClear, UINT4 *pCntFill, 
                                        UINT4 *pCrc32CntFill, 
                                        UINT4 *pWordCntFill);

INT4 hyPhy20gCbriCoreRxLaneGetStatus(struct file *fileHndl, UINT4 slice, 
                                     UINT4 intClear, UINT4 *pMfErr, 
                                     UINT4 *pCtlWrdErr, UINT4 *pScrbStateErr, 
                                     UINT4 *pCrc32Err,
                                     UINT4 *pWrdSyncChng, UINT4 *pWrdSyncStat, 
                                     UINT4 *pAlignStatus);

INT4 hyPhy20gCbriCoreDiagLpbk(struct file *fileHndl, UINT4 slice, UINT4 enable);

INT4 hyPhy20gCbriSerdesTestCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                               UINT4 mode, UINT4 prbs, UINT4 patternA, 
                               UINT4 patternB, UINT4 patternC, UINT4 repA, 
                               UINT4 repB, UINT4 repC);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_OPI_FEATURES_H */
/* end of file */
