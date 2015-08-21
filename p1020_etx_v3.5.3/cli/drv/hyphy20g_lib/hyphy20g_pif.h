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
**  FILE        : hyphy20g_pif.h
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

#ifndef _HYPHY20G_PIF_H
#define _HYPHY20G_PIF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_util_intlkn.h"
#include "hyphy20g_util_scbs.h"

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/
#define HYPHY20G_PIF_NUM_TX_SCHD                    2
#define HYPHY20G_PIF_TX_SCHD_MAX_CHNL               150
#define HYPHY20G_PIF_TX_SCHD_NULL_CHNL              0xff
#define HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW      4
#define HYPHY20G_PIF_TX_SCHD_MAX_NUM_ROW            128


#define HYPHY20G_PIF_MAX_CHNL                       150


#define HYPHY20G_PIF_XAUI_TXXG_RECOMMEND_TX_MAXFR        9618
#define HYPHY20G_PIF_XAUI_TXXG_RECOMMEND_TX_MINFR        28

#define HYPHY20G_PIF_XAUI_RXXG_RECOMMEND_RX_MAXFR        9618

#define HYPHY20G_PIF_XAUI_FC_CAL_MAXLEN             256
#define HYPHY20G_PIF_XAUI_FC_TXCAL_MAXLEN           256
#define HYPHY20G_PIF_XAUI_FC_RXCAL_MAXLEN           256
#define HYPHY20G_PIF_XAUI_FC_CAL_NULL_CHNL          151
#define HYPHY20G_PIF_XAUI_FC_CAL_LINKLVL_PORT       150


#define HYPHY20G_PIF_FC_MODE_INTLKN_IBFC            0
#define HYPHY20G_PIF_FC_MODE_INTLKN_OOBFC           1
#define HYPHY20G_PIF_FC_MODE_XAUI_SPIOOBFC          2
#define HYPHY20G_PIF_FC_MODE_XAUI_PAUSEFRM          3
#define HYPHY20G_PIF_FC_MODE_INTLKN_IBFC_INORDER    0x10
#define HYPHY20G_PIF_FC_MODE_INTLKN_IBFC_PROG       0x20

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/
typedef enum {
    HYPHY20G_PIF_OPMODE_DISABLE      = 0,    /* disable the PIF subsystem */
    HYPHY20G_PIF_OPMODE_EXAUI0       = 1,    /* XAUI mode with XAUI0 slice */
    HYPHY20G_PIF_OPMODE_EXAUI1       = 2,    /* XAUI mode with XAUI1 slice */
    HYPHY20G_PIF_OPMODE_DUAL_EXAUI   = 3,    /* XAUI mode with XAUI0 and XAUI1 */
    HYPHY20G_PIF_OPMODE_INTERLAKEN_INBAND = 4, /* INTERLAKEN with in-band flow control */
    HYPHY20G_PIF_OPMODE_INTERLAKEN_OOB    = 5, /* INTERLAKEN with out-of-band flow control */
    HYPHY20G_PIF_OPMODE_SXAUI0      = 6,    /* sXAUI mode with XAUI0 slice */
    HYPHY20G_PIF_OPMODE_SXAUI1      = 7,    /* sXAUI mode with XAUI1 slice */
    HYPHY20G_PIF_OPMODE_DUAL_SXAUI  = 8,    /* sXAUI mode with XAUI0 and XAUI1 */
    HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 = 9,    
    HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1 = 10    
} eHYPHY20G_PIF_OPERATION_MODE;

typedef enum {
    HYPHY20G_PIF_CHNL_START         = 0,
    HYPHY20G_PIF_CHNL_EQUIPPED      = 1,
    HYPHY20G_PIF_CHNL_OPERATIONAL   = 2
} eHYPHY20G_PIF_CHNL_STATE;


typedef enum {
    HYPHY20G_PIF_XAUI_LINE_LOOP = 0, /* set XAUI to loopback the rx data stream
                                        to the tx data stream at the SerDes. */
    HYPHY20G_PIF_XAUI_SYS_LOOP  = 1, /* set XAUI to loopback the tx data stream
                                        to the tx data stream. */
    HYPHY20G_PIF_XAUI_NO_LOOP   = 2  /* disable loopback, and set XAUI to
                                        normal operation */
} eHYPHY20G_PIF_XAUI_LOOPTYPE;


