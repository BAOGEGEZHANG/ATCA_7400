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
**  FILE        :  hyphy20g_hostmsg.h
**
**  $Date: 2011-11-15 16:38:16 +0800 (Tue, 15 Nov 2011) $
**
**  $Revision: 17059 $
**
**  DESCRIPTION :  Contains hardware specific definitions
**
**  NOTES       :
**
*******************************************************************************/

#ifndef _HYPHY20G_HOSTMSG_H
#define _HYPHY20G_HOSTMSG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
//#include "math.h"

/******************************************************************************/
/* COMMON MACRO DEFINITIONS                                                   */
/******************************************************************************/
#define HYPHY20G_HOSTMSG_IBND_MAILBOX_ADDR           0x3FE00
#define HYPHY20G_HOSTMSG_OBND_MAILBOX_ADDR           0x3FD00
#define HYPHY20G_HOSTMSG_MAX_MAILBOX_SIZE            64                 /*Max length of inbox and outbox mailbox (32bit word).*/
#define HYPHY20G_HOSTMSG_MAX_IBND_DOORBELL_NUM       32                 /*Max number of inbound doorbells.*/
#define HYPHY20G_HOSTMSG_MAX_OBND_DOORBELL_NUM       32                 /*Max number of outbound doorbells.*/

#define HYPHY20G_HOSTMSG_CATEGORY_CMD_MASK           0x00
#define HYPHY20G_HOSTMSG_CATEGORY_RSP_MASK           0xF0

#define HYPHY20G_HOSTMSG_NUM_HEADER_WORDS            2
/* Message header word 1 */
#define HYPHY20G_HOSTMSG_MODID_MSK                   (0xFF << 24)
#define HYPHY20G_HOSTMSG_MODID_OFF                   24
#define HYPHY20G_HOSTMSG_MSGCAT_MSK                  (0xFF << 16)
#define HYPHY20G_HOSTMSG_MSGCAT_OFF                  16
#define HYPHY20G_HOSTMSG_MSGLEN_MSK                  (0xFFFF << 0)
#define HYPHY20G_HOSTMSG_MSGLEN_OFF                  0
/* Message header word 2 */
#define HYPHY20G_HOSTMSG_MSGSUBTYP_MSK               (0xFF << 24)
#define HYPHY20G_HOSTMSG_MSGSUBTYP_OFF               24
#define HYPHY20G_HOSTMSG_MSGCTRL_MSK                 (0xFF << 16)
#define HYPHY20G_HOSTMSG_MSGCTRL_OFF                 16
#define HYPHY20G_HOSTMSG_MSGSEQNUM_MSK               (0xFFFF << 0)
#define HYPHY20G_HOSTMSG_MSGSEQNUM_OFF               0

#define HYPHY20G_HOSTMSG_MIN_MSG_LEN                 2
#define HYPHY20G_HOSTMSG_MAX_MSG_LEN                 HYPHY20G_HOSTMSG_MAX_MAILBOX_SIZE

#define HYPHY20G_HOSTMSG_5000_USEC                   5000
#define HYPHY20G_HOSTMSG_60_MSEC                     60
#define HYPHY20G_HOSTMSG_500_MSEC                    500
#define HYPHY20G_HOSTMSG_10000_MSEC                  10000
#define HYPHY20G_HOSTMSG_DOORBELL_POLL_TIME          HYPHY20G_HOSTMSG_500_MSEC
#define HYPHY20G_USEC_PER_MSEC                       1000

#define HYPHY20G_HOSTMSG_NUMWORDS_SYMBOL_PERIOD_EVENT_TLV                     10
#define HYPHY20G_HOSTMSG_NUMWORDS_FRAME_EVENT_TLV                              7
#define HYPHY20G_HOSTMSG_NUMWORDS_FRAME_PERIOD_EVENT_TLV                       7
#define HYPHY20G_HOSTMSG_NUMWORDS_FRAME_SECONDS_EVENT_TLV                      5
/* Message control value */
#define HYPHY20G_HOSTMSG_GET_REV                    1
#define HYPHY20G_HOSTMSG_GET_REV_BUILD             2

#define HYPHY20G_HOSTMSG_REQ_STATUS                 1
#define HYPHY20G_HOSTMSG_REQ_FIRMWARE_CHECK        2

#define HYPHY20G_HOSTMSG_FIRMWARE_ALIVE            0xABCD
#define HYPHY20G_HOSTMSG_FIRMWARE_SUPPORTED        0
#define HYPHY20G_HOSTMSG_FIRMWARE_NOT_SUPPORTED    1

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/
/*Define different message module ID*/
typedef enum {
    MODID_PTP       = 0x01,
    MODID_SYNC_E    = 0x02,
    MODID_LINKOAM   = 0x03,
    MODID_NGSERDES  = 0x04,
    MODID_CMF       = 0x05,
    MODID_PAUSEFRM  = 0x06,
    MODID_VCAT      = 0x07,
    MODID_TWI_LED   = 0x08,
    MODID_MGMT_EXT  = 0x09,
    MODID_OTN       = 0x0A,
    MODID_ENET_PMON = 0x0B,
    MODID_INFO_DBG  = 0x80,
    MODID_TEST      = 0xFF,

    MAX_MODID       = 0xFF                 /* Change this parameter as needed */
} eHYPHY20G_HOSTMSG_MODID;

/*Host Message Category Define*/
typedef enum {
    CATEGORY_CMD_CFG  = 0x01,
    CATEGORY_CMD_REQ  = 0x02,
    CATEGORY_RSP_AUTO = 0xF0,
    CATEGORY_RSP_CFG  = 0xF1,
    CATEGORY_RSP_REQ  = 0xF2,
    CATEGORY_RSP_NACK = 0xFF
} eHYPHY20G_HOSTMSG_CATEGORY;

/*Large Msg Control:basic, extend or concatenated message*/
typedef enum {
    BASIC_MSG             = 0x00,
    CONCATENATED_1ST_MSG  = 0x01,
    CONCATENATED_MID_MSG  = 0x03,
    CONCATENATED_LST_MSG  = 0x02,
    EXTEND_MSG            = 0xF0
} eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE;

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/
INT4 hyPhy20gHostMsgFwOp(struct file *fileHndl, UINT4 enable);

INT4 hyPhy20gHostMsgInit(struct file *fileHndl, UINT4 enable);

INT4 hyPhy20gHostMsgCmdCfgLnkOamCfgChnl(struct file *fileHndl, UINT4 chnlType,
                                        UINT4 chnlId, UINT4 varRetrSup,
                                        UINT4 lnkEvntSup, UINT4 lpbkSup,
                                        UINT4 uniDirSup, UINT4 mode, UINT4 rate,
                                        UINT4 pduMaxSize);

INT4 hyPhy20gHostMsgCmdReqLnkOamGetInfo(struct file *fileHndl, UINT4 chnlType,
                                        UINT4 chnlId, UINT4 *rspCtrl,
                                        UINT4 *rspLCfg, UINT4 *rspRCfg,
                                        UINT4 *rspLState, UINT4 *rspRState,
                                        UINT4 *rspLCritEvnt, UINT4 *rspRCritEvnt,
                                        UINT4 *rspMibReqCnt, UINT4 *rspMibRspCnt,
                                        UINT4 *rspRate, UINT4 *rspPduMaxSize,
                                        UINT4 *rspBranch0, UINT4 *rspLeaf0,
                                        UINT4 *rspBranch1, UINT4 *rspLeaf1,
                                        UINT4 *rspBranch2, UINT4 *rspLeaf2,
                                        UINT4 *rspBranch3, UINT4 *rspLeaf3);

INT4 hyPhy20gHostMsgCmdReqLnkOamGetRemoteMibVar(struct file *fileHndl,
                                                UINT4 chnlType, UINT4 chnlId,
                                                UINT4 *branch, UINT4 *leaf,
                                                UINT4 *width, UINT4 *valueLen,
                                                UINT4 *value);

INT4 hyPhy20gHostMsgCmdCfgLnkOamSetLpbk(struct file *fileHndl, UINT4 chnlType,
                                        UINT4 chnlId, UINT4 lpbkType,
                                        UINT4 enable);

