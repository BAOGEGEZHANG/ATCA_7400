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
**  FILE        :  hyphy20g_hsci_counters.c
**
**  $Date: 2011-04-28 14:20:12 +0800 (Thu, 28 Apr 2011) $
**
**  $Revision: 11852 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_hsci_counters.h"

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/


/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/
/*******************************************************************************
**
**  hyPhy20gHsci10GeGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts one of the HSCI 10GE block
**                 counters.
**
**                 This function is used for 10GE MAC Terminated mode.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - 10GE instance in:
**                                0 - HSCI 0
**                                1 - HSCI 1
**                 counter      - Counter to be accessed:
**                                00 - RX_FILTER_ERROR
**                                01 - TX_FILTER_ERROR
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeGetCounts(struct file *fileHndl, UINT4 slice,
                               UINT4 counter, UINT4 *pCount)
{
    UINT4 cntReg;
    UINT4 cntMsk;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 1 || counter > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    switch(counter)
    {
        case 0: /* RX_FILTER_ERROR */
            cntReg = HYPHY20G_HSCI_ENET_10G_RXXG_REG_LN_FLT_ERR_CNT;
            cntMsk = HYPHY20G_HSCI_ENET_10G_RXXG_BIT_LN_FLT_ERR_CNT_FILTER_ERROR_MSK;
            break;
        case 1: /* TX_FILTER_ERROR */
            cntReg = HYPHY20G_HSCI_ENET_10G_TXXG_REG_FLT_ERR_CNT;
            cntMsk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_FLT_ERR_CNT_FILTER_ERROR_MSK;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }
    cntReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cntReg, slice);

    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, cntReg, cntMsk, &val);
    if (result) return result;
    *pCount = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeGetCounts */


/*******************************************************************************
**
**  hyPhy20gHsciGsup43GetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts all of the HSCI GSUP43 block
**                 receive and transmit counters.
**
**                 It extracts the counters using the FW Command
**                 CMD_REQ_PMON_Get_HSCI_GSUP43_PMON_CNTR.
**
**                 40 bit counters are stored in two element arrays.  Counter 
**                 bit mapping is as follows:
**                 *pCountn[0] - bits 39:32
**                 *pCountn[1] - bits 31:0
**
**                 This function is used for 10GE MAC Terminated mode,
**                 G.Sup43 Clause 7.3 Terminating application bypass MAC mode, 
**                 G.Sup43 Clause 7.3 Terminating application through MAC mode,
**                 and G.Sup43 Clause 6.2 Terminating application through 
**                 MAC mode.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - GSUP43 instance in:
**                                0 - HSCI 0
**                                1 - HSCI 1
**                *pReserve     - Reserved parameter. Don't use.
**                *pCount1      - Pointer to counter storage for:
**                                BLK_TYPE_ERR_CNT
**                *pCount2      - Pointer to counter storage for:
**                                HI_BER_CNT
**                *pCount3      - Pointer to counter storage for:
**                                PRBS31_ERR_CNT 
**                *pCount4      - Pointer to counter storage for:
**                                RX_BLK_ERR_CNT 
**                *pCount5      - Pointer to counter storage for:
**                                RX_OS_DSCRCD_CNT 
**                *pCount6      - Pointer to counter storage for:
**                                RX_DATA_BYTE_CNT 
**                *pCount7      - Pointer to counter storage for:
**                                RX_OS_CNT 
**                *pCount8      - Pointer to counter storage for:
**                                RX_PKT_CNT 
**                *pCount9      - Pointer to counter storage for:
**                                TX_PKT_CNT 
**                *pCount10     - Pointer to counter storage for:
**                                TX_OS_CNT 
**                *pCount11     - Pointer to counter storage for:
**                                TX_DATA_BYTE_CNT 
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_RSP_REQ
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciGsup43GetCounts(struct file *fileHndl, UINT4 slice,
                                 UINT4 *pReserve, UINT4 *pCount1, 
                                 UINT4 *pCount2, UINT4 *pCount3, 
                                 UINT4 *pCount4, UINT4 *pCount5, 
                                 UINT4 *pCount6, UINT4 *pCount7, 
                                 UINT4 *pCount8, UINT4 *pCount9, 
                                 UINT4 *pCount10, UINT4 *pCount11)
{    
    INT4 result;
    UINT4 reserve;
    
    /* argument checking */
    if (slice > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    } 

    DBG_PRINT("%s, %s, pReserve %p - reserved parameter.\n",__FILE__, __FUNCTION__, pReserve);

    /* access the counter through FW command */
    result = hyPhy20gHostMsgCmdReqGetHsciGsup43PmonCntr(fileHndl, slice, 
                                                        &reserve, pCount1,
                                                        pCount2, pCount3,
                                                        pCount4, pCount5,
                                                        pCount6, pCount7,
                                                        pCount8, pCount9,
                                                        pCount10, pCount11,
                                                        &reserve);
    if (result) return result; 
    
    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciGsup43GetCounts */

/*******************************************************************************
**
**  hyPhy20gHsciEnetComGsup43GetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts one of the ENET_COM GSUP43 block
**                 receive and transmit counters.
**
**                 This function is used for G.Sup43 Clause 7.3 Terminating 
**                 application through MAC mode, and G.Sup43 Clause 6.2 
**                 Terminating application through MAC mode.
**
**                 Note, because ENET_COM GSUP43 Tx block is bypassed in the
**                 G.Sup43 Clause 6.2 Terminating application through MAC mode,
**                 Tx counters ares useless in this mode.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - GSUP43 instance in ENET_COM subsystem:
**                                0 - GSUP43 instance 0
**                                1 - GSUP43 instance 1
**                 accessMode   - 1 - locally trigger counter accumulation
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                00 - BLK_TYPE_ERR_CNT
**                                01 - HI_BER_CNT
**                                02 - RX_BLK_ERR_CNT
**                                03 - PRBS31_ERR_CNT
**                                04 - RX_OS_DSCRCD_CNT
**                                05 - RX_DATA_BYTE_CNT
**                                06 - RX_OS_CNT
**                                07 - RX_PKT_CNT
**                                08 - TX_PKT_CNT
**                                09 - TX_DATA_BYTE_CNT
**                                10 - TX_OS_CNT
**                *pCount0      - When counter = 00 to 03:
**                                Pointer to storage for counter data;
**                                When counter = 04 to 10:
**                                Pointer to storage for counter's bit 31:0.
**                *pCount1      - When counter = 00 to 03:
**                                not used;
**                                When counter = 04 to 10:
**                                Pointer to storage for counter's bit 49:32.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciEnetComGsup43GetCounts(struct file *fileHndl, UINT4 slice,
                                        UINT4 accessMode, UINT4 counter, 
                                        UINT4 *pCount0, UINT4 *pCount1)
{
    UINT4 updateReg = 0;
    UINT4 updateMsk = 0;
    UINT4 cntLsbReg = 0;
    UINT4 cntLsbMsk = 0;
    UINT4 cntMsbReg = 0;
    UINT4 cntMsbMsk = 0;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 1 || accessMode > 1 || counter > 10) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    if (pCount0 == NULL || (counter > 3 && pCount1 == NULL)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* determine registers and masks associated with counter */
    updateReg = HYPHY20G_ENET_COM_GSUP43_REG_CFG;
    updateReg = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(updateReg, slice);
    updateMsk = HYPHY20G_ENET_COM_GSUP43_BIT_CFG_PMON_UPDATE_MSK;
    switch(counter)
    {
        case 0: /* BLK_TYPE_ERR_CNT */
            cntLsbReg = HYPHY20G_ENET_COM_GSUP43_REG_RX_FRM_ERR_CNT;
            cntLsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_RX_FRM_ERR_CNT_BLK_TYPE_ERR_CNT_MSK;
            break;
        case 1: /* HI_BER_CNT */
            cntLsbReg = HYPHY20G_ENET_COM_GSUP43_REG_RX_FRM_ERR_CNT;
            cntLsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_RX_FRM_ERR_CNT_HI_BER_CNT_MSK;
            break;
        case 2: /* RX_BLK_ERR_CNT */
            cntLsbReg = HYPHY20G_ENET_COM_GSUP43_REG_RX_BLK_ERR_CNTS;
            cntLsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_RX_BLK_ERR_CNTS_RX_BLK_ERR_CNT_MSK;
            break;
        case 3: /* PRBS31_ERR_CNT */
            cntLsbReg = HYPHY20G_ENET_COM_GSUP43_REG_RX_PRBS31_AND_DISCARDED_ORD_SET_MSB_CNT;
            cntLsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_RX_PRBS31_AND_DISCARDED_ORD_SET_MSB_CNT_PRBS31_ERR_CNT_MSK;
            break;
        case 4: /* RX_OS_DSCRCD_CNT */
            cntLsbReg = HYPHY20G_ENET_COM_GSUP43_REG_RX_DISCARDED_ORD_SET_CNT_LSB;
            cntLsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_RX_DISCARDED_ORD_SET_CNT_LSB_RX_OS_DSCRCD_CNT_MSK;
            cntMsbReg = HYPHY20G_ENET_COM_GSUP43_REG_RX_PRBS31_AND_DISCARDED_ORD_SET_MSB_CNT;
            cntMsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_RX_PRBS31_AND_DISCARDED_ORD_SET_MSB_CNT_RX_OS_DSCRCD_CNT_MSK;
            break;
        case 5: /* RX_DATA_BYTE_CNT */
            cntLsbReg = HYPHY20G_ENET_COM_GSUP43_REG_RX_DAT_BYT_CNT_LSB;
            cntLsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_RX_DAT_BYT_CNT_LSB_RX_DATA_BYTE_CNT_MSK;
            cntMsbReg = HYPHY20G_ENET_COM_GSUP43_REG_RX_ORD_SET_AND_DAT_BYT_CNT_MSB;
            cntMsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_RX_ORD_SET_AND_DAT_BYT_CNT_MSB_RX_DATA_BYTE_CNT_MSK;
            break;
        case 6: /* RX_OS_CNT */
            cntLsbReg = HYPHY20G_ENET_COM_GSUP43_REG_RX_ORD_SET_CNT_LSB;
            cntLsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_RX_ORD_SET_CNT_LSB_RX_OS_CNT_MSK;
            cntMsbReg = HYPHY20G_ENET_COM_GSUP43_REG_RX_ORD_SET_AND_DAT_BYT_CNT_MSB;
            cntMsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_RX_ORD_SET_AND_DAT_BYT_CNT_MSB_RX_OS_CNT_MSK;
            break;
        case 7: /* RX_PKT_CNT */
            cntLsbReg = HYPHY20G_ENET_COM_GSUP43_REG_RX_PAC_CNT_LSB;
            cntLsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_RX_PAC_CNT_LSB_RX_PKT_CNT_MSK;
            cntMsbReg = HYPHY20G_ENET_COM_GSUP43_REG_RX_PAC_CNT_MSB;
            cntMsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_RX_PAC_CNT_MSB_RX_PKT_CNT_MSK;
            break;
        case 8: /* TX_PKT_CNT */
            cntLsbReg = HYPHY20G_ENET_COM_GSUP43_REG_TX_PAC_CNT_LSB;
            cntLsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_TX_PAC_CNT_LSB_TX_PKT_CNT_MSK;
            cntMsbReg = HYPHY20G_ENET_COM_GSUP43_REG_TX_PAC_CNT_MSB;
            cntMsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_TX_PAC_CNT_MSB_TX_PKT_CNT_MSK;
            break;
        case 9: /* TX_DATA_BYTE_CNT */
            cntLsbReg = HYPHY20G_ENET_COM_GSUP43_REG_TX_DAT_BYT_CNT_LSB;
            cntLsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_TX_DAT_BYT_CNT_LSB_TX_DATA_BYTE_CNT_MSK;
            cntMsbReg = HYPHY20G_ENET_COM_GSUP43_REG_TX_ORD_SET_AND_DAT_BYT_CNT_MSB;
            cntMsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_TX_ORD_SET_AND_DAT_BYT_CNT_MSB_TX_DATA_BYTE_CNT_MSK;
            break;
        case 10: /* TX_OS_CNT */
            cntLsbReg = HYPHY20G_ENET_COM_GSUP43_REG_TX_ORD_SET_CNT_LSB;
            cntLsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_TX_ORD_SET_CNT_LSB_TX_OS_CNT_MSK;
            cntMsbReg = HYPHY20G_ENET_COM_GSUP43_REG_TX_ORD_SET_AND_DAT_BYT_CNT_MSB;
            cntMsbMsk = HYPHY20G_ENET_COM_GSUP43_BIT_TX_ORD_SET_AND_DAT_BYT_CNT_MSB_TX_OS_CNT_MSK;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }
    cntLsbReg   = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(cntLsbReg, slice);
    cntMsbReg   = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(cntMsbReg, slice);

    /* update of the counter values if required */
    if (accessMode) {
        result = hyPhy20gRegFieldWrite(fileHndl, updateReg, updateMsk, 1);
        if (result) return result;
    }

    /* Poll PMON_UPDATE bit to ensure cleared for updating sucessfully */
    result = sysHyPhy20gPollBit(fileHndl, updateReg, updateMsk, 0, 10000);
    if (result) return result;

    /* Retrieve Counter Value */
    if (counter < 4) {
        result = hyPhy20gRegFieldRead(fileHndl, cntLsbReg, cntLsbMsk, &val);
        if (result) return result;
        *pCount0 = val;
    } else {
        UINT4 msbVal;

        result = hyPhy20gRegFieldRead(fileHndl, cntMsbReg, cntMsbMsk, &msbVal);
        if (result) return result;
        *pCount1 = msbVal;

        result = hyPhy20gRegFieldRead(fileHndl, cntLsbReg, cntLsbMsk, &val);
        if (result) return result;
        *pCount0 = val;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciEnetComGsup43GetCounts */

/*******************************************************************************
**
**  hyPhy20gHsciEthStatTrigCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function is used to trigger counter access to HSCI
**                 MSTATX.  It performs the following:
**
**                 - Aquires access to MSTATX counters for an
**                 HSCI slice from FW using 
**                 CMD_CFG_PMON_Request_Enet_MStatx_Control command.
**
**                 - Triggers counter accumulation for TX and RX
**                 MSTATX statistics counters.  The internal device counts are
**                 transferred to the holding registers and the internal 
**                 device counters are reset to 0.
**
**                 Following a call to this command 
**                 hyPhy20gHsciEthStatGetCounts may be called to retrieve one
**                 or more of the TX or RX counters.
**
**                 When all desired RX counters are extracted, the
**                 counter trigger access to MSTATX must be release for FW
**                 to continue use for Link OAM functionality.  The function
**                 hyPhy20gHsciEthStatTrigRelease is provided to allow FW to
**                 continue access to this register.
**
**                 Transmit counters may be extracted with 
**                 hyPhy20gHsciEthStatGetCounts as FW feature enabled with
**                 hyPhy20gHsciEthStatTrigRelease only performs access to
**                 Receive counters.
**
**                 This function is used for MAC Terminated (FE/GE, SGMII FE/GE)
**                 modes.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - MSTATX instance in:
**                                0  - HSCI 0
**                                1  - HSCI 1
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciEthStatTrigCounts(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 dummy;

    /* argument checking */
    if (slice > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    } 

    /* Request Control of Counters with FW command */
    /* call hyPhy20gHostMsgCmdCfgPmonReqMstatxCtrl with:
       chnlType = 0 for HSCI slices */
    result = hyPhy20gHostMsgCmdCfgPmonReqMstatxCtrl(fileHndl, 0, slice);
    if (result) return result;   
    
    /* Trigger counter accumulation with counter reset for Tx and Rx counters */
    /* for RX counter accumulation 
        call utilHyPhy20gEthStatGetCounts with:
        ctrlMsk = 0x2
        counter = 0
    */
    result = utilHyPhy20gEthStatGetCounts(fileHndl, slice, 0x2, 0, &dummy, 
                                          &dummy, &dummy);
    if (result) return result;

    /* for TX counter accumulation 
        call utilHyPhy20gEthStatGetCounts with:
        ctrlMsk = 0x2
        counter = 33
    */
    result = utilHyPhy20gEthStatGetCounts(fileHndl, slice, 0x2, 33, &dummy, 
                                          &dummy, &dummy);
    if (result) return result; 
    return HYPHY20G_SUCCESS;
} /* hyPhy20gHsciEthStatTrigCounts */

