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
**  FILE        :  hyphy20g_odukpp_counters.h
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

#ifndef _HYPHY20G_ODUKPP_COUNTERS_H
#define _HYPHY20G_ODUKPP_COUNTERS_H

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
INT4 hyPhy20gOdukppOpukTxGetCounts(struct file *fileHndl, UINT4 slice,
                                   UINT4 accessMode, UINT4 counter, 
                                   UINT4 *pCount);
INT4 hyPhy20gOdukppOdukTcmiTxGetCounts(struct file *fileHndl, UINT4 slice,
                                       UINT4 accessMode, UINT4 counter,
                                       UINT4 *pCount);
INT4 hyPhy20gOdukppFramerRxGetCounts(struct file *fileHndl, UINT4 slice,
                                     UINT4 accessMode, UINT4 counter,
                                     UINT4 *pCount);
INT4 hyPhy20gOdukppOtukRxGetCounts(struct file *fileHndl, UINT4 slice,
                                   UINT4 accessMode, UINT4 counter, 
                                   UINT4 *pCount);
INT4 hyPhy20gOdukppOdukRxPmTcmiGetCounts(struct file *fileHndl, UINT4 slice,
                                         UINT4 accessMode, UINT4 counter,
                                         UINT4 *pCount);
INT4 hyPhy20gOdukppOpukRxGetCounts(struct file *fileHndl, UINT4 slice,
                                   UINT4 accessMode, UINT4 counter, 
                                   UINT4 *pCount);
INT4 hyPhy20gOdukppOtukAccumCounts(struct file *fileHndl);
INT4 hyPhy20gOdukppCountTrigCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                                UINT4 enbl);
INT4 hyPhy20gOdukppOtukFrmGetCounts(struct file *fileHndl, UINT4 slice, 
                                    UINT4 *pCount);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_ODUKPP_COUNTERS_H */         
/* end of file */ 
