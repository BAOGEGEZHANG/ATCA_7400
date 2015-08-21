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
**  MODULE      : HyPHY 20G SGMII Subsystem Header File
**
**  FILE        :  hyphy20g_mgmt_features.h
**
**  $Date: 2010-03-06 02:00:30 +0800 (星期六, 06 三月 2010) $
**
**  $Revision: 6830 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_MGMT_FEATURES_H
#define _HYPHY20G_MGMT_FEATURES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_mgmt.h"

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
INT4 hyPhy20gMgmtPortFeGePauseFrmCfg(struct file *fileHndl, UINT4 pauseTime, 
                                     UINT4 pauseInt);

INT4 hyPhy20gMgmtPortFeGeSendFefi(struct file *fileHndl, UINT4 fefiEn);

INT4 hyPhy20gMgmtPortGeReAutoNeg(struct file *fileHndl);

INT4 hyPhy20gMgmtPortGeAnBasePageCfg(struct file *fileHndl, UINT4 remoteFlt,
                                     UINT4 pauseVal, UINT4 reNegFlg);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_MGMT_FEATURES_H */
/* end of file */
