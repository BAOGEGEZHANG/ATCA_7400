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
**  FILE        :  hyphy20g_sonet_ln.h
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

#ifndef _HYPHY20G_SONET_LN_H
#define _HYPHY20G_SONET_LN_H

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

INT4 hyPhy20gSonetLnInit(struct file *fileHndl, UINT4 enable);

INT4 hyPhy20gSonetLnXfiSfiDataPathCfg(struct file *fileHndl, UINT4 intf,
                                      UINT4 block, UINT4 timingMd);

INT4 hyPhy20gSonetLnXfiTxMulticastCfg(struct file *fileHndl, UINT4 block);

INT4 hyPhy20gSonetLnOtu2Odu2SysDataPathCfg(struct file *fileHndl, UINT4 sysIntf, 
                                           UINT4 block);

INT4 hyPhy20gSonetLnSonetSysDataPathCfg(struct file *fileHndl, UINT4 sysIntf,
                                        UINT4 block, UINT4 gfpEn,
                                        UINT4 timingMd);

INT4 hyPhy20gSonetLnXfiMonDataPathCfg(struct file *fileHndl, UINT4 intf, 
                                      UINT4 block);

INT4 hyPhy20gSonetLnSfpDataPathCfg(struct file *fileHndl, UINT4 intf, 
                                   UINT4 block, UINT4 sonetRate);

INT4 hyPhy20gSonetLnSfpMonDataPathCfg(struct file *fileHndl, UINT4 intf, 
                                      UINT4 block, UINT4 sonetRate);

INT4 hyPhy20gSonetLnOtu1Odu1SysDataPathCfg(struct file *fileHndl, UINT4 sysIntf,
                                           UINT4 block);

INT4 hyPhy20gSonetLnOtu1Odu1SysDataPathCfgEx(struct file *fileHndl, UINT4 sysIntf,
                   UINT4 block,UINT4 txClkSrc,UINT4 srliReset, UINT4 sfbaReset);

INT4 hyPhy20gSonetLnXfiSrcCSel(struct file *fileHndl, UINT4 slice, UINT4 source);

INT4 hyPhy20gSonetLnXfiSrcBSel(struct file *fileHndl, UINT4 slice, UINT4 source);

INT4 hyPhy20gSonetLnSfpSrcCSel(struct file *fileHndl, UINT4 slice, UINT4 source);

INT4 hyPhy20gSonetLnSfpSrcBSel(struct file *fileHndl, UINT4 slice, UINT4 source);

INT4 hyPhy20gSonetLnLineTimeSrcCfg(struct file *fileHndl, UINT4 lineTmClk,
                                   UINT4 sonetGrp);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/ 

INT4 utilHyPhy20gSonetLnGetSignalRate(struct file *fileHndl, UINT4 slice,
                                      UINT4 *rate);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_SONET_LN_H */
/* end of file */
