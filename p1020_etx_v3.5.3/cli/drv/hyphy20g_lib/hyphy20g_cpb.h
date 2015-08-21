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
**  FILE        :  hyphy20g_hw.h
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :  Contains hardware specific definitions
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_CPB_H
#define _HYPHY20G_CPB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_strs.h"
#include "hyphy20g_indirect.h"

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/
#define HYPHY20G_CPB_NUM_CHNL      293  /* Number of CPB channel offsets */
#define HYPHY20G_CPB_NUM_BLOCK     1024 /* Number of CPB block pointer offsets */

#define HYPHY20G_INTERNAL_RAM_BLOCK_SIZE          0x14 /*  */

#define HYPHY20G_MAX_CPB_BLOCK          1023       /* Last CPB FIFO block */
#define HYPHY20G_CPB_CHNL_NO_BLOCKS     0xffff     /* Indicates in DDB that CPB channel has no */
                                                   /* block pointer queues */

#define HYPHY20G_CPB_RDCHNL_LIST_DEFAULT_LEN            10
#define HYPHY20G_CPB_PRIMARY_RD_CHNL                    0
#define HYPHY20G_CPB_NON_PRIMARY_RD_CHNL                1

#define HYPHY20G_CPB_LINE_PORT_OFFSET                   0
#define HYPHY20G_CPB_GFPA_PORT_OFFSET                   19
#define HYPHY20G_CPB_GFPB_PORT_OFFSET                   83
#define HYPHY20G_CPB_PIF_PORT_OFFSET                    147
#define HYPHY20G_CPB_SCHD_NULL_CHNL                     0xff
#define HYPHY20G_CPB_LINE_MAX_NUM_CHNL                  19

#define HYPHY20G_CPB_LINE_LS_MIN_PORT                   0
#define HYPHY20G_CPB_LINE_LS_MAX_PORT                   15
#define HYPHY20G_CPB_LINE_HS_MIN_PORT                   16
#define HYPHY20G_CPB_LINE_HS_MAX_PORT                   17
#define HYPHY20G_CPB_GFPA_MIN_PORT                      19
#define HYPHY20G_CPB_GFPA_MAX_PORT                      82
#define HYPHY20G_CPB_GFPB_MIN_PORT                      83
#define HYPHY20G_CPB_GFPB_MAX_PORT                      146
#define HYPHY20G_CPB_SCHD_START_ADDR_MSK                0x7F
#define HYPHY20G_CPB_SCHD_START_ADDR_OFF                0
#define HYPHY20G_CPB_SCHD_END_ADDR_MSK                  0x7F
#define HYPHY20G_CPB_SCHD_END_ADDR_OFF                  9

#define HYPHY20G_CPB_SCHD_MAX_GRP                                               1
#define HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW                                     4

#define HYPHY20G_CPB_PIF_WR_INTF_MAX_CHNL                                       147
#define HYPHY20G_CPB_QDR_SCHD_MAX_WR_PORT                                       4
#define HYPHY20G_CPB_QDR_SCHD_MAX_RD_PORT                                       4
#define HYPHY20G_CPB_QDR_RAW_SCHD_ENTRY_RATE                                    2560
#define HYPHY20G_CPB_QDR_SCHD_ENTRTY_SCALE_PERCENT                              90

#define HYPHY20G_CPB_LINE_WR_SCHD_MAX_CHNL                                      19
#define HYPHY20G_CPB_LINE_RD_SCHD_MAX_CHNL                                      19
#define HYPHY20G_CPB_GFPA_WR_SCHD_MAX_CHNL                                      64
#define HYPHY20G_CPB_GFPA_RD_SCHD_MAX_CHNL                                      64
#define HYPHY20G_CPB_GFPB_WR_SCHD_MAX_CHNL                                      64
#define HYPHY20G_CPB_GFPB_RD_SCHD_MAX_CHNL                                      64
#define HYPHY20G_CPB_PIF_RD_SCHD_MAX_CHNL                                       150
#define HYPHY20G_CPB_QDR_SCHD_MAX_CHNL                                          (HYPHY20G_CPB_QDR_SCHD_MAX_WR_PORT + HYPHY20G_CPB_QDR_SCHD_MAX_RD_PORT)

