/******************************************************************************/
/**  COPYRIGHT (C) 2000-2007 PMC-SIERRA, INC. ALL RIGHTS RESERVED.           **/
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
**
**  MODULE      : HyPhy Configuration Code
**
**  FILE        : hyphy20g_indirect.h
**
**  $Date       : $
**
**  $Revision   : $
**
**  DESCRIPTION : This is the header file for hyphy20g_indirect.c file.
**
**  NOTES       :
**
*******************************************************************************/

#ifndef _HYPHY20G_INDIRECT_H
#define _HYPHY20G_INDIRECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <linux/kernel.h>
#include "hyphy20g.h"
#include "hyphy20g_typs.h"
#include "hyphy20g_defs.h"
#include "hyphy20g_err.h"
#include "hyphy20g_hw.h"

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* FUNCTION PROTOTYPES                                                        */
/******************************************************************************/
INT4 vcatHyPhy20gRVCPOMemIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 memSel, UINT4 memAddr, UINT4 *idata0,
                                 UINT4 *idata1, UINT4 *idata2, UINT4 *idata3,
                                 UINT4 *idata4);

INT4 vcatHyPhy20gRVCPOMemIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 memSel, UINT4 memAddr, UINT4 idata0,
                                  UINT4 idata1, UINT4 idata2, UINT4 idata3,
                                  UINT4 idata4);

INT4 vcatHyPhy20gRVCPOIntIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 memSel, UINT4 memAddr, UINT4 *idata0);

INT4 vcatHyPhy20gRVCPOIntIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 memSel, UINT4 memAddr, UINT4 idata0);

INT4 vcatHyPhy20gTVCPOMemIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 memSel, UINT4 memAddr,UINT4 *idata0,
                                 UINT4 *idata1);

INT4 vcatHyPhy20gTVCPOMemIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 memSel, UINT4 memAddr, UINT4 idata0,
                                  UINT4 idata1);

INT4 vcatHyPhy20gTVCPOIntIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 memAddr, UINT4 *idata0);

INT4 vcatHyPhy20gTVCPOIntIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 memAddr, UINT4 idata0);

INT4 pifHyPhy20gRXCalProgIndRead(struct file *fileHndl, UINT4 rxCalAddr,
                                 UINT4 *idata0);

INT4 pifHyPhy20gRXCalProgIndWrite(struct file *fileHndl, UINT4 rxCalAddr,
                                  UINT4 idata0);

INT4 pifHyPhy20gTXCalProgIndRead(struct file *fileHndl, UINT4 txCalAddr,
                                 UINT4 *idata0);

INT4 pifHyPhy20gTXCalProgIndWrite(struct file *fileHndl, UINT4 txCalAddr,
                                  UINT4 idata0);

INT4 pifHyPhy20gIntlnkCalRead (struct file *fileHndl, UINT4 dir, UINT4 cal_addr,
                               UINT4 *data);

INT4 pifHyPhy20gIntlnkCalWrite (struct file *fileHndl, UINT4 dir,
                                UINT4 cal_addr, UINT4 data);

INT4 cpbHyPhy20gWrPtrCtxtIndRead(struct file *fileHndl, UINT4 wrPtrCfgAddr,
                                 UINT4 *idata0, UINT4 *idata1);

INT4 cpbHyPhy20gWrPtrCtxtIndWrite(struct file *fileHndl, UINT4 wrPtrCfgAddr,
                                  UINT4 idata0, UINT4 idata1);

INT4 cpbHyPhy20gRdPtrCtxtIndRead(struct file *fileHndl, UINT4 rdPtrCfgAddr,
                                 UINT4 *idata0, UINT4 *idata1);

INT4 cpbHyPhy20gRdPtrCtxtIndWrite(struct file *fileHndl, UINT4 rdPtrCfgAddr,
                                  UINT4 idata0, UINT4 idata1);

INT4 cpbHyPhy20gFifoDepthIndRead(struct file *fileHndl, UINT4 fifoCfgAddr,
                                 UINT4 *idata0, UINT4 *idata1, UINT4 *idata2);

