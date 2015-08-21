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
**  FILE        :  hyPhy20g_util_scbs.h
**
**  $Date: 2011-03-02 18:08:37 +0800 (Wed, 02 Mar 2011) $
**
**  $Revision: 11174 $
**
**  DESCRIPTION :  This is the header file for hyPhy20g_util.c file.
**
**  NOTES       :
**
*******************************************************************************/

#ifndef _HYPHY20G_UTIL_SCBS_H
#define _HYPHY20G_UTIL_SCBS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_cpb.h"
#include "hyphy20g_pif.h"
#include "hyphy20g_opi.h"
#include "hyphy20g_gfp.h"

/******************************************************************************/
/* COMMON MACRO DEFINITIONS                                                   */
/******************************************************************************/


/******************************************************************************/
/* FUNCTION PROTOTYPES                                                        */
/******************************************************************************/

INT4 utilHyPhy20gScbsInitSchd(struct file *fileHndl, sHYPHY20G_SCHD_CTXT *pSchd,
                              eHYPHY20G_SCBS_SCHD_ID schdId, UINT4 length,
                              UINT4 schdMode);

INT4 utilHyPhy20gScbsCreateSchd(sHYPHY20G_SCHD_CTXT *pSchd, UINT4 length,
                                UINT4 numGrp);

INT4 utilHyPhy20gScbsDestroySchd(sHYPHY20G_SCHD_CTXT *pSchd);

INT4 utilHyPhy20gScbsResetSchd(struct file *fileHndl,
                               sHYPHY20G_SCHD_CTXT *pSchd);

INT4 utilHyPhy20gScbsUpdateCal(struct file *fileHndl,
                               sHYPHY20G_SCHD_CTXT *pSchd, UINT4 *pCalData);

INT4 utilHyPhy20gScbsWriteCalOffLinePage(struct file *fileHndl,
                                         sHYPHY20G_SCHD_CTXT *pSchd,
                                         UINT4 *pCalData);

INT4 utilHyPhy20gScbsSwitchActiveCalPage(struct file *fileHndl,
                                         sHYPHY20G_SCHD_CTXT *pSchd);

INT4 utilHyPhy20gScbsWriteSchdCfg(struct file *fileHndl,
                                  sHYPHY20G_SCHD_CTXT *pSchd, UINT4 reset,
                                  UINT4 schdMode, UINT4 workPgSel);

INT4 utilHyPhy20gScbsReadSchdCfg(struct file *fileHndl,
                                 sHYPHY20G_SCHD_CTXT *pSchd, UINT4 *pReset,
                                 UINT4 *pSchdMode, UINT4 *pWorkPgSel);

INT4 utilHyPhy20gScbsWriteSchdCtxt(struct file *fileHndl,
                                   sHYPHY20G_SCHD_CTXT *pSchd, UINT4 scbsGrpNum,
                                   UINT4 startAddr, UINT4 endAddr);
INT4 utilHyPhy20gScbsReadSchdCtxt(struct file *fileHndl,
                                  sHYPHY20G_SCHD_CTXT *pSchd,
                                  UINT4 scbsGrpNum,
                                  UINT4 *pStartAddr,
                                  UINT4 *pEndAddr);

INT4 utilHyPhy20gScbsWriteSchdCal(struct file *fileHndl,
                                  sHYPHY20G_SCHD_CTXT *pSchd, UINT4 row,
                                  UINT4 entry, UINT4 value);

INT4 utilHyPhy20gScbsReadSchdCal(struct file *fileHndl,
                                 sHYPHY20G_SCHD_CTXT *pSchd, UINT4 row,
                                 UINT4 entry, UINT4 *ptr);

INT4 utilHyPhy20gScbsQuadWriteSchdCal(struct file *fileHndl,
                                      sHYPHY20G_SCHD_CTXT *pSchd, UINT4 row,
                                      UINT4 entry1, UINT4 entry2, UINT4 entry3,
                                      UINT4 entry4);

INT4 utilHyPhy20gScbsQuadReadSchdCal(struct file *fileHndl,
                                     sHYPHY20G_SCHD_CTXT *pSchd,
                                     UINT4 row, UINT4 *pEntry1, UINT4 *pEntry2,
                                     UINT4 *pEntry3, UINT4 *pEntry4);

INT4 utilHyPhy20gScbsUpdateCalRows(struct file *fileHndl,
                                   sHYPHY20G_SCHD_CTXT *pSchd,
                                   UINT4 numRows, UINT4 srcOffset,
                                   UINT4 destOffset, UINT4 *pCalData);

INT4 utilHyPhy20gScbsWriteCalOffLinePageRows(struct file *fileHndl,
                                             sHYPHY20G_SCHD_CTXT *pSchd,
                                             UINT4 numRows,
                                             UINT4 srcOffset,
                                             UINT4 destOffset,
                                             UINT4 *pCalData);
UINT4 utilHyPhy20gScbsGetSchdCtxtSize(sHYPHY20G_SCHD_CTXT *pSchdCtxt);
INT4 utilHyPhy20gScbsExtractSchdCtxt(sHYPHY20G_SCHD_CTXT *pSchd, 
                                    void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 utilHyPhy20gScbsRestoreSchdCtxt(sHYPHY20G_SCHD_CTXT *pSchd, 
                                    void *pCtxtBuf, UINT4 *pParsedSize);
INT4 utilHyPhy20gScbsAuditSchdCtxt(struct file *fileHndl, sHYPHY20G_SCHD_CTXT *pSchd);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_UTIL_SCBS_H */
/* end of file */
