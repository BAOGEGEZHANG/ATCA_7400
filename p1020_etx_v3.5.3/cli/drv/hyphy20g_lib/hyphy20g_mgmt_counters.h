/******************************************************************************/
/**  COPYRIGHT (C);; 2008 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
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
**  FILE        :  hyphy20g_mgmt_counters.h
**
**  $Date: 2010-07-26 16:43:40 +0800 (星期一, 26 七月 2010); $
**
**  $Revision: 8861 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_MGMT_COUNTERS_H
#define _HYPHY20G_MGMT_COUNTERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_hostmsg.h"
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
INT4 hyPhy20gMgmtPortFeGeL1rppGetCounts(struct file *fileHndl, UINT4 accessMode, 
                                        UINT4 *pCount0, UINT4 *pCount1, 
                                        UINT4 *pCount2);

INT4 hyPhy20gMgmtPortEthStatGetCounts(struct file *fileHndl, UINT4 counter, 
                                      UINT4 accessMd, UINT4 *pCount0,
                                      UINT4 *pCount1, UINT4 *pCount2);

INT4 hyPhy20gMgmtPortEthStatVarCfg(struct file *fileHndl, UINT4 dir, 
                                   UINT4 index);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_MGMT_COUNTERS_H */
/* end of file */

