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
**  FILE        : hyphy20g_util_intlkn.c
**
**  $Date: 2012-03-22 11:24:00 +0800 (Thu, 22 Mar 2012) $
**
**  $Revision: 21218 $
**
**  DESCRIPTION : Contains the HyPHY 20G utility functions for Interlaken
**                blocks.
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_util_intlkn.h"

/******************************************************************************/
/* PUBLIC FUNCTIONS - Interlaken Operations                                   */
/******************************************************************************/

/*******************************************************************************
**
**  utilHyPhy20gIntlknCoreEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function controls the setting of INTLKN_SOFT_RST,
**                  RX_SOFT_RST and TX_SOFT_RST to configure the 'Major
**                  Bidirectional States' of the Interlaken block required for
**                  block configuration and operation.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - 0 - OPI CBRI #0 slice
**                              1 - OPI CBRI #1 slice
**                              2 - PIF Interlaken slice
**                  enable    - Major Bidirectional State:
**                              1 - in reset (disable)
**                              2 - standby (configuration registers accessible)
**                              3 - enable (configuration registers not
**                                          accessible)
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gIntlknCoreEnable(struct file *fileHndl, UINT4 slice,
                                  UINT4 enable)
{
    INT4 result;
    UINT4 addr, regData;
    UINT4 intlknSftRstMsk, rxSftRstMsk, txSftRstMsk;
    UINT4 intlknSftRstVal, rxSftRstVal, txSftRstVal;

    /* argument checking */
    if ((slice > HYPHY20G_INTLKN_PIF_INTLKN) ||
        (enable > HYPHY20G_INTLKN_STATE_OPERATIONAL)||
        (enable < HYPHY20G_INTLKN_STATE_INRESET)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (slice == HYPHY20G_INTLKN_PIF_INTLKN) {
        addr = HYPHY20G_PIF_INTLKN_SHIM_REG_SW_RST;
        intlknSftRstMsk =
                        HYPHY20G_PIF_INTLKN_SHIM_BIT_SW_RST_INTLKN_SOFT_RST_MSK;
        rxSftRstMsk = HYPHY20G_PIF_INTLKN_SHIM_BIT_SW_RST_RX_SOFT_RST_MSK;
        txSftRstMsk = HYPHY20G_PIF_INTLKN_SHIM_BIT_SW_RST_TX_SOFT_RST_MSK;

    } else {
        addr = HYPHY20G_CBRI_SHIM_REG_SW_RST;
        addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);
        intlknSftRstMsk = HYPHY20G_CBRI_SHIM_BIT_SW_RST_INTLKN_SOFT_RST_MSK;
        rxSftRstMsk = HYPHY20G_CBRI_SHIM_BIT_SW_RST_RX_SOFT_RST_MSK;
        txSftRstMsk = HYPHY20G_CBRI_SHIM_BIT_SW_RST_TX_SOFT_RST_MSK;
    }

    /* read the register data */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* modify the register data for the required 'bidirectional mode' */
    if (enable == HYPHY20G_INTLKN_STATE_OPERATIONAL) { /* set to operational */
        intlknSftRstVal = 0;
        rxSftRstVal = 0;
        txSftRstVal = 0;
    } else if (enable == HYPHY20G_INTLKN_STATE_STANDBY){ /* set to standby */
        intlknSftRstVal = 0;
        rxSftRstVal = 1;
        txSftRstVal = 1;
    } else {                /* put to reset */
        intlknSftRstVal = 1;
        rxSftRstVal = 1;
        txSftRstVal = 1;
    }

    result = hyPhy20gVarFieldWrite (&regData, intlknSftRstMsk, intlknSftRstVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, rxSftRstMsk, rxSftRstVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, txSftRstMsk, txSftRstVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gIntlknCoreEnable */

/*******************************************************************************
**
**  utilHyPhy20gIntlknCoreGetMajBiDirState
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves the 'Major
**                  Bidirectional States' of the Interlaken block required for
**                  block configuration and operation.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - 0 - OPI CBRI #0 slice
**                              1 - OPI CBRI #1 slice
**                              2 - PIF Interlaken slice
**                  pState    - State of reset bits
**                              0 - invalid state
**                              1 - State 1
**                              2 - State 2
**                              3 - State 3
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gIntlknCoreGetMajBiDirState(struct file *fileHndl, UINT4 slice,
                                            UINT4 *pState)
{
    INT4 result;
    UINT4 addr, data;
    UINT4 intlknSftRstMsk, rxSftRstMsk, txSftRstMsk;
    UINT4 intlknSftRstOff, rxSftRstOff, txSftRstOff;
    UINT4 intlknSftRstVal, rxSftRstVal, txSftRstVal;

    /* argument checking */
    if ((slice > HYPHY20G_INTLKN_PIF_INTLKN)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (slice == HYPHY20G_INTLKN_PIF_INTLKN) {
        addr = HYPHY20G_PIF_INTLKN_SHIM_REG_SW_RST;
        intlknSftRstMsk =
                        HYPHY20G_PIF_INTLKN_SHIM_BIT_SW_RST_INTLKN_SOFT_RST_MSK;
        rxSftRstMsk = HYPHY20G_PIF_INTLKN_SHIM_BIT_SW_RST_RX_SOFT_RST_MSK;
        txSftRstMsk = HYPHY20G_PIF_INTLKN_SHIM_BIT_SW_RST_TX_SOFT_RST_MSK;

        intlknSftRstOff =
                        HYPHY20G_PIF_INTLKN_SHIM_BIT_SW_RST_INTLKN_SOFT_RST_OFF;
        rxSftRstOff = HYPHY20G_PIF_INTLKN_SHIM_BIT_SW_RST_RX_SOFT_RST_OFF;
        txSftRstOff = HYPHY20G_PIF_INTLKN_SHIM_BIT_SW_RST_TX_SOFT_RST_OFF;

    } else {
        addr = HYPHY20G_CBRI_SHIM_REG_SW_RST;
        addr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr, slice);
        intlknSftRstMsk = HYPHY20G_CBRI_SHIM_BIT_SW_RST_INTLKN_SOFT_RST_MSK;
        rxSftRstMsk = HYPHY20G_CBRI_SHIM_BIT_SW_RST_RX_SOFT_RST_MSK;
        txSftRstMsk = HYPHY20G_CBRI_SHIM_BIT_SW_RST_TX_SOFT_RST_MSK;

        intlknSftRstOff = HYPHY20G_CBRI_SHIM_BIT_SW_RST_INTLKN_SOFT_RST_OFF;
        rxSftRstOff = HYPHY20G_CBRI_SHIM_BIT_SW_RST_RX_SOFT_RST_OFF;
        txSftRstOff = HYPHY20G_CBRI_SHIM_BIT_SW_RST_TX_SOFT_RST_OFF;
    }

    /* read the register data */
    result = sysHyPhy20gRead(fileHndl, addr, &data);
    if(result) return result;

    /* extract reset bit settings */
    intlknSftRstVal = mHYPHY20G_GET_FIELD4(data, intlknSftRstMsk,
                                          intlknSftRstOff);
    rxSftRstVal = mHYPHY20G_GET_FIELD4(data, rxSftRstMsk, rxSftRstOff);
    txSftRstVal = mHYPHY20G_GET_FIELD4(data, txSftRstMsk, txSftRstOff);

    if((rxSftRstVal==1)&&(txSftRstVal==1)&&(intlknSftRstVal==1)){
        *pState = HYPHY20G_INTLKN_STATE_INRESET;
    } else if((rxSftRstVal==1)&&(txSftRstVal==1)&&(intlknSftRstVal==0)) {
        *pState = HYPHY20G_INTLKN_STATE_STANDBY;
    } else if((rxSftRstVal==0)&&(txSftRstVal==0)&&(intlknSftRstVal==0)) {
        *pState = HYPHY20G_INTLKN_STATE_OPERATIONAL;
    }
    else{
        *pState = 0;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gIntlknCoreGetMajBiDirState */

/*******************************************************************************
**
**  utilHyPhy20gIntlknCoreCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    The caller function must ensure the that interlaken block is
**                  in 'Major Bidirectional States' of 2
**                  in order for this function to be executed.
**
**                  It performs the following operations:
**                  1. Enables data flow to SERDES links
**                  2. Disable Loopbacks
**                  3. Transmit FIFO Configuration
**                  4. Burst Setting Configuration
**                           BURST_SHORT - 32 bytes
**                           BURST_MIN - 64 bytes
**                           BURST_MAX - 256 bytes
**                  5. Metaframe Synchronization Configuration
**                           2048 bytes
**                  6. Disable SERDES Test Pattern Generation/Reception
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - 0 - OPI CBRI #0 slice
**                              1 - OPI CBRI #1 slice
**                              2 - PIF Interlaken slice
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gIntlknCoreCfg(struct file *fileHndl, UINT4 slice)
{
    UINT4 i;
    const UINT4 RXTX_SLICE_OFF = 0X0200;
    UINT4 lane, lowWM, highWM, burstMax, burstMin, burstShort, period;
    UINT4 laneDisReg, cfgReg, brstCfgReg, mfrmSyncReg;
    UINT4 disableMsk, wmHighMsk, wmLowMsk;
    UINT4 burstMaxMsk, burstMinMsk, burstShortMsk, mfrmSyncPeriodMsk;
    INT4 result;


    /* argument checking */
    if (slice > HYPHY20G_INTLKN_PIF_INTLKN) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    lane = 0; /* variable for DISABLE field setting */
    lowWM = 0x4; /* variable for WM_LOW field setting*/
    highWM = 0x3; /* variable for WM_HIGH field setting*/
    burstMax = 0x3; /* variable for BURSTMAX field setting 256 bytes */
    burstMin = 0x01; /* variable for BURSTMIN field setting 64 bytes */
    burstShort = 0; /* variable for BURSTSHORT field setting 32 bytes*/
    period = HYPHY20G_INTLKN_META_FRAME_PEROD; /* variable for PERIOD field
                                                  setting 2048 bytes*/

    if (slice == HYPHY20G_INTLKN_PIF_INTLKN) {
        /* lane disable */
        laneDisReg =
            HYPHY20G_PIF_INTLKN_CORE_REG_LANE_DIS_OFFSET(HYPHY20G_TX_DIRECTION);
        disableMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_LANE_DIS_DISABLE_MSK;

        /* tx fifo watermark */
        cfgReg = HYPHY20G_PIF_INTLKN_CORE_REG_TX_FIFO_CFG;
        wmHighMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_TX_FIFO_CFG_WM_HIGH_MSK;
        wmLowMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_TX_FIFO_CFG_WM_LOW_MSK;

        /* burst config */
        brstCfgReg =
            HYPHY20G_PIF_INTLKN_CORE_REG_BRST_CFG_OFFSET(HYPHY20G_TX_DIRECTION);
        burstMaxMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_BRST_CFG_BURSTMAX_MSK;
        burstMinMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_BRST_CFG_BURSTMIN_MSK;
        burstShortMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_BRST_CFG_BURSTSHORT_MSK;

        /* metaframe config */
        mfrmSyncReg =
           HYPHY20G_PIF_INTLKN_CORE_REG_MFRM_SYNC_OFFSET(HYPHY20G_TX_DIRECTION);
        mfrmSyncPeriodMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_MFRM_SYNC_PERIOD_MSK;
    } else {
        /* lane disable */
        laneDisReg =
                  HYPHY20G_CBRI_CORE_REG_LANE_DIS_OFFSET(HYPHY20G_TX_DIRECTION);
        laneDisReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(laneDisReg, slice);
        disableMsk = HYPHY20G_CBRI_CORE_BIT_LANE_DIS_DISABLE_MSK;

        /* tx fifo watermark */
        cfgReg = HYPHY20G_CBRI_CORE_REG_TX_FIFO_CFG;
        cfgReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(cfgReg, slice);
        wmHighMsk = HYPHY20G_CBRI_CORE_BIT_TX_FIFO_CFG_WM_HIGH_MSK;
        wmLowMsk = HYPHY20G_CBRI_CORE_BIT_TX_FIFO_CFG_WM_LOW_MSK;

        /* burst config */
        brstCfgReg =
                  HYPHY20G_CBRI_CORE_REG_BRST_CFG_OFFSET(HYPHY20G_TX_DIRECTION);
        brstCfgReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(brstCfgReg, slice);
        burstMaxMsk = HYPHY20G_CBRI_CORE_BIT_BRST_CFG_BURSTMAX_MSK;
        burstMinMsk = HYPHY20G_CBRI_CORE_BIT_BRST_CFG_BURSTMIN_MSK;
        burstShortMsk = HYPHY20G_CBRI_CORE_BIT_BRST_CFG_BURSTSHORT_MSK;

        /* metaframe config */
        mfrmSyncReg =
                 HYPHY20G_CBRI_CORE_REG_MFRM_SYNC_OFFSET(HYPHY20G_TX_DIRECTION);
        mfrmSyncReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(mfrmSyncReg, slice);
        mfrmSyncPeriodMsk = HYPHY20G_CBRI_CORE_BIT_MFRM_SYNC_PERIOD_MSK;
    }

    /* Disable Interlaken Core Loopbacks */
    result = utilHyPhy20gIntlknCoreSetLoopback(fileHndl, slice, 0);
    if (result) return result;

    /* Transmit FIFO WaterMark Configuration
       set WM_LOW to 0x4 and WM_HIGH to 0x3. */
    result = hyPhy20gRegFieldWrite(fileHndl, cfgReg, wmHighMsk, highWM);
    if (result) return result;
    result = hyPhy20gRegFieldWrite(fileHndl, cfgReg, wmLowMsk, lowWM);
    if (result) return result;

    for (i = HYPHY20G_TX_DIRECTION; i <= HYPHY20G_RX_DIRECTION; i++) {
        /* Enable Data Flow to SERDES Links for Rx and Tx both */
        cfgReg = cfgReg + RXTX_SLICE_OFF * i;
        result = hyPhy20gRegFieldWrite(fileHndl, laneDisReg, disableMsk, lane);
        if (result) return result;

        /* Burst Configuration */
        brstCfgReg = brstCfgReg + RXTX_SLICE_OFF * i;

        result = hyPhy20gRegFieldWrite(fileHndl, brstCfgReg, burstMaxMsk,
                                       burstMax);
        if (result) return result;

        result = hyPhy20gRegFieldWrite(fileHndl, brstCfgReg, burstMinMsk,
                                       burstMin);
        if (result) return result;

        result = hyPhy20gRegFieldWrite(fileHndl, brstCfgReg, burstShortMsk,
                                       burstShort);
        if (result) return result;


        /* Meta Frame Synchronization Configuration */
        mfrmSyncReg = mfrmSyncReg + RXTX_SLICE_OFF * i;
        result = hyPhy20gRegFieldWrite(fileHndl, mfrmSyncReg, mfrmSyncPeriodMsk,
                                       period);
        if (result) return result;

        /* Disable SERDES Test Pattern Generation/Reception */
        result = utilHyPhy20gIntlknCoreSerdesPtrnCfg(fileHndl, slice, i,
                                                     HYPHY20G_DISABLE);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gIntlknCoreCfg */

/*******************************************************************************
**
**  utilHyPhy20gIntlknCoreTxRateLimitCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configure the optional transmit rate limiting
**                  feature for one of the Interlaken blocks.  Transmit rate
**                  limiting sets the maximum rate for data to be transferred
**                  across the transmit interface to the downstream device.
**
**                  The calling function must ensure the that slice is
**                  'Major Bidirectional States' of 2
**                  in order for this function to be executed.
**
**                  BURSTMAX must be configured prior to calling this function.
**                  BURSTMAX is configured when calling
**                  utilHyPhy20gIntlknCoreCfg.
**
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - 0 - OPI CBRI #0 slice
**                              1 - OPI CBRI #1 slice
**                              2 - PIF Interlaken slice
**                  rateLimit  - Maximum Rate data may be transferred across
**                               transmit interface to downstream device.
**                               0 - disabled no rate limiting applied - maximum
**                                   transfer rate supported.
**                               1 to m - amount of rate limiting applied in
**                                        1 Gbps steps (125 MBps), where m is
**                                        the maxiumum number of 1 Gpbs steps
**                                        before exceeding the rate of the
**                                        interface.
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
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gIntlknCoreTxRateLimitCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 rateLimit, UINT4 burstLimit,
                                          UINT4 byteGran)
{
    INT4 result;
    UINT4 addr[3], val, offset, msk[6];
    UINT4 enable, rlRate, rlDelta, creditMax, burstMax, burstMaxBytes;
    UINT4 coreFreq;
    UINT4 reg, regMsk;
    UINT4 coreClk1Sel, div4DivEn, mode6_25G;

    /* argument checking */
    if ((slice > HYPHY20G_INTLKN_PIF_INTLKN) ||
        (byteGran > 5) ||
        (rateLimit > 50)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }


    if (slice == HYPHY20G_INTLKN_PIF_INTLKN) {
        /* retrieve Serdes Rate */
        reg = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_REG_CLK_SEL;
        regMsk = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_BIT_CLK_SEL_CORE_CLK1_SEL_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, reg, regMsk, &coreClk1Sel);
        if(result) return result;

        reg = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_13;
        regMsk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_13_DIV4_DIV_EN_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, reg, regMsk, &div4DivEn);
        if(result) return result;

        if (coreClk1Sel == 0 && div4DivEn == 1) {
            /* 6.25G mode  CORE_CLK1_SEL = 0
                           DIV4_DIV_EN = 1
            */
            mode6_25G = 1;
        } else if (coreClk1Sel == 1 && div4DivEn == 0) {
            /* 3.125G mode CORE_CLK1_SEL = 1
                           DIV4_DIV_EN = 0
            */
            mode6_25G = 0;
        } else {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }

        addr[0] = HYPHY20G_PIF_INTLKN_CORE_REG_TX_RT_LMT_0;
        addr[1] = HYPHY20G_PIF_INTLKN_CORE_REG_TX_RT_LMT_1;
        addr[2] =
            HYPHY20G_PIF_INTLKN_CORE_REG_BRST_CFG_OFFSET(HYPHY20G_TX_DIRECTION);

        offset = HYPHY20G_PIF_INTLKN_CORE_BIT_TX_RT_LMT_0_RL_ENABLE_OFF;

        msk[0] = HYPHY20G_PIF_INTLKN_CORE_BIT_TX_RT_LMT_0_CREDIT_MAX_MSK;
        msk[1] = HYPHY20G_PIF_INTLKN_CORE_BIT_TX_RT_LMT_0_BYTE_GRAN_MSK;
        msk[2] = HYPHY20G_PIF_INTLKN_CORE_BIT_TX_RT_LMT_1_RL_RATE_MSK;
        msk[3] = HYPHY20G_PIF_INTLKN_CORE_BIT_TX_RT_LMT_1_RL_DELTA_MSK;
        msk[4] = HYPHY20G_PIF_INTLKN_CORE_BIT_BRST_CFG_BURSTMAX_MSK;
        msk[5] = HYPHY20G_PIF_INTLKN_CORE_BIT_BRST_CFG_BURSTMAX_OFF;

        rlRate = 0x400;
        rlDelta = 0;

        if (mode6_25G == 1) {
            /* 6.25G mode */
            coreFreq = 390625; /* 390625 kbits per second for 6.25G Serdes */
        } else {
            /* 3.125G mode */
            coreFreq = 312500; /* 312500 kbits per second for 3.125G Serdes */
        }

    } else {
        addr[0] = HYPHY20G_CBRI_CORE_REG_TX_RT_LMT_0;
        addr[0] = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr[0], slice);
        addr[1] = HYPHY20G_CBRI_CORE_REG_TX_RT_LMT_1;
        addr[1] = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr[1], slice);
        addr[2] = HYPHY20G_CBRI_CORE_REG_BRST_CFG_OFFSET(HYPHY20G_TX_DIRECTION);
        addr[2] = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(addr[2], slice);

        offset = HYPHY20G_CBRI_CORE_BIT_TX_RT_LMT_0_RL_ENABLE_OFF;

        msk[0] = HYPHY20G_CBRI_CORE_BIT_TX_RT_LMT_0_CREDIT_MAX_MSK;
        msk[1] = HYPHY20G_CBRI_CORE_BIT_TX_RT_LMT_0_BYTE_GRAN_MSK;
        msk[2] = HYPHY20G_CBRI_CORE_BIT_TX_RT_LMT_1_RL_RATE_MSK;
        msk[3] = HYPHY20G_CBRI_CORE_BIT_TX_RT_LMT_1_RL_DELTA_MSK;
        msk[4] = HYPHY20G_CBRI_CORE_BIT_BRST_CFG_BURSTMAX_MSK;
        msk[5] = HYPHY20G_CBRI_CORE_BIT_BRST_CFG_BURSTMAX_OFF;

        rlRate = 0x400;
        rlDelta = 0;
        coreFreq = 400000; /* kbits per second */
    }

    /* enable rate limiting operations */
    if (rateLimit) {
        /* retrieve BURSTMAX for the interlaken slice */
        result = sysHyPhy20gRead(fileHndl, addr[2], &val);
        if(result) return result;

        burstMax = mHYPHY20G_GET_FIELD4(val, msk[4], msk[5]);

        switch(burstMax){
            case 0:
                burstMaxBytes = 64;
                break;
            case 1:
                burstMaxBytes = 128;
                break;
            case 2:
                burstMaxBytes = 192;
                break;
            case 3:
                burstMaxBytes = 256;
                break;
            case 4:
                burstMaxBytes = 320;
                break;
            case 5:
                burstMaxBytes = 384;
                break;
            case 6:
                burstMaxBytes = 448;
                break;
            case 7:
                burstMaxBytes = 512;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }

        /* return an error if burstLimit <= burstMaxBytes and if burstLimit -
           burstMaxBytes is less than or equal 96 */
        if((burstLimit <= burstMaxBytes)||
           ((burstLimit - burstMaxBytes) <= 96)){
            return HYPHY20G_ERR_INVALID_ARG;
        }

        /* calculate CREDIT_MAX field setting */
        creditMax = burstLimit - burstMaxBytes - 96;

        /* calculate RL_DELTA and RL_RATE */
        /* Transmit Bandwidth (bytes/s) = CORE_FREQ*(RL_DELTA/RL_RATE)
           Transmit Bandwidth (bits/s) = 8*CORE_FREQ*(RL_DELTA/RL_RATE)
           rateLimit is the transmit bandwidth in Gbps
           coreFreq is the CORE_FREQ
           RL_DELTA must be less than or equal to CREDIT_MAX
           Need to solve for RL_DELTA and RL_RATE, starting with
           RL_DELTA = CREDIT_MAX;

           rlRate = 8*coreFreq*rlDelta/(rateLimit*10^6)
        */

        rlDelta = creditMax;
        rlRate = 8*coreFreq*rlDelta; /* calculate numerator */
        /* if numerator is less than denominator, rlRate will be 0, in this
           case, burstLimit is too small for the system to achieve the
           desired rate limiting setting */
        if((rlRate < (rateLimit*1000000))||
           ((burstLimit - burstMaxBytes) <= 96)){
            return HYPHY20G_ERR_INVALID_ARG;
        }
        rlRate = rlRate/rateLimit; /* divide by denominator less the Gbps to
                                      kbps conversion */
        rlRate = rlRate/1000000; /* divide by denonimator for Gbps to kpbs
                                    conversion */

        val = 0;
        /* Set RL_RATE field */
        result = hyPhy20gVarFieldWrite(&val, msk[2], rlRate);
        if(result) return result;

        /* Set RL_DELTA field */
        result = hyPhy20gVarFieldWrite(&val, msk[3], rlDelta);
        if(result) return result;

        /* write RL_RATE and RL_DELTA to register */
        result = sysHyPhy20gWrite(fileHndl, addr[1], val);
        if(result) return result;

        /* read TRANSMIT RATE LIMITER 0 register */
        result = sysHyPhy20gRead(fileHndl, addr[0], &val);
        if(result) return result;

        /* Set CREDIT_MAX field */
        result = hyPhy20gVarFieldWrite(&val, msk[0], creditMax);
        if(result) return result;

        /* Set BYTE_GRAN field */
        result = hyPhy20gVarFieldWrite(&val, msk[1], byteGran);
        if(result) return result;

        /* write CREDIT_MAX and BYTE_GRAN to register */
        result = sysHyPhy20gWrite(fileHndl, addr[0], val);
        if(result) return result;

    }

    enable = (rateLimit == 0) ? HYPHY20G_DISABLE : HYPHY20G_ENABLE;

    /* Set RL_ENABLE bit */
    result = sysHyPhy20gBitWrite(fileHndl, addr[0], offset, enable);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gIntlknCoreTxRateLimitCfg */

