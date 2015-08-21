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
**  FILE        : hyphy20g_otn_status.c
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
#include "hyphy20g_otn_status.h"

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
**  hyPhy20gOtukTxGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the OTUk status of the Tx incoming ODUk
**                  frame from the CBRB.
**  
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to 
**                  optionally clear the status bits.
**
**
**  INPUTS:       *fileHndl     - base address
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
**                intClear      - 1 - all *INT_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this  
**                               function will retain their values.
**                *statusBits   - Pointer to storage for status bits
**                               bit 31-16: RESERVED                   
**                               bit 15   : IAE_INT_I                  
**                               bit 14   : TX_PJ_INT_I                
**                               bit 13   : TX_NJ_INT_I                
**                               bit 12   : TX_FIFO_THRESH_HIGH_INT_I  
**                               bit 11   : TX_FIFO_THRESH_LOW_INT_I   
**                               bit 10   : TX_FIFO_OVERFLOW_INT_I     
**                               bit 9    : TX_FIFO_UNDERFLOW_INT_I    
**                               bit 8    : FIFO_FULL_INT_I            
**                               bit 7    : LOS_DETECT_INT_I           
**                               bit 6-0  : RESERVED
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtukTxGetStatus(struct file *fileHndl, UINT4 slice, UINT4 intClear, 
                             UINT4 *statusBits)
{
    UINT4 addr;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 9 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_INT;
        addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_OTU1_FRM_REG_TX_OTN_INT;
        addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    }

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *statusBits = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtukTxGetStatus */


/*******************************************************************************
**
**  hyPhy20gOdukTxGetTcmiStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the OTUk TCMi status of the Tx incoming 
**                  ODUk frame from the CBRB.
**  
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to 
**                  optionally clear the status bits.
**
**
**  INPUTS:       *fileHndl     - base address
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
**                tcmiNum       - TCMi instance:
**                                1 to 6
**                intClear      - 1 - all *INT_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this  
**                               function will retain their values.
**                *statusBits   - Pointer to storage for status bits
**                               bit 31-16: RESERVED                       
**                               bit 15   : TX_TCMI_TIM_INT_I              
**                               bit 14   : TX_TCMI_TTI_ACCEPT_INT_I       
**                               bit 13   : TX_TCMI_TTI_INCONSISTENT_INT_I 
**                               bit 12   : TX_TCMI_BIP_INT_I              
**                               bit 11   : TX_TCMI_BEI_INT_I              
**                               bit 10   : TX_TCMI_BIAE_INT_I             
**                               bit 9    : TX_TCMI_BDI_INT_I              
**                               bit 8    : RESERVED                       
**                               bit 7    : TX_TCMI_STAT_ACCEPT_INT_I      
**                               bit 6    : TX_TCMI_STAT_INCONSISTENT_INT_I
**                               bit 5    : TX_TCMI_LTC_INT_I              
**                               bit 4    : TX_TCMI_LCK_INT_I              
**                               bit 3    : TX_TCMI_OCI_INT_I              
**                               bit 2    : TX_TCMI_AIS_INT_I              
**                               bit 1    : TX_TCMI_IAE_INT_I  
**                               bit 0    : RESERVED
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukTxGetTcmiStatus(struct file *fileHndl, UINT4 slice, 
                                 UINT4 tcmiNum, UINT4 intClear, 
                                 UINT4 *statusBits)
{
    UINT4 addr;
    UINT4 val;
    UINT4 subTcmiNum;
    INT4 result;

    /* argument checking */
    if (slice > 9 || tcmiNum > 6 || tcmiNum < 1 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    subTcmiNum = tcmiNum - 1;

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_ODU_TCMI_INT_OFFSET(subTcmiNum);
        addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_OTU1_FRM_REG_TX_OTN_ODU_TCMI_INT_OFFSET(subTcmiNum);  
        addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);        
    }

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *statusBits = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukTxGetTcmiStatus */


/*******************************************************************************
**
**  hyPhy20gOtukRxGetFrameStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive OTUk framer and FEC status.
**  
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to 
**                  optionally clear the status bits.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice       - OTUk Framer instance:
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
**                intClear      - 1 - all *INT_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this  
**                               function will retain their values.
**                *statusBits   - Pointer to storage for status bits
**                               bit 31-16: RESERVED                       
**                               bit 15   : LOS_INT_I                      
**                               bit 14   : OOF_INT_I                      
**                               bit 13   : LOA_INT_I                      
**                               bit 12   : LOF_INT_I                      
**                               bit 11   : FAS_CHANGED_INT_I              
**                               bit 10   : OOM_INT_I                      
**                               bit 9    : LOMA_INT_I                     
**                               bit 8    : LOM_INT_I                      
**                               bit 7    : MF_CHANGED_INT_I               
**                               bit 6    : FEC_CORRECTED_SYMBOL_INT_I     
**                               bit 5    : FEC_CORRECTED_ZERO_INT_I       
**                               bit 4    : FEC_CORRECTED_ONE_INT_I        
**                               bit 3    : FEC_UNCORRECTED_CODEWORD_INT_I 
**                               bit 2    : FEC_BER_EXCEEDED_INT_I         
**                               bit 1-0  : RESERVED
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtukRxGetFrameStatus(struct file *fileHndl, UINT4 slice, 
                                  UINT4 intClear, UINT4 *statusBits)
{
    UINT4 addr;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 9 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_FA_FEC_INT;
        addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_FA_FEC_INT;
        addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    }

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *statusBits = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtukRxGetFrameStatus */



/*******************************************************************************
**
**  hyPhy20gOtukRxGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive OTUk status.
**  
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to 
**                  optionally clear the status bits.
**
**                  If this API is called when the line side frame type is      
**                  in ODUk mode, OTUK_AIS_INT_I indicates that client             
**                  generic AIS (PN-11)  was detected.
**
**
**  INPUTS:       *fileHndl     - base address
**                slice         - OTUk Framer instance:
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
**                intClear      - 1 - all *INT_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this  
**                               function will retain their values.
**                *statusBits   - Pointer to storage for status bits
**                               bit 31-16: RESERVED                  
**                               bit 15   : OTUK_AIS_INT_I            
**                               bit 14   : SM_TIM_INT_I              
**                               bit 13   : SM_TTI_ACCEPT_INT_I       
**                               bit 12   : SM_TTI_INCONSISTENT_INT_I 
**                               bit 11   : SM_BIP_INT_I              
**                               bit 10   : SM_BEI_INT_I              
**                               bit 9    : SM_BIAE_INT_I             
**                               bit 8    : SM_BDI_INT_I              
**                               bit 7    : SM_IAE_INT_I              
**                               bit 6    : SM_RES_ACCEPT_INT_I       
**                               bit 5    : SM_RES_INCONSISTENT_INT_I 
**                               bit 4    : GCC0_ACCEPT_INT_I         
**                               bit 3    : GCC0_INCONSISTENT_INT_I   
**                               bit 2    : RES_ACCEPT_INT_I          
**                               bit 1    : RES0_INCONSISTENT_INT_I   
**                               bit 0    : RESERVED
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtukRxGetStatus(struct file *fileHndl, UINT4 slice, 
                             UINT4 intClear, UINT4 *statusBits)
{
    UINT4 addr;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 9 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_OTU_INT;
        addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_OTU_INT;
        addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    }

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *statusBits = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtukRxGetStatus */



/*******************************************************************************
**
**  hyPhy20gOdukRxGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive ODUk status.
**  
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to 
**                  optionally clear the status bits.
**
**
**  INPUTS:       *fileHndl     - base address
**                slice         - OTUk Framer instance:
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
**                intClear      - 1 - all *INT_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this  
**                               function will retain their values.
**                *statusBits   - Pointer to storage for status bits
**                               bit 31-16: RESERVED 
**                               bit 15   : RES1_ACCEPT_INT_I            
**                               bit 14   : RES1_INCONSISTENT_INT_I      
**                               bit 13   : TCM_ACT_ACCEPT_INT_I         
**                               bit 12   : TCM_ACT_INCONSISTENT_INT_I   
**                               bit 11   : FTFL_ACCEPT_INT_I            
**                               bit 10   : FTFL_INCONSISTENT_INT_I      
**                               bit 9    : EXP_ACCEPT_INT_I             
**                               bit 8    : EXP_INCONSISTENT_INT_I       
**                               bit 7    : GCC1_ACCEPT_INT_I            
**                               bit 6    : GCC1_INCONSISTENT_INT_I      
**                               bit 5    : GCC2_ACCEPT_INT_I            
**                               bit 4    : GCC2_INCONSISTENT_INT_I      
**                               bit 3    : RES2_ACCEPT_INT_I                 
**                               bit 2    : RES2_INCONSISTENT_INT_I      
**                               bit 2    : APS_PCC_MFAS2_INCONSISTENT_INT_I
**                               bit 1-0  : RESERVED
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukRxGetStatus(struct file *fileHndl, UINT4 slice, 
                             UINT4 intClear, UINT4 *statusBits)
{
    UINT4 addr;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 9 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    DBG_PRINT("%s, %s, %d, hyPhy20gOdukRxGetStatus args slice = %lu intClear %lu \n",__FILE__, __FUNCTION__, __LINE__, slice, intClear);

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_ODU_INT;
        addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_ODU_INT;
        addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    }

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *statusBits = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukRxGetStatus */

/*******************************************************************************
**
**  hyPhy20gOdukRxGetApsPccStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive ODUk PM status.
**  
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to 
**                  optionally clear the status bits.
**
**
**  INPUTS:       *fileHndl     - base address
**                slice         - OTUk Framer instance:
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
**                intClear      - 1 - all *INT_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this  
**                               function will retain their values.
**                *statusBits   - Pointer to storage for status bits
**                               bit 31-16: RESERVED                        
**                               bit 15   : APS_PCC_MFAS7_ACCEPT_INT_I      
**                               bit 14   : APS_PCC_MFAS6_ACCEPT_INT_I      
**                               bit 13   : APS_PCC_MFAS5_ACCEPT_INT_I      
**                               bit 12   : APS_PCC_MFAS4_ACCEPT_INT_I      
**                               bit 11   : APS_PCC_MFAS3_ACCEPT_INT_I      
**                               bit 10   : APS_PCC_MFAS2_ACCEPT_INT_I      
**                               bit 9    : APS_PCC_MFAS1_ACCEPT_INT_I      
**                               bit 8    : APS_PCC_MFAS0_ACCEPT_INT_I      
**                               bit 7    : APS_PCC_MFAS7_INCONSISTENT_INT_I
**                               bit 6    : APS_PCC_MFAS6_INCONSISTENT_INT_I
**                               bit 5    : APS_PCC_MFAS5_INCONSISTENT_INT_I
**                               bit 4    : APS_PCC_MFAS4_INCONSISTENT_INT_I      
**                               bit 3    : APS_PCC_MFAS3_INCONSISTENT_INT_I
**                               bit 2    : APS_PCC_MFAS2_INCONSISTENT_INT_I
**                               bit 1    : APS_PCC_MFAS1_INCONSISTENT_INT_I
**                               bit 0    : APS_PCC_MFAS0_INCONSISTENT_INT_I
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukRxGetApsPccStatus(struct file *fileHndl, UINT4 slice, 
                                   UINT4 intClear, UINT4 *statusBits)
{
    UINT4 addr;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 9 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_ODU_APS_PCC_INT_I;
        addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_ODU_APS_PCC_INT_I;
        addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    }

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *statusBits = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukRxGetApsPccStatus */


/*******************************************************************************
**
**  hyPhy20gOdukRxGetTcmiStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive ODUk TCMi status.
**  
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to 
**                  optionally clear the status bits.
**
**
**  INPUTS:       *fileHndl     - base address
**                slice         - OTUk Framer instance:
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
**                tcmiNum       - TCMi instance:
**                                1 to 6
**                intClear      - 1 - all *INT_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this  
**                               function will retain their values.
**                *statusBits   - Pointer to storage for status bits
**                               bit 31-16: RESERVED                        
**                               bit 15   : RX_TCMI_TIM_INT_I               
**                               bit 14   : RX_TCMI_TTI_ACCEPT_INT_I        
**                               bit 13   : RX_TCMI_TTI_INCONSISTENT_INT_I  
**                               bit 12   : RX_TCMI_BIP_INT_I               
**                               bit 11   : RX_TCMI_BEI_INT_I               
**                               bit 10   : RX_TCMI_BIAE_INT_I              
**                               bit 9    : RX_TCMI_BDI_INT_I               
**                               bit 7    : RX_TCMI_STAT_ACCEPT_INT_I       
**                               bit 6    : RX_TCMI_STAT_INCONSISTENT_INT_I 
**                               bit 5    : RX_TCMI_LTC_INT_I               
**                               bit 4    : RX_TCMI_LCK_INT_I               
**                               bit 3    : RX_TCMI_OCI_INT_I               
**                               bit 2    : RX_TCMI_AIS_INT_I                          
**                               bit 1    : RX_TCMI_IAE_INT_I
**                               bit 0    : RESERVED                        
**               
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukRxGetTcmiStatus(struct file *fileHndl, UINT4 slice, 
                                 UINT4 tcmiNum, UINT4 intClear, 
                                 UINT4 *statusBits)
{
    UINT4 addr;
    UINT4 val;
    UINT4 subTcmiNum;
    INT4 result;

    /* argument checking */
    if (slice > 9 || tcmiNum > 6 || tcmiNum < 1 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    subTcmiNum = tcmiNum - 1;

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_ODU_TCMI_INT_OFFSET(subTcmiNum);
        addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_ODU_TCMI_INT_OFFSET(subTcmiNum);  
        addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);        
    }

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *statusBits = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukRxGetTcmiStatus */



/*******************************************************************************
**
**  hyPhy20gOdukRxGetPmStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive ODUk PM status.
**  
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to 
**                  optionally clear the status bits.
**
**
**  INPUTS:       *fileHndl     - base address
**                slice         - OTUk Framer instance:
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
**                intClear      - 1 - all *INT_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this  
**                               function will retain their values.
**                *statusBits   - Pointer to storage for status bits
**                               bit 31-16: RESERVED                  
**                               bit 15   : PM_TIM_INT_I              
**                               bit 14   : PM_TTI_ACCEPT_INT_I       
**                               bit 13   : PM_TTI_INCONSISTENT_INT_I 
**                               bit 12   : PM_BIP_INT_I              
**                               bit 11   : PM_BEI_INT_I              
**                               bit 9    : PM_BDI_INT_I              
**                               bit 7    : PM_STAT_ACCEPT_INT_I      
**                               bit 6    : PM_STAT_INCONSISTENT_INT_I
**                               bit 4    : PM_LCK_INT_I              
**                               bit 3    : PM_OCI_INT_I              
**                               bit 2    : PM_AIS_INT_I              
**                               bit 1-0  : RESERVED                                 
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukRxGetPmStatus(struct file *fileHndl, UINT4 slice, 
                               UINT4 intClear, UINT4 *statusBits)
{
    UINT4 addr;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 9 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_ODU_PM_INT;
        addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_ODU_PM_INT;
        addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    }

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *statusBits = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukRxGetPmStatus */


/*******************************************************************************
**
**  hyPhy20gOpukRxGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive OPUk status.
**  
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to 
**                  optionally clear the status bits.
**
**                  Status report of the following 4 payload related interrupts  
**                  from this API requires RX_STREAMING_MODE to be 0. 
**                  Status report of these interrupts from this API is invalid  
**                  for data paths with system side data source being OPI or  
**                  ODTU12 as they require RX_STREAMING_MODE to be 1. 
**                      Bit 12 -  CLIENT_GENERIC_AIS_INT_I
**                      Bit 7  -  CLIENT_FIXED_PYLD_INT_I  
**                      Bit 6  -  CLIENT_PN23_PYLD_INT_I   
**                      Bit 5  -  CLIENT_PN31_PYLD_INT_I            
**                  
**                  
**
**  INPUTS:       *fileHndl     - base address
**                slice         - OTUk Framer instance:
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
**                intClear      - 1 - all *INT_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this  
**                               function will retain their values.
**                *statusBits   - Pointer to storage for status bits
**                               bit 31-16: RESERVED                 
**                               bit 15   : PLM_INT_I                
**                               bit 14   : PT_ACCEPT_INT_I          
**                               bit 13   : PT_INCONSISTENT_INT_I    
**                               bit 12   : CLIENT_GENERIC_AIS_INT_I 
**                               bit 11   : RX_FIFO_THRESH_HIGH_INT_I
**                               bit 10   : RX_FIFO_THRESH_LOW_INT_I 
**                               bit 9    : RX_FIFO_OVERFLOW_INT_I   
**                               bit 8    : RX_FIFO_UNDERFLOW_INT_I  
**                               bit 7    : CLIENT_FIXED_PYLD_INT_I  
**                               bit 6    : CLIENT_PN23_PYLD_INT_I   
**                               bit 5    : CLIENT_PN31_PYLD_INT_I   
**                               bit 4    : PJ_INT_I                 
**                               bit 3    : NJ_INT_I                 
**                               bit 2-0  : RESERVED                                 
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOpukRxGetStatus(struct file *fileHndl, UINT4 slice, 
                             UINT4 intClear, UINT4 *statusBits)
{
    UINT4 addr;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 9 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_OPU_INT;
        addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_OPU_INT;
        addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    }

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    *statusBits = val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOpukRxGetStatus */

/*******************************************************************************
**
**  hyPhy20gOpukRxGetPayloadStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This is a diagnostic function only that checks the receive  
**                  OPUk payload status when system side data source is from OPI
**                  or ODTU12. 
**
**                  This API should be used for testing or debugging purpose 
**                  only.
**
**                  For data paths with data coming from other system side
**                  interfaces (SONET or HSCI), hyPhy20gOpukRxGetStatus must
**                  be used to retrieve status of payload interrupts. 
**  
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to 
**                  optionally clear the status bits.
**
**                  When data is coming from OPI or ODTU12, 
**                  the RX_STREAMING_MODE bit needs to be set to 0 in order for
**                  the appropriate interrupt to be triggered. This API will
**                  reconfigure RX_STREAMING_MODE to be 0 which will corrupt
**                  data path until RX_STREAMING_MODE is restored to 1 after
**                  reading the status bits. 
**                  This function waits 100 multiframes before reading the 
**                  status to allow the framer sufficient recovery time after 
**                  RX_STREAMING_MODE is reconfigured.
**
**
**  INPUTS:       *fileHndl     - base address
**                slice         - OTUk Framer instance:
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
**                intClear      - 1 - all *INT_I interrupt bits accessed by this 
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this  
**                               function will retain their values.
**                *statusBits   - Pointer to storage for status bits
**                               bit 31-4 : RESERVED                    
**                               bit 3    : CLIENT_GENERIC_AIS_INT_I 
**                               bit 2    : CLIENT_FIXED_PYLD_INT_I  
**                               bit 1    : CLIENT_PN23_PYLD_INT_I   
**                               bit 0    : CLIENT_PN31_PYLD_INT_I                                                 
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOpukRxGetPayloadStatus (struct file *fileHndl, UINT4 slice, 
                                    UINT4 intClear, UINT4 *statusBits)
{
    UINT4 statusReg;
    UINT4 val;
    
    /* rx streaming mode */
    UINT4 rxOtnGlblReg;
    UINT4 rxStreamMsk;
    UINT4 rxStreamVal;
    UINT4 recoveryDelay;
    
    /* payload status */
    UINT4 aisPyldMsk, fixedPyldMsk, pn23PyldMsk, pn31PyldMsk, payloadStatusMsk;
    UINT4 aisPyldOff, fixedPyldOff, pn23PyldOff, pn31PyldOff;
   
    INT4 result;

    /* argument checking */
    if (slice > 9 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        statusReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_OPU_INT;
        statusReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(statusReg, slice-8);
        
        rxOtnGlblReg = HYPHY20G_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
        rxOtnGlblReg = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(rxOtnGlblReg, slice-8);
        rxStreamMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_STREAMING_MODE_MSK;
        
        aisPyldMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_INT_CLIENT_GENERIC_AIS_INT_I_MSK;
        fixedPyldMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_INT_CLIENT_FIXED_PYLD_INT_I_MSK;
        pn23PyldMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_INT_CLIENT_PN23_PYLD_INT_I_MSK;
        pn31PyldMsk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_INT_CLIENT_PN31_PYLD_INT_I_MSK;
        
        aisPyldOff = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_INT_CLIENT_GENERIC_AIS_INT_I_OFF;
        fixedPyldOff = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_INT_CLIENT_FIXED_PYLD_INT_I_OFF;
        pn23PyldOff = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_INT_CLIENT_PN23_PYLD_INT_I_OFF;
        pn31PyldOff = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_INT_CLIENT_PN31_PYLD_INT_I_OFF;

        recoveryDelay = 300;
    } else {
        /* OTU1 Framer case */
        statusReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_OPU_INT;
        statusReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(statusReg, slice);
        
        rxOtnGlblReg = HYPHY20G_OTU1_FRM_REG_RX_OTN_GLBL_CTL;
        rxOtnGlblReg = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(rxOtnGlblReg, slice);
        rxStreamMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_STREAMING_MODE_MSK;        
        
        aisPyldMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_INT_CLIENT_GENERIC_AIS_INT_I_MSK;
        fixedPyldMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_INT_CLIENT_FIXED_PYLD_INT_I_MSK;
        pn23PyldMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_INT_CLIENT_PN23_PYLD_INT_I_MSK;
        pn31PyldMsk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_INT_CLIENT_PN31_PYLD_INT_I_MSK;
        
        aisPyldOff = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_INT_CLIENT_GENERIC_AIS_INT_I_OFF;
        fixedPyldOff = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_INT_CLIENT_FIXED_PYLD_INT_I_OFF;
        pn23PyldOff = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_INT_CLIENT_PN23_PYLD_INT_I_OFF;
        pn31PyldOff = HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_INT_CLIENT_PN31_PYLD_INT_I_OFF;
        
        recoveryDelay = 1200;
    }
    
    /* Retrieve RX_STREAMING_MODE */
    result = hyPhy20gRegFieldRead(fileHndl, rxOtnGlblReg, rxStreamMsk, 
                                  &rxStreamVal);
    if (result) return result;
    
    if (rxStreamVal == 1) {
        /* Temporarily set RX_STREAMING_MODE to 0 for payload detection */
        result = hyPhy20gRegFieldWrite(fileHndl, rxOtnGlblReg, rxStreamMsk, 0);
        if (result) return result;
        
        /* Give the framers some time to recover */
        result = sysHyPhy20gMsDelay(recoveryDelay);
        if (result) return result;  
    } 
    
    /* retrieve status bits */
    result = sysHyPhy20gRead(fileHndl, statusReg, &val);
    if (result) return result;
    
    *statusBits = 0x0000 | ((val & pn31PyldMsk) >> (pn31PyldOff))
                         | ((val & pn23PyldMsk) >> (pn23PyldOff - 1))
                         | ((val & fixedPyldMsk) >> (fixedPyldOff - 2))
                         | ((val & aisPyldMsk) >> (aisPyldOff - 3));
    
    /* Restore the original RX_STREAMING_MODE value if it was changed */
    if (rxStreamVal == 1) {
        result = hyPhy20gRegFieldWrite(fileHndl, rxOtnGlblReg, rxStreamMsk, 1);
        if (result) return result;
    }
    
    /* clear payload status bits if requested */
    if(intClear == 1) {
        payloadStatusMsk = aisPyldMsk | fixedPyldMsk | pn23PyldMsk | pn31PyldMsk;
        result = sysHyPhy20gWrite(fileHndl, statusReg, payloadStatusMsk);
        if (result) return result;
    }
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gOpukRxGetPayloadStatus */

