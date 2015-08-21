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
**  FILE        :  hyphy20g_loopbacks.h
**
**  $Date: 2010-10-23 01:00:36 +0800 (Sat, 23 Oct 2010) $
**
**  $Revision: 9876 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_LOOPBACKS_H
#define _HYPHY20G_LOOPBACKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_indirect.h"

/******************************************************************************/
/* TEMPORARY DEFINES                                                          */
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
INT4 hyPhy20gEssiLnLpbk(struct file *fileHndl, UINT4 slice, 
                        UINT4 link, UINT4 lpbkEnbl);

INT4 hyPhy20gGfpDiagLpbk(struct file *fileHndl, UINT4 slice, UINT4 lpbkEnbl);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_LOOPBACKS_H */
/* end of file */