INT4 cpbHyPhy20gFifoDepthIndWrite(struct file *fileHndl, UINT4 fifoCfgAddr,
                                  UINT4 idata0, UINT4 idata1, UINT4 idata2);

INT4 gfpfHyPhy20gTXDPChanIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 cfgChan, UINT4 *idata0, UINT4 *idata1,
                                 UINT4 *idata2);

INT4 gfpfHyPhy20gTXDPChanIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 cfgChan, UINT4 idata0, UINT4 idata1,
                                  UINT4 idata2);

INT4 gfpfHyPhy20gTXDPExtHdrIndRead(struct file *fileHndl, UINT4 slice,
                                   UINT4 Chan, UINT4 cfrmPriority,
                                   UINT4 gfpCfrm, UINT4 burstInit,
                                   UINT4 burstEnd, UINT4 *idata0);

INT4 gfpfHyPhy20gTXDPExtHdrIndWrite(struct file *fileHndl, UINT4 slice,
                                    UINT4 Chan, UINT4 cfrmPriority,
                                    UINT4 gfpCfrm, UINT4 burstInit,
                                    UINT4 burstEnd, UINT4 idata0);

INT4 gfpfHyPhy20gTXDPPerfMonIndRead(struct file *fileHndl, UINT4 slice,
                                    UINT4 cfgChan, UINT4 trigger, UINT4 *idata0,
                                    UINT4 *idata1, UINT4 *idata2, UINT4 *idata3,
                                    UINT4 *idata4, UINT4 *idata5);

INT4 gfpfHyPhy20gRXDPChanIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 pageAddr, UINT4 cpbidAddr,
                                 UINT4 *idata0, UINT4 *idata1);

INT4 gfpfHyPhy20gRXDPChanIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 pageAddr, UINT4 cpbidAddr,
                                  UINT4 idata0, UINT4 idata1);

INT4 gfptHyPhy20gRXGFPTChanIndRead(struct file *fileHndl, UINT4 slice,
                                   UINT4 cfgChan, UINT4 *idata0);

INT4 gfptHyPhy20gRXGFPTChanIndWrite(struct file *fileHndl, UINT4 slice,
                                    UINT4 cfgChan, UINT4 idata0);

INT4 gfptHyPhy20gRXGFPTPerfMonIndRead(struct file *fileHndl, UINT4 slice,
                                      UINT4 cfgChan, UINT4 *idata0,
                                      UINT4 *idata1, UINT4 *idata2,
                                      UINT4 *idata3);

INT4 gfptHyPhy20gTXGFPTReqProChanIndRead(struct file *fileHndl, UINT4 slice,
                                         UINT4 cfgChan, UINT4 *idata0,
                                         UINT4 *idata1);

INT4 gfptHyPhy20gTXGFPTReqProChanIndWrite(struct file *fileHndl, UINT4 slice,
                                          UINT4 cfgChan, UINT4 idata0,
                                          UINT4 idata1);

INT4 gfptHyPhy20gTXGFPTChanIndRead(struct file *fileHndl, UINT4 slice,
                                   UINT4 cfgChan, UINT4 *idata0, UINT4 *idata1);

INT4 gfptHyPhy20gTXGFPTChanIndWrite(struct file *fileHndl, UINT4 slice,
                                    UINT4 cfgChan, UINT4 idata0, UINT4 idata1);


INT4 gfptHyPhy20gTXGFPTChanPauseIndWrite(struct file *fileHndl, UINT4 slice,
                                         UINT4 cfgChan, UINT4 idata0,
                                         UINT4 idata1);

INT4 gfptHyPhy20gTXGFPTPerfMonIndRead(struct file *fileHndl, UINT4 slice,
                                      UINT4 cfgChan, UINT4 *idata0,
                                      UINT4 *idata1, UINT4 *idata2,
                                      UINT4 *idata3);

INT4 vcatHyPhy20gTVCPMemIndRead(struct file *fileHndl, UINT4 memSel,
                                UINT4 memAddr, UINT4 *idata0, UINT4 *idata1,
                                UINT4 *idata2, UINT4 *idata3, UINT4 *idata4,
                                UINT4 *idata5, UINT4 *idata6, UINT4 *idata7);

