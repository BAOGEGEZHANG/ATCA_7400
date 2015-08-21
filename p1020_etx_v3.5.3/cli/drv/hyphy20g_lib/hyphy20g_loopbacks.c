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
**  FILE        : hyphy20g_loopbacks.c
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
#include "hyphy20g_loopbacks.h"

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
**  hyPhy20gEssiLnLpbk
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function enables or disables Line Loopback for one of
**                  the ESSI links.  The loopback is enabled from RSDM output
**                  to TSSI input.
**
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - ESSI slice : 0 - 3
**                  link        - Essi link within the slice : 0 - 7
**                  lpbkEnbl    - 0 - ESSI Line Loopback disabled
**                                1 - ESSI Line Loopback enabled
**            
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiLnLpbk(struct file *fileHndl, UINT4 slice, 
                        UINT4 link, UINT4 lpbkEnbl)
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
    if ((slice > 3)||(link > 7)||(lpbkEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    regAddr = HYPHY20G_ESSI_ESSI_REG_LN_LPBK;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    
    /* Configure LINE_LOOPBACK bit */
    offset = link;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, lpbkEnbl);
    if (result) return result;
        
    /* Configure ID_2_SEL_EN bit */        
    regAddr = HYPHY20G_ESSI_TSSI_REG_CFG;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET (regAddr, slice);
    regAddr = HYPHY20G_ESSI_TSSI_TSB_SLICE_REG_OFFSET(regAddr, link);
    
    offset = HYPHY20G_ESSI_TSSI_BIT_CFG_ID_2_SEL_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, lpbkEnbl);
    if (result) return result; 
      
    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiLnLpbk */   


/*******************************************************************************
**
**  hyPhy20gGfpDiagLpbk
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function enables or disables Diagnostic Loopback within
**                  the GFP subsystem.  The loopback is enabled from TXDP64
**                  output to RXDP64 input.  This loopback is only available for
**                  GFP-mapped traffic.  Transparently mapped traffic cannot
**                  be looped back within the GFP subsystem.
**
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 - GFP Subsystem Slice 'A'
**                              - 1 - GFP Subsystem Slice 'B'
**                  lpbkEnbl    - 0 - GFP Diagnostic Loopback disabled
**                                1 - GFP Diagnostic Loopback enabled
**            
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpDiagLpbk(struct file *fileHndl, UINT4 slice, UINT4 lpbkEnbl)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;

   /* argument checking */
    if ((slice > 1)||(lpbkEnbl > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    regAddr = HYPHY20G_GFP_FRM_RXDP_TSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    
    /* Configure RXDP64_LB bit */
    offset = HYPHY20G_GFP_FRM_RXDP_BIT_CFG_RXDP64_LB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, lpbkEnbl);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpDiagLpbk */

