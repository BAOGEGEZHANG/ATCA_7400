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
**  FILE        : hyphy20g_odukpp_overhead.c
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
#include "hyphy20g_odukpp_overhead.h"

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
**  hyPhy20gOdukppOtukRxSmTtiExpCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the expected receive SM TTI
**                  message in OTUk framers in ODUKPP subsystem.
**
**                  The function takes in the 16 byte Source Access Point
**                  Identifier (SAPI), the 16 byte Destination Access Point
**                  Identifier (DAPI), and the 32 byte Operator Specific fields
**                  as arguments to fill the 64 byte TTI.
**
**                  The range of a SAPI or DAPI characters 15 to 1 are 0 to 127.
**                  SAPI and DAPI character 0 must be set to 0x00.  Function
**                  will return HYPHY20G_ERR_INVALID_ARG if these rules are not
**                  adhered.
**
**                  There are no restrictions on the values placed in Operator
**                  Specific bytes.
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
**                  sapi[16]    - SAPI characters (16 bytes)
**                  dapi[16]    - DAPI characters (16 bytes)
**                  opSpec[32]  - Operator Specific (32 bytes)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxSmTtiExpCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 sapi[16], UINT4 dapi[16],
                                     UINT4 opSpec[32])
{
    UINT4 subSlcOff;
    UINT4 addr;
    UINT4 ttiMsk;
    UINT4 regData;
    UINT4 val;
    UINT4 i, j;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (sapi[0] > 0) || (dapi[0] > 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (i = 0; i < 32; i++) {
        if ((i < 16) && (sapi[i] > 127 || dapi[i] > 127)) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        if (opSpec[i] > 255) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }
    
    /* Configure expected receive SM TTI message */
    j = 0;
    for (i = 0; i < 32; i++) {
        /* Determine register address */
        if (slice > 7) {
            /* OTU2 Framers */
            subSlcOff = slice - 8;

            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_SM_TTI_EXP_MEM_OFFSET(i);
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
            ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_TTI_EXP_MEM_SM_TTI_EXPECT_VALUE_MSK;
        } else {
            /* OTU1 Framers */
            subSlcOff = slice;

            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_SM_TTI_EXP_MEM_OFFSET(i);
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
            ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_TTI_EXP_MEM_SM_TTI_EXPECT_VALUE_MSK;
        }

        /* Read register */
        result = sysHyPhy20gRead(fileHndl, addr, &regData);
        if(result) return result;

        /* Modify read data */
        j = i*2;
        if (i < 8) {
            /* Configure SAPI (2 bytes per register) */
            val = (sapi[j+1] << 8) | sapi[j];
        } else if (i < 16) {
            /* Configure DAPI (2 bytes per register) */
            j = j - 16;
            val = (dapi[j+1] << 8) | dapi[j];
        } else {
            /* Configure Operator Specific (2 bytes per register) */
            j = j - 32;
            val = (opSpec[j+1] << 8) | opSpec[j];
        }
        result = hyPhy20gVarFieldWrite(&regData, ttiMsk, val);
        if(result) return result;

        /* Write data to register */
        result = sysHyPhy20gWrite(fileHndl, addr, regData);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOtukRxSmTtiExpCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxSmTtiProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the SM TTI message of OTUk framers in ODUKPP subsystem.  
**                  It configures how the TTI message is monitored and which 
**                  fields of the message contribute to TIM defect detection.
**
**                  For Rev B HW:
**                  If cmpSapi, cmpDapi or cmpOs is enabled AIS generation in
**                  response to an SM TIM event will be unconditionally enabled.
**
**                  For Rev C and Rev D HW:
**                  This function does not control the contribution of SM TIM to
**                  AIS generation.
**
**                  hyPhy20gOdukppOtukRxSmTtiExpCfg must be called to configure
**                  the expected TTI message in order for the processing
**                  performed by these feature to be meaningful.
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
**                  acptCnt     - Number of identical SM TTI messages that that
**                                the RX framer needs to obtain before declaring
**                                the TTI as accepted.
**                              - Range 1 to 7
**                              - Default value 0x2
**                  inconCnt    - Number of non-identical SM TTI messages that
**                                that the RX framer needs to obtain before
**                                triggering a TTI inconsistent error interrupt.
**                              - Range 1 to 7
**                              - Default value 0x5
**                  cmpSapi     - Controls whether or not the RX framer uses the
**                                SAPI field in the accepted SM TTI to compare
**                                against the expected TTI to detect a TIM
**                                defect
**                              - 1: SM SAPI is used for comparison for TIM
**                                   detection.
**                              - 0: SM SAPI is not used for comparison for TIM
**                                   detection.
**                  cmpDapi     - Controls whether or not the RX framer uses the
**                                DAPI field in the accepted SM TTI to compare
**                                against the expected TTI to detect a TIM
**                                defect
**                              - 1: SM DAPI is used for comparison for TIM
**                                   detection.
**                              - 0: SM DAPI is not used for comparison for TIM
**                                   detection.
**                  cmpOs       - Controls whether or not the RX framer uses the
**                                Opearator Specific (OS) field in the accepted
**                                SM TTI to compare against the expected TTI
**                                to detect a TIM defect
**                              - 1: SM OS is used for comparison for TIM
**                                   detection.
**                              - 0: SM OS is not used for comparison for TIM
**                                   detection.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxSmTtiProcCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 acptCnt, UINT4 inconCnt, 
                                      UINT4 cmpSapi, UINT4 cmpDapi, UINT4 cmpOs)
{
    UINT4 devId, revId, status;
    UINT4 subSlcOff;
    UINT4 addr, smTimAddr;
    UINT4 regData;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;
    UINT4 cmpSapiMsk;
    UINT4 cmpDapiMsk;
    UINT4 cmpOsMsk;
    UINT4 smTimEnblOff, smTimEnblVal;
    INT4  result;

    /* Argument check */
    if ((slice > 9) || (acptCnt < 1) || (acptCnt > 7) ||
        (inconCnt < 1) || (inconCnt > 7) || (cmpSapi > 1) || (cmpDapi > 1) ||
        (cmpOs > 1))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* evaluation of compatibility between SW driver and device */
    result = hyPhy20gCheckRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_SM_TTI_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        smTimAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_MAINT_CTL;
        smTimAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(smTimAddr, slice-8);

        acptCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_TTI_CTL_SM_TTI_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_TTI_CTL_SM_TTI_INCONSISTENT_COUNT_MSK;
        cmpSapiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_TTI_CTL_SM_TTI_SAPI_TIM_MSK;
        cmpDapiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_TTI_CTL_SM_TTI_DAPI_TIM_MSK;
        cmpOsMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_TTI_CTL_SM_TTI_OS_TIM_MSK;
        smTimEnblOff = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_SM_TIM_OFF;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_SM_TTI_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        smTimAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_MAINT_CTL;
        smTimAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(smTimAddr, slice);

        acptCntMsk  = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_TTI_CTL_SM_TTI_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_TTI_CTL_SM_TTI_INCONSISTENT_COUNT_MSK;
        cmpSapiMsk  = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_TTI_CTL_SM_TTI_SAPI_TIM_MSK;
        cmpDapiMsk  = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_TTI_CTL_SM_TTI_DAPI_TIM_MSK;
        cmpOsMsk    = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_TTI_CTL_SM_TTI_OS_TIM_MSK;
        smTimEnblOff = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_ODU_AIS_SM_TIM_OFF;
    }

    /* Set the ODU_AIS_SM_TIM */  
    if ( revId == HYPHY20G_REV_ID_B ) {  
        smTimEnblVal = (cmpSapi == 1 || cmpDapi == 1 || cmpOs == 1)? 1:0;
        result = sysHyPhy20gBitWrite(fileHndl, smTimAddr, smTimEnblOff, smTimEnblVal);
        if (result) return result;        
    }
    
    /* Read register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Configure SM_TTI_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Configure SM_TTI_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* Configure SM_TTI_SAPI_TIM */
    result = hyPhy20gVarFieldWrite(&regData, cmpSapiMsk, cmpSapi);
    if(result) return result;

    /* Configure SM_TTI_DAPI_TIM */
    result = hyPhy20gVarFieldWrite(&regData, cmpDapiMsk, cmpDapi);
    if(result) return result;

    /* Configure SM_TTI_OS_TIM */
    result = hyPhy20gVarFieldWrite(&regData, cmpOsMsk, cmpOs);
    if(result) return result;

    /* Write data to register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOtukRxSmTtiProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxSmTtiAcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive SM TTI
**                  message in OTUk framers in ODUKPP subsystem.
**
**                  The function exptracts the 64 byte TTI relative to the 16
**                  byte Source Access Point Identifier (SAPI), the 16 byte
**                  Destination Access Point Identifier (DAPI), and the 32 byte
**                  Operator Specific fields.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - OTUk Framer instance:
**                                 0 - OTU1 Framer 0
**                                 1 - OTU1 Framer 1
**                                 2 - OTU1 Framer 2
**                                 3 - OTU1 Framer 3
**                                 4 - OTU1 Framer 4
**                                 5 - OTU1 Framer 5
**                                 6 - OTU1 Framer 6
**                                 7 - OTU1 Framer 7
**                                 8 - OTU2 Framer 0
**                                 9 - OTU2 Framer 1
**                  *pSapi[16]   - pointer to extracted SAPI characters
**                                (16 bytes)
**                  *pDapi[16]   - pointer to extracted DAPI characters
**                                (16 bytes)
**                  *pOpSpec[32] - pointer to extracted Operator Specific
**                                (32 bytes)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxSmTtiAcptExt(struct file *fileHndl, UINT4 slice,
                                      UINT4 *pSapi, UINT4 *pDapi, 
                                      UINT4 *pOpSpec)
{
    UINT4 subSlcOff;
    UINT4 addr;
    UINT4 regData;
    UINT4 i;
    INT4  result;

    /* Argument checking */
    if (slice > 9) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((pSapi == NULL) || (pDapi == NULL) || (pOpSpec == NULL)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Extract accepted receive SM TTI message */
    for (i = 0; i < 32; i++) {

        /* Determine register address */
        if (slice > 7) {
            /* OTU2 Framers */
            subSlcOff = slice - 8;

            addr = 
               HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_SM_TTI_EXT_MEM_OFFSET(i);
            addr = 
                 HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        } else {
            /* OTU1 Framers */
            subSlcOff = slice;

            addr = 
               HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_SM_TTI_EXT_MEM_OFFSET(i);
            addr = 
                 HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        }

        /* Read register */
        result = sysHyPhy20gRead(fileHndl, addr, &regData);
        if(result) return result;

        /* Modify read data */
        if (i < 8) {
            /* Store SAPI (2 bytes per register) */
            *pSapi = regData & 0xFF;
            pSapi++;
            *pSapi = (regData >> 8) & 0xFF;
            pSapi++;
        } else if (i < 16) {
            /* Store DAPI (2 bytes per register) */
            *pDapi = regData & 0xFF;
            pDapi++;
            *pDapi = (regData >> 8) & 0xFF;
            pDapi++;
        } else {
            /* Store Operator Specific (2 bytes per register) */
            *pOpSpec = regData & 0xFF;
            pOpSpec++;
            *pOpSpec = (regData >> 8) & 0xFF;
            pOpSpec++;
        }
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOtukRxSmTtiAcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxSmResProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the SM RES bits in OTUk framers in ODUKPP subsystem.  
**                  It configures how the SM RES bits are monitored for 
**                  operation of SM RES accepted and inconsistent interrupts.
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
**                  acptCnt     - Number of identical SM RES messages that the 
**                                RX framer needs to obtain before declaring
**                                the SM RES as accepted
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical  SM RES messages that
**                                the RX framer needs to obtain before
**                                triggering a SM RES inconsistent error
**                                interrupt
**                              - Range 1 to 7
**                              - Default value 0x5
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxSmResProcCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 acptCnt, UINT4 inconCnt)
{
    UINT4 subSlcOff;
    UINT4 addr;
    UINT4 regData;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;
    INT4  result;

    /* Argument check */
    if ((slice > 9) || (acptCnt < 1) || (acptCnt > 7) ||
        (inconCnt < 1) || (inconCnt > 7))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL_SM_RES_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL_SM_RES_INCONSISTENT_COUNT_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL_SM_RES_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL_SM_RES_INCONSISTENT_COUNT_MSK;
    }

    /* Read register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Configure SM_RES_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Configure SM_RES_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* Write data to register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOtukRxSmResProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxSmResAcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive SM RES bits in 
**                  OTUk framers in ODUKPP subsystem.
**
**                  The SM RES bits are reserved for future international
**                  standardization. G.709 specifies that they are set to "00".
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
**                  *pSmRes     - pointer to extracted SM RES bits
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxSmResAcptExt(struct file *fileHndl, UINT4 slice,
                                      UINT4 *pSmRes)
{
    UINT4 subSlcOff;
    UINT4 addr;
    UINT4 resMsk;
    UINT4 regData;
    INT4 result;

    /* Argument checking */
    if (slice > 9) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pSmRes == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        resMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL_SM_RES_ACCEPT_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        resMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL_SM_RES_ACCEPT_VALUE_MSK;
    }

    /* Read register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Return SM_RES_ACCEPT_VALUE field from read data */
    result = hyPhy20gVarFieldRead(regData, resMsk, pSmRes);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOtukRxSmResAcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukTxSmBeiBiaeSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the SM BEI/BIAE source for the
**                  transmit data stream in ODUKPP subsystem.
**
**                  When configured for 'Incoming SM BEI/BIAE value' the value
**                  transferred on SM BEI/BIAE bits will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
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
**                  source      - SM BEI/BIAE source
**                                0 - Incoming SM BEI/BIAE value
**                                1 - BEI/BIAE value generated from receive data
**                                    stream
**                                2 - Force BEI/BIAE value
**                  forceVal   - For source == 2 (otherwise set to 0)
**                               The value to be inserted into BEI/BIAE bits in
**                               outgoing data stream
**                             - Legal range 0x0 to 0xF
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxSmBeiBiaeSrcCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 source, UINT4 forceVal)
{
    UINT4 subSlcOff;
    UINT4 regData;
    UINT4 smCtlReg;
    UINT4 autoSmMsk, autoSmVal;
    UINT4 insSmMsk, insSmVal;
    UINT4 smValReg;
    UINT4 forceValMsk;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (source > 2) || (forceVal > 0xF) ||
        (source != 2 &&  forceVal != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        smCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_SM_CTL;
        smCtlReg = 
             HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        autoSmMsk = 
            HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_AUTO_SM_BEI_BIAE_MSK;
        insSmMsk  = 
          HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_INSERT_SM_BEI_BIAE_MSK;

        smValReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_SM_VAL;
        smValReg = 
             HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(smValReg, subSlcOff);
        forceValMsk = 
           HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_VAL_SM_BEI_BIAE_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        smCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_SM_CTL;
        smCtlReg = 
            HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        autoSmMsk = 
            HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_AUTO_SM_BEI_BIAE_MSK;
        insSmMsk  = 
          HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_INSERT_SM_BEI_BIAE_MSK;

        smValReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_SM_VAL;
        smValReg = 
             HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(smValReg, subSlcOff);
        forceValMsk = 
           HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_VAL_SM_BEI_BIAE_VALUE_MSK;
    }

    switch (source) {
        case 0:
            /* Incoming SM BEI/BIAE */
            autoSmVal = 0;
            insSmVal = 0;
            break;
        case 1:
            /* BEI/BIAE value generated from receive data stream */
            autoSmVal = 1;
            insSmVal = 0;
            break;
        case 2:
            /* Force BEI/BIAE */
            autoSmVal = 0;
            insSmVal = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if (source == 2) {
        /* Force insertion of BEI/BIAE
           Configure SM_BEI_BIAE_VALUE */
        result = hyPhy20gRegFieldWrite(fileHndl, smValReg, forceValMsk,
                                       forceVal);
        if(result) return result;
    }

    /* Read SM control register */
    result = sysHyPhy20gRead(fileHndl, smCtlReg, &regData);
    if(result) return result;

    /* Configure AUTO_SM_BEI_BIAE */
    result = hyPhy20gVarFieldWrite(&regData, autoSmMsk, autoSmVal);
    if(result) return result;

    /* Configure INSERT_SM_BEI_BIAE */
    result = hyPhy20gVarFieldWrite(&regData, insSmMsk, insSmVal);
    if(result) return result;

    /* Write SM control register */
    result = sysHyPhy20gWrite(fileHndl, smCtlReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOtukTxSmBeiBiaeSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukTxSmBdiAutoProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the SM BDI automatic generation
**                  criteria for the transmit data stream of an OTU2 or OTU1
**                  framer slice in ODUKPP subsystem.
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
**                  losEnbl     - Controls whether SM BDI is generated as a
**                                result of incoming LOS
**                              - 1: SM BDI generated as a result of LOS
**                              - 0: SM BDI not generated as a result of LOS
**                  lofEnbl     - Controls whether SM BDI is generated as a
**                                result of incoming LOF
**                              - 1: SM BDI generated as a result of LOF
**                              - 0: SM BDI not generated as a result of LOF
**                  lomEnbl     - Controls whether SM BDI is generated as a
**                                result of incoming LOM
**                              - 1: SM BDI generated as a result of LOM
**                              - 0: SM BDI not generated as a result of LOM
**                  otukAisEnbl - Controls whether SM BDI is generated as a
**                                result of incoming LOM
**                              - 1: SM BDI generated as a result of ODUk-AIS
**                              - 0: SM BDI not generated as a result of
**                                   ODUk-AIS
**                  timEnbl     - Controls whether SM BDI is generated as a
**                                result of incoming TIM
**                              - 1: SM BDI generated as a result of TIM
**                              - 0: SM BDI not generated as a result of TIM
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxSmBdiAutoProcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 losEnbl, UINT4 lofEnbl, 
                                          UINT4 lomEnbl, UINT4 otukAisEnbl, 
                                          UINT4 timEnbl)
{
    UINT4 subSlcOff;
    UINT4 regData;
    UINT4 smCtlReg;
    UINT4 losMsk, lofMsk, lomMsk, otukAisMsk, timMsk;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (losEnbl > 1) || (lofEnbl > 1) || (lomEnbl > 1) ||
        (otukAisEnbl > 1) || (timEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        smCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_SM_CTL;
        smCtlReg = 
             HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        losMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_SM_BDI_LOS_MSK;
        lofMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_SM_BDI_LOF_MSK;
        lomMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_SM_BDI_LOM_MSK;
        otukAisMsk = 
             HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_SM_BDI_OTUK_AIS_MSK;
        timMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_SM_BDI_TIM_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        smCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_SM_CTL;
        smCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        losMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_SM_BDI_LOS_MSK;
        lofMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_SM_BDI_LOF_MSK;
        lomMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_SM_BDI_LOM_MSK;
        otukAisMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_SM_BDI_OTUK_AIS_MSK;
        timMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_SM_BDI_TIM_MSK;
    }

    /* Read SM control register */
    result = sysHyPhy20gRead(fileHndl, smCtlReg, &regData);
    if(result) return result;
    
    /* Configure SM_BDI_LOS */
    result = hyPhy20gVarFieldWrite(&regData, losMsk, losEnbl);
    if(result) return result;

    /* Configure SM_BDI_LOF */
    result = hyPhy20gVarFieldWrite(&regData, lofMsk, lofEnbl);
    if(result) return result;

    /* Configure SM_BDI_LOM */
    result = hyPhy20gVarFieldWrite(&regData, lomMsk, lomEnbl);
    if(result) return result;

    /* Configure SM_BDI_OTUK_AIS */
    result = hyPhy20gVarFieldWrite(&regData, otukAisMsk, otukAisEnbl);
    if(result) return result;

    /* Configure SM_BDI_TIM */
    result = hyPhy20gVarFieldWrite(&regData, timMsk, timEnbl);
    if(result) return result;

    /* Write SM control register */
    result = sysHyPhy20gWrite(fileHndl, smCtlReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOtukTxSmBdiAutoProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukTxSmBdiSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the SM BDI source for the
**                  transmit data stream of OTUk framers in ODUKPP subsystem.
**
**                  When configured for 'Incoming SM BDI value' the value
**                  transferred on SM BDI bit will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
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
**                  source      - SM BDI source
**                                0 - Incoming SM BDI value
**                                1 - BDI value generated from receive data
**                                    stream
**                                2 - Force BDI value
**                  forceVal   - For source == 2 (otherwise set to 0)
**                               The value to be inserted into SM BDI bit in
**                               outgoing data stream
**                             - Legal range 0x0 to 0x1
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxSmBdiSrcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 source, UINT4 forceVal)
{
    UINT4 subSlcOff;
    UINT4 regData;
    UINT4 smCtlReg;
    UINT4 autoSmMsk, autoSmVal;
    UINT4 insSmMsk, insSmVal;
    UINT4 smValReg;
    UINT4 forceValMsk;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (source > 2) || (forceVal > 1) ||
        (source != 2 && forceVal != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        smCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_SM_CTL;
        smCtlReg = 
             HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        autoSmMsk = 
                 HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_AUTO_SM_BDI_MSK;
        insSmMsk = 
               HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_INSERT_SM_BDI_MSK;
        
        smValReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_SM_VAL;
        smValReg = 
             HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(smValReg, subSlcOff);
        forceValMsk = 
                HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_VAL_SM_BDI_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        smCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_SM_CTL;
        smCtlReg = 
             HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        autoSmMsk = 
                 HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_AUTO_SM_BDI_MSK;
        insSmMsk = 
               HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_INSERT_SM_BDI_MSK;
        
        smValReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_SM_VAL;
        smValReg = 
             HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(smValReg, subSlcOff);
        forceValMsk = 
                HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_VAL_SM_BDI_VALUE_MSK;
    }

    switch (source) {
        case 0:
            /* Incoming SM BDI */
            autoSmVal = 0;
            insSmVal = 0;
            break;
        case 1:
            /* BDI value generated from receive data stream */
            autoSmVal = 1;
            insSmVal = 0;
            break;
        case 2:
            /* Force BDI */
            autoSmVal = 0;
            insSmVal = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if (source == 2) {
        /* Force insertion of BDI 
           Configure SM_BDI_VALUE */
        result = hyPhy20gRegFieldWrite(fileHndl, smValReg, forceValMsk,
                                       forceVal);
        if(result) return result;
    }

    /* Read SM control register */
    result = sysHyPhy20gRead(fileHndl, smCtlReg, &regData);
    if(result) return result;

    /* Configure AUTO_SM_BDI */
    result = hyPhy20gVarFieldWrite(&regData, autoSmMsk, autoSmVal);
    if(result) return result;

    /* Configure INSERT_SM_BDI */
    result = hyPhy20gVarFieldWrite(&regData, insSmMsk, insSmVal);
    if(result) return result;

    /* Write SM control register */
    result = sysHyPhy20gWrite(fileHndl, smCtlReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOtukTxSmBdiSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukTxSmIaeSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the SM IAE source for the
**                  transmit data stream in OTUk framers in ODUKPP subsystem.
**
**                  When configured for 'Incoming SM IAE value' the value
**                  transferred on SM IAE bit will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
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
**                  source      - SM IAE source
**                                0 - Incoming SM IAE value
**                                1 - SM IAE value generated from receive data
**                                    stream
**                                2 - Force IAE value
**                  forceVal   - For source == 2 (otherwise set to 0)
**                               The value to be inserted into IAE bit in
**                               outgoing data stream
**                             - Legal range 0 to 1
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxSmIaeSrcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 source, UINT4 forceVal)
{
    UINT4 subSlcOff;
    UINT4 regData;
    UINT4 smCtlReg;
    UINT4 autoSmMsk, autoSmVal;
    UINT4 insSmMsk, insSmVal;
    UINT4 smValReg;
    UINT4 forceValMsk;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (source > 2) || (forceVal > 1) ||
        (source != 2 && forceVal != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        smCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_SM_CTL;
        smCtlReg = 
             HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        autoSmMsk = 
                 HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_AUTO_SM_IAE_MSK;
        insSmMsk  = 
               HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_INSERT_SM_IAE_MSK;

        smValReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_SM_VAL;
        smValReg = 
             HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(smValReg, subSlcOff);
        forceValMsk = 
                HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_VAL_SM_IAE_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        smCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_SM_CTL;
        smCtlReg = 
             HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        autoSmMsk = 
                 HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_AUTO_SM_IAE_MSK;
        insSmMsk  = 
               HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_INSERT_SM_IAE_MSK;

        smValReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_SM_VAL;
        smValReg = 
             HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(smValReg, subSlcOff);
        forceValMsk = 
                HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_VAL_SM_IAE_VALUE_MSK;
    }

    switch (source) {
        case 0:
            /* Incoming SM BDI */
            autoSmVal = 0;
            insSmVal = 0;
            break;
        case 1:
            /* SM BDI value generated from receive data stream */
            autoSmVal = 1;
            insSmVal = 0;
            break;
        case 2:
            /* Force SM BDI */
            autoSmVal = 0;
            insSmVal = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if (source == 2) {
        /* Force insertion of SM_IAE 
           Configure SM_IAE_VALUE */
        result = hyPhy20gRegFieldWrite(fileHndl, smValReg, forceValMsk,
                                       forceVal);
        if(result) return result;
    }

    /* Read SM control register */
    result = sysHyPhy20gRead(fileHndl, smCtlReg, &regData);
    if(result) return result;

    /* Configure AUTO_SM_IAE */
    result = hyPhy20gVarFieldWrite(&regData, autoSmMsk, autoSmVal);
    if(result) return result;

    /* Configure INSERT_SM_IAE */
    result = hyPhy20gVarFieldWrite(&regData, insSmMsk, insSmVal);
    if(result) return result;

    /* Write SM control register */
    result = sysHyPhy20gWrite(fileHndl, smCtlReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOtukTxSmIaeSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukTxSmResSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the SM RES source of OTUk framers
**                  for the transmit data stream in ODUKPP subsystem.
**
**                  When configured for 'Incoming SM RES value' the value
**                  transferred on SM RES bits will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
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
**                  source      - SM RES source
**                                0 - Incoming SM RES value
**                                1 - Force SM RES value
**                  forceVal   - For source == 1 (otherwise set to 0)
**                               The value to be inserted into SM RES bits in
**                               outgoing data stream
**                             - Legal range 0 to 3
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxSmResSrcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 source, UINT4 forceVal)
{
    UINT4 subSlcOff;
    UINT4 regData;
    UINT4 smCtlReg;
    UINT4 insSmMsk, insSmVal;
    UINT4 smValReg;
    UINT4 forceValMsk;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (source > 1) || (forceVal > 3) ||
        (source != 1 && forceVal != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        smCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_SM_CTL;
        smCtlReg = 
             HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        insSmMsk = 
               HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_INSERT_SM_RES_MSK;

        smValReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_SM_VAL;
        smValReg = 
             HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(smValReg, subSlcOff);
        forceValMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_VAL_SM_RES_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        smCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_SM_CTL;
        smCtlReg = 
             HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        insSmMsk = 
               HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_INSERT_SM_RES_MSK;

        smValReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_SM_VAL;
        smValReg = 
             HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(smValReg, subSlcOff);
        forceValMsk = 
                HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_VAL_SM_RES_VALUE_MSK;
    }

    switch (source) {
        case 0:
            /* Incoming SM RES */
            insSmVal = 0;
            break;
        case 1:
            /* Force SM RES */
            insSmVal = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if (source == 1) {
        /* Force insertion of SM_RES
           Configure SM_RES_VALUE */
        result = hyPhy20gRegFieldWrite(fileHndl, smValReg, forceValMsk,
                                       forceVal);
        if(result) return result;
    }

    /* Read SM control register */
    result = sysHyPhy20gRead(fileHndl, smCtlReg, &regData);
    if(result) return result;

    /* Configure INSERT_SM_RES */
    result = hyPhy20gVarFieldWrite(&regData, insSmMsk, insSmVal);
    if(result) return result;

    /* Write SM control register */
    result = sysHyPhy20gWrite(fileHndl, smCtlReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOtukTxSmResSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxSmBiaeBdiIaeProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the SM BIAE, BDI and IAE message of OTUk framers in 
**                  ODUKPP subsystem.  It configures whether the SM BIAE, BDI 
**                  and IAE acceptance processes accept a value after 3 
**                  consecutive frames or 5 consecutive frames.
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
**                  biaeFilter5 - Number of consecutive frames before accepting
**                                SM BIAE value
**                                0 - 3 frames (G.798)
**                                1 - 5 frames
**                  bdiFilter5  - Number of consecutive frames before accepting
**                                SM BDI value
**                                0 - 3 frames
**                                1 - 5 frames (G.798)
**                  iaeFilter5  - Number of consecutive frames before accepting
**                                SM IAE value
**                                0 - 3 frames
**                                1 - 5 frames (G.798)
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxSmBiaeBdiIaeProcCfg(struct file *fileHndl, UINT4 slice,
                                             UINT4 biaeFilter5, 
                                             UINT4 bdiFilter5, UINT4 iaeFilter5)
{
    UINT4 subSlcOff;
    UINT4 regData;
    UINT4 smCtlReg;
    UINT4 biaeMsk, bdiMsk, iaeMsk;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (biaeFilter5 > 1) || (bdiFilter5 > 1) ||
        (iaeFilter5 > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        smCtlReg = 
                HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL;
        smCtlReg = 
             HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        biaeMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL_SM_BIAE_FILTER_5_MSK;
        bdiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL_SM_BDI_FILTER_5_MSK;
        iaeMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL_SM_IAE_FILTER_5_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        smCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL;
        smCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        biaeMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL_SM_BIAE_FILTER_5_MSK;
        bdiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL_SM_BDI_FILTER_5_MSK;
        iaeMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_SM_BIAE_BDI_IAE_RES_CTL_SM_IAE_FILTER_5_MSK;
    }

    /* Read SM control register */
    result = sysHyPhy20gRead(fileHndl, smCtlReg, &regData);
    if(result) return result;

    /* Configure SM_BIAE_FILTER_5 */
    result = hyPhy20gVarFieldWrite(&regData, biaeMsk, biaeFilter5);
    if(result) return result;

    /* Configure SM_BDI_FILTER_5 */
    result = hyPhy20gVarFieldWrite(&regData, bdiMsk, bdiFilter5);
    if(result) return result;

    /* Configure SM_IAE_FILTER_5 */
    result = hyPhy20gVarFieldWrite(&regData, iaeMsk, iaeFilter5);
    if(result) return result;

    /* Write SM control register */
    result = sysHyPhy20gWrite(fileHndl, smCtlReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOtukRxSmBiaeBdiIaeProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukTxSmTtiMsgCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the forced transmit SM TTI
**                  message of OTUk framers in ODUKPP subsystem.
**
**                  The function takes in the 16 byte Source Access Point
**                  Identifier (SAPI), the 16 byte Destination Access Point
**                  Identifier (DAPI), and the 32 byte Operator Specific fields
**                  as arguments to fill the 64 byte TTI.
**
**                  The range of a SAPI or DAPI characters 15 to 1 are 0 to 127.
**                  SAPI and DAPI character 0 must be set to 0x00.  Function
**                  will return HYPHY20G_ERR_INVALID_ARG if these rules are not
**                  adhered.
**
**                  There are no restrictions on the values placed in Operator
**                  Specific bytes.
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
**                  sapi[16]    - SAPI characters (16 bytes)
**                  dapi[16]    - DAPI characters (16 bytes)
**                  opSpec[32]  - Operator Specific (32 bytes)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxSmTtiMsgCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 sapi[16], UINT4 dapi[16],
                                     UINT4 opSpec[32])
{
    UINT4 subSlcOff;
    UINT4 addr;
    UINT4 ttiMsk;
    UINT4 regData;
    UINT4 val;
    UINT4 i, j;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (sapi[0] > 0) || (dapi[0] > 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < 32; i++) {
        if ((i < 16) && (sapi[i] > 127 || dapi[i] > 127)) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        if (opSpec[i] > 255) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    /* Configure force transmit SM TTI message */
    j = 0;
    for (i = 0; i < 32; i++) {

        /* Determine register address */
        if (slice > 7) {
            /* OTU2 Framers */
            subSlcOff = slice - 8;

            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_SM_TTI_MEM_OFFSET(i);
            addr = 
                 HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
            ttiMsk = 
            HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_TTI_MEM_SM_TTI_VALUE_MSK;
        } else {
            /* OTU1 Framers */
            subSlcOff = slice;

            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_SM_TTI_MEM_OFFSET(i);
            addr = 
                 HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
            ttiMsk = 
            HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_TTI_MEM_SM_TTI_VALUE_MSK;
        }

        /* Read register */
        result = sysHyPhy20gRead(fileHndl, addr, &regData);
        if(result) return result;

        /* Modify read data */
        j = i*2;
        if (i < 8) {
            /* Configure SAPI (2 bytes per register) */
            val = (sapi[j+1] << 8) | sapi[j];
        } else if (i < 16) {
            /* Configure DAPI (2 bytes per register) */
            j = j - 16;
            val = (dapi[j+1] << 8) | dapi[j];
        } else {
            /* Configure Operator Specific (2 bytes per register) */
            j = j - 32;
            val = (opSpec[j+1] << 8) | opSpec[j];
        }
        result = hyPhy20gVarFieldWrite(&regData, ttiMsk, val);
        if(result) return result;

        /* Write data to register */
        result = sysHyPhy20gWrite(fileHndl, addr, regData);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOtukTxSmTtiMsgCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukTxSmTtiMsgSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the SM TTI message source of OTUk
**                  framers for the transmit data stream in ODUKPP subsystem.
**
**                  When configured for 'Incoming SM TTI value' the value
**                  transferred on SM TTI bits will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**                  When configured for 'Force TTI value',
**                  hyPhy20gOdukppOtukTxSmTtiMsgCfg must be called to configure the
**                  forced TTI values.
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
**                  source      - SM TTI source
**                                0 - Incoming SM TTI value
**                                1 - Force TTI value
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxSmTtiMsgSrcCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 source)
{
    UINT4 subSlcOff;
    UINT4 regData;
    UINT4 smCtlReg;
    UINT4 insSmMsk, insSmVal;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (source > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        smCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_SM_CTL;
        smCtlReg = 
             HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        insSmMsk = 
               HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_INSERT_SM_TTI_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        smCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_SM_CTL;
        smCtlReg = 
             HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(smCtlReg, subSlcOff);
        insSmMsk = 
               HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_INSERT_SM_TTI_MSK;
    }

    switch (source) {
        case 0:
            /* Incoming SM TTI value */
            insSmVal = 0;
            break;
        case 1:
            /* Force SM TTI value */
            insSmVal = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Read SM control register */
    result = sysHyPhy20gRead(fileHndl, smCtlReg, &regData);
    if(result) return result;

    /* Configure INSERT_SM_TTI */
    result = hyPhy20gVarFieldWrite(&regData, insSmMsk, insSmVal);
    if(result) return result;

    /* Write SM control register */
    result = sysHyPhy20gWrite(fileHndl, smCtlReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOtukTxSmTtiMsgSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxResProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the OTUk RES bits in ODUKPP subsystem.  It configures how 
**                  the RES bits are monitored for operation of RES accepted and 
**                  inconsistent interrupts.
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
**                  acptCnt     - Number of identical RES messages that the RX 
**                                framer needs to obtain before declaring
**                                the RES as accepted
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical  RES messages that
**                                the RX framer needs to obtain before
**                                triggering a RES inconsistent error
**                                interrupt
**                              - Range 1 to 7
**                              - Default value 0x5
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxResProcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 acptCnt, UINT4 inconCnt)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;

    /* argument checking */
    if (slice > 9 || acptCnt < 1 || acptCnt > 7 ||
        inconCnt < 1 || inconCnt > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_GCC0_RES_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_GCC0_RES_CTL_RES_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_GCC0_RES_CTL_RES_INCONSISTENT_COUNT_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_GCC0_RES_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_GCC0_RES_CTL_RES_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_GCC0_RES_CTL_RES_INCONSISTENT_COUNT_MSK;
    }

    /* read GCC0/RES control register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Modify regData for RES_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Modify regData for RES_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* write GCC0/RES control register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukRxResProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxResAcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive OTUk RES bits
**                  in ODUKPP subsystem.
**
**                  The RES bits are reserved for future international
**                  standardization. G.709 specifies that they are set to
**                  "0x00".
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
**                  *pRes       - RES bits
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxResAcptExt(struct file *fileHndl, UINT4 slice, 
                                    UINT4 *pRes)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 acptValMsk;
    UINT4 acptValOff;

    /* argument checking */
    if (slice > 9) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (pRes == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_RES_ACPT;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptValMsk  = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_RES_ACPT_RES_ACCEPT_VALUE_MSK;
        acptValOff  = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_RES_ACPT_RES_ACCEPT_VALUE_OFF;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_RES_ACPT;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptValMsk  = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_RES_ACPT_RES_ACCEPT_VALUE_MSK;
        acptValOff  = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_RES_ACPT_RES_ACCEPT_VALUE_OFF;
    }

    /* Read register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Store RES_ACCEPT_VALUE */
    *pRes = mHYPHY20G_GET_FIELD4(regData, acptValMsk, acptValOff);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukRxResAcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukTxResSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the OTUk RES source for the
**                  transmit data stream in ODUKPP subsystem.
**
**                  When configured for 'Incoming RES value' the value
**                  transferred on RES bits will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
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
**                  source      - OTUk RES source
**                                0 - Incoming RES value
**                                1 - Force RES value
**                  forceVal   - For source == 1 (otherwise set to 0)
**                               The value to be inserted into RES in
**                               outgoing data stream
**                             - Legal range 0 to 0xFFFF
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxResSrcCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 source, UINT4 forceVal)
{
    INT4 result;
    UINT4 insRegAddr;
    UINT4 resValRegAddr;
    UINT4 subSlcOff;
    UINT4 insOff;
    UINT4 resValMsk;

    /* argument checking */
    if (slice > 9 || source > 1 || forceVal > 0xFFFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        insRegAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_FA_OTU_CTL;
        insRegAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(insRegAddr,
                                                                   subSlcOff);

        resValRegAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_RES_VAL;
        resValRegAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET
                       (resValRegAddr, subSlcOff);

        insOff = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_FA_OTU_CTL_INSERT_RES_OFF;
        resValMsk
                = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_RES_VAL_RES_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        insRegAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_FA_OTU_CTL;
        insRegAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(insRegAddr,
                                                                   subSlcOff);

        resValRegAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_RES_VAL;
        resValRegAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET
                        (resValRegAddr, subSlcOff);
                        
        insOff = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_FA_OTU_CTL_INSERT_RES_OFF;
        resValMsk
                = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_RES_VAL_RES_VALUE_MSK;
    }

    /* Force insertion of RES - configure RES_VALUE when source == 1 */
    if (source) {
        result = hyPhy20gRegFieldWrite(fileHndl, resValRegAddr,
                                       resValMsk, forceVal);
        if (result) return result;
    }

    /* configure INSERT_RES */
    result = sysHyPhy20gBitWrite(fileHndl, insRegAddr, insOff, source);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukTxResSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxGcc0ProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the OTUk GCC0 bits in ODUKPP subsystem.  It configures 
**                  how the GCC0 bits are monitored for operation of GCC0 
**                  accepted and inconsistent interrupts.
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
**                  acptCnt     - Number of identical GCC0 messages that the RX
**                                framer needs to obtain before declaring the 
**                                GCC0 as accepted
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical GCC0 messages that
**                                the RX framer needs to obtain before
**                                triggering a GCC0 inconsistent error
**                                interrupt
**                              - Range 1 to 7
**                              - Default value 0x5
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxGcc0ProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 acptCnt, UINT4 inconCnt)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;

    /* argument checking */
    if (slice > 9 || acptCnt < 1 || acptCnt > 7 ||
        inconCnt < 1 || inconCnt > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_GCC0_RES_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_GCC0_RES_CTL_GCC0_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_GCC0_RES_CTL_GCC0_INCONSISTENT_COUNT_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_GCC0_RES_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_GCC0_RES_CTL_GCC0_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_GCC0_RES_CTL_GCC0_INCONSISTENT_COUNT_MSK;
    }

    /* read GCC0/RES control register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Modify regData for GCC0_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Modify regData for GCC0_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* write GCC0/RES control register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukRxGcc0ProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxGcc0AcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive OTUk GCC0 bits.
**
**                  The GCC0 channel is a general communication channel, this
**                  function is only useful to extract a continous two byte
**                  message.
**
**                  Customer applications will require use of overhead port to
**                  terminate GCC0 channels used for applications other than
**                  continuous two byte messages.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - OTUk Framer instance:
**                                 0 - OTU1 Framer 0
**                                 1 - OTU1 Framer 1
**                                 2 - OTU1 Framer 2
**                                 3 - OTU1 Framer 3
**                                 4 - OTU1 Framer 4
**                                 5 - OTU1 Framer 5
**                                 6 - OTU1 Framer 6
**                                 7 - OTU1 Framer 7
**                                 8 - OTU2 Framer 0
**                                 9 - OTU2 Framer 1
**                  *pGcc0      - GCC0 bits
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxGcc0AcptExt(struct file *fileHndl, UINT4 slice, 
                                     UINT4 *pGcc0)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 acptValMsk;
    UINT4 acptValOff;

    /* argument checking */
    if (slice > 9) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (pGcc0 == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_GCC0_ACPT;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptValMsk = 
        HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_GCC0_ACPT_GCC0_ACCEPT_VALUE_MSK;
        acptValOff = 
        HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_GCC0_ACPT_GCC0_ACCEPT_VALUE_OFF;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_GCC0_ACPT;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptValMsk =
        HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_GCC0_ACPT_GCC0_ACCEPT_VALUE_MSK;
        acptValOff = 
        HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_GCC0_ACPT_GCC0_ACCEPT_VALUE_OFF;
    }

    /* Read register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Store GCC0_ACCEPT_VALUE */
    *pGcc0 = mHYPHY20G_GET_FIELD4(regData, acptValMsk, acptValOff);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukRxGcc0AcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukTxGcc0SrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the OTUk GCC0 source for the
**                  transmit data stream.
**
**                  When configured for 'Incoming GCC0 value' the value
**                  transferred on GCC0 bits will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
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
**                  source      - OTUk GCC0 source
**                                0 - Incoming GCC0 value
**                                1 - Force GCC0 value
**                  forceVal   - For source == 1 (otherwise set to 0)
**                               The value to be inserted into GCC0 in
**                               outgoing data stream
**                             - Legal range 0 to 0xFFFF
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxGcc0SrcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 source, UINT4 forceVal)
{
    INT4 result;
    UINT4 subSlcOff;
    UINT4 insRegAddr;
    UINT4 gcc0ValRegAddr;
    UINT4 insOff;
    UINT4 gcc0ValMsk;

    /* argument checking */
    if (slice > 9 || source > 1 || forceVal > 0xFFFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        insRegAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_FA_OTU_CTL;
        insRegAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(insRegAddr,
                                                                   subSlcOff);

        gcc0ValRegAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_GCC0_VAL;
        gcc0ValRegAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET
                        (gcc0ValRegAddr, subSlcOff);

        insOff = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_FA_OTU_CTL_INSERT_GCC0_OFF;
        gcc0ValMsk = 
                HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_GCC0_VAL_GCC0_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        insRegAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_FA_OTU_CTL;
        insRegAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(insRegAddr,
                                                                   subSlcOff);

        gcc0ValRegAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_GCC0_VAL;
        gcc0ValRegAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET
                        (gcc0ValRegAddr, subSlcOff);

        insOff = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_FA_OTU_CTL_INSERT_GCC0_OFF;
        gcc0ValMsk 
              = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_GCC0_VAL_GCC0_VALUE_MSK;
    }

    /* Force insertion of GCC0 - configure GCC0_VALUE when source == 1 */
    if (source) {
        result = hyPhy20gRegFieldWrite(fileHndl, gcc0ValRegAddr,
                                       gcc0ValMsk, forceVal);
        if (result) return result;
    }

    /* configure INSERT_GCC0 */
    result = sysHyPhy20gBitWrite(fileHndl, insRegAddr, insOff, source);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukTxGcc0SrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxRes1ProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the first set of ODUk RES bytes.  RES1 corresponds to
**                  the bytes in Column 1, 2 and 3 Row 2 of ODUk overhead.
**                  It configures how the RES1 bytes are monitored for operation
**                  of RES1 accepted and inconsistent interrupts.
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
**                  acptCnt     - Number of identical RES1 bytes that
**                                the RX framer needs to obtain before declaring
**                                the RES1 bytes as accepted
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical RES1 bytes that
**                                the RX framer needs to obtain before
**                                triggering a RES1 byte inconsistent error
**                                interrupt
**                              - Range 1 to 7
**                              - Default value 0x5
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxRes1ProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 acptCnt, UINT4 inconCnt)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;

    /* argument checking */
    if (slice > 9 || acptCnt < 1 || acptCnt > 7 ||
        inconCnt < 1 || inconCnt > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_RES1_TCM_ACT_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_RES1_TCM_ACT_CTL_RES1_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_RES1_TCM_ACT_CTL_RES1_INCONSISTENT_COUNT_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_RES1_TCM_ACT_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_RES1_TCM_ACT_CTL_RES1_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_RES1_TCM_ACT_CTL_RES1_INCONSISTENT_COUNT_MSK;
    }

    /* read RES1/TCM ACT Control register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Modify regData for RES1_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Modify regData for RES1_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* write RES1/TCM ACT Control register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukRxRes1ProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxRes1AcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive ODUk RES1 bytes.
**
**                  RES1 corresponds to the bytes in Column 1, 2 and 3 Row 2 of
**                  ODUk overhead.
**
**                  The RES1 bytes are reserved for future international
**                  standardization. G.709 specifies that they are set to "00".
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - OTUk Framer instance:
**                                 0 - OTU1 Framer 0
**                                 1 - OTU1 Framer 1
**                                 2 - OTU1 Framer 2
**                                 3 - OTU1 Framer 3
**                                 4 - OTU1 Framer 4
**                                 5 - OTU1 Framer 5
**                                 6 - OTU1 Framer 6
**                                 7 - OTU1 Framer 7
**                                 8 - OTU2 Framer 0
**                                 9 - OTU2 Framer 1
**                  *pRes1      - ODUk RES1 bytes
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxRes1AcptExt(struct file *fileHndl, UINT4 slice, 
                                     UINT4 *pRes1)
{
    INT4 result;
    UINT4 res1HiAcptAddr, res1LoAcptAddr;
    UINT4 regData, val;
    UINT4 subSlcOff;
    UINT4 res1HiAcptMsk, res1LoAcptMsk;

    /* Argument checking */
    if (slice > 9) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pRes1 == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        res1HiAcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_RES1_HI_ACPT;
        res1HiAcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET
                        (res1HiAcptAddr, subSlcOff);

        res1LoAcptAddr = 
              HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_RES1_LOW_TCM_ACT_ACPT_VAL;
        res1LoAcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET
                        (res1LoAcptAddr, subSlcOff);

        res1HiAcptMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_RES1_HI_ACPT_RES1_HIGH_ACCEPT_VALUE_MSK;
        res1LoAcptMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_RES1_LOW_TCM_ACT_ACPT_VAL_RES1_LOW_ACCEPT_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        res1HiAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_RES1_HI_ACPT;
        res1HiAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET
                        (res1HiAcptAddr, subSlcOff);

        res1LoAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_RES1_LOW_TCM_ACT_ACPT_VAL;
        res1LoAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET
                        (res1LoAcptAddr, subSlcOff);

        res1HiAcptMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_RES1_HI_ACPT_RES1_HIGH_ACCEPT_VALUE_MSK;
        res1LoAcptMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_RES1_LOW_TCM_ACT_ACPT_VAL_RES1_LOW_ACCEPT_VALUE_MSK;
    }

    /* read RES1 HIGH ACCEPT register */
    result = sysHyPhy20gRead(fileHndl, res1HiAcptAddr, &regData);
    if(result) return result;

    /* store RES1_HIGH_ACCEPT_VALUE */
    result = hyPhy20gVarFieldRead(regData, res1HiAcptMsk, &val);
    if(result) return result;
    *pRes1 = val << 8;

    /* read RES1 LOW ACCEPT register */
    result = sysHyPhy20gRead(fileHndl, res1LoAcptAddr, &regData);
    if(result) return result;

    /* store RES1_LOW_ACCEPT_VALUE */
    result = hyPhy20gVarFieldRead(regData, res1LoAcptMsk, &val);
    if(result) return result;
    *pRes1 = *pRes1 | val;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukRxRes1AcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxRes2ProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the second set of ODUk RES bytes.  RES2 corresponds to
**                  the bytes in Column 9 to 14 Row 4 of ODUk overhead.
**
**                  It configures how the RES2 bytes are
**                  monitored for operation of RES2 accepted and inconsistent
**                  interrupts.
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
**                  acptCnt     - Number of identical RES2 bytes that
**                                the RX framer needs to obtain before declaring
**                                the RES2 bytes as accepted
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical RES2 bytes that
**                                the RX framer needs to obtain before
**                                triggering a RES1 byte inconsistent error
**                                interrupt
**                              - Range 1 to 7
**                              - Default value 0x5
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxRes2ProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 acptCnt, UINT4 inconCnt)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;

    /* argument checking */
    if (slice > 9 || acptCnt < 1 || acptCnt > 7 ||
        inconCnt < 1 || inconCnt > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_APS_PCC_RES2_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk  = 
 HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_APS_PCC_RES2_CTL_RES2_ACCEPT_COUNT_MSK;
        inconCntMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_APS_PCC_RES2_CTL_RES2_INCONSISTENT_COUNT_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_APS_PCC_RES2_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = 
 HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_APS_PCC_RES2_CTL_RES2_ACCEPT_COUNT_MSK;
        inconCntMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_APS_PCC_RES2_CTL_RES2_INCONSISTENT_COUNT_MSK;
    }

    /* read APS PCC/RES2 Control register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Modify regData for RES2_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Modify regData for RES2_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* write APS PCC/RES2 Control register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukRxRes2ProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxRes2AcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive ODUk RES2 bytes.
**
**                  RES2 corresponds to the bytes in Column 9 to 14 Row 4 of
**                  ODUk overhead.
**
**                  The RES2 bytes are reserved for future international
**                  standardization. G.709 specifies that they are set to "00".
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
**                  *pRes2Hi    - ODUk RES2 bytes 5:4
**                  *pRes2Low   - ODUk RES2 bytes 3:0
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxRes2AcptExt(struct file *fileHndl, UINT4 slice,
                                     UINT4 *pRes2Hi, UINT4 *pRes2Low)
{
    INT4 result;
    UINT4 res2HiAcptAddr, res2MidAcptAddr, res2LoAcptAddr;
    UINT4 regData, val;
    UINT4 subSlcOff;
    UINT4 res2HiAcptMsk, res2MidAcptMsk, res2LoAcptMsk;

    /* Argument checking */
    if (slice > 9) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pRes2Hi == NULL || pRes2Low == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        res2HiAcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_RES2_HI_ACPT;
        res2HiAcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET
                         (res2HiAcptAddr, subSlcOff);

        res2MidAcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_RES2_MID_ACPT;
        res2MidAcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET
                          (res2MidAcptAddr, subSlcOff);

        res2LoAcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_RES2_LOW_ACPT;
        res2LoAcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET
                         (res2LoAcptAddr, subSlcOff);

        res2HiAcptMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_RES2_HI_ACPT_RES2_HIGH_ACCEPT_VALUE_MSK;
        res2MidAcptMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_RES2_MID_ACPT_RES2_MID_ACCEPT_VALUE_MSK;
        res2LoAcptMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_RES2_LOW_ACPT_RES2_LOW_ACCEPT_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        res2HiAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_RES2_HI_ACPT;
        res2HiAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET
                         (res2HiAcptAddr, subSlcOff);

        res2MidAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_RES2_MID_ACPT;
        res2MidAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET
                          (res2MidAcptAddr, subSlcOff);

        res2LoAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_RES2_LOW_ACPT;
        res2LoAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET
                         (res2LoAcptAddr,
subSlcOff);

        res2HiAcptMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_RES2_HI_ACPT_RES2_HIGH_ACCEPT_VALUE_MSK;
        res2MidAcptMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_RES2_MID_ACPT_RES2_MID_ACCEPT_VALUE_MSK;
        res2LoAcptMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_RES2_LOW_ACPT_RES2_LOW_ACCEPT_VALUE_MSK;
    }

    /* read RES2 HIGH ACCEPT register */
    result = sysHyPhy20gRead(fileHndl, res2HiAcptAddr, &regData);
    if(result) return result;

    /* store RES2_HIGH_ACCEPT_VALUE */
    result = hyPhy20gVarFieldRead(regData, res2HiAcptMsk, &val);
    if(result) return result;
    *pRes2Hi = val;

    /* read RES2 MID ACCEPT register */
    result = sysHyPhy20gRead(fileHndl, res2MidAcptAddr, &regData);
    if(result) return result;

    /* store RES2_MID_ACCEPT_VALUE */
    result = hyPhy20gVarFieldRead(regData, res2MidAcptMsk, &val);
    if(result) return result;
    *pRes2Low = val << 16;

    /* read RES2 LOW ACCEPT register */
    result = sysHyPhy20gRead(fileHndl, res2LoAcptAddr, &regData);
    if(result) return result;

    /* store RES2_LOW_ACCEPT_VALUE */
    result = hyPhy20gVarFieldRead(regData, res2LoAcptMsk, &val);
    if(result) return result;
    *pRes2Low = *pRes2Low | val;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukRxRes2AcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxGcc1ProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the ODUk GCC1 bytes.
**
**                  It configures how the GCC1 bytes are monitored for operation
**                  of GCC1 accepted and inconsistent interrupts.
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
**                  acptCnt     - Number of identical GCC1 bytes that
**                                the RX framer needs to obtain before declaring
**                                the GCC1 bytes as accepted
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical GCC1 bytes that
**                                the RX framer needs to obtain before
**                                triggering a GCC1 byte inconsistent error
**                                interrupt
**                              - Range 1 to 7
**                              - Default value 0x5
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxGcc1ProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 acptCnt, UINT4 inconCnt)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;

    /* argument checking */
    if (slice > 9 || acptCnt < 1 || acptCnt > 7 ||
        inconCnt < 1 || inconCnt > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_GCC1_GCC2_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = 
    HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_GCC1_GCC2_CTL_GCC1_ACCEPT_COUNT_MSK;
        inconCntMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_GCC1_GCC2_CTL_GCC1_INCONSISTENT_COUNT_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_GCC1_GCC2_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = 
    HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_GCC1_GCC2_CTL_GCC1_ACCEPT_COUNT_MSK;
        inconCntMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_GCC1_GCC2_CTL_GCC1_INCONSISTENT_COUNT_MSK;
    }

    /* read GCC1/GCC2 Control register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Modify regData for GCC1_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Modify regData for GCC1_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* write GCC1/GCC2 Control register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxGcc1ProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxGcc1AcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive ODUk GCC1 byte.
**
**                  The GCC1 channel is a general communication channel, this
**                  function is only useful to extract a continous one byte
**                  message.
**
**                  Customer applications will require use of overhead port to
**                  terminate GCC1 channels used for applications other than
**                  continuous one byte messages.
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
**                  *pGcc1      - ODUk GCC1 bytes
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxGcc1AcptExt(struct file *fileHndl, UINT4 slice, 
                                     UINT4 *pGcc1)
{
    INT4 result;
    UINT4 gcc1AcptAddr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 gcc1AcptMsk;

    /* Argument checking */
    if (slice > 9) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pGcc1 == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        gcc1AcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_GCC1_ACPT;
        gcc1AcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET
                       (gcc1AcptAddr, subSlcOff);

        gcc1AcptMsk = 
        HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_GCC1_ACPT_GCC1_ACCEPT_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        gcc1AcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_GCC1_ACPT;
        gcc1AcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET
                       (gcc1AcptAddr, subSlcOff);

        gcc1AcptMsk = 
        HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_GCC1_ACPT_GCC1_ACCEPT_VALUE_MSK;
    }

    /* read GCC1 ACCEPT register */
    result = sysHyPhy20gRead(fileHndl, gcc1AcptAddr, &regData);
    if(result) return result;

    /* store GCC1_ACCEPT_VALUE */
    result = hyPhy20gVarFieldRead(regData, gcc1AcptMsk, pGcc1);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukRxGcc1AcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxGcc2ProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the ODUk GCC2 bytes.
**
**                  It configures how the GCC2 bytes are
**                  monitored for operation of GCC2 accepted and inconsistent
**                  interrupts.
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
**                  acptCnt     - Number of identical GCC2 bytes that
**                                the RX framer needs to obtain before declaring
**                                the GCC2 bytes as accepted
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical GCC2 bytes that
**                                the RX framer needs to obtain before
**                                triggering a GCC2 byte inconsistent error
**                                interrupt
**                              - Range 1 to 7
**                              - Default value 0x5
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxGcc2ProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 acptCnt, UINT4 inconCnt)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;

    /* argument checking */
    if (slice > 9 || acptCnt < 1 || acptCnt > 7 ||
        inconCnt < 1 || inconCnt > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_GCC1_GCC2_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = 
    HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_GCC1_GCC2_CTL_GCC2_ACCEPT_COUNT_MSK;
        inconCntMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_GCC1_GCC2_CTL_GCC2_INCONSISTENT_COUNT_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_GCC1_GCC2_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = 
    HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_GCC1_GCC2_CTL_GCC2_ACCEPT_COUNT_MSK;
        inconCntMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_GCC1_GCC2_CTL_GCC2_INCONSISTENT_COUNT_MSK;
    }

    /* read GCC1/GCC2 Control register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Modify regData for GCC2_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Modify regData for GCC2_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* write GCC1/GCC2 Control register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukRxGcc2ProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxGcc2AcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive ODUk GCC2 bytes.
**
**                  The GCC2 channel is a general communication channel, this
**                  function is only useful to extract a continous one byte
**                  message.
**
**                  Customer applications will require use of overhead port to
**                  terminate GCC2 channels used for applications other than
**                  continuous one byte messages.
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
**                  *pGcc2      - ODUk GCC2 bytes
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxGcc2AcptExt(struct file *fileHndl, UINT4 slice, 
                                     UINT4 *pGcc2)
{
    INT4 result;
    UINT4 gcc2AcptAddr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 gcc2AcptMsk;

    /* Argument checking */
    if (slice > 9) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pGcc2 == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        gcc2AcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_GCC2_ACPT;
        gcc2AcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET
                       (gcc2AcptAddr, subSlcOff);

        gcc2AcptMsk = 
        HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_GCC2_ACPT_GCC2_ACCEPT_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        gcc2AcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_GCC2_ACPT;
        gcc2AcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET
                       (gcc2AcptAddr, subSlcOff);

        gcc2AcptMsk = 
        HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_GCC2_ACPT_GCC2_ACCEPT_VALUE_MSK;
    }

    /* read GCC2 ACCEPT register */
    result = sysHyPhy20gRead(fileHndl, gcc2AcptAddr, &regData);
    if(result) return result;

    /* store GCC2_ACCEPT_VALUE */
    result = hyPhy20gVarFieldRead(regData, gcc2AcptMsk, pGcc2);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukRxGcc2AcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxTcmActProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the ODUk TCM_ACT byte.
**
**                  It configures how the TCM_ACT byte is monitored for 
**                  operation of TCM_ACT accepted and inconsistent interrupts.
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
**                  acptCnt     - Number of identical TCM_ACT bytes that
**                                the RX framer needs to obtain before declaring
**                                the TCM_ACT bytes as accepted
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical TCM_ACT bytes that
**                                the RX framer needs to obtain before
**                                triggering a TCM_ACT byte inconsistent error
**                                interrupt
**                              - Range 1 to 7
**                              - Default value 0x5
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxTcmActProcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 acptCnt, UINT4 inconCnt)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;

    /* argument checking */
    if (slice > 9 || acptCnt < 1 || acptCnt > 7 ||
        inconCnt < 1 || inconCnt > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_RES1_TCM_ACT_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_RES1_TCM_ACT_CTL_TCM_ACT_ACCEPT_COUNT_MSK;
        inconCntMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_RES1_TCM_ACT_CTL_TCM_ACT_INCONSISTENT_COUNT_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_RES1_TCM_ACT_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_RES1_TCM_ACT_CTL_TCM_ACT_ACCEPT_COUNT_MSK;
        inconCntMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_RES1_TCM_ACT_CTL_TCM_ACT_INCONSISTENT_COUNT_MSK;
    }

    /* read RES1/TCM ACT Control register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Modify regData for TCM_ACT_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Modify regData for TCM_ACT_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* write RES1/TCM ACT Control register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxTcmActProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxTcmActAcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive ODUk
**                  TCM_ACT byte.
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
**                  *pTcmAct    - ODUk TCM_ACT byte
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxTcmActAcptExt(struct file *fileHndl, UINT4 slice,
                                       UINT4 *pTcmAct)
{
    INT4 result;
    UINT4 tcmActAddr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 tcmActMsk;

    /* Argument checking */
    if (slice > 9) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pTcmAct == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        tcmActAddr = 
              HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_RES1_LOW_TCM_ACT_ACPT_VAL;
        tcmActAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(tcmActAddr,
                                                                   subSlcOff);

        tcmActMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_RES1_LOW_TCM_ACT_ACPT_VAL_TCM_ACT_ACCEPT_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        tcmActAddr = 
              HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_RES1_LOW_TCM_ACT_ACPT_VAL;
        tcmActAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(tcmActAddr,
                                                                   subSlcOff);

        tcmActMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_RES1_LOW_TCM_ACT_ACPT_VAL_TCM_ACT_ACCEPT_VALUE_MSK;
    }

    /* read TCM ACT register */
    result = sysHyPhy20gRead(fileHndl, tcmActAddr, &regData);
    if(result) return result;

    /* store TCM_ACT_ACCEPT_VALUE */
    result = hyPhy20gVarFieldRead(regData, tcmActMsk, pTcmAct);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukRxTcmActAcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxExpProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the ODUk EXP bytes.
**
**                  It configures how the EXP bytes re
**                  monitored for operation of EXP accepted and inconsistent
**                  interrupts.
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
**                  acptCnt     - Number of identical EXP bytes that
**                                the RX framer needs to obtain before declaring
**                                the EXP bytes as accepted
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical EXP bytes that
**                                the RX framer needs to obtain before
**                                triggering a EXP byte inconsistent error
**                                interrupt
**                              - Range 1 to 7
**                              - Default value 0x5
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxExpProcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 acptCnt, UINT4 inconCnt)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;

    /* argument checking */
    if (slice > 9 || acptCnt < 1 || acptCnt > 7 ||
        inconCnt < 1 || inconCnt > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_FTFL_EXP_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = 
      HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_FTFL_EXP_CTL_EXP_ACCEPT_COUNT_MSK;
        inconCntMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_FTFL_EXP_CTL_EXP_INCONSISTENT_COUNT_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_FTFL_EXP_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = 
      HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_FTFL_EXP_CTL_EXP_ACCEPT_COUNT_MSK;
        inconCntMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_FTFL_EXP_CTL_EXP_INCONSISTENT_COUNT_MSK;
    }

    /* read FTFL/EXP Control register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Modify regData for EXP_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Modify regData for EXP_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* write FTFL/EXP Control register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxExpProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxExpAcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive ODUk EXP bytes.
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
**                  *pExp       - ODUk EXP bytes
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxExpAcptExt(struct file *fileHndl, UINT4 slice, 
                                    UINT4 *pExp)
{
    INT4 result;
    UINT4 expAddr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 expMsk;

    /* Argument checking */
    if (slice > 9) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pExp == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        expAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_EXP_ACPT;
        expAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(expAddr, 
                                                                subSlcOff);

        expMsk = 
          HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_EXP_ACPT_EXP_ACCEPT_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        expAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_EXP_ACPT;
        expAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(expAddr, 
                                                                subSlcOff);

        expMsk = 
          HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_EXP_ACPT_EXP_ACCEPT_VALUE_MSK;
    }

    /* read EXP register */
    result = sysHyPhy20gRead(fileHndl, expAddr, &regData);
    if(result) return result;

    /* store EXP_ACCEPT_VALUE */
    result = hyPhy20gVarFieldRead(regData, expMsk, pExp);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukRxExpAcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxFtflProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the ODUk FTFL byte.
**
**                  It configures how the FTFL byte are
**                  monitored for operation of FTFL accepted and inconsistent
**                  interrupts.
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
**                  acptCnt     - Number of identical FTFL messages that
**                                the RX framer needs to obtain before declaring
**                                the FTFL message as accepted
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical FTFL messages that
**                                the RX framer needs to obtain before
**                                triggering a EXP message inconsistent error
**                                interrupt
**                              - Range 1 to 7
**                              - Default value 0x5
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxFtflProcCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 acptCnt, UINT4 inconCnt)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;

    /* argument checking */
    if (slice > 9 || acptCnt < 1 || acptCnt > 7 ||
        inconCnt < 1 || inconCnt > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_FTFL_EXP_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = 
     HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_FTFL_EXP_CTL_FTFL_ACCEPT_COUNT_MSK;
        inconCntMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_FTFL_EXP_CTL_FTFL_INCONSISTENT_COUNT_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_FTFL_EXP_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = 
     HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_FTFL_EXP_CTL_FTFL_ACCEPT_COUNT_MSK;
        inconCntMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_FTFL_EXP_CTL_FTFL_INCONSISTENT_COUNT_MSK;
    }

    /* read FTFL/EXP Control register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Modify regData for FTFL_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Modify regData for FTFL_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* write FTFL/EXP Control register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxFtflProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxFtflAcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive FTFL
**                  message.
**
**                  The function extracts the 256 byte FTFL message relative to
**                  the following fields:
**                  - one byte Forward Indication Field
**                  - 9 byte Forward Operator Identifier Field
**                  - 118 byte Forward Operator-specific Field
**                  - one byte Backward Indication Field
**                  - 9 byte Backward Operator Identifier Field
**                  - 118 byte Backward Operator-specific Field
**
**  INPUTS:         *fileHndl      - base address
**                  slice          - OTUk Framer instance:
**                                   0 - OTU1 Framer 0
**                                   1 - OTU1 Framer 1
**                                   2 - OTU1 Framer 2
**                                   3 - OTU1 Framer 3
**                                   4 - OTU1 Framer 4
**                                   5 - OTU1 Framer 5
**                                   6 - OTU1 Framer 6
**                                   7 - OTU1 Framer 7
**                                   8 - OTU2 Framer 0
**                                   9 - OTU2 Framer 1
**                  pFwdIdFld      - Forward Indication Field.
**                                   One byte field
**                                   Byte 0 of the FTFL message
**                  *pFwdOpId[9]   - Forward Operator Identifier Field
**                                   9 byte field
**                                   Byte 9:1 of the FTFL message
**                  *pFwdOpSp[118] - Forward Operator-specific Field.
**                                   118 byte field
**                                   Byte 127:10 of the FTFL message
**                  *pBwdIdFld     - Backward Indication Field.
**                                   One byte field
**                                   Byte 128 of the FTFL message
**                  *pBwdOpId[9]   - Backward Operator Identifier Field.
**                                   9 byte field
**                                   Byte 137:129 of the FTFL message
**                  *pBwdOpSp[118] - Backward Operator-specific Field.
**                                   118 byte field
**                                   Byte 255:138 of the FTFL message
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxFtflAcptExt(struct file *fileHndl, UINT4 slice,
                               UINT4 *pFwdIdFld, UINT4 *pFwdOpId,
                               UINT4 *pFwdOpSp, UINT4 *pBwdIdFld,
                               UINT4 *pBwdOpId, UINT4 *pBwdOpSp)
{
    INT4 result;
    UINT4 ftflExtAddr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 i;

    /* Argument checking */
    if (slice > 9) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pFwdIdFld == NULL || pFwdOpId == NULL || pFwdOpSp == NULL ||
        pBwdIdFld == NULL || pBwdOpId == NULL || pBwdOpSp == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    for (i = 0; i < 128; i++) {

        /* Determine register address */
        if (slice > 7) {
            /* OTU2 Framers */
            subSlcOff = slice - 8;

            ftflExtAddr = 
                 HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_FTFL_EXT_MEM_OFFSET(i);
            ftflExtAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(
                                                        ftflExtAddr, subSlcOff);
        } else {
            /* OTU1 Framers */
            subSlcOff = slice;

            ftflExtAddr = 
                 HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_FTFL_EXT_MEM_OFFSET(i);
            ftflExtAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                                                        ftflExtAddr, subSlcOff);
        }

        /* Read register */
        result = sysHyPhy20gRead(fileHndl, ftflExtAddr, &regData);
        if(result) return result;

        /* Extract data */
        if (i == 0) {
            /* Byte 0 - Forward Indication Field */
            *pFwdIdFld = regData & 0xFF;

            /* Byte 1 - Forward Operator Identifier Field */
            *pFwdOpId = (regData >> 8) & 0xFF;
            pFwdOpId++;

        } else if (i >= 1 && i <= 4) {
            /* Bytes 2 to 9 - Forward Operator Identifier Field */
            *pFwdOpId = regData & 0xFF;
            pFwdOpId++;
            *pFwdOpId = (regData >> 8) & 0xFF;
            pFwdOpId++;

        } else if (i >= 5 && i <= 63) {
            /* Bytes 10 to 127 - Forward Operator-specific Field */
            *pFwdOpSp = regData & 0xFF;
            pFwdOpSp++;
            *pFwdOpSp = (regData >> 8) & 0xFF;
            pFwdOpSp++;

        } else if (i == 64) {
            /* Byte 128 - Backward Indication Field */
            *pBwdIdFld = regData & 0xFF;

            /* Byte 129 - Backward Operator Identifier Field */
            *pBwdOpId = (regData >> 8) & 0xFF;
            pBwdOpId++;

        } else if (i >= 65 && i <= 68) {
            /* Bytes 130 to 137 - Backward Operator Identifier Field */
            *pBwdOpId = regData & 0xFF;
            pBwdOpId++;
            *pBwdOpId = (regData >> 8) & 0xFF;
            pBwdOpId++;

        } else {
            /* Bytes 138 to 255 - Forward Operator-specific Field */
            *pBwdOpSp = regData & 0xFF;
            pBwdOpSp++;
            *pBwdOpSp = (regData >> 8) & 0xFF;
            pBwdOpSp++;
        }
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukRxFtflAcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxApsPccProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the ODUk APS_PCC bytes.
**
**                  It configures how the APS_PCC bytes are monitored for 
**                  operation of APS_PCC accepted and inconsistent interrupts.
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
**                  acptCnt     - Number of identical APS_PCC messages that
**                                the RX framer needs to obtain before declaring
**                                the APS_PCC bytes as accepted
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical APS_PCC messages that
**                                the RX framer needs to obtain before
**                                triggering a APS_PCC message inconsistent
**                                error interrupt
**                              - Range 1 to 7
**                              - Default value 0x5
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxApsPccProcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 acptCnt, UINT4 inconCnt)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 subSlcOff;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;

    /* argument checking */
    if (slice > 9 || acptCnt < 1 || acptCnt > 7 ||
        inconCnt < 1 || inconCnt > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_APS_PCC_RES2_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_APS_PCC_RES2_CTL_APS_PCC_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_APS_PCC_RES2_CTL_APS_PCC_INCONSISTENT_COUNT_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_APS_PCC_RES2_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_APS_PCC_RES2_CTL_APS_PCC_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_APS_PCC_RES2_CTL_APS_PCC_INCONSISTENT_COUNT_MSK;
    }

    /* read APS PCC/RES2 Control register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Modify regData for APS_PCC_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Modify regData for APS_PCC_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* write APS PCC/RES2 Control register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxApsPccProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxApsPccAcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted received ODUk
**                  APS_PCC bytes.
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
**                  mFrame      - multiframe number to extract message from
**                                0 - 7
**                  *pApsPcc    - Extracted ODUk APS_PCC bytes
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxApsPccAcptExt(struct file *fileHndl, UINT4 slice,
                                       UINT4 mFrame, UINT4 *pApsPcc)
{
    INT4 result;
    UINT4 apsPccHiAcptAddr, apsPccLoAcptAddr;
    UINT4 regData, val;
    UINT4 subSlcOff;
    UINT4 apsPccHiAcptMsk, apsPccLoAcptMsk;

    /* Argument checking */
    if ((slice > 9) || (mFrame > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pApsPcc == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        apsPccHiAcptAddr = 
   HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_APS_PCC_MFASI_HI_ACPT_OFFSET(mFrame);
        apsPccHiAcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(
                                                   apsPccHiAcptAddr, subSlcOff);

        apsPccLoAcptAddr = 
  HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_APS_PCC_MFASI_LOW_ACPT_OFFSET(mFrame);
        apsPccLoAcptAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(
                                                   apsPccLoAcptAddr, subSlcOff);

        apsPccHiAcptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_APS_PCC_MFASI_HI_ACPT_APS_PCC_MFAS0_HIGH_ACCEPT_VALUE_MSK;
        apsPccLoAcptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_APS_PCC_MFASI_LOW_ACPT_APS_PCC_MFAS0_LOW_ACCEPT_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        apsPccHiAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_APS_PCC_MFASI_HI_ACPT_OFFSET(mFrame);
        apsPccHiAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                                                   apsPccHiAcptAddr, subSlcOff);

        apsPccLoAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_APS_PCC_MFASI_LOW_ACPT_OFFSET(mFrame);
        apsPccLoAcptAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                                                   apsPccLoAcptAddr, subSlcOff);

        apsPccHiAcptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_APS_PCC_MFASI_HI_ACPT_APS_PCC_MFAS0_HIGH_ACCEPT_VALUE_MSK;
        apsPccLoAcptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_APS_PCC_MFASI_LOW_ACPT_APS_PCC_MFAS0_LOW_ACCEPT_VALUE_MSK;
    }

    /* read APS/PCC HIGH ACCEPT register */
    result = sysHyPhy20gRead(fileHndl, apsPccHiAcptAddr, &regData);
    if(result) return result;

    /* store APS_PCC_MFAS0_HIGH_ACCEPT_VALUE */
    result = hyPhy20gVarFieldRead(regData, apsPccHiAcptMsk, &val);
    if(result) return result;
    *pApsPcc = val << 16;

    /* read APS/PCC LOW ACCEPT register */
    result = sysHyPhy20gRead(fileHndl, apsPccLoAcptAddr, &regData);
    if(result) return result;

    /* store APS_PCC_MFAS0_LOW_ACCEPT_VALUE */
    result = hyPhy20gVarFieldRead(regData, apsPccLoAcptMsk, &val);
    if(result) return result;
    *pApsPcc = *pApsPcc | val;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukRxApsPccAcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxFtflSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the FTFL message source for the
