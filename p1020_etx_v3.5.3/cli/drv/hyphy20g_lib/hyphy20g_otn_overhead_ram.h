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
**  FILE        :  hyphy20g_otn_overhead_ram.h
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :  Contains hardware specific definitions
**
**  NOTES       :  The user check the contents of this file and modify
**                 the contents as per their system's hardware characteristics
**
*******************************************************************************/

#ifndef _HYPHY20G_OTN_OVERHEAD_RAM_H
#define _HYPHY20G_OTN_OVERHEAD_RAM_H

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
INT4 hyPhy20gOtnOhRamCfg(struct file *fileHndl, UINT4 slice, UINT4 ohRam,
                         UINT4 maxRamSize, UINT4 cfgMsk0, UINT4 cfgMsk1);
INT4 hyPhy20gOtnOhRamApsPccCfg(struct file *fileHndl, UINT4 slice, UINT4 mfMsk);
INT4 hyPhy20gOtnOhRamInsEnable(struct file *fileHndl, UINT4 slice, 
                               UINT4 otukEnbl, UINT4 odukEnbl, UINT4 opukEnbl);
INT4 hyPhy20gOtnOhRamGetPointers(struct file *fileHndl, UINT4 *extRamPtr, 
                                 UINT4 *insRamPtr);
INT4 hyPhy20gOtnOhRamBurstRead(struct file *fileHndl, UINT4 slice, 
                               UINT4 addr, UINT4 *pData);
INT4 hyPhy20gOtnOhRamBurstWrite(struct file *fileHndl, UINT4 slice, 
                                UINT4 addr, UINT4 data[32]);
/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 hyPhy20gOtnOhRamDestroyCtxt(struct file *fileHndl);
UINT4 otnOhRamhyPhy20gOtnOhRamGetCtxtSize(struct file *fileHndl);
INT4 otnOhRamhyPhy20gOtnOhRamExtractCtxt(struct file *fileHndl, 
                                        void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 otnOhRamhyPhy20gOtnOhRamRestoreCtxt(struct file *fileHndl, 
                                        void *pCtxtBuf, UINT4 *pParsedSize);
INT4 otnOhRamHyPhy20gOtnOhRamAuditCtxt(struct file *fileHndl);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_OTN_OVERHEAD_RAM_H */         
/* end of file */
