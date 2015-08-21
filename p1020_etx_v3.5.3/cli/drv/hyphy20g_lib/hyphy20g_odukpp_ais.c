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
**  FILE        : hyphy20g_odukpp_ais.c
**
**  $Date: 2010-03-29 10:56:27 -0700 (Mon, 29 Mar 2010) $
**
**  $Revision: 7223 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_odukpp_ais.h"

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
**  hyPhy20gOdukppOtukTxInsAisCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables automatic ODUk-AIS
**                  insertion into the transmit data stream based on system
**                  side conditions. 
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - OTUk Framer instance:
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
**                  iaeEnbl     - 0 - incoming IAE condition on the system side
**                                    will not cause ODUk-AIS to overwrite data
**                                    stream
**                                1 - incoming IAE condition on the system side
**                                    will cause ODUk-AIS to overwrite data
**                                    stream
**                  fifoErrEnbl - 0 - TX FIFO overflow or underflow will not
**                                    cause ODUk-AIS to overwrite data
**                                    stream
**                                1 - TX FIFO overflow or underflow will
**                                    cause ODUk-AIS to overwrite data
**                                    stream
**                  losEnbl     - 0 - TX LOS will not cause ODUk-AIS to
**                                    overwrite data stream
**                                1 - TX LOS will cause ODUk-AIS to overwrite
**                                    data stream
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxInsAisCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 iaeEnbl, UINT4 fifoErrEnbl, 
                                   UINT4 losEnbl)
{
    UINT4 addr;
    UINT4 val;
    UINT4 iaeMsk;
    UINT4 losMsk;
    UINT4 fifoErrMsk;
    INT4 result;

    /* argument checking */
    if (slice > 9 || iaeEnbl > 1 || fifoErrEnbl > 1 || losEnbl > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr        = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_AUTO_AIS_CTL;
        addr        = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
        iaeMsk      = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_OTUK_AIS_IAE_MSK;
        fifoErrMsk  = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_OTUK_AIS_FIFO_ERROR_MSK;
        losMsk      = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_OTUK_AIS_LOS_MSK;
    } else {
        /* OTU1 Framer case */
        addr        = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_AUTO_AIS_CTL;
        addr        = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        iaeMsk      = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_OTUK_AIS_IAE_MSK;
        fifoErrMsk  = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_OTUK_AIS_FIFO_ERROR_MSK;
        losMsk      = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_OTUK_AIS_LOS_MSK;
    }

    /* configure TX OTN AUTO AIS CONTROL register */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, iaeMsk, iaeEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, fifoErrMsk, fifoErrEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, losMsk, losEnbl);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukTxInsAisCfg */



/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxInsAisCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables automatic ODUk-AIS
**                  insertion into the transmit data stream based on system
**                  side conditions. 
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - OTUk Framer instance:
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
**                  iaeEnbl     - Reserved
**                                Please set it to 0.    
**                                    
**                                 
**                                    
**                                    
**                  fifoErrEnbl - 0 - TX FIFO overflow or underflow will not
**                                    cause ODUk-AIS to overwrite data
**                                    stream
**                                1 - TX FIFO overflow or underflow will
**                                    cause ODUk-AIS to overwrite data
**                                    stream
**                  losEnbl     - 0 - TX LOS will not cause ODUk-AIS to
**                                    overwrite data stream
**                                1 - TX LOS will cause ODUk-AIS to overwrite
**                                    data stream
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxInsAisCfg(struct file *fileHndl, UINT4 slice, 
                                   UINT4 iaeEnbl, UINT4 fifoErrEnbl, 
                                   UINT4 losEnbl)
{
    UINT4 addr;
    UINT4 val;
    UINT4 iaeMsk;
    UINT4 losMsk;
    UINT4 fifoErrMsk;
    INT4 result;

    /* argument checking */
    if (slice > 9 || iaeEnbl > 1 || fifoErrEnbl > 1 || losEnbl > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr        = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_AUTO_AIS_CTL;
        addr        = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
        iaeMsk      = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_ODUK_AIS_IAE_MSK;
        fifoErrMsk  = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_ODUK_AIS_FIFO_ERROR_MSK;
        losMsk      = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_ODUK_AIS_LOS_MSK;
    } else {
        /* OTU1 Framer case */
        addr        = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_AUTO_AIS_CTL;
        addr        = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        iaeMsk      = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_ODUK_AIS_IAE_MSK;
        fifoErrMsk  = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_ODUK_AIS_FIFO_ERROR_MSK;
        losMsk      = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_ODUK_AIS_LOS_MSK;
    }

    /* configure TX OTN AUTO AIS CONTROL register */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    // result = hyPhy20gVarFieldWrite (&val, iaeMsk, iaeEnbl);
    // if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, fifoErrMsk, fifoErrEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, losMsk, losEnbl);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxInsAisCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxTcmiInsAisCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables automatic ODUk-AIS 
**                  insertion into the transmit data stream when a TCMi TIM,
**                  OCI, or LCK condition is detected.
**
**                  This function is only valid for OTN framers that are
**                  receiving ODUk data directly from the CBRB with valid TCMi
**                  data.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - OTUk Framer instance:
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
**                  tcm1Enbl    - 0 - TCM1 TIM, OCI, or LCK condition will not
**                                    contribute to ODUk-AIS
**                              - 1 - TCM1 TIM, OCI, or LCK condition
**                                    contributes to ODUk-AIS
**                  tcm2Enbl    - 0 - TCM2 TIM, OCI, or LCK condition will not
**                                    contribute to ODUk-AIS
**                              - 1 - TCM2 TIM, OCI, or LCK condition
**                                    contributes to ODUk-AIS
**                  tcm3Enbl    - 0 - TCM3 TIM, OCI, or LCK condition will not
**                                    contribute to ODUk-AIS
**                              - 1 - TCM3 TIM, OCI, or LCK condition
**                                    contributes to ODUk-AIS
**                  tcm4Enbl    - 0 - TCM4 TIM, OCI, or LCK condition will not
**                                    contribute to ODUk-AIS
**                              - 1 - TCM4 TIM, OCI, or LCK condition
**                                    contributes to ODUk-AIS
**                  tcm5Enbl    - 0 - TCM5 TIM, OCI, or LCK condition will not
**                                    contribute to ODUk-AIS
**                              - 1 - TCM5 TIM, OCI, or LCK condition
**                                    contributes to ODUk-AIS
**                  tcm6Enbl    - 0 - TCM6 TIM, OCI, or LCK condition will not
**                                    contribute to ODUk-AIS
**                              - 1 - TCM6 TIM, OCI, or LCK condition
**                                    contributes to ODUk-AIS
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxTcmiInsAisCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 tcm1Enbl, UINT4 tcm2Enbl,
                                       UINT4 tcm3Enbl, UINT4 tcm4Enbl,
                                       UINT4 tcm5Enbl, UINT4 tcm6Enbl)
{
    UINT4 addr;
    UINT4 val;
    UINT4 aisMsk;
    UINT4 tcmiEnlbMsk[6];
    INT4 result;

    /* argument checking */
    if (slice > 9 || tcm1Enbl > 1 || tcm2Enbl > 1 || tcm3Enbl > 1 ||
        tcm4Enbl > 1 || tcm5Enbl > 1 || tcm6Enbl > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr           = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_TCMI_ODU_AIS_CTL;
        addr           = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
        aisMsk         = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_AUTO_TCMI_OTU_ODU_AIS_MSK;
        tcmiEnlbMsk[0] = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_OTU_ODU_AIS_TCM1_TIM_OCI_LCK_MSK;
        tcmiEnlbMsk[1] = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_OTU_ODU_AIS_TCM2_TIM_OCI_LCK_MSK;
        tcmiEnlbMsk[2] = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_OTU_ODU_AIS_TCM3_TIM_OCI_LCK_MSK;
        tcmiEnlbMsk[3] = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_OTU_ODU_AIS_TCM4_TIM_OCI_LCK_MSK;
        tcmiEnlbMsk[4] = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_OTU_ODU_AIS_TCM5_TIM_OCI_LCK_MSK;
        tcmiEnlbMsk[5] = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_OTU_ODU_AIS_TCM6_TIM_OCI_LCK_MSK;
    } else {
        /* OTU1 Framer case */
        addr           = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_TCMI_ODU_AIS_CTL;
        addr           = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        aisMsk         = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_AUTO_TCMI_OTU_ODU_AIS_MSK;
        tcmiEnlbMsk[0] = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_OTU_ODU_AIS_TCM1_TIM_OCI_LCK_MSK;
        tcmiEnlbMsk[1] = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_OTU_ODU_AIS_TCM2_TIM_OCI_LCK_MSK;
        tcmiEnlbMsk[2] = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_OTU_ODU_AIS_TCM3_TIM_OCI_LCK_MSK;
        tcmiEnlbMsk[3] = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_OTU_ODU_AIS_TCM4_TIM_OCI_LCK_MSK;
        tcmiEnlbMsk[4] = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_OTU_ODU_AIS_TCM5_TIM_OCI_LCK_MSK;
        tcmiEnlbMsk[5] = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_TCMI_ODU_AIS_CTL_OTU_ODU_AIS_TCM6_TIM_OCI_LCK_MSK;
    }

    /* configure TX_OTN_TCMI_ODU_AIS_CONTROL register */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* always set AUTO_TCMI_OTU_ODU_AIS bit to 1 */
    result = hyPhy20gVarFieldWrite (&val, aisMsk, 1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiEnlbMsk[0], tcm1Enbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiEnlbMsk[1], tcm2Enbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiEnlbMsk[2], tcm3Enbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiEnlbMsk[3], tcm4Enbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiEnlbMsk[4], tcm5Enbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiEnlbMsk[5], tcm6Enbl);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxTcmiInsAisCfg */


/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxInsAisCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables automatic ODUk-AIS 
**                  insertion into the ingress data stream based on
**                  RX conditions. ODUk-AIS or generic AIS pattern is
**                  dependent on streaming mode.
**
**                  For Rev B HW:
**                  When hyPhy20gOdukppOtukRxSmTtiProcCfg is called with either
**                  cmpSapi or cmpDapi or cmpOs enabled, the device is 
**                  configured for SM TIM events to contribute to AIS 
**                  insertion.  The smTimEnbl argument is not processed in
**                  this function.
**
**                  For Rev C and Rev D HW:
**                  smTimEnbl behaves as described in argument description
**
**  INPUTS:         *fileHndl       - base address
**                  slice           - OTUk Framer instance:
**                                    0 - OTU1 Framer 0
**                                    1 - OTU1 Framer 1
**                                    2 - OTU1 Framer 2
**                                    3 - OTU1 Framer 3
**                                    4 - OTU1 Framer 4
**                                    5 - OTU1 Framer 5
**                                    6 - OTU1 Framer 6
**                                    7 - OTU1 Framer 7
**                                    8 - OTU2 Framer 0
**                                    9 - OTU2 Framer 1
**                  lomEnbl         - 0 - RX LOM does not contribute to AIS
**                                        insertion
**                                    1 - RX LOM contributes to AIS insertion
**                  ptMmEnbl        - 0 - RX PLM does not contribute to AIS
**                                        insertion
**                                    1 - RX PLM contributes to AIS insertion
**                  smTimEnbl       - Rev B HW:
**                                    Reserved.  ODU_AIS_SM_TIM is configured
**                                    as a function of cmpSapi, cmpDapi and 
**                                    cmpOs settings in 
**                                    hyPhy20gOdukppOtukRxSmTtiProcCfg.
**                                    Value not processed by this API. Must be 
**                                    set to 0 or 1.
**                                    Rev C and Rev D HW:
**                                    0 - RX SM TIM does not contribute to AIS
**                                        insertion
**                                    1 - RX SM TIM contributes to AIS insertion
**                  pmTimOciLckEnbl - 0 - RX PM TIM, OCI, or LCK condition does
**                                        not contribute to AIS insertion
**                                    1 - RX PM TIM, OCI, or LCK condition
**                                        contributes to AIS insertion
**                  tcm1TimOciLckEnbl - 0 - RX TCM1 TIM, OCI, or LCK condition
**                                          does not contribute to AIS insertion
**                                      1 - RX TCM1 TIM, OCI, or LCK condition
**                                           contributes to AIS insertion
**                  tcm2TimOciLckEnbl - 0 - RX TCM2 TIM, OCI, or LCK condition
**                                          does not contribute to AIS insertion
**                                      1 - RX TCM2 TIM, OCI, or LCK condition
**                                           contributes to AIS insertion
**                  tcm3TimOciLckEnbl - 0 - RX TCM3 TIM, OCI, or LCK condition
**                                          does not contribute to AIS insertion
**                                      1 - RX TCM3 TIM, OCI, or LCK condition
**                                           contributes to AIS insertion
**                  tcm4TimOciLckEnbl - 0 - RX TCM4 TIM, OCI, or LCK condition
**                                          does not contribute to AIS insertion
**                                      1 - RX TCM4 TIM, OCI, or LCK condition
**                                           contributes to AIS insertion
**                  tcm5TimOciLckEnbl - 0 - RX TCM5 TIM, OCI, or LCK condition
**                                          does not contribute to AIS insertion
**                                      1 - RX TCM5 TIM, OCI, or LCK condition
**                                           contributes to AIS insertion
**                  tcm6TimOciLckEnbl - 0 - RX TCM6 TIM, OCI, or LCK condition
**                                          does not contribute to AIS insertion
**                                      1 - RX TCM6 TIM, OCI, or LCK condition
**                                           contributes to AIS insertion
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxInsAisCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 lomEnbl, UINT4 ptMmEnbl, 
                                   UINT4 smTimEnbl, UINT4 pmTimOciLckEnbl, 
                                   UINT4 tcm1TimOciLckEnbl, 
                                   UINT4 tcm2TimOciLckEnbl, 
                                   UINT4 tcm3TimOciLckEnbl, 
                                   UINT4 tcm4TimOciLckEnbl, 
                                   UINT4 tcm5TimOciLckEnbl,
                                   UINT4 tcm6TimOciLckEnbl)
{
    UINT4 devId, revId, status;
    UINT4 smTimEnblMsk;
    UINT4 addr;
    UINT4 val;
    UINT4 aisMsk;
    UINT4 lomMsk;
    UINT4 ptMmEnblMsk;
    UINT4 ociEnblMsk;
    UINT4 tcmiMsk[6];
    INT4 result;

    /* argument checking */
    if (slice > 9 || lomEnbl > 1 || ptMmEnbl > 1 ||
        smTimEnbl > 1 || pmTimOciLckEnbl > 1 || tcm1TimOciLckEnbl > 1 ||
        tcm2TimOciLckEnbl > 1 || tcm3TimOciLckEnbl > 1 ||
        tcm4TimOciLckEnbl > 1 || tcm5TimOciLckEnbl > 1 ||
        tcm6TimOciLckEnbl > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* evaluation of compatibility between SW driver and device */
    result = hyPhy20gCheckRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr         = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_MAINT_CTL;
        addr         = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
        aisMsk       = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_AUTO_ODU_AIS_MSK;
        lomMsk       = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_LOM_MSK;
        ptMmEnblMsk  = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_PT_MM_MSK;
        smTimEnblMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_SM_TIM_MSK;
        ociEnblMsk   = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_PM_TIM_OCI_LCK_MSK;
        tcmiMsk[0]   = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_TCM1_TIM_OCI_LCK_MSK;
        tcmiMsk[1]   = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_TCM2_TIM_OCI_LCK_MSK;
        tcmiMsk[2]   = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_TCM3_TIM_OCI_LCK_MSK;
        tcmiMsk[3]   = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_TCM4_TIM_OCI_LCK_MSK;
        tcmiMsk[4]   = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_TCM5_TIM_OCI_LCK_MSK;
        tcmiMsk[5]   = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_TCM6_TIM_OCI_LCK_MSK;
    } else {
        /* OTU1 Framer case */
        addr         = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_MAINT_CTL;
        addr         = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        aisMsk       = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_AUTO_ODU_AIS_MSK;
        lomMsk       = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_LOM_MSK;
        smTimEnblMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_SM_TIM_MSK;
        ptMmEnblMsk  = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_PT_MM_MSK;
        ociEnblMsk   = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_PM_TIM_OCI_LCK_MSK;
        tcmiMsk[0]   = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_TCM1_TIM_OCI_LCK_MSK;
        tcmiMsk[1]   = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_TCM2_TIM_OCI_LCK_MSK;
        tcmiMsk[2]   = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_TCM3_TIM_OCI_LCK_MSK;
        tcmiMsk[3]   = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_TCM4_TIM_OCI_LCK_MSK;
        tcmiMsk[4]   = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_TCM5_TIM_OCI_LCK_MSK;
        tcmiMsk[5]   = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_TCM6_TIM_OCI_LCK_MSK;
    }

    /* configure RX OTN ODU Maintenance Control register */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;
        
    /* unconditionally enable AIS generation for user specified conditions */
    result = hyPhy20gVarFieldWrite (&val, aisMsk, 1);
    if(result) return result;

    /* Rev C and Rev D allow input argument to allow SM TIM to contribute to OTUk-AIS */
    if ( revId == HYPHY20G_REV_ID_C || revId == HYPHY20G_REV_ID_D ) {
        result = hyPhy20gVarFieldWrite (&val, smTimEnblMsk, smTimEnbl);
        if(result) return result;
    }
    
    result = hyPhy20gVarFieldWrite (&val, lomMsk, lomEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, ptMmEnblMsk, ptMmEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, ociEnblMsk, pmTimOciLckEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiMsk[0], tcm1TimOciLckEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiMsk[1], tcm2TimOciLckEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiMsk[2], tcm3TimOciLckEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiMsk[3], tcm4TimOciLckEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiMsk[4], tcm5TimOciLckEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiMsk[5], tcm6TimOciLckEnbl);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxInsAisCfg */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/* end of file */

