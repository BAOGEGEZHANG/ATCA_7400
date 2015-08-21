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
**  FILE        : hyphy20g_odukpp_counters.c
**
**  $Date: 2010-05-28 17:57:06 -0700 (Fri, 28 May 2010) $
**
**  $Revision: 7990 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_odukpp_counters.h"

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
**  hyPhy20gOdukppOpukTxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the OPUk level transmit
**                  counters in ODUKPP subsystem.
**
**                  System dependent, the transfer of counter data to holding
**                  registers is controlled at the subsystem level or the
**                  device level.  The accessMode field allows for the usage of  
**                  local counter accumulation or not.
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
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gOdukppOpukTxGetCounts(struct file *fileHndl, UINT4 slice,
                                   UINT4 accessMode, UINT4 counter, 
                                   UINT4 *pCount)
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
                trigReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_CNT_CTL;
                trigReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, 
                                                                       slice-8);
                trigMsk = 
          HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_CNT_CTL_TX_NJ_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OPU_NJ_CNT;
                cntReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, 
                                                                       slice-8);
                cntMsk = 
         HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_NJ_CNT_TX_NJ_COUNTER_VALUE_MSK;
                break;
            case 1: /* TX_PJ_COUNTER_VALUE */
                trigReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_CNT_CTL;
                trigReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, 
                                                                       slice-8);
                trigMsk = 
          HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_CNT_CTL_TX_PJ_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_OTU2_FRM_REG_TX_OTN_OPU_PJ_CNT;
                cntReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, 
                                                                       slice-8);
                cntMsk = 
         HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_PJ_CNT_TX_PJ_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
    } else {
        /* OTU1 Framer case */
        switch(counter)
        {            
            case 0: /* TX_NJ_COUNTER_VALUE */
                trigReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_CNT_CTL;
                trigReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, 
                                                                        slice);
                trigMsk = 
          HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_CNT_CTL_TX_NJ_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OPU_NJ_CNT;
                cntReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, 
                                                                       slice);
                cntMsk = 
         HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_NJ_CNT_TX_NJ_COUNTER_VALUE_MSK;
                break;
            case 1: /* TX_PJ_COUNTER_VALUE */
                trigReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_CNT_CTL;
                trigReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, 
                                                                        slice);
                trigMsk = 
          HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_CNT_CTL_TX_PJ_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_OTU1_FRM_REG_TX_OTN_OPU_PJ_CNT;
                cntReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, 
                                                                       slice);
                cntMsk = 
         HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_PJ_CNT_TX_PJ_COUNTER_VALUE_MSK;
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
} /* hyPhy20gOdukppOpukTxGetCounts */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTcmiTxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the ODUk level transmit
**                  TCMi counters in ODUKPP subsystem.
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
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTcmiTxGetCounts(struct file *fileHndl, UINT4 slice,
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
                cntReg = 
             HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_INT_BEI_CNT_OFFSET(n);
                cntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_INT_BEI_CNT_TX_TCMI_BEI_COUNTER_VALUE_MSK;
                break;
            case 1: /* TX_TCMI_BIP_COUNTER_VALUE */
                n      = 6 - (counter)/2;
                cntReg = 
             HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_INT_BIP_CNT_OFFSET(n);
                cntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_INT_BIP_CNT_TX_TCMI_BIP_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_TCMI_INTRP_CNT_CTL;
        trigReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, 
                                                                slice-8);
        trigMsk = 0x0001 << (HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_TCMI_INTRP_CNT_CTL_TX_TCM6_BEI_LATCH_AND_CLEAR_OFF + counter);
        cntReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice-8);
    } else {
        /* OTU1 Framer case */
        switch(counter%2)
        {
            case 0: /* TX_TCMI_BEI_COUNTER_VALUE */
                n      = 6 - (counter+1)/2;
                cntReg = 
             HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_INT_BEI_CNT_OFFSET(n);
                cntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_INT_BEI_CNT_TX_TCMI_BEI_COUNTER_VALUE_MSK;
                break;
            case 1: /* TX_TCMI_BIP_COUNTER_VALUE */
                n      = 6 - (counter)/2;
                cntReg =
             HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_INT_BIP_CNT_OFFSET(n);
                cntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_INT_BIP_CNT_TX_TCMI_BIP_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_TCMI_INTRP_CNT_CTL;
        trigReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice);
        trigMsk = 0x0001 << (HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_TCMI_INTRP_CNT_CTL_TX_TCM6_BEI_LATCH_AND_CLEAR_OFF + counter);
        cntReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice);
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
} /* hyPhy20gOdukppOdukTcmiTxGetCounts */