**                  transmit data stream.
**
**                  When configured for 'Incoming FTFL value' the value
**                  transferred on FTFL bits will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**                  When configured for 'Force FTFL value',
**                  hyPhy20gOdukppOdukTxFtflMsgCfg must be called to configure 
**                  the forced FTFL values.
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
**                  source      - FTFL source
**                                0 - Incoming FTFL value
**                                1 - Force FTFL message value
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxFtflSrcCfg(struct file *fileHndl, UINT4 slice, 
                                    UINT4 source)
{
    INT4 result;
    UINT4 addr;
    UINT4 subSlcOff;
    UINT4 insFtflOff;

    /* argument checking */
    if (slice > 9 || source > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        insFtflOff = 
                    HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_INSERT_FTFL_OFF;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        insFtflOff = 
                    HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_INSERT_FTFL_OFF;
    }

    /* configure INSERT_FTFL */
    result = sysHyPhy20gBitWrite(fileHndl, addr, insFtflOff, source);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxFtflSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxFtflMsgCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the FTFL message to be inserted in
**                  the transmit data stream.  This message is valid when
**                  hyPhy20gOdukppOdukTxFtflSrcCfg is called to set the FTFL 
**                  message source to the internally forced value.
**
**                  The function configures the 256 byte FTFL message relative
**                  to the following fields:
**                  - one byte Forward Indication Field
**                  - 9 byte Forward Operator Identifier Field
**                  - 118 byte Forward Operator-specific Field
**                  - one byte Backward Indication Field
**                  - 9 byte Backward Operator Identifier Field
**                  - 118 byte Backward Operator-specific Field
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - OTUk Framer instance:
**                                 0 - OTU1 Framer 0
**                                 1 - OTU1 Framer 1
**                                 2 - OTU1 Framer 2
**                                 3 - OTU1 Framer 3
**                                 4 - OTU1 Framer 4
**                                 5 - OTU1 Framer 5
**                                 6 - OTU1 Framer 6
**                                 7 - OTU1 Framer 7
**                                 8 - OTU2 Framer 0
**                                 9 - OTU2 Framer 1
**                  fwdIdFld     - Forward Indication Field.
**                                 One byte field
**                                 Byte 0 of the FTFL message
**                  fwdOpId[9]   - Forward Operator Identifier Field
**                                 9 byte field
**                                 Byte 9:1 of the FTFL message
**                  fwdOpSp[118] - Forward Operator-specific Field.
**                                 118 byte field
**                                 Byte 127:10 of the FTFL message
**                  bwdIdFld     - Backward Indication Field.
**                                 One byte field
**                                 Byte 128 of the FTFL message
**                  bwdOpId[9]   - Backward Operator Identifier Field.
**                                 9 byte field
**                                 Byte 137:129 of the FTFL message
**                  bwdOpSp[118] - Backward Operator-specific Field.
**                                 118 byte field
**                                 Byte 255:138 of the FTFL message
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxFtflMsgCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 fwdIdFld, UINT4 fwdOpId[9],
                                    UINT4 fwdOpSp[118], UINT4 bwdIdFld,
                                    UINT4 bwdOpId[9], UINT4 bwdOpSp[118])
{
    INT4 result;
    UINT4 ftflAddr;
    UINT4 regData, val;
    UINT4 subSlcOff;
    UINT4 ftflMsk;
    UINT4 *pFwdOpId, *pFwdOpSp, *pBwdOpId, *pBwdOpSp;
    UINT4 i;

    /* Argument checking */
    if ((slice > 9) || fwdIdFld > 255 || bwdIdFld > 255) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < 118; i++) {
        if ((fwdOpSp[i] > 255) || (bwdOpSp[i] > 255) ||
            ((i < 9) && (fwdOpId[i] > 255 || bwdOpId[i] > 255))) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    /* Initialize pointers to point to incoming arrays */
    pFwdOpId = fwdOpId;
    pFwdOpSp = fwdOpSp;
    pBwdOpId = bwdOpId;
    pBwdOpSp = bwdOpSp;

    for (i = 0; i < 128; i++) {

        /* Determine register address */
        if (slice > 7) {
            /* OTU2 Framers */
            subSlcOff = slice - 8;

            ftflAddr = 
                     HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_FTFL_MEM_OFFSET(i);
            ftflAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ftflAddr,
                                                                     subSlcOff);

            ftflMsk = 
                HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_FTFL_MEM_FTFL_VALUE_MSK;
        } else {
            /* OTU1 Framers */
            subSlcOff = slice;

            ftflAddr = 
                     HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_FTFL_MEM_OFFSET(i);
            ftflAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ftflAddr,
                                                                     subSlcOff);

            ftflMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_FTFL_MEM_FTFL_VALUE_MSK;
        }

        /* Read register */
        result = sysHyPhy20gRead(fileHndl, ftflAddr, &regData);
        if(result) return result;

        /* Modify read data */
        if (i == 0) {
            /* Byte 0 - Forward Indication Field */
            /* Byte 1 - Forward Operator Identifier Field */
            val = (*pFwdOpId << 8) | fwdIdFld;
            pFwdOpId++;

            result = hyPhy20gVarFieldWrite(&regData, ftflMsk, val);
            if(result) return result;

        } else if (i >= 1 && i <= 4) {
            /* Bytes 2 to 9 - Forward Operator Identifier Field */
            val = *pFwdOpId;
            pFwdOpId++;
            val |= *pFwdOpId << 8;
            pFwdOpId++;

            result = hyPhy20gVarFieldWrite(&regData, ftflMsk, val);
            if(result) return result;

        } else if (i >= 5 && i <= 63) {
            /* Bytes 10 to 127 - Forward Operator-specific Field */
            val = *pFwdOpSp;
            pFwdOpSp++;
            val |= *pFwdOpSp << 8;
            pFwdOpSp++;

            result = hyPhy20gVarFieldWrite(&regData, ftflMsk, val);
            if(result) return result;

        } else if (i == 64) {
            /* Byte 128 - Backward Indication Field */
            /* Byte 129 - Backward Operator Identifier Field */
            val = (*pBwdOpId << 8) | bwdIdFld;
            pBwdOpId++;

            result = hyPhy20gVarFieldWrite(&regData, ftflMsk, val);
            if(result) return result;

        } else if (i >= 65 && i <= 68) {
            /* Bytes 130 to 137 - Backward Operator Identifier Field */
            val = *pBwdOpId;
            pBwdOpId++;
            val |= *pBwdOpId << 8;
            pBwdOpId++;

            result = hyPhy20gVarFieldWrite(&regData, ftflMsk, val);
            if(result) return result;

        } else {
            /* Bytes 138 to 255 - Forward Operator-specific Field */
            val = *pBwdOpSp;
            pBwdOpSp++;
            val |= *pBwdOpSp << 8;
            pBwdOpSp++;

            result = hyPhy20gVarFieldWrite(&regData, ftflMsk, val);
            if(result) return result;
        }

        /* Write register */
        result = sysHyPhy20gWrite(fileHndl, ftflAddr, regData);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukTxFtflMsgCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxApsPccSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the APS_PCC message source for
