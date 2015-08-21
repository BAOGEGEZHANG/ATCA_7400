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
**  FILE        :  hyphy20g_sonet_features.h
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

#ifndef _HYPHY20G_SONET_FEATURES_H
#define _HYPHY20G_SONET_FEATURES_H

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
INT4 hyPhy20gSonetTohTxCfgTtmLength(struct file *fileHndl, UINT4 slice, 
                                    UINT4 ttmLength);
INT4 hyPhy20gSonetPohTxCfgTtmLength(struct file *fileHndl, UINT4 slice, 
                                    UINT4 sts12Num, UINT4 sts3Num, 
                                    UINT4 sts1Num, UINT4 ttmLength);
INT4 hyPhy20gSonetPohRxGetSts1PayloadCfg(struct file *fileHndl, UINT4 slice, 
                                         UINT4 *statusWord1, UINT4 *statusWord2, 
                                         UINT4 *statusWord3);
INT4 hyPhy20gSonetPohTxGetSts1PayloadCfg(struct file *fileHndl, UINT4 slice, 
                                         UINT4 *statusWord1, UINT4 *statusWord2, 
                                         UINT4 *statusWord3);
INT4 hyPhy20gSonetTohRxCfgTtmLength(struct file *fileHndl, UINT4 slice, 
                                    UINT4 ttmLength);
INT4 hyPhy20gSonetPohRxCfgTtmLength(struct file *fileHndl, UINT4 slice, 
                                    UINT4 sts12Num, UINT4 sts3Num, 
                                    UINT4 sts1Num, UINT4 ttmLength);
INT4 hyPhy20gSonetTohRxCfg1ByteTtm(struct file *fileHndl, UINT4 slice, 
                                   UINT4 msg, UINT4 enblMode);
INT4 hyPhy20gSonetTohRxCfg16ByteTtm(struct file *fileHndl, UINT4 slice, 
                                    UINT4 msg[15], UINT4 enbl);
INT4 hyPhy20gSonetPohRxCfg16ByteTtm(struct file *fileHndl, UINT4 slice, 
                                    UINT4 sts12Num, UINT4 sts3Num, 
                                    UINT4 sts1Num, UINT4 msg[15], 
                                    UINT4 enblMode);
INT4 hyPhy20gSonetPohRxCfg64ByteTtm(struct file *fileHndl, UINT4 slice, 
                                    UINT4 sts12Num, UINT4 sts3Num, 
                                    UINT4 sts1Num, UINT4 msg[62], 
                                    UINT4 enblMode);
INT4 hyPhy20gSonetTohTxCfg1ByteTtm(struct file *fileHndl, UINT4 slice, 
                                   UINT4 msg, UINT4 enbl);
INT4 hyPhy20gSonetTohTxCfg16ByteTtm(struct file *fileHndl, UINT4 slice, 
                                    UINT4 msg[15], UINT4 enbl);
INT4 hyPhy20gSonetPohTxCfg16ByteTtm(struct file *fileHndl, UINT4 slice, 
                                    UINT4 sts12Num, UINT4 sts3Num, 
                                    UINT4 sts1Num, UINT4 msg[15], UINT4 enbl);
INT4 hyPhy20gSonetPohTxCfg64ByteTtm(struct file *fileHndl, UINT4 slice, 
                                    UINT4 sts12Num, UINT4 sts3Num, 
                                    UINT4 sts1Num, UINT4 msg[62], UINT4  enbl);
INT4 hyPhy20gSonetTohRxExt1ByteTtm(struct file *fileHndl, UINT4 slice, 
                                   UINT4 extType, UINT4 *extMsg);
INT4 hyPhy20gSonetTohRxExt16ByteTtm(struct file *fileHndl, UINT4 slice, 
                                    UINT4 extType, UINT4 *extE164Crc, 
                                    UINT4 *extMsg);
INT4 hyPhy20gSonetPohRxExt16ByteTtm(struct file *fileHndl, UINT4 slice, 
                                    UINT4 sts12Num, UINT4 sts3Num, 
                                    UINT4 sts1Num, UINT4 extType, 
                                    UINT4 *extE164Crc, UINT4 *extMsg);
INT4 hyPhy20gSonetPohRxExt64ByteTtm(struct file *fileHndl, UINT4 slice, 
                                    UINT4 sts12Num, UINT4 sts3Num, 
                                    UINT4 sts1Num, UINT4 extType, 
                                    UINT4 *extMsg);
INT4 hyPhy20gSonetTohOc3SdSfCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 berModeSD, UINT4 berModeSF);
INT4 hyPhy20gSonetTohOc12SdSfCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 berModeSD, UINT4 berModeSF);
INT4 hyPhy20gSonetTohOc48SdSfCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 berModeSD, UINT4 berModeSF);
INT4 hyPhy20gSonetTohOc192SdSfCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 berModeSD, UINT4 berModeSF);
INT4 hyPhy20gSonetPohSts1SdSfCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts12, UINT4 sts3, UINT4 sts1, 
                                 UINT4 berModeSD, UINT4 berModeSF);
INT4 hyPhy20gSonetPohSts3cSdSfCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 sts12, UINT4 sts3c, UINT4 berModeSD,
                                  UINT4 berModeSF);
INT4 hyPhy20gSonetPohSts12cSdSfCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 sts12c, UINT4 berModeSD, 
                                   UINT4 berModeSF);
INT4 hyPhy20gSonetPohSts48cSdSfCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 berModeSD, UINT4 berModeSF);
INT4 hyPhy20gSonetPohSts192cSdSfCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 berModeSD, UINT4 berModeSF);
INT4 hyPhy20gSonetPohPslPdiCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 sts12, UINT4 sts3, UINT4 sts1, 
                               UINT4 pslValue, UINT4 pdiRangeEn, UINT4 pdiVal);
INT4 hyPhy20gSonetPohExtPsl(struct file *fileHndl, UINT4 slice, UINT4 sts12,
                            UINT4 sts3, UINT4 sts1, UINT4 extType,
                            UINT4 *extMsg);
INT4 hyPhy20gSonetTohRxExtSsm(struct file *fileHndl, UINT4 slice, 
                              UINT4 byteSsm, UINT4 filterEn,
                              UINT4 *extMsg);
INT4 hyPhy20gSonetTohRxExtCntrs(struct file *fileHndl, UINT4 slice, 
                                UINT4 accessMode, UINT4 *extBipS, 
                                UINT4 *extBipL, UINT4 *extReiL);
INT4 hyPhy20gSonetPohRxExtCntrs(struct file *fileHndl, UINT4 slice, 
                                UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num,
                                UINT4 accessMode, UINT4 *extBipP, 
                                UINT4 *extReiP);
INT4 hyPhy20gSonetR1iInsCfg(struct file *fileHndl, UINT4 slice, UINT4 mode, 
                            UINT4 r1Pos, UINT4 enable);
INT4 hyPhy20gSonetR1iTxMsgCfg(struct file *fileHndl, UINT4 slice, 
                              UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num, 
                              UINT4 txEsm, UINT4 txCid);
INT4 hyPhy20gSonetR1iTxErrIns(struct file *fileHndl, UINT4 slice, 
                              UINT4 sts12Num, UINT4 sts3Num, 
                              UINT4 sts1Num, UINT4 esmSbe, UINT4 cidSbe,
                              UINT4 crcSbe, UINT4 frmErr, UINT4 frmSbe);
INT4 hyPhy20gSonetR1iDetCfg(struct file *fileHndl, UINT4 slice, UINT4 mode, 
                            UINT4 r1Pos, UINT4 enable);
INT4 hyPhy20gSonetR1iRxExpCidCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 sts12Num, UINT4 sts3Num, 
                                 UINT4 sts1Num, UINT4 expCid);
INT4 hyPhy20gSonetR1iRxMsgStatus(struct file *fileHndl, UINT4 slice, 
                                 UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num, 
                                 UINT4 *statusBits, UINT4 *accESM, 
                                 UINT4 *crcCnt);
INT4 hyPhy20gSonetR1iRxMsgData(struct file *fileHndl, UINT4 slice, 
                               UINT4 sts12um, UINT4 sts3Num, UINT4 sts1Num, 
                               UINT4 *extMsg2, UINT4 *extMsg1);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_SONET_FEATURES_H */         
/* end of file */