INT4 hyPhy20gHostMsgCmdCfgLnkOamSetLocalMibVar(struct file *fileHndl,
                                               UINT4 chnlType, UINT4 chnlId,
                                               UINT4 branch, UINT4 leaf,
                                               UINT4 varInd, UINT4 varIndVal,
                                               UINT4 valueLen, UINT1 *pValue);

INT4 hyPhy20gHostMsgCmdCfgLnkOamSetCritEvntNotf(struct file *fileHndl,
                                                UINT4 chnlType,
                                                UINT4 chnlId,
                                                UINT4 critEvnt);

INT4 hyPhy20gHostMsgCmdCfgLnkOamCtrlChnl(struct file *fileHndl,
                                         UINT4 chnlType, UINT4 chnlId,
                                         UINT4 enable);

INT4 hyPhy20gHostMsgCmdCfgLnkOamLocEvntCfg(struct file *fileHndl,
                                           UINT4 chnlType, UINT4 chnlId,
                                           UINT4 symPerEvntWinMsw,
                                           UINT4 symPerEvntWinLsw,
                                           UINT4 symPerEvntThreshMsw,
                                           UINT4 symPerEvntThreshLsw,
                                           UINT4 frmEvntWin,
                                           UINT4 frmEvntThresh,
                                           UINT4 frmPerEvntWin,
                                           UINT4 frmPerEvntThresh,
                                           UINT4 frmSecSumEvntWin,
                                           UINT4 frmSecSumEvntThresh);

INT4 hyPhy20gHostMsgCmdCfgLnkOamSendMibVarReq(struct file *fileHndl,
                                              UINT4 chnlType, UINT4 chnlId,
                                              UINT4 branch, UINT4 leaf);

INT4 hyPhy20gHostMsgCmdReqLnkOamGetLocalLnkEvnt(struct file *fileHndl,
                                                UINT4 chnlType, UINT4 chnlId,
                                                UINT4 *pTLV0, UINT4 *pTLV1,
                                                UINT4 *pTLV2, UINT4 *pTLV3);

INT4 hyPhy20gHostMsgCmdReqLnkOamGetRemoteLnkEvnt(struct file *fileHndl,
                                                UINT4 chnlType, UINT4 chnlId,
                                                UINT4 *pTLV0, UINT4 *pTLV1,
                                                UINT4 *pTLV2, UINT4 *pTLV3);

INT4 hyPhy20gHostMsgCmdReqInfoGetStatus(struct file *fileHndl, UINT4 statCtrl,
                                        UINT4 *statType, UINT4 *statCode);

INT4 hyPhy20gHostMsgCmdCfgSyncECfgChnl(struct file *fileHndl, UINT4 chnlType,
                                        UINT4 chnlId, UINT4 mode, UINT4 qlSSM);

INT4 hyPhy20gHostMsgCmdCfgSyncECtrlChnl(struct file *fileHndl,
                                        UINT4 chnlType, UINT4 chnlId,
                                        UINT4 enable);

INT4 hyPhy20gHostMsgCmdReqSyncEGetSSM(struct file *fileHndl,
                                      UINT4 *pObndMsgBodyLen,
                                      UINT4 *pObndMsgBody);

INT4 hyPhy20gHostMsgCmdCfgSyncETxSSM(struct file *fileHndl, UINT4 chnlType,
                                     UINT4 chnlId, UINT4 evntFlag,
                                     UINT4 qlSSM);

INT4 hyPhy20gHostMsgCmdCfgPtpSetClkParam(struct file *fileHndl, UINT4 clkType,
                                         UINT4 twoStepFlg, UINT4 clkIdMsw,
                                         UINT4 clkIdLsw, UINT4 domainNum,
                                         UINT4 protocol, UINT4 operatingMode,
                                         UINT4 multiEnable);

INT4 hyPhy20gHostMsgCmdCfgPtpSetIrigCfg(struct file *fileHndl, UINT4 irigPort,
                                        UINT4 irigMode, UINT4 adjMode, 
                                        UINT4 enable);

INT4 hyPhy20gHostMsgCmdCfgPtpSetIPv4Param(struct file *fileHndl,
                                          UINT4 precedence, UINT4 delay,
                                          UINT4 throughput, UINT4 reliability,
                                          UINT4 ttl, UINT4 srcAddr[4], 
                                          UINT4 destAddr[4]);

