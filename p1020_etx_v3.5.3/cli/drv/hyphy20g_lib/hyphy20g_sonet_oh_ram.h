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
**  FILE        :  hyphy20g_sonet_oh_ram.h
**
**  $Date:  $
**
**  $Revision:  $
**
**  DESCRIPTION :  Contains hardware specific definitions
**
**  NOTES       :  The user check the contents of this file and modify
**                 the contents as per their system's hardware characteristics
**
*******************************************************************************/

#ifndef _HYPHY20G_SONET_OH_RAM_H
#define _HYPHY20G_SONET_OH_RAM_H

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
INT4 hyPhy20gSoaiInit(struct file *fileHndl, UINT4 enable);

INT4 hyPhy20gSoaiRcpInit(struct file *fileHndl, UINT4 dir, UINT4 enable);

INT4 hyPhy20gSoaiTohInit(struct file *fileHndl, UINT4 dir, UINT4 enable);

INT4 hyPhy20gSoaiRcpLinkEnable(struct file *fileHndl, UINT4 link, UINT4 dir,
                               UINT4 enable);
                                   
INT4 hyPhy20gSoaiRcpGetPointer(struct file *fileHndl, UINT4 link, UINT4 dir,
                               UINT4 *pPtrVal);
                               
INT4 hyPhy20gSoaiRcpGetMode(struct file *fileHndl, UINT4 link, UINT4 dir,
                            UINT4 *pMode);                               
                                
INT4 hyPhy20gSoaiTxRcpCfg(struct file *fileHndl, UINT4 link, UINT4 rdPtrIncr,
                          UINT4 rdWrapVal);

INT4 hyPhy20gSoaiRxRcpCfg(struct file *fileHndl, UINT4 link, UINT4 wrPtrIncr,
                          UINT4 wrWrapVal);
                                 
INT4 hyPhy20gSonetRcpRamBurstRead(struct file *fileHndl, UINT4 slice, 
                                  UINT4 addr, UINT4 *pData);                                 

INT4 hyPhy20gSonetRcpRamBurstWrite(struct file *fileHndl, UINT4 slice, 
                                   UINT4 addr, UINT4 data[32]);
                                                              
INT4 hyPhy20gSoaiTohLinkEnable(struct file *fileHndl, UINT4 link, UINT4 dir,
                               UINT4 enable);
                                
INT4 hyPhy20gSoaiTohGetPointer(struct file *fileHndl, UINT4 link, UINT4 dir,
                               UINT4 *pPtrVal);

INT4 hyPhy20gSoaiTohGetMode(struct file *fileHndl, UINT4 link, UINT4 dir,
                            UINT4 *pMode);
                            
INT4 hyPhy20gSoaiTxTohCfg(struct file *fileHndl, UINT4 link, UINT4 rdPtrIncr,
                         UINT4 rdWrapVal);

INT4 hyPhy20gSoaiRxTohCfg(struct file *fileHndl, UINT4 link, UINT4 wrPtrIncr,
                          UINT4 wrWrapVal);

INT4 hyPhy20gSoaiTohMaskCfg(struct file *fileHndl, UINT4 link, UINT4 dir, 
                            UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num,
                            UINT4 enMsk);
                                                             
INT4 hyPhy20gSonetTohRamBurstRead(struct file *fileHndl, UINT4 slice, 
                                  UINT4 addr, UINT4 *pData);
                                  
INT4 hyPhy20gSonetTohRamBurstWrite(struct file *fileHndl, UINT4 slice, 
                                   UINT4 addr, UINT4 data[32]);                                                              

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_SONET_OH_RAM_H */
/* end of file */
