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
**  FILE        : hyphy20g_otn_counters.c
**
**  $Date: 2012-12-03 17:15:25 +0800 (Mon, 03 Dec 2012) $
**
**  $Revision: 29409 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_otn_counters.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gOpukTxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the OPUk level transmit
**                  counters.
**
**                  System dependent, the transfer of counter data to holding
**                  registers is controlled at the subsystem level or the
**                  device level.  The accessMode field allows for the the
**                  usage of local counter accumulation or not.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTUk Framer instance:
**                                0 - OTU1 Framer 0
**                                1 - OTU1 Framer 1
**                                2 - OTU1 Framer 2
**                                3 - OTU1 Framer 3
**                                4 - OTU1 Framer 4
**                                5 - OTU1 Framer 5
**                                6 - OTU1 Framer 6
**                                7 - OTU1 Framer 7
**                                8 - OTU2 Framer 0
**                                9 - OTU2 Framer 1
**                 accessMode   - 1 - locally trigger counter accumulation
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                01 - TX_PJ_COUNTER_VALUE
**                                00 - TX_NJ_COUNTER_VALUE
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOpukTxGetCounts(struct file *fileHndl, UINT4 slice,
                             UINT4 accessMode, UINT4 counter, UINT4 *pCount)
{
    UINT4 trigReg = 0, trigMsk = 0, cntReg = 0, cntMsk = 0, val;
    INT4 result;

    /* argument checking */
    if (slice > 9 || accessMode > 1 || counter > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pCount == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        switch(counter)
        {
            case 0: /* TX_NJ_COUNTER_VALUE */
                trigReg = HYPHY20G_OTU2_FRM_REG_TX_OTN_CNT_CTL;
                trigReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice-8);
                trigMsk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_CNT_CTL_TX_NJ_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_OTU2_FRM_REG_TX_OTN_OPU_NJ_CNT;
                cntReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice-8);
                cntMsk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_OPU_NJ_CNT_TX_NJ_COUNTER_VALUE_MSK;
                break;
            case 1: /* TX_PJ_COUNTER_VALUE */
                trigReg = HYPHY20G_OTU2_FRM_REG_TX_OTN_CNT_CTL;
                trigReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice-8);
                trigMsk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_CNT_CTL_TX_PJ_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_OTU2_FRM_REG_TX_OTN_OPU_PJ_CNT;
                cntReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice-8);
                cntMsk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_OPU_PJ_CNT_TX_PJ_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
    } else {
        /* OTU1 Framer case */
        switch(counter)
        {            case 0: /* TX_NJ_COUNTER_VALUE */
                trigReg = HYPHY20G_OTU1_FRM_REG_TX_OTN_CNT_CTL;
                trigReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice);
                trigMsk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_CNT_CTL_TX_NJ_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_OTU1_FRM_REG_TX_OTN_OPU_NJ_CNT;
                cntReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice);
                cntMsk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_OPU_NJ_CNT_TX_NJ_COUNTER_VALUE_MSK;
                break;
            case 1: /* TX_PJ_COUNTER_VALUE */
                trigReg = HYPHY20G_OTU1_FRM_REG_TX_OTN_CNT_CTL;
                trigReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice);
                trigMsk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_CNT_CTL_TX_PJ_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_OTU1_FRM_REG_TX_OTN_OPU_PJ_CNT;
                cntReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice);
                cntMsk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_OPU_PJ_CNT_TX_PJ_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    /* Trigger access with *_LATCH_AND_CLEAR if required */
    if (accessMode == 1) {
        /* Trigger Access */
        result = hyPhy20gRegFieldWrite(fileHndl, trigReg, trigMsk, 1);
        if (result) return result;
    }

    /* Poll *_LATCH_AND_CLEAR bit to ensure cleared for valid counter access */
    result = sysHyPhy20gPollBit(fileHndl, trigReg, trigMsk, 0, 10000);
    if (result) return result;

    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, cntReg, cntMsk, &val);
    if (result) return result;
    *pCount = val;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOpukTxGetCounts */

/*******************************************************************************
**
**  hyPhy20gOdukTcmiTxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the ODUk level transmit
**                  TCMi counters.
**
**                  System dependent, the transfer of counter data to holding
**                  registers is controlled at the subsystem level or the
**                  device level.  The accessMode field allows for the the
**                  usage of local counter accumulation or not.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTUk Framer instance:
**                                0 - OTU1 Framer 0
**                                1 - OTU1 Framer 1
**                                2 - OTU1 Framer 2
**                                3 - OTU1 Framer 3
**                                4 - OTU1 Framer 4
**                                5 - OTU1 Framer 5
**                                6 - OTU1 Framer 6
**                                7 - OTU1 Framer 7
**                                8 - OTU2 Framer 0
**                                9 - OTU2 Framer 1
**                 accessMode   - 1 - locally trigger counter accumulation
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                11 - TX_TCM1_BIP_COUNTER_VALUE
**                                10 - TX_TCM1_BEI_COUNTER_VALUE
**                                09 - TX_TCM2_BIP_COUNTER_VALUE
**                                08 - TX_TCM2_BEI_COUNTER_VALUE
**                                07 - TX_TCM3_BIP_COUNTER_VALUE
**                                06 - TX_TCM3_BEI_COUNTER_VALUE
**                                05 - TX_TCM4_BIP_COUNTER_VALUE
**                                04 - TX_TCM4_BEI_COUNTER_VALUE
**                                03 - TX_TCM5_BIP_COUNTER_VALUE
**                                02 - TX_TCM5_BEI_COUNTER_VALUE
**                                01 - TX_TCM6_BIP_COUNTER_VALUE
**                                00 - TX_TCM6_BEI_COUNTER_VALUE
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukTcmiTxGetCounts(struct file *fileHndl, UINT4 slice,
                                 UINT4 accessMode, UINT4 counter,
                                 UINT4 *pCount)
{
    UINT4 trigReg = 0;
    UINT4 trigMsk = 0;
    UINT4 cntReg = 0;
    UINT4 cntMsk = 0;
    UINT4 val, n;
    INT4 result;

    /* argument checking */
    if (slice > 9 || accessMode > 1 || counter > 11) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pCount == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        switch(counter%2)
        {
            case 0: /* TX_TCMI_BEI_COUNTER_VALUE */
                n      = 6 - (counter+1)/2;
                cntReg = HYPHY20G_OTU2_FRM_REG_TX_OTN_ODU_TCMI_INT_BEI_CNT_OFFSET(n);
                cntMsk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_INT_BEI_CNT_TX_TCMI_BEI_COUNTER_VALUE_MSK;
                break;
            case 1: /* TX_TCMI_BIP_COUNTER_VALUE */
                n      = 6 - (counter)/2;
                cntReg = HYPHY20G_OTU2_FRM_REG_TX_OTN_ODU_TCMI_INT_BIP_CNT_OFFSET(n);
                cntMsk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_INT_BIP_CNT_TX_TCMI_BIP_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_OTU2_FRM_REG_TX_TCMI_INTRP_CNT_CTL;
        trigReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice-8);
        trigMsk = 0x0001 << (HYPHY20G_OTU2_FRM_BIT_TX_TCMI_INTRP_CNT_CTL_TX_TCM6_BEI_LATCH_AND_CLEAR_OFF + counter);
        cntReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice-8);
    } else {
        /* OTU1 Framer case */
        switch(counter%2)
        {
            case 0: /* TX_TCMI_BEI_COUNTER_VALUE */
                n      = 6 - (counter+1)/2;
                cntReg = HYPHY20G_OTU1_FRM_REG_TX_OTN_ODU_TCMI_INT_BEI_CNT_OFFSET(n);
                cntMsk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_INT_BEI_CNT_TX_TCMI_BEI_COUNTER_VALUE_MSK;
                break;
            case 1: /* TX_TCMI_BIP_COUNTER_VALUE */
                n      = 6 - (counter)/2;
                cntReg = HYPHY20G_OTU1_FRM_REG_TX_OTN_ODU_TCMI_INT_BIP_CNT_OFFSET(n);
                cntMsk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_INT_BIP_CNT_TX_TCMI_BIP_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_OTU1_FRM_REG_TX_TCMI_INTRP_CNT_CTL;
        trigReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice);
        trigMsk = 0x0001 << (HYPHY20G_OTU1_FRM_BIT_TX_TCMI_INTRP_CNT_CTL_TX_TCM6_BEI_LATCH_AND_CLEAR_OFF + counter);
        cntReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice);
    }

    /* Trigger access with *_LATCH_AND_CLEAR if required */
    if (accessMode == 1) {
        /* Trigger Access */
        result = hyPhy20gRegFieldWrite(fileHndl, trigReg, trigMsk, 1);
        if (result) return result;
    }

    /* Poll *_LATCH_AND_CLEAR bit to ensure cleared for valid counter access */
    result = sysHyPhy20gPollBit(fileHndl, trigReg, trigMsk, 0, 10000);
    if (result) return result;

    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, cntReg, cntMsk, &val);
    if (result) return result;
    *pCount = val;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukTcmiTxGetCounts */

/*******************************************************************************
**
**  hyPhy20gOtnFramerRxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the OTN Framer level receive
**                  counters.
**
**                  System dependent, the transfer of counter data to holding
**                  registers is controlled at the subsystem level or the
**                  device level.  The accessMode field allows for the 
**                  usage of local counter accumulation or not.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTUk Framer instance:
**                                0 - OTU1 Framer 0
**                                1 - OTU1 Framer 1
**                                2 - OTU1 Framer 2
**                                3 - OTU1 Framer 3
**                                4 - OTU1 Framer 4
**                                5 - OTU1 Framer 5
**                                6 - OTU1 Framer 6
**                                7 - OTU1 Framer 7
**                                8 - OTU2 Framer 0
**                                9 - OTU2 Framer 1
**                 accessMode   - 1 - locally trigger counter accumulation
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                05 - OOF_COUNTER_VALUE
**                                04 - OOM_COUNTER_VALUE
**                                03 - FEC_CORRECTABLE_SYMBOLS_COUNTER_VALUE
**                                02 - FEC_CORRECTABLE_ZEROS_COUNTER_VALUE
**                                01 - FEC_CORRECTABLE_ONES_COUNTER_VALUE
**                                00 - FEC_UNCORRECTABLE_CODEWORDS_COUNTER_VALUE
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtnFramerRxGetCounts(struct file *fileHndl, UINT4 slice,
                                  UINT4 accessMode, UINT4 counter,
                                  UINT4 *pCount)
{
    UINT4 trigReg = 0;
    UINT4 trigMsk = 0;
    UINT4 cntReg = 0;
    UINT4 cntMsk = 0;
    UINT4 uprCntReg = 0;
    UINT4 uprCntMsk = 0;
    UINT4 lowCntReg = 0;
    UINT4 lowCntMsk = 0;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 9 || accessMode > 1 || counter > 5) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pCount == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        switch(counter)
        {
            case 0: /* FEC_UNCORRECTABLE_CODEWORDS_COUNTER_VALUE */
                trigMsk   = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_FEC_UNCORRECTABLE_CODEWORDS_LATCH_AND_CLEAR_MSK;
                uprCntReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_FEC_UPR_UNCORR_CDWD_CNT;
                lowCntReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_FEC_LOW_UNCORR_CDWD_CNT;
                uprCntMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FEC_UPR_UNCORR_CDWD_CNT_UPPER_FEC_UNCORRECTABLE_CODEWORDS_COUNTER_VALUE_MSK;
                lowCntMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FEC_LOW_UNCORR_CDWD_CNT_LOWER_FEC_UNCORRECTABLE_CODEWORDS_COUNTER_VALUE_MSK;
                break;
            case 1: /* FEC_CORRECTABLE_ONES_COUNTER_VALUE */
                trigMsk   = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_FEC_CORRECTABLE_ONES_LATCH_AND_CLEAR_MSK;
                uprCntReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_FEC_UPR_CORR_ONE_CNT;
                lowCntReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_FEC_LOW_CORR_ONE_CNT;
                uprCntMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FEC_UPR_CORR_ONE_CNT_UPPER_FEC_CORRECTABLE_ONES_COUNTER_VALUE_MSK;
                lowCntMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FEC_LOW_CORR_ONE_CNT_LOWER_FEC_CORRECTABLE_ONES_COUNTER_VALUE_MSK;
                break;
            case 2: /* FEC_CORRECTABLE_ZEROS_COUNTER_VALUE */
                trigMsk   = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_FEC_CORRECTABLE_ZEROS_LATCH_AND_CLEAR_MSK;
                uprCntReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_FEC_UPR_CORR_ZRO_CNT;
                lowCntReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_FEC_LOW_CORR_ZRO_CNT;
                uprCntMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FEC_UPR_CORR_ZRO_CNT_UPPER_FEC_CORRECTABLE_ZEROS_COUNTER_VALUE_MSK;
                lowCntMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FEC_LOW_CORR_ZRO_CNT_LOWER_FEC_CORRECTABLE_ZEROS_COUNTER_VALUE_MSK;
                break;
            case 3: /* FEC_CORRECTABLE_SYMBOLS_COUNTER_VALUE */
                trigMsk   = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_FEC_CORRECTABLE_SYMBOLS_LATCH_AND_CLEAR_MSK;
                uprCntReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_FEC_UPR_CORR_SYM_CNT;
                lowCntReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_FEC_LOW_CORR_SYM_CNT;
                uprCntMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FEC_UPR_CORR_SYM_CNT_UPPER_FEC_CORRECTABLE_SYMBOLS_COUNTER_VALUE_MSK;
                lowCntMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FEC_LOW_CORR_SYM_CNT_LOWER_FEC_CORRECTABLE_SYMBOLS_COUNTER_VALUE_MSK;
                break;
            case 4: /* OOM_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_OOM_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU2_FRM_REG_RX_OTN_FA_OOM_CNT;
                cntMsk  = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FA_OOM_CNT_OOM_COUNTER_VALUE_MSK;
                break;
            case 5: /* OOF_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_OOF_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU2_FRM_REG_RX_OTN_FA_OOF_CNT;
                cntMsk  = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FA_OOF_CNT_OOF_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }

        trigReg   = HYPHY20G_OTU2_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
        trigReg   = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice-8);
        cntReg    = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice-8);
        uprCntReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(uprCntReg, slice-8);
        lowCntReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(lowCntReg, slice-8);
    } else {
        /* OTU1 Framer case */
        switch(counter)
        {
            case 0: /* FEC_UNCORRECTABLE_CODEWORDS_COUNTER_VALUE */
                trigMsk   = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_FEC_UNCORRECTABLE_CODEWORDS_LATCH_AND_CLEAR_MSK;
                uprCntReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_FEC_UPR_UNCORR_CDWD_CNT;
                lowCntReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_FEC_LOW_UNCORR_CDWD_CNT;
                uprCntMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FEC_UPR_UNCORR_CDWD_CNT_UPPER_FEC_UNCORRECTABLE_CODEWORDS_COUNTER_VALUE_MSK;
                lowCntMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FEC_LOW_UNCORR_CDWD_CNT_LOWER_FEC_UNCORRECTABLE_CODEWORDS_COUNTER_VALUE_MSK;
                break;
            case 1: /* FEC_CORRECTABLE_ONES_COUNTER_VALUE */
                trigMsk   = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_FEC_CORRECTABLE_ONES_LATCH_AND_CLEAR_MSK;
                uprCntReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_FEC_UPR_CORR_ONE_CNT;
                lowCntReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_FEC_LOW_CORR_ONE_CNT;
                uprCntMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FEC_UPR_CORR_ONE_CNT_UPPER_FEC_CORRECTABLE_ONES_COUNTER_VALUE_MSK;
                lowCntMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FEC_LOW_CORR_ONE_CNT_LOWER_FEC_CORRECTABLE_ONES_COUNTER_VALUE_MSK;
                break;
            case 2: /* FEC_CORRECTABLE_ZEROS_COUNTER_VALUE */
                trigMsk   = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_FEC_CORRECTABLE_ZEROS_LATCH_AND_CLEAR_MSK;
                uprCntReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_FEC_UPR_CORR_ZRO_CNT;
                lowCntReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_FEC_LOW_CORR_ZRO_CNT;
                uprCntMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FEC_UPR_CORR_ZRO_CNT_UPPER_FEC_CORRECTABLE_ZEROS_COUNTER_VALUE_MSK;
                lowCntMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FEC_LOW_CORR_ZRO_CNT_LOWER_FEC_CORRECTABLE_ZEROS_COUNTER_VALUE_MSK;
                break;
            case 3: /* FEC_CORRECTABLE_SYMBOLS_COUNTER_VALUE */
                trigMsk   = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_FEC_CORRECTABLE_SYMBOLS_LATCH_AND_CLEAR_MSK;
                uprCntReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_FEC_UPR_CORR_SYM_CNT;
                lowCntReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_FEC_LOW_CORR_SYM_CNT;
                uprCntMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FEC_UPR_CORR_SYM_CNT_UPPER_FEC_CORRECTABLE_SYMBOLS_COUNTER_VALUE_MSK;
                lowCntMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FEC_LOW_CORR_SYM_CNT_LOWER_FEC_CORRECTABLE_SYMBOLS_COUNTER_VALUE_MSK;
                break;
            case 4: /* OOM_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_OOM_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU1_FRM_REG_RX_OTN_FA_OOM_CNT;
                cntMsk  = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_OOM_CNT_OOM_COUNTER_VALUE_MSK;
                break;
            case 5: /* OOF_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_OOF_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU1_FRM_REG_RX_OTN_FA_OOF_CNT;
                cntMsk  = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_OOF_CNT_OOF_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }

        trigReg   = HYPHY20G_OTU1_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
        trigReg   = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice);
        cntReg    = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice);
        uprCntReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(uprCntReg, slice);
        lowCntReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(lowCntReg, slice);
    }

    /* Trigger access with *_LATCH_AND_CLEAR if required */
    if (accessMode == 1) {
        /* Trigger Access */
        result = hyPhy20gRegFieldWrite(fileHndl, trigReg, trigMsk, 1);
        if (result) return result;
    }

    /* Poll *_LATCH_AND_CLEAR bit to ensure cleared for valid counter access */
    result = sysHyPhy20gPollBit(fileHndl, trigReg, trigMsk, 0, 10000);
    if (result) return result;

    /* Retrieve Counter Value */
    if (counter < 4) {
        UINT4 uprVal;
        result = hyPhy20gRegFieldRead(fileHndl, uprCntReg, uprCntMsk, &uprVal);
        if (result) return result;

        result = hyPhy20gRegFieldRead(fileHndl, lowCntReg, lowCntMsk, &val);
        if (result) return result;
        *pCount = (uprVal << 16) + val;
    } else {
        result = hyPhy20gRegFieldRead(fileHndl, cntReg, cntMsk, &val);
        if (result) return result;
        *pCount = val;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOtnFramerRxGetCounts */

/*******************************************************************************
**
**  hyPhy20gOtukRxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the OTUk level receive
**                  counters.
**
**                  System dependent, the transfer of counter data to holding
**                  registers is controlled at the subsystem level or the
**                  device level.  The accessMode field allows for the the
**                  usage of local counter accumulation or not.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTUk Framer instance:
**                                0 - OTU1 Framer 0
**                                1 - OTU1 Framer 1
**                                2 - OTU1 Framer 2
**                                3 - OTU1 Framer 3
**                                4 - OTU1 Framer 4
**                                5 - OTU1 Framer 5
**                                6 - OTU1 Framer 6
**                                7 - OTU1 Framer 7
**                                8 - OTU2 Framer 0
**                                9 - OTU2 Framer 1
**                 accessMode   - 1 - locally trigger counter accumulation
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                01 - SM_BIP_COUNTER_VALUE
**                                00 - SM_BEI_COUNTER_VALUE
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtukRxGetCounts(struct file *fileHndl, UINT4 slice,
                             UINT4 accessMode, UINT4 counter, UINT4 *pCount)
{
    UINT4 trigReg = 0;
    UINT4 trigMsk = 0;
    UINT4 cntReg = 0;
    UINT4 cntMsk = 0;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 9 || accessMode > 1 || counter > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pCount == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        switch(counter)
        {
            case 0: /* SM_BEI_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_SM_BEI_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU2_FRM_REG_RX_OTN_OTU_SM_BEI_CNT;
                cntMsk  = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OTU_SM_BEI_CNT_SM_BEI_COUNTER_VALUE_MSK;
                break;
            case 1: /* SM_BIP_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_SM_BIP_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU2_FRM_REG_RX_OTN_OTU_SM_BIP_CNT;
                cntMsk  = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OTU_SM_BIP_CNT_SM_BIP_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
        trigReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice-8);
        cntReg  = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice-8);
    } else {
        /* OTU1 Framer case */
        switch(counter)
        {
            case 0: /* SM_BEI_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_SM_BEI_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU1_FRM_REG_RX_OTN_OTU_SM_BEI_CNT;
                cntMsk  = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OTU_SM_BEI_CNT_SM_BEI_COUNTER_VALUE_MSK;
                break;
            case 1: /* SM_BIP_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_SM_BIP_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU1_FRM_REG_RX_OTN_OTU_SM_BIP_CNT;
                cntMsk  = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OTU_SM_BIP_CNT_SM_BIP_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
        trigReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice);
        cntReg  = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice);
    }

    /* Trigger access with *_LATCH_AND_CLEAR if required */
    if (accessMode == 1) {
        /* Trigger Access */
        result = hyPhy20gRegFieldWrite(fileHndl, trigReg, trigMsk, 1);
        if (result) return result;
    }

    /* Poll *_LATCH_AND_CLEAR bit to ensure cleared for valid counter access */
    result = sysHyPhy20gPollBit(fileHndl, trigReg, trigMsk, 0, 10000);
    if (result) return result;

    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, cntReg, cntMsk, &val);
    if (result) return result;
    *pCount = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtukRxGetCounts */

/*******************************************************************************
**
**  hyPhy20gOdukRxPmTcmiGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the ODUk level receive PM or
**                  TCMi counters.
**
**                  System dependent, the transfer of counter data to holding
**                  registers is controlled at the subsystem level or the
**                  device level.  The accessMode field allows for the the
**                  usage of local counter accumulation or not.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTUk Framer instance:
**                                0 - OTU1 Framer 0
**                                1 - OTU1 Framer 1
**                                2 - OTU1 Framer 2
**                                3 - OTU1 Framer 3
**                                4 - OTU1 Framer 4
**                                5 - OTU1 Framer 5
**                                6 - OTU1 Framer 6
**                                7 - OTU1 Framer 7
**                                8 - OTU2 Framer 0
**                                9 - OTU2 Framer 1
**                 accessMode   - 1 - locally trigger counter accumulation
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                13 - RX_TCM1_BIP_COUNTER_VALUE
**                                12 - RX_TCM1_BEI_COUNTER_VALUE
**                                11 - RX_TCM2_BIP_COUNTER_VALUE
**                                10 - RX_TCM2_BEI_COUNTER_VALUE
**                                09 - RX_TCM3_BIP_COUNTER_VALUE
**                                08 - RX_TCM3_BEI_COUNTER_VALUE
**                                07 - RX_TCM4_BIP_COUNTER_VALUE
**                                06 - RX_TCM4_BEI_COUNTER_VALUE
**                                05 - RX_TCM5_BIP_COUNTER_VALUE
**                                04 - RX_TCM5_BEI_COUNTER_VALUE
**                                03 - RX_TCM6_BIP_COUNTER_VALUE
**                                02 - RX_TCM6_BEI_COUNTER_VALUE
**                                01 - PM_BIP_COUNTER_VALUE
**                                00 - PM_BEI_COUNTER_VALUE
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukRxPmTcmiGetCounts(struct file *fileHndl, UINT4 slice,
                                   UINT4 accessMode, UINT4 counter,
                                   UINT4 *pCount)
{
    UINT4 trigReg = 0;
    UINT4 trigMsk = 0;
    UINT4 cntReg = 0;
    UINT4 cntMsk = 0;
    UINT4 val;
    UINT4 n;
    INT4 result;

    /* argument checking */
    if (slice > 9 || accessMode > 1 || counter > 13) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pCount == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        switch(counter)
        {
            case 0: /* PM_BEI_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_ODU_CNT_CTL_PM_BEI_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU2_FRM_REG_RX_OTN_ODU_PM_BEI_CNT;
                cntMsk  = HYPHY20G_OTU2_FRM_BIT_RX_OTN_ODU_PM_BEI_CNT_PM_BEI_COUNTER_VALUE_MSK;
                break;
            case 1: /* PM_BIP_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_ODU_CNT_CTL_PM_BIP_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU2_FRM_REG_RX_OTN_ODU_PM_BIP_CNT;
                cntMsk  = HYPHY20G_OTU2_FRM_BIT_RX_OTN_ODU_PM_BIP_CNT_PM_BIP_COUNTER_VALUE_MSK;
                break;
            case 2: /* RX_TCMI_BEI_COUNTER_VALUE */
            case 4:
            case 6:
            case 8:
            case 10:
            case 12:
                n       = 6 - counter/2;
                trigMsk = 0x0001 << (HYPHY20G_OTU2_FRM_BIT_RX_OTN_ODU_CNT_CTL_RX_TCM6_BEI_LATCH_AND_CLEAR_OFF + counter - 2);
                cntReg  = HYPHY20G_OTU2_FRM_REG_RX_OTN_ODU_TCMI_BEI_CNT_OFFSET(n);
                cntMsk  = HYPHY20G_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_BEI_CNT_RX_TCMI_BEI_COUNTER_VALUE_MSK;
                break;
            case 3: /* RX_TCMI_BIP_COUNTER_VALUE */
            case 5:
            case 7:
            case 9:
            case 11:
            case 13:
                n       = 6 - (counter-1)/2;
                trigMsk = 0x0001 << (HYPHY20G_OTU2_FRM_BIT_RX_OTN_ODU_CNT_CTL_RX_TCM6_BIP_LATCH_AND_CLEAR_OFF + counter - 3);
                cntReg  = HYPHY20G_OTU2_FRM_REG_RX_OTN_ODU_TCMI_BIP_CNT_OFFSET(n);
                cntMsk  = HYPHY20G_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_BIP_CNT_RX_TCMI_BIP_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_ODU_CNT_CTL;
        trigReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice-8);
        cntReg  = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice-8);
    } else {
        /* OTU1 Framer case */
        switch(counter)
        {
            case 0: /* PM_BEI_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_ODU_CNT_CTL_PM_BEI_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU1_FRM_REG_RX_OTN_ODU_PM_BEI_CNT;
                cntMsk  = HYPHY20G_OTU1_FRM_BIT_RX_OTN_ODU_PM_BEI_CNT_PM_BEI_COUNTER_VALUE_MSK;
                break;
            case 1: /* PM_BIP_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_ODU_CNT_CTL_PM_BIP_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU1_FRM_REG_RX_OTN_ODU_PM_BIP_CNT;
                cntMsk  = HYPHY20G_OTU1_FRM_BIT_RX_OTN_ODU_PM_BIP_CNT_PM_BIP_COUNTER_VALUE_MSK;
                break;
            case 2: /* RX_TCMI_BEI_COUNTER_VALUE */
            case 4:
            case 6:
            case 8:
            case 10:
            case 12:
                n       = 6 - counter/2;
                trigMsk = 0x0001 << (HYPHY20G_OTU1_FRM_BIT_RX_OTN_ODU_CNT_CTL_RX_TCM6_BEI_LATCH_AND_CLEAR_OFF + counter - 2);
                cntReg  = HYPHY20G_OTU1_FRM_REG_RX_OTN_ODU_TCMI_BEI_CNT_OFFSET(n);
                cntMsk  = HYPHY20G_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_BEI_CNT_RX_TCMI_BEI_COUNTER_VALUE_MSK;
                break;
            case 3: /* RX_TCMI_BIP_COUNTER_VALUE */
            case 5:
            case 7:
            case 9:
            case 11:
            case 13:
                n       = 6 - (counter-1)/2;
                trigMsk = 0x0001 << (HYPHY20G_OTU1_FRM_BIT_RX_OTN_ODU_CNT_CTL_RX_TCM6_BIP_LATCH_AND_CLEAR_OFF + counter - 3);
                cntReg  = HYPHY20G_OTU1_FRM_REG_RX_OTN_ODU_TCMI_BIP_CNT_OFFSET(n);
                cntMsk  = HYPHY20G_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_BIP_CNT_RX_TCMI_BIP_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_ODU_CNT_CTL;
        trigReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice);
        cntReg  = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice);
    }

    /* Trigger access with *_LATCH_AND_CLEAR if required */
    if (accessMode == 1) {
        /* Trigger Access */
        result = hyPhy20gRegFieldWrite(fileHndl, trigReg, trigMsk, 1);
        if (result) return result;
    }

    /* Poll *_LATCH_AND_CLEAR bit to ensure cleared for valid counter access */
    result = sysHyPhy20gPollBit(fileHndl, trigReg, trigMsk, 0, 10000);
    if (result) return result;

    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, cntReg, cntMsk, &val);
    if (result) return result;
    *pCount = val;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukRxPmTcmiGetCounts */

