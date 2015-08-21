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
**  FILE        :  hyphy20g_gfp_features.h
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

#ifndef _HYPHY20G_GFP_FEATURES_H
#define _HYPHY20G_GFP_FEATURES_H

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
INT4 hyPhy20gGfpHdlcPrependCfg(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                               UINT4 hdrLength, UINT4 hdrData[4], UINT4 enable);
                               
INT4 hyPhy20gGfpHdlcCtrlFrmIns(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                               UINT4 ctrlFrmPrio, UINT4 ctrlFrmLength,
                               UINT4 ctrlFrmData[100]);
                                                                   
INT4 hyPhy20gGfpFDataExtHdrCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 cid, UINT4 hdrVersion,
                                 UINT4 enable);

                               
INT4 hyPhy20gGfpCmfIns(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                       UINT4 cmfPrio, UINT4 cmfLength, UINT4 cmFData[100]);
                       
INT4 hyPhy20gGfpRxFdfFrameRead(struct file *fileHndl, UINT4 slice, 
                                  UINT4 cpbId, UINT4 *fdfNumBytes,
                                  UINT4 *fdfErr, UINT4 *fdfData);
                                  
INT4 hyPhy20gGfpRxGetFailureId(struct file *fileHndl, UINT4 slice, 
                               UINT4 failureType, UINT4 clear,
                               UINT4 *gid, UINT4 *cid, UINT4 *type,
                               UINT4 *valid);                             

INT4 hyPhy20gGfpRxPauseCmfSliceCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 csfStThresh, UINT4 macDaMsb,
                                   UINT4 macDaLsb); 
                                   
INT4 hyPhy20gGfpRxPauseCmfChnlCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 cpbId, UINT4 csfDropEn, UINT4 csfMaxUpi,
                                  UINT4 pauseUpi, UINT4 pausePti);

#ifdef __cplusplus
}
#endif
                         
#endif /* _HYPHY20G_GFP_FEATURES_H */
/* end of file */
