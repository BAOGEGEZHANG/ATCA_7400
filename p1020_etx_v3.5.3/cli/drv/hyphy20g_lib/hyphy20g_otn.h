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
**  FILE        :  hyphy20g_otn.h
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/
#ifndef _HYPHY20G_OTN_H
#define _HYPHY20G_OTN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_indirect.h"
#include "hyphy20g_hostmsg.h"

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* TEMPORARY DEFINITIONS                                                      */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/

INT4 hyPhy20gOtu2DeviceInit(struct file *fileHndl, UINT4 enable);

INT4 hyPhy20gOtu2Init(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hyPhy20gOdtu12Init(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hyPhy20gOtu1DeviceInit(struct file *fileHndl, UINT4 enable);

INT4 hyPhy20gOtu1Init(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hyPhy20gOtu2SysDataPathCfg(struct file *fileHndl, UINT4 slice,
						        UINT4 source);

INT4 hyPhy20gOtu1SysDataPathCfg(struct file *fileHndl, UINT4 slice,
						        UINT4 source);

INT4 hyPhy20gOtu1LineDataPathCfg(struct file *fileHndl, UINT4 slice,
			                     UINT4 source);

INT4 hyPhy20gOtu2SetLoopback(struct file *fileHndl, UINT4 slice, UINT4 loopType,
                             UINT4 enable);

INT4 hyPhy20gOtu1SetLoopback(struct file *fileHndl, UINT4 slice, UINT4 loopType,
                             UINT4 enable);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_OTN_H */
/* end of file */
