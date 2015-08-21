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
**  FILE        :  hyphy20g_vcat_features.h
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

#ifndef _HYPHY20G_VCAT_FEATURES_H
#define _HYPHY20G_VCAT_FEATURES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_indirect.h"


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

INT4 hyPhy20gVcatPrbsTxConfig(struct file *fileHndl, UINT4 port, 
                              UINT4 grpId, UINT4 patType,
                              UINT4 enable);
                              
INT4 hyPhy20gVcatPrbsRxConfig(struct file *fileHndl, UINT4 port, 
                              UINT4 grpId, UINT4 patType,
                              UINT4 enable);

INT4 hyPhy20gVcatPrbsTxErrIns(struct file *fileHndl, UINT4 errNum);

INT4 hyPhy20gVcatPrbsRxStatus(struct file *fileHndl, UINT4 intClear, 
															UINT4 *statusBits, UINT4 *allZeroSync,
															UINT4 *errorCount);      
															
INT4 hyPhy20gVcatDdrBistTest(struct file *fileHndl, UINT4 startAddr, UINT4 addrSpace);
                                                     
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_VCAT_FEATURES_H */
/* end of file */
