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
**  FILE        :  hyphy20g_dmi.h
**
**  $Date: 2011-01-19 03:29:47 +0800 (Wed, 19 Jan 2011) $
**
**  $Revision: 10582 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_DMI_H
#define _HYPHY20G_DMI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_indirect.h"



/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/
#define HYPHY20G_DMI_MAX_PORTS                                                 4 

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/

INT4 hyPhy20gDmiInit(struct file *fileHndl, UINT4 slice, UINT4 enable);

INT4 hyPhy20gDmiPortInit(struct file *fileHndl, UINT4 slice, UINT4 port, UINT4 mode);

INT4 hyPhy20gDmiSrcCfg(struct file *fileHndl, UINT4 slice, UINT4 port, 
                       UINT4 src);
                                              
INT4 hyPhy20gDmiPldFilterModeCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 port, UINT4 pcdCfg, UINT4 pldSel);
                                 
INT4 hyPhy20gDmiPcdPaisDetCfg(struct file *fileHndl, UINT4 slice, UINT4 port,
                              UINT4 pAisDetEnbl, UINT4 aisFCnt, UINT4 normFCnt);                                 

INT4 hyPhy20gDmiSts192TslotCfg(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hyPhy20gDmiSts48TslotCfg(struct file *fileHndl, UINT4 slice, UINT4 port,
                                    UINT4 mode);
							  
INT4 hyPhy20gDmiSts24TslotCfg(struct file *fileHndl, UINT4 slice, UINT4 port,
                                        UINT4 sts24Num, UINT4 mode);

INT4 hyPhy20gDmiSts12TslotCfg(struct file *fileHndl, UINT4 slice, UINT4 port,
                                        UINT4 sts12Num, UINT4 mode);

INT4 hyPhy20gDmiSts3TslotCfg(struct file *fileHndl, UINT4 slice, UINT4 port,
                             UINT4 sts12Num, UINT4 sts3Num, UINT4 mode);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_DMI_H */                 
/* end of file */
