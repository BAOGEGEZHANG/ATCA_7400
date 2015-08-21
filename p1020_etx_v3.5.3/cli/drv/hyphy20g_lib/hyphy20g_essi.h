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
**  FILE        :  hyphy20g_essi.h
**
**  $Date: 2010-12-03 09:12:23 +0800 (Fri, 03 Dec 2010) $
**
**  $Revision: 10229 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_ESSI_H
#define _HYPHY20G_ESSI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_indirect.h"


/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/
#define HYPHY20G_ESSI_RECOMMENDED_DC_BAL_THRESH                             1000
#define HYPHY20G_ESSI_RECOMMENDED_DECAY_PERIOD                               430
#define HYPHY20G_ESSI_RECOMMENDED_CID_THRESH                                  10
#define HYPHY20G_ESSI_RECOMMENDED_TRAN_THRESH                                	 4
/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gEssiSliceInit(struct file *fileHndl, UINT4 slice, UINT4 enable);

INT4 hyPhy20gEssiLinkInit(struct file *fileHndl, UINT4 slice, UINT4 link,
                                            UINT4 enable);

INT4 hyPhy20gEssiFifoCtr(struct file *fileHndl, UINT4 slice, UINT4 link);

INT4 hyPhy20gEssiScrmblrCfg(struct file *fileHndl, UINT4 slice, UINT4 link,
							 UINT4 dir, UINT4 poly, UINT4 mode);

INT4 hyPhy20gEssiB1FrmCfg(struct file *fileHndl, UINT4 slice, UINT4 link,
							                                    UINT4 b1Mode);

INT4 hyPhy20gEssiDcBalMonCfg(struct file *fileHndl, UINT4 slice, UINT4 link,
                                  UINT4 thresh, UINT4 decayPer, UINT4 decayFac,
                                  UINT4 enable);

INT4 hyPhy20gEssiCidMonCfg(struct file *fileHndl, UINT4 slice, UINT4 link,
                                  UINT4 thresh, UINT4 enable);

INT4 hyPhy20gEssiTransitionDetCfg(struct file *fileHndl, UINT4 slice, UINT4 link,
                                  UINT4 thresh, UINT4 enable);

INT4 hyPhy20gEssiSlicingModeCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
				             UINT4 mode);

INT4 hyPhy20gEssiReAlign(struct file *fileHndl, UINT4 slice, UINT4 link);

INT4 hyPhy20gEssiSetSts192EssiJ0Delays(struct file *fileHndl, UINT4 slice, 
                                       UINT4 workProtSel);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_ESSI_H */
/* end of file */
