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
**  FILE        :  hyphy20g_hopp_alarms.h
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

#ifndef _HYPHY20G_HOPP_ALARMS_H
#define _HYPHY20G_HOPP_ALARMS_H

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
INT4 hyPhy20gHoppPohRxCfgInsAisP(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts12, UINT4 sts3, UINT4 sts1, 
                                 UINT4 sdEnbl, UINT4 sfEnbl,
                                 UINT4 pTimEnbl, UINT4 pTiuEnbl,
                                 UINT4 pErdiEnbl, UINT4 pRdiEnbl,
                                 UINT4 pPdiEnbl, UINT4 pUneqEnbl,
                                 UINT4 pPlmEnbl, UINT4 pPluEnbl,
                                 UINT4 pAisPtrEnbl, UINT4 pLopEnbl);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_HOPP_ALARMS_H */         
/* end of file */
