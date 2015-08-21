/******************************************************************************/
/**  COPYRIGHT (C); 2008 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
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
**  FILE        :  hyphy20g_mgmt.h
**
**  $Date: 2010-07-01 06:19:22 +0800 (Thu, 01 Jul 2010) $
**
**  $Revision: 8534 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_MGMT_STATUS_H
#define _HYPHY20G_MGMT_STATUS_H

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
INT4 hyPhy20gMgmtPortTopLevelGetStatus(struct file *fileHndl, UINT4 *statusBits);

INT4 hyPhy20gMgmtPortFegeGetStatus(struct file *fileHndl, UINT4 intClear,
                                   UINT4 *statusBits);

INT4 hyPhy20gMgmtPortL1rppGetStatus(struct file *fileHndl, UINT4 *statusBits);

INT4 hyPhy20gMgmtPortL1rppGetMiiANStatus(struct file *fileHndl, 
                                         UINT4 *statusBits0, UINT4 *statusBits1);

INT4 hyPhy20gMgmtPortFeGeMacGetStatus(struct file *fileHndl, UINT4 *statusBits);

INT4 hyPhy20gMgmtPortMstatxGetRxStatus(struct file *fileHndl, UINT4 intClear, 
                                       UINT4 *statusBits0, UINT4 *statusBits1);

INT4 hyPhy20gMgmtPortMstatxGetTxStatus(struct file *fileHndl, 
                                       UINT4 intClear, UINT4 *statusBits);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_MGMT_STATUS_H */
/* end of file */
