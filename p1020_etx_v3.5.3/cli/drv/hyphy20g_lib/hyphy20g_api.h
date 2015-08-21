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
**  FILE        :  hyphy20g_api.h
**
**  $Date: 2011-04-22 12:05:31 +0800 (Fri, 22 Apr 2011) $
**
**  $Revision: 11787 $
**
**  DESCRIPTION :  This file contains all the definitions & declarations
**                 that are required by external applications.
**
**  NOTES       :
**
*******************************************************************************/


#ifndef _HYPHY20G_API_H
#define _HYPHY20G_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_strs.h"
#include "hyphy20g_cpb.h"
#include "hyphy20g_pif.h"
#include "hyphy20g_opi.h"
#include "hyphy20g_gfp.h"
#include "hyphy20g_tse.h"
#include "hyphy20g_vcat.h"
#include "hyphy20g_lsci.h"
#include "hyphy20g_hsci.h"
#include "hyphy20g_serdes.h"
#include "hyphy20g_otn_overhead_ram.h"
#include "hyphy20g_odukpp_overhead_ram.h"

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
/******************************************************************************/
/* CONFIGURATION DUMP API FUNCTIONS                                           */
/******************************************************************************/
UINT4 hyPhy20gConfigDumpSizeGet(struct file *fileHndl, 
                                sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReqList);

INT4 hyPhy20gConfigDump(struct file *fileHndl, 
                        sHYPHY20G_CFG_DUMP_REQUEST_TYPE *pCfgDumpReqList, 
                        void *pCfgDumpBuf, UINT4 *pValidDumpBuffSize);


/******************************************************************************/
/* DEVICE MANAGEMENT API FUNCTIONS                                            */
/******************************************************************************/

INT4 hyPhy20gCreateCtxt(struct file *fileHndl);
INT4 hyPhy20gDestroyCtxt(struct file *fileHndl);

/******************************************************************************/
/* DEVICE REGISTER ACCESS                                                     */
/******************************************************************************/

INT4 hyPhy20gRead(struct file *fileHndl, UINT4 regNum, UINT4 *pvalue);
INT4 hyPhy20gWrite(struct file *fileHndl, UINT4 regNum, UINT4 value);
INT4 hyPhy20gPcieCfgRead(struct file *fileHndl, UINT4 regNum, UINT4 *pvalue);
INT4 hyPhy20gPcieCfgWrite(struct file *fileHndl, UINT4 regNum, UINT4 value);
INT4 hyPhy20gPcieBurstRead(struct file *fileHndl, UINT4 startAddr,
                           UINT4 length, UINT4 *dataList);
INT4 hyPhy20gPcieBurstWrite(struct file *fileHndl, UINT4 startAddr,
                            UINT4 length, UINT4 *dataList);
INT4 hyPhy20gFwDownload(struct file *fileHndl, STRING *fread, UINT4 jumpAddr);
INT4 hyPhy20gFwReload(struct file *fileHndl, STRING *fread);
INT4 hyPhy20gCheckRevEnable(struct file *fileHndl);
INT4 hyPhy20gCheckRevDisable(struct file *fileHndl, UINT4 revId);

/******************************************************************************/
/* STATUS AND COUNTS FUNCTIONS                                                */
/******************************************************************************/

/******************************************************************************/
/* DEVICE CONTECT EXTRACTION AND RESTORATION API FUNCTIONS                    */
/******************************************************************************/
UINT4 hyPhy20gGetCtxtSize(struct file *fileHndl);
INT4 hyPhy20gExtractCtxt(struct file *fileHndl, void *pCtxtBuf, 
                        UINT4 ctxtBufSize, UINT4 *pExtractedSize);
INT4 hyPhy20gRestoreCtxt(struct file *fileHndl, 
                        void *pCtxtBuf, UINT4 *pParsedSize);
INT4 hyPhy20gAuditCtxt(struct file *fileHndl);

/* NOT IMPLEMENTED */

/******************************************************************************/
/* DEVICE DIAGNOSTICS FUNCTIONS                                               */
/******************************************************************************/

/* NOT IMPLEMENTED */

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_API_H */
/* end of file */
