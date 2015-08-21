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
**  MODULE      : HyPHY 20G ENET_COM Subsystem Header File
**
**  FILE        :  hyphy20g_util_ecom.h
**
**  $Date: 2010-11-19 02:46:43 +0800 (Fri, 19 Nov 2010) $
**
**  $Revision: 10102 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_UTIL_ECOM_H
#define _HYPHY20G_UTIL_ECOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_lsci.h"
#include "hyphy20g_hsci.h"

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
INT4 hyPhy20gEnetComDeviceInit(struct file *fileHndl, UINT4 enable);

INT4 hyPhy20gEmpeiFeGeDaSaSwap(struct file *fileHndl, UINT4 slice, 
                               UINT4 enSwap);
                               

INT4 hyPhy20gEmpei10GeDaSaSwap(struct file *fileHndl, UINT4 slice, 
                               UINT4 enSwap);

INT4 hyPhy20gEmpei10GePreambleEn(struct file *fileHndl, UINT4 slice, UINT4 dir,
                                 UINT4 enable, UINT4 hdrSize);

INT4 hyPhy20gEnetComGsup43DsmCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 dsmNum, UINT4 dsmDenom, UINT4 updateMsk);
                                 
INT4 hyPhy20gEnetComGsup43C73ModeCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 txPktType, UINT4 txOsType, 
                                     UINT4 rxPktType, UINT4 rxOsType);                                 

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 hyPhy20gEnetComHsciInit(struct file *fileHndl, UINT4 slice, UINT4 mode);
INT4 hyPhy20gEnetComLsciInit(struct file *fileHndl, UINT4 slice, UINT4 mode);
INT4 hyPhy20gEnetComGsup43Init(struct file *fileHndl, UINT4 slice, UINT4 mode);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_UTIL_ECOM_H */
/* end of file */