INT4 hyPhy20gHostMsgCmdCfgPtpSetIPv6Param(struct file *fileHndl,
                                          UINT4 trfClass, UINT4 flowLabel,
                                          UINT4 hopLimit, UINT4 daScopeVal,
                                          UINT4 srcAddr[8], UINT4 destAddr[8]);

INT4 hyPhy20gHostMsgCmdCfgPtpSetDefaultProp(struct file *fileHndl,
                                            UINT4 clkClass, UINT4 clkAccuracy,
                                            UINT4 logVar, UINT4 priority1,
                                            UINT4 priority2, UINT4 slaveOnly);

INT4 hyPhy20gHostMsgCmdCfgPtpSetTimeProp(struct file *fileHndl, UINT4 utcOffset,
                                         UINT4 leap, UINT4 utcOffValid,
                                         UINT4 ptpTimescale,
                                         UINT4 timeTraceable,
                                         UINT4 freqTraceable, UINT4 timeSrc);

INT4 hyPhy20gHostMsgCmdCfgPtpSetAccptMstrClks(struct file *fileHndl,
                                              UINT4 enableTable,
                                              UINT4 clearTable,
                                              UINT4 numEntries,
                                              UINT4 clkIdMsw[20],
                                              UINT4 clkIdLsw[20],
                                              UINT4 portNum[20]);

INT4 hyPhy20gHostMsgCmdCfgPtpSetStartInit(struct file *fileHndl,
                                          UINT4 lsciPortInit,
                                          UINT4 hsciPortInit);

INT4 hyPhy20gHostMsgCmdReqGetMstrClkDbStatus(struct file *fileHndl,
                                             UINT4 *status,
                                             UINT4 *numEntries);

INT4 hyPhy20gHostMsgCmdReqPtpGetMstrClkDb(struct file *fileHndl,
                                          UINT4 *pRspLength, UINT4 *pMstrClkDb);

INT4 hyPhy20gHostMsgCmdCfgPtpSetParentData(struct file *fileHndl,
                                           UINT4 stepsRemoved,
                                           UINT4 parentClkIdMsw,
                                           UINT4 parentClkIdLsw,
                                           UINT4 parentPortNum,
                                           UINT4 gmPriority1, UINT4 gmPriority2,
                                           UINT4 gmClkClass, UINT4 gmClkAccu,
                                           UINT4 gmLogVar, UINT4 gmClkIdMsw,
                                           UINT4 gmClkIdLsw, 
                                           UINT4 unicastDA[6]);

INT4 hyPhy20gHostMsgCmdCfgPtpSetPortData(struct file *fileHndl,
                                         UINT4 logMinDlyReq,
                                         UINT4 logAnnounce,
                                         UINT4 announceRecTimeout,
                                         UINT4 logSync, UINT4 dlyMech,
                                         UINT4 logMinPDlyReq,
                                         UINT4 qualTimeoutN, 
                                         UINT4 syncRecTimeout,
                                         UINT4 dlyRespRecTimeout);

INT4 hyPhy20gHostMsgCmdCfgPtpSetPortState(struct file *fileHndl,
                                          UINT4 lsciState0, UINT4 lsciState1,
                                          UINT4 lsciState2, UINT4 lsciState3,
                                          UINT4 lsciState4, UINT4 lsciState5,
                                          UINT4 lsciState6, UINT4 lsciState7,
                                          UINT4 lsciState8, UINT4 lsciState9,
                                          UINT4 lsciState10, UINT4 lsciState11,
                                          UINT4 lsciState12, UINT4 lsciState13,
                                          UINT4 lsciState14, UINT4 lsciState15,
                                          UINT4 hsciState0, UINT4 hsciState1);

INT4 hyPhy20gHostMsgCmdCfgPtpSetPortEvnt(struct file *fileHndl,
                                         UINT4 chnlType, UINT4 chnlId,
                                         UINT4 enable);

INT4 hyPhy20gHostMsgCmdReqPtpGetPortState(struct file *fileHndl,
                                          UINT4 *pPortStates);

INT4 hyPhy20gHostMsgCmdCfgPtpSetPassPortParent(struct file *fileHndl,
                                               UINT4 chnlType, UINT4 chnlId,
                                               UINT4 clkIdMsw, UINT4 clkIdLsw,
                                               UINT4 portNum);