INT4 vcatHyPhy20gTVCPMemIndWrite(struct file *fileHndl, UINT4 memSel,
                                 UINT4 memAddr, UINT4 idata0, UINT4 idata1,
                                 UINT4 idata2, UINT4 idata3, UINT4 idata4,
                                 UINT4 idata5, UINT4 idata6, UINT4 idata7);

INT4 vcatHyPhy20gTVCPIntIndRead(struct file *fileHndl, UINT4 memAddr,
                                UINT4 *idata0);

INT4 vcatHyPhy20gTVCPIntIndWrite(struct file *fileHndl, UINT4 memAddr,
                                 UINT4 idata0);

INT4 vcatHyPhy20gRVCPMemIndRead(struct file *fileHndl, UINT4 memSel,
                                UINT4 memAddr, UINT4 *idata0, UINT4 *idata1,
                                UINT4 *idata2, UINT4 *idata3, UINT4 *idata4,
                                UINT4 *idata5, UINT4 *idata6, UINT4 *idata7);

INT4 vcatHyPhy20gRVCPMemIndWrite(struct file *fileHndl, UINT4 memSel,
                                 UINT4 memAddr, UINT4 idata0, UINT4 idata1,
                                 UINT4 idata2, UINT4 idata3, UINT4 idata4,
                                 UINT4 idata5, UINT4 idata6, UINT4 idata7);

INT4 vcatHyPhy20gRVCPIntIndRead(struct file *fileHndl, UINT4 memSel,
                                UINT4 memAddr, UINT4 *idata0);

INT4 vcatHyPhy20gRVCPIntIndWrite(struct file *fileHndl, UINT4 memSel,
                                 UINT4 memAddr, UINT4 idata0);

INT4 tseHyPhy20gPRGMIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                            UINT4 path, UINT4 *idata0);

INT4 tseHyPhy20gPRGMIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                             UINT4 path, UINT4 idata0);

INT4 tseHyPhy20gSHPIIndRead(struct file *fileHndl, UINT4 iAddr, UINT4 sts3,
                            UINT4 sts1, UINT4 *idata0);

INT4 tseHyPhy20gSHPIIndWrite(struct file *fileHndl, UINT4 iAddr, UINT4 sts3,
                             UINT4 sts1, UINT4 idata0);

INT4 tseHyPhy20gPCTLCfgMemIndRead(struct file *fileHndl, UINT4 oPort, UINT4 oTs,
                                  UINT4 *idata0);

INT4 tseHyPhy20gPCTLCfgMemIndWrite(struct file *fileHndl, UINT4 oPort,
                                   UINT4 oTs, UINT4 idata0);

INT4 dmiHyPhy20gSHPIIndRead(struct file *fileHndl, UINT4 sliceA, UINT4 sliceB,
                            UINT4 iAddr, UINT4 sts3, UINT4 sts1, UINT4 *idata0);

INT4 dmiHyPhy20gSHPIIndWrite(struct file *fileHndl, UINT4 sliceA, UINT4 sliceB,
                             UINT4 iAddr, UINT4 sts3, UINT4 sts1, UINT4 idata0);

INT4 dmiHyPhy20gSTFAIndRead(struct file *fileHndl, UINT4 sliceA, UINT4 sliceB,
                            UINT4 iAddr, UINT4 sts3, UINT4 sts1, UINT4 *idata0);

INT4 dmiHyPhy20gSTFAIndWrite(struct file *fileHndl, UINT4 sliceA, UINT4 sliceB,
                             UINT4 iAddr, UINT4 sts3, UINT4 sts1, UINT4 idata0);

INT4 sonetHyPhy20gSBERSectIndRead(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 *idata0);

INT4 sonetHyPhy20gSBERSectIndWrite(struct file *fileHndl, UINT4 slice,
                                   UINT4 iAddr, UINT4 idata0);

INT4 sonetHyPhy20gRTTPSectIndRead(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 *idata0);

INT4 sonetHyPhy20gRTTPSectIndWrite(struct file *fileHndl, UINT4 slice,
                                   UINT4 iAddr, UINT4 idata0);

