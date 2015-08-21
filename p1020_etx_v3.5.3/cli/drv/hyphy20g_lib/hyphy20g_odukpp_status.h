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
**  FILE        :  hyphy20g_odukpp_status.h
**
**  $Date: 2010-01-13 16:09:30 -0800 (Wed, 13 Jan 2010) $
**
**  $Revision: 6161 $
**
**  DESCRIPTION :  Contains hardware specific definitions
**
**  NOTES       :  The user check the contents of this file and modify
**                 the contents as per their system's hardware characteristics
**
*******************************************************************************/

#ifndef _HYPHY20G_ODUKPP_STATUS_H
#define _HYPHY20G_ODUKPP_STATUS_H

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
INT4 hyPhy20gOdukppOtukRxGetStatus(struct file *fileHndl, UINT4 slice, 
                                   UINT4 intClear, UINT4 *statusBits);
INT4 hyPhy20gOdukppOtukTxGetStatus(struct file *fileHndl, UINT4 slice, 
                                   UINT4 intClear, UINT4 *statusBits);
INT4 hyPhy20gOdukppOtukRxGetFrameStatus(struct file *fileHndl, UINT4 slice, 
                                        UINT4 intClear, UINT4 *statusBits);
INT4 hyPhy20gOdukppOdukTxGetTcmiStatus(struct file *fileHndl, UINT4 slice, 
                                       UINT4 tcmiNum, UINT4 intClear, 
                                       UINT4 *statusBits);
INT4 hyPhy20gOdukppOdukRxGetStatus(struct file *fileHndl, UINT4 slice, 
                                   UINT4 intClear, UINT4 *statusBits);
INT4 hyPhy20gOdukppOdukRxGetApsPccStatus(struct file *fileHndl, UINT4 slice, 
                                         UINT4 intClear, UINT4 *statusBits);
INT4 hyPhy20gOdukppOdukRxGetTcmiStatus(struct file *fileHndl, UINT4 slice, 
                                       UINT4 tcmiNum, UINT4 intClear, 
                                       UINT4 *statusBits);
INT4 hyPhy20gOdukppOdukRxGetPmStatus(struct file *fileHndl, UINT4 slice, 
                                     UINT4 intClear, UINT4 *statusBits);
INT4 hyPhy20gOdukppOpukRxGetStatus(struct file *fileHndl, UINT4 slice, 
                                   UINT4 intClear, UINT4 *statusBits);
INT4 hyPhy20gOdukppOtukJatGetStatus(struct file *fileHndl, UINT4 slice,
                                    UINT4 *pStatus);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_ODUKPP_STATUS_H */         
/* end of file */ 
