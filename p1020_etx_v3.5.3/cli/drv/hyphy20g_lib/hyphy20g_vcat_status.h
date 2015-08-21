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
**  FILE        :  hyphy20g_vcat_status.h
**
**  $Date: 2011-01-14 05:32:20 +0800 (Fri, 14 Jan 2011) $
**
**  $Revision: 10561 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_VCAT_STATUS_H
#define _HYPHY20G_VCAT_STATUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_indirect.h"


/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gVcatRxOtnFailStatus(struct file *fileHndl, UINT4 slice,
                                 UINT4 otuNum, UINT4 intClear,
                                 UINT4 *statusBits);

INT4 hyPhy20gVcatRxOtnVcatLinkStatus(struct file *fileHndl, UINT4 slice,
                                      UINT4 otu1Num, UINT4 *statusBits);

INT4 hyPhy20gVcatRxOtnVcatLinkIntStatus(struct file *fileHndl, UINT4 slice,
                                        UINT4 otu1Num, UINT4 intClear,
                                        UINT4 *statusBits);

INT4 hyPhy20gVcatTxSonetVcatGrpIntStatus(struct file *fileHndl, UINT4 grpId,
						                 UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gVcatTxSonetVcatMemberIntStatus(struct file *fileHndl,
									        UINT4 sts48Num, UINT4 sts12Num,
											UINT4 sts3Num, UINT4 sts1Num,
											UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gVcatRxSonetVcatMemberStatus(struct file *fileHndl,
										 UINT4 sts48Num, UINT4 sts12Num,
										 UINT4 sts3Num, UINT4 sts1Num,
										 UINT4 *statusBits);

INT4 hyPhy20gVcatRxSonetVcatMemberIntStatus(struct file *fileHndl,
											UINT4 sts48Num, UINT4 sts12Num,
											UINT4 sts3Num, UINT4 sts1Num,
											UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gVcatRxSonetVcatGrpIntStatus(struct file *fileHndl, UINT4 grpId,
										 UINT4 intClear, UINT4 *statusBits);

INT4 hyPhy20gVcatRxSonetVcatMemberCtrlPacketStatus(struct file *fileHndl,
                                            	   UINT4 sts48Num, 
                                            	   UINT4 sts12Num,
                                            	   UINT4 sts3Num, UINT4 sts1Num,
                                            	   UINT4 pktNum, UINT4 *mst,
                                            	   UINT4 *sq, UINT4 *mfi2,
                                            	   UINT4 *ctrl, 
                                            	   UINT4 *statusBits);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_VCAT_STATUS_H */
/* end of file */
