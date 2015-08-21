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
**  FILE        :  hyphy20g_otn_status.h
**
**  $Date: 2011-01-28 06:49:31 +0800 (Fri, 28 Jan 2011) $
**
**  $Revision: 10660 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_OTN_STATUS_H
#define _HYPHY20G_OTN_STATUS_H

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
INT4 hyPhy20gOtukTxGetStatus(struct file *fileHndl, UINT4 slice, 
                             UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gOdukTxGetTcmiStatus(struct file *fileHndl, UINT4 slice, 
                                 UINT4 tcmiNum, UINT4 intClear, 
                                 UINT4 *statusBits);

INT4 hyPhy20gOtukRxGetFrameStatus(struct file *fileHndl, UINT4 slice, 
                                  UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gOtukRxGetStatus(struct file *fileHndl, UINT4 slice, 
                             UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gOdukRxGetStatus(struct file *fileHndl, UINT4 slice, 
                             UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gOdukRxGetApsPccStatus(struct file *fileHndl, UINT4 slice, 
                                   UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gOdukRxGetTcmiStatus(struct file *fileHndl, UINT4 slice,
                                 UINT4 tcmiNum, UINT4 intClear, 
                                 UINT4 *statusBits);

INT4 hyPhy20gOdukRxGetPmStatus(struct file *fileHndl, UINT4 slice, 
                               UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gOpukRxGetStatus(struct file *fileHndl, UINT4 slice, UINT4 intClear, 
                             UINT4 *statusBits);

INT4 hyPhy20gOpukRxGetPayloadStatus (struct file *fileHndl, UINT4 slice, 
                            UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gOtukJatGetStatus(struct file *fileHndl, UINT4 slice,
                              UINT4 *pStatus);

INT4 hyPhy20gOtuEfecGetStatus(struct file *fileHndl, UINT4 slice,
                              UINT4 intClear, UINT4 *pStatus);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                 */
/******************************************************************************/  

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_OTN_STATUS_H */
/* end of file */
