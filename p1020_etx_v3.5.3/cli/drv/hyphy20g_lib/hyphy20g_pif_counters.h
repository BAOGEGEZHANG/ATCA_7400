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
**  FILE        :  hyphy20g_pif_counters.h
**
**  $Date: 2012-06-29 14:03:11 +0800 (Fri, 29 Jun 2012) $
**
**  $Revision: 23816 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_PIF_COUNTERS_H
#define _HYPHY20G_PIF_COUNTERS_H

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
INT4 hyPhy20gPifXauiGetCounts(struct file *fileHndl, UINT4 slice,
                              UINT4 accessMode, UINT4 counter, UINT4 *pCount);

INT4 hyPhy20gPifXauiGetBip8ErrCount(struct file *fileHndl, UINT4 slice,
                                    UINT4 accessMode, UINT4 *pCount);

INT4 hyPhy20gPifIntlknChnlCntCfg(struct file *fileHndl, UINT4 dir,
                                 UINT4 monChnl);

INT4 hyPhy20gPifIntlknGetChnlCountsEx(struct file *fileHndl, UINT4 dir,  
                                    UINT4 monChnl, UINT4 pByteCnt[2],  
                                    UINT4 pPktCnt[2], UINT4 *pErrCnt, 
                                    UINT4 *pBMaxCnt, UINT4 *pBMinCnt,  
                                    UINT4 *pByteStat, UINT4 *pPktStat, 
                                    UINT4 *pErrStat, UINT4 *pBMaxStat, 
                                    UINT4 *pBMinStat);

INT4 hyPhy20gPifIntlknGetChnlCounts(struct file *fileHndl, UINT4 dir,
                                    UINT4 monChnl, UINT4 *pByteCnt,
                                    UINT4 *pPktCnt, UINT4 *pErrCnt,
                                    UINT4 *pBMaxCnt, UINT4 *pBMinCnt,
                                    UINT4 *pByteStat, UINT4 *pPktStat,
                                    UINT4 *pErrStat, UINT4 *pBMaxStat,
                                    UINT4 *pBMinStat);

INT4 hyPhy20gPifIntlknGetRxIntfCounts(struct file *fileHndl, UINT4 *pIbFcErrCnt,
                                      UINT4 *pCrc24Cnt, UINT4 *pIbFcStat,
                                      UINT4 *pCrc24Stat);

INT4 hyPhy20gPifIntlknGetSerdesLaneCounts(struct file *fileHndl, UINT4 lane,
                                          UINT4 counter, UINT4 *pCntVal,
                                          UINT4 *pCntStat);

INT4 hyPhy20gPifIntlknRxBurstMinCntCfg(struct file *fileHndl, UINT4 enable);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_PIF_COUNTERS_H */
/* end of file */
