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
**  FILE        : hyphy20g_essi_features.c
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
#include "hyphy20g_essi_features.h"

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
**  hyPhy20gEssiPrbsRxConfig
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function configures the PRBS monitor within the RSDM
**                 block of the ESSI subsystem. 
**
**  INPUTS:       *fileHndl     - base address
**                slice         - ESSI Slice Instance
**                                0 to 3 
**                link          - ESSI Link within Slice Instance
**                                0 to 7
**                patType       - 1 - PRBS23 Pattern (x^23 + x^18 + 1)
**                              - 0 - PRBS7 pattern (x^7 + x^6 + 1)
**                invPrbs       - 1 - PRBS pattern is inverted
**                              - 0 - PRBS pattern is not inverted
**                rawMode       - 1 - Unframed PRBS monitoring
**                              - 0 - Framed PRBS monitoring
**                enable        - 1 - PRBS Monitor is enabled
**                              - 0 - PRBS Monitor is disabled
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiPrbsRxConfig(struct file *fileHndl, UINT4 slice, 
                              UINT4 link, UINT4 patType,
                              UINT4 invPrbs, UINT4 rawMode,
                              UINT4 enable)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 prbsCfg;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
         
   /* argument checking */
    if ((slice > 3)||(link > 7)||(patType > 1)||(invPrbs > 1)||(rawMode > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    regAddr = HYPHY20G_ESSI_RSDM_REG_CFG;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    /* Disable PRBS Monitor Before configuring */
    offset = HYPHY20G_ESSI_RSDM_BIT_CFG_PRBS_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure Parameters */
    result = sysHyPhy20gRead(fileHndl, regAddr, &prbsCfg);
    if (result) return result;
           
    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_ESSI_RSDM_BIT_CFG_PTR_INTR_EN_MSK, 1); 
    if (result) return result;        

    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_ESSI_RSDM_BIT_CFG_PRBS_23_POLY_MSK, patType); 
    if (result) return result;        

    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_ESSI_RSDM_BIT_CFG_PRBS_INV_MSK, invPrbs); 
    if (result) return result;        

    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_ESSI_RSDM_BIT_CFG_PRBS_RAW_MODE_MSK, rawMode); 
    if (result) return result;        

    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_ESSI_RSDM_BIT_CFG_PRBS_EN_MSK, enable); 
    if (result) return result;        

    result = sysHyPhy20gWrite(fileHndl, regAddr, prbsCfg);
    if (result) return result;             

    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiPrbsRxConfig */                                  

/*******************************************************************************
**
**  hyPhy20gEssiPrbsTxConfig
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function configures the PRBS generator within the TSSI
**                 block of the ESSI subsystem.  This function will also reset
**                 the PRBS LFSR registers. 
**
**  INPUTS:       *fileHndl     - base address
**                slice         - ESSI Slice Instance
**                                0 to 3 
**                link          - ESSI Link within Slice Instance
**                                0 to 7
**                patType       - 1 - PRBS23 Pattern (x^23 + x^18 + 1)
**                              - 0 - PRBS7 pattern (x^7 + x^6 + 1)
**                invPrbs       - 1 - PRBS pattern is inverted
**                              - 0 - PRBS pattern is not inverted
**                rawMode       - 1 - Unframed PRBS generation
**                              - 0 - Framed PRBS generation
**                enable        - 1 - PRBS Generator is enabled
**                              - 0 - PRBS Generator is disabled
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiPrbsTxConfig(struct file *fileHndl, UINT4 slice, 
                              UINT4 link, UINT4 patType,
                              UINT4 invPrbs, UINT4 rawMode,
                              UINT4 enable)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 prbsCfg;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
         
   /* argument checking */
    if ((slice > 3)||(link > 7)||(patType > 1)||(invPrbs > 1)||(rawMode > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    regAddr = HYPHY20G_ESSI_TSSI_REG_CFG;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_TSSI_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    /* Disable PRBS Generator Before configuring */
    offset = HYPHY20G_ESSI_TSSI_BIT_CFG_PRBS_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Reset PRBS LSFR registers before configuring */
    offset = HYPHY20G_ESSI_TSSI_BIT_CFG_PRBS_RESET_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure Parameters */
    result = sysHyPhy20gRead(fileHndl, regAddr, &prbsCfg);
    if (result) return result;
           
    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_ESSI_TSSI_BIT_CFG_PRBS_23_POLY_MSK, patType); 
    if (result) return result;        

    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_ESSI_TSSI_BIT_CFG_PRBS_INV_MSK, invPrbs); 
    if (result) return result;        

    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_ESSI_TSSI_BIT_CFG_PRBS_RAW_MODE_MSK, rawMode); 
    if (result) return result;        

    result = hyPhy20gVarFieldWrite(&prbsCfg, 
        HYPHY20G_ESSI_TSSI_BIT_CFG_PRBS_EN_MSK, enable); 
    if (result) return result;        

    result = sysHyPhy20gWrite(fileHndl, regAddr, prbsCfg);
    if (result) return result;             
      
    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiPrbsTxConfig */                

