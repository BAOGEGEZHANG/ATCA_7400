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
**  MODULE      : HyPHY 20G Packet Interface Configuration Code
**
**  FILE        : hyphy20g_pif.c
**
**  $Date: 2013-01-17 19:35:54 +0800 (Thu, 17 Jan 2013) $
**
**  $Revision: 30563 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_pif.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 pifHyPhy20gPifInitCtxt(struct file *fileHndl, sHYPHY20G_PIF_CTXT *pCtxt);

INT4 pifHyPhy20gPifCtxtGetMode(struct file *fileHndl, UINT4 *pMode);

INT4 pifHyPhy20gPifTopCfg(struct file *fileHndl, UINT4 hdrStrip, UINT4 mode,
                          UINT4 segSize);

INT4 pifHyPhy20gPifTxSchdInit(struct file *fileHndl, UINT4 schd, UINT4 schdLen,
                              UINT4 enable, UINT4 reset);

INT4 pifHyPhy20gPifSchdUpdate(struct file  *fileHndl,
                              sHYPHY20G_PIF_TX_CHNL_CTXT *pTxChnl);

INT4 pifHyPhy20gPifGenerateTxSchd(struct file  *fileHndl,
                                  sHYPHY20G_SCHD_CTXT *pSchd, UINT4 *pCalData);

INT4 pifHyPhy20gPifXauiInit(struct file *fileHndl, UINT4 slice, UINT4 enable,
                            UINT4 mode);

INT4 pifHyPhy20gPifSpiFcCalCfg(struct file *fileHndl, UINT4 dir,
                               UINT4 parity, UINT4 dip2En,
                               UINT4 iterationCnt, UINT4 calLen,
                               UINT4 updateMsk);

INT4 pifHyPhy20gPifSpiFcCalProg(struct file *fileHndl, UINT4 dir, UINT4 calLen,
                                const UINT4 *pCalData);

PRIVATE INT4 pifHyPhy20gPifAuditTxChnlCtxt(struct file *fileHndl);
/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/
/*******************************************************************************
**
**  hyPhy20gPifInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configures the PIF subsystem for a major interface mode
**                  of operation. This can be:
**                      Disabled
**                      eXAUI0 (with SPI4.2 Out-of-band flow control)
**                      eXAUI1 (with SPI4.2 Out-of-band flow control)
**                      eXAUI0 and eXAUI1 (with SPI4.2 out-of-band flow control)
**                      INTERLAKEN (with Interlaken in-band flow control)
**                      INTERLAKEN (with Interlaken out-of-band flow control)
**                      standard XAUI on XAUI0 (with inband PAUSE flow control)
**                      standard XAUI on XAUI1 (with inband PAUSE flow control)
**                      standard XAUI on XAUI0 and XAUI1 (with inband PAUSE flow
**                      control)
**
**                  When configuring for eXAUI or standard XAUI mode:
**                      1) hyPhy20gPifSerdesInit and hyPhy20gPifRateCfg
**                         must be called AFTER this function.
**                      2) hyPhy20gPifFlowCtrlInit, hyPhy20gPifChnlFcMap and
**                         hyPhy20gPifXauiInit should be called after
**                         hyPhy20gPifSerdesInit and hyPhy20gPifRateCfg.
**
**                  When configuring for Interlaken mode:
**                      1) hyPhy20gPifSerdesInit and hyPhy20gPifRateCfg
**                         must be called BEFORE this function.
**                      2) hyPhy20gPifIntlknInit should be called AFTER this
**                         function.
**
**                  Alternatively, this function can be called to disable the
**                  PIF subsystem.
**
**                  RD_TIMEOUT, ACK_TIMEOUT and NACK_RCNT timeouts are 
**                  internally hardcoded to maximum value (0xFFFF), because 
**                  these timeouts are not meant to happen in normal mode of 
**                  operation.
**
**                  User can set 'lnkZone' parameter along with equivalent
**                  'zoneX' (X = 1..3) parameter in hyPhy20gCpbPifLnkZnConfig,
**                  based on how quickly PIF interface need to
**                  backpressure the peer device sending data to HyPHY 20G.
**
**  INPUTS:         fileHndl    - device handle
**                  mode        - operation mode of PIF subsystem and
**                                eXAUI/INTERLAKEN/standard XAUI protocol being
**                                used by the interface
**                                0 - disable the PIF subsystem
**                                1 - XAUI0 slice in eXAUI mode
**                                2 - XAUI1 slice in eXAUI mode
**                                3 - XAUI0 and XAUI1 slices in eXAUI mode
**                                4 - INTERLAKEN (with Interlaken in-band
**                                    flow control)
**                                5 - INTERLAKEN (with Interlaken out-of-band
**                                    flow control)
**                                6 - XAUI0 slice in standard XAUI mode
**                                7 - XAUI1 slice in standard XAUI mode
**                                8 - XAUI0 and XAUI1 slice in standard XAUI
**                                    mode
**                                9 - XAUI0 in eXAUI mode and XAUI1 
**                                    in standard XAUI mode
**                                10 - XAUI0 in standard XAUI mode and XAUI1 
**                                     in eXAUI mode
**                  lnkZoneXoff - link zone threshold to control which CPB
**                                communal FIFO zone region that triggers PAUSE
**                                or link level XOFF status when the FIFO
**                                occupancy is rising.
**                                Setting to 0 always reports FULL status to
**                                upstream device.
**                                Legal range 0 to 3.
**                  lnkZoneXon  - link zone threshold to control which CPB
**                                communal FIFO zone region that disables PAUSE
**                                or generates link level XON status when the
**                                FIFO occupancy is falling.
**                                lnkZoneXon must always be less than
**                                lnkZoneXoff.  Legal range 0 to 3.
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 hyPhy20gPifInit(struct file *fileHndl, UINT4 mode, UINT4 lnkZoneXoff, 
                     UINT4 lnkZoneXon)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 laneOff, slice;
    sHYPHY20G_CTXT *pDevCtxt;
    const UINT4 RD_TIMEOUT    = 0xFFFF;
    const UINT4 ACK_TIMEOUT   = 0xFFFF;
    const UINT4 NACK_RCNT     = 0xFFFF;

    /* argument checking */
    if (mode > 10) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    } else if (mode != 0) {
        if (lnkZoneXoff > 3 || lnkZoneXon > 3 ||
            lnkZoneXon > lnkZoneXoff) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);

    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "pDevCtxt is NULL.");

    /* set lowpwr bit to 1 */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                             HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_PIF_LOWPWR_OFF,
                             1);
    if(result) return result;

    /* reset at top level */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_3_PIF_RST_OFF,
                                 1);
    if(result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_3,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_3_PIF_RST_OFF,
                                 0);
    if(result) return result;

    if (mode == HYPHY20G_DISABLE) {           /* disable operations */
        /* free memory used by PIF subsystem */
        result = pifHyPhy20gPifDestroyCtxt(fileHndl, 0);
        if (result) return result;

        return HYPHY20G_SUCCESS;
    }

    /*
     * enable operations
     */

    /* must ensure that we are not already initialized or we will duplicate
       the memory allocation */
    if (pDevCtxt->pPifCtxt != NULL) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* remove lowpwr if subsystem to be enabled */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                             HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_PIF_LOWPWR_OFF,
                             0);
    if(result) return result;

    /* allocate and initialize memory used by pif subsystem */
    result = pifHyPhy20gPifCreateCtxt(fileHndl, mode);
    if (result) return result;

    addr = HYPHY20G_PIF_PIF_REG_SW_RST;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_PIF_PIF_BIT_SW_RST_SCH1_RST_MSK,
            HYPHY20G_PIF_PIF_BIT_SW_RST_SCH1_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_PIF_PIF_BIT_SW_RST_SCH0_RST_MSK,
            HYPHY20G_PIF_PIF_BIT_SW_RST_SCH0_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_PIF_PIF_BIT_SW_RST_FC_CTRLR_RST_MSK,
            HYPHY20G_PIF_PIF_BIT_SW_RST_FC_CTRLR_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_PIF_PIF_BIT_SW_RST_RX_PATH_RST_MSK,
            HYPHY20G_PIF_PIF_BIT_SW_RST_RX_PATH_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_PIF_PIF_BIT_SW_RST_TX_PATH_RST_MSK,
            HYPHY20G_PIF_PIF_BIT_SW_RST_TX_PATH_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_PIF_PIF_BIT_SW_RST_INTLKN_RST_MSK,
            HYPHY20G_PIF_PIF_BIT_SW_RST_INTLKN_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_PIF_PIF_BIT_SW_RST_XAUI1_RST_MSK,
            HYPHY20G_PIF_PIF_BIT_SW_RST_XAUI1_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_PIF_PIF_BIT_SW_RST_XAUI0_RST_MSK,
            HYPHY20G_PIF_PIF_BIT_SW_RST_XAUI0_RST_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* put all sub-blocks into reset */
    if (mode == HYPHY20G_PIF_OPMODE_EXAUI0 ||
        mode == HYPHY20G_PIF_OPMODE_SXAUI0 ||
        mode == HYPHY20G_PIF_OPMODE_EXAUI1 ||
        mode == HYPHY20G_PIF_OPMODE_SXAUI1 ||
        mode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
        mode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
        mode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
        mode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1)
    {
        /* XAUI specific */
        for (slice = 0; slice < 2; slice++) {
            addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(HYPHY20G_XAUI_TOP_REG_SW_RST,
                                                       slice);
            result = hyPhy20gRegFieldWrite(fileHndl, addr,
                                  HYPHY20G_XAUI_TOP_BIT_SW_RST_XTEF_RST_MSK, 1);
            if(result) return result;
        }
    }

    /* configure read timeout */
    result = hyPhy20gRegFieldWrite (fileHndl, HYPHY20G_PIF_PIF_REG_RD_TOUT,
                            HYPHY20G_PIF_PIF_BIT_RD_TOUT_RD_TIMEOUT_MSK,
 				            RD_TIMEOUT);
    if (result) return result;

    /* configure ack timeout */
    result = hyPhy20gRegFieldWrite (fileHndl, HYPHY20G_PIF_PIF_REG_ACK_TM_OUT,
                            HYPHY20G_PIF_PIF_BIT_ACK_TM_OUT_ACK_TIMEOUT_MSK,
 				            ACK_TIMEOUT);
    if (result) return result;

    /* configure maximum number of NACKs */
    result = hyPhy20gRegFieldWrite (fileHndl,
                            HYPHY20G_PIF_PIF_REG_NACK_RETRY_CNT,
                            HYPHY20G_PIF_PIF_BIT_NACK_RETRY_CNT_NACK_RCNT_MSK,
 				            NACK_RCNT);
    if (result) return result;

    /* configure link zone threshold */
    addr = HYPHY20G_PIF_PIF_REG_LNK_FLOW_CTL;
    result = sysHyPhy20gRead(fileHndl, addr, &val);

    result = hyPhy20gVarFieldWrite(&val,
                      HYPHY20G_PIF_PIF_BIT_LNK_FLOW_CTL_LINK_XOFF_THRESHOLD_MSK,
                      lnkZoneXoff);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val,
                       HYPHY20G_PIF_PIF_BIT_LNK_FLOW_CTL_LINK_XON_THRESHOLD_MSK,
                       lnkZoneXon);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* XAUI specific operations that are sensitive relative to PIF SERDES
       bring up order */
    if (mode == HYPHY20G_PIF_OPMODE_EXAUI0 ||
        mode == HYPHY20G_PIF_OPMODE_SXAUI0 ||
        mode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
        mode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
        mode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
        mode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1)
    {
        /* enable lanes */
        laneOff = HYPHY20G_PIF_PIF_BIT_IF_EN_LANE_DISABLE_OFF;
        result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_PIF_PIF_REG_IF_EN,
                                       (0xF << laneOff), 0x0);
        if (result) return result;

        /* enable block */
        result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_PIF_PIF_REG_IF_EN,
                                    HYPHY20G_PIF_PIF_BIT_IF_EN_EN_XAUI0_MSK, 1);
        if(result) return result;

        /* release XAUI reset */
        result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST,
                                  HYPHY20G_PIF_PIF_BIT_SW_RST_XAUI0_RST_MSK, 0);
        if(result) return result;

        /* release SCBS reset */
        result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST,
                                   HYPHY20G_PIF_PIF_BIT_SW_RST_SCH0_RST_MSK, 0);
        if(result) return result;
    }
    if (mode == HYPHY20G_PIF_OPMODE_EXAUI1 ||
        mode == HYPHY20G_PIF_OPMODE_SXAUI1 ||
        mode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
        mode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
        mode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
        mode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1)
    {
        /* enable lanes */
        laneOff = HYPHY20G_PIF_PIF_BIT_IF_EN_LANE_DISABLE_OFF + 4;
        result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_PIF_PIF_REG_IF_EN,
                                       (0xF << laneOff), 0x0);
        if (result) return result;

        /* enable block */
        result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_PIF_PIF_REG_IF_EN,
                                    HYPHY20G_PIF_PIF_BIT_IF_EN_EN_XAUI1_MSK, 1);
        if(result) return result;

        /* release XAUI reset */
        result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST,
                                  HYPHY20G_PIF_PIF_BIT_SW_RST_XAUI1_RST_MSK, 0);
        if(result) return result;

        /* release SCBS reset */
        result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST,
                                   HYPHY20G_PIF_PIF_BIT_SW_RST_SCH1_RST_MSK, 0);
        if(result) return result;
    }


    /* release rx and tx reset */
    result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST,
                                HYPHY20G_PIF_PIF_BIT_SW_RST_RX_PATH_RST_MSK, 0);
    if(result) return result;
    result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST,
                                HYPHY20G_PIF_PIF_BIT_SW_RST_TX_PATH_RST_MSK, 0);
    if(result) return result;


    if (mode == HYPHY20G_PIF_OPMODE_EXAUI0 ||
        mode == HYPHY20G_PIF_OPMODE_SXAUI0 ||
        mode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
        mode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
        mode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
        mode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1)
    {
        addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(HYPHY20G_XAUI_TOP_REG_SW_RST,
                                                   0);
        result = hyPhy20gRegFieldWrite(fileHndl, addr,
                                  HYPHY20G_XAUI_TOP_BIT_SW_RST_XTEF_RST_MSK, 0);
        if(result) return result;
    }
    if (mode == HYPHY20G_PIF_OPMODE_EXAUI1 ||
        mode == HYPHY20G_PIF_OPMODE_SXAUI1 ||
        mode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
        mode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
        mode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
        mode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1)
    {
        addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(HYPHY20G_XAUI_TOP_REG_SW_RST,
                                                   1);
        result = hyPhy20gRegFieldWrite(fileHndl, addr,
                                  HYPHY20G_XAUI_TOP_BIT_SW_RST_XTEF_RST_MSK, 0);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifInit */

/*******************************************************************************
**
**  hyPhy20gPifIntfEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs PIF interface level enable and
**                  disable operations.
**
**                  When enabling:
**
**                    For the Tx direction, it will resume trasmission of data
**                    from the CPB subsystem.
**
**                    For the Rx direction, it will send valid link level flow
**                    control status to the upstream device.
**
**                  When disabling:
**
**                    For the Tx direction, it will pause data transfer
**                    from the CPB subsystem.
**
**                    For the Rx direction, it will backpressure the upstream
**                    device with pause frames or link level XOFF indications.
**
**                  This function must be called after:
**                  hyPhy20gPifFlowCtrlInit
**
**                  This function is called inside of hyPhy20gPifFlowCtrlInit
**                  function to intially disable PIF interface.
**
**                  When SPI4.2 or Interlaken out-of-band flow control is used,
**                  set RX_OOB_LINK_FC to control sending link level flow
**                  control status.
**
**                  When Interlaken in-band flow control is used,
**                  set RX_LINK0_FC to control sending link level flow
**                  control status.
**
**
**  INPUTS:         fileHndl  - device handle
**                  enable   -  1 - enable PIF interface to normal operation
**                              0 - send link level XOFF/FULL status or
**                              pause data transmission
**                  updateMsk - Mask of link level flow control related
**                              configuration field to updated.
**                              Set the corresponding bit to 1 to update a
**                              field, set to 0 to maintain value of configured
**                              field:
**                              bit 0 - update RX_OOB_LINK_FC field
**                              bit 1 - update RX_LINK0_FC field
**                              bit 2 - update RX_LINK1_FC field
**                              bit 3 - update TX_LINK0_FC field
**                              bit 4 - update TX_LINK1_FC field
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifIntfEnable(struct file *fileHndl, UINT4 enable, UINT4 updateMsk)
{
    INT4 result;
    UINT4 addr, val, disable;

    /* argument checking */
    if (updateMsk > 0x1F || enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    disable = (enable == 0) ? 1 : 0;
    addr = HYPHY20G_PIF_PIF_REG_LNK_FLOW_CTL;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    if (updateMsk & 0x10) {
        val = mHYPHY20G_SET_FIELD4(val, disable,
                HYPHY20G_PIF_PIF_BIT_LNK_FLOW_CTL_TX_LINK1_FC_MSK,
                HYPHY20G_PIF_PIF_BIT_LNK_FLOW_CTL_TX_LINK1_FC_OFF);
    }

    if (updateMsk & 0x8) {
        val = mHYPHY20G_SET_FIELD4(val, disable,
                HYPHY20G_PIF_PIF_BIT_LNK_FLOW_CTL_TX_LINK0_FC_MSK,
                HYPHY20G_PIF_PIF_BIT_LNK_FLOW_CTL_TX_LINK0_FC_OFF);
    }

    if (updateMsk & 0x4) {
        val = mHYPHY20G_SET_FIELD4(val, disable,
                HYPHY20G_PIF_PIF_BIT_LNK_FLOW_CTL_RX_LINK1_FC_MSK,
                HYPHY20G_PIF_PIF_BIT_LNK_FLOW_CTL_RX_LINK1_FC_OFF);
    }

    if (updateMsk & 0x2) {
        val = mHYPHY20G_SET_FIELD4(val, disable,
                HYPHY20G_PIF_PIF_BIT_LNK_FLOW_CTL_RX_LINK0_FC_MSK,
                HYPHY20G_PIF_PIF_BIT_LNK_FLOW_CTL_RX_LINK0_FC_OFF);
    }

    if (updateMsk & 0x1) {
        val = mHYPHY20G_SET_FIELD4(val, disable,
                HYPHY20G_PIF_PIF_BIT_LNK_FLOW_CTL_RX_OOB_LINK_FC_MSK,
                HYPHY20G_PIF_PIF_BIT_LNK_FLOW_CTL_RX_OOB_LINK_FC_OFF);
    }

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifIntfEnable */


/*******************************************************************************
**
**  hyPhy20gPifLaneDisable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function disables one or more of the 8 SERDES interface
**                  lanes of the PIF interface associated with Interlaken.
**                  It can be optionally used to disable the specific 3 lanes
**                  when PIF interface is configured in Interlaken 5 lanes mode.
**
**                  The transmit and receive lanes are configured symmetrically.
**
**                  The Interlaken block must be 'Major Bidirectional States' of
**                  2 in order for this function to be executed, otherwise
**                  HYPHY20g_ERR_INVALID_STATE will be returned.
**
**                  This function is only valid for Interlaken.
**
**  INPUTS:         *fileHndl  - base address
**                  disableMsk - Mask of lanes to be disabled
**                               Legal range 0 to 0xFF
**                               bit index n to n+7 corresponds to SERDES lanes
**                               m to m+7 where n = 0 and m = starting index of
**                               the groups of 8 SERDES lanes (m = 0, 8, 16, 24)
**                               Note setting of 0xFF irrelevant as all lanes
**                               disabled.
**                               Set to 1 to disable a lane.
**                               Set to 0 to enable a lane.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifLaneDisable(struct file *fileHndl, UINT4 disableMsk)
{
    /* variable declaration */
    UINT4 state;
    INT4 result;
    const UINT4 slice = HYPHY20G_INTLKN_PIF_INTLKN;

    /* argument checking */
    if(disableMsk > 0xFF){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve major bidirectional state */
    result = utilHyPhy20gIntlknCoreGetMajBiDirState(fileHndl, slice, &state);
    if(result) return result;

    /* confirm major bidirection state of 2 or return invalid state error */
    if(state != HYPHY20G_INTLKN_STATE_STANDBY){
        return HYPHY20G_ERR_INVALID_STATE;
    }

    /* Configure PIF top level lane */
    result = hyPhy20gRegFieldWrite(fileHndl, HYPHY20G_PIF_PIF_REG_IF_EN,
                                   HYPHY20G_PIF_PIF_BIT_IF_EN_LANE_DISABLE_MSK,
                                   disableMsk);
    if (result) return result;

    /* Configure Interlaken lane */
    result = utilHyPhy20gIntlknCoreLaneDisable(fileHndl, slice, disableMsk);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifLaneDisable */


/*******************************************************************************
**
**  hyPhy20gPifFlowCtrlInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes PIF Rx or Tx flow control.
**                  It configures flow control mode, zone threshold and
**                  internally enable flow control.
**
**                  Depending on the operating mode of PIF subsystem, this 
**                  function internally selects a flow control mode, and then
**                  initializes SPI4.2 flow control calendar, or
**                  Interlaken flow control calendar with user specific
**                  calendar length, by calling hyPhy20gPifSpiFcCalInit or
**                  hyPhy20gPifIntlknFcInit function.
**
**                  This function disable PIF interface at last step, by
**                  calling hyPhy20gPifIntfEnable function.
**
**                  For Interlaken flow control configuration, this function 
**                  must be called after hyPhy20gPifInit and 
**                  hyPhy20ghyPhy20gPifIntlknInit.
**
**                  For standard XAUI and eXAUI flow control configuration, this 
**                  function must be called after hyPhy20gPifInit and before
**                  hyPhy20gPifXauiInit.
**
**  INPUTS:         fileHndl  - device handle
**                  dir       - flow control direction
**                              0 - Tx flow control(flow control to HyPHY 20G)
**                              1 - Rx flow control(flow control from HyPHY 20G)
**                  zone      - For Tx Direction, only valid for SPI4.2 flow
**                              control. Specifies the flow control status that
**                              will be interpreted as XOFF by the PIF transmit
**                              scheduler.  Legal value b'01, b'10, b'11. If
**                              set to b'00, an empty FIFO will always be
**                              interpreted as FULL by the PIF transmitter and
**                              no data transfers will be made.
**                              For the Rx Direction, only valid for Interlaken
**                              mode. Specifies which CPB zone threshold will be
**                              used to report XOFF condition to upstream device.
**                              Legal values b'01, b'10, b'11. If programmed to
**                              b'00 PIF will always report FULL for the CPB
**                              channel.
**                              For other modes this value is not processed.
**                  calLen    - the meaning of 'calLen' depends on the operating
**                              mode of PIF subsystem, which is given by 'mode'
**                              parameter in hyPhy20gPifInit API:
**                              For 'mode' == 4:
**                              1 to 256 - Number of flow control channel
**                                         entries reported or received in
**                                         Interlaken Idle and Burst Control
**                                         Words.
**                              For 'mode' == 5:
**                              1 to 256 - Number of flow control channel
**                                         entries in Interlaken flow control
**                                         calendar.
**                              For 'mode' == 1, 2 or 3:
**                              1 to 256 - Number of flow control channel
**                                         entries in SPI4.2 flow control
**                                         calendar.
**                              For other modes this parameter is not processed.
**                  oobFcClk  - For the SPI4.2 flow control mode, this parameter 
**                              selects out-of-band flow control clock polarity;
**                              For the other flow control modes, this parameter 
**                              is not processed.
**                              0 - OOB status is driven out on the rising edge
**                                  of OOB clock;
**                              1 - OOB status is driven out on the falling edge 
**                                  of OOB clock;
**                              When 'dir' == 0:
**                                  OOB clock refers to TX_OOB_CLK (input);
**                              When 'dir' == 1:
**                                  OOB clock refers to RX_OOB_CLK (output);
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_STATE
**
*******************************************************************************/
INT4 hyPhy20gPifFlowCtrlInit(struct file *fileHndl, UINT4 dir, UINT4 zone, 
                             UINT4 calLen, UINT4 oobFcClk)
{
    INT4 result;
    UINT4 addr, msk;
    UINT4 fcModeMsk, zoneMsk, enblOff, znRstValMsk;
    UINT4 updateMsk;
    UINT4 pifMode;
    UINT4 fcMode;
    UINT4 oobFcClkMsk;
    UINT4 oobFcClkVal;

    /* argument checking */
    if (dir > 1 || zone > 3 || oobFcClk >1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = pifHyPhy20gPifCtxtGetMode(fileHndl,(UINT4 *)&pifMode);
    if (result) return result;

    /* select a flow control mode */
    switch(pifMode){
        case HYPHY20G_PIF_OPMODE_EXAUI0:
        case HYPHY20G_PIF_OPMODE_EXAUI1:
        case HYPHY20G_PIF_OPMODE_DUAL_EXAUI:
        case HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1:
        case HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1:
            fcMode = 2;     /* SPI4.2 out-of-band flow control */
            break;
        case HYPHY20G_PIF_OPMODE_INTERLAKEN_INBAND:
            fcMode = 0;     /* Interlaken in-band flow control */
            break;
        case HYPHY20G_PIF_OPMODE_INTERLAKEN_OOB:
            fcMode = 1;     /* Interlaken out-of-band flow control */
            break;
        case HYPHY20G_PIF_OPMODE_SXAUI0:
        case HYPHY20G_PIF_OPMODE_SXAUI1:
        case HYPHY20G_PIF_OPMODE_DUAL_SXAUI:
            fcMode = 3;     /* Pause Frame */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* argument checking */
    if (fcMode < 3 && (calLen < 1 || calLen > 256)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (dir == HYPHY20G_RX_DIRECTION) {
        addr = HYPHY20G_PIF_PIF_REG_RX_FC_CTL;
        enblOff = HYPHY20G_PIF_PIF_BIT_RX_FC_CTL_RX_ENABLE_FC_OFF;
        fcModeMsk = HYPHY20G_PIF_PIF_BIT_RX_FC_CTL_RX_FC_MODE_MSK;
        zoneMsk = HYPHY20G_PIF_PIF_BIT_RX_FC_CTL_RX_ZONE_THRESHOLD_MSK;
        znRstValMsk = HYPHY20G_PIF_PIF_BIT_RX_FC_CTL_RX_ZN_RST_VALUE_MSK;
        oobFcClkMsk = (0x0001 << 4);
    } else {
        addr = HYPHY20G_PIF_PIF_REG_TX_FC_CTL;
        enblOff = HYPHY20G_PIF_PIF_BIT_TX_FC_CTL_TX_ENABLE_FC_OFF;
        fcModeMsk = HYPHY20G_PIF_PIF_BIT_TX_FC_CTL_TX_FC_MODE_MSK;
        zoneMsk = HYPHY20G_PIF_PIF_BIT_TX_FC_CTL_TX_ZONE_THRESHOLD_MSK;
        znRstValMsk = HYPHY20G_PIF_PIF_BIT_TX_FC_CTL_TX_ZN_RST_VALUE_MSK;
        oobFcClkMsk = (0x0001 << 13);
    }

    /* put FC_CTRLR_RST into reset before setting FC_MODE, ZONE_THRESHOLD
       and XAUIn_FC_ALIAS_EN bits */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST,
                             HYPHY20G_PIF_PIF_BIT_SW_RST_FC_CTRLR_RST_OFF,
                             1);
    if(result) return result;

    /* Program the flow control mode */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, fcModeMsk, fcMode);
    if (result) return result;

    if (fcMode < 3) {
        /* Program the zone threshold */
        /* TX_ZONE_THRESHOLD is not used for Interlaken FC mode,
           and RX_ZONE_THRESHOLD is not used for SPI4.2 mode*/
        if (((fcMode == 0 || fcMode == 1) && dir == HYPHY20G_RX_DIRECTION) ||
            (fcMode == 2  && dir == HYPHY20G_TX_DIRECTION)) {
            result = hyPhy20gRegFieldWrite(fileHndl, addr, zoneMsk, zone);
            if (result) return result;
        }
    }

    oobFcClkVal = (fcMode == 2) ? oobFcClk : 0;
    /* Program the out-of-band flow control clock polarity */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, oobFcClkMsk, oobFcClkVal);
    if (result) return result;

    /* clear XAUIn_FC_ALIAS_EN */
    addr = HYPHY20G_PIF_PIF_REG_FLOW_CTL_ALIAS;
    if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI0 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI0 ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
        pifMode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1) {
        msk = HYPHY20G_PIF_PIF_BIT_FLOW_CTL_ALIAS_XAUI0_FC_ALIAS_EN_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0);
        if (result) return result;
    }

    if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
        pifMode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1) {
        msk = HYPHY20G_PIF_PIF_BIT_FLOW_CTL_ALIAS_XAUI1_FC_ALIAS_EN_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0);
        if (result) return result;
    }

    /* remove FC_CTRLR_RST reset */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST,
                             HYPHY20G_PIF_PIF_BIT_SW_RST_FC_CTRLR_RST_OFF,
                             0);
    if(result) return result;

    if (fcMode < 3) {
        /* set to 3 to ensure all the channels are in XOFF condition initially */
        result = hyPhy20gRegFieldWrite(fileHndl, addr, znRstValMsk, 3);
        if (result) return result;

        /* enable flow control */
        result = sysHyPhy20gBitWrite(fileHndl, addr, enblOff, 1);
        if(result) return result;

        /* initialize the flow control calendar */
        if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI0 ||
            pifMode == HYPHY20G_PIF_OPMODE_EXAUI1 ||
            pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI||
            pifMode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
            pifMode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1) {
            result = hyPhy20gPifSpiFcCalInit(fileHndl, dir, calLen);
            if(result) return result;

        } else if (pifMode == HYPHY20G_PIF_OPMODE_INTERLAKEN_INBAND) {
            result = hyPhy20gPifIntlknFcInit(fileHndl, dir, 1, calLen);
            if(result) return result;

        } else if (pifMode == HYPHY20G_PIF_OPMODE_INTERLAKEN_OOB) {
            result = hyPhy20gPifIntlknFcInit(fileHndl, dir, 2, calLen);
            if(result) return result;
        }

        if (dir == HYPHY20G_RX_DIRECTION) {
            switch(pifMode)
            {
                case HYPHY20G_PIF_OPMODE_EXAUI0:
                case HYPHY20G_PIF_OPMODE_EXAUI1:
                case HYPHY20G_PIF_OPMODE_DUAL_EXAUI:
                case HYPHY20G_PIF_OPMODE_SXAUI0:
                case HYPHY20G_PIF_OPMODE_SXAUI1:
                case HYPHY20G_PIF_OPMODE_DUAL_SXAUI:
                case HYPHY20G_PIF_OPMODE_INTERLAKEN_OOB:
                case HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1:
                case HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1:
                    updateMsk = 0x1;
                    break;
                case HYPHY20G_PIF_OPMODE_INTERLAKEN_INBAND:
                    updateMsk = 0x2;
                    break;
                default:
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
        } else {
            switch(pifMode)
            {
                case HYPHY20G_PIF_OPMODE_EXAUI0:
                case HYPHY20G_PIF_OPMODE_SXAUI0:
                case HYPHY20G_PIF_OPMODE_INTERLAKEN_INBAND:
                case HYPHY20G_PIF_OPMODE_INTERLAKEN_OOB:
                    updateMsk = 0x08;
                    break;
                case HYPHY20G_PIF_OPMODE_EXAUI1:
                case HYPHY20G_PIF_OPMODE_SXAUI1:
                    updateMsk = 0x10;
                    break;
                case HYPHY20G_PIF_OPMODE_DUAL_EXAUI:
                case HYPHY20G_PIF_OPMODE_DUAL_SXAUI:
                case HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1:
                case HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1:
                    updateMsk = 0x18;
                    break;
                default:
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
        }

        /* Initially send link level XOFF/FULL status or pause data transmission
           to peer device */
        result = hyPhy20gPifIntfEnable(fileHndl, 0, updateMsk);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /*  hyPhy20gPifFlowCtrlInit */


/*******************************************************************************
**
**  hyPhy20gPifFlowCtrlEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enable (turn on) or disable (turn off) the
**                  Tx or Rx direction of flow control, by setting register
**                  bits in the FC controller and based on the Flow Control
**                  mode.
**
**                  This function must be used after:
**                  hyPhy20gPifFlowCtrlInit
**
**  INPUTS:         fileHndl  - device handle
**                  dir       - determine the flow control calendar
**                              0 - Tx flow control(flow control to HyPHY 20G)
**                              1 - Rx flow control(flow control from HyPHY 20G)
**                  enable    - 1 - enable
**                              0 - disable
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifFlowCtrlEnable(struct file *fileHndl, UINT4 dir, UINT4 enable)
{
    INT4 result;
    UINT4 pifMode;
    UINT4 fcCtlAddr, znRstValMsk, znRstOff, enFcOff;

    /* argument checking */
    if (dir > 1 || enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = pifHyPhy20gPifCtxtGetMode(fileHndl, &pifMode);
    if (result) return result;

    if (dir == HYPHY20G_RX_DIRECTION) {
        fcCtlAddr = HYPHY20G_PIF_PIF_REG_RX_FC_CTL;
        znRstValMsk = HYPHY20G_PIF_PIF_BIT_RX_FC_CTL_RX_ZN_RST_VALUE_MSK;
        znRstOff = HYPHY20G_PIF_PIF_BIT_RX_FC_CTL_RX_ZN_RST_OFF;
        enFcOff = HYPHY20G_PIF_PIF_BIT_RX_FC_CTL_RX_ENABLE_FC_OFF;
    } else {
        fcCtlAddr = HYPHY20G_PIF_PIF_REG_TX_FC_CTL;
        znRstValMsk = HYPHY20G_PIF_PIF_BIT_TX_FC_CTL_TX_ZN_RST_VALUE_MSK;
        znRstOff = HYPHY20G_PIF_PIF_BIT_TX_FC_CTL_TX_ZN_RST_OFF;
        enFcOff = HYPHY20G_PIF_PIF_BIT_TX_FC_CTL_TX_ENABLE_FC_OFF;
    }

    if (enable == HYPHY20G_ENABLE) {
        /* Optionally force the reset value */
        result = hyPhy20gRegFieldWrite(fileHndl, fcCtlAddr, znRstValMsk, 0x3);
        if (result) return result;
        result = sysHyPhy20gBitWrite(fileHndl, fcCtlAddr, znRstOff, 1);
        if (result) return result;

        /* Enable the flow control */
        result = sysHyPhy20gBitWrite(fileHndl, fcCtlAddr, enFcOff,
                                     HYPHY20G_ENABLE);
        if (result) return result;

    } else {    /* disable FC controller operations */
        /* addtional programming sequence to disable
           SPI4.2 RX FC controller */
        if (dir == HYPHY20G_RX_DIRECTION &&
            (pifMode < HYPHY20G_PIF_OPMODE_INTERLAKEN_INBAND ||
             pifMode > HYPHY20G_PIF_OPMODE_INTERLAKEN_OOB)) {

            /* Insert SYNC pattern, set FORCE_SYNC to 'b1 */
            result = sysHyPhy20gBitWrite(fileHndl, fcCtlAddr,
                        HYPHY20G_PIF_PIF_BIT_RX_FC_CTL_FORCE_SYNC_OFF, 1);
            if (result) return result;

            /* Wait for the FORCE_SYNC to be cleared */
            result = sysHyPhy20gPollBit(fileHndl, fcCtlAddr,
                            HYPHY20G_PIF_PIF_BIT_RX_FC_CTL_FORCE_SYNC_MSK,
                            0, HYPHY20G_DELAY_20USEC);
            if (result) return result;
        }

        /* disable the flow control */
        result = sysHyPhy20gBitWrite(fileHndl, fcCtlAddr, enFcOff,
                                     HYPHY20G_DISABLE);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifFlowCtrlEnable */


/*******************************************************************************
**
**  hyPhy20gPifTxChnlEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs PIF transmit channel enable and
**                  disable operations.
**
**                  When enabling:
**                  it adds the channel ID to SCBS in PIF transmit controller
**
**                  When disabling:
**                  it removes the channel ID from SCBS in PIF transmit
**                  controller
**
**                  Note, even when a channel ID is added into SCBS, transmit
**                  controller will not select this channel if it has been
**                  XOFFed through flow control.
**
**                  This function takes in the chnlRate argument.  chnlRate is
**                  used to communicate the relative rate of channels in a user
**                  defined granularity to the scheduler calendar weighting
**                  algorithm.
**
**                  The following should be noted for chnlRate usage:
**
**                  Scheduler weighting algorithm Operational Background:
**                  1) The algorithm will popluate all calendar entries with
**                     channel numbers.
**                   - If there is only one channel all entries will contain
**                     that channel number.
**                   - A channel must occupy a minimum of 1 entry
**                   - If there are more than one channel of the same rate, some
**                     channels may consume more calendar entries.
**                  2) The algorith will use chnlRate to determine the relative
**                     bandwidth of all channels to allocate calendar entries.
**                  3) The algorithm will not test to relate the chnlRate with a
**                     true data rate.
**
**                  The resolution of any interface scheduler is a function of:
**                  1) The total bandwidth being transferred across the
**                     interface
**                  2) The number of entries configured for the scheduler
**
**                  For example, if the PIF interface is configured in
**                  Interlaken mode for a maximum possible rate of 20G and the
**                  PIF scheduler used the maximum 32 rows for a total of 128
**                  entries, a single calendar entry would use 1/128 of all
**                  requests or 1/128 * 20G = .15626 G bandwidth rate of
**                  requests for data.
**
**                  "scheduler entry data request rate" =
**                     "Maximum interface capacity / number of calendar entries"
**
**                  The potential bandwidth of each channel within an
**                  interface can be determined:
**
**                  "potential bandwidth" = "scheduler entry data request rate"
**                   * "number of scheduler entries"
**
**                  Relating the calendar usage of entries with the potential
**                  data rate is left to the higher level calling software.
**
**
**  INPUTS:         fileHndl  - device handle
**                  chnlId    - the channel ID to be enabled
**                              Legal range 0 to 149
**                  enable    - 1 - enable the specific channel; add channel ID
**                                  into the Tx scheduler
**                              0 - disable the specific channel; remove
**                                  the channel ID from the Tx scheduler
**                  intf      - choose the interface to be associated with
**                              this channel
**                              1 - associate the channel with
**                                  eXAUI0/Interlaken instance (tx schd0)
**                              2 - associate the channel with eXAUI1 instance
**                                  (tx schd1)
**                              3 - reserved
**                              When enable == 0, set to 0
**                  chnlRate  - Refer to DESCRIPTION
**                              1 - 12000 in units of relative rate
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifTxChnlEnable(struct file *fileHndl, UINT4 chnlId, UINT4 enable,
                             UINT4 intf, UINT4 chnlRate)
{
    INT4 result;
    UINT4 pifMode;
    sHYPHY20G_PIF_TX_CHNL_CTXT *pTxChnl;
    sHYPHY20G_PIF_CTXT *pPifCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (chnlId >= HYPHY20G_PIF_TX_SCHD_MAX_CHNL || enable > 1 ||
        intf < 1 || intf > 3 || (chnlRate < 1) || (chnlRate > 12000)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = pifHyPhy20gPifCtxtGetMode(fileHndl, &pifMode);
    if (result) return result;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    pPifCtxt = pDevCtxt->pPifCtxt;
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL) && (pPifCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt or pPifCtxt is NULL.");

    pTxChnl = &(pPifCtxt->txChnlCtxt[chnlId]);
    pTxChnl->valid = HYPHY20G_VALID_SIGN;
    pTxChnl->rate0 = chnlRate;
    pTxChnl->rate1 = chnlRate;
    pTxChnl->state = (enable == 1) ?
                     HYPHY20G_PIF_CHNL_OPERATIONAL : HYPHY20G_PIF_CHNL_EQUIPPED;
    pTxChnl->intf = intf;

    /* update PIF Tx scheduler for the specific channel */
    result = pifHyPhy20gPifSchdUpdate(fileHndl, pTxChnl);
    if (result) return result;

    if (!enable) {            /* disable operation */
        /* reset the channel context memory */
        pTxChnl->valid = HYPHY20G_INVALID_VAL_32BIT;
        pTxChnl->rate0 = 0;
        pTxChnl->rate1 = 0;
        pTxChnl->state = HYPHY20G_PIF_CHNL_START;
        pTxChnl->intf = HYPHY20G_INVALID_VAL_32BIT;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifTxChnlEnable */


/*******************************************************************************
**
**  hyPhy20gPifChnlFcMap
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function map or de-map a flow control port to the
**                  specific location in Tx or Rx flow control calendar entry.
**
**                  The type of calendar would be SPI4.2 or Interlaken flow
**                  control calendar, and this function determined the 'type'
**                  by retrieving pre-configured 'mode' parameter in
**                  hyPhy20gPifInit function.
**
**                  When fcPort == 0 to 149:
**                    This function is used as per channel level operation.
**                  When fcPort == 150:
**                    This function is used as PIF interface level operation.
**
**                  When mapping:
**
**                    It will map the fcPort to the calendar address one at
**                    a time. This operation can be performed N times to
**                    map 1 fcPort to N different calendar addresses.
**
**                    For the Tx direction and fcPort == 0 to 149, it must be
**                    called prior to hyPhy20gPifTxChnlEnable (enable == 1).
**
**                  When de-mapping:
**
**                    It will search all calendar entires associated with this
**                    fcPort and remmove them by setting the entries' to NULL
**                    flow control port.
**
**                    For the Tx direction, and fcPort == 0 to 149, it must be
**                    called after hyPhy20gPifTxChnlEnable (enable == 0).
**
**
**                  This function must be called after:
**                  hyPhy20gPifFlowCtrlInit
**
**
**  INPUTS:         fileHndl  - device handle
**                  dir       - determine the flow control calendar
**                              0 - Tx flow control(flow control to HyPHY 20G)
**                              1 - Rx flow control(flow control from HyPHY 20G)
**                  enable    - 1 - mapping a channel's flow control port to
**                                  the calendar address
**                              0 - remove all calendar entires associated
**                                  with the flow control port
**                  calAddr   - calendar address that fcPort mapping to
**                              0 to 0xFF - dependent on the length of calendar
**                              Not used when enable == 0
**                  fcPort    - Flow control port to be associated with the
**                              calendar address
**                              0 to 149 - flow control port number, it's 1:1
**                                         mapping to channel number
**                              150 - Link Level Flow Control Port
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifChnlFcMap(struct file *fileHndl, UINT4 dir, UINT4 enable,
                          UINT4 calAddr, UINT4 fcPort)
{
    INT4 result;
    UINT4 pifMode;

    /* argument checking */
    if (dir > 1 || enable > 1 || fcPort > 153 || calAddr > 0xFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = pifHyPhy20gPifCtxtGetMode(fileHndl, &pifMode);
    if (result) return result;

    if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI0 ||
        pifMode == HYPHY20G_PIF_OPMODE_EXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI0 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
        pifMode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1) {
        result = hyPhy20gPifSpiFcPortMap(fileHndl, dir, enable, calAddr,
                                         fcPort);
        if(result) return result;

    } else if (pifMode == HYPHY20G_PIF_OPMODE_INTERLAKEN_INBAND ||
               pifMode == HYPHY20G_PIF_OPMODE_INTERLAKEN_OOB) {
        result = hyPhy20gPifIntlknFcPortMap(fileHndl, dir, enable, calAddr,
                                            fcPort);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifChnlFcMap */


/*******************************************************************************
**
**  hyPhy20gPifXauiInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initialize PIF top level, XAUI and SCBS
**                  slices. This function determines which XAUI and SCBS
**                  instances to initialize, by retrieving the 'mode'
**                  parameter configured by hyPhy20gPifInit function.
**
**                  This function configures the XAUI slice(s) for pseudo
**                  segment mode.
**
**                  This function will configure both XAUI slices with the same
**                  configuration when hyPhy20gPifInit was called for a mode
**                  where both XAUI slices were to be configured.
**
**                  This function will internally set PI_RD_PKT_MODE and
**                  PI_RD_LIFO_EN register bits in CPB subsystem,
**                  by calling pifHyPhy20gPifTopCfg.
**
**                  This function must be called after hyPhy20gPifSerdesInit,
**                  hyPhy20gPifRateCfg, hyPhy20gPifFlowCtrlInit and
**                  hyPhy20gCpbInit functions.
**
**                  Alternatively, this function can be called to disable the
**                  XAUI and SCBS slices.
**
**  INPUTS:         fileHndl  - device handle
**                  enable    - enable or disable XAUI slice(s)
**                              0 - disable the XAUI slice(s)
**                              1 - enable the XAUI slice(s)
**                  hdrStrip  - determine over head bytes to be stripped
**                              3 - strip 3 bytes, TYPE field will be
**                                  transmitted out
**                              4 - strip 4 bytes, TYPE field will not be
**                                  transmitted out
**                              Value not processed when enable = 0
**                  segSize   - determine the segment size for
**                              pseudo segmented mode
**                              Legal range 2 to 7, correspond to (N+1)*48 bytes
**                              Must be set to 2 for proper operation otherwise
**                              unexpected PIF FIFO Underrun events may occur.
**                              Value not processed when enable = 0
**                  schdLen0  - number of 4 entry scheduler addresses in
**                              PIF Tx Scheduler 0
**                              Legal range 1 to 128 for PIF TX scheduler
**                              Value not processed when enable = 0
**                              Value not processed when XAUI0 not used
**                  schdLen1  - number of 4 entry scheduler addresses in
**                              PIF Tx Scheduler 1
**                              Legal range 1 to 128 for PIF TX scheduler
**                              Value not processed when enable = 0
**                              Value not processed when XAUI1 not used
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 hyPhy20gPifXauiInit(struct file *fileHndl, UINT4 enable, UINT4 hdrStrip,
                         UINT4 segSize, UINT4 schdLen0, UINT4 schdLen1)
{
    INT4 result;
    UINT4 pifMode;
    UINT4 pktMode;
    UINT4 xaui0Mode;
    UINT4 xaui1Mode;

    /* argument checking */
    if (enable > 1 || schdLen0 > 128 || schdLen1 > 128 ||
        (hdrStrip != 3 && hdrStrip != 4)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = pifHyPhy20gPifCtxtGetMode(fileHndl, &pifMode);
    if (result) return result;

    /* confirm pifMode is valid before processing */
    if ((pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI || pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI) &&
        (segSize < 2 || segSize > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    switch(pifMode)
    {
        case HYPHY20G_PIF_OPMODE_EXAUI0:
        case HYPHY20G_PIF_OPMODE_EXAUI1:
        case HYPHY20G_PIF_OPMODE_DUAL_EXAUI:
            /* eXAUI mode */
            xaui0Mode = 0;
            xaui1Mode = 0;
            break;
        case HYPHY20G_PIF_OPMODE_SXAUI0:
        case HYPHY20G_PIF_OPMODE_SXAUI1:
        case HYPHY20G_PIF_OPMODE_DUAL_SXAUI:
            /* standard XAUI mode */
            xaui0Mode = 1;
            xaui1Mode = 1;
            break;
        case HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1:
            xaui0Mode = 0;
            xaui1Mode = 1;
            break;
        case HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1:
            xaui0Mode = 1;
            xaui1Mode = 0;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_MODE;
    }

    /* disable operations */
    if (enable == HYPHY20G_DISABLE) {
        /* disable eXAUI slice 0 */
        if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI0 ||
            pifMode == HYPHY20G_PIF_OPMODE_SXAUI0 ||
            pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
            pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
            pifMode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
            pifMode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1) {
            result = pifHyPhy20gPifXauiInit(fileHndl, 0, HYPHY20G_DISABLE,
                                            xaui0Mode);
            if (result) return result;
        }

        /* disable eXAUI slice 1 */
        if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI1 ||
            pifMode == HYPHY20G_PIF_OPMODE_SXAUI1 ||
            pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
            pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
            pifMode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
            pifMode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1) {
            result = pifHyPhy20gPifXauiInit(fileHndl, 1, HYPHY20G_DISABLE,
                                            xaui1Mode);
            if (result) return result;
        }

        /* function operation complete - exit */
        return HYPHY20G_SUCCESS;
    }

    /*
     * proceed with PIF top level initialization and eXAUI, SCBS slices enabling
     */
    /* configure pseudo segment mode, strip and segment size for PIF top level */
    pktMode = 0x3;
    result = pifHyPhy20gPifTopCfg(fileHndl, hdrStrip, pktMode, segSize);
    if (result) return result;

    /* enable eXAUI and SCBS slices */
    if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI0 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI0 ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
        pifMode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1) {
        HYPHY20G_ASSERT((schdLen0 > 0), HYPHY20G_ERR_INVALID_ARG,
                        "HYPHY20G_ERR_INVALID_ARG, schdLen0 is %ld", schdLen0);

        /* XAUI slice 0 Initialization */
        result = pifHyPhy20gPifXauiInit(fileHndl, 0, HYPHY20G_ENABLE, xaui0Mode);
        if (result) return result;

        /* SCBS Initialization */
        result = pifHyPhy20gPifTxSchdInit(fileHndl, HYPHY20G_PIF_TX_SCHD_0,
                                          schdLen0, HYPHY20G_ENABLE, 0);
        if (result) return result;
    }

    if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
        pifMode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1) {
        HYPHY20G_ASSERT((schdLen1 > 0), HYPHY20G_ERR_INVALID_ARG,
                        "HYPHY20G_ERR_INVALID_ARG, schdLen1 is %ld", schdLen1);

        /* eXAUI slice 1 Initialization */
        result = pifHyPhy20gPifXauiInit(fileHndl, 1, HYPHY20G_ENABLE, xaui1Mode);
        if (result) return result;

        /* SCBS Initialization */
        result = pifHyPhy20gPifTxSchdInit(fileHndl, HYPHY20G_PIF_TX_SCHD_1,
                                          schdLen1, HYPHY20G_ENABLE, 0);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifXauiInit */

/*******************************************************************************
**
**  hyPhy20gPifXauiRxPadCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configures RX_PAD_BYTE in eXAUI slice to determine how many
**                  pad bytes to be inserted.
**
**                  If the NP/FPGA has provided:
**                      1. no extra padding as a header, set RX_PAD_BYTE to 0x0
**                         and PI_WR_TYPE_PRESENT to 0.
**                      2. 1-byte type field prepended header, set RX_PAD_BYTE
**                         to 0x3 and PI_WR_TYPE_PRESENT to 1.
**                      3. 4-bytes header including type field, set RX_PAD_BYTE
**                         to 0x0 and PI_WR_TYPE_PRESENT to 1.
**
**                  PI_WR_TYPE_PRESENT is a per channel register bit in the CPB,
**                  and it can be configured by 'pifSysIfType' parameter of
**                  hyPhy20gCpbChnlProv function.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - determine which slice to be configured
**                              0 - eXAUI0
**                              1 - eXAUI1
**                  rxPadEn   - 0 - No pad byte to be inserted (eXAUI transfers
**                                  without TYPE/DUMMY field)
**                              1 - 3 pad bytes to be inserted (eXAUI transfers
**                                  without TYPE/DUMMY field)
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifXauiRxPadCfg(struct file *fileHndl, UINT4 slice, UINT4 rxPadEn)
{
    INT4 result;
    UINT4 addr;

    /* argument checking */
    if (slice > 1 || rxPadEn > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_XAUI_TOP_REG_HDR_CFG;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_XAUI_TOP_BIT_HDR_CFG_RX_PAD_BYTE_MSK,
                (rxPadEn == 0 ? 0 : 3));
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifXauiRxPadCfg */

/*******************************************************************************
**
**  hyPhy20gPifStdXauiCpbChnlCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function specifies the channel within the CPB - PIF
**                  write interface that XAUI traffic will be transfered to.
**                  The CPB FIFO depth status for this channel will contribute
**                  to pause frame generation.
**
**                  This function is only valid for PIF subsystem configured in
**                  Standard XAUI mode of operation.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - slice to configure (standard XAUI mode)
**                              0 - XAUI0
**                              1 - XAUI1
**                  cpbChnlId - CPB PIF write channel ID (relative to CPB
**                              channel numbering of 0 to 293)
**                              Legal range 147 to 293. (PIF write channels 0
**                              to 146)
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gPifStdXauiCpbChnlCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 cpbChnlId)
{
    INT4 result;
    UINT4 addr, msk, val;
    UINT4 rxUsrChId;

    /* argument checking */
    if (slice > 1 || cpbChnlId < 147 || cpbChnlId > 293)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine RX_USR_CH_ID with respect to XAUI */
    rxUsrChId = cpbChnlId - HYPHY20G_CPB_PIF_PORT_OFFSET;

    addr = HYPHY20G_XAUI_TOP_REG_USR_CHNL_CFG;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = hyPhy20gRegFieldWrite (fileHndl, addr,
                            HYPHY20G_XAUI_TOP_BIT_USR_CHNL_CFG_RX_USR_CH_ID_MSK,
                            rxUsrChId);
    if (result) return result;

    /* place FC_CTRLR_RST into reset before setting XAUIn_FC_ALIAS_EN and
       XAUIn_FC_ALIAS bits */
    addr = HYPHY20G_PIF_PIF_REG_SW_RST;
    msk = HYPHY20G_PIF_PIF_BIT_SW_RST_FC_CTRLR_RST_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if (result) return result;

    addr = HYPHY20G_PIF_PIF_REG_FLOW_CTL_ALIAS;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    /* configure XAUIn_FC_ALIAS */
    msk = (slice == 0) ? HYPHY20G_PIF_PIF_BIT_FLOW_CTL_ALIAS_XAUI0_FC_ALIAS_MSK:
                         HYPHY20G_PIF_PIF_BIT_FLOW_CTL_ALIAS_XAUI1_FC_ALIAS_MSK;
    result = hyPhy20gVarFieldWrite(&val, msk, rxUsrChId);
    if (result) return result;

    /* enable flow control */
    msk = (slice == 0) ? HYPHY20G_PIF_PIF_BIT_FLOW_CTL_ALIAS_XAUI0_FC_ALIAS_EN_MSK:
                         HYPHY20G_PIF_PIF_BIT_FLOW_CTL_ALIAS_XAUI1_FC_ALIAS_EN_MSK;
    result = hyPhy20gVarFieldWrite(&val, msk, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* remove FC_CTRLR_RST reset */
    addr = HYPHY20G_PIF_PIF_REG_SW_RST;
    msk = HYPHY20G_PIF_PIF_BIT_SW_RST_FC_CTRLR_RST_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifStdXauiCpbChnlCfg */


/*******************************************************************************
**
**  hyPhy20gPifStdXauiPauseCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures parameters related to MAC PAUSE 
**                  control. 
**                  
**                  txPauseTime and txPauseInterval control 'PAUSETimerField'
**                  value in the pause frames generated by TXXG, and the 
**                  frequency of pause frame generation.
**                  
**                  txPauseQuantum and txPauseQuantumEn control the pause quanta 
**                  value, which is used by TXXG to decrease local pause timer
**                  that is loaded from ingress pause frames.
**
**                  Used for standard XAUI mode only.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - slice to configure (standard XAUI mode)
**                              0 - XAUI0
**                              1 - XAUI1
**                  txPauseTime 
**                            - 16-bit 'PAUSETimerField' of PAUSE frames
**                              generated by TXXG in this PIF XAUI slice
**                  txPauseInterval
**                            - 16-bit Tx PAUSE timer interval, a value of
**                              0 results in PAUSE frames being generated
**                              most frequently and 0xFFFF least frequently
**                  txPauseQuantum
**                            - determines the number of pause "cycle" that  
**                              TXXG should pause:
**                              0x00 - 1   cycles
**                              0x01 - 2   cycles
**                              ......
**                              0xFF - 256 cycles
**                  txPauseQuantumEn
**                            - determines the length of 1 pause "cycle":
**                              0 - sysclk*2 cycles
**                              1 - 33 sysclk*2 cycles 
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifStdXauiPauseCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 txPauseTime,UINT4 txPauseInterval,
                                UINT4 txPauseQuantum, UINT4 txPauseQuantumEn)
{
    INT4 result;
    UINT4 addr;

    /* argument checking */
    if (slice > 1 || 
        txPauseTime > 0xFFFF     || 
        txPauseInterval > 0xFFFF ||
        txPauseQuantum > 0xFF    ||
        txPauseQuantumEn > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Write pause time */
    addr = HYPHY20G_XAUI_TXXG_REG_PAUSE_TMR;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_XAUI_TXXG_BIT_PAUSE_TMR_PAUSE_TIME_MSK,
                txPauseTime);
    if(result) return result;

    /* Write pause interval */
    addr = HYPHY20G_XAUI_TXXG_REG_PAUSE_TMR_INTRVL;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_XAUI_TXXG_BIT_PAUSE_TMR_INTRVL_PAUSE_IVAL_MSK,
                txPauseInterval);
    if(result) return result;

    /* PAUSE Quantum Config */
    addr = HYPHY20G_XAUI_TXXG_REG_PAUSE_QNTM_VAL_CFG;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_XAUI_TXXG_BIT_PAUSE_QNTM_VAL_CFG_FC_PAUSE_QNTM_MSK,
                txPauseQuantum);
    if(result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_XAUI_TXXG_BIT_PAUSE_QNTM_VAL_CFG_FC_PHY_PACE_EN_OFF,
                txPauseQuantumEn);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifStdXauiPauseCfg */

/*******************************************************************************
**
**  hyPhy20gPifXauiSetLoopback
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures loopback mode for a eXAUI slice.
**
**
**
**  INPUTS:         fileHndl  - device handle
**                  instance  - determine which eXAUI slice to be configured
**                              0 - eXAUI0
**                              1 - eXAUI1
**                  loopType  - select loop type:
**                              0 - HYPHY20G_PIF_XAUI_LINE_LOOP
**                              1 - HYPHY20G_PIF_XAUI_SYS_LOOP
**                              2 - HYPHY20G_PIF_XAUI_NO_LOOP
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifXauiSetLoopback(struct file *fileHndl, UINT4 slice,
                                UINT4 loopType)
{
    INT4 result;
    UINT4 addr;

    /* argument checking */
    if (slice > 1 || loopType > HYPHY20G_PIF_XAUI_NO_LOOP) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(HYPHY20G_XAUI_TOP_REG_LPBK,
                                               slice);
    switch(loopType)
    {
        case HYPHY20G_PIF_XAUI_LINE_LOOP:
            result = sysHyPhy20gBitWrite(fileHndl, addr,
                        HYPHY20G_XAUI_TOP_BIT_LPBK_LINE_LOOP_BACK_OFF, 1);
            if (result) return result;
            break;
        case HYPHY20G_PIF_XAUI_SYS_LOOP:
            result = sysHyPhy20gBitWrite(fileHndl, addr,
                        HYPHY20G_XAUI_TOP_BIT_LPBK_SYS_LOOP_BACK_OFF, 1);
            if (result) return result;
            break;
        case HYPHY20G_PIF_XAUI_NO_LOOP:
            result = sysHyPhy20gBitWrite(fileHndl, addr,
                        HYPHY20G_XAUI_TOP_BIT_LPBK_SYS_LOOP_BACK_OFF, 0);
            if (result) return result;
            result = sysHyPhy20gBitWrite(fileHndl, addr,
                        HYPHY20G_XAUI_TOP_BIT_LPBK_LINE_LOOP_BACK_OFF, 0);
            if (result) return result;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifXauiSetLoopback */


/*******************************************************************************
**
**  hyPhy20gPifSpiFcCalCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures parity scheme, DIP-2 insertion and
**                  number of iterations for the SPI4.2 flow control calendar.
**
**                  This function is only valid for SPI4.2 flow control mode.
**
**  INPUTS:         fileHndl      - device handle
**                  dir           - 0 - Transmit Flow Control calendar
**                                  1 - Receive Flow Control calendar
**                  parity        - specify data protection parity scheme
**                                  0 - Odd parity
**                                  1 - Even parity
**                  dip2En        - enable/disable DIP-2 insertion
**                                  0 - disable DIP-2 protection
**                                  1 - enable DIP-2 protection
**                  iterationCnt  - 0x0 - Check for DIP-2 after 256 cycles
**                                  0x1 - Check for DIP-2 after each cycle
**                                  0xn - Check for DIP-2 after n cycles
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifSpiFcCalCfg(struct file *fileHndl, UINT4 dir, UINT4 parity,
                            UINT4 dip2En, UINT4 iterationCnt)
{
    INT4 result;

    /* argument checking */
    if (dir > 1 || parity > 1 || dip2En > 1 ||
        iterationCnt > 0xFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = pifHyPhy20gPifSpiFcCalCfg(fileHndl, dir, parity,
                                       dip2En, iterationCnt, 0, 0xE);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifSpiFcCalCfg */


/*******************************************************************************
**
**  hyPhy20gPifIntlknFlexInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    An enhanced API for hyPhy20gPifIntlknInit.
**                  This function configure the INTERLAKEN instance to
**                  transition between 'Major Bidirectional States' of:
**                  1.  Block in reset with all registers in default values
**                  2.  Block in reset with configuration registers maintaining
**                      their last initialized value
**
**                  This function will internally set PI_RD_PKT_MODE and
**                  PI_RD_LIFO_EN register bits in CPB subsystem,
**                  by calling pifHyPhy20gPifTopCfg.
**
**                  This function must be used after: hyPhy20gPifFlowCtrlInit
**                  hyPhy20gCpbInit, hyPhy20gPifSerdesInit and
**                  hyPhy20gPifRateCfg functions.
**
**                  This function must be called after hyPhy20gPifInit when
**                  the operation 'mode' of PIF subsystem is equal to 4 or 5.
**
**                  Setting enable = 1 moves from state 1 to 2.  Setting
**                  enable = 0 allows an interface to move from state 2 or 3 to
**                  state 1.
**
**                  When entering state 2, the INTERLAKEN interface will be
**                  initialized with a BURST_MAX of 256 bytes and a metaframe
**                  period of 2048 words.
**
**
**  INPUTS:         fileHndl    - device handle
**                  mode        - determine which mode the PIF operates on
**                                0 - disable the Interlaken block
**                                1 - packet mode
**                                2 - segmented mode
**                  lanes         Low 8 bit stands for 8 lanes
**                                bit 0 ~ bit 8: lane 0~ lane 7
**                  schdLen     - number of 4 entry scheduler addresses in
**                                PIF Tx Scheduler 0
**                                Legal range 1 to 128 for PIF TX scheduler
**                                Value not processed when mode = 0
**                  trafficType - traffic type flowing across Interlaken
**                                interface.  
**                                1 - 2 x 10G POS special case for packet mode
**                                    where interface limited to transfer 2 
**                                    10G rate channels and ensure no data hit.
**                                    Valid for mode = 0, 1, 2.
**                                0 - Standard
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 hyPhy20gPifIntlknFlexInit(struct file *fileHndl, UINT4 mode,UINT4 lanes,
                           UINT4 schdLen, UINT4 trafficType)
{
    UINT4 devId, revId, status;
    INT4 result;
    UINT4 rstReg, rstRegData, intlknRstOff;
    UINT4 pktMode = 0;
    UINT4 shimMode = 0;
    UINT4 maxDisMsk, minDisMsk, scbsEnbl;
    UINT4 val, burstCfgAddr;
    const UINT4 slice = HYPHY20G_INTLKN_PIF_INTLKN;

    /* argument checking */
    if (mode > 2 || schdLen < 1 || schdLen > 128 || trafficType > 1 ||
       (trafficType == 1 && mode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    rstReg = HYPHY20G_PIF_PIF_REG_SW_RST;
    intlknRstOff = HYPHY20G_PIF_PIF_BIT_SW_RST_INTLKN_RST_OFF;
    result = sysHyPhy20gRead(fileHndl, rstReg, &rstRegData);
    if(result) return result;

    /* if top level Interlaken reset bit set to 0,
       put Interlaken core into reset */
    if ((rstRegData & (1 << intlknRstOff)) == 0) {
	    /* Interlaken Core Reset bits */
	    result = utilHyPhy20gIntlknCoreEnable(fileHndl, slice,
	                                          HYPHY20G_INTLKN_STATE_INRESET);
	    if(result) return result;
    }

    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_PIF_PIF_REG_IF_EN,
                        HYPHY20G_PIF_PIF_BIT_IF_EN_EN_INTERLAKEN_OFF, 0);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, rstReg, intlknRstOff, 1);
    if (result) return result;

    /* function operation complete if disabling interface - exit */
    if (mode == HYPHY20G_DISABLE) {
        /* function disable operation complete - exit */
        return HYPHY20G_SUCCESS;
    }

    /*
     * proceed with enabling and initialization of block
     */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_PIF_PIF_REG_IF_EN,
                        HYPHY20G_PIF_PIF_BIT_IF_EN_EN_INTERLAKEN_OFF, 1);
    if (result) return result;

    /* remove the Interlaken TSB reset */
    result = sysHyPhy20gBitWrite(fileHndl, rstReg, intlknRstOff, 0);
    if (result) return result;

    /* select mode and interface, and configure the segment size */
    /* evaluation of compatibility between SW driver and device */
    result = hyPhy20gCheckRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    switch (mode) {
		case 0: // disable
			return HYPHY20G_SUCCESS;
			break;
        case 1:
            /* packet mode */
            pktMode = 1;
            lanes       = (~lanes)&0xff;
            shimMode    = 1;
            break;
        case 2:
            /* segmented mode */
            pktMode     = 2;
            lanes       = (~lanes)&0xff;
            shimMode    = 0;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
			return HYPHY20G_ERR_INVALID_ARG;
    }

    /* For segment mode, set SEG_SIZE to b'011 (256 bytes) to match the
       Interlaken BURSTMAX setting;
       For PACKET mode, SEG_SIZE is not used.
       For Interlaken used in PIF, hdrStrip always set to 0x0 */
    result = pifHyPhy20gPifTopCfg(fileHndl, 0, pktMode, 0x3);
    if (result) return result;

    /* for 2 x 10G POS packet mode operation PI_RD_LIFO_EN must be set to 0 */
    if(trafficType == 1){
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_PAC_IF_MISC_CFG,
                         HYPHY20G_CPB_CPB_BIT_PAC_IF_MISC_CFG_PI_RD_LIFO_EN_OFF,
                         0);
        if (result) return result;        
    }

    /*
     * INTERLAKEN initialization
     */
    /* Perform Shim Level initialization */
    result = utilHyPhy20gIntlknShimCfg(fileHndl, slice, shimMode);
    if(result) return result;

    /* Put Interlaken in 'Major Bidirectional State' of 2 */
    result = utilHyPhy20gIntlknCoreEnable(fileHndl, slice,
                                          HYPHY20G_INTLKN_STATE_STANDBY);
    if(result) return result;

    /* apply initial Interlaken Core configuration */
    result = utilHyPhy20gIntlknCoreCfg(fileHndl, slice);
    if(result) return result;

    /* enable lanes */
    result = hyPhy20gPifLaneDisable(fileHndl, lanes);
    if (result) return result;

    /* SCBS Initialization */
    scbsEnbl = (trafficType == 1) ? 2:1;
    result = pifHyPhy20gPifTxSchdInit(fileHndl, HYPHY20G_PIF_TX_SCHD_0,
                                      schdLen, scbsEnbl, 1);
    if (result) return result;
    


    /* Non-default setting of TX and RX MIN_DIS/MAX_DIS fields */
    
    /* Extract BMAX_CNT and BMIN_CNT data */
    maxDisMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_BRST_CFG_MAX_DIS_MSK;
    minDisMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_BRST_CFG_MIN_DIS_MSK;

    /* Configure TX direction */
    burstCfgAddr = HYPHY20G_PIF_INTLKN_CORE_REG_BRST_CFG_OFFSET(0);
    result = sysHyPhy20gRead(fileHndl, burstCfgAddr, &val);
    if (result) return result;

    /*set Tx MAX_DIS and MIN_DIS to 1 */
    result = hyPhy20gVarFieldWrite(&val, maxDisMsk, 1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&val, minDisMsk, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, burstCfgAddr, val);
    if (result) return result;

    /* Configure RX direction */
    burstCfgAddr = HYPHY20G_PIF_INTLKN_CORE_REG_BRST_CFG_OFFSET(1);
    result = sysHyPhy20gRead(fileHndl, burstCfgAddr, &val);
    if (result) return result;

    /*set Rx MAX_DIS and MIN_DIS to 0 */
    result = hyPhy20gVarFieldWrite(&val, maxDisMsk, 0);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&val, minDisMsk, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, burstCfgAddr, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifIntlknInit */

/*******************************************************************************
**
**  hyPhy20gPifIntlknInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    A flexible version of hyPhy20gPifIntlknInit.
**                  This function configure the INTERLAKEN instance to
**                  transition between 'Major Bidirectional States' of:
**                  1.  Block in reset with all registers in default values
**                  2.  Block in reset with configuration registers maintaining
**                      their last initialized value
**
**                  This function will internally set PI_RD_PKT_MODE and
**                  PI_RD_LIFO_EN register bits in CPB subsystem,
**                  by calling pifHyPhy20gPifTopCfg.
**
**                  This function must be used after: hyPhy20gPifFlowCtrlInit
**                  hyPhy20gCpbInit, hyPhy20gPifSerdesInit and
**                  hyPhy20gPifRateCfg functions.
**
**                  This function must be called after hyPhy20gPifInit when
**                  the operation 'mode' of PIF subsystem is equal to 4 or 5.
**
**                  Setting enable = 1 moves from state 1 to 2.  Setting
**                  enable = 0 allows an interface to move from state 2 or 3 to
**                  state 1.
**
**                  When entering state 2, the INTERLAKEN interface will be
**                  initialized with a BURST_MAX of 256 bytes and a metaframe
**                  period of 2048 words.
**
**
**  INPUTS:         fileHndl    - device handle
**                  mode        - determine which mode the PIF operates on
**                                0 - disable the Interlaken block
**                                1 - packet mode with 5 lanes
**                                2 - packet mode with 8 lanes
**                                3 - segmented mode with 5 lanes
**                                4 - segmented mode with 8 lanes
**                                other value 
**                                    - bit[15:8]:bit15~8 for lane 7~0.
**                                      1 - enable; 
**                                      0 - disable
**                                    - bit[1:0]: 
**                                      0 - same as mode = 0;
**                                      1 - packet mode;
**                                      2 - segmented mode
**                  schdLen     - number of 4 entry scheduler addresses in
**                                PIF Tx Scheduler 0
**                                Legal range 1 to 128 for PIF TX scheduler
**                                Value not processed when mode = 0
**                  trafficType - traffic type flowing across Interlaken
**                                interface.  
**                                1 - 2 x 10G POS special case for packet mode
**                                    where interface limited to transfer 2 
**                                    10G rate channels and ensure no data hit.
**                                    Valid for mode = 0, 1, 2.
**                                0 - Standard
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/

INT4 hyPhy20gPifIntlknInit(struct file *fileHndl, UINT4 mode, UINT4 schdLen, 
                           UINT4 trafficType)
{
	INT4 result;
    UINT4 usedLanes = 0x00;
	UINT4 modeType;
	
    /* argument checking */
	if(((mode>>8)&0xff) == 0) { 
	    if (mode > 4 || schdLen < 1 || schdLen > 128 || trafficType > 1 ||
	       (trafficType == 1 && mode > 2)) {
	        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
	        return HYPHY20G_ERR_INVALID_ARG;
	    }
	}

	switch (mode) {
		case 0:
			modeType = 0;
			usedLanes = 0x00;
			break;
		case 1:
			/* packet mode with 5 lanes */
			usedLanes		= 0x1F;
			modeType = 1;
			break;
		case 2:
			/* packet mode with 8 lanes */
			usedLanes		= 0xFF;
			modeType = 1;
			break;
		case 3:
			/* segmented mode with 5 lanes */
			usedLanes		= 0x1F;
			modeType = 2;
			break;
		case 4:
			/* segmented mode with 8 lanes */
			usedLanes		= 0xFF;
			modeType = 2;
			break;
		default:
			modeType = mode&0x03;
			usedLanes = (mode>>8)&0xff;
			break;
	}

    result = hyPhy20gPifIntlknFlexInit(fileHndl,modeType,usedLanes,schdLen,trafficType);
	if (result) return result;

	return HYPHY20G_SUCCESS;
}



/*******************************************************************************
**
**  hyPhy20gPifIntlknTxRateLimitCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configure the optional transmit rate limiting
**                  feature for one of the INTERLAKEN instance.
**
**                  The INTERLAKEN block must be 'Major Bidirectional States'
**                  of 2 in order for this function to be executed, otherwise
**                  HYPHY20G_ERR_INVALID_STATE will be returned.
**
**
**
**  INPUTS:         fileHndl   - device handle
**                  rateLimit  - 0 - disabled no rate limiting applied
**                               1 to 50 - amount of rate limiting applied in
**                               1 GBPS increments
**                  burstLimit - BurstLimit Parameter setting as described in
**                               Interlaken Protocol Definition.  'The maxiumum
**                               quantity of data that may be burst across the
**                               interface before invoking the rate limiting
**                               logic (bytes)'
**                  byteGran   - Number of credits that are to be subtracted
**                               from the token bucket when an EOP is
**                               encountered.
**                               0 - 1 byte
**                               1 - 2 bytes
**                               2 - 4 bytes
**                               3 - 8 bytes
**                               4 - 16 bytes
**                               5 - 32 bytes
****
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifIntlknTxRateLimitCfg(struct file *fileHndl, UINT4 rateLimit,
                                     UINT4 burstLimit, UINT4 byteGran)
{
    INT4 result;
    UINT4 state;
    const UINT4 slice = HYPHY20G_INTLKN_PIF_INTLKN;

    /* argument checking */
    if (rateLimit > 50 || byteGran > 5) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve major bidirectional state */
    result = utilHyPhy20gIntlknCoreGetMajBiDirState(fileHndl, slice, &state);
    if (result) return result;

    /* confirm major bidirection state of 2 or return invalid state error */
    if (state != HYPHY20G_INTLKN_STATE_STANDBY) {
        return HYPHY20G_ERR_INVALID_STATE;
    }

    result = utilHyPhy20gIntlknCoreTxRateLimitCfg(fileHndl, slice, rateLimit,
                                                  burstLimit, byteGran);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifIntlknTxRateLimitCfg */


/*******************************************************************************
**
**  hyPhy20gPifIntlknEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configure the INTERLAKEN instance to
**                  transition between 'Major Bidirectional States' of:
**                  2.  Block in reset with configuration registers maintaining
**                      their last initialized value
**                  3.  Block in operational state with the ability to send/
**                      receive user data or SERDES test patterns.
**
**                  Setting enable = 1 moves from state 2 to 3.  Setting
**                  enable = 0 allows an interface to move from state 3 to
**                  state 2.
**
**                  It is assumed that the CBRI instance is in state 2 when
**                  calling this function with enable = 1 and in state 3 when
**                  calling this function with enable = 0.  If these
**                  relationships are not trun HYPHY20G_ERR_INVALID_STATE is
**                  returned.
**
**  INPUTS:         fileHndl  - device handle
**                  enable    - enable/disable flag
**                              0 - disable
**                              1 - enable
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifIntlknEnable(struct file *fileHndl, UINT4 enable)
{
    INT4 result;
    UINT4 state, newState;
    const UINT4 slice = HYPHY20G_INTLKN_PIF_INTLKN;

    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine desired state */
    newState = (enable==0) ?
              HYPHY20G_INTLKN_STATE_STANDBY : HYPHY20G_INTLKN_STATE_OPERATIONAL;

    /* retrieve major bidirectional state */
    result = utilHyPhy20gIntlknCoreGetMajBiDirState(fileHndl, slice, &state);
    if(result) return result;

    /* determine if valid state transition */
    if(((newState == HYPHY20G_INTLKN_STATE_STANDBY)&&
       (state == HYPHY20G_INTLKN_STATE_OPERATIONAL))||
       ((newState == HYPHY20G_INTLKN_STATE_OPERATIONAL)&&
       (state == HYPHY20G_INTLKN_STATE_STANDBY))){
        /* Configure Interlaken Core Reset bits */
        result = utilHyPhy20gIntlknCoreEnable(fileHndl, slice, newState);
    } else {
        return HYPHY20G_ERR_INVALID_STATE;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifIntlknEnable */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/*******************************************************************************
**
**  pifHyPhy20gPifCreateCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates and intializes memory required to operate
**                  the PIF subsystem.
**
**                  References this memory to the device level structure
**                  pointed to with *pDevCtxt.
**
**                  Function pifHyPhy20gPifDestroyCtxt recovers this memory and
**                  set the device level reference to NULL.
**
**
**  INPUTS:         fileHndl  - device handle
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gPifCreateCtxt(struct file *fileHndl, UINT4 pifMode)
{
    INT4 result;
    UINT4 size;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_PIF_CTXT *pPifCtxt = NULL;
    sHYPHY20G_SCHD_CTXT *pSchdCtxt;

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* If pPifCtxt is not a NULL pointer,
       destroy it first to prevent memory leak */
    if (pDevCtxt->pPifCtxt != NULL) {
        pifHyPhy20gPifDestroyCtxt(fileHndl, 0);
    }

    /* allocate memory for and associate with pDevCtxt->pPifCtxt */
    size = sizeof(sHYPHY20G_PIF_CTXT);
    pDevCtxt->pPifCtxt = (sHYPHY20G_PIF_CTXT *) sysHyPhy20gMemAlloc(size);

    if(pDevCtxt->pPifCtxt == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    DBG_PRINT("PIF Context memory allocated:addr=0x%lx size=%ld\n",
              (UINT4)pPifCtxt, size);

    pPifCtxt = pDevCtxt->pPifCtxt;

    pPifCtxt->pFcRxCal = (sHYPHY20G_PIF_FC_CAL *)
                        sysHyPhy20gMemAlloc(sizeof(sHYPHY20G_PIF_FC_CAL));
    if (pPifCtxt->pFcRxCal == NULL) {
        DBG_PRINT("%s, %s, %d, sysHyPhy20gMemAlloc failed\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        pifHyPhy20gPifDestroyCtxt(fileHndl, 1);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pPifCtxt->pFcTxCal = (sHYPHY20G_PIF_FC_CAL *)
                        sysHyPhy20gMemAlloc(sizeof(sHYPHY20G_PIF_FC_CAL));
    if (pPifCtxt->pFcTxCal == NULL) {
        DBG_PRINT("%s, %s, %d, sysHyPhy20gMemAlloc failed\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        pifHyPhy20gPifDestroyCtxt(fileHndl, 1);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pSchdCtxt = (sHYPHY20G_SCHD_CTXT *)
                        sysHyPhy20gMemAlloc(sizeof(sHYPHY20G_SCHD_CTXT));
    if (pSchdCtxt == NULL) {
        DBG_PRINT("%s, %s, %d, sysHyPhy20gMemAlloc failed\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        pifHyPhy20gPifDestroyCtxt(fileHndl, 1);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* based on pifMode value, allocate memory for Tx Scheduler,
       and Tx/Rx flow control calendar context */
    if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI0 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI0 ||
        pifMode == HYPHY20G_PIF_OPMODE_INTERLAKEN_INBAND ||
        pifMode == HYPHY20G_PIF_OPMODE_INTERLAKEN_OOB) {
        pPifCtxt->pTxSchdCtxt0 = pSchdCtxt;
        pPifCtxt->pTxSchdCtxt1 = NULL;

    } else if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI1 ||
               pifMode == HYPHY20G_PIF_OPMODE_SXAUI1) {
        pPifCtxt->pTxSchdCtxt0 = NULL;
        pPifCtxt->pTxSchdCtxt1 = pSchdCtxt;

    } else if (pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
               pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
               pifMode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
               pifMode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1) {
        pPifCtxt->pTxSchdCtxt0 = pSchdCtxt;

        pSchdCtxt = (sHYPHY20G_SCHD_CTXT *)
                            sysHyPhy20gMemAlloc(sizeof(sHYPHY20G_SCHD_CTXT));
        if (pSchdCtxt == NULL) {
            DBG_PRINT("%s, %s, %d, sysHyPhy20gMemAlloc failed\n", 
                        __FILE__, __FUNCTION__, __LINE__);
            pifHyPhy20gPifDestroyCtxt(fileHndl, 1);
            return HYPHY20G_ERR_MEM_ALLOC;
        }
        pPifCtxt->pTxSchdCtxt1 = pSchdCtxt;
    }

    //DBG_PRINT("%s %d memory add: 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx\n",__FUNCTION__, __LINE__, pPifCtxt->pTxSchdCtxt0, pPifCtxt->pTxSchdCtxt1, pPifCtxt->pExaui0, pPifCtxt->pExaui1, pPifCtxt->pFcRxCal, pPifCtxt->pFcTxCal);    
    DBG_PRINT("%s %d memory size: sizeof(sHYPHY20G_SCHD_CTXT)=%d, sizeof(sHYPHY20G_PIF_FC_CAL)=%d\n",__FUNCTION__, __LINE__, sizeof(sHYPHY20G_SCHD_CTXT), sizeof(sHYPHY20G_PIF_FC_CAL));
    DBG_PRINT("%s %d memory add: %p\n %p\n %p\n %p\n",__FUNCTION__, __LINE__, 
              pPifCtxt->pFcRxCal, pPifCtxt->pFcTxCal, 
              pPifCtxt->pTxSchdCtxt0, pPifCtxt->pTxSchdCtxt1);

    /* initialize memories */
    pPifCtxt->valid = HYPHY20G_VALID_SIGN;
    pPifCtxt->mode = pifMode;
    result = pifHyPhy20gPifInitCtxt(fileHndl, pPifCtxt);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* pifHyPhy20gPifCreateCtxt */



/*******************************************************************************
**
**  pifHyPhy20gPifDestroyCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset context of the PIF subsystem.
**
**
**  INPUTS:         fileHndl  - device handle
**                  scope     - Scope of scheduler destroy operation
**                              0 - conditionally destroy scheduler by applying
**                                  reset configuration in addition to context
**                                  destrutction
**                              1 - unconditionally destroy regardless of
**                                  device state
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 pifHyPhy20gPifDestroyCtxt(struct file *fileHndl, UINT4 scope)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_PIF_CTXT *pPifCtxt;
    UINT4 pifMode;
    INT4 result;
    
    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pPifCtxt = pDevCtxt->pPifCtxt;
    if (pDevCtxt->pPifCtxt == NULL) 
    {
        /* Nothing to destroy */
        DBG_PRINT("%s, %s, %d, returning success as already NULL\n",__FILE__, __FUNCTION__, __LINE__);
    	return HYPHY20G_SUCCESS;
    }

    /* recover the PIF operating mode from the context memory */
    pifMode = pDevCtxt->pPifCtxt->mode;

    if (scope == 0) { /* conditionally destroy context */
        /* destroy scheduler related structures */
        if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI0 ||
            pifMode == HYPHY20G_PIF_OPMODE_SXAUI0 ||
            pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
            pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
            pifMode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
            pifMode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1 ||
            pifMode == HYPHY20G_PIF_OPMODE_INTERLAKEN_INBAND ||
            pifMode == HYPHY20G_PIF_OPMODE_INTERLAKEN_OOB) {
            /* disable schd 0 */
            result = pifHyPhy20gPifTxSchdInit(fileHndl, HYPHY20G_PIF_TX_SCHD_0, 0, HYPHY20G_DISABLE, 1);
            if(result){
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);                
                return result;
            }          
        }
    
        if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI1 ||
            pifMode == HYPHY20G_PIF_OPMODE_SXAUI1 ||
            pifMode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
            pifMode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1 ||
            pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
            pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI) {
            /* disable schd 1 */
            result = pifHyPhy20gPifTxSchdInit(fileHndl, HYPHY20G_PIF_TX_SCHD_1, 0, HYPHY20G_DISABLE, 1);
            if(result){
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);                
                return result;
            }          
        }
        
    } else { /* unconditionally destroy context */
        /* Destroy Scheduler context for pTxSchdCtxt0 if not NULL */
        if (pPifCtxt->pTxSchdCtxt0 != NULL) {
            if ((pPifCtxt->pTxSchdCtxt0->pScbsGrpCtxt != NULL) || 
                (pPifCtxt->pTxSchdCtxt0->pOnLineCalMem != NULL) ||
                (pPifCtxt->pTxSchdCtxt0->pOffLineCalMem != NULL)) {
                result = utilHyPhy20gScbsDestroySchd(pPifCtxt->pTxSchdCtxt0);
                DBG_PRINT("%s, %s, %d, Scheduler context destroyed for pTxSchdCtxt0\n",__FILE__, __FUNCTION__, __LINE__); 
                if (result) return result;
            } 
        } 
        /* Destroy Scheduler context for pTxSchdCtxt1 if not NULL */  
        if (pPifCtxt->pTxSchdCtxt1 != NULL) {
            if ((pPifCtxt->pTxSchdCtxt1->pScbsGrpCtxt != NULL) || 
                (pPifCtxt->pTxSchdCtxt1->pOnLineCalMem != NULL) ||
                (pPifCtxt->pTxSchdCtxt1->pOffLineCalMem != NULL)) {
                result = utilHyPhy20gScbsDestroySchd(pPifCtxt->pTxSchdCtxt1);
                DBG_PRINT("%s, %s, %d, Scheduler context destroyed for pTxSchdCtxt1\n",__FILE__, __FUNCTION__, __LINE__); 
                if (result) return result;
            } 
        }  
    }

    /* destroy memory allocated for *pFcRxCal, *pFcTxCal,
       *pTxSchdCtxt0 and pTxSchdCtxt1 */
    if (pPifCtxt->pFcRxCal != NULL) {
        sysHyPhy20gMemFree(pPifCtxt->pFcRxCal);
        DBG_PRINT("%s, %s, %d, returning success after pFcRxCal free\n",__FILE__, __FUNCTION__, __LINE__);
    }

    if (pPifCtxt->pFcTxCal != NULL) {
        sysHyPhy20gMemFree(pPifCtxt->pFcTxCal);
        DBG_PRINT("%s, %s, %d, returning success after pFcTxCal free\n",__FILE__, __FUNCTION__, __LINE__);
    }

    if (pPifCtxt->pTxSchdCtxt0 != NULL) {
        sysHyPhy20gMemFree(pPifCtxt->pTxSchdCtxt0);
        DBG_PRINT("%s, %s, %d, returning success after pTxSchdCtxt0 free\n",__FILE__, __FUNCTION__, __LINE__);
    }

    if (pPifCtxt->pTxSchdCtxt1 != NULL) {
        sysHyPhy20gMemFree(pPifCtxt->pTxSchdCtxt1);
        DBG_PRINT("%s, %s, %d, returning success after pTxSchdCtxt1 free\n",__FILE__, __FUNCTION__, __LINE__);
    }

    /* destroy pPifCtxt structure for the PIF subsystem */
    if (pDevCtxt->pPifCtxt != NULL) {        
        sysHyPhy20gMemFree(pDevCtxt->pPifCtxt);
        DBG_PRINT("%s, %s, %d, returning success after free\n",__FILE__, __FUNCTION__, __LINE__);
    }
    
    return HYPHY20G_SUCCESS;

} /* pifHyPhy20gPifDestroyCtxt */


/*******************************************************************************
**
**  pifHyPhy20gPifInitCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Intializes memory required to operate the PIF subsystem.
**
**                  This function should be called when the memory is first
**                  alocated and prior to performing any channel specific
**                  configuration in the PIF.
**
**
**  INPUTS:         fileHndl  - device handle
**                  *pCtxt    - pointer to PIF context structure
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gPifInitCtxt(struct file *fileHndl, sHYPHY20G_PIF_CTXT *pCtxt)
{
    UINT4 i;
    UINT4 pifMode;
    sHYPHY20G_PIF_TX_CHNL_CTXT *pTxChnl;
    sHYPHY20G_SCHD_CTXT *pSchd;

    /* argument checking */
    HYPHY20G_ASSERT((pCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pCtxt is NULL.");

    pifMode = pCtxt->mode;

    /* initialize PIF Tx channel structures */
    if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI0 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI0 ||
        pifMode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI ||
        pifMode == HYPHY20G_PIF_OPMODE_INTERLAKEN_INBAND ||
        pifMode == HYPHY20G_PIF_OPMODE_INTERLAKEN_OOB) {
        /* init Tx scheduler 0 context */
        pSchd = pCtxt->pTxSchdCtxt0;
        pSchd->valid = HYPHY20G_INVALID_VAL_32BIT;
        pSchd->schdId = HYPHY20G_PIF_TX_SCHD_0;
        pSchd->pScbsGrpCtxt = NULL;
        pSchd->numRows = 1;
        pSchd->numGrp = HYPHY20G_PIF_SCHD_MAX_GRP;        
        pSchd->portOffset = 0;
        pSchd->intfGrpDir = 0;
        pSchd->maxChnl = HYPHY20G_PIF_TX_SCHD_MAX_CHNL;
        pSchd->pOffLineCalMem = NULL;
        pSchd->pOnLineCalMem = NULL;
    }

    if (pifMode == HYPHY20G_PIF_OPMODE_EXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_EXAUI0_SXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1 ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI ||
        pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI) {
        /* init Tx scheduler 1 context */
        pSchd = pCtxt->pTxSchdCtxt1;
        pSchd->valid = HYPHY20G_INVALID_VAL_32BIT;
        pSchd->schdId = HYPHY20G_PIF_TX_SCHD_1;
        pSchd->pScbsGrpCtxt = NULL;
        pSchd->numRows = 1;
        pSchd->numGrp = HYPHY20G_PIF_SCHD_MAX_GRP;
        pSchd->portOffset = 0;
        pSchd->intfGrpDir = 0;
        pSchd->maxChnl = HYPHY20G_PIF_TX_SCHD_MAX_CHNL;
        pSchd->pOffLineCalMem = NULL;
        pSchd->pOnLineCalMem = NULL;
    }

    /* initialize PIF Tx channel structures */
    for (i = 0; i < HYPHY20G_PIF_TX_SCHD_MAX_CHNL; i++) {
        pTxChnl = &(pCtxt->txChnlCtxt[i]);
        pTxChnl->valid = HYPHY20G_INVALID_VAL_32BIT;
        pTxChnl->chnlId = i;
        pTxChnl->rate0 = 0;
        pTxChnl->rate1 = 0;
        pTxChnl->state = HYPHY20G_PIF_CHNL_START;
        pTxChnl->intf = HYPHY20G_INVALID_VAL_32BIT;
    }

    return HYPHY20G_SUCCESS;

} /* pifHyPhy20gPifInitCtxt */

/*******************************************************************************
**
**  pifHyPhy20gPifCtxtGetMode
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves the the configuration mode
**                  from PIF context memory.
**
**  INPUTS:         fileHndl   - device handle
**                  *pMode    - a pointer to the memory where store the
**                              return back value
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gPifCtxtGetMode(struct file *fileHndl, UINT4 *pMode)
{
    sHYPHY20G_PIF_CTXT *pPifCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    HYPHY20G_ASSERT((pMode != NULL) , HYPHY20G_ERR_NULL_PTR_PASSED,
                    "pMode is NULL.");

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    pPifCtxt = pDevCtxt->pPifCtxt;
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL) && (pPifCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt or pPifCtxt is NULL.");

    *pMode = pPifCtxt->mode;
    HYPHY20G_ASSERT((*pMode <= HYPHY20G_PIF_OPMODE_SXAUI0_EXAUI1),
                    HYPHY20G_ERR_INVALID_MODE,
                    "*pMode is %ld.", *pMode);

    return HYPHY20G_SUCCESS;

} /* pifHyPhy20gPifCtxtGetMode */

/*******************************************************************************
**
**  hyPhy20gPifIntlknFcInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function select the Interlaken in-band or Interlaken
**                  out-of-band flow control channel mapping type.
**
**                  When Interlaken in-band flow control is used, only
**                  programmable mode is supported.
**
**                  For both Interlaken in-band and out-of-band flow control,
**                  flow control calendar inside Interlaken block must be
**                  initialized.
**
**                  The INTERLAKEN block must be 'Major Bidirectional States' of
**                  2 in order for this function to be executed, otherwise
**                  HYPHY20G_ERR_INVALID_STATE will be returned.
**
**  INPUTS:         fileHndl  - device handle
**                  dir       - determine the flow control calendar
**                              0 - Tx flow control(flow control to HyPHY 20G)
**                              1 - Rx flow control(flow control from HyPHY 20G)
**                  mode      - 1 - Interlaken in-band flow control
**                                  (programmable mode)
**                              2 - Interlaken out-of-band flow control
**                  calLen    - For mode == 1:
**                              1 to 256 - Number of flow control channel
**                                         entries reported or received in
**                                         Interlaken Idle and Burst Control
**                                         Words.
**                              For mode == 2:
**                              1 to 256 - Number of flow control channel
**                                         entries
**
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifIntlknFcInit(struct file *fileHndl, UINT4 dir, UINT4 mode,
                             UINT4 calLen)
{
    INT4 result;
    UINT4 state, intlknDir;
    const UINT4 slice = HYPHY20G_INTLKN_PIF_INTLKN;

    /* argument checking */
    if (dir > 1 || mode < 1  || mode > 2 || calLen < 1 || calLen > 256) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve major bidirectional state */
    result = utilHyPhy20gIntlknCoreGetMajBiDirState(fileHndl, slice, &state);
    if(result) return result;

    /* confirm major bidirection state of 2 or return invalid state error */
    if(state != HYPHY20G_INTLKN_STATE_STANDBY){
        return HYPHY20G_ERR_INVALID_STATE;
    }

    /* modify dir argument for use by interlaken file API */
    intlknDir = (dir == 0) ? 1:0;

    /* Configure Interlaken Flow Control Mode */
    result = utilHyPhy20gIntlknCoreFcModeCfg(fileHndl, slice, intlknDir, mode, calLen);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifIntlknFcInit */


/*******************************************************************************
**
**  hyPhy20gPifIntlknFcPortMap
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the mapping of a flow control
**                  calendar entry to a flow control port for the transmit or
**                  receive calendar of INTERLAKEN instance.
**
**                  This function is only valid for a INTERLAKEN instance that
**                  has been configured for programmable calendar mode of FC.
**
**                  More than one calendar entry may be associated with a flow
**                  control port.
**
**                  The INTERLAKEN block must be 'Major Bidirectional States' of
**                  2 or 3 in order for this function to be executed, otherwise
**                  HYPHY20G_ERR_INVALID_STATE will be returned.
**
**  INPUTS:         fileHndl  - device handle
**                  dir       - determine the flow control calendar
**                              0 - Tx flow control(flow control to HyPHY 20G)
**                              1 - Rx flow control(flow control from HyPHY 20G)
**                  enable    - 1 - mapping a channel's flow control port to
**                                  the calendar address
**                              0 - remove all calendar entires associated
**                                  with the flow control port
**                  calAddr   - Calendar address to be configured
**                              0 to 256 - dependent on the length of calendar
**                                         selected when programmable calendar
**                                         mode was configured
**                              Not used when enable == 0
**                  fcPort    - Flow control port to be associated with the
**                              calendar address
**                              0 to 149 - Flow control port address
**                              150 - Link Level Flow Control Port
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_STATE
**
*******************************************************************************/
INT4 hyPhy20gPifIntlknFcPortMap(struct file *fileHndl, UINT4 dir, UINT4 enable,
                                UINT4 calAddr, UINT4 fcPort)
{
    INT4 result;
    UINT4 state;
    UINT4 i;
    UINT4 calData;
    UINT4 calLen;
    UINT4 calEnReg, lastMsk, intlknDir;
    const UINT4 slice = HYPHY20G_INTLKN_PIF_INTLKN;

    /* argument checking */
    if (dir > 1 || enable > 1|| calAddr > 256 || fcPort > 150) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* modify dir argument for use by interlaken file API */
    intlknDir = (dir == 0) ? 1:0;

    calEnReg = HYPHY20G_PIF_INTLKN_CORE_REG_CLDR_EN_OFFSET(intlknDir);
    lastMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_CLDR_EN_LAST_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, calEnReg, lastMsk, &calLen);
    if(result) return result;
    calLen++;

    /* retrieve major bidirectional state */
    result = utilHyPhy20gIntlknCoreGetMajBiDirState(fileHndl, slice, &state);
    if(result) return result;

    /* confirm major bidirection state of 2 or return invalid state error */
    if((state == HYPHY20G_INTLKN_STATE_STANDBY)||
       (state == HYPHY20G_INTLKN_STATE_OPERATIONAL)){

        if (enable) {
            /* Configure Interlaken Flow Control Calendar Entry */
            result = utilHyPhy20gIntlknCoreFcCalCfg(fileHndl, slice, intlknDir,
                                                    calAddr, fcPort);
            if(result) return result;

            /* enable operation finish. exit */
            return HYPHY20G_SUCCESS;
        }

        /* remove all calendar entry associated with this fcPort */
        for (i = 0; i < calLen; i++) {
            result = pifHyPhy20gIntlnkCalRead (fileHndl, intlknDir, i, &calData);
            if(result) return result;

            if (calData == fcPort) {
                result = utilHyPhy20gIntlknCoreFcCalCfg(fileHndl, slice, intlknDir,
                                                        i, 151);
                if(result) return result;
            }
        }

    } else {
        return HYPHY20G_ERR_INVALID_STATE;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifIntlknFcPortMap */


/*******************************************************************************
**
**  pifHyPhy20gPifTopCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the top level register of PIF.
**
**                  Denpending on packet mode or segment mode, this function
**                  configures the Packet Interface read interface (in CPB)
**                  scheduling algorithm for getting data from
**                  the QDR to the PIF.
**
**  INPUTS:         fileHndl  - device handle
**                  hdrStrip  - 0 - No strip. Used for Standard XAUI mode
**                              3 - transmit out TYPE field. Used fore eXAUI 
**                                  and Interlaken mode
**                              4 - don't transmit out TYPE field
**                                  Used for eXAUI and Interlaken mode.
**                  mode      - determine which mode the PIF operates on
**                              1 - packet mode
**                              2 - segmented mode
**                              3 - pseudo segmented mode
**                  segSize   - determine the segment size for segment
**                              and pseudo segment mode
**                              Legal range 0 to 15
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gPifTopCfg(struct file *fileHndl, UINT4 hdrStrip, UINT4 mode,
                          UINT4 segSize)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 cpbPktMode;

    /* argument checking */
    if (mode < 1 || mode > 3 || segSize > 15 ||
        (hdrStrip != 0 && hdrStrip != 3 && hdrStrip != 4)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_PIF_PIF_REG_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 0x6,
            HYPHY20G_PIF_PIF_BIT_CFG_EOP_NXT_DLY_MSK,
            HYPHY20G_PIF_PIF_BIT_CFG_EOP_NXT_DLY_OFF);
    /* set OH_BYT_CNT */
    val = mHYPHY20G_SET_FIELD4(val, hdrStrip,
            HYPHY20G_PIF_PIF_BIT_CFG_OH_BYT_CNT_MSK,
            HYPHY20G_PIF_PIF_BIT_CFG_OH_BYT_CNT_OFF);
    /* set Packet/Segment mode */
    val = mHYPHY20G_SET_FIELD4(val, mode,
            HYPHY20G_PIF_PIF_BIT_CFG_MODE_MSK,
            HYPHY20G_PIF_PIF_BIT_CFG_MODE_OFF);

    /* set segment size for segment and pseudo segment mode*/
    if (mode != 1) {
        val = mHYPHY20G_SET_FIELD4(val, segSize,
                HYPHY20G_PIF_PIF_BIT_CFG_SEG_SIZE_MSK,
                HYPHY20G_PIF_PIF_BIT_CFG_SEG_SIZE_OFF);
    }

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* select scheduling algorithm for getting data from the CPB to the PIF */
    cpbPktMode = (mode == 2) ? 0 : 1;
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_PAC_IF_MISC_CFG,
                     HYPHY20G_CPB_CPB_BIT_PAC_IF_MISC_CFG_PI_RD_PKT_MODE_OFF,
                     cpbPktMode);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_CPB_CPB_REG_PAC_IF_MISC_CFG,
                     HYPHY20G_CPB_CPB_BIT_PAC_IF_MISC_CFG_PI_RD_LIFO_EN_OFF,
                     cpbPktMode);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* pifHyPhy20gPifTopCfg */


/*******************************************************************************
**
**  pifHyPhy20gPifTxSchdInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs initialization required for the
**                  operation of TX channels within PIF interface.
**
**                  Function will setup the PIF Tx scheduler
**                  for the PIF interface
**
**                  Alternatively, this function can be called to disable the
**                  PIF Tx scheduler (it will also disable
**                  interface transmission).
**
**
**  INPUTS:         *fileHndl - base address
**                  schd      - 0x10000 - PIF Tx scheduler 0 
**                              (HYPHY20G_PIF_TX_SCHD_0) 
**                              0x10001 - PIF Tx scheduler 1
**                              (HYPHY20G_PIF_TX_SCHD_1) 
**                  schdLen   - number of 4 entry scheduler addresses
**                                  - Legal range 1 to 128 for PIF TX scheduler
**                                  - Value not processed when enable = 0
**                  enable    - 2 - enable PIF Tx scheduler 
**                                  for Interlaken 2 x 10G POS channel
**                              1 - enable PIF Tx scheduler for all other
**                                  cases other than enable = 2
**                              0 - disable PIF Tx scheduler
**                  reset     - reset scheduler prior to initialization
**                              (set to 0 when configuring scheduler in XAUI
**                               mode, else set to 1)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gPifTxSchdInit(struct file *fileHndl, UINT4 schd, UINT4 schdLen,
                              UINT4 enable, UINT4 reset)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_SCHD_CTXT *pSchd;
    UINT4 rstOff = 0, schdNum = 0, schdMode;
    INT4 result;

    /* argument checking */
    DBG_PRINT("%s, %s, %d,schd = %u, schdLen = %u, enable = %u, reset = %u\n",__FILE__, __FUNCTION__, __LINE__, 
              (unsigned int)schd, (unsigned int)schdLen, (unsigned int)enable, (unsigned int)reset);
    
    if (((schd != HYPHY20G_PIF_TX_SCHD_0) && (schd != HYPHY20G_PIF_TX_SCHD_1)) 
        || (enable > 2) || (schdLen > HYPHY20G_PIF_TX_SCHD_MAX_NUM_ROW) ||
        ((enable != 0) && (schdLen < 1)) || (reset > 1) ) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the PIF context is valid */
    if(pDevCtxt->pPifCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
        
    /* indentify parameters for the respective intf group scheduler(s) */
    switch(schd){
        case HYPHY20G_PIF_TX_SCHD_0:
            /* reset masks */
            rstOff = HYPHY20G_PIF_PIF_BIT_SW_RST_SCH0_RST_OFF;
            pSchd = pDevCtxt->pPifCtxt->pTxSchdCtxt0;
            /* scheduler instance */
            schdNum = HYPHY20G_PIF_TX_SCHD_0;
            break;
        case HYPHY20G_PIF_TX_SCHD_1:
            /* reset masks */
            rstOff = HYPHY20G_PIF_PIF_BIT_SW_RST_SCH1_RST_OFF;
            pSchd = pDevCtxt->pPifCtxt->pTxSchdCtxt1;
            /* scheduler instance */
            schdNum = HYPHY20G_PIF_TX_SCHD_1;
            break;
        default:
            /* redundant to initial argument checking */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if(enable != 0) {       /* enable operations */
        if (reset) {
            /* reset and initialize read scheduler */
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST,
                                         rstOff, 1);
            if(result){
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);                
                return result;
            }          
            result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST,
                                         rstOff, 0);
            if(result){
                DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);                
                return result;
            }                
                
        }

        schdMode = (enable == 1) ? HYPHY20G_SCBS_SCHD_BONUS_MODE_DISABLE:HYPHY20G_SCBS_SCHD_BONUS_MODE_ENABLE;
        result = utilHyPhy20gScbsInitSchd(fileHndl, pSchd,
                                          (eHYPHY20G_SCBS_SCHD_ID) schdNum,
                                          schdLen, schdMode);
        if(result){
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);                
            return result;
        }          

    } else {                /* disable operations */

        /* destroy read scheduler context and put in reset */
        result = utilHyPhy20gScbsResetSchd(fileHndl, pSchd);
        if(result){
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);                
            return result;
        }          

        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST,
                                     rstOff, 1);
        if(result){
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);                
            return result;
        }          
    }

    return HYPHY20G_SUCCESS;

} /* pifHyPhy20gPifTxSchdInit */