INT4 hyPhy20gHostMsgCmdCfgSerdesSetEquTap(struct file *fileHndl,
                                          UINT4 groupNum, UINT4 linkNum,
                                          UINT4 numIter);

INT4 hyPhy20gHostMsgCmdReqInfoGetVerInfo(struct file *fileHndl, UINT4 control,
                                         UINT4 *revNum, UINT4 *buildNum);

INT4 hyPhy20gHostMsgCmdCfgCsfSetLanToWanConsActTable(struct file *fileHndl,
                                                     UINT4 tableIdx,
                                                     UINT4 losMonVAct,
                                                     UINT4 rxDlolLsciMonVAct,
                                                     UINT4 rxDlolHsciMonVAct,
                                                     UINT4 rxGbeLosSyncVAct,
                                                     UINT4 rxGsup73LosSyncVAct,
                                                     UINT4 rxGsup73HberVAct,
                                                     UINT4 rxGsup73LnkFailVAct,
                                                     UINT4 rxFeLosSyncVAct,
                                                     UINT4 rxFeFiVAct,
                                                     UINT4 swAlarmVAct,
                                                     UINT4 rxFeGeLnkFailVAct,
                                                     UINT4 priority1Act,
                                                     UINT4 priority2Act,
                                                     UINT4 priority3Act,
                                                     UINT4 priority4Act,
                                                     UINT4 priority5Act,
                                                     UINT4 priority6Act);

INT4 hyPhy20gHostMsgCmdCfgCsfSetLanToWanPortMapTable(struct file *fileHndl,
                                                     UINT4 chnlType,
                                                     UINT4 chnlId,
                                                     UINT4 gfpIdx,
                                                     UINT4 gfpChnl,
                                                     UINT4 serdes,
                                                     UINT4 vcatGrp);

INT4 hyPhy20gHostMsgCmdCfgCsfSetLanToWanPortActTable(struct file *fileHndl,
                                                     UINT4 chnlType,
                                                     UINT4 chnlId,
                                                     UINT4 tableIdx,
                                                     UINT4 sendDci,
                                                     UINT4 exi, UINT4 cid,
                                                     UINT4 eHec,
                                                     UINT4 genKpAlvCmf,
                                                     UINT4 usePyld,
                                                     UINT4 genPyldFcs,
                                                     UINT4 upi,
                                                     UINT4 cmfPyld[8]);

INT4 hyPhy20gHostMsgCmdCfgCsfSetLanToWanCsfFrmGenInt(struct file *fileHndl,
                                                     UINT4 interval);

INT4 hyPhy20gHostMsgCmdCfgCsfSetC2Byte(struct file *fileHndl, UINT4 vcatGrp,
                                       UINT4 thppMsk, UINT4 sts3Msk,
                                       UINT4 sts1Msk, UINT4 pdiP, UINT4 psl);

INT4 hyPhy20gHostMsgCmdCfgCsfSetLanToWanUpdateTblNotif(struct file *fileHndl,
                                                       UINT4 hsciChnlUpdateMsk,
                                                       UINT4 lsciChnlUpdateMsk);

INT4 hyPhy20gHostMsgCmdCfgCsfSendLanToWanCmfHdlcFrm(struct file *fileHndl,
                                                    UINT4 gfpIdx, UINT4 gfpChnl,
                                                    UINT4 type, UINT4 length,
                                                    UINT4 pData[100]);

INT4 hyPhy20gHostMsgCmdReqCsfGetLanToWanConsActStat(struct file *fileHndl,
                                                    UINT4 *pHsci, UINT4 *pLsci);

INT4 hyPhy20gHostMsgCmdCfgCsfSetWanToLanMapTable(struct file *fileHndl,
                                                 UINT4 chnlType, UINT4 chnlId,
                                                 UINT4 gfpIdx, UINT4 gfpChnl,
                                                 UINT4 action, UINT4 upi);

INT4 hyPhy20gHostMsgCmdCfgCsfSetWanToLanUpdateTblNotif(struct file *fileHndl,
                                                       UINT4 gfpIdx,
                                                       UINT4 gfpChnl);