/*******************************************************************************
**
**  hyPhy20gEssiPrbsRxResync
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function will issue a 'resync' command to a PRBS 
**                 monitor within the ESSI subsystem.  
**                 
**
**  INPUTS:       *fileHndl     - base address
**                slice         - ESSI Slice Instance
**                                0 to 3 
**                link          - ESSI Link within Slice Instance
**                                0 to 7
**                 
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiPrbsRxResync(struct file *fileHndl, UINT4 slice, 
                             UINT4 link)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
         
   /* argument checking */
    if ((slice > 3)||(link > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    regAddr = HYPHY20G_ESSI_RSDM_REG_CFG;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    /* Set PRBS_RESYNC bit to '1' */
    offset = HYPHY20G_ESSI_RSDM_BIT_CFG_PRBS_RESYNC_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiPrbsRxResync */ 

/*******************************************************************************
**
**  hyPhy20gTsePrbsTxErrIns
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function will inject a single error on a PRBS 
**                 generator within the ESSI subsystem.  The error is 
**                 injected by inverting the 8th bit of the next byte to be sent.
**
**  INPUTS:       *fileHndl     - base address
**                slice         - ESSI Slice Instance
**                                0 to 3 
**                link          - ESSI Link within Slice Instance
**                                0 to 7
**                 
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTsePrbsTxErrIns(struct file *fileHndl, UINT4 slice, 
                             UINT4 link)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
         
   /* argument checking */
    if ((slice > 3)||(link > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    regAddr = HYPHY20G_ESSI_TSSI_REG_CFG;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_TSSI_TSB_SLICE_REG_OFFSET(regAddr, link);

    /* Set PRBS_SBE bit to '1' */
    offset = HYPHY20G_ESSI_TSSI_BIT_CFG_PRBS_SBE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePrbsTxErrIns */ 

/*******************************************************************************
**
**  hyPhy20gEssiPrbsRxStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of the PRBS monitor within
**                  the RSDM block of the ESSI subsystem.
**
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentioal
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**                  
**  
**
**  INPUTS:       *fileHndl    - base address
**                slice        - ESSI Slice Instance
**                                0 to 3 
**                link         - ESSI Link within Slice Instance
**                                0 to 7
**                intClear     - 1 - all *_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                             - 0 - all *_I interrupt bits accessed by this  
**                               function will be left untouched.
**                errCntUpdate - 1 - The PRBS error count reported by 
**                                   *errorCount will be newly latched by 
**                                   writing to the PRBS_ERR_CNT register field.
**                             - 0 - The PRBS error count reported by 
**                                   *errorCount will be the previously stored 
**                                   version of the PRBS_ERR_CNT register field.
**                *statusBits  - Pointer to storage for status bits
**                               Bit 2 - PRBS_SYNC_V
**                               Bit 1 - PRBS_SYNC_I
**                               Bit 0 - PRBS_ERR_I
**                *errorCount  - Pointer to storage for 8-bit prbs Error count
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiPrbsRxStatus(struct file *fileHndl, UINT4 slice, 
                               UINT4 link, UINT4 intClear, UINT4 errCntUpdate,
                               UINT4 *statusBits, UINT4 *errorCount)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;
    UINT4 prbsSyncV;
    UINT4 prbsSyncI;
    UINT4 prbsErrI;
    UINT4 prbsErrReg;
    UINT4 prbsIntSt;
    UINT4 resultMask = 0;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

         
   /* argument checking */
    if ((slice > 3)||(link > 7)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    regAddr = HYPHY20G_ESSI_RSDM_REG_PRBS_ERR_CNT;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    if (errCntUpdate) {
    		/* Trigger the counter update */
    		result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
    		if (result) return result;             
    
    		/* make sure the TIP_PRBS bit clears */
    		result = sysHyPhy20gPollBit(fileHndl, regAddr, 
                              HYPHY20G_ESSI_RSDM_BIT_PRBS_ERR_CNT_TIP_PRBS_MSK, 0, 
                              HYPHY20G_DELAY_20USEC);                                                       
    		if (result) return result;
    }
      
    /* Get PRBS error count */
    result = sysHyPhy20gRead(fileHndl, regAddr, &prbsErrReg);
    if (result) return result;     
      
    mask = HYPHY20G_ESSI_RSDM_BIT_PRBS_ERR_CNT_PRBS_ERR_CNT_MSK;       
    offset = HYPHY20G_ESSI_RSDM_BIT_PRBS_ERR_CNT_PRBS_ERR_CNT_OFF; 
    *errorCount = mHYPHY20G_GET_FIELD4(prbsErrReg, mask, offset);
    
    /* Get PRBS SYNCV Status */
    regAddr = HYPHY20G_ESSI_RSDM_REG_STAT;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    offset = HYPHY20G_ESSI_RSDM_BIT_STAT_PRBS_SYNC_V_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &prbsSyncV);
    if (result) return result;    
   
    /* Get PRBS SYNCI Status */
    regAddr = HYPHY20G_ESSI_RSDM_REG_INT_STAT;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    result = sysHyPhy20gRead(fileHndl, regAddr, &prbsIntSt);
    if (result) return result;    

    mask = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_PRBS_SYNC_I_MSK;       
    offset = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_PRBS_SYNC_I_OFF; 
    prbsSyncI = mHYPHY20G_GET_FIELD4(prbsIntSt, mask, offset);

    /* Get PRBS ERRI Status */
    mask = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_PRBS_ERR_I_MSK;       
    offset = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_PRBS_ERR_I_OFF; 
    prbsErrI = mHYPHY20G_GET_FIELD4(prbsIntSt, mask, offset);

    /* Clear interrupts if enabled */
    if (intClear) {
        resultMask = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_PRBS_SYNC_I_MSK +
                     HYPHY20G_ESSI_RSDM_BIT_INT_STAT_PRBS_ERR_I_MSK;
        result = sysHyPhy20gWrite(fileHndl, regAddr, resultMask);
        if (result) return result;   
    } 
          
    /* Compute results */
    *statusBits = (prbsSyncV << 2) + (prbsSyncI << 1) + prbsErrI;
    
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiPrbsRxStatus */


/*******************************************************************************
**
**  hyPhy20gEssiAisPatEnable
**  ___________________________________________________________________________ 
**
**  DESCRIPTION: This function enables or disables the insertion of the AIS 
**               pattern into the output data stream when the RSDM is
**               out-of-frame alignment. 
**               When the ingress link is disabled, this function need to be 
**               called for enable = 1 so that AIS consequential action can be 
**               manually forced into the data stream by calling function 
**               hyPhy20gEssiForceAisEnable with relative arguments.            
**  
**
**  INPUTS:      *fileHndl     - base address
**                slice        - ESSI Slice Instance
**                               0 to 3 
**                link         - ESSI Link within Slice Instance
**                               0 to 7
**                enable       - 1 - enable
**                             - 0 - disable
**
**  OUTPUTS:      None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiAisPatEnable(struct file *fileHndl, UINT4 slice, UINT4 link, 
                              UINT4 enable)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 msk;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
  
    /* argument checking */
    if ((slice > 3)||(link > 7)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    /* configure AIS_PAT_EN bit as required */ 
    regAddr = HYPHY20G_ESSI_RSDM_REG_AIS_CFG;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    msk = HYPHY20G_ESSI_RSDM_BIT_AIS_CFG_AIS_PAT_EN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, enable);
    if (result) return result;
           
    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiAisPatEnable */
/*******************************************************************************
**
**  hyPhy20gEssiForceAisEnable
**  ___________________________________________________________________________ 
**
**  DESCRIPTION: This function enables or disables the entire frame is 
**               overwritten with the pattern independent of the frame alignment
**               state. When the ingress link is disabled, this function can be 
**               used to manually insert the AIS consequential action into the 
**               data stream.
**               before to enables the entire frame is constrainedly overwritten
**               with the pattern, AIS_PAT_EN must be set to logic 1.                  
**  
**
**  INPUTS:      *fileHndl     - base address
**                slice        - ESSI Slice Instance
**                               0 to 3 
**                link         - ESSI Link within Slice Instance
**                               0 to 7
**                enable       - 1 - enable
**                             - 0 - disable
**
**  OUTPUTS:      None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiForceAisEnable(struct file *fileHndl, UINT4 slice, UINT4 link,
                                UINT4 enable)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 msk;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
         
    /* argument checking */
    if ((slice > 3)||(link > 7)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    /* configure FORCE_AIS bit as required */ 
    regAddr = HYPHY20G_ESSI_RSDM_REG_AIS_CFG;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    msk = HYPHY20G_ESSI_RSDM_BIT_AIS_CFG_FORCE_AIS_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, enable);
    if (result) return result;
           
    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiForceAisEnable */


