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
**  FILE        : hyphy20g_odukpp_status.c
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
#include "hyphy20g_odukpp_status.h"

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
**  hyPhy20gOdukppOtukRxGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive OTUk status in ODUKPP
**                  subsystem.
**  
**                  The status bits accessed by this function are write one to
**                  clear.  The intClear argument provides the ability to 
**                  optionally clear the status bits.
**
**                  When the line side frame type is in ODUk mode, 
**                  OTUK_AIS_INT_I indicates that client generic AIS (PN-11)  
**                  was detected.                          
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
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukRxGetStatus(struct file *fileHndl, UINT4 slice, 
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
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OTU_INT;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OTU_INT;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
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

} /* hyPhy20gOdukppOtukRxGetStatus */


/*******************************************************************************
**
**  hyPhy20gOdukppOtukTxGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of the Tx incoming ODUk
**                  frame in ODUKPP subsystem from the CBRB.
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
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukTxGetStatus(struct file *fileHndl, UINT4 slice, 
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
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_INT;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_INT;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
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

} /* hyPhy20gOdukppOtukTxGetStatus */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukRxGetFrameStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive OTUk framer in ODUKPP 
**                  subsystem. 
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
**                               bit 8   : LOS_INT_I                      
**                               bit 7   : OOF_INT_I                      
**                               bit 6   : LOA_INT_I                      
**                               bit 5   : LOF_INT_I                      
**                               bit 4   : FAS_CHANGED_INT_I              
**                               bit 3   : OOM_INT_I                      
**                               bit 2   : LOMA_INT_I                     
**                               bit 1   : LOM_INT_I                      
**                               bit 0   : MF_CHANGED_INT_I        
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
INT4 hyPhy20gOdukppOtukRxGetFrameStatus(struct file *fileHndl, UINT4 slice, 
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
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_FA_FEC_INT;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_FEC_INT;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    }

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    /* Ignore lower 7 bits related to FEC status and two reserved bits */ 
    *statusBits = val >> 7;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukRxGetFrameStatus */


/*******************************************************************************
**
**  hyPhy20gOdukppOdukTxGetTcmiStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the OTUk TCMi status of the Tx  
**                  incoming ODUk frame in ODUKPP subsystem from the CBRB.
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
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukTxGetTcmiStatus(struct file *fileHndl, UINT4 slice, 
                                       UINT4 tcmiNum, UINT4 intClear, 
                                       UINT4 *statusBits)
{
    UINT4 addr;
    UINT4 val;
    UINT4 subTcmi;
    INT4 result;

    /* argument checking */
    if (slice > 9 || tcmiNum > 6 || tcmiNum < 1 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    subTcmi = tcmiNum - 1;

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_INT_OFFSET(subTcmi);
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_INT_OFFSET(subTcmi);  
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);        
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

} /* hyPhy20gOdukppOdukTxGetTcmiStatus */


/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive ODUKPP ODUk status.
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
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxGetStatus(struct file *fileHndl, UINT4 slice, 
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
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_INT;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_INT;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
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

} /* hyPhy20gOdukppOdukRxGetStatus */

/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxGetApsPccStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive ODUk PM status in ODUKPP
**                  subsystem.
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
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxGetApsPccStatus(struct file *fileHndl, UINT4 slice, 
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
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_APS_PCC_INT_I;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_APS_PCC_INT_I;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
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

} /* hyPhy20gOdukppOdukRxGetApsPccStatus */


/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxGetTcmiStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive ODUKPP ODUk TCMi status.
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
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxGetTcmiStatus(struct file *fileHndl, UINT4 slice, 
                                       UINT4 tcmiNum, UINT4 intClear, 
                                       UINT4 *statusBits)
{
    UINT4 addr;
    UINT4 val;
    UINT4 subTcmi;
    INT4 result;

    /* argument checking */
    if (slice > 9 || tcmiNum > 6 || tcmiNum < 1 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    subTcmi = tcmiNum - 1;

    /* determine registers and masks associated with counter */
    if (slice > 7){
        /* OTU2 Framer case */
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_INT_OFFSET(subTcmi);
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_INT_OFFSET(subTcmi);  
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);        
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

} /* hyPhy20gOdukppOdukRxGetTcmiStatus */



/*******************************************************************************
**
**  hyPhy20gOdukppOdukRxGetPmStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive ODUk PM status in ODUKPP
**                  subsystem.
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
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdukRxGetPmStatus(struct file *fileHndl, UINT4 slice, 
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
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_PM_INT;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_PM_INT;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
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

} /* hyPhy20gOdukppOdukRxGetPmStatus */


/*******************************************************************************
**
**  hyPhy20gOdukppOpukRxGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the receive OPUk status in ODUKPP
**                  subsystem.
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
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOpukRxGetStatus(struct file *fileHndl, UINT4 slice, 
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
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OPU_INT;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice-8);
    } else {
        /* OTU1 Framer case */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OPU_INT;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
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

} /* hyPhy20gOdukppOpukRxGetStatus */

/*******************************************************************************
**
**  hyPhy20gOdukppOtukJatGetStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function extracts OTUk framer JAT status in ODUKPP
**                  subsystem.
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
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOtukJatGetStatus(struct file *fileHndl, UINT4 slice,
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
        addr = HYPHY20G_ODUKPP_JAT_JAT_REG_DLL_STAT;
        addr = HYPHY20G_ODUKPP_JAT_JAT_TSB_SLICE_REG_OFFSET(addr, slice + 2);
    } else {
        addr = HYPHY20G_ODUKPP_JAT_JAT_REG_DLL_STAT;
        addr = HYPHY20G_ODUKPP_JAT_JAT_TSB_SLICE_REG_OFFSET(addr, slice - 8);
    }

    msk = HYPHY20G_ODUKPP_JAT_JAT_BIT_DLL_STAT_DLL_RUN_MSK;
    msk = msk | HYPHY20G_ODUKPP_JAT_JAT_BIT_DLL_STAT_DLL_ERR_MSK;

    /* Retrieve Counter Value */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    /* mask out bits other than DLL_ERR and DLL_RUN */
    val = val & msk;
    *pStatus = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppOtukJatGetStatus */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/* end of file */

