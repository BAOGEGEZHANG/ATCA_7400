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
**  FILE        :  hyphy20g_util_acb.h
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :  This file contains all the definitions & declarations
**                 that are required by external applications.
**
**  NOTES       :
**
*******************************************************************************/


#ifndef _HYPHY20G_UTIL_ACB_H
#define _HYPHY20G_UTIL_ACB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_util_scbs.h"

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

INT4 hyPhy20gSfpAcbInit(struct file *fileHndl, UINT4 slice, UINT4 enable);

INT4 hyPhy20gSfpAcbModeSel(struct file *fileHndl, UINT4 slice, UINT4 mode,
                           UINT4 lineFrm, UINT4 bRate);

INT4 hyPhy20gSfpAcbCfgGain(struct file *fileHndl, UINT4 slice, UINT4 acbGain,
                           UINT4 lpGain,UINT4 cfg0, UINT4 cfg1, UINT4 cfg2,
                           UINT4 cfg3, UINT4 updateMsk);

INT4 hyPhy20gXfiAcbInit(struct file *fileHndl, UINT4 slice, UINT4 enable);

INT4 hyPhy20gXfiAcbModeSel(struct file *fileHndl, UINT4 slice, UINT4 mode,
                           UINT4 lineFrm);

INT4 hyPhy20gXfiAcbCfgGain(struct file *fileHndl, UINT4 slice, UINT4 acbGain,
                           UINT4 lpGain,UINT4 cfg0, UINT4 cfg1, UINT4 cfg2,
                           UINT4 cfg3, UINT4 updateMsk);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/

INT4 utilHyPhy20gXfiAcbGetPmmPktData(struct file *fileHndl,
                                     UINT4 pmmAddr, UINT4 *pMonPktSz,
                                     UINT4 *pMkrPktSz, UINT4 *pMkrPktSzVar,
                                     UINT4 *pMkrPktPeriod);

INT4 utilHyPhy20gSfpAcbGetPmmPktData(struct file *fileHndl, UINT4 slice,
                                     UINT4 pmmAddr, UINT4 *pMonPktSz,
                                     UINT4 *pMkrPktSz, UINT4 *pMkrPktSzVar,
                                     UINT4 *pMkrPktPeriod);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_UTIL_ACB_H */
/* end of file */
