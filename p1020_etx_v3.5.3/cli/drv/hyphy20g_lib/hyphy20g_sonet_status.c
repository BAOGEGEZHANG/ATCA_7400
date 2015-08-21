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
**  FILE        : hyphy20g_sonet_status.c
**
**  $Date: 2011-03-11 17:07:43 +0800 (Fri, 11 Mar 2011) $
**
**  $Revision: 11320 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/


/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_sonet_status.h"

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
**  hyPhy20gSonetTohGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function reports the status of section/line events 
**                  within a SONET slice and the associated processing resource
**                  within SONET_LINE.  
**
**                  Some status bits such as TIM_V and TIM_U are invalid until
**                  trail trace messages are properly configured.
**
**                  Note that TIM_V bit is invalid when 1 byte TTM is used.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15 
**                  *statusBits  - Pointer to storage for status bits
**                                  Bit 10 - LOT_V
**                                  Bit 9 - APSBF_V
**                                  Bit 8 - LRDI_V 
**                                  Bit 7 - LAIS_V
**                                  Bit 6 - LOS_V 
**                                  Bit 5 - LOF_V
**                                  Bit 4 - OOF_V
**                                  Bit 3 - SF_V
**                                  Bit 2 - SD_V
**                                  Bit 1 - TIM_V
**                                  Bit 0 - TIU_V
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetTohGetStatus(struct file *fileHndl, UINT4 slice, 
                               UINT4 *statusBits)
{
    /* variable declaration */ 
	UINT4 iaddr; 
	UINT4 regAddr;			
	UINT4 sfbaCfgStatRegData; 
	UINT4 rttpStatRegData;
	UINT4 sberStatRegData;
	UINT4 rrmpStatRegData;
	UINT4 msk;
	UINT4 lotStat = 0;
	UINT4 apsbfStat = 0;
	UINT4 rdilStat = 0;
	UINT4 lofStat = 0;
	UINT4 losStat = 0;
	UINT4 oofStat = 0;
	UINT4 aislStat = 0;
	UINT4 timStat = 0;
	UINT4 tiuStat = 0;
	UINT4 sfStat = 0;
	UINT4 sdStat = 0;
	INT4 result; 

	/* argument checking */
	if (slice > 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }	
    
    /* register address for SFBA configuration and status register */ 
    regAddr = HYPHY20G_SONET_LINE_SFBA_REG_CFG_AND_STAT;
    regAddr = HYPHY20G_SONET_LINE_SFBA_TSB_SLICE_REG_OFFSET(regAddr, slice);
    
    /* direct register read from SFBA register */ 
    result = sysHyPhy20gRead(fileHndl, regAddr, &sfbaCfgStatRegData);
    if (result) return result; 
    
    /* debug purpose */
    result = sysHyPhy20gWrite(fileHndl, regAddr, 0x6);
    if (result) return result; 
    
    result = sysHyPhy20gRead(fileHndl, regAddr, &sfbaCfgStatRegData);
    if (result) return result;
    /* debug end */ 
       
    /* LOT 0x49500C0 + 0x10 *A (A=0:15) LOT_V Status of LOT defect */
    msk = HYPHY20G_SONET_LINE_SFBA_BIT_CFG_AND_STAT_LOT_V_MSK;
    result = hyPhy20gVarFieldRead(sfbaCfgStatRegData, msk, &lotStat);
    if (result) return result;  

	/* register address for RRMP status register */
	regAddr = HYPHY20G_SONET_RRMP_REG_STAT;
	regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
	
	/* direct register read from RRMP status register */ 
	result = sysHyPhy20gRead(fileHndl, regAddr, &rrmpStatRegData);
	if (result) return result;
	    	
    /* APSBF 0x4940088+0x01000*A(A=0:15) APSBF_V Status of APS failure defect */
	msk =  HYPHY20G_SONET_RRMP_BIT_STAT_APSBF_V_MSK;
	result = hyPhy20gVarFieldRead(rrmpStatRegData, msk, &apsbfStat);
	if (result) return result;            
    			
	/* LRDI 0x4940088 + 0x1000 *A (A=0:15) LRDI_V Status of LRDI defect */
	msk = HYPHY20G_SONET_RRMP_BIT_STAT_LRDI_V_MSK;
	result = hyPhy20gVarFieldRead(rrmpStatRegData, msk, &rdilStat);	
	if (result) return result;
	    
    /* LAIS 0x4940088 + 0x1000 *A (A=0:15) LAIS_V Status of LAIS defect */
    msk = HYPHY20G_SONET_RRMP_BIT_STAT_LAIS_V_MSK; 
	result = hyPhy20gVarFieldRead(rrmpStatRegData, msk, &aislStat);
	if (result) return result;
	    		
	/* LOS 0x4940088 + 0x01000 *A (A=0:15) LOS_V Status of LOS defect */
	msk = HYPHY20G_SONET_RRMP_BIT_STAT_LOS_V_MSK;
	result = hyPhy20gVarFieldRead(rrmpStatRegData, msk, &losStat);
	if (result) return result;
	    			
	/* LOF Signal 0x4940088 + 0x01000 *A (A=0:15) LOF_V Status of LOF defect */
	msk = HYPHY20G_SONET_RRMP_BIT_STAT_LOF_V_MSK; 
	result = hyPhy20gVarFieldRead(rrmpStatRegData, msk, &lofStat);
	if (result) return result;
	    	
	/* OOF 0x4940088 + 0x01000 *A (A=0:15) OOF_V Status of OOF defect */ 
	msk = HYPHY20G_SONET_RRMP_BIT_STAT_OOF_V_MSK;
	result = hyPhy20gVarFieldRead(rrmpStatRegData, msk, &oofStat);
	if (result) return result;
	    		
	/* TIM_V and TIU_V Ind addr 0x4940180 + 0x01000 *A (A=0:15) RTTP_Section */
	iaddr = 0x0; /* indirect TIM/TIU RTTP Status memory */
	result = sonetHyPhy20gRTTPSectIndRead(fileHndl, slice, iaddr, 
	                                      &rttpStatRegData);
	if (result) return result;
	 
	msk = 
	    HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_MSMTCH_UNSTBL_STAT_TIM_V_MSK;
	result = hyPhy20gVarFieldRead(rttpStatRegData, msk, &timStat);
	if (result) return result;
	
	msk = 
	    HYPHY20G_SONET_RTTP_SECT_RTTP_IND_IBIT_TRC_MSMTCH_UNSTBL_STAT_TIU_V_MSK;
	result = hyPhy20gVarFieldRead(rttpStatRegData, msk, &tiuStat);
	if (result) return result; 
	 
	/* SF_V and SD_V Ind addr 0x4940100 + 0x01000 *A (A=0:15) SBER_Section */
	iaddr = 0x0; /* indirect SD/SF BERM Status memory */ 
	result = sonetHyPhy20gSBERSectIndRead(fileHndl, slice, iaddr,
	                                      &sberStatRegData);
	if (result) return result;
	    
	msk = HYPHY20G_SONET_SBER_SECT_SBER_IND_IBIT_SDSF_BERM_STAT_SF_V_MSK;                                     
	result = hyPhy20gVarFieldRead(sberStatRegData, msk, &sfStat);
	if (result) return result;
	    
	msk = HYPHY20G_SONET_SBER_SECT_SBER_IND_IBIT_SDSF_BERM_STAT_SD_V_MSK;
    result = hyPhy20gVarFieldRead(sberStatRegData, msk, &sdStat);
             
	*statusBits = (lotStat << 10) + (apsbfStat << 9) + (rdilStat << 8) + 
	              (aislStat << 7) + (losStat << 6) + (lofStat << 5) + 
	              (oofStat << 4) + (sfStat << 3) + (sdStat << 2) + 
	              (timStat << 1) + tiuStat; 
	                                          
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetTohGetStatus */

/*******************************************************************************
**
**  hyPhy20gSonetPohGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of High-Order Path alarms
**                  within the SONET subsystem.
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 15 
**                sts12Num     - 0 to 3 
**                sts3Num      - 0 to 3 
**                sts1Num      - 0 to 2
**                *statusBits  - Pointer to storage for status bits
**                                  Bit 9 - TIM_V 
**                                  Bit 8 - TIU_V
**                                  Bit 7 - PERDI_V 
**                                  Bit 6 - PRDI_V 
**                                  Bit 5 - PPDI_V 
**                                  Bit 4 - PUNEQ_V
**                                  Bit 3 - PPLM_V 
**                                  Bit 2 - PPLU_V
**                                  Bit 1 - SF_V 
**                                  Bit 0 - SD_V
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
INT4 hyPhy20gSonetPohGetStatus(struct file *fileHndl, UINT4 slice, 
                               UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num, 
                               UINT4 *statusBits)
{
    /* variable declaration */ 
	UINT4 iAddr; 		
	UINT4 sonetRttpRegData;
	UINT4 sonetRhapRegData;
	UINT4 sonetSberRegData;
	UINT4 msk;
	UINT4 sts3Off;
	UINT4 timStat = 0;
	UINT4 tiuStat = 0;
	UINT4 erdipStat = 0;
	UINT4 rdipStat = 0;
	UINT4 pdipStat = 0;
	UINT4 uneqpStat = 0;
	UINT4 plmpStat = 0;
	UINT4 plupStat = 0;
	UINT4 sfStat = 0;
	UINT4 sdStat = 0; 
	INT4 result; 
	
	/* argument checking */
	if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }	
    
    sts3Off = 4*sts12Num + sts3Num;
    
    /* TIM_V and TIU_V Ind addr 0x4940240 + 0x01000 *A (A=0:15) RTTP_Path */ 
    iAddr = 0x0; /* indirect TIM/TIU RTTP Status memory */
	result = sonetHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddr, sts3Off, 
	                                      sts1Num, &sonetRttpRegData);   
	if (result) return result;
	
	msk = 
	  HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_MSMTCH_UNSTBL_STAT_TIM_V_MSK;
	result = hyPhy20gVarFieldRead(sonetRttpRegData, msk, &timStat);
	if (result) return result;     
	
	msk = 
	  HYPHY20G_SONET_RTTP_PATH_RTTP48_IND_IBIT_TRC_MSMTCH_UNSTBL_STAT_TIU_V_MSK;                               
    result = hyPhy20gVarFieldRead(sonetRttpRegData, msk, &tiuStat);  
    if (result) return result; 
        
    /* PERDI_V, PRDI_V, PPDI_V, PUNEQ_V, PPLM_V, and PPLU_V
       0x4940200 + 0x01000*A (A=0:15) RHAP Indirect Address */
    iAddr = 0x0; /* indirect RHAP Error Monitor Status */ 
    result = sonetHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                      &sonetRhapRegData);
    if (result) return result; 
    
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PERDI_V_MSK; 
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &erdipStat);
    if (result) return result; 
        
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PRDI_V_MSK;
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &rdipStat);
    if (result) return result;
        
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PPDI_V_MSK;
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &pdipStat); 
    if (result) return result;
        
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PUNEQ_V_MSK;
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &uneqpStat);  
    if (result) return result;
        
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PPLM_V_MSK; 
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &plmpStat);
    if (result) return result; 
    
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PPLU_V_MSK;
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &plupStat);
    if (result) return result; 
      
    /* SF_V and SD_V Ind addr 0x4940340 + 0x01000*A(A=0:15)SBER_Path */ 
    iAddr = 0x0; /* indirect SD/SF BERM Status */
    result = sonetHyPhy20gSBERPathIndRead(fileHndl, slice, iAddr, sts3Off, 
                                          sts1Num, &sonetSberRegData); 
    if (result) return result; 
    
    msk = HYPHY20G_SONET_SBER_PATH_SBER48_IND_IBIT_SDSF_BERM_STAT_SF_V_MSK;
    result = hyPhy20gVarFieldRead(sonetSberRegData, msk, &sfStat);
    if (result) return result; 
        
    msk = HYPHY20G_SONET_SBER_PATH_SBER48_IND_IBIT_SDSF_BERM_STAT_SD_V_MSK ;
    result = hyPhy20gVarFieldRead(sonetSberRegData, msk, &sdStat);
    if (result) return result; 

    *statusBits = (timStat << 9) + (tiuStat << 8) + (erdipStat << 7) +
                  (rdipStat << 6) + (pdipStat << 5) + (uneqpStat << 4) + 
                  (plmpStat << 3) + (plupStat << 2) + (sfStat << 1) + sdStat; 
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohGetStatus */

/*******************************************************************************
**
**  hyPhy20gSonetPohGetAisStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of AIS-P defect 
**                  within the SONET subsystem.
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 15 
**                sts12Num     - 0 to 3 
**                sts3Num      - 0 to 3 
**                sts1Num      - 0 to 2
**                dir          - 0 - Rx
**                               1 - Tx
**               *aisStatus    - Pointer to storage for path level AIS status 
**                               generated as a result of alarm signals
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohGetAisStatus(struct file * fileHndl, UINT4 slice, 
                                  UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num,
                                  UINT4 dir, UINT4 *aisStatus)
{
    /* Variable declaration */
    UINT4 iAddr;
    UINT4 sonetRsvcaRegData;
    UINT4 sonetTsvcaRegData;
    UINT4 sts3Off;
    UINT4 msk;
    UINT4 aisStat = 0;
	INT4 result; 
	
	/* Argument checking */
	if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) ||
	    (dir > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }	
    
    sts3Off = 4*sts12Num + sts3Num;
    
    if (dir == 0) {
        /* Read AIS_V in SONET:RSVCA 0x4940280 + 0x01000*A(A=0:15) 
           indirect register 0x0 Status Bits */
        iAddr = 0x0;
        result = sonetHyPhy20gRSVCAIndRead(fileHndl, slice, iAddr, sts3Off, 
                                           sts1Num, &sonetRsvcaRegData);
        if (result) return result; 
    
        msk = HYPHY20G_SONET_RSVCA_RSVCA_IND_IBIT_STAT_BITS_AIS_V_MSK;
        result = hyPhy20gVarFieldRead(sonetRsvcaRegData, msk, &aisStat);
        if (result) return result;
     } else {
        /* Read AIS_V in SONET:TSVCA 0x4940680 + 0x01000*A(A=0:15)
           indirect register 0x0 Status Bits */
        iAddr = 0x0;
        result = sonetHyPhy20gTSVCAIndRead(fileHndl, slice, iAddr, sts3Off,
                                           sts1Num, &sonetTsvcaRegData);
        if (result) return result;
            
        msk = HYPHY20G_SONET_TSVCA_TSVCA_IND_IBIT_STAT_BITS_AIS_V_MSK;
        result = hyPhy20gVarFieldRead(sonetTsvcaRegData, msk, &aisStat);
        if (result) return result;
    }
     
    *aisStatus = aisStat; 
        
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohGetAisPStatus */

