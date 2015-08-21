/******************************************************************************/
/**  COPYRIGHT (C) 2009 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
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
**  FILE        : hyphy20g_bp_serdes.c
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
#include "hyphy20g_bp_serdes.h"

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
**  hyPhy20gTxBpSerdesSrcSel
**  ___________________________________________________________________________ 
**
**  DESCRIPTION:    This function selects the data source for a group of 8
**                  backplane SERDES ADDI signals.
**
**                  This function further configures the data bus width
**                  correctly for intefacing between SERDES and ESSI or CBRI
**                  subsystems.  This applies to both Tx and Rx directions.
**
**
**  INPUTS:         *fileHndl  - base address
**                  group      - set of backplane SERDES links
**                               0 - ADDI[7:0]
**                               1 - ADDI[15:8]
**                               2 - ADDI[23:16]
**                               3 - ADDI[31:24]
**                  source     - transmit data source
**                               0 - ESSI subsystem
**                               1 - CBRI subsystem
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gTxBpSerdesSrcSel(struct file *fileHndl, UINT4 group, UINT4 source)
{ 
    /* variable declaration */
    UINT4 txMuxSelMsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_TOP_CTL_TX_MUX_SEL_MSK;
    UINT4 addr = HYPHY20G_SERDES_BP_TXRXSLICE_REG_TOP_CTL;
    UINT4 sliceAddr, slice, i;
    UINT4 busWidthAddr, busWidthData;
    UINT4 txWidthMsk = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_TXPATH_BUS_WIDTH_MSK; 
    UINT4 rxWidthMsk = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_RXPATH_BUS_WIDTH_MSK;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((group > 3)||(source > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine slice offset of 8 SERDES links to be configured */
    slice = group * 8;

    for(i=slice; i < (slice + 8); i++){
        sliceAddr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(addr, i);
        result = hyPhy20gRegFieldWrite(fileHndl, sliceAddr, txMuxSelMsk, 
                                       source);
        if(result) return result;

        /* set TXPATH_BUS_WIDTH and RXPATH_BUS_WIDTH to 0 for ESSI and to 1 for
           CBRI */
        busWidthAddr = HYPHY20G_SERDES_BP_C8_REG_CFG_8;
        busWidthAddr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(busWidthAddr, 
                                                                slice);

        result = sysHyPhy20gRead(fileHndl, busWidthAddr, &busWidthData);
        if(result) return result;
    
        result = hyPhy20gVarFieldWrite (&busWidthData,  txWidthMsk,  source);
        if(result) return result;
    
        result = hyPhy20gVarFieldWrite (&busWidthData,  rxWidthMsk,  source);
        if(result) return result;
    
    
        result = sysHyPhy20gWrite(fileHndl, busWidthAddr, busWidthData);
        if(result) return result;

    }
   
    return HYPHY20G_SUCCESS;

} /* hyPhy20gTxBpSerdesSrcSel */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
