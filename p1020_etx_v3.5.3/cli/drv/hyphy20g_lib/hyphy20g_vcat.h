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

#ifndef _HYPHY20G_VCAT_H
#define _HYPHY20G_VCAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_util_scbs.h"
#include "hyphy20g_indirect.h"

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/
#define HYPHY20G_VCAT_SLICE_MODE_TS_MODE                                       0
#define HYPHY20G_VCAT_SLICE_MODE_ODU1_MODE                                     1
#define HYPHY20G_VCAT_SLICE_MODE_VCAT_MODE                                     2
#define HYPHY20G_VCAT_SLICE_MODE_UNPROV_MODE                                   0
#define HYPHY20G_VCAT_NUM_TRIBSLOTS                                           16
#define HYPHY20G_VCAT_NUM_GID                                                 64
#define HYPHY20G_VCAT_NUM_STS1_MEMBERS                                       192
#define HYPHY20G_VCAT_NUM_STS3c_MEMBERS                                       64
#define HYPHY20G_VCAT_NULL_STSNUM                                           0xFF

/******************************************************************************/
/* TEMPORARY DEFINITIONS                                                       */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/

INT4 hyPhy20gVcatLcasAInit(struct file *fileHndl, UINT4 mode, UINT4 enable);

INT4 hyPhy20gVcatLcasBInit(struct file *fileHndl, UINT4 enable);
       
INT4 hyPhy20gVcatLcasOtnModeCfg(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hyPhy20gVcatLcasOtnOtu2TransGrpCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 grpId, UINT4 mapping, 
                                        UINT4 master);

INT4 hyPhy20gVcatLcasOtnOtu2GfpGrpCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 grpId, UINT4 lan10Ge);
                                 

INT4 hyPhy20gVcatLcasOtnOtu1LinkMode(struct file *fileHndl, UINT4 slice,
                                     UINT4 otu1Num, UINT4 master, UINT4 mode);

INT4 hyPhy20gVcatLcasOtnOtu1TsModeGrpCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 otu1Num, UINT4 grpId, 
                                         UINT4 grpSize, UINT4 startSlot, 
                                         UINT4 dataSrc, UINT4 tsEnable,
                                         UINT4 lnkEnable);           

INT4 hyPhy20gVcatLcasOtnOtu1Odu1ModeGrpCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 otu1Num, UINT4 grpId, 
                                           UINT4 dataSrc, UINT4 enable); 
                                          
INT4 hyPhy20gVcatLcasOtnOtu1VcatModeGrpCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 otu1Num, UINT4 vcatBuf, 
                                           UINT4 grpId, UINT4 seqNo, 
                                           UINT4 enable);
INT4 hyPhy20gVcatLcasOtnOtu1VcatSetDefault(struct file *fileHndl, 
                                           UINT4 slice, UINT4 otu1Num);                                                  
INT4 hyPhy20gVcatLcasSonetTopCfg(struct file *fileHndl, UINT4 ovrHead,
                                 UINT4 h1ss, UINT4 ddrEn);

INT4 hyPhy20gVcatLcasSonetLinkSts192Cfg(struct file *fileHndl, UINT4 grpId);                 

INT4 hyPhy20gVcatLcasSonetLinkSts48Cfg(struct file *fileHndl, UINT4 grpId,
                                       UINT4 sts48Num);                                  

INT4 hyPhy20gVcatLcasSonetLinkSts24Cfg(struct file *fileHndl, UINT4 grpId,
                                       UINT4 sts48Num, UINT4 sts24Num);                       

INT4 hyPhy20gVcatLcasSonetLinkSts12Cfg(struct file *fileHndl, UINT4 grpId,
                                       UINT4 sts48Num, UINT4 sts12Num);                       

INT4 hyPhy20gVcatLcasSonetLinkSts3Sts1Cfg(struct file *fileHndl, UINT4 sts48Num,
                                          UINT4 sts12Num, UINT4 sts3Num,
                                          UINT4 concat);                    


INT4 hyPhy20gVcatLcasSonetLinkSts3cTxMemberProv(struct file *fileHndl, 
                                                UINT4 sts48Num, UINT4 sts12Num, 
                                                UINT4 sts3Num, UINT4 grpId, 
                                                UINT4 mode);                                     

INT4 hyPhy20gVcatLcasSonetLinkSts3cRxMemberProv(struct file *fileHndl, 
                                              UINT4 sts48Num, UINT4 sts12Num, 
                                              UINT4 sts3Num, UINT4 grpId, 
                                              UINT4 mode);                                     

INT4 hyPhy20gVcatLcasSonetLinkSts3cTxMemberUnProv(struct file *fileHndl, 
                                                  UINT4 sts48Num, 
                                                  UINT4 sts12Num,
                                                  UINT4 sts3Num);                             

INT4 hyPhy20gVcatLcasSonetLinkSts3cRxMemberUnProv(struct file *fileHndl, 
                                                  UINT4 sts48Num, 
                                                  UINT4 sts12Num, 
                                                  UINT4 sts3Num);                             

INT4 hyPhy20gVcatLcasSonetSts3cLcasMemberAddRemove (struct file *fileHndl, 
                                                    UINT4 sts48Num, 
                                                    UINT4 sts12Num, 
                                                    UINT4 sts3Num, 
                                                    UINT4 command); 

INT4 hyPhy20gVcatLcasSonetLinkSts1RxMemberProv(struct file *fileHndl, 
                                               UINT4 sts48Num, UINT4 sts12Num, 
                                               UINT4 sts3Num, UINT4 sts1Num, 
                                               UINT4 grpId,  UINT4 mode);                             