INT4 hyPhy20gHostMsgCmdCfgCsfSetWanToLanSetDefCondStopParam(struct file *fileHndl,
                                                            UINT4 interval);

INT4 hyPhy20gHostMsgCmdReqCsfGetWanToLanCmfInd(struct file *fileHndl,
                                               UINT4 gfpIdx, UINT4 *pUpiGfp);

INT4 hyPhy20gHostMsgCmdReqCsfGetWanToLanPropCmf(struct file *fileHndl,
                                                UINT4 gfpIdx, UINT4 gfpChnl,
                                                UINT4 *pGfpIdx, UINT4 *pGfpChnl,
                                                UINT4 *pUpiType, UINT4 *pLength,
                                                UINT4 *pData);

INT4 hyPhy20gHostMsgCmdCfgCsfSendLanToWanOneShotCmf(struct file *fileHndl,
                                                    UINT4 gfpIdx, UINT4 gfpChnl,
                                                    UINT4 upi, UINT4 genPyldFcs,
                                                    UINT4 cmfPyld[8]);

INT4 hyPhy20gHostMsgCmdCfgCsfSetLanToWanSoftAlarmForce(struct file *fileHndl,
                                                       UINT4 chnlType,
                                                       UINT4 chnlId,
                                                       UINT4 enable);

INT4 hyPhy20gHostMsgCmdCfgVcatLcasSetDefectCollectCtrl(struct file *fileHndl,
                                                       UINT4 defectColCtrl);

INT4 hyPhy20gHostMsgCmdCfgVcatLcasSetStateMachineCtrl(struct file *fileHndl,
                                                      UINT4 sts48Num,
                                                      UINT4 sts12Num,
                                                      UINT4 sts3Num,
                                                      UINT4 sts1Num,
                                                      UINT4 smCtrl);

INT4 hyPhy20gHostMsgCmdReqVcatLcasGetDefectIntInd(struct file *fileHndl,
                                                  UINT4 reqCtrl,
                                                  UINT4 *pIntData);

INT4 hyPhy20gHostMsgCmdReqVcatLcasGetDefectIntStatus(struct file *fileHndl,
                                                     UINT4 *pIntData);

INT4 hyPhy20gHostMsgCmdCfgVcatLcasSetForceState(struct file *fileHndl,
                                                UINT4 sts48Num, UINT4 sts12Num,
                                                UINT4 sts3Num, UINT4 sts1Num,
                                                UINT4 defectType, UINT4 enable,
                                                UINT4 state);

INT4 hyPhy20gHostMsgCmdReqVcatLcasGetDefectState(struct file *fileHndl,
                                                 UINT4 reqCtrl, UINT4 *pState);

INT4 hyPhy20gHostMsgCmdReqVcatLcasGetLinkStatus(struct file *fileHndl,
                                                UINT4 *pStatus);

INT4 hyPhy20gHostMsgCmdCfgVcatLcasSetHeartbeat(struct file *fileHndl);

INT4 hyPhy20gHostMsgCmdCfgLedSetTwiAddrSetup(struct file *fileHndl,
                                             UINT4 devAddr, UINT4 memAddr);

INT4 hyPhy20gHostMsgCmdCfgLedSetMonCtrl(struct file *fileHndl,
                                        UINT4 chnlType, UINT4 chnlId,
                                        UINT4 clientType, UINT4 enable);

INT4 hyPhy20gHostMsgCmdCfgLedSetHsciMap(struct file *fileHndl, UINT4 map);

INT4 hyPhy20gHostMsgCmdCfgEnetMpFilterSetEnetMpFilterSetup(struct file *fileHndl,
                                                           UINT4 chnlType,
                                                           UINT4 chnlId,
                                                           UINT4 pktType,
                                                           UINT4 vlanTpid,
                                                           UINT4 vlanTci,
                                                           UINT4 enable);

INT4 hyPhy20gHostMsgCmdCfgEnetMpFilterSetTestPattParam(struct file *fileHndl,
                                                       UINT4 macAddr[6],
                                                       UINT4 pattType,
                                                       UINT4 pattData,
                                                       UINT4 pyldSize);

