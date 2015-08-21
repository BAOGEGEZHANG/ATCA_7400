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
**  FILE        :  hyphy20g_serdes_features.h
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :  Contains hardware specific definitions
**
**  NOTES       :  The user check the contents of this file and modify
**                 the contents as per their system's hardware characteristics
**
*******************************************************************************/

#ifndef _HYPHY20G_SERDES_FEATURES_H
#define _HYPHY20G_SERDES_FEATURES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_serdes.h"

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* TEMPORARY DEFINES                                                          */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gXfiSerdesDiagLpbk(struct file *fileHndl, UINT4 slice,
                               UINT4 enable);
INT4 hyPhy20gSfpSerdesDiagLpbk(struct file *fileHndl, UINT4 slice, UINT4 block,
                               UINT4 enable);
INT4 hyPhy20gSfpSerdesLsciDiagLpbkLosInvCfg(struct file *fileHndl, UINT4 slice);
INT4 hyPhy20gBackplaneSerdesDiagLpbk(struct file *fileHndl, UINT4 sliceMsk,
                                     UINT4 enblMsk);
INT4 hyPhy20gPifSerdesDiagLpbk(struct file *fileHndl, UINT4 sliceMsk,
                               UINT4 enblMsk);
INT4 hyPhy20gSfpSerdesPattMonCfg(struct file *fileHndl,
                                 UINT4 linkId,
                                 UINT4 patType,
                                 UINT4 invPatt,
                                 UINT4 enable);
INT4 hyPhy20gSfpSerdesPattMonResync(struct file *fileHndl,
                                    UINT4 linkId);
INT4 hyPhy20gSfpSerdesPattMonStatus(struct file *fileHndl, UINT4 linkId,
                                    UINT4 intClear, UINT4 errCntUpdate,
                                    UINT4 *pStatusV, UINT4 *pStatusI,
                                    UINT4 *errorCount);
INT4 hyPhy20gSfpSerdesPattGenCfg(struct file *fileHndl,
                                 UINT4 linkId,
                                 UINT4 patType,
                                 UINT4 invPatt,
                                 UINT4 enable);
INT4 hyPhy20gSfpSerdesPattGenErrInj(struct file *fileHndl,
                                    UINT4 linkId);
INT4 hyPhy20gSgmiiSerdesPattMonCfg(struct file *fileHndl,
                                   UINT4 patType,
                                   UINT4 invPatt,
                                   UINT4 enable);
INT4 hyPhy20gSgmiiSerdesPattMonResync(struct file *fileHndl);
INT4 hyPhy20gSgmiiSerdesPattMonStatus(struct file *fileHndl, UINT4 intClear,
                                      UINT4 errCntUpdate, UINT4 *pStatusV,
                                      UINT4 *pStatusI, UINT4 *errorCount);
INT4 hyPhy20gSgmiiSerdesPattGenCfg(struct file *fileHndl,
                                   UINT4 patType,
                                   UINT4 invPatt,
                                   UINT4 enable);
INT4 hyPhy20gSgmiiSerdesPattGenErrInj(struct file *fileHndl);
INT4 hyPhy20gXfiSerdesPattMonCfg(struct file *fileHndl,
                                 UINT4 linkId,
                                 UINT4 patType,
                                 UINT4 invPatt,
                                 UINT4 enable);
INT4 hyPhy20gXfiSerdesPattMonResync(struct file *fileHndl,
                                    UINT4 linkId);
INT4 hyPhy20gXfiSerdesPattMonStatus(struct file *fileHndl, UINT4 linkId,
                                    UINT4 intClear, UINT4 errCntUpdate,
                                    UINT4 *pStatusV, UINT4 *pStatusI,
                                    UINT4 *errorCount);
INT4 hyPhy20gXfiSerdesPattGenCfg(struct file *fileHndl,
                                 UINT4 linkId,
                                 UINT4 patType,
                                 UINT4 invPatt,
                                 UINT4 enable);
INT4 hyPhy20gXfiSerdesPattGenErrInj(struct file *fileHndl,
                                    UINT4 linkId);
INT4 hyPhy20gPifSerdesPattMonCfg(struct file *fileHndl,
                                 UINT4 linkId,
                                 UINT4 patType,
                                 UINT4 invPatt,
                                 UINT4 enable);
INT4 hyPhy20gPifSerdesPattMonResync(struct file *fileHndl,
                                    UINT4 linkId);
INT4 hyPhy20gPifSerdesPattMonStatus(struct file *fileHndl,UINT4 linkId,
                                    UINT4 intClear, UINT4 errCntUpdate,
                                    UINT4 *pStatusV, UINT4 *pStatusI,
                                    UINT4 *errorCount);
INT4 hyPhy20gPifSerdesPattGenCfg(struct file *fileHndl,
                                 UINT4 linkId,
                                 UINT4 patType,
                                 UINT4 invPatt,
                                 UINT4 enable);
INT4 hyPhy20gPifSerdesPattGenErrInj(struct file *fileHndl,
                                    UINT4 linkId);
INT4 hyPhy20gBackplaneSerdesPattMonCfg(struct file *fileHndl,
                                       UINT4 linkId,
                                       UINT4 patType,
                                       UINT4 invPatt,
                                       UINT4 enable);
INT4 hyPhy20gBackplaneSerdesPattMonResync(struct file *fileHndl,
                                          UINT4 linkId);
INT4 hyPhy20gBackplaneSerdesPattMonStatus(struct file *fileHndl,UINT4 linkId,
                                          UINT4 intClear, UINT4 errCntUpdate,
                                          UINT4 *pStatusV, UINT4 *pStatusI,
                                          UINT4 *errorCount);
INT4 hyPhy20gBackplaneSerdesPattGenCfg(struct file *fileHndl,
                                       UINT4 linkId,
                                       UINT4 patType,
                                       UINT4 invPatt,
                                       UINT4 enable);
INT4 hyPhy20gBackplaneSerdesPattGenErrInj(struct file *fileHndl,
                                          UINT4 linkId);
INT4 hyPhy20gSfiSerdesSyncStatus(struct file *fileHndl, UINT4 slice,
                                 UINT4 intClear, UINT4 *pStatusV,
                                 UINT4 *pStatusI);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_SERDES__FEATURES_H */
/* end of file */