/*******************************************************************************
**
**  hyPhy20gSonetPohGetG1Status
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of G1 byte of High-Order 
**                  Path alarms within the SONET subsystem.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15 
**                  sts12Num     - 0 to 3 
**                  sts3Num      - 0 to 3 
**                  sts1Num      - 0 to 2
**                  *statusBits  - Pointer to storage for status bits
**                                  Bit 1 - PERDI_V 
**                                  Bit 0 - PRDI_V 
**                  *filtErdiVal - Pointer to storage for filtered ERDI values
**                                  Bit 2 - bit #5 of G1 byte
**                                  Bit 1 - bit #6 of G1 byte
**                                  Bit 0 - bit #7 of G1 byte 
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohGetG1Status(struct file *fileHndl, UINT4 slice, 
                                 UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num, 
                                 UINT4 *statusBits, UINT4 *filtErdiVal)
{
    /* variable declaration */ 
	UINT2 iAddr; 		
	UINT4 sonetRhapRegData;
	UINT4 msk;
	UINT4 sts3Off;
	UINT4 erdipStat = 0;
	UINT4 rdipStat = 0;
	UINT4 fltErdiBitVal = 0;
	INT4 result; 
	
    /* argument checking */
	if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }	
    
    sts3Off = 4*sts12Num + sts3Num;
    
    /* PERDI_V and PRDI_V
       0x4940200 + 0x01000*A (A=0:15) RHAP Indirect Address */
    iAddr = 0x0; /* indirect RHAP Error Monitor Status */ 
    result = sonetHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                      &sonetRhapRegData);
    if (result) return result; 
    
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PERDI_V_MSK; 
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &erdipStat);
    if (result) return result; 
        
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PRDI_V_MSK;
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &rdipStat);
    if (result) return result;
        
    /* Filtered ERDI Value (bits 5-7 of G1)
       0x4940200 + 0x01000*A (A=0:15) RHAP Indirect address */
    iAddr = 0x5; /* indirect RHAP Filtered PERDI Value */ 
    result = sonetHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                      &sonetRhapRegData);
    if (result) return result; 
    
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_FLT_PERDI_VAL_PERDIV_MSK;
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &fltErdiBitVal);
    if (result) return result;
 
    *statusBits = (erdipStat << 1) + rdipStat;     
    *filtErdiVal = fltErdiBitVal;
                 
