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
**  FILE        :  hyphy20g_lsci_counters.h
**
**  $Date: 2011-11-26 13:15:47 +0800 (Sat, 26 Nov 2011) $
**
**  $Revision: 17445 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_LSCI_COUNTERS_H
#define _HYPHY20G_LSCI_COUNTERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_hostmsg.h"
//#include "hyphy20g_util_ecom.h"
#include "hyphy20g_util_enet.h"
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

INT4 hyPhy20gLsciFeGeL1rppGetCounts(struct file *fileHndl, UINT4 slice,
                                    UINT4 *pCount, UINT4 *pOvFlStatus);

INT4 hyPhy20gLsciGeGfpTRxGetCounts(struct file *fileHndl, UINT4 slice,
                                   UINT4 accessMode, UINT4 counter, 
                                   UINT4 *pCount0, UINT4 *pCount1);

INT4 hyPhy20gLsciGeGfpTTxGetCounts(struct file *fileHndl, UINT4 slice,
                                   UINT4 accessMode, UINT4 counter, 
                                   UINT4 *pCount0, UINT4 *pCount1);

INT4 hyPhy20gLsciEthStatTrigCounts(struct file *fileHndl, UINT4 slice);

INT4 hyPhy20gLsciEthStatTrigRelease(struct file *fileHndl, UINT4 slice);

INT4 hyPhy20gLsciEthStatGetCounts(struct file *fileHndl, UINT4 slice, 
                                  UINT4 counter, UINT4 *pCount0,
                                  UINT4 *pCount1, UINT4 *pCount2);
                                  
INT4 hyPhy20gLsciEthStatVarCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 dir, UINT4 index);
                               
INT4 hyPhy20gLsciFcCbrTxGetCounts(struct file *fileHndl, UINT4 slice,
                                  UINT4 accessMode, UINT4 counter, 
                                  UINT4 *pCount);
                                  
INT4 hyPhy20gLsciFcCbrRxGetCounts(struct file *fileHndl, UINT4 slice,
                                  UINT4 accessMode, UINT4 counter, 
                                  UINT4 *pCount);                                                                  
                                  
INT4 hyPhy20gLsciEmpeiGetRxPmonCounts(struct file *fileHndl, UINT4 slice,
                                      UINT4 accessMode, UINT4 counter,
                                      UINT4 *pCount0, UINT4 *pCount1);                                  

INT4 hyPhy20gLsciEmpeiGetTxPmonCounts(struct file *fileHndl, UINT4 slice,
                                      UINT4 accessMode, UINT4 counter,
                                      UINT4 *pCount0, UINT4 *pCount1);

INT4 hyPhy20gLsciEmpeiGetInsQPmonCounts(struct file *fileHndl, UINT4 slice,
                                        UINT4 accessMode, UINT4 counter, 
                                        UINT4 *pCount);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_LSCI_COUNTERS_H */
/* end of file */

