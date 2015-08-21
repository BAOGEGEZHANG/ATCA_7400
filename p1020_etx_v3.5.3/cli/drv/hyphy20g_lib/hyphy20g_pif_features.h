/******************************************************************************/
/**  COPYRIGHT (C); 2008 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
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
**  FILE        :  hyphy20g_pif_features.h
**
**  $Date: 2010-10-28 14:19:48 +0800 (Thu, 28 Oct 2010) $
**
**  $Revision: 9964 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_PIF_FEATURES_H
#define _HYPHY20G_PIF_FEATURES_H

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
INT4 hyPhy20gPifIntlknSerdesTestCfg(struct file *fileHndl, UINT4 dir,
                                    UINT4 mode, UINT4 prbs, UINT4 patternA,
                                    UINT4 patternB, UINT4 patternC, UINT4 repA,
                                    UINT4 repB, UINT4 repC);

INT4 hyPhy20gPifIntlknCoreDiagLpbk(struct file *fileHndl, UINT4 enable);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_PIF_FEATURES_H */
/* end of file */
