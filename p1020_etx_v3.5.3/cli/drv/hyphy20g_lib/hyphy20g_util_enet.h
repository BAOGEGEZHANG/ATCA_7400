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
**  FILE        : hyphy20g_util_enet.h
**
**  $Date: 2010-07-26 16:43:40 +0800 (Mon, 26 Jul 2010) $
**
**  $Revision: 8861 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_UTIL_ENET_H
#define _HYPHY20G_UTIL_ENET_H

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
INT4 utilHyPhy20gMstatxGetRxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits0,
                                   UINT4 *statusBits1);

INT4 utilHyPhy20gMstatxGetTxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits);

INT4 utilHyPhy20gFcCbrGetStatus(struct file *fileHndl, UINT4 slice,
                                UINT4 *statusBits);

INT4 utilHyPhy20gEthStatGetCounts(struct file *fileHndl, UINT4 slice, 
                                  UINT4 ctrlMsk, UINT4 counter, UINT4 *pCount0,
                                  UINT4 *pCount1, UINT4 *pCount2);

INT4 utilHyPhy20gEthStatVarCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 dir, UINT4 index);

INT4 utilHyPhy20gFcCbrTxGetCounts(struct file *fileHndl, UINT4 slice,
                                  UINT4 update, UINT4 counter, UINT4 *pCount);

INT4 utilHyPhy20gFcCbrRxGetCounts(struct file *fileHndl, UINT4 slice,
                                  UINT4 update, UINT4 counter, UINT4 *pCount);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_UTIL_ENET_H */
/* end of file */
