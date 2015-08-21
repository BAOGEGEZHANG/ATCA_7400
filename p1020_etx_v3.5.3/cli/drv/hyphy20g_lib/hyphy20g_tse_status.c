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
**  FILE        : hyphy20g_tse_status.c
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
#include "hyphy20g_tse_status.h"

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
**  hyPhy20gTsePtrIntprtGetStatus
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:   This function reports the status of the Path Pointer 
**                 interpreter in the TSE Subsystem for the port being monitored
**                 by the PRGM.
**
**  INPUTS:       *fileHndl     - base address
**                sts12Num      - 0 to 3
**                sts3Num       - 0 to 3
**                sts1Num       - 0 to 2
**                *statusBits   - Pointer to storage for status bits
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
INT4 hyPhy20gTsePtrIntprtGetStatus(struct file *fileHndl, UINT4 sts12Num,
                                   UINT4 sts3Num, UINT4 sts1Num, 
                                   UINT4 *statusBits)
{
    /* Variable declaration */
    UINT4 iAddr;
    UINT4 tseRegData;
    UINT4 msk;
    UINT4 sts3Off;
    UINT4 aiscPStat, lopcPStat;
    UINT4 aisPStat, lopPStat;
    INT4 result;
    
    /* Argument checking */
    if ((sts12Num > 3) || (sts3Num > 3) || (sts1Num > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;   
    }
    
    sts3Off = 4*sts12Num + sts3Num;
    
    /* Read from TSE:SHPI 0x4932600 indirect addr:0x0 
       Indirect Interrupt Values */
    iAddr = 0x0;
    result = tseHyPhy20gSHPIIndRead(fileHndl, iAddr, sts3Off, sts1Num, 
                                    &tseRegData);
    if (result) return result;
        
    msk = HYPHY20G_TSE_SHPI_SHPI_IND_IBIT_SHPI_INT_VAL_PAISCV_MSK;
    result = hyPhy20gVarFieldRead(tseRegData, msk, &aiscPStat);
    if (result) return result;
        
    msk = HYPHY20G_TSE_SHPI_SHPI_IND_IBIT_SHPI_INT_VAL_PLOPCV_MSK;
    result = hyPhy20gVarFieldRead(tseRegData, msk, &lopcPStat);
    if (result) return result;
        
    msk = HYPHY20G_TSE_SHPI_SHPI_IND_IBIT_SHPI_INT_VAL_PAISV_MSK;
    result = hyPhy20gVarFieldRead(tseRegData, msk, &aisPStat);
    if (result) return result;
    
    msk = HYPHY20G_TSE_SHPI_SHPI_IND_IBIT_SHPI_INT_VAL_PLOPV_MSK;
    result = hyPhy20gVarFieldRead(tseRegData, msk, &lopPStat);
    if (result) return result;
    
   *statusBits = (aiscPStat << 3) + (aiscPStat << 2) + (aisPStat << 1) 
                 + lopPStat;
                 
    return HYPHY20G_SUCCESS;
} /* hyPhy20gTsePtrIntprtGetStatus */   

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/* end of file */
