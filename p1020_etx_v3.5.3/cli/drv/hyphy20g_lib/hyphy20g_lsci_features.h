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
**  FILE        :  hyphy20g_lsci_features.h
**
**  $Date: 2012-11-30 16:03:07 +0800 (Fri, 30 Nov 2012) $
**
**  $Revision: 29336 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_LSCI_FEATURES_H
#define _HYPHY20G_LSCI_FEATURES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_lsci.h"

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
INT4 hyPhy20gLsciFeGePauseFrmCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 pauseTime, UINT4 pauseInt);

INT4 hyPhy20gLsciFeGeSendFefi(struct file *fileHndl, UINT4 slice, UINT4 fefiEn);

INT4 hyPhy20gLsciGeGfpTPattCfg(struct file *fileHndl, UINT4 slice, UINT4 type,
                               UINT4 pattern, UINT4 pattEn);

INT4 hyPhy20gLsciFcCbrPattForce(struct file *fileHndl, UINT4 slice, UINT4 dir,
                                UINT4 mode);

INT4 hyPhy20gLsciByteAlignedCbrUsrPattCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 dir, UINT4 mode,
                                          UINT4 userChar);

INT4 hyPhy20gLsci8b10bCbrUsrPattCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 dir, UINT4 userChar[4],
                                    UINT4 userCharDisp);

INT4 hyPhy20gLsciFcUsrPattCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                              UINT4 userChar[4]);

INT4 hyPhy20gLsciEsconUsrPattCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                                 UINT4 userChar[4]);

INT4 hyPhy20gLsciGeReAutoNeg(struct file *fileHndl, UINT4 slice);

INT4 hyPhy20gLsciGeAnBasePageCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 remoteFlt, UINT4 pauseVal,
                                 UINT4 reNegFlg);

INT4 hyPhy20gLsciGeAnModeUpdate(struct file *fileHndl, UINT4 slice,
                                UINT4 anMode);

INT4 hyPhy20gLsciUniDirEn(struct file *fileHndl, UINT4 slice, UINT4 enbl);

INT4 hyPhy20gLsciFeGePattCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                             UINT4 ptrnSel, UINT4 ptrnVal);

INT4 hyPhy20gLsciGeGfpTSofPattDetectCfg(struct file *fileHndl,UINT4 slice,UINT4 enable);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_LSCI_FEATURES_H */
/* end of file */