/*******************************************************************************
**
**  hyPhy20gHsciEthStatTrigRelease
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function is used to release counter access from SW and
**                 return counter control to FW for an HSCI MSTATX slice.  It 
**                 makes access to FW command 
**                 CMD_CFG_PMON_Request_Enet_MStatx_Control.
**
**                 Refer to hyPhy20gHsciEthStatTrigCounts for details of 
**                 operation.
**
**                 This function is used for MAC Terminated (FE/GE, SGMII FE/GE)
**                 modes.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - MSTATX instance in:
**                                0  - HSCI 0
**                                1  - HSCI 1
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
INT4 hyPhy20gHsciEthStatTrigRelease(struct file *fileHndl, UINT4 slice)
{
    INT4 result;

    /* argument checking */
    if (slice > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    } 

    /* call hyPhy20gHostMsgCmdCfgPmonReleaseMstatxCtrl with:
       chnlType = 0 for HSCI slices */
    result = hyPhy20gHostMsgCmdCfgPmonReleaseMstatxCtrl(fileHndl, 0x0, slice);
    if (result) return result;    

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHsciEthStatTrigRelease */

/*******************************************************************************
**
**  hyPhy20gHsciEthStatGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts one of the HSCI
**                 MSTATX Ethernet Statistics 40-bits counters.
**
**                 Refer to hyPhy20gHsciEthStatTrigCounts for details of 
**                 operation.
**
**                 The #11 RX_ALIGNMENT_ERROR counter is
**                 not applicable for HSCI ENET_10G slices.
**
**                 This function is used for 10GE MAC Terminated mode.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - MSTATX instance in:
**                                0 - HSCI 0
**                                1 - HSCI 1
**                 counter      - Counter to be accessed:
**                                0  RX_FRAMES_OK
**                                1  RX_OCTETS_OK
**                                2  RX_FRAMES
**                                3  RX_OCTETS
**                                4  RX_UNICAST_FRAMES_OK
**                                5  RX_BROADCAST_FRAMES_OK
**                                6  RX_MULTICAST_FRAMES_OK
**                                7  RX_TAGGED_FRAMES_OK
**                                8  RX_MAC_PAUSE_CONTROL_FRAMES_OK
**                                9  RX_MAC_CONTROL_FRAMES_OK
**                                10 RX_FCS_ERROR
**                                11 RESERVED
**                                12 RX_SYMBOL_ERROR
**                                13 RX_LAYER1_ERROR
**                                14 RX_INRANGE_LENGTH_ERROR
**                                15 RX_LONG_LENGTH_ERROR
**                                16 RX_LONG_LENGTH_CRC_ERROR
**                                17 RX_SHORT_LENGTH_ERROR
**                                18 RX_SHORT_LENGTH_CRC_ERROR
**                                19 RX_FRAMES_64_OCTETS
**                                20 RX_FRAMES_65_TO_127_OCTETS
**                                21 RX_FRAMES_128_TO_255_OCTETS
**                                22 RX_FRAMES_256_TO_511_OCTETS
**                                23 RX_FRAMES_512_TO_1023_OCTETS
**                                24 RX_FRAMES_1024_TO_1518_OCTETS
**                                25 RX_FRAMES_1519_TO_MAXSIZE
**                                26 RX_STATION_ADDRESS_FILTERED
**                                27 RX_VARIABLE
**                                28 RX_FRAMES_1519_TO_1522_OCTETS
**                                29 RX_FRAMES_1523_TO_1548_OCTETS
**                                30 RX_FRAMES_1549_TO_2000_OCTETS
**                                31 RX_FRAMES_2001_TO_2500_OCTETS
**                                32 RX_FRAMES_2501_TO_MAXSIZE
**                                33 TX_FRAMES_OK
**                                34 TX_OCTETS_OK
**                                35 TX_OCTETS
**                                36 TX_TAGGED_FRAMES_OK
**                                37 TX_MAC_PAUSE_CONTROL_FRAMES_OK
**                                38 TX_FCS_ERROR
**                                39 TX_SHORT_LENGTH_ERROR
**                                40 TX_LONG_LENGTH_ERROR
**                                41 TX_SYSTEM_ERROR
**                                42 TX_MAC_ERROR
**                                43 TX_FRAMES_1523_TO_MAXSIZE
**                                   (TX_TAGGED_JUMBO_FRAMES_OK)
**                                44 TX_MAC_CONTROLFRAMES_OK
**                                45 TX_UNICAST_FRAMES_OK
**                                46 TX_BROADCAST_FRAMES_OK
**                                47 TX_MULTICAST_FRAMES_OK
**                                48 TX_UNICAST_FRAMES_ATTEMPTED
**                                49 TX_BROADCAST_FRAMES_ATTEMPTED
**                                50 TX_MULTICAST_FRAMES_ATTEMPTED
**                                51 TX_FRAMES_64_OCTETS
**                                52 TX_FRAMES_65_TO_127_OCTETS
**                                53 TX_FRAMES_128_TO_255_OCTETS
**                                54 TX_FRAMES_256_TO_511_OCTETS
**                                55 TX_FRAMES_512_TO_1023_OCTETS
**                                56 TX_FRAMES_1024_TO_1518_OCTETS
**                                57 TX_FRAMES_1519_TO_MAXSIZE
**                                58 TX_VARIABLE
**                                59 RX_FRAMES_1523_TO_MAXSIZE
**                                   (RX_TAGGED_JUMBO_FRAMES_OK)
**                                60 TX_FRAMES_1519_TO_1522_OCTETS
**                                61 TX_FRAMES_1523_TO_1548_OCTETS
**                                62 TX_FRAMES_1549_TO_2000_OCTETS
**                                63 TX_FRAMES_2001_TO_2500_OCTETS
**                                64 TX_FRAMES_2501_TO_MAXSIZE
**                *pCount0      - Pointer to storage for counter's bit 15:0
**                *pCount1      - Pointer to storage for counter's bit 31:16
**                *pCount2      - Pointer to storage for counter's bit 39:32
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciEthStatGetCounts(struct file *fileHndl, UINT4 slice, 
                                  UINT4 counter, UINT4 *pCount0,
                                  UINT4 *pCount1, UINT4 *pCount2)
{
    INT4 result;

    /* argument checking */
    if (slice > 1  || counter > 64) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Extract Counter */
    result = utilHyPhy20gEthStatGetCounts(fileHndl, slice, 0x0, counter,
                                          pCount0, pCount1, pCount2);
    if (result) return result;
        
    return HYPHY20G_SUCCESS;
} /* hyPhy20gHsciEthStatGetCounts */


