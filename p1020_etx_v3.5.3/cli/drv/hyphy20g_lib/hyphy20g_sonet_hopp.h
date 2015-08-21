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
**  FILE        :  hyphy20g_sonet_hopp.h
**
**  $Date: 2011-11-17 17:04:59 +0800 (Thu, 17 Nov 2011) $
**
**  $Revision: 17155 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_SONET_HOPP_H
#define _HYPHY20G_SONET_HOPP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_indirect.h"


/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/
#define HYPHY20G_HOPP_MAX_SLICES                                               4

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/ 
INT4 hyPhy20gHoppDeviceInit(struct file *fileHndl, UINT4 enable);

INT4 hyPhy20gHoppSliceInit(struct file *fileHndl, UINT4 slice, UINT4 enable);

INT4 hyPhy20gHoppSts192LinkCfg(struct file *fileHndl);

INT4 hyPhy20gHoppSts48LinkCfg(struct file *fileHndl, UINT4 slice);

INT4 hyPhy20gHoppSts24LinkCfg(struct file *fileHndl, UINT4 slice,
							  UINT4 sts24Num);

INT4 hyPhy20gHoppSts12LinkCfg(struct file *fileHndl, UINT4 slice,
							  UINT4 sts12Num);

INT4 hyPhy20gHoppSts3LinkCfg(struct file *fileHndl, UINT4 slice, UINT4 sts12Num, 
							 UINT4 sts3Num, UINT4 concat);
							 
INT4 hyPhy20gHoppSts24LinkUnprov(struct file *fileHndl, UINT4 slice, 
                                 UINT4 sts24Num);
                                 
INT4 hyPhy20gHoppSts12LinkUnprov(struct file *fileHndl, UINT4 slice,
							     UINT4 sts12Num);   							 

INT4 hyPhy20gHoppSts48TxLinkCfg(struct file *fileHndl, UINT4 slice);
INT4 hyPhy20gHoppSts48RxLinkCfg(struct file *fileHndl, UINT4 slice);
INT4 hyPhy20gHoppSts24TxLinkCfg(struct file *fileHndl, UINT4 slice,
															UINT4 sts24Num);
INT4 hyPhy20gHoppSts24RxLinkCfg(struct file *fileHndl, UINT4 slice,
															UINT4 sts24Num);
INT4 hyPhy20gHoppSts12TxLinkCfg(struct file *fileHndl, UINT4 slice,
															UINT4 sts12Num);
															
INT4 hyPhy20gHoppSts12RxLinkCfg(struct file *fileHndl, UINT4 slice,
															UINT4 sts12Num);
INT4 hyPhy20gHoppSts3TxLinkCfg(struct file *fileHndl, UINT4 slice, UINT4 sts12Num,
                             UINT4 sts3Num, UINT4 concat);
INT4 hyPhy20gHoppSts3RxLinkCfg(struct file *fileHndl, UINT4 slice, UINT4 sts12Num,
                             UINT4 sts3Num, UINT4 concat);
INT4 hyPhy20gHoppSts24TxLinkUnprov(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts24Num);
INT4 hyPhy20gHoppSts24RxLinkUnprov(struct file *fileHndl, UINT4 slice,
                                 UINT4 sts24Num);
INT4 hyPhy20gHoppSts12TxLinkUnprov(struct file *fileHndl, UINT4 slice,
							     UINT4 sts12Num);
INT4 hyPhy20gHoppSts12RxLinkUnprov(struct file *fileHndl, UINT4 slice,
							     UINT4 sts12Num);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_SONET_HOPP_H */         
/* end of file */
