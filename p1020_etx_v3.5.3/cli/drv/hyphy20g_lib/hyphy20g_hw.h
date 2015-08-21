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
**  FILE        :  hyphy20g_hw.h
**
**  $Date: 2011-04-18 17:07:26 +0800 (Mon, 18 Apr 2011) $
**
**  $Revision: 11721 $
**
**  DESCRIPTION :  Contains hardware specific definitions
**
**  NOTES       :  The user check the contents of this file and modify
**                 the contents as per their system's hardware characteristics
**
*******************************************************************************/

#ifndef _HYPHY20G_HW_H
#define _HYPHY20G_HW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_strs.h"
#if 0
#ifndef PMC_HW_CODE
#include "hyphy20g_hw_internal.h"
#include "gendrv.h"
#endif
#endif

/******************************************************************************/
/* COMMON MACRO DEFINITIONS                                                   */
/******************************************************************************/

#define mHYPHY20G_GET_FIELD4(word, fMsk, fOff) \
        (((word) & ((UINT4)(fMsk))) >> (fOff))

#define mHYPHY20G_FORM_FIELD4(fVal, fMsk, fOff) \
        ((((UINT4)(fVal))<< (fOff)) & ((UINT4)(fMsk)))

#define mHYPHY20G_SET_FIELD4(word, fVal, fMsk, fOff) \
        (((word) & (~(UINT4)(fMsk))) |     \
        ((((UINT4)(fVal)) << fOff)  & ((UINT4)(fMsk))))

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/
#define HYPHY20G_MUTEX_RXDP																										 0
#define HYPHY20G_MUTEX_HOPP_THPP																							 1
#define HYPHY20G_MUTEX_RVCP192																								 2

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gCheckRev(struct file *fileHndl, UINT4 *pDevId, UINT4 *pRevId,
                      UINT4 *pStatus);

INT4 hyPhy20gCheckRealRev(struct file *fileHndl, UINT4 *pDevId, UINT4 *pRevId,
                      UINT4 *pStatus);
                      
#ifndef PMC_HW_CODE
INT4 hwHyPhy20gTimerInit(struct file *fileHndl, UINT4 enable);

INT4 hwHyPhy20gStartTimer(struct file *fileHndl, UINT4 timerId);

INT4 hwHyPhy20gStopTimer(struct file *fileHndl, UINT4 timerId);

INT4 hwHyPhy20gGetTimer(struct file *fileHndl, UINT4 timerId, UINT4 *pStartSec,
                      UINT4 *pStartUs, UINT4 *pEndSec, UINT4 *pEndUs);

INT4 hwHyPhy20gGetVar(struct file *fileHndl, UINT4 varId, UINT4 *pVal);

INT4 hwHyPhy20gSetVar(struct file *fileHndl, UINT4 varId, UINT4 value);
#endif /* PMC_HW_CODE */

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 sysHyPhy20gPcieCfgRead (struct file *fileHndl, UINT4 offset, UINT4 *pvalue);
INT4 sysHyPhy20gPcieCfgWrite (struct file *fileHndl, UINT4 offset, UINT4 value);
INT4 sysHyPhy20gPcieBurstRead (struct file *fileHndl, UINT4 startAddr,
                               UINT4 length, UINT4 *dataList);
INT4 sysHyPhy20gPcieBurstWrite (struct file *fileHndl, UINT4 startAddr,
                                UINT4 length, UINT4 *dataList);
INT4 sysHyPhy20gFwDownload(struct file *fileHndl, STRING *fread, UINT4 jumpAddr);
INT4 sysHyPhy20gFwReload(struct file *fileHndl, STRING *fread);
struct file* sysHyPhy20gFileOpen(const STRING *path);
INT4 sysHyPhy20gFileClose(struct file *file);
INT4 sysHyPhy20gFileRead(struct file *f, STRING *buffer, UINT8 *offset,
                         UINT4 *rdLen);
INT4 sysHyPhy20gFileReadBytes(struct file *f, STRING *buffer, UINT4 bytes,
                              UINT8 *offset, UINT4 *rdLen);
INT4 sysHyPhy20gPollBit(struct file *fileHndl, UINT4 offset, UINT4  mask,
                         UINT4  pollFlg, UINT4 delay);
INT4 sysHyPhy20gRead (struct file *fileHndl, UINT4 offset, UINT4 *value);
INT4 sysHyPhy20gWrite (struct file *fileHndl, UINT4 offset, UINT4 value);
INT4 sysHyPhy20gBitWrite (struct file *fileHndl, UINT4 offset, UINT4 bit,
                        UINT4 value);
INT4 sysHyPhy20gBitRead (struct file *fileHndl, UINT4 offset, UINT4 bit,
                         UINT4 *value);
INT4 hyPhy20gRegFieldWrite (struct file *fileHndl, UINT4 reg, UINT4 fldMsk,
                            UINT4 fldValue);
INT4 hyPhy20gRegFieldRead (struct file *fileHndl, UINT4 reg, UINT4 fldMsk,
                           UINT4 *fldValue);
INT4 hyPhy20gVarFieldWrite (UINT4 *variable, UINT4 fldMsk, UINT4 fldValue);
INT4 hyPhy20gVarFieldRead (UINT4 variable, UINT4 fldMsk, UINT4 *fldValue);
INT4 hyPhy20gSetTraceWrite (struct file *fileHndl, UINT2 enbl);
INT4 sysHyPhy20gGetDevicePartType (struct file *fileHndl, UINT4 *pPartType);
INT4 sysHyPhy20gGetDeviceRevId (struct file *fileHndl, UINT4 *pRevId);
INT4 sysHyPhy20gMutexAquire (struct file *fileHndl, UINT4 mutex);
INT4 sysHyPhy20gMutexRelease (struct file *fileHndl, UINT4 mutex);
INT4 sysHyPhy20gMsDelay (UINT4 msDelay);
INT4 sysHyPhy20gUsDelay (UINT4 usDelay);
sHYPHY20G_CTXT* sysHyPhy20gGetDeviceCtxt(struct file *fileHndl);
sHYPHY20G_CTXT* sysHyPhy20gCreateCtxt(struct file *fileHndl);
sHYPHY20G_CTXT* sysHyPhy20gDestroyCtxt(struct file *fileHndl);
#ifndef PMC_HW_CODE
INT4 sysHyPhy20gTimerCreateCtxt(struct file *fileHndl);
INT4 sysHyPhy20gTimerDestroyCtxt(struct file *fileHndl);
UINT4 sysHyPhy20gTimerGetCtxtSize(struct file *fileHndl);
INT4 sysHyPhy20gTimerExtractCtxt(struct file *fileHndl, void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 sysHyPhy20gTimerRestoreCtxt(struct file *fileHndl, void *pCtxtBuf, UINT4 *pParsedSize);
INT4 rtosTimerHyPhy20gRtosTimerAuditCtxt(struct file *fileHndl);
#endif /* PMC_HW_CODE */

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_HW_H */
/* end of file */
