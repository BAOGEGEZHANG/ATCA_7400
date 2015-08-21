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
**  FILE        :  hyphy20g_sonet.h
**
**  $Date: 2012-05-23 10:36:56 +0800 (Wed, 23 May 2012) $
**
**  $Revision: 23033 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_SONET_H
#define _HYPHY20G_SONET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_sonet_ln.h"



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

INT4 hyPhy20gSonetInit(struct file *fileHndl, UINT4 slice, UINT4 enable);

INT4 hyPhy20gSonetPldFilterModeCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 pcdCfg, UINT4 pldSel);
                                 
INT4 hyPhy20gSonetSts192FrmrCfg(struct file *fileHndl, UINT4 slice, 
                                UINT4 mode);
                                
INT4 hyPhy20gSonetPcdPaisDetCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 pAisDetEnbl, UINT4 aisFCnt, 
                                UINT4 normFCnt);                                

INT4 hyPhy20gSonetSts48FrmrCfg(struct file *fileHndl, UINT4 slice, 
                                UINT4 mode);

INT4 hyPhy20gSonetSts24FrmrCfg(struct file *fileHndl, UINT4 slice, 
                                UINT4 sts24Num, UINT4 mode);

INT4 hyPhy20gSonetSts12FrmrCfg(struct file *fileHndl, UINT4 slice, 
                                UINT4 sts12Num, UINT4 mode);

INT4 hyPhy20gSonetSts3FrmrCfg(struct file *fileHndl, UINT4 slice, 
                              UINT4 sts12Num, UINT4 sts3Num, UINT4 mode);

INT4 hyPhy20gSonetSts48CalFrmrTxClk(struct file *fileHndl, 
              UINT4 slice, UINT4 spareSfp, UINT4 otu1Sfp, UINT4 maxNumAlign);


#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_SONET_H */         
/* end of file */
