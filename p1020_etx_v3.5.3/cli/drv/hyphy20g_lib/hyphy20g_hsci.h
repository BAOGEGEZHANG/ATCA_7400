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
**  MODULE      : HyPHY 20G HSCI Subsystem Header File
**
**  FILE        : hyphy20g_hsci.h
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_HSCI_H
#define _HYPHY20G_HSCI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_util_pmm.h"
#include "hyphy20g_util_ecom.h"
#include "hyphy20g_hostmsg.h"

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/
#define HYPHY20G_HSCI_NUM_SLICE         2  /* Number of HSCI slices */

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* TEMPORARY DEFINES                                                          */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/
typedef enum {
    /* High speed client interface traffics */
    HSCI_10GE_MAC_TEMINATION    = 1, /* 10GE MAC Terminated (including Gsup43
                                        Clause 7.1 and 7.2 10GE MAC terminated) */
    HSCI_GSUP43_CLAUSE73        = 2, /* Gsup43 Clause 7.3 */
    HSCI_FC_8G                  = 3, /* FC 8G and subrate */
    HSCI_HS_CBR_32BIT_MODE      = 4, /* High Speed CBR (including Gsup43
                                        Clause 7.1 and 7.2 10GE without
                                        MAC termination, STS-192 asynchronous
                                        transparent) */
    HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_EN      = 5, 
                                     /* High Speed CBR (including Gsup43
                                        Clause 7.1 and 7.2 10GE transparent,
                                        STS-192 asynchronous transparent).
                                        Enable disparity inverter on the 
                                        transmit path */
    HSCI_MAC_GSUP43_CLAUSE73    = 6, /* Gsup43 Clause 7.3 Terminating
                                        application through MAC */
    HSCI_MAC_GSUP43_CLAUSE62    = 7, /* Gsup43 Clause 6.2 Terminating
                                        application through MAC */
    HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_DIS      = 8, 
                                     /* High Speed CBR (including Gsup43
                                        Clause 7.1 and 7.2 10GE transparent,
                                        STS-192 asynchronous transparent).
                                        Disable disparity inverter on the 
                                        transmit path */
    HSCI_TRAFFIC_MAX            = 8  /* maximun HSCI traffic number */
} eHYPHY20G_HSCI_TRAFFIC_TYPES;


/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gHsciInit(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hyPhy20gHsciSelRefClk(struct file *fileHndl, UINT4 slice, UINT4 clkMode);

INT4 hyPhy20gHsciDataPathEnable(struct file *fileHndl, UINT4 slice,
                                UINT4 dir, UINT4 enable);

INT4 hyPhy20gHsci10GeLanTxxgCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 ipg, UINT4 ipgReducEn,
                                UINT4 crcAppendEn, UINT4 frmPadEn);

INT4 hyPhy20gHsci10GeLanRxxgCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 oversizeResp,
                                UINT4 mifgEn, UINT4 noPreamble,
                                UINT4 purePreamble, UINT4 longPreamble,
                                UINT4 passAll, UINT4 frmLenCheck,
                                UINT4 passErrFrm, UINT4 passCtlFrm,
                                UINT4 contiguousEn, UINT4 crcStrip,
                                UINT4 mifg, UINT4 crcCkDis, UINT4 shortPktEn,
                                UINT4 hdrPrmblEn, UINT4 hdrSize,
                                UINT4 cutThruThreshold);

INT4 hyPhy20gHsciRxxgPrmbCfg(struct file *fileHndl, UINT4 slice, 
                             UINT4 prmbProc, UINT4 hdrSize);
                             
INT4 hyPhy20gHsciGsup43C73ModeCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 txErrDrop, UINT4 txPktType,
                                  UINT4 txOsType, UINT4 txMinIpg,
                                  UINT4 unidirEn, UINT4 sendIdleEn,
                                  UINT4 fifoHmark,
                                  UINT4 losInv, UINT4 dscrdZone, UINT4 sigEn,
                                  UINT4 seqEn, UINT4 allDscrd, UINT4 osPrtDis,
                                  UINT4 minOsPrt, UINT4 rxPktType,
                                  UINT4 rxOsType);

INT4 hyPhy20gHsciFc8GModeCfg(struct file *fileHndl, UINT4 slice, UINT4 osCfgMsk,
                             UINT4 arbLsbMsk, UINT4 arbLsbVal, UINT4 rxAlignMd, 
                             UINT4 txAlignMd, UINT4 rxDeScr, UINT4 txDeScr);

INT4 hyPhy20gHsciCbr10GModeCfg(struct file *fileHndl, UINT4 slice);

INT4 hyPhy20gHsciCbrUsrPattCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 dir, UINT4 olSeqHi, UINT4 olSeqLow,
                               UINT4 pattDisp);

INT4 hyPhy20gHsciMacGsup43C73ModeCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 txPayload, UINT4 txFcsAppend, 
                                     UINT4 rxFcsStrip);

INT4 hyPhy20gHsciMacGsup43C62ModeCfg(struct file *fileHndl, UINT4 slice, 
                                     UINT4 txFcsAppend, UINT4 rxFcsStrip);

INT4 hyPhy20gHsciMacGsup43RxCfg(struct file *fileHndl, UINT4 slice, 
                                UINT4 txGSup43Drop, UINT4 rxxgPassCfg);

INT4 hyPhy20gHsci10GeLanSetFrameSizes(struct file *fileHndl, UINT4 slice,
                                      UINT4 txMaxFrmSize, UINT4 txMinFrmSize,
                                      UINT4 rxMaxFrmSize);

INT4 hyPhy20gHsci10GeLanSetStationAddr(struct file *fileHndl, UINT4 slice,
                                       UINT4 hiSA, UINT4 midSA, UINT4 lowSA);

INT4 hyPhy20gHsci10GeLanSetPromiscuous(struct file *fileHndl, UINT4 slice,
                                       UINT4 promiscuous);

INT4 hyPhy20gHsci10GeLanSetPauseCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 txPauseEn, UINT4 rxPauseEn,
                                    UINT4 rxForwardPause, UINT4 rxRespOversized,
                                    UINT4 hostPause, UINT4 txPauseTime,
                                    UINT4 txPauseInterval, UINT4 txPauseQuantum,
                                    UINT4 txPauseQuantumEn);

INT4 hyPhy20gHsci10GeLanSetHostPause(struct file *fileHndl, UINT4 slice,
                                     UINT4 hostPause);
INT4 hyPhy20gHsci10GeSendLf(struct file *fileHndl, UINT4 slice,
                                    UINT4 fault);
INT4 hyPhy20gHsci10GeSendRf(struct file *fileHndl, UINT4 slice,
                                    UINT4 fault);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 hsciHyPhy20gHsciDestroyCtxt(struct file *fileHndl, UINT4 slice);

INT4 hsciHyPhy20gHsciRxFcDlolbIntPer(struct file *fileHndl, UINT4 slice,
                                     UINT4 period, UINT4 frequency,
                                     UINT4 busWidth);

INT4 hsciHyPhy20gHsciTxFcDlolbIntPer(struct file *fileHndl, UINT4 slice,
                                     UINT4 period);

INT4 hsciHyPhy20gHsciCtxtGetMode(struct file *fileHndl, UINT4 slice,
                                 UINT4 *pMode);
UINT4 hsciHyPhy20gHsciGetCtxtSize(struct file *fileHndl);
INT4 hsciHyPhy20gHsciExtractCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 hsciHyPhy20gHsciRestoreCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pParsedSize);
INT4 hsciHyPhy20gHsciAuditCtxt (struct file *fileHndl);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_HSCI_H */
/* end of file */
