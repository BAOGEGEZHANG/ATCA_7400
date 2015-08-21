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
**  FILE        :  hyphy20g_hsci_status.h
**
**  $Date: 2010-07-07 15:45:15 +0800 (Wed, 07 Jul 2010) $
**
**  $Revision: 8593 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_HSCI_STATUS_H
#define _HYPHY20G_HSCI_STATUS_H

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
INT4 hyPhy20gHsciTopLevelGetStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 *statusBits);

INT4 hyPhy20gHsci10GeGetStatus(struct file *fileHndl, UINT4 slice,
                               UINT4 *statusBits);

INT4 hyPhy20gHsci10GeGetTxxgStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gHsci10GeGetRxxgStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gHsciMstatxGetRxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits0,
                                   UINT4 *statusBits1);

INT4 hyPhy20gHsciMstatxGetTxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gHsciGsup43GetRxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 *statusBits);

INT4 hyPhy20gHsciGsup43GetTxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 *statusBits);

INT4 hyPhy20gHsciFcCbrGetStatus(struct file *fileHndl, UINT4 slice,
                                UINT4 *statusBits);

INT4 hyPhy20gHsciPmmGetStatus(struct file *fileHndl, UINT4 slice,
                              UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gEnetComGsup43GetTopStatus(struct file *fileHndl, 
                                       UINT4 *statusBits);

INT4 hyPhy20gEnetComGsup43GetRxStatus(struct file *fileHndl, UINT4 slice,
                                      UINT4 *statusBits);

INT4 hyPhy20gEnetComGsup43GetTxStatus(struct file *fileHndl, UINT4 slice,
                                      UINT4 *statusBits);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_HSCI_STATUS_H */
/* end of file */