/*******************************************************************************
**
**  hyPhy20gHsciEthStatVarCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function selects a user-defined RX Variable or
**                 TX Variable counters in the HSCI
**                 MSTATX Ethernet Statistics 40-bits counters.
**
**                 The user-defined RX Variable is the number 27 counter
**                 of the Ethernet Statistics 40-bits counters.
**                 The user-defined TX Variable is the number 58 counter
**                 of the Ethernet Statistics 40-bits counters.
**
**                 This function is used for 10GE MAC Terminated mode.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - MSTATX instance in:
**                                0 - HSCI 0
**                                1 - HSCI 1
**                 dir          - 0 - TX Variable counter
**                                1 - RX Variable counter
**                 index        - RX Variable counter Programmable options
**                                     TX                  RX
**                                0x0  Octet OK            Octet OK
**                                0x1  Octet Error         Octet Error
**                                0x2  Unicast             Unicast
**                                0x3  Multicast           Multicast
**                                0x4  Broadcast           Broadcast
**                                0x5  Tagged              Tagged
**                                0x6  Control             Control
**                                0x7  PAUSE               PAUSE
**                                0x8  FCS                 FCS
**                                0x9  Long                In Range
**                                0xA  Short               Long
**                                0xB  transmit MAC Error  Jabber
**                                0xC  System Error        Short
**                                0xD  Reserved            Runt
**                                0xE  Reserved            Alignment
**                                0xF  Reserved            Filtered
**                                0x10 Reserved            Symbol
**                                0x11 Reserved            Layer1 Error
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciEthStatVarCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 dir, UINT4 index)
{
    INT4 result;

    /* argument checking */
    if (slice > 1 || dir > 1 || index > 0x11 ||
        (dir == 0 && index > 0xC)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* access the counter */
    result = utilHyPhy20gEthStatVarCfg(fileHndl, slice, dir, index);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHsciEthStatVarCfg */

/*******************************************************************************
**
**  hyPhy20gHsciFcCbrTxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts one of the HSCI FC_CBR block transmit
**                 counters.
**
**                 The argument accessMode when set to 1 applies to all counters
**                 within a slice.  When accessMode is set to 1 for any counter
**                 within a slice all counters within the slice are accumulated.
**                 Subsequent counters can be extracted by calling function
**                 with accessMode set to 0 - this will avoid multiple counter
**                 accumulations corrupting returned counts.
**
**                 This function is used for FC 8G and
**                 High Speed CBR (both byte-aligned and 8b10b encoded) modes.
**
**                 The counter "07 - TX_LCV_CNT" is only applicable for the
**                 8b10b encoded traffics.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - FC_CBR instance in:
**                                0 - HSCI 0
**                                1 - HSCI 1
**                 accessMode   - 1 - locally trigger counter accumulation
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                00 - TX_WRD_CNT
**                                01 - TX_BAD_CRC
**                                02 - TX_TOT_PKTS
**                                03 - FW_INS_CNT
**                                04 - PS_INS_CNT
**                                05 - FW_DEL_CNT
**                                06 - PS_DEL_CNT
**                                07 - TX_LCV_CNT
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciFcCbrTxGetCounts(struct file *fileHndl, UINT4 slice,
                                  UINT4 accessMode, UINT4 counter, 
                                  UINT4 *pCount)
{
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > 1 || accessMode > 1 || counter > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = utilHyPhy20gFcCbrTxGetCounts(fileHndl, slice,
                                          accessMode, counter, pCount);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciFcCbrTxGetCounts */


/*******************************************************************************
**
**  hyPhy20gHsciFcCbrRxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts one of the HSCI FC_CBR block receive
**                 counters.
**
**                 The argument accessMode when set to 1 applies to all counters
**                 within a slice.  When accessMode is set to 1 for any counter
**                 within a slice all counters within the slice are accumulated.
**                 Subsequent counters can be extracted by calling function
**                 with accessMode set to 0 - this will avoid multiple counter
**                 accumulations corrupting returned counts.
**
**                 This function is used for FC 8G and
**                 High Speed CBR (both byte-aligned and 8b10b encoded) modes.
**
**                 The counter "03 - RX_LCV_CNT" is only applicable for the
**                 8b10b encoded traffics.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - FC_CBR instance in:
**                                0 - HSCI 0
**                                1 - HSCI 1
**                 accessMode   - 1 - locally trigger counter accumulation
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                00 - RX_WRD_CNT
**                                01 - RX_BAD_CRC
**                                02 - RX_TOT_PKTS
**                                03 - RX_LCV_CNT
**
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciFcCbrRxGetCounts(struct file *fileHndl, UINT4 slice,
                                  UINT4 accessMode, UINT4 counter,
                                  UINT4 *pCount)
{
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > 1 || accessMode > 1 || counter > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = utilHyPhy20gFcCbrRxGetCounts(fileHndl, slice,
                                          accessMode, counter, pCount);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciFcCbrRxGetCounts */

/*******************************************************************************
**
**  hyPhy20gHsciEmpeiGetRxPmonCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts one of the EMPEI block RX inline
**                 buffer PMON counters.
**
**                 The argument accessMode when set to 1 or 2 applies to all 
**                 counters within the EMPEI block. When accessMode is set to 1 
**                 or 2 for any counter within the EMPEI block all counters 
**                 within the block are accumulated. Subsequent counters can be 
**                 extracted by calling function with accessMode set to 0 - this
**                 will avoid multiple counter accumulations corrupting returned
**                 counts.
**
**                 accessMode applies to counters associated with the following
**                 API:
**                     hyPhy20gLsciEmpeiGetInsQPmonCounts
**                     hyPhy20gLsciEmpeiGetTxPmonCounts
**                     hyPhy20gLsciEmpeiGetRxPmonCounts
**                     hyPhy20gHsciEmpeiGetInsQPmonCounts
**                     hyPhy20gHsciEmpeiGetTxPmonCounts
**                     hyPhy20gHsciEmpeiGetRxPmonCounts
**
**                 accessMode need only be set to 1 or 2 for one of these API
**                 for counter accumulation to be performed on all counters
**                 above.
**
**                 This function is used for 10GE MAC Terminated mode.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - EMPEI instance corresponding to:
**                                0 - HSCI 0
**                                1 - HSCI 1
**                 accessMode   - 2 - locally trigger counter accumulation
**                                    without resetting the internal counter
**                                    values to 0.
**                              - 1 - locally trigger counter accumulation
**                                    with resetting the internal counter
**                                    values to 0.
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                     pCount0             pCount1
**                                07 - RX_DROP_TAG7_10GE   RX_IN_TAG7_10GE
**                                06 - RX_DROP_TAG6_10GE   RX_IN_TAG6_10GE
**                                05 - RX_DROP_TAG5_10GE   RX_IN_TAG5_10GE
**                                04 - RX_DROP_TAG4_10GE   RX_IN_TAG4_10GE
**                                03 - RX_DROP_TAG3_10GE   RX_IN_TAG3_10GE
**                                02 - RX_DROP_TAG2_10GE   RX_IN_TAG2_10GE
**                                01 - RX_DROP_TAG1_10GE   RX_IN_TAG1_10GE
**                                00 - RX_DROP_TAG0_10GE   RX_IN_TAG0_10GE
**                *pCount0      - Pointer to storage for RX_DROP_TAGx_10GE
**                                (x=0~7) counter data
**                *pCount1      - Pointer to storage for RX_IN_TAGx_10GE
**                                (x=0~7) counter data
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_TIP_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciEmpeiGetRxPmonCounts(struct file *fileHndl, UINT4 slice,
                                      UINT4 accessMode, UINT4 counter,
                                      UINT4 *pCount0, UINT4 *pCount1)
{
    UINT4 addr,updateMsk, clearMsk;
    UINT4 cntDropReg, cntDropMsk;
    UINT4 cntInReg, cntInMsk;
    UINT4 tipMsk;
    UINT4 val0, val1;
    INT4 result;

    /* Argument check */
    if ((slice > 1) || (accessMode > 2) || (counter > 7))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* determine registers and masks associated with counter */
    addr = HYPHY20G_ENET_COM_EMPEI_REG_CFG;
    updateMsk = HYPHY20G_ENET_COM_EMPEI_BIT_CFG_PMON_UPDATE_MSK;
    clearMsk = HYPHY20G_ENET_COM_EMPEI_BIT_CFG_PMON_CLEAR_MSK;
    tipMsk = HYPHY20G_ENET_COM_EMPEI_BIT_CFG_TIP_MSK;
    
    switch(counter)
    {
        case 0: /* RX_DROP_TAG0_10GE  RX_IN_TAG0_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_DRP_1_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_DRP_1_RX_DROP_TAG0_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_IN_1_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_IN_1_RX_IN_TAG0_10GE_MSK;
            break;
        case 1: /* 01 - RX_DROP_TAG1_10GE   RX_IN_TAG1_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_DRP_1_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_DRP_1_RX_DROP_TAG1_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_IN_1_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_IN_1_RX_IN_TAG1_10GE_MSK;
            break;
        case 2: /* RX_DROP_TAG2_10GE   RX_IN_TAG2_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_DRP_1_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_DRP_1_RX_DROP_TAG2_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_IN_1_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_IN_1_RX_IN_TAG2_10GE_MSK;
            break;
        case 3: /* RX_DROP_TAG3_10GE   RX_IN_TAG3_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_DRP_2_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_DRP_2_RX_DROP_TAG3_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_IN_2_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_IN_2_RX_IN_TAG3_10GE_MSK;
            break;
        case 4: /* RX_DROP_TAG4_10GE   RX_IN_TAG4_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_DRP_2_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_DRP_2_RX_DROP_TAG4_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_IN_2_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_IN_2_RX_IN_TAG4_10GE_MSK;
            break;
        case 5: /* RX_DROP_TAG5_10GE   RX_IN_TAG5_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_DRP_2_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_DRP_2_RX_DROP_TAG5_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_IN_2_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_IN_2_RX_IN_TAG5_10GE_MSK;
            break;
        case 6: /* RX_DROP_TAG6_10GE   RX_IN_TAG6_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_DRP_3_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_DRP_3_RX_DROP_TAG6_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_IN_3_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_DRP_3_RX_DROP_TAG6_10GE_MSK;
            break;
        case 7: /* RX_DROP_TAG7_10GE   RX_IN_TAG7_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_DRP_3_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_DRP_3_RX_DROP_TAG7_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_RX_PMON_PKT_IN_3_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_RX_PMON_PKT_DRP_3_RX_DROP_TAG7_10GE_MSK;
            break;    
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* write PMON_UPDATE to 0 */
        result = hyPhy20gRegFieldWrite(fileHndl, addr, updateMsk, 0);
        if (result) return result;
            
    /* Poll PMON_UPDATE bit to ensure cleared for updating sucessfully */     
        result = sysHyPhy20gPollBit(fileHndl, addr, updateMsk, 0, 1000000);
        if(result==HYPHY20G_ERR_POLL_TIMEOUT) return HYPHY20G_ERR_PMON_FAULT;
        if(result) return result;
            
    /* update of the counter values if required */     
    if (accessMode > 0){
        switch(accessMode)
        {
        case 1: /* locally trigger counter accumulation with resetting the 
                   internal counter values to 0 */
            result =hyPhy20gRegFieldWrite (fileHndl, addr, clearMsk, 1);                
            break;
        case 2: /* locally trigger counter accumulation without resetting the 
                   internal counter values to 0 */
            result =hyPhy20gRegFieldWrite (fileHndl, addr, clearMsk, 0);
            if (result) return result;
            break;    
        default:
            return HYPHY20G_ERR_INVALID_ARG;
        }
        
        result = hyPhy20gRegFieldWrite(fileHndl, addr, updateMsk, 1);
        if (result) return result;    
    /* Poll TIP bit to ensure cleared for updating sucessfully */      
        result = sysHyPhy20gPollBit(fileHndl, addr, tipMsk, 0, 1000000);
        if(result==HYPHY20G_ERR_POLL_TIMEOUT) return HYPHY20G_ERR_TIP_TIMEOUT;
        if(result) return result;
    
    /***********Another method to poll TIP bit*******************
    interations = 0;
    i = 1;
    while (i) {
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if (result) return result;
        
        if ((val&0x2) == 0) {
            i = 0;
        }
        // fail if TIP bits not cleared within one second 
        if (iterations == 1000000) {
            return HYPHY20G_ERR_TIP_TIMEOUT;
        }
        // delay 1 us before next evaluation 
        result = sysHyPhy20gUsDelay(1);
        if (result) return result;
        iterations ++;
    }
    ************Poll TIP bit end******************/               
    }
   
    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, cntDropReg, cntDropMsk, &val0);
    if (result) return result;
    *pCount0 = val0;
    
    result = hyPhy20gRegFieldRead(fileHndl, cntInReg, cntInMsk, &val1);
    if (result) return result;
    *pCount1 = val1;
      
    return HYPHY20G_SUCCESS;
} /* hyPhy20gHsciEmpeiGetRxPmonCounts */


