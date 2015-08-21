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
**  FILE        :  hyphy20g_pgmrclk.h
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :  This is the header file for hyPhy20g_pgmrclk.c file.
**
**  NOTES       :
**
*******************************************************************************/

#ifndef _HYPHY20G_PGMRCLK_H
#define _HYPHY20G_PGMRCLK_H

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
INT4 hyPhy20gPgmrclkInit(struct file *fileHndl, UINT4 enable);

INT4 hyPhy20gPgmrclkTrigTypeCfg(struct file *fileHndl, UINT4 counter, 
                                UINT4 mode);

INT4 hyPhy20gPgmrclkAutoTrigCfg(struct file *fileHndl, UINT4 interval,  
                                UINT4 pinCtrl, UINT4 pinWindow, 
                                UINT4 windowCtrl);

INT4 hyPhy20gPgmrclkAutoTrigEnbl(struct file *fileHndl, UINT4 enable);

INT4 hyPhy20gPgmrclkAutoTrigSwResync(struct file *fileHndl);

INT4 hyPhy20gPgmrclkManTrig(struct file *fileHndl, UINT4 msk);

INT4 hyPhy20gPgmrclkClkSrcCfg(struct file *fileHndl, UINT4 slice, UINT4 source, 
                              UINT4 divider);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_PGMRCLK_H */
/* end of file */
