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
**  FILE        : hyphy20g_essi_status.c
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
#include "hyphy20g_essi_status.h"

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
**  hyPhy20gEssiGetFrmrStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of framer interrupts within
**                  the RSDM block of the ESSI subsystem
**
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentioal
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**  INPUTS:       *fileHndl     - base address
**                slice         - ESSI Slice Instance
**                                0 to 3 
**                link          - ESSI Link within Slice Instance
**                                0 to 7
**                intClear      - 1 - all *_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *_I interrupt bits accessed by this  
**                               function will be left untouched.
**                errCntUpdate - 1 - The BIP-8 error count reported by 
**                                   *errorCount will be newly latched by 
**                                   writing to the BIP8_ERR_CNT register field.
**                             - 0 - The BIP-8 error count reported by 
**                                   *errorCount will be the previously stored 
**                                   version of the BIP8_ERR_CNT register field.
**                *statusBits   - Pointer to storage for status bits
**                               Bit 7 - BIP8_ERR_I 
**                               Bit 6 - FIFO_ERR_I
**                               Bit 5 - DC_BAL_ERR_I 
**                               Bit 4 - CID_ERR_I
**                               Bit 3 - LOW_TRAN_I 
**                               Bit 2 - LOA_I
**                               Bit 1 - OOF_ESSI_I
**                               Bit 0 - OOF_ESSI_V
**                *errorCount   - Pointer to storage for 8-bit BIP-8 error count
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
INT4 hyPhy20gEssiGetFrmrStatus(struct file *fileHndl, UINT4 slice, 
                               UINT4 link, UINT4 intClear, UINT4 errCntUpdate,
                               UINT4 *statusBits, UINT4 *errorCount)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset; 
    UINT4 oofEssiSt;
    UINT4 rsdmIntSt;
    UINT4 resultMask = 0;
    UINT4 bipErrReg;
    UINT4 bip8ErrI, fifoErrI, oofEssiI, loaI, dcBalErrI, cidErrI, lowTranI;
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

    regAddr = HYPHY20G_ESSI_RSDM_REG_BIP_8_ERR_CNT;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    if (errCntUpdate) {
		    /* Trigger the counter update */
    		result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
    		if (result) return result;             
    
    		/* make sure the TIP_BIP8 bit clears */
    		result = sysHyPhy20gPollBit(fileHndl, regAddr, 
                              HYPHY20G_ESSI_RSDM_BIT_BIP_8_ERR_CNT_TIP_BIP8_MSK, 0, 
                              HYPHY20G_DELAY_20USEC);                                                       
    		if (result) return result;
    }
      
    /* Get BIP8 error count */
    result = sysHyPhy20gRead(fileHndl, regAddr, &bipErrReg);
    if (result) return result;     
      
    mask = HYPHY20G_ESSI_RSDM_BIT_BIP_8_ERR_CNT_BIP8_ERR_CNT_MSK;       
    offset = HYPHY20G_ESSI_RSDM_BIT_BIP_8_ERR_CNT_BIP8_ERR_CNT_OFF; 
    *errorCount = mHYPHY20G_GET_FIELD4(bipErrReg, mask, offset);
    
    /* Get OOF_ESSI_V Status */
    regAddr = HYPHY20G_ESSI_RSDM_REG_STAT;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    offset = HYPHY20G_ESSI_RSDM_BIT_STAT_OOF_ESSI_V_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &oofEssiSt);
    if (result) return result;    
      
    /* Get BIP8 ERRI Status */
    regAddr = HYPHY20G_ESSI_RSDM_REG_INT_STAT;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    result = sysHyPhy20gRead(fileHndl, regAddr, &rsdmIntSt);
    if (result) return result;    

    mask = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_BIP8_ERR_I_MSK;       
    offset = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_BIP8_ERR_I_OFF; 
    bip8ErrI = mHYPHY20G_GET_FIELD4(rsdmIntSt, mask, offset);

    /* Get FIFO ERRI Status */
    mask = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_FIFO_ERR_I_MSK;       
    offset = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_FIFO_ERR_I_OFF; 
    fifoErrI = mHYPHY20G_GET_FIELD4(rsdmIntSt, mask, offset);

    /* Get OOF ESSI_I Status */
    mask = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_OOF_ESSI_I_MSK;       
    offset = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_OOF_ESSI_I_OFF; 
    oofEssiI = mHYPHY20G_GET_FIELD4(rsdmIntSt, mask, offset);

    /* Get LOA_I Status */
    mask = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_LOA_I_MSK;       
    offset = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_LOA_I_OFF; 
    loaI = mHYPHY20G_GET_FIELD4(rsdmIntSt, mask, offset);

    /* Get DC_BAL_ERR_I Status */
    mask = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_DC_BAL_ERR_I_MSK;       
    offset = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_DC_BAL_ERR_I_OFF; 
    dcBalErrI = mHYPHY20G_GET_FIELD4(rsdmIntSt, mask, offset);

    /* Get CID_ERR_I Status */
    mask = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_CID_ERR_I_MSK;       
    offset = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_CID_ERR_I_OFF; 
    cidErrI = mHYPHY20G_GET_FIELD4(rsdmIntSt, mask, offset);

    /* Get LOW_TRAN_I Status */
    mask = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_LOW_TRAN_I_MSK;       
    offset = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_LOW_TRAN_I_OFF; 
    lowTranI = mHYPHY20G_GET_FIELD4(rsdmIntSt, mask, offset);
   
    /* Clear interrupts if enabled */
    if (intClear) {
        resultMask = HYPHY20G_ESSI_RSDM_BIT_INT_STAT_BIP8_ERR_I_MSK +
                     HYPHY20G_ESSI_RSDM_BIT_INT_STAT_FIFO_ERR_I_MSK +
                     HYPHY20G_ESSI_RSDM_BIT_INT_STAT_OOF_ESSI_I_MSK +
                     HYPHY20G_ESSI_RSDM_BIT_INT_STAT_LOA_I_MSK +
                     HYPHY20G_ESSI_RSDM_BIT_INT_STAT_DC_BAL_ERR_I_MSK +
                     HYPHY20G_ESSI_RSDM_BIT_INT_STAT_CID_ERR_I_MSK +
                     HYPHY20G_ESSI_RSDM_BIT_INT_STAT_LOW_TRAN_I_MSK;
        result = sysHyPhy20gWrite(fileHndl, regAddr, resultMask);
        if (result) return result;   
    } 

    /* Compute results */
    *statusBits = (bip8ErrI << 7) + (fifoErrI << 6) +
                  (dcBalErrI << 5) + (cidErrI << 4) + (lowTranI << 3) +
                  (loaI << 2) + (oofEssiI << 1) + oofEssiSt;
                  
    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiGetFrmrStatus */



/*******************************************************************************
**
**  hyPhy20gEssiGetTssiFifoStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of the Tx FIFO within the
**                  TSSI block of the ESSI subsystem.
**  
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentioal
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**  INPUTS:       *fileHndl    - base address
**                slice         - ESSI Slice Instance
**                                0 to 3 
**                link          - ESSI Link within Slice Instance
**                                0 to 7
**                intClear      - 1 - all *_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *_I interrupt bits accessed by this  
**                               function will be left untouched.
**               *fifoStatus    - Pointer to storage for FIFO status bit
**                               Bit 0 - FIFO_ERR_I
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiGetTssiFifoStatus(struct file *fileHndl, UINT4 slice, 
                                   UINT4 link, UINT4 intClear, UINT4 *fifoStatus)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;    
    UINT4 resultMask = 0;
    UINT4 fifoErrI;
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

    /* Get FIFO_ERR_I Status */
    regAddr = HYPHY20G_ESSI_TSSI_REG_INT_STAT;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_TSSI_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    offset = HYPHY20G_ESSI_TSSI_BIT_INT_STAT_FIF0_ERRI_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &fifoErrI);
    if (result) return result;    
      
    /* Clear interrupts if enabled */
    if (intClear) {
        resultMask = HYPHY20G_ESSI_TSSI_BIT_INT_STAT_FIF0_ERRI_MSK;
        result = sysHyPhy20gWrite(fileHndl, regAddr, resultMask);
        if (result) return result;   
    } 

    /* Compute results */
    *fifoStatus = fifoErrI;   

    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiGetTssiFifoStatus */
