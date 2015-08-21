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
**
**  MODULE      : HyPHY 20G Configuration Code
**
**  FILE        : hyphy20g_hostmsg.c
**
**  $Date: 2011-11-24 15:02:48 +0800 (Thu, 24 Nov 2011) $
**
**  $Revision: 17389 $
**
**  DESCRIPTION : This file contains all the firmware interface functions.
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_hostmsg.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* MACRO: CHECK_FW_OP_ENABLED                                                 */
/******************************************************************************/
#define CHECK_FW_OP_ENABLED(fileHndl)                                   \
do {                                                                    \
        sHYPHY20G_CTXT *pDevCtxt;                                       \
                                                                        \
        pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);                  \
        if (pDevCtxt == NULL) {                                         \
            return HYPHY20G_ERR_NULL_PTR_PASSED;                        \
        }                                                               \
                                                                        \
        if (pDevCtxt->gData.hostMsgFwOp == 0) {                         \
            return HYPHY20G_SUCCESS;                                    \
        }                                                               \
}while(0)                                                               \

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgGetCtxt(struct file *fileHndl, UINT4 *pValid,
                                   UINT4 *pSeqCount);
INT4 hostMsgHyPhy20gHostMsgSetMskCtxt(struct file *fileHndl, UINT4 valid,
                                      UINT4 seqCount, UINT4 setMsk);
INT4 hostMsgHyPhy20gHostMsgIbndMsgHdr(struct file *fileHndl, UINT4 modID,
                                      UINT4 msgCat, UINT4 msgLen,
                                      UINT4 msgSubtype, UINT4 msgCtrl,
                                      UINT4 seqNum, UINT4 *pMsgHdr);
INT4 hostMsgHyPhy20gHostMsgSendIbndMsg(struct file *fileHndl, UINT4 msgLen,
                                       UINT4 *pMsg);
INT4 hostMsgHyPhy20gHostMsgGetObndMsgHdr(struct file *fileHndl, UINT4 *modID,
                                         UINT4 *msgCat, UINT4 *msgLen,
                                         UINT4 *msgSubtype, UINT4 *msgCtrl,
                                         UINT4 *seqNum);
INT4 hostMsgHyPhy20gHostMsgGetObndMsgBody(struct file *fileHndl, UINT4 msgLen,
                                          UINT4 *msg);
INT4 hostMsgHyPhy20gHostMsgChkObndRspCfg(struct file *fileHndl, UINT4 modID,
                                         UINT4 subtype, UINT4 seqNum);
INT4 hostMsgHyPhy20gHostMsgChkObndRspReqHdr(struct file *fileHndl, UINT4 modID,
                                            UINT4 subtype, UINT4 seqNum,
                                            UINT4 *rspLen, UINT4 *rspCtrl);
INT4 hostMsgHyPhy20gHostMsgChkIbndDbelMskStatus(struct file *fileHndl,
                                                UINT4 doorBellNum,
                                                UINT4 status);
INT4 hostMsgHyPhy20gHostMsgSetIbndDbel(struct file *fileHndl,
                                       UINT4 doorBellNum);
INT4 hostMsgHyPhy20gHostMsgChkObndDbelMskStatus(struct file *fileHndl,
                                                UINT4 doorBellNum,
                                                UINT4 status);
INT4 hostMsgHyPhy20gHostMsgPollObndDbelMskStatus(struct file *fileHndl,
                                                 UINT4 doorBellNum,
                                                 UINT4 status, UINT4 msDelay);
INT4 hostMsgHyPhy20gHostMsgClrObndDbel(struct file *fileHndl,
                                       UINT4 doorBellNum);
INT4 hostMsgHyPhy20gHostMsgNextAvailSeqNum(struct file *fileHndl,
                                           UINT4 *pvalue);
INT4 hostMsgHyPhy20gHostMsgCurrSeqNum(struct file *fileHndl, UINT4 *pvalue);
INT4 hostMsgHyPhy20gHostMsgGetExtMsg(struct file *fileHndl, UINT4 msgLen,
                                     UINT4 msgAddr, UINT4 *msg);
INT4 hostMsgHyPhy20gHostMsgCmdCfgLnkOamCfgChnl(struct file *fileHndl,
                                               UINT4 chnlCnt,
                                               UINT4 *pChnlType,
                                               UINT4 *pChnlId,
                                               UINT4 *pVarRetrSup,
                                               UINT4 *pLnkEvntSup,
                                               UINT4 *pLbkSup,
                                               UINT4 *pUniDirSup,
                                               UINT4 *pMode,
                                               UINT4 *pRate,
                                               UINT4 *pPduMaxSize);
INT4 hostMsgHyPhy20gHostMsgCmdReqLnkOamGetRemoteMibVar(struct file *fileHndl,
                                                       UINT4 chnlCnt,
                                                       UINT4 *pChnlType,
                                                       UINT4 *pChnlId,
                                                       UINT4 *pObndMsgBodyLen,
                                                       UINT4 *pObndMsgBody);
INT4 hostMsgHyPhy20gHostMsgCmdCfgLnkOamCtrlChnl(struct file *fileHndl,
                                                UINT4 chnlCnt, UINT4 *pChnlType,
                                                UINT4 *pChnlId, UINT4 *pEnable);
INT4 hostMsgHyPhy20gHostMsgCmdCfgLnkOamSetCritEvntNotf(struct file *fileHndl,
                                                       UINT4 chnlCnt,
                                                       UINT4 *pChnlType,
                                                       UINT4 *pChnlId,
                                                       UINT4 *pCritEvnt);
INT4 hostMsgHyPhy20gHostMsgCmdReqLnkOamGetInfo(struct file *fileHndl,
                                               UINT4 chnlCnt, UINT4 *pChnlType,
                                               UINT4 *pChnlId,
                                               UINT4 *pObndMsgBodyLen,
                                               UINT4 *pObndMsgBody);
INT4 hostMsgHyPhy20gHostMsgCmdCfgLnkOamSetLpbk(struct file *fileHndl,
                                               UINT4 chnlCnt, UINT4 *pChnlType,
                                               UINT4 *pChnlId, UINT4 *pLpbkType,
                                               UINT4 *pEnable);
INT4 hostMsgHyPhy20gHostMsgCmdCfgLnkOamSendMibVarReq(struct file *fileHndl,
                                                     UINT4 chnlCnt,
                                                     UINT4 *pChnlType,
                                                     UINT4 *pChnlId,
                                                     UINT4 *pBranch,
                                                     UINT4 *pLeaf);
INT4 hostMsgHyPhy20gHostMsgCmdReqLnkOamGetRemoteLnkEvnt(struct file *fileHndl,
                                                        UINT4 chnlType,
                                                        UINT4 chnlId,
                                                        UINT4 *pObndMsgLen,
                                                        UINT4 *pObndMsgBody);
INT4 hostMsgHyPhy20gHostMsgCmdCfgSyncECfgChnl(struct file *fileHndl,
                                              UINT4 chnlCnt, UINT4 *pChnlType,
                                              UINT4 *pChnlId, UINT4 *pMode,
                                              UINT4 *pQlSSM);
INT4 hostMsgHyPhy20gHostMsgCmdCfgSyncECtrlChnl(struct file *fileHndl,
                                               UINT4 chnlCnt, UINT4 *pChnlType,
                                               UINT4 *pChnlId, UINT4 *pEnable);
INT4 hostMsgHyPhy20gHostMsgCmdCfgSyncETxSSM(struct file *fileHndl,
                                            UINT4 chnlCnt, UINT4 *pChnlType,
                                            UINT4 *pChnlId, UINT4 *pEvntFlag,
                                            UINT4 *pQlSSM);
INT4 hostMsgHyPhy20gHostMsgCmdCfgCsfSetLanToWanPortMapTable(struct file *fileHndl,
                                                            UINT4 chnlCnt,
                                                            UINT4 *pChnlType,
                                                            UINT4 *pChnlId,
                                                            UINT4 *pGfpIdx,
                                                            UINT4 *pGfpChnl,
                                                            UINT4 *pSerdes,
                                                            UINT4 *pVcatGrp);
INT4 hostMsgHyPhy20gHostMsgCmdCfgCsfSetLanToWanPortActTable(struct file *fileHndl,
                                                            UINT4 chnlCnt,
                                                            UINT4 *pChnlType,
                                                            UINT4 *pChnlId,
                                                            UINT4 *pTableIdx,
                                                            UINT4 *pSendDci,
                                                            UINT4 *pExi,
                                                            UINT4 *pCid,
                                                            UINT4 *pEHec,
                                                            UINT4 *pGenKpAlvCmf,
                                                            UINT4 *pUsePyld,
                                                            UINT4 *pGenPyldFcs,
                                                            UINT4 *pUpi,
                                                            UINT4 *pCmfPyld);
INT4 hostMsgHyPhy20gHostMsgCmdCfgCsfSetWanToLanMapTable(struct file *fileHndl,
                                                        UINT4 chnlCnt,
                                                        UINT4 *pChnlType,
                                                        UINT4 *pChnlId,
                                                        UINT4 *pGfpIdx,
                                                        UINT4 *pGfpChnl,
                                                        UINT4 *pAction,
                                                        UINT4 *pUpi);
INT4 hostMsgHyPhy20gHostMsgCmdCfgCsfSetWanToLanUpdateTblNotif(struct file *fileHndl,
                                                     UINT4 gfp0ChnlUpdateMskMsb,
                                                     UINT4 gfp0ChnlUpdateMskLsb,
                                                     UINT4 gfp1ChnlUpdateMskMsb,
                                                     UINT4 gfp1ChnlUpdateMskLsb);

INT4 hostMsgHyPhy20gHostMsgCmdCfgGfpPauseSetEnable(struct file *fileHndl,
                                                   UINT4 gfp0ChnlEnableMskMsb,
                                                   UINT4 gfp0ChnlEnableMskLsb,
                                                   UINT4 gfp1ChnlEnableMskMsb,
                                                   UINT4 gfp1ChnlEnableMskLsb,
                                                   UINT4 gfp0ChnlDisableMskMsb,
                                                   UINT4 gfp0ChnlDisableMskLsb,
                                                   UINT4 gfp1ChnlDisableMskMsb,
                                                   UINT4 gfp1ChnlDisableMskLsb);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gHostMsgFwOp
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function enables or disables the sending of host
**                 messages to FW.  If FW is not loaded, call this function
**                 to disable host messages BEFORE calling other APIs.  If FW
**                 is loaded, this function can be called after
**                 hyPhy20gHostMsgInit to enable/disable host messages during
**                 device bring up.
**
**  INPUTS:        *fileHndl   - base address
**                 enable      - 0 - ignores error conditions from FW
**                               1 - returns error conditions from FW
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHostMsgFwOp(struct file *fileHndl, UINT4 enable)
{
    sHYPHY20G_CTXT *pDevCtxt;
    	
    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

	/* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pDevCtxt->gData.hostMsgFwOp = enable;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHostMsgFwOp */

/*******************************************************************************
**
**  hyPhy20gHostMsgInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function initializes the message sequence
**                 counter and must be called prior to any other host
**                 messaging functions.  Please ensure FW is operational
**                 before calling this function.
**
**  INPUTS:        *fileHndl   - base address
**                 enable      - 1 to initialize message sequence counter
**                               0 to disable message sequence counter
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHostMsgInit(struct file *fileHndl, UINT4 enable)
{
    sHYPHY20G_CTXT *pDevCtxt;
#ifndef HWACCESSMODE_USB
    UINT4 reg, val, cnt;
#endif
    INT4 result;


    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    if (enable == 1) {
#ifndef HWACCESSMODE_USB
        /* check that FW is ready */
        reg = HYPHY20G_TOP_REG_APP_USR_1;
	 cnt = 500;
	 do {
	 	udelay(100);
		cnt--;
		result = sysHyPhy20gRead(fileHndl, reg, &val);
        	if(result) return result;
	 } while (((val & 0x1) == 0) && (cnt));
	 
        if ((val & 0x1) != 0x1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
#endif

        /* If pHostMsgCtxt is not a NULL pointer,
           destroy it first to prevent memory leak */
        if (pDevCtxt->pHostMsgCtxt != NULL) {
            hyPhy20gHostMsgDestroyCtxt(fileHndl);
        }

        /* allocate and initialize memory used by CPB subsystem */
        result = hyPhy20gHostMsgCreateCtxt(fileHndl);
        if(result) return result;

	} else {
	    /* free memory */
	    result = hyPhy20gHostMsgDestroyCtxt(fileHndl);
	    if(result) return result;
	}

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHostMsgInit */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgLnkOamCfgChnl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_LINKOAM_Config_Channel message
**                 to FW for a single channel and waits for a RSP_CFG response
**                 from FW.
**
**  INPUTS:        *fileHndl  - base address
**                 chnlType   - channel type
**                                  0 - HSCI
**                                  1 - LSCI
**                 chnlId     - channel ID
**                                  0 to 1  - HSCI
**                                  0 to 15 - LSCI
**                 varRetrSup - variable retrieval
**                                  0 - DTE does not support sending Variable
**                                      Respsonse OAMPDUS
**                                  1 - DTE supports sending Variable Respsonse
**                                      OAMPDUS
**                 lnkEvntSup - link events
**                                  0 - DTE does not support interpreting
**                                      Link Events
**                                  1 - DTE supports interpreting Link Events
**                 lpbkSup    - OAM remote loopback
**                                  0 - DTE is not capbable of OAM remote
**                                      loopback mode
**                                  1 - DTE is capbable of OAM remote loopback
**                                      mode
**                 uniDirSup  - unidirectional support
**                                  0 - DTE is not capbable of sending OAMPDUs
**                                      when receive path is non-operational
**                                  1 - DTE is capbable of sending OAMPDUs
**                                      when receive path is non-operational
**                 mode       - OAM mode
**                                  0 - DTE is configured in Passive mode
**                                  1 - DTE is configured in Active mode
**                 rate       - OAM generation rate (2 to 10)
**                 pduMaxSize - OAMPDU max size (42 to 1496)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgLnkOamCfgChnl(struct file *fileHndl, UINT4 chnlType,
                                        UINT4 chnlId, UINT4 varRetrSup,
                                        UINT4 lnkEvntSup, UINT4 lpbkSup,
                                        UINT4 uniDirSup, UINT4 mode, UINT4 rate,
                                        UINT4 pduMaxSize)
{
    UINT4 chnlCnt = 1;
    UINT4 numParamsPerChnl = 9;
    UINT4 *pCfgMem, *pTmpCfgMem;
    UINT4 *pChnlType, *pChnlId;
    UINT4 *pVarRetrSup, *pLnkEvntSup, *pLpbkSup, *pUniDirSup;
    UINT4 *pMode, *pRate, *pPduMaxSize;
    UINT4 size;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) ||(chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (varRetrSup > 1) || (lnkEvntSup > 1) ||
        (lpbkSup > 1) || (uniDirSup > 1) ||
        (mode > 1) ||
        (rate < 2 || rate > 10) ||
        (pduMaxSize < 42 || pduMaxSize > 1496))
    {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* allocate memory */
    size = sizeof(UINT4) * chnlCnt * numParamsPerChnl;
    pCfgMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pCfgMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pTmpCfgMem = pCfgMem;
    pChnlType  = pTmpCfgMem;
    *pChnlType = chnlType;

    pTmpCfgMem++;
    pChnlId  = pTmpCfgMem;
    *pChnlId = chnlId;

    pTmpCfgMem++;
    pVarRetrSup = pTmpCfgMem;
    *pVarRetrSup = varRetrSup;

    pTmpCfgMem++;
    pLnkEvntSup = pTmpCfgMem;
    *pLnkEvntSup = lnkEvntSup;

    pTmpCfgMem++;
    pLpbkSup = pTmpCfgMem;
    *pLpbkSup = lpbkSup;

    pTmpCfgMem++;
    pUniDirSup = pTmpCfgMem;
    *pUniDirSup = uniDirSup;

    pTmpCfgMem++;
    pMode = pTmpCfgMem;
    *pMode = mode;

    pTmpCfgMem++;
    pRate = pTmpCfgMem;
    *pRate = rate;

    pTmpCfgMem++;
    pPduMaxSize = pTmpCfgMem;
    *pPduMaxSize = pduMaxSize;

    /* send message and check response */
    result = hostMsgHyPhy20gHostMsgCmdCfgLnkOamCfgChnl(fileHndl, chnlCnt,
                                                       pChnlType, pChnlId,
                                                       pVarRetrSup, pLnkEvntSup,
                                                       pLpbkSup, pUniDirSup,
                                                       pMode, pRate,
                                                       pPduMaxSize);
    if (result) {
        sysHyPhy20gMemFree(pCfgMem);
        return result;
    }

    sysHyPhy20gMemFree(pCfgMem);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgLnkOamCfgChnl */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqLnkOamGetInfo
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_LINKOAM_Get_Infomation message
**                 to FW for a single channel and returns FW respsonse
**                 RSP_REQ_LINKOAM_Get_Information.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *rspCtrl       - control
**                 *rspLCfg       - local configuration
**                 *rspRCfg       - remote configuration
**                 *rspLState     - local state
**                 *rspRState     - remote state
**                 *rspLCritEvnt  - local critical events
**                 *rspRCritEvnt  - remote critical events
**                 *rspMibReqCnt  - remote request MIB variable count
**                 *rspMibRspCnt  - remote response MIB variable count
**                 *rspRate       - rate
**                 *rspPduMaxSize - maximum PDU size
**                 *rspBranch0    - variable branch 0
**                 *rspLeaf0      - variable leaf 0
**                 *rspBranch1    - variable branch 1
**                 *rspLeaf1      - variable leaf 1
**                 *rspBranch2    - variable branch 2
**                 *rspLeaf2      - variable leaf 2
**                 *rspBranch3    - variable branch 3
**                 *rspLeaf3      - variable leaf 3
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
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
                                        UINT4 *rspBranch3, UINT4 *rspLeaf3)
{
    UINT4 chnlCnt = 1;
    UINT4 numParamsPerIbndChnl = 2;
    UINT4 obndMsgBodyLenPerChnl = 9;
    UINT4 obndMsgBodyLen;
    UINT4 *pIbndCfgMem, *pTmpIbndCfgMem;
    UINT4 *pObndMsgBodyMem, *pTmpObndMsgBodyMem;
    UINT4 *pChnlType, *pChnlId;
    UINT4 size;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15))
    {
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx\n", chnlType, chnlId);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (rspCtrl == NULL || rspLCfg == NULL || rspRCfg == NULL ||
        rspLState == NULL || rspRState == NULL ||
        rspLCritEvnt == NULL || rspRCritEvnt == NULL ||
        rspMibReqCnt == NULL || rspMibRspCnt == NULL ||
        rspRate == NULL || rspPduMaxSize == NULL ||
        rspBranch0 == NULL || rspLeaf0 == NULL ||
        rspBranch1 == NULL || rspLeaf1 == NULL ||
        rspBranch2 == NULL || rspLeaf2 == NULL ||
        rspBranch3 == NULL || rspLeaf3 == NULL)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* allocate memory for inbound message */
    size = sizeof(UINT4) * chnlCnt * numParamsPerIbndChnl;
    pIbndCfgMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndCfgMem == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pTmpIbndCfgMem = pIbndCfgMem;
    pChnlType  = pTmpIbndCfgMem;
    *pChnlType = chnlType;

    pTmpIbndCfgMem++;
    pChnlId  = pTmpIbndCfgMem;
    *pChnlId = chnlId;

    /* allocate memory for outbound message */
    size = sizeof(UINT4) * chnlCnt * obndMsgBodyLenPerChnl;
    pObndMsgBodyMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBodyMem == NULL) {
        sysHyPhy20gMemFree(pIbndCfgMem);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* send message and check response */
    result = hostMsgHyPhy20gHostMsgCmdReqLnkOamGetInfo(fileHndl, chnlCnt,
                                                       pChnlType, pChnlId,
                                                       &obndMsgBodyLen,
                                                       pObndMsgBodyMem);
    if (result) {
        sysHyPhy20gMemFree(pIbndCfgMem);
        sysHyPhy20gMemFree(pObndMsgBodyMem);
        return result;
    }

    /* parse message response */
    if (obndMsgBodyLen != 9) {
        sysHyPhy20gMemFree(pIbndCfgMem);
        sysHyPhy20gMemFree(pObndMsgBodyMem);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_RSP_REQ\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_RSP_REQ;
    }

    pTmpObndMsgBodyMem = pObndMsgBodyMem + 2;
    *rspCtrl   = (*pTmpObndMsgBodyMem >> 24) & 0xFF;
    *rspLCfg   = (*pTmpObndMsgBodyMem >> 16) & 0xFF;
    *rspRCfg   = (*pTmpObndMsgBodyMem >> 8) & 0xFF;
    *rspLState = (*pTmpObndMsgBodyMem >> 0) & 0xFF;

    pTmpObndMsgBodyMem++;
    *rspRState    = (*pTmpObndMsgBodyMem >> 24) & 0xFF;
    *rspLCritEvnt = (*pTmpObndMsgBodyMem >> 16) & 0xFF;
    *rspRCritEvnt = (*pTmpObndMsgBodyMem >> 8) & 0xFF;
    *rspMibReqCnt = (*pTmpObndMsgBodyMem >> 0) & 0xFF;

    pTmpObndMsgBodyMem++;
    *rspMibRspCnt  = (*pTmpObndMsgBodyMem >> 24) & 0xFF;
    *rspRate       = (*pTmpObndMsgBodyMem >> 16) & 0xFF;
    *rspPduMaxSize = (*pTmpObndMsgBodyMem >> 0) & 0xFFFF;

    pTmpObndMsgBodyMem++;
    *rspBranch0 = (*pTmpObndMsgBodyMem >> 24) & 0xFF;
    *rspLeaf0   = (*pTmpObndMsgBodyMem >> 8) & 0xFFFF;

    pTmpObndMsgBodyMem++;
    *rspBranch1 = (*pTmpObndMsgBodyMem >> 24) & 0xFF;
    *rspLeaf1   = (*pTmpObndMsgBodyMem >> 8) & 0xFFFF;

    pTmpObndMsgBodyMem++;
    *rspBranch2 = (*pTmpObndMsgBodyMem >> 24) & 0xFF;
    *rspLeaf2   = (*pTmpObndMsgBodyMem >> 8) & 0xFFFF;

    pTmpObndMsgBodyMem++;
    *rspBranch3 = (*pTmpObndMsgBodyMem >> 24) & 0xFF;
    *rspLeaf3   = (*pTmpObndMsgBodyMem >> 8) & 0xFFFF;

    sysHyPhy20gMemFree(pIbndCfgMem);
    sysHyPhy20gMemFree(pObndMsgBodyMem);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqLnkOamGetInfo */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqLnkOamGetRemoteMibVar
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_LINKOAM_Get_Remote_MIB_Variable
**                 message to FW for a single channel and returns FW response
**                 RSP_REQ_LINKOAM_Get_Remote_MIB_Variable.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - pointer to storage for channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - pointer to storage for channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**
**                 Response parameters:
**
**                 *branch    - pointer to storage for branch
**                 *leaf      - pointer to storage for leaf
**                 *width     - pointer to storage for width
**                 *valueLen  - pointer to storage for value length in bytes
**                 *value[32] - pointer to storage for value (size of storage
**                              must be 32 x 4 bytes)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqLnkOamGetRemoteMibVar(struct file *fileHndl,
                                                UINT4 chnlType, UINT4 chnlId,
                                                UINT4 *branch, UINT4 *leaf,
                                                UINT4 *width, UINT4 *valueLen,
                                                UINT4 *value)
{
    UINT4 chnlCnt = 1;
    UINT4 numParamsPerChnl = 2;
    UINT4 oneChnlObndMsgBodyLen = 3 + 32;
    UINT4 obndMsgBodyLen, widthBit7;
    UINT4 *pIbndCfgMem, *pTmpIbndCfgMem;
    UINT4 *pObndMsgBody, *pTmpObndMsgBody;
    UINT4 *pChnlType, *pChnlId, *pValue;
    UINT4 size, i;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (branch == NULL || leaf == NULL || width == NULL || value == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* allocate memory */
    size = sizeof(UINT4) * chnlCnt * numParamsPerChnl;
    pIbndCfgMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndCfgMem == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    size = sizeof(UINT4) * oneChnlObndMsgBodyLen;
    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        sysHyPhy20gMemFree(pIbndCfgMem);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pTmpIbndCfgMem = pIbndCfgMem;
    pChnlType      = pTmpIbndCfgMem;
    *pChnlType     = chnlType;

    pTmpIbndCfgMem++;
    pChnlId  = pTmpIbndCfgMem;
    *pChnlId = chnlId;

    /* send message and check response */
    result = hostMsgHyPhy20gHostMsgCmdReqLnkOamGetRemoteMibVar(fileHndl,
                                                               chnlCnt,
                                                               pChnlType,
                                                               pChnlId,
                                                               &obndMsgBodyLen,
                                                               pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pIbndCfgMem);
        sysHyPhy20gMemFree(pObndMsgBody);
        return result;
    }

    /* parse response */

    if (obndMsgBodyLen == 1) {
        /* no remote MIB Variable returned */
        *branch = 0;
        *leaf = 0;
        *width = 0;
        *valueLen = 0;
        *value = 0;
    } else {
        if (obndMsgBodyLen < 3) {
            sysHyPhy20gMemFree(pIbndCfgMem);
            sysHyPhy20gMemFree(pObndMsgBody);
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_RSP_REQ\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_RSP_REQ;
        }
        /* skip to word containing branch, leaf, width */
        pTmpObndMsgBody = pObndMsgBody + 2;
        *branch = (*pTmpObndMsgBody >> 24) & 0xFF;
        *leaf   = (*pTmpObndMsgBody >> 8) & 0xFFFF;
        *width  = (*pTmpObndMsgBody >> 0) & 0xFF;


        widthBit7 = (*width >> 7) & 0x1;
        if (widthBit7 == 1) {
            /* width[6:0] represents a Variable Indication */
            *valueLen = 0;
        } else {
            if (*width == 0) {
                /* 128 octets */
                *valueLen = 128;
            } else {
                *valueLen = *width & 0x7F;
            }
        }

        if (widthBit7 == 0) {
            if (obndMsgBodyLen < 4) {
                sysHyPhy20gMemFree(pIbndCfgMem);
                sysHyPhy20gMemFree(pObndMsgBody);
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_RSP_REQ\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_RSP_REQ;
            }
            /* update contents of value pointer */
            pTmpObndMsgBody = pObndMsgBody + 3;
            pValue = value;
            for (i = 0; i < (obndMsgBodyLen - 3); i++) {
                *pValue = *pTmpObndMsgBody;

                pTmpObndMsgBody++;
                pValue++;
            }
        }
    }

    sysHyPhy20gMemFree(pIbndCfgMem);
    sysHyPhy20gMemFree(pObndMsgBody);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqLnkOamGetRemoteMibVar */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgLnkOamSetLpbk
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_LINKOAM_Set_Loopback message
**                 to FW for a single channel and waits for a RSP_CFG response
**                 from FW.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 lpbkType  - loopback type
**                                 0 for local loopback
**                                 1 for remote loopback
**                 enable    - enable/disable
**                                 0 to disable
**                                 1 to enable
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgLnkOamSetLpbk(struct file *fileHndl, UINT4 chnlType,
                                        UINT4 chnlId, UINT4 lpbkType,
                                        UINT4 enable)
{
    UINT4 chnlCnt = 1;
    UINT4 numParamsPerChnl = 4;
    UINT4 *pCfgMem, *pTmpCfgMem;
    UINT4 *pChnlType, *pChnlId;
    UINT4 *pLpbkType, *pEnable;
    UINT4 size;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (lpbkType > 1) || (enable > 1))
    {
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx, lpbkType=0x%lx, enable=0x%lx\n", chnlType, chnlId, lpbkType, enable);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* allocate memory */
    size = sizeof(UINT4) * chnlCnt * numParamsPerChnl;
    pCfgMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pCfgMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pTmpCfgMem = pCfgMem;
    pChnlType  = pTmpCfgMem;
    *pChnlType = chnlType;

    pTmpCfgMem++;
    pChnlId  = pTmpCfgMem;
    *pChnlId = chnlId;

    pTmpCfgMem++;
    pLpbkType  = pTmpCfgMem;
    *pLpbkType = lpbkType;

    pTmpCfgMem++;
    pEnable  = pTmpCfgMem;
    *pEnable = enable;

    /* send message and check response */
    result = hostMsgHyPhy20gHostMsgCmdCfgLnkOamSetLpbk(fileHndl, chnlCnt,
                                                       pChnlType, pChnlId,
                                                       pLpbkType, pEnable);
    if (result) {
        sysHyPhy20gMemFree(pCfgMem);
        return result;
    }

    sysHyPhy20gMemFree(pCfgMem);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgLnkOamSetLpbk */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgLnkOamSetLocalMibVar
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_LINKOAM_Set_Local_MIB_Variables
**                 message to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl  - base address
**                 chnlType   - channel type
**                                  0 - HSCI
**                                  1 - LSCI
**                 chnlId     - channel ID
**                                  0 to 1  - HSCI
**                                  0 to 15 - LSCI
**                 branch     - variable branch
**                 leaf       - variable leaf
**                 varInd     - 0 - include Variable Value field
**                              1 - include Variable Indication
**                 varIndVal  - variable indication value (7 bits)
**                            - set to 0 if varInd = 0
**                 valueLen   - number of bytes in pValue
**                                  - set to 0 if varInd = 1
**                 *pValue    - pointer to storage for variable value
**                                  - set to 0 if varInd = 1
**                                  - If varInd is 0, pValue contains the list
**                                    of octets in Variable Value, up to a
**                                    maximum of 128 octets.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgLnkOamSetLocalMibVar(struct file *fileHndl,
                                               UINT4 chnlType, UINT4 chnlId,
                                               UINT4 branch, UINT4 leaf,
                                               UINT4 varInd, UINT4 varIndVal,
                                               UINT4 valueLen, UINT1 *pValue)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 width;
    UINT1 *pTmpValue;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size, numDataWords = 0, tmpWord;
    UINT4 i, rv;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (branch > 0xFF) || (leaf > 0xFFFF) ||
        (varInd > 1) || (varIndVal > 0x7F) ||
        (valueLen > 128))
    {
        DBG_PRINT("error 1\n");
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx, branch=0x%lx, leaf=0x%lx, varInd=0x%lx, varIndVal=0x%lx, valueLen=0x%lx\n", chnlType, chnlId, branch, leaf, varInd, varIndVal, valueLen);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (varInd == 0 && pValue == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_LINKOAM;
    ibndMsgCat = CATEGORY_CMD_CFG;

    ibndMsgLen = 0x4;
    if (varInd == 0) {
        i = valueLen / 4;
        ibndMsgLen += i;

        i = valueLen % 4;
        if (i != 0) {
            ibndMsgLen++;
        }
    }

    ibndMsgSubtype = 0x05;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and id */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) |
                   ((chnlId & 0xFF) << 16);

    /* branch, leaf and width */
    if (varInd == 1) {
        width = (0x1 << 7) | (varIndVal & 0x7F);
    } else {
        if (valueLen == 128) {
            width = 0;
        } else {
            width = valueLen;
        }
    }
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((branch & 0xFF) << 24) |
                   ((leaf & 0xFFFF) << 8) |
                   (width & 0xFF);

    /* value */
    if (varInd == 0) {

        pTmpValue = pValue;
        numDataWords = valueLen / 4;

        for (i = 0; i < numDataWords; i++) {
            tmpWord = (*pTmpValue & 0xFF) << 24;
            pTmpValue++;
            tmpWord |= (*pTmpValue & 0xFF) << 16;
            pTmpValue++;
            tmpWord |= (*pTmpValue & 0xFF) << 8;
            pTmpValue++;
            tmpWord |= (*pTmpValue & 0xFF);

            pTmpIbndMsg++;
            *pTmpIbndMsg = tmpWord;

            pTmpValue++;
            DBG_PRINT("tmpWord=0x%lx\n", tmpWord);
        }

        rv = valueLen % 4;
        if (rv != 0) {
            tmpWord = 0;
            for (i = 0; i < rv; i++) {
                tmpWord |= *pTmpValue << (8*(3-i));
                pTmpValue++;
            }
            pTmpIbndMsg++;
            *pTmpIbndMsg = tmpWord;
            DBG_PRINT("tmpWord=0x%lx\n", tmpWord);
        }
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgLnkOamSetLocalMibVar */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgLnkOamSetCritEvntNotf
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_LINKOAM_Set_Critical_Event_Notification message
**                 to FW for a single channel and waits for a RSP_CFG response
**                 from FW.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 critEvnt  - critical event flag
**                                 0 - link fault
**                                 1 - dying gasp
**                                 2 - critical event
**                                 3 - clear all critical event flags
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgLnkOamSetCritEvntNotf(struct file *fileHndl,
                                                UINT4 chnlType,
                                                UINT4 chnlId,
                                                UINT4 critEvnt)
{
    UINT4 chnlCnt = 1;
    UINT4 numParamsPerChnl = 3;
    UINT4 *pCfgMem, *pTmpCfgMem;
    UINT4 *pChnlType, *pChnlId, *pCritEvnt;
    UINT4 size;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (critEvnt > 3))
    {
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx, critEvnt=0x%lx\n", chnlType, chnlId, critEvnt);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* allocate memory */
    size = sizeof(UINT4) * chnlCnt * numParamsPerChnl;
    pCfgMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pCfgMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pTmpCfgMem = pCfgMem;
    pChnlType  = pTmpCfgMem;
    *pChnlType = chnlType;

    pTmpCfgMem++;
    pChnlId  = pTmpCfgMem;
    *pChnlId = chnlId;

    pTmpCfgMem++;
    pCritEvnt = pTmpCfgMem;
    *pCritEvnt = critEvnt;

    /* send message and check response */
    result = hostMsgHyPhy20gHostMsgCmdCfgLnkOamSetCritEvntNotf(fileHndl,
                                                               chnlCnt,
                                                               pChnlType,
                                                               pChnlId,
                                                               pCritEvnt);
    if (result) {
        sysHyPhy20gMemFree(pCfgMem);
        return result;
    }

    sysHyPhy20gMemFree(pCfgMem);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgLnkOamSetCritEvntNotf */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgLnkOamCtrlChnl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_LINKOAM_Control_Channel message
**                 to FW for a single channel and waits for a RSP_CFG response
**                 from FW.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 enable    - enable/disable flag
**                                 0 to disable channel
**                                 1 to enable channel
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgLnkOamCtrlChnl(struct file *fileHndl,
                                         UINT4 chnlType, UINT4 chnlId,
                                         UINT4 enable)
{
    UINT4 chnlCnt = 1;
    UINT4 numParamsPerChnl = 3;
    UINT4 *pCfgMem, *pTmpCfgMem;
    UINT4 *pChnlType, *pChnlId, *pEnable;
    UINT4 size;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (enable > 1))
    {
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx, enable=0x%lx\n", chnlType, chnlId, enable);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* allocate memory */
    size = sizeof(UINT4) * chnlCnt * numParamsPerChnl;
    pCfgMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pCfgMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pTmpCfgMem = pCfgMem;
    pChnlType  = pTmpCfgMem;
    *pChnlType = chnlType;

    pTmpCfgMem++;
    pChnlId  = pTmpCfgMem;
    *pChnlId = chnlId;

    pTmpCfgMem++;
    pEnable  = pTmpCfgMem;
    *pEnable = enable;

    /* send message and check response */
    result = hostMsgHyPhy20gHostMsgCmdCfgLnkOamCtrlChnl(fileHndl, chnlCnt,
                                                        pChnlType, pChnlId,
                                                        pEnable);
    if (result) {
        sysHyPhy20gMemFree(pCfgMem);
        return result;
    }

    sysHyPhy20gMemFree(pCfgMem);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgLnkOamCtrlChnl */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgLnkOamLocEvntCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_LINKOAM_Local_Event_Config
**                 message to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl           - base address
**                 chnlType            - channel type
**                                         0 - HSCI
**                                         1 - LSCI
**                 chnlId              - channel ID
**                                         0 to 1  - HSCI
**                                         0 to 15 - LSCI
**                 symPerEvntWinMsw    - most significant word (32-bit) of the
**                                       Errored Symbol Period Event window
**                 symPerEvntWinLsw    - least significant word (32-bit) of the
**                                       Errored Symbol Period Event window
**                 symPerEvntThreshMsw - most significant word (32-bit) of the
**                                       Errored Symbol Period Event threshold
**                 symPerEvntThreshLsw - least significant word (32-bit) of the
**                                       Errored Symbol Period Event threshold
**                 frmEvntWin          - Errored Frame Event window (16-bit)
**                 frmEvntThresh       - Errored Frame Event threshold (32-bit)
**                 frmPerEvntWin       - Errored Frame Perod Event window
**                                       (32-bit)
**                 frmPerEvntThresh    - Errored Frame Perod Event threshold
**                                       (32-bit)
**                 frmSecSumEvntWin    - Errored Frame Seconds Summary Event
**                                       window (16-bit)
**                 frmSecSumEvntThresh - Errored Frame Seconds Summary
**                                       Event threshold (16-bit)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
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
                                           UINT4 frmSecSumEvntThresh)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (frmEvntWin > 0xFFFF) || (frmSecSumEvntWin > 0xFFFF) ||
        (frmSecSumEvntThresh > 0xFFFF))
    {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_LINKOAM;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 12;
    ibndMsgSubtype = 0x08;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and ID */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) | ((chnlId & 0xFF) << 16);

    /* errored symbol window */
    pTmpIbndMsg++;
    *pTmpIbndMsg = symPerEvntWinMsw;
    pTmpIbndMsg++;
    *pTmpIbndMsg = symPerEvntWinLsw;

    /* errored symbol threshold */
    pTmpIbndMsg++;
    *pTmpIbndMsg = symPerEvntThreshMsw;
    pTmpIbndMsg++;
    *pTmpIbndMsg = symPerEvntThreshLsw;

    /* errored frame window */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (frmEvntWin & 0xFFFF) << 16;

    /* errored frame threshold */
    pTmpIbndMsg++;
    *pTmpIbndMsg = frmEvntThresh;

    /* errored frame period window */
    pTmpIbndMsg++;
    *pTmpIbndMsg = frmPerEvntWin;

    /* errored frame period threshold */
    pTmpIbndMsg++;
    *pTmpIbndMsg = frmPerEvntThresh;

    /* errored frame seconds summary window and threshold */
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((frmSecSumEvntWin & 0xFFFF) << 16) |
                       (frmSecSumEvntThresh & 0xFFFF);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgLnkOamLocEvntCfg */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgLnkOamSendMibVarReq
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_LINKOAM_Send_MIB_Variables_Request message to FW
**                 for a single channel and waits for a RSP_CFG response from
**                 FW.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 branch    - branch variable (8-bits)
**                 leaf      - leaf variable (16-bits)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgLnkOamSendMibVarReq(struct file *fileHndl,
                                              UINT4 chnlType, UINT4 chnlId,
                                              UINT4 branch, UINT4 leaf)
{
    UINT4 chnlCnt = 1;
    UINT4 numParamsPerChnl = 4;
    UINT4 *pCfgMem, *pTmpCfgMem;
    UINT4 *pChnlType, *pChnlId, *pBranch, *pLeaf;
    UINT4 size;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (branch > 0xFF) || (leaf > 0xFFFF))
    {
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx, branch=0x%lx, leaf=0x%lx\n", chnlType, chnlId, branch, leaf);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* allocate memory */
    size = sizeof(UINT4) * chnlCnt * numParamsPerChnl;
    pCfgMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pCfgMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pTmpCfgMem = pCfgMem;
    pChnlType  = pTmpCfgMem;
    *pChnlType = chnlType;

    pTmpCfgMem++;
    pChnlId  = pTmpCfgMem;
    *pChnlId = chnlId;

    pTmpCfgMem++;
    pBranch  = pTmpCfgMem;
    *pBranch = branch;

    pTmpCfgMem++;
    pLeaf  = pTmpCfgMem;
    *pLeaf = leaf;

    /* send message and check response */
    result = hostMsgHyPhy20gHostMsgCmdCfgLnkOamSendMibVarReq(fileHndl, chnlCnt,
                                                             pChnlType, pChnlId,
                                                             pBranch, pLeaf);
    if (result) {
        sysHyPhy20gMemFree(pCfgMem);
        return result;
    }

    sysHyPhy20gMemFree(pCfgMem);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgLnkOamSendMibVarReq */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqLnkOamGetLocalLnkEvnt
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_LINKOAM_Get_Local_Link_Event
**                 message to FW and returns FW response
**                 RSP_REQ_LINKOAM_ Get_Local_Link_Event.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                               0 - HSCI
**                               1 - LSCI
**                 chnlId    - channel ID
**                               0 to 1  - HSCI
**                               0 to 15 - LSCI
**
**                 Response message parameters:
**
**                  *pTLV0[10] - pointer to storage for Errored Symbol Period
**                               Event TLV (size of storage must be
**                               10 x 4 bytes)
**                  *pTLV1[7]  - pointer to storage for Errored Frame Event TLV
**                               (size of storage must be 7 x 4 bytes)
**                  *pTLV2[7]  - pointer to storage for Errored Frame Period
**                               Event TLV (size of storage must be 7 x 4 bytes)
**                  *pTLV3[5]  - pointer to storage for Errored Frame Seconds
**                               Summary Event TLV (size of storage must be
**                               5 x 4 bytes)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqLnkOamGetLocalLnkEvnt(struct file *fileHndl,
                                                UINT4 chnlType, UINT4 chnlId,
                                                UINT4 *pTLV0, UINT4 *pTLV1,
                                                UINT4 *pTLV2, UINT4 *pTLV3)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 *pTmpTLV0, *pTmpTLV1, *pTmpTLV2, *pTmpTLV3;
    UINT4 *pObndMsgBody, *pTmpObndMsgBody;
    UINT4 obndMsgLen, obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 tlv0Len = HYPHY20G_HOSTMSG_NUMWORDS_SYMBOL_PERIOD_EVENT_TLV;
    UINT4 tlv1Len = HYPHY20G_HOSTMSG_NUMWORDS_FRAME_EVENT_TLV;
    UINT4 tlv2Len = HYPHY20G_HOSTMSG_NUMWORDS_FRAME_PERIOD_EVENT_TLV;
    UINT4 tlv3Len = HYPHY20G_HOSTMSG_NUMWORDS_FRAME_SECONDS_EVENT_TLV;
    UINT4 size, i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pTLV0 == NULL || pTLV1 == NULL || pTLV2 == NULL || pTLV3 == NULL)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_LINKOAM;
    ibndMsgCat     = CATEGORY_CMD_REQ;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x0a;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and ID */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) | ((chnlId & 0xFF) << 16);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    if (obndMsgLen != 32) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_RSP_REQ\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_RSP_REQ;
    }

    /* allocate memory for the message body (ie. without headers) */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);
    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* parse message body */

    /* copy data into pTVL0*/
    pTmpObndMsgBody = pObndMsgBody + 1;
    pTmpTLV0 = pTLV0;
    for (i = 0; i < tlv0Len; i++) {
        *pTmpTLV0 = *pTmpObndMsgBody;
        pTmpTLV0++;
        pTmpObndMsgBody++;
    }

    /* copy data into pTVL1*/
    pTmpObndMsgBody = pObndMsgBody + 1 + tlv0Len;
    pTmpTLV1 = pTLV1;
    for (i = 0; i < tlv1Len; i++) {
        *pTmpTLV1 = *pTmpObndMsgBody;
        pTmpTLV1++;
        pTmpObndMsgBody++;
    }

    /* copy data into pTVL2*/
    pTmpObndMsgBody = pObndMsgBody + 1 + tlv0Len + tlv1Len;
    pTmpTLV2 = pTLV2;
    for (i = 0; i < tlv2Len; i++) {
        *pTmpTLV2 = *pTmpObndMsgBody;
        pTmpTLV2++;
        pTmpObndMsgBody++;
    }

    /* copy data into pTVL3*/
    pTmpObndMsgBody = pObndMsgBody + 1 + tlv0Len + tlv1Len + tlv2Len;
    pTmpTLV3 = pTLV3;
    for (i = 0; i < tlv3Len; i++) {
        *pTmpTLV3 = *pTmpObndMsgBody;
        pTmpTLV3++;
        pTmpObndMsgBody++;
    }

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqLnkOamGetLocalLnkEvnt */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqLnkOamGetRemoteLnkEvnt
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_LINKOAM_Get_Remote_Link_Event
**                 message to FW and returns FW response
**                 RSP_REQ_LINKOAM_ Get_Remote_Link_Event.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                               0 - HSCI
**                               1 - LSCI
**                 chnlId    - channel ID
**                               0 to 1  - HSCI
**                               0 to 15 - LSCI
**
**                 Response message parameters:
**
**                  *pTLV0[10] - pointer to storage for Errored Symbol Period
**                               Event TLV (size of storage must be
**                               10 x 4 bytes)
**                  *pTLV1[7]  - pointer to storage for Errored Frame Event TLV
**                               (size of storage must be 7 x 4 bytes)
**                  *pTLV2[7]  - pointer to storage for Errored Frame Period
**                               Event TLV (size of storage must be 7 x 4 bytes)
**                  *pTLV3[5]  - pointer to storage for Errored Frame Seconds
**                               Summary Event TLV (size of storage must be
**                               5 x 4 bytes)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqLnkOamGetRemoteLnkEvnt(struct file *fileHndl,
                                                 UINT4 chnlType, UINT4 chnlId,
                                                 UINT4 *pTLV0, UINT4 *pTLV1,
                                                 UINT4 *pTLV2, UINT4 *pTLV3)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 *pTmpTLV0, *pTmpTLV1, *pTmpTLV2, *pTmpTLV3;
    UINT4 *pObndMsgBody, *pTmpObndMsgBody;
    UINT4 obndMsgLen, obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 tlv0Len = HYPHY20G_HOSTMSG_NUMWORDS_SYMBOL_PERIOD_EVENT_TLV;
    UINT4 tlv1Len = HYPHY20G_HOSTMSG_NUMWORDS_FRAME_EVENT_TLV;
    UINT4 tlv2Len = HYPHY20G_HOSTMSG_NUMWORDS_FRAME_PERIOD_EVENT_TLV;
    UINT4 tlv3Len = HYPHY20G_HOSTMSG_NUMWORDS_FRAME_SECONDS_EVENT_TLV;
    UINT4 size, i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pTLV0 == NULL || pTLV1 == NULL || pTLV2 == NULL || pTLV3 == NULL)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_LINKOAM;
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x0b;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and ID */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) | ((chnlId & 0xFF) << 16);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    if (obndMsgLen != 32) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_RSP_REQ\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_RSP_REQ;
    }

    /* allocate memory */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);
    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* parse message body */

    /* copy data into pTVL0*/
    pTmpObndMsgBody = pObndMsgBody + 1;
    pTmpTLV0 = pTLV0;
    for (i = 0; i < tlv0Len; i++) {
        *pTmpTLV0 = *pTmpObndMsgBody;
        pTmpTLV0++;
        pTmpObndMsgBody++;
    }

    /* copy data into pTVL1*/
    pTmpObndMsgBody = pObndMsgBody + 1 + tlv0Len;
    pTmpTLV1 = pTLV1;
    for (i = 0; i < tlv1Len; i++) {
        *pTmpTLV1 = *pTmpObndMsgBody;
        pTmpTLV1++;
        pTmpObndMsgBody++;
    }

    /* copy data into pTVL2*/
    pTmpObndMsgBody = pObndMsgBody + 1 + tlv0Len + tlv1Len;
    pTmpTLV2 = pTLV2;
    for (i = 0; i < tlv2Len; i++) {
        *pTmpTLV2 = *pTmpObndMsgBody;
        pTmpTLV2++;
        pTmpObndMsgBody++;
    }

    /* copy data into pTVL3*/
    pTmpObndMsgBody = pObndMsgBody + 1 + tlv0Len + tlv1Len + tlv2Len;
    pTmpTLV3 = pTLV3;
    for (i = 0; i < tlv3Len; i++) {
        *pTmpTLV3 = *pTmpObndMsgBody;
        pTmpTLV3++;
        pTmpObndMsgBody++;
    }

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqLnkOamGetRemoteLnkEvnt */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqInfoGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_INFO_Get_Status message to FW
**                 and returns FW response RSP_REQ_INFO_Get_Status.
**
**  INPUTS:        *fileHndl  - base address
**                 statCtrl   - Status Control
**                              1 - request status information
**                              2 - request FW version check information
**
**                 Response message parameters:
**
**                 *statType  - pointer to storage for returned status type.
**                              This value should be matched with the Status
**                              Control value.
**                 *statCode  - pointer to storage for returned status code.
**                              When status type = 0x01:
**                              0xABCD - Firmware is alive, no error
**                              When status type = 0x02:
**                              0x0000 - Firmware version supported on current
**                                       device revision
**                              0x0001 - Firmware version not supported on 
**                                       current device revision
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqInfoGetStatus(struct file *fileHndl, UINT4 statCtrl,
                                        UINT4 *statType, UINT4 *statCode)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 *pTmpObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (statCtrl < 1 || statCtrl > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    if (statCode == NULL || statType == NULL) {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_INFO_DBG;
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x01;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = statCtrl;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pTmpObndMsgBody = pObndMsgBody;

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;
    *statCode = (*pTmpObndMsgBody >> 16) & 0xFFFF;
    *statType = *pTmpObndMsgBody & 0xFFFF;

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqInfoGetStatus */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqInfoGetVerInfo
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_INFO_Get_Version_Info message
**                 to FW and returns FW response RSP_REQ_INFO_Get_Version_Info.
**
**  INPUTS:        *fileHndl - base address
**                 control   - response message control
**                               1 - get revision number only (return value for
**                                   buildNum will be 0)
**                               2 - get revision and build number
**
**                 Response message parameters:
**
**                 *revNum   - pointer to storage for returned revision number.
**                             32-bit revision number in format 0xmmnnsstt:
**                             mm = major number
**                             nn = minor number
**                             ss = sub-minor number
**                             tt = status info
**                                  01 - official build
**                                  02 - developer build
**                 *buildNum - pointer to storage for returned build number.
**                             32-bit build number for specifying the build 
**                             date information with format 0xyyyymmdd:
**                             yyyy = year
**                             mm   = month
**                             dd   = day
**                             (if control=1, returned value is always 0)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqInfoGetVerInfo(struct file *fileHndl, UINT4 control,
                                         UINT4 *revNum, UINT4 *buildNum)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 *pTmpObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (control < 1 || control > 2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (revNum == NULL || buildNum == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_INFO_DBG;
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x02;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = control & 0xFF;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pTmpObndMsgBody = pObndMsgBody;

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;
    *revNum = *pTmpObndMsgBody;

    if (control == 2) {
        pTmpObndMsgBody++;
        *buildNum = *pTmpObndMsgBody;
    } else {
        *buildNum = 0;
    }

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqInfoGetVerInfo */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgSyncECfgChnl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_SYNCE_Config_Channel message
**                 to FW for a single channel and waits for a RSP_CFG response
**                 from FW.
**
**  INPUTS:        *fileHndl  - base address
**                 chnlType   - channel type
**                                  0 - HSCI
**                                  1 - LSCI
**                 chnlId     - channel ID
**                                  0 to 1  - HSCI
**                                  0 to 15 - LSCI
**                 mode       - mode
**                                  0 - slave
**                                  1 - master
**                                  2 - passive
**                 qlSSM      - SSM code of quality level
**                              Legal range 0x0 to 0xF
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgSyncECfgChnl(struct file *fileHndl, UINT4 chnlType,
                                        UINT4 chnlId, UINT4 mode, UINT4 qlSSM)
{
    UINT4 chnlCnt = 1;
    UINT4 numParamsPerChnl = 4;
    UINT4 *pCfgMem, *pTmpCfgMem;
    UINT4 *pChnlType, *pChnlId;
    UINT4 *pMode, *pQlSSM;
    UINT4 size;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (mode > 2) || (qlSSM > 15))
    {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* allocate memory */
    size = sizeof(UINT4) * chnlCnt * numParamsPerChnl;
    pCfgMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pCfgMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pTmpCfgMem = pCfgMem;
    pChnlType  = pTmpCfgMem;
    *pChnlType = chnlType;

    pTmpCfgMem++;
    pChnlId  = pTmpCfgMem;
    *pChnlId = chnlId;

    pTmpCfgMem++;
    pMode = pTmpCfgMem;
    *pMode = mode;

    pTmpCfgMem++;
    pQlSSM = pTmpCfgMem;
    *pQlSSM = qlSSM;

    /* send message and check response */
    result = hostMsgHyPhy20gHostMsgCmdCfgSyncECfgChnl(fileHndl, chnlCnt,
                                                      pChnlType, pChnlId,
                                                      pMode, pQlSSM);
    if (result) {
        sysHyPhy20gMemFree(pCfgMem);
        return result;
    }

    sysHyPhy20gMemFree(pCfgMem);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgSyncECfgChnl */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgSyncECtrlChnl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_SYNCE_Control_Channel message
**                 to FW for a single channel and waits for a RSP_CFG response
**                 from FW.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 enable    - enable/disable flag
**                                 0 to disable channel
**                                 1 to enable channel
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgSyncECtrlChnl(struct file *fileHndl,
                                        UINT4 chnlType, UINT4 chnlId,
                                        UINT4 enable)
{
    UINT4 chnlCnt = 1;
    UINT4 numParamsPerChnl = 3;
    UINT4 *pCfgMem, *pTmpCfgMem;
    UINT4 *pChnlType, *pChnlId, *pEnable;
    UINT4 size;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (enable > 1))
    {
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx, enable=0x%lx\n", chnlType, chnlId, enable);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* allocate memory */
    size = sizeof(UINT4) * chnlCnt * numParamsPerChnl;
    pCfgMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pCfgMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pTmpCfgMem = pCfgMem;
    pChnlType  = pTmpCfgMem;
    *pChnlType = chnlType;

    pTmpCfgMem++;
    pChnlId  = pTmpCfgMem;
    *pChnlId = chnlId;

    pTmpCfgMem++;
    pEnable  = pTmpCfgMem;
    *pEnable = enable;

    /* send message and check response */
    result = hostMsgHyPhy20gHostMsgCmdCfgSyncECtrlChnl(fileHndl, chnlCnt,
                                                       pChnlType, pChnlId,
                                                       pEnable);
    if (result) {
        sysHyPhy20gMemFree(pCfgMem);
        return result;
    }

    sysHyPhy20gMemFree(pCfgMem);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgSyncECtrlChnl */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqSyncEGetSSM
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_SYNCE_Get_SSM message
**                 to FW and returns FW response RSP_REQ_SYNCE_Get_SSMFW.
**
**  INPUTS:        *fileHndl   - base address
**
**                 Response message parameters:
**
**                 *pObndMsgBodyLen  - pointer to storage for response message
**                                     body length (ie. without header)
**                 *pObndMsgBody[55] - pointer to storage for response message
**                                     body without header
**                                     Size of storage must be 55 words
**                                     (55 x 4 bytes)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqSyncEGetSSM(struct file *fileHndl,
                                      UINT4 *pObndMsgBodyLen,
                                      UINT4 *pObndMsgBody)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (pObndMsgBodyLen == NULL || pObndMsgBody == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_SYNC_E;
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x02;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* reserved field */
    *pTmpIbndMsg = 0;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    *pObndMsgBodyLen = obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqSyncEGetSSM */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgSyncETxSSM
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_SYNCE_Transmit_SSM message
**                 to FW for a single channel and waits for a RSP_CFG response
**                 from FW.
**
**  INPUTS:        *fileHndl  - base address
**                 chnlType   - channel type
**                                  0 - HSCI
**                                  1 - LSCI
**                 chnlId     - channel ID
**                                  0 to 1  - HSCI
**                                  0 to 15 - LSCI
**                 evntFlag   - event flag
**                                  0 - information PDU
**                                  1 - event PDU
**                 qlSSM      - SSM code of quality level
**                              Legal range 0x0 to 0xF
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgSyncETxSSM(struct file *fileHndl, UINT4 chnlType,
                                     UINT4 chnlId, UINT4 evntFlag,
                                     UINT4 qlSSM)
{
    UINT4 chnlCnt = 1;
    UINT4 numParamsPerChnl = 4;
    UINT4 *pCfgMem, *pTmpCfgMem;
    UINT4 *pChnlType, *pChnlId;
    UINT4 *pEvntFlag, *pQlSSM;
    UINT4 size;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (evntFlag > 1) || (qlSSM > 15))
    {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* allocate memory */
    size = sizeof(UINT4) * chnlCnt * numParamsPerChnl;
    pCfgMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pCfgMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pTmpCfgMem = pCfgMem;
    pChnlType  = pTmpCfgMem;
    *pChnlType = chnlType;

    pTmpCfgMem++;
    pChnlId  = pTmpCfgMem;
    *pChnlId = chnlId;

    pTmpCfgMem++;
    pEvntFlag = pTmpCfgMem;
    *pEvntFlag = evntFlag;

    pTmpCfgMem++;
    pQlSSM = pTmpCfgMem;
    *pQlSSM = qlSSM;

    /* send message and check response */
    result = hostMsgHyPhy20gHostMsgCmdCfgSyncETxSSM(fileHndl, chnlCnt,
                                                    pChnlType, pChnlId,
                                                    pEvntFlag, pQlSSM);
    if (result) {
        sysHyPhy20gMemFree(pCfgMem);
        return result;
    }

    sysHyPhy20gMemFree(pCfgMem);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgSyncETxSSM */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetClkParam
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Clock_Param message to
**                 FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl     - base address
**                 clkType       - clock type
**                                 1 - Boundary clock
**                                 2 - End-to-end transparent clock
**                                 3 - Peer-to-peer transparent clock
**                 twoStepFlg    - two-step clock operation
**                                 0 - false
**                                 1 - true
**                 clkIdMsw      - clock identity (most significant word, 
**                                 32-bit, clockIdentity[0][1][2][3])
**                 clkIdLsw      - clock identity (least significant word,
**                                 32-bit, clockIdentity[4][5][6][7])
**                 domainNum     - clock domain number
**                                 0 - default domain
**                                 1 - alternate domain 1
**                                 2 - alternate domain 2
**                                 3 - alternate domain 3
**                                 4 to 127 - user defined domains
**                                 128 to 255 - reserved
**                 protocol      - transport protocol
**                                 0 - PTP over UDP over IPv4
**                                 1 - PTP over UDP over IPv6
**                                 2 - PTP over IEEE802.3/Ethernet
**                 operatingMode - operating modes
**                                 0 - Processing Mode, FW processes the PTP 
**                                     messages, timestamps and synchronizes to
**                                     the master.
**                                 1 - Terminate Mode, FW processes the PTP 
**                                     messages and forwards the timestamps to
**                                     the external interface.  External timing 
**                                     card processes the timestamps and 
**                                     synchronizes to the master.     
**                                 2 - Pass-Thru Mode, FW processes the 
**                                     timestamps and forwards PTP messages to 
**                                     the external interface.  External timing
**                                     card processes PTP messages and 
**                                     synchronizes to the master. 
**                 multiEnable   - Multicast Enable
**                                 0 - Do not transmit or listen to multicast
**                                     PTP messages. Only unicast.
**                                 1 - Transmit and listen to multicast PTP 
**                                     messages
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetClkParam(struct file *fileHndl, UINT4 clkType,
                                         UINT4 twoStepFlg, UINT4 clkIdMsw,
                                         UINT4 clkIdLsw, UINT4 domainNum,
                                         UINT4 protocol, UINT4 operatingMode,
                                         UINT4 multiEnable)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (clkType < 1 || clkType > 3 || twoStepFlg > 1 || domainNum > 127 ||
        protocol > 2 || operatingMode > 2 || multiEnable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x7;
    ibndMsgSubtype = 0x01;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* clock type (aka. device type) and two step flag */
    *pTmpIbndMsg = ((clkType & 0xFF) << 24) |
                   ((twoStepFlg & 0xFF) << 16);

    /* clock identity */
    pTmpIbndMsg++;
    *pTmpIbndMsg = clkIdMsw;
    pTmpIbndMsg++;
    *pTmpIbndMsg = clkIdLsw;

    /* domain number */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (domainNum & 0xFF) << 24;

    /* transport protocol, multicastEnable and operatingMode */
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((protocol & 0xFF) << 24) |
                   ((multiEnable & 0xFF) << 16) |
                   ((operatingMode & 0xFF) << 8);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetClkParam */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetIrigCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_IRIG_Config message to
**                 FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 irigPort  - IRIG port
**                               0 - IRIGI (slave)
**                               1 - IRIGO (master)
**                 irigMode  - IRIG mode
**                               0 - clock and data and 1PPS TOD interface
**                               1 - IRIG-B B00x (PWM) format
**                 adjMode   - adjustment mode
**                               0 - adjust both frequency and phase using the 
**                                   IRIG information
**                               1 - adjust frequency only
**                               2 - adjust phase only
**                 enable    - enable/disable port
**                               0 - disable port
**                               1 - enable port
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetIrigCfg(struct file *fileHndl, UINT4 irigPort,
                                        UINT4 irigMode, UINT4 adjMode, 
                                        UINT4 enable)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (irigPort > 1 || irigMode > 2 || adjMode > 2 || enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x02;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* IRIG port, enable, irigMode and adjMode */
    *pTmpIbndMsg = ((irigPort & 0xFF) << 24) |
                   ((enable & 0xFF) << 16) |
                   ((irigMode & 0xFF) << 8) |
                   ((adjMode & 0xFF) << 0);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetIrigCfg */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetIPv4Param
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_PTP_IPv4_Param message
**                 to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl   - base address
**                 precedence  - TOS precedence
**                                0 - routine
**                                1 - priority
**                                2 - immediate
**                                3 - flash
**                                4 - flash override
**                                5 - CRITIC/ECP
**                                6 - internetwork control
**                                7 - network control
**                 delay       - TOS delay mode
**                                0 - normal delay
**                                1 - low delay
**                 throughput  - TOS throughput mode
**                                0 - normal throughput
**                                1 - high throughput
**                 reliability - TOS throughput mode
**                                0 - normal reliability
**                                1 - high reliability
**                 ttl         - time to live
**                               Legal range 0 to 255
**                 srcAddr[4]  - source address of IPv4 header
**                               Legal range 0 to 255.
**                               For example: 192.168.13.1
**                                 srcAddr[3] = 192
**                                 srcAddr[2] = 168
**                                 srcAddr[1] = 13
**                                 srcAddr[0] = 1
**                 destAddr[4] - master destination address
**                               Legal range 0 to 255.
**                               For example: 192.168.13.1
**                                 destAddr[3] = 192
**                                 destAddr[2] = 168
**                                 destAddr[1] = 13
**                                 destAddr[0] = 1
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetIPv4Param(struct file *fileHndl,
                                          UINT4 precedence, UINT4 delay,
                                          UINT4 throughput, UINT4 reliability,
                                          UINT4 ttl, UINT4 srcAddr[4], 
                                          UINT4 destAddr[4])
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 tos;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (precedence > 7 || delay > 1 || throughput > 1 || reliability > 1 ||
        ttl > 255) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < 4; i++) {
        if ((srcAddr[i] > 255) || (destAddr[i] > 255)) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x5;
    ibndMsgSubtype = 0x03;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* type of service and time to live */
    tos = (precedence << 5) | (delay << 4) | (throughput << 3) |
          (reliability << 2);
    *pTmpIbndMsg = (tos << 24) | (ttl << 16);

    /* source address */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (srcAddr[3] << 24) | (srcAddr[2] << 16) | (srcAddr[1] << 8) |
                   (srcAddr[0] <<  0);
    
    /* master destination address */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (destAddr[3] << 24) | (destAddr[2] << 16) | 
                   (destAddr[1] << 8) | (destAddr[0] <<  0);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetIPv4Param */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetIPv6Param
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_PTP_IPv6_Param message
**                 to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl   - base address
**                 trfClass    - traffic class
**                               Legal range 0 to 0xFF
**                 flowLabel   - flow label
**                               Legal range 0 to 0xFFFFFF
**                 hopLimit    - hop limit
**                               Legal range 0 to 0xFF
**                 daScopeVal  - scope of multicast address
**                                 0 - RESERVED
**                                 1 - Interface-Local scope
**                                 2 - Link-Local scope
**                                 3 - RESERVED
**                                 4 - Admin-Local scope
**                                 5 - Site-Local scope
**                                 6 to 7 - unassigned
**                                 8 - Organization-Local scope
**                                 9 to 13 - unassigned
**                                 14 - Global scope
**                                 15 - RESERVED
**                 srcAddr[8]  - source address of IPv6 header
**                               Legal range 0 to 0xFFFF
**                               For example: 1762:0:0:0:0:B03:1:AF18
**                                 srcAddr[7] = 0x1762
**                                 srcAddr[6] = 0
**                                 srcAddr[5] = 0
**                                 srcAddr[4] = 0
**                                 srcAddr[3] = 0
**                                 srcAddr[2] = 0xB03
**                                 srcAddr[1] = 0x1
**                                 srcAddr[0] = 0xAF18
**                 destAddr[8] - master destination address of IPv6 header
**                               Legal range 0 to 0xFFFF
**                               For example: 1762:0:0:0:0:B03:1:AF18
**                                 destAddr[7] = 0x1762
**                                 destAddr[6] = 0
**                                 destAddr[5] = 0
**                                 destAddr[4] = 0
**                                 destAddr[3] = 0
**                                 destAddr[2] = 0xB03
**                                 destAddr[1] = 0x1
**                                 destAddr[0] = 0xAF18
** 
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetIPv6Param(struct file *fileHndl,
                                          UINT4 trfClass, UINT4 flowLabel,
                                          UINT4 hopLimit, UINT4 daScopeVal,
                                          UINT4 srcAddr[8], UINT4 destAddr[8])
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 idx;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (trfClass > 0xFF || flowLabel > 0xFFFFFF || hopLimit > 0xFF ||
        daScopeVal > 15 || daScopeVal == 0 || daScopeVal == 3 ||
        daScopeVal == 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < 8; i++) {
        if ((srcAddr[i] > 0xFFFF) || (destAddr[i] > 0xFFFF)) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0xc;
    ibndMsgSubtype = 0x04;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* traffic class and flow label */
    *pTmpIbndMsg = (trfClass << 24) | flowLabel;

    /* hop limit and DA scope */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (hopLimit << 24) | (daScopeVal << 16);

    /* source address */
    for (i = 0; i < 4; i++) {
        idx = 6 - (i*2);

        pTmpIbndMsg++;
        *pTmpIbndMsg = (srcAddr[idx+1] << 16) | srcAddr[idx];
    }

    /* master destination address */
    for (i = 0; i < 4; i++) {
        idx = 6 - (i*2);

        pTmpIbndMsg++;
        *pTmpIbndMsg = (destAddr[idx+1] << 16) | destAddr[idx];
    }
    
    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetIPv6Param */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetUnicastMasterParam
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Unicast_Master_Param 
**                 message to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl          - base address
**                 logIntMsgPeriod[3] - log base 2 of the mean period in seconds
**                                      between messages that can be granted for
**                                      the specified message type.
**                                      logIntMsgPeriod[2]: Delay_Resp
**                                      logIntMsgPeriod[1]: Sync
**                                      logIntMsgPeriod[0]: Announce
**                 durationField[3]   - maximum number of seconds for which
**                                      message type will be transmitted.
**                                      The configurable range is 60 seconds to
**                                      1000 seconds.
**                                      durationField[2]: Delay_Resp
**                                      durationField[1]: Sync
**                                      durationField[0]: Announce
** 
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetUnicastMasterParam(struct file *fileHndl,
                                                   UINT4 logIntMsgPeriod[3], 
                                                   UINT4 durationField[3])
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    for (i = 0; i < 3; i++) {
        if ((logIntMsgPeriod[i] > 0xFF) || (durationField[i] > 0x3E8) || 
            (durationField[i] < 0x3C)) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x06;
    ibndMsgSubtype = 0x1C;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* log of the inter-message period */
    *pTmpIbndMsg = (logIntMsgPeriod[0] << 24) | (logIntMsgPeriod[1] << 16) |
                   (logIntMsgPeriod[2] << 8 );

    /* duration field */
    for (i = 0 ; i < 3 ; i++) {
        pTmpIbndMsg++;
        *pTmpIbndMsg = durationField[i];
    }
    
    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetUnicastMasterParam */


/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetUnicastReq
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Unicast_Request message
**                 to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl          - base address
**                 chnlType           - channel type
**                                      0 - HSCI
**                                      1 - LSCI
**                 chnlId             - channel ID
**                                      0 to 1  - HSCI
**                                      0 to 15 - LSCI
**                 msgType            - PTP Message Type to configure.
**                                      Multiple bits can be set at the same 
**                                      time.
**                                      bit 2: Delay_Resp
**                                      bit 1: Sync
**                                      bit 0: Announce
**                                      Other bits are unused.
**                 logIntMsgPeriod[3] - log base 2 of the mean period in seconds
**                                      between messages to be requested for the 
**                                      specified message type.
**                                      logIntMsgPeriod[2]: Delay_Resp
**                                      logIntMsgPeriod[1]: Sync
**                                      logIntMsgPeriod[0]: Announce
**                 durationField[3]   - number of seconds to request for the 
**                                      specified message type. The configurable
**                                      range is 60 seconds to 1000 seconds. The
**                                      default is 300 seconds.
**                                      durationField[2]: Delay_Resp
**                                      durationField[1]: Sync
**                                      durationField[0]: Announce
**                 continuous         - continuous flag
**                                      0 - transmit the request once.
**                                      1 - repeat the request continuously.
**                 indTlv             - individual TLV
**                                      0 - send TLVs together in the same 
**                                          signalling message
**                                      1 - send TLVs individually
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetUnicastReq(struct file *fileHndl, 
                                           UINT4 chnlType, UINT4 chnlId,
                                           UINT4 msgType,
                                           UINT4 logIntMsgPeriod[3], 
                                           UINT4 durationField[3],
                                           UINT4 continuous, UINT4 indTlv)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) || (msgType > 7) || 
        (continuous > 1) || (indTlv > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < 3; i++) {
        if ((logIntMsgPeriod[i] > 0xFF) || (durationField[i] > 0x3E8) || 
            (durationField[i] < 0x3C)) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x7;
    ibndMsgSubtype = 0x1D;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* msgType and logIntMsgPeriod[3]*/
    *pTmpIbndMsg = ((msgType & 0x7) << 24) |
                   ((logIntMsgPeriod[0] & 0xFF) << 16) |
                   ((logIntMsgPeriod[1] & 0xFF) << 8) |
                   ((logIntMsgPeriod[2] & 0xFF) << 0);

    /* duration field */
    for (i = 0 ; i < 3 ; i++) {
        pTmpIbndMsg++;
        *pTmpIbndMsg = durationField[i];
    }
    
    /* chnlType, chnlId, continuous and indTlv */
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) |
                   ((chnlId & 0xFF) << 16) |
                   ((continuous & 0x1) << 8) |
                   ((indTlv & 0x1) << 0);
    
    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetUnicastReq */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetUnicastCancel
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Unicast_Cancel 
**                 message to FW for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl  - base address
**                 chnlType   - channel type
**                              0 - HSCI
**                              1 - LSCI
**                 chnlId     - channel ID
**                              0 to 1  - HSCI
**                              0 to 15 - LSCI
**                 msgType    - PTP Message Type to cancel.
**                              Multiple bits can be set at the same 
**                              time.
**                              bit 2: Delay_Resp
**                              bit 1: Sync
**                              bit 0: Announce
**                              Other bits are unused.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetUnicastCancel(struct file *fileHndl,
                                              UINT4 chnlType, UINT4 chnlId,
                                              UINT4 msgType)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;
    
    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) ||(chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) || (msgType > 7))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x1E;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* msgType and chnlType and chnlId*/
    *pTmpIbndMsg = ((msgType & 0x7) << 24) |
                   ((chnlType & 0xFF) << 16) |
                   ((chnlId & 0xFF) << 8);
    
    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHostMsgCmdCfgPtpSetUnicastCancel */

/*******************************************************************************
**  
**  hyPhy20gHostMsgCmdReqPtpGetUnicastSlvStat
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_PTP_Get_Unicast_Slave_State
**                 message to FW and returns FW response
**                 RSP_REQ_PTP_Get_Unicast_Slave_State.
**
**  INPUTS:        *fileHndl - base address
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pChnlType           - channel type
**                 *pChnlId             - channel ID
**                 *pLogIntMsgPeriod[3] - pointers to storage the values of 
**                                        log of the inter-message period.
**                                        logIntMsgPeriod[2]: Delay_Resp
**                                        logIntMsgPeriod[1]: Sync
**                                        logIntMsgPeriod[0]: Announce
**                 *pDurationField[3]   - pointer to storage the numbers of
**                                        seconds granted for the specified
**                                        message type. A value of 0 indicates 
**                                        that the request was denied.
**                                        durationField[2]: Delay_Resp
**                                        durationField[1]: Sync
**                                        durationField[0]: Announce
**                                        
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqPtpGetUnicastSlvStat(struct file *fileHndl,
                                               UINT4 *pChnlType, UINT4 *pChnlId,
                                               UINT4 *pLogIntMsgPeriod,
                                               UINT4 *pDurationField)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 *pObndMsgBody, *pTmpObndMsgBody; 
    UINT4 *pTmpLogIntMsgPeriod, *pTmpDurationField ;
    UINT4 obndMsgLen, obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size, i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (pChnlType == NULL || pChnlId == NULL || pLogIntMsgPeriod == NULL ||
        pDurationField == NULL) 
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP; 
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x03;
    ibndMsgSubtype = 0x1F;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = 0;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);
    
    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    
    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;
    pTmpLogIntMsgPeriod = pLogIntMsgPeriod;
    pTmpDurationField = pDurationField;
    
    DBG_PRINT("pTmpObndMsgBody addr=0x%x, value=0x%ld \n", (unsigned int)pTmpObndMsgBody, *pTmpObndMsgBody);    
    *pTmpLogIntMsgPeriod = (*pTmpObndMsgBody >> 16) & 0xFF;
    pTmpLogIntMsgPeriod++;
    *pTmpLogIntMsgPeriod = (*pTmpObndMsgBody >> 8) & 0xFF;
    pTmpLogIntMsgPeriod++;
    *pTmpLogIntMsgPeriod = (*pTmpObndMsgBody >> 0) & 0xFF;
    
    for (i = 0; i < 3; i++) {
        pTmpObndMsgBody++;
        *pTmpDurationField = *pTmpObndMsgBody;
        pTmpDurationField++;
    }
    
    pTmpObndMsgBody++;   
    *pChnlType  = (*pTmpObndMsgBody >> 24) & 0xFF;
    *pChnlId    = (*pTmpObndMsgBody >> 16) & 0xFF;   

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqPtpGetUnicastSlvStat */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetDefaultProp
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Default_Properties
**                 message to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl   - base address
**                 clkClass    - clockClass attribute of clock
**                 clkAccuracy - clockAccuracy attribute of clock
**                 logVar      - offsetScaledLogVariance attribute of clock
**                 priority1   - priority1 attribute of clock
**                 priority2   - priority2 attribute of clock
**                 slaveOnly   - slaveOnly attribute of clock
**                                 0 - false
**                                 1 - true
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetDefaultProp(struct file *fileHndl,
                                            UINT4 clkClass, UINT4 clkAccuracy,
                                            UINT4 logVar, UINT4 priority1,
                                            UINT4 priority2, UINT4 slaveOnly)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (clkClass > 0xFF || clkAccuracy > 0xFF || logVar > 0xFFFF ||
        priority1 > 0xFF || priority2 > 0xFF || slaveOnly > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x4;
    ibndMsgSubtype = 0x10;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* clock class, clock accuracy and log variance */
    *pTmpIbndMsg = ((clkClass & 0xFF) << 24) | ((clkAccuracy & 0xFF) << 16) |
                   ((logVar & 0xFFFF) << 0);

    /* priority1, priority2 and slaveOnly */
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((priority1 & 0xFF) << 24) | ((priority2 & 0xFF) << 16) |
                   ((slaveOnly & 0xFF) << 8);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetDefaultProp */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetTimeProp
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Time_Properties
**                 message to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl   - base address
**                 utcOffset   - current UTC offset
**                 leap        - number of seconds in the last minute of current
**                               UTC day
**                                 0 - none
**                                 1 - 61 seconds
**                                 2 - 59 seconds
**                 utcOffValid - currentUtcOffsetValid attribute
**                                 0 - false
**                                 1 - true
**                 ptpTimescale  - ptpTimescale attribute
**                                 0 - false
**                                 1 - true
**                 timeTraceable - timeTraceable attribute
**                                 0 - false
**                                 1 - true
**                 freqTraceable - frequencyTraceable attribute
**                                 0 - false
**                                 1 - true
**                 timeSrc       - time source used by grandmaster clock
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetTimeProp(struct file *fileHndl, UINT4 utcOffset,
                                         UINT4 leap, UINT4 utcOffValid,
                                         UINT4 ptpTimescale,
                                         UINT4 timeTraceable,
                                         UINT4 freqTraceable, UINT4 timeSrc)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 leapVal;
    UINT4 timePropFlags;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (utcOffset > 0xFFFF || leap > 2 || utcOffValid > 1 ||
        ptpTimescale > 1 || timeTraceable > 1 || freqTraceable > 1 ||
        timeSrc > 0xFF) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x11;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* time properties flags */
    leapVal = (leap == 0) ? 0 : (0x1 << (leap-1));
    timePropFlags = leapVal | (utcOffValid << 2) | (ptpTimescale << 3) |
                    (timeTraceable << 4) | (freqTraceable << 5);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* UTC offset, time properties flags and time source */
    *pTmpIbndMsg = ((utcOffset & 0xFFFF) << 16) | ((timePropFlags & 0xFF) << 8) |
                   ((timeSrc & 0xFF) << 0);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetTimeProp */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetAccptMstrClks
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_PTP_Set_Acceptable_Master_Clocks message to FW and
**                 waits for FW response.
**
**  INPUTS:        *fileHndl    - base address
**                 enableTable  - enable Acceptable Master table
**                                  0 - Disable the Acceptable Master table. All
**                                      Announce messages will be stored in the
**                                      Master clock database.
**                                  1 - Enable the Acceptable Master table. Only
**                                      announce messages received from a master
**                                      in the table will be stored in the
**                                      Master clock database.
**                 clearTable   - clear Acceptable Master table
**                                  0 - Do not clear the Acceptable Master table.
**                                  1 - Clear the old entries from the
**                                      Acceptable Master table.
**                 numEntries   - number of entries of acceptable master
**                                clockIdentity in the message.
**                                If enableTable == 0, set to 0.
**                                Legal range is 0-20.
**                 clkIdMsw[20] - pointer to storage for most significant word
**                                of clockIdentity (32-bit).  Each entry
**                                clkIdMsw[i] represents clockIdentity[0:3].
**                 clkIdLsw[20] - pointer to storage for least significant word
**                                of clockIdentity (32-bit).  Each entry
**                                clkIdLsw[i] represents clockIdentity[4:7].
**                 portNum[20]  - pointer to storage for portNumber (16-bit)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetAccptMstrClks(struct file *fileHndl,
                                              UINT4 enableTable,
                                              UINT4 clearTable,
                                              UINT4 numEntries,
                                              UINT4 clkIdMsw[20],
                                              UINT4 clkIdLsw[20],
                                              UINT4 portNum[20])
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (enableTable > 1 || clearTable > 1 ||  numEntries > 20 ||
       (enableTable == 0 && numEntries != 0)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < numEntries; i++) {
        if (portNum[i] > 0xFFFF) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = HYPHY20G_HOSTMSG_NUM_HEADER_WORDS + 1 + (3*numEntries);
    ibndMsgSubtype = 0x12;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* start assembling message body (ie. without header) */
    pTmpIbndMsg      = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* enable table, clear table and number of entries */
    *pTmpIbndMsg = ((enableTable & 0xFF) << 24) | ((clearTable & 0xFF) << 16) |
                   ((numEntries & 0xFF) << 8);


    /* acceptable master clockIdentity data */
    for (i = 0; i < numEntries; i++) {
        /* clockIdentity */
        pTmpIbndMsg++;
        *pTmpIbndMsg = clkIdMsw[i];
        pTmpIbndMsg++;
        *pTmpIbndMsg = clkIdLsw[i];

        /* portNumber */
        pTmpIbndMsg++;
        *pTmpIbndMsg = (portNum[i] & 0xFFFF) << 16;
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetAccptMstrClks */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetStartInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Start_Initialize
**                 message to FW and waits for a RSP_CFG response from FW.
**
**                 In order to ensure timestamps do not become out of sync
**                 with packets this function should be called before
**                 hyPhy20gLsciDataPathEnable or hyPhy20gHsciDataPathEnable
**                 (for dir == RX) for the LSCI/HSCI slices being intialized.
**
**  INPUTS:        *fileHndl    - base address
**                 lsciPortInit - LSCI port initialization.
**                                Each bit position corresponds to a LSCI
**                                port. Bit0 = LSCI #0, ..., Bit15 = LSCI #15.
**                                  0 - do not initialize port for PTP
**                                  1 - initialize port for PTP
**                 hsciPortInit - HSCI port initialization.
**                                Each bit position corresponds to a HSCI
**                                port. Bit0 = HSCI #0 and Bit1 = HSCI #1.
**                                  0 - do not initialize port for PTP
**                                  1 - initialize port for PTP
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetStartInit(struct file *fileHndl,
                                          UINT4 lsciPortInit,
                                          UINT4 hsciPortInit)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((lsciPortInit > 0xFFFF) || (hsciPortInit > 0x3)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x13;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* LSCI and HSCI port initialization */
    *pTmpIbndMsg = ((lsciPortInit & 0xFFFF) << 16) |
                   ((hsciPortInit & 0x3) << 8);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetStartInit */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqGetMstrClkDbStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_REQ_PTP_Get_Master_Clock_Database_Status message to FW
**                 and returns FW response
**                 RSP_REQ_PTP_Get_Master_Clock_Database_Status.
**
**  INPUTS:        *fileHndl   - base address
**
**                 Response message parameters:
**
**                 *status     - pointer to storage for returned status value
**                 *numEntries - pointer to storage for returned numEntries
**                               value
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqGetMstrClkDbStatus(struct file *fileHndl,
                                             UINT4 *status,
                                             UINT4 *numEntries)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (status == NULL || numEntries == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x14;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;
    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = 0;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);
    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    *status = (*pObndMsgBody >> 24) & 0xFF;
    *numEntries = (*pObndMsgBody >> 16) & 0xFF;

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqGetMstrClkDbStatus */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqPtpGetMstrClkDb
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_PTP_Get_Master_Clock_Database
**                 message to FW and returns FW response
**                 RSP_REQ_PTP_Get_Master_Clock_Database.
**
**  INPUTS:        *fileHndl   - base address
**
**                 Response message parameters:
**
**                 *pRspLength      - pointer to storage for the number of words
**                                    in pMstrClkDb in the response message
**                 *pMstrClkDb[240] - pointer to storage for returned Master
**                                    Clock database
**                                    Size of storage must be
**                                    20 x 12 words = 240 words = 240 x 4 bytes
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqPtpGetMstrClkDb(struct file *fileHndl,
                                          UINT4 *pRspLength, UINT4 *pMstrClkDb)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 *pTmpObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 rspMsgSize, rspMsgAddr;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (pMstrClkDb == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x15;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* reserved */
    *pTmpIbndMsg = 0;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    pTmpObndMsgBody = pObndMsgBody;
    rspMsgSize = (*pTmpObndMsgBody >> 16) & 0xFFFF;
    *pRspLength = rspMsgSize;

    pTmpObndMsgBody++;
    rspMsgAddr = *pTmpObndMsgBody;

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    DBG_PRINT("rspMsgSize=%lu, rspMsgAddr=0x%lx\n", rspMsgSize, rspMsgAddr);

    /* retrieve extended message */
    if (rspMsgSize != 0) {
        result = hostMsgHyPhy20gHostMsgGetExtMsg(fileHndl, rspMsgSize,
                                                 rspMsgAddr, pMstrClkDb);
        if (result) {
            /* clear outbound doorbell */
            rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
            if (rv) return rv;
            return result;
        }
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqPtpGetMstrClkDb */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetParentData
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Parent_Data message to
**                 FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl     - base address
**                 stepsRemoved  - number of communication paths traversed
**                                 between the local clock and the grandmaster
**                                 clock
**                 parentClkIdMsw- clockIdentity of the master (most
**                                 significant word, 32-bit)
**                 parentClkIdLsw- clockIdentity of the master (least
**                                 significant word, 32-bit)
**                 parentPortNum - portNumber of the master
**                 gmPriority1   - priority1 attribute of the grandmaster clock
**                 gmPriority2   - priority2 attribute of the grandmaster clock
**                 gmClkClass    - clock class attribute of the grandmaster
**                                 clock
**                 gmClkAccu     - clock accuracy attribute of the grandmaster
**                                 clock
**                 gmLogVar      - offsetScaledLogVariance attribute of the
**                                 grandmaster clock
**                 gmClkIdMsw    - clockIdentity of grandmaster clock (most
**                                 significant word, 32-bit)
**                 gmClkIdLsw    - clockIdentity of grandmaster clock (least
**                                 significant word, 32-bit)
**                 unicastDA[6]  - unicast destination MAC address
**                                  Legal range 0 to 0xFF
**                                  For example: MAC address = 12-34-56-78-9a-bc                            
**                                  unicastDA[0] = 0x12
**                                  unicastDA[1] = 0x34
**                                  unicastDA[2] = 0x56
**                                  unicastDA[3] = 0x78
**                                  unicastDA[4] = 0x9a
**                                  unicastDA[5] = 0xbc
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetParentData(struct file *fileHndl,
                                           UINT4 stepsRemoved,
                                           UINT4 parentClkIdMsw,
                                           UINT4 parentClkIdLsw,
                                           UINT4 parentPortNum,
                                           UINT4 gmPriority1, UINT4 gmPriority2,
                                           UINT4 gmClkClass, UINT4 gmClkAccu,
                                           UINT4 gmLogVar, UINT4 gmClkIdMsw,
                                           UINT4 gmClkIdLsw, 
                                           UINT4 unicastDA[6])
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size, i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (stepsRemoved > 0xFFFF || parentPortNum > 0xFFFF || gmPriority1 > 0xFF ||
        gmPriority2 > 0xFF || gmClkClass > 0xFF || gmClkAccu > 0xFF ||
        gmLogVar > 0xFFFF) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    for (i = 0; i < 6; i++) {
        if (unicastDA[i] > 0xFF) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }
    
    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0xc;
    ibndMsgSubtype = 0x16;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* steps removed */
    *pTmpIbndMsg = (stepsRemoved & 0xFFFF) << 16;

    /* parent clock identity */
    pTmpIbndMsg++;
    *pTmpIbndMsg = parentClkIdMsw;
    pTmpIbndMsg++;
    *pTmpIbndMsg = parentClkIdLsw;

    /* parent port number */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (parentPortNum & 0xFFFF) << 16;

    /* grandmaster priority 1 and 2 */
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((gmPriority1 & 0xFF) << 24) | ((gmPriority2 & 0xFF) << 16);

    /* grandmaster class, accuracy and log variance */
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((gmClkClass & 0xFF) << 24) | ((gmClkAccu & 0xFF) << 16) |
                   ((gmLogVar & 0xFFFF) << 0);

    /* grandmaster clock identity */
    pTmpIbndMsg++;
    *pTmpIbndMsg = gmClkIdMsw;
    pTmpIbndMsg++;
    *pTmpIbndMsg = gmClkIdLsw;

    /* unicast destination MAC address */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (unicastDA[0] << 24) |
                   (unicastDA[1] << 16) |
                   (unicastDA[2] <<  8) |
                   (unicastDA[3] <<  0);
    
    pTmpIbndMsg++;
    *pTmpIbndMsg = (unicastDA[4] << 24) |
                   (unicastDA[5] << 16);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetParentData */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetPortData
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Port_Data message to
**                 FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl          - base address
**                 logMinDlyReq       - log base 2 of the Delay_Req message
**                                      transmission interval. The supported
**                                      Delay_Req interval range is 
*                                       1/128s - 16s,
**                                      0xF9 =< logMinDlyReq <= 0x04.
**                 logAnnounce        - log base 2 of the Announce message
**                                      transmission interval. The supported
**                                      Announce interval range is 1/8s - 16s,
**                                      0xFD =< logAnnounce <= 0x04.
**                 announceRecTimeout - number of announceInterval that has to
**                                      pass without receipt of an Announce 
**                                      message before the occurrence of the 
**                                      event ANNOUNCE_RECEIPT_TIMEOUT_EXPIRES. 
**                                      The range should be 2 to 255.
**                 logSync            - log base 2 of the multicast Sync message 
**                                      transmission interval.The supported Sync 
**                                      interval range is 1/128s - 16s,
**                                      0xF9 =< logSync <= 0x04.
**                 dlyMech            - propagation delay measuring option.
**                                          0 - Disabled
**                                          1 - E2E, delay request-response
**                                          2 - P2P, peer delay
**                 logMinPDlyReq      - log base 2 of the Delay_Req message
**                                      transmission interval. The supported
**                                      Pdelay_Req interval range is 
*                                       1/128s - 16s,
**                                      0xF9 =< logMinPDlyReq <= 0x04.
**                 qualTimeoutN       - N multiplier used to calculate the
**                                      qualificationTimeoutInterval. 
**                                      The supported range is 0 to 255.
**                 syncRecTimeout     - Number of logSyncInterval that has to 
**                                      pass without receipt of a Sync message 
**                                      before PTSF-lossSync (Sync) is asserted. 
**                                      The supported range is 2 to 255.
**                 dlyRespRecTimeout  - Number of logDelayReqInterval that has
**                                      to pass without receipt of a Delay_Resp 
**                                      message before PTSF-lossSync 
**                                      (Delay_Resp) is asserted. 
**                                      The supported range is 2 to 255.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetPortData(struct file *fileHndl,
                                         UINT4 logMinDlyReq,
                                         UINT4 logAnnounce,
                                         UINT4 announceRecTimeout,
                                         UINT4 logSync, UINT4 dlyMech,
                                         UINT4 logMinPDlyReq,
                                         UINT4 qualTimeoutN, 
                                         UINT4 syncRecTimeout,
                                         UINT4 dlyRespRecTimeout)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 dlyMechFwVal = 0xFE; /* disable by default */
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((logMinDlyReq > 0x4 && logMinDlyReq < 0xF9) || logMinDlyReq > 0xFF ||
        (logAnnounce  > 0x4 && logAnnounce  < 0xFD) || logAnnounce  > 0xFF ||
         announceRecTimeout < 2  || announceRecTimeout > 0xFF ||
        (logSync > 0x4 && logSync < 0xF9) || logSync > 0xFF || dlyMech > 2 ||
        (logMinPDlyReq > 0x4 && logMinPDlyReq < 0xF9) || logMinPDlyReq > 0xFF ||
         qualTimeoutN > 0xFF || syncRecTimeout < 2 || syncRecTimeout > 0xFF ||
         dlyRespRecTimeout < 2 || dlyRespRecTimeout > 0xFF )
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x5;
    ibndMsgSubtype = 0x17;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* Delay mechanism enumeration in FW */
    switch (dlyMech) {
        case 0:
            /* Disabled */
            dlyMechFwVal = 0xFE;
            break;
        case 1:
            /* E2E, delay request-response */
            dlyMechFwVal = 0x1;
            break;
        case 2:
            /* P2P, peer delay */
            dlyMechFwVal = 0x2;
            break;
       default:
            /* redundant to initial argument checking */
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* logMinDlyReq, logAnnounce, announceRecTimeout, logSync */
    *pTmpIbndMsg = ((logMinDlyReq & 0xFF) << 24) |
                   ((logAnnounce & 0xFF) << 16) |
                   ((announceRecTimeout & 0xFF) << 8) |
                   ((logSync & 0xFF) << 0);

    /* dlyMech, logMinPDlyReq, qualTimeoutN and syncRecTimeout */
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((dlyMechFwVal & 0xFF) << 24) |
                   ((logMinPDlyReq & 0xFF) << 16) |
                   ((qualTimeoutN & 0xFF) << 8)|
                   ((syncRecTimeout & 0xFF) << 0);

    /* delayRespReceiptTimeout */
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((dlyRespRecTimeout & 0xFF) << 24);
    
    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetPortData */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetPortState
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Port_State message to
**                 FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl     - base address
**                 lsciState[n]  - port state for LSCI[n] (n=0:15)
**                                   0 - do not configure the port state
**                                   6 - master
**                                   7 - passive
**                                   9 - slave
**                                   All other values are reserved
**                 hsciState[n]  - port state for HSCI[n] (n=0:1)
**                                   0 - do not configure the port state
**                                   6 - master
**                                   7 - passive
**                                   9 - slave
**                                   All other values are reserved
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetPortState(struct file *fileHndl,
                                          UINT4 lsciState0, UINT4 lsciState1,
                                          UINT4 lsciState2, UINT4 lsciState3,
                                          UINT4 lsciState4, UINT4 lsciState5,
                                          UINT4 lsciState6, UINT4 lsciState7,
                                          UINT4 lsciState8, UINT4 lsciState9,
                                          UINT4 lsciState10, UINT4 lsciState11,
                                          UINT4 lsciState12, UINT4 lsciState13,
                                          UINT4 lsciState14, UINT4 lsciState15,
                                          UINT4 hsciState0, UINT4 hsciState1)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if((lsciState0>0 && lsciState0<6) || lsciState0==8 || lsciState0>9 ||
       (lsciState1>0 && lsciState1<6) || lsciState1==8 || lsciState1>9 ||
       (lsciState2>0 && lsciState2<6) || lsciState2==8 || lsciState2>9 ||
       (lsciState3>0 && lsciState3<6) || lsciState3==8 || lsciState3>9 ||
       (lsciState4>0 && lsciState4<6) || lsciState4==8 || lsciState4>9 ||
       (lsciState5>0 && lsciState5<6) || lsciState5==8 || lsciState5>9 ||
       (lsciState6>0 && lsciState6<6) || lsciState6==8 || lsciState6>9 ||
       (lsciState7>0 && lsciState7<6) || lsciState7==8 || lsciState7>9 ||
       (lsciState8>0 && lsciState8<6) || lsciState8==8 || lsciState8>9 ||
       (lsciState9>0 && lsciState9<6) || lsciState9==8 || lsciState9>9 ||
       (lsciState10>0 && lsciState10<6) || lsciState10==8 || lsciState10>9 ||
       (lsciState11>0 && lsciState11<6) || lsciState11==8 || lsciState11>9 ||
       (lsciState12>0 && lsciState12<6) || lsciState12==8 || lsciState12>9 ||
       (lsciState13>0 && lsciState13<6) || lsciState13==8 || lsciState13>9 ||
       (lsciState14>0 && lsciState14<6) || lsciState14==8 || lsciState14>9 ||
       (lsciState15>0 && lsciState15<6) || lsciState15==8 || lsciState15>9 ||
       (hsciState0>0 && hsciState0<6) || hsciState0==8 || hsciState0>9 ||
       (hsciState1>0 && hsciState1<6) || hsciState1==8 || hsciState1>9) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x14;
    ibndMsgSubtype = 0x18;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* lsciState0 to lsciState15 */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (lsciState0 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState1 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState2 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState3 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState4 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState5 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState6 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState7 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState8 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState9 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState10 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState11 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState12 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState13 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState14 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (lsciState15 & 0xFF) << 24;

    /* hsciState0 to hsciState1 */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (hsciState0 & 0xFF) << 24;
    pTmpIbndMsg++;
    *pTmpIbndMsg = (hsciState1 & 0xFF) << 24;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetPortState */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetPortEvnt
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Port_Event message to
**                 FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl  - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 enable    - enable/disable flag
**                                 0 - trigger a DESIGNATED_DISABLED event
**                                 1 - Trigger a DESIGNATED_ENABLED event
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetPortEvnt(struct file *fileHndl,
                                         UINT4 chnlType, UINT4 chnlId,
                                         UINT4 enable)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) ||  (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) || (enable > 1))
    {
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx, enable=0x%lx\n", chnlType, chnlId, enable);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x19;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* portType, portNumber and enable */
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) |
                   ((chnlId & 0xFF) << 16) |
                   ((enable & 0xFF) << 8);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetPortEvnt */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqPtpGetPortState
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_PTP_Get_Port_State message to
**                 FW and returns FW response RSP_REQ_PTP_Get_Port_State.
**
**  INPUTS:        *fileHndl    - base address
**
**                 Response message parameters:
**
**                 *pPortStates[18] - pointer to storage for returned port states.
**                                    states
**                                    Size of storage must be 18 words
**                                    (18 x 4 bytes)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqPtpGetPortState(struct file *fileHndl,
                                          UINT4 *pPortStates)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (pPortStates == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x1A;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = 0;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pPortStates);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqPtpGetPortState */

/*******************************************************************************
**  
**  hyPhy20gHostMsgCmdReqPtpGetPortRxMsgCnts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_PTP_Get_Port_Rx_Msg_Cnts
**                 message to FW and returns FW response
**                 RSP_REQ_PTP_Get_Port_Rx_Msg_Cnts.
**
**  INPUTS:        *fileHndl  - base address
**                 chnlType   - channel type
**                              0 - HSCI
**                              1 - LSCI
**                 chnlId     - channel ID
**                              0 to 1  - HSCI
**                              0 to 15 - LSCI
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pChnlType - channel type
**                 *pChnlId   - channel ID
**                 *pCnts[8]  - pointers to storage 8 counters. Each counter 
**                              saturates at 0xFFFFFFFF. The counter value will
**                              be cleared after a read.(size of storage must be
**                              8 x 4 bytes)
**                              *pCnts[0]: pointer to storage the number of
**                                         Sync messages received. 
**                              *pCnts[1]: pointer to storage the number of
**                                         Delay_Req messages received.
**                              *pCnts[2]: pointer to storage the number of
**                                         Pdelay_Req messages received.
**                              *pCnts[3]: pointer to storage the number of
**                                         Pdelay_Resp messages received.
**                              *pCnts[4]: pointer to storage the number of
**                                         Follow_Up messages received.
**                              *pCnts[5]: pointer to storage the number of
**                                         Delay_Resp messages received.
**                              *pCnts[6]: pointer to storage the number of
**                                         Pdelay_Resp_Follow_Up messages 
**                                         received.
**                              *pCnts[7]: pointer to storage the number of 
**                                         Announce messages received.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqPtpGetPortRxMsgCnts(struct file *fileHndl,
                                              UINT4 chnlType, UINT4 chnlId,
                                              UINT4 *pChnlType, UINT4 *pChnlId,
                                              UINT4 *pCnts)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 *pObndMsgBody, *pTmpObndMsgBody, *pTmpCnts;
    UINT4 obndMsgLen, obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size, i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pCnts == NULL || pChnlType == NULL || pChnlId == NULL)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP; 
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x03;
    ibndMsgSubtype = 0x05;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and ID */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) | 
                   ((chnlId & 0xFF) << 16);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);
    
    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    
    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;
    pTmpCnts = pCnts;
    
    *pChnlType  = (*pTmpObndMsgBody >> 24) & 0xFF;
    *pChnlId    = (*pTmpObndMsgBody >> 16) & 0xFF;    
    pTmpObndMsgBody++;
    
    for (i = 0; i < 8; i++) {
        *pTmpCnts = *pTmpObndMsgBody;
        pTmpCnts++;
        pTmpObndMsgBody++;
    }

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqPtpGetPortRxMsgCnts */


/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetPassPortParent
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Passive_Port_Parent
**                 message to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 clkIdMsw  - clockIdentity of the master that the
**                             specified passive channel should listen to
**                             (most significant word, 32-bit)
**                 clkIdLsw  - clockIdentity of the master that the
**                             specified passive channel should listen to
**                             (least significant word, 32-bit)
**                 portNum   - portNumber of the master that the specified
**                             passive channel should listen to
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetPassPortParent(struct file *fileHndl,
                                               UINT4 chnlType, UINT4 chnlId,
                                               UINT4 clkIdMsw, UINT4 clkIdLsw,
                                               UINT4 portNum)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
   if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
       (chnlType == 1 && chnlId > 15) || portNum > 0xFFFF) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x6;
    ibndMsgSubtype = 0x1B;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* portType and portIndex */
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) | ((chnlId & 0xFF) << 16);

    /* clock identity */
    pTmpIbndMsg++;
    *pTmpIbndMsg = clkIdMsw;
    pTmpIbndMsg++;
    *pTmpIbndMsg = clkIdLsw;

    /* port number */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (portNum & 0xFFFF) << 16;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetPassPortParent */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgSerdesSetEquTap
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_SERDES_Set_Equalizer_Tap
**                 message to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 groupNum  - SERDES group number
**                                 0 - XFI
**                                 1 - PIF
**                                 2 - PCIe Lane
**                                 3 - Backplane
**                 linkNum   - SERDES link number
**                                 0 to 1  - XFI
**                                 0 to 7  - PIF
**                                 0 to 4  - PCIe Lane
**                                 0 to 31 - Backplane
**                 numIter   - number of iteration to use in the equalizer
**                             adaptation routine before finalizing the
**                             equalizer tap value (please refer to FW user
**                             manual, PMC-2091015, for recommended values)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgSerdesSetEquTap(struct file *fileHndl,
                                          UINT4 groupNum, UINT4 linkNum,
                                          UINT4 numIter)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 fwGroupNumVal = 0x3; /* default to XFI */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((groupNum > 3) ||
        (groupNum == 0 && linkNum > 1) ||
        (groupNum == 1 && linkNum > 7) ||
        (groupNum == 2 && linkNum > 4) ||
        (groupNum == 3 && linkNum > 31))
    {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* determine FW value for group number */
    switch (groupNum) {
        case 0:
            /* XFI */
            fwGroupNumVal = 0x3;
            break;
        case 1:
            /* PIF */
            fwGroupNumVal = 0x4;
            break;
        case 2:
            /* PCIe Lane */
            fwGroupNumVal = 0x5;
            break;
        case 3:
            /* Backplane */
            fwGroupNumVal = 0x6;
            break;
        default:
            /* redundant to initial argument checking */
            return HYPHY20G_ERR_INVALID_ARG;
    }



    /* set message header parameters */
    ibndModID = MODID_NGSERDES;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x5;
    ibndMsgSubtype = 0x01;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* SERDES group number */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = fwGroupNumVal & 0xFFFFFFFF;

    /* SERDES link number */
    pTmpIbndMsg++;
    *pTmpIbndMsg = linkNum & 0xFFFFFFFF;

    /* number of iterations */
    pTmpIbndMsg++;
    *pTmpIbndMsg = numIter & 0xFFFFFFFF;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgSerdesSetEquTap */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSetLanToWanConsActTable
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Set_LanToWan_Conseq_Action_Table message to FW
**                 and waits for a RSP_CFG response from FW.
**
**                 Consequential action generated for detected defect:
**                      0 - Consequential action disabled
**                      1 - CMF with UPI value 0x01 (Client Signal Fail - Loss
**                          of Client Signal)
**                      2 - CMF with UPI value 0x02 (Client Signal Fail - Loss
**                          of Character Synchronization)
**                      3 - CMF with UPI value 0x03 (Client Defect Clear
**                          Indication - DCI)
**                      4 - CMF with UPI value 0x04 (Client Forward Defect
**                          Indication - FDI)
**                      5 - CMF with UPI value 0x05 (Client Reverse Defect
**                          Indication - RDI)
**                      6 - PDI-P
**
**  INPUTS:        *fileHndl           - base address
**                 tableIdx            - action table index
**                                       Legal range 1 to 15
**                 losMonVAct          - consequential action for LOS_MON_V
**                                       defect (see DESCRIPTION for legal
**                                       range)
**                 rxDlolLsciMonVAct   - consequential action for
**                                       RX_DLOL_LSCI_MON_V defect
**                                       (see DESCRIPTION for legal range)
**                 rxDlolHsciMonVAct   - consequential action for
**                                       RX_DLOL_HSCI_MON_V defect
**                                       (see DESCRIPTION for legal range)
**                 rxGbeLosSyncVAct    - consequential action for
**                                       RX_GBE_LOS_SYNC_V defect
**                                       (see DESCRIPTION for legal range)
**                 rxGsup73LosSyncVAct - consequential action for
**                                       RX_GSUP73_LOS_SYNC_V defect
**                                       (see DESCRIPTION for legal range)
**                 rxGsup73HberVAct    - consequential action for
**                                       RX_GSUP73_HBER_V defect
**                                       (see DESCRIPTION for legal range)
**                 rxGsup73LnkFailVAct - consequential action for
**                                       RX_GSUP73_LNKFAIL_V defect
**                                       (see DESCRIPTION for legal range)
**                 rxFeLosSyncVAct     - consequential action for
**                                       RX_FE_LOS_SYNC_V defect
**                                       (see DESCRIPTION for legal range)
**                 rxFeFiVAct          - consequential action for RX_FEFI_V
**                                       defect (see DESCRIPTION for legal
**                                       range)
**                 swAlarmVAct         - consequential action for SOFT_ALARM_V
**                                       action (see DESCRIPTION for legal
**                                       range)
**                 rxFeGeLnkFailVAct   - consequential action for
**                                       RX_FEGE_LINK_FAIL_V defect (see
**                                       DESCRIPTION for legal range)
**                 priority1Act        - highest priority consequential action
**                                       (see DESCRIPTION for legal range)
**                 priority2Act        - 2nd highest priority consequential
**                                       action (see DESCRIPTION for legal
**                                       range)
**                 priority3Act        - 3rd highest priority consequential
**                                       action (see DESCRIPTION for legal
**                                       range)
**                 priority4Act        - 4th highest priority consequential
**                                       action (see DESCRIPTION for legal
**                                       range)
**                 priority5Act        - 5th highest priority consequential
**                                       action (see DESCRIPTION for legal
**                                       range)
**                 priority6Act        - lowest priority consequential action
**                                       (see DESCRIPTION for legal range)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
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
                                                     UINT4 priority6Act)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((tableIdx < 1) || (tableIdx > 15) || (losMonVAct > 6) ||
        (rxDlolLsciMonVAct > 6) || (rxDlolHsciMonVAct > 6)   ||
        (rxGbeLosSyncVAct > 6)  || (rxGsup73LosSyncVAct > 6) ||
        (rxGsup73HberVAct > 6)  || (rxGsup73LnkFailVAct > 6) ||
        (rxFeLosSyncVAct > 6)   || (rxFeFiVAct > 6)   || (swAlarmVAct > 6)  ||
        (rxFeGeLnkFailVAct > 6) || (priority1Act > 6) || (priority2Act > 6) ||
        (priority3Act > 6)      || (priority4Act > 6) || (priority5Act > 6) ||
        (priority6Act > 6))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x7;
    ibndMsgSubtype = 0x01;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* action table index, los, rx_dlol_lsci, rx_dlol_hsci */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (tableIdx          << 24) |
                   (losMonVAct        << 16) |
                   (rxDlolLsciMonVAct << 8 ) |
                   (rxDlolHsciMonVAct << 0 );

    /* rx_gbe_los_sync, rx_gsup73_los_sync, rx_gsup73_hber, rx_gsup73_lnkfail */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (rxGbeLosSyncVAct    << 24) |
                   (rxGsup73LosSyncVAct << 16) |
                   (rxGsup73HberVAct    << 8 ) |
                   (rxGsup73LnkFailVAct << 0 );

    /* rx_fe_los_sync, rx_fefi, sw_alarm, rx_fege_lnkfail */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (rxFeLosSyncVAct   << 24) |
                   (rxFeFiVAct        << 16) |
                   (swAlarmVAct       << 8 ) |
                   (rxFeGeLnkFailVAct << 0 );

    /* priority1, priority2, priority3, priority4 */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (priority1Act << 24) |
                   (priority2Act << 16) |
                   (priority3Act << 8 ) |
                   (priority4Act << 0 );

    /* priority5, priority6 */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (priority5Act << 24) |
                   (priority6Act << 16);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSetLanToWanConsActTable */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSetLanToWanPortMapTable
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Set_LanToWan_Port_Mapping_Table message to FW for
**                 a single channel and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 gfpIdx    - GFP instance
**                                 0 - Slice 'A'
**                                 1 - Slice 'B'
**                 gfpChnl   - GFP channel number
**                             Legal range 0x0 to 0x3F (0-63)
**                 serdes    - SERDES interface mapping
**                                 0 to 15  - SFP ports 0 to 15
**                                 16 to 17 - XFI ports 0 to 1
**                                 18       - Dedicated SFI interface
**                                 19       - Shared SFI interface
**                 vcatGrp   - VCAT group number
**                                 Legal range 0x0 to 0x3F (0-63)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgCsfSetLanToWanPortMapTable(struct file *fileHndl,
                                                     UINT4 chnlType,
                                                     UINT4 chnlId,
                                                     UINT4 gfpIdx,
                                                     UINT4 gfpChnl,
                                                     UINT4 serdes,
                                                     UINT4 vcatGrp)
{
    UINT4 chnlCnt = 1;
    UINT4 numParamsPerChnl = 6;
    UINT4 *pCfgMem, *pTmpCfgMem;
    UINT4 *pChnlType, *pChnlId, *pGfpIdx, *pGfpChnl, *pSerdes, *pVcatGrp;
    UINT4 size;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) ||(chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (gfpIdx > 1) || (gfpChnl > 63) ||
        (serdes > 19) || (vcatGrp > 63))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* allocate memory */
    size = sizeof(UINT4) * chnlCnt * numParamsPerChnl;
    pCfgMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pCfgMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pTmpCfgMem = pCfgMem;
    pChnlType  = pTmpCfgMem;
    *pChnlType = chnlType;

    pTmpCfgMem++;
    pChnlId  = pTmpCfgMem;
    *pChnlId = chnlId;

    pTmpCfgMem++;
    pGfpIdx  = pTmpCfgMem;
    *pGfpIdx = gfpIdx;

    pTmpCfgMem++;
    pGfpChnl  = pTmpCfgMem;
    *pGfpChnl = gfpChnl;

    pTmpCfgMem++;
    pSerdes  = pTmpCfgMem;
    *pSerdes = serdes;

    pTmpCfgMem++;
    pVcatGrp  = pTmpCfgMem;
    *pVcatGrp = vcatGrp;

    /* send message and check response */
    result = hostMsgHyPhy20gHostMsgCmdCfgCsfSetLanToWanPortMapTable(fileHndl,
                                                                    chnlCnt,
                                                                    pChnlType,
                                                                    pChnlId,
                                                                    pGfpIdx,
                                                                    pGfpChnl,
                                                                    pSerdes,
                                                                    pVcatGrp);
    if (result) {
        sysHyPhy20gMemFree(pCfgMem);
        return result;
    }

    sysHyPhy20gMemFree(pCfgMem);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSetLanToWanPortMapTable */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSetLanToWanPortActTable
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Set_LanToWan_Port_Action_Table message to FW for
**                 a single channel and waits for a RSP_CFG response from FW.
**
**                 If linear frame extension header is used (exi == 1), the
**                 values for the CID and eHEC fields must be specified, while
**                 the spare field is automatically forced to zero.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlType    - channel type
**                                   0 - HSCI
**                                   1 - LSCI
**                 chnlId      - channel ID
**                                   0 to 1  - HSCI
**                                   0 to 15 - LSCI
**                 tableIdx    - action table index
**                               Legal range 0 to 15 (table 0 is pre-defined by
**                               FW to disable all consequential actions)
**                 sendDci     - 0 - do not send DCI when defects are cleared
**                               1 - send DCI when defects are cleared
**                 exi         - extension header identifier
**                                   0 - null extension header
**                                   1 - linear frame
**                 cid         - For exi == 1 (otherwise set to 0)
**                               Channel ID (CID) field (0x0 to 0xFF)
**                 eHec        - For exi == 1 (otherwise set to 0)
**                               Extension HEC field (0x0 to 0xFFFF)
**                 genKpAlvCmf - generate Keep-Alive CMF every 100ms
**                                   0 - do not generate Keep-Alive CMF
**                                   1 - generate Keep-Alive CMF with UPI and
**                                       8-byte client payload data
**                 usePyld     - use 8-byte client payload data with regular
**                               CSF (CMF with UPI = 1 or 2)
**                                   0 - do not include client payload
**                                   1 - include 8-byte client payload specified
**                                       by cmfPyld
**                 genPyldFcs  - For usePyld == 1 (otherwise set to 0)
**                               Generate 4-byte payload FCS
**                                   0 - do not generate payload FCS
**                                   1 - generate payload FCS
**                 upi         - For genKpAlvCmf == 1 (otherwise set to 0)
**                               UPI value used for Keep-Alive CMF
**                               Legal range 0x0 to 0xFF
**                 cmfPyld[8]  - If genKpAlvCmf == 0 and usePyld == 0, set to 0.
**                               8-byte client payload data used in Keep-Alive
**                               CMF and/or regular CSF.
**                                   cmfPyld[0] - payload octet N in CMF
**                                   cmfPyld[1] - payload octet N+1 in CMF
**                                   ...
**                                   cmfPyld[7] - payload octet N+7 in CMF
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
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
                                                     UINT4 cmfPyld[8])
{
    UINT4 chnlCnt = 1;
    UINT4 numParamsPerChnl = 19;
    UINT4 *pCfgMem, *pTmpCfgMem;
    UINT4 *pChnlType, *pChnlId, *pTableIdx, *pSendDci, *pExi, *pCid, *pEHec;
    UINT4 *pGenKpAlvCmf, *pUsePyld, *pGenPyldFcs, *pUpi, *pCmfPyld, *pTmpCmfPyld;
    UINT4 size;
    UINT4 i;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) ||(chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (tableIdx > 15) || (sendDci > 1) ||
        (exi > 1) || (exi == 1 && cid > 0xFF) || (exi == 1 && eHec > 0xFFFF) ||
        (genKpAlvCmf > 1) || (usePyld > 1) || (genPyldFcs > 1) || (upi > 0xFF))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < 8; i++) {
        if (cmfPyld[i] > 0xFF) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* allocate memory */
    size = sizeof(UINT4) * chnlCnt * numParamsPerChnl;
    pCfgMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pCfgMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pTmpCfgMem = pCfgMem;
    pChnlType  = pTmpCfgMem;
    *pChnlType = chnlType;

    pTmpCfgMem++;
    pChnlId  = pTmpCfgMem;
    *pChnlId = chnlId;

    pTmpCfgMem++;
    pTableIdx  = pTmpCfgMem;
    *pTableIdx = tableIdx;

    pTmpCfgMem++;
    pSendDci  = pTmpCfgMem;
    *pSendDci = sendDci;

    pTmpCfgMem++;
    pExi  = pTmpCfgMem;
    *pExi = exi;

    pTmpCfgMem++;
    pCid  = pTmpCfgMem;
    *pCid = cid;

    pTmpCfgMem++;
    pEHec  = pTmpCfgMem;
    *pEHec = eHec;

    pTmpCfgMem++;
    pGenKpAlvCmf  = pTmpCfgMem;
    *pGenKpAlvCmf = genKpAlvCmf;

    pTmpCfgMem++;
    pUsePyld  = pTmpCfgMem;
    *pUsePyld = usePyld;

    pTmpCfgMem++;
    pGenPyldFcs  = pTmpCfgMem;
    *pGenPyldFcs = genPyldFcs;

    pTmpCfgMem++;
    pUpi  = pTmpCfgMem;
    *pUpi = upi;

    pTmpCfgMem++;
    pCmfPyld = pTmpCfgMem;
    pTmpCmfPyld = pCmfPyld;
    for (i = 0; i < 8; i++) {
        *pTmpCmfPyld = cmfPyld[i];
        pTmpCmfPyld++;
    }

    /* send message and check response */
    result = hostMsgHyPhy20gHostMsgCmdCfgCsfSetLanToWanPortActTable(fileHndl,
                                                                    chnlCnt,
                                                                    pChnlType,
                                                                    pChnlId,
                                                                    pTableIdx,
                                                                    pSendDci,
                                                                    pExi,
                                                                    pCid,
                                                                    pEHec,
                                                                    pGenKpAlvCmf,
                                                                    pUsePyld,
                                                                    pGenPyldFcs,
                                                                    pUpi,
                                                                    pCmfPyld);
    if (result) {
        sysHyPhy20gMemFree(pCfgMem);
        return result;
    }

    sysHyPhy20gMemFree(pCfgMem);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSetLanToWanPortActTable */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSetLanToWanCsfFrmGenInt
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Set_LanToWan_CSFframe_Gen_Interval message to FW
**                 and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 interval  - CSF insertion interval (interval*100ms)
**                             Legal range 1 to 10
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgCsfSetLanToWanCsfFrmGenInt(struct file *fileHndl,
                                                     UINT4 interval)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((interval < 1) || (interval > 10))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x04;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* interval */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = interval << 24;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSetLanToWanCsfFrmGenInt */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSetC2Byte
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_CSF_Set_C2_Byte message to FW
**                 and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 vcatGrp   - VCAT group number (SONET/SDH can only use GFP0
**                             to map into VCAT slice 0)
**                             Legal range 0x0 to 0x3F, 0xFF (0-63, 255)
**                             0xFF indicates STS1 does not contain PSL/PDI-P
**                 thppMsk   - HOPP THPP bit map
**                             Bit[3:0] corresponds to HOPP THPP[3:0]
**                 sts3Msk   - STS3[15:0] bit map
**                             Set any bits to set C2 of the corresponding STS3
**                 sts1Msk   - STS1[2:0] bit map
**                             Set any bits to set C2 of the corresponding STS1
**                 pdiP      - PDI-P defect (C2 byte)
**                             Legal range 0xE1 to 0xFC
**                 psl       - path signal label (C2 byte) value for normal
**                             operation
**                             Legal range 0x0 to E0, 0xFD to 0xFF
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgCsfSetC2Byte(struct file *fileHndl, UINT4 vcatGrp,
                                       UINT4 thppMsk, UINT4 sts3Msk,
                                       UINT4 sts1Msk, UINT4 pdiP, UINT4 psl)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (((vcatGrp > 63) && (vcatGrp != 255)) || (thppMsk > 0xF) ||
        (sts3Msk > 0xFFFF) || (sts1Msk > 0x7) || (pdiP < 0xE1) ||
        (pdiP > 0xFC) || (psl > 0xFF) || ((psl > 0xE0) && (psl < 0xFD)))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x4;
    ibndMsgSubtype = 0x05;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* vcat group, thpp slice and sts3 bit mask */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (vcatGrp << 24) |
                   (thppMsk << 16) |
                   (sts3Msk << 0);

    /* sts1 bit mask, c2 pdip and c2 psl */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (sts1Msk << 24) |
                   (pdiP    << 16) |
                   (psl     << 8);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSetC2Byte */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSetLanToWanUpdateTblNotif
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Set_LanToWan_Update_Table_Notification message
**                 to FW and waits for a RSP_CFG response from FW.
**
**                 The following functions should be called before using this
**                 function:
**                     hyPhy20gHostMsgCmdCfgCsfSetLanToWanConsActTable
**                     hyPhy20gHostMsgCmdCfgCsfSetLanToWanPortMapTable
**                     hyPhy20gHostMsgCmdCfgCsfSetLanToWanPortActTable
**                     hyPhy20gHostMsgCmdCfgCsfSetLanToWanCsfFrmGenInt
**                     hyPhy20gHostMsgCmdCfgCsfSetC2Byte
**
**  INPUTS:        *fileHndl         - base address
**                 hsciChnlUpdateMsk - consequential or port mapping table
**                                     update for HSCI
**                                   - Bits[1:0] corresponds to HSCI[1:0]
**                                         0 - no update
**                                         1 - update channel
**                 lsciChnlUpdateMsk - consequential or port mapping table
**                                     update for LSCI
**                                   - Bits[15:0] corresponds to LSCI[15:0]
**                                         0 - no update
**                                         1 - update channel
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgCsfSetLanToWanUpdateTblNotif(struct file *fileHndl,
                                                       UINT4 hsciChnlUpdateMsk,
                                                       UINT4 lsciChnlUpdateMsk)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((hsciChnlUpdateMsk > 0x3) || (lsciChnlUpdateMsk > 0xFFFF))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x4;
    ibndMsgSubtype = 0x06;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* hsci channel update mask */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = hsciChnlUpdateMsk;

    /* lsci channel update mask */
    pTmpIbndMsg++;
    *pTmpIbndMsg = lsciChnlUpdateMsk;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSetLanToWanUpdateTblNotif */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSendLanToWanCmfHdlcFrm
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Send_LanToWan_CMF_HDLC_Frame message to FW
**                 and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl   - base address
**                 gfpIdx      - GFP instance
**                                   0 - Slice 'A'
**                                   1 - Slice 'B'
**                 gfpChnl     - GFP channel number
**                               Legal range 0x0 to 0x3F (0-63)
**                 type        - frame type
**                                   0 - CMF
**                                   1 - HDLC control frames
**                 length      - frame length (number of bytes in frame data)
**                               Legal range 5 to 100
**                 pData[100]  - frame data (100 x 1 byte)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgCsfSendLanToWanCmfHdlcFrm(struct file *fileHndl,
                                                    UINT4 gfpIdx, UINT4 gfpChnl,
                                                    UINT4 type, UINT4 length,
                                                    UINT4 pData[100])
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 *pTmpData;
    UINT4 size, numDataWrds, count, j;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((gfpIdx > 1) || (gfpChnl > 63) || (type > 1) ||
        (length < 5) || (length > 100))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pData == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* determine number of words pData will occupy */
    numDataWrds = length / 4;
    if ((length % 4) != 0) {
        numDataWrds++;
    }

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = HYPHY20G_HOSTMSG_NUM_HEADER_WORDS + 1 + numDataWrds;
    ibndMsgSubtype = 0x07;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* gfp index, gfp channel, frame length and frame type */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (gfpIdx  << 24) |
                   (gfpChnl << 16) |
                   (length  << 8 ) |
                   (type    << 0 );

    /* frame data */
    pTmpIbndMsg++;
    *pTmpIbndMsg = 0;
    pTmpData = pData;
    count = j = 0;
    do {
        if (j > 3) {
            j = 0;
            pTmpIbndMsg++;
            *pTmpIbndMsg = 0;
        }

        *pTmpIbndMsg |= (*pTmpData << (24-(j*8)));

        j++;
        count++;
        pTmpData++;
    } while (count < length);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSendLanToWanCmfHdlcFrm */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqCsfGetLanToWanConsActStat
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_REQ_CSF_Get_LanToWan_Conseq_Action_Status message to FW
**                 and returns FW response
**                 RSP_REQ_CSF_Get_LanToWan_Conseq_Action_Status.
**
**                 Returned consequential action status:
**                     Bit[6]   - PDI-P
**                     Bit[5:1] - CMF types 5 to 1
**                     Bit[0]   - Consequential action disabled
**
**  INPUTS:        *fileHndl  - base address
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pHsci[2]  - pointer to consequential action status for
**                              HSCI[1:0] (2 x 1 byte)
**                 *pLsci[16] - pointer to consequential action status for
**                              LSCI[15:0] (16 x 1 byte)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqCsfGetLanToWanConsActStat(struct file *fileHndl,
                                                    UINT4 *pHsci, UINT4 *pLsci)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 *pTmpObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size, count, j;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((pHsci == NULL) || (pLsci == NULL)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_REQ;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x08;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = 0x0;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pTmpObndMsgBody = pObndMsgBody;

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;
    *pHsci     = (*pTmpObndMsgBody >> 24) & 0xFF;
    *(pHsci+1) = (*pTmpObndMsgBody >> 16) & 0xFF;

    pTmpObndMsgBody++;
    count = j = 0;
    do {
        if (j > 3) {
            j = 0;
            pTmpObndMsgBody++;
            count++;
        }

        *pLsci = (*pTmpObndMsgBody >> (24-(j*8))) & 0xFF;

        j++;
        pLsci++;
    } while (count < (obndMsgLen - 3));

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqCsfGetLanToWanConsActStat */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSetWanToLanMapTable
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Set_WanToLan_Mapping_Table message to FW for
**                 a single channel and waits for a RSP_CFG response from FW.
**
**                 hyPhy20gHostMsgCmdCfgCsfSetWanToLanUpdateTblNotif should be
**                 called after this function so that new mapping table may
**                 take effect.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 gfpIdx    - GFP instance
**                                 0 - Slice 'A'
**                                 1 - Slice 'B'
**                 gfpChnl   - GFP channel number
**                             Legal range 0x0 to 0x3F (0-63)
**                 action    - consequential action
**                                 0 - No Firmware action required
**                                 1 - Transparent GbE
**                                 2 - G.Sup43 Clause 7.3 10 GbE
**                                 3 - MAC Terminated GbE
**                                 4 - Insert local fault ordered set in MAC 
**                                     Terminated 10 GbE
**                                 5 - MAC Terminated FE - User Pattern
**                                 6 - MAC Terminated FE - FEFI
**                                 7 - Insert remote fault ordered set in MAC 
**                                     Terminated 10 GbE
**                 upi       - CMF UPI
**                             Legal range 0 to 255
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgCsfSetWanToLanMapTable(struct file *fileHndl,
                                                 UINT4 chnlType, UINT4 chnlId,
                                                 UINT4 gfpIdx, UINT4 gfpChnl,
                                                 UINT4 action, UINT4 upi)
{
    UINT4 chnlCnt = 1;
    UINT4 numParamsPerChnl = 6;
    UINT4 *pCfgMem, *pTmpCfgMem;
    UINT4 *pChnlType, *pChnlId, *pGfpIdx, *pGfpChnl, *pAction, *pUpi;
    UINT4 size;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) ||(chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (gfpIdx > 1) || (gfpChnl > 63) ||
        (action > 7) || (upi > 255))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* allocate memory */
    size = sizeof(UINT4) * chnlCnt * numParamsPerChnl;
    pCfgMem = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pCfgMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pTmpCfgMem = pCfgMem;
    pChnlType  = pTmpCfgMem;
    *pChnlType = chnlType;

    pTmpCfgMem++;
    pChnlId  = pTmpCfgMem;
    *pChnlId = chnlId;

    pTmpCfgMem++;
    pGfpIdx  = pTmpCfgMem;
    *pGfpIdx = gfpIdx;

    pTmpCfgMem++;
    pGfpChnl  = pTmpCfgMem;
    *pGfpChnl = gfpChnl;

    pTmpCfgMem++;
    pAction  = pTmpCfgMem;
    *pAction = action;

    pTmpCfgMem++;
    pUpi  = pTmpCfgMem;
    *pUpi = upi;

    /* send message and check response */
    result = hostMsgHyPhy20gHostMsgCmdCfgCsfSetWanToLanMapTable(fileHndl,
                                                                chnlCnt,
                                                                pChnlType,
                                                                pChnlId,
                                                                pGfpIdx,
                                                                pGfpChnl,
                                                                pAction,
                                                                pUpi);
    if (result) {
        sysHyPhy20gMemFree(pCfgMem);
        return result;
    }

    sysHyPhy20gMemFree(pCfgMem);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSetWanToLanMapTable */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSetWanToLanUpdateTblNotif
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Set_WanToLan_Update_Table_Notification message
**                 to FW for a single channel and waits for a RSP_CFG response
**                 from FW.
**
**                 This function will set the FW message gfp_ch_update bit of
**                 the specified GFP channel.
**
**                 The following functions should be called before this
**                 function:
**                     hostMsgHyPhy20gHostMsgCmdCfgCsfSetWanToLanMapTable
**                     hyPhy20gHostMsgCmdCfgCsfSetWanToLanSetDefCondStopParam
**
**  INPUTS:        *fileHndl - base address
**                 gfpIdx    - GFP instance
**                                 0 - Slice 'A'
**                                 1 - Slice 'B'
**                 gfpChnl   - GFP channel number
**                             Legal range 0x0 to 0x3F (0-63)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgCsfSetWanToLanUpdateTblNotif(struct file *fileHndl,
                                                       UINT4 gfpIdx,
                                                       UINT4 gfpChnl)
{
    UINT4 gfp0ChnlUpdateMskMsb = 0;
    UINT4 gfp0ChnlUpdateMskLsb = 0;
    UINT4 gfp1ChnlUpdateMskMsb = 0;
    UINT4 gfp1ChnlUpdateMskLsb = 0;
    UINT4 msbBitMsk = 0;
    UINT4 lsbBitMsk = 0;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((gfpIdx > 1) || (gfpChnl > 63)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    if (gfpChnl < 32) {
        lsbBitMsk = 0x1 << gfpChnl;
    } else {
        msbBitMsk = 0x1 << (gfpChnl - 32);
    }

    switch (gfpIdx) {
        case 0:
            /* GFP A */
            gfp0ChnlUpdateMskMsb = msbBitMsk;
            gfp0ChnlUpdateMskLsb = lsbBitMsk;
            break;
        case 1:
            /* GFP B */
            gfp1ChnlUpdateMskMsb = msbBitMsk;
            gfp1ChnlUpdateMskLsb = lsbBitMsk;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    result = hostMsgHyPhy20gHostMsgCmdCfgCsfSetWanToLanUpdateTblNotif(fileHndl,
                                                          gfp0ChnlUpdateMskMsb,
                                                          gfp0ChnlUpdateMskLsb,
                                                          gfp1ChnlUpdateMskMsb,
                                                          gfp1ChnlUpdateMskLsb);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSetWanToLanUpdateTblNotif */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSetWanToLanSetDefCondStopParam
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Set_Defect_Condition_Stop_Param message to FW
**                 and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 interval  - CSF insertion interval (interval*1000ms)
**                             Legal range 1 to 10
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgCsfSetWanToLanSetDefCondStopParam(struct file *fileHndl,
                                                            UINT4 interval)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((interval < 1) || (interval > 10))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x0B;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* interval */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = interval << 24;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSetWanToLanSetDefCondStopParam */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqCsfGetWanToLanCmfInd
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_REQ_CSF_Get_WanToLan_CMF_Indication message to FW
**                 and returns FW response
**                 RSP_REQ_CSF_Get_WanToLan_CMF_Indication.
**
**                 The returned CMF indication status contains the UPI value 
**                 of the last received CMF.  
**                 Note that this value is cleared on read.
**                     Bit[7] - Proprietary CMF (UPI=224-254)
**                     Bit[6] - Reserved for future use (UPI=6-223)
**                     Bit[5] - UPI=5
**                     Bit[4] - UPI=4
**                     Bit[3] - UPI=3
**                     Bit[2] - UPI=2
**                     Bit[1] - UPI=1
**                     Bit[0] - reserved by G.7041 (i.e. UPI=0, 255)
**
**  INPUTS:        *fileHndl  - base address
**                 gfpIdx     - GFP instance
**                                 0 - Slice 'A'
**                                 1 - Slice 'B'
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pUpiGfp[64] - pointer to CMF indication status for
**                                GFP channels (64 x 1 byte)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqCsfGetWanToLanCmfInd(struct file *fileHndl,
                                               UINT4 gfpIdx, UINT4 *pUpiGfp)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 *pTmpObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size, count, j;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (gfpIdx > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pUpiGfp == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_REQ;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x0C;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = gfpIdx << 24;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pTmpObndMsgBody = pObndMsgBody;

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;

    /* check RX_GFP_idx */
    if (((*pTmpObndMsgBody >> 24) & 0xFF) != gfpIdx) {
        sysHyPhy20gMemFree(pObndMsgBody);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_RSP_REQ\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_RSP_REQ;
    }

    pTmpObndMsgBody++;
    count = j = 0;
    do {
        if (j > 3) {
            j = 0;
            pTmpObndMsgBody++;
            count++;
        }

        *pUpiGfp = (*pTmpObndMsgBody >> (24-(j*8))) & 0xFF;

        j++;
        pUpiGfp++;
    } while (count < (obndMsgLen - 3));

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqCsfGetWanToLanCmfInd */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqCsfGetWanToLanPropCmf
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_REQ_CSF_Get_WanToLan_ProprietaryCMF message to FW
**                 and returns FW response
**                 RSP_REQ_CSF_Get_WanToLan_ProprietaryCMF.
**
**  INPUTS:        *fileHndl - base address
**                 gfpIdx    - GFP instance
**                                0 - Slice 'A'
**                                1 - Slice 'B'
**                 gfpChnl   - GFP channel number
**                             Legal range 0x0 to 0x3F (0-63)
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pGfpIdx    - GFP instance
**                 *pGfpChnl   - GFP channel
**                 *pUpiType   - CMF type
**                 *pLength    - frame length (number of valid bytes in returned
**                               frame data, maximum 100 bytes)
**                 *pData[100] - frame data (pointer size must be 100 x 1 byte)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqCsfGetWanToLanPropCmf(struct file *fileHndl,
                                                UINT4 gfpIdx, UINT4 gfpChnl,
                                                UINT4 *pGfpIdx, UINT4 *pGfpChnl,
                                                UINT4 *pUpiType, UINT4 *pLength,
                                                UINT4 *pData)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 *pTmpObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size, numUpiTypeWords, count, j;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((gfpIdx > 1) || (gfpChnl > 63)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((pGfpIdx == NULL) || (pGfpChnl == NULL) || (pUpiType == NULL) ||
        (pLength == NULL) || (pData == NULL)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_REQ;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x0D;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (gfpIdx  << 24) |
                   (gfpChnl << 16);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;
    *pGfpIdx  = (*pTmpObndMsgBody >> 24) & 0xFF;
    *pGfpChnl = (*pTmpObndMsgBody >> 16) & 0xFF;
    *pLength  = (*pTmpObndMsgBody >> 8 ) & 0xFF;
    *pUpiType = (*pTmpObndMsgBody >> 0 ) & 0xFF;

    numUpiTypeWords = *pLength / 4;
    if ((*pLength % 4) != 0) {
        numUpiTypeWords++;
    }

    pTmpObndMsgBody++;
    count = j = 0;
    do {
        if (j > 3) {
            j = 0;
            pTmpObndMsgBody++;
            count++;
        }

        *pData = (*pTmpObndMsgBody >> (24-(j*8))) & 0xFF;

        j++;
        pData++;
    } while (count < numUpiTypeWords);

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqCsfGetWanToLanPropCmf */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqCsfGetWanToLanConsActStat
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_REQ_CSF_Get_WanToLan_Conseq_Action_Status message to FW
**                 and returns FW response
**                 RSP_REQ_CSF_Get_WanToLan_Conseq_Action_Status.
**
**  INPUTS:        *fileHndl  - base address
**                 chnlType   - channel type
**                                  0 - HSCI
**                                  1 - LSCI
**                 chnlId     - channel ID
**                                  0 to 1  - HSCI
**                                  0 to 15 - LSCI                
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pChnlType  - channel type
**                 *pChnlId    - channel ID
**                 *pForceCtrl - Software Forced Consequential Action 
**                               Control
**                 *pActionNum - Active Consequential Action Number
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqCsfGetWanToLanConsActStat(struct file *fileHndl,
                                                    UINT4 chnlType, 
                                                    UINT4 chnlId, 
                                                    UINT4 *pChnlType,
                                                    UINT4 *pChnlId,
                                                    UINT4 *pForceCtrl,
                                                    UINT4 *pActionNum)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 *pTmpObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) ||(chnlType == 0 && chnlId > 1) ||(chnlId > 15)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((pChnlType == NULL) || (pChnlId == NULL) || (pForceCtrl == NULL) ||
        (pActionNum == NULL)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_REQ;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x11;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (chnlType << 24) |
                   (chnlId   << 16);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;
    *pChnlType  = (*pTmpObndMsgBody >> 24) & 0xFF;
    *pChnlId    = (*pTmpObndMsgBody >> 16) & 0xFF;
    *pForceCtrl = (*pTmpObndMsgBody >> 8 ) & 0xFF;
    *pActionNum = (*pTmpObndMsgBody >> 0 ) & 0xFF;

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqCsfGetWanToLanConsActStat */


/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqCsfGetRxFefiStat
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_REQ_CSF_Get_Rx_FEFI_Status message to FW
**                 and returns FW response
**                 RSP_REQ_CSF_Get_Rx_FEFI_Status.
**
**  INPUTS:        *fileHndl      - base address
**                 lsciChStat     - LSCI Channel Status Checking Bit Field
**                                  This is a bit-field to indicate which LSCI 
**                                  channel should be used to check the FEFI
**                                  status.
**                                  bit0 : LSCI 0
**                                  bit1 : LSCI 1
**                                  ... 
**                                  bit15: LSCI 15
**                                  1 - enable status checking
**                                  0 - disable status checking
**                                 
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pLsciChStat   - LSCI Channel Status Checking Bit Field
**                                  This is a bit-field to indicate which LSCI
**                                  channel should be used to check the FEFI
**                                  status.
**                                  bit0 : LSCI 0
**                                  bit1 : LSCI 1
**                                  ...
**                                  bit15: LSCI 15
**                                  1 - enable status checking
**                                  0 - disable status checking
**                 *pLsciFefiStat - LSCI FEFI Status Bit Field 
**                                  This is a bit-field to indicate which LSCI
**                                  channel should be used to check the FEFI 
**                                  status.
**                                  bit0 : FEF_STAT_V for LSCI 0
**                                  bit1 : FEF_STAT_V for LSCI 1
**                                  ...
**                                  bit15: FEF_STAT_V for LSCI 15
**                                  1 - faulting is TRUE
**                                  0 - faulting is FALSE
**
**  OUTPUTS:       None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqCsfGetRxFefiStat(struct file *fileHndl,
                                           UINT4 lsciChStat, UINT4 *pLsciChStat,
                                           UINT4 *pLsciFefiStat)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 *pTmpObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (lsciChStat > 0xFFFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((pLsciChStat == NULL) || (pLsciFefiStat == NULL)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_REQ;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0xA0;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = lsciChStat & 0xFFFF;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;
    *pLsciChStat    = *pTmpObndMsgBody & 0xFFFF;
    
    pTmpObndMsgBody++;
    *pLsciFefiStat  = *pTmpObndMsgBody & 0xFFFF;

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqCsfGetRxFefiStat */


/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSendLanToWanOneShotCmf
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_CSF_Send_LanToWan_One_Shot_CMF
**                 message to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl  - base address
**                 gfpIdx     - GFP instance
**                                 0 - Slice 'A'
**                                 1 - Slice 'B'
**                 gfpChnl    - GFP channel number
**                              Legal range 0x0 to 0x3F (0-63)
**                 upi        - UPI value
**                              Legal range 0x0 to 0xFF
**                 genPyldFcs - generate 4-byte payload FCS
**                                  0 - do not generate payload FCS
**                                  1 - generate payload FCS
**                 cmfPyld[8] - 8-byte client payload data
**                                  cmfPyld[0] - payload octet N in CMF
**                                  cmfPyld[1] - payload octet N+1 in CMF
**                                  ...
**                                  cmfPyld[7] - payload octet N+7 in CMF
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgCsfSendLanToWanOneShotCmf(struct file *fileHndl,
                                                    UINT4 gfpIdx, UINT4 gfpChnl,
                                                    UINT4 upi, UINT4 genPyldFcs,
                                                    UINT4 cmfPyld[8])
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((gfpIdx > 1) || (gfpChnl > 63) || (upi > 0xFF) || (genPyldFcs > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < 8; i++) {
        if (cmfPyld[i] > 0xFF) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x5;
    ibndMsgSubtype = 0x0E;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* GFP_idx, GFP_ch, upi_value and genPayloadFCS */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (gfpIdx     << 24) |
                   (gfpChnl    << 16) |
                   (upi        << 8)  |
                   (genPyldFcs << 0);

    /* cmfPayload */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (cmfPyld[0] << 24) |
                   (cmfPyld[1] << 16) |
                   (cmfPyld[2] << 8)  |
                   (cmfPyld[3] << 0);
    pTmpIbndMsg++;
    *pTmpIbndMsg = (cmfPyld[4] << 24) |
                   (cmfPyld[5] << 16) |
                   (cmfPyld[6] << 8)  |
                   (cmfPyld[7] << 0);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSendLanToWanOneShotCmf */


/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSetLanToWanSoftAlarmForce
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Set_LanToWan_Soft_Alarm_Force message to FW and
**                 waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl  - base address
**                 chnlType   - channel type
**                                  0 - HSCI
**                                  1 - LSCI
**                 chnlId     - channel ID
**                                  0 to 1  - HSCI
**                                  0 to 15 - LSCI
**                 enable     - software alarm control
**                                  0 - disable
**                                  1 - enable
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgCsfSetLanToWanSoftAlarmForce(struct file *fileHndl,
                                                       UINT4 chnlType,
                                                       UINT4 chnlId,
                                                       UINT4 enable)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;


    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) ||(chnlType == 0 && chnlId > 1) ||(chnlId > 15)  ||
        (enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x03;
    ibndMsgSubtype = 0x0F;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* chnlType, chnlId and enable */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (chnlType    << 24) |
                   (chnlId      << 16) |
                   (enable      << 0);


    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;


    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSetLanToWanSoftAlarmForce */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSetWanToLanSoftConseqForce
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a 
**                 CMD_CFG_CSF_Set_WanToLan_Soft_Conseq_Force message to FW and
**                 waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl  - base address
**                 chnlType   - channel type
**                                  0 - HSCI
**                                  1 - LSCI
**                 chnlId     - channel ID
**                                  0 to 1  - HSCI
**                                  0 to 15 - LSCI
**                 forceCtrl  - software forced consequential action control
**                                  0 - disable
**                                  1 - enable
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgCsfSetWanToLanSoftConseqForce(struct file *fileHndl,
                                                    UINT4 chnlType,
                                                    UINT4 chnlId,
                                                    UINT4 forceCtrl)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) ||(chnlType == 0 && chnlId > 1) ||(chnlId > 15)  ||
        (forceCtrl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x03;
    ibndMsgSubtype = 0x10;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* port type, port number and force_ctrl */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (chnlType     << 24) |
                   (chnlId       << 16) |
                   (forceCtrl    << 8);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSetWanToLanSoftConseqForce */



/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgCsfSetWanToLanCmfRetrOption
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a 
**                 CMD_CFG_CSF_Set_WanToLan_CMF_Retrieval_Option message to FW
**                 and waits for a RSP_CFG response from FW.
**
**                 Note that firmware does not perform any WAN-to-LAN 
**                 consequential actions whenever interrupt-driven mode is 
**                 enabled.  Therefore, the firmware does not use the 
**                 configuration from the following messages once the 
**                 interrupt-driven mode is enabled by this host message:
**                 - CMD_CFG_CSF_Set_WanToLan_Mapping_Table
**                 - CMD_CFG_CSF_Set_WanToLan_Update_Table_Notification
**                 - CMD_CFG_CSF_Set_Defect_Condition_Stop_Param
**                 - CMD_CFG_CSF_Set_WanToLan_Soft_Conseq_Force
**                 
**                 In addition, please use 
**                 hyPhy20gHostMsgCmdReqCsfGetWanToLanOneShotKeepAliveCmfMultiCh
**                 to retrieve the latest One-Shot or Keep-Alive CMF from the 
**                 device instead of using 
**                 hyPhy20gHostMsgCmdReqCsfGetWanToLanPropCmf
**
**  INPUTS:        *fileHndl  - base address
**                 mode       - CMF Retrieval Mode
**                              0 - Default value, polling the hardware FIFO 
**                                  every 100ms.  This mode must be used if 
**                                  software wants to use FW to perform the 
**                                  WAN-to-LAN consequential action.
**                              1 - Interrupt-driven, firmware retrieves the 
**                                  CMF whenever it receives an interrupt from
**                                  the hardware.  Firmware sets Outbound 
**                                  Doorbell interrupt bit 1 to indicate to 
**                                  software that there is change in the CMF.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgCsfSetWanToLanCmfRetrOption(struct file *fileHndl,
                                                      UINT4 mode)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (mode > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x03;
    ibndMsgSubtype = 0x12;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* port type, port number and force_ctrl */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (mode << 24);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgCsfSetWanToLanCmfRetrOption */


/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqCsfGetWanToLanOneShotKeepAliveCmfMultiCh
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a 
**                 CMD_REQ_CSF_Get_WanToLan_OneShot_KeepAlive_CMF_MultiCh 
**                 message to FW for the multi-channel and returns FW respsonse 
**                 RSP_REQ_CSF_Get_WanToLan_OneShot_KeepAlive_CMF_MultiCh.
**
**  INPUTS:        *fileHndl    - base address
**                 chnlCnt      - Channel Count, number of channels to configure
**                                legal value 1 to 15
**                 chnlSlc[15]  - GFP Slice containing channel to access, 
**                                0 - GFP0
**                                1 - GFP1
**                                i.e. if chnlCnt = 10, then chnlIdx[10-14] 
**                                will not be processed.
**                 chnlNum[15]  - GFP channel number
**                                0 to 63 for channels within slice
**                                i.e. if chnlCnt = 10, then chnlNum[10-14] 
**                                will not be processed.
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pChnlCnt        - pointer to storage for Channel Count
**                 *pChnlData[4x15] - pointer to storage respsonse information 
**                                    for each channel in 4 words with format:
**                                    word0: 0xsscceerr
**                                    word1: 0xh0h1h2h3
**                                    word2: 0xp0p1p2p3
**                                    word3: 0xp4p5p6p7
**                                    Where
**                                    ss = GFP Slice,
**                                         0 - GFP0
**                                         1 - GFP1
**                                    cc = GFP Chanel Number,
**                                         0 to 63 for channels within slice
**                                    ee = CMF Extraction Status in Bit Fields
**                                         bit0 - Indicate CMF extraction 
**                                                hardware returns an error. 
**                                                Failed to extract CMF.
**                                         bit1 - Indicate a change in received
**                                                CMF is detected by firmware.  
**                                         bit2 - Indicate FDF overflow is 
**                                                detected. 
**                                         bit3 to bit7 reserved
**                                    rr = reserved 
**                                    h0 = Tpye Header Octet 1 in CMF
**                                    h1 = Tpye Header Octet 2 in CMF
**                                    h2 = Tpye Header Octet 3 in CMF
**                                    h3 = Tpye Header Octet 4 in CMF
**                                    p0 = Payload Octet N   in CMF
**                                    p1 = Payload Octet N+1 in CMF
**                                    p2 = Payload Octet N+2 in CMF
**                                    ...
**                                    p7 = Payload Octet N+7 in CMF
**                                    
**                                    NOTE:
**                                    word0~3 would be repeated 15 times,
**                                    i.e. if chnlCnt = 10, word0~3 will be 
**                                    initialized to 0xFFFFFFFF in the last
**                                    5 times.
**                                    
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqCsfGetWanToLanOneShotKeepAliveCmfMultiCh(
                                         struct file *fileHndl, UINT4 chnlCnt,
                                         UINT4 chnlSlc[15], UINT4 chnlNum[15],
                                         UINT4 *pChnlCnt, UINT4 *pChnlData)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 *pObndMsgBody, *pTmpObndMsgBody, *pTmpChnlData;
    UINT4 obndMsgLen, obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size, i, j;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    for (i = 0; i < 15; i++) {
        if ((chnlSlc[i] > 1  && chnlSlc[i]!= 0xFF) ||
            (chnlNum[i] > 63 && chnlNum[i]!= 0xFF) ||
            chnlCnt > 15 || chnlCnt < 1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }
    
    if (pChnlData == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);        


    /* set message header parameters */
    ibndModID = MODID_CMF; 
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 3 + chnlCnt;
    ibndMsgSubtype = 0x13;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }                                              

    /* assemble message body (ie. without header) */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    
    *pTmpIbndMsg = (chnlCnt & 0xFF) << 24;
    
    /* channel type and ID */
    for (i = 0; i < chnlCnt; i++) {
        pTmpIbndMsg++;
        *pTmpIbndMsg = ((chnlSlc[i] & 0xFF) << 24) | 
                       ((chnlNum[i] & 0xFF) << 16);
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);
    
    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    
    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;
    pTmpChnlData = pChnlData;
    
    *pChnlCnt = (*pTmpObndMsgBody >> 24) & 0xFF;
    pTmpObndMsgBody++;
    
    for (i = 0; i < chnlCnt; i++) {
        for (j = 0; j<4 ; j++) {
            *pTmpChnlData = *pTmpObndMsgBody;
            pTmpObndMsgBody++;
            pTmpChnlData++;
        }
    }
    for (i = chnlCnt; i < 15; i++) {
        for (j = 0; j<4 ; j++) {
            *pTmpChnlData = 0xFFFFFFFF;
            pTmpChnlData++;
        }
    }

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqCsfGetWanToLanOneShotKeepAliveCmfMultiCh */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgVcatLcasSetDefectCollectCtrl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_VCAT_LCAS_Set_Defect_Collect_Control message to
**                 FW and waits for a RSP_CFG response from FW.
**
**                 If defect collection is enabled and RVCP192 indirect
**                 accesses are required, SW MUTEX control is maintained by
**                 calling hyPhy20gHostMsgCmdCfgVcatLcasSetHeartbeat before
**                 the expiration of FW's 125ms count down timer.
**                 hyPhy20gHostMsgCmdCfgVcatLcasSetHeartbeat should be called
**                 every 100ms when defect collection is enabled.
**
**  INPUTS:        *fileHndl     - base address
**                 defectColCtrl - defect collection control for all 192 links
**                                     0 - disable defect collection
**                                     1 - enable defect collection
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgVcatLcasSetDefectCollectCtrl(struct file *fileHndl,
                                                       UINT4 defectColCtrl)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (defectColCtrl > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_VCAT;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x01;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* defect collection control */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = defectColCtrl;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgVcatLcasSetDefectCollectCtrl */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgVcatLcasSetStateMachineCtrl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_VCAT_LCAS_Set_State_Machine_Control message to
**                 FW and waits for a RSP_CFG response from FW.
**
**                 Defect collection should be enabled by calling
**                 hyPhy20gHostMsgCmdCfgVcatLcasSetDefectCollectCtrl before
**                 enabling state machine.
**
**  INPUTS:        *fileHndl - base address
**                 sts48Num  - selects one of the 4 possible STS-48 channels
**                             Legal range 0x0 - 0x3
**                 sts12Num  - selects one of the 4 possible STS-12 channels
**                             within sts48Num
**                             Legal range 0x0 - 0x3
**                 sts3Num   - selects one of the 4 possible STS-3 channels
**                             within sts12Num
**                             Legal range 0x0 - 0x3
**                 sts1Num   - selects one of the 3 possible STS-1 channels
**                             within sts3Num
**                             Legal range 0x0 - 0x2
**                 smCtrl    - state machine control
**                                 0 - disable state machine
**                                 1 - enable state machine
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgVcatLcasSetStateMachineCtrl(struct file *fileHndl,
                                                      UINT4 sts48Num,
                                                      UINT4 sts12Num,
                                                      UINT4 sts3Num,
                                                      UINT4 sts1Num,
                                                      UINT4 smCtrl)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size, sts3Off, sts1Off;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((sts48Num > 3) || (sts12Num > 3) ||(sts3Num > 3) ||(sts1Num > 2) ||
        (smCtrl > 1))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_VCAT;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x02;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* convert ITU-T G.707 numbering to Bellcore numbering */
    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;
    sts1Off = 3*sts3Off + sts1Num;

    /* assemble message body (ie. without header) */
    /* vcat link number and sm control */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (sts1Off << 16) |
                   (smCtrl  << 0 );

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgVcatLcasSetStateMachineCtrl */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqVcatLcasGetDefectIntInd
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_REQ_VCAT_LCAS_Get_Defect_Interrupt_Indication message to
**                 FW and returns FW response
**                 RSP_REQ_VCAT_LCAS_Get_Defect_Interrupt_Indication.
**
**                 Prior to calling this function, defect collection should be
**                 enabled via
**                 hyPhy20gHostMsgCmdCfgVcatLcasSetDefectCollectCtrl.
**
**  INPUTS:        *fileHndl - base address
**                 reqCtrl   - interrupt indication request control
**                                 0 - interrupt indication for paths 0 to 95
**                                 1 - interrupt indication for paths 96 to 191
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pIntData[96] - interrupt indication data (96 x 2 bytes)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqVcatLcasGetDefectIntInd(struct file *fileHndl,
                                                  UINT4 reqCtrl,
                                                  UINT4 *pIntData)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 *pTmpObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size, obndCtrl;
    UINT4 j;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (reqCtrl > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pIntData == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_VCAT;
    ibndMsgCat     = CATEGORY_CMD_REQ;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x03;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = reqCtrl;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pTmpObndMsgBody = pObndMsgBody;

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* check ctrl field in returned message */
    pTmpObndMsgBody = pObndMsgBody;
    obndCtrl = *pTmpObndMsgBody & 0xFFFF;
    if (obndCtrl != reqCtrl) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        return HYPHY20G_ERR_RSP_REQ;
    }

    /* decode message */
    pTmpObndMsgBody++;
    for (j = 0; j < 48; j++) {
        *(pIntData + (j*2))     = (*pTmpObndMsgBody >> 16) & 0xFFFF;
        *(pIntData + ((j*2)+1)) =  *pTmpObndMsgBody        & 0xFFFF;
        pTmpObndMsgBody++;
    }

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqVcatLcasGetDefectIntInd */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqVcatLcasGetDefectIntStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_REQ_VCAT_LCAS_Get_Defect_Interrupt_Status message to
**                 FW and returns FW response
**                 RSP_REQ_VCAT_LCAS_Get_Defect_Interrupt_Status.
**
**                 Prior to calling this function, defect collection should be
**                 enabled via
**                 hyPhy20gHostMsgCmdCfgVcatLcasSetDefectCollectCtrl.
**
**  INPUTS:        *fileHndl - base address
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pIntData[192] - interrupt status data (192 x 1 byte)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqVcatLcasGetDefectIntStatus(struct file *fileHndl,
                                                     UINT4 *pIntData)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 *pTmpObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 j;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (pIntData == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_VCAT;
    ibndMsgCat     = CATEGORY_CMD_REQ;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x04;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = 0;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pTmpObndMsgBody = pObndMsgBody;

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;
    for (j = 0; j < 48; j++) {
        *(pIntData + (j*4))     = (*pTmpObndMsgBody >> 24) & 0xFF;
        *(pIntData + ((j*4)+1)) = (*pTmpObndMsgBody >> 16) & 0xFF;
        *(pIntData + ((j*4)+2)) = (*pTmpObndMsgBody >> 8)  & 0xFF;
        *(pIntData + ((j*4)+3)) =  *pTmpObndMsgBody        & 0xFF;
        pTmpObndMsgBody++;
    }

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqVcatLcasGetDefectIntStatus */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgVcatLcasSetForceState
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_VCAT_LCAS_Set_Force_State
**                 message to FW and waits for a RSP_CFG response from FW.
**
**                 Prior to calling this function, state machine should be
**                 enabled via
**                 hyPhy20gHostMsgCmdCfgVcatLcasSetStateMachineCtrl.
**
**  INPUTS:        *fileHndl  - base address
**                 sts48Num   - selects one of the 4 possible STS-48 channels
**                              Legal range 0x0 - 0x3
**                 sts12Num   - selects one of the 4 possible STS-12 channels
**                              within sts48Num
**                              Legal range 0x0 - 0x3
**                 sts3Num    - selects one of the 4 possible STS-3 channels
**                              within sts12Num
**                              Legal range 0x0 - 0x3
**                 sts1Num    - selects one of the 3 possible STS-1 channels
**                              within sts3Num
**                              Legal range 0x0 - 0x2
**                 defectType - defect type
**                                  0 - member signal unavailable (MSU_L)
**                                  1 - trail signal degrade (TSD)
**                 enable     - 0 - disable forced state
**                              1 - enable forced state
**                 state      - For enable == 1 (otherwise set to 0)
**                              Set state machine
**                                  0 - defect cleared
**                                  1 - defect active
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgVcatLcasSetForceState(struct file *fileHndl,
                                                UINT4 sts48Num, UINT4 sts12Num,
                                                UINT4 sts3Num, UINT4 sts1Num,
                                                UINT4 defectType, UINT4 enable,
                                                UINT4 state)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size, sts3Off, sts1Off;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((sts48Num > 3) || (sts12Num > 3) ||(sts3Num > 3) ||(sts1Num > 2) ||
        (defectType > 1) || (enable > 1) || (state > 1))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_VCAT;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x4;
    ibndMsgSubtype = 0x05;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* convert ITU-T G.707 numbering to Bellcore numbering */
    sts3Off = 16*sts48Num + 4*sts12Num + sts3Num;
    sts1Off = 3*sts3Off + sts1Num;

    /* assemble message body (ie. without header) */
    /* vcat link number */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (sts1Off << 16) |
                   (enable  << 0 );

    /* defect type and state */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (defectType << 16) |
                   (state      << 0 );

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgVcatLcasSetForceState */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqVcatLcasGetDefectState
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_VCAT_LCAS_Get_Defect_State
**                 message to FW and returns FW response
**                 RSP_REQ_VCAT_LCAS_Get_Defect_State.
**
**                 Prior to calling this function, state machine should be
**                 enabled via
**                 hyPhy20gHostMsgCmdCfgVcatLcasSetStateMachineCtrl.
**
**  INPUTS:        *fileHndl - base address
**                 reqCtrl   - 0 - request MSU_L state
**                             1 - request TSD state
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pState[192] - link state (192 x 1 byte)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqVcatLcasGetDefectState(struct file *fileHndl,
                                                 UINT4 reqCtrl, UINT4 *pState)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 *pTmpObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size, obndCtrl;
    UINT4 j;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (reqCtrl > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pState == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_VCAT;
    ibndMsgCat     = CATEGORY_CMD_REQ;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x06;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = reqCtrl;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pTmpObndMsgBody = pObndMsgBody;

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* check defect_control field in returned message */
    pTmpObndMsgBody = pObndMsgBody;
    obndCtrl = *pTmpObndMsgBody & 0xFF;
    if (obndCtrl != reqCtrl) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        return HYPHY20G_ERR_RSP_REQ;
    }

    /* decode message */
    pTmpObndMsgBody++;
    for (j = 0; j < 48; j++) {
        *(pState + (j*4))     = (*pTmpObndMsgBody >> 24) & 0xFF;
        *(pState + ((j*4)+1)) = (*pTmpObndMsgBody >> 16) & 0xFF;
        *(pState + ((j*4)+2)) = (*pTmpObndMsgBody >> 8)  & 0xFF;
        *(pState + ((j*4)+3)) =  *pTmpObndMsgBody        & 0xFF;
        pTmpObndMsgBody++;
    }

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqVcatLcasGetDefectState */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqVcatLcasGetLinkStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_VCAT_LCAS_Get_Link_Status
**                 message to FW and returns FW response
**                 RSP_REQ_VCAT_LCAS_Get_Link_Status.
**
**                 Prior to calling this function, state machine should be
**                 enabled via
**                 hyPhy20gHostMsgCmdCfgVcatLcasSetStateMachineCtrl.
**
**  INPUTS:        *fileHndl - base address
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pStatus[192] - link status (192 x 1 byte)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqVcatLcasGetLinkStatus(struct file *fileHndl,
                                                UINT4 *pStatus)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 *pTmpObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 j;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (pStatus == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_VCAT;
    ibndMsgCat     = CATEGORY_CMD_REQ;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x07;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = 0;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* check response message header */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pTmpObndMsgBody = pObndMsgBody;

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;
    for (j = 0; j < 48; j++) {
        *(pStatus + (j*4))     = (*pTmpObndMsgBody >> 24) & 0xFF;
        *(pStatus + ((j*4)+1)) = (*pTmpObndMsgBody >> 16) & 0xFF;
        *(pStatus + ((j*4)+2)) = (*pTmpObndMsgBody >> 8)  & 0xFF;
        *(pStatus + ((j*4)+3)) =  *pTmpObndMsgBody        & 0xFF;
        pTmpObndMsgBody++;
    }

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqVcatLcasGetLinkStatus */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgVcatLcasSetHeartbeat
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_VCAT_LCAS_Set_Heartbeat
**                 message to FW and waits for a RSP_CFG response from FW.
**
**                 This host message resets FW's 125ms count down timer for
**                 MUTEX control.
**
**                 If defect collection is enabled via
**                 hyPhy20gHostMsgCmdCfgVcatLcasSetDefectCollectCtrl, this
**                 function must be called every 100ms for system software
**                 to maintain MUTEX control for RVCP indirect register access.
**
**  INPUTS:        *fileHndl  - base address
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgVcatLcasSetHeartbeat(struct file *fileHndl)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_VCAT;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x3;
    ibndMsgSubtype = 0x08;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* vcat link number */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = 0;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgVcatLcasSetHeartbeat */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgVcatLcasSetMemberToGrpMpTbl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_VCAT_LCAS_Set_Vcat_Member_To_Group_Mapping_Table
**                 message to FW and waits for a RSP_CFG response from FW.
**
**
**  INPUTS:        *fileHndl - base address
**                 vcatGrp   - VCAT group number
**                             Legal range 0x0 to 0x3F
**                 isLcasMember[6]
**                          - each bit presents whether the LCAT Member belongs
**                            to the VCAT group.
**                                 0 - not a member of this VCAT group
**                                 1 - a member of this VCAT group
**                            The mapping between LCAT Member ID and bit 
**                            position of this parameter:
**                 Array Num      Bit31                                      Bit0
**                 0 LCAT Member ID [7:0]       [15:8]      [23:16]     [31:24]
**                 1 ... ...
**                 2 ... ...
**                 3 ... ...
**                 4 ... ...
**                 5 LCAT Member ID [167:160]   [175:168]   [183:176]   [191:184]
**                 
**                 For example, to map STS-1 #0 and #3 to VCAT group 0:
**                      set vcatGrp to 0
**                      set isLcasMember[6] to 0x90000000 0x0 0x0 0x0 0x0 0x0
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgVcatLcasSetMemberToGrpMpTbl(struct file *fileHndl,
                                                      UINT4 vcatGrp,
                                                      UINT4 isLcasMember[6])
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (vcatGrp > 63)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID      = MODID_VCAT;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x9;
    ibndMsgSubtype = 0x0A;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* vcat group */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (vcatGrp << 24);

    for (i = 0; i <= 5; i++) {
        pTmpIbndMsg++;
        *pTmpIbndMsg = isLcasMember[i];
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgVcatLcasSetMemberToGrpMpTbl */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgLedSetTwiAddrSetup
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_LED_Set_TWI_Addr_Setup
**                 message to FW for the led blinking feature and waits for a
**                 RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 devAddr   - External FPGA/CPLD device address
**                                 Legal Range:
**                                 0x0 - 0x3F
**                 memAddr   - External FPGA/CPLD 16 bit address of LED Status
**                             Blinking register
**                                 0x0 to 0xFFFF
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgLedSetTwiAddrSetup(struct file *fileHndl,
                                             UINT4 devAddr, UINT4 memAddr)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;

    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((devAddr > 0x3F) || (memAddr > 0xFFFF))
    {
        DBG_PRINT("devAddr=0x%lx, memAddr=0x%lx\n", devAddr, memAddr);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_TWI_LED;
    ibndMsgCat = CATEGORY_CMD_CFG;

    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x01;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and id */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((devAddr & 0x3F) << 16) |
                    ((memAddr & 0xFFFF) << 0);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgLedSetTwiAddrSetup */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgLedSetMonCtrl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_LED_Set_Monitoring_Control
**                 message to FW for the led blinking feature and waits for a
**                 RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                                 2 - MGMT Port
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                                 0 - MGMT Port
**                 clientType - HSCI/LSCI Client Type:
**                                 0 - ENET
**                                 1 - FC/CBR
**                                 SW forces value to 0 for MGMT Port
**                 enable    - LED Blinking Control
**                                 0: Disable LED Blinking Feature for port
**                                 1: Enable LED Blinking Feature for port
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgLedSetMonCtrl(struct file *fileHndl,
                                        UINT4 chnlType, UINT4 chnlId,
                                        UINT4 clientType, UINT4 enable)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;

    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 2) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) || (chnlType == 2 && chnlId > 0) ||
        (clientType > 1) || (enable > 1))
    {
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx, clientType=0x%lx, enable=0x%lx\n", chnlType, chnlId, clientType, enable);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* force value to 0 for MGMT Port as FC/CBR not supported on MGMT port */
    if (chnlType == 2){
        clientType = 0;
    }

    /* set message header parameters */
    ibndModID = MODID_TWI_LED;
    ibndMsgCat = CATEGORY_CMD_CFG;

    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x02;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and id */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) |
                   ((chnlId & 0xFF) << 16) |
                   ((enable & 0x1) << 8) |
                   ((clientType & 0x1) << 0);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgLedSetMonCtrl */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgLedSetHsciMap
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_LED_Set_HSCI_Mapping
**                 message to FW for the led blinking feature and waits for a
**                 RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 map       - HSCI slice to HSCI LED mapping
**                                 0 - Direct Mapping
**                                     HSCI slice 0 maps to HSCI_LED[0]
**                                     HSCI slice 1 maps to HSCI_LED[1]
**                                 1 - Swizzled Mapping
**                                     HSCI slice 0 maps to HSCI_LED[1]
**                                     HSCI slice 1 maps to HSCI_LED[0]
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgLedSetHsciMap(struct file *fileHndl, UINT4 map)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;

    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (map > 1)
    {
        DBG_PRINT("map=0x%lx\n", map);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_TWI_LED;
    ibndMsgCat = CATEGORY_CMD_CFG;

    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x03;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and id */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((map & 0x1) << 0);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgLedSetHsciMap */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgEnetMpFilterSetEnetMpFilterSetup
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_ENET_MP_FILTER_Set_Enet_MP_Filter_Setup
**                 message to FW to enable or disable the extraction of
**                 a packet type to the management port and waits for a
**                 RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                                 2 - MGMT Port
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                                 0 - MGMT Port
**                 pktType   - Management Packet Type to be configured:
**                                 0 - LACP
**                                 1 - Marker
**                                 2 - VLAN tagged
**                 vlanTpid  - VLAN TPID value for pktType = 2
**                                 Legal range:
**                                 0x0 to 0xFFFF
**                             Field not processed when enable = 0 or
**                             pktType = 0 or pktType = 1
**                 vlanTci   - VLAN TCI value for pktType = 2
**                                 Legal range:
**                                 0x0 to 0xFFFF
**                             Field not processed when enable = 0 or
**                             pktType = 0 or pktType = 1
**                 enable    - LED Blinking Control
**                                 0: Disable LED Blinking Feature for port
**                                 1: Enable LED Blinking Feature for port
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgEnetMpFilterSetEnetMpFilterSetup(struct file *fileHndl,
                                                           UINT4 chnlType,
                                                           UINT4 chnlId,
                                                           UINT4 pktType,
                                                           UINT4 vlanTpid,
                                                           UINT4 vlanTci,
                                                           UINT4 enable)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;

    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 2) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) || (pktType > 2) ||
        (vlanTpid > 0xFFFF) || (vlanTci > 0xFFFF) || (enable > 1))
    {
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx, pktType=0x%lx, vlanTpid=0x%lx, vlanTci=0x%lx, enable=0x%lx\n", chnlType, chnlId, pktType, vlanTpid, vlanTci, enable);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_MGMT_EXT;
    ibndMsgCat = CATEGORY_CMD_CFG;

    ibndMsgLen = 0x4;
    ibndMsgSubtype = 0x01;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and id */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((enable & 0x1) << 24) |
                   ((chnlType & 0xFF) << 16) |
                   ((chnlId & 0xFF) << 8) |
                   ((pktType & 0x3) << 0);
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((vlanTpid & 0xFFFF) << 16) |
                   ((vlanTci & 0xFFFF) << 0);


    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgEnetMpFilterSetEnetMpFilterSetup */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgEnetMpFilterSetTestPattParam
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_ENET_PKT_Set_Test_Pattern_Parameters
**                 message to FW to configure the parameters of the Ethernet
**                 Test Packet generator and monitor and waits for a
**                 RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl  - base address
**                 macAddr[6] - Destination MAC address of Ethernet Test packet
**                              Format of macAddr:
**                                  MAC address = 12-34-56-78-9a-bc
**                                  macAddr[0] = 12
**                                  macAddr[1] = 34
**                                  macAddr[2] = 56
**                                  macAddr[3] = 78
**                                  macAddr[4] = 9a
**                                  macAddr[5] = bc
**                 pattType  - Test pattern type
**                                  0 - PRBS31
**                                  1 - fixed pattern
**                 pattData  - For pattType == 1 (otherwise ignored)
**                             Test pattern for fixed pattern test packet.
**                           - Legal range 0x0 to 0xFF.
**                 pyldSize  - Payload size
**                                  PRBS31        - 256 or 1024 bytes
**                                  Fixed pattern - legal range 46 to 1024 bytes
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgEnetMpFilterSetTestPattParam(struct file *fileHndl,
                                                       UINT4 macAddr[6],
                                                       UINT4 pattType,
                                                       UINT4 pattData,
                                                       UINT4 pyldSize)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;

    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    for (i = 0; i < 6; i++) {
        if (macAddr[i] > 0xFF) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }
    if ((pattType > 1) || (pattData > 0xFF) ||
        (pattType == 0 && (pyldSize != 256 && pyldSize != 1024)) ||
        (pattType == 1 && (pyldSize < 46 || pyldSize > 1024)))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_MGMT_EXT;
    ibndMsgCat = CATEGORY_CMD_CFG;

    ibndMsgLen = 0x5;
    ibndMsgSubtype = 0x02;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* mac_da */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((macAddr[0] & 0xFF) << 24) |
                   ((macAddr[1] & 0xFF) << 16) |
                   ((macAddr[2] & 0xFF) << 8)  |
                   ((macAddr[3] & 0xFF) << 0);
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((macAddr[4] & 0xFF) << 24) |
                   ((macAddr[5] & 0xFF) << 16);

    /* test type, test pattern, payload size */
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((pattType & 0xFF)   << 24) |
                   ((pattData & 0xFF)   << 16) |
                   ((pyldSize & 0xFFFF) << 0);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgEnetMpFilterSetTestPattParam */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgEnetMpFilterSetTestPattCtrl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_ENET_PKT_Set_Test_Pattern_Control
**                 message to FW to enable or disable the Ethernet
**                 Test Packet Generator/Monitor and waits for a
**                 RSP_CFG response from FW.
**
**                 Note that the Test Packet Generator/Monitor can only be
**                 mapped to a maximum of one Ethernet Client port at a time.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 enable    - Test Pattern Generator/Monitor control
**                                 0 - disable
**                                 1 - enable
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgEnetMpFilterSetTestPattCtrl(struct file *fileHndl,
                                                      UINT4 chnlType,
                                                      UINT4 chnlId,
                                                      UINT4 enable)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;

    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) || (enable > 1))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_MGMT_EXT;
    ibndMsgCat = CATEGORY_CMD_CFG;

    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x03;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* port type, port index, test enable */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) |
                   ((chnlId   & 0xFF) << 16) |
                   ((enable   & 0xFF) << 8);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgEnetMpFilterSetTestPattCtrl */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqEnetMpFilterGetTestPktStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_REQ_ENET_PKT_Get_Test_Pattern_Status message to FW
**                 to request the status of the Ethernet Test Pattern
**                 Generator/Monitor and returns FW response
**                 RSP_REQ_ENET_PKT_Get_Test_Packet_Status.
**
**  INPUTS:        *fileHndl   - base address
**
**                 The following are pointers to storage for response message
**                 parameters:
**
**                 *pPattType    - test pattern type
**                 *pPattData    - fixed test pattern data
**                 *pPyldSize    - payload size
**                 *pChnlType    - port type
**                 *pChnlId      - port index
**                 *pEnable      - test enable
**                 *pNumTxPkt    - number of transmitted packets
**                 *pNumRxPkt    - number of received packets
**                 *pNumRxErrPkt - number of received errored packets
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqEnetMpFilterGetTestPktStatus(struct file *fileHndl,
                                                       UINT4 *pPattType,
                                                       UINT4 *pPattData,
                                                       UINT4 *pPyldSize,
                                                       UINT4 *pChnlType,
                                                       UINT4 *pChnlId,
                                                       UINT4 *pEnable,
                                                       UINT4 *pNumTxPkt,
                                                       UINT4 *pNumRxPkt,
                                                       UINT4 *pNumRxErrPkt)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 *pObndMsgBody;
    UINT4 *pTmpObndMsgBody;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (pPattType == NULL || pPattData == NULL || pPyldSize == NULL ||
        pChnlType == NULL || pChnlId == NULL || pEnable == NULL ||
        pNumTxPkt == NULL || pNumRxPkt == NULL || pNumRxErrPkt == NULL)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_MGMT_EXT;
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x04;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* reserved field */
    *pTmpIbndMsg = 0;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }


    /* determine message body length and allocate memory if necessary */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pTmpObndMsgBody = pObndMsgBody;

    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;
    *pPattType = (*pTmpObndMsgBody >> 24) & 0xFF;
    *pPattData = (*pTmpObndMsgBody >> 16) & 0xFF;
    *pPyldSize = (*pTmpObndMsgBody >> 0)  & 0xFFFF;

    pTmpObndMsgBody++;
    *pChnlType = (*pTmpObndMsgBody >> 24) & 0xFF;
    *pChnlId   = (*pTmpObndMsgBody >> 16) & 0xFF;
    *pEnable   = (*pTmpObndMsgBody >> 8)  & 0xFF;

    pTmpObndMsgBody++;
    *pNumTxPkt = *pTmpObndMsgBody;

    pTmpObndMsgBody++;
    *pNumRxPkt = *pTmpObndMsgBody;

    pTmpObndMsgBody++;
    *pNumRxErrPkt = *pTmpObndMsgBody;

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqEnetMpFilterGetTestPktStatus */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPmonEnetStatColCtrl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PMON_Enet_Stat_Collect_Control
**                 message to FW for a single LSCI or HSCI slice and waits for a
**                 RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 enable    - Statistics Collection Control
**                                 0: Disable Statistics Collection
**                                 1: Enable Stattistics Collection
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPmonEnetStatColCtrl(struct file *fileHndl,
                                              UINT4 chnlType, UINT4 chnlId,
                                              UINT4 enable)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;

    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) || (enable > 1))
    {
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx, enable=0x%lx\n", chnlType, chnlId, enable);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_ENET_PMON;
    ibndMsgCat = CATEGORY_CMD_CFG;

    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x01;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and id */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) |
                   ((chnlId & 0xFF) << 16) |
                   ((enable & 0x1) << 0);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPmonEnetStatColCtrl */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPmonReqMstatxCtrl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_PMON_Request_Enet_MStatx_Control
**                 message to FW for a single LSCI or HSCI slice and waits for a
**                 RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPmonReqMstatxCtrl(struct file *fileHndl,
                                            UINT4 chnlType, UINT4 chnlId)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15))
    {
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx\n", chnlType, chnlId);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_ENET_PMON;
    ibndMsgCat = CATEGORY_CMD_CFG;

    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x02;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and id */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) |
                   ((chnlId & 0xFF) << 16);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPmonReqMstatxCtrl */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPmonReleaseMstatxCtrl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_PMON_Request_Enet_MStatx_Control
**                 message to FW for a single LSCI or HSCI slice and waits for a
**                 RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPmonReleaseMstatxCtrl(struct file *fileHndl,
                                                UINT4 chnlType, UINT4 chnlId)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15))
    {
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx\n", chnlType, chnlId);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_ENET_PMON;
    ibndMsgCat = CATEGORY_CMD_CFG;

    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x03;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and id */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) |
                   ((chnlId & 0xFF) << 16);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPmonReleaseMstatxCtrl */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqGetLsciL1RppPmonCntr
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_PMON_Get_LSCI_L1RPP_PMON_CNTR
**                 message to FW and returns data from FW response
**                 RSP_REQ_PMON_Get_LSCI_L1RPP_PMON_CNTR.
**
**  INPUTS:        *fileHndl - base address
**                 chnlId    - channel ID
**                               0 to 15 - LSCI slice
**
**                 Response message parameters:
**
**                  *pCount[2]  - pointer to storage for 40-bit line code
**                               violation counter
**                               *pCount[1] PMON_CNTR[39:32]
**                               *pCount[0] PMON_CNTR[31:0]
**                               (size of storage must be 2 x 4 bytes)
**                  *pOverflowI - pointer to storage for counter overflow
**                                indication
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqGetLsciL1RppPmonCntr(struct file *fileHndl,
                                               UINT4 chnlId, UINT4 *pCount,
                                               UINT4 *pOverflowI)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 *pObndMsgBody, *pTmpObndMsgBody;
    UINT4 obndMsgLen, obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 expObndMsgLen = 4;  /* RSP_REQ message length minus 2 words */
    UINT4 *pTmpCount, *pTmpOverflowI;
    UINT4 countLen = 2;
    UINT4 size, i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (chnlId > 15)
    {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pCount == NULL || pOverflowI == NULL)
    {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_ENET_PMON;
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3; /* CMD_REQ message length */
    ibndMsgSubtype = 0x04;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and ID */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (chnlId & 0xFF) << 24;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    if (obndMsgLen != (expObndMsgLen + 2)) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return HYPHY20G_ERR_RSP_REQ;
    }

    /* allocate memory */
    size = sizeof(UINT4)*(expObndMsgLen);
    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* parse message body */

    /* copy data into pCount */
    pTmpObndMsgBody = pObndMsgBody + 1;
    pTmpCount = pCount;
    for (i = 0; i < countLen; i++) {
        *pTmpCount = *pTmpObndMsgBody;
        pTmpCount++;
        pTmpObndMsgBody++;
    }

    /* copy data into pOverflowI */
    pTmpObndMsgBody = pObndMsgBody + 1 + countLen;
    pTmpOverflowI = pOverflowI;
    *pTmpOverflowI = *pTmpObndMsgBody;

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqGetLsciL1RppPmonCntr */



/*******************************************************************************
**
**  hyPhy20gHostMsgCmdReqGetHsciGsup43PmonCntr
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_PMON_Get_HSCI_GSUP43_PMON_CNTR
**                 message to FW and returns data from FW response
**                 RSP_REQ_PMON_Get_HSCI_GSUP43_PMON_CNTR.
**
**  INPUTS:        *fileHndl - base address
**                 chnlId    - channel ID
**                               0 to 1 - HSCI slice
**
**                 Response message parameters:
**
**                  *pCount0      - Pointer to counter storage for:
**                                  RX_125US_CNT
**                  *pCount1      - Pointer to counter storage for:
**                                  BLK_TYPE_ERR_CNT
**                  *pCount2      - Pointer to counter storage for:
**                                  HI_BER_CNT
**                  *pCount3      - Pointer to counter storage for:
**                                  PRBS31_ERR_CNT
**                  *pCount4      - Pointer to counter storage for:
**                                  RX_BLK_ERR_CNT
**                  *pCount5[2]   - Pointer to counter storage for:
**                                  RX_OS_DSCRCD_CNT
**                  *pCount6[2]   - Pointer to counter storage for:
**                                  RX_DATA_BYTE_CNT
**                  *pCount7[2]   - Pointer to counter storage for:
**                                  RX_OS_CNT
**                  *pCount8[2]   - Pointer to counter storage for:
**                                  RX_PKT_CNT
**                  *pCount9[2]   - Pointer to counter storage for:
**                                  TX_PKT_CNT
**                  *pCount10[2]  - Pointer to counter storage for:
**                                  TX_OS_CNT
**                  *pCount11[2]  - Pointer to counter storage for:
**                                  TX_DATA_BYTE_CNT
**                  *pOvFlStatus  - Pointer to overflow status for counters
**                                  Bit0 - RX_125US_CNT overflow
**                                  Bit1 - BLK_TYPE_ERR_CNT overflow
**                                  Bit2 - HI_BER_CNT overflow
**                                  Bit3 - PRBS31_ERR_CNT overflow
**                                  Bit4 - RX_BLK_ERR_CNT overflow
**                                  Bit5 - RX_OS_DSCRCD_CNT overflow
**                                  Bit6 - RX_DATA_BYTE_CNT overflow
**                                  Bit7 - RX_OS_CNT overflow
**                                  Bit8 - RX_PKT_CNT overflow
**                                  Bit9 - TX_PKT_CNT overflow
**                                  Bit10 - TX_OS_CNT overflow
**                                  Bit11- TX_DATA_BYTE_CNT overflow
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqGetHsciGsup43PmonCntr(struct file *fileHndl,
                                                UINT4 chnlId, UINT4 *pCount0,
                                                UINT4 *pCount1, UINT4 *pCount2,
                                                UINT4 *pCount3, UINT4 *pCount4,
                                                UINT4 *pCount5, UINT4 *pCount6,
                                                UINT4 *pCount7, UINT4 *pCount8,
                                                UINT4 *pCount9, UINT4 *pCount10,
                                                UINT4 *pCount11,
                                                UINT4 *pOverflowI)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 *pObndMsgBody, *pTmpObndMsgBody;
    UINT4 obndMsgLen, obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 expObndMsgLen = 20;  /* RSP_REQ message length minus 2 words */
    UINT4 *pTmpCount0, *pTmpCount1, *pTmpCount2, *pTmpCount3,  *pTmpCount4;
    UINT4 *pTmpCount5, *pTmpCount6, *pTmpCount7, *pTmpCount8,  *pTmpCount9;
    UINT4 *pTmpCount10, *pTmpCount11;
    UINT4 *pTmpOverflowI;
    UINT4 countLen = 2;
    UINT4 size, i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (chnlId > 1)
    {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pCount0 == NULL || pCount1 == NULL || pCount2 == NULL ||
        pCount3 == NULL || pCount4 == NULL || pCount5 == NULL ||
        pCount6 == NULL || pCount7 == NULL || pCount8 == NULL ||
        pCount9 == NULL || pCount10 == NULL || pCount11 == NULL ||
        pOverflowI == NULL)
    {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_ENET_PMON;
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3; /* CMD_REQ message length */
    ibndMsgSubtype = 0x05;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and ID */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (chnlId & 0xFF) << 24;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    if (obndMsgLen != (expObndMsgLen + 2)) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return HYPHY20G_ERR_RSP_REQ;
    }

    /* allocate memory */
    size = sizeof(UINT4)*(expObndMsgLen);
    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* parse message body */

    /* copy data into pCount0 */
    pTmpObndMsgBody = pObndMsgBody + 1;
    pTmpCount0 = pCount0;
    *pTmpCount0 = *pTmpObndMsgBody;

    /* copy data into pCount1 and pCount2 */
    pTmpObndMsgBody = pObndMsgBody + 2;
    pTmpCount2 = pCount2;
    *pTmpCount2 = (*pTmpObndMsgBody & 0xFF);
    pTmpCount1 = pCount1;
    *pTmpCount1 = (*pTmpObndMsgBody & 0xFF00) >> 8;

    /* copy data into pCount3 */
    pTmpObndMsgBody = pObndMsgBody + 3;
    pTmpCount3 = pCount3;
    *pTmpCount3 = *pTmpObndMsgBody;

    /* copy data into pCount4 */
    pTmpObndMsgBody = pObndMsgBody + 4;
    pTmpCount4 = pCount4;
    *pTmpCount4 = *pTmpObndMsgBody;

    /* copy data into pCount5 */
    pTmpObndMsgBody = pObndMsgBody + 5;
    pTmpCount5 = pCount5;
    for (i = 0; i < countLen; i++) {
        *pTmpCount5 = *pTmpObndMsgBody;
        pTmpCount5++;
        pTmpObndMsgBody++;
    }

    /* copy data into pCount6 */
    pTmpObndMsgBody = pObndMsgBody + 7;
    pTmpCount6 = pCount6;
    for (i = 0; i < countLen; i++) {
        *pTmpCount6 = *pTmpObndMsgBody;
        pTmpCount6++;
        pTmpObndMsgBody++;
    }

    /* copy data into pCount7 */
    pTmpObndMsgBody = pObndMsgBody + 9;
    pTmpCount7 = pCount7;
    for (i = 0; i < countLen; i++) {
        *pTmpCount7 = *pTmpObndMsgBody;
        pTmpCount7++;
        pTmpObndMsgBody++;
    }

    /* copy data into pCount8 */
    pTmpObndMsgBody = pObndMsgBody + 11;
    pTmpCount8 = pCount8;
    for (i = 0; i < countLen; i++) {
        *pTmpCount8 = *pTmpObndMsgBody;
        pTmpCount8++;
        pTmpObndMsgBody++;
    }

    /* copy data into pCount9 */
    pTmpObndMsgBody = pObndMsgBody + 13;
    pTmpCount9 = pCount9;
    for (i = 0; i < countLen; i++) {
        *pTmpCount9 = *pTmpObndMsgBody;
        pTmpCount9++;
        pTmpObndMsgBody++;
    }

    /* copy data into pCount10 */
    pTmpObndMsgBody = pObndMsgBody + 15;
    pTmpCount10 = pCount10;
    for (i = 0; i < countLen; i++) {
        *pTmpCount10 = *pTmpObndMsgBody;
        pTmpCount10++;
        pTmpObndMsgBody++;
    }

    /* copy data into pCount11 */
    pTmpObndMsgBody = pObndMsgBody + 17;
    pTmpCount11 = pCount11;
    for (i = 0; i < countLen; i++) {
        *pTmpCount11 = *pTmpObndMsgBody;
        pTmpCount11++;
        pTmpObndMsgBody++;
    }

    /* copy data into pOverflowI */
    pTmpObndMsgBody = pObndMsgBody + 19;
    pTmpOverflowI = pOverflowI;
    *pTmpOverflowI = *pTmpObndMsgBody;

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqGetHsciGsup43PmonCntr */

/*******************************************************************************
**  
**  hyPhy20gHostMsgCmdReqPtpGetPathDelay
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_PTP_Get_Path_Delay  message to 
**                 FW and returns FW response RSP_REQ_PTP_Get_Path_Delay.
**
**  INPUTS:        *fileHndl  - base address
**                 chnlType   - channel type
**                              0 - HSCI
**                              1 - LSCI
**                 chnlId     - channel ID
**                              0 to 1  - HSCI
**                              0 to 15 - LSCI
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pathDelay[2]  - pointers to storage the path delay in 
**                                  nanoseconds for the specified channel. 
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqPtpGetPathDelay(struct file *fileHndl, UINT4 chnlType,
                                          UINT4 chnlId, UINT4 *pathDelay)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 *pObndMsgBody, *pTmpObndMsgBody, *pTmpPathDelay;
    UINT4 obndMsgLen, obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pathDelay == NULL)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP; 
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x03;
    ibndMsgSubtype = 0x06;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and ID */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) | 
                   ((chnlId & 0xFF) << 16);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);
    
    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    
    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */
    pTmpObndMsgBody = pObndMsgBody;
    pTmpPathDelay = pathDelay;
    
    *pTmpPathDelay  = *pTmpObndMsgBody;
    pTmpPathDelay++;
    pTmpObndMsgBody++;
    *pTmpPathDelay  = *pTmpObndMsgBody;

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqPtpGetPathDelay */


/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetPortAsymmetry
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Port_Asymmetry 
**                 message to FW for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl      - base address
**                 chnlType       - channel type
**                                  0 - HSCI
**                                  1 - LSCI
**                 chnlId         - channel ID
**                                  0 to 1  - HSCI
**                                  0 to 15 - LSCI
**                 delayAsymmetry - the delay asymmetry for this channel. 
**                                  delayAsymmetry is a 32-bit signed value in 
**                                  nanoseconds.
**                                  The maximum allowable value is +/- 1s.
**                 rxStaticOffset - static offset for the receive path.
**                                  rxStaticOffset is a 32-bit signed value in 
**                                  nanoseconds that will be added to the 
**                                  timestamp of each PTP message received on 
**                                  this channel. 
**                                  The maximum allowable value is +/- 1s.
**                 txStaticOffset - static offset for the transmit path. 
**                                  txStaticOffset is a 32-bit signed value in 
**                                  nanoseconds that will be added to the 
**                                  timestamp of each PTP message transmitted on
**                                  this channel.
**                                  The maximum allowable value is +/s 1s.   
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetPortAsymmetry(struct file *fileHndl,
                                              UINT4 chnlType, UINT4 chnlId,
                                              UINT4 delayAsymmetry,
                                              UINT4 rxStaticOffset,
                                              UINT4 txStaticOffset)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;

    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) || (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (delayAsymmetry > 0xFFFFFFFF) || (rxStaticOffset > 0xFFFFFFFF) ||
        (txStaticOffset > 0xFFFFFFFF) ||
        ((delayAsymmetry > 0x3B9ACA00) && (delayAsymmetry < 0xC4653600)) ||
        ((rxStaticOffset > 0x3B9ACA00) && (rxStaticOffset < 0xC4653600)) ||
        ((txStaticOffset > 0x3B9ACA00) && (txStaticOffset < 0xC4653600)))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x6;
    ibndMsgSubtype = 0x07;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and id */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) |
                   ((chnlId & 0xFF) << 16);

    /* delayAsymmetry, rxStaticOffset and txStaticOffset */
    pTmpIbndMsg++;
    *pTmpIbndMsg = delayAsymmetry; 
    
    pTmpIbndMsg++;
    *pTmpIbndMsg = rxStaticOffset; 
    
    pTmpIbndMsg++;               
    *pTmpIbndMsg = txStaticOffset;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetPortAsymmetry */


/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetTcParam
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_TC_Param
**                 message to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl       - base address
**                 clkIdMsw        - clockIdentity of the master used to 
**                                   syntonize this clock. 
**                                   (most significant word, 32-bit)
**                 clkIdLsw        - clockIdentity of the master used to 
**                                   syntonize this clock.
**                                   (least significant word, 32-bit)
**                 portNum         - portNumber of the master used to syntonize
**                                   this clock.
**                                   legal range 0x0 to 0xFFFF
**                 syntEnable      - enable or disable syntonization to the
**                                   master specified by clockIdentity and
**                                   portNumber.
**                                   0 - Disable
**                                   1 - Enable
**                 lsciChnlEnable  - Each bit position represents the 
**                                   corresponding port on an LSCI channel
**                                   0 - disable port for PTP
**                                   1 - enable port for PTP 
**                 hsciChnlEnable  - Each bit position represents the 
**                                   corresponding port on an HSCI channel
**                                   0 - disable port for PTP
**                                   1 - enable port for PTP 
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetTcParam(struct file *fileHndl, UINT4 clkIdMsw, 
                                        UINT4 clkIdLsw, UINT4 portNum, 
                                        UINT4 syntEnable, UINT4 lsciChnlEnable,
                                        UINT4 hsciChnlEnable)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */ 
    if ((clkIdMsw > 0xFFFFFFFF) || (clkIdLsw > 0xFFFFFFFF) ||
        (portNum > 0xFFFF) || (syntEnable > 1) || (lsciChnlEnable > 0xFFFF) ||
        (hsciChnlEnable > 0x3))  {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x6;
    ibndMsgSubtype = 0xA1;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* clkIdMsw and clkIdLsw */
    *pTmpIbndMsg = clkIdMsw;
    pTmpIbndMsg++;
    *pTmpIbndMsg = clkIdLsw;

    /* port number */
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((portNum & 0xFFFF) << 16) | 
                   ((syntEnable & 0xFF) << 8) ;
    
    /* lsciChnlEnable and hsciChnlEnable */
    pTmpIbndMsg++;
    *pTmpIbndMsg = ((lsciChnlEnable & 0xFFFF) << 16) | 
                   ((hsciChnlEnable & 0xFFFF) << 8) ;
    
    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetTcParam */


/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetPartialPtpParam
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_Partial_PTP_Param
**                 message to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl      - base address
**                 exterInterface - external interface
**                                   0 - PIF
**                                   1 - FE/GE Mgmt port
**                 vlanTci        - VLAN-TCI
**                                  Legal range 0x0 to 0xFFFF                                  
**                 rxTimestamp    - operations for a PTP packet on the 
**                                  received path
**                                  0 - record receive timestamp only
**                                  1 - record receive timestamp and update PTP
**                                      correction field
**                 eventFeedback  - feed back PTP event message to external
**                                  host. Each bit postion represents the 
**                                  corresponding PTP event message.
**                                  bit 0 - Sync
**                                  bit 1 - Delay Req
**                                  bit 2 - Pdelay Req
**                                  bit 3 - Pdelay Resp
**                                  0 - disable feedback
**                                  1 - enable feedabck
**                 hostMacAddr[6] - external host MAC address, the MAC address
**                                  of Customer's External Host that is used to 
**                                  handle pass-thru PTP.
**                                  Legal range 0 to 0xFF
**                                  For example: MAC address = 12-34-56-78-9a-bc                            
**                                  hostMacAddr[0] = 0x12
**                                  hostMacAddr[1] = 0x34
**                                  hostMacAddr[2] = 0x56
**                                  hostMacAddr[3] = 0x78
**                                  hostMacAddr[4] = 0x9a
**                                  hostMacAddr[5] = 0xbc
**                 mipsMacAddr[6] - the MAC address of MIPS of HYPHY that is 
**                                  used to exchange pass-thru PTP messages with
**                                  external host
**                                  Legal range 0 to 0xFF
**                                  For example: MAC address = 12-34-56-78-9a-bc                            
**                                  mipsMacAddr[0] = 0x12
**                                  mipsMacAddr[1] = 0x34
**                                  mipsMacAddr[2] = 0x56
**                                  mipsMacAddr[3] = 0x78
**                                  mipsMacAddr[4] = 0x9a
**                                  mipsMacAddr[5] = 0xbc
**                 vlanPresent   -  bit 1: PTP packet with vlan tagged.
**                                  bit 0: PTP packet without vlan tagged.
**                                  vlanPresent bits 31-24 for LsciPort[15:8]	
**                                  vlanPresent bits 23-16 for LsciPort [7:0]	
**                                  vlanPresent bits 9-8 for HsciPort [1:0]
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetPartialPtpParam(struct file *fileHndl, 
                                                UINT4 exterInterface, 
                                                UINT4 vlanTci, 
                                                UINT4 rxTimestamp, 
                                                UINT4 eventFeedback, 
                                                UINT4 hostMacAddr[6],
                                                UINT4 mipsMacAddr[6],
                                                UINT4 vlanPresent)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size, i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */ 
    if ((exterInterface > 1) || (vlanTci > 0xFFFF) ||
        (rxTimestamp > 1) || (eventFeedback > 0xF)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (i = 0; i < 6; i++) {
        if (hostMacAddr[i] > 0xFF || mipsMacAddr[i] > 0xFF) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }
    
    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x9;
    ibndMsgSubtype = 0x30;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* exterInterface, vlanTci and rxTimestamp */
    *pTmpIbndMsg = ((exterInterface & 0xFF) << 24) |
                   ((vlanTci & 0xFFFF) << 8) |
                   ((rxTimestamp & 0xFF) << 0);

    /* eventFeedback */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (eventFeedback & 0xFF) << 24;
    
    /* hostMacAddr[6] */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (hostMacAddr[0] << 24) |
                   (hostMacAddr[1] << 16) |
                   (hostMacAddr[2] <<  8) |
                   (hostMacAddr[3] <<  0);
    pTmpIbndMsg++;
    *pTmpIbndMsg = (hostMacAddr[4] << 24) |
                   (hostMacAddr[5] << 16);
    
    /* mipsMacAddr[6] */
    pTmpIbndMsg++;
    *pTmpIbndMsg = (mipsMacAddr[0] << 24) |
                   (mipsMacAddr[1] << 16) |
                   (mipsMacAddr[2] <<  8) |
                   (mipsMacAddr[3] <<  0);   
    pTmpIbndMsg++;
    *pTmpIbndMsg = (mipsMacAddr[4] << 24) |
                   (mipsMacAddr[5] << 16);
    
    /* vlanPresent */
    pTmpIbndMsg++;    
    *pTmpIbndMsg = (vlanPresent & 0xFFFF0300);
        
    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetPartialPtpParam */


/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPtpSetTcPortParam
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_PTP_Set_TC_Port_Param message 
**                 to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl     - base address
**                 chnlType      - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 chnlId        - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 logMinPDlyReq - log base 2 of the Pdelay_Req message
**                                 transmission interval. The supported
**                                 Pdelay_Req interval range is 1/128s - 16s,
**                                 0xF9 =< logMinPDlyReq <= 0x04.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPtpSetTcPortParam(struct file *fileHndl,
                                            UINT4 chnlType, UINT4 chnlId,
                                            UINT4 logMinPDlyReq)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) ||  (chnlType == 0 && chnlId > 1) ||
        (chnlType == 1 && chnlId > 15) ||
        (logMinPDlyReq > 0x4 && logMinPDlyReq < 0xF9))
    {
        DBG_PRINT("chnlType=0x%lx, chnlId=0x%lx\n", chnlType, chnlId);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_PTP;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0xA2;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* portType, portNumber and enable */
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) |
                   ((chnlId & 0xFF) << 16) |
                   ((logMinPDlyReq & 0xFF) << 8);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPtpSetTcPortParam */

/*******************************************************************************
**
**  hyPhy20gHostMsgChkObndDbelMskStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function checks the status of an outbound doorbell to be
**                 used by FW upon arrival of GFP-CMF.
**
**  INPUTS:        *fileHndl   - base address
**                 doorBellNum - doorbell number 
**                             - valid values are 0 to 31
**                               - Bit 0: Reserved for General messaging 
**                               - Bit 1: GFP-CMF arrival indication
**                               - Bit 2-31: Reserved
**                 status      - expected doorbell status
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHostMsgChkObndDbelMskStatus(struct file *fileHndl,
                                                UINT4 doorBellNum,
                                                UINT4 status)
{
	return hostMsgHyPhy20gHostMsgChkObndDbelMskStatus(fileHndl, doorBellNum, 
	                                                  status);
} /* hyPhy20gHostMsgChkObndDbelMskStatus */

/*******************************************************************************
**
**  hyPhy20gHostMsgPollObndDbelMskStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function checks the status of an outbound doorbell after 
**                 polling the outbound doorbell bit for a specified delay time.
**                 The status of outbound doorbell is used by FW upon arrival
**                 of GFP-CMF.
**
**  INPUTS:        *fileHndl   - base address
**                 doorBellNum - doorbell number 
**                             - valid values are 0 to 31
**                               - Bit 0: Reserved for General messaging
**                               - Bit 1: GFP-CMF arrival indication
**                               - Bit 2-31: Reserved
**                 status      - expected poll status
**                 msDelay     - polling time in milli-seconds
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gHostMsgPollObndDbelMskStatus(struct file *fileHndl,
                                                 UINT4 doorBellNum,
                                                 UINT4 status, UINT4 msDelay)
{
	return hostMsgHyPhy20gHostMsgPollObndDbelMskStatus(fileHndl, doorBellNum, 
	                                                   status, msDelay);
} /* hyPhy20gHostMsgPollObndDbelMskStatus */

/*******************************************************************************
**
**  hyPhy20gHostMsgClrObndDbel
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function clears the specified outbound doorbell.
**                 
**                  This function can be called after data has been retrieved 
**                  using the following GFP-CMF functions:
**
**               - hyPhy20gHostMsgCmdCfgCsfSetWanToLanCmfRetrOption
**               - hyPhy20gHostMsgCmdReqCsfGetWanToLanOneShotKeepAliveCmfMultiCh
**
**
**  INPUTS:        *fileHndl   - base address
**                 doorBellNum - doorbell number
**                             - valid values are 0 to 31
**                               - Bit 0: Reserved for General messaging
**                               - Bit 1: GFP-CMF arrival indication
**                               - Bit 2-31: Reserved
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHostMsgClrObndDbel(struct file *fileHndl, UINT4 doorBellNum)
{
	return hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
} /* hyPhy20gHostMsgClrObndDbel */

/*******************************************************************************
**
**  hyPhy20gHostMsgCmdCfgPmonCtrlEnbl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a 
**                 CMD_CFG_PMON_Control_Alarm_Counters_Collection message to 
**                 FW to enable or disable the PMON feature.
**
**  INPUTS:        *fileHndl     - base address
**                 enblPmon      - 
**                                 0 - disable FW PMON feature.
**                                 1 - enable FW PMON feature.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdCfgPmonCtrlEnbl(struct file *fileHndl, UINT4 enblPmon)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (enblPmon > 1)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_ENET_PMON;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x11;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* enblPmon */
    *pTmpIbndMsg = (enblPmon & 0xFF) << 24;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdCfgPmonCtrlEnbl */

/*******************************************************************************
**  
**  hyPhy20gHostMsgCmdReqPmonGetAlarms
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_PMON_Get_PMON_Alarms message
**                 to FW and returns FW response RSP_REQ_PMON_Get_PMON_Alarms.
**
**  INPUTS:        *fileHndl  - base address
**
**                 Response message parameters:
**
**                 *pRspMsgSize - pointer to storage the size of PMON alarms
**                                data.
**                 *pRspMsgAddr - pointer to storage the start address of a 
**                                contiguous memory area for PMON alarms.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqPmonGetAlarms(struct file *fileHndl, 
                                        UINT4 *pRspMsgSize, UINT4 *pRspMsgAddr)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (pRspMsgSize == NULL || pRspMsgAddr == NULL)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_ENET_PMON; 
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x12;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* reserved */
    *pTmpIbndMsg = 0;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);
    
    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    
    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */  
    *pRspMsgSize  = *pObndMsgBody;
    *pRspMsgAddr  = *(pObndMsgBody + 1);

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);
    DBG_PRINT("rspMsgSize=%lu, rspMsgAddr=0x%lx\n", *pRspMsgSize, *pRspMsgAddr);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqPmonGetAlarms */

/*******************************************************************************
**  
**  hyPhy20gHostMsgCmdReqPmonGetCnts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_PMON_Get_PMON_Counters 
**                 message to FW and returns FW response 
**                 RSP_REQ_PMON_Get_PMON_Counters.
**
**  INPUTS:        *fileHndl  - base address
**
**                 The following are pointers to storage for response
**                 parameters:
**
**                 *pRspMsgSize - pointer to storage the size of PMON counters
**                              data.
**                 *pRspMsgAddr - pointer to storage the start address of a 
**                              contiguous memory area for PMON counters.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCmdReqPmonGetCnts(struct file *fileHndl, 
                                      UINT4 *pRspMsgSize, UINT4 *pRspMsgAddr)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 *pObndMsgBody;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (pRspMsgSize == NULL || pRspMsgAddr == NULL)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_ENET_PMON; 
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x13;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* reserved */
    *pTmpIbndMsg = 0;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory for inbound message body */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* determine message body length and allocate memory */
    size = sizeof(UINT4)*(obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);
    
    pObndMsgBody = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pObndMsgBody == NULL) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    
    /* retrieve message body */
    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        sysHyPhy20gMemFree(pObndMsgBody);
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* decode message */  
    *pRspMsgSize  = *pObndMsgBody;
    *pRspMsgAddr  = *(pObndMsgBody + 1);

    /* free memory */
    sysHyPhy20gMemFree(pObndMsgBody);
    DBG_PRINT("rspMsgSize=%lu, rspMsgAddr=0x%lx\n", *pRspMsgSize, *pRspMsgAddr);

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCmdReqPmonGetCnts */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gHostMsgCreateCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates and intializes memory required to operate
**                  host messages.
**
**                  References this memory to the device level structure
**                  pointed to with *pDevCtxt.
**
**                  Function hyPhy20gHostMsgDestroyCtxt recovers this memory and
**                  set the device level reference to NULL.
**
**  INPUTS:         fileHndl        - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHostMsgCreateCtxt(struct file *fileHndl)
{
    /* variable declaration */
    sHYPHY20G_HOSTMSG_CTXT *pHostMsgCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 *pHostMsgSeqCount;
    UINT4 size;
    INT4 result;    
    UINT4 revNum,buildNum;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* If pHostMsgCtxt is not a NULL pointer,
       destroy it first to prevent memory leak */
    if (pDevCtxt->pHostMsgCtxt != NULL) {
        hyPhy20gHostMsgDestroyCtxt(fileHndl);
    }

    /* allocate memory for and associate with pDevCtxt */
    size = sizeof(sHYPHY20G_HOSTMSG_CTXT);
    pDevCtxt->pHostMsgCtxt = (sHYPHY20G_HOSTMSG_CTXT *) sysHyPhy20gMemAlloc(size);

    if(pDevCtxt->pHostMsgCtxt == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    DBG_PRINT("HostMsg Context memory allocated: pHostMsgCtxt addr=0x%x, size=%ld\n",
              (unsigned int)pDevCtxt->pHostMsgCtxt, size);
    pHostMsgCtxt = pDevCtxt->pHostMsgCtxt;

    size = sizeof(UINT4);
    pHostMsgCtxt->pHostMsgSeqCount = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if(pHostMsgCtxt->pHostMsgSeqCount == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        sysHyPhy20gMemFree(pDevCtxt->pHostMsgCtxt);
		return HYPHY20G_ERR_MEM_ALLOC;
    }
    DBG_PRINT("HostMsg Context memory allocated: pHostMsgSeqCount addr=0x%x, size=%ld\n",
              (unsigned int)pHostMsgCtxt->pHostMsgSeqCount, size);

    pHostMsgSeqCount = pHostMsgCtxt->pHostMsgSeqCount;

    /* initialize context */
    pHostMsgCtxt->valid = HYPHY20G_VALID_VAL_32BIT;
    *pHostMsgSeqCount = 0;
    /* store the running firmware version in pHostMsgCtxt->firmwareVer */
    result=hyPhy20gHostMsgCmdReqInfoGetVerInfo(fileHndl, HYPHY20G_HOSTMSG_GET_REV, &revNum, &buildNum);
    if (result != HYPHY20G_SUCCESS)
    {
        DBG_PRINT("%s, %s, %d, hyPhy20gHostMsgCmdReqInfoGetVerInfo fail.\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }
    pHostMsgCtxt->firmwareVer = revNum;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgCreateCtxt */

/*******************************************************************************
**
**  hyPhy20gHostMsgDestroyCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset context used by host messages.  Deallocates memory
**                  used for storing this information.
**
**  INPUTS:         fileHndl        - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHostMsgDestroyCtxt(struct file *fileHndl)
{
    /* variable declaration */
    sHYPHY20G_HOSTMSG_CTXT *pHostMsgCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* retrieve host message context */
    pHostMsgCtxt = pDevCtxt->pHostMsgCtxt;
    if(pHostMsgCtxt == NULL){
        /* Nothing to destroy */
        DBG_PRINT("%s, %s, %d, returning success as already NULL\n",__FILE__, __FUNCTION__, __LINE__);
    	return HYPHY20G_SUCCESS;
    }
	
	sysHyPhy20gMemFree(pHostMsgCtxt->pHostMsgSeqCount);
	DBG_PRINT("%s, %s, %d, returning success after pHostMsgSeqCount free\n",__FILE__, __FUNCTION__, __LINE__);

    /* destroy host message context */
    sysHyPhy20gMemFree(pDevCtxt->pHostMsgCtxt);
    DBG_PRINT("%s, %s, %d, returning success after free\n",__FILE__, __FUNCTION__, __LINE__);
    return HYPHY20G_SUCCESS;

} /* hyPhy20gHostMsgDestroyCtxt */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgGetCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Gets Host Message context elements.
**
**
**  INPUTS:         fileHndl   - device handle
**                  *pValid    - Pointer to storage for validity of
**                               context
**                                   HYPHY20G_VALID_VAL_32BIT - valid
**                                   Others - invalid
**                  *pSeqCount - Pointer to storage for sequence counter
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgGetCtxt(struct file *fileHndl, UINT4 *pValid,
                                   UINT4 *pSeqCount)
{
    sHYPHY20G_HOSTMSG_CTXT *pHostMsgCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 *pHostMsgSeqCount;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* retrieve host message context */
    pHostMsgCtxt = pDevCtxt->pHostMsgCtxt;
    HYPHY20G_ASSERT((pHostMsgCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pHostMsgCtxt is NULL.");

    pHostMsgSeqCount = pHostMsgCtxt->pHostMsgSeqCount;
    HYPHY20G_ASSERT((pHostMsgSeqCount != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pHostMsgSeqCount is NULL.");

    /* get context value */
    *pValid    = pHostMsgCtxt->valid;
    *pSeqCount = *pHostMsgSeqCount;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgGetCtxt */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgSetMskCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Sets Host Mesasge context elements.
**
**  INPUTS:         fileHndl - device handle
**                  valid    - Validity of context:
**                                 HYPHY20G_VALID_VAL_32BIT - valid
**                                 Others - invalid
**                  seqCount - Sequence count
**                                 Legal range 0 to 0xFFFF
**                  setMsk   - Mask of fields to update
**                                  bit 0: valid
**                                  bit 1: seqCount
**                             Values:
**                                  0 - do not update field
**                                  1 - update field
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgSetMskCtxt(struct file *fileHndl, UINT4 valid,
                                      UINT4 seqCount, UINT4 setMsk)
{    
    sHYPHY20G_HOSTMSG_CTXT *pHostMsgCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 *pHostMsgSeqCount;
    UINT4 devValid, devSeqCount;
    INT4 result;

    /* argument check */
    if ((seqCount > 0xFFFF) || (setMsk > 0x3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* retrieve host message context */
    pHostMsgCtxt = pDevCtxt->pHostMsgCtxt;
    HYPHY20G_ASSERT((pHostMsgCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pHostMsgCtxt is NULL.");

    pHostMsgSeqCount = pHostMsgCtxt->pHostMsgSeqCount;
    HYPHY20G_ASSERT((pHostMsgSeqCount != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pHostMsgSeqCount is NULL.");

    /* retrieve context value */
    result = hostMsgHyPhy20gHostMsgGetCtxt(fileHndl, &devValid, &devSeqCount);
    if (result) return result;

    /* update arguments based on setMsk */
    if (setMsk & 0x1) {
        devValid = valid;
    }
    if (setMsk & 0x2) {
        devSeqCount = seqCount;
    }

    /* update context settings */
    pHostMsgCtxt->valid = devValid;
    *pHostMsgSeqCount   = devSeqCount;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgSetMskCtxt */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgIbndMsgHdr
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function formats the message header and writes it
**                 into memory.
**
**  INPUTS:        *fileHndl    - base address
**                 modID        - FW module ID
**                 msgCat       - message category
**                 msgLen       - message length (2 to 64)
**                 msgSubtype   - message subtype
**                 msgCtrl      - large message control
**                 seqNum       - message sequence number
**                 *pMsgHdr     - pointer to storage for message header
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgIbndMsgHdr(struct file *fileHndl, UINT4 modID,
                                      UINT4 msgCat, UINT4 msgLen,
                                      UINT4 msgSubtype, UINT4 msgCtrl,
                                      UINT4 seqNum, UINT4 *pMsgHdr)
{
    UINT4 word1, word2;
    UINT4 *pTmpMsgHdr;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checks */
    if ((modID > MAX_MODID)
        || ((msgCat > CATEGORY_CMD_REQ  && msgCat < CATEGORY_RSP_AUTO) ||
           (msgCat > CATEGORY_RSP_REQ))
        || ((msgLen < HYPHY20G_HOSTMSG_NUM_HEADER_WORDS) ||
           (msgLen > HYPHY20G_HOSTMSG_MAX_MSG_LEN)
        || (msgCtrl > CONCATENATED_LST_MSG && msgCtrl != EXTEND_MSG)))
    {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pMsgHdr == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    DBG_PRINT("modID=0x%lx, msgCat=0x%lx, msgLen=0x%lx, msgSubtype=0x%lx, msgCtrl=0x%lx, seqNum=0x%lx,\n", modID, msgCat, msgLen, msgSubtype, msgCtrl, seqNum);

    /* assemble header (word 1 of 2) */
    word1 = 0x0;
    word1 = mHYPHY20G_SET_FIELD4(word1, modID, HYPHY20G_HOSTMSG_MODID_MSK,
                                 HYPHY20G_HOSTMSG_MODID_OFF);
    word1 = mHYPHY20G_SET_FIELD4(word1, msgCat, HYPHY20G_HOSTMSG_MSGCAT_MSK,
                                 HYPHY20G_HOSTMSG_MSGCAT_OFF);
    word1 = mHYPHY20G_SET_FIELD4(word1, msgLen, HYPHY20G_HOSTMSG_MSGLEN_MSK,
                                 HYPHY20G_HOSTMSG_MSGLEN_OFF);

    /* assemble header (word 2 of 2) */
    word2 = 0x0;
    word2 = mHYPHY20G_SET_FIELD4(word2, msgSubtype,
                                 HYPHY20G_HOSTMSG_MSGSUBTYP_MSK,
                                 HYPHY20G_HOSTMSG_MSGSUBTYP_OFF);
    word2 = mHYPHY20G_SET_FIELD4(word2, msgCtrl, HYPHY20G_HOSTMSG_MSGCTRL_MSK,
                                 HYPHY20G_HOSTMSG_MSGCTRL_OFF);
    word2 = mHYPHY20G_SET_FIELD4(word2, seqNum, HYPHY20G_HOSTMSG_MSGSEQNUM_MSK,
                                 HYPHY20G_HOSTMSG_MSGSEQNUM_OFF);

    /* write message header to memory */
    pTmpMsgHdr = pMsgHdr;
    *pTmpMsgHdr = word1;

    pTmpMsgHdr++;
    *pTmpMsgHdr = word2;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgIbndMsgHdr */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgSendIbndMsg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function writes a message to the inbound mailbox and
**                 notifies FW to retrieve the message by asserting inbound
**                 doorbell #0.
**
**  INPUTS:        *fileHndl    - base address
**                 msgLen       - message length (2 to 64)
**                 *pMsg        - pointer to message
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgSendIbndMsg(struct file *fileHndl, UINT4 msgLen,
                                       UINT4 *pMsg)
{
    UINT4 burstLen, numFullBurst;
    UINT4 addr;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 *pTmpMsg;
    UINT4 i;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checks */
    if ((msgLen < HYPHY20G_HOSTMSG_NUM_HEADER_WORDS) ||
        (msgLen > HYPHY20G_HOSTMSG_MAX_MSG_LEN))
    {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pMsg == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* make sure inbound doorbell is cleared*/
    result = hostMsgHyPhy20gHostMsgChkIbndDbelMskStatus(fileHndl, doorBellNum,
                                                        0);
    if (result) return result;

    numFullBurst = msgLen / 32;
    burstLen = msgLen % 32;
    DBG_PRINT("msgLen=0x%lx, numFullBurst=0x%lx, burstLen=0x%lx\n", msgLen, numFullBurst, burstLen);

    /* write message to inbox */
    pTmpMsg = pMsg;
    addr = HYPHY20G_HOSTMSG_IBND_MAILBOX_ADDR;
    for (i = 0; i < numFullBurst; i++) {
        result = sysHyPhy20gPcieBurstWrite (fileHndl, addr, 32, pTmpMsg);
        if (result) return result;

        addr = addr + (32 * 0x4);
        pTmpMsg = pTmpMsg + 32;
    }
    result = sysHyPhy20gPcieBurstWrite (fileHndl, addr, burstLen, pTmpMsg);
    if (result) return result;

    /* set inbound doorbell */
    result = hostMsgHyPhy20gHostMsgSetIbndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgSendIbndMsg */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgGetObndMsgHdr
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reads a message header from the outbound
**                 mailbox.
**
**  INPUTS:        *fileHndl    - base address
**                 *modID       - pointer to storage for FW module ID
**                 *msgCat      - pointer to storage for message category
**                 *msgLen      - pointer to storage for message length
**                 *msgSubtype  - pointer to storage for message subtype
**                 *msgCtrl     - pointer to storage for large message control
**                 *seqNum      - pointer to storage for message sequence number
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgGetObndMsgHdr(struct file *fileHndl, UINT4 *modID,
                                         UINT4 *msgCat, UINT4 *msgLen,
                                         UINT4 *msgSubtype, UINT4 *msgCtrl,
                                         UINT4 *seqNum)
{
    UINT4 word1, word2;
    UINT4 addr;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checks */
    if (modID == NULL || msgCat == NULL || msgLen == NULL || msgSubtype == NULL
        || msgCtrl == NULL || seqNum == NULL)
    {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* make sure outbound doorbell is set */
    result = hostMsgHyPhy20gHostMsgChkObndDbelMskStatus(fileHndl, doorBellNum,
                                                        1);
    if (result) return result;

    /* read header (words 1 and 2) from outbound mailbox */
    addr = HYPHY20G_HOSTMSG_OBND_MAILBOX_ADDR;
    result = sysHyPhy20gRead(fileHndl, addr, &word1);
    if (result) return result;
    addr += 0x4;
    result = sysHyPhy20gRead(fileHndl, addr, &word2);
    if (result) return result;

    /* parse header fields (word 1 of 2) */
    *modID = (word1 & HYPHY20G_HOSTMSG_MODID_MSK)
              >> HYPHY20G_HOSTMSG_MODID_OFF;
    *msgCat = (word1 & HYPHY20G_HOSTMSG_MSGCAT_MSK)
               >> HYPHY20G_HOSTMSG_MSGCAT_OFF;
    *msgLen = (word1 & HYPHY20G_HOSTMSG_MSGLEN_MSK)
               >> HYPHY20G_HOSTMSG_MSGLEN_OFF;

    /* parse header fields (word 2 of 2) */
    *msgSubtype = (word2 & HYPHY20G_HOSTMSG_MSGSUBTYP_MSK)
              >> HYPHY20G_HOSTMSG_MSGSUBTYP_OFF;
    *msgCtrl = (word2 & HYPHY20G_HOSTMSG_MSGCTRL_MSK)
               >> HYPHY20G_HOSTMSG_MSGCTRL_OFF;
    *seqNum = (word2 & HYPHY20G_HOSTMSG_MSGSEQNUM_MSK)
               >> HYPHY20G_HOSTMSG_MSGSEQNUM_OFF;

    DBG_PRINT("modID=0x%lx, msgCat=0x%lx, msgLen=%ld, msgSubtype=0x%lx, msgCtrl=0x%lx, seqNum=%ld\n", *modID, *msgCat, *msgLen, *msgSubtype, *msgCtrl, *seqNum);

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgGetObndMsgHdr */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgGetObndMsgBody
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reads the message body (ie. without headers)
**                 from the outbound mailbox.
**
**  INPUTS:        *fileHndl    - base address
**                 msgLen       - message length (including header)
**                 *msg         - pointer to storage for msg body.  *msg must
**                                be msgLen - 2 words in size.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgGetObndMsgBody(struct file *fileHndl, UINT4 msgLen,
                                          UINT4 *msg)
{
    UINT4 addr;
    UINT4 burstLen, numFullBurst;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 *pTmpMsg;
    UINT4 i;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checks */
    if (msgLen < HYPHY20G_HOSTMSG_MIN_MSG_LEN ||
        msgLen > HYPHY20G_HOSTMSG_MAX_MSG_LEN ||
        msg == NULL)
    {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* make sure outbound doorbell is set */
    result = hostMsgHyPhy20gHostMsgChkObndDbelMskStatus(fileHndl, doorBellNum,
                                                        1);
    if (result) return result;

    numFullBurst = (msgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS) / 32;
    burstLen = (msgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS) % 32;
    DBG_PRINT("msgLen=0x%lx, numFullBurst=0x%lx, burstLen=0x%lx\n", msgLen, numFullBurst, burstLen);

    /* skip header */
    pTmpMsg = msg;
    addr = HYPHY20G_HOSTMSG_OBND_MAILBOX_ADDR +
           (0x4 * HYPHY20G_HOSTMSG_NUM_HEADER_WORDS);

    /* read message body */
    for (i = 0; i < numFullBurst; i++) {
        result = sysHyPhy20gPcieBurstRead (fileHndl, addr, 32, pTmpMsg);
        if (result) return result;

        addr = addr + (32 * 0x4);
        pTmpMsg = pTmpMsg + 32;
    }
    result = sysHyPhy20gPcieBurstRead (fileHndl, addr, burstLen, pTmpMsg);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgGetObndMsgBody */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgChkObndRspCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reads a message from the outbound mailbox
**                 and confirms if it is a RSP_CFG message type.
**
**                 Message checks include FW module ID, category, length,
**                 subtype, control and sequence number.
**
**
**  INPUTS:        *fileHndl    - base address
**                 modID        - expected FW module ID
**                 subtype      - expected subtype
**                 seqNum       - expected sequence number
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgChkObndRspCfg(struct file *fileHndl, UINT4 modID,
                                         UINT4 subtype, UINT4 seqNum)
{
    UINT4 rspModID;
    UINT4 rspMsgCat;
    UINT4 rspMsgLen;
    UINT4 rspMsgSubtype;
    UINT4 rspMsgCtrl;
    UINT4 rspSeqNum;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 pollDelay;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checks */
    if (modID > MAX_MODID) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* poll delay is 10s for serdes cfg, 0.5s for everything else */
    pollDelay = ((modID == MODID_NGSERDES) && (subtype == CATEGORY_CMD_CFG)) ? 
                (HYPHY20G_HOSTMSG_10000_MSEC) : (HYPHY20G_HOSTMSG_500_MSEC);
        
    /* poll for outbound doorbell */
    result = hostMsgHyPhy20gHostMsgPollObndDbelMskStatus(fileHndl, doorBellNum,
                                                        1, pollDelay);
    if (result) return result;

    /* read outbound message header info */
    result = hostMsgHyPhy20gHostMsgGetObndMsgHdr(fileHndl, &rspModID,
                                                 &rspMsgCat, &rspMsgLen,
                                                 &rspMsgSubtype, &rspMsgCtrl,
                                                 &rspSeqNum);
    if (result) return result;

    /* check FW module ID */
    if (rspModID != modID) {
        DBG_PRINT("ERROR: rspModID=0x%lx not equal expected modID=0x%lx\n", rspModID, modID);
        return HYPHY20G_ERR_RSP_CFG;
    }

    /* check message category */
    if (rspMsgCat != CATEGORY_RSP_CFG) {
        if (rspMsgCat == CATEGORY_RSP_NACK) {
            return HYPHY20G_ERR_RSP_NACK;
        } else {
            DBG_PRINT("ERROR: rspMsgCat=0x%lx not equal expected CATEGORY_RSP_CFG=0x%x\n", rspMsgCat, CATEGORY_RSP_CFG);
            return HYPHY20G_ERR_RSP_CFG;
        }
    }

    /* check message length */
    if (rspMsgLen != 0x2) {
        DBG_PRINT("ERROR: rspMsgLen=0x%lx not equal expected 0x2\n", rspMsgLen);
        return HYPHY20G_ERR_RSP_CFG;
    }

    /* check message subtype */
    if (rspMsgSubtype != subtype) {
        DBG_PRINT("ERROR: rspMsgSubtype=0x%lx not equal expected subtype=0x%lx\n", rspMsgSubtype, subtype);
        return HYPHY20G_ERR_RSP_CFG;
    }

    /* check message control */
    if (rspMsgCtrl != BASIC_MSG) {
        DBG_PRINT("ERROR: rspMsgCtrl=0x%lx not equal expected BASIC_MSG=0x%x\n", rspMsgCtrl, BASIC_MSG);
        return HYPHY20G_ERR_RSP_CFG;
    }

    /* check sequence number */
    if (rspSeqNum != seqNum) {
        DBG_PRINT("ERROR: rspSeqNum=0x%lx not equal expected seqNum=0x%lx\n", rspSeqNum, seqNum);
        return HYPHY20G_ERR_RSP_CFG;
    }

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgChkObndRspCfg */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgChkObndRspReqHdr
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reads the message header from the outbound
**                 mailbox and confirms if it is a RSP_REQ message type.
**
**                 Header checks include FW module ID, category,
**                 subtype and sequence number.
**
**                 Returns message length and message control type to caller.
**
**  INPUTS:        *fileHndl    - base address
**                 modID        - expected FW module ID
**                 subtype      - expected subtype
**                 seqNum       - expected sequence number
**                 *rspLen      - pointer to storage for response message's
**                                length field
**                 *rspCtrl     - pointer to storage for response message's
**                                control field
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgChkObndRspReqHdr(struct file *fileHndl, UINT4 modID,
                                            UINT4 subtype, UINT4 seqNum,
                                            UINT4 *rspLen, UINT4 *rspCtrl)
{
    UINT4 rspModID;
    UINT4 rspMsgCat;
    UINT4 rspMsgLen;
    UINT4 rspMsgSubtype;
    UINT4 rspMsgCtrl;
    UINT4 rspSeqNum;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checks */
    if (modID > MAX_MODID) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* poll for outbound doorbell */
    result = hostMsgHyPhy20gHostMsgPollObndDbelMskStatus(fileHndl, doorBellNum,
                                        1, HYPHY20G_HOSTMSG_DOORBELL_POLL_TIME);
    if (result) return result;

    /* read outbound message header info */
    result = hostMsgHyPhy20gHostMsgGetObndMsgHdr(fileHndl, &rspModID,
                                                 &rspMsgCat, &rspMsgLen,
                                                 &rspMsgSubtype, &rspMsgCtrl,
                                                 &rspSeqNum);
    if (result) return result;

    /* check FW module ID */
    if (rspModID != modID) {
        DBG_PRINT("ERROR: rspModID=0x%lx not equal expected modID=0x%lx\n", rspModID, modID);
        return HYPHY20G_ERR_RSP_CFG;
    }

    /* check message category */
    if (rspMsgCat != CATEGORY_RSP_REQ) {
        if (rspMsgCat == CATEGORY_RSP_NACK) {
            return HYPHY20G_ERR_RSP_NACK;
        } else {
            DBG_PRINT("ERROR: rspMsgCat=0x%lx not equal expected CATEGORY_RSP_REQ=0x%x\n", rspMsgCat, CATEGORY_RSP_REQ);
            return HYPHY20G_ERR_RSP_REQ;
        }
    }

    /* check message subtype */
    if (rspMsgSubtype != subtype) {
        DBG_PRINT("ERROR: rspMsgSubtype=0x%lx not equal expected subtype=0x%lx\n", rspMsgSubtype, subtype);
        return HYPHY20G_ERR_RSP_REQ;
    }

    /* check sequence number */
    if (rspSeqNum != seqNum) {
        DBG_PRINT("ERROR: rspSeqNum=0x%lx not equal expected seqNum=0x%lx\n", rspSeqNum, seqNum);
        return HYPHY20G_ERR_RSP_REQ;
    }

    /* return message length and control fields */
    *rspLen = rspMsgLen;
    *rspCtrl = rspMsgCtrl;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgChkObndRspReqHdr */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgChkIbndDbelMskStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function compares the MASKED value of the specified
**                 inbound doorbell status with the expected value.
**
**  INPUTS:        *fileHndl   - base address
**                 doorBellNum - doorbell number (range 0 to 31)
**                 status      - expected doorbell status
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgChkIbndDbelMskStatus(struct file *fileHndl,
                                                UINT4 doorBellNum,
                                                UINT4 status)
{
    UINT4 val, rdStatus;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (doorBellNum >= HYPHY20G_HOSTMSG_MAX_IBND_DOORBELL_NUM || status > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = sysHyPhy20gRead(fileHndl, HYPHY20G_PCIE_APP_REG_IBND_DBEL_MSK_STAT,
                             &val);
    if (result) return result;

    rdStatus = (val >> doorBellNum) & 0x1;
    if (rdStatus != status) {
        return HYPHY20G_ERR_STATUS_CHECK_MISMATCH;
    }

    return HYPHY20G_SUCCESS;
} /* hostMsgHyPhy20gHostMsgChkIbndDbelMskStatus */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgSetIbndDbel
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sets the specified inbound doorbell.
**
**  INPUTS:        *fileHndl   - base address
**                 doorBellNum - doorbell number (range 0 to 31)
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgSetIbndDbel(struct file *fileHndl, UINT4 doorBellNum)
{
    UINT4 val;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (doorBellNum >= HYPHY20G_HOSTMSG_MAX_IBND_DOORBELL_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    val = 0x1 << doorBellNum;
    result = sysHyPhy20gWrite(fileHndl, HYPHY20G_PCIE_APP_REG_IBND_DBEL_SET,
                              val);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hostMsgHyPhy20gHostMsgSetIbndDbel */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgChkObndDbelMskStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function compares the MASKED value of the specified
**                 outbound doorbell status with the expected value.
**
**  INPUTS:        *fileHndl   - base address
**                 doorBellNum - doorbell number (range 0 to 31)
**                 status      - expected doorbell status
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgChkObndDbelMskStatus(struct file *fileHndl,
                                                UINT4 doorBellNum,
                                                UINT4 status)
{
    UINT4 val, rdStatus;
    INT4 result;

    //DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (doorBellNum >= HYPHY20G_HOSTMSG_MAX_OBND_DOORBELL_NUM || status > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = sysHyPhy20gRead(fileHndl, HYPHY20G_PCIE_APP_REG_OBND_DBEL_MSK_STAT,
                             &val);
    if (result) return result;

    rdStatus = (val >> doorBellNum) & 0x1;
    if (rdStatus != status) {
        return HYPHY20G_ERR_STATUS_CHECK_MISMATCH;
    }

    return HYPHY20G_SUCCESS;
} /* hostMsgHyPhy20gHostMsgChkObndDbelMskStatus */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgPollObndDbelMskStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function compares the MASKED value of the specified
**                 outbound doorbell status with the expected value.
**
**  INPUTS:        *fileHndl   - base address
**                 doorBellNum - doorbell number (range 0 to 31)
**                 status      - expected poll status
**                 msDelay     - polling time in milli-seconds
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgPollObndDbelMskStatus(struct file *fileHndl,
                                                 UINT4 doorBellNum,
                                                 UINT4 status, UINT4 msDelay)
{
    UINT4 i;
    UINT4 usDelay;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (doorBellNum >= HYPHY20G_HOSTMSG_MAX_OBND_DOORBELL_NUM || status > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    usDelay = msDelay * HYPHY20G_USEC_PER_MSEC;
    /* Poll masked field until same as pollFlg value, or quit after delay cycles. */
    for (i = 0; i < (usDelay + 1); i++) {
        /* Read register containing status bit */
        result = hostMsgHyPhy20gHostMsgChkObndDbelMskStatus(fileHndl,
                                                            doorBellNum,
                                                            status);
        if (result == HYPHY20G_SUCCESS) {
            return HYPHY20G_SUCCESS;
        } else if (result == HYPHY20G_ERR_STATUS_CHECK_MISMATCH) {
            /* continue polling */
        } else {
            return result;
        }

        if (i < usDelay) {
            /* wait 1 us between reads except for last possible read */
            result = sysHyPhy20gUsDelay(1);
            if (result) return result;
        }
    }

    return HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT;

} /* hostMsgHyPhy20gHostMsgPollObndDbelMskStatus */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgClrObndDbel
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function clears the specified outbound doorbell.
**                 
**
**  INPUTS:        *fileHndl   - base address
**                 doorBellNum - doorbell number (range 0 to 31)
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgClrObndDbel(struct file *fileHndl, UINT4 doorBellNum)
{
    UINT4 val;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (doorBellNum >= HYPHY20G_HOSTMSG_MAX_OBND_DOORBELL_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    val = 0x1 << doorBellNum;
    result = sysHyPhy20gWrite(fileHndl, HYPHY20G_PCIE_APP_REG_OBND_DBEL_CLR,
                              val);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hostMsgHyPhy20gHostMsgClrObndDbel */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgNextAvailSeqNum
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function returns the next available message sequence
**                 number.
**
**  INPUTS:        *fileHndl   - base address
**                 *pvalue     - pointer to storage for status value
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgNextAvailSeqNum(struct file *fileHndl, UINT4 *pvalue)
{
    UINT4 valid, count;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    result = hostMsgHyPhy20gHostMsgGetCtxt(fileHndl, &valid, &count);
    if (result) return result;
    
    if (count == 0xFFFF) {
        count = 1;
    } else {
        count++;
    }
    
    result = hostMsgHyPhy20gHostMsgSetMskCtxt(fileHndl, valid, count, 0x2);
    if (result) return result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);
    
    *pvalue = count;

    DBG_PRINT("hostMsgSeqCount=0x%lx\n", count);

    return HYPHY20G_SUCCESS;
} /* hostMsgHyPhy20gHostMsgNextAvailSeqNum */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCurrSeqNum
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function returns the current message sequence
**                 number.
**
**  INPUTS:        *fileHndl   - base address
**                 *pvalue     - pointer to storage for status value
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCurrSeqNum(struct file *fileHndl, UINT4 *pvalue)
{
    UINT4 valid, count;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    result = hostMsgHyPhy20gHostMsgGetCtxt(fileHndl, &valid, &count);
    if (result) return result;

    *pvalue = count;

    DBG_PRINT("hostMsgSeqCount=0x%lx\n", count);

    return HYPHY20G_SUCCESS;
} /* hostMsgHyPhy20gHostMsgCurrSeqNum */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgGetExtMsg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function reads the SPRAM for the extended message.
**
**  INPUTS:        *fileHndl - base address
**                 msgLen    - extended message length
**                 msgAddr   - SPRAM address of extended message
**                 *msg      - pointer to storage for extended message
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgGetExtMsg(struct file *fileHndl, UINT4 msgLen,
                                     UINT4 msgAddr, UINT4 *msg)
{
    UINT4 addr;
    UINT4 burstLen, numFullBurst;
    UINT4 *pTmpMsg;
    UINT4 i;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checks */
    if (msg == NULL){
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    numFullBurst = msgLen / 32;
    burstLen = msgLen % 32;
    DBG_PRINT("msgLen=0x%lx, numFullBurst=0x%lx, burstLen=0x%lx\n", msgLen, numFullBurst, burstLen);

    /* skip header */
    pTmpMsg = msg;
    addr = msgAddr;

    /* read message body */
    for (i = 0; i < numFullBurst; i++) {
        result = sysHyPhy20gPcieBurstRead (fileHndl, addr, 32, pTmpMsg);
        if (result) return result;

        addr = addr + (32 * 0x4);
        pTmpMsg = pTmpMsg + 32;
    }
    result = sysHyPhy20gPcieBurstRead (fileHndl, addr, burstLen, pTmpMsg);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgGetExtMsg */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgLnkOamCfgChnl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_LINKOAM_Config_Channel message
**                 to FW and waits for FW response.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlCnt     - number of channels to configure (1 to 18)
**
**                 Configuration for each channel must contain the following
**                 parameters:
**
**                 *pChnlType   - pointer to storage for channel type
**                                  0 - HSCI
**                                  1 - LSCI
**                 *pChnlId     - pointer to storage for channel ID
**                                  0 to 1  - HSCI
**                                  0 to 15 - LSCI
**                 *pVarRetrSup - pointer to storage for variable retrieval
**                                  0 - DTE does not support sending Variable
**                                      Respsonse OAMPDUS
**                                  1 - DTE supports sending Variable Respsonse
**                                      OAMPDUS
**                 *pLnkEvntSup - pointer to storage for link events
**                                  0 - DTE does not support interpreting
**                                      Link Events
**                                  1 - DTE supports interpreting Link Events
**                 *pLbkSup     - pointer to storage for OAM remote loopback
**                                support
**                                  0 - DTE is not capbable of OAM remote
**                                      loopback mode
**                                  1 - DTE is capbable of OAM remote loopback
**                                      mode
**                 *pUniDirSup  - pointer to storage for unidirectional support
**                                  0 - DTE is not capbable of sending OAMPDUs
**                                      when receive path is non-operational
**                                  1 - DTE is capbable of sending OAMPDUs
**                                      when receive path is non-operational
**                 *pMode       - pointer to storage for OAM mode
**                                  0 - DTE is configured in Passive mode
**                                  1 - DTE is configured in Active mode
**                 *pRate       - pointer to storage for OAM generation rate
**                                (2 to 10)
**                 *pPduMaxSize - pointer to storage for OAMPDU max size
**                                (42 to 1496)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgLnkOamCfgChnl(struct file *fileHndl,
                                               UINT4 chnlCnt,
                                               UINT4 *pChnlType,
                                               UINT4 *pChnlId,
                                               UINT4 *pVarRetrSup,
                                               UINT4 *pLnkEvntSup,
                                               UINT4 *pLbkSup,
                                               UINT4 *pUniDirSup,
                                               UINT4 *pMode,
                                               UINT4 *pRate,
                                               UINT4 *pPduMaxSize)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 ibndICfg;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndChnlType;
    UINT4 *pTmpIbndChnlId;
    UINT4 *pTmpIbndVarRetrSup;
    UINT4 *pTmpIbndLnkEvntSup;
    UINT4 *pTmpIbndLbkSup;
    UINT4 *pTmpIbndUniDirSup;
    UINT4 *pTmpIbndMode;
    UINT4 *pTmpIbndRate;
    UINT4 *pTmpIbndPduMaxSize;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    DBG_PRINT("chnlCnt=%lu\n", chnlCnt);

    /* set temp pointers */
    pTmpIbndChnlType   = pChnlType;
    pTmpIbndChnlId     = pChnlId;
    pTmpIbndVarRetrSup = pVarRetrSup;
    pTmpIbndLnkEvntSup = pLnkEvntSup;
    pTmpIbndLbkSup     = pLbkSup;
    pTmpIbndUniDirSup  = pUniDirSup;
    pTmpIbndMode       = pMode;
    pTmpIbndRate       = pRate;
    pTmpIbndPduMaxSize = pPduMaxSize;

    //// DEBUG
    //for (i = 0; i < chnlCnt; i++) {
    //    DBG_PRINT("i=%lu\n", i);
    //    DBG_PRINT("pTmpIbndChnlType=%lu, pTmpIbndChnlId=%lu, pTmpIbndDteMode=%lu, pTmpIbndPktRate=%lu, pTmpIbndMipSup=%lu\n", *pTmpIbndChnlType, *pTmpIbndChnlId, *pTmpIbndDteMode, *pTmpIbndPktRate, *pTmpIbndMipSup);
    //    pTmpIbndChnlType++;
    //    pTmpIbndChnlId++;
    //    pTmpIbndDteMode++;
    //    pTmpIbndPktRate++;
    //    pTmpIbndMipSup++;
    //}
    //pTmpIbndChnlType = pChnlType;
    //pTmpIbndChnlId   = pChnlId;
    //pTmpIbndDteMode  = pDteMode;
    //pTmpIbndPktRate  = pPktRate;
    //pTmpIbndMipSup   = pMipSup;
    //// END DEBUG

    /* argument check */
    if (chnlCnt < 1 || chnlCnt > 18) {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < chnlCnt; i++) {
        if ((*pTmpIbndChnlType > 1) ||
            (*pTmpIbndChnlType == 0 && *pTmpIbndChnlId > 1) ||
            (*pTmpIbndChnlType == 1 && *pTmpIbndChnlId > 15) ||
            (*pVarRetrSup > 1) || (*pLnkEvntSup > 1) ||
            (*pLbkSup > 1) || (*pUniDirSup > 1) ||
            (*pMode > 1) ||
            (*pRate < 2 || *pRate > 10) ||
            (*pPduMaxSize < 42 || *pPduMaxSize > 1496))
        {
            DBG_PRINT("error 1\n");
            return HYPHY20G_ERR_INVALID_ARG;
        }
        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndVarRetrSup++;
        pTmpIbndLnkEvntSup++;
        pTmpIbndLbkSup++;
        pTmpIbndUniDirSup++;
        pTmpIbndMode++;
        pTmpIbndRate++;
        pTmpIbndPduMaxSize++;
    }

    /* set message header parameters */
    ibndModID = MODID_LINKOAM;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = HYPHY20G_HOSTMSG_NUM_HEADER_WORDS + 1 + (2*chnlCnt);
    ibndMsgSubtype = 0x01;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* start assembling message body (ie. without header) */
    pTmpIbndMsg        = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    pTmpIbndChnlType   = pChnlType;
    pTmpIbndChnlId     = pChnlId;
    pTmpIbndVarRetrSup = pVarRetrSup;
    pTmpIbndLnkEvntSup = pLnkEvntSup;
    pTmpIbndLbkSup     = pLbkSup;
    pTmpIbndUniDirSup  = pUniDirSup;
    pTmpIbndMode       = pMode;
    pTmpIbndRate       = pRate;
    pTmpIbndPduMaxSize = pPduMaxSize;

    /* channel count */
    *pTmpIbndMsg = (chnlCnt & 0xFF) << 24;
    for (i = 0; i < chnlCnt; i++) {
        /* channel type and ID */
        DBG_PRINT("pTmpIbndChnlType=0x%lx, pTmpIbndChnlId=0x%lx\n", *pTmpIbndChnlType, *pTmpIbndChnlId);

        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndChnlType & 0xFF) << 24) |
                       ((*pTmpIbndChnlId & 0xFF) << 16);

        /* config, rate and PDU max size */
        DBG_PRINT("pTmpIbndVarRetrSup=0x%lx, pTmpIbndLnkEvntSup=0x%lx, pTmpIbndLbkSup=0x%lx, pTmpIbndUniDirSup=0x%lx, pTmpIbndMode=0x%lx, pTmpIbndRate=0x%lx, pTmpIbndPduMaxSize=0x%lx\n", *pTmpIbndVarRetrSup, *pTmpIbndLnkEvntSup, *pTmpIbndLbkSup, *pTmpIbndUniDirSup, *pTmpIbndMode, *pTmpIbndRate, *pTmpIbndPduMaxSize);
        ibndICfg = 0;
        ibndICfg = (*pTmpIbndVarRetrSup << 4) | (*pTmpIbndLnkEvntSup << 3) |
                   (*pTmpIbndLbkSup << 2) | (*pTmpIbndUniDirSup << 1) |
                   (*pTmpIbndMode << 0);
        pTmpIbndMsg++;
        *pTmpIbndMsg = ((ibndICfg & 0xFF) << 24) |
                       ((*pTmpIbndRate & 0xFF) << 16) |
                       ((*pTmpIbndPduMaxSize & 0xFFFF) << 0);

        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndVarRetrSup++;
        pTmpIbndLnkEvntSup++;
        pTmpIbndLbkSup++;
        pTmpIbndUniDirSup++;
        pTmpIbndMode++;
        pTmpIbndRate++;
        pTmpIbndPduMaxSize++;
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgLnkOamCfgChnl */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdReqLnkOamGetInfo
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_LINKOAM_Get_Infomation message
**                 to FW and waits for FW response.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlCnt     - number of channels to configure (1 to 4)
**
**                 Configuration for each channel must contain the following
**                 parameters:
**
**                 *pChnlType  - pointer to storage for channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 *pChnlId    - pointer to storage for channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**
**                 Response message parameters:
**
**                 *pObndMsgBodyLen - pointer to storage for outbound message
**                                    body length without headers
**                 *pObndMsgBody    - pointer to storage for outbound message
**                                    body without header info and size of
**                                    storage must be (1 + 8*chnlCnt)*4 bytes
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdReqLnkOamGetInfo(struct file *fileHndl,
                                               UINT4 chnlCnt, UINT4 *pChnlType,
                                               UINT4 *pChnlId,
                                               UINT4 *pObndMsgBodyLen,
                                               UINT4 *pObndMsgBody)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndChnlType;
    UINT4 *pTmpIbndChnlId;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    DBG_PRINT("chnlCnt=%lu\n", chnlCnt);

    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;

    /* argument check */
    if (chnlCnt < 1 || chnlCnt > 4) {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < chnlCnt; i++) {
        if ((*pTmpIbndChnlType > 1) ||
            (*pTmpIbndChnlType == 0 && *pTmpIbndChnlId > 1) ||
            (*pTmpIbndChnlType == 1 && *pTmpIbndChnlId > 15))
        {
            DBG_PRINT("error 1\n");
            return HYPHY20G_ERR_INVALID_ARG;
        }
        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
    }

    /* set message header parameters */
    ibndModID = MODID_LINKOAM;
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3 + chnlCnt;
    ibndMsgSubtype = 0x02;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* channel count */
    *pTmpIbndMsg = (chnlCnt & 0xFF) << 24;

    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    for (i = 0; i < chnlCnt; i++) {
        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndChnlType & 0xFF) << 24) |
                       ((*pTmpIbndChnlId & 0xFF) << 16);
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    *pObndMsgBodyLen = obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }


    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdReqLnkOamGetInfo */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdReqLnkOamGetRemoteMibVar
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_LINKOAM_Get_Remote_MIB_Variable
**                 message to FW and waits for FW response.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlCnt     - number of channels to configure (1 to 4)
**
**                 Configuration for each channel must contain the following
**                 parameters:
**
**                 *pChnlType  - pointer to storage for channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 *pChnlId    - pointer to storage for channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**
**                 Response message parameters:
**
**                 *pObndMsgBodyLen - pointer to storage for outbound message
**                                    body length without headers
**                 *pObndMsgBody    - pointer to storage for outbound message
**                                    body without header info
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdReqLnkOamGetRemoteMibVar(struct file *fileHndl,
                                                       UINT4 chnlCnt,
                                                       UINT4 *pChnlType,
                                                       UINT4 *pChnlId,
                                                       UINT4 *pObndMsgBodyLen,
                                                       UINT4 *pObndMsgBody)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndChnlType;
    UINT4 *pTmpIbndChnlId;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    DBG_PRINT("chnlCnt=%lu\n", chnlCnt);

    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;

    /* argument check */
    if (chnlCnt < 1 || chnlCnt > 4) {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < chnlCnt; i++) {
        if ((*pTmpIbndChnlType > 1) ||
            (*pTmpIbndChnlType == 0 && *pTmpIbndChnlId > 1) ||
            (*pTmpIbndChnlType == 1 && *pTmpIbndChnlId > 15))
        {
            DBG_PRINT("error 1\n");
            return HYPHY20G_ERR_INVALID_ARG;
        }
        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
    }

    /* set message header parameters */
    ibndModID = MODID_LINKOAM;
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3 + chnlCnt;
    ibndMsgSubtype = 0x03;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* channel count */
    *pTmpIbndMsg = (chnlCnt & 0xFF) << 24;

    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    for (i = 0; i < chnlCnt; i++) {
        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndChnlType & 0xFF) << 24) |
                       ((*pTmpIbndChnlId & 0xFF) << 16);
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    *pObndMsgBodyLen = obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdReqLnkOamGetRemoteMibVar */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgLnkOamSetLpbk
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_LINKOAM_Set_Loopback message
**                 to FW and waits for FW response.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlCnt     - number of channels to configure (1 to 18)
**
**                 Configuration for each channel must contain the following
**                 parameters:
**
**                 *pChnlType  - pointer to storage for channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 *pChnlId    - pointer to storage for channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 *pLpbkType  - pointer to storage for loopback type
**                                 0 for local loopback
**                                 1 for remote loopback
**                 *pEnable    - pointer to storage for enable
**                                 0 to disable
**                                 1 to enable
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgLnkOamSetLpbk(struct file *fileHndl,
                                               UINT4 chnlCnt, UINT4 *pChnlType,
                                               UINT4 *pChnlId, UINT4 *pLpbkType,
                                               UINT4 *pEnable)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndChnlType;
    UINT4 *pTmpIbndChnlId;
    UINT4 *pTmpIbndLpbkType;
    UINT4 *pTmpIbndEnable;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 enable;
    UINT4 size;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    DBG_PRINT("chnlCnt=%lu\n", chnlCnt);

    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndLpbkType = pLpbkType;
    pTmpIbndEnable   = pEnable;

    /* argument check */
    if (chnlCnt < 1 || chnlCnt > 18) {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < chnlCnt; i++) {
        if ((*pTmpIbndChnlType > 1) ||
            (*pTmpIbndChnlType == 0 && *pTmpIbndChnlId > 1) ||
            (*pTmpIbndChnlType == 1 && *pTmpIbndChnlId > 15) ||
            (*pTmpIbndEnable > 1))
        {
            DBG_PRINT("error 2\n");
            return HYPHY20G_ERR_INVALID_ARG;
        }
        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndLpbkType++;
        pTmpIbndEnable++;
    }

    /* set message header parameters */
    ibndModID = MODID_LINKOAM;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x3 + (2 * chnlCnt);
    ibndMsgSubtype = 0x04;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (chnlCnt & 0xFF) << 24;

    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndLpbkType = pLpbkType;
    pTmpIbndEnable   = pEnable;
    for (i = 0; i < chnlCnt; i++) {
        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndChnlType & 0xFF) << 24) |
                       ((*pTmpIbndChnlId & 0xFF) << 16);
        DBG_PRINT("reg1=0x%lx\n", *pTmpIbndMsg);

        if (*pTmpIbndEnable == 1) {
            /* enable */
            enable = 1;
        } else {
            /* disable */
            enable = 2;
        }

        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndLpbkType & 0xFF) << 24) |
                       (enable << 16);
        DBG_PRINT("reg2=0x%lx\n", *pTmpIbndMsg);

        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndLpbkType++;
        pTmpIbndEnable++;
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgLnkOamSetLpbk */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgLnkOamSetCritEvntNotf
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_LINKOAM_Set_Critical_Event_Notification message
**                 to FW and waits for FW response.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlCnt     - number of channels to configure (1 to 18)
**
**                 Configuration for each channel must contain the following
**                 parameters:
**
**                 *pChnlType  - pointer to storage for channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 *pChnlId    - pointer to storage for channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 *pCritEvnt  - pointer to storage for critical event
**                                 0 - link fault
**                                 1 - dying gasp
**                                 2 - critical event
**                                 3 - clear all critical event flags
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgLnkOamSetCritEvntNotf(struct file *fileHndl,
                                                       UINT4 chnlCnt,
                                                       UINT4 *pChnlType,
                                                       UINT4 *pChnlId,
                                                       UINT4 *pCritEvnt)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndChnlType;
    UINT4 *pTmpIbndChnlId;
    UINT4 *pTmpInbdCritEvnt;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    DBG_PRINT("chnlCnt=%lu\n", chnlCnt);

    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpInbdCritEvnt = pCritEvnt;

    /* argument check */
    if (chnlCnt < 1 || chnlCnt > 18) {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < chnlCnt; i++) {
        if ((*pTmpIbndChnlType > 1) ||
            (*pTmpIbndChnlType == 0 && *pTmpIbndChnlId > 1) ||
            (*pTmpIbndChnlType == 1 && *pTmpIbndChnlId > 15) ||
            (*pTmpInbdCritEvnt > 3))
        {
            DBG_PRINT("error 1\n");
            return HYPHY20G_ERR_INVALID_ARG;
        }
        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpInbdCritEvnt++;
    }

    /* set message header parameters */
    ibndModID = MODID_LINKOAM;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x3 + 2 * chnlCnt;
    ibndMsgSubtype = 0x06;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* start assembling message body (ie. without header) */
    pTmpIbndMsg      = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpInbdCritEvnt = pCritEvnt;

    /* channel count */
    *pTmpIbndMsg = (chnlCnt & 0xFF) << 24;

    for (i = 0; i < chnlCnt; i++) {
        /* channel type and ID */
        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndChnlType & 0xFF) << 24) |
                       ((*pTmpIbndChnlId & 0xFF) << 16);

        /* critical event */
        pTmpIbndMsg++;
        if (*pTmpInbdCritEvnt == 3) {
            /* clear all flags */
            *pTmpIbndMsg = 0;
        } else {
            /* set corresponding flag */
            *pTmpIbndMsg = ((0x1 << *pTmpInbdCritEvnt) << 24);
        }

        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpInbdCritEvnt++;
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgLnkOamSetCritEvntNotf */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgLnkOamCtrlChnl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_LINKOAM_Control_Channel message
**                 to FW and waits for FW response.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlCnt     - number of channels to configure (1 to 18)
**
**                 Configuration for each channel must contain the following
**                 parameters:
**
**                 *pChnlType  - pointer to storage for channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 *pChnlId    - pointer to storage for channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 *pEnable    - pointer to storage for enable/disable flag
**                                 0 to disable channel
**                                 1 to enable channel
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgLnkOamCtrlChnl(struct file *fileHndl,
                                                UINT4 chnlCnt, UINT4 *pChnlType,
                                                UINT4 *pChnlId, UINT4 *pEnable)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndChnlType;
    UINT4 *pTmpIbndChnlId;
    UINT4 *pTmpIbndEnable;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 enableChnlHsci;
    UINT4 disableChnlHsci;
    UINT4 enableChnlLsci;
    UINT4 disableChnlLsci;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    DBG_PRINT("chnlCnt=%lu\n", chnlCnt);

    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndEnable   = pEnable;

    //// DEBUG
    //for (i = 0; i < chnlCnt; i++) {
    //    DBG_PRINT("i=%lu\n", i);
    //    DBG_PRINT("pTmpIbndChnlType=%lu, pTmpIbndChnlId=%lu, pTmpIbndEnable=%lu\n", *pTmpIbndChnlType, *pTmpIbndChnlId, *pTmpIbndEnable);
    //    pTmpIbndChnlType++;
    //    pTmpIbndChnlId++;
    //    pTmpIbndEnable++;
    //}
    //pTmpIbndChnlType = pChnlType;
    //pTmpIbndChnlId   = pChnlId;
    //pTmpIbndEnable   = pEnable;
    //// END DEBUG

    /* argument check */
    if (chnlCnt < 1 || chnlCnt > 18) {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < chnlCnt; i++) {
        if ((*pTmpIbndChnlType > 1) ||
            (*pTmpIbndChnlType == 0 && *pTmpIbndChnlId > 1) ||
            (*pTmpIbndChnlType == 1 && *pTmpIbndChnlId > 15) ||
            (*pTmpIbndEnable > 1))
        {
            DBG_PRINT("error 1\n");
            return HYPHY20G_ERR_INVALID_ARG;
        }
        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndEnable++;
    }

    /* set message header parameters */
    ibndModID = MODID_LINKOAM;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x6;
    ibndMsgSubtype = 0x07;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    enableChnlHsci  = 0;
    disableChnlHsci = 0;
    enableChnlLsci  = 0;
    disableChnlLsci = 0;
    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndEnable   = pEnable;
    for (i = 0; i < chnlCnt; i++) {
        /* HSCI */
        if (*pTmpIbndChnlType == 0) {
            if (*pTmpIbndEnable == 1) {
                enableChnlHsci |= (0x1 << *pTmpIbndChnlId);
            } else {
                disableChnlHsci |= (0x1 << *pTmpIbndChnlId);
            }
        }

        /* LSCI */
        if (*pTmpIbndChnlType == 1) {
            if (*pTmpIbndEnable == 1) {
                enableChnlLsci |= (0x1 << *pTmpIbndChnlId);
            } else {
                disableChnlLsci |= (0x1 << *pTmpIbndChnlId);
            }
        }

        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndEnable++;
    }

    DBG_PRINT("enableChnlHsci=0x%lx, disableChnlHsci=0x%lx, enableChnlLsci=0x%lx, disableChnlLsci=0x%lx\n", enableChnlHsci, disableChnlHsci, enableChnlLsci, disableChnlLsci);

    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = enableChnlHsci;

    pTmpIbndMsg++;
    *pTmpIbndMsg = disableChnlHsci;

    pTmpIbndMsg++;
    *pTmpIbndMsg = enableChnlLsci;

    pTmpIbndMsg++;
    *pTmpIbndMsg = disableChnlLsci;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype,
                                                 ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgLnkOamCtrlChnl */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgLnkOamSendMibVarReq
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_LINKOAM_Send_MIB_Variables_Request message to FW and
**                 waits for FW response.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlCnt     - number of channels to configure (1 to 18)
**
**                 Configuration for each channel must contain the following
**                 parameters:
**
**                 *pChnlType  - pointer to storage for channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 *pChnlId    - pointer to storage for channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 *pBranch     - pointer to storage for branch variable
**                                (8-bits)
**                 *pLeaf       - pointer to storage for leaf variable
**                                (16-bits)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgLnkOamSendMibVarReq(struct file *fileHndl,
                                                     UINT4 chnlCnt,
                                                     UINT4 *pChnlType,
                                                     UINT4 *pChnlId,
                                                     UINT4 *pBranch,
                                                     UINT4 *pLeaf)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndChnlType;
    UINT4 *pTmpIbndChnlId;
    UINT4 *pTmpIbndBranch;
    UINT4 *pTmpIbndLeaf;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    DBG_PRINT("chnlCnt=%lu\n", chnlCnt);

    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndBranch   = pBranch;
    pTmpIbndLeaf     = pLeaf;

    /* argument check */
    if (chnlCnt < 1 || chnlCnt > 18) {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < chnlCnt; i++) {
        if ((*pTmpIbndChnlType > 1) ||
            (*pTmpIbndChnlType == 0 && *pTmpIbndChnlId > 1) ||
            (*pTmpIbndChnlType == 1 && *pTmpIbndChnlId > 15) ||
            (*pTmpIbndBranch > 0xFF) || (*pTmpIbndLeaf > 0xFFFF))
        {
            DBG_PRINT("error 1\n");
            return HYPHY20G_ERR_INVALID_ARG;
        }
        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndBranch++;
        pTmpIbndLeaf++;
    }

    /* set message header parameters */
    ibndModID = MODID_LINKOAM;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x3 + (2 * chnlCnt);
    ibndMsgSubtype = 0x09;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = (chnlCnt & 0xFF) << 24;

    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndBranch   = pBranch;
    pTmpIbndLeaf     = pLeaf;
    for (i = 0; i < chnlCnt; i++) {
        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndChnlType & 0xFF) << 24) |
                       ((*pTmpIbndChnlId & 0xFF) << 16);

        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndBranch & 0xFF) << 24) |
                       ((*pTmpIbndLeaf & 0xFFFF) << 8);

        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndBranch++;
        pTmpIbndLeaf++;
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgLnkOamSendMibVarReq */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdReqLnkOamGetRemoteLnkEvnt
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_REQ_LINKOAM_Get_Remote_Link_Event
**                 message to FW and waits for FW response.
**
**  INPUTS:        *fileHndl - base address
**                 chnlType  - channel type
**                               0 - HSCI
**                               1 - LSCI
**                 chnlId    - channel ID
**                               0 to 1  - HSCI
**                               0 to 15 - LSCI
**
**                 Response message parameters:
**
**                 *pObndMsgLen   - pointer to storage for response message
**                                  length
**                 *pObndMsgBody  - pointer to storage for response message
**                                - must be large enough to store 30 words
**                                  (32-bits per word)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdReqLnkOamGetRemoteLnkEvnt(struct file *fileHndl,
                                                        UINT4 chnlType,
                                                        UINT4 chnlId,
                                                        UINT4 *pObndMsgLen,
                                                        UINT4 *pObndMsgBody)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndMsg;
    UINT4 obndMsgLen;
    UINT4 obndMsgCtrl;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((chnlType > 1) ||
        (chnlType == 0 && chnlId > 1) || (chnlType == 1 && chnlId > 15))
    {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* set message header parameters */
    ibndModID = MODID_LINKOAM;
    ibndMsgCat = CATEGORY_CMD_REQ;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x0b;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* channel type and ID */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = ((chnlType & 0xFF) << 24) | ((chnlId & 0xFF) << 16);

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspReqHdr(fileHndl, ibndModID,
                                                    ibndMsgSubtype, ibndSeqNum,
                                                    &obndMsgLen, &obndMsgCtrl);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    result = hostMsgHyPhy20gHostMsgGetObndMsgBody(fileHndl, obndMsgLen,
                                                  pObndMsgBody);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    *pObndMsgLen = obndMsgLen - HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdReqLnkOamGetRemoteLnkEvnt */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgOtnSetInitState
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_OTN_Set_InitState message
**                 to FW and waits for FW response.
**
**  INPUTS:        *fileHndl   - base address
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgOtnSetInitState(struct file *fileHndl)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    CHECK_FW_OP_ENABLED(fileHndl);

    /* set message header parameters */
    ibndModID = MODID_OTN;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x3;
    ibndMsgSubtype = 0x01;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = 0x01;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgOtnSetInitState */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgSyncECfgChnl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_SYNCE_Config_Channel
**                 message to FW and waits for FW response.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlCnt     - number of channels to configure (1 to 18)
**
**                 Configuration for each channel must contain the following
**                 parameters:
**
**                 *pChnlType   - pointer to storage for channel type
**                                  0 - HSCI
**                                  1 - LSCI
**                 *pChnlId     - pointer to storage for channel ID
**                                  0 to 1  - HSCI
**                                  0 to 15 - LSCI
**                 *pMode       - pointer to storage for mode
**                                  0 - slave
**                                  1 - master
**                                  2 - passive
**                 *pQlSSM      - pointer to storage for SSM code of quality
**                                level
**                                Legal range 0x0 to 0xF
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgSyncECfgChnl(struct file *fileHndl,
                                              UINT4 chnlCnt, UINT4 *pChnlType,
                                              UINT4 *pChnlId, UINT4 *pMode,
                                              UINT4 *pQlSSM)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 ibndQLSSMCode;
    UINT4 ibndQLType;
    UINT4 ibndQLLen;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndChnlType;
    UINT4 *pTmpIbndChnlId;
    UINT4 *pTmpIbndMode;
    UINT4 *pTmpIbndQlSSM;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    DBG_PRINT("chnlCnt=%lu\n", chnlCnt);

    /* set temp pointers */
    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndMode     = pMode;
    pTmpIbndQlSSM    = pQlSSM;

    /* argument check */
    if (chnlCnt < 1 || chnlCnt > 18) {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < chnlCnt; i++) {
        if ((*pTmpIbndChnlType > 1) ||
            (*pTmpIbndChnlType == 0 && *pTmpIbndChnlId > 1) ||
            (*pTmpIbndChnlType == 1 && *pTmpIbndChnlId > 15) ||
            (*pTmpIbndMode > 2) || (*pTmpIbndQlSSM > 15))
        {
            DBG_PRINT("error 1\n");
            return HYPHY20G_ERR_INVALID_ARG;
        }
        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndMode++;
        pTmpIbndQlSSM++;
    }

    /* set message header parameters */
    ibndModID = MODID_SYNC_E;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = HYPHY20G_HOSTMSG_NUM_HEADER_WORDS + 1 + (3*chnlCnt);
    ibndMsgSubtype = 0x0;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* prepare pointers for assembling message body (ie. without header) */
    pTmpIbndMsg      = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndMode     = pMode;
    pTmpIbndQlSSM    = pQlSSM;

    /* channel count */
    *pTmpIbndMsg = (chnlCnt & 0xFF) << 24;

    for (i = 0; i < chnlCnt; i++) {
        /* channel type and ID */
        DBG_PRINT("pTmpIbndChnlType=0x%lx, pTmpIbndChnlId=0x%lx\n", *pTmpIbndChnlType, *pTmpIbndChnlId);

        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndChnlType & 0xFF) << 24) |
                       ((*pTmpIbndChnlId & 0xFF) << 16);

        /* mode */
        pTmpIbndMsg++;
        *pTmpIbndMsg = *pTmpIbndMode & 0xFF;

        /* QL_type, QL_length, QL_SSM */
        ibndQLType = 0x1;
        ibndQLLen = 0x4;

        ibndQLSSMCode = *pTmpIbndQlSSM;

        pTmpIbndMsg++;
        *pTmpIbndMsg = ((ibndQLType << 24) | (ibndQLLen << 8) |
                        (ibndQLSSMCode << 0));

        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndMode++;
        pTmpIbndQlSSM++;
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgSyncECfgChnl */


/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgSyncECtrlChnl
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_SYNCE_Control_Channel message
**                 to FW and waits for FW response.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlCnt     - number of channels to configure (1 to 18)
**
**                 Configuration for each channel must contain the following
**                 parameters:
**
**                 *pChnlType  - pointer to storage for channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 *pChnlId    - pointer to storage for channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 *pEnable    - pointer to storage for enable/disable flag
**                                 0 to disable channel
**                                 1 to enable channel
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgSyncECtrlChnl(struct file *fileHndl,
                                               UINT4 chnlCnt, UINT4 *pChnlType,
                                               UINT4 *pChnlId, UINT4 *pEnable)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndChnlType;
    UINT4 *pTmpIbndChnlId;
    UINT4 *pTmpIbndEnable;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 enableChnlHsci;
    UINT4 disableChnlHsci;
    UINT4 enableChnlLsci;
    UINT4 disableChnlLsci;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    DBG_PRINT("chnlCnt=%lu\n", chnlCnt);

    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndEnable   = pEnable;

    /* argument check */
    if (chnlCnt < 1 || chnlCnt > 18) {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < chnlCnt; i++) {
        if ((*pTmpIbndChnlType > 1) ||
            (*pTmpIbndChnlType == 0 && *pTmpIbndChnlId > 1) ||
            (*pTmpIbndChnlType == 1 && *pTmpIbndChnlId > 15) ||
            (*pTmpIbndEnable > 1))
        {
            DBG_PRINT("error 1\n");
            return HYPHY20G_ERR_INVALID_ARG;
        }
        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndEnable++;
    }

    /* set message header parameters */
    ibndModID = MODID_SYNC_E;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = 0x6;
    ibndMsgSubtype = 0x01;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    enableChnlHsci  = 0;
    disableChnlHsci = 0;
    enableChnlLsci  = 0;
    disableChnlLsci = 0;
    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndEnable   = pEnable;
    for (i = 0; i < chnlCnt; i++) {
        /* HSCI */
        if (*pTmpIbndChnlType == 0) {
            if (*pTmpIbndEnable == 1) {
                enableChnlHsci |= (0x1 << *pTmpIbndChnlId);
            } else {
                disableChnlHsci |= (0x1 << *pTmpIbndChnlId);
            }
        }

        /* LSCI */
        if (*pTmpIbndChnlType == 1) {
            if (*pTmpIbndEnable == 1) {
                enableChnlLsci |= (0x1 << *pTmpIbndChnlId);
            } else {
                disableChnlLsci |= (0x1 << *pTmpIbndChnlId);
            }
        }

        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndEnable++;
    }

    DBG_PRINT("enableChnlHsci=0x%lx, disableChnlHsci=0x%lx, enableChnlLsci=0x%lx, disableChnlLsci=0x%lx\n", enableChnlHsci, disableChnlHsci, enableChnlLsci, disableChnlLsci);

    pTmpIbndMsg  = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = enableChnlHsci;

    pTmpIbndMsg++;
    *pTmpIbndMsg = disableChnlHsci;

    pTmpIbndMsg++;
    *pTmpIbndMsg = enableChnlLsci;

    pTmpIbndMsg++;
    *pTmpIbndMsg = disableChnlLsci;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype,
                                                 ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgSyncECtrlChnl */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgSyncETxSSM
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_SYNCE_Transmit_SSM
**                 message to FW and waits for FW response.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlCnt     - number of channels to configure (1 to 18)
**
**                 Configuration for each channel must contain the following
**                 parameters:
**
**                 *pChnlType   - pointer to storage for channel type
**                                  0 - HSCI
**                                  1 - LSCI
**                 *pChnlId     - pointer to storage for channel ID
**                                  0 to 1  - HSCI
**                                  0 to 15 - LSCI
**                 *pEvntFlag   - pointer to storage for event flag
**                                  0 - information PDU
**                                  1 - event PDU
**                 *pQlSSM      - pointer to storage for SSM code of quality
**                                level
**                                Legal range 0x0 to 0xF
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgSyncETxSSM(struct file *fileHndl,
                                            UINT4 chnlCnt, UINT4 *pChnlType,
                                            UINT4 *pChnlId, UINT4 *pEvntFlag,
                                            UINT4 *pQlSSM)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 ibndQLSSMCode;
    UINT4 ibndQLType;
    UINT4 ibndQLLen;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndChnlType;
    UINT4 *pTmpIbndChnlId;
    UINT4 *pTmpIbndEvntFlag;
    UINT4 *pTmpIbndQlSSM;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    DBG_PRINT("chnlCnt=%lu\n", chnlCnt);

    /* set temp pointers */
    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndEvntFlag = pEvntFlag;
    pTmpIbndQlSSM   = pQlSSM;

    /* argument check */
    if (chnlCnt < 1 || chnlCnt > 18) {
        DBG_PRINT("error 1\n");
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < chnlCnt; i++) {
        if ((*pTmpIbndChnlType > 1) ||
            (*pTmpIbndChnlType == 0 && *pTmpIbndChnlId > 1) ||
            (*pTmpIbndChnlType == 1 && *pTmpIbndChnlId > 15) ||
            (*pTmpIbndEvntFlag > 1) || (*pTmpIbndQlSSM > 15))
        {
            DBG_PRINT("error 1\n");
            return HYPHY20G_ERR_INVALID_ARG;
        }
        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndEvntFlag++;
        pTmpIbndQlSSM++;
    }

    /* set message header parameters */
    ibndModID = MODID_SYNC_E;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = HYPHY20G_HOSTMSG_NUM_HEADER_WORDS + 1 + (3*chnlCnt);
    ibndMsgSubtype = 0x3;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;
    DBG_PRINT("ibndMsgLen=0x%lx, ibndSeqNum=%lu\n", ibndMsgLen, ibndSeqNum);

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* prepare pointers for assembling message body (ie. without header) */
    pTmpIbndMsg      = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndEvntFlag = pEvntFlag;
    pTmpIbndQlSSM    = pQlSSM;

    /* channel count */
    *pTmpIbndMsg = (chnlCnt & 0xFF) << 24;

    for (i = 0; i < chnlCnt; i++) {
        /* channel type and ID */
        DBG_PRINT("pTmpIbndChnlType=0x%lx, pTmpIbndChnlId=0x%lx\n", *pTmpIbndChnlType, *pTmpIbndChnlId);

        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndChnlType & 0xFF) << 24) |
                       ((*pTmpIbndChnlId & 0xFF) << 16);

        /* event flag */
        pTmpIbndMsg++;
        *pTmpIbndMsg = *pTmpIbndEvntFlag & 0xFF;

        /* QL_type, QL_length, QL_SSM */
        ibndQLType = 0x1;
        ibndQLLen = 0x4;

        ibndQLSSMCode = *pTmpIbndQlSSM;

        pTmpIbndMsg++;
        *pTmpIbndMsg = ((ibndQLType << 24) | (ibndQLLen << 8) |
                        (ibndQLSSMCode << 0));

        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndEvntFlag++;
        pTmpIbndQlSSM++;
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgSyncETxSSM */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgCsfSetLanToWanPortMapTable
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Set_LanToWan_Port_Mapping_Table message to FW
**                 and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlCnt     - number of channels to configure (1 to 18)
**
**                 Configuration for each channel must contain the following
**                 parameters:
**
**                 *pChnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 *pChnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 *pGfpIdx    - GFP instance
**                                 0 - Slice 'A'
**                                 1 - Slice 'B'
**                 *pGfpChnl   - GFP channel number
**                               Legal range 0x0 to 0x3F (0-63)
**                 *pSerdes    - SERDES interface mapping
**                                 0 to 15  - SFP ports 0 to 15
**                                 16 to 17 - XFI ports 0 to 1
**                                 18       - Dedicated SFI interface
**                                 19       - Shared SFI interface
**                 *pVcatGrp   - VCAT group number
**                               Legal range 0x0 to 0x3F (0-63)
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgCsfSetLanToWanPortMapTable(struct file *fileHndl,
                                                            UINT4 chnlCnt,
                                                            UINT4 *pChnlType,
                                                            UINT4 *pChnlId,
                                                            UINT4 *pGfpIdx,
                                                            UINT4 *pGfpChnl,
                                                            UINT4 *pSerdes,
                                                            UINT4 *pVcatGrp)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndChnlType;
    UINT4 *pTmpIbndChnlId;
    UINT4 *pTmpIbndGfpIdx;
    UINT4 *pTmpIbndGfpChnl;
    UINT4 *pTmpIbndSerdes;
    UINT4 *pTmpIbndVcatGrp;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* set temp pointers */
    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndGfpIdx   = pGfpIdx;
    pTmpIbndGfpChnl  = pGfpChnl;
    pTmpIbndSerdes   = pSerdes;
    pTmpIbndVcatGrp  = pVcatGrp;

    /* argument check */
    if (chnlCnt < 1 || chnlCnt > 18) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < chnlCnt; i++) {
        if ((*pTmpIbndChnlType > 1) ||
            (*pTmpIbndChnlType == 0 && *pTmpIbndChnlId > 1) ||
            (*pTmpIbndChnlType == 1 && *pTmpIbndChnlId > 15) ||
            (*pTmpIbndGfpIdx > 1) || (*pTmpIbndGfpChnl > 63) ||
            (*pTmpIbndSerdes > 19) || (*pTmpIbndVcatGrp > 63))
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndGfpIdx++;
        pTmpIbndGfpChnl++;
        pTmpIbndSerdes++;
        pTmpIbndVcatGrp++;
    }

    /* set message header parameters */
    ibndModID = MODID_CMF;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = HYPHY20G_HOSTMSG_NUM_HEADER_WORDS + 1 + (2*chnlCnt);
    ibndMsgSubtype = 0x02;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* start assembling message body (ie. without header) */
    pTmpIbndMsg      = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndGfpIdx   = pGfpIdx;
    pTmpIbndGfpChnl  = pGfpChnl;
    pTmpIbndSerdes   = pSerdes;
    pTmpIbndVcatGrp  = pVcatGrp;

    /* channel count */
    *pTmpIbndMsg = (chnlCnt & 0xFF) << 24;
    for (i = 0; i < chnlCnt; i++) {
        /* port type, port number, serdes, vcat group */
        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndChnlType & 0xFF) << 24) |
                       ((*pTmpIbndChnlId   & 0xFF) << 16) |
                       ((*pTmpIbndSerdes   & 0xFF) << 8)  |
                       ((*pTmpIbndVcatGrp  & 0xFF) << 0);

        /* gfp idx, gfp ch */
        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndGfpIdx  & 0xFF) << 24) |
                       ((*pTmpIbndGfpChnl & 0xFF) << 16);

        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndGfpIdx++;
        pTmpIbndGfpChnl++;
        pTmpIbndSerdes++;
        pTmpIbndVcatGrp++;
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgCsfSetLanToWanPortMapTable */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgCsfSetLanToWanPortActTable
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Set_LanToWan_Port_Action_Table message to FW for
**                 and waits for a RSP_CFG response from FW.
**
**                 If linear frame extension header is used (exi == 1), the
**                 values for the CID and eHEC fields must be specified, while
**                 the spare field is automatically forced to zero.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlCnt     - number of channels to configure (1 to 18)
**
**                 Configuration for each channel must contain the following
**                 parameters:
**
**                 *pChnlType    - channel type
**                                   0 - HSCI
**                                   1 - LSCI
**                 *pChnlId      - channel ID
**                                   0 to 1  - HSCI
**                                   0 to 15 - LSCI
**                 *pTableIdx    - action table index
**                                 Legal range 0 to 15 (table 0 is pre-defined
**                                 by FW to disable all consequential actions)
**                 *pSendDci     - 0 - do not send DCI when defects are cleared
**                                 1 - send DCI when defects are cleared
**                 *pExi         - extension header identifier
**                                   0 - null extension header
**                                   1 - linear frame
**                 *pCid         - For exi == 1 (otherwise set to 0)
**                                 Channel ID (CID) field (0x0 to 0xFF)
**                 *pEHec        - For exi == 1 (otherwise set to 0)
**                                 Extension HEC field (0x0 to 0xFFFF)
**                 *pGenKpAlvCmf - generate Keep-Alive CMF every 100ms
**                                   0 - do not generate Keep-Alive CMF
**                                   1 - generate Keep-Alive CMF with UPI and
**                                       8-byte client payload data
**                 *pUsePyld     - use 8-byte client payload data with regular
**                                 CSF (CMF with UPI = 1 or 2)
**                                   0 - do not include client payload
**                                   1 - include 8-byte client payload specified
**                                       by cmfPyld
**                 *pGenPyldFcs  - For usePyld == 1 (otherwise set to 0)
**                                 Generate 4-byte payload FCS
**                                   0 - do not generate payload FCS
**                                   1 - generate payload FCS
**                 *pUpi         - For genKpAlvCmf == 1 (otherwise set to 0)
**                                 UPI value used for Keep-Alive CMF
**                                 Legal range 0x0 to 0xFF
**                 *pCmfPyld     - If genKpAlvCmf == 0 and usePyld == 0, set to 0.
**                                 8-byte client payload data used in Keep-Alive
**                                 CMF and/or regular CSF.
**                                   cmfPyld[0] - payload octet N in CMF
**                                   cmfPyld[1] - payload octet N+1 in CMF
**                                   ...
**                                   cmfPyld[7] - payload octet N+7 in CMF
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgCsfSetLanToWanPortActTable(struct file *fileHndl,
                                                            UINT4 chnlCnt,
                                                            UINT4 *pChnlType,
                                                            UINT4 *pChnlId,
                                                            UINT4 *pTableIdx,
                                                            UINT4 *pSendDci,
                                                            UINT4 *pExi,
                                                            UINT4 *pCid,
                                                            UINT4 *pEHec,
                                                            UINT4 *pGenKpAlvCmf,
                                                            UINT4 *pUsePyld,
                                                            UINT4 *pGenPyldFcs,
                                                            UINT4 *pUpi,
                                                            UINT4 *pCmfPyld)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndChnlType;
    UINT4 *pTmpIbndChnlId;
    UINT4 *pTmpIbndTableIdx;
    UINT4 *pTmpIbndSendDci;
    UINT4 *pTmpIbndExi;
    UINT4 *pTmpIbndCid;
    UINT4 *pTmpIbndEHec;
    UINT4 *pTmpGenKpAlvCmf;
    UINT4 *pTmpUsePyld;
    UINT4 *pTmpGenPyldFcs;
    UINT4 *pTmpUpi;
    UINT4 *pTmpCmfPyld;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i, j;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* set temp pointers */
    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndTableIdx = pTableIdx;
    pTmpIbndSendDci  = pSendDci;
    pTmpIbndExi      = pExi;
    pTmpIbndCid      = pCid;
    pTmpIbndEHec     = pEHec;
    pTmpGenKpAlvCmf  = pGenKpAlvCmf;
    pTmpUsePyld      = pUsePyld;
    pTmpGenPyldFcs   = pGenPyldFcs;
    pTmpUpi          = pUpi;
    pTmpCmfPyld      = pCmfPyld;

    /* argument check */
    if (chnlCnt < 1 || chnlCnt > 18) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < chnlCnt; i++) {
        if ((*pTmpIbndChnlType > 1) ||
            (*pTmpIbndChnlType == 0 && *pTmpIbndChnlId > 1) ||
            (*pTmpIbndChnlType == 1 && *pTmpIbndChnlId > 15) ||
            (*pTmpIbndTableIdx > 15) || (*pTmpIbndSendDci > 1) ||
            (*pTmpIbndExi > 1) || (*pTmpIbndExi == 1 && *pTmpIbndCid > 0xFF) ||
            (*pTmpIbndExi == 1 && *pTmpIbndEHec > 0xFFFF) ||
            (*pTmpGenKpAlvCmf > 1) || (*pTmpUsePyld > 1) || (*pTmpGenPyldFcs > 1) ||
            (*pTmpUpi > 0xFF))
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }

        pTmpCmfPyld = pCmfPyld + 8*i;
        for (j = 0; j < 8; j++) {
            if (*pTmpCmfPyld > 0xFF) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
            }
        }

        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndTableIdx++;
        pTmpIbndSendDci++;
        pTmpIbndExi++;
        pTmpIbndCid++;
        pTmpIbndEHec++;
        pTmpGenKpAlvCmf++;
        pTmpUsePyld++;
        pTmpGenPyldFcs++;
        pTmpUpi++;
    }

    /* set message header parameters */
    ibndModID = MODID_CMF;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = HYPHY20G_HOSTMSG_NUM_HEADER_WORDS + 1 + (6*chnlCnt);
    ibndMsgSubtype = 0x03;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* start assembling message body (ie. without header) */
    pTmpIbndMsg      = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndTableIdx = pTableIdx;
    pTmpIbndSendDci  = pSendDci;
    pTmpIbndExi      = pExi;
    pTmpIbndCid      = pCid;
    pTmpIbndEHec     = pEHec;
    pTmpGenKpAlvCmf  = pGenKpAlvCmf;
    pTmpUsePyld      = pUsePyld;
    pTmpGenPyldFcs   = pGenPyldFcs;
    pTmpUpi          = pUpi;
    pTmpCmfPyld      = pCmfPyld;

    /* channel count */
    *pTmpIbndMsg = (chnlCnt & 0xFF) << 24;
    for (i = 0; i < chnlCnt; i++) {
        /* port type, port number, action, send dci */
        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndChnlType & 0xFF) << 24) |
                       ((*pTmpIbndChnlId   & 0xFF) << 16) |
                       ((*pTmpIbndTableIdx & 0xFF) << 8)  |
                       ((*pTmpIbndSendDci  & 0xFF) << 0);

        /* exi */
        pTmpIbndMsg++;
        *pTmpIbndMsg = (*pTmpIbndExi & 0xFF) << 24;

        /* exi info - cid, spare, eHEC */
        pTmpIbndMsg++;
        *pTmpIbndMsg = (*pTmpIbndCid  << 24) |
                       (*pTmpIbndEHec << 0 );

        /* genKeepAliveCMF, usePayloadInCSF, genPayloadFCS,
           upi_for_KeepAliveCMF */
        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpGenKpAlvCmf & 0xFF) << 24) |
                       ((*pTmpUsePyld     & 0xFF) << 16) |
                       ((*pTmpGenPyldFcs  & 0xFF) << 8)  |
                       ((*pTmpUpi         & 0xFF) << 0);

        /* cmfPayload */
        pTmpIbndMsg++;
        pTmpCmfPyld = pCmfPyld + 8*i;
        *pTmpIbndMsg = ((*(pTmpCmfPyld+0) & 0xFF) << 24) |
                       ((*(pTmpCmfPyld+1) & 0xFF) << 16) |
                       ((*(pTmpCmfPyld+2) & 0xFF) << 8)  |
                       ((*(pTmpCmfPyld+3) & 0xFF) << 0);

        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*(pTmpCmfPyld+4) & 0xFF) << 24) |
                       ((*(pTmpCmfPyld+5) & 0xFF) << 16) |
                       ((*(pTmpCmfPyld+6) & 0xFF) << 8)  |
                       ((*(pTmpCmfPyld+7) & 0xFF) << 0);

        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndTableIdx++;
        pTmpIbndSendDci++;
        pTmpIbndExi++;
        pTmpIbndCid++;
        pTmpIbndEHec++;
        pTmpGenKpAlvCmf++;
        pTmpUsePyld++;
        pTmpGenPyldFcs++;
        pTmpUpi++;
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgCsfSetLanToWanPortActTable */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgCsfSetWanToLanMapTable
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Set_WanToLan_Mapping_Table message to FW
**                 and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl   - base address
**                 chnlCnt     - number of channels to configure (1 to 30)
**
**                 Configuration for each channel must contain the following
**                 parameters:
**
**                 *pChnlType  - channel type
**                                 0 - HSCI
**                                 1 - LSCI
**                 *pChnlId    - channel ID
**                                 0 to 1  - HSCI
**                                 0 to 15 - LSCI
**                 *pGfpIdx    - GFP instance
**                                 0 - Slice 'A'
**                                 1 - Slice 'B'
**                 *pGfpChnl   - GFP channel number
**                               Legal range 0x0 to 0x3F (0-63)
**                 *pAction    - consequential action
**                                 0 - No Firmware action required
**                                 1 - Transparent GbE
**                                 2 - G.Sup43 Clause 7.3 10 GbE
**                                 3 - MAC Terminated GbE
**                                 4 - Insert local fault ordered set in MAC 
**                                     Terminated 10 GbE
**                                 5 - MAC Terminated FE - User Pattern
**                                 6 - MAC Terminated FE - FEFI
**                                 7 - Insert remote fault ordered set in MAC 
**                                     Terminated 10 GbE
**                 *pUpi       - CMF UPI
**                               Legal range 0 to 255
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgCsfSetWanToLanMapTable(struct file *fileHndl,
                                                        UINT4 chnlCnt,
                                                        UINT4 *pChnlType,
                                                        UINT4 *pChnlId,
                                                        UINT4 *pGfpIdx,
                                                        UINT4 *pGfpChnl,
                                                        UINT4 *pAction,
                                                        UINT4 *pUpi)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen;
    UINT4 ibndMsgSubtype;
    UINT4 ibndSeqNum;
    UINT4 *pIbndMsg;
    UINT4 *pTmpIbndChnlType;
    UINT4 *pTmpIbndChnlId;
    UINT4 *pTmpIbndGfpIdx;
    UINT4 *pTmpIbndGfpChnl;
    UINT4 *pTmpIbndAction;
    UINT4 *pTmpIbndUpi;
    UINT4 *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    UINT4 i;
    INT4 result, rv;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* set temp pointers */
    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndGfpIdx   = pGfpIdx;
    pTmpIbndGfpChnl  = pGfpChnl;
    pTmpIbndAction   = pAction;
    pTmpIbndUpi      = pUpi;

    /* argument check */
    if (chnlCnt < 1 || chnlCnt > 30) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < chnlCnt; i++) {
        if ((*pTmpIbndChnlType > 1) ||
            (*pTmpIbndChnlType == 0 && *pTmpIbndChnlId > 1) ||
            (*pTmpIbndChnlType == 1 && *pTmpIbndChnlId > 15) ||
            (*pTmpIbndGfpIdx > 1) || (*pTmpIbndGfpChnl > 63) ||
            (*pTmpIbndAction > 7) || (*pTmpIbndUpi > 255))
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndGfpIdx++;
        pTmpIbndGfpChnl++;
        pTmpIbndAction++;
        pTmpIbndUpi++;
    }

    /* set message header parameters */
    ibndModID = MODID_CMF;
    ibndMsgCat = CATEGORY_CMD_CFG;
    ibndMsgLen = HYPHY20G_HOSTMSG_NUM_HEADER_WORDS + 1 + (2*chnlCnt);
    ibndMsgSubtype = 0x09;
    ibndMsgCtrl = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* start assembling message body (ie. without header) */
    pTmpIbndMsg      = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    pTmpIbndChnlType = pChnlType;
    pTmpIbndChnlId   = pChnlId;
    pTmpIbndGfpIdx   = pGfpIdx;
    pTmpIbndGfpChnl  = pGfpChnl;
    pTmpIbndAction   = pAction;
    pTmpIbndUpi      = pUpi;

    /* channel count */
    *pTmpIbndMsg = (chnlCnt & 0xFF) << 24;
    for (i = 0; i < chnlCnt; i++) {
        /* port type, port number, action, send dci */
        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndGfpIdx   & 0xFF) << 24) |
                       ((*pTmpIbndGfpChnl  & 0xFF) << 16) |
                       ((*pTmpIbndChnlType & 0xFF) << 8)  |
                       ((*pTmpIbndChnlId   & 0xFF) << 0);

        /* action, upi */
        pTmpIbndMsg++;
        *pTmpIbndMsg = ((*pTmpIbndAction & 0xFF) << 24) |
                       ((*pTmpIbndUpi    & 0xFF) << 16);

        pTmpIbndChnlType++;
        pTmpIbndChnlId++;
        pTmpIbndGfpIdx++;
        pTmpIbndGfpChnl++;
        pTmpIbndAction++;
        pTmpIbndUpi++;
    }

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        rv = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (rv) return rv;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgCsfSetWanToLanMapTable */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgCsfSetWanToLanUpdateTblNotif
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a
**                 CMD_CFG_CSF_Set_WanToLan_Update_Table_Notification message
**                 to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl            - base address
**                 gfp0ChnlUpdateMskMsb - update consequential or port mapping
**                                        table for GFP0 channels
**                                        Bits[31:0] corresponds to GFP0
**                                        channels[63:32]
**                                            0 - no update
**                                            1 - update channel
**                 gfp0ChnlUpdateMskLsb - see gfp0ChnlUpdateMskMsb description
**                                        Bits[31:0] corresponds to GFP0
**                                        channels[31:0]
**                 gfp1ChnlUpdateMskMsb - update consequential or port mapping
**                                        table for GFP1 channels
**                                        Bits[31:0] corresponds to GFP1
**                                        channels[63:32]
**                                            0 - no update
**                                            1 - update channel
**                 gfp1ChnlUpdateMskLsb - see gfp1ChnlUpdateMskMsb description
**                                        Bits[31:0] corresponds to GFP1
**                                        channels[31:0]
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgCsfSetWanToLanUpdateTblNotif(struct file *fileHndl,
                                                     UINT4 gfp0ChnlUpdateMskMsb,
                                                     UINT4 gfp0ChnlUpdateMskLsb,
                                                     UINT4 gfp1ChnlUpdateMskMsb,
                                                     UINT4 gfp1ChnlUpdateMskLsb)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((gfp0ChnlUpdateMskMsb > 0xFFFFFFFF) ||
        (gfp0ChnlUpdateMskLsb > 0xFFFFFFFF) ||
        (gfp1ChnlUpdateMskMsb > 0xFFFFFFFF) ||
        (gfp1ChnlUpdateMskLsb > 0xFFFFFFFF))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* set message header parameters */
    ibndModID      = MODID_CMF;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0x6;
    ibndMsgSubtype = 0x0A;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    /* action table index, los, rx_dlol_lsci, rx_dlol_hsci */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = gfp0ChnlUpdateMskLsb;

    pTmpIbndMsg++;
    *pTmpIbndMsg = gfp0ChnlUpdateMskMsb;

    pTmpIbndMsg++;
    *pTmpIbndMsg = gfp1ChnlUpdateMskLsb;

    pTmpIbndMsg++;
    *pTmpIbndMsg = gfp1ChnlUpdateMskMsb;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgCsfSetWanToLanUpdateTblNotif */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgCmdCfgGfpPauseSetEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function sends a CMD_CFG_GFPPAUSE_Set_Enable message
**                 to FW and waits for a RSP_CFG response from FW.
**
**  INPUTS:        *fileHndl             - base address
**                 gfp0ChnlEnableMskMsb  - GFP pause enable control for GFP0
**                                         channels
**                                         Bits[31:0] corresponds to GFP0
**                                         channels[63:32]
**                                             0 - no effect
**                                             1 - enable GFP pause
**                 gfp0ChnlEnableMskLsb  - see gfp0ChnlEnableMskMsb description
**                                         Bits[31:0] corresponds to GFP0
**                                         channels[31:0]
**                 gfp1ChnlEnableMskMsb  - GFP pause enable control for GFP1
**                                         channels
**                                         Bits[31:0] corresponds to GFP1
**                                         channels[63:32]
**                                             0 - no effect
**                                             1 - enable GFP pause
**                 gfp1ChnlEnableMskLsb  - see gfp1ChnlEnableMskMsb description
**                                         Bits[31:0] corresponds to GFP1
**                                         channels[31:0]
**                 gfp0ChnlDisableMskMsb - GFP pause disable control for GFP0
**                                         channels
**                                         Bits[31:0] corresponds to GFP0
**                                         channels[63:32]
**                                             0 - no effect
**                                             1 - disable GFP pause
**                 gfp0ChnlDisableMskLsb - see gfp0ChnlDisableMskMsb description
**                                         Bits[31:0] corresponds to GFP0
**                                         channels[31:0]
**                 gfp1ChnlDisableMskMsb - GFP pause disable control for GFP1
**                                         channels
**                                         Bits[31:0] corresponds to GFP1
**                                         channels[63:32]
**                                             0 - no effect
**                                             1 - disable GFP pause
**                 gfp1ChnlDisableMskLsb - see gfp1ChnlDisableMskMsb description
**                                         Bits[31:0] corresponds to GFP1
**                                         channels[31:0]
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgCmdCfgGfpPauseSetEnable(struct file *fileHndl,
                                                   UINT4 gfp0ChnlEnableMskMsb,
                                                   UINT4 gfp0ChnlEnableMskLsb,
                                                   UINT4 gfp1ChnlEnableMskMsb,
                                                   UINT4 gfp1ChnlEnableMskLsb,
                                                   UINT4 gfp0ChnlDisableMskMsb,
                                                   UINT4 gfp0ChnlDisableMskLsb,
                                                   UINT4 gfp1ChnlDisableMskMsb,
                                                   UINT4 gfp1ChnlDisableMskLsb)
{
    eHYPHY20G_HOSTMSG_MODID ibndModID;
    eHYPHY20G_HOSTMSG_CATEGORY ibndMsgCat;
    eHYPHY20G_HOSTMSG_MSG_CTRL_TYPE ibndMsgCtrl;
    UINT4 ibndMsgLen, ibndMsgSubtype, ibndSeqNum;
    UINT4 *pIbndMsg, *pTmpIbndMsg;
    UINT4 doorBellNum = 0; /* always use first doorbell */
    UINT4 size;
    INT4 result, tmpResult;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((gfp0ChnlEnableMskMsb > 0xFFFFFFFF) ||
        (gfp0ChnlEnableMskLsb > 0xFFFFFFFF) ||
        (gfp1ChnlEnableMskMsb > 0xFFFFFFFF) ||
        (gfp1ChnlEnableMskLsb > 0xFFFFFFFF) ||
        (gfp0ChnlDisableMskMsb > 0xFFFFFFFF) ||
        (gfp0ChnlDisableMskLsb > 0xFFFFFFFF) ||
        (gfp1ChnlDisableMskMsb > 0xFFFFFFFF) ||
        (gfp1ChnlDisableMskLsb > 0xFFFFFFFF))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* set message header parameters */
    ibndModID      = MODID_PAUSEFRM;
    ibndMsgCat     = CATEGORY_CMD_CFG;
    ibndMsgLen     = 0xA;
    ibndMsgSubtype = 0x02;
    ibndMsgCtrl    = BASIC_MSG;
    result = hostMsgHyPhy20gHostMsgNextAvailSeqNum(fileHndl, &ibndSeqNum);
    if (result) return result;

    /* allocate memory for message */
    size = sizeof(UINT4)*ibndMsgLen;

    pIbndMsg = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if (pIbndMsg == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* assemble message header */
    result = hostMsgHyPhy20gHostMsgIbndMsgHdr(fileHndl, ibndModID, ibndMsgCat,
                                              ibndMsgLen, ibndMsgSubtype,
                                              ibndMsgCtrl, ibndSeqNum,
                                              pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* assemble message body (ie. without header) */
    pTmpIbndMsg = pIbndMsg + HYPHY20G_HOSTMSG_NUM_HEADER_WORDS;
    *pTmpIbndMsg = gfp0ChnlEnableMskLsb;

    pTmpIbndMsg++;
    *pTmpIbndMsg = gfp0ChnlEnableMskMsb;

    pTmpIbndMsg++;
    *pTmpIbndMsg = gfp1ChnlEnableMskLsb;

    pTmpIbndMsg++;
    *pTmpIbndMsg = gfp1ChnlEnableMskMsb;

    pTmpIbndMsg++;
    *pTmpIbndMsg = gfp0ChnlDisableMskLsb;

    pTmpIbndMsg++;
    *pTmpIbndMsg = gfp0ChnlDisableMskMsb;

    pTmpIbndMsg++;
    *pTmpIbndMsg = gfp1ChnlDisableMskLsb;

    pTmpIbndMsg++;
    *pTmpIbndMsg = gfp1ChnlDisableMskMsb;

    /* send message */
    result = hostMsgHyPhy20gHostMsgSendIbndMsg(fileHndl, ibndMsgLen, pIbndMsg);
    if (result) {
        sysHyPhy20gMemFree(pIbndMsg);
        return result;
    }

    /* free memory */
    sysHyPhy20gMemFree(pIbndMsg);

    /* retrieve message and check its parameters */
    result = hostMsgHyPhy20gHostMsgChkObndRspCfg(fileHndl, ibndModID,
                                                 ibndMsgSubtype, ibndSeqNum);
    if (result) {
        /* clear outbound doorbell */
        tmpResult = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
        if (tmpResult) return tmpResult;
        return result;
    }

    /* clear outbound doorbell */
    result = hostMsgHyPhy20gHostMsgClrObndDbel(fileHndl, doorBellNum);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hostMsgHyPhy20gHostMsgCmdCfgGfpPauseSetEnable */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the size of HostMsg context.
**
**
**  INPUTS:         fileHndl      - device handle
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The size of HostMsg context
**
*******************************************************************************/
UINT4 hostMsgHyPhy20gHostMsgGetCtxtSize(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_HOSTMSG_CTXT *pHostMsgCtxt;
    UINT4 size = 0;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    pHostMsgCtxt = pDevCtxt->pHostMsgCtxt;

    /* include the ctxt header size */
    size += utilHyPhy20gGetCtxtHdrSize();
	
    /* calculate the context size of hostMsg subsystem */
    if (pHostMsgCtxt != NULL) {
    	size += sizeof(sHYPHY20G_HOSTMSG_CTXT);

	    /* calculate the second level size,
	    pHostMsgCtxt->pHostMsgSeqCount
	    */
	    /* include the ctxt header size */
	    size += utilHyPhy20gGetCtxtHdrSize();
	    size += sizeof(*(pHostMsgCtxt->pHostMsgSeqCount));
	}

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, size);

    return size;
}/* hostMsgHyPhy20gHostMsgGetCtxtSize */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the HostMsg context into pre-allocated buffer.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-allocated buffer for HostMsg context 
**                  extraction
**
**  OUTPUTS:        pExtractedSize -the context size(in byte) has been extracted
**                  into the buffer
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgExtractCtxt(struct file *fileHndl, 
                                     void *pCtxtBuf, UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_HOSTMSG_CTXT *pHostMsgCtxt;   
    UINT4 rtLen;
    UINT4 offset = 0, len = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pExtractedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pExtractedSize is NULL.");

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pHostMsgCtxt = pDevCtxt->pHostMsgCtxt;

    DBG_PRINT("%s, %s, %d, 1st offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* insert ctxt buffer header */
	len = (pHostMsgCtxt == NULL) ? sizeof(sHYPHY20G_CTXT_HDR) : 
			(sizeof(sHYPHY20G_CTXT_HDR) + sizeof(sHYPHY20G_HOSTMSG_CTXT));
    rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                        HYPHY_HOSTMSG_CTXT_ID, HYPHY_NO_INDEX);
    HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");  
    offset += sizeof(sHYPHY20G_CTXT_HDR);

    if (pHostMsgCtxt != NULL) {
	    /* extract the first level context of HostMsg subsystem */
	    /* copy the payload into the buffer */
	    sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
	                    (void *)pHostMsgCtxt, sizeof(sHYPHY20G_HOSTMSG_CTXT));
	    offset += sizeof(sHYPHY20G_HOSTMSG_CTXT);

	    /* extract the second level context,
	    pHostMsgCtxt->pHostMsgSeqCount
	    */
	    /* insert ctxt buffer header */
	    len = sizeof(sHYPHY20G_CTXT_HDR) +
	            sizeof(*(pHostMsgCtxt->pHostMsgSeqCount));
	    rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
	                                        HYPHY_HOSTMSG_SEQ_COUNT_CTXT_ID, HYPHY_NO_INDEX);
	    HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
	        "%s", "pCtxtBuf is NULL.");  
	    offset += sizeof(sHYPHY20G_CTXT_HDR);

	    /* copy the payload into the buffer */
	    len = sizeof(*(pHostMsgCtxt->pHostMsgSeqCount));
	    sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
	                    (void *)(pHostMsgCtxt->pHostMsgSeqCount), len);
	    offset += len;
	}

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pExtractedSize = offset;

    return HYPHY20G_SUCCESS;
}/* hostMsgHyPhy20gHostMsgExtractCtxt */

/*******************************************************************************
**
**  hostMsgHyPhy20gHostMsgRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the HostMsg context into the driver context memory.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-stored HostMsg context for restoration
**
**  OUTPUTS:        pParsedSize -the data size(in byte) has been parsed
**                  in the buffer for restoration
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_CTXT_CORRUPTED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgRestoreCtxt(struct file *fileHndl, 
                                     void *pCtxtBuf, UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_HOSTMSG_CTXT *pHostMsgCtxt;   
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    UINT4 *pTmpSeqCount;
    INT4 result;
    UINT4 offset = 0, len = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pParsedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pParsedSize is NULL.");

    /* try to retrieve the HostMsg header in the pre-stored context buffer */
    pCtxtBufHdr = utilHyPhy20gSearchCtxtHdr(pCtxtBuf, 
                                HYPHY_HOSTMSG_CTXT_ID, HYPHY_NO_INDEX);
    if (pCtxtBufHdr == NULL) {
        DBG_PRINT("%s, %s, %d, can not find HYPHY_HOSTMSG_CTXT_ID hdr\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    pCtxtBuf = (void *)pCtxtBufHdr;

    /* check if the payload is valid */
    if (pCtxtBufHdr->len == sizeof(sHYPHY20G_CTXT_HDR)) {
        offset = sizeof(sHYPHY20G_CTXT_HDR);
    } else if (pCtxtBufHdr->len > sizeof(sHYPHY20G_CTXT_HDR)) {
        /* create HostMsg context */
        result = hyPhy20gHostMsgCreateCtxt(fileHndl);
        if (result) {
            DBG_PRINT("%s, %s, %d, hyPhy20gHostMsgCreateCtxt failed\n", 
                        __FILE__, __FUNCTION__, __LINE__);
            *pParsedSize = 0;
            return result;
        }
        
        /* retrieve device context */
        pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
        /* no failed: confirm device context pointer is valid before processing */
        HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                        "%s", "pDevCtxt is NULL.");

        pHostMsgCtxt = pDevCtxt->pHostMsgCtxt;

        DBG_PRINT("%s, %s, %d, 1st offset 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, offset);

        /* restore the first level context for HostMsg subsystem */
        /* backup the useful pointer values */
        pTmpSeqCount = pHostMsgCtxt->pHostMsgSeqCount;

        /* offset pCtxtBuf to figure out the payload position */
        offset += sizeof(sHYPHY20G_CTXT_HDR);

        /* payload len */
        len = pCtxtBufHdr->len - 
                sizeof(sHYPHY20G_CTXT_HDR);
        
        sysHyPhy20gMemCpy((void *)(pHostMsgCtxt), 
                        ((UINT1 *)pCtxtBuf + offset), len);
        offset += len;

        /* finish restore the first level context for hostMsg subsystem */
        pHostMsgCtxt->pHostMsgSeqCount = pTmpSeqCount;

        DBG_PRINT("%s, %s, %d, 2nd offset 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, offset);

        /* restore the second level context */
        /* try to retrieve the pHostMsgSeqCount header */
        pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
        if (pCtxtBufHdr->id != HYPHY_HOSTMSG_SEQ_COUNT_CTXT_ID) {
            DBG_PRINT("%s, %s, %d, HYPHY_HOSTMSG_SEQ_COUNT_CTXT_ID wrong\n", 
                        __FILE__, __FUNCTION__, __LINE__);
            *pParsedSize = 0;
            hyPhy20gHostMsgDestroyCtxt(fileHndl);
            return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
        }
        HYPHY20G_ASSERT((pCtxtBufHdr->len > sizeof(sHYPHY20G_CTXT_HDR)), 
                        HYPHY20G_ERR_DEV_CTXT_CORRUPTED,
                        "%s", "HYPHY20G_ERR_DEV_CTXT_CORRUPTED.");
        
        /* offset pCtxtBuf to figure out the payload position */
        offset += sizeof(sHYPHY20G_CTXT_HDR);
        
        /* payload len */
        len = pCtxtBufHdr->len - 
                sizeof(sHYPHY20G_CTXT_HDR);
        
        sysHyPhy20gMemCpy((void *)(pHostMsgCtxt->pHostMsgSeqCount), 
                        ((UINT1 *)pCtxtBuf + offset), len);
        offset += len;
    } else {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_CORRUPTED\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        *pParsedSize = 0;
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pParsedSize = offset;

    return HYPHY20G_SUCCESS;
}/* hostMsgHyPhy20gHostMsgRestoreCtxt */

/********************************************************************************
**
**  hostMsgHyPhy20gHostMsgAuditCtxt  
**  ___________________________________________________________________________
**
**  DESCRIPTION:    the function used to audit the running HostMsg subCTXT.
**
**
**  INPUTS:         fileHndl            - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_FAILURE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hostMsgHyPhy20gHostMsgAuditCtxt (struct file *fileHndl)
{
    INT4 result;
    UINT4 val;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 revNum,buildNum;
    UINT4 statType,statCode;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    

    /* the below check rule mainly come from hyPhy20gHostMsgInit */
    if (pDevCtxt->pHostMsgCtxt!=NULL)
    {
#ifndef HWACCESSMODE_USB
        /* check that FW is ready */
        result = sysHyPhy20gRead(fileHndl, HYPHY20G_TOP_REG_APP_USR_1, &val);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gRead fail."); 

        if ((val & 0x1) != 0x1)
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
#endif
        /* check the pHostMsgCtxt->firmwareVer == running firmware version? */
        result=hyPhy20gHostMsgCmdReqInfoGetVerInfo(fileHndl, HYPHY20G_HOSTMSG_GET_REV, &revNum, &buildNum);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "hyPhy20gHostMsgCmdReqInfoGetVerInfo fail.");    

        if (pDevCtxt->pHostMsgCtxt->firmwareVer != revNum)
        {
            DBG_PRINT("%s, %s, %d, pHostMsgCtxt firmwareVer mismatched with running firmware version.\n",
                        __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
        
        /* check the firmware is alive */
        result=hyPhy20gHostMsgCmdReqInfoGetStatus(fileHndl, HYPHY20G_HOSTMSG_REQ_STATUS, &statType, &statCode);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "hyPhy20gHostMsgCmdReqInfoGetStatus fail.");    


        if (statType == HYPHY20G_HOSTMSG_REQ_STATUS)
        {
            if (statCode != HYPHY20G_HOSTMSG_FIRMWARE_ALIVE )
            {
                DBG_PRINT("%s, %s, %d, Firmware died.\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
        }
        else
        {
            DBG_PRINT("%s, %s, %d, hyPhy20gHostMsgCmdReqInfoGetStatus statType mismatch.\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
        
    }

    return HYPHY20G_SUCCESS;
    
}/* hostMsgHyPhy20gHostMsgAuditCtxt */
/* end of file */