INT4 hyPhy20gVcatLcasSonetLinkSts1RxMemberUnProv(struct file *fileHndl, 
                                                 UINT4 sts48Num, UINT4 sts12Num,
                                                 UINT4 sts3Num, UINT4 sts1Num);                             

INT4 hyPhy20gVcatLcasSonetLinkSts1TxMemberProv(struct file *fileHndl, 
                                               UINT4 sts48Num, UINT4 sts12Num, 
                                               UINT4 sts3Num, UINT4 sts1Num, 
                                               UINT4 grpId, UINT4 mode);                             

INT4 hyPhy20gVcatLcasSonetLinkSts1TxMemberUnProv(struct file *fileHndl, 
                                                 UINT4 sts48Num, UINT4 sts12Num,
                                                 UINT4 sts3Num, UINT4 sts1Num);                             
                                        
INT4 hyPhy20gVcatLcasSonetSts1LcasMemberAddRemove (struct file *fileHndl, 
                                                   UINT4 sts48Num, 
                                                   UINT4 sts12Num, 
                                                   UINT4 sts3Num, UINT4 sts1Num, 
                                                   UINT4 command); 

INT4 hyPhy20gVcatLcasSonetSts3Sts1GrpDiffDelayCfg(struct file *fileHndl, 
                                              		UINT4 grpId, UINT4 grpType,
                                              		UINT4 maxDly);                                     

INT4 hyPhy20gVcatLcasSonetLinkSts192TxProv(struct file *fileHndl, UINT4 grpId);
INT4 hyPhy20gVcatLcasSonetLinkSts192RxProv(struct file *fileHndl, UINT4 grpId);
INT4 hyPhy20gVcatLcasSonetLinkSts192TxUnProv(struct file *fileHndl);
INT4 hyPhy20gVcatLcasSonetLinkSts192RxUnProv(struct file *fileHndl);
INT4 hyPhy20gVcatLcasSonetLinkSts48TxProv(struct file *fileHndl, UINT4 grpId,
                                          UINT4 sts48Num);
INT4 hyPhy20gVcatLcasSonetLinkSts48RxProv(struct file *fileHndl, UINT4 grpId,
                                          UINT4 sts48Num);
INT4 hyPhy20gVcatLcasSonetLinkSts48TxUnProv(struct file *fileHndl,
                                            UINT4 sts48Num);
INT4 hyPhy20gVcatLcasSonetLinkSts48RxUnProv(struct file *fileHndl, 
                                            UINT4 sts48Num);
INT4 hyPhy20gVcatLcasSonetLinkSts24TxProv(struct file *fileHndl, UINT4 grpId,
                                          UINT4 sts48Num, UINT4 sts24Num);
INT4 hyPhy20gVcatLcasSonetLinkSts24RxProv(struct file *fileHndl, UINT4 grpId,
                                          UINT4 sts48Num, UINT4 sts24Num);
INT4 hyPhy20gVcatLcasSonetLinkSts24TxUnProv(struct file *fileHndl,
                                            UINT4 sts48Num, UINT4 sts24Num);
INT4 hyPhy20gVcatLcasSonetLinkSts24RxUnProv(struct file *fileHndl,
                                            UINT4 sts48Num, UINT4 sts24Num);
INT4 hyPhy20gVcatLcasSonetLinkSts12TxProv(struct file *fileHndl, UINT4 grpId,
                                          UINT4 sts48Num, UINT4 sts12Num);
INT4 hyPhy20gVcatLcasSonetLinkSts12RxProv(struct file *fileHndl, UINT4 grpId,
                                          UINT4 sts48Num, UINT4 sts12Num);
INT4 hyPhy20gVcatLcasSonetLinkSts12TxUnProv(struct file *fileHndl, 
                                            UINT4 sts48Num, UINT4 sts12Num);
INT4 hyPhy20gVcatLcasSonetLinkSts12RxUnProv(struct file *fileHndl, 
                                            UINT4 sts48Num, UINT4 sts12Num);
INT4 hyPhy20gVcatLcasSonetLinkSts3Sts1TxProv(struct file *fileHndl, UINT4 sts48Num,
                                             UINT4 sts12Num, UINT4 sts3Num,
                                             UINT4 concat);
INT4 hyPhy20gVcatLcasSonetLinkSts3Sts1RxProv(struct file *fileHndl, UINT4 sts48Num,
                                          UINT4 sts12Num, UINT4 sts3Num,
                                          UINT4 concat);
INT4 hyPhy20gVcatLcasSonetLinkSts3Sts1TxUnProv(struct file *fileHndl, UINT4 sts48Num,
                                             UINT4 sts12Num, UINT4 sts3Num);
INT4 hyPhy20gVcatLcasSonetLinkSts3Sts1RxUnProv(struct file *fileHndl, UINT4 sts48Num,
                                          UINT4 sts12Num, UINT4 sts3Num);

                                                           
/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/ 

INT4 vcatHyPhy20gVcatDestroyCtxt(struct file *fileHndl, UINT4 slice);

INT4 vcatHyPhy20gVcatGetGrpRate(struct file *fileHndl, UINT4 slice, UINT4 grpId,
                                UINT4 *memType, UINT4 *numMem);
UINT4 vcatHyPhy20gVcatGetCtxtSize(struct file *fileHndl);
INT4 vcatHyPhy20gVcatExtractCtxt(struct file *fileHndl, 
                                 void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 vcatHyPhy20gVcatRestoreCtxt(struct file *fileHndl, 
                                 void *pCtxtBuf, UINT4 *pParsedSize);
INT4 vcataHyPhy20gVcataAuditCtxt(struct file *fileHndl);
INT4 vcatbHyPhy20gVcatbAuditCtxt(struct file *fileHndl);
INT4 vcatHyPhy20gVcatAuditCtxt(struct file *fileHndl);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_VCAT_H */
/* end of file */
