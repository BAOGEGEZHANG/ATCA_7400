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
**  FILE        :  hyphy20g_odukpp_ais.h
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

#ifndef _HYPHY20G_ODUKPP_AIS_H
#define _HYPHY20G_ODUKPP_AIS_H

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
INT4 hyPhy20gOdukppOtukTxInsAisCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 iaeEnbl, UINT4 fifoErrEnbl, 
                                   UINT4 losEnbl);
INT4 hyPhy20gOdukppOdukTxInsAisCfg(struct file *fileHndl, UINT4 slice, 
                                   UINT4 iaeEnbl, UINT4 fifoErrEnbl, 
                                   UINT4 losEnbl);
INT4 hyPhy20gOdukppOdukTxTcmiInsAisCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 tcm1Enbl, UINT4 tcm2Enbl,
                                       UINT4 tcm3Enbl, UINT4 tcm4Enbl,
                                       UINT4 tcm5Enbl, UINT4 tcm6Enbl);
INT4 hyPhy20gOdukppOdukRxInsAisCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 lomEnbl, UINT4 ptMmEnbl, 
                                   UINT4 smTimEnbl, UINT4 pmTimOciLckEnbl, 
                                   UINT4 tcm1TimOciLckEnbl, 
                                   UINT4 tcm2TimOciLckEnbl, 
                                   UINT4 tcm3TimOciLckEnbl, 
                                   UINT4 tcm4TimOciLckEnbl, 
                                   UINT4 tcm5TimOciLckEnbl,
                                   UINT4 tcm6TimOciLckEnbl);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_ODUKPP_AIS_H */         
/* end of file */ 