#define HYPHY20G_CPB_LINE_WR_SCHD_MAX_NUM_ROW                                   32
#define HYPHY20G_CPB_LINE_RD_SCHD_MAX_NUM_ROW                                   16
#define HYPHY20G_CPB_GFPA_WR_SCHD_MAX_NUM_ROW                                   64
#define HYPHY20G_CPB_GFPA_RD_SCHD_MAX_NUM_ROW                                   16
#define HYPHY20G_CPB_GFPB_WR_SCHD_MAX_NUM_ROW                                   64
#define HYPHY20G_CPB_GFPB_RD_SCHD_MAX_NUM_ROW                                   16
#define HYPHY20G_CPB_PIF_RD_SCHD_MAX_NUM_ROW                                    64
#define HYPHY20G_CPB_QDR_SCHD_MAX_NUM_ROW                                       15

#define HYPHY20G_CPB_NUM_INTF_GRP                                               4
#define HYPHY20G_CPB_NUM_INTF_GRP_RD_DIR                                        1
#define HYPHY20G_CPB_NUM_INTF_GRP_WR_DIR                                        0

#define HYPHY20G_CPB_LS_MAX_RD_THRESH                                           0x1F
#define HYPHY20G_CPB_HS_MAX_OUT_THRESH                                          0x3F
#define HYPHY20G_CPB_PI_MAX_RD_THRESH                                           0x1F
#define HYPHY20G_CPB_GFP_MAX_RD_THRESH                                          0x1F

#define HYPHY20G_CPB_SCBS_SHRINK                                                0
#define HYPHY20G_CPB_SCBS_GROW                                                  1

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
    HYPHY20G_CPB_LINE_INTF_GRP = 0, /* CPB Line Interface Group */
    HYPHY20G_CPB_GFPA_INTF_GRP = 1, /* CPB GFP A Interface Group */
    HYPHY20G_CPB_GFPB_INTF_GRP = 2, /* CPB GFP B Interface Group */
    HYPHY20G_CPB_PIF_INTF_GRP  = 3 /* CPB PIF Interface Group */
} eHYPHY20G_CPB_INTF_GFP;

typedef enum {
    LINE_WR_INTF_GRP  = 0, /* Line Write Interface Group */
    LINE_RD_INTF_GRP  = 1, /* Line Read Interface Group */
    GFP_A_WR_INTF_GRP = 2, /* GFP A Write Interface Group */
    GRP_A_RD_INTF_GRP = 3, /* GFP A Read Interface Group */
    GFP_B_WR_INTF_GRP = 4, /* GFP B Write Interface Group */
    GRP_B_RD_INTF_GRP = 5, /* GFP B Read Interface Group */
    PIF_WR_INTF_GRP   = 6, /* PIF Write Interface Group */
    PIF_RD_INTF_GRP   = 7  /* PIF Read Interface Group */
} eHYPHY20G_QDR_SCHD_ENTRIES;

/* Channel State Definition */
typedef enum {
    HYPHY20G_CPB_CHNL_START         = 0,
    HYPHY20G_CPB_CHNL_EQUIPPED      = 1,
    HYPHY20G_CPB_CHNL_OPERATIONAL   = 2
} eHYPHY20G_CPB_CHNL_STATE;

/* Interface State Definition */
typedef enum {
    HYPHY20G_CPB_INTF_DISABLED         = 0,
    HYPHY20G_CPB_INTF_ENABLED          = 1
} eHYPHY20G_CPB_INTF_STATE;

typedef enum {
    /* Low speed client interface traffics */
    LS_FCCBR_FC_8B10B       = 0, /* FC 8B/10B encoded traffic */
    LS_FCCBR_ESCON_8B10B    = 1, /* ESCON 8B/10B encoded traffic */
    LS_FCCBR_CBR_8B10B      = 2, /* 8B/10B encoded CBR traffic */
    LS_FCCBR_STS3           = 3, /* STS-3 */
    LS_FCCBR_STS12          = 4, /* STS-12 */
    LS_FCCBR_STS48          = 5, /* STS-48 */
    LS_FCCBR_CBR_NON8B10B   = 6, /* Other non-8B/10B CBR traffic */
    LS_FEGE_MAC_TEMINATION  = 7, /* FEGE MAC termination traffic */
    LS_FEGE_GE_GFPT_MAPPING = 8, /* GE to GFP-T Mapping traffic */

    /* High speed client interface traffics */
    HS_FCCBR_FC8G_8B10B         = 50, /* 8B/10B encoded 8G FC */
    HS_FCCBR_FC10G_64B66B       = 51, /* 64B/66B encoded 10G FC */
    HS_FCCBR_10GELAN_GSUP43_71  = 52, /* 10GE LAN G.Sup43 7.1 */
    HS_FCCBR_10GELAN_GSUP43_72  = 53, /* 10GE LAN G.Sup43 7.2 */
    HS_FCCBR_STS192             = 54, /* STS-192 */
    HS_10GE_MAC_TEMINATION      = 55, /* 10GE MAC termination traffic */
    HS_10GE_10GELAN_GSUP43_73   = 56, /* 10GE LAN G.Sup43 7.3 */

    /* Packet interface traffics */

    /* GFP interface traffics */

    TRAFFIC_TYPES_MAX_VALUE = 1000
} eHYPHY20G_CPB_TRAFFIC_TYPES;


/* Line Rd/Wr Interface Channel Scheduling Allocation Definition */
typedef enum {
    HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_UNDER             = 0,
    HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_OVER              = 1,
    HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_OKAY              = 2,
    HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_ADD               = 3,
    HYPHY20G_CPB_SCBS_CHNL_SUBSCRIBED_REMOVE            = 4 
} eHYPHY20G_CPB_LINE_SCBS_CHNL_ALLOC_STATE;

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gCpbSetIntQdrBlkSize(struct file *fileHndl, UINT4 blkSize);

INT4 hyPhy20gCpbChnlProv(struct file *fileHndl, UINT4 numBlocks, UINT4 wrChnlId,
                         UINT4 wrChnlPortWidth, UINT4 primRdChnlId,
                         UINT4 primRdChnlPortWidth, UINT4 chnlRate, UINT4 zone1,
                         UINT4 zone2, UINT4 zone3, UINT4 pifSysIfType,
                         UINT4 zone3Discard);

INT4 hyPhy20gCpbChnlUnProv(struct file *fileHndl, UINT4 wrChnlId);

INT4 hyPhy20gCpbNonPrimRdChnlProv(struct file *fileHndl, UINT4 wrChnlId,
                                  UINT4 rdChnlId, UINT4 rdChnlPortWidth,
                                  UINT4 pifSysIfType);

INT4 hyPhy20gCpbNonPrimRdChnlUnProv(struct file *fileHndl, UINT4 wrChnlId,
                                    UINT4 rdChnlId);

INT4 hyPhy20gCpbChnlEnable(struct file *fileHndl, UINT4 wrChnlId);

INT4 hyPhy20gCpbChnlDisable(struct file *fileHndl, UINT4 wrChnlId);

INT4 hyPhy20gCpbNonPrimRdChnlEnable(struct file *fileHndl, UINT4 wrChnlId,
                                    UINT4 rdChnlId);

INT4 hyPhy20gCpbNonPrimRdChnlDisable(struct file *fileHndl, UINT4 wrChnlId,
                                    UINT4 rdChnlId);

INT4 hyPhy20gCpbIntfInit(struct file *fileHndl, UINT4 intfGrp,
                         UINT4 rdSchdLength, UINT4 wrSchdLength,
                         UINT4 enable);

INT4 hyPhy20gCpbInit(struct file *fileHndl, UINT4 ramType, UINT4 enable);

INT4 hyPhy20gCpbPifLnkZnConfig(struct file *fileHndl,
                                UINT4 zone1,
                                UINT4 zone2,
                                UINT4 zone3);

INT4 hyPhy20gCpbChnlRateChng(struct file *fileHndl, UINT4 wrChnlId,
                             UINT4 chnlRate, UINT4 upIntfSchd, UINT4 upQdrSchd);

INT4 hyPhy20gCpbRdChnlRateChng(struct file *fileHndl, UINT4 wrChnlId,
                               UINT4 rdChnlId, UINT4 chnlRate);

INT4 hyPhy20gCpbIntfRateAwareInit(struct file *fileHndl, UINT4 intfGrp,
                                  UINT4 rdSchdLength, UINT4 wrSchdLength,
                                  UINT4 enable);

INT4 hyPhy20gCpbChnlRateAwareProv(struct file *fileHndl, UINT4 numBlocks,
                                  UINT4 wrChnlId, UINT4 wrChnlPortWidth,
                                  UINT4 primRdChnlId, UINT4 primRdChnlPortWidth,
                                  UINT4 chnlRate, UINT4 zone1, UINT4 zone2,
                                  UINT4 zone3, UINT4 pifSysIfType,
                                  UINT4 zone3Discard);