/*******************************************************************************
**
**  hyPhy20gOtukJatGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts OTUk framer JAT status.
**
**                  JAT status is only relevant for OTUk framers that are
**                  connected to the SONET subsystem via SONET_LINE.
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
**                *pStatus      - Returned Status
**                                bit 31-2 : RESERVED
**                                bit 1    : DLL_ERR
**                                bit 0    : DLL_RUN
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtukJatGetStatus(struct file *fileHndl, UINT4 slice,
                              UINT4 *pStatus)
{
    UINT4 addr, msk, val;
    INT4 result;

    /* argument checking */
    if(slice > 9){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    /*
    DLL STATUS Register Index n = 0 to 9, where:
    0 to 1 maps to OTU2 framer 0 to 1
    2 to 9 maps to OTU1 framer 0 to 7
    */
    if (slice < 8) {
        addr = HYPHY20G_OTU2_JAT_REG_DLL_STAT;
        addr = HYPHY20G_OTU2_JAT_TSB_SLICE_REG_OFFSET(addr, slice + 2);
    } else {
        addr = HYPHY20G_OTU2_JAT_REG_DLL_STAT;
        addr = HYPHY20G_OTU2_JAT_TSB_SLICE_REG_OFFSET(addr, slice - 8);
    }

    msk = HYPHY20G_OTU2_JAT_BIT_DLL_STAT_DLL_RUN_MSK;
    msk = msk | HYPHY20G_OTU2_JAT_BIT_DLL_STAT_DLL_ERR_MSK;

    /* Retrieve Counter Value */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    /* mask out bits other than DLL_ERR and DLL_RUN */
    val = val & msk;
    *pStatus = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtukJatGetStatus */

/*******************************************************************************
**
**  hyPhy20gOtuEfecGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts I.7 and I.4 FEC status for one of
**                  the OTU2 framers.
**
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to
**                  optionally clear the status bits.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTU2 Framer instance:
**                                0 - OTU2 Framer 0
**                                1 - OTU2 Framer 1
**                 intClear     - 1 - all *INT_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this
**                               function will retain their values.
**                *pStatus      - Returned Status
**                                bit 31-20: RESERVED
**                                bit 19   : I7_OOMS1_INT_I
**                                bit 18   : I7_MSYNC1_UNEXP_INT_I
**                                bit 17   : I7_OOMS0_INT_I
**                                bit 16   : I7_MSYNC0_UNEXP_INT_I
**                                bit 15   : I7_MSYNC_CRC_ERR_INT_I
**                                bit 14   : I4_BER_ALARM_INT_I
**                                bit 13   : I4_CORRECTABLE_ONE_INT_I
**                                bit 12   : I4_CORRECTABLE_ZERO_INT_I
**                                bit 11   : Reserved - return 0
**                                bit 10   : Reserved - return 0
**                                bit 9    : Reserved - return 0
**                                bit 8    : I4_UNCORR_BCH2_ERROR_INT_I
**                                bit 7    : I7_BER_ALARM_INT_I
**                                bit 6    : I7_CORRECTABLE_ONE_INT_I
**                                bit 5    : I7_CORRECTABLE_ZERO_INT_I
**                                bit 4    : Reserved - return 0
**                                bit 3    : Reserved - return 0
**                                bit 2    : I7_UNCORR_ROW3_ERROR_INT_I
**                                bit 1-0  : RESERVED
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtuEfecGetStatus(struct file *fileHndl, UINT4 slice,
                              UINT4 intClear, UINT4 *pStatus)
{
    UINT4 addr, val;
    INT4 result;

    /* argument checking */
    if((slice > 1)||(intClear > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_EFEC_REG_INT_STAT;
    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(addr, slice);

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    /* mask the some status bits that aren't needed. */
    val &= (~(HYPHY20G_EFEC_BIT_INT_STAT_I7_UNCORR_ROW2_ERROR_INT_I_MSK
	        |HYPHY20G_EFEC_BIT_INT_STAT_I7_UNCORR_ROW1_ERROR_INT_I_MSK
	        |HYPHY20G_EFEC_BIT_INT_STAT_I4_UNCORR_BCH1_ERROR_INT_I_MSK
	        |HYPHY20G_EFEC_BIT_INT_STAT_I4_UNCORR_RS2_ERROR_INT_I_MSK
	        |HYPHY20G_EFEC_BIT_INT_STAT_I4_UNCORR_RS1_ERROR_INT_I_MSK));
    *pStatus= val;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtuEfecGetStatus */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/* end of file */