/*******************************************************************************
**
**  hyPhy20gHsciEmpeiGetTxPmonCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts one of the EMPEI block TX inline
**                 buffer PMON counters.
**
**                 The argument accessMode when set to 1 or 2 applies to all 
**                 counters within the EMPEI block. When accessMode is set to 1 
**                 or 2 for any counter within the EMPEI block all counters 
**                 within the block are accumulated. Subsequent counters can be 
**                 extracted by calling function with accessMode set to 0 - this
**                 will avoid multiple counter accumulations corrupting returned
**                 counts.
**
**                 accessMode applies to counters associated with the following
**                 API:
**                     hyPhy20gLsciEmpeiGetInsQPmonCounts
**                     hyPhy20gLsciEmpeiGetTxPmonCounts
**                     hyPhy20gLsciEmpeiGetRxPmonCounts
**                     hyPhy20gHsciEmpeiGetInsQPmonCounts
**                     hyPhy20gHsciEmpeiGetTxPmonCounts
**                     hyPhy20gHsciEmpeiGetRxPmonCounts
**
**                 accessMode need only be set to 1 or 2 for one of these API
**                 for counter accumulation to be performed on all counters
**                 above.
**
**                 This function is used for 10GE MAC Terminated mode.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - EMPEI instance corresponding to:
**                                0 - HSCI 0
**                                1 - HSCI 1
**                 accessMode   - 2 - locally trigger counter accumulation
**                                    without resetting the internal counter
**                                    values to 0.
**                              - 1 - locally trigger counter accumulation
**                                    with resetting the internal counter 
**                                    values to 0.
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                     pCount0             pCount1
**                                07 - TX_DROP_TAG7_10GE   TX_IN_TAG7_10GE
**                                06 - TX_DROP_TAG6_10GE   TX_IN_TAG6_10GE
**                                05 - TX_DROP_TAG5_10GE   TX_IN_TAG5_10GE
**                                04 - TX_DROP_TAG4_10GE   TX_IN_TAG4_10GE
**                                03 - TX_DROP_TAG3_10GE   TX_IN_TAG3_10GE
**                                02 - TX_DROP_TAG2_10GE   TX_IN_TAG2_10GE
**                                01 - TX_DROP_TAG1_10GE   TX_IN_TAG1_10GE
**                                00 - TX_DROP_TAG0_10GE   TX_IN_TAG0_10GE
**                *pCount0      - Pointer to storage for TX_DROP_TAGx_10GE
**                                (x=0~7) counter data
**                *pCount1      - Pointer to storage for TX_IN_TAGx_10GE
**                                (x=0~7) counter data
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciEmpeiGetTxPmonCounts(struct file *fileHndl, UINT4 slice,
                                      UINT4 accessMode, UINT4 counter,
                                      UINT4 *pCount0, UINT4 *pCount1)
{
    UINT4 addr,updateMsk, clearMsk;
    UINT4 cntDropReg, cntDropMsk;
    UINT4 cntInReg, cntInMsk;
    UINT4 tipMsk;
    UINT4 val0, val1;
    INT4 result;

    /* Argument check */
    if ((slice > 1) || (accessMode > 2) || (counter > 7))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* determine registers and masks associated with counter */
    addr = HYPHY20G_ENET_COM_EMPEI_REG_CFG;
    updateMsk = HYPHY20G_ENET_COM_EMPEI_BIT_CFG_PMON_UPDATE_MSK;
    clearMsk = HYPHY20G_ENET_COM_EMPEI_BIT_CFG_PMON_CLEAR_MSK;
    tipMsk = HYPHY20G_ENET_COM_EMPEI_BIT_CFG_TIP_MSK;
    
    switch(counter)
    {
        case 0: /* TX_DROP_TAG0_10GE   TX_IN_TAG0_10GE */                    
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_DRP_1_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_DRP_1_TX_DROP_TAG0_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_IN_1_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_IN_1_TX_IN_TAG0_10GE_MSK;
            break;
        case 1: /* TX_DROP_TAG1_10GE   TX_IN_TAG1_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_DRP_1_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_DRP_1_TX_DROP_TAG1_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_IN_1_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_IN_1_TX_IN_TAG1_10GE_MSK;
            break;
        case 2: /* TX_DROP_TAG2_10GE   TX_IN_TAG2_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_DRP_1_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_DRP_1_TX_DROP_TAG2_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_IN_1_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_IN_1_TX_IN_TAG2_10GE_MSK;
            break;
        case 3: /* TX_DROP_TAG3_10GE   TX_IN_TAG3_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_DRP_2_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_DRP_2_TX_DROP_TAG3_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_IN_2_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_IN_2_TX_IN_TAG3_10GE_MSK;
            break;
        case 4: /* TX_DROP_TAG4_10GE   TX_IN_TAG4_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_DRP_2_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_DRP_2_TX_DROP_TAG4_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_IN_2_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_IN_2_TX_IN_TAG4_10GE_MSK;
            break;
        case 5: /* TX_DROP_TAG5_10GE   TX_IN_TAG5_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_DRP_2_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_DRP_2_TX_DROP_TAG5_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_IN_2_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_IN_2_TX_IN_TAG5_10GE_MSK;
            break;
        case 6: /* TX_DROP_TAG6_10GE   TX_IN_TAG6_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_DRP_3_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_DRP_3_TX_DROP_TAG6_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_IN_3_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_IN_3_TX_IN_TAG6_10GE_MSK;
            break;
        case 7: /* TX_DROP_TAG7_10GE   TX_IN_TAG7_10GE */
            cntDropReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_DRP_3_OFFSET(slice);
            cntDropMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_DRP_3_TX_DROP_TAG7_10GE_MSK;
            cntInReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_TX_PMON_PKT_IN_3_OFFSET(slice);
            cntInMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_TX_PMON_PKT_IN_3_TX_IN_TAG7_10GE_MSK;
            break;    
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* write PMON_UPDATE to 0 */
        result = hyPhy20gRegFieldWrite(fileHndl, addr, updateMsk, 0);
        if (result) return result;
            
    /* Poll PMON_UPDATE bit to ensure cleared for updating sucessfully */     
        result = sysHyPhy20gPollBit(fileHndl, addr, updateMsk, 0, 1000000);
        if(result==HYPHY20G_ERR_POLL_TIMEOUT) return HYPHY20G_ERR_PMON_FAULT;
        if(result) return result;
            
    /* update of the counter values if required */     
    if (accessMode > 0){
        switch(accessMode)
        {
        case 1: /* locally trigger counter accumulation with resetting the 
                   internal counter values to 0 */
            result =hyPhy20gRegFieldWrite (fileHndl, addr, clearMsk, 1);                
            break;
        case 2: /* locally trigger counter accumulation without resetting the 
                   internal counter values to 0 */
            result =hyPhy20gRegFieldWrite (fileHndl, addr, clearMsk, 0);
            if (result) return result;
            break;    
        default:
            return HYPHY20G_ERR_INVALID_ARG;
        }
        
        result = hyPhy20gRegFieldWrite(fileHndl, addr, updateMsk, 1);
        if (result) return result;    
    /* Poll TIP bit to ensure cleared for updating sucessfully */      
        result = sysHyPhy20gPollBit(fileHndl, addr, tipMsk, 0, 1000000);
        if(result==HYPHY20G_ERR_POLL_TIMEOUT) return HYPHY20G_ERR_TIP_TIMEOUT;
        if(result) return result;            
    }
    
    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, cntDropReg, cntDropMsk, &val0);
    if (result) return result;
    *pCount0 = val0;
    
    result = hyPhy20gRegFieldRead(fileHndl, cntInReg, cntInMsk, &val1);
    if (result) return result;
    *pCount1 = val1;
       
    return HYPHY20G_SUCCESS;
} /* hyPhy20gHsciEmpeiGetTxPmonCounts */


