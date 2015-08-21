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
**  FILE        : hyphy20g_soai_features.c
**
**  $Date: 2009-10-23 11:35:53 -0700 (Fri, 23 Oct 2009) $
**
**  $Revision: 5678 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_sonet_oh_ram.h"

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
**  hyPhy20gSoaiInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function performs a top level reset of the SOAI block.
**
**  INPUTS:       *fileHndl     - base address
**                 enable       - 1 Enable
**                              - 0 Disable (block in low power mode)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiInit(struct file *fileHndl, UINT4 enable)
{
    /* variable declaration */
    UINT4 regAddr;
    UINT4 msk;
    INT4 result;
    
    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    /* Configure top level reset for SOAI */  
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    msk = HYPHY20G_TOP_BIT_SYS_SW_RST_1_SOAI_RST_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 1);
    if (result) return result;

        
    if (enable){
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 0);
        if (result) return result;
    }
    /* apply low power settings */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    msk = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_SOAI_LOWPWR_MSK;
    if (enable) {
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 0);
        if (result) return result;
    } else {
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 1);
        if (result) return result;
    }         

    /* configure SFTWRST bit for SOAI as required */
    regAddr = HYPHY20G_SOAI_SOAI_REG_SFTWRST; 
    msk = HYPHY20G_SOAI_SOAI_BIT_SFTWRST_SOAI_SFTWRST_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, (enable == 0)?1:0);
    if (result) return result;
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSoaiInit */ 

/*******************************************************************************
**
**  hyPhy20gSoaiRcpInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function enables SOAI block for RCP processing for
**                Insertion, Extraction or both Insertion and Extraction.
**
**  INPUTS:       *fileHndl     - base address
**                 dir          - Resource to configure
**                                0 - Tx (Insertion)
**                                1 - Rx (Extraction)
**                                2 - Both Tx and Rx
**                 enable       - 1 Enable
**                              - 0 Disable
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiRcpInit(struct file *fileHndl, UINT4 dir, UINT4 enable)
{
    /* variable declaration */
    UINT4 reg;
    UINT4 msk;
    INT4 result;
    
    /* argument checking */
    if ((dir > 2) || (enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
        
    /* configure *RCP_MASTER_EN for RCP as required */
    reg = HYPHY20G_SOAI_SOAI_REG_MSTR_EN;

     if (dir == 0 || dir == 2) {
        msk = HYPHY20G_SOAI_SOAI_BIT_MSTR_EN_TRCP_MASTER_EN_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, enable);
        if(result) return result;
    }

    if (dir == 1 || dir == 2) {
        msk = HYPHY20G_SOAI_SOAI_BIT_MSTR_EN_RRCP_MASTER_EN_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, enable);
        if(result) return result;
    }
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSoaiRcpInit */

/*******************************************************************************
**
**  hyPhy20gSoaiTohInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function enables SOAI block for TOH processing for
**                Insertion, Extraction or both Insertion and Extraction.
**
**  INPUTS:       *fileHndl     - base address
**                 dir          - Resource to configure
**                                0 - Tx (Insertion)
**                                1 - Rx (Extraction)
**                                2 - Both Tx and Rx
**                 enable       - 1 Enable
**                              - 0 Disable
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiTohInit(struct file *fileHndl, UINT4 dir, UINT4 enable)
{
    /* variable declaration */
    UINT4 reg;
    UINT4 msk;
    INT4 result;
    
    /* argument checking */
    if ((dir > 2) || (enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
        
    /* configure *TOH_MASTER_EN for TOH as required */
    reg = HYPHY20G_SOAI_SOAI_REG_MSTR_EN;

    if (dir == 0 || dir == 2) {
        msk = HYPHY20G_SOAI_SOAI_BIT_MSTR_EN_TTOH_MASTER_EN_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, enable);
        if(result) return result;
    }
    if (dir == 1 || dir == 2) {
        msk = HYPHY20G_SOAI_SOAI_BIT_MSTR_EN_RTOH_MASTER_EN_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, enable);
        if(result) return result;
    }
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSoaiTohInit */

