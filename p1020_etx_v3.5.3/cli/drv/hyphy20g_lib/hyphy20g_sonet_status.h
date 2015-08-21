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
**  FILE        :  hyphy20g_sonet_status.h
**
**  $Date: 2010-07-01 06:19:22 +0800 (Thu, 01 Jul 2010) $
**
**  $Revision: 8534 $
**
**  DESCRIPTION :  Contains hardware specific definitions
**
**  NOTES       :  The user check the contents of this file and modify
**                 the contents as per their system's hardware characteristics
**
*******************************************************************************/

#ifndef _HYPHY20G_SONET_STATUS_H
#define _HYPHY20G_SONET_STATUS_H

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

INT4 hyPhy20gSonetTohGetStatus(struct file *fileHndl, UINT4 slice, 
                               UINT4 *statusBits);
INT4 hyPhy20gSonetPohGetStatus(struct file *fileHndl, UINT4 slice, 
                               UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num, 
                               UINT4 *statusBits);
INT4 hyPhy20gSonetPohGetAisStatus(struct file *fileHndl, UINT4 slice, 
                                   UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num,
                                   UINT4 dir, UINT4 *aisStatus);
INT4 hyPhy20gSonetPohGetG1Status(struct file *fileHndl, UINT4 slice, 
                                 UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num, 
                                 UINT4 *statusBits, UINT4 *filtErdiVal);
INT4 hyPhy20gSonetPohGetC2Status(struct file *fileHndl, UINT4 slice, 
                                 UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num, 
                                 UINT4 *statusBits, UINT4 *cPslVal, 
                                 UINT4 *aPslVal, UINT4 *ePslVal);
INT4 hyPhy20gSonetPohGetPtrStatus(struct file *fileHndl, UINT4 slice, 
                                  UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num, 
                                  UINT4 dir, UINT4 *statusBits);
INT4 hyPhy20gSonetPohTxPtrIntprtGetStatus(struct file *fileHndl, UINT4 slice, 
                                          UINT4 sts12Num, UINT4 sts3Num, 
                                          UINT4 sts1Num, UINT4 *statusBits);
INT4 hyPhy20gSonetPohRxPtrIntprtGetStatus(struct file *fileHndl, UINT4 slice,
                                          UINT4 sts12Num, UINT4 sts3Num, 
                                          UINT4 sts1Num, UINT4 *statusBits);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_SONET_STATUS_H */         
/* end of file */