INT4 hyPhy20gCpbChnlRateAwareEnable(struct file *fileHndl, UINT4 wrChnlId);

INT4 hyPhy20gCpbChnlRateAwareDisable(struct file *fileHndl, UINT4 wrChnlId);

INT4 hyPhy20gCpbChnlRateAwareRateChng(struct file *fileHndl, UINT4 wrChnlId,
                                      UINT4 chnlRate, UINT4 upIntfSchd,
                                      UINT4 upQdrSchd);

INT4 hyPhy20gCpbNonPrimRdChnlRateAwareProv(struct file *fileHndl,
                                           UINT4 wrChnlId, UINT4 rdChnlId,
                                           UINT4 rdChnlPortWidth,
                                           UINT4 pifSysIfType);

INT4 hyPhy20gCpbNonPrimRdChnlRateAwareEnable(struct file *fileHndl,
                                             UINT4 wrChnlId, UINT4 rdChnlId);

INT4 hyPhy20gCpbNonPrimRdChnlRateAwareDisable(struct file *fileHndl,
                                              UINT4 wrChnlId, UINT4 rdChnlId);

INT4 hyPhy20gCpbRdChnlRateAwareRateChng(struct file *fileHndl, UINT4 wrChnlId,
                                        UINT4 rdChnlId, UINT4 chnlRate);

INT4 hyPhy20gCpbFifoResize(struct file *fileHndl, UINT4 wrChnlId, UINT4 addDel,
                           UINT4 zone1, UINT4 zone2, UINT4 zone3);

INT4 hyPhy20gCpbGfpPauseInit(struct file *fileHndl, UINT4 gfpSABase[2]);

INT4 hyPhy20gCpbGfpIntfPauseInit(struct file *fileHndl, UINT4 slice,
                                 UINT4 mode, UINT4 pauseTime, UINT4 repeatTime,
                                 UINT4 startZone, UINT4 endZone);

INT4 hyPhy20gCpbGfpChnlPauseSrcAddrCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 chnlId, UINT4 gfpSaOffset);

INT4 hyPhy20gCpbGfpChnlFarEndPauseEnable(struct file *fileHndl, UINT4 slice,
                                         UINT4 chnlId, UINT4 enable);

INT4 hyPhy20gCpbGfpChnlNearEndPauseEnable(struct file *fileHndl, UINT4 slice,
                                          UINT4 chnlId, UINT4 enable);

INT4 hyPhy20gCpbSetQdrStressMode (struct file *fileHndl, UINT4 mode);

INT4 hyPhy20gCpbSetQdrStressFourIntfDist (struct file *fileHndl, UINT4 dist);

INT4 hyPhy20gCpbSetRdPortThresh(struct file *fileHndl, UINT4 chnlId, 
                                UINT4 cbrMode);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 hyPhy20gCpbCreateCtxt(struct file *fileHndl);

INT4 hyPhy20gCpbDestroyCtxt(struct file *fileHndl);

INT4 cpbHyPhy20gCpbGetQdrEntryWeight(struct file *fileHndl, UINT4 *pValid,
                                     UINT4 *pQdrRate);

INT4 cpbHyPhy20gCpbSetQdrEntryWeight(struct file *fileHndl, UINT4 qdrRate);

INT4 cpbHyPhy20gCpbGetIntfCtxt(struct file  *fileHndl, UINT4 intfGrp,
                               UINT4 *pValid, UINT4 *pIntfGrpNum,
                               UINT4 *pEnable, UINT4 *pNumWrChnls,
                               UINT4 *pTotalWrRate, UINT4 *pAvailWrRate,
                               UINT4 *pNumRdChnls, UINT4 *pTotalRdRate,
                               UINT4 *pAvailRdRate);

INT4 cpbHyPhy20gCpbGetQdrRamCtxt(struct file *fileHndl, UINT4 *pValid,
                                 UINT4 *pRamType, UINT4 *pBlkSize,
                                 UINT4 *pMinFifoSize, UINT4 *pMaxFifoSize,
                                 UINT4 *pNumFreeBlk, UINT4 *pNumFifo,
                                 UINT4 *pCalEntryRate);
UINT4 cpbHyPhy20gCpbGetCtxtSize(struct file *fileHndl);
INT4 cpbHyPhy20gCpbExtractCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 cpbHyPhy20gCpbRestoreCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pParsedSize);
INT4 cpbHyPhy20gCpbAuditCtxt(struct file *fileHndl);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_CPB_H */
/* end of file */
