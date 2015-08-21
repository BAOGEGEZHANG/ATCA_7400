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
**  FILE        : hyphy20g_hopp_status.c
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
#include "hyphy20g_hopp_status.h"

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
**  hyPhy20gHoppPohGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of High-Order Path alarms
**                  within the Hopp subsystem.
**  
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 3
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
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppPohGetStatus(struct file *fileHndl, UINT4 slice, 
                              UINT4 sts12Num, UINT4 sts3Num, 
                              UINT4 sts1Num, UINT4 *statusBits)
{
    /* Variable declaration */ 
    UINT4 iAddr;
    UINT4 msk;
    UINT4 hoppRttpRegData, hoppRhapRegData, hoppSberRegData;
    UINT4 sts3Off;
    UINT4 timStat, tiuStat;
    UINT4 erdiPStat, rdiPStat;
    UINT4 pdiPStat, uneqPStat;
    UINT4 plmPStat, pluPStat;
    UINT4 sfStat, sdStat;
    INT4 result;
    
    /* Argument checking */
    if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2))
        return  HYPHY20G_ERR_INVALID_ARG;
    
    sts3Off = 4*sts12Num + sts3Num;
        
    /* Read from HOPP:RTTP at 0x4954240 + 0x01000*A(A=0:3)
       TIM_V and TIU_V indirect register 0x0 Trace Identifier Mismatch */ 
    iAddr = 0x0;
    result = hoppHyPhy20gRTTPPathIndRead(fileHndl, slice, iAddr, sts3Off, 
                                         sts1Num, &hoppRttpRegData);
    if (result) return result;
    
    msk = 
       HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_MSMTCH_UNSTBL_STAT_TIM_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRttpRegData, msk, &timStat);
    if (result) return result;
        
    msk = 
       HYPHY20G_HOPP_RTTP_PATH_RTTP48_IND_IBIT_TRC_MSMTCH_UNSTBL_STAT_TIU_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRttpRegData, msk, &tiuStat);
    if (result) return result;   
        
    /* PERDI_V, PRDI_V, PPDI_V, PUNEQ_V, PPLM_V, and PPLU_V
       HOPP:RHAP 0x4954200 + 0x01000*A(A=0:3) */ 
    iAddr = 0x0; /* indirect register Error Monitor Status */
    result = hoppHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                     &hoppRhapRegData);
    if (result) return result;
        
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PERDI_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &erdiPStat);
    if (result) return result;
        
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PRDI_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &rdiPStat);
    if (result) return result;
    
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PPDI_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &pdiPStat);
    if (result) return result;
        
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PUNEQ_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &uneqPStat);
    if (result) return result;
     
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PPLM_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &plmPStat);
    if (result) return result;    
    
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PPLU_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &pluPStat);
    if (result) return result;
        
    /* SF_V and SD_V Ind addr 0x4954340 + 0x01000*A(A=0:3) SBER_Path */  
    iAddr = 0x0; /* SD/SF BERM Status */ 
    result = hoppHyPhy20gSBERPathIndRead(fileHndl, slice, iAddr, sts3Off, 
                                         sts1Num, &hoppSberRegData);
    if (result) return result;
        
    msk = HYPHY20G_HOPP_SBER_PATH_SBER48_IND_IBIT_SDSF_BERM_STAT_SF_V_MSK;
    result = hyPhy20gVarFieldRead(hoppSberRegData, msk, &sfStat);
    if (result) return result; 
        
    msk = HYPHY20G_HOPP_SBER_PATH_SBER48_IND_IBIT_SDSF_BERM_STAT_SD_V_MSK;
    result = hyPhy20gVarFieldRead(hoppSberRegData, msk, &sdStat);
    if (result) return result;
        
    *statusBits = (timStat << 9) + (tiuStat << 8) + (erdiPStat << 7) +
                  (rdiPStat << 6) + (pdiPStat << 5) + (uneqPStat << 4) + 
                  (plmPStat << 3) + (pluPStat << 2) + (sfStat << 1) + sdStat; 
               
    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohGetStatus */

/*******************************************************************************
**
**  hyPhy20gHoppPohGetG1Status
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of G1 byte of High-Order 
**                  Path alarms within the SONET subsystem.
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 3 
**                sts12Num     - 0 to 3 
**                sts3Num      - 0 to 3 
**                sts1Num      - 0 to 2
**                *statusBits  - Pointer to storage for status bits
**                                  Bit 1 - PERDI_V 
**                                  Bit 0 - PRDI_V 
**                *filtErdiVal - Pointer to storage for filtered ERDI values
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
INT4 hyPhy20gHoppPohGetG1Status(struct file *fileHndl, UINT4 slice, 
                                 UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num, 
                                 UINT4 *statusBits, UINT4 *filtErdiVal)
{
    /* variable declaration */ 
	UINT2 iAddr; 		
	UINT4 hoppRhapRegData;
	UINT4 msk;
	UINT4 sts3Off;
	UINT4 erdiPStat = 0;
	UINT4 rdiPStat = 0;
	UINT4 fltErdiBitVal = 0;
	INT4 result; 
	
    /* argument checking */
	if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }	
    
    sts3Off = 4*sts12Num + sts3Num;
    
    /* PERDI_V and PRDI_V
       0x4954200 + 0x01000*A(A=0:3) RHAP Indirect Address */
    iAddr = 0x0; /* indirect RHAP Error Monitor Status */ 
    result = hoppHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                     &hoppRhapRegData);
    if (result) return result; 
    
     msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PERDI_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &erdiPStat);
    if (result) return result;
        
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PRDI_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &rdiPStat);
    if (result) return result;
        
    /* Filtered ERDI Value (bits 5-7 of G1)
       0x4954200 + 0x01000*A(A=0:3) RHAP Indirect address */
    iAddr = 0x5; /* indirect RHAP Filtered PERDI Value */ 
    result = hoppHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                     &hoppRhapRegData);
    if (result) return result; 
    
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_FLT_PERDI_VAL_PERDIV_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &fltErdiBitVal);
    if (result) return result;
 
    *statusBits = (erdiPStat << 1) + rdiPStat;     
    *filtErdiVal = fltErdiBitVal;
                 
return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohGetG1Status */

/*******************************************************************************
**
**  hyPhy20gHoppPohGetC2Status
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of C2 byte of High-Order 
**                  Path alarms within the SONET subsystem.
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 3 
**                sts12Num     - 0 to 3 
**                sts3Num      - 0 to 3 
**                sts1Num      - 0 to 2
**                *statusBits  - Pointer to storage for status bits
**                                  Bit 1 - PPLM_V 
**                                  Bit 0 - PPLU_V
**                *cPslVal     - captured PSL value in CPSLV
**                *aPslVal     - accepted PSL value in APSLV 
**                *ePslVal     - user programmed expected PSL value in EPSL
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
INT4 hyPhy20gHoppPohGetC2Status(struct file *fileHndl, UINT4 slice, 
                                 UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num, 
                                 UINT4 *statusBits, UINT4 *cPslVal, 
                                 UINT4 *aPslVal, UINT4 *ePslVal)
{
    /* variable declaration */ 
	UINT2 iAddr; 		
	UINT4 hoppRhapRegData;
	UINT4 msk;
	UINT4 sts3Off;
	UINT4 plmpStat = 0;
	UINT4 plupStat = 0;
	UINT4 cpslv = 0;
	UINT4 apslv = 0;
	UINT4 epsl = 0;
	INT4 result; 
	
	/* argument checking */
	if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }	
    
    sts3Off = 4*sts12Num + sts3Num;
        
    /* PPLM_V and PPLU_V at HOPP:RHAP 0x4954200 + 0x01000*A(A=0:3) */
    iAddr = 0x0; /* indirect RHAP Error Monitor Status */ 
    result = hoppHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                      &hoppRhapRegData);
    if (result) return result; 
        
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PPLM_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &plmpStat);
    if (result) return result;
        
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_ERR_MON_STAT_PPLU_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &plupStat);
    if (result) return result;
    
    /* CPSLV and APSLV at HOPP:RHAP 0x4954200 + 0x01000*A(A=0:3) */ 
    iAddr = 0x6; /* indirect RHAP Captured and Accepted PSL */
    result = hoppHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                      &hoppRhapRegData);
    if (result) return result;
    
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_CAPT_AND_ACC_PSL_CPSLV_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &cpslv);
    if (result) return result;
        
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_CAPT_AND_ACC_PSL_APSLV_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &apslv);
    if (result) return result;
    
    /* EPSL at HOPP:RHAP 0x4954200 + 0x01000*A(A=0:3) Indirect address */ 
    iAddr = 0x4; /* indirect RHAP Expected PSL and PDI */ 
    result = hoppHyPhy20gRHAPIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                      &hoppRhapRegData);
    if (result) return result; 
    
    msk = HYPHY20G_HOPP_RHAP_RHAP48_IND_IBIT_EPSL_PDI_EPSL_MSK;
    result = hyPhy20gVarFieldRead(hoppRhapRegData, msk, &epsl);
    if (result) return result;
     
    *statusBits = (plmpStat << 1) + plupStat;  
    *cPslVal = cpslv; 
    *aPslVal = apslv;
    *ePslVal = epsl;
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohGetC2Status */

/*******************************************************************************
**
**  hyPhy20gHoppPohGetPtrStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function checks the status of High-Order Path pointer
**                  status within the HOPP subsystem.
**  
**
**  INPUTS:       *fileHndl    - base address
**                slice        - 0 to 3 
**                sts12Num     - 0 to 3
**                sts3Num      - 0 to 3
**                sts1Num      - 0 to 2
**                *statusBits  - Pointer to storage for status bits
**                                  Bit 1 - PAISPTR_V  
**                                  Bit 0 - PLOPTR_V 
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHoppPohGetPtrStatus(struct file *fileHndl, UINT4 slice, 
                                 UINT4 sts12Num, UINT4 sts3Num, UINT4 sts1Num, 
                                 UINT4 *statusBits)
{
     /* variable declaration */ 
    UINT2 iAddr; 
    UINT4 msk;
    UINT4 hoppSarcRegData; 
    UINT4 aisptrpStat = 0;
    UINT4 lopptrpStat = 0;
    INT4 result; 
    
     /* argument checking */
	if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }	
    
    /* PAISPTR_V and PLOPPTR_V
       0x4954400 + 0x01000*A(A=0:3) SARC Indirect Address */
    iAddr = 0x0; /* indirect SARC Receive Path LOP and AIS Status */ 
    result = hoppHyPhy20gSARCIndRead(fileHndl, slice, iAddr, sts3Num, 
                                     sts1Num, &hoppSarcRegData);
    if (result) return result; 
        
    msk = 
      HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_LOP_AIS_STAT_PAISPTR_V_MSK; 
    result = hyPhy20gVarFieldRead(hoppSarcRegData, msk, &aisptrpStat);
    if (result) return result; 
    
    msk = 
      HYPHY20G_HOPP_SARC_SARC48_IND_IBIT_RX_PATH_LOP_AIS_STAT_PLOPTR_V_MSK;
    result = hyPhy20gVarFieldRead(hoppSarcRegData, msk, &lopptrpStat);
    if (result) return result; 
  
    *statusBits = (aisptrpStat << 1) + lopptrpStat;
  
    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohGetPtrStatus */

/*******************************************************************************
**
**  hyPhy20gHoppPohRxPtrIntprtGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function reports the status of the Path Pointer 
**                 interpreter in the Hopp Subsystem Rx Direction. 
**
**  INPUTS:       *fileHndl   - base address
**                slice       - 0 to 3 
**                sts12Num    - 0 to 3
**                sts3Num     - 0 to 3
**                sts1Num     - 0 to 2
**                *statusBits  - Pointer to storage for status bits
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
INT4 hyPhy20gHoppPohRxPtrIntprtGetStatus(struct file *fileHndl, UINT4 slice, 
                                         UINT4 sts12Num, UINT4 sts3Num, 
                                         UINT4 sts1Num, UINT4 *statusBits)
{
    /* variable declaration */
    UINT4 iAddr; 
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 hoppRshpiRegData; 
    UINT4 aiscpStat = 0;
    UINT4 lopcpStat = 0;
    UINT4 aispStat = 0;
    UINT4 loppStat = 0; 
    INT4 result; 
    
    /* Argument checking */
    if ((slice > 3) || (sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }	
    
    sts3Off = 4*sts12Num + sts3Num;
    
    /* Read from HOPP:RSHPI 0x49541C0+0x01000*A(A=0:3) indirect register 0x0 */
    iAddr = 0x0; /* Indirect Interrupt Values */
    result = hoppHyPhy20gRSHPIIndRead(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                      &hoppRshpiRegData); 
    if (result) return result;
    
    msk = HYPHY20G_HOPP_RSHPI_RSHPI_IND_IBIT_SHPI_INT_VAL_PAISC_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRshpiRegData, msk,&aiscpStat);
    if (result) return result;
        
    msk = HYPHY20G_HOPP_RSHPI_RSHPI_IND_IBIT_SHPI_INT_VAL_PLOPC_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRshpiRegData, msk, &lopcpStat);
    if (result) return result;
        
    msk = HYPHY20G_HOPP_RSHPI_RSHPI_IND_IBIT_SHPI_INT_VAL_PAIS_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRshpiRegData, msk, &aispStat);
    if (result) return result;
        
    msk = HYPHY20G_HOPP_RSHPI_RSHPI_IND_IBIT_SHPI_INT_VAL_PLOP_V_MSK;
    result = hyPhy20gVarFieldRead(hoppRshpiRegData, msk, &loppStat);
    if (result) return result;
    DBG_PRINT("loppStat = 0x%lx\n", loppStat);
        
    /* Write to HOPP:RSHPI indirect address 0x0 Indirect Interrupt Values */
    result = hoppHyPhy20gRSHPIIndWrite(fileHndl, slice, iAddr, sts3Off, sts1Num, 
                                       hoppRshpiRegData);
    if (result) return result;
    
    *statusBits = (aiscpStat << 3) + (lopcpStat << 2) + (aispStat << 1) 
                  + loppStat;  
     
    return HYPHY20G_SUCCESS;
} /* hyPhy20gHoppPohRxPtrIntprtGetStatus */   

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/* end of file */
