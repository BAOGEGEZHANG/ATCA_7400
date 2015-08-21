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

#ifndef _HYPHY20G_GFP_H
#define _HYPHY20G_GFP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_vcat.h"
#include "hyphy20g_util_scbs.h"
#include "hyphy20g_indirect.h"

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/
#define HYPHY20G_GFP_SCBS_NUM_ROWS                                           127
#define HYPHY20G_GFP_NUM_CPBID                                                64
#define HYPHY20G_GFP_NUM_VCATID                                               64
#define HYPHY20G_GFP_GFPA_CPB_PORT_OFFSET                                     19
#define HYPHY20G_GFP_GFPB_CPB_PORT_OFFSET                                     83
#define HYPHY20G_GFP_SCHD_CAL_ENTRY_PER_ROW                                    4

#define HYPHY20G_GFP_MAX_NUM_CHNL                                             64

#define HYPHY20G_GFP_OTNMA_10GE_BIT_RATE                             10312500000
#define HYPHY20G_GFP_OTNMA_10GFC_BIT_RATE                            10518750000
#define HYPHY20G_GFP_OTNMA_OC192_BIT_RATE                             9953280000
#define HYPHY20G_GFP_OTNMA_OC48_BIT_RATE                              2488320000

#define HYPHY20G_GFP_OTNMA_10GE_VAL                                           31
#define HYPHY20G_GFP_OTNMA_10GFC_VAL                                          31
#define HYPHY20G_GFP_OTNMA_OC192_VAL                                          30
#define HYPHY20G_GFP_OTNMA_OC48_VAL                                            7

#define HYPHY20G_GFP_OTNMA_10G_WATCHDOG_TIME_PERIOD                           50
#define HYPHY20G_GFP_OTNMA_2G5_WATCHDOG_TIME_PERIOD                          200

#define HYPHY20G_OTNMA_VCAT_TO_CPB_REQ_DELAY                                   6
#define HYPHY20G_OTNMA_CPB_DATA_DELAY                                          5
#define HYPHY20G_OTNMA_VCAT_DATA_DELAY                                        24
#define HYPHY20G_OTNMD_DATA_DELAY                                             21
#define HYPHY20G_GFP_MAX_PACKET_LEN                                         9600
#define HYPHY20G_GFP_MAX_ENCAP_HEAD_LEN                                       72

#define HYPHY20G_GFP_NUM_SLICE                                                 2

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gGfpInit(struct file *fileHndl, UINT4 slice, UINT4 enable);

INT4 hyPhy20gGfpGfpGrpInit(struct file *fileHndl, UINT4 slice, UINT4 grpId,
                           UINT4 pFcsStrip, UINT4 enable);


INT4 hyPhy20gGfpFChInitRx(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                          UINT4 pauseMode, UINT4 fdfStripMode, UINT4 fdoHdrStrip,
                          UINT4 fdoFcsMode, UINT4 preambleSize);

INT4 hyPhy20gGfpTChInitRx(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 alignment, UINT4 geClient,
                                 UINT4 pauseMode, UINT4 fdfStripMode,
                                 UINT4 fdoHdrStrip, UINT4 fdoFcsMode,
                                 UINT4 preambleSize);


INT4 hyPhy20gGfpFChInitTx(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                          UINT4 upi, UINT4 gSup43Mode, UINT4 pfcsMode);

INT4 hyPhy20gGfpTChInitTx(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 numSb, UINT4 alignment,
                                 UINT4 upi, UINT4 pfcsMode);

INT4 hyPhy20gGfpHdlcGrpChInitRx (struct file *fileHndl, UINT4 slice,
                                 UINT4 fullRate, UINT4 grpId, UINT4 cpbId,
                                 UINT4 encaps, UINT4 pfcsMode,
                                 UINT4 pFcsStrip, UINT4 fdoByteStrip,
                                 UINT4 fdfByteStrip, UINT4 enable,
                                 UINT4 payloadScram);

INT4 hyPhy20gGfpHdlcGrpChInitTx (struct file *fileHndl, UINT4 slice,
                                 UINT4 fullRate, UINT4 grpId, UINT4 cpbId,
                                 UINT4 encaps, UINT4 pfcsMode, UINT4 abortType,
                                 UINT4 minFlags, UINT4 enable);


