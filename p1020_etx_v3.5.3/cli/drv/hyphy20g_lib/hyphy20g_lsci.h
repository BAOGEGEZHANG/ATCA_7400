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
**  MODULE      : HyPHY 20G LSCI Subsystem Header File
**
**  FILE        :  hyphy20g_lsci.h
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

#ifndef _HYPHY20G_LSCI_H
#define _HYPHY20G_LSCI_H

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
#define HYPHY20G_LSCI_NUM_SLICE         16  /* Number of LSCI slices */

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/
#define HYPHY20G_LSCI_ENET_FEGE_RXMAXFRMSIZE_UPPERLT       0xFFFB
#define HYPHY20G_LSCI_ENET_FEGE_RXMINFRMSIZE_UPPERLT       0xFF
#define HYPHY20G_LSCI_ENET_FEGE_TXMAXFRMSIZE_UPPERLT       0xFFFC
#define HYPHY20G_LSCI_ENET_FEGE_TXMINFRMSIZE_LOWERLT       0x16

/******************************************************************************/
/* MANUAL DEFINES                                                             */
/******************************************************************************/
/*----------------------------------------------------------------------------*/
/* Register 48840A0H: MII AUTO-NEGOTIATION ADVERTISEMENT                      */
/*----------------------------------------------------------------------------*/
/* AN_ABLTY_ADV[15:0] has two definitions based on mode of operation          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Register 48840A0H: MII AUTO-NEGOTIATION ADVERTISEMENT (non-SGMII mode)     */
/*----------------------------------------------------------------------------*/
/* bit 15   : NEXT_PAGE                                                       */
/* bit 14   : ACK                                                             */
/* bit 13-12: RF[2:1]                                                         */
/* bit 8-7  : PS[2:1]                                                         */
/* bit 6    : HALF_DUPLEX                                                     */
/* bit 5    : FULL_DUPLEX                                                     */
/*----------------------------------------------------------------------------*/
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_NEXT_PAGE_OFF       15             
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_NEXT_PAGE_MSK        (0x0001 << 15)
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_ACK_OFF             14             
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_ACK_MSK              (0x0001 << 14)
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_RF_OFF              12             
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_RF_MSK               (0x0003 << 12)
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_PS_OFF               7             
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_PS_MSK               (0x0003 << 7) 
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_HALF_DUPLEX_OFF      6             
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_HALF_DUPLEX_MSK      (0x0001 << 6) 
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_FULL_DUPLEX_OFF      5             
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_FULL_DUPLEX_MSK      (0x0001 << 5) 


/*----------------------------------------------------------------------------*/
/* Register 48840A0H: MII AUTO-NEGOTIATION ADVERTISEMENT (SGMII mode)         */
/*----------------------------------------------------------------------------*/
/* bit 0    : BIT0                                                            */
/*----------------------------------------------------------------------------*/
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_BIT0_OFF              0
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_BIT0_MSK              (0x0001 << 0)  

/*----------------------------------------------------------------------------*/
/* Register 48840A4H: MII AUTO-NEGOTIATION LINK PARTNER BASE PAGE ABILITY     */
/*----------------------------------------------------------------------------*/
/* AN_LP_ABLTY[15:0] has two definitions based on mode of operation,          */
/* only definition in SGMII mode is presented and used                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Register 48840A4H: MII AUTO-NEGOTIATION LINK PARTNER BASE PAGE ABILITY     */
/*                    (SGMII mode)                                            */
/*----------------------------------------------------------------------------*/
/* bit 15   : LINK_STATUS                                                     */
/* bit 13   : RSVD_0                                                          */
/* bit 12   : DUPLEX_MODE                                                     */
/* bit 11-10: SPEED                                                           */
/* bit 0    : RSVD_1                                                          */
/*----------------------------------------------------------------------------*/
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_LINK_STATUS_OFF     15
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_LINK_STATUS_MSK      (0x0001 << 15)
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_RSVD_0_OFF          13
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_RSVD_0_MSK           (0x0001 << 13) 
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_DUPLEX_MODE_OFF     12
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_DUPLEX_MODE_MSK      (0x0001 << 12)
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_SPEED_OFF           10
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_SPEED_MSK            (0x0003 << 10)
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_RSVD_1_OFF           0
#define HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_RSVD_1_MSK           (0x0001 << 0)  

/******************************************************************************/
/* TEMPORARY DEFINES                                                          */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/