INT4 hyPhy20gHostMsgCmdCfgEnetMpFilterSetTestPattCtrl(struct file *fileHndl,
                                                      UINT4 chnlType,
                                                      UINT4 chnlId,
                                                      UINT4 enable);

INT4 hyPhy20gHostMsgCmdReqEnetMpFilterGetTestPktStatus(struct file *fileHndl,
                                                       UINT4 *pPattType,
                                                       UINT4 *pPattData,
                                                       UINT4 *pPyldSize,
                                                       UINT4 *pChnlType,
                                                       UINT4 *pChnlId,
                                                       UINT4 *pEnable,
                                                       UINT4 *pNumTxPkt,
                                                       UINT4 *pNumRxPkt,
                                                       UINT4 *pNumRxErrPkt);

INT4 hyPhy20gHostMsgCmdCfgPmonEnetStatColCtrl(struct file *fileHndl,
                                              UINT4 chnlType, UINT4 chnlId,
                                              UINT4 enable);

INT4 hyPhy20gHostMsgCmdCfgPmonReqMstatxCtrl(struct file *fileHndl,
                                            UINT4 chnlType, UINT4 chnlId);

INT4 hyPhy20gHostMsgCmdCfgPmonReleaseMstatxCtrl(struct file *fileHndl,
                                                UINT4 chnlType, UINT4 chnlId);

INT4 hyPhy20gHostMsgCmdReqGetLsciL1RppPmonCntr(struct file *fileHndl,
                                               UINT4 chnlId, UINT4 *pCount,
                                               UINT4 *pOverflowI);

INT4 hyPhy20gHostMsgCmdReqGetHsciGsup43PmonCntr(struct file *fileHndl,
                                                UINT4 chnlId, UINT4 *pCount0,
                                                UINT4 *pCount1, UINT4 *pCount2,
                                                UINT4 *pCount3, UINT4 *pCount4,
                                                UINT4 *pCount5, UINT4 *pCount6,
                                                UINT4 *pCount7, UINT4 *pCount8,
                                                UINT4 *pCount9, UINT4 *pCount10,
                                                UINT4 *pCount11,
                                                UINT4 *pOverflowI);

INT4 hyPhy20gHostMsgCmdReqCsfGetWanToLanConsActStat(struct file *fileHndl,
                                                    UINT4 chnlType, 
                                                    UINT4 chnlId, 
                                                    UINT4 *pChnlType,
                                                    UINT4 *pChnlId,
                                                    UINT4 *pForceCtrl,
                                                    UINT4 *pActionNum);

INT4 hyPhy20gHostMsgCmdCfgCsfSetWanToLanSoftConseqForce(struct file *fileHndl,
                                                    UINT4 chnlType,
                                                    UINT4 chnlId,
                                                    UINT4 forceCtrl);

INT4 hyPhy20gHostMsgCmdReqPtpGetPortRxMsgCnts(struct file *fileHndl,
                                              UINT4 chnlType, UINT4 chnlId,
                                              UINT4 *pChnlType, UINT4 *pChnlId,
                                              UINT4 *pCnts);
                                              
INT4 hyPhy20gHostMsgCmdCfgPtpSetUnicastMasterParam(struct file *fileHndl,
                                                   UINT4 logIntMsgPeriod[3], 
                                                   UINT4 durationField[3]);
                                                   
INT4 hyPhy20gHostMsgCmdCfgPtpSetUnicastReq(struct file *fileHndl, 
                                           UINT4 chnlType, UINT4 chnlId,
                                           UINT4 msgType,
                                           UINT4 logIntMsgPeriod[3], 
                                           UINT4 durationField[3],
                                           UINT4 continuous, UINT4 indTlv);

INT4 hyPhy20gHostMsgCmdCfgPtpSetUnicastCancel(struct file *fileHndl,
                                              UINT4 chnlType, UINT4 chnlId,
                                              UINT4 msgType);

INT4 hyPhy20gHostMsgCmdReqPtpGetUnicastSlvStat(struct file *fileHndl,
                                               UINT4 *pChnlType, UINT4 *pChnlId,
                                               UINT4 *pLogIntMsgPeriod,
                                               UINT4 *pDurationField);

INT4 hyPhy20gHostMsgCmdReqPtpGetPathDelay(struct file *fileHndl, UINT4 chnlType,
                                          UINT4 chnlId, UINT4 *pathDelay);

