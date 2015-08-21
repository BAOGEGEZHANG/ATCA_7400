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
**  FILE        :  hyphy20g_cpb_features.h
**
**  $Date: 2010-12-16 06:04:49 +0800 (Thu, 16 Dec 2010) $
**
**  $Revision: 10348 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
*******************************************************************************/

#ifndef _HYPHY20G_CPB_FEATURES_H
#define _HYPHY20G_CPB_FEATURES_H

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
INT4 hyPhy20gCpbQdrBistTest(struct file *fileHndl, UINT4 startAddr,
                            UINT4 endAddr, UINT4 wrPatt[13], UINT4 *testResult,
                            UINT4 *burstErr, UINT4 *addrErr, UINT4 *rdData);

INT4 hyPhy20gCpbQdrBistWrite(struct file *fileHndl, UINT4 startAddr,
                             UINT4 endAddr, UINT4 wrPatt[13]);

INT4 hyPhy20gCpbQdrBistReadTest(struct file *fileHndl, UINT4 startAddr,
                                UINT4 endAddr, UINT4 wrPatt[13],
                                UINT4 *testResult, UINT4 *burstErr,
                                UINT4 *addrErr, UINT4 *rdData);

INT4 hyPhy20gCpbQdrDebugPortWrite(struct file *fileHndl, UINT4 wrAddr,
                                  UINT4 wrPatt[12]);

INT4 hyPhy20gCpbQdrDebugPortRead(struct file *fileHndl, UINT4 rdAddr,
                                 UINT4 *rdData);

INT4 hyPhy20gCpbTrigDropCnt(struct file *fileHndl);

INT4 hyPhy20gCpbGetChnlDropCnt(struct file *fileHndl, UINT4 chnl, UINT4 *pCnt);


#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_CPB_FEATURES_H */
/* end of file */