typedef enum {
    /* Low-Speed Client Interface Traffics */
    LSCI_MAC_TEMINATION           = 1, /* MAC termination traffic */
    LSCI_GE_GFPT_MAPPING          = 2, /* GE to GFP-T Mapping traffic */
    LSCI_FC_4G                    = 3, /* FC 4G and lower rate traffic */
    LSCI_ESCON                    = 4, /* ESCON 8B/10B encoded traffic */
    LSCI_BYTEALIGED_CBR_STS       = 5, /* STS-48/12/3 asynchronous transparent */
    LSCI_BYTEALIGED_CBR           = 6, /* Other byte-aligned CBR */
    LSCI_8B10B_TX_RD_INV_EN_CBR  = 7, /* 8B/10B encoded CBR traffic TX_RD_INV_EN*/
    LSCI_8B10B_TX_RD_INV_DIS_CBR = 8, /* 8B/10B encoded CBR traffic TX_RD_INV_DIS*/    
    LSCI_TRAFFIC_MAX              = 8  /* maximun LSCI traffic number */
} eHYPHY20G_LSCI_TRAFFIC_TYPES;

typedef enum {
    /* MAC terminated traffics */

    /* GE MAC termination mode - with auto-negotiation enabled, 
       with base page exchange only */
    LSCI_GE_MAC_TEMINATION        = 0, 
    /* FE MAC termination traffic */    
    LSCI_FE_MAC_TEMINATION        = 1, 
    /* SGMII GE MAC termination traffic, with auto 
       Acknowledge in auto-negotiation processing */
    LSCI_SGMII_GE_MAC_TEMINATION  = 2, 
    /* SGMII FE MAC termination traffic, with auto 
       Acknowledge in auto-negotiation processing */    
    LSCI_SGMII_FE_MAC_TEMINATION  = 3, 
    /* GE MAC termination mode - with auto-negotiation disabled */
    LSCI_GE_MAC_TEMINATION_AN_DISABLE   = 4, 
    /* GE MAC termination mode - with auto-negotiation enabled, 
       with next page exchange */
    LSCI_GE_MAC_TEMINATION_AN_NEXTPG    = 5,
    /* SGMII GE MAC termination traffic, with manual 
       Acknowledge in auto-negotiation processing */ 
    LSCI_SGMII_GE_MAC_TEMINATION_MANACK = 6, 
    /* SGMII FE MAC termination traffic, with manual 
       Acknowledge in auto-negotiation processing */    
    LSCI_SGMII_FE_MAC_TEMINATION_MANACK = 7 
} eHYPHY20G_LSCI_MAC_TERMINATED_TYPES;

typedef enum {
    /* Refer to hyPhy20gLsciSgmiiAnBpAbilityCompare function 
       description for each defintion in detail */
    HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_MATCH             = 0,
    HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_NOT_DEFINED       = (0x0001 << 0),
    HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_SPEED_MM          = (0x0001 << 1),
    HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_DPLXMODE_MM       = (0x0001 << 2),
    HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_LINKDOWN          = (0x0001 << 3)
} eHYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_COMPARE_RESULTS;




/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gLsciInit(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hyPhy20gLsciDataPathEnable(struct file *fileHndl, UINT4 slice,
                                UINT4 dir, UINT4 enable);

INT4 hyPhy20gLsciFeGeModeCfg(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hyPhy20gLsciGeAnNextPageCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 nextPg, UINT4 msgType, UINT4 ack2,
                                 UINT4 msgCode);

INT4 hyPhy20gLsciSgmiiAnBpAbilityCompare(struct file *fileHndl, UINT4 slice, 
                                         UINT4 *mmCode);

INT4 hyPhy20gLsciFeGeTmacCfg1(struct file *fileHndl, UINT4 slice,
                              UINT4 crcCkEn, UINT4 padAddEn,
                              UINT4 crcAddEn, UINT4 minFrmCkEn,
                              UINT4 truncEn, UINT4 maxFrmCkEn,
                              UINT4 ibufHthrEn, UINT4 xonEn, UINT4 rmacPauseEn,
                              UINT4 ebufErrEn);

INT4 hyPhy20gLsciFeGeTmacCfg2(struct file *fileHndl, UINT4 slice,
                              UINT4 sfdEn, UINT4 pambLen);

INT4 hyPhy20gLsciFeGeTxIpgCfg(struct file *fileHndl, UINT4 slice,
                              UINT4 txIPG1, UINT4 txIPG2);

INT4 hyPhy20gLsciFeGeRmacCfg1(struct file *fileHndl, UINT4 slice,
                              UINT4 preambleCkEn, UINT4 tagAwareEn,
                              UINT4 inrangeDisEn, UINT4 inrangeCk,
                              UINT4 maxFrmTrnEn, UINT4 maxFrmDisEn,
                              UINT4 maxFrmCkEn, UINT4 minFrmDisEn,
                              UINT4 minFrmCkEn, UINT4 crcDisEn,
                              UINT4 crcCkEn);

INT4 hyPhy20gLsciFeGeRmacCfg2(struct file *fileHndl, UINT4 slice,
                              UINT4 interPktL1En,
                              UINT4 padStripEn, UINT4 crcStripEn,
                              UINT4 carrMonEn, UINT4 pauseFrmEn,
                              UINT4 pauseFrmDropEn, UINT4 ctlFrmDropEn,
                              UINT4 desAddrDropEn);

INT4 hyPhy20gLsciFeGeSetStationAddr(struct file *fileHndl, UINT4 slice,
                                    UINT4 hiSA, UINT4 midSA, UINT4 lowSA);

INT4 hyPhy20gLsciFeGeSetFrameSizes(struct file *fileHndl, UINT4 slice,
                                   UINT4 txMaxFrameSize, UINT4 txMinFrameSize,
                                   UINT4 rxMaxFrameSize, UINT4 rxMinFrameSize);

INT4 hyPhy20gLsciGeGfpTModeCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 rxResyncCnt, UINT4 rxLosCnt,
                               UINT4 rxSyncCnt,
                               UINT4 txResyncCnt, UINT4 txLosCnt,
                               UINT4 txSyncCnt, UINT4 disInvEn);

INT4 hyPhy20gLsciCbrModeCfg(struct file *fileHndl, UINT4 slice);

INT4 hyPhy20gLsciCbrLcvErrInsCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 dir, UINT4 mode, UINT4 err_patt_n, 
                                 UINT4 err_patt_p);
                                                                
INT4 hyPhy20gLsciFcModeCfg(struct file *fileHndl, UINT4 slice, UINT4 osCfgMsk,
                           UINT4 arbLsbMsk, UINT4 arbLsbVal, UINT4 rxAlignMd, 
                           UINT4 txAlignMd);

INT4 hyPhy20gLsciFcLcvErrInsCfg(struct file *fileHndl, UINT4 slice, UINT4 dir, 
                                UINT4 mode, UINT4 err_patt_n, UINT4 err_patt_p);
                              
INT4 hyPhy20gLsciEsconModeCfg(struct file *fileHndl, UINT4 slice);

INT4 hyPhy20gLsciEsconLcvErrInsCfg(struct file *fileHndl, UINT4 slice, 
                                   UINT4 dir, UINT4 mode, UINT4 err_patt_n, 
                                   UINT4 err_patt_p);
                                                                    
/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 hyPhy20gLsciCreateCtxt(struct file *fileHndl, UINT4 slice, UINT4 traffic);

INT4 hyPhy20gLsciDestroyCtxt(struct file *fileHndl, UINT4 slice);

INT4 lsciHyPhy20gLsciCtxtGetMode(struct file *fileHndl, UINT4 slice,
                                 UINT4 *pMode);

INT4 lsciHyPhy20gLsciRxFcDlolbIntPer(struct file *fileHndl, UINT4 slice, 
                                     UINT4 period, UINT4 frequency, 
                                     UINT4 busWidth);
                                     
INT4 lsciHyPhy20gLsciTxFcDlolbIntPer(struct file *fileHndl, UINT4 slice, 
                                     UINT4 period); 
                                     
INT4 lsciHyPhy20gLsciGeGfpTPattUpdate(struct file *fileHndl, UINT4 slice,
                                      UINT4 type, UINT4 pattern);                                                                          
UINT4 lsciHyPhy20gLsciGetCtxtSize(struct file *fileHndl);
INT4 lsciHyPhy20gLsciExtractCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 lsciHyPhy20gLsciRestoreCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pParsedSize);
INT4 lsciHyPhy20gLsciAuditCtxt (struct file *fileHndl);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_LSCI_H */
/* end of file */