INT4 hyPhy20gHostMsgCmdCfgPtpSetPortAsymmetry(struct file *fileHndl,
                                              UINT4 chnlType, UINT4 chnlId,
                                              UINT4 delayAsymmetry,
                                              UINT4 rxStaticOffset,
                                              UINT4 txStaticOffset);

INT4 hyPhy20gHostMsgCmdCfgPtpSetTcParam(struct file *fileHndl, UINT4 clkIdMsw, 
                                        UINT4 clkIdLsw, UINT4 portNum, 
                                        UINT4 syntEnable, UINT4 lsciChnlEnable,
                                        UINT4 hsciChnlEnable);

INT4 hyPhy20gHostMsgCmdCfgPtpSetTcPortParam(struct file *fileHndl,
                                            UINT4 chnlType, UINT4 chnlId,
                                            UINT4 logMinPDlyReq);

INT4 hyPhy20gHostMsgCmdCfgPtpSetPartialPtpParam(struct file *fileHndl, 
                                                UINT4 exterInterface, 
                                                UINT4 vlanTci, 
                                                UINT4 rxTimestamp, 
                                                UINT4 eventFeedback, 
                                                UINT4 hostMacAddr[6],
                                                UINT4 mipsMacAddr[6],
                                                UINT4 vlanPresent);

INT4 hyPhy20gHostMsgCmdReqCsfGetRxFefiStat(struct file *fileHndl,
                                           UINT4 lsciChStat, UINT4 *pLsciChStat,
                                           UINT4 *pLsciFefiStat);

INT4 hyPhy20gHostMsgCmdCfgCsfSetWanToLanCmfRetrOption(struct file *fileHndl,
                                                      UINT4 mode);

INT4 hyPhy20gHostMsgCmdReqCsfGetWanToLanOneShotKeepAliveCmfMultiCh(
                                         struct file *fileHndl, UINT4 chnlCnt,
                                         UINT4 chnlSlc[15], UINT4 chnlNum[15],
                                         UINT4 *pChnlCnt, UINT4 *pChnlData);

INT4 hyPhy20gHostMsgChkObndDbelMskStatus(struct file *fileHndl,
                                                UINT4 doorBellNum,
                                                UINT4 status);

INT4 hyPhy20gHostMsgPollObndDbelMskStatus(struct file *fileHndl,
                                                 UINT4 doorBellNum,
                                                 UINT4 status, UINT4 msDelay);
												 
INT4 hyPhy20gHostMsgClrObndDbel(struct file *fileHndl, UINT4 doorBellNum);  

INT4 hyPhy20gHostMsgCmdCfgPmonCtrlEnbl(struct file *fileHndl, UINT4 enblPmon);

INT4 hyPhy20gHostMsgCmdReqPmonGetAlarms(struct file *fileHndl, 
                                        UINT4 *pRspMsgSize, UINT4 *pRspMsgAddr);

INT4 hyPhy20gHostMsgCmdReqPmonGetCnts(struct file *fileHndl, 
                                      UINT4 *pRspMsgSize, UINT4 *pRspMsgAddr);

INT4 hyPhy20gHostMsgCmdCfgVcatLcasSetMemberToGrpMpTbl(struct file *fileHndl,
                                                      UINT4 vcatGrp,
                                                      UINT4 isLcasMember[6]);

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 hyPhy20gHostMsgCreateCtxt(struct file *fileHndl);

INT4 hyPhy20gHostMsgDestroyCtxt(struct file *fileHndl);

INT4 hostMsgHyPhy20gHostMsgCmdCfgOtnSetInitState(struct file *fileHndl);
UINT4 hostMsgHyPhy20gHostMsgGetCtxtSize(struct file *fileHndl);
INT4 hostMsgHyPhy20gHostMsgExtractCtxt(struct file *fileHndl, 
                                    void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 hostMsgHyPhy20gHostMsgRestoreCtxt(struct file *fileHndl, 
                                    void *pCtxtBuf, UINT4 *pParsedSize);
INT4 hostMsgHyPhy20gHostMsgAuditCtxt (struct file *fileHndl);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_HOSTMSG_H */
/* end of file */
