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
**  FILE        :  hyphy20g_util_pmm.h
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


#ifndef _HYPHY20G_UTIL_PMM_H
#define _HYPHY20G_UTIL_PMM_H

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
/* TEMPORARY DEFINITIONS                                                      */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/    


INT4 hyPhy20gHsciPmmInit(struct file *fileHndl, UINT4 slice, UINT4 payloadType,
                         UINT4 timingMode);
						 
INT4 hyPhy20gHsciPmmDsmCfg(struct file *fileHndl, UINT4 slice, UINT4 dsmMode,
                          UINT4 dsmDenom, UINT4 predDenom, UINT4 predNum, 
                          UINT4 updateMsk);
						  
INT4 hyPhy20gHsciPmmMonEnable(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hyPhy20gHsciPmmMkrEnable(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hyPhy20gLsciPmmInit(struct file *fileHndl, UINT4 slice, UINT4 payloadType,
                         UINT4 timingMode);
                         
INT4 hyPhy20gLsciPmmDsmCfg(struct file *fileHndl, UINT4 slice, UINT4 dsmMode,
                          UINT4 dsmDenom, UINT4 predDenom, UINT4 predNum, 
                          UINT4 updateMsk);

INT4 hyPhy20gLsciPmmMonEnable(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hyPhy20gLsciPmmMkrEnable(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hyPhy20gOpiPmmInit(struct file *fileHndl, UINT4 slice, UINT4 payloadType,
                        UINT4 sourceType);                     

INT4 hyPhy20gOpiPmmDsmCfg(struct file *fileHndl, UINT4 slice, UINT4 dsmMode,
                          UINT4 dsmDenom, UINT4 predDenom, UINT4 predNum, 
                          UINT4 updateMsk);
                          
INT4 hyPhy20gOpiPmmTmStmpCfg(struct file *fileHndl, UINT4 slice, UINT4 tsGenEn,
                             UINT4 csiAlarmVal, UINT4 csiAlarmTh, 
                             UINT4 monSeqChkEn);
                             
INT4 hyPhy20gOpiPmmTmStmpTrig(struct file *fileHndl, UINT4 slice, UINT4 tsAddr);                                                  
                             
INT4 hyPhy20gOpiPmmForceCsi(struct file *fileHndl, UINT4 slice, UINT4 forceCsi);

INT4 hyPhy20gOpiPmmMonEnable(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hyPhy20gOpiPmmMkrEnable(struct file *fileHndl, UINT4 slice, UINT4 mode);  

INT4 hyPhy20gLsciPmmAnyRateInit(struct file *fileHndl, UINT4 slice, 
                                UINT4 rateKbits, UINT4 encoding, UINT4 busWidth,
                                UINT4 size, UINT4 period);    

INT4 hyPhy20gLsciPmmSet(struct file *fileHndl, UINT4 slice, 
                            UINT4 monitorPktSize, UINT4 makerPktSize, 
                            UINT4 makerPktPeriod);

INT4 hyPhy20gHsciPmmAnyRateInit(struct file *fileHndl, UINT4 slice, 
                                UINT4 rateKbits, UINT4 encoding, UINT4 size, 
                                UINT4 period);     

INT4 hyPhy20gHsciPmmSet(struct file *fileHndl, UINT4 slice, 
                            UINT4 monitorPktSize, UINT4 makerPktSize, 
                            UINT4 makerPktPeriod);

INT4 hyPhy20gOpiPmmOtu2DataSourceCfg(struct file *fileHndl, UINT4 slice, 
                                     UINT4 mode);                                                          

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_UTIL_PMM_H */
/* end of file */
