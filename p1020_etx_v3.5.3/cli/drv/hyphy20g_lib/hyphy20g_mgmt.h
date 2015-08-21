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
**  MODULE      : HyPHY 20G SGMII Subsystem Header File
**
**  FILE        :  hyphy20g_mgmt.h
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

#ifndef _HYPHY20G_MGMT_H
#define _HYPHY20G_MGMT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_lsci.h"
#include "hyphy20g_hostmsg.h"

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/
#define HYPHY20G_SGMII_ENET_FEGE_RXMAXFRMSIZE_UPPERLT       0xFFFB
#define HYPHY20G_SGMII_ENET_FEGE_TXMAXFRMSIZE_UPPERLT       0xFFFC
#define HYPHY20G_SGMII_ENET_FEGE_TXMINFRMSIZE_LOWERLT       0x16

/******************************************************************************/
/* MANUAL DEFINES                                                             */
/******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/* Register 49040A0H: MII AUTO-NEGOTIATION ADVERTISEMENT                      */
/*----------------------------------------------------------------------------*/
/* AN_ABLTY_ADV[15:0] has two definitions based on mode of operation          */

/*----------------------------------------------------------------------------*/
/* Register 49040A0H: MII AUTO-NEGOTIATION ADVERTISEMENT (non-SGMII mode)     */
/*----------------------------------------------------------------------------*/
/* bit 15   : NEXT_PAGE                                                       */
/* bit 14   : ACK                                                             */
/* bit 13-12: RF[2:1]                                                         */
/* bit 8-7  : PS[2:1]                                                         */
/* bit 5    : FULL_DUPLEX                                                     */
/*----------------------------------------------------------------------------*/
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_NEXT_PAGE_OFF      15
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_NEXT_PAGE_MSK       (0x0001 << 15)
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_ACK_OFF            14
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_ACK_MSK             (0x0001 << 14)
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_RF_OFF             12
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_RF_MSK              (0x0003 << 12)
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_PS_OFF              7
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_PS_MSK              (0x0003 << 7)
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_FULL_DUPLEX_MSK     5
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_FULL_DUPLEX_OFF     (0x0001 << 5)


/*----------------------------------------------------------------------------*/
/* Register 49040A0H: MII AUTO-NEGOTIATION ADVERTISEMENT (SGMII mode)         */
/*----------------------------------------------------------------------------*/
/* bit 0    : BIT0                                                            */
/*----------------------------------------------------------------------------*/
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_BIT0_OFF              0
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_BIT0_MSK              (0x0001 << 0)  

/*----------------------------------------------------------------------------*/
/* Register 49040A0H: MII AUTO-NEGOTIATION LINK PARTNER BASE PAGE ABILITY     */
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
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_LINK_STATUS_OFF     15
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_LINK_STATUS_MSK      (0x0001 << 15)
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_RSVD_0_OFF          13
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_RSVD_0_MSK           (0x0001 << 13) 
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_DUPLEX_MODE_OFF     12
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_DUPLEX_MODE_MSK      (0x0001 << 12)
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_SPEED_OFF           10
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_SPEED_MSK            (0x0003 << 10)
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_RSVD_1_OFF           0
#define HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_RSVD_1_MSK           (0x0001 << 0)  

/******************************************************************************/
/* TEMPORARY DEFINES                                                          */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/


/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gMgmtPortInit(struct file *fileHndl, UINT4 enable);

INT4 hyPhy20gMgmtPortDataPathEnable(struct file *fileHndl, UINT4 dir,
                                    UINT4 enable);

INT4 hyPhy20gMgmPortFeGeModeCfg(struct file *fileHndl, UINT4 mode);

INT4 hyPhy20gMgmtPortGeAnNextPageCfg(struct file *fileHndl, UINT4 nextPg,
                                     UINT4 msgType, UINT4 ack2, UINT4 msgCode);

INT4 hyPhy20gMgmtPortSgmiiAnBpAbilityCompare(struct file *fileHndl,
                                             UINT4 *mmCode);
                                             
INT4 hyPhy20gMgmtPortFeGeTmacCfg1(struct file *fileHndl,
                                  UINT4 crcCkEn, UINT4 padAddEn,
                                  UINT4 crcAddEn, UINT4 minFrmCkEn,
                                  UINT4 truncEn, UINT4 maxFrmCkEn,
                                  UINT4 ibufHthrEn, UINT4 xonEn, 
                                  UINT4 rmacPauseEn, UINT4 ebufErrEn);

INT4 hyPhy20gMgmtPortFeGeTmacCfg2(struct file *fileHndl,
                                  UINT4 sfdEn, UINT4 pambLen);

INT4 hyPhy20gMgmtPortFeGeTxIpgCfg(struct file *fileHndl, UINT4 txIPG1,
                                  UINT4 txIPG2);

INT4 hyPhy20gMgmtPortFeGeRmacCfg1(struct file *fileHndl,
                                  UINT4 preambleCkEn, UINT4 tagAwareEn,
                                  UINT4 inrangeDisEn, UINT4 inrangeCk,
                                  UINT4 maxFrmTrnEn, UINT4 maxFrmDisEn,
                                  UINT4 maxFrmCkEn, UINT4 minFrmDisEn,
                                  UINT4 minFrmCkEn, UINT4 crcDisEn,
                                  UINT4 crcCkEn);

INT4 hyPhy20gMgmtPortFeGeRmacCfg2(struct file *fileHndl,
                                  UINT4 interPktL1En,
                                  UINT4 padStripEn, UINT4 crcStripEn,
                                  UINT4 carrMonEn, UINT4 pauseFrmEn,
                                  UINT4 pauseFrmDropEn, UINT4 ctlFrmDropEn,
                                  UINT4 desAddrDropEn);

INT4 hyPhy20gMgmtPortFeGeSetStationAddr(struct file *fileHndl,
                                        UINT4 hiSA, UINT4 midSA, UINT4 lowSA);

INT4 hyPhy20gMgmtPortFeGeSetFrameSizes(struct file *fileHndl,
                                       UINT4 txMaxFrameSize, 
                                       UINT4 txMinFrameSize,
                                       UINT4 rxMaxFrameSize, 
                                       UINT4 rxMinFrameSize);

INT4 hyPhy20gMgmtQueueInit(struct file *fileHndl, UINT4 vlanTpid, 
                           UINT4 fcsStrip, UINT4 vlanStrip);

INT4 hyPhy20gFeGeMgmtChnlQueueInit(struct file *fileHndl, UINT4 slice, 
                                   UINT4 fcsAppend, UINT4 hiTci, UINT4 lowTci,
                                   UINT4 extQueTci);

INT4 hyPhy20gFeGeMgmtChnlExtQueuePktEnable(struct file *fileHndl, UINT4 slice, 
                                           UINT4 pktType, UINT4 vlanTpid, 
                                           UINT4 vlanTci, UINT4 enable);

INT4 hyPhy20gFeGeMgmtChnlInsQueueEnable(struct file *fileHndl, UINT4 slice, 
                                        UINT4 insQueue, UINT4 enable);

INT4 hyPhy20g10GeMgmtChnlQueueInit(struct file *fileHndl, UINT4 slice,
                                   UINT4 fcsAppend, UINT4 hiTci, UINT4 lowTci,
                                   UINT4 extQueTci, UINT4 prmbAdd, 
                                   UINT4 prmbStrip);

INT4 hyPhy20g10GeMgmtChnlExtQueuePktEnable(struct file *fileHndl, UINT4 slice, 
                                           UINT4 pktType, UINT4 vlanTpid, 
                                           UINT4 vlanTci, UINT4 enable);

INT4 hyPhy20g10GeMgmtChnlInsQueueEnable(struct file *fileHndl, UINT4 slice, 
                                        UINT4 insQueue, UINT4 enable);

#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_MGMT_H */
/* end of file */
