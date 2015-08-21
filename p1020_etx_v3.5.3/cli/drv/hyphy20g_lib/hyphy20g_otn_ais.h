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
**  FILE        :  hyphy20g_otn_ais.h
**
**  $Date: 2011-01-19 03:29:47 +0800 (Wed, 19 Jan 2011) $
**
**  $Revision: 10582 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_OTN_AIS_H
#define _HYPHY20G_OTN_AIS_H

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
INT4 hyPhy20gOtukTxFailEnblCfg(struct file *fileHndl, UINT4 slice, UINT4 mode);
 
INT4 hyPhy20gOtukTxInsAisCfg(struct file *fileHndl, UINT4 slice, UINT4 iaeEnbl,
                             UINT4 fifoErrEnbl, UINT4 losEnbl);

INT4 hyPhy20gOdukTxInsAisCfg(struct file *fileHndl, UINT4 slice, UINT4 iaeEnbl,
                             UINT4 fifoErrEnbl, UINT4 losEnbl);

INT4 hyPhy20gOdukTxTcmiInsAisCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 tcm1Enbl, UINT4 tcm2Enbl,
                                 UINT4 tcm3Enbl, UINT4 tcm4Enbl,
                                 UINT4 tcm5Enbl, UINT4 tcm6Enbl);

INT4 hyPhy20gOdukRxAisCfg(struct file *fileHndl, UINT4 slice, 
                          UINT4 trafficType);

INT4 hyPhy20gOdukRxInsAisCfg(struct file *fileHndl, UINT4 slice,
                             UINT4 lomEnbl, UINT4 ptMmEnbl, UINT4 smTimEnbl,
                             UINT4 pmTimOciLckEnbl, UINT4 tcm1TimOciLckEnbl,
                             UINT4 tcm2TimOciLckEnbl, UINT4 tcm3TimOciLckEnbl,
                             UINT4 tcm4TimOciLckEnbl, UINT4 tcm5TimOciLckEnbl,
                             UINT4 tcm6TimOciLckEnbl);


/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                 */
/******************************************************************************/  

#ifdef __cplusplus
}
#endif

#endif /* _OTN_AIS_H */
/* end of file */