**                  a multiframe of the transmit data stream.
**
**                  When configured for 'Incoming APS_PCC value' the value
**                  transferred on APS_PCC will be from highest to
**                  lowest priority:
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**                  When configured for 'Force APS_PCC message value' only one
**                  multiframe can contain the forceVal.  Should a multiframe
**                  have been set to force the APS_PCC value for the multiframe
**                  and a new multiframe is selected, the previous multiframe
**                  would revert back to transmitting source 0 or 1 (depending
**                  upon which was previously configured or source == 0 if the
**                  multiframe had not been configured for either of these
**                  modes)
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
**                  mFrame      - multiframe number to configure
**                                0 - 7
**                  source      - APS_PCC message source
**                                0 - Incoming APS_PCC value
**                                1 - Overhead RAM
**                                2 - Force APS_PCC message value
**                  forceVal    - For source == 2 (otherwise set to 0)
**                                The four byte value to be inserted in the
**                                specified mFrame of the outgoing data stream
**                              - Legal range 0x0 to 0xFFFFFFFF
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxApsPccSrcCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 mFrame, UINT4 source, 
                                      UINT4 forceVal)
{
    INT4 result;
    UINT4 insMfasAddr;
    UINT4 insMfasOff = 0;
    UINT4 oduCtlAddr;
    UINT4 insOff, insMsk;
    UINT4 mfasSelMsk;
    UINT4 hiValAddr, loValAddr;
    UINT4 hiValMsk, loValMsk;
    UINT4 regData, val;
    UINT4 subSlcOff;

    /* Argument checking */
    if ((slice > 9) || (mFrame > 7) || (source > 2) ||
        (forceVal > 0xFFFFFFFF) || (source != 2 && forceVal != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register address */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        insMfasAddr = HYPHY20G_ODUKPP_TOP_REG_OTU2_APS_PCC_MFAS_INS_CTL;

        oduCtlAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_CTL;
        oduCtlAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(oduCtlAddr,
                                                                   subSlcOff);

        hiValAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_APS_PCC_HI_VAL;
        hiValAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(hiValAddr,
                                                                  subSlcOff);

        loValAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_APS_PCC_LOW_VAL;
        loValAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(loValAddr,
                                                                  subSlcOff);

        switch (subSlcOff) {
            case 0:
                insMfasOff = 
   HYPHY20G_ODUKPP_TOP_BIT_OTU2_APS_PCC_MFAS_INS_CTL_OTU2_APS_PCC_INS_MFAS0_OFF;
                break;
            case 1:
                insMfasOff = 
   HYPHY20G_ODUKPP_TOP_BIT_OTU2_APS_PCC_MFAS_INS_CTL_OTU2_APS_PCC_INS_MFAS1_OFF;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }

        insOff = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_INSERT_APS_PCC_OFF;
        insMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_INSERT_APS_PCC_MSK;
        mfasSelMsk = 
            HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_APS_PCC_MFAS_SELECT_MSK;

        hiValMsk = 
  HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_APS_PCC_HI_VAL_APS_PCC_HIGH_VALUE_MSK;
        loValMsk = 
  HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_APS_PCC_LOW_VAL_APS_PCC_LOW_VALUE_MSK;

    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        if (subSlcOff < 4) {
            insMfasAddr = HYPHY20G_ODUKPP_TOP_REG_OTU1_APS_PCC_MFAS_INS_CTL_0;
        } else {
            insMfasAddr = HYPHY20G_ODUKPP_TOP_REG_OTU1_APS_PCC_MFAS_INS_CTL_1;
        }

        oduCtlAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_CTL;
        oduCtlAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(oduCtlAddr,
                                                                   subSlcOff);

        hiValAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_APS_PCC_HI_VAL;
        hiValAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(hiValAddr,
                                                                  subSlcOff);

        loValAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_APS_PCC_LOW_VAL;
        loValAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(loValAddr,
                                                                  subSlcOff);

        switch (subSlcOff) {
            case 0:
                insMfasOff = HYPHY20G_ODUKPP_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CTL_0_OTU1_APS_PCC_INS_MFAS0_OFF;
                break;
            case 1:
                insMfasOff = HYPHY20G_ODUKPP_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CTL_0_OTU1_APS_PCC_INS_MFAS1_OFF;
                break;
            case 2:
                insMfasOff = HYPHY20G_ODUKPP_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CTL_0_OTU1_APS_PCC_INS_MFAS2_OFF;
                break;
            case 3:
                insMfasOff = HYPHY20G_ODUKPP_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CTL_0_OTU1_APS_PCC_INS_MFAS3_OFF;
                break;
            case 4:
                insMfasOff = HYPHY20G_ODUKPP_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CTL_1_OTU1_APS_PCC_INS_MFAS4_OFF;
                break;
            case 5:
                insMfasOff = HYPHY20G_ODUKPP_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CTL_1_OTU1_APS_PCC_INS_MFAS5_OFF;
                break;
            case 6:
                insMfasOff = HYPHY20G_ODUKPP_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CTL_1_OTU1_APS_PCC_INS_MFAS6_OFF;
                break;
            case 7:
                insMfasOff = HYPHY20G_ODUKPP_TOP_BIT_OTU1_APS_PCC_MFAS_INS_CTL_1_OTU1_APS_PCC_INS_MFAS7_OFF;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }

        insOff = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_INSERT_APS_PCC_OFF;
        insMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_INSERT_APS_PCC_MSK;
        mfasSelMsk = 
            HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_APS_PCC_MFAS_SELECT_MSK;

        hiValMsk = 
  HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_APS_PCC_HI_VAL_APS_PCC_HIGH_VALUE_MSK;
        loValMsk = 
  HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_APS_PCC_LOW_VAL_APS_PCC_LOW_VALUE_MSK;
    }

    switch (source) {
        case 0:
            /* Incoming APS_PCC value */

            /* Clear OTUk_APS_PCC_INS_MFASn */
            result = sysHyPhy20gBitWrite(fileHndl, insMfasAddr,
                                         insMfasOff + mFrame, 0);
            if(result) return result;

            /* Clear INSERT_APS_PCC */
            result = sysHyPhy20gBitWrite(fileHndl, oduCtlAddr, insOff, 0);
            if(result) return result;
            break;
        case 1:
            /* Overhead RAM */

            /* Clear INSERT_APS_PCC */
            result = sysHyPhy20gBitWrite(fileHndl, oduCtlAddr, insOff, 0);
            if(result) return result;

            /* Set OTUk_APS_PCC_INS_MFASn */
            result = sysHyPhy20gBitWrite(fileHndl, insMfasAddr,
                                         insMfasOff + mFrame, 1);
            if(result) return result;
            break;
        case 2:
            /* Force APS_PCC message value */

            /* Clear OTUk_APS_PCC_INS_MFASn */
            result = sysHyPhy20gBitWrite(fileHndl, insMfasAddr,
                                         insMfasOff + mFrame, 0);
            if(result) return result;

            /* Configure forced APS_PCC message */
            val = (forceVal >> 16) & 0xFFFF;
            result = hyPhy20gRegFieldWrite(fileHndl, hiValAddr, hiValMsk, val);
            if(result) return result;

            val = forceVal & 0xFFFF;
            result = hyPhy20gRegFieldWrite(fileHndl, loValAddr, loValMsk, val);
            if(result) return result;

            /* Read ODU control register */
            result = sysHyPhy20gRead(fileHndl, oduCtlAddr, &regData);
            if(result) return result;

            /* Set INSERT_APS_PCC */
            result = hyPhy20gVarFieldWrite(&regData, insMsk, 1);
            if(result) return result;

            /* Configure APS_PCC_MFAS_SELECT */
            result = hyPhy20gVarFieldWrite(&regData, mfasSelMsk, mFrame);
            if(result) return result;

            /* Write ODU control register */
            result = sysHyPhy20gWrite(fileHndl, oduCtlAddr, regData);
            if(result) return result;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukTxApsPccSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxRes1SrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the first set of ODUk RES bytes
**                  for the transmit data stream.  RES1 corresponds to the
**                  bytes in Column 1, 2 and 3 Row 2 of ODUk overhead.
**
**                  When configured for 'Incoming RES1 value' the value
**                  transferred on RES1 bytes will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTUk Framer instance:
**                               0 - OTU1 Framer 0
**                               1 - OTU1 Framer 1
**                               2 - OTU1 Framer 2
**                               3 - OTU1 Framer 3
**                               4 - OTU1 Framer 4
**                               5 - OTU1 Framer 5
**                               6 - OTU1 Framer 6
**                               7 - OTU1 Framer 7
**                               8 - OTU2 Framer 0
**                               9 - OTU2 Framer 1
**                  source     - RES1 source
**                               0 - Incoming RES1 value
**                               1 - Force RES1 message value
**                  forceVal   - For source == 1 (otherwise set to 0)
**                               The value to be inserted into RES1 in the
**                               transmit data stream
**                             - Legal range 0 to 0xFFFFFF
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxRes1SrcCfg(struct file *fileHndl, UINT4 slice, 
                                    UINT4 source, UINT4 forceVal)
{
    INT4 result;
    UINT4 ctlAddr;
    UINT4 resHiAddr, resLoAddr;
    UINT4 val;
    UINT4 subSlcOff;
    UINT4 resHiMsk, resLoMsk;
    UINT4 insMsk;

    /* Argument checking */
    if ((slice > 9) || (source > 1) || (forceVal > 0xFFFFFF) ||
        (source != 1 && forceVal != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_CTL;
        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, subSlcOff);

        resHiAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_RES1_HI_VAL;
        resHiAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(resHiAddr, subSlcOff);

        resLoAddr = 
                   HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_RES1_LOW_TCM_ACT_VAL;
        resLoAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(resLoAddr, subSlcOff);

        insMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_INSERT_RES1_MSK;

        resHiMsk = 
        HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_RES1_HI_VAL_RES1_HIGH_VALUE_MSK;
        resLoMsk = 
HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_RES1_LOW_TCM_ACT_VAL_RES1_LOW_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_CTL;
        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, subSlcOff);
                                                                
        resHiAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_RES1_HI_VAL;
        resHiAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(resHiAddr, subSlcOff);
                                                                  
        resLoAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_RES1_LOW_TCM_ACT_VAL;
        resLoAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(resLoAddr, subSlcOff);
                                                                  

        insMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_INSERT_RES1_MSK;

        resHiMsk = 
        HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_RES1_HI_VAL_RES1_HIGH_VALUE_MSK;
        resLoMsk = 
HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_RES1_LOW_TCM_ACT_VAL_RES1_LOW_VALUE_MSK;
    }

    if (source == 0) {
        /* Clear INSERT_RES1 */
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, insMsk, 0);
        if(result) return result;

    } else {
        /* Configure RES1_HIGH_VALUE */
        val = (forceVal >> 8) & 0xFFFF;
        result = hyPhy20gRegFieldWrite(fileHndl, resHiAddr, resHiMsk, val);
        if(result) return result;

        /* Configure RES1_LOW_VALUE */
        val = forceVal & 0xFF;
        result = hyPhy20gRegFieldWrite(fileHndl, resLoAddr, resLoMsk, val);
        if(result) return result;

        /* Set INSERT_RES1 */
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, insMsk, 1);
        if(result) return result;

    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukTxRes1SrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxRes2SrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the RES2 bytes for the transmit
**                  data stream.
**
**                  RES2 corresponds to the bytes in Column 9 to 14 Row 4 of
**                  ODUk overhead.
**
**                  When configured for 'Incoming RES2 value' the value
**                  transferred on RES1 bytes will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTUk Framer instance:
**                               0 - OTU1 Framer 0
**                               1 - OTU1 Framer 1
**                               2 - OTU1 Framer 2
**                               3 - OTU1 Framer 3
**                               4 - OTU1 Framer 4
**                               5 - OTU1 Framer 5
**                               6 - OTU1 Framer 6
**                               7 - OTU1 Framer 7
**                               8 - OTU2 Framer 0
**                               9 - OTU2 Framer 1
**                  source     - RES2 source
**                               0 - Incoming RES2 value
**                               1 - Force RES2 message value
**                  res2Hi     - For source == 1 (otherwise set to 0)
**                               The value to be inserted into ODUk RES2
**                               bytes 5:4
**                  res2Low    - For source == 1 (otherwise set to 0)
**                               The value to be inserted into ODUk RES2
**                               bytes 3:0
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxRes2SrcCfg(struct file *fileHndl, UINT4 slice, 
                                    UINT4 source, UINT4 res2Hi, UINT4 res2Low)
{
    INT4 result;
    UINT4 ctlAddr;
    UINT4 resHiAddr, resMidAddr, resLoAddr;
    UINT4 val;
    UINT4 subSlcOff;
    UINT4 resHiMsk, resMidMsk, resLoMsk;
    UINT4 insMsk;

    /* Argument checking */
    if ((slice > 9) || (source > 1) || (res2Hi > 0xFFFF) ||
        (res2Low > 0xFFFFFFFF) ||
        (source != 1 && res2Hi != 0 && res2Low != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_CTL;
        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, 
                                                                subSlcOff);

        resHiAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_RES2_HI_VAL;
        resHiAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(resHiAddr,
                                                                  subSlcOff);

        resMidAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_RES2_MID_VAL;
        resMidAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(resMidAddr,
                                                                   subSlcOff);

        resLoAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_RES2_LOW_VAL;
        resLoAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(resLoAddr,
                                                                  subSlcOff);

        insMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_INSERT_RES2_MSK;

        resHiMsk = 
        HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_RES2_HI_VAL_RES2_HIGH_VALUE_MSK;
        resMidMsk = 
        HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_RES2_MID_VAL_RES2_MID_VALUE_MSK;
        resLoMsk = 
        HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_RES2_LOW_VAL_RES2_LOW_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_CTL;
        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, 
                                                                subSlcOff);

        resHiAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_RES2_HI_VAL;
        resHiAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(resHiAddr,
                                                                  subSlcOff);

        resMidAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_RES2_MID_VAL;
        resMidAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(resMidAddr,
                                                                   subSlcOff);

        resLoAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_RES2_LOW_VAL;
        resLoAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(resLoAddr,
                                                                  subSlcOff);

        insMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_INSERT_RES2_MSK;

        resHiMsk = 
        HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_RES2_HI_VAL_RES2_HIGH_VALUE_MSK;
        resMidMsk = 
        HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_RES2_MID_VAL_RES2_MID_VALUE_MSK;
        resLoMsk = 
        HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_RES2_LOW_VAL_RES2_LOW_VALUE_MSK;
    }

    if (source == 0) {
        /* Clear INSERT_RES2 */
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, insMsk, 0);
        if(result) return result;

    } else {
        /* Configure RES2_HIGH_VALUE */
        val = res2Hi;
        result = hyPhy20gRegFieldWrite(fileHndl, resHiAddr, resHiMsk, val);
        if(result) return result;

        /* Configure RES2_MID_VALUE */
        val = (res2Low >> 16) & 0xFFFF;
        result = hyPhy20gRegFieldWrite(fileHndl, resMidAddr, resMidMsk, val);
        if(result) return result;

        /* Configure RES1_LOW_VALUE */
        val = res2Low & 0xFFFF;
        result = hyPhy20gRegFieldWrite(fileHndl, resLoAddr, resLoMsk, val);
        if(result) return result;

        /* Set INSERT_RES2 */
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, insMsk, 1);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukTxRes2SrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxGcc1SrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the ODUk GCC1 bytes for the
**                  transmit data stream.
**
**                  When configured for 'Incoming GCC1 value' the value
**                  transferred on GCC1 bytes will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTUk Framer instance:
**                               0 - OTU1 Framer 0
**                               1 - OTU1 Framer 1
**                               2 - OTU1 Framer 2
**                               3 - OTU1 Framer 3
**                               4 - OTU1 Framer 4
**                               5 - OTU1 Framer 5
**                               6 - OTU1 Framer 6
**                               7 - OTU1 Framer 7
**                               8 - OTU2 Framer 0
**                               9 - OTU2 Framer 1
**                  source     - GCC1 source
**                               0 - Incoming GCC1 value
**                               1 - Force GCC1 message value
**                  forceVal   - For source == 1 (otherwise set to 0)
**                               The value to be inserted into GCC1 in the
**                               transmit data stream
**                             - Legal range 0 to 0xFFFF
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxGcc1SrcCfg(struct file *fileHndl, UINT4 slice, 
                                    UINT4 source, UINT4 forceVal)
{
    INT4 result;
    UINT4 ctlAddr;
    UINT4 gccAddr;
    UINT4 subSlcOff;
    UINT4 gccMsk;
    UINT4 insMsk;

    /* Argument checking */
    if ((slice > 9) || (source > 1) || (forceVal > 0xFFFF) ||
        (source != 1 && forceVal != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_CTL;
        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, 
                                                                subSlcOff);

        gccAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_GCC1_VAL;
        gccAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(gccAddr, 
                                                                subSlcOff);

        insMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_INSERT_GCC1_MSK;
        gccMsk = 
                HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_GCC1_VAL_GCC1_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_CTL;
        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, 
                                                                subSlcOff);

        gccAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_GCC1_VAL;
        gccAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(gccAddr, 
                                                                subSlcOff);

        insMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_INSERT_GCC1_MSK;
        gccMsk = 
                HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_GCC1_VAL_GCC1_VALUE_MSK;
    }

    if (source == 0) {
        /* Clear INSERT_GCC1 */
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, insMsk, 0);
        if(result) return result;

    } else {
        /* Configure GCC1_VALUE */
        result = hyPhy20gRegFieldWrite(fileHndl, gccAddr, gccMsk, forceVal);
        if(result) return result;

        /* Set INSERT_GCC1 */
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, insMsk, 1);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukTxGcc1SrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxGcc2SrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the ODUk GCC2 bytes for the
**                  transmit data stream.
**
**                  When configured for 'Incoming GCC2 value' the value
**                  transferred on GCC2 bytes will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTUk Framer instance:
**                               0 - OTU1 Framer 0
**                               1 - OTU1 Framer 1
**                               2 - OTU1 Framer 2
**                               3 - OTU1 Framer 3
**                               4 - OTU1 Framer 4
**                               5 - OTU1 Framer 5
**                               6 - OTU1 Framer 6
**                               7 - OTU1 Framer 7
**                               8 - OTU2 Framer 0
**                               9 - OTU2 Framer 1
**                  source     - GCC2 source
**                               0 - Incoming GCC2 value
**                               1 - Force GCC2 message value
**                  forceVal   - For source == 1 (otherwise set to 0)
**                               The value to be inserted into GCC2 in the
**                               transmit data stream
**                             - Legal range 0 to 0xFFFF
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxGcc2SrcCfg(struct file *fileHndl, UINT4 slice, 
                                    UINT4 source, UINT4 forceVal)
{
    INT4 result;
    UINT4 ctlAddr;
    UINT4 gccAddr;
    UINT4 subSlcOff;
    UINT4 gccMsk;
    UINT4 insMsk;

    /* Argument checking */
    if ((slice > 9) || (source > 1) || (forceVal > 0xFFFF) ||
        (source != 1 && forceVal != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_CTL;
        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, 
                                                                subSlcOff);

        gccAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_GCC2_VAL;
        gccAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(gccAddr, 
                                                                subSlcOff);

        insMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_INSERT_GCC2_MSK;
        gccMsk = 
                HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_GCC2_VAL_GCC2_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_CTL;
        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, 
                                                                subSlcOff);

        gccAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_GCC2_VAL;
        gccAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(gccAddr, 
                                                                subSlcOff);

        insMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_INSERT_GCC2_MSK;
        gccMsk = 
                HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_GCC2_VAL_GCC2_VALUE_MSK;
    }

    if (source == 0) {
        /* Clear INSERT_GCC2 */
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, insMsk, 0);
        if(result) return result;

    } else {
        /* Configure GCC2_VALUE */
        result = hyPhy20gRegFieldWrite(fileHndl, gccAddr, gccMsk, forceVal);
        if(result) return result;

        /* Set INSERT_GCC2 */
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, insMsk, 1);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukTxGcc2SrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxTcmActSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the ODUk TCM ACT byte for the
**                  transmit data stream.
**
**                  When configured for 'Incoming TCM ACT value' the value
**                  transferred on TCM ACT byte will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTUk Framer instance:
**                               0 - OTU1 Framer 0
**                               1 - OTU1 Framer 1
**                               2 - OTU1 Framer 2
**                               3 - OTU1 Framer 3
**                               4 - OTU1 Framer 4
**                               5 - OTU1 Framer 5
**                               6 - OTU1 Framer 6
**                               7 - OTU1 Framer 7
**                               8 - OTU2 Framer 0
**                               9 - OTU2 Framer 1
**                  source     - TCM ACT source
**                               0 - Incoming TCM ACT value
**                               1 - Force TCM ACT message value
**                  forceVal   - For source == 1 (otherwise set to 0)
**                               The value to be inserted into TCM ACT in the
**                               transmit data stream
**                             - Legal range 0 to 0xFF
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxTcmActSrcCfg(struct file *fileHndl, UINT4 slice,
                                      UINT4 source, UINT4 forceVal)
{
    INT4 result;
    UINT4 ctlAddr;
    UINT4 tcmActAddr;
    UINT4 subSlcOff;
    UINT4 tcmActMsk;
    UINT4 insMsk;

    /* Argument checking */
    if ((slice > 9) || (source > 1) || (forceVal > 0xFF) ||
        (source != 1 && forceVal != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_CTL;
        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, 
                                                                subSlcOff);

        tcmActAddr = 
                   HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_RES1_LOW_TCM_ACT_VAL;
        tcmActAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(tcmActAddr,
                                                                   subSlcOff);

        insMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_INSERT_TCM_ACT_MSK;
        tcmActMsk = 
 HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_RES1_LOW_TCM_ACT_VAL_TCM_ACT_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_CTL;
        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, 
                                                                subSlcOff);

        tcmActAddr = 
                   HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_RES1_LOW_TCM_ACT_VAL;
        tcmActAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(tcmActAddr,
                                                                   subSlcOff);

        insMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_INSERT_TCM_ACT_MSK;
        tcmActMsk = 
 HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_RES1_LOW_TCM_ACT_VAL_TCM_ACT_VALUE_MSK;
    }

    if (source == 0) {
        /* Clear INSERT_TCM_ACT */
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, insMsk, 0);
        if(result) return result;

    } else {
        /* Configure TCM_ACT_VALUE */
        result = hyPhy20gRegFieldWrite(fileHndl, tcmActAddr, tcmActMsk,
                                       forceVal);
        if(result) return result;

        /* Set INSERT_TCM_ACT */
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, insMsk, 1);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukTxTcmActSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxExpSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the ODUk EXP bytes for the
**                  transmit data stream.
**
**                  When configured for 'Incoming EXP value' the value
**                  transferred on EXP bytes will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTUk Framer instance:
**                               0 - OTU1 Framer 0
**                               1 - OTU1 Framer 1
**                               2 - OTU1 Framer 2
**                               3 - OTU1 Framer 3
**                               4 - OTU1 Framer 4
**                               5 - OTU1 Framer 5
**                               6 - OTU1 Framer 6
**                               7 - OTU1 Framer 7
**                               8 - OTU2 Framer 0
**                               9 - OTU2 Framer 1
**                  source     - EXP source
**                               0 - Incoming EXP value
**                               1 - Force EXP message value
**                  forceVal   - For source == 1 (otherwise set to 0)
**                               The value to be inserted into EXP in the
**                               transmit data stream
**                             - Legal range 0 to 0xFFFF
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxExpSrcCfg(struct file *fileHndl, UINT4 slice, 
                                   UINT4 source, UINT4 forceVal)
{
    INT4 result;
    UINT4 ctlAddr;
    UINT4 expAddr;
    UINT4 subSlcOff;
    UINT4 expMsk;
    UINT4 insMsk;

    /* Argument checking */
    if ((slice > 9) || (source > 1) || (forceVal > 0xFFFF) ||
        (source != 1 && forceVal != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_CTL;
        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, 
                                                                subSlcOff);

        expAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_EXP_VAL;
        expAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(expAddr, 
                                                                subSlcOff);

        insMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_INSERT_EXP_MSK;
        expMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_EXP_VAL_EXP_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_CTL;
        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, 
                                                                subSlcOff);

        expAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_EXP_VAL;
        expAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(expAddr, 
                                                                subSlcOff);

        insMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_INSERT_EXP_MSK;
        expMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_EXP_VAL_EXP_VALUE_MSK;
    }

    if (source == 0) {
        /* Clear INSERT_EXP */
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, insMsk, 0);
        if(result) return result;

    } else {
        /* Configure EXP_VALUE */
        result = hyPhy20gRegFieldWrite(fileHndl, expAddr, expMsk, forceVal);
        if(result) return result;

        /* Set INSERT_EXP */
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, insMsk, 1);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdukTxExpSrcCfg */


/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxPmTcmiTtiExpCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the expected receive PM or TCMi TTI
**                  message.
**
**                  The function takes in the 16 byte Source Access Point
**                  Identifier (SAPI), the 16 byte Destination Access Point
**                  Identifier (DAPI), and the 32 byte Operator Specific fields
**                  as arguments to fill the 64 byte TTI.
**
**                  The range of a SAPI or DAPI characters 15 to 1 are 0 to 127.
**                  SAPI and DAPI character 0 must be set to 0x00.  Function
**                  will return HYPHY20G_ERR_INVALID_ARG if these rules are not
**                  adhered.
**
**                  There are no restrictions on the values placed in Operator
**                  Specific bytes.
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
**                  pmTcmNum    - PM or TCMi field instance:
**                                0 - PM
**                                1 to 6 - TCM1 to TCM6
**                  sapi[16]    - SAPI characters (16 bytes)
**                  dapi[16]    - DAPI characters (16 bytes)
**                  opSpec[32]  - Operator Specific (32 bytes)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxPmTcmiTtiExpCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 pmTcmNum, UINT4 sapi[16],
                                         UINT4 dapi[16], UINT4 opSpec[32])
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr;
    UINT4 ttiMsk;
    UINT4 regData;
    UINT4 val;
    UINT4 i, j;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (pmTcmNum > 6) || (sapi[0] > 0) || (dapi[0] > 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < 32; i++) {
        if ((i < 16) && (sapi[i] > 127 || dapi[i] > 127)) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        if (opSpec[i] > 255) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    /* Configure expected receive PM/TCMi TTI message */
    j = 0;
    for (i = 0; i < 32; i++) {
        /* Determine register address */
        sliceRate = slice < 8 ? 1:0;
        switch (pmTcmNum + sliceRate*7) {
            case 0: /* ODU2 Rate PM TTI */
                addr = 
               HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_PM_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = 
 HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_PM_TTI_EXP_MEM_PM_TTI_EXPECT_VALUE_MSK;
                break;
            case 1: /* ODU2 Rate TCM1 TTI */
                addr = 
             HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCM1_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCM1_TTI_EXP_MEM_TCM1_TTI_EXPECT_VALUE_MSK;
                break;
            case 2: /* ODU2 Rate TCM2 TTI */
                addr = 
             HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCM2_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCM2_TTI_EXP_MEM_TCM2_TTI_EXPECT_VALUE_MSK;
                break;
            case 3: /* ODU2 Rate TCM3 TTI */
                addr = 
             HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCM3_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCM3_TTI_EXP_MEM_TCM3_TTI_EXPECT_VALUE_MSK;
                break;
            case 4: /* ODU2 Rate TCM4 TTI */
                addr = 
             HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCM4_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCM4_TTI_EXP_MEM_TCM4_TTI_EXPECT_VALUE_MSK;
                break;
            case 5: /* ODU2 Rate TCM5 TTI */
                addr = 
             HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCM5_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCM5_TTI_EXP_MEM_TCM5_TTI_EXPECT_VALUE_MSK;
                break;
            case 6: /* ODU2 Rate TCM6 TTI */
                addr = 
             HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCM6_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCM6_TTI_EXP_MEM_TCM6_TTI_EXPECT_VALUE_MSK;
                break;
            case 7: /* ODU1 Rate PM TTI */
                addr = 
               HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_PM_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = 
 HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_PM_TTI_EXP_MEM_PM_TTI_EXPECT_VALUE_MSK;
                break;
            case 8: /* ODU1 Rate TCM1 TTI */
                addr = 
             HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCM1_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCM1_TTI_EXP_MEM_TCM1_TTI_EXPECT_VALUE_MSK;
                break;
            case 9: /* ODU1 Rate TCM2 TTI */
                addr = 
             HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCM2_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCM2_TTI_EXP_MEM_TCM2_TTI_EXPECT_VALUE_MSK;
                break;
            case 10: /* ODU1 Rate TCM3 TTI */
                addr = 
             HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCM3_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCM3_TTI_EXP_MEM_TCM3_TTI_EXPECT_VALUE_MSK;
                break;
            case 11: /* ODU1 Rate TCM4 TTI */
                addr = 
             HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCM4_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCM4_TTI_EXP_MEM_TCM4_TTI_EXPECT_VALUE_MSK;
                break;
            case 12: /* ODU1 Rate TCM5 TTI */
                addr = 
             HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCM5_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCM5_TTI_EXP_MEM_TCM5_TTI_EXPECT_VALUE_MSK;
                break;
            case 13: /* ODU1 Rate TCM6 TTI */
                addr = 
             HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCM6_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCM6_TTI_EXP_MEM_TCM6_TTI_EXPECT_VALUE_MSK;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        if(slice > 7) {
            subSlcOff = slice - 8;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, 
                                                                 subSlcOff);
        } else {
            subSlcOff = slice;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, 
                                                                 subSlcOff);
        }

        /* Read register */
        result = sysHyPhy20gRead(fileHndl, addr, &regData);
        if(result) return result;

        /* Modify read data */
        j = i*2;
        if (i < 8) {
            /* Configure SAPI (2 bytes per register) */
            val = (sapi[j+1] << 8) | sapi[j];
        } else if (i < 16) {
            /* Configure DAPI (2 bytes per register) */
            j = j - 16;
            val = (dapi[j+1] << 8) | dapi[j];
        } else {
            /* Configure Operator Specific (2 bytes per register) */
            j = j - 32;
            val = (opSpec[j+1] << 8) | opSpec[j];
        }
        result = hyPhy20gVarFieldWrite(&regData, ttiMsk, val);
        if(result) return result;

        /* Write data to register */
        result = sysHyPhy20gWrite(fileHndl, addr, regData);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxPmTcmiTtiExpCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxPmTcmiTtiProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the PM or TCMi TTI message.  It configures how the TTI
**                  message is monitored and which fields of the message
**                  contribute to TIM defect detection.
**
**                  hyPhy20gOdukppOdukRxPmTcmiTtiExpCfg must be called to 
**                  configure the expected TTI message in order for the 
**                  processing performed by these feature to be meaningful.
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
**                  pmTcmNum    - PM or TCMi field instance:
**                                0 - PM
**                                1 to 6 - TCM1 to TCM6
**                  acptCnt     - Number of identical PM/TCMi TTI messages that
**                                the RX framer needs to obtain before declaring
**                                the TTI as accepted.
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical PM/TCMi TTI messages
**                                that the RX framer needs to obtain before
**                                triggering a TTI inconsistent error interrupt.
**                              - Range 1 to 7
**                              - Default value 0x5
**                  cmpSapi     - Controls whether or not the RX framer uses the
**                                SAPI field in the accepted PM/TCMi TTI to
**                                compare against the expected TTI to detect a
**                                TIM defect
**                              - 1: PM/TCMi SAPI is used for comparison for TIM
**                                   detection.
**                              - 0: PM/TCMi SAPI is not used for comparison for
**                                   TIM detection.
**                  cmpDapi     - Controls whether or not the RX framer uses the
**                                DAPI field in the accepted PM/TCMi TTI to
**                                compare against the expected TTI to detect a
**                                TIM defect
**                              - 1: PM/TCMi DAPI is used for comparison for TIM
**                                   detection.
**                              - 0: PM/TCMi DAPI is not used for comparison for
**                                   TIM detection.
**                  cmpOs       - Controls whether or not the RX framer uses the
**                                Opearator Specific (OS) field in the accepted
**                                PM/TCMi TTI to compare against the expected
**                                TTI to detect a TIM defect
**                              - 1: PM/TCMi OS is used for comparison for TIM
**                                   detection.
**                              - 0: PM/TCMi OS is not used for comparison for
**                                   TIM detection.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxPmTcmiTtiProcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 pmTcmNum, UINT4 acptCnt,
                                          UINT4 inconCnt, UINT4 cmpSapi,
                                          UINT4 cmpDapi, UINT4 cmpOs)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr;
    UINT4 regData;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;
    UINT4 cmpSapiMsk;
    UINT4 cmpDapiMsk;
    UINT4 cmpOsMsk;
    INT4  result;

    /* Argument check */
    if ((slice > 9) || (pmTcmNum > 6) || (acptCnt < 1) || (acptCnt > 7) ||
        (inconCnt < 1) || (inconCnt > 7) || (cmpSapi > 1) || (cmpDapi > 1) ||
        (cmpOs > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register address */
    sliceRate = slice < 8 ? 1:0;
    switch (pmTcmNum + sliceRate*7) {
        case 0: /* ODU2 Rate PM TTI */
            subSlcOff = slice-8;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_PM_CTL;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, 
                                                                 subSlcOff);
            cmpSapiMsk = 
             HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_TTI_SAPI_TIM_MSK;
            cmpDapiMsk = 
             HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_TTI_DAPI_TIM_MSK;
            cmpOsMsk = 
               HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_TTI_OS_TIM_MSK;
            acptCntMsk = 
         HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_TTI_ACCEPT_COUNT_MSK;
            inconCntMsk = 
   HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_TTI_INCONSISTENT_COUNT_MSK;
            break;
        case 1: /* ODU2 Rate TCM1 TTI */
        case 2: /* ODU2 Rate TCM2 TTI */
        case 3: /* ODU2 Rate TCM3 TTI */
        case 4: /* ODU2 Rate TCM4 TTI */
        case 5: /* ODU2 Rate TCM5 TTI */
        case 6: /* ODU2 Rate TCM6 TTI */
            subSlcOff = slice-8;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_CTL_OFFSET(
                                                                    pmTcmNum-1);
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, 
                                                                 subSlcOff);
            cmpSapiMsk = 
      HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_TTI_SAPI_TIM_MSK;
            cmpDapiMsk = 
      HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_TTI_DAPI_TIM_MSK;
            cmpOsMsk = 
         HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_CTL_TCMI_TTI_OS_TIM_2_MSK;
            acptCntMsk = 
  HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_TTI_ACCEPT_COUNT_MSK;
            inconCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_TTI_INCONSISTENT_COUNT_MSK;
            break;
        case 7: /* ODU1 Rate PM TTI */
            subSlcOff = slice;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_PM_CTL;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, 
                                                                 subSlcOff);
            cmpSapiMsk = 
             HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_TTI_SAPI_TIM_MSK;
            cmpDapiMsk = 
             HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_TTI_DAPI_TIM_MSK;
            cmpOsMsk = 
               HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_TTI_OS_TIM_MSK;
            acptCntMsk = 
         HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_TTI_ACCEPT_COUNT_MSK;
            inconCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_TTI_INCONSISTENT_COUNT_MSK;
            break;
        case 8: /* ODU1 Rate TCM1 TTI */
        case 9: /* ODU1 Rate TCM2 TTI */
        case 10: /* ODU1 Rate TCM3 TTI */
        case 11: /* ODU1 Rate TCM4 TTI */
        case 12: /* ODU1 Rate TCM5 TTI */
        case 13: /* ODU1 Rate TCM6 TTI */
            subSlcOff = slice;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_CTL_OFFSET(
                                                                    pmTcmNum-1);
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, 
                                                                 subSlcOff);
            cmpSapiMsk = 
      HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_TTI_SAPI_TIM_MSK;
            cmpDapiMsk = 
      HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_TTI_DAPI_TIM_MSK;
            cmpOsMsk = 
         HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_CTL_TCMI_TTI_OS_TIM_2_MSK;
            acptCntMsk = 
  HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_TTI_ACCEPT_COUNT_MSK;
            inconCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_TTI_INCONSISTENT_COUNT_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Read register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Configure x_TTI_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Configure x_TTI_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* Configure x_TTI_SAPI_TIM */
    result = hyPhy20gVarFieldWrite(&regData, cmpSapiMsk, cmpSapi);
    if(result) return result;

    /* Configure x_TTI_DAPI_TIM */
    result = hyPhy20gVarFieldWrite(&regData, cmpDapiMsk, cmpDapi);
    if(result) return result;

    /* Configure x_TTI_OS_TIM */
    result = hyPhy20gVarFieldWrite(&regData, cmpOsMsk, cmpOs);
    if(result) return result;

    /* Write data to register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxPmTcmiTtiProcCfg */


/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxPmTcmiTtiAcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive PM or TCMi TTI
**                  message.
**
**                  The function exptracts the 64 byte TTI relative to the 16
**                  byte Source Access Point Identifier (SAPI), the 16 byte
**                  Destination Access Point Identifier (DAPI), and the 32 byte
**                  Operator Specific fields.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - OTUk Framer instance:
**                                 0 - OTU1 Framer 0
**                                 1 - OTU1 Framer 1
**                                 2 - OTU1 Framer 2
**                                 3 - OTU1 Framer 3
**                                 4 - OTU1 Framer 4
**                                 5 - OTU1 Framer 5
**                                 6 - OTU1 Framer 6
**                                 7 - OTU1 Framer 7
**                                 8 - OTU2 Framer 0
**                                 9 - OTU2 Framer 1
**                  pmTcmNum     - PM or TCMi field instance:
**                                 0 - PM
**                                 1 to 6 - TCM1 to TCM6
**                  *pSapi[16]   - SAPI characters (16 bytes)
**                  *pDapi[16]   - DAPI characters (16 bytes)
**                  *pOpSpec[32] - Operator Specific (32 bytes)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxPmTcmiTtiAcptExt(struct file *fileHndl, UINT4 slice,
                                          UINT4 pmTcmNum, UINT4 *pSapi, 
                                          UINT4 *pDapi, UINT4 *pOpSpec)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr;
    UINT4 regData;
    UINT4 i;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (pmTcmNum > 6)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((pSapi == NULL) || (pDapi == NULL) || (pOpSpec == NULL)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Extract accepted receive PM / TCMi TTI message */
    for (i = 0; i < 32; i++) {

        /* Determine register address */
        sliceRate = slice < 8 ? 1:0;
        switch (pmTcmNum + sliceRate*7) {
            case 0: /* ODU2 Rate PM TTI */
                addr = 
               HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_PM_TTI_EXT_MEM_OFFSET(i);
                break;
            case 1: /* ODU2 Rate TCM1 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCM1_TTI_EXT_MEM_OFFSET(i);
                break;
            case 2: /* ODU2 Rate TCM2 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCM2_TTI_EXT_MEM_OFFSET(i);
                break;
            case 3: /* ODU2 Rate TCM3 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCM3_TTI_EXT_MEM_OFFSET(i);
                break;
            case 4: /* ODU2 Rate TCM4 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCM4_TTI_EXT_MEM_OFFSET(i);
                break;
            case 5: /* ODU2 Rate TCM5 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCM5_TTI_EXT_MEM_OFFSET(i);
                break;
            case 6: /* ODU2 Rate TCM6 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCM6_TTI_EXT_MEM_OFFSET(i);
                break;
            case 7: /* ODU1 Rate PM TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_PM_TTI_EXT_MEM_OFFSET(i);
                break;
            case 8: /* ODU1 Rate TCM1 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCM1_TTI_EXT_MEM_OFFSET(i);
                break;
            case 9: /* ODU1 Rate TCM2 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCM2_TTI_EXT_MEM_OFFSET(i);
                break;
            case 10: /* ODU1 Rate TCM3 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCM3_TTI_EXT_MEM_OFFSET(i);
                break;
            case 11: /* ODU1 Rate TCM4 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCM4_TTI_EXT_MEM_OFFSET(i);
                break;
            case 12: /* ODU1 Rate TCM5 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCM5_TTI_EXT_MEM_OFFSET(i);
                break;
            case 13: /* ODU1 Rate TCM6 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCM6_TTI_EXT_MEM_OFFSET(i);
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        if(slice > 7) {
            subSlcOff = slice - 8;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, 
                                                                 subSlcOff);
        } else {
            subSlcOff = slice;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, 
                                                                 subSlcOff);
        }

        /* Read register */
        result = sysHyPhy20gRead(fileHndl, addr, &regData);
        if(result) return result;

        /* Modify read data */
        if (i < 8) {
            /* Store SAPI (2 bytes per register) */
            *pSapi = regData & 0xFF;
            pSapi++;
            *pSapi = (regData >> 8) & 0xFF;
            pSapi++;
        } else if (i < 16) {
            /* Store DAPI (2 bytes per register) */
            *pDapi = regData & 0xFF;
            pDapi++;
            *pDapi = (regData >> 8) & 0xFF;
            pDapi++;
        } else {
            /* Store Operator Specific (2 bytes per register) */
            *pOpSpec = regData & 0xFF;
            pOpSpec++;
            *pOpSpec = (regData >> 8) & 0xFF;
            pOpSpec++;
        }
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxPmTcmiTtiAcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxTcmiBeiBipProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the accumulation mode of the 
**                  internal counters for transmit TCMi BEI/BIP errors for an 
**                  OTU2 or OTU1 framer slice.
**
**                  When configured to accumulate on a block basis, the counter
**                  is incremented by at most 1 for each frame that contain
**                  TCMi BEI/BIP errors. If it is configured to
**                  accumulate on a bit basis, then the counter is advanced by
**                  the number of BEI/BIP errors in the frame.
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
**                  accuMode    - Controls whether TCMi BEI/BIP error
**                                counts are accumulated on a block basis or a
**                                bit basis.
**                              - 1: errors are accumulated on a block basis.
**                              - 0: errors are accumulated on a bit basis.
**                                Set to 1 to conform to G.798
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxTcmiBeiBipProcCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 accuMode)
{
    UINT4 sliceSub;
    UINT4 reg, msk;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (accuMode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice TCM1 */
        case 1: /* OTU1 Slice TCM2 */
        case 2: /* OTU1 Slice TCM3 */
        case 3: /* OTU1 Slice TCM4 */
        case 4: /* OTU1 Slice TCM5 */
        case 5: /* OTU1 Slice TCM6 */
        case 6: /* OTU1 Slice TCM5 */
        case 7: /* OTU1 Slice TCM6 */
            sliceSub = slice;
            reg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_TCMI_INTRP_CNT_CTL;
            reg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(reg, sliceSub);
            msk = 
           HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_TCMI_INTRP_CNT_CTL_TX_BLOCK_MODE_MSK;
            break;
        case 8: /* OTU2 Slice 0*/
        case 9: /* OTU2 Slice 1*/
            sliceSub = slice - 8;
            reg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_TCMI_INTRP_CNT_CTL;
            reg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(reg, sliceSub);
            msk = 
           HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_TCMI_INTRP_CNT_CTL_TX_BLOCK_MODE_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure TX_BLOCK_MODE */
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, accuMode);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxTcmiBeiBipProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxPmTcmiBeiBiaeSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the PM BEI or the TCMi BEI/BIAE
**                  source for the transmit data stream.
**
**                  When configured for
**                  'Incoming PM BEI or the TCMi BEI/BIAE value' the value
**                  transferred on PM BEI or TCMi BEI/BIAE bits will be from
**                  highest to lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
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
**                  pmTcmNum    - PM or TCMi field instance:
**                                0 - PM
**                                1 to 6 - TCM1 to TCM6
**                  source      - PM BEI and TCMi BEI/BIAE source
**                                0 - Incoming PM BEI or TCMi BEI/BIAE value
**                                1 - BEI/BIAE value generated from receive data
**                                    stream
**                                2 - Force BEI/BIAE value
**                  forceVal   - For source == 2 (otherwise set to 0)
**                               The value to be inserted into PM BEI or TCMi
**                               BEI/BIAE bits in outgoing data stream
**                             - Legal range 0x0 to 0xF
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxPmTcmiBeiBiaeSrcCfg(struct file *fileHndl, UINT4 slice,
                                             UINT4 pmTcmNum, UINT4 source,
                                             UINT4 forceVal)
{
    UINT4 sliceSub, sliceRate;
    UINT4 ctlReg, autoMsk, insMsk;
    UINT4 valReg, valMsk, val;
    UINT4 insVal, autoVal, tcmiTransOff = 0;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (pmTcmNum > 6) || (source > 2) || (forceVal > 0xF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    sliceRate = slice < 8 ? 1:0;
    switch (pmTcmNum + sliceRate*7) {
        case 0: /* OTU2 Slice PM */
            sliceSub = slice - 8;
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_PM_CTL;
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlReg, 
                                                                   sliceSub);
            insMsk = 
               HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_CTL_INSERT_PM_BEI_MSK;
            autoMsk = 
                 HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_CTL_AUTO_PM_BEI_MSK;
            valReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_PM_VAL;
            valReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(valReg, 
                                                                   sliceSub);
            valMsk = 
                HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_VAL_PM_BEI_VALUE_MSK;
            break;
        case 1: /* OTU2 Slice TCM1 */
        case 2: /* OTU2 Slice TCM2 */
        case 3: /* OTU2 Slice TCM3 */
        case 4: /* OTU2 Slice TCM4 */
        case 5: /* OTU2 Slice TCM5 */
        case 6: /* OTU2 Slice TCM6 */
            sliceSub = slice - 8;
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_CTL_OFFSET(
                                                                    pmTcmNum-1);
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlReg, 
                                                                   sliceSub);
            insMsk = 
   HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_INSERT_TCMI_BEI_BIAE_MSK;
            autoMsk = 
     HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_AUTO_TCMI_BEI_BIAE_MSK;
            tcmiTransOff = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_TCMI_TRANSPARENT_OFF;
            valReg = 
            HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_VAL_OFFSET(pmTcmNum-1);
            valReg = 
                HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(valReg, sliceSub);
            valMsk = 
    HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_VAL_TX_TCMI_BEI_BIAE_VALUE_MSK;
            break;
        case 7: /* OTU1 Slice PM */
            sliceSub = slice;
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_PM_CTL;
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlReg, 
                                                                   sliceSub);
            insMsk = 
               HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_CTL_INSERT_PM_BEI_MSK;
            autoMsk = 
                 HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_CTL_AUTO_PM_BEI_MSK;
            valReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_PM_VAL;
            valReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(valReg, 
                                                                   sliceSub);
            valMsk = 
                HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_VAL_PM_BEI_VALUE_MSK;
            break;
        case 8: /* OTU1 Slice TCM1 */
        case 9: /* OTU1 Slice TCM2 */
        case 10: /* OTU1 Slice TCM3 */
        case 11: /* OTU1 Slice TCM4 */
        case 12: /* OTU1 Slice TCM5 */
        case 13: /* OTU1 Slice TCM6 */
            sliceSub = slice;
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_CTL_OFFSET(
                                                                    pmTcmNum-1);
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlReg, 
                                                                   sliceSub);
            insMsk = 
   HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_INSERT_TCMI_BEI_BIAE_MSK;
            autoMsk = 
     HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_AUTO_TCMI_BEI_BIAE_MSK;
            tcmiTransOff = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_TCMI_TRANSPARENT_OFF;
            valReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_VAL_OFFSET(
                                                                    pmTcmNum-1);
            valReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(valReg, 
                                                                   sliceSub);
            valMsk = 
    HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_VAL_TX_TCMI_BEI_BIAE_VALUE_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check if TX_TCMI_TRANSPARENT = 0 since TX_INSERT_TCMI_BEI_BIAE and
     TX_AUTO_TCMI_BEI_BIAE bits take effect only when TX_TCMI_TRANSPARENT = 0 */
    if (pmTcmNum != 0) {
        /* For TCMi control only */
        result = sysHyPhy20gBitRead(fileHndl, ctlReg, tcmiTransOff, &val);
        if (result) return result; 
            
        if (val != 0) {
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
    }
            
    switch (source) {
        case 0:
            /* Incoming PM/TCMi BEI value */
            insVal = 0; /* INSERT_PM/TCMi_BEI */
            autoVal = 0; /* AUTO_PM/TCMi_BEI */
            break;
        case 1:
            /* Automatically generate PM/TCMi BEI value */
            insVal = 0; /* INSERT_PM/TCMi_BEI */
            autoVal = 1; /* AUTO_PM/TCMi_BEI */
            break;
        case 2:
            /* Force PM/TCMi BEI value */
            insVal = 1; /* INSERT_PM/TCMi_BEI */
            autoVal = 0; /* AUTO_PM/TCMi_BEI */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure forced value */
    if (source == 2) {
        result = hyPhy20gRegFieldWrite(fileHndl, valReg, valMsk, forceVal);
        if(result) return result;
    }
    
    /* configure BEI/BIAE source selection */
    result = sysHyPhy20gRead(fileHndl, ctlReg, &val);
    if(result) return result;

    
    result = hyPhy20gVarFieldWrite(&val, insMsk, insVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&val, autoMsk, autoVal);
    if(result) return result;

    
    result = sysHyPhy20gWrite(fileHndl, ctlReg, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxPmTcmiBeiBiaeSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxPmBdiAutoProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the PM BDI automatic generation
**                  criteria for an OTU1 or OTU2 framer slice.
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
**                  losEnbl     - Controls whether PM BDI is generated as a
**                                result of incoming LOS
**                                1: BDI generated as a result of LOS
**                                0: BDI not generated as a result of LOS
**                  lofEnbl     - Controls whether PM BDI is generated as a
**                                result of incoming LOF
**                                1: BDI generated as a result of LOF
**                                0: BDI not generated as a result of LOF
**                  lomEnbl     - Controls whether PM BDI is generated as a
**                                result of incoming LOM
**                                1: BDI generated as a result of LOM
**                                0: BDI not generated as a result of LOM
**                  otukAisEnbl - Controls whether PM BDI is generated as a
**                                result of incoming ODUk-AIS
**                                1: BDI generated as a result of ODUk-AIS
**                                0: BDI not generated as a result of ODUk-AIS
**                  odukAisEnbl - Controls whether PM BDI is generated as a
**                                result of incoming PM ODUk-AIS
**                                1: BDI generated as a result of PM ODUk-AIS
**                                0: BDI not generated as a result of PM
**                                   ODUk-AIS
**                  ociEnbl     - Controls whether PM BDI is generated as
**                                a result of incoming PM OCI
**                                1: BDI generated as a result of PM OCI
**                                0: BDI not generated as a result of PM OCI
**                  lckEnbl     - Controls whether PM BDI is generated as
**                                a result of incoming PM LCK
**                                1: BDI generated as a result of PM LCK
**                                0: BDI not generated as a result of PM LCK
**                  pmTimEnbl   - Controls whether PM BDI is generated as
**                                a result of incoming PM TIM
**                                1: BDI generated as a result of PM TIM
**                                0: BDI not generated as a result of PM TIM
**                  smTimEnbl   - Controls whether PM BDI is generated as
**                                a result of incoming SM TIM
**                                1: BDI generated as a result of SM TIM
**                                0: BDI not generated as a result of SM TIM
**                  tcmiTimOciLckEnblMsk - Controls whether PM BDI is generated
**                                         as a result of incoming TCMi TIM,
**                                         OCI or LCK.
**                                       - Bits 5:0 corresponds to TCM6:1
**                                       - Legal range 0x0 to 0x3F.
**                                         1: BDI generated as a result of TCMi
**                                            TIM, OCI or LCK
**                                         0: BDI not generated as a result of
**                                            TCMi TIM, OCI or LCK
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxPmBdiAutoProcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 losEnbl, UINT4 lofEnbl, 
                                          UINT4 lomEnbl, UINT4 otukAisEnbl, 
                                          UINT4 odukAisEnbl, UINT4 ociEnbl, 
                                          UINT4 lckEnbl, UINT4 pmTimEnbl, 
                                          UINT4 smTimEnbl, 
                                          UINT4 tcmiTimOciLckEnblMsk)
{
    UINT4 sliceSub, val;
    UINT4 bdiCtlReg, losMsk;
    UINT4 lofMsk, lomMsk;
    UINT4 otukAisMsk, odukAisMsk, pmOciMsk;
    UINT4 pmLckMsk, pmTimMsk, smTimMsk;
    UINT4 tcmiTimOciLckMsk[6], tcmiEnbl;
    UINT4 i;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (losEnbl > 1) || (lofEnbl > 1) || (lomEnbl > 1) ||
        (otukAisEnbl > 1) || (odukAisEnbl > 1) || (ociEnbl > 1) ||
        (lckEnbl > 1) || (pmTimEnbl > 1) || (smTimEnbl > 1) ||
        (tcmiTimOciLckEnblMsk > 0x3f)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice 0 */
        case 1: /* OTU1 Slice 1 */
        case 2: /* OTU1 Slice 2 */
        case 3: /* OTU1 Slice 3 */
        case 4: /* OTU1 Slice 4 */
        case 5: /* OTU1 Slice 5 */
        case 6: /* OTU1 Slice 6 */
        case 7: /* OTU1 Slice 7 */
            sliceSub = slice;

            bdiCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_PM_AUTO_BDI_CTL;
            bdiCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(bdiCtlReg, 
                                                                      sliceSub);
            losMsk = 
         HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_LOS_MSK;
            lofMsk = 
         HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_LOF_MSK;
            lomMsk = 
         HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_LOM_MSK;
            otukAisMsk = 
    HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_OTUK_AIS_MSK;
            odukAisMsk = 
 HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_PM_ODUK_AIS_MSK;
            pmOciMsk = 
      HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_PM_OCI_MSK;
            pmLckMsk = 
      HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_PM_LCK_MSK;
            pmTimMsk = 
      HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_PM_TIM_MSK;
            smTimMsk = 
      HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_SM_TIM_MSK;
            tcmiTimOciLckMsk[0] = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_TCM1_TIM_OCI_LCK_MSK;
            tcmiTimOciLckMsk[1] = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_TCM2_TIM_OCI_LCK_MSK;
            tcmiTimOciLckMsk[2] = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_TCM3_TIM_OCI_LCK_MSK;
            tcmiTimOciLckMsk[3] = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_TCM4_TIM_OCI_LCK_MSK;
            tcmiTimOciLckMsk[4] = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_TCM5_TIM_OCI_LCK_MSK;
            tcmiTimOciLckMsk[5] = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_TCM6_TIM_OCI_LCK_MSK;
            break;
        case 8: /* OTU2 Slice 0 */
        case 9: /* OTU2 Slice 1 */
            sliceSub = slice - 8;

            bdiCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_PM_AUTO_BDI_CTL;
            bdiCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(bdiCtlReg, 
                                                                      sliceSub);
            losMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_LOS_MSK;
            lofMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_LOF_MSK;
            lomMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_LOM_MSK;
            otukAisMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_OTUK_AIS_MSK;
            odukAisMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_PM_ODUK_AIS_MSK;
            pmOciMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_PM_OCI_MSK;
            pmLckMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_PM_LCK_MSK;
            pmTimMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_PM_TIM_MSK;
            smTimMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_SM_TIM_MSK;
            tcmiTimOciLckMsk[0] = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_TCM1_TIM_OCI_LCK_MSK;
            tcmiTimOciLckMsk[1] = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_TCM2_TIM_OCI_LCK_MSK;
            tcmiTimOciLckMsk[2] = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_TCM3_TIM_OCI_LCK_MSK;
            tcmiTimOciLckMsk[3] = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_TCM4_TIM_OCI_LCK_MSK;
            tcmiTimOciLckMsk[4] = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_TCM5_TIM_OCI_LCK_MSK;
            tcmiTimOciLckMsk[5] = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_AUTO_BDI_CTL_PM_BDI_TCM6_TIM_OCI_LCK_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure BDI enable bits*/
    result = sysHyPhy20gRead(fileHndl, bdiCtlReg, &val);
    if(result) return result;


    result = hyPhy20gVarFieldWrite (&val, losMsk, losEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, lofMsk, lofEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, lomMsk, lomEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, otukAisMsk, otukAisEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, odukAisMsk, odukAisEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, pmOciMsk, ociEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, pmLckMsk, lckEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, pmTimMsk, pmTimEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, smTimMsk, smTimEnbl);
    if(result) return result;

    for (i = 0; i < 6; i++) {
        tcmiEnbl = (tcmiTimOciLckEnblMsk >> i) & 0x1;
        result = hyPhy20gVarFieldWrite (&val, tcmiTimOciLckMsk[i], tcmiEnbl);
        if(result) return result;
    }
    
    result = sysHyPhy20gWrite(fileHndl, bdiCtlReg, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxPmBdiAutoProcCfg */


/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxPmBdiSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the PM BDI source for the
**                  transmit data stream.
**
**                  When configured for 'Incoming PM BDI value' the value
**                  transferred on PM BDI bit will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
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
**                  source      - PM BDI source
**                                0 - Incoming BDI value
**                                1 - BDI value generated from receive data
**                                    stream
**                                2 - Force BDI value
**                  forceVal    - For source == 2 (otherwise set to 0)
**                                The value to be inserted into PM BDI
**                                bit in outgoing data stream
**                              - Legal range 0x0 to 0x1
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxPmBdiSrcCfg(struct file *fileHndl, UINT4 slice, 
                                     UINT4 source, UINT4 forceVal)
{
    UINT4 sliceSub, val;
    UINT4 pmCtlReg, insPmBdiMsk, autoPmBdiMsk;
    UINT4 pmValReg, pmBdiValMsk;
    UINT4 insPmBdiVal, autoPmBdiVal;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (source > 2) || (forceVal > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice 0 */
        case 1: /* OTU1 Slice 1 */
        case 2: /* OTU1 Slice 2 */
        case 3: /* OTU1 Slice 3 */
        case 4: /* OTU1 Slice 4 */
        case 5: /* OTU1 Slice 5 */
        case 6: /* OTU1 Slice 6 */
        case 7: /* OTU1 Slice 7 */
            sliceSub = slice;
            pmCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_PM_CTL;
            pmCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(pmCtlReg, 
                                                                     sliceSub);
            insPmBdiMsk = 
               HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_CTL_INSERT_PM_BDI_MSK;
            autoPmBdiMsk = 
                 HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_CTL_AUTO_PM_BDI_MSK;

            pmValReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_PM_VAL;
            pmValReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(pmValReg, 
                                                                     sliceSub);
            pmBdiValMsk = 
                HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_VAL_PM_BDI_VALUE_MSK;
            break;
        case 8: /* OTU2 Slice 0 */
        case 9: /* OTU2 Slice 1 */
            sliceSub = slice - 8;
            pmCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_PM_CTL;
            pmCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(pmCtlReg, 
                                                                     sliceSub);
            insPmBdiMsk = 
               HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_CTL_INSERT_PM_BDI_MSK;
            autoPmBdiMsk = 
                 HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_CTL_AUTO_PM_BDI_MSK;

            pmValReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_PM_VAL;
            pmValReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(pmValReg, 
                                                                     sliceSub);
            pmBdiValMsk = 
                HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_VAL_PM_BDI_VALUE_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (source) {
        case 0:
            /* Incoming PM BDI value */
            insPmBdiVal = 0; /* INSERT_PM_BDI */
            autoPmBdiVal = 0; /* AUTO_PM_BDI */
            break;
        case 1:
            /* Automatically generate PM BDI value */
            insPmBdiVal = 0; /* INSERT_PM_BDI */
            autoPmBdiVal = 1; /* AUTO_PM_BDI */
            break;
        case 2:
            /* Force PM BDI value */
            insPmBdiVal = 1; /* INSERT_PM_BDI */
            autoPmBdiVal = 0; /* AUTO_PM_BDI */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure forced value */
    if (source == 2) {
        result = hyPhy20gRegFieldWrite(fileHndl, pmValReg, pmBdiValMsk,
                                       forceVal);
        if(result) return result;
    }

    /* configure BDI source selection */
    result = sysHyPhy20gRead(fileHndl, pmCtlReg, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, insPmBdiMsk, insPmBdiVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, autoPmBdiMsk, autoPmBdiVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, pmCtlReg, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxPmBdiSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxTcmiBdiAutoProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the TCMi BDI automatic generation
**                  criteria for TCM1 to TCM6 for an OTU2 or OTU1 framer slice.
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
**                  losEnbl     - Controls whether TCMi BDI is generated as a
**                                result of incoming LOS
**                              - 1: BDI generated as a result of LOS
**                              - 0: BDI not generated as a result of LOS
**                  lofEnbl     - Controls whether TCMi BDI is generated as a
**                                result of incoming LOF
**                              - 1: BDI generated as a result of LOF
**                              - 0: BDI not generated as a result of LOF
**                  lomEnbl     - Controls whether TCMi BDI is generated as a
**                                result of incoming LOM
**                              - 1: BDI generated as a result of LOM
**                              - 0: BDI not generated as a result of LOM
**                  otukAisEnbl - Controls whether TCMi BDI is generated as a
**                                result of incoming ODUk-AIS
**                              - 1: BDI generated as a result of ODUk-AIS
**                              - 0: BDI not generated as a result of ODUk-AIS
**                  odukAisEnbl - Controls whether TCMi BDI is generated as a
**                                result of incoming TCMi ODUk-AIS
**                              - 1: BDI generated as a result of TCMi ODUk-AIS
**                              - 0: BDI not generated as a result of TCMi
*                                    ODUk-AIS
**                  ltcEnbl     - Controls whether TCMi BDI is generated as
**                                a result of incoming TCMi LTC
**                              - 1: BDI generated as a result of TCMi LTC
**                              - 0: BDI not generated as a result of TCMi LTC
**                  ociEnbl     - Controls whether TCMi BDI is generated as
**                                a result of incoming TCMi OCI
**                              - 1: BDI generated as a result of TCMi OCI
**                              - 0: BDI not generated as a result of TCMi OCI
**                  lckEnbl     - Controls whether TCMi BDI is generated as
**                                a result of incoming TCMi LCK
**                              - 1: BDI generated as a result of TCMi LCK
**                              - 0: BDI not generated as a result of TCMi LCK
**                  tcmiTimEnbl - Controls whether TCMi BDI is generated as
**                                a result of incoming TCMi TIM
**                              - 1: BDI generated as a result of TCMi TIM
**                              - 0: BDI not generated as a result of TCMi TIM
**                  smTimEnbl   - Controls whether TCMi BDI is generated as
**                                a result of incoming SM TIM
**                              - 1: BDI generated as a result of SM TIM
**                              - 0: BDI not generated as a result of SM TIM
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxTcmiBdiAutoProcCfg(struct file *fileHndl, UINT4 slice,
                                            UINT4 losEnbl, UINT4 lofEnbl,
                                            UINT4 lomEnbl, UINT4 otukAisEnbl,
                                            UINT4 odukAisEnbl, UINT4 ltcEnbl,
                                            UINT4 ociEnbl, UINT4 lckEnbl,
                                            UINT4 tcmiTimEnbl, UINT4 smTimEnbl)
{
    UINT4 sliceSub, val;
    UINT4 bdiCtlReg;
    UINT4 losMsk, lofMsk, lomMsk, otukAisMsk, odukAisMsk, tcmiLtcMsk;
    UINT4 tcmiOciMsk, tcmiLckMsk, tcmiTimMsk, smTimMsk;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (losEnbl > 1) || (lofEnbl > 1) || (lomEnbl > 1) ||
        (otukAisEnbl > 1) || (odukAisEnbl > 1) || (ltcEnbl > 1) ||
        (ociEnbl > 1) || (lckEnbl > 1) || (tcmiTimEnbl > 1) ||
        (smTimEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice 0 */
        case 1: /* OTU1 Slice 1 */
        case 2: /* OTU1 Slice 2 */
        case 3: /* OTU1 Slice 3 */
        case 4: /* OTU1 Slice 4 */
        case 5: /* OTU1 Slice 5 */
        case 6: /* OTU1 Slice 6 */
        case 7: /* OTU1 Slice 7 */
            sliceSub = slice;
            bdiCtlReg = 
                      HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_AUTO_BDI_CTL;
            bdiCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(bdiCtlReg, 
                                                                      sliceSub);
            losMsk = 
  HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_LOS_MSK;
            lofMsk = 
  HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_LOF_MSK;
            lomMsk = 
  HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_LOM_MSK;
            otukAisMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_OTUK_AIS_MSK;
            odukAisMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_TCMI_ODUK_AIS_MSK;
            tcmiLtcMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_TCMI_LTC_MSK;
            tcmiOciMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_TCMI_OCI_MSK;
            tcmiLckMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_TCMI_LCK_MSK;
            tcmiTimMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_TCMI_TIM_MSK;
            smTimMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_SM_TIM_MSK;
            break;
        case 8: /* OTU2 Slice 0 */
        case 9: /* OTU2 Slice 1 */
            sliceSub = slice - 8;
            bdiCtlReg = 
                      HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_AUTO_BDI_CTL;
            bdiCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(bdiCtlReg, 
                                                                      sliceSub);
            losMsk = 
  HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_LOS_MSK;
            lofMsk = 
  HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_LOF_MSK;
            lomMsk = 
  HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_LOM_MSK;
            otukAisMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_OTUK_AIS_MSK;
            odukAisMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_TCMI_ODUK_AIS_MSK;
            tcmiLtcMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_TCMI_LTC_MSK;
            tcmiOciMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_TCMI_OCI_MSK;
            tcmiLckMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_TCMI_LCK_MSK;
            tcmiTimMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_TCMI_TIM_MSK;
            smTimMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_TCMI_BDI_SM_TIM_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure BDI enable bits*/
    result = sysHyPhy20gRead(fileHndl, bdiCtlReg, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, losMsk, losEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, lofMsk, lofEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, lomMsk, lomEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, otukAisMsk, otukAisEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, odukAisMsk, odukAisEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiLtcMsk, ltcEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiOciMsk, ociEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiLckMsk, lckEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiTimMsk, tcmiTimEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, smTimMsk, smTimEnbl);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, bdiCtlReg, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxTcmiBdiAutoProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxTcmiBdiSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the TCMi BDI source for the
**                  transmit data stream.
**
**                  When configured for 'Incoming TCMi BDI value' the value
**                  transferred on TCMi BDI bit will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**                  When configured for 'BDI value generated from receive data
**                  stream', the conditions that generate BDI for TCM1 to TCM6
**                  for an OTU2 or OTU1 Framer slice are configured with
**                  hyPhy20gOdukppOdukTxTcmiBdiAutoProcCfg
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
**                  tcmNum      - TCMi field instance:
**                                1 to 6 - TCM1 to TCM6
**                  source      - TCMi BDI source
**                                0 - Incoming TCMi BDI value
**                                1 - BDI value generated from receive data
**                                    stream
**                                2 - Force BDI value
**                  forceVal    - For source == 2 (otherwise set to 0)
**                                The value to be inserted into TCMi BDI
**                                bit in outgoing data stream
**                              - Legal range 0x0 to 0x1
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxTcmiBdiSrcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 tcmNum, UINT4 source, 
                                       UINT4 forceVal)
{
    UINT4 sliceSub, tcmNumSub;
    UINT4 ctlReg, autoBdiMsk, insBdiMsk;
    UINT4 valReg, valMsk;
    UINT4 insBdiVal, autoBdiVal;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (tcmNum > 6) || (tcmNum < 1) || (source > 2) ||
        (forceVal > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    tcmNumSub = tcmNum - 1;

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice 0 */
        case 1: /* OTU1 Slice 1 */
        case 2: /* OTU1 Slice 2 */
        case 3: /* OTU1 Slice 3 */
        case 4: /* OTU1 Slice 4 */
        case 5: /* OTU1 Slice 5 */
        case 6: /* OTU1 Slice 6 */
        case 7: /* OTU1 Slice 7 */
            sliceSub = slice;
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_CTL_OFFSET(
                                                                     tcmNumSub);
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlReg, 
                                                                   sliceSub);
            autoBdiMsk = 
          HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_AUTO_TCMI_BDI_MSK;
            insBdiMsk = 
        HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_INSERT_TCMI_BDI_MSK;
            valReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_VAL_OFFSET(
                                                                     tcmNumSub);
            valReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(valReg, 
                                                                      sliceSub);
            valMsk = 
         HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_VAL_TX_TCMI_BDI_VALUE_MSK;
            break;
        case 8: /* OTU2 Slice 0 */
        case 9: /* OTU2 Slice 1 */
            sliceSub = slice - 8;
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_CTL_OFFSET(
                                                                     tcmNumSub);
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlReg, 
                                                                   sliceSub);
            autoBdiMsk = 
          HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_AUTO_TCMI_BDI_MSK;
            insBdiMsk = 
        HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_INSERT_TCMI_BDI_MSK;
            valReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_VAL_OFFSET(
                                                                     tcmNumSub);
            valReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(valReg, 
                                                                   sliceSub);
            valMsk = 
         HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_VAL_TX_TCMI_BDI_VALUE_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (source) {
        case 0:
            /* Incoming TCMi BDI value */
            insBdiVal = 0; /* INSERT_TCMi_BDI */
            autoBdiVal = 0; /* AUTO_TCMi_BDI */
            break;
        case 1:
            /* Automatically generate TCMi BDI value */
            insBdiVal = 0; /* INSERT_TCMi_BDI */
            autoBdiVal = 1; /* AUTO_TCMi_BDI */
            break;
        case 2:
            /* Force TCMi BDI value */
            insBdiVal = 1; /* INSERT_TCMi_BDI */
            autoBdiVal = 0; /* AUTO_TCMi_BDI */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure forced value */
    if (source == 2) {
        result = hyPhy20gRegFieldWrite(fileHndl, valReg, valMsk, forceVal);
        if(result) return result;
    }

    /* configure BDI source selection */
    result = hyPhy20gRegFieldWrite(fileHndl, ctlReg, insBdiMsk, insBdiVal);
    if(result) return result;

    result = hyPhy20gRegFieldWrite(fileHndl, ctlReg, autoBdiMsk, autoBdiVal);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxTcmiBdiSrcCfg */



/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxPmTcmiStatSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the PM/TCMi STAT source for the
**                  transmit data stream.
**
**                  When configured for 'Incoming PM/TCMi STAT value' the value
**                  transferred on PM/TCMi STAT bits will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTUk Framer instance:
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
**                  pmTcmNum   - PM or TCMi field instance:
**                               0 - PM
**                               1 to 6 - TCM1 to TCM6
**                  source     - PM/TCMi STAT source
**                               0 - Incoming PM/TCMi STAT value
**                               1 - For pmTcmNum == 1 to 6
**                                   Automatic generation in use with IAE, 
**                                   STAT = b'010 otherwise STAT = b'001.
**                               2 - Force STAT value
**                  forceVal   - For source == 2 (otherwise set to 0)
**                               The value to be inserted into STAT bits in
**                               outgoing data stream
**                             - Legal range 0 to 7
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxPmTcmiStatSrcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 pmTcmNum, UINT4 source,
                                          UINT4 forceVal)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 val;
    UINT4 ctlAddr; /* TX_OTN_ODU_PM/TCMi_CONTROL */
    UINT4 ctlMsk0, ctlMsk1 = 0, ctlVal0, ctlVal1;
    UINT4 valAddr; /* TX_OTN_ODU_PM/TCMI_VALUE */
    UINT4 valMsk;
    UINT4 tcmiTransOff = 0;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (pmTcmNum > 6) || (source > 2) || (forceVal > 7) ||
        ((pmTcmNum == 0)&&(source == 1))) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    sliceRate = slice < 8 ? 1:0;
    switch (pmTcmNum + sliceRate*7) {
        case 0: /* ODU2 Rate PM TTI */
            ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_PM_CTL;
            ctlMsk0 = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_CTL_INSERT_PM_STAT_MSK;
            valAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_PM_VAL;
            valMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_VAL_PM_STAT_VALUE_MSK;
            break;
        case 1: /* ODU2 Rate TCM1 TTI STAT */
        case 2: /* ODU2 Rate TCM2 TTI STAT */
        case 3: /* ODU2 Rate TCM3 TTI STAT */
        case 4: /* ODU2 Rate TCM4 TTI STAT */
        case 5: /* ODU2 Rate TCM5 TTI STAT */
        case 6: /* ODU2 Rate TCM6 TTI STAT */
            ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_CTL_OFFSET(pmTcmNum-1);
            ctlMsk0 = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_INSERT_TCMI_STAT_MSK;
            ctlMsk1 = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_AUTO_TCMI_STAT_IAE_MSK;
            valAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_VAL_OFFSET(pmTcmNum-1);
            valMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_VAL_TX_TCMI_STAT_VALUE_MSK;
            tcmiTransOff = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_TCMI_TRANSPARENT_OFF; 
            break;
        case 7: /* ODU1 Rate PM TTI STAT */
            ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_PM_CTL;
            ctlMsk0 = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_CTL_INSERT_PM_STAT_MSK;
            valAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_PM_VAL;
            valMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_VAL_PM_STAT_VALUE_MSK;
            break;
        case 8: /* ODU1 Rate TCM1 TTI STAT */
        case 9: /* ODU1 Rate TCM2 TTI STAT */
        case 10: /* ODU1 Rate TCM3 TTI STAT */
        case 11: /* ODU1 Rate TCM4 TTI STAT */
        case 12: /* ODU1 Rate TCM5 TTI STAT */
        case 13: /* ODU1 Rate TCM6 TTI STAT */
            ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_CTL_OFFSET(pmTcmNum-1);
            ctlMsk0 = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_INSERT_TCMI_STAT_MSK;
            ctlMsk1 = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_AUTO_TCMI_STAT_IAE_MSK;
            valAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_VAL_OFFSET(pmTcmNum-1);
            valMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_VAL_TX_TCMI_STAT_VALUE_MSK;
            tcmiTransOff = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_TCMI_TRANSPARENT_OFF;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if(slice > 7) {
        subSlcOff = slice - 8;
        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, subSlcOff);
        valAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(valAddr, subSlcOff);
    } else {
        subSlcOff = slice;
        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, subSlcOff);
        valAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(valAddr, subSlcOff);
    }

    /* Check if TX_TCMI_TRANSPARENT = 0 since TX_INSERT_TCMI_BEI_BIAE and
     TX_AUTO_TCMI_BEI_BIAE bits take effect only when TX_TCMI_TRANSPARENT = 0 */
    if (pmTcmNum != 0) {
        /* For TCMi control only */
        result = sysHyPhy20gBitRead(fileHndl, ctlAddr, tcmiTransOff, &val);
        if (result) return result; 
            
        if (val != 0) return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    
    switch (source) {
        case 0:
            /* Incoming TTI STAT value */
            ctlVal0 = 0; /* INSERT_PM/TCMI_STAT */
            ctlVal1 = 0; /* TX_AUTO_TCMI_STAT_IAE */
            break;
        case 1:
            /* Automatically generate TTI STAT value */
            ctlVal0 = 0; /* INSERT_PM/TCMI_STAT */
            ctlVal1 = 1; /* TX_AUTO_TCMI_STAT_IAE */
            break;
        case 2:
            /* Force TTI STAT value */
            ctlVal0 = 1; /* INSERT_PM/TCMI_STAT */
            ctlVal1 = 0; /* TX_AUTO_TCMI_STAT_IAE */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure forced value if source = 2 */
    if (source == 2) {
        result = hyPhy20gRegFieldWrite(fileHndl, valAddr, valMsk, forceVal);
        if(result) return result;
    }

    /* update INSERT_PM_TTI / INSERT_TCMi_TTI field */
    result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, ctlMsk0, ctlVal0);
    if(result) return result;

    if (pmTcmNum > 0) {
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, ctlMsk1, ctlVal1);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxPmTcmiStatSrcCfg */




/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxPmTcmiStatProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the PM or TCMi STAT bits.  It configures how the PM/TCMi
**                  STAT bits are monitored for operation of PM/TCMi STAT
**                  accepted and inconsistent interrupts.
**
**                  This function also configures the number of consecutive
**                  frames that IAE should be inserted in the TCMi STAT field.
**                  The value configured in the iaeInsCnt field applies to
**                  TCM1 to TCM6 within the specified slice.  Each time this
**                  function is called with pmTcmNum = 1 to 6, iaeInsCnt must
**                  be set to the desired value.
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
**                  pmTcmNum    - PM or TCMi field instance:
**                                0 - PM
**                                1 to 6 - TCM1 to TCM6
**                  acptCnt     - Controls whether the PM/TCMi STAT acceptance
**                                process accepts a STAT value after 3
**                                consecutive frames or 5 consecutive frames.
**                                This bit should be set to logic 0 to conform
**                                to G.798.
**                              - 1: acceptance process uses 5 frames.
**                              - 0: acceptance process uses 3 frames.
**                  iaeInsCnt   - For pmTcmNum 1 to 6 (otherwise set to 0)
**                                Controls number of consecutive frames that
**                                IAE should be inserted in the TCMi STAT field.
**                              - Legal range 0x0 to 0x1F
**                              - Default value 0x2
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxPmTcmiStatProcCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 pmTcmNum, UINT4 acptCnt,
                                           UINT4 iaeInsCnt)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 ctlReg, mfCntReg = 0;
    UINT4 acptCntMsk, mfCntMsk = 0;
    INT4  result;

    /* Argument check */
    if ((slice > 9) || (pmTcmNum > 6) || (acptCnt > 1) || (iaeInsCnt > 0x1F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register address */
    sliceRate = slice < 8 ? 1:0;
    switch (pmTcmNum + sliceRate*7) {
        case 0: /* ODU2 Rate PM TTI */
            subSlcOff = slice - 8;
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_PM_CTL;
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlReg, subSlcOff);
            acptCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_STAT_FILTER_5_MSK;
            break;
        case 1: /* ODU2 Rate TCM1 TTI */
        case 2: /* ODU2 Rate TCM2 TTI */
        case 3: /* ODU2 Rate TCM3 TTI */
        case 4: /* ODU2 Rate TCM4 TTI */
        case 5: /* ODU2 Rate TCM5 TTI */
        case 6: /* ODU2 Rate TCM6 TTI */
            subSlcOff = slice - 8;
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_CTL_OFFSET(pmTcmNum-1);
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlReg, subSlcOff);
            acptCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_STAT_FILTER_5_MSK;
            mfCntReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL;
            mfCntReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(mfCntReg, subSlcOff);
            mfCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_IAE_MULTIFRAME_COUNT_MSK;
            break;
        case 7: /* ODU1 Rate PM TTI */
            subSlcOff = slice;
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_PM_CTL;
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlReg, subSlcOff);
            acptCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_STAT_FILTER_5_MSK;
            break;
        case 8: /* ODU1 Rate TCM1 TTI */
        case 9: /* ODU1 Rate TCM2 TTI */
        case 10: /* ODU1 Rate TCM3 TTI */
        case 11: /* ODU1 Rate TCM4 TTI */
        case 12: /* ODU1 Rate TCM5 TTI */
        case 13: /* ODU1 Rate TCM6 TTI */
            subSlcOff = slice;
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_CTL_OFFSET(pmTcmNum-1);
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlReg, subSlcOff);
            acptCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_STAT_FILTER_5_MSK;
            mfCntReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL;
            mfCntReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(mfCntReg, subSlcOff);
            mfCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_IAE_MULTIFRAME_COUNT_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Configure RX_IAE_MULTIFRAME_COUNT */
    if (pmTcmNum > 0) {
        result = hyPhy20gRegFieldWrite(fileHndl, mfCntReg, mfCntMsk, iaeInsCnt);
        if(result) return result;
    }

    /* Configure PM/TCMI_STAT_FILTER_5 */
    result = hyPhy20gRegFieldWrite(fileHndl, ctlReg, acptCntMsk, acptCnt);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxPmTcmiStatProcCfg */


/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxPmTcmiStatAcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive PM/TCMi STAT
**                  bits.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - OTUk Framer instance:
**                                 0 - OTU1 Framer 0
**                                 1 - OTU1 Framer 1
**                                 2 - OTU1 Framer 2
**                                 3 - OTU1 Framer 3
**                                 4 - OTU1 Framer 4
**                                 5 - OTU1 Framer 5
**                                 6 - OTU1 Framer 6
**                                 7 - OTU1 Framer 7
**                                 8 - OTU2 Framer 0
**                                 9 - OTU2 Framer 1
**                  pmTcmNum     - PM or TCMi field instance:
**                                 0 - PM
**                                 1 to 6 - TCM1 to TCM6
**                  *pPmTcmiStat - PM/TCMi STAT bits
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxPmTcmiStatAcptExt(struct file *fileHndl, UINT4 slice,
                                           UINT4 pmTcmNum, UINT4 *pPmTcmiStat)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr, val = 0, msk;
    INT4 result;

    /* argument checking */
    if ((slice > 9) || (pmTcmNum > 6)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    sliceRate = slice < 8 ? 1:0;
    switch (pmTcmNum + sliceRate*7) {
        case 0: /* ODU2 Rate PM TTI */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_PM_CTL;
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_STAT_ACCEPT_VALUE_MSK;
            break;
        case 1: /* ODU2 Rate TCM1 TTI */
        case 2: /* ODU2 Rate TCM2 TTI */
        case 3: /* ODU2 Rate TCM3 TTI */
        case 4: /* ODU2 Rate TCM4 TTI */
        case 5: /* ODU2 Rate TCM5 TTI */
        case 6: /* ODU2 Rate TCM6 TTI */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_CTL_OFFSET(pmTcmNum-1);
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_STAT_ACCEPT_VALUE_MSK;
            break;
        case 7: /* ODU1 Rate PM TTI */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_PM_CTL;
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_STAT_ACCEPT_VALUE_MSK;
            break;
        case 8: /* ODU1 Rate TCM1 TTI */
        case 9: /* ODU1 Rate TCM2 TTI */
        case 10: /* ODU1 Rate TCM3 TTI */
        case 11: /* ODU1 Rate TCM4 TTI */
        case 12: /* ODU1 Rate TCM5 TTI */
        case 13: /* ODU1 Rate TCM6 TTI */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_CTL_OFFSET(pmTcmNum-1);
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_STAT_ACCEPT_VALUE_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if(slice > 7) {
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
    } else {
        subSlcOff = slice;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
    }


    /* Read register for field */
    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &val);
    if(result) return result;
    *pPmTcmiStat = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxPmTcmiStatAcptExt */



/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxPmTcmiTtiMsgCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the forced transmit PM or TCMi TTI
**                  message.
**
**                  The function takes in the 16 byte Source Access Point
**                  Identifier (SAPI), the 16 byte Destination Access Point
**                  Identifier (DAPI), and the 32 byte Operator Specific fields
**                  as arguments to fill the 64 byte TTI.
**
**                  The range of a SAPI or DAPI characters 15 to 1 are 0 to 127.
**                  SAPI and DAPI character 0 must be set to 0x00.  Function
**                  will return HYPHY20G_ERR_INVALID_ARG if these rules are not
**                  adhered.
**
**                  There are no restrictions on the values placed in Operator
**                  Specific bytes.
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
**                  pmTcmNum    - PM or TCMi field instance:
**                                0 - PM
**                                1 to 6 - TCM1 to TCM6
**                  sapi[16]    - SAPI characters (16 bytes)
**                  dapi[16]    - DAPI characters (16 bytes)
**                  opSpec[32]  - Operator Specific (32 bytes)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxPmTcmiTtiMsgCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 pmTcmNum, UINT4 sapi[16],
                                         UINT4 dapi[16], UINT4 opSpec[32])
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr;
    UINT4 ttiMsk;
    UINT4 regData;
    UINT4 val;
    UINT4 i, j;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (pmTcmNum > 6) ||  (sapi[0] > 0) || (dapi[0] > 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < 32; i++) {
        if ((i < 16) && (sapi[i] > 127 || dapi[i] > 127)) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        if (opSpec[i] > 255) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    /* Configure force transmit PM / TCMi TTI message */
    j = 0;
    for (i = 0; i < 32; i++) {

        /* Determine register address */
        sliceRate = slice < 8 ? 1:0;
        switch (pmTcmNum + sliceRate*7) {
            case 0: /* ODU2 Rate PM TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_PM_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_TTI_MEM_PM_TTI_VALUE_MSK;
                break;
            case 1: /* ODU2 Rate TCM1 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_TCM1_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_TCM1_TTI_MEM_TCM1_TTI_VALUE_MSK;
                break;
            case 2: /* ODU2 Rate TCM2 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_TCM2_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_TCM2_TTI_MEM_TCM2_TTI_VALUE_MSK;
                break;
            case 3: /* ODU2 Rate TCM3 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_TCM3_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_TCM3_TTI_MEM_TCM3_TTI_VALUE_MSK;
                break;
            case 4: /* ODU2 Rate TCM4 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_TCM4_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_TCM4_TTI_MEM_TCM4_TTI_VALUE_MSK;
                break;
            case 5: /* ODU2 Rate TCM5 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_TCM5_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_TCM5_TTI_MEM_TCM5_TTI_VALUE_MSK;
                break;
            case 6: /* ODU2 Rate TCM6 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_TCM6_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_TCM6_TTI_MEM_TCM6_TTI_VALUE_MSK;
                break;
            case 7: /* ODU1 Rate PM TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_PM_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_TTI_MEM_PM_TTI_VALUE_MSK;
                break;
            case 8: /* ODU1 Rate TCM1 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_TCM1_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_TCM1_TTI_MEM_TCM1_TTI_VALUE_MSK;
                break;
            case 9: /* ODU1 Rate TCM2 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_TCM2_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_TCM2_TTI_MEM_TCM2_TTI_VALUE_MSK;
                break;
            case 10: /* ODU1 Rate TCM3 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_TCM3_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_TCM3_TTI_MEM_TCM3_TTI_VALUE_MSK;
                break;
            case 11: /* ODU1 Rate TCM4 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_TCM4_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_TCM4_TTI_MEM_TCM4_TTI_VALUE_MSK;
                break;
            case 12: /* ODU1 Rate TCM5 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_TCM5_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_TCM5_TTI_MEM_TCM5_TTI_VALUE_MSK;
                break;
            case 13: /* ODU1 Rate TCM6 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_TCM6_TTI_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_TCM6_TTI_MEM_TCM6_TTI_VALUE_MSK;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        if(slice > 7) {
            subSlcOff = slice - 8;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        } else {
            subSlcOff = slice;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        }

        /* Read register */
        result = sysHyPhy20gRead(fileHndl, addr, &regData);
        if(result) return result;

        /* Modify read data */
        j = i*2;
        if (i < 8) {
            /* Configure SAPI (2 bytes per register) */
            val = (sapi[j+1] << 8) | sapi[j];
        } else if (i < 16) {
            /* Configure DAPI (2 bytes per register) */
            j = j - 16;
            val = (dapi[j+1] << 8) | dapi[j];
        } else {
            /* Configure Operator Specific (2 bytes per register) */
            j = j - 32;
            val = (opSpec[j+1] << 8) | opSpec[j];
        }
        result = hyPhy20gVarFieldWrite(&regData, ttiMsk, val);
        if(result) return result;

        /* Write data to register */
        result = sysHyPhy20gWrite(fileHndl, addr, regData);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxPmTcmiTtiMsgCfg */



/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxPmTcmiTtiMsgSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the PM/TCMi TTI source for the
**                  transmit data stream.
**
**                  When configured for 'Incoming PM/TCMi TTI value' the value
**                  transferred on PM/TCMi TTI bits will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**                  When configured for 'Force TTI value',
**                  hyPhy20gOdukppOdukTxPmTcmiTtiMsgCfg must be called to 
**                  configure the forced TTI values.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTUk Framer instance:
**                               0 - OTU1 Framer 0
**                               1 - OTU1 Framer 1
**                               2 - OTU1 Framer 2
**                               3 - OTU1 Framer 3
**                               4 - OTU1 Framer 4
**                               5 - OTU1 Framer 5
**                               6 - OTU1 Framer 6
**                               7 - OTU1 Framer 7
**                               8 - OTU2 Framer 0
**                               9 - OTU2 Framer 1
**                  pmTcmNum   - PM or TCMi field instance:
**                               0 - PM
**                               1 to 6 - TCM1 to TCM6
**                  source     - PM/TCMi TTI source
**                               0 - Incoming PM/TCMi TTI value
**                               1 - Force TTI value
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxPmTcmiTtiMsgSrcCfg(struct file *fileHndl, UINT4 slice,
                                            UINT4 pmTcmNum, UINT4 source)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr;
    UINT4 msk, val;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (pmTcmNum > 6) || (source > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    sliceRate = slice < 8 ? 1:0;
    switch (pmTcmNum + sliceRate*7) {
        case 0: /* ODU2 Rate PM TTI */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_PM_CTL;
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_CTL_INSERT_PM_TTI_MSK;
            break;
        case 1: /* ODU2 Rate TCM1 TTI */
        case 2: /* ODU2 Rate TCM2 TTI */
        case 3: /* ODU2 Rate TCM3 TTI */
        case 4: /* ODU2 Rate TCM4 TTI */
        case 5: /* ODU2 Rate TCM5 TTI */
        case 6: /* ODU2 Rate TCM6 TTI */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_CTL_OFFSET(pmTcmNum - 1);
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_INSERT_TCMI_TTI_MSK;
            break;
        case 7: /* ODU1 Rate PM TTI */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_PM_CTL;
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_CTL_INSERT_PM_TTI_MSK;
            break;
        case 8: /* ODU1 Rate TCM1 TTI */
        case 9: /* ODU1 Rate TCM2 TTI */
        case 10: /* ODU1 Rate TCM3 TTI */
        case 11: /* ODU1 Rate TCM4 TTI */
        case 12: /* ODU1 Rate TCM5 TTI */
        case 13: /* ODU1 Rate TCM6 TTI */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_CTL_OFFSET(pmTcmNum - 1);
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_INSERT_TCMI_TTI_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if(slice > 7) {
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
    } else {
        subSlcOff = slice;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
    }

    switch (source) {
        case 0:
            /* Incoming SM TTI value */
            val = 0;
            break;
        case 1:
            /* Force SM TTI value */
            val = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* update INSERT_PM_TTI / INSERT_TCMi_TTI field */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxPmTcmiTtiMsgSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOpukRxPayloadByteExpCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the expected receive OPUk payload
**                  fixed byte.
**
**                  The fixed payload expect value bits control the expected
**                  8-bit payload value that is to be repeatedly compared with
**                  the payload portion of received frames. If a match is
**                  detected, the interrupt bit CLIENT_FIXED_PYLD_INT_I is
**                  triggered. The feature is for diagnostic purposes.
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
**                  payloadVal  - Fixed Payload Expected Value
**                              - Legal Range 0 to 0xFF
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOpukRxPayloadByteExpCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 payloadVal)
{
    UINT4 addr, msk;
    UINT4 subSlcOff;
    INT4 result;

    /* argument checking */
    if (slice > 9 || payloadVal > 0xFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OPU_FXD_PYLD_EXP_PT_ACPT;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OPU_FXD_PYLD_EXP_PT_ACPT_FIXED_PAYLOAD_EXPECT_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OPU_FXD_PYLD_EXP_PT_ACPT;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OPU_FXD_PYLD_EXP_PT_ACPT_FIXED_PAYLOAD_EXPECT_VALUE_MSK;
    }

    /* Configure the FIXED_PAYLOAD_EXPECT_VALUE field  */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, payloadVal);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOpukRxPayloadByteExpCfg */


/*******************************************************************************
**
**  hyPhy20gOdukppOpukTxPayloadSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the ODUk payload source for the
**                  transmit data stream.
**
**                  This function is only valid for OTUk framers with system
**                  side data being sources from SONET or HSCI subsystems.
**
**                  This function is not valid for ODUk streams being received
**                  from the OPI subsystem.
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
**                  source      - ODUk payload source
**                                0 - Incoming payload from system side
**                                    interface
**                                1 - Reserved
**                                2 - Non inverted PN-31
**                                3 - PN-23
**                                4 - Regular (inverted) PN-31
**                                5 - Force value in payload
**                  forceVal   - For source == 5 (otherwise set to 0)
**                               The value to be inserted into OPUk payload
**                               of outgoing data stream
**                             - Legal range 0x0 to 0xFF
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOpukTxPayloadSrcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 source, UINT4 forceVal)
{
    UINT4 subSlcOff;
    UINT4 regData;
    UINT4 opuCtlReg;
    UINT4 insTstPyldMsk, tstPyldTypMsk;
    UINT4 insTstPyldVal, tstPyldTypVal;
    UINT4 pyldTypReg;
    UINT4 forceValMsk;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (source > 5) || (source == 1) || (forceVal > 0xFF) ||
        (source != 5 && forceVal != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        opuCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OPU_CTL;
        opuCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(opuCtlReg, subSlcOff);
        insTstPyldMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_CTL_INSERT_TEST_PAYLOAD_MSK;
        tstPyldTypMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_CTL_TEST_PAYLOAD_TYPE_MSK;

        pyldTypReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OPU_FXD_PYLD_TYP;
        pyldTypReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(pyldTypReg, subSlcOff);
        forceValMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_FXD_PYLD_TYP_FIXED_PAYLOAD_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        opuCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OPU_CTL;
        opuCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(opuCtlReg, subSlcOff);
        insTstPyldMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_CTL_INSERT_TEST_PAYLOAD_MSK;
        tstPyldTypMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_CTL_TEST_PAYLOAD_TYPE_MSK;

        pyldTypReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OPU_FXD_PYLD_TYP;
        pyldTypReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(pyldTypReg, subSlcOff);
        forceValMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_FXD_PYLD_TYP_FIXED_PAYLOAD_VALUE_MSK;
    }

    switch (source) {
        case 0:
            /* 0 - Incoming payload from system side interface */
        case 1:
            /* 1 - Generic AIS PN11 */
            insTstPyldVal = 0;
            tstPyldTypVal = 0;
            break;
        case 2:
            /* 2 - Non inverted PN-31 */
            insTstPyldVal = 1;
            tstPyldTypVal = 1;
            break;
        case 3:
            /* 3 - PN-23 */
            insTstPyldVal = 1;
            tstPyldTypVal = 2;
            break;
        case 4:
            /* 4 - Regular (inverted) PN-31 */
            insTstPyldVal = 1;
            tstPyldTypVal = 3;
            break;
        case 5:
            /* 5 - Force PT value to a fixed byte */
            insTstPyldVal = 1;
            tstPyldTypVal = 0;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if (source == 5) {
        /* Configure FIXED_PAYLOAD_VALUE */
        result = hyPhy20gRegFieldWrite(fileHndl, pyldTypReg, forceValMsk,
                                       forceVal);
        if(result) return result;
    }

    /* Read OPU control register */
    result = sysHyPhy20gRead(fileHndl, opuCtlReg, &regData);
    if(result) return result;

    /* Configure INSERT_TEST_PAYLOAD */
    result = hyPhy20gVarFieldWrite(&regData, insTstPyldMsk, insTstPyldVal);
    if(result) return result;

    /* Configure TEST_PAYLOAD_TYPE */
    result = hyPhy20gVarFieldWrite(&regData, tstPyldTypMsk, tstPyldTypVal);
    if(result) return result;

    /* Write OPU control register */
    result = sysHyPhy20gWrite(fileHndl, opuCtlReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOpukTxPayloadSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOpukTxPtSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the ODUk PSI PT source for the
**                  transmit data stream.
**
**                  When configured for 'Incoming PT value' the value
**                  transferred on ODUk PSI PT bits will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
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
**                  source      - ODUk PSI PT source
**                                0 - Incoming PT value
**                                1 - Force PT value
**                  forceVal   - For source == 1 (otherwise set to 0)
**                               The value to be inserted into OPUk PSI PT bits
**                               outgoing data stream
**                             - Legal range 0x0 to 0xFF
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOpukTxPtSrcCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 source, UINT4 forceVal)
{
    UINT4 subSlcOff;
    UINT4 regData;
    UINT4 pyldTypReg;
    UINT4 insPyldMsk, insPyldVal;
    UINT4 opuCtlReg;
    UINT4 forceValMsk;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (source > 1) || (forceVal > 0xFF) ||
        (source != 1 && forceVal != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice >7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        opuCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OPU_CTL;
        opuCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(opuCtlReg, subSlcOff);
        insPyldMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_CTL_INSERT_PAYLOAD_TYPE_MSK;

        pyldTypReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OPU_FXD_PYLD_TYP;
        pyldTypReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(pyldTypReg, subSlcOff);
        forceValMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_FXD_PYLD_TYP_PAYLOAD_TYPE_MSK;
     } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        opuCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OPU_CTL;
        opuCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(opuCtlReg, subSlcOff);
        insPyldMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_CTL_INSERT_PAYLOAD_TYPE_MSK;

        pyldTypReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OPU_FXD_PYLD_TYP;
        pyldTypReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(pyldTypReg, subSlcOff);
        forceValMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_FXD_PYLD_TYP_PAYLOAD_TYPE_MSK;
    }

    switch (source) {
        case 0:
            /* Incoming PT value */
            insPyldVal = 0;
            break;
        case 1:
            /* Force PT value */
            insPyldVal = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if (source == 1) {
        /* Configure PAYLOAD_TYPE */
        result = hyPhy20gRegFieldWrite(fileHndl, pyldTypReg, forceValMsk,
                                       forceVal);
        if(result) return result;
    }

    /* Read OPU control register */
    result = sysHyPhy20gRead(fileHndl, opuCtlReg, &regData);
    if(result) return result;

    /* Configure INSERT_PAYLOAD_TYPE */
    result = hyPhy20gVarFieldWrite(&regData, insPyldMsk, insPyldVal);
    if(result) return result;

    /* Write OPU control register */
    result = sysHyPhy20gWrite(fileHndl, opuCtlReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOpukTxPtSrcCfg */


/*******************************************************************************
**
**  hyPhy20gOdukppOpukRxPtExpCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the expected OPUk PSI PT value.
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
**                  pt          - Expected OPUk PSI PT value
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOpukRxPtExpCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 pt)
{
    UINT4 addr, msk;
    UINT4 subSlcOff;
    INT4 result;

    /* argument checking */
    if (slice > 9 || pt > 0xFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OPU_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OPU_CTL_PT_EXPECT_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OPU_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OPU_CTL_PT_EXPECT_VALUE_MSK;
    }

    /* Configure the PT_EXPECT_VALUE field  */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, pt);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOpukRxPtExpCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOpukRxPtAcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive PT bits.
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
**                  *pPt        - PT bits
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOpukRxPtAcptExt(struct file *fileHndl, UINT4 slice,                      
                                   UINT4 *pPt)
{
    UINT4 addr, val, msk;
    UINT4 subSlcOff;
    INT4 result;

    /* argument checking */
    if (slice > 9) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OPU_FXD_PYLD_EXP_PT_ACPT;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OPU_FXD_PYLD_EXP_PT_ACPT_PT_ACCEPT_VALUE_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OPU_FXD_PYLD_EXP_PT_ACPT;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OPU_FXD_PYLD_EXP_PT_ACPT_PT_ACCEPT_VALUE_MSK;
    }

    /* Configure the PT_ACCEPT_VALUE field  */
    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &val);
    if(result) return result;

    *pPt = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOpukRxPtAcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOpukRxPtProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the OPUk PSI PT byte.
**
**                  It configures how the PT byte are
**                  monitored for operation of PT accepted and inconsistent
**                  interrupts.
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
**                  acptCnt     - Number of identical PT bytes that
**                                the RX framer needs to obtain before declaring
**                                the PT bytes as accepted
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical PT bytes that
**                                the RX framer needs to obtain before
**                                triggering a PT byte inconsistent error
**                                interrupt
**                              - Range 1 to 7
**                              - Default value 0x5
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOpukRxPtProcCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 acptCnt, UINT4 inconCnt)
{
    UINT4 subSlcOff;
    UINT4 addr;
    UINT4 regData;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;
    INT4  result;

    /* Argument check */
    if ((slice > 9) || (acptCnt < 1) || (acptCnt > 7) ||
        (inconCnt < 1) || (inconCnt > 7))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OPU_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OPU_CTL_PT_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OPU_CTL_PT_INCONSISTENT_COUNT_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OPU_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);

        acptCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OPU_CTL_PT_ACCEPT_COUNT_MSK;
        inconCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OPU_CTL_PT_INCONSISTENT_COUNT_MSK;
    }

    /* Read register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Configure PT_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Configure PT_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* Write data to register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOpukRxPtProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxFrameProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the frame alignment bytes.
**
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
**                  nLomLomCnt  - Non LOM to LOM count:
**                              - Legal Range 0x2 to 0xFFF
**                              - Default value 0xF6 (G.798 compliance)
**                  imOomCnt    - In-multiframe (IM) to out-of-multiframe (OOM) 
**                                count:
**                              - Legal Range 0x1 to 0xF
**                              - Default value 0x5 (G.798 compliance)
**                  lomNlomCnt  - LOM to non-LOM count:
**                              - Legal Range 0x0 to 0xF
**                              - Default value 0x0 (G.798 compliance)
**                  nLofLofCnt  - Non LOF to LOF count:
**                              - Legal Range 0x2 to 0xFFF
**                              - Default value 0xF6 (G.798 compliance)
**                  ifOofCnt    - In-frame (IF) to out-of-frame (OOF) count:
**                              - Legal Range 0x2 to 0xF
**                              - Default value 0x5 (G.798 compliance)
**                  lofNlofCnt  - LOF to non-LOF count :
**                              - Legal Range 0x2 to 0xFFF
**                              - Default value 0xF6 (G.798 compliance)
**                 
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxFrameProcCfg(struct file *fileHndl, UINT4 slice, 
                                      UINT4 nLomLomCnt, UINT4 imOomCnt, 
                                      UINT4 lomNlomCnt, UINT4 nLofLofCnt,
                                      UINT4 ifOofCnt, UINT4 lofNlofCnt)
{
    UINT4 subSlcOff;
    UINT4 mfCtl1Reg, mfCtl2Reg, fCtl1Reg, fCtl2Reg;
    UINT4 regData;
    UINT4 nLomLomCntMsk;
    UINT4 imOomCntMsk;
    UINT4 lomNLomCntMsk;
    UINT4 nLofLofCntMsk;
    UINT4 ifOofCntMsk;
    UINT4 lofNLofCntMsk;
    INT4  result;

    /* Argument check */
    if ((slice > 9) || 
        (nLomLomCnt < 2) || (nLomLomCnt > 0xFFF) ||
        (imOomCnt < 1) || (imOomCnt > 0xF) || 
        (lomNlomCnt > 0xF) ||
        (nLofLofCnt < 2) || (nLofLofCnt > 0xFFF) ||
        (ifOofCnt < 2) || (ifOofCnt > 0xF) || 
        (lofNlofCnt < 2) || (lofNlofCnt > 0xFFF))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        /* N_LOM_LOM_COUNT */
        mfCtl1Reg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_FA_MFAU_CTL_1;
        mfCtl1Reg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(mfCtl1Reg, subSlcOff);
        nLomLomCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_MFAU_CTL_1_N_LOM_LOM_COUNT_MSK;
        
        /* IM_OOM_COUNT LOM_N_LOM_COUNT */
        mfCtl2Reg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_FA_MFAU_CTL_2;
        mfCtl2Reg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(mfCtl2Reg, subSlcOff);
        imOomCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_MFAU_CTL_2_IM_OOM_COUNT_MSK;
        lomNLomCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_MFAU_CTL_2_LOM_N_LOM_COUNT_MSK;
        
        /* N_LOF_LOF_COUNT */
        fCtl1Reg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_FA_FAU_CTL_1;
        fCtl1Reg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(fCtl1Reg, subSlcOff);
        nLofLofCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_FAU_CTL_1_N_LOF_LOF_COUNT_MSK;
        
        /* IF_OOF_COUNT LOF_N_LOF_COUNT */
        fCtl2Reg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_FA_FAU_CTL_2;
        fCtl2Reg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(fCtl2Reg, subSlcOff);
        ifOofCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_FAU_CTL_2_IF_OOF_COUNT_MSK;
        lofNLofCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_FAU_CTL_2_LOF_N_LOF_COUNT_MSK;

    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        /* N_LOM_LOM_COUNT */
        mfCtl1Reg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_MFAU_CTL_1;
        mfCtl1Reg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(mfCtl1Reg, subSlcOff);
        nLomLomCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_MFAU_CTL_1_N_LOM_LOM_COUNT_MSK;
        
        /* IM_OOM_COUNT LOM_N_LOM_COUNT */
        mfCtl2Reg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_MFAU_CTL_2;
        mfCtl2Reg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(mfCtl2Reg, subSlcOff);
        imOomCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_MFAU_CTL_2_IM_OOM_COUNT_MSK;
        lomNLomCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_MFAU_CTL_2_LOM_N_LOM_COUNT_MSK;
        
        /* N_LOF_LOF_COUNT */
        fCtl1Reg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_FAU_CTL_1;
        fCtl1Reg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(fCtl1Reg, subSlcOff);
        nLofLofCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_FAU_CTL_1_N_LOF_LOF_COUNT_MSK;
        
        /* IF_OOF_COUNT LOF_N_LOF_COUNT */
        fCtl2Reg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_FAU_CTL_2;
        fCtl2Reg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(fCtl2Reg, subSlcOff);
        ifOofCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_FAU_CTL_2_IF_OOF_COUNT_MSK;
        lofNLofCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_FAU_CTL_2_LOF_N_LOF_COUNT_MSK;
    }

    /* MF CTL 1 Register */
    /* Read register */
    /* configure N_LOM_LOM_COUNT */
    result = hyPhy20gRegFieldWrite(fileHndl, mfCtl1Reg, nLomLomCntMsk, 
                                   nLomLomCnt);
    if(result) return result;

    /* MF CTL 2 Register */
    /* Read register */
    result = sysHyPhy20gRead(fileHndl, mfCtl2Reg, &regData);
    if(result) return result;

    /* Configure IM_OOM_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, imOomCntMsk, imOomCnt);
    if(result) return result;

    /* Configure LOM_N_LOM_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, lomNLomCntMsk, lomNlomCnt);
    if(result) return result;

    /* Write data to register */
    result = sysHyPhy20gWrite(fileHndl, mfCtl2Reg, regData);
    if(result) return result;

    /* F CTL 1 Register */
    /* configure N_LOF_LOF_COUNT */
    result = hyPhy20gRegFieldWrite(fileHndl, fCtl1Reg, nLofLofCntMsk, 
                                   nLofLofCnt);
    if(result) return result;

    /* F CTL 2 Register */
    /* Read register */
    result = sysHyPhy20gRead(fileHndl, fCtl2Reg, &regData);
    if(result) return result;

    /* Configure IF_OOF_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, ifOofCntMsk, ifOofCnt);
    if(result) return result;

    /* Configure LOF_N_LOF_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, lofNLofCntMsk, lofNlofCnt);
    if(result) return result;

    /* Write data to register */
    result = sysHyPhy20gWrite(fileHndl, fCtl2Reg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukRxFrameProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxPmTcmiBipSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the PM/TCMi BIP source for the
**                  transmit data stream.
**
**                  When configured for 'Incoming PM/TCMi BIP value' the value
**                  transferred on PM/TCMi BIP bit will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
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
**                  pmTcmNum    - PM or TCMi field instance:
**                                0 - PM
**                                1 to 6 - TCM1 to TCM6
**                  source      - PM/TCMi BIP source
**                                0 - Incoming PM/TCMi BIP value
**                                1 - BIP value calculated from incoming data
**                                    stream
**                                2 - Generate Corrupted BIP value
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxPmTcmiBipSrcCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 pmTcmNum, UINT4 source)
{
    UINT4 sliceSub, sliceRate;
    UINT4 addr, val;
    UINT4 insVal, crptVal, insBipMsk, bipCrptMsk;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (pmTcmNum > 6) || (source > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    sliceRate = slice < 8 ? 1:0;
    switch (pmTcmNum + sliceRate*7) {
        case 0: /* OTU2 Slice PM */
            sliceSub = slice - 8;
            /* PM_BIP_CORRUPT INSERT_PM_BIP */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_PM_CTL;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            insBipMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_CTL_INSERT_PM_BIP_MSK; 
            bipCrptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_PM_CTL_PM_BIP_CORRUPT_MSK;
            break;
        case 1: /* OTU2 Slice TCM1 */
        case 2: /* OTU2 Slice TCM2 */
        case 3: /* OTU2 Slice TCM3 */
        case 4: /* OTU2 Slice TCM4 */
        case 5: /* OTU2 Slice TCM5 */
        case 6: /* OTU2 Slice TCM6 */
            sliceSub = slice - 8;
            /* TX_TCMI_BIP_CORRUPT TX_INSERT_TCMI_BIP */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_CTL_OFFSET(pmTcmNum-1);
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            insBipMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_INSERT_TCMI_BIP_MSK;
            bipCrptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_TCMI_BIP_CORRUPT_MSK;
            break;
        case 7: /* OTU1 Slice PM */
            sliceSub = slice;
            /* PM_BIP_CORRUPT INSERT_PM_BIP */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_PM_CTL;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            insBipMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_CTL_INSERT_PM_BIP_MSK; 
            bipCrptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_PM_CTL_PM_BIP_CORRUPT_MSK;
            break;
        case 8: /* OTU1 Slice TCM1 */
        case 9: /* OTU1 Slice TCM2 */
        case 10: /* OTU1 Slice TCM3 */
        case 11: /* OTU1 Slice TCM4 */
        case 12: /* OTU1 Slice TCM5 */
        case 13: /* OTU1 Slice TCM6 */
            sliceSub = slice;
            /* TX_TCMI_BIP_CORRUPT TX_INSERT_TCMI_BIP */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_CTL_OFFSET(pmTcmNum-1);
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            insBipMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_INSERT_TCMI_BIP_MSK;
            bipCrptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_CTL_TX_TCMI_BIP_CORRUPT_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }
                             
    switch (source) {
        case 0:
            /* 0 - Incoming PM/TCMi BIP value */
            insVal = 0; /* INSERT_PM_BIP/TX_INSERT_TCMI_BIP */
            crptVal = 0; /* PM_BIP_CORRUPT/TX_TCMI_BIP_CORRUPT */
            break;
        case 1:
            /* 1 - BIP value calculated from incoming data stream */
            insVal = 1; /* INSERT_PM_BIP/TX_INSERT_TCMI_BIP */
            crptVal = 0; /* PM_BIP_CORRUPT/TX_TCMI_BIP_CORRUPT */
            break;
        case 2:
            /* 2 - Generate Corrupted BIP value */
            insVal = 1; /* INSERT_PM_BIP/TX_INSERT_TCMI_BIP */
            crptVal = 1; /* PM_BIP_CORRUPT/TX_TCMI_BIP_CORRUPT */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure BIP source selection */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    
    result = hyPhy20gVarFieldWrite (&val, insBipMsk, insVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, bipCrptMsk, crptVal);
    if(result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxPmTcmiBipSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukTxSmBipSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the SM BIP source for the
**                  transmit data stream.
**
**                  When configured for 'Incoming SM BIP value' the value
**                  transferred on SM BIP bit will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
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
**                  source      - SM BIP source
**                                0 - Incoming SM BIP value
**                                1 - BIP value calculated from incoming data
**                                    stream
**                                2 - Generate Corrupted BIP value
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxSmBipSrcCfg(struct file *fileHndl, UINT4 slice, 
                                     UINT4 source)
{
    UINT4 subSlcOff;
    UINT4 addr, val;
    UINT4 insBipMsk, bipCrptMsk, insVal, crptVal;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (source > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    if (slice > 7) {
        /* OTU2 Framers */
        subSlcOff = slice - 8;

        /* SM_BIP_CORRUPT INSERT_SM_BIP */
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OTU_SM_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        insBipMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_INSERT_SM_BIP_MSK;
        bipCrptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OTU_SM_CTL_SM_BIP_CORRUPT_MSK;
    } else {
        /* OTU1 Framers */
        subSlcOff = slice;

        /* SM_BIP_CORRUPT INSERT_SM_BIP */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OTU_SM_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        insBipMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_INSERT_SM_BIP_MSK;
        bipCrptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OTU_SM_CTL_SM_BIP_CORRUPT_MSK;
    }

    switch (source) {
        case 0:
            /* 0 - Incoming SM BIP value */
            insVal = 0; /* INSERT_SM_BIP */
            crptVal = 0; /* SM_BIP_CORRUPT */
            break;
        case 1:
            /* 1 - BIP value calculated from incoming data stream */
            insVal = 1; /* INSERT_SM_BIP */  
            crptVal = 0; /* SM_BIP_CORRUPT */
            break;
        case 2:
            /* 2 - Generate Corrupted BIP value */
            insVal = 1; /* INSERT_SM_BIP */  
            crptVal = 1; /* SM_BIP_CORRUPT */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Read register */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* Configure INSERT_SM_BIP */
    result = hyPhy20gVarFieldWrite(&val, insBipMsk, insVal);
    if(result) return result;

    /* Configure SM_BIP_CORRUPT */
    result = hyPhy20gVarFieldWrite(&val, bipCrptMsk, crptVal);
    if(result) return result;

    /* Write SM control register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukTxSmBipSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxPmTcmiBdiProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the receive data path processing
**                  of the PM/TCMi BIP.  It configures PM/TCMi BIP acceptance 
**                  criteria.
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
**                  pmTcmNum    - PM or TCMi field instance:
**                                0 - PM
**                                1 to 6 - TCM1 to TCM6
**                  bdiAcpt     - BDI acceptance criteria
**                              - 0 - BDI acceptance in 3 frames
**                              - 1 - BDI acceptance in 5 frames
**                                Set to 1 for G.798 compliance.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxPmTcmiBdiProcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 pmTcmNum, UINT4 bdiAcpt)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr, msk;
    INT4  result;

    /* Argument check */
    if ((slice > 9) || (pmTcmNum > 6) || (bdiAcpt > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register address */
    sliceRate = slice < 8 ? 1:0;
    switch (pmTcmNum + sliceRate*7) {
        case 0: /* ODU2 Rate PM TTI */
            subSlcOff = slice - 8;
            /* PM_BDI_FILTER_5 */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_PM_CTL;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_BDI_FILTER_5_MSK;
            break;
        case 1: /* ODU2 Rate TCM1 TTI */
        case 2: /* ODU2 Rate TCM2 TTI */
        case 3: /* ODU2 Rate TCM3 TTI */
        case 4: /* ODU2 Rate TCM4 TTI */
        case 5: /* ODU2 Rate TCM5 TTI */
        case 6: /* ODU2 Rate TCM6 TTI */
            subSlcOff = slice - 8;
            /* RX_TCMI_BDI_FILTER_5 */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_CTL_OFFSET(pmTcmNum-1) ;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_BDI_FILTER_5_MSK;
            break;
        case 7: /* ODU1 Rate PM TTI */
            subSlcOff = slice;
            /* PM_BDI_FILTER_5 */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_PM_CTL;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_PM_CTL_PM_BDI_FILTER_5_MSK;
            break;
        case 8: /* ODU1 Rate TCM1 TTI */
        case 9: /* ODU1 Rate TCM2 TTI */
        case 10: /* ODU1 Rate TCM3 TTI */
        case 11: /* ODU1 Rate TCM4 TTI */
        case 12: /* ODU1 Rate TCM5 TTI */
        case 13: /* ODU1 Rate TCM6 TTI */
            subSlcOff = slice;
            /* RX_TCMI_BDI_FILTER_5 */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_CTL_OFFSET(pmTcmNum-1) ;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_CTL_RX_TCMI_BDI_FILTER_5_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Configure PM_BDI_FILTER_5/RX_TCMI_BDI_FILTER_5 */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, bdiAcpt);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukRxPmTcmiBdiProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukTxTcmiBdiProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the transmit data path processing
**                  of the TCMi BDI.  It configures TCMi BDI acceptance 
**                  criteria.
**
**                  This feature is only relevant for ODUk frames received from
**                  the OPI subsystem.
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
**                  tcmNum      - TCMi field instance:
**                                1 to 6 - TCM1 to TCM6
**                  bdiAcpt     - BDI acceptance criteria
**                              - 0 - BDI acceptance in 3 frames
**                              - 1 - BDI acceptance in 5 frames
**                                Set to 1 for G.798 compliance.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxTcmiBdiProcCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 tcmNum, UINT4 bdiAcpt)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr, msk;
    INT4  result;

    /* Argument check */
    if ((slice > 9) || (tcmNum > 6) || (tcmNum < 1) || (bdiAcpt > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register address */
    sliceRate = slice < 8 ? 1:0;
    switch (tcmNum + sliceRate*7) {
        case 0: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 1: /* ODU2 Rate TCM1 TTI */
        case 2: /* ODU2 Rate TCM2 TTI */
        case 3: /* ODU2 Rate TCM3 TTI */
        case 4: /* ODU2 Rate TCM4 TTI */
        case 5: /* ODU2 Rate TCM5 TTI */
        case 6: /* ODU2 Rate TCM6 TTI */
            subSlcOff = slice - 8;
            /* TX_TCMI_BDI_FILTER_5 */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM1_INTRP_CTL_OFFSET(tcmNum-1) ;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_BDI_FILTER_5_MSK;
            break;
        case 7: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 8: /* ODU1 Rate TCM1 TTI */
        case 9: /* ODU1 Rate TCM2 TTI */
        case 10: /* ODU1 Rate TCM3 TTI */
        case 11: /* ODU1 Rate TCM4 TTI */
        case 12: /* ODU1 Rate TCM5 TTI */
        case 13: /* ODU1 Rate TCM6 TTI */
            subSlcOff = slice;
            /* TX_TCMI_BDI_FILTER_5 */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM1_INTRP_CTL_OFFSET(tcmNum-1) ;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_BDI_FILTER_5_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Configure TX_TCMI_BDI_FILTER_5 */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, bdiAcpt);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukTxTcmiBdiProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxTcmiBipSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the TCMi BIP source for the
**                  ODUk data stream bound for the OPI subsystem
**
**                  This function is only relevant for OTN framers that are
**                  connected to the OPI subsystem.
**
**                  When configured for 'Incoming TCMi BIP value' the value
**                  transferred on TCMi BIP bit will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
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
**                  tcmNum      - TCMi field instance:
**                                1 to 6 - TCM1 to TCM6
**                  source      - TCMi BIP source
**                                0 - Incoming TCMi BIP value
**                                1 - BIP value calculated from incoming data
**                                    stream
**                                2 - Generate Corrupted BIP value
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxTcmiBipSrcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 tcmNum, UINT4 source)
{
    UINT4 sliceSub, sliceRate;
    UINT4 addr, val;
    UINT4 insVal, crptVal, insBipMsk, bipCrptMsk;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (tcmNum > 6) || (tcmNum < 1) || (source > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    sliceRate = slice < 8 ? 1:0;
    switch (tcmNum + sliceRate*7) {
        case 0: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 1: /* OTU2 Slice TCM1 */
        case 2: /* OTU2 Slice TCM2 */
        case 3: /* OTU2 Slice TCM3 */
        case 4: /* OTU2 Slice TCM4 */
        case 5: /* OTU2 Slice TCM5 */
        case 6: /* OTU2 Slice TCM6 */
            sliceSub = slice - 8;
            /* TX_TCMI_BIP_CORRUPT TX_INSERT_TCMI_BIP */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_INS_CTL_OFFSET(tcmNum-1);
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            insBipMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_INSERT_TCMI_BIP_MSK;
            bipCrptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_TCMI_BIP_CORRUPT_MSK;
            break;
        case 7: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 8: /* OTU1 Slice TCM1 */
        case 9: /* OTU1 Slice TCM2 */
        case 10: /* OTU1 Slice TCM3 */
        case 11: /* OTU1 Slice TCM4 */
        case 12: /* OTU1 Slice TCM5 */
        case 13: /* OTU1 Slice TCM6 */
            sliceSub = slice;
            /* RX_TCMI_BIP_CORRUPT RX_INSERT_TCMI_BIP */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_INS_CTL_OFFSET(tcmNum-1);
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            insBipMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_INSERT_TCMI_BIP_MSK;
            bipCrptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_TCMI_BIP_CORRUPT_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }
                             
    switch (source) {
        case 0:
            /* 0 - Incoming PM/TCMi BIP value */
            insVal = 0; /* RX_INSERT_TCMI_BIP */
            crptVal = 0; /* RX_TCMI_BIP_CORRUPT */
            break;
        case 1:
            /* 1 - BIP value calculated from incoming data stream */
            insVal = 1; /* RX_INSERT_TCMI_BIP */
            crptVal = 0; /* RX_TCMI_BIP_CORRUPT */
            break;
        case 2:
            /* 2 - Generate Corrupted BIP value */
            insVal = 1; /* RX_INSERT_TCMI_BIP */
            crptVal = 1; /* RX_TCMI_BIP_CORRUPT */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure BIP source selection */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    
    result = hyPhy20gVarFieldWrite (&val, insBipMsk, insVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, bipCrptMsk, crptVal);
    if(result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxTcmiBipSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxTcmiTtiExpCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the expected TCMi TTI message
**                  to be received from the OPI subsystem. 
**
**                  Inbound ODUk frames are only received from the OPI 
**                  subsystem.
**
**                  The function takes in the 16 byte Source Access Point
**                  Identifier (SAPI), the 16 byte Destination Access Point
**                  Identifier (DAPI), and the 32 byte Operator Specific fields
**                  as arguments to fill the 64 byte TTI.
**
**                  The range of a SAPI or DAPI characters 15 to 1 are 0 to 127.
**                  SAPI and DAPI character 0 must be set to 0x00.  Function
**                  will return HYPHY20G_ERR_INVALID_ARG if these rules are not
**                  adhered.
**
**                  There are no restrictions on the values placed in Operator
**                  Specific bytes.
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
**                  tcmNum      - TCMi field instance:
**                                1 to 6 - TCM1 to TCM6
**                  sapi[16]    - SAPI characters (16 bytes)
**                  dapi[16]    - DAPI characters (16 bytes)
**                  opSpec[32]  - Operator Specific (32 bytes)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxTcmiTtiExpCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 tcmNum, UINT4 sapi[16],
                                       UINT4 dapi[16], UINT4 opSpec[32])
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr;
    UINT4 ttiMsk;
    UINT4 regData;
    UINT4 val;
    UINT4 i, j;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (tcmNum > 6) || (tcmNum < 1) || (sapi[0] > 0) || 
        (dapi[0] > 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < 32; i++) {
        if ((i < 16) && (sapi[i] > 127 || dapi[i] > 127)) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        if (opSpec[i] > 255) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    /* Configure expected receive PM/TCMi TTI message */
    j = 0;
    for (i = 0; i < 32; i++) {
        /* Determine register address */
        sliceRate = slice < 8 ? 1:0;
        switch (tcmNum + sliceRate*7) {
            case 0: /* Invalid Case */
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
                break;
            case 1: /* ODU2 Rate TCM1 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM1_INTRP_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_TTI_EXP_MEM_TCMI_TTI_EXPECT_VALUE_1_MSK;
                break;
            case 2: /* ODU2 Rate TCM2 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM2_INTRP_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM2_INTRP_TTI_EXP_MEM_TCMI_TTI_EXPECT_VALUE_2_MSK;
                break;
            case 3: /* ODU2 Rate TCM3 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM3_INTRP_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM3_INTRP_TTI_EXP_MEM_TCMI_TTI_EXPECT_VALUE_3_MSK;
                break;
            case 4: /* ODU2 Rate TCM4 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM4_INTRP_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM4_INTRP_TTI_EXP_MEM_TCMI_TTI_EXPECT_VALUE_4_MSK;
                break;
            case 5: /* ODU2 Rate TCM5 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM5_INTRP_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM5_INTRP_TTI_EXP_MEM_TCMI_TTI_EXPECT_VALUE_5_MSK;
                break;
            case 6: /* ODU2 Rate TCM6 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM6_INTRP_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM6_INTRP_TTI_EXP_MEM_TCMI_TTI_EXPECT_VALUE_6_MSK;
                break;
            case 7: /* Invalid Case */
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
                break;
            case 8: /* ODU1 Rate TCM1 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM1_INTRP_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_TTI_EXP_MEM_TCMI_TTI_EXPECT_VALUE_1_MSK;
                break;
            case 9: /* ODU1 Rate TCM2 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM2_INTRP_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM2_INTRP_TTI_EXP_MEM_TCMI_TTI_EXPECT_VALUE_2_MSK;
                break;
            case 10: /* ODU1 Rate TCM3 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM3_INTRP_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM3_INTRP_TTI_EXP_MEM_TCMI_TTI_EXPECT_VALUE_3_MSK;
                break;
            case 11: /* ODU1 Rate TCM4 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM4_INTRP_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM4_INTRP_TTI_EXP_MEM_TCMI_TTI_EXPECT_VALUE_4_MSK;
                break;
            case 12: /* ODU1 Rate TCM5 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM5_INTRP_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM5_INTRP_TTI_EXP_MEM_TCMI_TTI_EXPECT_VALUE_5_MSK;
                break;
            case 13: /* ODU1 Rate TCM6 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM6_INTRP_TTI_EXP_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM6_INTRP_TTI_EXP_MEM_TCMI_TTI_EXPECT_VALUE_6_MSK;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        if(slice > 7) {
            subSlcOff = slice - 8;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        } else {
            subSlcOff = slice;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        }

        /* Read register */
        result = sysHyPhy20gRead(fileHndl, addr, &regData);
        if(result) return result;

        /* Modify read data */
        j = i*2;
        if (i < 8) {
            /* Configure SAPI (2 bytes per register) */
            val = (sapi[j+1] << 8) | sapi[j];
        } else if (i < 16) {
            /* Configure DAPI (2 bytes per register) */
            j = j - 16;
            val = (dapi[j+1] << 8) | dapi[j];
        } else {
            /* Configure Operator Specific (2 bytes per register) */
            j = j - 32;
            val = (opSpec[j+1] << 8) | opSpec[j];
        }
        result = hyPhy20gVarFieldWrite(&regData, ttiMsk, val);
        if(result) return result;

        /* Write data to register */
        result = sysHyPhy20gWrite(fileHndl, addr, regData);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxTcmiTtiExpCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxTcmiTtiProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the transmit data path processing
**                  of TCMi TTI message receieved from the OPI subsystem.  It 
**                  configures how the TTI message is monitored and which fields 
**                  of the message contribute to TIM defect detection. 
**
**                  Inbound ODUk frames are only received from the OPI 
**                  subsystem.
**
**                  hyPhy20gOdukppOdukTxTcmiTtiExpCfg must be called to 
**                  configure the expected TTI message in order for the 
**                  processing performed by these feature to be meaningful.
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
**                  tcmNum      - TCMi field instance:
**                                1 to 6 - TCM1 to TCM6
**                  acptCnt     - Number of identical TCMi TTI messages that
**                                the RX framer needs to obtain before declaring
**                                the TTI as accepted.
**                              - Range 1 to 7
**                              - Default value 0x5
**                  inconCnt    - Number of non-identical TCMi TTI messages
**                                that the RX framer needs to obtain before
**                                triggering a TTI inconsistent error interrupt.
**                              - Range 1 to 7
**                              - Default value 0x5
**                  cmpSapi     - Controls whether or not the RX framer uses the
**                                SAPI field in the accepted TCMi TTI to
**                                compare against the expected TTI to detect a
**                                TIM defect
**                              - 1: TCMi SAPI is used for comparison for TIM
**                                   detection.
**                              - 0: TCMi SAPI is not used for comparison for
**                                   TIM detection.
**                  cmpDapi     - Controls whether or not the RX framer uses the
**                                DAPI field in the accepted TCMi TTI to
**                                compare against the expected TTI to detect a
**                                TIM defect
**                              - 1: TCMi DAPI is used for comparison for TIM
**                                   detection.
**                              - 0: TCMi DAPI is not used for comparison for
**                                   TIM detection.
**                  cmpOs       - Controls whether or not the RX framer uses the
**                                Opearator Specific (OS) field in the accepted
**                                TCMi TTI to compare against the expected
**                                TTI to detect a TIM defect
**                              - 1: TCMi OS is used for comparison for TIM
**                                   detection.
**                              - 0: TCMi OS is not used for comparison for
**                                   TIM detection.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxTcmiTtiProcCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 tcmNum, UINT4 acptCnt,
                                        UINT4 inconCnt, UINT4 cmpSapi,
                                        UINT4 cmpDapi, UINT4 cmpOs)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr;
    UINT4 regData;
    UINT4 acptCntMsk;
    UINT4 inconCntMsk;
    UINT4 cmpSapiMsk;
    UINT4 cmpDapiMsk;
    UINT4 cmpOsMsk;
    INT4  result;

    /* Argument check */
    if ((slice > 9) || (tcmNum > 6) || (tcmNum < 1) || (acptCnt < 1) ||
        (acptCnt > 7) ||(inconCnt < 1) || (inconCnt > 7) || (cmpSapi > 1) ||
        (cmpDapi > 1) || (cmpOs > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register address */
    sliceRate = slice < 8 ? 1:0;
    switch (tcmNum + sliceRate*7) {
        case 0: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 1: /* ODU2 Rate TCM1 TTI */
        case 2: /* ODU2 Rate TCM2 TTI */
        case 3: /* ODU2 Rate TCM3 TTI */
        case 4: /* ODU2 Rate TCM4 TTI */
        case 5: /* ODU2 Rate TCM5 TTI */
        case 6: /* ODU2 Rate TCM6 TTI */
            subSlcOff = slice-8;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM1_INTRP_CTL_OFFSET(tcmNum-1);
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
            cmpSapiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_TTI_SAPI_TIM_MSK;
            cmpDapiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_TTI_DAPI_TIM_MSK;
            cmpOsMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TCMI_TTI_OS_TIM_1_MSK;
            acptCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_TTI_ACCEPT_COUNT_MSK;
            inconCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_TTI_INCONSISTENT_COUNT_MSK;
            break;
        case 7: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 8: /* ODU1 Rate TCM1 TTI */
        case 9: /* ODU1 Rate TCM2 TTI */
        case 10: /* ODU1 Rate TCM3 TTI */
        case 11: /* ODU1 Rate TCM4 TTI */
        case 12: /* ODU1 Rate TCM5 TTI */
        case 13: /* ODU1 Rate TCM6 TTI */
            subSlcOff = slice;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM1_INTRP_CTL_OFFSET(tcmNum-1);
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
            cmpSapiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_TTI_SAPI_TIM_MSK;
            cmpDapiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_TTI_DAPI_TIM_MSK;
            cmpOsMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TCMI_TTI_OS_TIM_1_MSK;
            acptCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_TTI_ACCEPT_COUNT_MSK;
            inconCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_TTI_INCONSISTENT_COUNT_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Read register */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* Configure TTI_ACCEPT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, acptCntMsk, acptCnt);
    if(result) return result;

    /* Configure TTI_INCONSISTENT_COUNT */
    result = hyPhy20gVarFieldWrite(&regData, inconCntMsk, inconCnt);
    if(result) return result;

    /* Configure TTI_SAPI_TIM */
    result = hyPhy20gVarFieldWrite(&regData, cmpSapiMsk, cmpSapi);
    if(result) return result;

    /* Configure TTI_DAPI_TIM */
    result = hyPhy20gVarFieldWrite(&regData, cmpDapiMsk, cmpDapi);
    if(result) return result;

    /* Configure TTI_OS_TIM */
    result = hyPhy20gVarFieldWrite(&regData, cmpOsMsk, cmpOs);
    if(result) return result;

    /* Write data to register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxTcmiTtiProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxTcmiTtiAcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive TCMi TTI
**                  message for the transmit direction received from the OPI
**                  subsystem.
**
**                  Inbound ODUk frames are only received from the OPI 
**                  subsystem.
**
**                  The function exptracts the 64 byte TTI relative to the 16
**                  byte Source Access Point Identifier (SAPI), the 16 byte
**                  Destination Access Point Identifier (DAPI), and the 32 byte
**                  Operator Specific fields.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - OTUk Framer instance:
**                                 0 - OTU1 Framer 0
**                                 1 - OTU1 Framer 1
**                                 2 - OTU1 Framer 2
**                                 3 - OTU1 Framer 3
**                                 4 - OTU1 Framer 4
**                                 5 - OTU1 Framer 5
**                                 6 - OTU1 Framer 6
**                                 7 - OTU1 Framer 7
**                                 8 - OTU2 Framer 0
**                                 9 - OTU2 Framer 1
**                  tcmNum      - TCMi field instance:
**                                1 to 6 - TCM1 to TCM6
**                  *pSapi[16]   - SAPI characters (16 bytes)
**                  *pDapi[16]   - DAPI characters (16 bytes)
**                  *pOpSpec[32] - Operator Specific (32 bytes)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxTcmiTtiAcptExt(struct file *fileHndl, UINT4 slice,
                                        UINT4 tcmNum, UINT4 *pSapi, 
                                        UINT4 *pDapi, UINT4 *pOpSpec)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr;
    UINT4 regData;
    UINT4 i;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (tcmNum > 6) || (tcmNum < 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((pSapi == NULL) || (pDapi == NULL) || (pOpSpec == NULL)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Extract accepted receive PM / TCMi TTI message */
    for (i = 0; i < 32; i++) {

        /* Determine register address */
        sliceRate = slice < 8 ? 1:0;
        switch (tcmNum + sliceRate*7) {
            case 0: /* Invalid Case */
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
                break;
            case 1: /* ODU2 Rate TCM1 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM1_INTRP_TTI_EXT_MEM_OFFSET(i);
                break;
            case 2: /* ODU2 Rate TCM2 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM2_INTRP_TTI_EXT_MEM_OFFSET(i);
                break;
            case 3: /* ODU2 Rate TCM3 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM3_INTRP_TTI_EXT_MEM_OFFSET(i);
                break;
            case 4: /* ODU2 Rate TCM4 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM4_INTRP_TTI_EXT_MEM_OFFSET(i);
                break;
            case 5: /* ODU2 Rate TCM5 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM5_INTRP_TTI_EXT_MEM_OFFSET(i);
                break;
            case 6: /* ODU2 Rate TCM6 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM6_INTRP_TTI_EXT_MEM_OFFSET(i);
                break;
            case 7: /* Invalid Case */
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
                break;
            case 8: /* ODU1 Rate TCM1 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM1_INTRP_TTI_EXT_MEM_OFFSET(i);
                break;
            case 9: /* ODU1 Rate TCM2 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM2_INTRP_TTI_EXT_MEM_OFFSET(i);
                break;
            case 10: /* ODU1 Rate TCM3 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM3_INTRP_TTI_EXT_MEM_OFFSET(i);
                break;
            case 11: /* ODU1 Rate TCM4 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM4_INTRP_TTI_EXT_MEM_OFFSET(i);
                break;
            case 12: /* ODU1 Rate TCM5 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM5_INTRP_TTI_EXT_MEM_OFFSET(i);
                break;
            case 13: /* ODU1 Rate TCM6 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM6_INTRP_TTI_EXT_MEM_OFFSET(i);
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        if(slice > 7) {
            subSlcOff = slice - 8;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        } else {
            subSlcOff = slice;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        }

        /* Read register */
        result = sysHyPhy20gRead(fileHndl, addr, &regData);
        if(result) return result;

        /* Modify read data */
        if (i < 8) {
            /* Store SAPI (2 bytes per register) */
            *pSapi = regData & 0xFF;
            pSapi++;
            *pSapi = (regData >> 8) & 0xFF;
            pSapi++;
        } else if (i < 16) {
            /* Store DAPI (2 bytes per register) */
            *pDapi = regData & 0xFF;
            pDapi++;
            *pDapi = (regData >> 8) & 0xFF;
            pDapi++;
        } else {
            /* Store Operator Specific (2 bytes per register) */
            *pOpSpec = regData & 0xFF;
            pOpSpec++;
            *pOpSpec = (regData >> 8) & 0xFF;
            pOpSpec++;
        }
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxTcmiTtiAcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxTcmiBeiBipProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the accumulation mode of the 
**                  internal counters for receive TCMi BEI/BIP errors for an 
**                  OTU2 or OTU1 framer slice.
**
**                  When configured to accumulate on a block basis, the counter
**                  is incremented by at most 1 for each frame that contain
**                  TCMi BEI/BIP errors. If it is configured to
**                  accumulate on a bit basis, then the counter is advanced by
**                  the number of BEI/BIP errors in the frame.
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
**                  accuMode    - Controls whether TCMi BEI/BIP error
**                                counts are accumulated on a block basis or a
**                                bit basis.
**                              - 1: errors are accumulated on a block basis.
**                              - 0: errors are accumulatedon a bit basis.
**                                Set to 1 to conform to G.798
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxTcmiBeiBipProcCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 accuMode)
{
    UINT4 sliceSub;
    UINT4 reg, msk;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (accuMode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice TCM1 */
        case 1: /* OTU1 Slice TCM2 */
        case 2: /* OTU1 Slice TCM3 */
        case 3: /* OTU1 Slice TCM4 */
        case 4: /* OTU1 Slice TCM5 */
        case 5: /* OTU1 Slice TCM6 */
        case 6: /* OTU1 Slice TCM5 */
        case 7: /* OTU1 Slice TCM6 */
            sliceSub = slice;
            reg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_CNT_CTL;
            reg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(reg, sliceSub);
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_CNT_CTL_RX_BLOCK_MODE1_MSK;
            break;
        case 8: /* OTU2 Slice 0*/
        case 9: /* OTU2 Slice 1*/
            sliceSub = slice - 8;
            reg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_CNT_CTL;
            reg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(reg, sliceSub);
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_CNT_CTL_RX_BLOCK_MODE1_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure RX_BLOCK_MODE1 */
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, accuMode);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxTcmiBeiBipProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxSmBipProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the accumulation mode of the 
**                  internal counters for receive SM BIP errors for an 
**                  OTU2 or OTU1 framer slice.
**
**                  When configured to accumulate on a block basis, the counter
**                  is incremented by at most 1 for each frame that contain
**                  SM BIP errors. If it is configured to
**                  accumulate on a bit basis, then the counter is advanced by
**                  the number of BIP errors in the frame.
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
**                  accuMode    - Controls whether TCMi BEI/BIP error
**                                counts are accumulated on a block basis or a
**                                bit basis.
**                              - 1: errors are accumulated on a block basis.
**                              - 0: errors are accumulatedon a bit basis.
**                                Set to 1 to conform to G.798
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxSmBipProcCfg(struct file *fileHndl, UINT4 slice, 
                                      UINT4 accuMode)
{
    UINT4 sliceSub;
    UINT4 reg, msk;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (accuMode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice TCM1 */
        case 1: /* OTU1 Slice TCM2 */
        case 2: /* OTU1 Slice TCM3 */
        case 3: /* OTU1 Slice TCM4 */
        case 4: /* OTU1 Slice TCM5 */
        case 5: /* OTU1 Slice TCM6 */
        case 6: /* OTU1 Slice TCM5 */
        case 7: /* OTU1 Slice TCM6 */
            sliceSub = slice;
            reg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
            reg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(reg, sliceSub);
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_RX_BLOCK_MODE0_MSK;
            break;
        case 8: /* OTU2 Slice 0*/
        case 9: /* OTU2 Slice 1*/
            sliceSub = slice - 8;
            reg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL;
            reg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(reg, sliceSub);
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_FA_FEC_OTU_OPU_CNT_CTL_RX_BLOCK_MODE0_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure RX_BLOCK_MODE0 */
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, accuMode);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukRxSmBipProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxTcmiBeiBiaeSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the TCMi BEI/BIAE
**                  source for the ODUk frames destined for the OPI subsystem.
**
**                  Inbound ODUk frames are only generated for ODUk frames 
**                  destined for the OPI subsystem.
**
**                  When configured for
**                  'Incoming TCMi BEI/BIAE value' the value
**                  transferred on TCMi BEI/BIAE bits will be from
**                  highest to lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**                  When configured for 'BEI/BIAE value generated from receive
**                  data stream', the accumulation mode is configured with
**                  hyPhy20gOdukRxTcmiBeiBiaeAutoProcCfg for all TCMi
**                  BEI/BIAE indications within the OTU2 or OTU1 framer slice.
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
**                  tcmNum      - TCMi field instance:
**                                1 to 6 - TCM1 to TCM6
**                  source      - TCMi BEI/BIAE source
**                                0 - Incoming TCMi BEI/BIAE value
**                                1 - BEI/BIAE value generated from receive data
**                                    stream
**                                2 - Force BEI/BIAE value
**                  forceVal   - For source == 2 (otherwise set to 0)
**                               The value to be inserted into TCMi
**                               BEI/BIAE bits in outgoing data stream
**                             - Legal range 0x0 to 0xF
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxTcmiBeiBiaeSrcCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 tcmNum, UINT4 source,
                                           UINT4 forceVal)
{
    UINT4 sliceSub, sliceRate;
    UINT4 ctlReg, autoMsk, insMsk;
    UINT4 valReg, valMsk, val;
    UINT4 insVal, autoVal;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (tcmNum > 6) || (tcmNum < 1) || (source > 2) ||
        (forceVal > 0xF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    sliceRate = slice < 8 ? 1:0;
    switch (tcmNum + sliceRate*7) {
        case 0: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 1: /* OTU2 Slice TCM1 */
        case 2: /* OTU2 Slice TCM2 */
        case 3: /* OTU2 Slice TCM3 */
        case 4: /* OTU2 Slice TCM4 */
        case 5: /* OTU2 Slice TCM5 */
        case 6: /* OTU2 Slice TCM6 */
            sliceSub = slice - 8;
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_INS_CTL_OFFSET(tcmNum-1);
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlReg, sliceSub);
            insMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_INSERT_TCMI_BEI_BIAE_MSK;
            autoMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_AUTO_TCMI_BEI_BIAE_MSK;
            valReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_TCMI_VAL_INS_OFFSET(tcmNum-1);
            valReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(valReg, sliceSub);
            valMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_TCMI_VAL_INS_RX_TCMI_BEI_BIAE_VALUE_MSK;
            break;
        case 7: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 8: /* OTU1 Slice TCM1 */
        case 9: /* OTU1 Slice TCM2 */
        case 10: /* OTU1 Slice TCM3 */
        case 11: /* OTU1 Slice TCM4 */
        case 12: /* OTU1 Slice TCM5 */
        case 13: /* OTU1 Slice TCM6 */
            sliceSub = slice;
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_INS_CTL_OFFSET(tcmNum-1);
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlReg, sliceSub);
            insMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_INSERT_TCMI_BEI_BIAE_MSK;
            autoMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_AUTO_TCMI_BEI_BIAE_MSK;
            valReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_TCMI_VAL_INS_OFFSET(tcmNum-1);
            valReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(valReg, sliceSub);
            valMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_TCMI_VAL_INS_RX_TCMI_BEI_BIAE_VALUE_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (source) {
        case 0:
            /* Incoming TCMi BEI value */
            insVal = 0; /* INSERT_TCMi_BEI */
            autoVal = 0; /* AUTO_TCMi_BEI */
            break;
        case 1:
            /* Automatically generate TCMi BEI value */
            insVal = 0; /* INSERT_TCMi_BEI */
            autoVal = 1; /* AUTO_TCMi_BEI */
            break;
        case 2:
            /* Force TCMi BEI value */
            insVal = 1; /* INSERT_TCMi_BEI */
            autoVal = 0; /* AUTO_TCMi_BEI */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure forced value */
    if (source == 2) {
        result = hyPhy20gRegFieldWrite(fileHndl, valReg, valMsk, forceVal);
        if(result) return result;
    }
    
    /* configure BEI/BIAE source selection */
    result = sysHyPhy20gRead(fileHndl, ctlReg, &val);
    if(result) return result;

    
    result = hyPhy20gVarFieldWrite (&val, insMsk, insVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, autoMsk, autoVal);
    if(result) return result;

    
    result = sysHyPhy20gWrite(fileHndl, ctlReg, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxTcmiBeiBiaeSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxTcmiBdiAutoProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the TCMi BDI automatic generation
**                  criteria for TCM1 to TCM6 for an OTU2 or OTU1 framer slice
**                  for ODUk frames destined for the OPI subsystem.
**
**                  These configuration are active for TCMi instances that call
**                  hyPhy20gOdukppOdukRxTcmiBdiSrcCfg to select automatically
**                  generated as the source.
**
**                  Inbound ODUk frames are only generated for ODUk frames 
**                  destined for the OPI subsystem.
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
**                  odukAisEnbl - Controls whether TCMi BDI is generated as a
**                                result of incoming TCMi ODUk-AIS
**                              - 1: BDI generated as a result of TCMi ODUk-AIS
**                              - 0: BDI not generated as a result of TCMi
**                                   ODUk-AIS
**                  ltcEnbl     - Controls whether TCMi BDI is generated as
**                                a result of incoming TCMi LTC
**                              - 1: BDI generated as a result of TCMi LTC
**                              - 0: BDI not generated as a result of TCMi LTC
**                  ociEnbl     - Controls whether TCMi BDI is generated as
**                                a result of incoming TCMi OCI
**                              - 1: BDI generated as a result of TCMi OCI
**                              - 0: BDI not generated as a result of TCMi OCI
**                  lckEnbl     - Controls whether TCMi BDI is generated as
**                                a result of incoming TCMi LCK
**                              - 1: BDI generated as a result of TCMi LCK
**                              - 0: BDI not generated as a result of TCMi LCK
**                  timEnbl     - Controls whether TCMi BDI is generated as
**                                a result of incoming TCMi TIM
**                              - 1: BDI generated as a result of TCMi TIM
**                              - 0: BDI not generated as a result of TCMi TIM
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxTcmiBdiAutoProcCfg(struct file *fileHndl, UINT4 slice,
                                            UINT4 odukAisEnbl, UINT4 ltcEnbl, 
                                            UINT4 ociEnbl, UINT4 lckEnbl, 
                                            UINT4 timEnbl)
{
    UINT4 sliceSub, val;
    UINT4 bdiCtlReg;
    UINT4 odukAisMsk, tcmiLtcMsk;
    UINT4 tcmiOciMsk, tcmiLckMsk, tcmiTimMsk;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (odukAisEnbl > 1) || (ltcEnbl > 1) ||(ociEnbl > 1) ||
        (lckEnbl > 1) || (timEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice 0 */
        case 1: /* OTU1 Slice 1 */
        case 2: /* OTU1 Slice 2 */
        case 3: /* OTU1 Slice 3 */
        case 4: /* OTU1 Slice 4 */
        case 5: /* OTU1 Slice 5 */
        case 6: /* OTU1 Slice 6 */
        case 7: /* OTU1 Slice 7 */
            sliceSub = slice;
            bdiCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL;
            bdiCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(bdiCtlReg, sliceSub);
            odukAisMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_TCMI_BDI_TCMI_ODUK_AIS_MSK;
            tcmiLtcMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_TCMI_BDI_TCMI_LTC_MSK;
            tcmiOciMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_TCMI_BDI_TCMI_OCI_MSK;
            tcmiLckMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_TCMI_BDI_TCMI_LCK_MSK;
            tcmiTimMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_TCMI_BDI_TCMI_TIM_MSK;
            break;
        case 8: /* OTU2 Slice 0 */
        case 9: /* OTU2 Slice 1 */
            sliceSub = slice - 8;
            bdiCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL;
            bdiCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(bdiCtlReg, sliceSub);
            odukAisMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_TCMI_BDI_TCMI_ODUK_AIS_MSK;
            tcmiLtcMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_TCMI_BDI_TCMI_LTC_MSK;
            tcmiOciMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_TCMI_BDI_TCMI_OCI_MSK;
            tcmiLckMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_TCMI_BDI_TCMI_LCK_MSK;
            tcmiTimMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_TCMI_BDI_TCMI_TIM_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure BDI enable bits*/
    result = sysHyPhy20gRead(fileHndl, bdiCtlReg, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, odukAisMsk, odukAisEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiLtcMsk, ltcEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiOciMsk, ociEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiLckMsk, lckEnbl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tcmiTimMsk, timEnbl);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, bdiCtlReg, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxTcmiBdiAutoProcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxTcmiBdiSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the TCMi BDI source for ODUk frames
**                  bound for the OPI subsystem.
**
**                  Inbound ODUk frames are only generated for ODUk frames 
**                  destined for the OPI subsystem.
**
**                  When configured for 'Incoming TCMi BDI value' the value
**                  transferred on TCMi BDI bit will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**                  When configured for 'BDI value generated from receive data
**                  stream', the conditions that generate BDI for TCM1 to TCM6
**                  for an OTU2 or OTU1 Framer slice are configured with
**                  hyPhy20gOdukppOdukRxTcmiBdiAutoProcCfg
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
**                  tcmNum      - TCMi field instance:
**                                1 to 6 - TCM1 to TCM6
**                  source      - TCMi BDI source
**                                0 - Incoming TCMi BDI value
**                                1 - BDI value generated from receive data
**                                    stream
**                                2 - Force BDI value
**                  forceVal    - For source == 2 (otherwise set to 0)
**                                The value to be inserted into TCMi BDI
**                                bit in outgoing data stream
**                              - Legal range 0x0 to 0x1
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxTcmiBdiSrcCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 tcmNum, UINT4 source, 
                                       UINT4 forceVal)
{
    UINT4 sliceSub, tcmNumSub;
    UINT4 ctlReg, autoBdiMsk, insBdiMsk;
    UINT4 valReg, valMsk;
    UINT4 insBdiVal, autoBdiVal;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (tcmNum > 6) || (tcmNum < 1) || (source > 2) ||
        (forceVal > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    tcmNumSub = tcmNum - 1;

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice 0 */
        case 1: /* OTU1 Slice 1 */
        case 2: /* OTU1 Slice 2 */
        case 3: /* OTU1 Slice 3 */
        case 4: /* OTU1 Slice 4 */
        case 5: /* OTU1 Slice 5 */
        case 6: /* OTU1 Slice 6 */
        case 7: /* OTU1 Slice 7 */
            sliceSub = slice;
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_INS_CTL_OFFSET(tcmNumSub);
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlReg, sliceSub);
            autoBdiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_AUTO_TCMI_BDI_MSK;
            insBdiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_INSERT_TCMI_BDI_MSK;
            valReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_TCMI_VAL_INS_OFFSET(tcmNumSub);
            valReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(valReg, sliceSub);
            valMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_TCMI_VAL_INS_RX_TCMI_BDI_VALUE_MSK;
            break;
        case 8: /* OTU2 Slice 0 */
        case 9: /* OTU2 Slice 1 */
            sliceSub = slice - 8;
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_INS_CTL_OFFSET(tcmNumSub);
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlReg, sliceSub);
            autoBdiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_AUTO_TCMI_BDI_MSK;
            insBdiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_INSERT_TCMI_BDI_MSK;
            valReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_TCMI_VAL_INS_OFFSET(tcmNumSub);
            valReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(valReg, sliceSub);
            valMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_TCMI_VAL_INS_RX_TCMI_BDI_VALUE_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (source) {
        case 0:
            /* Incoming TCMi BDI value */
            insBdiVal = 0; /* INSERT_TCMi_BDI */
            autoBdiVal = 0; /* AUTO_TCMi_BDI */
            break;
        case 1:
            /* Automatically generate TCMi BDI value */
            insBdiVal = 0; /* INSERT_TCMi_BDI */
            autoBdiVal = 1; /* AUTO_TCMi_BDI */
            break;
        case 2:
            /* Force TCMi BDI value */
            insBdiVal = 1; /* INSERT_TCMi_BDI */
            autoBdiVal = 0; /* AUTO_TCMi_BDI */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure forced value */
    if (source == 2) {
        result = hyPhy20gRegFieldWrite(fileHndl, valReg, valMsk, forceVal);
        if(result) return result;
    }

    /* configure BDI source selection */
    result = hyPhy20gRegFieldWrite(fileHndl, ctlReg, insBdiMsk, insBdiVal);
    if(result) return result;

    result = hyPhy20gRegFieldWrite(fileHndl, ctlReg, autoBdiMsk, autoBdiVal);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxTcmiBdiSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxTcmiStatSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the TCMi STAT source for the
**                  OPI subsystem bound data stream.
**
**                  Inbound ODUk frames are only generated for ODUk frames 
**                  destined for the OPI subsystem.
**
**                  When configured for 'Incoming PM/TCMi STAT value' the value
**                  transferred on PM/TCMi STAT bits will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTUk Framer instance:
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
**                  tcmNum     - TCMi field instance:
**                               1 to 6 - TCM1 to TCM6
**                  source     - TCMi STAT source
**                               0 - Incoming TCMi STAT value
**                               1 - Automatic generation in use with IAE, 
**                                   STAT = b'010 otherwise STAT = b'001.
**                               2 - Force STAT value
**                  forceVal   - For source == 2 (otherwise set to 0)
**                               The value to be inserted into STAT bits in
**                               outgoing data stream
**                             - Legal range 0 to 7
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxTcmiStatSrcCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 tcmNum, UINT4 source,
                                        UINT4 forceVal)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 ctlAddr; /* TX_OTN_ODU_TCMi_CONTROL */
    UINT4 ctlMsk0, ctlMsk1 = 0, ctlVal0, ctlVal1;
    UINT4 valAddr; /* TX_OTN_ODU_TCMI_VALUE */
    UINT4 valMsk;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (tcmNum > 6) || (tcmNum < 1) || (source > 2) || 
        (forceVal > 7) ||
        ((forceVal != 0)&&(source != 2))) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    sliceRate = slice < 8 ? 1:0;
    switch (tcmNum + sliceRate*7) {
        case 0: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 1: /* ODU2 Rate TCM1 TTI STAT */
        case 2: /* ODU2 Rate TCM2 TTI STAT */
        case 3: /* ODU2 Rate TCM3 TTI STAT */
        case 4: /* ODU2 Rate TCM4 TTI STAT */
        case 5: /* ODU2 Rate TCM5 TTI STAT */
        case 6: /* ODU2 Rate TCM6 TTI STAT */   
            valAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_TCMI_VAL_INS_OFFSET(tcmNum-1);
            ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_INS_CTL_OFFSET(tcmNum-1);
            valMsk  = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_TCMI_VAL_INS_RX_TCMI_STAT_VALUE_MSK;
            ctlMsk1 = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_AUTO_TCMI_STAT_IAE_MSK;
            ctlMsk0 = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_INSERT_TCMI_STAT_MSK;         
            break;
        case 7: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 8: /* ODU1 Rate TCM1 TTI STAT */
        case 9: /* ODU1 Rate TCM2 TTI STAT */
        case 10: /* ODU1 Rate TCM3 TTI STAT */
        case 11: /* ODU1 Rate TCM4 TTI STAT */
        case 12: /* ODU1 Rate TCM5 TTI STAT */
        case 13: /* ODU1 Rate TCM6 TTI STAT */
            valAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_TCMI_VAL_INS_OFFSET(tcmNum-1);
            ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_INS_CTL_OFFSET(tcmNum-1);
            valMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_TCMI_VAL_INS_RX_TCMI_STAT_VALUE_MSK;
            ctlMsk1 = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_AUTO_TCMI_STAT_IAE_MSK;
            ctlMsk0 = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_INSERT_TCMI_STAT_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if(slice > 7) {
        subSlcOff = slice - 8;
        ctlAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, subSlcOff);
        valAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(valAddr, subSlcOff);
    } else {
        subSlcOff = slice;
        ctlAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlAddr, subSlcOff);
        valAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(valAddr, subSlcOff);
    }

    switch (source) {
        case 0:
            /* Incoming TTI STAT value */
            ctlVal0 = 0; /* INSERT_TCMI_STAT */
            ctlVal1 = 0; /* RX_AUTO_TCMI_STAT_IAE */
            break;
        case 1:
            /* Automatically generate TTI STAT value */
            ctlVal0 = 0; /* INSERT_TCMI_STAT */
            ctlVal1 = 1; /* RX_AUTO_TCMI_STAT_IAE */
            break;
        case 2:
            /* Force TTI STAT value */
            ctlVal0 = 1; /* INSERT_TCMI_STAT */
            ctlVal1 = 0; /* RX_AUTO_TCMI_STAT_IAE */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* update INSERT_TCMi_TTI field */
    result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, ctlMsk0, ctlVal0);
    if(result) return result;

    if (tcmNum > 0) {
        result = hyPhy20gRegFieldWrite(fileHndl, ctlAddr, ctlMsk1, ctlVal1);
        if(result) return result;
    }

    /* configure forced value if source = 2 */
    if (source == 2) {
        result = hyPhy20gRegFieldWrite(fileHndl, valAddr, valMsk, forceVal);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxTcmiStatSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxTcmiStatProcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the transmit data path processing
**                  of the TCMi STAT bits received from the OPI subsystem.  It 
**                  configures how the TCMi STAT bits are monitored for 
**                  operation of TCMi STAT accepted and inconsistent interrupts.
**
**                  Inbound ODUk frames are only received from the OPI 
**                  subsystem.
**
**                  This function also configures the number of consecutive
**                  frames that IAE should be inserted in the TCMi STAT field.
**                  The value configured in the iaeInsCnt field applies to
**                  TCM1 to TCM6 within the specified slice. 
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
**                  tcmNum      - TCMi field instance:
**                                1 to 6 - TCM1 to TCM6
**                  acptCnt     - Controls whether the TCMi STAT acceptance
**                                process accepts a STAT value after 3
**                                consecutive frames or 5 consecutive frames.
**                                This bit should be set to logic 0 to conform
**                                to G.798.
**                              - 1: acceptance process uses 5 frames.
**                              - 0: acceptance process uses 3 frames.
**                  iaeInsCnt   - Controls number of consecutive frames that
**                                IAE should be inserted in the TCMi STAT field.
**                              - Legal range 0x0 to 0x1F
**                              - Default value 0x2
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxTcmiStatProcCfg(struct file *fileHndl, UINT4 slice,
                                         UINT4 tcmNum, UINT4 acptCnt,
                                         UINT4 iaeInsCnt)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 ctlReg, mfCntReg = 0;
    UINT4 acptCntMsk, mfCntMsk = 0;
    INT4  result;

    /* Argument check */
    if ((slice > 9) || (tcmNum > 6) || (tcmNum < 1) || (acptCnt > 1) || 
        (iaeInsCnt > 0x1F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register address */
    sliceRate = slice < 8 ? 1:0;
    switch (tcmNum + sliceRate*7) {
        case 0: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 1: /* ODU2 Rate TCM1 TTI */
        case 2: /* ODU2 Rate TCM2 TTI */
        case 3: /* ODU2 Rate TCM3 TTI */
        case 4: /* ODU2 Rate TCM4 TTI */
        case 5: /* ODU2 Rate TCM5 TTI */
        case 6: /* ODU2 Rate TCM6 TTI */
            subSlcOff = slice - 8;
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM1_INTRP_CTL_OFFSET(tcmNum-1);
            ctlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(ctlReg, subSlcOff);
            acptCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_STAT_FILTER_5_MSK;
            mfCntReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_AUTO_BDI_CTL;
            mfCntReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(mfCntReg, subSlcOff);
            mfCntMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_IAE_MULTIFRAME_COUNT_MSK;
            break;
        case 7: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 8: /* ODU1 Rate TCM1 TTI */
        case 9: /* ODU1 Rate TCM2 TTI */
        case 10: /* ODU1 Rate TCM3 TTI */
        case 11: /* ODU1 Rate TCM4 TTI */
        case 12: /* ODU1 Rate TCM5 TTI */
        case 13: /* ODU1 Rate TCM6 TTI */
            subSlcOff = slice;
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM1_INTRP_CTL_OFFSET(tcmNum-1);
            ctlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(ctlReg, subSlcOff);
            acptCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_STAT_FILTER_5_MSK;
            mfCntReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_AUTO_BDI_CTL;
            mfCntReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(mfCntReg, subSlcOff);
            mfCntMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_IAE_MULTIFRAME_COUNT_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Configure TX_IAE_MULTIFRAME_COUNT */
    if (tcmNum > 0) {
        result = hyPhy20gRegFieldWrite(fileHndl, mfCntReg, mfCntMsk, iaeInsCnt);
        if(result) return result;
    }

    /* Configure TCMI_STAT_FILTER_5 */
    result = hyPhy20gRegFieldWrite(fileHndl, ctlReg, acptCntMsk, acptCnt);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxTcmiStatProcCfg */


/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxTcmiStatAcptExt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts the accepted receive TCMi STAT
**                  bits for ODUk frames received from the OPI subsystem.
**
**                  Inbound ODUk frames are only received from the OPI 
**                  subsystem.
**
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - OTUk Framer instance:
**                                 0 - OTU1 Framer 0
**                                 1 - OTU1 Framer 1
**                                 2 - OTU1 Framer 2
**                                 3 - OTU1 Framer 3
**                                 4 - OTU1 Framer 4
**                                 5 - OTU1 Framer 5
**                                 6 - OTU1 Framer 6
**                                 7 - OTU1 Framer 7
**                                 8 - OTU2 Framer 0
**                                 9 - OTU2 Framer 1
**                  tcmNum       - TCMi field instance:
**                                 1 to 6 - TCM1 to TCM6
**                  *pTcmiStat   - TCMi STAT bits
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxTcmiStatAcptExt(struct file *fileHndl, UINT4 slice,
                                         UINT4 tcmNum, UINT4 *pTcmiStat)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr, val = 0, msk;
    INT4 result;

    /* argument checking */
    if ((slice > 9) || (tcmNum > 6) || (tcmNum < 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    sliceRate = slice < 8 ? 1:0;
    switch (tcmNum + sliceRate*7) {
        case 0: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 1: /* ODU2 Rate TCM1 TTI */
        case 2: /* ODU2 Rate TCM2 TTI */
        case 3: /* ODU2 Rate TCM3 TTI */
        case 4: /* ODU2 Rate TCM4 TTI */
        case 5: /* ODU2 Rate TCM5 TTI */
        case 6: /* ODU2 Rate TCM6 TTI */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCM1_INTRP_CTL_OFFSET(tcmNum-1);
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_STAT_ACCEPT_VALUE_MSK;
            break;
        case 7: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 8: /* ODU1 Rate TCM1 TTI */
        case 9: /* ODU1 Rate TCM2 TTI */
        case 10: /* ODU1 Rate TCM3 TTI */
        case 11: /* ODU1 Rate TCM4 TTI */
        case 12: /* ODU1 Rate TCM5 TTI */
        case 13: /* ODU1 Rate TCM6 TTI */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCM1_INTRP_CTL_OFFSET(tcmNum-1);
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCM1_INTRP_CTL_TX_TCMI_STAT_ACCEPT_VALUE_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if(slice > 7) {
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
    } else {
        subSlcOff = slice;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
    }


    /* Read register for field */
    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &val);
    if(result) return result;
    *pTcmiStat = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxTcmiStatAcptExt */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxTcmiTtiMsgCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the forced TCMi TTI message that
**                  is forwarded to the OPI subsystem.  
**
**                  Inbound ODUk frames are only generated for ODUk frames 
**                  destined for the OPI subsystem.
**
**                  The function takes in the 16 byte Source Access Point
**                  Identifier (SAPI), the 16 byte Destination Access Point
**                  Identifier (DAPI), and the 32 byte Operator Specific fields
**                  as arguments to fill the 64 byte TTI.
**
**                  The range of a SAPI or DAPI characters 15 to 1 are 0 to 127.
**                  SAPI and DAPI character 0 must be set to 0x00.  Function
**                  will return HYPHY20G_ERR_INVALID_ARG if these rules are not
**                  adhered.
**
**                  There are no restrictions on the values placed in Operator
**                  Specific bytes.
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
**                  tcmNum      - TCMi field instance:
**                                1 to 6 - TCM1 to TCM6
**                  sapi[16]    - SAPI characters (16 bytes)
**                  dapi[16]    - DAPI characters (16 bytes)
**                  opSpec[32]  - Operator Specific (32 bytes)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxTcmiTtiMsgCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 tcmNum, UINT4 sapi[16],
                                       UINT4 dapi[16], UINT4 opSpec[32])
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr;
    UINT4 ttiMsk;
    UINT4 regData;
    UINT4 val;
    UINT4 i, j;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (tcmNum > 6) || (tcmNum < 1) || (sapi[0] > 0) || 
        (dapi[0] > 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    for (i = 0; i < 32; i++) {
        if ((i < 16) && (sapi[i] > 127 || dapi[i] > 127)) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
        if (opSpec[i] > 255) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    /* Configure force transmit PM / TCMi TTI message */
    j = 0;
    for (i = 0; i < 32; i++) {

        /* Determine register address */
        sliceRate = slice < 8 ? 1:0;
        switch (tcmNum + sliceRate*7) {
            case 0: /* Invalid Case */
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
                break;
            case 1: /* ODU2 Rate TCM1 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_TCM1_INS_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_TCM1_INS_MEM_TCMI_TTI_VALUE_1_MSK;
                break;
            case 2: /* ODU2 Rate TCM2 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_TCM2_INS_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_TCM2_INS_MEM_TCMI_TTI_VALUE_2_MSK;
                break;
            case 3: /* ODU2 Rate TCM3 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_TCM3_INS_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_TCM3_INS_MEM_TCMI_TTI_VALUE_3_MSK;
                break;
            case 4: /* ODU2 Rate TCM4 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_TCM4_INS_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_TCM4_INS_MEM_TCMI_TTI_VALUE_4_MSK;
                break;
            case 5: /* ODU2 Rate TCM5 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_TCM5_INS_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_TCM5_INS_MEM_TCMI_TTI_VALUE_5_MSK;
                break;
            case 6: /* ODU2 Rate TCM6 TTI */
                addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_TCM6_INS_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OTU_TCM6_INS_MEM_TCMI_TTI_VALUE_6_MSK;
                break;
            case 7: /* Invalid Case */
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
                break;
            case 8: /* ODU1 Rate TCM1 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_TCM1_INS_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_TCM1_INS_MEM_TCMI_TTI_VALUE_1_MSK;
                break;
            case 9: /* ODU1 Rate TCM2 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_TCM2_INS_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_TCM2_INS_MEM_TCMI_TTI_VALUE_2_MSK;
                break;
            case 10: /* ODU1 Rate TCM3 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_TCM3_INS_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_TCM3_INS_MEM_TCMI_TTI_VALUE_3_MSK;
                break;
            case 11: /* ODU1 Rate TCM4 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_TCM4_INS_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_TCM4_INS_MEM_TCMI_TTI_VALUE_4_MSK;
                break;
            case 12: /* ODU1 Rate TCM5 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_TCM5_INS_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_TCM5_INS_MEM_TCMI_TTI_VALUE_5_MSK;
                break;
            case 13: /* ODU1 Rate TCM6 TTI */
                addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_TCM6_INS_MEM_OFFSET(i);
                ttiMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OTU_TCM6_INS_MEM_TCMI_TTI_VALUE_6_MSK;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        if(slice > 7) {
            subSlcOff = slice - 8;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        } else {
            subSlcOff = slice;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
        }

        /* Read register */
        result = sysHyPhy20gRead(fileHndl, addr, &regData);
        if(result) return result;

        /* Modify read data */
        j = i*2;
        if (i < 8) {
            /* Configure SAPI (2 bytes per register) */
            val = (sapi[j+1] << 8) | sapi[j];
        } else if (i < 16) {
            /* Configure DAPI (2 bytes per register) */
            j = j - 16;
            val = (dapi[j+1] << 8) | dapi[j];
        } else {
            /* Configure Operator Specific (2 bytes per register) */
            j = j - 32;
            val = (opSpec[j+1] << 8) | opSpec[j];
        }
        result = hyPhy20gVarFieldWrite(&regData, ttiMsk, val);
        if(result) return result;

        /* Write data to register */
        result = sysHyPhy20gWrite(fileHndl, addr, regData);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxTcmiTtiMsgCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxTcmiTtiMsgSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the TCMi TTI source for ODUk frames
**                  destined for the OPI subsystem.
**
**                  Inbound ODUk frames are only generated for ODUk frames 
**                  destined for the OPI subsystem.
**
**                  When configured for 'Incoming TCMi TTI value' the value
**                  transferred on TCMi TTI bits will be from highest to
**                  lowest priority:
**                  - Data from OH RAM (if enabled)
**                  - Data from incoming ODUk frame (if frame from CBRB)
**                  - 0 (zero)
**
**                  When configured for 'Force TTI value',
**                  hyPhy20gOdukppOdukRxTcmiTtiMsgCfg must be called to 
**                  configure the forced TTI values.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTUk Framer instance:
**                               0 - OTU1 Framer 0
**                               1 - OTU1 Framer 1
**                               2 - OTU1 Framer 2
**                               3 - OTU1 Framer 3
**                               4 - OTU1 Framer 4
**                               5 - OTU1 Framer 5
**                               6 - OTU1 Framer 6
**                               7 - OTU1 Framer 7
**                               8 - OTU2 Framer 0
**                               9 - OTU2 Framer 1
**                  tcmNum    - TCMi field instance:
**                               1 to 6 - TCM1 to TCM6
**                  source     - TCMi TTI source
**                               0 - Incoming TCMi TTI value
**                               1 - Force TTI value
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxTcmiTtiMsgSrcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 tcmNum, UINT4 source)
{
    UINT4 subSlcOff, sliceRate;
    UINT4 addr;
    UINT4 msk, val;
    INT4  result;

    /* Argument checking */
    if ((slice > 9) || (tcmNum > 6) || (tcmNum < 1)|| (source > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    sliceRate = slice < 8 ? 1:0;
    switch (tcmNum + sliceRate*7) {
        case 0: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 1: /* ODU2 Rate TCM1 TTI */
        case 2: /* ODU2 Rate TCM2 TTI */
        case 3: /* ODU2 Rate TCM3 TTI */
        case 4: /* ODU2 Rate TCM4 TTI */
        case 5: /* ODU2 Rate TCM5 TTI */
        case 6: /* ODU2 Rate TCM6 TTI */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_INS_CTL_OFFSET(tcmNum - 1);
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_INSERT_TCMI_TTI_MSK;
            break;
        case 7: /* Invalid Case */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 8: /* ODU1 Rate TCM1 TTI */
        case 9: /* ODU1 Rate TCM2 TTI */
        case 10: /* ODU1 Rate TCM3 TTI */
        case 11: /* ODU1 Rate TCM4 TTI */
        case 12: /* ODU1 Rate TCM5 TTI */
        case 13: /* ODU1 Rate TCM6 TTI */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_INS_CTL_OFFSET(tcmNum - 1);
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_INS_CTL_RX_INSERT_TCMI_TTI_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if(slice > 7) {
        subSlcOff = slice - 8;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
    } else {
        subSlcOff = slice;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, subSlcOff);
    }

    switch (source) {
        case 0:
            /* Incoming TTI value */
            val = 0;
            break;
        case 1:
            /* Force TTI value */
            val = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* update INSERT_TCMi_TTI field */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxTcmiTtiMsgSrcCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukFrameTxDiagCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures one or more diagnostic test
**                  features that impact the transmit OTUk/ODUk frame
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
**                 fasCrpt      - FAS Corrupt
**                                0 - Do not corrupt FAS
**                                1 - Corrupt FAS
**                 mfasCrpt     - MFAS Corrupt
**                                0 - Do not corrupt MFAS
**                                1 - Corrupt MFAS
**                 scmbCrpt     - Transmit Scrambling Corrupt
**                                0 - Do not corrupt transmit scrambling
**                                1 - Corrupt transmit scrambling
**                  
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukFrameTxDiagCfg(struct file *fileHndl, UINT4 slice, 
                                      UINT4 fasCrpt, UINT4 mfasCrpt, 
                                      UINT4 scmbCrpt)
{
    UINT4 sliceSub;
    UINT4 val;
    UINT4 otuCtlReg, scrbCrptReg;
    UINT4 fCrptMsk, mfCrptMsk, scrbCrptMsk;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (fasCrpt > 1) || (mfasCrpt > 1) || (scmbCrpt > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice 0 */
        case 1: /* OTU1 Slice 1 */
        case 2: /* OTU1 Slice 2 */
        case 3: /* OTU1 Slice 3 */
        case 4: /* OTU1 Slice 4 */
        case 5: /* OTU1 Slice 5 */
        case 6: /* OTU1 Slice 6 */
        case 7: /* OTU1 Slice 7 */
            sliceSub = slice;
            /* FAS_CORRUPT MFAS_CORRUPT */
            otuCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_FA_OTU_CTL;
            otuCtlReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(otuCtlReg, sliceSub);
            fCrptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_FA_OTU_CTL_FAS_CORRUPT_MSK;
            mfCrptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_FA_OTU_CTL_MFAS_CORRUPT_MSK;

            /* TX_SCRAMBLE_CORRUPT */
            scrbCrptReg = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_GLBL_CTL;
            scrbCrptReg = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(scrbCrptReg, sliceSub);
            scrbCrptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_TX_SCRAMBLE_CORRUPT_MSK;
            break;
        case 8: /* OTU2 Slice 0*/
        case 9: /* OTU2 Slice 1*/
            sliceSub = slice - 8;
            /* FAS_CORRUPT MFAS_CORRUPT */
            otuCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_FA_OTU_CTL;
            otuCtlReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(otuCtlReg, sliceSub);
            fCrptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_FA_OTU_CTL_FAS_CORRUPT_MSK;
            mfCrptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_FA_OTU_CTL_MFAS_CORRUPT_MSK;

            /* TX_SCRAMBLE_CORRUPT */
            scrbCrptReg = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_GLBL_CTL;
            scrbCrptReg = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(scrbCrptReg, sliceSub);
            scrbCrptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_TX_SCRAMBLE_CORRUPT_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    result = sysHyPhy20gRead(fileHndl, otuCtlReg, &val);
    if(result) return result;

    /* configure FAS_CORRUPT */    
    result = hyPhy20gVarFieldWrite (&val, fCrptMsk, fasCrpt);
    if(result) return result;

    /* configure MFAS_CORRUPT */    
    result = hyPhy20gVarFieldWrite (&val, mfCrptMsk, mfasCrpt);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, otuCtlReg, val);
    if(result) return result;

    /* configure TX_SCRAMBLE_CORRUPT */
    result = hyPhy20gRegFieldWrite(fileHndl, scrbCrptReg, scrbCrptMsk, 
                                   scmbCrpt);
    if(result) return result;    

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukFrameTxDiagCfg */


/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxDiagCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures diagnostic test
**                  features that impact the transmit ODUk overhead
**
**
**  INPUTS:       *fileHndl      - base address
**                 slice         - OTUk Framer instance:
**                                 0 - OTU1 Framer 0
**                                 1 - OTU1 Framer 1
**                                 2 - OTU1 Framer 2
**                                 3 - OTU1 Framer 3
**                                 4 - OTU1 Framer 4
**                                 5 - OTU1 Framer 5
**                                 6 - OTU1 Framer 6
**                                 7 - OTU1 Framer 7
**                                 8 - OTU2 Framer 0
**                                 9 - OTU2 Framer 1
**                 forceAlarmSig - ODUk Alarm Signal Control
**                                 0 - no alarm signal forced
**                                 1 - Force ODUK_AIS
**                                 2 - Force ODUK_OCI
**                                 3 - Force ODUK_LCK
**                  
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxDiagCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 forceAlarmSig)
{
    UINT4 sliceSub;
    UINT4 addr, val, oduAisMsk, oduOciMsk, oduLckMsk;
    UINT4 oduAisVal, oduOciVal, oduLckVal;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (forceAlarmSig > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice 0 */
        case 1: /* OTU1 Slice 1 */
        case 2: /* OTU1 Slice 2 */
        case 3: /* OTU1 Slice 3 */
        case 4: /* OTU1 Slice 4 */
        case 5: /* OTU1 Slice 5 */
        case 6: /* OTU1 Slice 6 */
        case 7: /* OTU1 Slice 7 */
            sliceSub = slice;
            /* FORCE_ODUK_AIS FORCE_ODUK_OCI FORCE_ODUK_LCK */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_CTL;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            oduAisMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_FORCE_ODUK_AIS_MSK;
            oduOciMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_FORCE_ODUK_OCI_MSK;
            oduLckMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_FORCE_ODUK_LCK_MSK;
            break;
        case 8: /* OTU2 Slice 0*/
        case 9: /* OTU2 Slice 1*/
            sliceSub = slice - 8;
            /* FORCE_ODUK_AIS FORCE_ODUK_OCI FORCE_ODUK_LCK */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_CTL;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            oduAisMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_FORCE_ODUK_AIS_MSK;
            oduOciMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_FORCE_ODUK_OCI_MSK;
            oduLckMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_FORCE_ODUK_LCK_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (forceAlarmSig) {
        case 0:
            /* 0 - no alarm signal forced */
            oduAisVal = 0; /* FORCE_ODU_AIS */
            oduOciVal = 0; /* FORCE_ODU_OCI */
            oduLckVal = 0; /* FORCE_ODU_LCK */
            break;
        case 1:
            /* 1 - Force ODUK_AIS  */
            oduAisVal = 1; /* FORCE_ODU_AIS */
            oduOciVal = 0; /* FORCE_ODU_OCI */
            oduLckVal = 0; /* FORCE_ODU_LCK */
            break;
        case 2:
            /* 2 - Force ODUK_OCI   */
            oduAisVal = 0; /* FORCE_ODU_AIS */
            oduOciVal = 1; /* FORCE_ODU_OCI */
            oduLckVal = 0; /* FORCE_ODU_LCK */
            break;
        case 3:
            /* 2 - Force ODUK_LCK   */
            oduAisVal = 0; /* FORCE_ODU_AIS */
            oduOciVal = 0; /* FORCE_ODU_OCI */
            oduLckVal = 1; /* FORCE_ODU_LCK */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* configure ODUK_AIS */    
    result = hyPhy20gVarFieldWrite (&val, oduAisMsk, oduAisVal);
    if(result) return result;

    /* configure ODUK_OCI */    
    result = hyPhy20gVarFieldWrite (&val, oduOciMsk, oduOciVal);
    if(result) return result;

    /* configure ODUK_LCK */    
    result = hyPhy20gVarFieldWrite (&val, oduLckMsk, oduLckVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukTxDiagCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOpukTxDiagCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures diagnostic test
**                  features that impact the transmit OPUk overhead
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
**                 jc1Crpt      - JC1 Corruption
**                                0 - Do not corrupt JC1
**                                1 - Corrupt JC1
**                 jc2Crpt      - JC2 Corruption
**                                0 - Do not corrupt JC2
**                                1 - Corrupt JC2
**                 jc3Crpt      - JC3 Corruption
**                                0 - Do not corrupt JC3
**                                1 - Corrupt JC3
**                 forcePj      - Force state of PJ to 1
**                                0 - do not force state of PJ
**                                1 - Force state of PJ to 1
**                 forceNj      - Force state of NJ to 1
**                                0 - do not force state of NJ
**                                1 - Force state of NJ to 1
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOpukTxDiagCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 jc1Crpt, UINT4 jc2Crpt, UINT4 jc3Crpt, 
                                 UINT4 forcePj, UINT4 forceNj)
{
    UINT4 sliceSub;
    UINT4 addr, val;
    UINT4 pjCrptMsk, njCrptMsk;
    UINT4 jc1CrptMsk, jc2CrptMsk, jc3CrptMsk;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (jc1Crpt > 1) || (jc2Crpt > 1) || (jc3Crpt > 1) || 
        (forcePj > 1) || (forceNj > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice 0 */
        case 1: /* OTU1 Slice 1 */
        case 2: /* OTU1 Slice 2 */
        case 3: /* OTU1 Slice 3 */
        case 4: /* OTU1 Slice 4 */
        case 5: /* OTU1 Slice 5 */
        case 6: /* OTU1 Slice 6 */
        case 7: /* OTU1 Slice 7 */
            sliceSub = slice;
            /* FORCE_JC1_CORRUPT FORCE_JC2_CORRUPT FORCE_JC3_CORRUPT 
               FORCE_PJ FORCE_NJ */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OPU_CTL;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            pjCrptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_CTL_FORCE_PJ_MSK;
            njCrptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_CTL_FORCE_NJ_MSK;        
            jc1CrptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_CTL_FORCE_JC1_CORRUPT_MSK;
            jc2CrptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_CTL_FORCE_JC2_CORRUPT_MSK;
            jc3CrptMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_CTL_FORCE_JC3_CORRUPT_MSK;
            break;
        case 8: /* OTU2 Slice 0*/
        case 9: /* OTU2 Slice 1*/
            sliceSub = slice - 8;
            /* FORCE_JC1_CORRUPT FORCE_JC2_CORRUPT FORCE_JC3_CORRUPT 
               FORCE_PJ FORCE_NJ */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OPU_CTL;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            pjCrptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_CTL_FORCE_PJ_MSK;
            njCrptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_CTL_FORCE_NJ_MSK;        
            jc1CrptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_CTL_FORCE_JC1_CORRUPT_MSK;
            jc2CrptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_CTL_FORCE_JC2_CORRUPT_MSK;
            jc3CrptMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_CTL_FORCE_JC3_CORRUPT_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* configure FORCE_JC1_CORRUPT */    
    result = hyPhy20gVarFieldWrite (&val, jc1CrptMsk, jc1Crpt);
    if(result) return result;

    /* configure FORCE_JC2_CORRUPT */    
    result = hyPhy20gVarFieldWrite (&val, jc2CrptMsk, jc2Crpt);
    if(result) return result;

    /* configure FORCE_JC3_CORRUPT */    
    result = hyPhy20gVarFieldWrite (&val, jc3CrptMsk, jc3Crpt);
    if(result) return result;

    /* configure FORCE_PJ */    
    result = hyPhy20gVarFieldWrite (&val, pjCrptMsk, forcePj);
    if(result) return result;

    /* configure FORCE_NJ */    
    result = hyPhy20gVarFieldWrite (&val, njCrptMsk, forceNj);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOpukTxDiagCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukFrameRxDiagCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures one or more diagnostic test
**                  features that impact the receive OTUk/ODUk frame
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
**                 scmbCrpt     - Receive Scrambling Corrupt
**                                0 - Do not corrupt receive scrambling
**                                1 - Corrupt receive scrambling
**                  
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukFrameRxDiagCfg(struct file *fileHndl, UINT4 slice, 
                                      UINT4 scmbCrpt)
{
    UINT4 sliceSub;
    UINT4 addr, msk;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (scmbCrpt > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice 0 */
        case 1: /* OTU1 Slice 1 */
        case 2: /* OTU1 Slice 2 */
        case 3: /* OTU1 Slice 3 */
        case 4: /* OTU1 Slice 4 */
        case 5: /* OTU1 Slice 5 */
        case 6: /* OTU1 Slice 6 */
        case 7: /* OTU1 Slice 7 */
            sliceSub = slice;
            /* RX_SCRAMBLE_CORRUPT */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_GLBL_CTL;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_SCRAMBLE_CORRUPT_MSK;
            break;
        case 8: /* OTU2 Slice 0*/
        case 9: /* OTU2 Slice 1*/
            sliceSub = slice - 8;
            /* RX_SCRAMBLE_CORRUPT */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_SCRAMBLE_CORRUPT_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure RX_SCRAMBLE_CORRUPT */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, scmbCrpt);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukFrameRxDiagCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukTxDiagCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures diagnostic test
**                  features that impact the transmit OTUk/ODUk overhead
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
**                 forceAis    - Force OTUK_AIS
**                                0 - Do not force OTUK_AIS
**                                1 - Force OTUK_AIS
**                  
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxDiagCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 forceAis)
{
    UINT4 sliceSub;
    UINT4 addr, msk;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (forceAis > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice 0 */
        case 1: /* OTU1 Slice 1 */
        case 2: /* OTU1 Slice 2 */
        case 3: /* OTU1 Slice 3 */
        case 4: /* OTU1 Slice 4 */
        case 5: /* OTU1 Slice 5 */
        case 6: /* OTU1 Slice 6 */
        case 7: /* OTU1 Slice 7 */
            sliceSub = slice;
            /* FORCE_OTUK_AIS */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_FA_OTU_CTL;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_FA_OTU_CTL_FORCE_OTUK_AIS_MSK;
            break;
        case 8: /* OTU2 Slice 0*/
        case 9: /* OTU2 Slice 1*/
            sliceSub = slice - 8;
            /* FORCE_OTUK_AIS */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_FA_OTU_CTL;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_FA_OTU_CTL_FORCE_OTUK_AIS_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure FORCE_OTUK_AIS */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, forceAis);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukTxDiagCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxDiagCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures diagnostic test
**                  features that impact the receive ODUk overhead
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
**                 forceAlarmSig - ODUk Alarm Signal Control
**                                 0 - no alarm signal forced
**                                 1 - Force ODUK_AIS
**                                 2 - Force ODUK_LCK
**                  
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxDiagCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 forceAlarmSig)
{
    UINT4 sliceSub;
    UINT4 addr, val, oduAisMsk, oduLckMsk, oduAisVal, oduLckVal;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (forceAlarmSig > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice 0 */
        case 1: /* OTU1 Slice 1 */
        case 2: /* OTU1 Slice 2 */
        case 3: /* OTU1 Slice 3 */
        case 4: /* OTU1 Slice 4 */
        case 5: /* OTU1 Slice 5 */
        case 6: /* OTU1 Slice 6 */
        case 7: /* OTU1 Slice 7 */
            sliceSub = slice;
            /* FORCE_ODU_AIS FORCE_ODU_LCK */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_MAINT_CTL;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            oduAisMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_FORCE_ODU_AIS_MSK;
            oduLckMsk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_FORCE_ODU_LCK_MSK;
            break;
        case 8: /* OTU2 Slice 0*/
        case 9: /* OTU2 Slice 1*/
            sliceSub = slice - 8;
            /* FORCE_ODU_AIS FORCE_ODU_LCK */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_MAINT_CTL;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            oduAisMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_FORCE_ODU_AIS_MSK;
            oduLckMsk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_FORCE_ODU_LCK_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (forceAlarmSig) {
        case 0:
            /* 0 - no alarm signal forced */
            oduAisVal = 0; /* FORCE_ODU_AIS */
            oduLckVal = 0; /* FORCE_ODU_LCK */
            break;
        case 1:
            /* 1 - Force ODUK_AIS  */
            oduAisVal = 1; /* FORCE_ODU_AIS */
            oduLckVal = 0; /* FORCE_ODU_LCK */
            break;
        case 2:
            /* 2 - Force ODUK_LCK   */
            oduAisVal = 0; /* FORCE_ODU_AIS */
            oduLckVal = 1; /* FORCE_ODU_LCK */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }


    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* configure ODUK_AIS */    
    result = hyPhy20gVarFieldWrite (&val, oduAisMsk, oduAisVal);
    if(result) return result;

    /* configure ODUK_LCK */    
    result = hyPhy20gVarFieldWrite (&val, oduLckMsk, oduLckVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOdukRxDiagCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOpukRxDiagCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures diagnostic test
**                  features that impact the receive OPUk signal
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
**                 forceAis     - Force Generic AIS signal
**                                0 - do not force generic AIS signal
**                                1 - Force generic AIS signal
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOpukRxDiagCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 forceAis)
{
    UINT4 sliceSub;
    UINT4 addr, msk;
    INT4 result;

    /* Argument checking */
    if ((slice > 9) || (forceAis > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    switch (slice) {
        case 0: /* OTU1 Slice 0 */
        case 1: /* OTU1 Slice 1 */
        case 2: /* OTU1 Slice 2 */
        case 3: /* OTU1 Slice 3 */
        case 4: /* OTU1 Slice 4 */
        case 5: /* OTU1 Slice 5 */
        case 6: /* OTU1 Slice 6 */
        case 7: /* OTU1 Slice 7 */
            sliceSub = slice;
            /* FORCE_GENERIC_AIS */
            addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_MAINT_CTL;
            addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_MAINT_CTL_FORCE_GENERIC_AIS_MSK;
            break;
        case 8: /* OTU2 Slice 0*/
        case 9: /* OTU2 Slice 1*/
            sliceSub = slice - 8;
            /* FORCE_GENERIC_AIS */
            addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_MAINT_CTL;
            addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, sliceSub);
            msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_MAINT_CTL_FORCE_GENERIC_AIS_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure FORCE_GENERIC_AIS */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, forceAis);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOpukRxDiagCfg */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */


