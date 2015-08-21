/******************************************************************************/
/**  COPYRIGHT (C); 2008 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
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
**  FILE        :  hyphy20g_pif_status.h
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

#ifndef _HYPHY20G_PIF_STATUS_H
#define _HYPHY20G_PIF_STATUS_H

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
INT4 hyPhy20gPifTopLevelGetStatus(struct file *fileHndl, UINT4 *statusBits);

INT4 hyPhy20gPifXauiGetStatus(struct file *fileHndl, UINT4 slice,
                              UINT4 *statusBits);

INT4 hyPhy20gPifXauiGetLaneStatus(struct file *fileHndl, UINT4 slice,
                                  UINT4 *statusBits);

INT4 hyPhy20gPifXauiGetTxxgStatus(struct file *fileHndl, UINT4 slice,
                                  UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gPifXauiGetRxxgStatus(struct file *fileHndl, UINT4 slice,
                                  UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gPifIntlknShimGetStatus(struct file *fileHndl, UINT4 *pStatusBits);

INT4 hyPhy20gPifIntlknCoreRxIntGetStatus(struct file *fileHndl, UINT4 intClear,
                                         UINT4 *pStatusBits);
INT4 hyPhy20gPifIntlknCoreTxIntGetStatus(struct file *fileHndl, UINT4 intClear,
                                         UINT4 *pStatusBits);

INT4 hyPhy20gPifIntlknCoreTxCounterGetStatus(struct file *fileHndl,
                                             UINT4 intClear,
                                             UINT4 *pStatusBits);

INT4 hyPhy20gPifIntlknCoreRxCounterGetStatus(struct file *fileHndl,
                                             UINT4 intClear, UINT4 *pCntFill,
                                             UINT4 *pCrc32CntFill,
                                             UINT4 *pWordCntFill);

INT4 hyPhy20gPifIntlknCoreRxLaneGetStatus(struct file *fileHndl,
                                          UINT4 intClear, UINT4 *pMfErr,
                                          UINT4 *pCtlWrdErr,
                                          UINT4 *pScrbStateErr,
                                          UINT4 *pCrc32Err,
                                          UINT4 *pWrdSyncChng,
                                          UINT4 *pWrdSyncStat,
                                          UINT4 *pAlignStatus);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_PIF_STATUS_H */
/* end of file */