return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohGetG1Status */

/*******************************************************************************
**
**  hyPhy20gSonetPohGetC2Status
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of C2 byte of High-Order 
**                  Path alarms within the SONET subsystem.
**
**  INPUTS:         *fileHndl    - base address
**                  slice        - 0 to 15 
**                  sts12Num     - 0 to 3 
**                  sts3Num      - 0 to 3 
**                  sts1Num      - 0 to 2
**                  *statusBits  - Pointer to storage for status bits
**                                  Bit 1 - PPLM_V 
**                                  Bit 0 - PPLU_V
**                  *cPslVal     - captured PSL value in CPSLV
**                  *aPslVal     - accepted PSL value in APSLV 
**                  *ePslVal     - user programmed expected PSL value in EPSL
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
INT4 hyPhy20gSonetPohGetC2Status(struct file *fileHndl, UINT4 slice, 
                                 UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num, 
                                 UINT4 *statusBits, UINT4 *cPslVal, 
                                 UINT4 *aPslVal, UINT4 *ePslVal)
{
    /* variable declaration */ 
	UINT2 iAddr; 		
	UINT4 sonetRhapRegData;
	UINT4 msk;
	UINT4 sts3Off;
	UINT4 plmpStat = 0;
	UINT4 plupStat = 0;
	UINT4 cpslv = 0;
	UINT4 apslv = 0;
	UINT4 epsl = 0;
	INT4 result; 
	
	/* argument checking */
	if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }	
    
    sts3Off = 4*sts12Num + sts3Num;
        
    /* PPLM_V and PPLU_V
       0x4940200 + 0x01000*A (A=0:15) RHAP Indirect Address */
    iAddr = 0x0; /* indirect RHAP Error Monitor Status */ 
    result = sonetHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                      &sonetRhapRegData);
    if (result) return result; 
        
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PPLM_V_MSK; 
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &plmpStat);
    if (result) return result; 
    
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PPLU_V_MSK;
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &plupStat);
    if (result) return result; 
    
    /* CPSLV and APSLV
       0x4940200 + 0x01000*A (A=0:15) RHAP Indirect Address */ 
    iAddr = 0x6; /* indirect RHAP Captured and Accepted PSL */
    result = sonetHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                      &sonetRhapRegData);
    if (result) return result;
    
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_CAPT_AND_ACC_PSL_CPSLV_MSK;
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &cpslv);
    if (result) return result;
        
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_CAPT_AND_ACC_PSL_APSLV_MSK;
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &apslv);
    if (result) return result;
    
    /* EPSL 0x4940200 + 0x01000*A (A=0:15) RHAP Indirect Address */ 
    iAddr = 0x4; /* indirect RHAP Expected PSL and PDI */ 
    result = sonetHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                      &sonetRhapRegData);
    if (result) return result; 
    
    msk = HYPHY20G_SONET_RHAP_RHAP48_IND_IBIT_EPSL_PDI_EPSL_MSK;
    result = hyPhy20gVarFieldRead(sonetRhapRegData, msk, &epsl);
    if (result) return result;
     
    *statusBits = (plmpStat << 1) + plupStat;  
    *cPslVal = cpslv; 
    *aPslVal = apslv;
    *ePslVal = epsl;
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohGetC2Status */

