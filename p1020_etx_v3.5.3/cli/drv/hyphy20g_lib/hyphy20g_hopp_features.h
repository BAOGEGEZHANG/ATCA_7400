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
**  FILE        :  hyphy20g_hopp_features.h
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

#ifndef _HYPHY20G_HOPP_FEATURES_H
#define _HYPHY20G_HOPP_FEATURES_H

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
INT4 hyPhy20gHoppPohTxCfgTtmLength(struct file *fileHndl, UINT4 slice, 
                                   UINT4 sts12Num, UINT4 sts3Num, 
                                   UINT4 sts1Num, UINT4 ttmLength);
INT4 hyPhy20gHoppPohTxCfg16ByteTtm(struct file *fileHndl, UINT4 slice, 
                                   UINT4 sts12Num, UINT4 sts3Num, 
                                   UINT4 sts1Num, UINT4 msg[15], UINT4 enbl);
INT4 hyPhy20gHoppPohTxCfg64ByteTtm(struct file *fileHndl, UINT4 slice, 
                                   UINT4 sts12Num, UINT4 sts3Num, 
                                   UINT4 sts1Num, UINT4 msg[62], UINT4  enbl);
INT4 hyPhy20gHoppPohRxCfgTtmLength(struct file *fileHndl, UINT4 slice, 
                                   UINT4 sts12Num, UINT4 sts3Num, 
                                   UINT4 sts1Num, UINT4 ttmLength);
INT4 hyPhy20gHoppPohRxCfg16ByteTtm(struct file *fileHndl, UINT4 slice, 
                                   UINT4 sts12Num, UINT4 sts3Num, 
                                   UINT4 sts1Num, UINT4 msg[15], 
                                   UINT4 enblMode);
INT4 hyPhy20gHoppPohRxCfg64ByteTtm(struct file *fileHndl, UINT4 slice, 
                                   UINT4 sts12Num, UINT4 sts3Num, 
                                   UINT4 sts1Num, UINT4 msg[62], 
                                   UINT4 enblMode);
INT4 hyPhy20gHoppPohRxExt16ByteTtm(struct file *fileHndl, UINT4 slice, 
                                   UINT4 sts12Num, UINT4 sts3Num, 
                                   UINT4 sts1Num, UINT4 extType, 
                                   UINT4 *extE164Crc, UINT4 *extMsg);
INT4 hyPhy20gHoppPohRxExt64ByteTtm(struct file *fileHndl, UINT4 slice, 
                                   UINT4 sts12Num, UINT4 sts3Num, 
                                   UINT4 sts1Num, UINT4 extType, 
                                   UINT4 *extMsg);
INT4 hyPhy20gHoppPohSts1SdSfCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 sts12Num, UINT4 sts3Num, 
                                UINT4 sts1Num, UINT4 berModeSD, 
                                UINT4 berModeSF);
INT4 hyPhy20gHoppPohSts3cSdSfCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts12Num, UINT4 sts3c, 
                                 UINT4 berModeSD, UINT4 berModeSF);
INT4 hyPhy20gHoppPohSts12cSdSfCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 sts12c, UINT4 berModeSD, 
                                  UINT4 berModeSF);
INT4 hyPhy20gHoppPohSts48cSdSfCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 berModeSD, UINT4 berModeSF);
INT4 hyPhy20gHoppPohSts192cSdSfCfg(struct file *fileHndl, UINT4 berModeSD, 
                                   UINT4 berModeSF);
INT4 hyPhy20gHoppPohPslPdiCfg(struct file *fileHndl, UINT4 slice,
                              UINT4 sts12, UINT4 sts3, UINT4 sts1, 
                              UINT4 pslValue, UINT4 pdiRangeEn, UINT4 pdiVal);
INT4 hyPhy20gHoppPohExtPsl(struct file *fileHndl, UINT4 slice, 
                           UINT4 sts12, UINT4 sts3, UINT4 sts1, 
                           UINT4 extType, UINT4 *extMsg);
INT4 hyPhy20gHoppPohRxCntrs(struct file *fileHndl, UINT4 slice, 
                            UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num,
                            UINT4 accessMode, UINT4 *extBipP, UINT4 *extReiP);
INT4 hyPhy20gHoppPohCfgRdiModeP(struct file *fileHndl, UINT4 slice, 
                                 UINT4 rdiMode);
INT4 hyPhy20gHoppPohTxCfgRdiReiP(struct file *fileHndl, UINT4 slice, 
                                 UINT4 sts12, UINT4 sts3, UINT4 sts1, 
                                 UINT4 erdiMode, UINT4 reiMode, 
                                 UINT4 rdiErdiEnbl);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_HOPP__FEATURES_H */         
/* end of file */
