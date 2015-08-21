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
**  FILE        : hyphy20g_vcat_features.c
**
**  $Date: 2011-01-30 17:12:19 +0800 (Sun, 30 Jan 2011) $
**
**  $Revision: 10693 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_vcat_features.h"

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
**  hyPhy20gVcatPrbsTxConfig
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function configures the PRBS generator within the VCAT
**                 subsystem. 
**
**  INPUTS:       *fileHndl     - base address
**                port          - PRBS generator to configure 
**                                0 - TVCPO_0 (OTN Traffic on Slice 0)
**                                1 - TVCPO_1 (OTN Traffic on Slice 1)
**                                2 - TVCP192 (SONET/SDH Traffic on Slice 0)
**                grpId         - VCAT Group ID to configure for PRBS generation
**                                valid values are 0x0 - 0x3F (0-63)
**                patType       - 0 - PRBS31 pattern is generated
**                              - 1 - PRBS23 Pattern is generated
**                enable        - 1 - PRBS Generator is enabled
**                              - 0 - PRBS Generator is disabled
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatPrbsTxConfig(struct file *fileHndl, UINT4 port, 
                              UINT4 grpId, UINT4 patType,
                              UINT4 enable)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 prbsCfg;
         
   /* argument checking */
    if ((port > 2)||(grpId > 0x3F)||(patType > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_PRBS_CFG;
    
    /* Disable PRBS Generator Before configuring */
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CFG_GEN_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure Parameters */
    result = sysHyPhy20gRead(fileHndl, regAddr, &prbsCfg);
    if (result) return result;
           
    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CFG_GEN_PORT_MSK, port); 
    if (result) return result;        

    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CFG_GEN_PRBS23_MSK, patType); 
    if (result) return result;        

    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CFG_GEN_GRP_ID_MSK, grpId); 
    if (result) return result;        

    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CFG_GEN_EN_MSK, enable); 
    if (result) return result;        

    result = sysHyPhy20gWrite(fileHndl, regAddr, prbsCfg);
    if (result) return result;                 
      
    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatPrbsTxConfig */                

/*******************************************************************************
**
**  hyPhy20gVcatPrbsRxConfig
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function configures the PRBS monitor within the VCAT
**                 subsystem. 
**
**  INPUTS:       *fileHndl     - base address
**                port          - PRBS monitor to configure 
**                                0 - RVCPO_0 (OTN Traffic on Slice 0)
**                                1 - RVCPO_1 (OTN Traffic on Slice 1)
**                                2 - RVCP192 (SONET/SDH Traffic on Slice 0)
**                grpId         - VCAT Group ID to configure for PRBS monitoring
**                                valid values are 0x0 - 0x3F (0-63)
**                patType       - 0 - PRBS31 pattern is monitored
**                              - 1 - PRBS23 Pattern is monitored
**                enable        - 1 - PRBS Monitor is enabled
**                              - 0 - PRBS Monitor is disabled
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatPrbsRxConfig(struct file *fileHndl, UINT4 port, 
                              UINT4 grpId, UINT4 patType,
                              UINT4 enable)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 prbsCfg;
         
   /* argument checking */
    if ((port > 2)||(grpId > 0x3F)||(patType > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_PRBS_CFG;
    
    /* Disable PRBS Monitor Before configuring */
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CFG_MON_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure Parameters */
    result = sysHyPhy20gRead(fileHndl, regAddr, &prbsCfg);
    if (result) return result;
           
    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CFG_MON_PORT_MSK, port); 
    if (result) return result;        

    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CFG_MON_PRBS23_MSK, patType); 
    if (result) return result;        

    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CFG_MON_GRP_ID_MSK, grpId); 
    if (result) return result;        

    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CFG_MON_EN_MSK, enable); 
    if (result) return result;        

    result = sysHyPhy20gWrite(fileHndl, regAddr, prbsCfg);
    if (result) return result;                 
      
    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatPrbsRxConfig */                

/*******************************************************************************
**
**  hyPhy20gVcatPrbsTxErrIns
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function will inject a up to 4 errors on the active PRBS 
**                 generator within the VCAT subsystem.  
**
**  INPUTS:       *fileHndl     - base address
**                errNum        - Number of errors to insert on the output PRBS
**                                stream.
**                              - valid values are 1 - 4 
**                 
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatPrbsTxErrIns(struct file *fileHndl, UINT4 errNum)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;
    UINT4 prbsCfg;
    UINT4 errorVal = 0;
         
   /* argument checking */
    if ((errNum > 4)||(errNum < 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_PRBS_CFG;
    
    /* Configure Error Number Parameter */
    result = sysHyPhy20gRead(fileHndl, regAddr, &prbsCfg);
    if (result) return result;
           
    switch (errNum) {
        case 1:
          errorVal = 0x1;
          break;
        case 2:
          errorVal = 0x3;
          break;
        case 3:
          errorVal = 0x7;
          break;
        case 4:
          errorVal = 0xF;
          break;
    }
           
    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CFG_ERROR_EN_MSK, errorVal); 
    if (result) return result;        

    result = sysHyPhy20gWrite(fileHndl, regAddr, prbsCfg);
    if (result) return result;                 
      
    /* Inject error(s) */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_PRBS_CNT_UDA_ERR_INS;    
    mask = HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CNT_UDA_ERR_INS_ERROR_INS_MSK;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CNT_UDA_ERR_INS_ERROR_INS_OFF;

    result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                        HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;
    
    result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                        HYPHY20G_DELAY_20USEC);
    if (result) return result;
      
    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatPrbsTxErrIns */ 

/*******************************************************************************
**
**  hyPhy20gVcatPrbsRxStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of the PRBS monitor within
**                  the VCAT subsystem
**  
**                  The counter(s) accessed by this function are latched by 
**                  performing a write to the UPDATE bit.
**
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentioal
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**  INPUTS:       *fileHndl    - base address
**                intClear     - 1 - all *_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                             - 0 - all *_I interrupt bits accessed by this  
**                               function will be left untouched.
**                *statusBits  - Pointer to storage for status bits
**                               Bit 2 - SYNC_V_RAW - Raw status of SYNC_V
**                               Bit 1 - SYNC_V_FILTER - Debounced status SYNC_V
**                               Bit 0 - SYNC_I
**                *allZeroSync - When set to '1' indicates that PRBS_REG[30:0]
**                               is all zeroes, meaning the PRBS monitor has 
**                               locked to an all Zero pattern.
**                *errorCount  - Pointer to storage for 32-bit prbs Error count
**
**  COMMENTS:   When the incoming data for a group disappears (i.e. an LOS 
**              condition for all members within the group), the RVCP192 will
**              stop forwarding data to the PRBS monitor, and the monitor's
**              SYNC_V bit will remain in the previous state.  The SYNC_V_FILTER
**              output of this command will be set to '1' when the SYNC_V bit is
**              '1' and the data arriving at the PRBS monitor is dynamically 
**              changing.  
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatPrbsRxStatus(struct file *fileHndl, UINT4 intClear, 
                              UINT4 *statusBits, UINT4 *allZeroSync,
                              UINT4 *errorCount)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;
    UINT4 prbsSyncVRaw = 0;
    UINT4 prbsSyncVFilt = 0;
    UINT4 prbsSyncI;
    UINT4 prbsErrCnt;
    UINT4 prbsRegVal1;
    UINT4 prbsReg1;
    UINT4 prbsRegVal2;
    UINT4 prbsReg2;
    UINT4 prbsRegVal3;
    UINT4 prbsReg3;
    UINT4 resultMask = 0;
    
         
   /* argument checking */
    if (intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    *allZeroSync = 0;
    
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_PRBS_CNT_UDA_ERR_INS;
    mask = HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CNT_UDA_ERR_INS_UPDATE_MSK;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_CNT_UDA_ERR_INS_UPDATE_OFF;    

    /* Trigger the counter update */
    result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                        HYPHY20G_DELAY_20USEC);
    if (result) return result;
    
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;
    
    /* make sure the UPDATE bit clears */
    result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                        HYPHY20G_DELAY_20USEC);
    if (result) return result;
          
    /* Get PRBS error count */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_PRBS_ERR;
    result = sysHyPhy20gRead(fileHndl, regAddr, &prbsErrCnt);
    if (result) return result;     
      
    /* Get PRBS SYNCV Status */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_LVL_INT_VAL;    
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_LVL_INT_VAL_SYNC_V_OFF;

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &prbsSyncVRaw);
    if (result) return result;    
      
    /* If in sync, check to make sure PRBS_SHIFT_REGISTER value is changing */
    if (prbsSyncVRaw) {
        regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_PRBS_SHIFT;  
        offset = HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_SHIFT_PRBS_REG_OFF;
        mask = HYPHY20G_VCAT_LCAS_TOP_BIT_PRBS_SHIFT_PRBS_REG_MSK;

        result = sysHyPhy20gRead(fileHndl, regAddr, &prbsReg1);
        if (result) return result;              
        prbsRegVal1 =  mHYPHY20G_GET_FIELD4(prbsReg1, mask, offset);

        result = sysHyPhy20gRead(fileHndl, regAddr, &prbsReg2);
        if (result) return result;              
        prbsRegVal2 =  mHYPHY20G_GET_FIELD4(prbsReg2, mask, offset);

        result = sysHyPhy20gRead(fileHndl, regAddr, &prbsReg3);
        if (result) return result;              
        prbsRegVal3 =  mHYPHY20G_GET_FIELD4(prbsReg3, mask, offset);

        if ((prbsRegVal1 != prbsRegVal2) && (prbsRegVal2 != prbsRegVal3)) {
            prbsSyncVFilt = 1;
        }
        
     /* Check for all Zeros pattern */
       if ((prbsRegVal1 == 0) && (prbsRegVal2 == 0) && (prbsRegVal3 == 0)) {
            *allZeroSync = 1;
        }
    }
   
    /* Get PRBS SYNCI Status */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_LVL_INT;
    offset = HYPHY20G_VCAT_LCAS_TOP_BIT_LVL_INT_SYNC_I_OFF;     

    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &prbsSyncI);
    if (result) return result;    


    /* Clear interrupts if enabled */
    if (intClear) {
        resultMask = HYPHY20G_VCAT_LCAS_TOP_BIT_LVL_INT_SYNC_I_MSK;
        result = sysHyPhy20gWrite(fileHndl, regAddr, resultMask);
        if (result) return result;   
    } 
          
    /* Compute results */
    *statusBits = (prbsSyncVRaw << 2) + (prbsSyncVFilt << 1) + prbsSyncI;
    *errorCount = prbsErrCnt;
        
    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatPrbsRxStatus */

/*******************************************************************************
**
**  hyPhy20gVcatDdrBistTest           
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Performs a Built In Self Test (BIST) on the DDR Controller.
**                  This should only be performed once DDR PHY initialization
**                  is complete, using vcatHyPhy20gVcatDdrPhyInit.
**
**  INPUTS:         *fileHndl   - base address
**                  startAddr   - start Address (in bytes) for BIST RAM checking
**                              - 0x0 - 0x3FFFFFFF
**                  addrSpce    - 2^addrSpace addresses will be checked
**                              - valid values are : 0x0 - 0x1F
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_DDR_BIST_TIMEOUT
**                  HYPHY20G_DDR_BIST_DATA_FAILURE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatDdrBistTest(struct file *fileHndl, UINT4 startAddr, UINT4 addrSpace)
{                                                        
    INT4 result;
    UINT4 offset = 0;
    UINT4 mask = 0;
    UINT4 regAddr = 0;
    UINT4 dataResult = 0;
    UINT4 dsdc17 = 0;
    
   /* argument checking */
    if ((startAddr > 0x3FFFFFFF)||(addrSpace > 0x1F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

/* Step 1 */
    regAddr = HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_2;
    result = sysHyPhy20gPollBit(fileHndl, regAddr, 
                HYPHY20G_VCAT_LCAS_DSDC_BIT_DCTL_2_DLLLOCKREG_MSK,
                HYPHY20G_VCAT_LCAS_DSDC_BIT_DCTL_2_DLLLOCKREG_MSK, 
                HYPHY20G_DELAY_1250USEC);
    if (result) return HYPHY20G_DDR_BIST_TIMEOUT;       
        
/* Step 2 */
    regAddr = HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_17;
    result = sysHyPhy20gRead(fileHndl, regAddr, &dsdc17);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&dsdc17, 
        HYPHY20G_VCAT_LCAS_DSDC_BIT_DCTL_17_ADDR_SPACE_MSK, addrSpace);
    if (result) return result;        

    result = sysHyPhy20gWrite(fileHndl, regAddr, dsdc17);
    if (result) return result;           
    
/* Step 3 */
    regAddr = HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_31;
    result = sysHyPhy20gWrite(fileHndl, regAddr, startAddr);
    if (result) return result;           

/* Step 4 */
    regAddr = HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_1;
    offset = HYPHY20G_VCAT_LCAS_DSDC_BIT_DCTL_1_BIST_DATA_CHECK_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;           

/* Step 5 */
    regAddr = HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_1;
    offset = HYPHY20G_VCAT_LCAS_DSDC_BIT_DCTL_1_BIST_ADDR_CHECK_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;           
        
/* Step 5a */
    regAddr = HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_20;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, 23, 1);
    if (result) return result;           

/* Step 6 */
    result = sysHyPhy20gUsDelay(1);
    if (result) return result;
    regAddr = HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_1;
    offset = HYPHY20G_VCAT_LCAS_DSDC_BIT_DCTL_1_BIST_GO_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;           

/* Step 7 */
    regAddr = HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_21;
    mask = 0x1 << 23;
    result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, mask, 
                                        HYPHY20G_DELAY_1MINUTE);
    if (result) return HYPHY20G_DDR_BIST_TIMEOUT;       

/* Step 8 */
    regAddr = HYPHY20G_VCAT_LCAS_DSDC_REG_DCTL_8;
    offset = HYPHY20G_VCAT_LCAS_DSDC_BIT_DCTL_8_BIST_RESULT_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &dataResult);
    if (result) return result;           
                                  

    if (dataResult == 0) {
        return HYPHY20G_DDR_BIST_DATA_FAILURE;
    } 

        
    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatDdrBistTest */
