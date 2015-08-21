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
**  FILE        : hyphy20g_dmi_status.c
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
#include "hyphy20g_dmi_status.h"

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
**  hyPhy20gDmiGetSts1PayloadCfg
**  ___________________________________________________________________________ 
**
**  DESCRIPTION: This function reports the master/slave status of the 48 STS1s
**               within a DMI slice in the Rx Direction.  
**                
**               STS3_1[15:0] refers to STS1_0's in every STS3 of each STS12.
**               STS3_2[15:0] refers to STS1_1's in every STS3 of each STS12.
**               STS3_3[15:0] refers to STS1_2's in every STS3 of each STS12.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - DMI block instance 
**                                0 to 4
**                 port         - 2.5 G DMI processing port
**                                0 to 3 
**                 statusType   - 0 - Status of the auto-detector PCD filter
**                                    output
**                              - 1 - Status of what the SHPI is using (based
**                                    on combination of manually and 
**                                    provisioned timeslots).  
**                *statusWord1  - Pointer to storage for status word1
**                *statusWord2  - Pointer to storage for status word2
**                *statusWord3  - Pointer to storage for status word3
**                                  Word 1 - STS3_1[15:0]
**                                  Word 2 - STS3_2[15:0] 
**                                  Word 3 - STS3_3[15:0]
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gDmiGetSts1PayloadCfg(struct file *fileHndl, UINT4 slice, 
                                  UINT4 port, UINT4 statusType, 
                                  UINT4 *statusWord1, UINT4 *statusWord2, 
                                  UINT4 *statusWord3)
{
    /* Variable declaration */
    UINT4 regAddr1, regAddr2, regAddr3;
    UINT4 linkRegAddr1, linkRegAddr2, linkRegAddr3;
    UINT4 intRdRegAddr, dmiCfgRegData, dmiShpiRegData;
    UINT4 dmiIntRdData;
    UINT4 pOutEventMsk, msk;
    UINT4 msk1, msk2, msk3;
    UINT4 stat1, stat2, stat3;
    UINT4 pOutEvent;
    UINT4 readCount = 0;
    UINT4 valid = 0;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Argument checking */
    if ((slice > 4) || (port > 3) || (statusType > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;   
    }
    
    if (statusType == 0) { 
        
        /* set up for the specific link */
        if (port == 0) {
            pOutEventMsk = HYPHY20G_DMI_DMI_BIT_INT_RD_PCD_OUT_EVENT_4_I_MSK;
            linkRegAddr1 = HYPHY20G_DMI_DMI_REG_OBS_LNK_0_STS3_1;
            linkRegAddr2 = HYPHY20G_DMI_DMI_REG_OBS_LNK_0_STS3_2;
            linkRegAddr3 = HYPHY20G_DMI_DMI_REG_OBS_LNK_0_STS3_3;
            msk1 = HYPHY20G_DMI_DMI_BIT_OBS_LNK_0_STS3_1_OBS_LINK0_STS3_1_MSK;
            msk2 = HYPHY20G_DMI_DMI_BIT_OBS_LNK_0_STS3_2_OBS_LINK0_STS3_2_MSK;
            msk3 = HYPHY20G_DMI_DMI_BIT_OBS_LNK_0_STS3_3_OBS_LINK0_STS3_3_MSK;
        } else if (port == 1) {
            pOutEventMsk = HYPHY20G_DMI_DMI_BIT_INT_RD_PCD_OUT_EVENT_3_I_MSK;
            linkRegAddr1 = HYPHY20G_DMI_DMI_REG_OBS_LNK_1_STS3_1;
            linkRegAddr2 = HYPHY20G_DMI_DMI_REG_OBS_LNK_1_STS3_2;
            linkRegAddr3 = HYPHY20G_DMI_DMI_REG_OBS_LNK_1_STS3_3;
            msk1 = HYPHY20G_DMI_DMI_BIT_OBS_LNK_1_STS3_1_OBS_LINK1_STS3_1_MSK;
            msk2 = HYPHY20G_DMI_DMI_BIT_OBS_LNK_1_STS3_2_OBS_LINK1_STS3_2_MSK;
            msk3 = HYPHY20G_DMI_DMI_BIT_OBS_LNK_1_STS3_3_OBS_LINK1_STS3_3_MSK;
        } else if (port == 2) {
            pOutEventMsk = HYPHY20G_DMI_DMI_BIT_INT_RD_PCD_OUT_EVENT_2_I_MSK;
            linkRegAddr1 = HYPHY20G_DMI_DMI_REG_OBS_LNK_2_STS3_1;
            linkRegAddr2 = HYPHY20G_DMI_DMI_REG_OBS_LNK_2_STS3_2;
            linkRegAddr3 = HYPHY20G_DMI_DMI_REG_OBS_LNK_2_STS3_3;
            msk1 = HYPHY20G_DMI_DMI_BIT_OBS_LNK_2_STS3_1_OBS_LINK2_STS3_1_MSK;
            msk2 = HYPHY20G_DMI_DMI_BIT_OBS_LNK_2_STS3_2_OBS_LINK2_STS3_2_MSK;
            msk3 = HYPHY20G_DMI_DMI_BIT_OBS_LNK_2_STS3_3_OBS_LINK2_STS3_3_MSK; 
        } else {
            pOutEventMsk = HYPHY20G_DMI_DMI_BIT_INT_RD_PCD_OUT_EVENT_1_I_MSK;
            linkRegAddr1 = HYPHY20G_DMI_DMI_REG_OBS_LNK_3_STS3_1;
            linkRegAddr2 = HYPHY20G_DMI_DMI_REG_OBS_LNK_3_STS3_2;
            linkRegAddr3 = HYPHY20G_DMI_DMI_REG_OBS_LNK_3_STS3_3;
            msk1 = HYPHY20G_DMI_DMI_BIT_OBS_LNK_3_STS3_1_OBS_LINK3_STS3_1_MSK;
            msk2 = HYPHY20G_DMI_DMI_BIT_OBS_LNK_3_STS3_2_OBS_LINK3_STS3_2_MSK;
            msk3 = HYPHY20G_DMI_DMI_BIT_OBS_LNK_3_STS3_3_OBS_LINK3_STS3_3_MSK;
        }
        
        /* STS3_1: Read the PCD_OUT_EVENT resigister from DMI INTERRUPT READ 
           at 0x4934004 + 0x00800*A(A=0:4) for the corresponding link 
           and clear it */
        intRdRegAddr = HYPHY20G_DMI_DMI_REG_INT_RD;
        intRdRegAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(intRdRegAddr, slice);
        
        result = sysHyPhy20gRead(fileHndl, intRdRegAddr, &dmiIntRdData);
        if (result) return result;
        
        result = hyPhy20gVarFieldRead(dmiIntRdData, pOutEventMsk, &pOutEvent);
        if (result) return result; 
            
        if (pOutEvent != 0x0) {
            result = hyPhy20gRegFieldWrite (fileHndl, intRdRegAddr, 
                                            pOutEventMsk, 0x0);
            if (result) return result;
        }
        
        /* Wait for one frame to allow propogation of the new payload 
           configuration */    
        result = sysHyPhy20gUsDelay(125);
        if (result) return result;
        
        /* Direct read from DMI:SHPI at 0x493402C + 0x00800*A(A=0:4)
           OBSERVE LINK STS3_1 */ 
        linkRegAddr1 = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(linkRegAddr1, slice);
         
        result = sysHyPhy20gRead(fileHndl, linkRegAddr1, &dmiCfgRegData);
        if (result) return result;
            
        result = hyPhy20gVarFieldRead(dmiCfgRegData, msk1, &stat1);
        if (result) return result;
        
        /* Read PCD_OUT_EVENT register */ 
        result = sysHyPhy20gRead(fileHndl, intRdRegAddr, &dmiIntRdData);
        if (result) return result;
        
        result = hyPhy20gVarFieldRead(dmiIntRdData, pOutEventMsk, &pOutEvent);
        if (result) return result; 
            
        if (pOutEvent == 0x0) {
            valid = 1;
        } else valid = 0;
        
        /* Ensure PCD_OUT_EVENT register was not interrupted during the read:
           the value is valid only when PCD_OUT_EVENT == 0 for the link */
        while ((valid != 1) && (readCount < 10)) {
            result = sysHyPhy20gRead(fileHndl, linkRegAddr1, &dmiCfgRegData);
            if (result) return result;  
                
            result = hyPhy20gVarFieldRead(dmiCfgRegData, msk1, &stat1);
            if (result) return result;    
            
            result = sysHyPhy20gRead(fileHndl, intRdRegAddr, &dmiIntRdData);
            if (result) return result;
        
            result = hyPhy20gVarFieldRead(dmiCfgRegData, pOutEventMsk, 
                                          &pOutEvent);
            if (result) return result; 
            
            if (pOutEvent == 0x0) {
                valid = 1;
                readCount++;
            } else {
                valid = 0; 
                readCount++;
            }  
        }  
           
        /* STS3_2: Read the PCD_OUT_EVENT resigister from DMI INTERRUPT READ 
           at 0x4934004 + 0x00800*A(A=0:4) for the corresponding link 
           and clear it */
        result = sysHyPhy20gRead(fileHndl, intRdRegAddr, &dmiIntRdData);
        if (result) return result;
        
        result = hyPhy20gVarFieldRead(dmiIntRdData, pOutEventMsk, &pOutEvent);
        if (result) return result; 
            
        if (pOutEvent != 0x0) {
            result = hyPhy20gRegFieldWrite (fileHndl, intRdRegAddr, 
                                            pOutEventMsk, 0x0);
            if (result) return result;
        }
        
        /* Wait for one frame to allow propogation of the new payload 
           configuration */    
        result = sysHyPhy20gUsDelay(125);
        if (result) return result;
        
        /* Direct read from DMI:SHPI at 0x493402C + 0x00800*A(A=0:4)
           OBSERVE LINK STS3_2 */ 
        linkRegAddr2 = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(linkRegAddr2, slice);
         
        result = sysHyPhy20gRead(fileHndl, linkRegAddr2, &dmiCfgRegData);
        if (result) return result;
            
        result = hyPhy20gVarFieldRead(dmiCfgRegData, msk2, &stat2);
        if (result) return result;
        
        /* Read PCD_OUT_EVENT register */ 
        result = sysHyPhy20gRead(fileHndl, intRdRegAddr, &dmiIntRdData);
        if (result) return result;
        
        result = hyPhy20gVarFieldRead(dmiIntRdData, pOutEventMsk, &pOutEvent);
        if (result) return result; 
            
        if (pOutEvent == 0x0) {
            valid = 1;
        } else valid = 0;
        
        /* Ensure the value did not change while reading the register:
           the value is valid only when PCD_OUT_EVENT == 0 for the link */
        while ((valid != 1) && (readCount < 10)) {
            /* Read payload configuration for STS3_2 */
            result = sysHyPhy20gRead(fileHndl, linkRegAddr2, &dmiCfgRegData);
            if (result) return result;  
                
            result = hyPhy20gVarFieldRead(dmiCfgRegData, msk2, &stat2);
            if (result) return result;    
            
            /* Read interrupt read register */
            result = sysHyPhy20gRead(fileHndl, intRdRegAddr, &dmiIntRdData);
            if (result) return result;
        
            result = hyPhy20gVarFieldRead(dmiIntRdData, pOutEventMsk, 
                                          &pOutEvent);
            if (result) return result; 
            
            if (pOutEvent == 0x0) {
                valid = 1;
                readCount++;
            } else {
                valid = 0; 
                readCount++;
            }  
        }  
        
        /* STS3_3: Read the PCD_OUT_EVENT resigister from DMI INTERRUPT READ 
           at 0x4934004 + 0x00800*A(A=0:4) for the corresponding link 
           and clear it */
        result = sysHyPhy20gRead(fileHndl, intRdRegAddr, &dmiIntRdData);
        if (result) return result;
        
        result = hyPhy20gVarFieldRead(dmiIntRdData, pOutEventMsk, &pOutEvent);
        if (result) return result; 
            
        if (pOutEvent != 0x0) {
            result = hyPhy20gRegFieldWrite (fileHndl, intRdRegAddr, 
                                            pOutEventMsk, 0x0);
            if (result) return result;
        }
        
        /* Wait for one frame to allow propogation of the new payload 
           configuration */    
        result = sysHyPhy20gUsDelay(125);
        if (result) return result;
        
        /* Direct read from DMI:SHPI at 0x493402C + 0x00800*A(A=0:4)
           OBSERVE LINK STS3_3 */ 
        linkRegAddr3 = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(linkRegAddr3, slice);
         
        result = sysHyPhy20gRead(fileHndl, linkRegAddr3, &dmiCfgRegData);
        if (result) return result;
            
        result = hyPhy20gVarFieldRead(dmiCfgRegData, msk3, &stat3);
        if (result) return result;
        
        /* Read PCD_OUT_EVENT register */ 
        result = sysHyPhy20gRead(fileHndl, intRdRegAddr, &dmiIntRdData);
        if (result) return result;
        
        result = hyPhy20gVarFieldRead(dmiIntRdData, pOutEventMsk, &pOutEvent);
        if (result) return result; 
            
        if (pOutEvent == 0x0) {
            valid = 1;
        } else valid = 0;
        
        /* Ensure the value did not change while reading the register:
           the value is valid only when PCD_OUT_EVENT == 0 for the link */
        while ((valid != 1) && (readCount < 10)){
            /* Read payload configuration for STS3_3 */
            result = sysHyPhy20gRead(fileHndl, linkRegAddr3, &dmiCfgRegData);
            if (result) return result;  
                
            result = hyPhy20gVarFieldRead(dmiCfgRegData, msk3, &stat3);
            if (result) return result;    
            
            /* Read interrupt read register */
            result = sysHyPhy20gRead(fileHndl, intRdRegAddr, &dmiIntRdData);
            if (result) return result;
        
            result = hyPhy20gVarFieldRead(dmiIntRdData, pOutEventMsk, 
                                          &pOutEvent);
            if (result) return result; 
            
            if (pOutEvent == 0x0) {
                valid = 1;
                readCount++;
            } else {
                valid = 0;  
                readCount++;
            } 
        }      
    } else if (statusType == 1) {
            
        /* Direct read from DMI at 0x4934108 + 0x00800*A(A=0:4) + 0x40*B(B=0:3) 
           Payload Configuration 1 */
        regAddr1 = HYPHY20G_DMI_SHPI_REG_PLD_CFG_1;
        regAddr1 = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr1, slice);
        regAddr1 = HYPHY20G_DMI_SHPI_TSB_SLICE_REG_OFFSET(regAddr1, port);
    
        result = sysHyPhy20gRead(fileHndl, regAddr1, &dmiShpiRegData);
        if (result) return result; 
        
        msk = HYPHY20G_DMI_SHPI_BIT_PLD_CFG_1_STS3_1_TUG3_MSK;
        result = hyPhy20gVarFieldRead(dmiShpiRegData, msk, &stat1);
        if (result) return result;  
    
        /* Direct read from DMI at 0x493410C + 0x00800*A(A=0:4) + 0x40*B(B=0:3) 
           Payload Configuration 2 */
        regAddr2 = HYPHY20G_DMI_SHPI_REG_PLD_CFG_2;
        regAddr2 = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr2, slice);
        regAddr2 = HYPHY20G_DMI_SHPI_TSB_SLICE_REG_OFFSET(regAddr2, port);
        
        result = sysHyPhy20gRead(fileHndl, regAddr2, &dmiShpiRegData);
        if (result) return result; 
            
        msk = HYPHY20G_DMI_SHPI_BIT_PLD_CFG_2_STS3_2_MSK;
        result = hyPhy20gVarFieldRead(dmiShpiRegData, msk, &stat2);
        if (result) return result;  
    
         /* Direct read from DMI at 0x4934110 + 0x00800*A(A=0:4) + 0x40*B(B=0:3) 
           Payload Configuration 3 */
        regAddr3 = HYPHY20G_DMI_SHPI_REG_PLD_CFG_3;
        regAddr3 = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr3, slice);
        regAddr3 = HYPHY20G_DMI_SHPI_TSB_SLICE_REG_OFFSET(regAddr3, port);
        
        result = sysHyPhy20gRead(fileHndl, regAddr3, &dmiShpiRegData);
        if (result) return result; 
            
        msk = HYPHY20G_DMI_SHPI_BIT_PLD_CFG_3_STS3_3_MSK;
        result = hyPhy20gVarFieldRead(dmiShpiRegData, msk, &stat3);
        if (result) return result;  
    }
    
    *statusWord1 = stat1;
    *statusWord2 = stat2;
    *statusWord3 = stat3; 
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gDmiGetSts1PayloadCfg */

/*******************************************************************************
**
**  hyPhy20gDmiRxPtrIntprtGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function reports the status of the Path Pointer 
**                 interpreter in the DMI Subsystem Rx Direction. 
**
**  INPUTS:       *fileHndl     - base address
**                slice         - DMI block instance
**                                0 to 4
**                port          - 2.5 G DMI processing port
**                                0 to 3 
**                sts12Num      - 0 to 3
**                sts3Num       - 0 to 3
**                sts1Num       - 0 to 2
**                *statusBits   - Pointer to storage for status bits
**                                  Bit 4 - PCOP_V
**                                  Bit 3 - PAISC_V 
**                                  Bit 2 - PLOPC_V
**                                  Bit 1 - PAIS_V 
**                                  Bit 0 - PLOP_V
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gDmiRxPtrIntprtGetStatus(struct file *fileHndl, UINT4 slice, 
                                     UINT4 port, UINT4 sts12Num, UINT4 sts3Num, 
                                     UINT4 sts1Num, UINT4 *statusBits)
{
    /* Variable declaration */
    UINT4 iAddr;
    UINT4 dmiShpiRegData;
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 copPStat, aiscPStat, lopcPStat;
    UINT4 aisPStat, lopPStat;
    INT4 result;
     UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Argument checking */
    if ((slice > 4) || (port > 3) || (sts12Num > 3) || (sts3Num > 3) || 
        (sts1Num > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;   
    }
    
    sts3Off = 4*sts12Num + sts3Num;
    
    /* Read from DMI:SHPI 0x4934100 + 0x00800*A(A=0:4) + 0x40*B(B=0:3)
       indirect address 0x0 Indirect Interrupt Values */
    iAddr = 0x0;
    result = dmiHyPhy20gSHPIIndRead(fileHndl, slice, port, iAddr, sts3Off, 
                                    sts1Num, &dmiShpiRegData);
    if (result) return result;
        
    msk = HYPHY20G_DMI_SHPI_SHPI_IND_IBIT_SHPI_INT_VAL_PCOPV_MSK;
    result = hyPhy20gVarFieldRead(dmiShpiRegData, msk, &copPStat);
    if (result) return result;
        
    msk = HYPHY20G_DMI_SHPI_SHPI_IND_IBIT_SHPI_INT_VAL_PAISCV_MSK;
    result = hyPhy20gVarFieldRead(dmiShpiRegData, msk, &aiscPStat);
    if (result) return result;
    
    msk = HYPHY20G_DMI_SHPI_SHPI_IND_IBIT_SHPI_INT_VAL_PLOPCV_MSK;
    result = hyPhy20gVarFieldRead(dmiShpiRegData, msk, &lopcPStat);
    if (result) return result;
    
    msk = HYPHY20G_DMI_SHPI_SHPI_IND_IBIT_SHPI_INT_VAL_PAISV_MSK;
    result = hyPhy20gVarFieldRead(dmiShpiRegData, msk, &aisPStat);
    if (result) return result;
        
    msk = HYPHY20G_DMI_SHPI_SHPI_IND_IBIT_SHPI_INT_VAL_PLOPV_MSK;
    result = hyPhy20gVarFieldRead(dmiShpiRegData, msk, &lopPStat);
    if (result) return result;
        
    *statusBits = (copPStat << 4) + (aiscPStat << 3) + (lopcPStat << 2) +
                  (aisPStat << 1) + lopPStat;
        
    return HYPHY20G_SUCCESS;
} /* hyPhy20gDmiRxPtrIntprtGetStatus */                              

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/* end of file */