INT4 hyPhy20gGfpOtu2TranspSliceInit(struct file *fileHndl, UINT4 slice,
                                 UINT4 mode, UINT4 fixStuffEn, UINT4 enable);

INT4 hyPhy20gGfpOtu2TranspChTxAdd(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 grpId,
                                 UINT4 mappingType, UINT4 pktSize);

INT4 hyPhy20gGfpOtu2TranspChTxDelete(struct file *fileHndl, UINT4 slice,
                                      UINT4 grpId);

INT4 hyPhy20gGfpOtu2TranspChRxAdd(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 grpId, UINT4 pktSize);

INT4 hyPhy20gGfpOtu2TranspChRxDelete(struct file *fileHndl, UINT4 slice,
                                     UINT4 grpId);

INT4 hyPhy20gGfpOtu1TranspSliceInit(struct file *fileHndl, UINT4 slice,
                                 UINT4 otu1Num, UINT4 mode, UINT4 enable);

INT4 hyPhy20gGfpOtu1TranspChTxAdd(struct file *fileHndl, UINT4 slice,
                                 UINT4 otu1Num, UINT4 cpbId, UINT4 grpId,
                                 UINT4 pktSize);

INT4 hyPhy20gGfpOtu1TranspChTxDelete(struct file *fileHndl, UINT4 slice,
                                     UINT4 otu1Num, UINT4 grpId);

INT4 hyPhy20gGfpOtu1TranspChRxAdd(struct file *fileHndl, UINT4 slice,
                                 UINT4 otu1Num, UINT4 cpbId, UINT4 grpId,
                                 UINT4 pktSize);

INT4 hyPhy20gGfpOtu1TranspChRxDelete(struct file *fileHndl, UINT4 slice,
                                      UINT4 grpId);

INT4 hyPhy20gGfpFExiFilterCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 priority, UINT4 exiMatch,
                                 UINT4 ehdrLen, UINT4 fdfSend);

INT4 hyPhy20gGfpFPtiUpiFilterCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 priority, UINT4 upiMatch,
                                 UINT4 ptiMatch, UINT4 ptiEn, UINT4 upiEn,
                                 UINT4 fdfSend, UINT4 dropEn, UINT4 csfEn);

INT4 hyPhy20gGfpGenericFilterCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 grpId, UINT4 genFilter, UINT4 index,
                                 UINT4 data, UINT4 include, UINT4 reverse,
                                 UINT4 fdoSend, UINT4 fdfSend, UINT4 dropEn);


INT4 hyPhy20gGfpTxChAdd(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 grpId, UINT4 chType);

INT4 hyPhy20gGfpRxChAdd(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                                UINT4 cid, UINT4 grpId, UINT4 chSrc);

INT4 hyPhy20gGfpTxChDelete(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 grpId);

INT4 hyPhy20gGfpRxChDelete(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                                UINT4 grpId, UINT4 chSrc);

INT4 hyPhy20gGfpRateAwareTxChAdd(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 grpId, UINT4 mode,
                                 UINT4 numSb, UINT4 cpbPktLen,
                                 UINT4 gfpEncapLen);

INT4 hyPhy20gGfpRateAwareTxChDelete(struct file *fileHndl, UINT4 slice,
                                    UINT4 cpbId, UINT4 grpId, UINT4 *pOptimal);
                                    
INT4 hyPhy20gGfpSonetSysClkCfg(struct file *fileHndl, UINT4 slice, 
                               UINT4 timingMode);                                   

INT4 hyPhy20gGfpTxPayloadSrcCfg (struct file *fileHndl, UINT4 slice, 
                               UINT4 grpId, UINT4 mode);
                               
/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                 */
/******************************************************************************/

INT4 gfpHyPhy20gGfpDestroySliceCtxt(struct file *fileHndl,
                                    UINT4 slice);
UINT4 gfpHyPhy20gGfpGetCtxtSize(struct file *fileHndl);
INT4 gfpHyPhy20gGfpExtractCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 gfpHyPhy20gGfpRestoreCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pParsedSize);
INT4 gfpHyPhy20gGfpAuditCtxt(struct file *fileHndl);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_GFP_H */
/* end of file */
