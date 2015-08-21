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
**  FILE        :  hyphy20g_hsci_counters.h
**
**  $Date: 2011-04-28 14:20:12 +0800 (Thu, 28 Apr 2011) $
**
**  $Revision: 11852 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_HSCI_COUNTERS_H
#define _HYPHY20G_HSCI_COUNTERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_util_enet.h"
#include "hyphy20g_hostmsg.h"

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
INT4 hyPhy20gHsci10GeGetCounts(struct file *fileHndl, UINT4 slice,
                               UINT4 counter, UINT4 *pCount);

INT4 hyPhy20gHsciGsup43GetCounts(struct file *fileHndl, UINT4 slice,
                                 UINT4 *pReserve, UINT4 *pCount1, 
                                 UINT4 *pCount2, UINT4 *pCount3, 
                                 UINT4 *pCount4, UINT4 *pCount5, 
                                 UINT4 *pCount6, UINT4 *pCount7, 
                                 UINT4 *pCount8, UINT4 *pCount9, 
                                 UINT4 *pCount10, UINT4 *pCount11);

INT4 hyPhy20gHsciEnetComGsup43GetCounts(struct file *fileHndl, UINT4 slice,
                                        UINT4 accessMode, UINT4 counter, 
                                        UINT4 *pCount0, UINT4 *pCount1);
                                   
INT4 hyPhy20gHsciEthStatTrigCounts(struct file *fileHndl, UINT4 slice);

INT4 hyPhy20gHsciEthStatTrigRelease(struct file *fileHndl, UINT4 slice);

INT4 hyPhy20gHsciEthStatGetCounts(struct file *fileHndl, UINT4 slice, 
                                  UINT4 counter, UINT4 *pCount0,
                                  UINT4 *pCount1, UINT4 *pCount2);

INT4 hyPhy20gHsciEthStatVarCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 dir, UINT4 index);
                               
INT4 hyPhy20gHsciFcCbrTxGetCounts(struct file *fileHndl, UINT4 slice,
                                  UINT4 accessMode, UINT4 counter, 
                                  UINT4 *pCount);                               

INT4 hyPhy20gHsciFcCbrRxGetCounts(struct file *fileHndl, UINT4 slice,
                                  UINT4 accessMode, UINT4 counter,
                                  UINT4 *pCount);

INT4 hyPhy20gHsciEmpeiGetRxPmonCounts(struct file *fileHndl, UINT4 slice,
                                      UINT4 accessMode, UINT4 counter,
                                      UINT4 *pCount0, UINT4 *pCount1);

INT4 hyPhy20gHsciEmpeiGetTxPmonCounts(struct file *fileHndl, UINT4 slice,
                                      UINT4 accessMode, UINT4 counter,
                                      UINT4 *pCount0, UINT4 *pCount1);

INT4 hyPhy20gHsciEmpeiGetInsQPmonCounts(struct file *fileHndl, UINT4 slice,
                                        UINT4 accessMode, UINT4 counter,
                                        UINT4 *pCount);

INT4 hyPhy20gHsci10GeR64b66bGetCounts(struct file *fileHndl, UINT4 slice,
                                      UINT4 *pCount);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_HSCI_COUNTERS_H */
/* end of file */