/*******************************************************************************
**
**  hyPhy20gSonetPohGetPtrStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of High-Order Path pointer
**                  status within the SONET subsystem.
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 15 
**                sts12Num     - 0 to 3 
**                sts3Num      - 0 to 3 
**                sts1Num      - 0 to 2
**                dir          - direction from which to extract pointer status
**                                0 - Rx Direction
**                                1 - Tx Direction
**                *statusBits  - Pointer to storage for status bits
**                                  Bit 1 - PAISPTR_V / TPAISPTR_V 
**                                  Bit 0 - PLOPTR_V / TPLOPTR_V
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohGetPtrStatus(struct file *fileHndl, UINT4 slice, 
                                  UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num, 
                                  UINT4 dir, UINT4 *statusBits)
{
     /* variable declaration */ 
    UINT2 iAddr; 
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 sonetSarcRegData; 
    UINT4 aisptrpRxStat = 0;
    UINT4 lopptrpRxStat = 0;
    UINT4 aisptrpTxStat = 0;
    UINT4 lopptrpTxStat = 0;
    INT4 result; 
    
     /* argument checking */
	if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2) || 
	    (dir > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }	
    
    sts3Off = 4*sts12Num + sts3Num;
    
    /* PAISPTR_V and PLOPPTR_V
       0x4940400 + 0x01000*A(A=0:15) SARC Indirect Address */
    iAddr = 0x0; /* indirect SARC Receive Path LOP and AIS Status */ 
    result = sonetHyPhy20gSARCIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                      &sonetSarcRegData);
    if (result) return result; 
        
    msk = 
     HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_LOP_AIS_STAT_PAISPTR_V_MSK; 
    result = hyPhy20gVarFieldRead(sonetSarcRegData, msk, &aisptrpRxStat);
    if (result) return result; 
    
    msk = 
      HYPHY20G_SONET_SARC_SARC48_IND_IBIT_RX_PATH_LOP_AIS_STAT_PLOPTR_V_MSK;
    result = hyPhy20gVarFieldRead(sonetSarcRegData, msk, &lopptrpRxStat);
    if (result) return result; 
        
    /* TPAISPTR_V and TPLOPPTR_V
       0x4940400 + 0x01000*A(A=0:15) SARC Indirect Address */
    iAddr = 0x3; /* indirect SARC Transmit Path LOP and AIS Status */ 
    result = sonetHyPhy20gSARCIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                      &sonetSarcRegData);
    if (result) return result; 
        
    msk = 
    HYPHY20G_SONET_SARC_SARC48_IND_IBIT_TX_PATH_LOP_AIS_STAT_TPAISPTR_V_MSK; 
    result = hyPhy20gVarFieldRead(sonetSarcRegData, msk, &aisptrpTxStat);
    if (result) return result; 
    
    msk = 
     HYPHY20G_SONET_SARC_SARC48_IND_IBIT_TX_PATH_LOP_AIS_STAT_TPLOPTR_V_MSK; 
    result = hyPhy20gVarFieldRead(sonetSarcRegData, msk, &lopptrpTxStat);
    if (result) return result; 
    
    /* Rx Direction */ 
    if (dir == 0) {
        *statusBits = (aisptrpRxStat << 1) + lopptrpRxStat;
    }
    /* Tx Direction */ 
    else if (dir == 1) {
        *statusBits = (aisptrpTxStat << 1) + lopptrpTxStat;
    }
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohGetPtrStatus */

/*******************************************************************************
**
**  hyPhy20gSonetPohTxPtrIntprtGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function reports the status of the Path Pointer 
**                 interpreter in the Sonet Subsystem Tx Direction. 
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 15 
**                sts12Num     - 0 to 3 
**                sts3Num      - 0 to 3 
**                sts1Num      - 0 to 2
**                *statusBits  - Pointer to storage for status bits
**                                  Bit 4 - PCOP_V
**                                  Bit 3 - PAISC_V 
**                                  Bit 2 - PLOPC_V
**                                  Bit 1 - PAIS_V 
**                                  Bit 0 - PLOP_V
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohTxPtrIntprtGetStatus(struct file *fileHndl, UINT4 slice, 
                                          UINT4 sts12Num, UINT4 sts3Num, 
                                          UINT4 sts1Num, UINT4 *statusBits)
{
    /* variable declaration */ 
    UINT2 iAddr; 
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 sonetTshpiRegData; 
    UINT4 coppStat = 0;
    UINT4 aiscpStat = 0;
    UINT4 lopcpStat = 0;
    UINT4 aispStat = 0;
    UINT4 loppStat = 0; 
    INT4 result; 
    
     /* argument checking */
	if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }	
    
    sts3Off = 4*sts12Num + sts3Num;
    
    /* PCOP_V, PAISC_V, PLOPC_V, PAIS_V, and PLOP_V 
     0x4940880 + 0x01000*A (A=0:15) TSHPI Indirect address */
    iAddr = 0x0; /* TSHPI Indirect Interrupt Values  */
    result = sonetHyPhy20gTSHPIIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num,
                                       &sonetTshpiRegData);
    if (result) return result;
        
    msk = HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_INT_VAL_PCOP_V_MSK;
    result = hyPhy20gVarFieldRead (sonetTshpiRegData, msk, &coppStat);
    if (result) return result;
        
    msk =HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_INT_VAL_PAISC_V_MSK;
    result = hyPhy20gVarFieldRead (sonetTshpiRegData, msk, &aiscpStat);
    if (result) return result; 
        
    msk =HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_INT_VAL_PLOPC_V_MSK;
    result = hyPhy20gVarFieldRead (sonetTshpiRegData, msk, &lopcpStat);
    if (result) return result; 
    
    msk = HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_INT_VAL_PAIS_V_MSK;
    result = hyPhy20gVarFieldRead (sonetTshpiRegData, msk, &aispStat);
    if (result) return result; 
    
    msk = HYPHY20G_SONET_TSHPI_TSHPI_IND_IBIT_SHPI_INT_VAL_PLOP_V_MSK;
    result = hyPhy20gVarFieldRead (sonetTshpiRegData, msk, &loppStat);
    if (result) return result; 
        
    *statusBits = (coppStat << 4) + (aiscpStat << 3) + (lopcpStat << 2) +
                  (aispStat << 1) + loppStat;
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohTxPtrIntprtGetStatus */

/*******************************************************************************
**
**  hyPhy20gSonetPohRxPtrIntprtGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function reports the status of the Path Pointer 
**                 interpreter in the Sonet Subsystem Rx Direction.
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 15 
**                sts12Num     - 0 to 3 
**                sts3Num      - 0 to 3
**                sts1Num      - 0 to 2
**                *statusBits  - Pointer to storage for status bits
**                                  Bit 4 - PCOP_V
**                                  Bit 3 - PAISC_V 
**                                  Bit 2 - PLOPC_V
**                                  Bit 1 - PAIS_V 
**                                  Bit 0 - PLOP_V
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetPohRxPtrIntprtGetStatus(struct file *fileHndl, UINT4 slice,
                                          UINT4 sts12Num, UINT4 sts3Num, 
                                          UINT4 sts1Num, UINT4 *statusBits)
{
    /* variable declaration */
    UINT2 iAddr; 
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 sonetRshpiRegData; 
    UINT4 coppStat = 0;
    UINT4 aiscpStat = 0;
    UINT4 lopcpStat = 0;
    UINT4 aispStat = 0;
    UINT4 loppStat = 0; 
    INT4 result; 
       
    /* argument checking */
	if ((slice > 15) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }	
    
    sts3Off = 4*sts12Num + sts3Num;
    
    /* PCOP_V, PAISC_V, PLOPC_V, PAIS_V, and PLOP_V 
     0x49401C0 + 0x01000*A(A=0:15) RSHPI Indirect address */
    iAddr = 0x0; /* RSHPI indirect interrupt values */
    
    result = sonetHyPhy20gRSHPIIndRead(fileHndl, slice, iAddr, sts3Off, 
                                       sts1Num, &sonetRshpiRegData); 
        
    msk = HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_INT_VAL_PCOP_V_MSK;
    result = hyPhy20gVarFieldRead(sonetRshpiRegData, msk, &coppStat);
    if (result) return result; 
    
    msk =HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_INT_VAL_PAISC_V_MSK;
    result = hyPhy20gVarFieldRead(sonetRshpiRegData, msk, &aiscpStat);
    if (result) return result;
        
    msk =HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_INT_VAL_PLOPC_V_MSK;
    result = hyPhy20gVarFieldRead(sonetRshpiRegData, msk, &lopcpStat);
    if (result) return result;
        
    msk = HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_INT_VAL_PAIS_V_MSK;
    result = hyPhy20gVarFieldRead(sonetRshpiRegData, msk, &aispStat);
    if (result) return result; 
    
    msk = HYPHY20G_SONET_RSHPI_RSHPI_IND_IBIT_SHPI_INT_VAL_PLOP_V_MSK;
    result = hyPhy20gVarFieldRead(sonetRshpiRegData, msk, &loppStat);
    if (result) return result; 
        
    *statusBits = (coppStat << 4) + (aiscpStat << 3) + (lopcpStat << 2) +
                  (aispStat << 1) + loppStat;  
                                         
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetPohRxPtrIntprtGetStatus */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/* end of file */