/*******************************************************************************
**
**  hyPhy20gHsciEmpeiGetInsQPmonCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts one of the EMPEI block insert queues'
**                 PMON counters.
**
**                 The argument accessMode when set to 1 or 2 applies to all 
**                 counters within the EMPEI block. When accessMode is set to 1 
**                 or 2 for any counter within the EMPEI block all counters 
**                 within the block are accumulated. Subsequent counters can be 
**                 extracted by calling function with accessMode set to 0 - this
**                 will avoid multiple counter accumulations corrupting returned
**                 counts.
**
**                 accessMode applies to counters associated with the following
**                 API:
**                     hyPhy20gLsciEmpeiGetInsQPmonCounts
**                     hyPhy20gLsciEmpeiGetTxPmonCounts
**                     hyPhy20gLsciEmpeiGetRxPmonCounts
**                     hyPhy20gHsciEmpeiGetInsQPmonCounts
**                     hyPhy20gHsciEmpeiGetTxPmonCounts
**                     hyPhy20gHsciEmpeiGetRxPmonCounts
**
**                 accessMode need only be set to 1 or 2 for one of these API
**                 for counter accumulation to be performed on all counters
**                 above.
**
**                 This function is used for 10GE MAC Terminated mode.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - EMPEI instance corresponding to:
**                                0 - HSCI 0
**                                1 - HSCI 1
**                 accessMode   - 2 - locally trigger counter accumulation
**                                    without resetting the internal counter
**                                    values to 0.
**                              - 1 - locally trigger counter accumulation
**                                    with resetting the internal counter
**                                    values to 0.
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                09 - MIPS_TX_INSERT_HI_Q_IN_10GE
**                                08 - MIPS_TX_INSERT_LOW_Q_IN_10GE
**                                07 - MIPS_RX_INSERT_Q_IN_10GE
**                                06 - MIPS_TX_INSERT_HI_Q_DROP_10GE
**                                05 - MIPS_TX_INSERT_LOW_Q_DROP_10GE
**                                04 - MIPS_RX_INSERT_Q_DROP_10GE
**                                03 - MGMT_INSERT_HI_Q_IN_10GE
**                                02 - MGMT_INSERT_LOW_Q_IN_10GE
**                                01 - MGMT_INSERT_HI_Q_DROP_10GE
**                                00 - MGMT_INSERT_LOW_Q_DROP_10GE
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciEmpeiGetInsQPmonCounts(struct file *fileHndl, UINT4 slice,
                                        UINT4 accessMode, UINT4 counter,
                                        UINT4 *pCount)
{
    UINT4 addr,updateMsk, clearMsk, tipMsk;
    UINT4 cntReg, cntMsk;
    UINT4 val;
    INT4 result;

    /* Argument check */
    if ((slice > 1) || (accessMode > 2) || (counter > 9))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* determine registers and masks associated with counter */
    addr = HYPHY20G_ENET_COM_EMPEI_REG_CFG;
    updateMsk = HYPHY20G_ENET_COM_EMPEI_BIT_CFG_PMON_UPDATE_MSK;
    clearMsk = HYPHY20G_ENET_COM_EMPEI_BIT_CFG_PMON_CLEAR_MSK;
    tipMsk = HYPHY20G_ENET_COM_EMPEI_BIT_CFG_TIP_MSK;

    switch(counter)
    {
        case 0: /* MGMT_INSERT_LOW_Q_DROP_10GE*/
            cntReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_INS_QUE_PMON_PKT_DRP_1_OFFSET(slice);
            cntMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_INS_QUE_PMON_PKT_DRP_1_MGMT_INSERT_LOW_Q_DROP_10GE_MSK;

            break;
        case 1: /* MGMT_INSERT_HI_Q_DROP_10GE */
            cntReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_INS_QUE_PMON_PKT_DRP_1_OFFSET(slice);
            cntMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_INS_QUE_PMON_PKT_DRP_1_MGMT_INSERT_HI_Q_DROP_10GE_MSK;
            break;
        case 2: /* MGMT_INSERT_LOW_Q_IN_10GE */
            cntReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_INS_QUE_PMON_PKT_IN_1_OFFSET(slice);
            cntMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_INS_QUE_PMON_PKT_IN_1_MGMT_INSERT_LOW_Q_IN_10GE_MSK; 
            break;
        case 3: /* MGMT_INSERT_HI_Q_IN_10GE */
            cntReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_INS_QUE_PMON_PKT_IN_1_OFFSET(slice);
            cntMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_INS_QUE_PMON_PKT_IN_1_MGMT_INSERT_HI_Q_IN_10GE_MSK;
            break;
        case 4: /* MIPS_RX_INSERT_Q_DROP_10GE */
            cntReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_INS_QUE_PMON_PKT_DRP_2_OFFSET(slice);
            cntMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_INS_QUE_PMON_PKT_DRP_2_MIPS_RX_INSERT_Q_DROP_10GE_MSK;
            break;
        case 5: /* MIPS_TX_INSERT_LOW_Q_DROP_10GE */
            cntReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_INS_QUE_PMON_PKT_DRP_2_OFFSET(slice);
            cntMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_INS_QUE_PMON_PKT_DRP_2_MIPS_TX_INSERT_LOW_Q_DROP_10GE_MSK;
            break;
        case 6: /* MIPS_TX_INSERT_HI_Q_DROP_10GE */
            cntReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_INS_QUE_PMON_PKT_DRP_2_OFFSET(slice);
            cntMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_INS_QUE_PMON_PKT_DRP_2_MIPS_TX_INSERT_HI_Q_DROP_10GE_MSK;
            break;
        case 7: /* MIPS_RX_INSERT_Q_IN_10GE */
            cntReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_INS_QUE_PMON_PKT_IN_2_OFFSET(slice);
            cntMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_INS_QUE_PMON_PKT_IN_2_MIPS_RX_INSERT_Q_IN_10GE_MSK;
            break; 
        case 8: /* MIPS_TX_INSERT_LOW_Q_IN_10GE */
            cntReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_INS_QUE_PMON_PKT_IN_2_OFFSET(slice);
            cntMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_INS_QUE_PMON_PKT_IN_2_MIPS_TX_INSERT_LOW_Q_IN_10GE_MSK;
            break;
        case 9: /* MIPS_TX_INSERT_HI_Q_IN_10GE */
            cntReg = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_INS_QUE_PMON_PKT_IN_2_OFFSET(slice);
            cntMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_INS_QUE_PMON_PKT_IN_2_MIPS_TX_INSERT_HI_Q_IN_10GE_MSK;
            break;       
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* update of the counter values if required */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, updateMsk, 0);
        if (result) return result;
            
    /* Poll PMON_UPDATE bit to ensure cleared for updating sucessfully */     
        result = sysHyPhy20gPollBit(fileHndl, addr, updateMsk, 0, 1000000);
        if(result==HYPHY20G_ERR_POLL_TIMEOUT) return HYPHY20G_ERR_PMON_FAULT;
        if(result) return result; 
 
    /* update of the counter values if required */     
    if (accessMode > 0){
        switch(accessMode)
        {
        case 1: /* locally trigger counter accumulation with resetting the 
                   internal counter values to 0 */
            result =hyPhy20gRegFieldWrite (fileHndl, addr, clearMsk, 1);                
            break;
        case 2: /* locally trigger counter accumulation without resetting the 
                   internal counter values to 0 */
            result =hyPhy20gRegFieldWrite (fileHndl, addr, clearMsk, 0);
            if (result) return result;
            break;    
        default:
            return HYPHY20G_ERR_INVALID_ARG;
        }
        
        result = hyPhy20gRegFieldWrite(fileHndl, addr, updateMsk, 1);
        if (result) return result;    
    /* Poll TIP bit to ensure cleared for updating sucessfully */      
        result = sysHyPhy20gPollBit(fileHndl, addr, tipMsk, 0, 1000000);
        if(result==HYPHY20G_ERR_POLL_TIMEOUT) return HYPHY20G_ERR_TIP_TIMEOUT;
        if(result) return result;         
    }   
       
    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, cntReg, cntMsk, &val);
    if (result) return result;
    *pCount = val;
       
    return HYPHY20G_SUCCESS;
} /* hyPhy20gHsciEmpeiGetInsQPmonCounts */


