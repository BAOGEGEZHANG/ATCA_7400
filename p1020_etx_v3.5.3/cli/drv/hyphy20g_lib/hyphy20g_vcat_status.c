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
**  FILE        : hyphy20g_vcat_status.c
**
**  $Date: 2011-11-08 10:46:38 +0800 (Tue, 08 Nov 2011) $
**
**  $Revision: 16843 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_vcat_status.h"

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
**  hyPhy20gVcatRxOtnFailStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the status of ODU fail indications
**                  within the RVCPO block of the VCAT subsystem.
**                  
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentioal
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 - Slice 'A' (RVCPO_0)
**                              - 1 - Slice 'B' (RVCPO_1)
**                  otuNum      - Selects which OTU within the  
**                                RVCPO to query 
**                                - 0-3 in 4xOTU1 mode
**                                - 0 in OTU2 mode
**                  intClear    - 1 - all *_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *_I interrupt bits accessed by this  
**                               function will be left untouched.
**                  *statusBits - Pointer to storage for status bits
**                                      Bit 1 - ODU_FAIL_I
**                                      Bit 0 - ODU_FAIL_V
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatRxOtnFailStatus(struct file *fileHndl, UINT4 slice, 
                                 UINT4 otuNum, UINT4 intClear,
                                 UINT4 *statusBits)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 oduFailV;
    UINT4 oduFailI;
             
   /* argument checking */
    if ((slice > 1)||(otuNum > 3)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Get ODU_FAIL_V Status */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_INT_VAL;
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);    
    result = sysHyPhy20gBitRead(fileHndl, regAddr, otuNum, &oduFailV);
    if (result) return result;    
      
    /* Get ODU_FAIL_I Status */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_INT;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);    
    result = sysHyPhy20gBitRead(fileHndl, regAddr, (otuNum+28), &oduFailI);
    if (result) return result;    

    /* Clear interrupts if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, (otuNum+28), 1);
        if (result) return result;   
    } 

    /* Compute results */
    *statusBits = (oduFailI << 1) + oduFailV;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatRxOtnFailStatus */  

