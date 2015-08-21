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
**  FILE        :  hyphy20g_odukpp.h
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

#ifndef _HYPHY20G_ODUKPP_H
#define _HYPHY20G_ODUKPP_H

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
INT4 hyPhy20gOdukppDeviceInit(struct file *fileHndl, UINT4 enable);
INT4 hyPhy20gOdukppOdu1DataPathCfg(struct file *fileHndl, UINT4 slice, 
                                   UINT4 mode, UINT4 lineIf, UINT4 sysIf);
INT4 hyPhy20gOdukppOdu2DataPathCfg(struct file *fileHndl, UINT4 slice, 
                                   UINT4 mode, UINT4 sysIf, UINT4 odukType);
INT4 hyPhy20gOdukppOdu2SetLoopback(struct file *fileHndl, UINT4 slice, 
                                   UINT4 loopType, UINT4 enable);
INT4 hyPhy20gOdukppOdu1SetLoopback(struct file *fileHndl, UINT4 slice, 
                                   UINT4 loopType, UINT4 enable);
INT4 hyPhy20gOdukppCbrbFifoMonCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 zoneChkEnbl);
INT4 hyPhy20gOdukppCbrbFifoZoneChkPrdCfg(struct file *fileHndl, UINT4 frmType,
                                         UINT4 zoneChkPeriod);
INT4 hyPhy20gOdukppOdu1DataAlign(struct file *fileHndl, UINT4 slice, 
                                 UINT4 alignMsk, UINT4 odukPpSlc0, 
                                 UINT4 odukPpSlc1, UINT4 odukPpSlc2,
                                 UINT4 odukPpSlc3);
INT4 hyPhy20gOdukppPacerModeCfg(struct file *fileHndl, UINT4 slice, 
                                UINT4 sourceType);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_ODUKPP_H */         
/* end of file */ 
