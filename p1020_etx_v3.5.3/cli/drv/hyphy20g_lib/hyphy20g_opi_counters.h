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
**  FILE        :  hyphy20g_opi_counters.h
**
**  $Date: 2010-08-12 07:31:27 +0800 (Thu, 12 Aug 2010) $
**
**  $Revision: 9021 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_OPI_COUNTERS_H
#define _HYPHY20G_OPI_COUNTERS_H

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
INT4 hyPhy20gCbriChnlCntCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                            UINT4 monChnl);

INT4 hyPhy20gCbriGetChnlCounts(struct file *fileHndl, UINT4 slice, UINT4 dir,
                               UINT4 monChnl, UINT4 *pByteCnt, UINT4 *pPktCnt,
                               UINT4 *pErrCnt, UINT4 *pBMaxCnt, UINT4 *pBMinCnt,
                               UINT4 *pByteStat, UINT4 *pPktStat,
                               UINT4 *pErrStat, UINT4 *pBMaxStat,
                               UINT4 *pBMinStat);

INT4 hyPhy20gCbriGetRxIntfCounts(struct file *fileHndl, UINT4 slice,
                                 UINT4 *pIbFcErrCnt, UINT4 *pCrc24Cnt,
                                 UINT4 *pIbFcStat, UINT4 *pCrc24Stat);

INT4 hyPhy20gCbriGetSerdesLaneCounts(struct file *fileHndl, UINT4 slice,
                                     UINT4 lane, UINT4 counter, UINT4 *pCntVal,
                                     UINT4 *pCntStat);

INT4 hyPhy20gCbriRxBurstMinCntCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 enable);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_OPI_COUNTERS_H */
/* end of file */
