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
**  FILE        : hyphy20g_pif_status.c
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_pif_status.h"

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/


/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/
/*******************************************************************************
**
**  hyPhy20gPifTopLevelGetStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the PIF Top Level Status.
**
**                  This function is used for both eXAUI and Interlaken modes.
**
**  INPUTS:        *fileHndl   - device handle
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31:4 - RESERVED
**                                  Bit  3   - TX_OOB_DP_ERROR_V
**                                  Bit  2   - NACK_RCNT_V
**                                  Bit  1   - ACK_TIMEOUT_V
**                                  Bit  0   - RD_TIMEOUT_V
**
**  OUTPUTS:        None      - None
**
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifTopLevelGetStatus(struct file *fileHndl, UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, regData;

    /* Read Pif Top Level status */
    addr = HYPHY20G_PIF_PIF_REG_INT_VAL;
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    *statusBits = regData;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifTopLevelGetStatus */


/*******************************************************************************
**
**  hyPhy20gPifXauiGetStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the PIF eXAUI and its sub-block's
**                  status (interrupt values), including TXXG, XTEF, RXOAM
**                  and eXAUI top level.
**
**                  This function is used for eXAUI mode only.
**
**  INPUTS:        *fileHndl   - device handle
**                  slice       - PIF eXAUI slice 0 to 1
**                                0 - eXAUI0
**                                1 - eXAUI1
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31-25: RESERVED
**                                  Bit 24   : TXACTIVE
**                                  Bit 23   : PAUSED
**                                  Bit 22   : LOST_SYNC_V
**                                  Bit 17-21: RESERVED
**                                  Bit 16   : SOP_V
**                                  Bit 14-15: RESERVED
**                                  Bit 13   : DV_ERR_V
**                                  Bit 12   : DATA_INVALID_V
**                                  Bit 9-11 : RESERVED
**                                  Bit 3-8  : RESERVED
**                                  Bit 2    : PF_UNDERFLOW_V
**                                  Bit 1    : PF_OVERFLOW_V
**                                  Bit 0    : BIP8_ERR_THRESHOLD_V
**
**  OUTPUTS:        None      - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifXauiGetStatus(struct file *fileHndl, UINT4 slice,
                              UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr;
    UINT4 txxgStat = 0;
    UINT4 losStat = 0;
    UINT4 intStat = 0;
    UINT4 topStat = 0;

    /* argument checking */
    if (slice > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Read TXXG status */
    addr = HYPHY20G_XAUI_TXXG_REG_STAT;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &txxgStat);
    if(result) return result;

    /* Read XTEF status */
    addr = HYPHY20G_XAUI_XTEF_REG_INT_VAL;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &losStat);
    if(result) return result;

    /* Read RXOAM status */
    addr = HYPHY20G_XAUI_RXOAM_REG_INT_VAL;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &intStat);
    if(result) return result;

    /* Read XAUI Top Level status */
    addr = HYPHY20G_XAUI_TOP_REG_INT_VAL;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &topStat);
    if(result) return result;

    *statusBits = ((txxgStat & 0x3) << 23) + ((losStat & 0x1) << 22) +
                  ((intStat & 0x1FFF) << 8) + (topStat & 0x1FF);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifXauiGetStatus */



/*******************************************************************************
**
**  hyPhy20gPifXauiGetLaneStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the PIF eXAUI LANE Status.
**
**                  This function is used for eXAUI mode only.
**
**  INPUTS:        *fileHndl   - device handle
**                  slice       - PIF eXAUI slice 0 to 1
**                                0 - eXAUI0
**                                1 - eXAUI1
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31-9 : RESERVED
**                                  Bit 8-5  : HS_SD_LANE[3:0]
**                                  Bit 4    : ALIGN_STATUS_ERR
**                                  Bit 3-0  : LANE_SYNC_STAT_ERR[3:0]
**
**  OUTPUTS:        None      - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifXauiGetLaneStatus(struct file *fileHndl, UINT4 slice,
                                  UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, regData;

    /* argument checking */
    if (slice > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Read XRF status */
    addr = HYPHY20G_XAUI_XRF_REG_INT_VAL;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    *statusBits = regData;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifXauiGetLaneStatus */


/*******************************************************************************
**
**  hyPhy20gPifXauiGetTxxgStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the PIF eXAUI TXXG status
**                  (interrupt indicator).
**
**                  It is recommended to have the device level WCI_MODE bit
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts within the registers accessed by
**                  this function.
**
**                  This function is used for eXAUI mode only.
**
**  INPUTS:        *fileHndl   - device handle
**                  slice       - PIF eXAUI slice 0 to 1
**                                0 - eXAUI0
**                                1 - eXAUI1
**                  intClear    - 1 - all *INT_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this
**                               function will retain their values.
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31:5 - RESERVED
**                                  Bit  4   - FIFO_ERR_I
**                                  Bit  3   - FIFO_UDR_I
**                                  Bit  2   - MAX_LERR_I
**                                  Bit  1   - MIN_LERR_I
**                                  Bit  0   - RESERVED
**
**  OUTPUTS:        None      - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifXauiGetTxxgStatus(struct file *fileHndl, UINT4 slice,
                                  UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, regData;

    /* argument checking */
    if (slice > 1 || intClear > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    addr = HYPHY20G_XAUI_TXXG_REG_INT;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    *statusBits = regData >> 11;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifXauiGetTxxgStatus */



/*******************************************************************************
**
**  hyPhy20gPifXauiGetRxxgStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the PIF eXAUI RXXG Status.
**                  (interrupt indicator).
**
**                  It is recommended to have the device level WCI_MODE bit
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts within the registers accessed by
**                  this function.
**
**                  This function is used for eXAUI mode only.
**
**  INPUTS:        *fileHndl   - device handle
**                  slice      - PIF eXAUI slice 0 to 1
**                               0 - eXAUI0
**                               1 - eXAUI1
**                  intClear   - 1 - all *INT_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                             - 0 - all *INT_I interrupt bits accessed by this
**                               function will retain their values.
**                 *statusBits - Pointer to storage for status bits
**                                  Bit 31:11- RESERVED
**                                  Bit 10   - MIN_LERR_I
**                                  Bit  9   - MAX_LERR_I
**                                  Bit  8   - ODD_ALIGN_I
**                                  Bit  7   - LINE_ERR_I
**                                  Bit  6   - CONTIG_ERR_I
**                                  Bit  5   - RESERVED
**                                  Bit  4   - RESERVED
**                                  Bit  3   - ERR_DETECT_I
**                                  Bit  2   - RESERVED
**                                  Bit  1   - IFG_ERR_I
**                                  Bit  0   - PRMB_ERR_I
**
**  OUTPUTS:        None      - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifXauiGetRxxgStatus(struct file *fileHndl, UINT4 slice,
                                  UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, regData;

    /* argument checking */
    if (slice > 1 || intClear > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    addr = HYPHY20G_XAUI_RXXG_REG_INT;
    addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    *statusBits = regData >> 5;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifXauiGetRxxgStatus */

/*******************************************************************************
**
**  hyPhy20gPifIntlknShimGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves the status for PIF Interlaken
**                  interface at the Shim level.
**
**  INPUTS:       *fileHndl    - base address
**                *pStatusBits - Pointer to storage for status bits
**                               bit 6    : PB_FIFO_2B_ECC_V
**                               bit 5    : PB_FIFO_1B_ECC_V
**                               bit 4    : RX_SOP_MISSING_V
**                               bit 3    : RX_EOP_MISSING_V
**                               bit 2    : PB_FIFO_OVERFLOW_V
**                               bit 1    : TX_SOP_MISSING_V
**                               bit 0    : TX_EOP_MISSING_V
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifIntlknShimGetStatus(struct file *fileHndl, UINT4 *pStatusBits)
{
    UINT4 regAddr;
    UINT4 pifIntlknShimData;
    INT4 result;

    regAddr = HYPHY20G_PIF_INTLKN_SHIM_REG_INT_VAL;

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, regAddr, &pifIntlknShimData);
    if (result) return result;
    *pStatusBits= pifIntlknShimData;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifIntlknShimGetStatus */

/*******************************************************************************
**
**  hyPhy20gPifIntlknCoreRxIntGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves the top level interrupt status for
**                  the PIF Interlaken Core instance for the receive
**                  direction.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits.
**
**  INPUTS:       *fileHndl    - base address
**                intClear     - 1 - all *INT_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                             - 0 - all *INT_I interrupt bits accessed by this
**                               function will retain their values.
**                *pStatusBits - Pointer to storage for interrupt bits
**                               bit 26   : CTLBIT_ERR_I
**                               bit 25   : MF_ERR_I
**                               bit 24   : BADCTL_ERR_I
**                               bit 23   : SCRAM_ERR_I
**                               bit 22   : CRC32_ERR_I
**                               bit 20   : WRD_SYNC_CHANGE_I
**                               bit 19   : ALIGN_CHANGE_I
**                               bit 18   : ALIGN_ERR_I
**                               bit 17   : BURST_ERR_I
**                               bit 16   : BMIN_ERR_I
**                               bit 15   : BMAX_ERR_I
**                               bit 14   : CRC24_ERR_I
**                               bit 13   : IB_FC_ERR_I
**                               bit 12   : RX_STATUS_MSG_CHANGE_I
**                               bit 11   : BADCTL_CNT_FILL_I
**                               bit 10   : CRC32_CNT_FILL_I
**                               bit 8    : WRD_SYNC_CNT_FILL_I
**                               bit 7    : ALIGN_FAIL_CNT_FILL_I
**                               bit 6    : ALIGN_CNT_FILL_I
**                               bit 5    : BURST_CNT_FILL_I
**                               bit 4    : BMIN_CNT_FILL_I
**                               bit 3    : BMAX_CNT_FILL_I
**                               bit 2    : CRC24_CNT_FILL_I
**                               bit 1    : RX_FC_CNT_FILL_I
**                               bit 0    : RX_CHAN_CNT_FILL_I
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gPifIntlknCoreRxIntGetStatus(struct file *fileHndl, UINT4 intClear,
                                         UINT4 *pStatusBits)
{
    UINT4 regAddr;
    UINT4 msk;
    UINT4 pifIntlknIntData, ctlBitErrInt;
    UINT4 mfErrInt, badCtlErrInt;
    UINT4 scramErrInt, crc32ErrInt;
    UINT4 wrdSyncChngInt;
    UINT4 alignChngInt, alignErrInt;
    UINT4 burstErrInt, bMinErrInt, bMaxErrInt;
    UINT4 crc24ErrInt, ibFcErrInt;
    UINT4 rxStatMsgChngInt, badCtlCntInt;
    UINT4 crc32CntInt, wrdSyncCntInt;
    UINT4 alignFailCntInt, alignCntInt;
    UINT4 burstCntInt, bMinCntInt, bMaxCntInt;
    UINT4 crc24CntInt, rxFcCntInt, rxChanCntInt;
    INT4 result;

    /* argument checking */
    if (intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* select RX_INT register */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_RX_INT;

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, regAddr, &pifIntlknIntData);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_CTLBIT_ERR_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &ctlBitErrInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_MF_ERR_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &mfErrInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_BADCTL_ERR_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &badCtlErrInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_SCRAM_ERR_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &scramErrInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_CRC32_ERR_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &crc32ErrInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_WRD_SYNC_CHANGE_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &wrdSyncChngInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_ALIGN_CHANGE_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &alignChngInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_ALIGN_ERR_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &alignErrInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_BURST_ERR_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &burstErrInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_BMIN_ERR_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &bMinErrInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_BMAX_ERR_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &bMaxErrInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_CRC24_ERR_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &crc24ErrInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_IB_FC_ERR_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &ibFcErrInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_RX_STATUS_MSG_CHANGE_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &rxStatMsgChngInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_BADCTL_CNT_FILL_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &badCtlCntInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_CRC32_CNT_FILL_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &crc32CntInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_WRD_SYNC_CNT_FILL_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &wrdSyncCntInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_ALIGN_FAIL_CNT_FILL_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &alignFailCntInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_ALIGN_CNT_FILL_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &alignCntInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_BURST_CNT_FILL_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &burstCntInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_BMIN_CNT_FILL_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &bMinCntInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_BMAX_CNT_FILL_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &bMaxCntInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_CRC24_CNT_FILL_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &crc24CntInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_RX_FC_CNT_FILL_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &rxFcCntInt);
    if (result) return result;

    msk = HYPHY20G_CBRI_CORE_BIT_RX_INT_RX_CHAN_CNT_FILL_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &rxChanCntInt);
    if (result) return result;

    *pStatusBits = (ctlBitErrInt << 26) + (mfErrInt << 25) +
                   (badCtlErrInt << 24) + (scramErrInt << 23) +
                   (crc32ErrInt << 22) + 
                   (wrdSyncChngInt << 20) + (alignChngInt << 19) +
                   (alignErrInt << 18) + (burstErrInt << 17) +
                   (bMinErrInt << 16) + (bMaxErrInt << 15) +
                   (crc24ErrInt << 14) + (ibFcErrInt << 13) +
                   (rxStatMsgChngInt << 12) + (badCtlCntInt << 11) +
                   (crc32CntInt << 10) +
                   (wrdSyncCntInt << 8) + (alignFailCntInt << 7) +
                   (alignCntInt << 6) + (burstCntInt << 5) +
                   (bMinCntInt << 4) + (bMaxCntInt << 3) +
                   (crc24CntInt << 2) + (rxFcCntInt << 1) + rxChanCntInt;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifIntlknCoreRxIntGetStatus */

/*******************************************************************************
**
**  hyPhy20gPifIntlknCoreTxIntGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves the top level interrupt status for
**                  the PIF Interlaken Core instance for the transmit
**                  direction.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits.
**
**
**  INPUTS:       *fileHndl     - base address
**                intClear      - 1 - all *INT_I interrupt bits accessed by this
**                                function will be cleared by writing them to
**                                '1'
**                              - 0 - all *INT_I interrupt bits accessed by this
**                                function will retain their values.
**                *pStatusBits  - Pointer to storage for interrupt bits
**                                bit 2    : TX_STATUS_MSG_CHANGE_I
**                                bit 1    : TX_FC_CNT_FILL_I
**                                bit 0    : TX_CHAN_CNT_FILL_I
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gPifIntlknCoreTxIntGetStatus(struct file *fileHndl, UINT4 intClear,
                                         UINT4 *pStatusBits)
{
    UINT4 regAddr;
    UINT4 pifIntlknIntData;
    UINT4 statMsgChngInt;
    UINT4 fcCntInt;
    UINT4 chanCntInt;
    UINT4 msk;
    INT4 result;

    /* argument checking */
    if (intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* select TX_INT register */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_TX_INT;

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, regAddr, &pifIntlknIntData);
    if (result) return result;

    msk = HYPHY20G_PIF_INTLKN_CORE_BIT_TX_INT_TX_STATUS_MSG_CHANGE_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &statMsgChngInt);
    if (result) return result;

    msk = HYPHY20G_PIF_INTLKN_CORE_BIT_TX_INT_TX_FC_CNT_FILL_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &fcCntInt);
    if (result) return result;

    msk = HYPHY20G_PIF_INTLKN_CORE_BIT_TX_INT_TX_CHAN_CNT_FILL_I_MSK;
    result = hyPhy20gVarFieldRead(pifIntlknIntData, msk, &chanCntInt);
    if (result) return result;

    *pStatusBits= (statMsgChngInt << 2) + (fcCntInt << 1) + chanCntInt;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifIntlknCoreTxIntGetStatus */

/*******************************************************************************
**
**  hyPhy20gPifIntlknCoreTxCounterGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves the status for transmit counters
**                  from the PIF Interlaken Core.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits.
**
**
**  INPUTS:       *fileHndl     - base address
**                intClear      - 1 - all status bits accessed by this
**                                function will be cleared by writing them to
**                                '1'
**                              - 0 - all status bits accessed by this
**                                function will retain their values.
**                *pCntFill     - Pointer to storage for counter fill status:
**                                bit 5   : BOVER
**                                bit 4   : POVER
**                                bit 3   : EOVER
**                                bit 2   : BHALF
**                                bit 1   : PHALF
**                                bit 0   : EHALF
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifIntlknCoreTxCounterGetStatus(struct file *fileHndl,
                                             UINT4 intClear,
                                             UINT4 *pStatusBits)
{
    UINT4 regAddr;
    UINT4 pifIntlknCoreCntData;
    INT4 result;

    /* argument checking */
    if (intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve counter fill status for Tx direction (index = 0) */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_CNT_FILL_OFFSET(0);

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, regAddr, &pifIntlknCoreCntData);
    if (result) return result;

    *pStatusBits = pifIntlknCoreCntData >> 26;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifIntlknCoreTxCounterGetStatus */

/*******************************************************************************
**
**  hyPhy20gPifIntlknCoreRxCounterGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves the status for receive counters from
**                  the PIF Interlaken Core.
**
**                  The status bits accessed by this function are write one to
**                  clear. The intClear argument provides the ability to
**                  optionally clear the status bits.
**
**
**  INPUTS:       *fileHndl      - base address
**                intClear       - 1 - all status bits accessed by this
**                                     function will be cleared by writing
**                                     them to '1'
**                               - 0 - all status bits accessed by this
**                                     function will retain their values.
**                *pCntFill      - Pointer to storage for counter fill status:
**                                 bit 5   : BOVER
**                                 bit 4   : POVER
**                                 bit 3   : EOVER
**                                 bit 2   : BHALF
**                                 bit 1   : PHALF
**                                 bit 0   : EHALF
**                *pCrc32CntFill - Rx Lane CRC Error Count over half full status
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Counter over half full
**                                 0 - Counter upto half full
**                *pWordCntFill  - Rx Lane Word Sync Error Count over half full
**                                 status
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Counter over half full
**                                 0 - Counter upto half full
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifIntlknCoreRxCounterGetStatus(struct file *fileHndl,
                                             UINT4 intClear, UINT4 *pCntFill,
                                             UINT4 *pCrc32CntFill,
                                             UINT4 *pWordCntFill)
{
    UINT4 regAddr;
    UINT4 pifIntlknCoreCntData;
    UINT4 rxCrc32Cnt;
    UINT4 wrdSyncCnt;
    INT4 result;

    /* argument checking */
    if (intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve counter fill status for Rx direction (index = 1) */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_CNT_FILL_OFFSET(1);

    result = sysHyPhy20gRead(fileHndl, regAddr, &pifIntlknCoreCntData);
    if (result) return result;
    *pCntFill = pifIntlknCoreCntData >> 26;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_CRC32_CNT_FILL_LANE */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_CRC32_CNT_FILL;

    result = sysHyPhy20gRead(fileHndl, regAddr, &rxCrc32Cnt);
    if (result) return result;
    *pCrc32CntFill = rxCrc32Cnt;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_WRD_SYNC_CNT_FILL_LANE */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_RX_WD_SYNC_CNT_FILL;

    result = sysHyPhy20gRead(fileHndl, regAddr, &wrdSyncCnt);
    if (result) return result;
    *pWordCntFill = wrdSyncCnt;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifIntlknCoreRxCounterGetStatus */

/*******************************************************************************
**
**  hyPhy20gPifIntlknCoreRxLaneGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves the status for receive lanes from
**                  the PIF Interlaken Core.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits.
**
**  INPUTS:       *fileHndl      - base address
**                intClear       - 1 - all status bits accessed by this
**                                     function will be cleared by writing them
**                                     to '1'
**                               - 0 - all status bits accessed by this
**                                     function will retain their values.
**                *pMfErr        - Pointer to storage for per lane metaframe
**                                 errors:
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Error present
**                                 0 - No Error present
**                *pCtlWrdErr    - Pointer to storage for per lane bad control
**                                 word errors:
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Error present
**                                 0 - No Error present
**                *pScrbStateErr - Pointer to storage for per lane scrambler
**                                 state errors:
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Error present
**                                 0 - No Error present
**                *pCrc32Err     - Pointer to storage for per lane CRC 32
**                                 errors:
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Error present
**                                 0 - No Error present
**                *pWrdSyncChng  - Pointer to storate for per lane word
**                                 synchronization changes
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Change of word synchronization occurred
**                                 0 - No change occurred
**                *pWrdSyncStat  - Pointer to storage for per lane word
**                                 synchronization status
**                                 bit 7-0  : Status for Lane 7 to 0
**                                 1 - Word synchronization present
**                                 0 - Word synchronization not present
**                *pAlignStatus  - Pointer to storage for interface alignment
**                                 status.  One bit:
**                                 1 - Alignment Good
**                                 0 - Alignment Bad
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifIntlknCoreRxLaneGetStatus(struct file *fileHndl,
                                          UINT4 intClear, UINT4 *pMfErr,
                                          UINT4 *pCtlWrdErr,
                                          UINT4 *pScrbStateErr,
                                          UINT4 *pCrc32Err,
                                          UINT4 *pWrdSyncChng,
                                          UINT4 *pWrdSyncStat,
                                          UINT4 *pAlignStatus)
{
    UINT4 regAddr;
    UINT4 pIntlknCrMfErr;
    UINT4 pIntlknCrBadCtlErr;
    UINT4 pIntlknCrScramErr;
    UINT4 pIntlknCrCrcErr;
    UINT4 pIntlknCrWrdSyncChng;
    UINT4 pIntlknCrWrdSync;
    UINT4 pIntlknCrAlignStat;
    INT4 result;

    /* argument checking */
    if (intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* RX_MF_ERR */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_RX_MFRM_ERR;

    result = sysHyPhy20gRead(fileHndl, regAddr, &pIntlknCrMfErr);
    if (result) return result;
    *pMfErr = pIntlknCrMfErr;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_BADCTL_ERR */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_RX_BAD_CTL_WD_ERR;

    result = sysHyPhy20gRead(fileHndl, regAddr, &pIntlknCrBadCtlErr);
    if (result) return result;
    *pCtlWrdErr = pIntlknCrBadCtlErr;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_SCRAM_ERR */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_RX_SCR_STAT_ERR;

    result = sysHyPhy20gRead(fileHndl, regAddr, &pIntlknCrScramErr);
    if (result) return result;
    *pScrbStateErr = pIntlknCrScramErr;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_CRC32_ERR */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_RX_CRC_ERR;

    result = sysHyPhy20gRead(fileHndl, regAddr, &pIntlknCrCrcErr);
    if (result) return result;
    *pCrc32Err = pIntlknCrCrcErr;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_WRD_SYNC_CHG */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_RX_WD_SYNC_CHG;

    result = sysHyPhy20gRead(fileHndl, regAddr, &pIntlknCrWrdSyncChng);
    if (result) return result;
    *pWrdSyncChng = pIntlknCrWrdSyncChng;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_WRD_SYNC_LANE */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_RX_WD_SYNC;

    result = sysHyPhy20gRead(fileHndl, regAddr, &pIntlknCrWrdSync);
    if (result) return result;
    *pWrdSyncStat = pIntlknCrWrdSync;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
    }

    /* RX_ALIGN_GOOD */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_RX_ALGN_STAT_STAT;

    result = sysHyPhy20gRead(fileHndl, regAddr, &pIntlknCrAlignStat);
    if (result) return result;
    *pAlignStatus= pIntlknCrAlignStat >> 23;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifIntlknCoreRxLaneGetStatus */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/* end of file */