/*******************************************************************************
**
**  hyPhy20gVcatRxOtnVcatLinkStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the status of an OTU1 VCAT enabled
**                  link within the RVCPO block of the VCAT subsystem.
**                  
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 - Slice 'A' (RVCPO_0)
**                              - 1 - Slice 'B' (RVCPO_1)
**                  otu1Num     - Selects which OTU1 within the  
**                                RVCPO to query 
**                              - valid values are 0-3
**                  *statusBits - Pointer to storage for status bits
**                                      Bit 7 - DELAY_ACCEPT
**                                      Bit 6 - LINK_ST_OK
**                                      Bit 5 - FG_PKT_GOOD
**                                      Bit 4 - FG_ACTIVE
**                                      Bit 3 - FG_SEQ_VALID
**                                      Bits 2:1 - MF_SYNC_STATE
**                                      Bit 0 - AIS_STATUS
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatRxOtnVcatLinkStatus(struct file *fileHndl, UINT4 slice, 
                                     UINT4 otu1Num, UINT4 *statusBits)
{
    INT4 result;
    UINT4 offset, mask;
    UINT4 dlyAcc, lnkStOk, fgPktGd, fgActv;
    UINT4 fgSqVal, mfSyncSt, aisSt;
    UINT4 data0, data1, data2, data3, data4;
 
             
   /* argument checking */
    if ((slice > 1)||(otu1Num > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    result = vcatHyPhy20gRVCPOMemIndRead(fileHndl, slice, 0x2, 0x2*otu1Num, &data0, 
                                         &data1, &data2, &data3, &data4);
    if (result) return result;
    
    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_CXT_TABLE_IBIT_LNK_CTXT_WORD0_TBL_DELAY_ACCEPT_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_LINK_CXT_TABLE_IBIT_LNK_CTXT_WORD0_TBL_DELAY_ACCEPT_OFF;
    dlyAcc = mHYPHY20G_GET_FIELD4(data1, mask, offset);
    
    result = vcatHyPhy20gRVCPOMemIndRead(fileHndl, slice, 0x2, (0x2*otu1Num)+1, &data0, 
                                         &data1, &data2, &data3, &data4);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_CXT_TABLE_IBIT_LNK_CTXT_WORD1_TBL_LINK_ST_OK_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_LINK_CXT_TABLE_IBIT_LNK_CTXT_WORD1_TBL_LINK_ST_OK_OFF;
    lnkStOk = mHYPHY20G_GET_FIELD4(data1, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_CXT_TABLE_IBIT_LNK_CTXT_WORD1_TBL_FG_PKT_GOOD_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_LINK_CXT_TABLE_IBIT_LNK_CTXT_WORD1_TBL_FG_PKT_GOOD_OFF;
    fgPktGd = mHYPHY20G_GET_FIELD4(data1, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_CXT_TABLE_IBIT_LNK_CTXT_WORD1_TBL_FG_ACTIVE_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_LINK_CXT_TABLE_IBIT_LNK_CTXT_WORD1_TBL_FG_ACTIVE_OFF;
    fgActv = mHYPHY20G_GET_FIELD4(data1, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_CXT_TABLE_IBIT_LNK_CTXT_WORD1_TBL_FG_SQ_VALID_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_LINK_CXT_TABLE_IBIT_LNK_CTXT_WORD1_TBL_FG_SQ_VALID_OFF;
    fgSqVal = mHYPHY20G_GET_FIELD4(data1, mask, offset);
    
    result = vcatHyPhy20gRVCPOMemIndRead(fileHndl, slice, 0xB, otu1Num, &data0, 
                                         &data1, &data2, &data3, &data4);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCPO_CTRL_PKT_CONTEXT_TABLE_IBIT_CTL_PKT_CTXT_TBL_MF_SYNC_STATE_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_CTRL_PKT_CONTEXT_TABLE_IBIT_CTL_PKT_CTXT_TBL_MF_SYNC_STATE_OFF;
    mfSyncSt = mHYPHY20G_GET_FIELD4(data2, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCPO_CTRL_PKT_CONTEXT_TABLE_IBIT_CTL_PKT_CTXT_TBL_AIS_STATUS_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_CTRL_PKT_CONTEXT_TABLE_IBIT_CTL_PKT_CTXT_TBL_AIS_STATUS_OFF;
    aisSt = mHYPHY20G_GET_FIELD4(data1, mask, offset);
    
    /* Compute results */
    *statusBits = (dlyAcc << 7) +
                  (lnkStOk << 6) + (fgPktGd << 5) + (fgActv << 4) +
                  (fgSqVal << 3) + (mfSyncSt << 1) + aisSt;
 
     return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatRxOtnVcatLinkStatus */  


/*******************************************************************************
**
**  hyPhy20gVcatRxOtnVcatLinkIntStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the interrupt status of an OTU1 VCAT 
**                  enabled link  within the RVCPO block of the VCAT subsystem.
**                  
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentioal
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**  INPUTS:         *fileHndl     - base address
**                  slice         - 0 - Slice 'A' (RVCPO_0)
**                                - 1 - Slice 'B' (RVCPO_1)
**                  otu1Num       - Selects which OTU1 within the  
**                                 RVCPO to query 
**                                - valid values are 0-3
**                  intClear      - 1 - all *_I interrupt bits accessed by this 
**                                function will be cleared by writing them to '1'
**                                - 0 - all *_I interrupt bits accessed by this  
**                                function will be left untouched.
**                  *statusBits   - Pointer to storage for status bits
**                                      Bit 7 - ACCEPT_CHANGE_I
**                                      Bit 6 - AIS_CHANGE_I
**                                      Bit 5 - MF_SYNC_CHANGE_I
**                                      Bit 4 - CTRL_CHANGE_I
**                                      Bit 3 - CTRL_PKT_CRC_I
**                                      Bit 2 - CTRL_ERR_I
**                                      Bit 1 - ACTIVE_CHANGE_I
**                                      Bit 0 - LINK_ST_CHANGE_I
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatRxOtnVcatLinkIntStatus(struct file *fileHndl, UINT4 slice, 
                                        UINT4 otu1Num, UINT4 intClear, 
                                        UINT4 *statusBits)
{
    INT4 result;
    UINT4 offset, mask;
    UINT4 accChI, aisChI, mfsChI, ctrlChI;
    UINT4 ctrlCrcI, ctrlErrI, actChI, linkStChI;
    UINT4 data0;
    UINT4 resultMask = 0;
 
             
   /* argument checking */
    if ((slice > 1)||(otu1Num > 3)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    result = vcatHyPhy20gRVCPOIntIndRead(fileHndl, slice, 0x0, otu1Num, &data0);
    if (result) return result;
    
    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_ACCEPT_CHANGE_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_ACCEPT_CHANGE_I_OFF;
    accChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);
    
    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_AIS_CHANGE_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_AIS_CHANGE_I_OFF;
    aisChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_MF_SYNC_CHANGE_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_MF_SYNC_CHANGE_I_OFF;
    mfsChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_CTRL_CHANGE_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_CTRL_CHANGE_I_OFF;
    ctrlChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_CTRL_PKT_CRC_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_CTRL_PKT_CRC_I_OFF;
    ctrlCrcI = mHYPHY20G_GET_FIELD4(data0, mask, offset);
    
    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_CTRL_ERR_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_CTRL_ERR_I_OFF;
    ctrlErrI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_ACTIVE_CHANGE_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_ACTIVE_CHANGE_I_OFF;
    actChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_LINK_ST_CHANGE_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_LINK_ST_CHANGE_I_OFF;
    linkStChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    /* Clear interrupts if enabled */
    if (intClear) {
        resultMask = HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_ACCEPT_CHANGE_I_MSK +
                     HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_AIS_CHANGE_I_MSK +
                     HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_MF_SYNC_CHANGE_I_MSK +
                     HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_CTRL_CHANGE_I_MSK +
                     HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_CTRL_PKT_CRC_I_MSK +
                     HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_CTRL_ERR_I_MSK +
                     HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_ACTIVE_CHANGE_I_MSK +
                     HYPHY20G_VCAT_LCAS_RVCPO_LINK_INT_STAT_TABLE_IBIT_LNK_INT_STAT_TBL_LINK_ST_CHANGE_I_MSK;
        result = vcatHyPhy20gRVCPOIntIndWrite(fileHndl, slice, 0x0, otu1Num, resultMask);
        if (result) return result;   
    } 
    
    /* Compute results */
    *statusBits = (accChI << 7) +(aisChI << 6) +
                  (mfsChI << 5) + (ctrlChI << 4) + (ctrlCrcI << 3) +
                  (ctrlErrI << 2) + (actChI << 1) + linkStChI;
 
    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatRxOtnVcatLinkIntStatus */  


/*******************************************************************************
**
**  hyPhy20gVcatTxSonetVcatGrpIntStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the interrupt status of an SONET-based 
**                  VCAT group within the TVCP192 block of the VCAT subsystem.
**                  
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentioal
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**  INPUTS:         *fileHndl     - base address
**                  grpId         - VCAT Group ID to query
**                                  valid values are 0x0 - 0x3F (0-63)
**                  intClear      - 1 - all *_I interrupt bits accessed by this 
**                                function will be cleared by writing them to '1'
**                                - 0 - all *_I interrupt bits accessed by this  
**                                function will be left untouched.
**                  *statusBits   - Pointer to storage for status bits
**                                      Bit 2 - GRP_N_FE_RSACK_TIMEOUT_I
**                                      Bit 1 - GRP_N_RESEQ_DONE_I
**                                      Bit 0 - GRP_N_RESEQ_DONE_V
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatTxSonetVcatGrpIntStatus(struct file *fileHndl, UINT4 grpId,
                                         UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 ackToI, resDoneI, resDoneV;
             
   /* argument checking */
    if ((grpId > 0x3F)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Get GRP_N_RESEQ_DONE_V Status */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_GRP2_INT_STAT_31_0_V + (0xC*(grpId/32));
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &resDoneV);
    if (result) return result;    
      
    /* Get GRP_N_RESEQ_DONE_I Status */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_GRP2_INT_STAT_31_0_I + (0xC*(grpId/32));
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &resDoneI);
    if (result) return result;    

    /* Clear interrupt if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    } 

    /* Get GRP_N_FE_RSACK_TIMEOUT_I Status */
    regAddr = HYPHY20G_VCAT_LCAS_TVCP_REG_GRP_INT_STAT_31_0_I + (0x8*(grpId/32));
    offset = grpId%32;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &ackToI);
    if (result) return result;    

    /* Clear interrupts if enabled */
    if (intClear) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;   
    } 

    /* Compute results */
    *statusBits = (ackToI << 1) + (resDoneI << 1) + resDoneV;
    
    return HYPHY20G_SUCCESS;

} /* hyPhy20gVcatTxSonetVcatGrpIntStatus */  

/*******************************************************************************
**
**  hyPhy20gVcatTxSonetVcatMemberIntStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the interrupt status of a SONET-based 
**                  VCAT group member within the TVCP192 block of the VCAT subsystem.
**                  
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentioal
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels 
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3 
**                  sts12Num    - selects one of the 4 possible offsets for 
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3 
**                  sts3Num     - selects one of the 4 possible offsets for 
**                                this STS-3 within sts12Num
**                              - valid values are 0x0 - 0x3
**                  sts1Num     - selects one of the 3 possible offsets for 
**                                this STS-1 within sts3Num (for sts3c Members, 
**                                set this value to 0x0)
**                              - valid values are 0x0 - 0x2
**                  intClear    - 1 - all *_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *_I interrupt bits accessed by this  
**                                function will be left untouched.
**                  *statusBits - Pointer to storage for status bits
**                                      Bit 2 - LINK_UNEXPECTED_MST_OK_INT
**                                      Bit 2 - LINK_ADD_TIMEOUT_INT
**                                      Bit 1 - LINK_IN_OUT_INT
**                                      Bit 0 - LINK_CONTROL_CHANGE_INT
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatTxSonetVcatMemberIntStatus(struct file *fileHndl,
                                            UINT4 sts48Num, UINT4 sts12Num,
                                            UINT4 sts3Num, UINT4 sts1Num,
                                            UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;
    UINT4 offset, mask;
    UINT4 unexMstI, addToI, linkInOutI, ctrlChI;
    UINT4 data0;
    UINT4 resultMask = 0;
    UINT4 memberAddr;
 
             
   /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 3)||(sts3Num > 0x3)||(sts1Num > 0x2)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    memberAddr = 48*sts48Num + 12*sts12Num + 3*sts3Num + sts1Num;
    
    result = vcatHyPhy20gTVCPIntIndRead(fileHndl, memberAddr, &data0);
    if (result) return result;
    
    mask = HYPHY20G_VCAT_LCAS_TVCP_MEM_INT_STAT_TBL_IBIT_MMBR_INT_STAT_TBL_LINK_UNEXPECTED_MST_OK_INT_MSK;
    offset = HYPHY20G_VCAT_LCAS_TVCP_MEM_INT_STAT_TBL_IBIT_MMBR_INT_STAT_TBL_LINK_UNEXPECTED_MST_OK_INT_OFF;
    unexMstI = mHYPHY20G_GET_FIELD4(data0, mask, offset);
    
    mask = HYPHY20G_VCAT_LCAS_TVCP_MEM_INT_STAT_TBL_IBIT_MMBR_INT_STAT_TBL_LINK_ADD_TIMEOUT_INT_MSK;
    offset = HYPHY20G_VCAT_LCAS_TVCP_MEM_INT_STAT_TBL_IBIT_MMBR_INT_STAT_TBL_LINK_ADD_TIMEOUT_INT_OFF;
    addToI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_TVCP_MEM_INT_STAT_TBL_IBIT_MMBR_INT_STAT_TBL_LINK_IN_OUT_INT_MSK;
    offset = HYPHY20G_VCAT_LCAS_TVCP_MEM_INT_STAT_TBL_IBIT_MMBR_INT_STAT_TBL_LINK_IN_OUT_INT_OFF;
    linkInOutI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_TVCP_MEM_INT_STAT_TBL_IBIT_MMBR_INT_STAT_TBL_LINK_CONTROL_CHANGE_INT_MSK;
    offset = HYPHY20G_VCAT_LCAS_TVCP_MEM_INT_STAT_TBL_IBIT_MMBR_INT_STAT_TBL_LINK_CONTROL_CHANGE_INT_OFF;
    ctrlChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);


    /* Clear interrupts if enabled */
    if (intClear) {
        resultMask = HYPHY20G_VCAT_LCAS_TVCP_MEM_INT_STAT_TBL_IBIT_MMBR_INT_STAT_TBL_LINK_CONTROL_CHANGE_INT_MSK +
                     HYPHY20G_VCAT_LCAS_TVCP_MEM_INT_STAT_TBL_IBIT_MMBR_INT_STAT_TBL_LINK_IN_OUT_INT_MSK +
                     HYPHY20G_VCAT_LCAS_TVCP_MEM_INT_STAT_TBL_IBIT_MMBR_INT_STAT_TBL_LINK_ADD_TIMEOUT_INT_MSK +
                     HYPHY20G_VCAT_LCAS_TVCP_MEM_INT_STAT_TBL_IBIT_MMBR_INT_STAT_TBL_LINK_UNEXPECTED_MST_OK_INT_MSK;
        result = vcatHyPhy20gTVCPIntIndWrite(fileHndl, memberAddr, resultMask);
        if (result) return result;   
    } 
    
    /* Compute results */
    *statusBits = (unexMstI << 3) + (addToI << 2) + 
                  (linkInOutI << 1) + ctrlChI;
 
    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatTxSonetVcatMemberIntStatus */  

/*******************************************************************************
**
**  hyPhy20gVcatRxSonetVcatMemberStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the status of a SONET-based 
**                  VCAT group member within the RVCP192 block of the VCAT subsystem.
**                  
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels 
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3 
**                  sts12Num    - selects one of the 4 possible offsets for 
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3 
**                  sts3Num     - selects one of the 4 possible offsets for 
**                                this STS-3 within sts12Num
**                              - valid values are 0x0 - 0x3
**                  sts1Num     - selects one of the 3 possible offsets for 
**                                this STS-1 within sts3Num (for sts3c Members, 
**                                set this value to 0x0)
**                              - valid values are 0x0 - 0x2
**                  *statusBits  - Pointer to storage for status bits
**                                      Bit 22 - AIS_IND
**                                      Bit 21:20 - MF_SYNC_STATE
**                                      Bit 19 - DELAY_ACCEPT
**                                      Bit 18 - IW_MODE_VALID
**                                      Bit 17 - IW_MODE
**                                      Bit 16 - LINK_ST_OK_NO_DEFECT
**                                      Bit 15 - LINK_ST_OK
**                                      Bit 14 - FG_PKT_GOOD
**                                      Bit 13 - FG_ACTIVE
**                                      Bit 12 - FG_SQ_VALID
**                                      Bit 11:8 - FG_CTRL
**                                      Bits 7:0 - FG_SQ
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatRxSonetVcatMemberStatus(struct file *fileHndl,
                                         UINT4 sts48Num, UINT4 sts12Num,
                                         UINT4 sts3Num, UINT4 sts1Num,
                                         UINT4 *statusBits)
{
    INT4 result;
    UINT4 offset, mask;
    UINT4 aisInd, mfSyncSt, dlyAcc, iwMdVal; 
    UINT4 iwMd, lnkStOkNoDfct, lnkStOk, fgPktGd; 
    UINT4 fgAct, fgSqVal, fgCtrl, fgSq;
    UINT4 data0, data1, data2, data3, data4, data5, data6, data7;
    UINT4 memberAddr;
 
             
   /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 3)||(sts3Num > 0x3)||(sts1Num > 0x2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    memberAddr = 64*sts48Num + 16*sts12Num + 4*sts3Num + sts1Num;
    
    result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0xD, memberAddr, &data0, 
                                         &data1, &data2, &data3, &data4, 
                                         &data5, &data6, &data7);
    if (result) return result;
    
    mask = HYPHY20G_VCAT_LCAS_RVCP_SNT_CTRL_CNTXT_MEM_IBIT_SONET_CTL_CTXT_AIS_IND_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_SNT_CTRL_CNTXT_MEM_IBIT_SONET_CTL_CTXT_AIS_IND_OFF;
    aisInd = mHYPHY20G_GET_FIELD4(data1, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_SNT_CTRL_CNTXT_MEM_IBIT_SONET_CTL_CTXT_MF_SYNC_STATE_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_SNT_CTRL_CNTXT_MEM_IBIT_SONET_CTL_CTXT_MF_SYNC_STATE_OFF;
    mfSyncSt = mHYPHY20G_GET_FIELD4(data1, mask, offset);
    
    memberAddr = 48*sts48Num + 12*sts12Num + 3*sts3Num + sts1Num;

    result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x2, 2*memberAddr, &data0, 
                                         &data1, &data2, &data3, &data4, 
                                         &data5, &data6, &data7);
    if (result) return result;
 
    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD0_MEM_DELAY_ACCEPT_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD0_MEM_DELAY_ACCEPT_OFF;
    dlyAcc = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x2, (2*memberAddr) + 1, &data0, 
                                         &data1, &data2, &data3, &data4, 
                                         &data5, &data6, &data7);
    if (result) return result;

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_IW_MODE_VALID_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_IW_MODE_VALID_OFF;
    iwMdVal = mHYPHY20G_GET_FIELD4(data1, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_IW_MODE_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_IW_MODE_OFF;
    iwMd = mHYPHY20G_GET_FIELD4(data1, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_LINK_ST_OK_NO_DEFECT_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_LINK_ST_OK_NO_DEFECT_OFF;
    lnkStOkNoDfct = mHYPHY20G_GET_FIELD4(data1, mask, offset);
    
    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_LINK_ST_OK_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_LINK_ST_OK_OFF;
    lnkStOk = mHYPHY20G_GET_FIELD4(data1, mask, offset);
    
    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_PKT_GOOD_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_PKT_GOOD_OFF;
    fgPktGd = mHYPHY20G_GET_FIELD4(data1, mask, offset);
    
    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_ACTIVE_OFF;
    fgAct = mHYPHY20G_GET_FIELD4(data1, mask, offset);
    
    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_SQ_VALID_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_SQ_VALID_OFF;
    fgSqVal = mHYPHY20G_GET_FIELD4(data1, mask, offset);
    
    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_CTRL_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_CTRL_OFF;
    fgCtrl = mHYPHY20G_GET_FIELD4(data0, mask, offset);
    
    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_SQ_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CXT_WRD0_MEM_IBIT_LNK_CTXT_WORD1_MEM_FG_SQ_OFF;
    fgSq = mHYPHY20G_GET_FIELD4(data0, mask, offset);
    
    /* Compute results */
    *statusBits = (aisInd << 22) + (mfSyncSt << 20) + (dlyAcc << 19) + 
                  (iwMdVal << 18) + (iwMd << 17) + (lnkStOkNoDfct << 16) +
                  (lnkStOk << 15) + (fgPktGd << 14) + (fgAct << 13) +
                  (fgSqVal << 12) + (fgCtrl << 8) + fgSq;
 
     return HYPHY20G_SUCCESS;
     
} /* hyPhy20gVcatRxSonetVcatMemberStatus */  

/*******************************************************************************
**
**  hyPhy20gVcatRxSonetVcatMemberIntStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the interrupt status of a SONET-based 
**                  VCAT group member within the RVCP192 block of the VCAT subsystem.
**                  
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentioal
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**  INPUTS:         *fileHndl     - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels 
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3 
**                  sts12Num    - selects one of the 4 possible offsets for 
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3 
**                  sts3Num     - selects one of the 4 possible offsets for 
**                                this STS-3 within sts12Num
**                              - valid values are 0x0 - 0x3
**                  sts1Num     - selects one of the 3 possible offsets for 
**                                this STS-1 within sts3Num (for sts3c Members, 
**                                set this value to 0x0)
**                              - valid values are 0x0 - 0x2
**                  intClear      - 1 - all *_I interrupt bits accessed by this 
**                                function will be cleared by writing them to '1'
**                                - 0 - all *_I interrupt bits accessed by this  
**                                function will be left untouched.
**                  *statusBits   - Pointer to storage for status bits
**                                      Bit 11 - IW_CHANGE_I
**                                      Bit 10 - ACCEPT_CHANGE_I
**                                      Bit 9 - AIS_CHANGE_I
**                                      Bit 8 - MF_SYNC_CHANGE_I
**                                      Bit 7 - CTRL_CHANGE_I
**                                      Bit 6 - CTRL_PKT_CRC_I
**                                      Bit 5 - CTRL_ERR_I
**                                      Bit 4 - LCAS_ERR_I
**                                      Bit 3 - SQ_MISMATCH_I
**                                      Bit 2 - SQ_INVALID_I
**                                      Bit 1 - ACTIVE_CHANGE_I
**                                      Bit 0 - LINK_ST_CHANGE_I
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatRxSonetVcatMemberIntStatus(struct file *fileHndl,
                                            UINT4 sts48Num, UINT4 sts12Num,
                                            UINT4 sts3Num, UINT4 sts1Num,
                                            UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;
    UINT4 offset, mask;
    UINT4 iwChI, accChI, aisChI, mfSyncChI, ctrlChI; 
    UINT4 ctrlCrcI, ctrlErrI, lcasErrI; 
    UINT4 sqMismI, sqInvI, actChI, lnkStChI;
    UINT4 data0;
    UINT4 memberAddr;
 
             
   /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 3)||(sts3Num > 0x3)||(sts1Num > 0x2)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    memberAddr = 48*sts48Num + 12*sts12Num + 3*sts3Num + sts1Num;

    result = vcatHyPhy20gRVCPIntIndRead(fileHndl, 0x0, memberAddr, &data0);
    if (result) return result;
    
    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_IW_CHANGE_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_IW_CHANGE_I_OFF;
    iwChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);
    
    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_ACCEPT_CHANGE_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_ACCEPT_CHANGE_I_OFF;
    accChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_AIS_CHANGE_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_AIS_CHANGE_I_OFF;
    aisChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_MF_SYNC_CHANGE_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_MF_SYNC_CHANGE_I_OFF;
    mfSyncChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_CTRL_CHANGE_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_CTRL_CHANGE_I_OFF;
    ctrlChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);
    
    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_CTRL_PKT_CRC_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_CTRL_PKT_CRC_I_OFF;
    ctrlCrcI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_CTRL_ERR_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_CTRL_ERR_I_OFF;
    ctrlErrI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_LCAS_ERR_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_LCAS_ERR_I_OFF;
    lcasErrI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_SQ_MISMATCH_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_SQ_MISMATCH_I_OFF;
    sqMismI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_SQ_INVALID_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_SQ_INVALID_I_OFF;
    sqInvI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_ACTIVE_CHANGE_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_ACTIVE_CHANGE_I_OFF;
    actChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_LINK_ST_CHANGE_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_INT_MEM_IBIT_LNK_INT_MEM_LINK_ST_CHANGE_I_OFF;
    lnkStChI = mHYPHY20G_GET_FIELD4(data0, mask, offset);
    
    /* Clear interrupts if enabled */
    if (intClear) {
        result = vcatHyPhy20gRVCPIntIndWrite(fileHndl, 0x0, memberAddr, data0);
        if (result) return result;   
    } 
    /* Compute results */
    *statusBits = (iwChI << 11) +(accChI << 10) + (aisChI << 9) + (mfSyncChI << 8) +
                  (ctrlChI << 7) + (ctrlCrcI << 6) + (ctrlErrI << 5) + (lcasErrI << 4) +
                  (sqMismI << 3) + (sqInvI << 2) + (actChI << 1) + lnkStChI;
 
    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatRxSonetVcatMemberIntStatus */  

/*******************************************************************************
**
**  hyPhy20gVcatRxSonetVcatGrpIntStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the interrupt status of an SONET-based 
**                  VCAT group within the RVCP192 block of the VCAT subsystem.
**                  
**                  It is recommended to have the device level WCI_MODE bit 
**                  set to '1' if using this function to prevent unintentioal
**                  clearing of interrupts that share registers with the 
**                  interrupts accessed by this function.
**
**  INPUTS:         *fileHndl     - base address
**                  grpId         - VCAT Group ID to query
**                                  valid values are 0x0 - 0x3F (0-63)
**                  intClear      - 1 - all *_I interrupt bits accessed by this 
**                                function will be cleared by writing them to '1'
**                                - 0 - all *_I interrupt bits accessed by this  
**                                function will be left untouched.
**                  *statusBits   - Pointer to storage for status bits
**                                      Bit 2 - GID_ERR_I
**                                      Bit 1 - READ_PTR_I
**                                      Bit 0 - RS_I
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatRxSonetVcatGrpIntStatus(struct file *fileHndl, UINT4 grpId,
                                         UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;
    UINT4 offset, mask;
    UINT4 gidErrI, rdPtrI, rsI;
    UINT4 data0;
 
             
   /* argument checking */
    if ((grpId > 0x3F)||(intClear > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    result = vcatHyPhy20gRVCPIntIndRead(fileHndl, 0x1, grpId, &data0);
    if (result) return result;
    
    mask = HYPHY20G_VCAT_LCAS_RVCP_GRP_INT_MEM_IBIT_GRP_INT_GID_ERR_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_GRP_INT_MEM_IBIT_GRP_INT_GID_ERR_I_OFF;
    gidErrI = mHYPHY20G_GET_FIELD4(data0, mask, offset);
    
    mask = HYPHY20G_VCAT_LCAS_RVCP_GRP_INT_MEM_IBIT_GRP_INT_READ_PTR_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_GRP_INT_MEM_IBIT_GRP_INT_READ_PTR_I_OFF;
    rdPtrI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_GRP_INT_MEM_IBIT_GRP_INT_RS_I_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_GRP_INT_MEM_IBIT_GRP_INT_RS_I_OFF;
    rsI = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    /* Clear interrupts if enabled */
    if (intClear) {
        result = vcatHyPhy20gRVCPIntIndWrite(fileHndl, 0x1, grpId, data0);
        if (result) return result;   
    } 
    /* Compute results */
    *statusBits = (gidErrI << 2) + (rdPtrI << 1) + rsI;
 
    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatRxSonetVcatGrpIntStatus */  

/*******************************************************************************
**
**  hyPhy20gVcatRxSonetVcatMemberCtrlPacketStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the contents of a VCAT/LCAS control  
**                  packet received for a SONET-based VCAT group member within 
**                  the RVCP192 block of the VCAT subsystem.
**                  
**
**  INPUTS:         *fileHndl   - base address
**                  sts48Num    - selects one of the 4 possible STS-48 channels 
**                                within the 10G (STS-192) output
**                              - valid values are 0x0 - 0x3 
**                  sts12Num    - selects one of the 4 possible offsets for 
**                                this STS-12 channel within sts48Num
**                              - valid values are 0x0 - 0x3 
**                  sts3Num     - selects one of the 4 possible offsets for 
**                                this STS-3 within sts12Num
**                              - valid values are 0x0 - 0x3
**                  sts1Num     - selects one of the 3 possible offsets for 
**                                this STS-1 within sts3Num (for sts3c Members, 
**                                set this value to 0x0)
**                              - valid values are 0x0 - 0x2
**                  pktNum      - selects one of the 64 control packets
**                                stored within the RVCP192.
**                              - valid values are 0x0 - 0x3F (0 - 63)
**                  *mst        - Pointer to 8-bit MST value
**                  *sq         - Pointer to 8-bit SQ value
**                  *mfi2       - Pointer to 8-bit MFI2 value
**                  *ctrl       - Pointer to 4-bit CTRL value
**                  *statusBits - Pointer to storage for status bits
**                                      Bit 3 - RS_ACK
**                                      Bit 2 - MF_GOOD (MFI1 Sequence Valid)
**                                      Bit 1 - CRC_GOOD (CRC Valid)
**                                      Bit 0 - CRC_ZERO (CRC is all 0's)
**                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gVcatRxSonetVcatMemberCtrlPacketStatus(struct file *fileHndl,
                                                   UINT4 sts48Num, UINT4 sts12Num,
                                                   UINT4 sts3Num, UINT4 sts1Num,
                                                   UINT4 pktNum, UINT4 *mst,
                                                   UINT4 *sq, UINT4 *mfi2,
                                                   UINT4 *ctrl, UINT4 *statusBits)
{
    INT4 result;
    UINT4 offset, mask;
    UINT4 rsAck, mfGood, crcGood, crcZero; 
    UINT4 data0, data1, data2, data3, data4, data5, data6, data7;
    UINT4 memberAddr;
    UINT4 memberOffset;
 
             
   /* argument checking */
    if ((sts48Num > 0x3)||(sts12Num > 3)||(sts3Num > 0x3)||(sts1Num > 0x2)||(pktNum > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    memberOffset = 48*sts48Num + 12*sts12Num + 3*sts3Num + sts1Num;
    memberAddr = 64*memberOffset + pktNum;

    result = vcatHyPhy20gRVCPMemIndRead(fileHndl, 0x4, memberAddr, &data0, 
                                         &data1, &data2, &data3, &data4, 
                                         &data5, &data6, &data7);
    if (result) return result;
    
    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_MST_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_MST_OFF;
    *mst = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_SQ_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_SQ_OFF;
    *sq = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_MFI2_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_MFI2_OFF;
    *mfi2 = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_CTRL_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_CTRL_OFF;
    *ctrl = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_RS_ACK_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_RS_ACK_OFF;
    rsAck = mHYPHY20G_GET_FIELD4(data1, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_MF_GOOD_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_MF_GOOD_OFF;
    mfGood = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_CRC_GOOD_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_CRC_GOOD_OFF;
    crcGood = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    mask = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_CRC_ZERO_MSK;
    offset = HYPHY20G_VCAT_LCAS_RVCP_LNK_CTRL_PKT_DCB_MEM_IBIT_LNK_CTL_PKT_DCB_CRC_ZERO_OFF;
    crcZero = mHYPHY20G_GET_FIELD4(data0, mask, offset);

    *statusBits = (rsAck << 3) + (mfGood << 2) + (crcGood << 1) + crcZero;

 
    return HYPHY20G_SUCCESS;
} /* hyPhy20gVcatRxSonetVcatMemberCtrlPacketStatus */  