/*******************************************************************************
**
**  utilHyPhy20gIntlknCoreLaneDisable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures an Interlaken slice to disable
**                  SERDES lanes
**
**                  The calling function must ensure the that slice is in
**                  'Major Bidirectional States' of 2 for this function
**                  to be executed.
**
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - 0 - OPI CBRI #0 slice
**                               1 - OPI CBRI #1 slice
**                               2 - PIF Interlaken slice
**                  disableMsk - For OPI and PIF slices:
**                               8 bits one per SERDES lane set to 1 to disable
**                               set to 0 to enable.  Legal range 0 to 0xFF
**
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gIntlknCoreLaneDisable(struct file *fileHndl, UINT4 slice,
                                       UINT4 disableMsk)
{
    INT4 result;
    UINT4 txReg, rxReg;
    UINT4 disOffset, disMsk;

    /* argument checking */
    if (slice > HYPHY20G_INTLKN_PIF_INTLKN || disableMsk > 0xFF) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (slice == HYPHY20G_INTLKN_PIF_INTLKN) {
        txReg = HYPHY20G_PIF_INTLKN_CORE_REG_LANE_DIS_OFFSET(0);
        rxReg = HYPHY20G_PIF_INTLKN_CORE_REG_LANE_DIS_OFFSET(1);
        disOffset = HYPHY20G_PIF_INTLKN_CORE_BIT_LANE_DIS_DISABLE_OFF;
        disMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_LANE_DIS_DISABLE_MSK;
    } else {
        txReg = HYPHY20G_CBRI_CORE_REG_LANE_DIS_OFFSET(0);
        txReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(txReg, slice);
        rxReg = HYPHY20G_CBRI_CORE_REG_LANE_DIS_OFFSET(1);
        rxReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(rxReg, slice);
        disOffset = HYPHY20G_CBRI_CORE_BIT_LANE_DIS_DISABLE_OFF;
        disMsk = HYPHY20G_CBRI_CORE_BIT_LANE_DIS_DISABLE_MSK;
    }

    /* apply DISABLE bit settings for TX and RX */
    result = hyPhy20gRegFieldWrite(fileHndl, txReg, disMsk, disableMsk);
    if (result) return result;

    result = hyPhy20gRegFieldWrite(fileHndl, rxReg, disMsk, disableMsk);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gIntlknCoreLaneDisable */

