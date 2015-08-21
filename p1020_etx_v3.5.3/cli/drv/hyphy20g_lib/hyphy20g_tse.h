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

#ifndef _HYPHY20G_TSE_H
#define _HYPHY20G_TSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include    "hyphy20g.h"
#include    "hyphy20g_indirect.h"


/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/

#define HYPHY20G_TSE_TX_J0DLY                                               1549 
#define HYPHY20G_TSE_RX_J0DLY                                               1549 

#define HYPHY20G_TSE_UNEQ_MASTER_TIMESLOT                                      2
#define HYPHY20G_TSE_DIAG_GENERATOR_PORT                                      40
#define HYPHY20G_TSE_PRGM_GENERATOR_PORT                                      41

#define HYPHY20G_TSE_MAX_NUM_OPORTS                                           40
#define HYPHY20G_TSE_MAX_NUM_TSLOTS                                           48
#define HYPHY20G_TSE_NULL_IPORT                                               63
#define HYPHY20G_TSE_NULL_ITS                                                 63


#define HYPHY20G_TSE_MAX_NUMBER_DPSF_BLOCKS                                   20
#define HYPHY20G_TSE_MAX_NUMBER_DPSF_PORTS                                    40
#define HYPHY20G_TSE_MAX_NUMBER_SONET_SLICE_BLOCKS                            16
#define HYPHY20G_TSE_MAX_NUMBER_ESSI_LINKS                                    32
#define HYPHY20G_TSE_MAX_NUMBER_PCTL_TIMESLOTS                                48

#define HYPHY20G_TSE_PCTL_STS12_TIMESLOTS                                     12
#define HYPHY20G_TSE_PCTL_STS3_TIMESLOTS                                       3


#define HYPHY20G_TSE_SWITCH_GROUP0                                             0
#define HYPHY20G_TSE_SWITCH_GROUP1                                             1
#define HYPHY20G_TSE_SWITCH_BOTH_GROUPS                                        2


/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/ 
INT4 hyPhy20gTseInit(struct file *fileHndl, UINT4 mode, UINT4 enable); 
 
INT4 hyPhy20gTseSts192PortMapCfg(struct file *fileHndl, UINT4 sts192Src, 
                                 UINT4 sts192Dest);
                                  
INT4 hyPhy20gTseSts48PortMapCfg(struct file *fileHndl, UINT4 sts48Src, 
                                UINT4 sts48Dest);
                                 
INT4 hyPhy20gTseSts24PortMapCfg(struct file *fileHndl, UINT4 sts48Src, 
                                UINT4 sts24Src, UINT4 sts48Dest, UINT4 sts24Dest);

INT4 hyPhy20gTseSts12PortMapCfg(struct file *fileHndl, UINT4 sts48Src, 
                                UINT4 sts12Src, UINT4 sts48Dest, UINT4 sts12Dest);
                                 
INT4 hyPhy20gTseSts3PortMapCfg(struct file *fileHndl, UINT4 sts48Src, 
                                UINT4 sts12Src, UINT4 sts3Src, 
                                UINT4 sts48Dest, UINT4 sts12Dest, UINT4 sts3Dest);
                                 
INT4 hyPhy20gTseSts1PortMapCfg(struct file *fileHndl, UINT4 sts48Src, 
                               UINT4 sts12Src, UINT4 sts3Src, UINT4 sts1Src,
                               UINT4 sts48Dest, UINT4 sts12Dest, UINT4 sts3Dest, 
                               UINT4 sts1Dest); 

INT4 hyPhy20gTsePageSwitch(struct file *fileHndl, UINT4 group); 

INT4 hyPhy20gTse2x50SetJ0Delays(struct file *fileHndl);

                                 
/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                 */
/******************************************************************************/ 

INT4 tseHyPhy20gTseDestroyCtxt(struct file *fileHndl);
UINT4 tseHyPhy20gTseGetCtxtSize(struct file *fileHndl);
INT4 tseHyPhy20gTseExtractCtxt(struct file *fileHndl, 
                            void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 tseHyPhy20gTseRestoreCtxt(struct file *fileHndl, 
                            void *pCtxtBuf, UINT4 *pParsedSize);
INT4 tseHyPhy20gTseAuditCtxt(struct file *fileHndl);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_TSE_H */
/* end of file */