/*******************************************************************************
**
**  hyPhy20gHsci10GeR64b66bGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts jitter counter from the R64B66B block
**                 of one of the two HSCI slices.
**
**                  Note: this function must be called after
**                        hyPhy20gHsci10GeTestPattInit.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - 10GE instance in:
**                                0 - HSCI 0
**                                1 - HSCI 1
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeR64b66bGetCounts(struct file *fileHndl, UINT4 slice,
                                      UINT4 *pCount)
{
    UINT4 addr;
    UINT4 msk;
    UINT4 cntReg;
    UINT4 cntMsk;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pCount == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    addr = HYPHY20G_HSCI_ENET_10G_R64B66B_REG_CFG;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk  = HYPHY20G_HSCI_ENET_10G_R64B66B_BIT_CFG_TIP_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if(result) return result;

    /* poll TIP bit */
    result = sysHyPhy20gPollBit(fileHndl, addr, msk, 0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    cntReg = HYPHY20G_HSCI_ENET_10G_R64B66B_REG_JTR_CNT;
    cntReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cntReg, slice);
    cntMsk = HYPHY20G_HSCI_ENET_10G_R64B66B_BIT_JTR_CNT_JITTER_COUNT_MSK;

    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, cntReg, cntMsk, &val);
    if (result) return result;
    *pCount = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeR64b66bGetCounts */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