/*******************************************************************************
**
**  pifHyPhy20gPifSchdUpdate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Updates specified interface group schedulers and the QDR
**                  scheduler using read and write channel state context.
**
**  INPUTS:         fileHndl  - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gPifSchdUpdate(struct file  *fileHndl,
                              sHYPHY20G_PIF_TX_CHNL_CTXT *pTxChnl)
{
    UINT4 size;
    UINT4 *pCalData = NULL;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;

    /* argument checking */
    HYPHY20G_ASSERT((pTxChnl != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pTxChnl is NULL.");

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL) && (pDevCtxt->pPifCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt or pDevCtxt->pPifCtxt is NULL.");


    /* allocate memory to pCalData used for holding scheduler contents */
    /* use PIF Scheduler context as this is the largest scheduler */
    size = sizeof(UINT4)*(HYPHY20G_PIF_TX_SCHD_MAX_NUM_ROW *
                          HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW);

    pCalData = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if(pCalData == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* update PIF Tx scheduler(s) */
    if((pTxChnl->intf & 0x1) == ((HYPHY20G_PIF_TX_SCHD_0 & 0x0000FFFF) + 1)){
        pSchd = pDevCtxt->pPifCtxt->pTxSchdCtxt0;
        /* get scheduler content data */
        result = pifHyPhy20gPifGenerateTxSchd(fileHndl, pSchd, pCalData);
        if(result){
            sysHyPhy20gMemFree(pCalData);
            return result;
        }
        /* update scheduler with content data */
        result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
        if(result){
            sysHyPhy20gMemFree(pCalData);
            return result;
        }
    }

    sysHyPhy20gMemSet(pCalData, 0, size);
    if((pTxChnl->intf & 0x2) == ((HYPHY20G_PIF_TX_SCHD_1 & 0x0000FFFF) + 1)){
        pSchd = pDevCtxt->pPifCtxt->pTxSchdCtxt1;
        /* get scheduler content data */
        result = pifHyPhy20gPifGenerateTxSchd(fileHndl, pSchd, pCalData);
        if(result){
            sysHyPhy20gMemFree(pCalData);
            return result;
        }
        /* update scheduler with content data */
        result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, pCalData);
        if(result){
            sysHyPhy20gMemFree(pCalData);
            return result;
        }

    }

    sysHyPhy20gMemFree(pCalData);
    return HYPHY20G_SUCCESS;

} /* pifHyPhy20gPifSchdUpdate */


/*******************************************************************************
**
**  pifHyPhy20gPifGenerateTxSchd
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Evaluates the channel context for all channels used in
**                  an interface group scheduler, generates the scheduler
**                  calendar memory based on a best effort even distribution.
**
**
**  INPUTS:         fileHndl  - device handle
**                 *pSchd     - pointer to PIF Tx scheduler context
**                              for one of the following schedulers
**                               - PIF Tx Scheduler 0
**                               - PIF Tx Scheduler 1
**                 *pCalData  - pointer to formatted scheduler calendar data
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   This function updates the totalChnlRate parameter of the
**                  pSchd structure.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_TOO_MANY_CHANNEL
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**
*******************************************************************************/
INT4 pifHyPhy20gPifGenerateTxSchd(struct file  *fileHndl,
                                  sHYPHY20G_SCHD_CTXT *pSchd, UINT4 *pCalData)
{
    sHYPHY20G_PIF_TX_CHNL_CTXT *pTxChnl = NULL;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 totalChnlRate = 0, numCalChnl = 0;
    UINT4 lowestChnlRate, maxChnlRate;
    UINT4 chnlToCalMultFactor;
    UINT4 numCalEntries, populateChnl, populateRate;
    UINT4 calSingles, calSpace;
    UINT4 i, j, k, size;
    UINT4 lowestIndex = 0, lowestRate;
    UINT4 emptyLocOffset, linearCaloffset;
    UINT4 tmp, swap;
    UINT4 halfNumCalEntries, updatedNumCalChnl, splitLowRate;
    UINT4 workingRate, workingEntries, chnlRate, scaleEntries;
    /* alloate memory for local use by function */
    UINT4 *pChnlNum, *pRawChnlNum, *pChnlRate, *pRawChnlRate;
    UINT4 *pLinearCal, *pFormattedCalData;
    UINT4 *pValid;
    UINT4 *pEmptyLoc, numEmptyLoc, newNumEmptyLoc;
    UINT4 chnl0, chnl1, chnlNumData;

    /* verify pointer to global PIF data */

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");
    /* verify pointer to pSchd is not NULL! */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSchd is NULL.");

    HYPHY20G_ASSERT((pSchd->maxChnl == HYPHY20G_PIF_TX_SCHD_MAX_CHNL),
                    HYPHY20G_ERR_INVALID_ARG,
                    "%s", "For PIF pSchd->maxChnl must set to HYPHY20G_PIF_TX_SCHD_MAX_CHNL.");

    size = sizeof(UINT4)*((pSchd->maxChnl)*2) +
           sizeof(UINT4)*(pSchd->maxChnl) +
           sizeof(UINT4)*((pSchd->maxChnl)*2) +
           sizeof(UINT4)*(pSchd->maxChnl) +
           sizeof(UINT4)*((pSchd->numRows)*HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW)+
           sizeof(UINT4)*(pSchd->maxChnl) +
           sizeof(UINT4)*((pSchd->numRows)*HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW);


    pChnlNum = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if(pChnlNum == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pRawChnlNum  = (UINT4 *)(pChnlNum + ((pSchd->maxChnl)*2));
    pChnlRate    = (UINT4 *)(pRawChnlNum + (pSchd->maxChnl));
    pRawChnlRate = (UINT4 *)(pChnlRate + ((pSchd->maxChnl)*2));
    pLinearCal   = (UINT4 *)(pRawChnlRate + (pSchd->maxChnl));
    pValid       = (UINT4 *)(pLinearCal + ((pSchd->numRows)*
                             HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW));
    pEmptyLoc    = (UINT4 *)(pValid + (pSchd->maxChnl));

    pFormattedCalData = pCalData;

    /* extract channel context for making calendar population decisions
       1) calculate the total channel rate
       2) get the lowest channel rate
       3) get the maximum channel rate
       4) store target channel number to a local list
       5) store target channel rate to a local list */
    j = 0;
    lowestChnlRate = 0xFFFFFFFF;
    maxChnlRate = 0;
    for (i = 0; i < HYPHY20G_PIF_TX_SCHD_MAX_CHNL; i++) {

        pTxChnl = &(pDevCtxt->pPifCtxt->txChnlCtxt[i]);

        /* check if this channel belongs to the SCBS */
        if((pTxChnl->intf == 1 && pSchd->schdId == HYPHY20G_PIF_TX_SCHD_1) ||
           (pTxChnl->intf == 2 && pSchd->schdId == HYPHY20G_PIF_TX_SCHD_0)) {
            continue;
        }

        if((pTxChnl->valid == HYPHY20G_VALID_SIGN) &&
           (pTxChnl->state == HYPHY20G_PIF_CHNL_OPERATIONAL)){

            *(pRawChnlNum + j) = pTxChnl->chnlId;
            *(pRawChnlRate + j) = pTxChnl->rate0;

            totalChnlRate += *(pRawChnlRate + j);
            if (lowestChnlRate > *(pRawChnlRate + j)){
                lowestChnlRate = *(pRawChnlRate + j);
            }
            if (maxChnlRate < *(pRawChnlRate + j)){
                maxChnlRate = *(pRawChnlRate + j);
            }
            numCalChnl++;
            j++;
        }
    } /* end of for loop */

    /* Update scheduler structure with context gathered from individual
       channels */
    pSchd->totalChnlRate = totalChnlRate;


    /* if totalChnlRate = 0, the entire scheduler must be populated with
        NULL addresses */
    if(totalChnlRate == 0){
        for (i=0; i < pSchd->numRows; i++) {
            for (j=0; j < HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW; j++){
                *(pCalData + (HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW*i) + j) =
                    HYPHY20G_PIF_TX_SCHD_NULL_CHNL;
            }
        }
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_SUCCESS;
    }

    /* sort chnlNum and chnlRate so that they are ordered from lowest to
       highest chnlRate */
    for(i=0; i < pSchd->maxChnl; i++){
        if(i < numCalChnl){
            *(pValid + i) = 1;
        } else {
            *(pValid + i) = 0;
        }
    }
    lowestRate = 0xFFFFFFFF;
    for(j=0; j < numCalChnl; j++) {
        /* find lowest valid in set */
        for(i=0; i < numCalChnl; i++) {
            if((*(pRawChnlRate + i) < lowestRate)&& *(pValid + i)){
                lowestRate = *(pRawChnlRate + i);
                lowestIndex = i;
            }
        }
        *(pChnlRate + j) = *(pRawChnlRate + lowestIndex);
        *(pChnlNum + j) = *(pRawChnlNum + lowestIndex);
        *(pValid + lowestIndex) = 0;
        lowestRate = 0xFFFFFFFF;
    }

    /* generate scheduler for Interlaken Packet 2 x 10G POS special case */
    if(pSchd->schdMode == HYPHY20G_SCBS_SCHD_BONUS_MODE_ENABLE){
        DBG_PRINT("%s, %s, %d, Interlaken Packet 2 x 10G POS setup\n",__FILE__, __FUNCTION__, __LINE__);
        switch(numCalChnl){
            case 0: /* no channels */
                chnl0 = HYPHY20G_PIF_TX_SCHD_NULL_CHNL;
                chnl1 = HYPHY20G_PIF_TX_SCHD_NULL_CHNL;
                break;
            case 1: /* one channel */
                chnl0 = *(pChnlNum + 0);
                chnl1 = HYPHY20G_PIF_TX_SCHD_NULL_CHNL;
                break;
            case 2: /* two channels */
                chnl0 = *(pChnlNum + 0);
                chnl1 = *(pChnlNum + 1);
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_TOO_MANY_CHANNEL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_TOO_MANY_CHANNEL;
        }
        /* apply to scheduler entry format */
        for (i=0; i < pSchd->numRows; i++) {
            for (j=0; j < HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW; j++){
                chnlNumData = ( j==3 || j==0 ) ? chnl0:chnl1;
                *(pCalData + (HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW*i) + j) =
                    chnlNumData;
            }
        }
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_SUCCESS;
    }

    /* retrieve scheduler context to determine size of scheduler calendar
       needing to be populated */
    numCalEntries = HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW * (pSchd->numRows);

    if (numCalChnl > numCalEntries){
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_ERR_SCHD_CAL_SIZE;
    }

    /* ensure totalChnlRate > numCalEntries */
    if (totalChnlRate < numCalEntries){

        chnlToCalMultFactor = (numCalEntries/totalChnlRate)+1;

        for (i=0; i < numCalChnl; i++){

            *(pChnlRate + i) *= chnlToCalMultFactor;

        }
        totalChnlRate *= chnlToCalMultFactor;
    }

    /* scale required calendar entries so that they can be contained in
       the number of available calendar entries ensuring that at least one
       entry is present for a channel */
    workingRate = totalChnlRate;
    workingEntries = numCalEntries;
    for(i=0; i < numCalChnl; i++){
        if(i == (numCalChnl-1)){
            /* ensure last channel consumes remainder of calendar entries */
            *(pChnlRate+i) = workingEntries;
        } else {
            chnlRate = *(pChnlRate+i);
            chnlRate *= 100;
            workingRate *= 100;
            workingEntries *= 100;

            scaleEntries = (chnlRate*workingEntries)/workingRate;
            if ((scaleEntries%100) > 49){
                scaleEntries = (scaleEntries/100) + 1;
            } else {
                scaleEntries = scaleEntries/100;
            }

            /* ensure a minimum of 1 entry */
            if(scaleEntries == 0){
                scaleEntries = 1;
            }

            /* update results */
            workingRate = workingRate/100;
            workingRate -= *(pChnlRate+i);
            workingEntries = workingEntries/100;
            workingEntries -= scaleEntries;
            *(pChnlRate+i) = scaleEntries;
        }
    }

    /* populate the calendar so that entries are roughly evenly distributed */
    for(i=0; i < numCalEntries; i++){
        *(pLinearCal + i) = HYPHY20G_PIF_TX_SCHD_NULL_CHNL;
        *(pEmptyLoc + i) = i;
    }
    numEmptyLoc = newNumEmptyLoc = numCalEntries;

    /* test for entry greater than 50% calendar length and split into two
       entries */
    halfNumCalEntries = numCalEntries/2;
    updatedNumCalChnl = numCalChnl;
    for (i = 0; i < numCalChnl; i++) {
        if (*(pChnlRate + i) > halfNumCalEntries) {
            /* split into two entries */
            splitLowRate = *(pChnlRate + i) - halfNumCalEntries;
            *(pChnlRate + updatedNumCalChnl) = splitLowRate;
            *(pChnlNum + updatedNumCalChnl) = *(pChnlNum + i);
            updatedNumCalChnl++;

            *(pChnlRate + i) = halfNumCalEntries;
        }
    }
    numCalChnl = updatedNumCalChnl;

    /* order entries from high-to-low based on channel rate */
    do {
        swap = 0;
        for(i=0; i < numCalChnl-1; i++) {
            j = i + 1;
            if (*(pChnlRate + j) > *(pChnlRate + i)) {
                /* swap */
                tmp = *(pChnlRate + i);
                *(pChnlRate + i) = *(pChnlRate + j);
                *(pChnlRate + j) = tmp;

                tmp = *(pChnlNum + i);
                *(pChnlNum + i) = *(pChnlNum + j);
                *(pChnlNum + j) = tmp;

                swap = 1;
            }
        }
    } while (swap);

    /* STRICT POPULATION ALGO */
    for (i=0; i < numCalChnl; i++) {
        DBG_PRINT("DEBUG_JC 1: numEmptyLoc = %lu, newNumEmptyLoc = %lu\n", numEmptyLoc, newNumEmptyLoc);

        populateRate = *(pChnlRate + i);
        populateChnl = *(pChnlNum + i);
        /* identify the required number of calendar entries between each
           occurance of this channel number */
        if(populateRate == 0){
            sysHyPhy20gMemFree(pChnlNum);
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_ALGO_FAIL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_ALGO_FAIL;
        }
        calSpace = numEmptyLoc/populateRate;
        calSingles = numEmptyLoc%populateRate;
        if (calSpace == 0) {
            DBG_PRINT("DEBUG_JC 1a: calSpace forced to 1\n");
            calSpace = 1;
        }

        DBG_PRINT("DEBUG_JC 2: LINE %d\n", __LINE__);
        emptyLocOffset = 0;
        for (j=0; j < populateRate; j++) {
            /* fetch location of empty entry, alternating between start and
               end of list */
            if(j==0) {
                emptyLocOffset = 0;
            } else {
                if(calSingles){
                    emptyLocOffset += (calSpace+1);
                    calSingles--;
                } else {
                    emptyLocOffset += calSpace;
                }
            }
            linearCaloffset = *(pEmptyLoc + emptyLocOffset);

            /*pPopulate calendar with channel number */
           *(pLinearCal + linearCaloffset) = populateChnl;

           /* remove empty location from list */
           *(pEmptyLoc + emptyLocOffset) = 0xFFFF;
           newNumEmptyLoc--;
        }

        DBG_PRINT("DEBUG_JC 3: LINE %d\n", __LINE__);
        /* push all empty locations to front of list while maintaining order */
        do {
            swap = 0;
            for(j=0; j < numCalEntries-1; j++) {
                k = j + 1;
                if (*(pEmptyLoc + k) < *(pEmptyLoc + j)) {
                    /* swap */
                    tmp = *(pEmptyLoc + j);
                    *(pEmptyLoc + j) = *(pEmptyLoc + k);
                    *(pEmptyLoc + k) = tmp;

                    swap = 1;
                }
            }
        } while (swap);

        numEmptyLoc = newNumEmptyLoc;
    }

    DBG_PRINT("%s, %s, %d, Format Calendar Entry Data to Rows of SCBS\n",__FILE__, __FUNCTION__, __LINE__);


    /* apply the linear calendar data to the device scheduler calendar
       format */
    j = 0;
    for(i = 0; i < pSchd->numRows; i++){
        *(pFormattedCalData + (i*HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW)) =
            *(pLinearCal + j);
        *(pFormattedCalData + (i*HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW + 1)) =
            *(pLinearCal + (j+1));
        j += 2;
    }

    for(i=0; i < pSchd->numRows; i++){
        *(pFormattedCalData + (i*HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW + 2)) =
            *(pLinearCal + j);
        *(pFormattedCalData + (i*HYPHY20G_PIF_TX_SCHD_CAL_ENTRY_PER_ROW + 3)) =
            *(pLinearCal + (j+1));
        j += 2;
    }

    /* provided the formatted calendar data to the calling function */

    /* free allocated memory used by local function */
    sysHyPhy20gMemFree(pChnlNum);
    return HYPHY20G_SUCCESS;

} /* pifHyPhy20gPifGenerateTxSchd */

/*******************************************************************************
**
**  pifHyPhy20gPifXauiInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes a XAUI slice for standard XAUI
**                  or eXAUI modes of operation, then enables the ingress and
**                  egress data path for the slice.
**
**                  This function unconditionally sets the CNT_MODE field to
**                  b'01.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - determine which slice to be configured
**                              0 - XAUI0
**                              1 - XAUI1
**                  enable    - 0 - disable
**                              1 - enable
**                  mode      - Value not processed when enable = 0
**                              0 - eXAUI
**                              1 - standard XAUI
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gPifXauiInit(struct file *fileHndl, UINT4 slice, UINT4 enable,
                            UINT4 mode)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 rstOff, enOff;
    UINT4 spre, tx_maxfr, noPrmb, disCrcChk, rxMaxFr, cutThruThSel;
    UINT4 txInsCh, txInsS, rxUsrCh, rxExtCh, rxExtS;
    UINT4 bip8Strip, bip8Verify, bip8Insert;

    /* argument checking */
    if (slice > 1 || enable > 1 || mode > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (slice == 0) {
        rstOff = HYPHY20G_PIF_PIF_BIT_SW_RST_XAUI0_RST_OFF;
        enOff = HYPHY20G_PIF_PIF_BIT_IF_EN_EN_XAUI0_OFF;
    } else {
        rstOff = HYPHY20G_PIF_PIF_BIT_SW_RST_XAUI1_RST_OFF;
        enOff = HYPHY20G_PIF_PIF_BIT_IF_EN_EN_XAUI1_OFF;
    }

    if (enable == HYPHY20G_DISABLE) {
        /* reset the eXAUI slice */
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_PIF_PIF_REG_SW_RST,
                                     rstOff, 1);
        if (result) return result;

        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_PIF_PIF_REG_IF_EN,
                                     enOff, 0);
        if (result) return result;

        /* function operation complete - exit */
        return HYPHY20G_SUCCESS;
    }

    /*
     * proceed with enabling and initialization of block and sub blocks
     */
    /* eXAUI Top Level Initialization */
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(HYPHY20G_XAUI_TOP_REG_SW_RST,
                                               slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                                   (0x00FF << 0), 0xFE);
    if (result) return result;
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                                   (0x00FF << 0), 0x00);
    if (result) return result;


    /*
     * configure the TXXG
     */
    spre = (mode == 0) ? 1 : 0;

    addr = HYPHY20G_XAUI_TXXG_REG_CFG_1;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&val, HYPHY20G_XAUI_TXXG_BIT_CFG_1_TXEN0_MSK,
                                   0);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_TXXG_BIT_CFG_1_IPG_REDUC_EN_MSK,
                                   1);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_TXXG_BIT_CFG_1_ALIGN_32BIT_MSK,
                                   1);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val, HYPHY20G_XAUI_TXXG_BIT_CFG_1_FCTX_MSK,
                                   mode);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val, HYPHY20G_XAUI_TXXG_BIT_CFG_1_FCRX_MSK,
                                   mode);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val, HYPHY20G_XAUI_TXXG_BIT_CFG_1_SPRE_MSK,
                                   spre);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* configure the tx max frame size */
    tx_maxfr = HYPHY20G_PIF_XAUI_TXXG_RECOMMEND_TX_MAXFR;
    addr = HYPHY20G_XAUI_TXXG_REG_TX_MAX_FRM_SZ;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                              HYPHY20G_XAUI_TXXG_BIT_TX_MAX_FRM_SZ_TX_MAXFR_MSK,
                              tx_maxfr);
    if (result) return result;

    /* configure the tx min frame size */
    addr = HYPHY20G_XAUI_TXXG_REG_TX_MIN_FRM_SZ;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                    HYPHY20G_XAUI_TXXG_BIT_TX_MIN_FRM_SZ_TX_MINFR_MSK,
                    HYPHY20G_PIF_XAUI_TXXG_RECOMMEND_TX_MINFR);
    if (result) return result;


    /*
     * configure the RXXG
     */
    noPrmb = (mode == 0) ? 1 : 0;

    addr = HYPHY20G_XAUI_RXXG_REG_CFG_1;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&val, HYPHY20G_XAUI_RXXG_BIT_CFG_1_RXEN_MSK,
                                   1);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_RXXG_BIT_CFG_1_NO_PRMB_MSK,
                                   noPrmb);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_RXXG_BIT_CFG_1_PASS_ERRORS_MSK,
                                   1);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_RXXG_BIT_CFG_1_PASS_CTRL_MSK,
                                   1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;


    disCrcChk = (mode == 0) ? 1 : 0;

    addr = HYPHY20G_XAUI_RXXG_REG_CFG_2;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_RXXG_BIT_CFG_2_DIS_CRC_CHK_MSK,
                                   disCrcChk);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_RXXG_BIT_CFG_2_SHORTPKT_EN_MSK,
                                   1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* configure rx max frame size */
    rxMaxFr = HYPHY20G_PIF_XAUI_RXXG_RECOMMEND_RX_MAXFR;
    addr = HYPHY20G_XAUI_RXXG_REG_MAX_FRM_LEN;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                                   HYPHY20G_XAUI_RXXG_BIT_MAX_FRM_LEN_MAXFR_MSK,
                                   rxMaxFr);
    if (result) return result;

    /* configure cut through threshold */
    cutThruThSel = 0x0;
    addr = HYPHY20G_XAUI_RXXG_REG_CUT_THRU_TH_SEL;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                  HYPHY20G_XAUI_RXXG_BIT_CUT_THRU_TH_SEL_CUT_THRU_THRES_SEL_MSK,
                  cutThruThSel);
    if (result) return result;


    /*
     * configure the RXOAM
     */
    addr = HYPHY20G_XAUI_RXOAM_REG_CFG_2;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 0x3,
            HYPHY20G_XAUI_RXOAM_BIT_CFG_2_BYPASS_MODE_MSK,
            HYPHY20G_XAUI_RXOAM_BIT_CFG_2_BYPASS_MODE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            (0x0001 << 9),
            9);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;


    /*
     * configure XAUI header
     */
     txInsCh = (mode == 0) ? 1 : 0;
     txInsS  = (mode == 0) ? 1 : 0;
     rxUsrCh = (mode == 0) ? 0 : 1;
     rxExtCh = (mode == 0) ? 1 : 0;
     rxExtS  = (mode == 0) ? 1 : 0;

    addr = HYPHY20G_XAUI_TOP_REG_HDR_CFG;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_TOP_BIT_HDR_CFG_TX_INS_CH_MSK,
                                   txInsCh);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_TOP_BIT_HDR_CFG_TX_INS_S_MSK,
                                   txInsS);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_TOP_BIT_HDR_CFG_RX_USR_CH_MSK,
                                   rxUsrCh);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_TOP_BIT_HDR_CFG_RX_EXT_CH_MSK,
                                   rxExtCh);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_TOP_BIT_HDR_CFG_RX_EXT_S_MSK,
                                   rxExtS);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;


    /*
     * configure BIP control
     */
    addr = HYPHY20G_XAUI_TOP_REG_BIP_ERR_CNTR_CFG;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                        HYPHY20G_XAUI_TOP_BIT_BIP_ERR_CNTR_CFG_CNT_MODE_MSK, 1);
    if(result) return result;

    bip8Strip  = (mode == 0) ? 1 : 0;
    bip8Verify = (mode == 0) ? 1 : 0;
    bip8Insert = (mode == 0) ? 1 : 0;

    addr = HYPHY20G_XAUI_TOP_REG_BIP_CTL;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_TOP_BIT_BIP_CTL_BIP8_STRIP_MSK,
                                   bip8Strip);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_TOP_BIT_BIP_CTL_BIP8_VERIFY_MSK,
                                   bip8Verify);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&val,
                                   HYPHY20G_XAUI_TOP_BIT_BIP_CTL_BIP8_INSERT_MSK,
                                   bip8Insert);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;


    /*
     * enable the transmitter by setting TXEN0 to b'1
     */
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(HYPHY20G_XAUI_TXXG_REG_CFG_1,
                                               slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_XAUI_TXXG_BIT_CFG_1_TXEN0_OFF, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* pifHyPhy20gPifXauiInit */


/*******************************************************************************
**
**  hyPhy20gPifSpiFcCalInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initialize the SPI4.2 Flow Control calendar,
**                  and configure the calendar to the specified length, then
**                  initially program the entries to NULL flow control port.
**
**                  This function is only valid for SPI4.2 flow control mode.
**
**  INPUTS:         fileHndl  - device handle
**                  dir       - determine the flow control calendar
**                              0 - Tx flow control(flow control to HyPHY 20G)
**                              1 - Rx flow control(flow control from HyPHY 20G)
**                  calLen    - Calendar length to be configured
**                              1 to 256 - specific the length of calendar
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifSpiFcCalInit(struct file *fileHndl, UINT4 dir, UINT4 calLen)
{
    INT4 result;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_PIF_FC_CAL *pCal = NULL;
    UINT4 i;

    /* argument checking */
    if (dir > 1 || calLen > 0xFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL) && (pDevCtxt->pPifCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    if (dir == HYPHY20G_TX_DIRECTION) {
        pCal= pDevCtxt->pPifCtxt->pFcTxCal;
    } else {
        pCal= pDevCtxt->pPifCtxt->pFcRxCal;
    }

    for (i = 0; i < calLen; i++) {
        *(pCal->onlineCalMem + i) = HYPHY20G_PIF_XAUI_FC_CAL_NULL_CHNL;
    }

    /* save calendar attribute to the context memory */
    pCal->calLen = calLen;

    /* setup calendar length and programe calendar memory */
    result = pifHyPhy20gPifSpiFcCalProg(fileHndl, dir, calLen, pCal->onlineCalMem);
    if (result) return result;

    /* double write the calendar to init both page 0 and page 1 */
    result = pifHyPhy20gPifSpiFcCalProg(fileHndl, dir, calLen, pCal->onlineCalMem);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifSpiFcCalInit */


/*******************************************************************************
**
**  hyPhy20gPifSpiFcPortMap
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the mapping of a SPI4.2
**                  flow control calendar entry to a flow control port.
**
**                  Alternatively, this function can remove a entry from the
**                  SPI4.2 flow control calendar by reducing calendar length.
**
**                  This function is only valid for SPI4.2 flow control mode.
**
**
**  INPUTS:         fileHndl  - device handle
**                  dir       - determine the flow control calendar
**                              0 - Tx flow control(flow control to HyPHY 20G)
**                              1 - Rx flow control(flow control from HyPHY 20G)
**                  enable    - 1 - mapping a channel's flow control port to
**                                  the calendar address
**                              0 - remove all calendar entires associated
**                                  with the flow control port
**                  calAddr   - Calendar address to be programmed
**                              0 to 0xFF - dependent on the length of calendar
**                              Not used when enable == 0
**                  fcPort    - Flow control port to be associated with the
**                              calendar address
**                              0 to 149 - Flow control port address
**                              150 - Link Level Flow Control Port
**                              151 - Null Flow Control Port - always report
**                                    XOFF.
**                              152 - remove the calendar entry which is
**                                    specified by calAddr
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifSpiFcPortMap(struct file *fileHndl, UINT4 dir, UINT4 enable,
                             UINT4 calAddr, UINT4 fcPort)
{
    INT4 result;
    UINT4 calLen;
    UINT4 offlinePage;
    UINT4 calState, calStatOff, calStatMsk;
    UINT4 i, addr, val;
    UINT4 startAddr;
    UINT4 calData[HYPHY20G_PIF_XAUI_FC_TXCAL_MAXLEN];
    sHYPHY20G_PIF_FC_CAL *pCal;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (dir > 1 || enable > 1 || fcPort > 153 || calAddr > 0xFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL) && (pDevCtxt->pPifCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    if (dir) {
        pCal = pDevCtxt->pPifCtxt->pFcRxCal;
        addr = HYPHY20G_PIF_PIF_REG_RX_CLDR_CFG;
        calStatOff = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_CAL_STATE_OFF;
        calStatMsk = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_CAL_STATE_MSK;

    } else {
        pCal = pDevCtxt->pPifCtxt->pFcTxCal;
        addr = HYPHY20G_PIF_PIF_REG_TX_CLDR_CFG;
        calStatOff = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_CAL_STATE_OFF;
        calStatMsk = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_CAL_STATE_MSK;
    }

    calLen = pCal->calLen;

    /* read out the cal state and determine which page is offline page */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    calState = mHYPHY20G_GET_FIELD4(val, calStatMsk, calStatOff);

    /* if CAL_STATE is b'00, this decode still treat cal 0 as online page
       and treat cal 1 as the offline page */
    if (calState == 0x0 || calState == 0x2) {
        /* locations corresponding to calendar 0 */
        offlinePage = 0;
        startAddr   = 0x100;
    } else {
        /* locations corresponding to calendar 1 */
        offlinePage = 1;
        startAddr   = 0x00;
    }

    DBG_PRINT("%s, %d, startAddr=%lx offlinePage=%ld\n",__FUNCTION__, __LINE__, startAddr, offlinePage);

    /* retrieve calendar data */
    sysHyPhy20gMemCpy(calData, pCal->onlineCalMem, calLen*sizeof(UINT4));

    /* map or remove a fcPort */
    for (i = 0; i < calLen; i++) {
        if (enable && i == calAddr) {
            /* mapping a fcPort to a calendar entry */
            *(calData + i) = fcPort;
        }

        if ((!enable) && (*(calData + i) == fcPort)) {
            /* 'remove' all calendar entry associated with this fcPort */
            *(calData + i) = HYPHY20G_PIF_XAUI_FC_CAL_NULL_CHNL;
        }
    }

    DBG_PRINT("%s, %d, Dump cal memory to be programmed, calLen=%ld\n", __FUNCTION__, __LINE__, calLen);
    for (i = 0; i < calLen; i++) {
        DBG_PRINT("i=%ld *(calData+i)=%ld\n", i, *(calData+i));
    }

    /* apply calendar length and program calendar memory */
    result = pifHyPhy20gPifSpiFcCalProg(fileHndl, dir, calLen, calData);
    if (result) return result;

    /* save calendar data to SW context memory */
    sysHyPhy20gMemCpy(pCal->onlineCalMem, calData, calLen*sizeof(UINT4));

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifSpiFcPortMap */


/*******************************************************************************
**
**  pifHyPhy20gPifSpiFcCalCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures parity scheme, DIP-2 insertion,
**                  calendar length, and number of iterations for the
**                  SPI4.2 flow control calendar.
**
**                  This function is only valid for SPI4.2 flow control mode.
**
**
**  INPUTS:         fileHndl  - device handle
**                  dir       - 0 - Transmit Flow Control calendar
**                              1 - Receive Flow Control calendar
**                  parity    - specify data protection parity scheme
**                              0 - Odd parity
**                              1 - Even parity
**                  dip2En    - enable/disable DIP-2 insertion
**                              0 - disable DIP-2 protection
**                              1 - enable DIP-2 protection
**                  iterationCnt  - 0x0 - Check for DIP-2 after 256 cycles
**                                  0x1 - Check for DIP-2 after each cycle
**                                  0xn - Check for DIP-2 after n cycles
**                  calAddr   - Calendar length to be configured
**                              0 to 0xFF - specific the length of calendar
**                  updateMsk - Mask of SPI4.2 flow control calendar related
**                              configuration field to updated.
**                              Set the corresponding bit to 1 to update a
**                              field, set to 0 to maintain value of configured
**                              field:
**                              bit 3 - update FCDP_PARITY field
**                              bit 2 - update DIP2_EN field
**                              bit 1 - update TX_ITERATION_CNT field
**                              bit 0 - update ACTIVE_CAL_LEN field
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gPifSpiFcCalCfg(struct file *fileHndl, UINT4 dir,
                               UINT4 parity, UINT4 dip2En,
                               UINT4 iterationCnt, UINT4 calLen,
                               UINT4 updateMsk)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 parityOff, parityMsk, dip2EnOff, dip2EnMsk;
    UINT4 iterationOff, iterationMsk, calLenOff, calLenMsk;

    /* argument checking */
    if (dir > 1 || parity > 1 || dip2En > 1 ||
        iterationCnt > 0xFF || calLen > 0xFF || updateMsk > 0xF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (dir == HYPHY20G_TX_DIRECTION) {
        addr = HYPHY20G_PIF_PIF_REG_TX_CLDR_CFG;
        parityOff = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_FCDP_PARITY_OFF;
        parityMsk = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_FCDP_PARITY_MSK;
        dip2EnOff = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_DIP2_EN_OFF;
        dip2EnMsk = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_DIP2_EN_MSK;
        iterationOff = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_ITERATION_CNT_OFF;
        iterationMsk = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_ITERATION_CNT_MSK;
        calLenOff = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_ACTIVE_CAL_LEN_OFF;
        calLenMsk = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_ACTIVE_CAL_LEN_MSK;
    } else {
        addr = HYPHY20G_PIF_PIF_REG_RX_CLDR_CFG;

        parityOff = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_FCDP_PARITY_OFF;
        parityMsk = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_FCDP_PARITY_MSK;
        dip2EnOff = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_DIP2_EN_OFF;
        dip2EnMsk = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_DIP2_EN_MSK;
        iterationOff = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_ITERATION_CNT_OFF;
        iterationMsk = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_ITERATION_CNT_MSK;
        calLenOff = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_ACTIVE_CAL_LEN_OFF;
        calLenMsk = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_ACTIVE_CAL_LEN_MSK;
    }

    /* Set the CAL_LEN, ITERATION_CNT, DIP2_EN, FCDP_PARITY */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    if (updateMsk & 0x8) {
        val = mHYPHY20G_SET_FIELD4(val, parity, parityMsk, parityOff);
    }

    if (updateMsk & 0x4) {
        val = mHYPHY20G_SET_FIELD4(val, dip2En, dip2EnMsk, dip2EnOff);
    }

    if (updateMsk & 0x2) {
        val = mHYPHY20G_SET_FIELD4(val, iterationCnt,
                                   iterationMsk, iterationOff);
    }

    if (updateMsk & 0x1) {
        val = mHYPHY20G_SET_FIELD4(val, calLen, calLenMsk, calLenOff);
    }

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* pifHyPhy20gPifSpiFcCalCfg */


/*******************************************************************************
**
**  pifHyPhy20gPifSpiFcCalProg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function write the formatted calendar data
**                  a specific flow control calendar memory's offline page,
**                  then configure the new calendar length, and finally switch
**                  the offline page to active.
**
**                  The active page and offline page are determined
**                  by decoding register bits.
**
**  INPUTS:         fileHndl  - device handle
**                  dir       - 0 - Transmit Flow Control calendar
**                              1 - Receive Flow Control calendar
**                  calLen    - calendar length
**                  *pCalData - pointer to formatted calendar data
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gPifSpiFcCalProg(struct file *fileHndl, UINT4 dir, UINT4 calLen,
                                const UINT4 *pCalData)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 i, offlinePage;
    UINT4 calState, calStatOff, calStatMsk;
    UINT4 selOff, reqOff, reqMsk;
    UINT4 startAddr, endAddr;

    /* argument checking */
    if (dir > 1 || calLen < 1 || calLen > HYPHY20G_PIF_XAUI_FC_TXCAL_MAXLEN) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (dir == HYPHY20G_TX_DIRECTION) {
        addr = HYPHY20G_PIF_PIF_REG_TX_CLDR_CFG;
        calStatOff = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_CAL_STATE_OFF;
        calStatMsk = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_CAL_STATE_MSK;
        selOff = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_CAL_SEL_OFF;
        reqOff = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_CAL_ACTIVE_REQ_OFF;
        reqMsk = HYPHY20G_PIF_PIF_BIT_TX_CLDR_CFG_TX_CAL_ACTIVE_REQ_MSK;
    } else {
        addr = HYPHY20G_PIF_PIF_REG_RX_CLDR_CFG;
        calStatOff = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_CAL_STATE_OFF;
        calStatMsk = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_CAL_STATE_MSK;
        selOff = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_CAL_SEL_OFF;
        reqOff = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_CAL_ACTIVE_REQ_OFF;
        reqMsk = HYPHY20G_PIF_PIF_BIT_RX_CLDR_CFG_RX_CAL_ACTIVE_REQ_MSK;
    }

    /* Determine which page is offline page */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    calState = mHYPHY20G_GET_FIELD4(val, calStatMsk, calStatOff);

    /* when CAL_STATE is b'00, this decode still treat cal 0 as online page
       and treat cal 1 as the offline page */
    if (calState == 0x0 || calState == 0x2) {
        /* locations corresponding to calendar 0 */
        offlinePage = 0;
        startAddr   = 0x00;
        endAddr     = startAddr + calLen - 1;
    } else {
        /* locations corresponding to calendar 1 */
        offlinePage = 1;
        startAddr   = 0x100;
        endAddr     = startAddr + calLen - 1;
    }

    DBG_PRINT("%s, %d calState=%ld offlinePage=%ld startAddr=%ld endAddr=%ld\n",__FUNCTION__, __LINE__, calState, offlinePage, startAddr, endAddr);

    /* Program the offline calendar RAM page */
    for (i = startAddr; i <= endAddr; i++) {
        if (dir == HYPHY20G_TX_DIRECTION) {
            result = pifHyPhy20gTXCalProgIndWrite(fileHndl, i,
                                                  *(pCalData + i - startAddr));
        } else {
            result = pifHyPhy20gRXCalProgIndWrite(fileHndl, i,
                                                  *(pCalData + i - startAddr));
        }
        if (result) return result;
    }

    DBG_PRINT("%s, %d Program the offline calendar done!\n",__FUNCTION__, __LINE__);

    /* Set the CAL_LEN */
    val = (calLen == HYPHY20G_PIF_XAUI_FC_TXCAL_MAXLEN) ? 0 : calLen;
    result = pifHyPhy20gPifSpiFcCalCfg(fileHndl, dir,
                                       0, 0, 0, val, 0x1);
    if (result) return result;

    /*
     * switch the offline page to ative
     */
    /* Select the offline page by writing into CAL_SEL */
    result = sysHyPhy20gBitWrite(fileHndl, addr, selOff, offlinePage);
    if (result) return result;

    /* Active the offline page by setting CAL_ACTIVE_REQ */
    result = sysHyPhy20gBitWrite(fileHndl, addr, reqOff, HYPHY20G_ENABLE);
    if (result) return result;

    /* Wait for the page to be made active by polling CAL_ACTIVE_REQ */
    result = sysHyPhy20gPollBit(fileHndl, addr, reqMsk, 0,
                                HYPHY20G_DELAY_20USEC);
    if (result) return result;

    DBG_PRINT("%s, %d switch the offline page to ative done!\n",__FUNCTION__, __LINE__);

    return HYPHY20G_SUCCESS;

} /* pifHyPhy20gPifSpiFcCalProg */

/*******************************************************************************
**
**  pifHyPhy20gPifGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the size of PIF context.
**
**
**  INPUTS:         fileHndl      - device handle
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The size of PIF context
**
*******************************************************************************/
UINT4 pifHyPhy20gPifGetCtxtSize(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_PIF_CTXT  *pPifCtxt;
    sHYPHY20G_SCHD_CTXT *pSchdCtxt;
    UINT4 size = 0, i = 0;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    pPifCtxt = pDevCtxt->pPifCtxt;

    DBG_PRINT("%s, %s, %d, 1st offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, size);
    
	/* include the ctxt header size */
    size += utilHyPhy20gGetCtxtHdrSize();
	
	/* calculate the context size of PIF subsystem */
    if (pPifCtxt != NULL) {
	    /* calculate the first level structure size of PIF subsystem */
	    size += sizeof(sHYPHY20G_PIF_CTXT);
    
	    DBG_PRINT("%s, %s, %d, 2nd FC_CAL offset 0x%lx\n", 
	                __FILE__, __FUNCTION__, __LINE__, size);

	    /* calculate the second level size of PIF_FC_CAL */
	    /* both the tx and rx */
	    /* include the ctxt header size */
	    size += utilHyPhy20gGetCtxtHdrSize() * 2;
	    size += sizeof(sHYPHY20G_PIF_FC_CAL) * 2;

	    /* calculate the second level size of SCHD */
	    /* both the tx schd#1 and schd#2 */
	    for (i = 0; i < 2; i++) {
	        DBG_PRINT("%s, %s, %d, 2nd SCHD %ld offset 0x%lx\n", 
	                    __FILE__, __FUNCTION__, __LINE__, i, size);
        
	        pSchdCtxt = (i == 0)?(pPifCtxt->pTxSchdCtxt0):(pPifCtxt->pTxSchdCtxt1);
	        if (pSchdCtxt != NULL) {
	            /* include the ctxt header size */
	            size += utilHyPhy20gGetCtxtHdrSize();
	            size += sizeof(sHYPHY20G_SCHD_CTXT);

	            /* calculate the third level size */
	            /* pPifCtxt->pTxSchdCtxtx->pScbsGrpCtxt, 
	            pPifCtxt->pTxSchdCtxtx->pOnLineCalMem, 
	            pPifCtxt->pTxSchdCtxtx->pOffLineCalMem
	            */
	            size += utilHyPhy20gScbsGetSchdCtxtSize(pSchdCtxt);
	        }
	    }
	}
    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, size);

    return size;
}/* pifHyPhy20gPifGetCtxtSize */

/*******************************************************************************
**
**  pifHyPhy20gPifExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the PIF context into pre-allocated buffer.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-allocated buffer for PIF context extraction
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
INT4 pifHyPhy20gPifExtractCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_PIF_CTXT  *pPifCtxt;
    sHYPHY20G_PIF_FC_CAL *pFcCal;
    sHYPHY20G_SCHD_CTXT *pSchdCtxt;
    INT4 result;
    UINT4 rtLen;
    UINT4 offset = 0, len = 0, i = 0;
    
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

    pPifCtxt = pDevCtxt->pPifCtxt;

    DBG_PRINT("%s, %s, %d, 1st offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

	/* insert ctxt buffer header */
    len = (pPifCtxt == NULL) ? sizeof(sHYPHY20G_CTXT_HDR) : 
			(sizeof(sHYPHY20G_CTXT_HDR) + sizeof(sHYPHY20G_PIF_CTXT));
    rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                        HYPHY_PIF_CTXT_ID, HYPHY_NO_INDEX);
    HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");  
    offset += sizeof(sHYPHY20G_CTXT_HDR);

    if (pPifCtxt != NULL) {
	    /* extract the first level context of PIF subsystem */
	    /* copy the pure payload into the buffer */
	    sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
	                    (void *)pPifCtxt, sizeof(sHYPHY20G_PIF_CTXT));
	    offset += sizeof(sHYPHY20G_PIF_CTXT);

	    /* extract the second level context of PIF_FC_CAL */
	    for (i = 0; i < 2; i++) {
	        DBG_PRINT("%s, %s, %d, 2nd FC_CAL %ld offset 0x%lx\n", 
	                    __FILE__, __FUNCTION__, __LINE__, i, offset);

	        len = sizeof(sHYPHY20G_CTXT_HDR) +  
	            sizeof(sHYPHY20G_PIF_FC_CAL);

	        /* insert ctxt buffer header */
	        rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
	                                    HYPHY_PIF_FC_CAL_CTXT_ID, i);
	        HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
	            "%s", "pCtxtBuf is NULL.");  
	        offset += sizeof(sHYPHY20G_CTXT_HDR);

	        /* copy the pure payload into the buffer */
	        pFcCal = (i == 0)?(pPifCtxt->pFcRxCal):(pPifCtxt->pFcTxCal);
	        sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
	                        (void *)(pFcCal), sizeof(sHYPHY20G_PIF_FC_CAL));
	        offset += sizeof(sHYPHY20G_PIF_FC_CAL);
	    }

	    /* extract the second level context of SCHD */
	    for (i = 0; i < 2; i++) {
	        DBG_PRINT("%s, %s, %d, 2nd SCHD %ld offset 0x%lx\n", 
	                    __FILE__, __FUNCTION__, __LINE__, i, offset);

	        pSchdCtxt = (i == 0)?(pPifCtxt->pTxSchdCtxt0):(pPifCtxt->pTxSchdCtxt1);
	        if (pSchdCtxt != NULL) {
	            len = sizeof(sHYPHY20G_CTXT_HDR) +  
	                sizeof(sHYPHY20G_SCHD_CTXT);

	            /* insert ctxt buffer header */
	            rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
	                                        HYPHY_SCHD_CTXT_ID, HYPHY_NO_INDEX);
	            HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
	                "%s", "pCtxtBuf is NULL.");  
	            offset += sizeof(sHYPHY20G_CTXT_HDR);

	            /* copy the pure payload into the buffer */
	            sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
	                            (void *)(pSchdCtxt), sizeof(sHYPHY20G_SCHD_CTXT));
	            offset += sizeof(sHYPHY20G_SCHD_CTXT);
	        }
	    }

	    /* extract the third level context of scbs */
	    for (i = 0; i < 2; i++) {
	        DBG_PRINT("%s, %s, %d, 3rd scbs %ld offset 0x%lx\n", 
	                    __FILE__, __FUNCTION__, __LINE__, i, offset);
        
	        pSchdCtxt = (i == 0)?(pPifCtxt->pTxSchdCtxt0):(pPifCtxt->pTxSchdCtxt1);
	        if (pSchdCtxt != NULL) {
	            /* extract pPifCtxt->pTxSchdCtxt->pScbsGrpCtxt, 
	            pPifCtxt->pTxSchdCtxt->pOnLineCalMem, 
	            and pPifCtxt->pTxSchdCtxt->pOffLineCalMem context */
	            result = utilHyPhy20gScbsExtractSchdCtxt(pSchdCtxt, 
	                                    ((UINT1 *)pCtxtBuf + offset), &len);
	            HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
	                            "%s", "utilHyPhy20gScbsExtractSchdCtxt failed.");
	            offset += len;
	        }
	    }
	}
	
    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pExtractedSize = offset;
    
    return HYPHY20G_SUCCESS;
}/* pifHyPhy20gPifExtractCtxt */

/*******************************************************************************
**
**  pifHyPhy20gPifRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the PIF context into the driver context memory.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-stored PIF context for restoration
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
INT4 pifHyPhy20gPifRestoreCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_PIF_CTXT  *pPifCtxt;
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    sHYPHY20G_PIF_FC_CAL *pFcCal;
    sHYPHY20G_SCHD_CTXT *pSchdCtxt;
    INT4 result;
    UINT4 mode;
    UINT4 *p4 = NULL;
    UINT4 offset = 0, len = 0, i = 0;
    
    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pParsedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pParsedSize is NULL.");

    /* try to retrieve the PIF header in the pre-stored context buffer */
    pCtxtBufHdr = utilHyPhy20gSearchCtxtHdr(pCtxtBuf, 
                                HYPHY_PIF_CTXT_ID, HYPHY_NO_INDEX);
    if (pCtxtBufHdr == NULL) {
        DBG_PRINT("%s, %s, %d, can not find HYPHY_PIF_CTXT_ID hdr\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    pCtxtBuf = (void *)pCtxtBufHdr;
    
    /* check if the payload is valid */
    if (pCtxtBufHdr->len == sizeof(sHYPHY20G_CTXT_HDR)) {
        offset = sizeof(sHYPHY20G_CTXT_HDR);
    } else if (pCtxtBufHdr->len > sizeof(sHYPHY20G_CTXT_HDR)) {
        /* offset pCtxtBuf to figure out the payload position */
        offset += sizeof(sHYPHY20G_CTXT_HDR);

        /* retrieve the valid flag of the pre-stored context */
        /* retrieve the mode of the pre-stored context */
        p4 = (UINT4 *)((UINT1 *)pCtxtBuf + offset);
        mode = *(++p4);

        /* create PIF context */
        result = pifHyPhy20gPifCreateCtxt(fileHndl, mode);
        if (result) {
            DBG_PRINT("%s, %s, %d, pifHyPhy20gPifCreateCtxt failed\n", 
                        __FILE__, __FUNCTION__, __LINE__);
            *pParsedSize = 0;
            return result;
        }
        
        /* retrieve device context */
        pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
        /* confirm device context pointer is valid before processing */
        HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                        "%s", "pDevCtxt is NULL.");
        
        pPifCtxt = pDevCtxt->pPifCtxt;

        /* restore the first level context for PIF subsystem */
        /* not copy the unuseful pointer value from the pre-stored data */
        len = offsetof(sHYPHY20G_PIF_CTXT, pFcRxCal);
        sysHyPhy20gMemCpy((void *)pPifCtxt, 
                        ((UINT1 *)pCtxtBuf + offset), len);
        
        /* finish restore the first level context for PIF subsystem */
        len = sizeof(sHYPHY20G_PIF_CTXT);
        offset += len;

        /* restore the second level context of PIF_FC_CAL */
        for (i = 0; i < 2; i++) {
            DBG_PRINT("%s, %s, %d, 2nd FC_CAL %ld offset 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, i, offset);
            
            /* try to retrieve the PIF_FC_CAL header in the pre-stored context buffer */
            pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
            if (pCtxtBufHdr->id != HYPHY_PIF_FC_CAL_CTXT_ID) {
                *pParsedSize = 0;
                pifHyPhy20gPifDestroyCtxt(fileHndl, 1);
                return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
            }
            HYPHY20G_ASSERT((pCtxtBufHdr->len >= sizeof(sHYPHY20G_CTXT_HDR)), 
                            HYPHY20G_ERR_DEV_CTXT_CORRUPTED,
                            "%s", "HYPHY20G_ERR_DEV_CTXT_CORRUPTED.");

            /* offset pCtxtBuf to figure out the payload position */
            offset += sizeof(sHYPHY20G_CTXT_HDR);

            /* payload len */
            len = pCtxtBufHdr->len - 
                    sizeof(sHYPHY20G_CTXT_HDR);

            pFcCal = (i == 0)?(pPifCtxt->pFcRxCal):(pPifCtxt->pFcTxCal);
            sysHyPhy20gMemCpy((void *)(pFcCal), 
                            ((UINT1 *)pCtxtBuf + offset), len);
            offset += len;
        }

        /* restore the second level context of SCHD */
        /* note that pTxSchdCtxt0 and/or pTxSchdCtxt1 was already allocated 
        by pifHyPhy20gPifCreateCtxt with pre-stored mode */
        for (i = 0; i < 2; i++) {
            DBG_PRINT("%s, %s, %d, 2nd SCHD %ld offset 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, i, offset);

            pSchdCtxt = (i == 0)?(pPifCtxt->pTxSchdCtxt0):(pPifCtxt->pTxSchdCtxt1);
            if (pSchdCtxt != NULL) {
                /* try to retrieve the SCHD header in the pre-stored context buffer */
                pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
                if (pCtxtBufHdr->id != HYPHY_SCHD_CTXT_ID) {
                    *pParsedSize = 0;
                    pifHyPhy20gPifDestroyCtxt(fileHndl, 1);
                    return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
                }
                HYPHY20G_ASSERT((pCtxtBufHdr->len >= sizeof(sHYPHY20G_CTXT_HDR)), 
                                HYPHY20G_ERR_DEV_CTXT_CORRUPTED,
                                "%s", "HYPHY20G_ERR_DEV_CTXT_CORRUPTED.");

                /* offset pCtxtBuf to figure out the payload position */
                offset += sizeof(sHYPHY20G_CTXT_HDR);

                /* payload len */
                len = pCtxtBufHdr->len - 
                        sizeof(sHYPHY20G_CTXT_HDR);

                sysHyPhy20gMemCpy((void *)pSchdCtxt, 
                                ((UINT1 *)pCtxtBuf + offset), len);
                offset += len;
            }
        }

        /* restore the third level context of SCBS */
        for (i = 0; i < 2; i++) {
            DBG_PRINT("%s, %s, %d, 3rd scbs %ld offset 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, i, offset);
            
            pSchdCtxt = (i == 0) ? (pPifCtxt->pTxSchdCtxt0) : (pPifCtxt->pTxSchdCtxt1);
            if (pSchdCtxt != NULL) {
                /* restore pPifCtxt->pTxSchdCtxt->pScbsGrpCtxt, 
                    pPifCtxt->pTxSchdCtxt->pOnLineCalMem, 
                    and pPifCtxt->pTxSchdCtxt->pOffLineCalMem context */
                result = utilHyPhy20gScbsRestoreSchdCtxt(pSchdCtxt, 
                                                ((UINT1 *)pCtxtBuf + offset), &len);
                if (result) {
                    DBG_PRINT("%s, %s, %d, utilHyPhy20gScbsRestoreSchdCtxt failed\n", 
                                __FILE__, __FUNCTION__, __LINE__);
                    pifHyPhy20gPifDestroyCtxt(fileHndl, 1);
                    *pParsedSize = 0;
                    return result;
                }
                offset += len;
            }
        }

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
}/* pifHyPhy20gPifRestoreCtxt */

/*******************************************************************************
**
**  pifHyPhy20gPifAuditTxChnlCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit the TxChnl in the PIF subCtxt.
**
**                  PRIVATE function called by pifHyPhy20gPifAuditCtxt.
**
**  INPUTS:         fileHndl - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_FAILURE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
PRIVATE INT4 pifHyPhy20gPifAuditTxChnlCtxt(struct file *fileHndl)
{  
    UINT4 i,totalChnlRate0=0, totalChnlRate1=0, pifMode;
    sHYPHY20G_CTXT *pDevCtxt = NULL;
    sHYPHY20G_PIF_CTXT *pPifCtxt = NULL;
    sHYPHY20G_PIF_TX_CHNL_CTXT *pPifTxChnlCtxt = NULL;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt == NULL."); 
    
    pPifCtxt = pDevCtxt->pPifCtxt;
    if (pPifCtxt==NULL)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    pifMode = pPifCtxt->mode;
    
    pPifTxChnlCtxt = pPifCtxt->txChnlCtxt;

    /* audit rule according to cpbHyPhy20gPifInitCtxt and hyPhy20gPifTxChnlEnable function */
    for (i=0;i<HYPHY20G_PIF_TX_SCHD_MAX_CHNL;i++)
    {
        if (pPifTxChnlCtxt->valid!=HYPHY20G_VALID_SIGN)
        {
            if ((pPifTxChnlCtxt->rate0!=0)||(pPifTxChnlCtxt->rate1!=0)||
                (pPifTxChnlCtxt->state!=HYPHY20G_PIF_CHNL_START)||(pPifTxChnlCtxt->intf!=HYPHY20G_INVALID_VAL_32BIT))
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
            else 
                return HYPHY20G_SUCCESS;
        }
        else
        {
            if ((pPifTxChnlCtxt->chnlId!=i)||(pPifTxChnlCtxt->rate0!=pPifTxChnlCtxt->rate1))
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
            if ((pPifTxChnlCtxt->rate0<1)||(pPifTxChnlCtxt->rate0>12000)||
                (pPifTxChnlCtxt->rate1<1)||(pPifTxChnlCtxt->rate1>12000)||
                (pPifTxChnlCtxt->intf<1)||(pPifTxChnlCtxt->intf>2)||
                (pPifTxChnlCtxt->state!=HYPHY20G_PIF_CHNL_OPERATIONAL))
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }

            if((pPifTxChnlCtxt->intf & 0x1) == ((HYPHY20G_PIF_TX_SCHD_0 & 0x0000FFFF) + 1))
            {
                if ((pifMode == HYPHY20G_PIF_OPMODE_EXAUI0) ||(pifMode == HYPHY20G_PIF_OPMODE_SXAUI0) ||
                    (pifMode == HYPHY20G_PIF_OPMODE_INTERLAKEN_INBAND) || (pifMode == HYPHY20G_PIF_OPMODE_INTERLAKEN_OOB)||
                    (pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI) || (pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI))
                    totalChnlRate0 += pPifTxChnlCtxt->rate0;
                else
                {
                    DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_FAILURE;
                }
            }

            if((pPifTxChnlCtxt->intf & 0x2) == ((HYPHY20G_PIF_TX_SCHD_1 & 0x0000FFFF) + 1))
            {
                if ((pifMode == HYPHY20G_PIF_OPMODE_EXAUI1) ||(pifMode == HYPHY20G_PIF_OPMODE_SXAUI1) ||
                    (pifMode == HYPHY20G_PIF_OPMODE_DUAL_EXAUI) || (pifMode == HYPHY20G_PIF_OPMODE_DUAL_SXAUI))
                    totalChnlRate1 += pPifTxChnlCtxt->rate0;
                else
                {
                    DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_FAILURE;
                }
            }
        }
        
        pPifTxChnlCtxt++;
    }

	return HYPHY20G_SUCCESS;
} /* pifHyPhy20gPifAuditTxChnlCtxt */


/*******************************************************************************
**
**  pifHyPhy20gPifAuditCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit the PIF subCtxt.
**
**                  PRIVATE function called by the hyPhy20gAuditCtxt
**
**  INPUTS:         fileHndl - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_FAILURE  
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifHyPhy20gPifAuditCtxt(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt = NULL;
    sHYPHY20G_PIF_CTXT *pPifCtxt = NULL;
    UINT4 mode=0;
    UINT4 lowPwrBit=0;
    INT4 result;
    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt == NULL."); 

    /* audit pPifCtxt first level attributes according to hyPhy20gPifInit,
    pifHyPhy20gPifCreateCtxt and pifHyPhy20gPifDestroyCtxt functions */
    pPifCtxt = pDevCtxt->pPifCtxt;

    result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_3,
                             HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_PIF_LOWPWR_OFF,
                             &lowPwrBit);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                    "%s", "sysHyPhy20gBitRead fail."); 

    /*PIF module disabled */
    if (lowPwrBit == 1)
    {
        if (pPifCtxt!=NULL)
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
        else
            return HYPHY20G_SUCCESS;
    }

    /*PIF module enabled */
    if (pPifCtxt==NULL)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    mode=pPifCtxt->mode;

    if ((pPifCtxt->valid != HYPHY20G_VALID_SIGN)||
        (mode >HYPHY20G_PIF_OPMODE_DUAL_SXAUI))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    switch (mode)
    {
        case HYPHY20G_PIF_OPMODE_EXAUI0:
        case HYPHY20G_PIF_OPMODE_SXAUI0:
        case HYPHY20G_PIF_OPMODE_INTERLAKEN_INBAND:
        case HYPHY20G_PIF_OPMODE_INTERLAKEN_OOB:
            if ((pPifCtxt->pFcRxCal==NULL)||(pPifCtxt->pFcTxCal==NULL)||
                (pPifCtxt->pTxSchdCtxt0==NULL)||(pPifCtxt->pTxSchdCtxt1!=NULL))
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
            break;
        case HYPHY20G_PIF_OPMODE_EXAUI1:
        case HYPHY20G_PIF_OPMODE_SXAUI1:
            if ((pPifCtxt->pFcRxCal==NULL)||(pPifCtxt->pFcTxCal==NULL)||
                (pPifCtxt->pTxSchdCtxt0!=NULL)||(pPifCtxt->pTxSchdCtxt1==NULL))
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
            break;
        case HYPHY20G_PIF_OPMODE_DUAL_EXAUI:
        case HYPHY20G_PIF_OPMODE_DUAL_SXAUI:
            if ((pPifCtxt->pFcRxCal==NULL)||(pPifCtxt->pFcTxCal==NULL)||
                (pPifCtxt->pTxSchdCtxt0==NULL)||(pPifCtxt->pTxSchdCtxt1==NULL))
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
            break;
        case HYPHY20G_PIF_OPMODE_DISABLE:
            /* No need to check further since the mode is DISABLE*/
            return HYPHY20G_SUCCESS;
            break;
        default:
            /* Actually the branch will not be touched when "mode" value is not corrupted in the memory */ 
            HYPHY20G_ASSERT((mode <= HYPHY20G_PIF_OPMODE_DUAL_SXAUI), HYPHY20G_FAILURE,
                             "%s", "mode value is out of valid range."); 
            break;
    }

    /* audit txChnlCtxt */
    result=pifHyPhy20gPifAuditTxChnlCtxt(fileHndl);
    if (result!= HYPHY20G_SUCCESS)
    {
        return result;
    }
    DBG_PRINT("%s, %s, %d, pifHyPhy20gPifAuditTxChnlCtxt(fileHndl) SUCCESS in pifHyPhy20gPifAuditCtxt.\n",__FILE__, __FUNCTION__, __LINE__);

    /* audit *pFcRxCal and *pFcTxCal  */
    if ((pPifCtxt->pFcRxCal->calLen>256)||(pPifCtxt->pFcTxCal->calLen>256))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* audit *pTxSchdCtxt0 and *pTxSchdCtxt1 */ 
    if (pPifCtxt->pTxSchdCtxt0!=NULL)
    {
        result = utilHyPhy20gScbsAuditSchdCtxt(fileHndl,pPifCtxt->pTxSchdCtxt0);
        if (result!= HYPHY20G_SUCCESS)
        {
            return result;
        }
    }
    DBG_PRINT("%s, %s, %d, utilHyPhy20gScbsAuditSchdCtxt(fileHndl,pPifCtxt->pTxSchdCtxt0) SUCCESS in pifHyPhy20gPifAuditCtxt.\n",__FILE__, __FUNCTION__, __LINE__);

    if (pPifCtxt->pTxSchdCtxt1!=NULL)
    {
        result = utilHyPhy20gScbsAuditSchdCtxt(fileHndl,pPifCtxt->pTxSchdCtxt1);
        if (result!= HYPHY20G_SUCCESS)
        {
            return result;
        }
    }
    DBG_PRINT("%s, %s, %d, utilHyPhy20gScbsAuditSchdCtxt(fileHndl,pPifCtxt->pTxSchdCtxt1) SUCCESS in pifHyPhy20gPifAuditCtxt.\n",__FILE__, __FUNCTION__, __LINE__);

    return HYPHY20G_SUCCESS;
} /* pifHyPhy20gPifAuditCtxt */

/* end of file */