/*******************************************************************************
**
**  hyPhy20gOpukRxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the OPUk level receive
**                  counters.
**
**                  System dependent, the transfer of counter data to holding
**                  registers is controlled at the subsystem level or the
**                  device level.  The accessMode field allows for the the
**                  usage of local counter accumulation or not.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTUk Framer instance:
**                                0 - OTU1 Framer 0
**                                1 - OTU1 Framer 1
**                                2 - OTU1 Framer 2
**                                3 - OTU1 Framer 3
**                                4 - OTU1 Framer 4
**                                5 - OTU1 Framer 5
**                                6 - OTU1 Framer 6
**                                7 - OTU1 Framer 7
**                                8 - OTU2 Framer 0
**                                9 - OTU2 Framer 1
**                 accessMode   - 1 - locally trigger counter accumulation
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                02 - RX_PJ_COUNTER_VALUE
**                                01 - RX_NJ_COUNTER_VALUE
**                                00 - PRBS_ERROR_COUNTER_VALUE
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOpukRxGetCounts(struct file *fileHndl, UINT4 slice,
                             UINT4 accessMode, UINT4 counter, UINT4 *pCount)
{
    UINT4 trigReg = 0;
    UINT4 trigMsk = 0;
    UINT4 cntReg = 0;
    UINT4 cntMsk = 0;
    UINT4 uprCntReg = 0;
    UINT4 uprCntMsk = 0;
    UINT4 lowCntReg = 0;
    UINT4 lowCntMsk = 0;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 9 || accessMode > 1 || counter > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pCount == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        switch(counter)
        {
            case 0: /* PRBS_ERROR_COUNTER_VALUE */
                trigMsk   = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_PRBS_ERROR_LATCH_AND_CLEAR_MSK;
                uprCntReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_OPU_PRBS_UPR_ERR_CNT;
                lowCntReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_OPU_PRBS_LOW_ERR;
                uprCntMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_PRBS_UPR_ERR_CNT_PRBS_UPPER_ERROR_COUNTER_HIGH_VALUE_MSK;
                lowCntMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_PRBS_LOW_ERR_PRBS_LOWER_ERROR_COUNTER_HIGH_VALUE_MSK;
                break;
            case 1: /* RX_NJ_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_RX_NJ_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU2_FRM_REG_RX_OTN_OPU_NJ_CNT;
                cntMsk  = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_NJ_CNT_RX_NJ_COUNTER_VALUE_MSK;
                break;
            case 2: /* RX_PJ_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_RX_PJ_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU2_FRM_REG_RX_OTN_OPU_PJ_CNT;
                cntMsk  = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_PJ_CNT_RX_PJ_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg   = HYPHY20G_OTU2_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
        trigReg   = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice-8);
        cntReg    = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice-8);
        uprCntReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(uprCntReg, slice-8);
        lowCntReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(lowCntReg, slice-8);
    } else {
        /* OTU1 Framer case */
        switch(counter)
        {
            case 0: /* PRBS_ERROR_COUNTER_VALUE */
                trigMsk   = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_PRBS_ERROR_LATCH_AND_CLEAR_MSK;
                uprCntReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_OPU_PRBS_UPR_ERR_CNT;
                lowCntReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_OPU_PRBS_LOW_ERR;
                uprCntMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_PRBS_UPR_ERR_CNT_PRBS_UPPER_ERROR_COUNTER_HIGH_VALUE_MSK;
                lowCntMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_PRBS_LOW_ERR_PRBS_LOWER_ERROR_COUNTER_HIGH_VALUE_MSK;
                break;
            case 1: /* RX_NJ_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_RX_NJ_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU1_FRM_REG_RX_OTN_OPU_NJ_CNT;
                cntMsk  = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_NJ_CNT_RX_NJ_COUNTER_VALUE_MSK;
                break;
            case 2: /* RX_PJ_COUNTER_VALUE */
                trigMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_RX_PJ_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_OTU1_FRM_REG_RX_OTN_OPU_PJ_CNT;
                cntMsk  = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_PJ_CNT_RX_PJ_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg   = HYPHY20G_OTU1_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
        trigReg   = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice);
        cntReg    = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice);
        uprCntReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(uprCntReg, slice);
        lowCntReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(lowCntReg, slice);
    }

    /* Trigger access with *_LATCH_AND_CLEAR if required */
    if (accessMode == 1) {
        /* Trigger Access */
        result = hyPhy20gRegFieldWrite(fileHndl, trigReg, trigMsk, 1);
        if (result) return result;
    }

    /* Poll *_LATCH_AND_CLEAR bit to ensure cleared for valid counter access */
    result = sysHyPhy20gPollBit(fileHndl, trigReg, trigMsk, 0, 10000);
    if (result) return result;

    /* Retrieve Counter Value */
    if (counter < 1) {
        UINT4 uprVal;
        result = hyPhy20gRegFieldRead(fileHndl, uprCntReg, uprCntMsk, &uprVal);
        if (result) return result;

        result = hyPhy20gRegFieldRead(fileHndl, lowCntReg, lowCntMsk, &val);
        if (result) return result;
        *pCount = (uprVal << 16) + val;
    } else {
        result = hyPhy20gRegFieldRead(fileHndl, cntReg, cntMsk, &val);
        if (result) return result;
        *pCount = val;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOpukRxGetCounts */

/*******************************************************************************
**
**  hyPhy20gOdtu12TxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the ODTU12 level transmit
**                  counters.
**
**                  System dependent, the transfer of counter data to holding
**                  registers is controlled at the subsystem level or the
**                  device level.  The accessMode field allows for the the
**                  usage of local counter accumulation or not.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - ODTU12 Slice and timeslot number:
**                                0 - ODTU12 Mux 0
**                                1 - ODTU12 Mux 1
**                 accessMode   - 1 - locally trigger counter accumulation
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                02 - TX_PJ_COUNTER_VALUE
**                                01 - TX_PJ2_COUNTER_VALUE
**                                00 - TX_NJ_COUNTER_VALUE
**                *pCount0      - Pointer to storage for time slot 0 data
**                *pCount1      - Pointer to storage for time slot 1 data
**                *pCount2      - Pointer to storage for time slot 2 data
**                *pCount3      - Pointer to storage for time slot 3 data
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdtu12TxGetCounts(struct file *fileHndl, UINT4 slice,
                               UINT4 accessMode, UINT4 counter, UINT4 *pCount0,
                               UINT4 *pCount1, UINT4 *pCount2, UINT4 *pCount3)
{
    UINT4 trigReg = 0;
    UINT4 trigMsk = 0;
    UINT4 cntReg0 = 0, cntReg1 = 0, cntReg2 = 0, cntReg3 = 0;
    UINT4 cntMsk = 0;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 7 || accessMode > 1 || counter > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pCount0 == NULL || pCount1 == NULL || pCount2 == NULL ||
        pCount3 == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* determine registers and masks associated with counter */
    switch(counter)
    {
        case 0: /* RX_NJ_COUNTER_VALUE */
            trigMsk = HYPHY20G_OTU2_ODTU12_BIT_TX_MX_GLBL_CTL_TX_NJ_LATCH_AND_CLEAR_MSK;
            cntReg0 = HYPHY20G_OTU2_ODTU12_REG_TX_MX_TSLT_N_NJ_CNT_OFFSET(0);
            cntReg1 = HYPHY20G_OTU2_ODTU12_REG_TX_MX_TSLT_N_NJ_CNT_OFFSET(1);
            cntReg2 = HYPHY20G_OTU2_ODTU12_REG_TX_MX_TSLT_N_NJ_CNT_OFFSET(2);
            cntReg3 = HYPHY20G_OTU2_ODTU12_REG_TX_MX_TSLT_N_NJ_CNT_OFFSET(3);
            cntMsk  = HYPHY20G_OTU2_ODTU12_BIT_TX_MX_TSLT_N_NJ_CNT_TX_NJ_COUNTER_VALUE_MSK;
            break;
        case 1: /* RX_PJ2_COUNTER_VALUE */
            trigMsk = HYPHY20G_OTU2_ODTU12_BIT_TX_MX_GLBL_CTL_TX_PJ2_LATCH_AND_CLEAR_MSK;
            cntReg0 = HYPHY20G_OTU2_ODTU12_REG_TX_MX_TSLT_N_PJ2_CNT_OFFSET(0);
            cntReg1 = HYPHY20G_OTU2_ODTU12_REG_TX_MX_TSLT_N_PJ2_CNT_OFFSET(1);
            cntReg2 = HYPHY20G_OTU2_ODTU12_REG_TX_MX_TSLT_N_PJ2_CNT_OFFSET(2);
            cntReg3 = HYPHY20G_OTU2_ODTU12_REG_TX_MX_TSLT_N_PJ2_CNT_OFFSET(3);
            cntMsk  = HYPHY20G_OTU2_ODTU12_BIT_TX_MX_TSLT_N_PJ2_CNT_TX_PJ2_COUNTER_VALUE_MSK;
            break;
        case 2: /* RX_PJ_COUNTER_VALUE */
            trigMsk = HYPHY20G_OTU2_ODTU12_BIT_TX_MX_GLBL_CTL_TX_PJ_LATCH_AND_CLEAR_MSK;
            cntReg0 = HYPHY20G_OTU2_ODTU12_REG_TX_MX_TSLT_N_PJ_CNT_OFFSET(0);
            cntReg1 = HYPHY20G_OTU2_ODTU12_REG_TX_MX_TSLT_N_PJ_CNT_OFFSET(1);
            cntReg2 = HYPHY20G_OTU2_ODTU12_REG_TX_MX_TSLT_N_PJ_CNT_OFFSET(2);
            cntReg3 = HYPHY20G_OTU2_ODTU12_REG_TX_MX_TSLT_N_PJ_CNT_OFFSET(3);
            cntMsk  = HYPHY20G_OTU2_ODTU12_BIT_TX_MX_TSLT_N_PJ_CNT_TX_PJ_COUNTER_VALUE_MSK;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    trigReg = HYPHY20G_OTU2_ODTU12_REG_TX_MX_GLBL_CTL;
    trigReg = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(trigReg, slice);

    cntReg0 = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(cntReg0, slice);
    cntReg1 = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(cntReg1, slice);
    cntReg2 = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(cntReg2, slice);
    cntReg3 = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(cntReg3, slice);


    /* Trigger access with *_LATCH_AND_CLEAR if required */
    if (accessMode == 1) {
        /* Trigger Access */
        result = hyPhy20gRegFieldWrite(fileHndl, trigReg, trigMsk, 1);
        if (result) return result;
    }

    /* Poll *_LATCH_AND_CLEAR bit to ensure cleared for valid counter access */
    result = sysHyPhy20gPollBit(fileHndl, trigReg, trigMsk, 0, 10000);
    if (result) return result;

    /* Retrieve Counter Values */
    result = hyPhy20gRegFieldRead(fileHndl, cntReg0, cntMsk, &val);
    if (result) return result;
    *pCount0 = val;

    result = hyPhy20gRegFieldRead(fileHndl, cntReg1, cntMsk, &val);
    if (result) return result;
    *pCount1 = val;

    result = hyPhy20gRegFieldRead(fileHndl, cntReg2, cntMsk, &val);
    if (result) return result;
    *pCount2 = val;

    result = hyPhy20gRegFieldRead(fileHndl, cntReg3, cntMsk, &val);
    if (result) return result;
    *pCount3 = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdtu12TxGetCounts */

/*******************************************************************************
**
**  hyPhy20gOdtu12RxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the ODTU12 level receive
**                  counters for the four ODU1 time slots.
**
**                  System dependent, the transfer of counter data to holding
**                  registers is controlled at the subsystem level or the
**                  device level.  The accessMode field allows for the the
**                  usage of local counter accumulation or not.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - ODTU12 Slice and timeslot number:
**                                0 - ODTU12 Mux 0
**                                1 - ODTU12 Mux 1
**                 accessMode   - 1 - locally trigger counter accumulation
**                              - 0 - do not locally trigger counter
**                                    accumulation
**                 counter      - Counter to be accessed:
**                                02 - RX_PJ_COUNTER_VALUE
**                                01 - RX_PJ2_COUNTER_VALUE
**                                00 - RX_NJ_COUNTER_VALUE
**                *pCount0      - Pointer to storage for time slot 0 data
**                *pCount1      - Pointer to storage for time slot 1 data
**                *pCount2      - Pointer to storage for time slot 2 data
**                *pCount3      - Pointer to storage for time slot 3 data
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdtu12RxGetCounts(struct file *fileHndl, UINT4 slice,
                               UINT4 accessMode, UINT4 counter, UINT4 *pCount0,
                               UINT4 *pCount1, UINT4 *pCount2, UINT4 *pCount3)
{
    UINT4 trigReg = 0;
    UINT4 trigMsk = 0;
    UINT4 cntReg0 = 0, cntReg1 = 0, cntReg2 = 0, cntReg3 = 0;
    UINT4 cntMsk = 0;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 1 || accessMode > 1 || counter > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pCount0 == NULL || pCount1 == NULL || pCount2 == NULL ||
        pCount3 == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* determine registers and masks associated with counter */
    switch(counter)
    {
        case 0: /* RX_NJ_COUNTER_VALUE */
            trigMsk = HYPHY20G_OTU2_ODTU12_BIT_RX_MX_GLBL_CTL_RX_NJ_LATCH_AND_CLEAR_MSK;
            cntReg0 = HYPHY20G_OTU2_ODTU12_REG_RX_MX_TSLT_N_NJ_CNT_OFFSET(0);
            cntReg1 = HYPHY20G_OTU2_ODTU12_REG_RX_MX_TSLT_N_NJ_CNT_OFFSET(1);
            cntReg2 = HYPHY20G_OTU2_ODTU12_REG_RX_MX_TSLT_N_NJ_CNT_OFFSET(2);
            cntReg3 = HYPHY20G_OTU2_ODTU12_REG_RX_MX_TSLT_N_NJ_CNT_OFFSET(3);
            cntMsk  = HYPHY20G_OTU2_ODTU12_BIT_RX_MX_TSLT_N_NJ_CNT_RX_NJ_COUNTER_VALUE_MSK;
            break;
        case 1: /* RX_PJ2_COUNTER_VALUE */
            trigMsk = HYPHY20G_OTU2_ODTU12_BIT_RX_MX_GLBL_CTL_RX_PJ2_LATCH_AND_CLEAR_MSK;
            cntReg0 = HYPHY20G_OTU2_ODTU12_REG_RX_MX_TSLT_N_PJ2_CNT_OFFSET(0);
            cntReg1 = HYPHY20G_OTU2_ODTU12_REG_RX_MX_TSLT_N_PJ2_CNT_OFFSET(1);
            cntReg2 = HYPHY20G_OTU2_ODTU12_REG_RX_MX_TSLT_N_PJ2_CNT_OFFSET(2);
            cntReg3 = HYPHY20G_OTU2_ODTU12_REG_RX_MX_TSLT_N_PJ2_CNT_OFFSET(3);
            cntMsk  = HYPHY20G_OTU2_ODTU12_BIT_RX_MX_TSLT_N_PJ2_CNT_RX_PJ2_COUNTER_VALUE_MSK;
            break;
        case 2: /* RX_PJ_COUNTER_VALUE */
            trigMsk = HYPHY20G_OTU2_ODTU12_BIT_RX_MX_GLBL_CTL_RX_PJ_LATCH_AND_CLEAR_MSK;
            cntReg0 = HYPHY20G_OTU2_ODTU12_REG_RX_MX_TSLT_N_PJ_CNT_OFFSET(0);
            cntReg1 = HYPHY20G_OTU2_ODTU12_REG_RX_MX_TSLT_N_PJ_CNT_OFFSET(1);
            cntReg2 = HYPHY20G_OTU2_ODTU12_REG_RX_MX_TSLT_N_PJ_CNT_OFFSET(2);
            cntReg3 = HYPHY20G_OTU2_ODTU12_REG_RX_MX_TSLT_N_PJ_CNT_OFFSET(3);
            cntMsk  = HYPHY20G_OTU2_ODTU12_BIT_RX_MX_TSLT_N_PJ_CNT_RX_PJ_COUNTER_VALUE_MSK;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    trigReg = HYPHY20G_OTU2_ODTU12_REG_RX_MX_GLBL_CTL;
    trigReg = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(trigReg, slice);

    cntReg0 = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(cntReg0, slice);
    cntReg1 = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(cntReg1, slice);
    cntReg2 = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(cntReg2, slice);
    cntReg3 = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(cntReg3, slice);


    /* Trigger access with *_LATCH_AND_CLEAR if required */
    if (accessMode == 1) {
        /* Trigger Access */
        result = hyPhy20gRegFieldWrite(fileHndl, trigReg, trigMsk, 1);
        if (result) return result;
    }

    /* Poll *_LATCH_AND_CLEAR bit to ensure cleared for valid counter access */
    result = sysHyPhy20gPollBit(fileHndl, trigReg, trigMsk, 0, 10000);
    if (result) return result;

    /* Retrieve Counter Values */
    result = hyPhy20gRegFieldRead(fileHndl, cntReg0, cntMsk, &val);
    if (result) return result;
    *pCount0 = val;

    result = hyPhy20gRegFieldRead(fileHndl, cntReg1, cntMsk, &val);
    if (result) return result;
    *pCount1 = val;

    result = hyPhy20gRegFieldRead(fileHndl, cntReg2, cntMsk, &val);
    if (result) return result;
    *pCount2 = val;

    result = hyPhy20gRegFieldRead(fileHndl, cntReg3, cntMsk, &val);
    if (result) return result;
    *pCount3 = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdtu12RxGetCounts */

/*******************************************************************************
**
**  hyPhy20gOtuI7EfecGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts I.7 FEC Counters.  It is only
**                  relevant for an OTU2 framer configured for I.7 FEC mode of
**                  operation.
**
**                  Prior to accessing counters, the counter registers must be
**                  updated by triggering counter accumulation.  Counters
**                  may be accumulated at the subsystem level or the device
**                  level.
**
**                  Counter accumulation applies to both OTU2 framer instances.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTU2 Framer instance:
**                                0 - OTU2 Framer 0
**                                1 - OTU2 Framer 1
**                *pCount0      - Correctable Ones Counter data
**                *pCount1      - Correctable Zeros Counter data
**                *pCount2      - Reserved - return 0
**                *pCount3      - Reserved - return 0
**                *pCount4      - Uncorrectable Row 3 Errors Counter data
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtuI7EfecGetCounts(struct file *fileHndl, UINT4 slice,
                                UINT4 *pCount0, UINT4 *pCount1, UINT4 *pCount2,
                                UINT4 *pCount3, UINT4 *pCount4)
{
    UINT4 addr, val;
    INT4 result;

    /* argument checking */
    if(slice > 1){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Retrieve Correctable Ones Counter data */
    addr = HYPHY20G_EFEC_REG_I7_CORR_ONE_CNT;
    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pCount0 = val;

    /* Retrieve Correctable Zeros Counter data */
    addr = HYPHY20G_EFEC_REG_I7_CORR_ZRO_CNT;
    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pCount1 = val;

    /* Retrieve Uncorrectable Row 1 Errors Counter data */
    addr = HYPHY20G_EFEC_REG_I7_UNCORR_ROW1_ERR_CNT;
    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    /* *pCount2 = val; // Hide the count in the intermediate stage */
    *pCount2 = 0;

    /* Retrieve Uncorrectable Row 2 Errors Counter data */
    addr = HYPHY20G_EFEC_REG_I7_UNCORR_ROW2_ERR_CNT;
    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    /* *pCount3 = val; // Hide the count in the intermediate stage */
    *pCount3 = 0;

    /* Retrieve Uncorrectable Row 3 Errors Counter data */
    addr = HYPHY20G_EFEC_REG_I7_UNCORR_ROW3_ERR_CNT;
    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pCount4 = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtuI7EfecGetCounts */



/*******************************************************************************
**
**  hyPhy20gOtuI4EfecGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts I.7 FEC Counters.  It is only
**                  relevant for an OTU2 framer configured for I.7 FEC mode of
**                  operation.
**
**                  Prior to accessing counters, the counter registers must be
**                  updated by triggering counter accumulation.  Counters
**                  may be accumulated at the subsystem level or the device
**                  level.
**
**                  Counter accumulation applies to both OTU2 framer instances.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTU2 Framer instance:
**                                0 - OTU2 Framer 0
**                                1 - OTU2 Framer 1
**                *pCount0      - Correctable Ones Counter data
**                *pCount1      - Correctable Zeros Counter data
**                *pCount2      - Reserved - return 0
**                *pCount3      - Reserved - return 0
**                *pCount4      - Reserved - return 0
**                *pCount5      - Uncorrectable BCH2 Errors Counter data
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtuI4EfecGetCounts(struct file *fileHndl, UINT4 slice,
                                UINT4 *pCount0, UINT4 *pCount1, UINT4 *pCount2,
                                UINT4 *pCount3, UINT4 *pCount4, UINT4 *pCount5)
{
    UINT4 addr, val;
    INT4 result;

    /* argument checking */
    if(slice > 1){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Retrieve Correctable Ones Counter data  */
    addr = HYPHY20G_EFEC_REG_OTU2_I4_CORR_ONE_CNT;
    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pCount0 = val;

    /* Retrieve Correctable Zeros Counter data  */
    addr = HYPHY20G_EFEC_REG_OTU2_I4_CORR_ZRO_CNT;
    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pCount1 = val;

    /* Retrieve Uncorrectable RS1 Errors Counter data  */
    addr = HYPHY20G_EFEC_REG_OTU2_I4_UNCORR_RS1_ERR_CNT;
    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    /* *pCount2 = val; // Hide the count in the intermediate stage*/
    *pCount2 = 0;

    /* Retrieve Uncorrectable RS2 Errors Counter data  */
    addr = HYPHY20G_EFEC_REG_I4_UNCORR_RS2_ERR_CNT;
    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    /* *pCount3 = val; // Hide the count in the intermediate stage*/
    *pCount3 = 0;

    /* Retrieve Uncorrectable BCH1 Errors Counter data */
    addr = HYPHY20G_EFEC_REG_I4_UNCORR_BCH1_ERR_CNT;
    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    /* *pCount4 = val; // Hide the count in the intermediate stage*/
    *pCount4 = 0;

    /* Retrieve Uncorrectable BCH2 Errors Counter data */
    addr = HYPHY20G_EFEC_REG_I4_UNCORR_BCH2_ERR_CNT;
    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pCount5 = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtuI4EfecGetCounts */

/*******************************************************************************
**
**  hyPhy20gOtuEfecAccumCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function trigger OTU2 subsystem FEC counter
**                  accumulation for one of the OTU2 framer instances.
**                  Alternatively FEC counter accumulation may be triggered at
**                  the device level.
**
**                  This trigger is only relevant for OTU2 framers that
**                  are configured for I.4 or I.7 FEC modes of operation.
**
**                  Function returns HYPHY20G_ERR_POLL_TIMEOUT if TIP bit
**                  does not clear.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTU2 Framer instance
**                                0 - OTU2 Framer 0
**                                1 - OTU2 Framer 1
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtuEfecAccumCounts(struct file *fileHndl, UINT4 slice)
{
    UINT4 addr, msk;
    INT4 result;

    /* argument checking */
    if(slice > 1){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_EFEC_REG_CFG;
    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(addr, slice);
    msk  = HYPHY20G_EFEC_BIT_CFG_TIP_MSK;

    /* write TIP bit to 1 */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if (result) return result;

    /* poll TIP bit to go to 0, if not return an error */
    result = sysHyPhy20gPollBit(fileHndl, addr, msk, 0, 10000);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtuEfecAccumCounts */

/*******************************************************************************
**
**  hyPhy20gOtukAccumCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function trigger OTU2/OTU1 subsystem frame counter
**                  accumulation at the OTU2/OTU1 subsystem level.  It may
**                  further accumulate OTU2/OTU1/ODTU12 framer counters when
**                  TX_ERR_SYNC_MODE and RX_ERR_SYNC_MODE bits are set to 1.
**
**                  OTU2/OTU1 frame counter are accessed with:
**                  - hyPhy20gOtukFrmGetCounts
**
**                  OTU2/OTU1/ODTU12 framer counters are accessed with:
**                  - hyPhy20gOpukTxGetCounts
**                  - hyPhy20gOdukTcmiTxGetCounts
**                  - hyPhy20gOtnFramerRxGetCounts
**                  - hyPhy20gOtukRxGetCounts
**                  - hyPhy20gOdukRxPmTcmiGetCounts
**                  - hyPhy20gOpukRxGetCounts
**                  - hyPhy20gOdtu12TxGetCounts
**                  - hyPhy20gOdtu12RxGetCounts
**
**                  OTU2/OTU1/ODTU12 TX_ERR_SYNC_MODE and RX_ERR_SYNC_MODE are
**                  controlled with:
**                  - hyPhy20gOtnCfgCountTrig
**
**                  Alternatively the combined counters selected may be
**                  accumulated at the device level trigger.
**
**                  There are two subsystem level counter accumulators.  The
**                  OTU2 framer counter accumulator accumulates counts for
**                  all OTU2 framers and ODTU12 mux.  The OTU1 framer counter
**                  accumulater accumulates counts for all OTU1 framers.
**
**
**  INPUTS:       *fileHndl     - base address
**                 subsystem    - Framer type to accumulate counters for
**                              - 0 - OTU1 framers 0 to 7
**                              - 1 - OTU2 framers 0 to 1
**                              - 2 - Both OTU1 and OTU2 framers
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtukAccumCounts(struct file *fileHndl, UINT4 subsystem)
{
    UINT4 addr, msk;
    INT4 result;

    /* argument checking */
    if(subsystem > 2){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if((subsystem == 0)||(subsystem == 2)){
        addr = HYPHY20G_OTU1_TOP_REG_CFG;
        msk = HYPHY20G_OTU1_TOP_BIT_CFG_TIP_MSK;
        /* write TIP bit to 1 */
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
        if (result) return result;

        /* poll TIP bit to go to 0, if not return an error */
        result = sysHyPhy20gPollBit(fileHndl, addr, msk, 0, 10000);
        if(result) return result;
    }

    if((subsystem == 1)||(subsystem == 2)){
        addr = HYPHY20G_OTU2_TOP_REG_CFG;
        msk = HYPHY20G_OTU2_TOP_BIT_CFG_TIP_MSK;
        /* write TIP bit to 1 */
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
        if (result) return result;

        /* poll TIP bit to go to 0, if not return an error */
        result = sysHyPhy20gPollBit(fileHndl, addr, msk, 0, 10000);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtukAccumCounts */


/*******************************************************************************
**
**  hyPhy20gOtnCountTrigCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the OTU2/OTU1 framer counter
**                  accumulation control signals.  It allows counters from
**                  OTU2/OTU1 framers and ODTU12 Mux to be controlled at the
**                  device or subsystem level.  Alternatively, it disables
**                  accumulation at this level at a per slice granularity.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTUk Framer instance:
**                                0 - OTU1 Framer 0
**                                1 - OTU1 Framer 1
**                                2 - OTU1 Framer 2
**                                3 - OTU1 Framer 3
**                                4 - OTU1 Framer 4
**                                5 - OTU1 Framer 5
**                                6 - OTU1 Framer 6
**                                7 - OTU1 Framer 7
**                                8 - OTU2 Framer 0
**                                9 - OTU2 Framer 1
**                                10 - ODTU12 Mux 0
**                                11 - ODTU12 Mux 1
**                                12 - All
**                 dir          - Counter direction to be configured
**                                0 - Transmit Counters
**                                1 - Receive Counters
**                                2 - Both Transmit and Receive Counters
**                 enbl         - Enable subsystem level TIP control of counters
**                                0 - Disabled
**                                1 - Enabled
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtnCountTrigCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                             UINT4 enbl)
{
    UINT4 addr, msk;
    UINT4 i;
    INT4 result;

    /* argument checking */
    if((slice > 12)||(dir > 2)||(enbl > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if((slice == 12)&&((dir == 0)||(dir == 2))){
        /* Configure All OTU1 Tx Framer */
        for(i=0; i < 8; i++){
            addr = HYPHY20G_OTU1_FRM_REG_TX_OTN_CNT_CTL;
            addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, i);
            msk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_CNT_CTL_TX_ERR_SYNC_MODE_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
            if (result) return result;
        }
        /* Configure All OTU2 Tx Framer */
        for(i=0; i < 1; i++){
            addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_CNT_CTL;
            addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, i);
            msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_CNT_CTL_TX_ERR_SYNC_MODE_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
            if (result) return result;
        }
        /* Configure All ODTU12 Tx Mux */
        for(i=0; i < 1; i++){
            addr = HYPHY20G_OTU2_ODTU12_REG_TX_MX_GLBL_CTL;
            addr = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(addr, i);
            msk = HYPHY20G_OTU2_ODTU12_BIT_TX_MX_GLBL_CTL_TX_ERR_SYNC_MODE_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
            if (result) return result;
        }
    }

    if((slice == 12)&&((dir == 1)||(dir == 2))){
        /* Configure All OTU1 Rx Framer */
        for(i=0; i < 8; i++){
            addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_GLBL_CTL;
            addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, i);
            msk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
            if (result) return result;
        }
        /* Configure All OTU2 Rx Framer */
        for(i=0; i < 1; i++){
            addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
            addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, i);
            msk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
            if (result) return result;
        }
        /* Configure All ODTU12 Rx Mux */
        for(i=0; i < 1; i++){
            addr = HYPHY20G_OTU2_ODTU12_REG_RX_MX_GLBL_CTL;
            addr = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(addr, i);
            msk = HYPHY20G_OTU2_ODTU12_BIT_RX_MX_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
            if (result) return result;
        }
    }

    if((slice < 8)&&((dir == 0)||(dir == 2))){
        /* Configure Single OTU1 Tx Framer */
        addr = HYPHY20G_OTU1_FRM_REG_TX_OTN_CNT_CTL;
        addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        msk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_CNT_CTL_TX_ERR_SYNC_MODE_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
        if (result) return result;
    }

    if((slice < 8)&&((dir == 1)||(dir == 2))){
        /* Configure Single OTU1 Rx Framer */
        addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_GLBL_CTL;
        addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        msk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
        if (result) return result;
    }

    if(((slice > 7)&&(slice < 10))&&((dir == 0)||(dir == 2))){
        /* Configure Single OTU2 Tx Framer */
        addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_CNT_CTL;
        addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice - 8);
        msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_CNT_CTL_TX_ERR_SYNC_MODE_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
        if (result) return result;
    }

    if(((slice > 7)&&(slice < 10))&&((dir == 1)||(dir == 2))){
        /* Configure Single OTU2 Rx Framer */
        addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
        addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice - 8);
        msk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
        if (result) return result;
    }

    if(((slice > 9)&&(slice < 12))&&((dir == 0)||(dir == 2))){
        /* Configure Single ODTU12 Tx Mux */
        addr = HYPHY20G_OTU2_ODTU12_REG_TX_MX_GLBL_CTL;
        addr = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(addr, slice - 10);
        msk = HYPHY20G_OTU2_ODTU12_BIT_TX_MX_GLBL_CTL_TX_ERR_SYNC_MODE_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
        if (result) return result;
    }

    if(((slice > 9)&&(slice < 12))&&((dir == 1)||(dir == 2))){
        /* Configure Single ODTU12 Rx Mux */
        addr = HYPHY20G_OTU2_ODTU12_REG_RX_MX_GLBL_CTL;
        addr = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(addr, slice - 10);
        msk = HYPHY20G_OTU2_ODTU12_BIT_RX_MX_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtnCountTrigCfg */



/*******************************************************************************
**
**  hyPhy20gOtukFrmGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts OTUk Frame Counts for an OTU2/OTU1
**                  Slice.
**
**                  Prior to accessing counters, the counter registers must be
**                  updated by triggering counter accumulation.  Counters
**                  may be accumulated at the subsystem or device level.
**                  hyPhy20gOtukAccumCounts triggers counter accumulation at the
**                  subsystem level.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTUk Framer instance:
**                                0 - OTU1 Framer 0
**                                1 - OTU1 Framer 1
**                                2 - OTU1 Framer 2
**                                3 - OTU1 Framer 3
**                                4 - OTU1 Framer 4
**                                5 - OTU1 Framer 5
**                                6 - OTU1 Framer 6
**                                7 - OTU1 Framer 7
**                                8 - OTU2 Framer 0
**                                9 - OTU2 Framer 1
**                *pCount       - Receive Frame Counter data
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtukFrmGetCounts(struct file *fileHndl, UINT4 slice, UINT4 *pCount)
{
    UINT4 addr, val;
    INT4 result;

    /* argument checking */
    if(slice > 9){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (slice < 8) {
        addr = HYPHY20G_OTU1_TOP_REG_RX_FRM_CNT_N_OFFSET(slice);
    } else {
        addr = HYPHY20G_OTU2_TOP_REG_RX_FRM_CNT_N_OFFSET(slice-8);
    }

    /* Retrieve Counter Value */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pCount = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtukFrmGetCounts */



/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
