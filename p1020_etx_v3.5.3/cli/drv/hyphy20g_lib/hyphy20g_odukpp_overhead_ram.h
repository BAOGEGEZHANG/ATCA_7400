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
**  FILE        :  hyphy20g_odukpp_overhead_ram.h
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

#ifndef _HYPHY20G_ODUKPP_OVERHEAD_RAM_H
#define _HYPHY20G_ODUKPP_OVERHEAD_RAM_H

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
INT4 hyPhy20gOdukppOhRamCfg(struct file *fileHndl, UINT4 slice, UINT4 ohRam,
                            UINT4 maxRamSize, UINT4 cfgMsk0, UINT4 cfgMsk1);
INT4 hyPhy20gOdukppOhRamApsPccCfg(struct file *fileHndl, UINT4 slice, 
                                  UINT4 mfMsk);
INT4 hyPhy20gOdukppOhRamInsEnable(struct file *fileHndl, UINT4 slice, 
                                  UINT4 otukEnbl, UINT4 odukEnbl, 
                                  UINT4 opukEnbl);
INT4 hyPhy20gOdukppOhRamGetPointers(struct file *fileHndl, UINT4 *extRamPtr, 
                                    UINT4 *insRamPtr);
INT4 hyPhy20gOdukppOhRamBurstRead(struct file *fileHndl, UINT4 slice, 
                                  UINT4 addr, UINT4 *pData);
INT4 hyPhy20gOdukppOhRamBurstWrite(struct file *fileHndl, UINT4 slice, 
                                   UINT4 addr, UINT4 data[32]);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 hyPhy20gOdukppOhRamDestroyCtxt(struct file *fileHndl);
UINT4 odukppOhRamhyPhy20gOdukppOhRamGetCtxtSize(struct file *fileHndl);
INT4 odukppOhRamhyPhy20gOdukppOhRamExtractCtxt(struct file *fileHndl, 
                                        void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 odukppOhRamhyPhy20gOdukppOhRamRestoreCtxt(struct file *fileHndl, 
                                        void *pCtxtBuf, UINT4 *pParsedSize);
INT4 odukppOhRamHyPhy20gOdukppOhRamAuditCtxt(struct file *fileHndl);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_ODUKPP_OVERHEAD_RAM_H */         
/* end of file */ 
