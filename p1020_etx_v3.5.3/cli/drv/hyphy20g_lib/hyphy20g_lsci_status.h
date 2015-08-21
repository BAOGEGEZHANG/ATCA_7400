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
**  FILE        :  hyphy20g_lsci_status.h
**
**  $Date: 2011-04-28 14:18:56 +0800 (Thu, 28 Apr 2011) $
**
**  $Revision: 11851 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_LSCI_STATUS_H
#define _HYPHY20G_LSCI_STATUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_util_enet.h"

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
INT4 hyPhy20gLsciTopLevelGetStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 *statusBits);

INT4 hyPhy20gLsciFegeGetStatus(struct file *fileHndl, UINT4 slice,
                               UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gLsciL1rppGetStatus(struct file *fileHndl, UINT4 slice,
                                UINT4 *statusBits);

INT4 hyPhy20gLsciL1rppGetMiiANStatus(struct file *fileHndl, UINT4 slice,
                                     UINT4 *statusBits0, UINT4 *statusBits1);

INT4 hyPhy20gLsciFeGeMacGetStatus(struct file *fileHndl, UINT4 slice,
                                  UINT4 *statusBits);

INT4 hyPhy20gLsciMstatxGetRxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits0,
                                   UINT4 *statusBits1);

INT4 hyPhy20gLsciMstatxGetTxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gLsciGeGfptGetStatus(struct file *fileHndl, UINT4 slice,
                                 UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gLsciFcCbrGetStatus(struct file *fileHndl, UINT4 slice,
                                UINT4 *statusBits);

INT4 hyPhy20gLsciPmmGetStatus(struct file *fileHndl, UINT4 slice,
                              UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gLsciL1rppGetPattStatus(struct file *fileHndl, UINT4 slice,
                                    UINT4 *statusBits);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_LSCI_STATUS_H */
/* end of file */