/*******************************************************************************
**
**  hyPhy20gOdukppFramerRxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the OTN Framer level 
**                  receive counters in ODUKPP subsystem.
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
**                                01 - OOF_COUNTER_VALUE
**                                00 - OOM_COUNTER_VALUE
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gOdukppFramerRxGetCounts(struct file *fileHndl, UINT4 slice,
                                     UINT4 accessMode, UINT4 counter,
                                     UINT4 *pCount)
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
            case 0: /* OOM_COUNTER_VALUE */
                trigMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_OOM_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_FA_OOM_CNT;
                cntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_OOM_CNT_OOM_COUNTER_VALUE_MSK;
                break;
            case 1: /* OOF_COUNTER_VALUE */
                trigMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_OOF_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_FA_OOF_CNT;
                cntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_OOF_CNT_OOF_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }

        trigReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
        trigReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, 
                                                                slice-8);
        cntReg  = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, 
                                                                slice-8);
    } else {
        /* OTU1 Framer case */
        switch(counter)
        {
            case 0: /* OOM_COUNTER_VALUE */
                trigMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_OOM_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_OOM_CNT;
                cntMsk  = 
           HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_OOM_CNT_OOM_COUNTER_VALUE_MSK;
                break;
            case 1: /* OOF_COUNTER_VALUE */
                trigMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_OOF_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_OOF_CNT;
                cntMsk  = 
           HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_OOF_CNT_OOF_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }

        trigReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
        trigReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice);
        cntReg  = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice);
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
} /* hyPhy20gOdukppFramerRxGetCounts */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the OTUk level receive
**                  counters in ODUKPP subsystem.
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
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxGetCounts(struct file *fileHndl, UINT4 slice,
                                   UINT4 accessMode, UINT4 counter, 
                                   UINT4 *pCount)
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
                trigMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_SM_BEI_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_SM_BEI_CNT;
                cntMsk  = 
    HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_BEI_CNT_SM_BEI_COUNTER_VALUE_MSK;
                break;
            case 1: /* SM_BIP_COUNTER_VALUE */
                trigMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_SM_BIP_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_SM_BIP_CNT;
                cntMsk  = 
    HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_BIP_CNT_SM_BIP_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
        trigReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, 
                                                                slice-8);
        cntReg  = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, 
                                                                slice-8);
    } else {
        /* OTU1 Framer case */
        switch(counter)
        {
            case 0: /* SM_BEI_COUNTER_VALUE */
                trigMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_SM_BEI_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_SM_BEI_CNT;
                cntMsk  = 
    HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_BEI_CNT_SM_BEI_COUNTER_VALUE_MSK;
                break;
            case 1: /* SM_BIP_COUNTER_VALUE */
                trigMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_SM_BIP_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_SM_BIP_CNT;
                cntMsk  = 
    HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_BIP_CNT_SM_BIP_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
        trigReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice);
        cntReg  = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice);
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

} /* hyPhy20gOdukppOtukRxGetCounts */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxPmTcmiGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the ODUk level receive PM or
**                  TCMi counters in ODUKPP subsystem.
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
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxPmTcmiGetCounts(struct file *fileHndl, UINT4 slice,
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
                trigMsk = 
     HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_CNT_CTL_PM_BEI_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_PM_BEI_CNT;
                cntMsk  = 
    HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_PM_BEI_CNT_PM_BEI_COUNTER_VALUE_MSK;
                break;
            case 1: /* PM_BIP_COUNTER_VALUE */
                trigMsk = 
     HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_CNT_CTL_PM_BIP_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_PM_BIP_CNT;
                cntMsk  = 
    HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_PM_BIP_CNT_PM_BIP_COUNTER_VALUE_MSK;
                break;
            case 2: /* RX_TCMI_BEI_COUNTER_VALUE */
            case 4:
            case 6:
            case 8:
            case 10:
            case 12:
                n       = 6 - counter/2;
                trigMsk = 0x0001 << (HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_CNT_CTL_RX_TCM6_BEI_LATCH_AND_CLEAR_OFF 
                                                                 + counter - 2);
                cntReg = 
                 HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_BEI_CNT_OFFSET(n);
                cntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_BEI_CNT_RX_TCMI_BEI_COUNTER_VALUE_MSK;
                break;
            case 3: /* RX_TCMI_BIP_COUNTER_VALUE */
            case 5:
            case 7:
            case 9:
            case 11:
            case 13:
                n       = 6 - (counter-1)/2;
                trigMsk = 0x0001 << (HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_CNT_CTL_RX_TCM6_BIP_LATCH_AND_CLEAR_OFF
                                                                 + counter - 3);
                cntReg  = 
                 HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_BIP_CNT_OFFSET(n);
                cntMsk  = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_BIP_CNT_RX_TCMI_BIP_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_CNT_CTL;
        trigReg = 
                HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice-8);
        cntReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice-8);
    } else {
        /* OTU1 Framer case */
        switch(counter)
        {
            case 0: /* PM_BEI_COUNTER_VALUE */
                trigMsk = 
     HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_CNT_CTL_PM_BEI_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_PM_BEI_CNT;
                cntMsk = 
    HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_PM_BEI_CNT_PM_BEI_COUNTER_VALUE_MSK;
                break;
            case 1: /* PM_BIP_COUNTER_VALUE */
                trigMsk = 
     HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_CNT_CTL_PM_BIP_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_PM_BIP_CNT;
                cntMsk  = 
    HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_PM_BIP_CNT_PM_BIP_COUNTER_VALUE_MSK;
                break;
            case 2: /* RX_TCMI_BEI_COUNTER_VALUE */
            case 4:
            case 6:
            case 8:
            case 10:
            case 12:
                n       = 6 - counter/2;
                trigMsk = 0x0001 << (HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_CNT_CTL_RX_TCM6_BEI_LATCH_AND_CLEAR_OFF 
                                                                 + counter - 2);
                cntReg = 
                 HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_BEI_CNT_OFFSET(n);
                cntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_BEI_CNT_RX_TCMI_BEI_COUNTER_VALUE_MSK;
                break;
            case 3: /* RX_TCMI_BIP_COUNTER_VALUE */
            case 5:
            case 7:
            case 9:
            case 11:
            case 13:
                n       = 6 - (counter-1)/2;
                trigMsk = 0x0001 << (HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_CNT_CTL_RX_TCM6_BIP_LATCH_AND_CLEAR_OFF 
                                                                 + counter - 3);
                cntReg = 
                 HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_BIP_CNT_OFFSET(n);
                cntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_BIP_CNT_RX_TCMI_BIP_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_CNT_CTL;
        trigReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice);
        cntReg  = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice);
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
} /* hyPhy20gOdukppOdukRxPmTcmiGetCounts */

/*******************************************************************************
**
**  hyPhy20gOdukppOpukRxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts one of the OPUk level receive
**                  counters in ODUKPP subsystem.
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
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gOdukppOpukRxGetCounts(struct file *fileHndl, UINT4 slice,
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
                trigMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_PRBS_ERROR_LATCH_AND_CLEAR_MSK;
                uprCntReg = 
                       HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OPU_PRBS_UPR_ERR_CNT;
                lowCntReg = 
                           HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OPU_PRBS_LOW_ERR;
                uprCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OPU_PRBS_UPR_ERR_CNT_PRBS_UPPER_ERROR_COUNTER_HIGH_VALUE_MSK;
                lowCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OPU_PRBS_LOW_ERR_PRBS_LOWER_ERROR_COUNTER_HIGH_VALUE_MSK;
                break;
            case 1: /* RX_NJ_COUNTER_VALUE */
                trigMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_RX_NJ_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OPU_NJ_CNT;
                cntMsk = 
         HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OPU_NJ_CNT_RX_NJ_COUNTER_VALUE_MSK;
                break;
            case 2: /* RX_PJ_COUNTER_VALUE */
                trigMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_RX_PJ_LATCH_AND_CLEAR_MSK;
                cntReg  = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OPU_PJ_CNT;
                cntMsk = 
         HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OPU_PJ_CNT_RX_PJ_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
        trigReg = 
                HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice-8);
        cntReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice-8);
        uprCntReg = 
              HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(uprCntReg, slice-8);
        lowCntReg = 
              HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(lowCntReg, slice-8);
    } else {
        /* OTU1 Framer case */
        switch(counter)
        {
            case 0: /* PRBS_ERROR_COUNTER_VALUE */
                trigMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_PRBS_ERROR_LATCH_AND_CLEAR_MSK;
                uprCntReg = 
                       HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OPU_PRBS_UPR_ERR_CNT;
                lowCntReg = 
                           HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OPU_PRBS_LOW_ERR;
                uprCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OPU_PRBS_UPR_ERR_CNT_PRBS_UPPER_ERROR_COUNTER_HIGH_VALUE_MSK;
                lowCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OPU_PRBS_LOW_ERR_PRBS_LOWER_ERROR_COUNTER_HIGH_VALUE_MSK;
                break;
            case 1: /* RX_NJ_COUNTER_VALUE */
                trigMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_RX_NJ_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OPU_NJ_CNT;
                cntMsk  = 
         HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OPU_NJ_CNT_RX_NJ_COUNTER_VALUE_MSK;
                break;
            case 2: /* RX_PJ_COUNTER_VALUE */
                trigMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_RX_PJ_LATCH_AND_CLEAR_MSK;
                cntReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OPU_PJ_CNT;
                cntMsk = 
         HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OPU_PJ_CNT_RX_PJ_COUNTER_VALUE_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        trigReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
        trigReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(trigReg, slice);
        cntReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(cntReg, slice);
        uprCntReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(uprCntReg, 
                                                                  slice);
        lowCntReg = 
                HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(lowCntReg, 
                                                              slice);
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

} /* hyPhy20gOdukppOpukRxGetCounts */


/*******************************************************************************
**
**  hyPhy20gOdukppOtukAccumCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function trigger OTU2/OTU1 frame counter accumulation
**                  at the ODUKPP subsystem level.  It may further accumulate
**                  OTU2/OTU1 framer counters when TX_ERR_SYNC_MODE and 
**                  RX_ERR_SYNC_MODE bits are set to 1.
**
**                  OTU2/OTU1 frame counter are accessed with:
**                  - hyPhy20gOdukppOtukFrmGetCounts
**
**                  OTU2/OTU1 framer counters are accessed with:
**                  - hyPhy20gOdukppOpukTxGetCounts
**                  - hyPhy20gOdukppOdukTcmiTxGetCounts
**                  - hyPhy20gOdukppFramerRxGetCounts
**                  - hyPhy20gOdukppOtukRxGetCounts
**                  - hyPhy20gOdukppOdukRxPmTcmiGetCounts
**                  - hyPhy20gOdukppOpukRxGetCounts
**                  
**
**                  OTU2/OTU1 TX_ERR_SYNC_MODE and RX_ERR_SYNC_MODE are
**                  controlled with:
**                  - hyPhy20gOdukppCountTrigCfg
**
**                  Alternatively the combined counters selected may be
**                  accumulated at the device level trigger.
**
**                  The subsystem level counter accumulator accumulates counts 
**                  for all OTU2/OTU1 framers.  
**
**
**  INPUTS:       *fileHndl     - base address
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukAccumCounts(struct file *fileHndl)
{
    UINT4 addr, msk;
    INT4 result;

    addr = HYPHY20G_ODUKPP_TOP_REG_CFG;
    msk = HYPHY20G_ODUKPP_TOP_BIT_CFG_TIP_MSK;
    /* write TIP bit to 1 */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if (result) return result;

    /* poll TIP bit to go to 0, if not return an error */
    result = sysHyPhy20gPollBit(fileHndl, addr, msk, 0, 10000);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukAccumCounts */


/*******************************************************************************
**
**  hyPhy20gOdukppCountTrigCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the OTU2/OTU1 framer counter
**                  accumulation control signals in ODUKPP subsystem.  It allows 
**                  counters from OTU2/OTU1 framers in ODUKPP subsystem to be  
**                  controlled at the device or subsystem level.  Alternatively, 
**                  it disables accumulation at this level at a per slice 
**                  granularity.
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
**                                10 - All
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
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppCountTrigCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                                UINT4 enbl)
{
    UINT4 addr, msk;
    UINT4 i;
    INT4 result;

    /* argument checking */
    if((slice > 10)||(dir > 2)||(enbl > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if((slice == 10)&&((dir == 0)||(dir == 2))){
        /* Configure All OTU1 Tx Framer */
        for(i=0; i < 8; i++){
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_CNT_CTL;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, i);
            msk = 
               HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_CNT_CTL_TX_ERR_SYNC_MODE_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
            if (result) return result;
        }
        /* Configure All OTU2 Tx Framer */
        for(i=0; i < 2; i++){
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_CNT_CTL;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, i);
            msk = 
               HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_CNT_CTL_TX_ERR_SYNC_MODE_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
            if (result) return result;
        }
    }

    if((slice == 10)&&((dir == 1)||(dir == 2))){
        /* Configure All OTU1 Rx Framer */
        for(i=0; i < 8; i++){
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_GLBL_CTL;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, i);
            msk = 
              HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
            if (result) return result;
        }
        /* Configure All OTU2 Rx Framer */
        for(i=0; i < 2; i++){
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, i);
            msk = 
              HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
            if (result) return result;
        }
    }

    if((slice < 8)&&((dir == 0)||(dir == 2))){
        /* Configure Single OTU1 Tx Framer */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_CNT_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_CNT_CTL_TX_ERR_SYNC_MODE_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
        if (result) return result;
    }

    if((slice < 8)&&((dir == 1)||(dir == 2))){
        /* Configure Single OTU1 Rx Framer */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_GLBL_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
        if (result) return result;
    }

    if(((slice > 7)&&(slice < 10))&&((dir == 0)||(dir == 2))){
        /* Configure Single OTU2 Tx Framer */
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_CNT_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice - 8);
        msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_CNT_CTL_TX_ERR_SYNC_MODE_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
        if (result) return result;
    }

    if(((slice > 7)&&(slice < 10))&&((dir == 1)||(dir == 2))){
        /* Configure Single OTU2 Rx Framer */
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice - 8);
        msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppCountTrigCfg */



/*******************************************************************************
**
**  hyPhy20gOdukppOtukFrmGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts OTUk Frame Counts for an OTU2/OTU1
**                  Slice in ODUKPP subsystem.
**
**                  Prior to accessing counters, the counter registers must be
**                  updated by triggering counter accumulation.  Counters
**                  may be accumulated at the subsystem or device level.
**                  hyPhy20gOdukppOtukAccumCounts triggers counter accumulation 
**                  at the subsystem level.
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
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukFrmGetCounts(struct file *fileHndl, UINT4 slice, 
                                    UINT4 *pCount)
{
    UINT4 addr, val;
    INT4 result;

    /* argument checking */
    if(slice > 9){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (slice < 8) {
        addr = HYPHY20G_ODUKPP_TOP_REG_OTU1_RX_FRM_CNT_OFFSET(slice);
    } else {
        addr = HYPHY20G_ODUKPP_TOP_REG_OTU2_RX_FRM_CNT_OFFSET(slice-8);
    }

    /* Retrieve Counter Value */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *pCount = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukFrmGetCounts */



/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
