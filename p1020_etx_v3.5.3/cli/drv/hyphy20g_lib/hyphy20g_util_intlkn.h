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
**  FILE        :  hyPhy20g_util_intlkn.h
**
**  $Date: 2010-07-01 06:19:22 +0800 (Thu, 01 Jul 2010) $
**
**  $Revision: 8534 $
**
**  DESCRIPTION :  This is the header file for hyPhy20g_util_intlkn.c file.
**
**  NOTES       :
**
*******************************************************************************/

#ifndef _HYPHY20G_UTIL_INTLKN_H
#define _HYPHY20G_UTIL_INTLKN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_indirect.h"
#include "hyphy20g_serdes.h"

/******************************************************************************/
/* COMMON MACRO DEFINITIONS                                                   */
/******************************************************************************/

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

#define HYPHY20G_INTLKN_META_FRAME_PEROD      2048
#define HYPHY20G_INTLKN_EXTERNAL_LOOP_BACK    0x01
#define HYPHY20G_INTLKN_INTERNAL_LOOP_BACK    0x02

/******************************************************************************/
/* FUNCTION PROTOTYPES                                                        */
/******************************************************************************/
INT4 utilHyPhy20gIntlknCoreEnable(struct file *fileHndl, UINT4 slice,
                                  UINT4 enable);
INT4 utilHyPhy20gIntlknCoreGetMajBiDirState(struct file *fileHndl, UINT4 slice,
                                            UINT4 *pState);

INT4 utilHyPhy20gIntlknCoreCfg(struct file *fileHndl, UINT4 slice);

INT4 utilHyPhy20gIntlknCoreTxRateLimitCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 rateLimit, UINT4 burstLimit,
                                          UINT4 byteGran);

INT4 utilHyPhy20gIntlknCoreLaneDisable(struct file *fileHndl, UINT4 slice,
                                       UINT4 disableMsk);

INT4 utilHyPhy20gIntlknCoreFcModeCfg(struct file *fileHndl, UINT4 slice, 
                                     UINT4 dir, UINT4 mode, UINT4 ctxt);

INT4 utilHyPhy20gIntlknCoreFcCalCfg(struct file *fileHndl, UINT4 slice, 
                                    UINT4 dir, UINT4 calAddr, UINT4 fcPort);

INT4 utilHyPhy20gIntlknCoreSerdesPtrnCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 dir, UINT4 enable);

INT4 utilHyPhy20gIntlknCoreSetLoopback(struct file *fileHndl, UINT4 slice,
                                       UINT4 loopType);

INT4 utilHyPhy20gIntlknShimSetLoopback(struct file *fileHndl, UINT4 slice,
                                       UINT4 loopType);

INT4 utilHyPhy20gIntlknShimCfg(struct file *fileHndl, UINT4 slice, UINT4 mode);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_UTIL_INTLKN_H */
/* end of file */