/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gPifInit(struct file *fileHndl, UINT4 mode, UINT4 lnkZoneXoff, 
                     UINT4 lnkZoneXon);

INT4 hyPhy20gPifIntfEnable(struct file *fileHndl, UINT4 enable, UINT4 updateMsk);

INT4 hyPhy20gPifLaneDisable(struct file *fileHndl, UINT4 disableMsk);

INT4 hyPhy20gPifFlowCtrlInit(struct file *fileHndl, UINT4 dir, UINT4 zone, 
                             UINT4 calLen, UINT4 oobFcClk);

INT4 hyPhy20gPifFlowCtrlEnable(struct file *fileHndl, UINT4 dir, UINT4 enable);

INT4 hyPhy20gPifTxChnlEnable(struct file *fileHndl, UINT4 chnlId, UINT4 enable,
                             UINT4 intf, UINT4 chnlRate);

INT4 hyPhy20gPifChnlFcMap(struct file *fileHndl, UINT4 dir, UINT4 enable,
                          UINT4 calAddr, UINT4 fcPort);

INT4 hyPhy20gPifXauiInit(struct file *fileHndl, UINT4 enable, UINT4 hdrStrip,
                         UINT4 segSize, UINT4 schdLen0, UINT4 schdLen1);

INT4 hyPhy20gPifXauiRxPadCfg(struct file *fileHndl, UINT4 slice, UINT4 rxPadEn);

INT4 hyPhy20gPifStdXauiCpbChnlCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 cpbChnlId);

INT4 hyPhy20gPifStdXauiPauseCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 txPauseTime,UINT4 txPauseInterval,
                                UINT4 txPauseQuantum, UINT4 txPauseQuantumEn);

INT4 hyPhy20gPifXauiSetLoopback(struct file *fileHndl, UINT4 slice,
                                UINT4 loopType);

INT4 hyPhy20gPifSpiFcCalCfg(struct file *fileHndl, UINT4 dir, UINT4 parity,
                            UINT4 dip2En, UINT4 iterationCnt);

INT4 hyPhy20gPifIntlknInit(struct file *fileHndl, UINT4 mode, UINT4 schdLen, 
                           UINT4 trafficType);
INT4 hyPhy20gPifIntlknFlexInit(struct file *fileHndl, UINT4 mode,UINT4 lanes,
                           UINT4 schdLen, UINT4 trafficType);

INT4 hyPhy20gPifIntlknTxRateLimitCfg(struct file *fileHndl, UINT4 rateLimit,
                                     UINT4 burstLimit, UINT4 byteGran);

INT4 hyPhy20gPifIntlknEnable(struct file *fileHndl, UINT4 enable);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 pifHyPhy20gPifCreateCtxt(struct file *fileHndl, UINT4 pifMode);

INT4 pifHyPhy20gPifDestroyCtxt(struct file *fileHndl, UINT4 scope);

INT4 hyPhy20gPifIntlknFcInit(struct file *fileHndl, UINT4 dir, UINT4 mode,
                             UINT4 calLen);

INT4 hyPhy20gPifIntlknFcPortMap(struct file *fileHndl, UINT4 dir, UINT4 enable,
                                UINT4 calAddr, UINT4 fcPort);

INT4 hyPhy20gPifSpiFcCalInit(struct file *fileHndl, UINT4 dir, UINT4 calLen);

INT4 hyPhy20gPifSpiFcPortMap(struct file *fileHndl, UINT4 dir, UINT4 enable,
                             UINT4 calAddr, UINT4 fcPort);
UINT4 pifHyPhy20gPifGetCtxtSize(struct file *fileHndl);
INT4 pifHyPhy20gPifExtractCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 pifHyPhy20gPifRestoreCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pParsedSize);
INT4 pifHyPhy20gPifAuditCtxt(struct file *fileHndl);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_PIF_H */
/* end of file */
