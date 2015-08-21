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
**  FILE        :  hyphy20g_otn_counters.h
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

#ifndef _HYPHY20G_OTN_COUNTERS_H
#define _HYPHY20G_OTN_COUNTERS_H

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

INT4 hyPhy20gOpukTxGetCounts(struct file *fileHndl, UINT4 slice,
                             UINT4 accessMode, UINT4 counter, UINT4 *pCount);

INT4 hyPhy20gOdukTcmiTxGetCounts(struct file *fileHndl, UINT4 slice,
                                 UINT4 accessMode, UINT4 counter,
                                 UINT4 *pCount);

INT4 hyPhy20gOtnFramerRxGetCounts(struct file *fileHndl, UINT4 slice,
                                  UINT4 accessMode, UINT4 counter,
                                  UINT4 *pCount);

INT4 hyPhy20gOtukRxGetCounts(struct file *fileHndl, UINT4 slice,
                             UINT4 accessMode, UINT4 counter, UINT4 *pCount);

INT4 hyPhy20gOdukRxPmTcmiGetCounts(struct file *fileHndl, UINT4 slice,
                                   UINT4 accessMode, UINT4 counter,
                                   UINT4 *pCount);

INT4 hyPhy20gOpukRxGetCounts(struct file *fileHndl, UINT4 slice,
                             UINT4 accessMode, UINT4 counter, UINT4 *pCount);

INT4 hyPhy20gOdtu12TxGetCounts(struct file *fileHndl, UINT4 slice,
                               UINT4 accessMode, UINT4 counter, UINT4 *pCount0,
                               UINT4 *pCount1, UINT4 *pCount2, UINT4 *pCount3);

INT4 hyPhy20gOdtu12RxGetCounts(struct file *fileHndl, UINT4 slice,
                               UINT4 accessMode, UINT4 counter, UINT4 *pCount0,
                               UINT4 *pCount1, UINT4 *pCount2, UINT4 *pCount3);

INT4 hyPhy20gOtuI7EfecGetCounts(struct file *fileHndl, UINT4 slice,
                                UINT4 *pCount0, UINT4 *pCount1, UINT4 *pCount2,
                                UINT4 *pCount3, UINT4 *pCount4);

INT4 hyPhy20gOtuI4EfecGetCounts(struct file *fileHndl, UINT4 slice,
                                UINT4 *pCount0, UINT4 *pCount1, UINT4 *pCount2,
                                UINT4 *pCount3, UINT4 *pCount4, UINT4 *pCount5);

INT4 hyPhy20gOtuEfecAccumCounts(struct file *fileHndl, UINT4 slice);

INT4 hyPhy20gOtuEfecTxSetBitError(struct file *fileHndl, UINT4 slice,
                                  UINT4 fecType);

INT4 hyPhy20gOtukAccumCounts(struct file *fileHndl, UINT4 subsystem);

INT4 hyPhy20gOtnCountTrigCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                             UINT4 enbl);

INT4 hyPhy20gOtukFrmGetCounts(struct file *fileHndl, UINT4 slice, UINT4 *pCount);



/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_OTN_COUNTERS_H */
/* end of file */