/*******************************************************************************
**
**  hyPhy20gSoaiRcpLinkEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function enables a Receive or Transmit Ring Control Port
**                link.
**                Note: Before enabling a TRCP link, it is required to
**                previously fill the TRCP Data Buffer with data.
**                In the event that _*RCP_MODE changes from any of the valid 
**                modes to the invalid mode, this function need to be called to
**                disable the link, then enable the link, to resume operation on
**                the next frame boundary. 
**
**  INPUTS:       *fileHndl     - base address
**                 link         - SONET slice 0 - 15
**                 dir          - direction of the link to be enabled
**                                0 - Transmit Ring Control Port Link
**                                1 - Receive Ring Control Port Link
**                 enable       - 0 - Disable
**                              - 1 - Enable
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiRcpLinkEnable(struct file *fileHndl, UINT4 link, UINT4 dir,
                               UINT4 enable)
{
    /* variable declaration */
    UINT4 cntReg;
    UINT4 cntMsk;
    INT4 result;

    /* argument checking */
    if ((link > 15) || (dir > 1) || (enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Determine register addresses */        
    cntReg = HYPHY20G_SOAI_RCP_CONTROL_REG_LINKX;
    cntReg = HYPHY20G_SOAI_RCP_CONTROL_TSB_SLICE_REG_OFFSET(cntReg, link);
    
    /* Determine offset to configure */
    if (dir == 0)
    {
        cntMsk = HYPHY20G_SOAI_RCP_CONTROL_BIT_LINKX_LINKX_TRCP_EN_MSK;
    } else {
        cntMsk = HYPHY20G_SOAI_RCP_CONTROL_BIT_LINKX_LINKX_RRCP_EN_MSK;
    }
         
    result = hyPhy20gRegFieldWrite(fileHndl, cntReg, cntMsk, enable);
    if(result) return result;
    
    return HYPHY20G_SUCCESS;

} /* hyPhy20gSoaiRcpLinkEnable */

/*******************************************************************************
**
**  hyPhy20gSoaiRcpGetPointer
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function indicates the current Transmit RCP Read pointer 
**                vlaue or Receive RCP Write Pointer value. 
**                
**  INPUTS:       *fileHndl     - base address
**                 link         - SONET slice 0 - 15
**                 dir          - direction of RCP 
**                                0 - Transmit RCP
**                                1 - Receive RCP  
**                *pPtrVal      - pointer to storage for TRCP Read Pointer
**                                Value:              Description:
**                                0 to 0x87           Location 0 to 135
**                                0x88 to 0xFE        Invalid Values
**                                0xFF                TRCP Data Buffer contains
**                                                    invalid data.
**                              - pointer to storage for RRCP Write pointer
**                                Value:              Description:
**                                0 to 0x1FF          Location 0 to 511
**                                0x200 to 0x3FE      Invalid Values
**                                0x3FF               RRCP Data Buffer contains
**                                                    invalid data.
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiRcpGetPointer(struct file *fileHndl, UINT4 link, UINT4 dir,
                               UINT4 *pPtrVal)
{
    /* variable declaration */
    UINT4 regAddr;
    UINT4 ptrMsk;
    UINT4 val;
    INT4 result;
    
    /* argument checking */
    if ((link > 15) || (dir > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* determine registers and masks */
    regAddr = HYPHY20G_SOAI_RCP_STATUS_REG_LINKX;
    regAddr = HYPHY20G_SOAI_RCP_STATUS_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    if (dir == 0) {
        ptrMsk = HYPHY20G_SOAI_RCP_STATUS_BIT_LINKX_LINKX_TRCP_RDPTR_MSK;
    } else {
        ptrMsk = HYPHY20G_SOAI_RCP_STATUS_BIT_LINKX_LINKX_RRCP_WRPTR_MSK;
    } 
    
    /* retrieve pointer value */
    result = hyPhy20gRegFieldRead(fileHndl, regAddr, ptrMsk, &val);
    if (result) return result;  
    *pPtrVal = val;
      
    return HYPHY20G_SUCCESS;

} /* hyPhy20gSoaiRcpGetPointer */

/*******************************************************************************
**
**  hyPhy20gSoaiRcpGetMode
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function indicates the current Transmit RCP Mode or 
**                Receive Rcp Mode. 
**
**  INPUTS:       *fileHndl     - base address
**                 link         - SONET slice 0 - 15
**                 dir          - direction of RCP 
**                                0 - Transmit RCP
**                                1 - Receive RCP  
**                *pMode        - pointer to storage for RCP mode
**                                Returned values:
**                                0x0 STS-3/STM-1 
**                                0x1 STS-12/STM-4 
**                                0x2 STS-48/STM-16 
**                                0x3 Invalid mode 
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiRcpGetMode(struct file *fileHndl, UINT4 link, UINT4 dir,
                            UINT4 *pMode)
{
    /* variable declaration */
    UINT4 regAddr;
    UINT4 modeMsk;
    UINT4 val;
    INT4 result;
    
    /* argument checking */
    if ((link > 35) || (dir > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* determine registers and masks */
    regAddr = HYPHY20G_SOAI_RCP_STATUS_REG_LINKX;
    regAddr = HYPHY20G_SOAI_RCP_STATUS_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    if (dir == 0) {
        modeMsk = HYPHY20G_SOAI_RCP_STATUS_BIT_LINKX_LINKX_TRCP_MODE_MSK;
    } else {
        modeMsk = HYPHY20G_SOAI_RCP_STATUS_BIT_LINKX_LINKX_RRCP_MODE_MSK;
    }
    
    /* retrieve pointer value */
    result = hyPhy20gRegFieldRead(fileHndl, regAddr, modeMsk, &val);
    if (result) return result;
    *pMode = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSoaiRcpGetMode */


/*******************************************************************************
**
**  hyPhy20gSoaiTxRcpCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures the transmit direction increment
**                 value used to specify gap between two consecutive frames and
**                 the length of circular buffer to use for TRCP data.
**
**  INPUTS:       *fileHndl     - base address
**                 link         - SONET slice 0 - 15   
**                 rdPtrIncr    - increment values used to specify the gap
**                                between two consecutive frames. From end of 
**                                frame n to start of frame n+1.
**                                0 - disable read increment feature, and the 
**                                    TRCP Data Buffer becomes a circular data
**                                    buffer.
**                                0x01 - 0x3F : read Increment value to use
**                 rdWrapVal    - Last data buffer address to write before
**                                wrapping to start of data buffer
**                                0x00 - 0x87 : location 0 to 135
**                                0x88 - 0x1FF: Invalid values equivalent to 135 
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiTxRcpCfg(struct file *fileHndl, UINT4 link, UINT4 rdPtrIncr,
                          UINT4 rdWrapVal)
{
    /* variable declaration */
    UINT4 regAddr, val;
    UINT4 rdIncrMsk, rdWrapMsk;
    INT4 result;
    
    /* argument checking */
    if ((link > 15) || (rdPtrIncr > 0x3f) || (rdWrapVal > 0x1FF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    if (rdWrapVal > 0x87) {
        rdWrapVal = 0x87;
    }
       
    /* Configure _TRCP_RDINCR, _TRCP_RDWRAP */
    regAddr = HYPHY20G_SOAI_RCP_CONTROL_REG_LINKX;
    regAddr = HYPHY20G_SOAI_RCP_CONTROL_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    rdIncrMsk = HYPHY20G_SOAI_RCP_CONTROL_BIT_LINKX_LINKX_TRCP_RDINCR_MSK;
    rdWrapMsk = HYPHY20G_SOAI_RCP_CONTROL_BIT_LINKX_LINKX_TRCP_RDWRAP_MSK;
    
    result = sysHyPhy20gRead(fileHndl, regAddr, &val);
    if(result) return result;       
    
    result = hyPhy20gVarFieldWrite(&val, rdIncrMsk, rdPtrIncr);
    if(result) return result;
   
    result = hyPhy20gVarFieldWrite(&val, rdWrapMsk, rdWrapVal);
    if(result) return result;
    
             
    result = sysHyPhy20gWrite(fileHndl, regAddr, val);
    if(result) return result;    
        
    return HYPHY20G_SUCCESS;

} /* hyPhy20gSoaiTxRcpCfg */                                           
                   
/*******************************************************************************
**
**  hyPhy20gSoaiRxRcpCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures the transmit direction increment
**                 value used to specify gap between two consecutive frames and
**                 the length of circular buffer to use for RRCP data.
**
**  INPUTS:       *fileHndl     - base address
**                 link         - SONET slice 0 - 15   
**                 wrPtrIncr    - increment values used to specify the gap
**                                between two consecutive frames. From end of 
**                                frame n to start of frame n+1.
**                                0 - disable read increment feature, and the 
**                                    RRCP Data Buffer becomes a circular data
**                                    buffer.
**                                0x01 - 0x3F :read Increment value to use
**                 wrWrapVal    - Last data buffer address to write before
**                                wrapping to start of data buffer
**                                0x00 - 0x1FF : locatioin 0 to 511
**                                
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiRxRcpCfg(struct file *fileHndl, UINT4 link, UINT4 wrPtrIncr,
                          UINT4 wrWrapVal)
{
    /* variable declaration */
    UINT4 regAddr, val;
    UINT4 wrIncrMsk, wrWrapMsk;
    INT4 result;
    
    /* argument checking */
    if ((link > 15) || (wrPtrIncr > 0x3f) || (wrWrapVal > 0x1FF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }  
    
    /* Configure _TRCP_WRINCR, _TRCP_WRWRAP */
    regAddr = HYPHY20G_SOAI_RCP_CONTROL_REG_LINKX;
    regAddr = HYPHY20G_SOAI_RCP_CONTROL_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    wrIncrMsk = HYPHY20G_SOAI_RCP_CONTROL_BIT_LINKX_LINKX_RRCP_WRINCR_MSK;
    wrWrapMsk = HYPHY20G_SOAI_RCP_CONTROL_BIT_LINKX_LINKX_RRCP_WRWRAP_MSK;
    
    result = sysHyPhy20gRead(fileHndl, regAddr, &val);
    if(result) return result;
            
    result = hyPhy20gVarFieldWrite(&val, wrIncrMsk, wrPtrIncr);
    if(result) return result;
   
    result = hyPhy20gVarFieldWrite(&val, wrWrapMsk, wrWrapVal);
    if(result) return result;
       
    result = sysHyPhy20gWrite(fileHndl, regAddr, val);
    if(result) return result;    
        
    return HYPHY20G_SUCCESS;

} /* hyPhy20gSoaiRxRcpCfg */   


/*******************************************************************************
**
**  hyPhy20gSonetRcpRamBurstRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs a 128 byte burst read access to the
**                  Ring Control Ports RAM. In cases where the starting access 
**                  address is less than 128 bytes (32 rows) from the end of the
**                  RAM for the framer. The function will set the pointer go 
**                  back to the start of the circular buffer.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - 0 - 15 SONET slice
**                 addr         - RAM row number to burst read to
**                                0 - 511 
**                *pData[32]    - Pointer to an array for read date from TOH RAM
**                                Consists of 32 RAM rows.                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetRcpRamBurstRead(struct file *fileHndl, UINT4 slice, 
                                  UINT4 addr, UINT4 *pData)
{
    /* variable declaration */
    UINT4 regAddr, startAddr;
    UINT4 tmpData[32];
    UINT4 szToEnd, readRest;
    UINT4 i = 0, j = 0;
    INT4 result;
    
    /* argument checking */
    if ((slice > 15) || (addr > 511)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
 
    /* Determine register for each RAM */ 
    startAddr = HYPHY20G_SONET_OH_SOAI_RCP_REG_STR_RAM_OFFSET(0);
    startAddr = HYPHY20G_SONET_OH_SOAI_RCP_TSB_SLICE_REG_OFFSET(startAddr, slice);   
    regAddr = HYPHY20G_SONET_OH_SOAI_RCP_REG_STR_RAM_OFFSET(addr);
    regAddr = HYPHY20G_SONET_OH_SOAI_RCP_TSB_SLICE_REG_OFFSET(regAddr, slice);
    
    /* Calculate number of reads until the end of the RAM */
    szToEnd = 512 - addr; /* RCP RAM size = 512 words */
    
    /* Calculate number of reads after wrap around */
    readRest = 32 - szToEnd;
    
    /* if the starting access addres is less than 128 bytes from the end of 
       the RAM for the framer */
    if (szToEnd < 32) {
        /* if the starting access addres is less than 128 bytes from the end of 
           the RAM for the framer */
        result = sysHyPhy20gPcieBurstRead(fileHndl, regAddr, szToEnd, tmpData);
        if (result) {  
            return result;
        }

        /* Copy wrapped around data */      
        for (i = 0; i < szToEnd; i++) {
            pData[i] = tmpData[i];  
        }
		
        /* Wrap around and read from the beginning of the RAM */
        result = sysHyPhy20gPcieBurstRead(fileHndl, startAddr, readRest,
                                          tmpData);
        if (result) {
            return result; 
        }
        
        /* Copy wrapped around data */      
        for (j = 0; j < readRest; j++) {
            pData[j+szToEnd] = tmpData[j];  
        }
        
    }
    /* if the starting access addres is more than 128 bytes from the end of 
       the RAM for the framer */
    else {
        result = sysHyPhy20gPcieBurstRead(fileHndl, regAddr, 32, tmpData);
        if (result) {
            return result;
        }
		
		/* Copy data to return */   
	    for (i = 0; i < 32; i++) {
	         pData[i] = tmpData[i];         
	    }   
    }
      
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetRcpRamBurstRead */                                      
                                  
/*******************************************************************************
**
**  hyPhy20gSonetRcpRamBurstWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs a 128 byte burst write access to
**                  Ring Control Ports RAM. In cases where the starting access 
**                  address is less than 128 bytes (32 rows) from the end of the
**                  RAM for the framer. The function will set the pointer go 
**                  back to the start of the circular buffer.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - 0 - 15  - SONET slice
**                 addr         - RAM row number to burst write to
**                                0 - 135 
**                 data         - Data to be written to TOH RAM
**                                Consists of 32 RAM rows.
**                                
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetRcpRamBurstWrite(struct file *fileHndl, UINT4 slice, 
                                   UINT4 addr, UINT4 data[32])
{
    /* variable declaration */
    UINT4 szToEnd, writeRest;
    UINT4 regAddr, startAddr;
    INT4 result;
    
    /* argument checking */                                   
    if ((slice > 35) || (addr > 135 )) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }                             
    
    /* Determine register for each RAM */ 
    startAddr = HYPHY20G_SONET_OH_SOAI_RCP_REG_STR_RAM_OFFSET(0);
    startAddr = HYPHY20G_SONET_OH_SOAI_RCP_TSB_SLICE_REG_OFFSET(startAddr, slice);   
    regAddr = HYPHY20G_SONET_OH_SOAI_RCP_REG_STR_RAM_OFFSET(addr);
    regAddr = HYPHY20G_SONET_OH_SOAI_RCP_TSB_SLICE_REG_OFFSET(regAddr, slice);
    
    /* Calculate number of writes until the end of the RAM */
    szToEnd = 136 - addr; /* RCP RAM size = 136 words */                           

    /* Calculate number of writes after wrap around */
    writeRest = 32 - szToEnd;
    
    /* if the starting access addres is less than 128 bytes from the end of 
       the RAM for the framer */
    if (szToEnd < 32) {
        
        /* the pointer will go back to the start of the circular buffer */
        result = sysHyPhy20gPcieBurstWrite(fileHndl, regAddr, szToEnd, data);
        if (result) {
            return result; 
        }

        result = sysHyPhy20gPcieBurstWrite(fileHndl, startAddr, writeRest, 
                                           &data[szToEnd]);
        if (result) {
            return result;
        }
    }
    /* if the starting access addres is more than 128 bytes from the end of 
       the RAM for the framer */
    else {
        result = sysHyPhy20gPcieBurstWrite(fileHndl, regAddr, 32, data);
        if (result) {
            return result;
        }
    }
         
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetRcpRamBurstWrite */


/*******************************************************************************
**
**  hyPhy20gSoaiTohLinkEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function enables a Receive or Transmit Transport Overhead
**                Link.
**                
**                Note: Before enabling a TTOH link, it is required to
**                previously fill the TTOH Data Buffer with data.
**                In the event that _*TOH_MODE changes from any of the valid 
**                modes to the invalid mode, this function need to be called to
**                disable the link, then enable the link, to resume operation on
**                the next frame boundary. 
**                               
**  INPUTS:       *fileHndl     - base address
**                 link         - SONET slice or TSE/DMI port
**                                valid range 0 - 35
**                                0 - 19  - TSE ports OD0 to OD20 (for dir = 0)
**                                          TSE ports ID20 to ID39 (for dir = 1)
**                                20 - 35 - SONET slice 0 to 15
**                 dir          - direction of a link to be enabled
**                                0 - Transmit TOH
**                                1 - Receive TOH
**                 enable       - 0 - Disable
**                              - 1 - Enable
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiTohLinkEnable(struct file *fileHndl, UINT4 link, UINT4 dir,
                               UINT4 enable)
{
    /* variable declaration */
    UINT4 cntReg;
    UINT4 cntMsk;
    INT4 result;

    /* argument checking */
    if ((link > 35) || (dir > 1) || (enable >1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */      
    cntReg = HYPHY20G_SOAI_TOH_CONTROL_REG_LINKX;
    cntReg = HYPHY20G_SOAI_TOH_CONTROL_TSB_SLICE_REG_OFFSET(cntReg, link);
    /* Determine msk offset to configure */
    if (dir == 0) {
        cntMsk = HYPHY20G_SOAI_TOH_CONTROL_BIT_LINKX_LINKX_TTOH_EN_MSK;
    } else {
        cntMsk = HYPHY20G_SOAI_TOH_CONTROL_BIT_LINKX_LINKX_RTOH_EN_MSK;
    }
    
    /* configure the register for the requested resource */  
    result = hyPhy20gRegFieldWrite(fileHndl, cntReg, cntMsk, enable);
    if(result) return result;
        
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSoaiTohLinkEnable */

/*******************************************************************************
**
**  hyPhy20gSoaiTohGetPointer
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function indicates the current Transmit TOH Read pointer 
**                vlaue or Receive TOH Write Pointer value. 
**
**  INPUTS:       *fileHndl     - base address
**                 link         - SONET slice or TSE/DMI port
**                                valid range 0 - 35
**                                0 - 19  - DMI slice 0 to 4 ports connected to
**                                          TSE ports OD0 to OD20 (for dir = 0)
**                                          TSE ports ID20 to ID39 (for dir = 1)
**                                20 - 35 - SONET slice 0 to 15 
**                 dir          - direction of TOH 
**                                0 - Transmit TOH
**                                1 - Receive TOH
**                *pPtrVal      - pointer to storage for TTOH Read Pointer
**                                Value:             Description:
**                                0 to 0x147         Location 0 to 327
**                                0x148 to 0x1FE     Invalid values
**                                0x1FF              TTOH Data Buffer contains
**                                                   invalid data.
**                              - pointer to storage for RTOH Write pointer
**                                Value:             Description:
**                                0 to 0x147         Location 0 to 327
**                                0x148 to 0x1FE     Invalid values
**                                0x1FF              RTOH Data Buffer contains
**                                                   invalid data.
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiTohGetPointer(struct file *fileHndl, UINT4 link, UINT4 dir,
                               UINT4 *pPtrVal)
{
    /* variable declaration */
    UINT4 regAddr;
    UINT4 ptrMsk;
    UINT4 val;
    INT4 result;
    
    /* argument checking */
    if ((link > 35) || (dir > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* determine registers and masks */
    regAddr = HYPHY20G_SOAI_TOH_STATUS_REG_LINKX;
    regAddr = HYPHY20G_SOAI_TOH_STATUS_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    if (dir == 0) {
        ptrMsk = HYPHY20G_SOAI_TOH_STATUS_BIT_LINKX_LINKX_TTOH_RDPTR_MSK;
    } else {
        ptrMsk = HYPHY20G_SOAI_TOH_STATUS_BIT_LINKX_LINKX_RTOH_WRPTR_MSK;
    } 
    
    /* retrieve pointer value */
    result = hyPhy20gRegFieldRead(fileHndl, regAddr, ptrMsk, &val);
    if (result) return result;
    *pPtrVal = val;
      
    return HYPHY20G_SUCCESS;

} /* hyPhy20gSoaiTohGetPointer */

/*******************************************************************************
**
**  hyPhy20gSoaiTohGetMode
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function indicates the current Transmit TOH Mode or 
**                Receive TOH Mode. 
**
**  INPUTS:       *fileHndl     - base address
**                 link         - SONET slice or TSE/DMI port
**                                valid range 0 - 35
**                                0 - 19  - DMI slice 0 to 4 ports connected to
**                                          TSE ports OD0 to OD20 (for dir = 0)
**                                          TSE ports ID20 to ID39 (for dir = 1)
**                                20 - 35 - SONET slice 0 to 15 
**                 dir          - direction of TOH 
**                                0 - Transmit TOH
**                                1 - Receive TOH
**                *pMode        - pointer to storage for TOH mode
**                                Returned values:
**                                0x0 STS-3/STM-1 
**                                0x1 STS-12/STM-4 
**                                0x2 STS-48/STM-16 
**                                0x3 Invalid mode 
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiTohGetMode(struct file *fileHndl, UINT4 link, UINT4 dir,
                            UINT4 *pMode)
{
    /* variable declaration */
    UINT4 regAddr;
    UINT4 modeMsk;
    UINT4 val;
    INT4 result;
    
    /* argument checking */
    if ((link > 35) || (dir > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* determine registers and masks */
    regAddr = HYPHY20G_SOAI_TOH_STATUS_REG_LINKX;
    regAddr = HYPHY20G_SOAI_TOH_STATUS_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    if (dir == 0) {
        modeMsk = HYPHY20G_SOAI_TOH_STATUS_BIT_LINKX_LINKX_TTOH_MODE_MSK;
    } else {
        modeMsk = HYPHY20G_SOAI_TOH_STATUS_BIT_LINKX_LINKX_RTOH_MODE_MSK;
    }
    
    /* retrieve pointer value */
    result = hyPhy20gRegFieldRead(fileHndl, regAddr, modeMsk, &val);
    if (result) return result;
    *pMode = val;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSoaiTohGetMode */

/*******************************************************************************
**
**  hyPhy20gSoaiTxTohCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures the transmit direction increment
**                 value used to specify gap between two consecutive frames and
**                 the length of circular buffer to use for TTOH data.
**
**  INPUTS:       *fileHndl     - base address
**                 link         - SONET slice or TSE/DMI port
**                                valid range 0 - 35
**                                0 - 19  - TSE ports OD0 to OD20 (for dir = 0)
**                                          TSE ports ID20 to ID39 (for dir = 1)
**                                20 - 35 - SONET slice 0 to 15
**                rdPtrIncr     - increment values used to specify the gap
**                                between two consecutive frames. From end of 
**                                frame n to the start of frame n+1.
**                                0 - disable read increment feature, and the 
**                                    TTOH Data Buffer becomes a circular data
**                                    buffer.
**                                0x01 - 0x3F: read increment value to use
**                rdWrapVal     - Last data buffer address to write before
**                                wrapping to start of data buffer
**                                0x000 - 0x147 : locatioin 0 to 327
**                                0x148 - 0x1FF: defaults to 327 
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiTxTohCfg(struct file *fileHndl, UINT4 link, UINT4 rdPtrIncr,
                         UINT4 rdWrapVal)
{
    /* variable declaration */
    UINT4 regAddr, val;
    UINT4 rdIncrMsk, rdWrapMsk ,insenMsk;
    INT4 result;
    
    /* argument checking */
    if ((link > 35) || (rdPtrIncr > 0x3f) || (rdWrapVal > 0x1FF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    if (rdWrapVal > 0x147) {
        rdWrapVal = 0x147;
    }
    
    /* Configure _TTOHPINSEN */   
    if (link > 19) {
        regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_CFG1;        
        regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, link - 20);
        insenMsk = HYPHY20G_SONET_TOP_BIT_SDH_PROC_SLC_CFG1_TTOHPINSEN_MSK;
        
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, insenMsk, 1);
        if(result) return result;
    } 
    /* Configure _TRCP_RDINCR, _TRCP_RDWRAP */
    regAddr = HYPHY20G_SOAI_TOH_CONTROL_REG_LINKX;
    regAddr = HYPHY20G_SOAI_TOH_CONTROL_TSB_SLICE_REG_OFFSET(regAddr, link);
                    
    rdIncrMsk = HYPHY20G_SOAI_TOH_CONTROL_BIT_LINKX_LINKX_TTOH_RDINCR_MSK;
    rdWrapMsk = HYPHY20G_SOAI_TOH_CONTROL_BIT_LINKX_LINKX_TTOH_RDWRAP_MSK;
    
    result = sysHyPhy20gRead(fileHndl, regAddr, &val);
    if(result) return result;
           
    result = hyPhy20gVarFieldWrite(&val, rdIncrMsk, rdPtrIncr);
    if(result) return result;
   
    result = hyPhy20gVarFieldWrite(&val, rdWrapMsk, rdWrapVal);
    if(result) return result;
        
    result = sysHyPhy20gWrite(fileHndl, regAddr, val);
    if(result) return result;    
        
    return HYPHY20G_SUCCESS;    

} /* hyPhy20gSoaiTxTohCfg */      

/*******************************************************************************
**
**  hyPhy20gSoaiRxTohCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures the transmit direction increment
**                 value used to specify gap between two consecutive frames and
**                 the length of circular buffer to use for RTOH data.
**
**  INPUTS:       *fileHndl     - base address
**                 link         - SONET slice or TSE/DMI port
**                                valid range 0 - 35
**                                0 - 19  - TSE ports OD0 to OD20 (for dir = 0)
**                                          TSE ports ID20 to ID39 (for dir = 1)
**                                20 - 35 - SONET slice 0 to 15
**                wrPtrIncr     - increment values used to specify the gap
**                                between two consecutive frames. From end of 
**                                frame n to the start of frame n+1.
**                                0 - disable read increment feature, and the 
**                                    RTOH Data Buffer becomes a circular data
**                                    buffer.
**                                0x01 - 0x3F: read increment value to use
**                wrWrapVal     - Last data buffer address to write before
**                                wrapping to start of data buffer
**                                0x000 - 0x147 : locatioin 0 to 327
**                                0x148 - 0x1FF: defaults to 327 
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiRxTohCfg(struct file *fileHndl, UINT4 link, UINT4 wrPtrIncr,
                          UINT4 wrWrapVal)
{
    /* variable declaration */
    UINT4 regAddr, val;
    UINT4 wrIncrMsk, wrWrapMsk, extrenMsk;
    INT4 result;
    
    /* argument checking */
    if ((link > 35) || (wrPtrIncr > 0x3f) || (wrWrapVal > 0x1FF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    if (wrWrapVal > 0x147) {
        wrWrapVal = 0x147;
    }
    
    /* Configure _RTOHPEXTREN */
    if (link > 19) {
        regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_CFG1;
        regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, link - 20);
        extrenMsk = HYPHY20G_SONET_TOP_BIT_SDH_PROC_SLC_CFG1_RTOHPEXTREN_MSK;

        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, extrenMsk, 1);
        if(result) return result;
    }
       
    /* Configure _TRCP_WRINCR, _TRCP_WRWRAP */
    regAddr = HYPHY20G_SOAI_TOH_CONTROL_REG_LINKX;
    regAddr = HYPHY20G_SOAI_TOH_CONTROL_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    wrIncrMsk = HYPHY20G_SOAI_TOH_CONTROL_BIT_LINKX_LINKX_RTOH_WRINCR_MSK;
    wrWrapMsk = HYPHY20G_SOAI_TOH_CONTROL_BIT_LINKX_LINKX_RTOH_WRWRAP_MSK;
    
    result = sysHyPhy20gRead(fileHndl, regAddr, &val);
    if(result) return result;
       
    result = hyPhy20gVarFieldWrite(&val, wrIncrMsk, wrPtrIncr);
    if(result) return result;
   
    result = hyPhy20gVarFieldWrite(&val, wrWrapMsk, wrWrapVal);
    if(result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, regAddr, val);
    if(result) return result;    
        
    return HYPHY20G_SUCCESS; 
} /* hyPhy20gSoaiRxTohCfg */  

/*******************************************************************************
**
**  hyPhy20gSoaiTohMaskCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures the mask of overhead bytes to
**                 insert or extract for a link.  Overhead bytes are configured
**                 at an STS-1/STM-0 level.   For each STS-1/STM-0 one or more
**                 of the 27 associated overhead bits may be filtered.
**
**                 Note: Take note that the *TOH_MSK registers are accessible 
**                 by software only when *TOH_EN is set to logic 0. The hardware
**                 uses the *TOH_MSK only when *TOH_EN is set to logic 1.
**
**  INPUTS:       *fileHndl     - base address
**                 link         - SONET slice or TSE/DMI port
**                                valid range 0 - 35
**                                0 - 19  - TSE ports OD0 to OD20 (for dir = 0)
**                                          TSE ports ID20 to ID39 (for dir = 1)
**                                20 - 35 - SONET slice 0 to 15
**                 dir          - Direction to configure:
**                                0 - Tx (insertion)
**                                1 - Rx (extraction)
**                 sts12Num     - STS-12 number within the link to be configured
**                                0 to 3
**                 sts3Num      - STS-3 number within the link to be configured
**                                0 to 3
**                 sts1Num      - STS-1 number within the link to be configured
**                                0 to 2
**                                For STM-1/STS-3 ovehead columns 1 to 9
**                                sts1Num set to 0 composes columns 1, 4, 7
**                                sts1Num set to 1 composes columns 2, 5, 8
**                                sts1Num set to 2 composes columns 3, 6, 9
**                 enMsk        - mask of the 27 STS-1 overhead bits to enable
**                                for insertion or extraction, with bits:
**                                2:0   - row 1 columns 3:1
**                                5:3   - row 2 columns 3:1
**                                8:6   - row 3 columns 3:1
**                                11:9  - row 4 columns 3:1
**                                14:12 - row 5 columns 3:1
**                                17:15 - row 6 columns 3:1
**                                20:18 - row 7 columns 3:1
**                                23:21 - row 8 columns 3:1
**                                26:24 - row 9 columns 3:1
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSoaiTohMaskCfg(struct file *fileHndl, UINT4 link, UINT4 dir, 
                            UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num,
                            UINT4 enMsk)
{
    /* variable declaration */
    UINT4 regAddr;
    UINT4 n;
    INT4 result;
    
    /* argument checking */                                   
    if ((link > 35) || (dir > 1 ) || (sts12Num > 3) || (sts3Num > 3) ||
        (sts1Num > 2) || (enMsk > 0x7FFFFFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    } 
    
    /* Determine index n */
    n = 16 * sts1Num + sts3Num + 4 * sts12Num;
    
    /* Determine register addresses  */    
    if (dir == 0) {
        regAddr = HYPHY20G_SONET_OH_SOAI_TOH_REG_TTOH_MSK_RAM_OFFSET(n);
        regAddr = HYPHY20G_SONET_OH_SOAI_TOH_TSB_SLICE_REG_OFFSET(regAddr, link);
    } else {
        regAddr = HYPHY20G_SONET_OH_SOAI_TOH_REG_RTOH_MSK_RAM_OFFSET(n);
        regAddr = HYPHY20G_SONET_OH_SOAI_TOH_TSB_SLICE_REG_OFFSET(regAddr, link);
    } 
      
    result = sysHyPhy20gWrite(fileHndl, regAddr, enMsk);
    if(result) return result;    
        
    return HYPHY20G_SUCCESS;    

} /* hyPhy20gSoaiTohMaskCfg */    

/*******************************************************************************
**
**  hyPhy20gSonetTohRamBurstRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs a 128 byte burst read access to the
**                  Transport Overhead RAM. In cases where the starting access 
**                  address is less than 128 bytes (32 rows) from the end of the
**                  RAM for the framer. The function will set the pointer go 
**                  back to the start of the circular buffer.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - 0 - 19  - TSE ports
**                                20 - 35 - SONET slice 0 to 15
**                 addr         - RAM row number to burst read to
**                                0 - 327 
**                *pData[32]    - Pointer to an array for read date from TOH RAM
**                                Consists of 32 RAM rows.                                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohRamBurstRead(struct file *fileHndl, UINT4 slice, 
                                  UINT4 addr, UINT4 *pData)
{
    /* variable declaration */
    UINT4 regAddr, startAddr;
    UINT4 tmpData[32];
    UINT4 szToEnd, readRest;
    UINT4 i = 0, j = 0;
    INT4 result;
    
    /* argument checking */
    if ((slice > 35) || (addr > 327)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
 
    /* Determine register for each RAM */ 
    startAddr = HYPHY20G_SONET_OH_SOAI_TOH_REG_STR_RAM_OFFSET(0);
    startAddr = HYPHY20G_SONET_OH_SOAI_TOH_TSB_SLICE_REG_OFFSET(startAddr, slice);   
    regAddr = HYPHY20G_SONET_OH_SOAI_TOH_REG_STR_RAM_OFFSET(addr);
    regAddr = HYPHY20G_SONET_OH_SOAI_TOH_TSB_SLICE_REG_OFFSET(regAddr, slice);
    
    /* Calculate number of reads until the end of the RAM */
    szToEnd = 328 - addr; /* TOH RAM size = 328 words */
    
    /* Calculate number of reads after wrap around */
    readRest = 32 - szToEnd;
    
    /* if the starting access addres is less than 128 bytes from the end of 
       the RAM for the framer */
    if (szToEnd < 32) {
        
        /* if the starting access addres is less than 128 bytes from the end of 
           the RAM for the framer */
        result = sysHyPhy20gPcieBurstRead(fileHndl, regAddr, szToEnd, tmpData);
        if (result) { 
            return result;
        }
		
         /* Copy wrapped around data */      
        for (i = 0; i < readRest; i++) {
            pData[i] = tmpData[i];  
        }   
        
        /* Wrap around and read from the beginning of the RAM */
        result = sysHyPhy20gPcieBurstRead(fileHndl, startAddr, readRest,
                                          tmpData);
        if (result) {
            return result; 
        }
        
        /* Copy wrapped around data */      
        for (j = 0; j < readRest; j++) {
            pData[j+szToEnd] = tmpData[j];  
        }
        
    }
    /* if the starting access addres is more than 128 bytes from the end of 
       the RAM for the framer */
    else {
        result = sysHyPhy20gPcieBurstRead(fileHndl, regAddr, 32, tmpData);
        if (result) {
            return result;
        }

       /* Copy data to return */   
	    for (i = 0; i < 32; i++) {
	         pData[i] = tmpData[i];         
	         // DBG_PRINT("pData[%li] = 0x%lx\n", i, pData[i]);
	    }   
      
    }
    
      
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohRamBurstRead */                                      
                                  
/*******************************************************************************
**
**  hyPhy20gSonetTohRamBurstWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs a 128 byte burst write access to
**                  Transport Overhead RAM. In cases where the starting access 
**                  address is less than 128 bytes (32 rows) from the end of the
**                  RAM for the framer. The function will set the pointer go 
**                  back to the start of the circular buffer.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - 0 - 19  - TSE ports
**                                20 - 35 - SONET slice 0 to 15
**                 addr         - RAM row number to burst write to
**                                0 - 327 
**                 data         - Data to be written to TOH RAM
**                                Consists of 32 RAM rows.
**                                
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohRamBurstWrite(struct file *fileHndl, UINT4 slice, 
                                   UINT4 addr, UINT4 data[32])
{
    /* variable declaration */
    UINT4 szToEnd, writeRest;
    UINT4 regAddr, startAddr;
    INT4 result;
    
    /* argument checking */                                   
    if ((slice > 35) || (addr > 327 )) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }                             
    
    /* Determine register for each RAM */ 
    startAddr = HYPHY20G_SONET_OH_SOAI_TOH_REG_STR_RAM_OFFSET(0);
    startAddr = HYPHY20G_SONET_OH_SOAI_TOH_TSB_SLICE_REG_OFFSET(startAddr, slice);   
    regAddr = HYPHY20G_SONET_OH_SOAI_TOH_REG_STR_RAM_OFFSET(addr);
    regAddr = HYPHY20G_SONET_OH_SOAI_TOH_TSB_SLICE_REG_OFFSET(regAddr, slice);
    
    /* Calculate number of writes until the end of the RAM */
    szToEnd = 328 - addr; /* TOH RAM size = 328 words */                           

    /* Calculate number of writes after wrap around */
    writeRest = 32 - szToEnd;
    
    /* if the starting access addres is less than 128 bytes from the end of 
       the RAM for the framer */
    if (szToEnd < 32) {

        /* the pointer will go back to the start of the circular buffer */
        result = sysHyPhy20gPcieBurstWrite(fileHndl, regAddr, szToEnd, data);
        if (result) {
            return result; 
        }
        
        result = sysHyPhy20gPcieBurstWrite(fileHndl, startAddr, writeRest, 
                                           &data[szToEnd]);
        if (result) {
            return result;
        }
    }
    /* if the starting access addres is more than 128 bytes from the end of 
       the RAM for the framer */
    else {
        result = sysHyPhy20gPcieBurstWrite(fileHndl, regAddr, 32, data);
        if (result) {
            return result;
        }
    }
         
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohRamBurstWrite */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/* end of file */