/*******************************************************************************
**
**  utilHyPhy20gIntlknCoreFcModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures an Interlaken slice for in-band or
**                  out-of-band flow control reporting for the Tx or Rx
**                  interface.
**
**                  The in-band flow control may be programmed to generate
**                  flow control data using an in-order calendar or a
**                  programmable calendar.
**
**                  Out-of-band flow control (only valid for PIF) uses
**                  programmable calendar.
**
**                  When programmable inband/out-of-band calendar mode of
**                  operation is selected, the programmable calendar is
**                  initialized so that all entries are mapped to the NULL flow
**                  control port.
**
**                  The calling function must ensure the that slice is in
**                  'Major Bidirectional States' of 2 for this function
**                  to be executed.
**
**
**  INPUTS:         *fileHndl  - base address
**                  slice     - 0 - OPI CBRI #0 slice
**                              1 - OPI CBRI #1 slice
**                              2 - PIF Interlaken slice
**                  dir        - 0 - Transmit (flow control sourced by HyPHY)
**                               1 - Receive (flow control terminated by HyPHY)
**                  mode       - 0 - in-band Interlaken flow control:
**                                   In-order flow control calendar
**                               1 - in-band Interlaken flow control:
**                                   Programmable calendar
**                               2 - out-of-band Interlaken flow control
**                                   (this mode is only valid for PIF)
**                  ctxt       - For mode == 0:
**                               0 - In-order flow control calendar without link
**                                   level flow control
**                               1 - In-order flow control calendar with link
**                                   level flow control
**                               For mode == 1 and slice == 0 or 1:
**                               1 to 16 - Number of flow control channel
**                                         entries reported or received in
**                                         Interlaken Idle and Burst Control
**                                         Words.
**                               For mode == 1 or 2 and slice == 2:
**                               1 to 256 - Number of flow control channel
**                                         entries reported or received in
**                                         Interlaken Idle and Burst Control
**                                         Words.
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gIntlknCoreFcModeCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 dir, UINT4 mode, UINT4 ctxt)
{
    INT4 result;
    UINT4 cfgReg, calEnReg;
    UINT4 progMsk, lastMsk, inBandOff;
    UINT4 outBandOff = HYPHY20G_PIF_INTLKN_CORE_BIT_CFG_OUTBAND_OFF;
    UINT4 outBand, inBand;
    UINT4 prog = 0;
    UINT4 last = 0;
    UINT4 i;

    /* argument checking */
    if (slice > HYPHY20G_INTLKN_PIF_INTLKN || mode > 2 || dir > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    } else if (slice == 0 || slice == 1) {
        if ((mode == 0 && ctxt > 1) ||
            (mode == 1 && (ctxt < 1 || ctxt > 16))||
            (mode == 2)) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else if (slice == 2) {
        if ((mode == 0 && ctxt > 1) ||
            ((mode == 1 || mode == 2) && (ctxt < 1 || ctxt > 256))) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    if (slice == HYPHY20G_INTLKN_PIF_INTLKN) {
        cfgReg = HYPHY20G_PIF_INTLKN_CORE_REG_CFG_OFFSET(dir);
        inBandOff = HYPHY20G_PIF_INTLKN_CORE_BIT_CFG_INBAND_OFF;

        calEnReg = HYPHY20G_PIF_INTLKN_CORE_REG_CLDR_EN_OFFSET(dir);
        progMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_CLDR_EN_PROG_OFF;
        lastMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_CLDR_EN_LAST_MSK;


    } else {
        cfgReg = HYPHY20G_CBRI_CORE_REG_CFG_OFFSET(dir);
        cfgReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(cfgReg, slice);
        inBandOff = HYPHY20G_CBRI_CORE_BIT_CFG_INBAND_OFF;

        calEnReg = HYPHY20G_CBRI_CORE_REG_CLDR_EN_OFFSET(dir);
        calEnReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(calEnReg, slice);
        progMsk = HYPHY20G_CBRI_CORE_BIT_CLDR_EN_PROG_OFF;
        lastMsk = HYPHY20G_CBRI_CORE_BIT_CLDR_EN_LAST_MSK;
    }

    /*
     * configure  INBAND bit and OUTBAND bit
     */
    if (mode == 2) {
        outBand = 1;     /* value for OUTBAND bit */
        inBand = 0;     /* value for INBAND bit */
    } else {
        outBand = 0;     /* value for OUTBAND bit */
        inBand = 1;     /* value for INBAND bit */
    }

    /* set OUTBAND bit only for PIF interface */
    if (slice == 2) {
        result = sysHyPhy20gBitWrite(fileHndl, cfgReg, outBandOff, outBand);
        if (result) return result;
    }
    result = sysHyPhy20gBitWrite(fileHndl, cfgReg, inBandOff, inBand);
    if (result) return result;

    /*
     * configure flow control calendar
     */
    if (mode == 0) {                /* In-order Calendar */
        prog = mode;
        if (slice == 2) {
            last = (ctxt == 0) ? 0x95 : 0x96;
        } else {
            last = (ctxt == 0) ? 0x7 : 0x8;
        }

    } else if (mode == 1 || mode == 2) {    /* Programmable Calendar */
        prog = 1;
        last = ctxt - 1;            /* Set this to calendar length minus 1 */
    }

    /* configure PROG bit*/
    result = sysHyPhy20gBitWrite(fileHndl, calEnReg, progMsk, prog);
    if (result) return result;

    /* configure LAST bits*/
    result = hyPhy20gRegFieldWrite(fileHndl, calEnReg, lastMsk, last);
    if (result) return result;

    /* initialize PIF programmable calendar entries to the null flow
       control port */
    if(((mode == 1)||(mode==2))&&(slice==2)){
        for(i=0; i <= last; i++){
            result = pifHyPhy20gIntlnkCalWrite(fileHndl, dir, i, 151);
            if(result) return result;
        }
    }

    /* initialize OPI programmable calendar entries to the NULL flow
       control port */
    if((mode == 1)&&((slice==0)||(slice==1))){
        for(i=0; i <= last; i++){
            result = cbriHyphy20gIntlnkCalWrite(fileHndl, slice, dir, i, 9);
            if(result) return result;
        }
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gIntlknCoreFcModeCfg */

/*******************************************************************************
**
**  utilHyPhy20gIntlknCoreFcCalCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the mapping of a flow control
**                  calendar entry to a flow control port for the transmit or
**                  receive calendar of a Interlaken instances.
**
**                  This function is only valid for a Interlaken instance that
**                  has been configured for programmable calendar mode of FC.
**
**                  More than one calendar entry may be associated with a flow
**                  control port.
**
**                  The Interlaken block may be in 'Major Bidirectional
**                  States' of 2 or 3 in order for this function to be executed,
**                  otherwise HYPHY20G_ERR_INVALID_PRECONFIG will be returned.
**
**                  Allowing 'Major Bidirectional States' of 3 allows for the
**                  flow control calendar to be dynamically updated while
**                  user data is being transferred across the interface.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - 0 - OPI CBRI #0 slice
**                               1 - OPI CBRI #1 slice
**                               2 - PIF Interlaken slice
**                  dir        - 0 - Transmit (flow control sourced by HyPHY)
**                               1 - Receive (flow control terminated by HyPHY)
**                  calAddr    - Calendar address to be configured
**                               when slice == 0 or 1:
**                               0 to 15 - dependent on the length of calendar
**                                         selected when programmable calendar
**                                         mode was configured
**                               when slice == 2:
**                               0 to 255 - dependent on the length of calendar
**                                         selected when programmable calendar
**                                         mode was configured
**                  fcPort     - Flow control port to be associated with the
**                               calendar address
**                               when OPI slice:
**                               0 to 7 - Flow control port address
**                               8 - Link Level Flow Control Port
**                               when PIF slice:
**                               9 - Null Flow Control Port (for dir == 1)
**                               0 to 149 - Flow control port address
**                               150 - Link Level Flow Control Port
**                               151 - Null Flow Control Port
**
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gIntlknCoreFcCalCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 dir, UINT4 calAddr, UINT4 fcPort)
{
//Gordon: there is no policy for how user sets up calendar, so we can only let them set it up in any way
    INT4 result;

    /* argument checking */
    if (slice > HYPHY20G_INTLKN_PIF_INTLKN || dir > HYPHY20G_RX_DIRECTION) {
        return HYPHY20G_ERR_INVALID_ARG;
    } else if (slice == 0 || slice == 1) {
        if ((calAddr > 15) ||
            ((fcPort > 8)&&(dir == 0))||
            ((fcPort > 9)&&(dir == 1))){
            return HYPHY20G_ERR_INVALID_ARG;
        }
    } else if (slice == 2) {
        if ((calAddr > 255) ||
            (fcPort > 151)) {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    if (slice == HYPHY20G_INTLKN_PIF_INTLKN) {
        result = pifHyPhy20gIntlnkCalWrite(fileHndl, dir, calAddr, fcPort);
        if(result) return result;
    } else {
        result = cbriHyphy20gIntlnkCalWrite(fileHndl, slice, dir, calAddr,
                                            fcPort);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gIntlknCoreFcCalCfg */



/*******************************************************************************
**
**  utilHyPhy20gIntlknCoreSerdesPtrnCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    None
**
//Gordon: Coded to disable SERDES test pattern.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice      - 0 - OPI CBRI #0 slice
**                               1 - OPI CBRI #1 slice
**                               2 - PIF Interlaken slice
**                  dir        - 0 - Transmit (data from HyPHY 20G)
**                               1 - Receive (data to HyPHY 20G)
**                  enable    - 0 - test pattern disabled
**                              1 - ?
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gIntlknCoreSerdesPtrnCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 dir, UINT4 enable)
{
    UINT4 tstCtlReg, progEnMsk, prbsEnMsk;
    INT4 result;

    /* argument checking */
    if (slice > HYPHY20G_INTLKN_PIF_INTLKN ||
        dir > 1 || enable >1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (slice == HYPHY20G_INTLKN_PIF_INTLKN) {

        tstCtlReg = HYPHY20G_PIF_INTLKN_CORE_REG_SERDES_TST_CTL_OFFSET(dir);
        progEnMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_SERDES_TST_CTL_PROG_EN_MSK;
        prbsEnMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_SERDES_TST_CTL_PRBS_EN_MSK;


    } else {
        tstCtlReg = HYPHY20G_CBRI_CORE_REG_SERDES_TST_CTL_OFFSET(dir);
        tstCtlReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(tstCtlReg, slice);

        progEnMsk = HYPHY20G_CBRI_CORE_BIT_SERDES_TST_CTL_PROG_EN_MSK;
        prbsEnMsk = HYPHY20G_CBRI_CORE_BIT_SERDES_TST_CTL_PRBS_EN_MSK;


    }

    if (enable == HYPHY20G_DISABLE) {

        /* configure PROG_EN bit*/
        result = hyPhy20gRegFieldWrite(fileHndl, tstCtlReg, progEnMsk,
                                       HYPHY20G_DISABLE);
        if (result) return result;

        /* configure PRBS_EN bit*/
        result = hyPhy20gRegFieldWrite(fileHndl, tstCtlReg, prbsEnMsk,
                                       HYPHY20G_DISABLE);
        if (result) return result;

    }
    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gIntlknCoreSerdesPtrnCfg */


/*******************************************************************************
**
**  utilHyPhy20gIntlknCoreSetLoopback
**  ___________________________________________________________________________
**
**  DESCRIPTION:    None
**
//Gordon: This function must disable loopbacks only control loopbacks in the CORE
// Ajay provided feedback that both M=0:1 (TX/RX) must be set to the same values
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - 0 - OPI CBRI #0 slice
**                              1 - OPI CBRI #1 slice
**                              2 - PIF Interlaken slice
**                  loopType  - 0 - all Core Loopbacks Disabled
**                              1 - ???????
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gIntlknCoreSetLoopback(struct file *fileHndl, UINT4 slice,
                                       UINT4 loopType)
{
    UINT4 cfgReg;
    UINT4 dataRxToTxLbMsk, dataTxToRxLbMsk;
    UINT4 fcRxToTxLbMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_CFG_FC_RX_TO_TX_LB_MSK;
    UINT4 fcTxToRxLbMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_CFG_FC_TX_TO_RX_LB_MSK;
    UINT4 regData;
    INT4 result;

    /* argument checking */
    if (slice > HYPHY20G_INTLKN_PIF_INTLKN) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (slice == HYPHY20G_INTLKN_PIF_INTLKN) {
        cfgReg = HYPHY20G_PIF_INTLKN_CORE_REG_CFG_OFFSET(HYPHY20G_TX_DIRECTION);
        dataRxToTxLbMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_CFG_DATA_RX_TO_TX_LB_MSK;
        dataTxToRxLbMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_CFG_DATA_TX_TO_RX_LB_MSK;
    } else {
        cfgReg = HYPHY20G_CBRI_CORE_REG_CFG_OFFSET(HYPHY20G_TX_DIRECTION);
        cfgReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(cfgReg, slice);
        dataRxToTxLbMsk = HYPHY20G_CBRI_CORE_BIT_CFG_DATA_RX_TO_TX_LB_MSK;
        dataTxToRxLbMsk = HYPHY20G_CBRI_CORE_BIT_CFG_DATA_TX_TO_RX_LB_MSK;
    }

    /* read the register data */
    result = sysHyPhy20gRead(fileHndl, cfgReg, &regData);
    if(result) return result;

    if(loopType==0){
        /* update flow control loopback for PIF only */
        if (slice == 2){
            result = hyPhy20gVarFieldWrite (&regData, fcRxToTxLbMsk, 0);
            if(result) return result;

            result = hyPhy20gVarFieldWrite (&regData, fcTxToRxLbMsk, 0);
            if(result) return result;
        }

        result = hyPhy20gVarFieldWrite (&regData, dataRxToTxLbMsk, 0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, dataTxToRxLbMsk, 0);
        if(result) return result;

        /* update TX register */
        result = sysHyPhy20gWrite(fileHndl, cfgReg, regData);
        if(result) return result;

        cfgReg += 0x0200; /* write identical data to RX register */
        result = sysHyPhy20gWrite(fileHndl, cfgReg, regData);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gIntlknCoreSetLoopback */


/*******************************************************************************
**
**  utilHyPhy20gIntlknShimSetLoopback
**  ___________________________________________________________________________
**
**  DESCRIPTION:    None
**
//Gordon: This function must disable loopbacks only control loopbacks in the SHIM
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - 0 - OPI CBRI #0 slice
**                              1 - OPI CBRI #1 slice
**                              2 - PIF Interlaken slice
**                  loopType  - 0 - all Shim Loopbacks Disabled
**                              1 - ???????
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gIntlknShimSetLoopback(struct file *fileHndl, UINT4 slice,
                                       UINT4 loopType)
{
    UINT4 coreLbReg;
    UINT4 extLbMsk, intLbMsk;
    UINT4 regData;
    INT4 result;

    /* argument checking */
    if (slice > HYPHY20G_INTLKN_PIF_INTLKN) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    coreLbReg = HYPHY20G_CBRI_SHIM_REG_CORE_LP_BACK;
    if (slice == HYPHY20G_INTLKN_PIF_INTLKN) {
        coreLbReg = HYPHY20G_PIF_INTLKN_SHIM_REG_CORE_LP_BACK;
        extLbMsk =
               HYPHY20G_PIF_INTLKN_SHIM_BIT_CORE_LP_BACK_EXTERNAL_LOOP_BACK_MSK;
        intLbMsk =
               HYPHY20G_PIF_INTLKN_SHIM_BIT_CORE_LP_BACK_INTERNAL_LOOP_BACK_MSK;
    } else {
        coreLbReg = HYPHY20G_CBRI_SHIM_REG_CORE_LP_BACK;
        coreLbReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(coreLbReg, slice);
        extLbMsk = HYPHY20G_CBRI_SHIM_BIT_CORE_LP_BACK_EXTERNAL_LOOP_BACK_MSK;
        intLbMsk = HYPHY20G_CBRI_SHIM_BIT_CORE_LP_BACK_INTERNAL_LOOP_BACK_MSK;
    }

    /* read the register data */
    result = sysHyPhy20gRead(fileHndl, coreLbReg, &regData);
    if(result) return result;

    if(loopType==0){
        result = hyPhy20gVarFieldWrite (&regData, extLbMsk, 0);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, intLbMsk, 0);
        if(result) return result;

        /* update TX register */
        result = sysHyPhy20gWrite(fileHndl, coreLbReg, regData);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gIntlknShimSetLoopback */

/*******************************************************************************
**
**  utilHyPhy20gIntlknShimCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Performs Interlaken Shim block initialization. Sets:
**                  - BURST_MAX to 256 bytes
**                  - Packet transfer mode for OPI instances or user
**                    configurable segment or packet transfer mode for PIF
**                  - Disables Loopbacks
**                  - User specified prebuffer FIFO low and high watermarks.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - 0 - OPI CBRI #0 slice
**                              1 - OPI CBRI #1 slice
**                              2 - PIF Interlaken slice
**                  mode      - only valid for PIF
**                              0 - segment mode
**                              1 - packet mode
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gIntlknShimCfg(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    UINT4 burstMax = 0x3; /* 256 byte BURST_MAX setting */
    UINT4 cfgReg, brstMaxMsk, modeMsk, ignoreOutBandFcMsk;
    INT4 result;

    /* argument checking */
    if (slice > HYPHY20G_INTLKN_PIF_INTLKN || mode > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (slice == HYPHY20G_INTLKN_PIF_INTLKN) {
        cfgReg = HYPHY20G_PIF_INTLKN_SHIM_REG_CFG;
        brstMaxMsk = HYPHY20G_PIF_INTLKN_SHIM_BIT_CFG_BURST_MAX_MSK;
        modeMsk = HYPHY20G_PIF_INTLKN_SHIM_BIT_CFG_MODE_MSK;
        ignoreOutBandFcMsk = HYPHY20G_PIF_INTLKN_SHIM_BIT_CFG_IGNORE_TX_OB_FC_MSK;

    } else {
        cfgReg = HYPHY20G_CBRI_SHIM_REG_CFG;
        brstMaxMsk = HYPHY20G_CBRI_SHIM_BIT_CFG_BURST_MAX_MSK;
        modeMsk = HYPHY20G_CBRI_SHIM_BIT_CFG_MODE_MSK;
        ignoreOutBandFcMsk = HYPHY20G_CBRI_SHIM_BIT_CFG_IGNORE_TX_OB_FC_MSK;

    }

    /* disable EXTERNAL_LOOP_BACK and INTERNAL_LOOP_BACK */
    result = utilHyPhy20gIntlknShimSetLoopback(fileHndl, slice, 0);
    if (result) return result;

    /* Set BURST_MAX, which must match the BURSTMAX setting in the Interlaken
       Core */
    result = hyPhy20gRegFieldWrite(fileHndl, cfgReg, brstMaxMsk, burstMax);
    if (result) return result;

    /* IGNORE_TX_OB_FC bit must be set to 1 because the default value of 
        CORE:OUTBAND bit is 0 */
    result = hyPhy20gRegFieldWrite(fileHndl, cfgReg, ignoreOutBandFcMsk, 1);
    if (result) return result;

    /* Set MODE to 1 for Packet Mode or 0 for Segment mode, which should
       match the MODE filed setting for PIF interface */
    if (slice != HYPHY20G_INTLKN_PIF_INTLKN) {
        /* always set MODE to 1 for Packet Mode for CBRI interface */
        mode = 1;
    }

    result = hyPhy20gRegFieldWrite(fileHndl, cfgReg, modeMsk, mode);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gIntlknShimCfg */

/* end of file */