INT4 sonetHyPhy20gRSHPIIndRead(struct file *fileHndl, UINT4 slice,
                               UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                               UINT4 *idata0);

INT4 sonetHyPhy20gRSHPIIndWrite(struct file *fileHndl, UINT4 slice,
                                UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                UINT4 idata0);

INT4 sonetHyPhy20gRHAPIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 *idata0);

INT4 sonetHyPhy20gRHAPIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                               UINT4 sts3, UINT4 sts1, UINT4 idata0);

INT4 sonetHyPhy20gRTTPPathIndRead(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                  UINT4 *idata0);

INT4 sonetHyPhy20gRTTPPathIndWrite(struct file *fileHndl, UINT4 slice,
                                   UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                   UINT4 idata0);

INT4 sonetHyPhy20gRSVCAIndRead(struct file *fileHndl, UINT4 slice,
                               UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                               UINT4 *idata0);

INT4 sonetHyPhy20gRSVCAIndWrite(struct file *fileHndl, UINT4 slice,
                                UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                UINT4 idata0);

INT4 sonetHyPhy20gSBERPathIndRead(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                  UINT4 *idata0);

INT4 sonetHyPhy20gSBERPathIndWrite(struct file *fileHndl, UINT4 slice,
                                   UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                   UINT4 idata0);

INT4 sonetHyPhy20gSARCIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 *idata0);

INT4 sonetHyPhy20gSARCIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                               UINT4 sts3, UINT4 sts1, UINT4 idata0);

INT4 sonetHyPhy20gTTTPSectIndRead(struct file *fileHndl, UINT4 slice,
                                   UINT4 iAddr, UINT4 *idata0);

INT4 sonetHyPhy20gTTTPSectIndWrite(struct file *fileHndl, UINT4 slice,
                                   UINT4 iAddr, UINT4 idata0);

INT4 sonetHyPhy20gTHPPIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 *idata0);

INT4 sonetHyPhy20gTHPPIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                               UINT4 sts3, UINT4 sts1, UINT4 idata0);

INT4 sonetHyPhy20gTTTPPathIndRead(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                  UINT4 *idata0);

INT4 sonetHyPhy20gTTTPPathIndWrite(struct file *fileHndl, UINT4 slice,
                                   UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                   UINT4 idata0);

INT4 sonetHyPhy20gTSVCAIndRead(struct file *fileHndl, UINT4 slice,
                               UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                               UINT4 *idata0);

INT4 sonetHyPhy20gTSVCAIndWrite(struct file *fileHndl, UINT4 slice,
                                UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                UINT4 idata0);

INT4 sonetHyPhy20gRTERBIndRead(struct file *fileHndl, UINT4 slice,
                               UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                               UINT4 *idata0);

INT4 sonetHyPhy20gRTERBIndWrite(struct file *fileHndl, UINT4 slice,
                                UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                UINT4 idata0);

INT4 sonetHyPhy20gTRERBIndRead(struct file *fileHndl, UINT4 slice,
                               UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                               UINT4 *idata0);

INT4 sonetHyPhy20gTRERBIndWrite(struct file *fileHndl, UINT4 slice,
                                UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                UINT4 idata0);

INT4 sonetHyPhy20gTSHPIIndRead(struct file *fileHndl, UINT4 slice,
                               UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                               UINT4 *idata0);

INT4 sonetHyPhy20gTSHPIIndWrite(struct file *fileHndl, UINT4 slice,
                                UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                UINT4 idata0);

INT4 hoppHyPhy20gRSHPIIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 *idata0);

INT4 hoppHyPhy20gRSHPIIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                               UINT4 sts3, UINT4 sts1, UINT4 idata0);

INT4 hoppHyPhy20gRHAPIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                             UINT4 sts3, UINT4 sts1, UINT4 *idata0);

INT4 hoppHyPhy20gRHAPIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 idata0);

INT4 hoppHyPhy20gRTTPPathIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                 UINT4 *idata0);

INT4 hoppHyPhy20gRTTPPathIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                  UINT4 idata0);

INT4 hoppHyPhy20gSBERPathIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                 UINT4 *idata0);

INT4 hoppHyPhy20gSBERPathIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                  UINT4 idata0);

INT4 hoppHyPhy20gSARCIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                             UINT4 sts3, UINT4 sts1, UINT4 *idata0);

INT4 hoppHyPhy20gSARCIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 idata0);

INT4 hoppHyPhy20gTHPPIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                             UINT4 sts3, UINT4 sts1, UINT4 *idata0);

INT4 hoppHyPhy20gTHPPIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 idata0);

INT4 hoppHyPhy20gTTTPPathIndRead(struct file *fileHndl, UINT4 slice,
                                 UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                 UINT4 *idata0);

INT4 hoppHyPhy20gTTTPPathIndWrite(struct file *fileHndl, UINT4 slice,
                                  UINT4 iAddr, UINT4 sts3, UINT4 sts1,
                                  UINT4 idata0);

INT4 hoppHyPhy20gTTERBIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 *idata0);

INT4 hoppHyPhy20gTTERBIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                               UINT4 sts3, UINT4 sts1, UINT4 idata0);

INT4 hoppHyPhy20gRRERBIndRead(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                              UINT4 sts3, UINT4 sts1, UINT4 *idata0);

INT4 hoppHyPhy20gRRERBIndWrite(struct file *fileHndl, UINT4 slice, UINT4 iAddr,
                               UINT4 sts3, UINT4 sts1, UINT4 idata0);

INT4 cbriHyphy20gIntlnkCalRead (struct file *fileHndl, UINT4 slice, UINT4 dir,
                                UINT4 cal_addr, UINT4 *data);

INT4 cbriHyphy20gIntlnkCalWrite (struct file *fileHndl, UINT4 slice, UINT4 dir,
                                 UINT4 cal_addr, UINT4 data);
INT4 vcatHyPhy20gRVCPOMemIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 vcatHyPhy20gRVCPOIntIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 vcatHyPhy20gTVCPOMemIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 vcatHyPhy20gTVCPOIntIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 pifHyPhy20gRXCalProgIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 pifHyPhy20gTXCalProgIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 cpbHyPhy20gWrPtrCtxtIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 cpbHyPhy20gRdPtrCtxtIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 cpbHyPhy20gFifoDepthIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 gfpfHyPhy20gTXDPChanIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 gfpfHyPhy20gTXDPExtHdrIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 gfpfHyPhy20gTXDPPerfMonIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 gfpfHyPhy20gRXDPChanIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 gfptHyPhy20gRXGFPTChanIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 gfptHyPhy20gRXGFPTPerfMonIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 gfptHyPhy20gTXGFPTReqProChanIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 gfptHyPhy20gTXGFPTChanIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 gfptHyPhy20gTXGFPTPerfMonIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 vcatHyPhy20gTVCPMemIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 vcatHyPhy20gTVCPIntIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 vcatHyPhy20gRVCPMemIndIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 vcatHyPhy20gRVCPIntIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 tseHyPhy20gPRGMIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);                

INT4 tseHyPhy20gSHPIIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 tseHyPhy20gPCTLCfgMemIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 dmiHyPhy20gSHPIIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 dmiHyPhy20gSTFAIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gSBERSectIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gRTTPSectIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gRSHPIIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gRHAPIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gRTTPPathIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gRSVCAIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gSBERPathIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gSARCIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gTTTPSectIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gTHPPIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gTTTPPathIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gTSVCAIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gRTERBIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gTRERBIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 sonetHyPhy20gTSHPIIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 hoppHyPhy20gRSHPIIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 hoppHyPhy20gRHAPIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 hoppHyPhy20gSBERPathIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 hoppHyPhy20gSARCIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 hoppHyPhy20gTHPPIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 hoppHyPhy20gTTTPPathIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 hoppHyPhy20gTTERBIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 hoppHyPhy20gRRERBIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 hoppHyPhy20gRTTPPathIndDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 cbriHyphy20gIntlnkCalDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

INT4 pifHyPhy20gIntlnkCalDump(struct file *fileHndl, void *pDumpBuff, UINT4 *pValidDumpBuffSize);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_INDIRECT_H */
/* end of file */
