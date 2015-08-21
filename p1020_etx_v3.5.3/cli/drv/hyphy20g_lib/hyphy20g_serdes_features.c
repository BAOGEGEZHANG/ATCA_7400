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
**  FILE        : hyphy20g_serdes_features.c
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_serdes_features.h"

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
**  hyPhy20gXfiSerdesDiagLpbk
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enable or disable XFI Serdes diagnostic
**                  loopback.  Data from Tx is looped back to Rx.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - XFI Serdes Slice instance
**                                0 - XFI0
**                                1 - XFI1
**                 enable       - Looback enable
**                                1 - Loopback enabled
**                                0 - Loopback disabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gXfiSerdesDiagLpbk(struct file *fileHndl, UINT4 slice, UINT4 enable)
{
    /* variable declaration */
    UINT4 regAddr;
    UINT4 lpbckMsk;
    UINT4 en0Msk, en1Msk;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Enable or disable PISO_SIPO loopback at 0x4968018 + 0x00200*A(A=0:1)
       XFI Configuration 7 */
    regAddr = HYPHY20G_SERDES_XFI_CORE_REG_CFG_7;
    regAddr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    lpbckMsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_PISO_SIPO_LOOPBACK_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, lpbckMsk, enable);
    if (result) return result;

    /* Determine register and mask values for LOS propagation */
    regAddr = HYPHY20G_SONET_LINE_TOP_REG_SQUELCH_EN;
    en0Msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_XFI_LOS_EN0_MSK;
    en1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_XFI_LOS_EN1_MSK;

    /* Configure LOS to propagate downstream on the specified XFI port */
    switch (slice) {
        case 0:
            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, en0Msk, 1-enable);
            if (result) return result;
            break;
        case 1:
            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, en1Msk, 1-enable);
            if (result) return result;
            break;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gXfiSerdesDiagLpbk */


/*******************************************************************************
**
**  hyPhy20gSfpSerdesDiagLpbk
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables SFP Serdes diagnostic
**                  loopback.  Data from Tx is looped back to Rx.
**
**                  When this API is called with block set to 1 and enable set        
**                  to 1 the LSCI slice LOS_INVERT bit will be set as a function   
**                  of the Serdes LOS_V state. The next time this API is called, 
**                  block must be set to 1 and enable set to 0.
**
**                  For a slice called with block set to 1 and enable set to 1,
**                  hyPhy20gSfpSerdesLsciDiagLpbkLosInvCfg may be called to 
**                  update the value of LOS_INVERT based on the
**                  current state of LOS_V.    
**                                            
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - SFP Serdes Slice instance
**                                0 to 15
**                 block        - Subsystems receiving data stream from loopback
**                                1 - Contains an LSCI slice
**                                0 - Does not contain an LSCI slice
**                 enable       - Loopback enable
**                                1 - Loopback enabled
**                                0 - Loopback disabled
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSfpSerdesDiagLpbk(struct file *fileHndl, UINT4 slice, UINT4 block,
                               UINT4 enable)
{
    /* variable declaration */
    UINT4 sfPRegAddr;
    UINT4 sonetLnTopRegAddr;
    UINT4 lpbckMsk;
    UINT4 addr, msk;
    UINT4 losEnVal;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
	

    /* argument checking */
    if ((slice > 15)||(block > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* determine register address and mask value for SFP loopback */
    sfPRegAddr = HYPHY20G_SERDES_SFP_CORE_REG_CONF_6;
    sfPRegAddr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(sfPRegAddr, slice);

    lpbckMsk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_PISO_SIPO_LOOPBACK_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, sfPRegAddr, lpbckMsk, enable);
    if (result) return result;

    /* determine register and mask values to disable LOS propagation if
       loopback is enabled or to enable LOS propogation if loopback
       is disabled for OTN or to invert the LOS state from SFP Serdes
       for LSCI */
    sonetLnTopRegAddr = HYPHY20G_SONET_LINE_TOP_REG_SQUELCH_EN;

    /* SFP to SONET or OTU1 */
    losEnVal = (enable==1) ? 0:1;
    if (block == 0) {
        switch (slice) {
            case 0:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN0_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 1:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN1_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 2:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN2_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 3:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN3_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 4:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN4_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 5:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN5_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 6:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN6_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 7:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN7_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 8:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN8_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 9:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN9_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 10:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN10_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 11:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN11_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 12:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN12_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 13:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN12_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 14:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN14_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            case 15:
                msk = HYPHY20G_SONET_LINE_TOP_BIT_SQUELCH_EN_SFP_LOS_EN15_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, sonetLnTopRegAddr,
                                               msk, losEnVal);
                if (result) return result;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;       
        }
    } else {
        if (enable == 1) {
            result = hyPhy20gSfpSerdesLsciDiagLpbkLosInvCfg(fileHndl, slice);
            if (result) return result;
        } else {
            /* Set LOS_INVERT to 0 */
            addr = HYPHY20G_LSCI_TOP_REG_CFG;
            addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
            msk = HYPHY20G_LSCI_TOP_BIT_CFG_LOS_INVERT_MSK;
        
            result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0);
            if (result) return result;
        }
    } 

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSfpSerdesDiagLpbk */

/*******************************************************************************
**
**  hyPhy20gSfpSerdesLsciDiagLpbkLosInvCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves LOS_V state from SFP Serdes and sets
**                  LOS_INVERT in LSCI to the appropriate value for slice that 
**                  is in SFP diagnostic loopback.
**                  
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - SFP Serdes Slice instance
**                                0 to 15
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSfpSerdesLsciDiagLpbkLosInvCfg(struct file *fileHndl, UINT4 slice)
{
    /* variable declaration */
    UINT4 addr, msk, val;
    UINT4 enblLpbk;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
	
    /* argument checking */
    if (slice > 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Check if SFP loopback is enabled */ 
    addr = HYPHY20G_SERDES_SFP_CORE_REG_CONF_6;
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_PISO_SIPO_LOOPBACK_MSK; 
    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &enblLpbk);
    if (result) return result;
   
    if (enblLpbk != 1) { /* Do nothing if loopback is disabled */
        return HYPHY20G_SUCCESS;
    } else { /* Loopback is enabled */
        /* read LOS_V bit*/
        addr = HYPHY20G_SERDES_SFP_TOP_REG_INT_VAL;
        addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(addr, slice);
        msk = HYPHY20G_SERDES_SFP_TOP_BIT_INT_VAL_LOS_V_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &val);
        if (result) return result;
        
        /* set LOS_INVERT */
        addr = HYPHY20G_LSCI_TOP_REG_CFG;
        addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
        msk = HYPHY20G_LSCI_TOP_BIT_CFG_LOS_INVERT_MSK;
        
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
        if (result) return result;
    }
    
    return HYPHY20G_SUCCESS;
}

/*******************************************************************************
**
**  hyPhy20gBackplaneSerdesDiagLpbk
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enable or disable Backplane Serdes diagnostic
**                  loopback.  Data from Tx Serdes is looped back to Rx Serdes.
**
**                  The function will modify the device for each Serdes link
**                  specified in sliceMsk.  The loopback state for each of these
**                  links will be specified with enblMsk
**
**
**  INPUTS:       *fileHndl     - base address
**                 sliceMsk     - Mask of Backplane Serdes Slices to
**                                modify loopback configuration
**                                Legal range:
**                                0x00 to 0xFFFFFFFF
**                                Bit[31] - ASSI_P/N[31] and DSSI_P/N[31]
**                                ...
**                                Bit[1] - ASSI_P/N[1] and DSSI_P/N[1]
**                                Bit[0] - ASSI_P/N[0] and DSSI_P/N[0]
**                                Setting for each bit:
**                                1 - modify the configuration for link
**                                0 - do not modify configuration for link
**                 enblMsk      - Mask of bits containing loopback state for
**                                the bits specified by sliceMsk
**                                Bit[31] - ASSI_P/N[31] and DSSI_P/N[31]
**                                ...
**                                Bit[1] - ASSI_P/N[1] and DSSI_P/N[1]
**                                Bit[0] - ASSI_P/N[0] and DSSI_P/N[0]
**                                Setting for each bit:
**                                1 - Loopback enabled
**                                0 - Loopback disabled
**                                This mask must only have bits set to 1 that
**                                have been specified for configuration by
**                                sliceMsk.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gBackplaneSerdesDiagLpbk(struct file *fileHndl, UINT4 sliceMsk,
                                     UINT4 enblMsk)
{
    /* variable declaration */
    UINT4 regAddr;
    UINT4 lpbckMsk;
    UINT4 i = 0;
    UINT4 enable = 0;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
	
    /* argument checking */
    if ((sliceMsk > 0xFFFFFFFF)||(enblMsk > 0xFFFFFFFF)||
        (enblMsk & sliceMsk) != enblMsk ) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Enable or disable PISO_SIPO loopback at 0x496C01C + 0x00200*A(A=0:31)
     C8 Configuration 8: C8_CFG_8 */
    lpbckMsk = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_PISO_SIPO_LOOPBACK_MSK;
    for (i = 0; i < 32; i++) {
        if ((sliceMsk >> i) & 0x1) {
            regAddr = HYPHY20G_SERDES_BP_C8_REG_CFG_8;
            regAddr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(regAddr, i);
            enable = (enblMsk >> i) & 0x1;

            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, lpbckMsk, enable);
            if (result) return result;
        }
    }
    return HYPHY20G_SUCCESS;

} /* hyPhy20gBackplaneSerdesDiagLpbk */

/*******************************************************************************
**
**  hyPhy20gPifSerdesDiagLpbk
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enable or disable PIF Serdes diagnostic
**                  loopback.  Data from Tx Serdes is looped back to Rx Serdes.
**
**                  The function will modify the device for each Serdes link
**                  specified in sliceMsk.  The loopback state for each of these
**                  links will be specified with enblMsk
**
**
**  INPUTS:       *fileHndl     - base address
**                 sliceMsk     - Mask of PIF Serdes Slices to
**                                modify loopback configuration
**                                Legal range:
**                                0x00 to 0xFF
**                                Bit[7] - Lane[7]
**                                ...
**                                Bit[1] - Lane[1]
**                                Bit[0] - Lane[0]
**                                Setting for each bit:
**                                1 - modify the configuration for link
**                                0 - do not modify configuration for link
**                 enblMsk      - Mask of bits containing loopback state for
**                                the bits specified by sliceMsk
**                                Bit[7] - Lane[7]
**                                ...
**                                Bit[1] - Lane[1]
**                                Bit[0] - Lane[0]
**                                Setting for each bit:
**                                1 - Loopback enabled
**                                0 - Loopback disabled
**                                This mask must only have bits set to 1 that
**                                have been specified for configuration by
**                                sliceMsk.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifSerdesDiagLpbk(struct file *fileHndl, UINT4 sliceMsk,
                               UINT4 enblMsk)
{
    /* variable declaration */
    UINT4 regAddr;
    UINT4 lpbckMsk;
    UINT4 i = 0;
    UINT4 enable = 0;
    INT4 result;

    /* argument checking */
    if ((sliceMsk > 0xFF)||(enblMsk > 0xFF)||
        (enblMsk & sliceMsk) != enblMsk ) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Enable or disable PISO_SIPO loopback at 0x496901C + 0x00200*A(A=0:7)
       C8 Configuration 8: C8_CFG_8 */
    lpbckMsk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_8_PISO_SIPO_LOOPBACK_MSK;
    for (i = 0; i < 8; i++) {
        if ((sliceMsk >> i) & 0x1) {
            regAddr = HYPHY20G_SERDES_PIF_C8_REG_CFG_8;
            regAddr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(regAddr, i);
            enable = (enblMsk >> i) & 0x1;

            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, lpbckMsk, enable);
            if (result) return result;
        }
    }
    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifSerdesDiagLpbk */

/*******************************************************************************
**
**  hyPhy20gSfpSerdesPattMonCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Pattern Monitor within the
**                  SFP Serdes subsystem.
**
**                  If hyPhy20gSfpRateCfg is used to generate rate
**                  configuration, buswidth is set as a function of the system
**                  side subsystem that is processing the data stream, buswidths
**                  of 16/20 may be configured.  In such cases, the equivalent
**                  rate configuration may be performed with
**                  hyPhy20gSfpCustomRateCfg using an 8/10 bit buswidth to
**                  perform pattern testing.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - SFP interface 0
**                                1 - SFP interface 1
**                                ..
**                                15 - SFP interface 15
**                  patType     - 0 - PRBS5 Pattern
**                                1 - PRBS7 Pattern
**                                2 - PRBS11 Pattern
**                                3 - PRBS15 Pattern
**                                4 - PRBS23 Pattern
**                                5 - PRBS31 Pattern
**                  invPatt     - 0 - PRBS pattern is not inverted
**                              - 1 - PRBS pattern is inverted
**                  enable      - 0 - PRBS Monitor is disabled
**                              - 1 - PRBS Monitor is enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gSfpSerdesPattMonCfg(struct file *fileHndl, UINT4 linkId,
                                 UINT4 patType, UINT4 invPatt, UINT4 enable)
{
    /* local variables */
    INT4 result = 0;
    UINT4 addr, cfgRegAddr;
    UINT4 val = 0;
    UINT4 enMsk, modeMsk, invMsk, rstMsk;

    /* argument checking */
    if ((linkId > HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM) && (linkId != 17)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((patType > 5)||(invPatt > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    cfgRegAddr = HYPHY20G_SERDES_SFP_CORE_REG_CONF_6;
    cfgRegAddr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(cfgRegAddr, linkId);

    enMsk = HYPHY20G_SERDES_SFP_TOP_BIT_PAT_MON_CFG_PATT_MON_EN_MSK;
    modeMsk = HYPHY20G_SERDES_SFP_TOP_BIT_PAT_MON_CFG_PATT_MON_MODE_MSK;
    invMsk = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_1_INV_REC_DATA_MSK;
    rstMsk = HYPHY20G_SERDES_SFP_TOP_BIT_PAT_MON_CFG_PATT_MON_RST_MSK;

    addr = HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_1;
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(addr, linkId);

    /* Configure INV_REC_DATA bit */
    result = hyPhy20gRegFieldWrite (fileHndl, addr, invMsk, invPatt);
    if(result) return result;

    addr = HYPHY20G_SERDES_SFP_TOP_REG_PAT_MON_CFG;
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(addr, linkId);

    /* Disable Pattern Monitor before configuring */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* set PATT_MON_EN = 0 */
    result = hyPhy20gVarFieldWrite (&val, enMsk, 0x0);
    if(result) return result;

    /* set PATT_MON_RST = 1 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x1);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* exit function if disabling */
    if(enable == 0){
        return HYPHY20G_SUCCESS;
    }

    /* Configure PAT_MON_CFG register */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* set PATT_MON_EN = 1 */
    result = hyPhy20gVarFieldWrite (&val, enMsk, 0x1);
    if(result) return result;

    /* Configure PATT_MON_MODE bit */
    result = hyPhy20gVarFieldWrite (&val, modeMsk, patType);
    if(result) return result;

    /* set PATT_MON_RST = 0 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSfpSerdesPattMonCfg */

/*******************************************************************************
**
**  hyPhy20gSfpSerdesPattMonResync
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function will issue a 'resync' command to the
**                  Pattern Monitor within the SFP Serdes subsystem.
**
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - SFP interface 0
**                                1 - SFP interface 1
**                                ..
**                                15 - SFP interface 15
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSfpSerdesPattMonResync(struct file *fileHndl, UINT4 linkId)
{
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;

    /* argument checking */
    if ((linkId > HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM) && (linkId != 17)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;

    /* setup register address and bit offset */
    addr = HYPHY20G_SERDES_SFP_TOP_REG_PAT_MON_CFG;
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(addr, linkId);
    bitoffset = HYPHY20G_SERDES_SFP_TOP_BIT_PAT_MON_CFG_PATT_MON_RESYNC_OFF;

    /* bit is not self-clearing so clear before re-sync */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* Set RESYNC bit to '1' */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSfpSerdesPattMonResync */

/*******************************************************************************
**
**  hyPhy20gSfpSerdesPattMonStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function checks the status of the Pattern Monitor
**                  within the SFP Serdes subsystem.
**
**                  Other status indicators sharing the same registers as
**                  the Pattern Monitor status will also be reported/cleared
**                  by this function.
**
**                  To reset the ZERO_DET_V/I status bits after a sychronization, 
**                  the monitor must be disabled and then re-enabled using 
**                  hyPhy20gSfpSerdesPattMonCfg.
**
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - SFP interface 0
**                                1 - SFP interface 1
**                                ..
**                                15- SFP interface 15
**                  intClear    - 1 - all *_I interrupt bits accessed by this
**                                    function will be cleared by writing them
**                                    to '1'
**                              - 0 - all *_I interrupt bits accessed by this
**                                    function will retain their values if
**                                    WCIMODE is used.
**                  errCntUpdate- 0 - retrieve counter without performing
**                                    local counter accumulation
**                                1 - retrieve counter after performing local
**                                    counter accumulation
**                  *pStatusV  - Pointer to storage for event state indication
**                                Bit 31:5 - Reserved
**                                Bit 4    - ACB_FAIL_V
**                                Bit 3    - LOS_V
**                                Bit 2    - PATT_MON_ZERO_DET_V
**                                Bit 1    - PATT_MON_SYNC_V
**                                Bit 0    - PATT_MON_ERR_V
**                  *pStatusI  - Pointer to storage for event indication bits
**                                Bit 31:5 - Reserved
**                                Bit 4    - ACB_FAIL_I
**                                Bit 3    - LOS_I
**                                Bit 2    - PATT_MON_ZERO_DET_I
**                                Bit 1    - PATT_MON_SYNC_I
**                                Bit 0    - PATT_MON_ERR_I
**                  *errorCount - Pointer to storage for 32-bit PRBS Error count
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSfpSerdesPattMonStatus(struct file *fileHndl, UINT4 linkId,
                                    UINT4 intClear, UINT4 errCntUpdate,
                                    UINT4 *pStatusV, UINT4 *pStatusI,
                                    UINT4 *errorCount)
{
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 bitmsk;
    UINT4 reg_val = 0;
    UINT4 losI, losV;
    UINT4 zeroDetI, zeroDetV;
    UINT4 syncI, syncV;
    UINT4 errI, errV;
    UINT4 acbFailI, acbFailV;
    UINT4 pattMonStat = 0, pattMonValue = 0;

    /* argument checking */
    if ((linkId > HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM) && (linkId != 17)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (errCntUpdate > 1 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Get PRBS error count */
    addr = HYPHY20G_SERDES_SFP_TOP_REG_PAT_MON_BIT_ERR_CNT;
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(addr, linkId);

    if (errCntUpdate) {
    		/* Trigger the counter update */
    		result = sysHyPhy20gWrite(fileHndl, addr, 0x0);
    		if (result) return result;
    }

    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;
    *errorCount = reg_val;

    /* Get PRBS Value bits */
    addr = HYPHY20G_SERDES_SFP_TOP_REG_INT_VAL;
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(addr, linkId);
    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;

    /* read out ACB_FAIL_V */
    bitmsk = HYPHY20G_SERDES_SFP_TOP_BIT_INT_VAL_ACB_FAIL_V_MSK;
    bitoffset = HYPHY20G_SERDES_SFP_TOP_BIT_INT_VAL_ACB_FAIL_V_OFF;
    acbFailV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out LOS_V */
    bitmsk = HYPHY20G_SERDES_SFP_TOP_BIT_INT_VAL_LOS_V_MSK;
    bitoffset = HYPHY20G_SERDES_SFP_TOP_BIT_INT_VAL_LOS_V_OFF;
    losV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out PATT_MON_ZERO_DET_V */
    bitmsk = HYPHY20G_SERDES_SFP_TOP_BIT_INT_VAL_PATT_MON_ZERO_DET_V_MSK;
    bitoffset = HYPHY20G_SERDES_SFP_TOP_BIT_INT_VAL_PATT_MON_ZERO_DET_V_OFF;
    zeroDetV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out PATT_MON_SYNC_V */
    bitmsk = HYPHY20G_SERDES_SFP_TOP_BIT_INT_VAL_PATT_MON_SYNC_V_MSK;
    bitoffset = HYPHY20G_SERDES_SFP_TOP_BIT_INT_VAL_PATT_MON_SYNC_V_OFF;
    syncV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out PATT_MON_ERR_V */
    bitmsk = HYPHY20G_SERDES_SFP_TOP_BIT_INT_VAL_PATT_MON_ERR_V_MSK;
    bitoffset = HYPHY20G_SERDES_SFP_TOP_BIT_INT_VAL_PATT_MON_ERR_V_OFF;
    errV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* return results */
    pattMonValue = (acbFailV << 4) | (losV << 3) | (zeroDetV << 2) |
                   (syncV << 1) | errV;
    *pStatusV = pattMonValue;

    /* Get PRBS Status bits */
    addr = HYPHY20G_SERDES_SFP_TOP_REG_INT_STAT;
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(addr, linkId);
    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;

    /* read out ACB_FAIL_I */
    bitmsk = HYPHY20G_SERDES_SFP_TOP_BIT_INT_STAT_ACB_FAIL_I_MSK;
    bitoffset = HYPHY20G_SERDES_SFP_TOP_BIT_INT_STAT_ACB_FAIL_I_OFF;
    acbFailI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out LOS_I */
    bitmsk = HYPHY20G_SERDES_SFP_TOP_BIT_INT_STAT_LOS_I_MSK;
    bitoffset = HYPHY20G_SERDES_SFP_TOP_BIT_INT_STAT_LOS_I_OFF;
    losI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out ZERO_DET_I */
    bitmsk = HYPHY20G_SERDES_SFP_TOP_BIT_INT_STAT_PATT_MON_ZERO_DET_I_MSK;
    bitoffset = HYPHY20G_SERDES_SFP_TOP_BIT_INT_STAT_PATT_MON_ZERO_DET_I_OFF;
    zeroDetI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out SYNC_I */
    bitmsk = HYPHY20G_SERDES_SFP_TOP_BIT_INT_STAT_PATT_MON_SYNC_I_MSK;
    bitoffset = HYPHY20G_SERDES_SFP_TOP_BIT_INT_STAT_PATT_MON_SYNC_I_OFF;
    syncI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out ERR_I */
    bitmsk = HYPHY20G_SERDES_SFP_TOP_BIT_INT_STAT_PATT_MON_ERR_I_MSK;
    bitoffset = HYPHY20G_SERDES_SFP_TOP_BIT_INT_STAT_PATT_MON_ERR_I_OFF;
    errI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* return results */
    pattMonStat = (acbFailI << 4) | (losI << 3) | (zeroDetI << 2) |
                  (syncI << 1) | errI;
    *pStatusI = pattMonStat;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSfpSerdesPattMonStatus */



/*******************************************************************************
**
**  hyPhy20gSfpSerdesPattGenCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Pattern Generator within the
**                  SFP Serdes subsystem.
**
**                  If hyPhy20gSfpRateCfg is used to generate rate
**                  configuration, buswidth is set as a function of the system
**                  side subsystem that is processing the data stream, buswidths
**                  of 16/20 may be configured.  In such cases, the equivalent
**                  rate configuration may be performed with
**                  hyPhy20gSfpCustomRateCfg using an 8/10 bit buswidth to
**                  perform pattern testing.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - SFP interface 0
**                                1 - SFP interface 1
**                                ..
**                                15 - SFP interface 15
**                  patType     - 0 - PRBS5 Pattern
**                                1 - PRBS7 Pattern
**                                2 - PRBS11 Pattern
**                                3 - PRBS15 Pattern
**                                4 - PRBS23 Pattern
**                                5 - PRBS31 Pattern
**                  invPatt     - 0 - PRBS pattern is not inverted
**                              - 1 - PRBS pattern is inverted
**                  enable      - 0 - PRBS Generator is disabled
**                              - 1 - PRBS Generator is enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gSfpSerdesPattGenCfg(struct file *fileHndl, UINT4 linkId,
                                 UINT4 patType, UINT4 invPatt, UINT4 enable)
{
    /* local variables */
    INT4 result = 0;
    UINT4 addr, cfgRegAddr;
    UINT4 val = 0;
    UINT4 enMsk, modeMsk, invMsk, rstMsk;

    /* argument checking */
    if ((linkId > HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM) && (linkId != 17)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (patType > 5) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((invPatt > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* initialize local variables */
    cfgRegAddr = HYPHY20G_SERDES_SFP_CORE_REG_CONF_6;
    cfgRegAddr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(cfgRegAddr, linkId);

    addr = HYPHY20G_SERDES_SFP_TOP_REG_PAT_GNRTR_CFG;
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(addr, linkId);
    enMsk = HYPHY20G_SERDES_SFP_TOP_BIT_PAT_GNRTR_CFG_PATT_GEN_EN_MSK;
    modeMsk = HYPHY20G_SERDES_SFP_TOP_BIT_PAT_GNRTR_CFG_PATT_GEN_MODE_MSK;
    invMsk = HYPHY20G_SERDES_SFP_TOP_BIT_PAT_GNRTR_CFG_PATT_GEN_INV_MSK;
    rstMsk = HYPHY20G_SERDES_SFP_TOP_BIT_PAT_GNRTR_CFG_PATT_GEN_RST_MSK;

    /* Disable Pattern Generator before configuring */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* set PATT_GEN_RST = 1 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x1);
    if(result) return result;

    /* set PATT_GEN_EN = 0 */
    result = hyPhy20gVarFieldWrite (&val, enMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* exit function if disabling */
    if(enable == 0){
        return HYPHY20G_SUCCESS;
    }

    /* Configure PAT_GNRTR_CFG register */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* Configure PATT_GEN_MODE bit */
    result = hyPhy20gVarFieldWrite (&val, modeMsk, patType);
    if(result) return result;

    /* Configure PATT_GEN_INV bit */
    result = hyPhy20gVarFieldWrite (&val, invMsk, invPatt);
    if(result) return result;

    /* set PATT_GEN_RST = 0 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* write the PATT_GEN_EN bit to 1 seperately */
    /* set PATT_GEN_EN = 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, addr, enMsk, 0x1);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSfpSerdesPattGenCfg */

/*******************************************************************************
**
**  hyPhy20gSfpSerdesPattGenErrInj
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function will inject a single error on the Pattern
**                  Generator within the SFP Serdes subsystem.  The error is
**                  injected by inverting the 8th bit of the next byte to be
**                  sent.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - SFP interface 0
**                                1 - SFP interface 1
**                                ..
**                                15 - SFP interface 15
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSfpSerdesPattGenErrInj(struct file *fileHndl, UINT4 linkId)
{
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;

    /* argument checking */
    if ((linkId > HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM) && (linkId != 17)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;

    /* setup register address and bit offset */
    addr = HYPHY20G_SERDES_SFP_TOP_REG_PAT_GNRTR_CFG;
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(addr, linkId);
    bitoffset = HYPHY20G_SERDES_SFP_TOP_BIT_PAT_GNRTR_CFG_PATT_GEN_ERR_INJ_OFF;

    /* bit is not self-clearing so clear before injecting error */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* set ERR_INJ bit to '1' */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSfpSerdesPattGenErrInj */



/*******************************************************************************
**
**  hyPhy20gSgmiiSerdesPattMonCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Pattern Monitor within the
**                  Sgmii Serdes subsystem.
**
**                  If hyPhy20gSfpRateCfg is used to generate rate
**                  configuration, buswidth is set as a function of the system
**                  side subsystem that is processing the data stream, buswidths
**                  of 16/20 may be configured.  In such cases, the equivalent
**                  rate configuration may be performed with
**                  hyPhy20gSfpCustomRateCfg using an 8/10 bit buswidth to
**                  perform pattern testing.
**
**  INPUTS:         *fileHndl   - base address
**                  patType     - 0 - PRBS5 Pattern
**                                1 - PRBS7 Pattern
**                                2 - PRBS11 Pattern
**                                3 - PRBS15 Pattern
**                                4 - PRBS23 Pattern
**                                5 - PRBS31 Pattern
**                  invPatt     - 0 - PRBS pattern is not inverted
**                              - 1 - PRBS pattern is inverted
**                  enable      - 0 - PRBS Monitor is disabled
**                              - 1 - PRBS Monitor is enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSgmiiSerdesPattMonCfg(struct file *fileHndl, UINT4 patType,
                                   UINT4 invPatt, UINT4 enable)
{
    INT4 result;
    UINT4 linkId;

    /* argument checking */
    if ((patType > 5)||(invPatt > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    linkId = 17;

    /* reuse SFP call w/ linkId*/
    result = hyPhy20gSfpSerdesPattMonCfg(fileHndl, linkId, patType, invPatt,
                                         enable);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSgmiiSerdesPattMonCfg */

/*******************************************************************************
**
**  hyPhy20gSgmiiSerdesPattMonResync
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function will issue a 'resync' command to the
**                  Pattern Monitor within the Sgmii Serdes subsystem.
**
**
**  INPUTS:         *fileHndl   - base address
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSgmiiSerdesPattMonResync(struct file *fileHndl)
{
    INT4 result;
    UINT4 linkId;

    /* initialize local variables */
    result = 0;
    linkId = 17;

    /* reuse SFP call w/ linkId*/
    result = hyPhy20gSfpSerdesPattMonResync(fileHndl, linkId);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSgmiiSerdesPattMonResync */

/*******************************************************************************
**
**  hyPhy20gSgmiiSerdesPattMonStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function checks the status of the Pattern Monitor
**                  within the Sgmii Serdes subsystem.
**
**                  Other status indicators sharing the same registers as
**                  the Pattern Monitor status will also be reported/cleared
**                  by this function.
**
**                  To reset the ZERO_DET_V/I status bits after a sychronization, 
**                  the monitor must be disabled and then re-enabled using 
**                  hyPhy20gSgmiiSerdesPattMonCfg.
**
**  INPUTS:         *fileHndl   - base address
**                  intClear    - 1 - all *_I interrupt bits accessed by this
**                                    function will be cleared by writing them
**                                    to '1'
**                              - 0 - all *_I interrupt bits accessed by this
**                                    function will retain their values if
**                                    WCIMODE is used.
**                  errCntUpdate- 0 - retrieve counter without performing
**                                    local counter accumulation
**                                1 - retrieve counter after performing local
**                                    counter accumulation
**                  *pStatusV - Pointer to storage for event state indication
**                                Bit 31:5 - Reserved
**                                Bit 4    - ACB_FAIL_V
**                                Bit 3    - LOS_V
**                                Bit 2    - PATT_MON_ZERO_DET_V
**                                Bit 1    - PATT_MON_SYNC_V
**                                Bit 0    - PATT_MON_ERR_V
**                  *pStatusI  - Pointer to storage for event indication bits
**                                Bit 31:5 - Reserved
**                                Bit 4    - ACB_FAIL_I
**                                Bit 3    - LOS_I
**                                Bit 2    - PATT_MON_ZERO_DET_I
**                                Bit 1    - PATT_MON_SYNC_I
**                                Bit 0    - PATT_MON_ERR_I
**                  *errorCount - Pointer to storage for 32-bit PRBS Error count
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSgmiiSerdesPattMonStatus(struct file *fileHndl, UINT4 intClear,
                                      UINT4 errCntUpdate, UINT4 *pStatusV,
                                      UINT4 *pStatusI, UINT4 *errorCount)
{
    INT4 result;
    UINT4 linkId;

    /* argument checking */
    if (errCntUpdate > 1 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    linkId = 17;

    /* reuse SFP call w/ linkId*/
    result = hyPhy20gSfpSerdesPattMonStatus(fileHndl, linkId, intClear,
                                            errCntUpdate, pStatusV, pStatusI,
                                            errorCount);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSgmiiSerdesPattMonStatus */


/*******************************************************************************
**
**  hyPhy20gSgmiiSerdesPattGenCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Pattern Generator within the
**                  Sgmii Serdes subsystem.
**
**                  If hyPhy20gSfpRateCfg is used to generate rate
**                  configuration, buswidth is set as a function of the system
**                  side subsystem that is processing the data stream, buswidths
**                  of 16/20 may be configured.  In such cases, the equivalent
**                  rate configuration may be performed with
**                  hyPhy20gSfpCustomRateCfg using an 8/10 bit buswidth to
**                  perform pattern testing.
**
**  INPUTS:         *fileHndl   - base address
**                  patType     - 0 - PRBS5 Pattern
**                                1 - PRBS7 Pattern
**                                2 - PRBS11 Pattern
**                                3 - PRBS15 Pattern
**                                4 - PRBS23 Pattern
**                                5 - PRBS31 Pattern
**                  invPatt     - 0 - PRBS pattern is not inverted
**                              - 1 - PRBS pattern is inverted
**                  enable      - 0 - PRBS Generator is disabled
**                              - 1 - PRBS Generator is enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSgmiiSerdesPattGenCfg(struct file *fileHndl, UINT4 patType,
                                   UINT4 invPatt, UINT4 enable)
{
    INT4 result;
    UINT4 linkId;

    /* argument checking */
    if (patType > 5) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((invPatt > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    linkId = 17;

    /* reuse SFP call w/ linkId*/
    result = hyPhy20gSfpSerdesPattGenCfg(fileHndl, linkId, patType, invPatt,
                                         enable);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSgmiiSerdesPattGenCfg */

/*******************************************************************************
**
**  hyPhy20gSgmiiSerdesPattGenErrInj
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function will inject a single error on the Pattern
**                  Generator within the Sgmii Serdes subsystem.  The error is
**                  injected by inverting the 8th bit of the next byte to be
**                  sent.
**
**  INPUTS:         *fileHndl   - base address
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSgmiiSerdesPattGenErrInj(struct file *fileHndl)
{
    INT4 result;
    UINT4 linkId;

    /* initialize local variables */
    result = 0;
    linkId = 17;

    /* reuse SFP call w/ linkId*/
    result = hyPhy20gSfpSerdesPattGenErrInj(fileHndl, linkId);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSgmiiSerdesPattGenErrInj */



/*******************************************************************************
**
**  hyPhy20gXfiSerdesPattMonCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Pattern Monitor within the
**                  XFI Serdes subsystem.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - XFI interface 0
**                                1 - XFI interface 1
**                  patType     - 0 - PRBS5 Pattern
**                                1 - PRBS7 Pattern
**                                2 - PRBS11 Pattern
**                                3 - PRBS15 Pattern
**                                4 - PRBS23 Pattern
**                                5 - PRBS31 Pattern
**                  invPatt     - 0 - PRBS pattern is not inverted
**                              - 1 - PRBS pattern is inverted
**                  enable      - 0 - PRBS Monitor is disabled
**                              - 1 - PRBS Monitor is enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gXfiSerdesPattMonCfg(struct file *fileHndl, UINT4 linkId,
                                 UINT4 patType, UINT4 invPatt, UINT4 enable)
{
    INT4 result = 0;
    UINT4 addr;
    UINT4 val = 0;
    UINT4 enMsk, modeMsk, invMsk, rstMsk;

    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((patType > 5)||(invPatt > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */

    enMsk = HYPHY20G_SERDES_XFI_TOP_BIT_PAT_MON_CFG_PATT_MON_EN_MSK;
    modeMsk = HYPHY20G_SERDES_XFI_TOP_BIT_PAT_MON_CFG_PATT_MON_MODE_MSK;
    invMsk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_1_INV_REC_DATA_MSK;
    rstMsk = HYPHY20G_SERDES_XFI_TOP_BIT_PAT_MON_CFG_PATT_MON_RST_MSK;

    addr = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_1;
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(addr, linkId);

    /* Configure PATT_MON_INV bit */
    result = hyPhy20gRegFieldWrite (fileHndl, addr, invMsk, invPatt);
    if(result) return result;

    addr = HYPHY20G_SERDES_XFI_TOP_REG_PAT_MON_CFG;
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(addr, linkId);

    /* Disable Pattern Monitor before configuring */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* set PATT_MON_RST = 1 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x1);
    if(result) return result;

    /* set PATT_MON_EN = 0 */
    result = hyPhy20gVarFieldWrite (&val, enMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* exit function if disabling */
    if(enable == 0){
        return HYPHY20G_SUCCESS;
    }

    /* Configure PAT_MON_CFG register */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* set PATT_MON_EN = 1 */
    result = hyPhy20gVarFieldWrite (&val, enMsk, 0x1);
    if(result) return result;

    /* Configure PATT_MON_MODE bit */
    result = hyPhy20gVarFieldWrite (&val, modeMsk, patType);
    if(result) return result;

    /* set PATT_MON_RST = 0 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gXfiSerdesPattMonCfg */

/*******************************************************************************
**
**  hyPhy20gXfiSerdesPattMonResync
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function will issue a 'resync' command to the
**                  Pattern Monitor within the XFI Serdes subsystem.
**
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - XFI interface 0
**                                1 - XFI interface 1
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gXfiSerdesPattMonResync(struct file *fileHndl, UINT4 linkId)
{
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;

    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;

    /* setup register address and bit offset */
    addr = HYPHY20G_SERDES_XFI_TOP_REG_PAT_MON_CFG;
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(addr, linkId);
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_PAT_MON_CFG_PATT_MON_RESYNC_OFF;

    /* bit is not self-clearing so clear before re-sync */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* Set RESYNC bit to '1' */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gXfiSerdesPattMonResync */

/*******************************************************************************
**
**  hyPhy20gXfiSerdesPattMonStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function checks the status of the Pattern Monitor
**                  within the XFI Serdes subsystem.
**
**                  Other status indicators sharing the same registers as
**                  the Pattern Monitor status will also be reported/cleared
**                  by this function.
**
**                  To reset the ZERO_DET_V/I status bits after a sychronization, 
**                  the monitor must be disabled and then re-enabled using 
**                  hyPhy20gXfiSerdesPattMonCfg.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - XFI interface 0
**                                1 - XFI interface 1
**                  intClear    - 1 - all *_I interrupt bits accessed by this
**                                    function will be cleared by writing them
**                                    to '1'
**                              - 0 - all *_I interrupt bits accessed by this
**                                    function will retain their values if
**                                    WCIMODE is used.
**                  errCntUpdate- 0 - retrieve counter without performing
**                                    local counter accumulation
**                                1 - retrieve counter after performing local
**                                    counter accumulation
**                  *pStatusV  - Pointer to storage for event state indication
**                                Bit 31:6 - Reserved
**                                Bit 5    - ACB_FAIL_V
**                                Bit 4    - Reserved
**                                Bit 3    - LOS_V
**                                Bit 2    - PATT_MON_ZERO_DET_V
**                                Bit 1    - PATT_MON_SYNC_V
**                                Bit 0    - PATT_MON_ERR_V
**                  *pStatusI  - Pointer to storage for event indication bits
**                                Bit 31:6 - Reserved
**                                Bit 5    - ACB_FAIL_I
**                                Bit 4    - Reserved
**                                Bit 3    - LOS_I
**                                Bit 2    - PATT_MON_ZERO_DET_I
**                                Bit 1    - PATT_MON_SYNC_I
**                                Bit 0    - PATT_MON_ERR_I
**                  *errorCount - Pointer to storage for 32-bit PRBS Error count
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gXfiSerdesPattMonStatus(struct file *fileHndl, UINT4 linkId,
                                    UINT4 intClear, UINT4 errCntUpdate,
                                    UINT4 *pStatusV, UINT4 *pStatusI,
                                    UINT4 *errorCount)
{
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 bitmsk;
    UINT4 reg_val = 0;
    UINT4 losV, losI;
    UINT4 zeroDetV, zeroDetI;
    UINT4 syncV, syncI;
    UINT4 errV, errI;
    UINT4 acbFailI, acbFailV;
    UINT4 pattMonStat = 0, pattMonValue = 0;

    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (errCntUpdate > 1 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Get PRBS error count */
    addr = HYPHY20G_SERDES_XFI_TOP_REG_PAT_MON_BIT_ERR_CNT;
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(addr, linkId);

    if (errCntUpdate) {
    		/* Trigger the counter update */
    		result = sysHyPhy20gWrite(fileHndl, addr, 0x0);
    		if (result) return result;
    }

    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;
    *errorCount = reg_val;

    /* Get PRBS Value bits */
    addr = HYPHY20G_SERDES_XFI_TOP_REG_INT_VAL;
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(addr, linkId);
    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;

    /* read out ACB_FAIL_V */
    bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_INT_VAL_ACB_FAIL_V_MSK;
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_INT_VAL_ACB_FAIL_V_OFF;
    acbFailV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out LOS_V */
    bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_INT_VAL_LOS_V_MSK;
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_INT_VAL_LOS_V_OFF;
    losV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out PATT_MON_ZERO_DET_V */
    bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_INT_VAL_PATT_MON_ZERO_DET_V_MSK;
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_INT_VAL_PATT_MON_ZERO_DET_V_OFF;
    zeroDetV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out PATT_MON_SYNC_V */
    bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_INT_VAL_PATT_MON_SYNC_V_MSK;
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_INT_VAL_PATT_MON_SYNC_V_OFF;
    syncV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out PATT_MON_ERR_V */
    bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_INT_VAL_PATT_MON_ERR_V_MSK;
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_INT_VAL_PATT_MON_ERR_V_OFF;
    errV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* return results */
    pattMonValue = (acbFailV << 5) | (losV << 3) | (zeroDetV << 2) |
                   (syncV << 1) | errV;
    *pStatusV = pattMonValue;

    /* Get PRBS Status bits */
    addr = HYPHY20G_SERDES_XFI_TOP_REG_INT_STAT;
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(addr, linkId);
    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;

    /* read out ACB_FAIL_I */
    bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_INT_STAT_ACB_FAIL_I_MSK;
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_INT_STAT_ACB_FAIL_I_OFF;
    acbFailI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out LOS_I */
    bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_INT_STAT_LOS_I_MSK;
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_INT_STAT_LOS_I_OFF;
    losI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out ZERO_DET_I */
    bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_INT_STAT_PATT_MON_ZERO_DET_I_MSK;
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_INT_STAT_PATT_MON_ZERO_DET_I_OFF;
    zeroDetI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out SYNC_I */
    bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_INT_STAT_PATT_MON_SYNC_I_MSK;
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_INT_STAT_PATT_MON_SYNC_I_OFF;
    syncI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out ERR_I */
    bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_INT_STAT_PATT_MON_ERR_I_MSK;
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_INT_STAT_PATT_MON_ERR_I_OFF;
    errI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* return results */
    pattMonStat = (acbFailI << 5) | (losI << 3) | (zeroDetI << 2) |
                  (syncI << 1) | errI;
    *pStatusI = pattMonStat;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gXfiSerdesPattMonStatus */



/*******************************************************************************
**
**  hyPhy20gXfiSerdesPattGenCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Pattern Generator within the
**                  XFI Serdes subsystem.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - XFI interface 0
**                                1 - XFI interface 1
**                  patType     - 0 - PRBS5 Pattern
**                                1 - PRBS7 Pattern
**                                2 - PRBS11 Pattern
**                                3 - PRBS15 Pattern
**                                4 - PRBS23 Pattern
**                                5 - PRBS31 Pattern
**                  invPatt     - 0 - PRBS pattern is not inverted
**                              - 1 - PRBS pattern is inverted
**                  enable      - 0 - PRBS Generator is disabled
**                              - 1 - PRBS Generator is enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gXfiSerdesPattGenCfg(struct file *fileHndl, UINT4 linkId,
                                 UINT4 patType, UINT4 invPatt, UINT4 enable)
{
    INT4 result = 0;
    UINT4 addr;
    UINT4 val = 0;
    UINT4 enMsk, modeMsk, invMsk, rstMsk;

    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (patType > 5) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((invPatt > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    addr = HYPHY20G_SERDES_XFI_TOP_REG_PAT_GNRTR_CFG;
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(addr, linkId);

    enMsk = HYPHY20G_SERDES_XFI_TOP_BIT_PAT_GNRTR_CFG_PATT_GEN_EN_MSK;
    modeMsk = HYPHY20G_SERDES_XFI_TOP_BIT_PAT_GNRTR_CFG_PATT_GEN_MODE_MSK;
    invMsk = HYPHY20G_SERDES_XFI_TOP_BIT_PAT_GNRTR_CFG_PATT_GEN_INV_MSK;
    rstMsk = HYPHY20G_SERDES_XFI_TOP_BIT_PAT_GNRTR_CFG_PATT_GEN_RST_MSK;

    /* Disable Pattern Generator before configuring */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* set PATT_GEN_RST = 1 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x1);
    if(result) return result;

    /* set PATT_GEN_EN = 0 */
    result = hyPhy20gVarFieldWrite (&val, enMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* exit function if disabling */
    if(enable == 0){
        return HYPHY20G_SUCCESS;
    }

    /* Configure PAT_GNRTR_CFG register */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* Configure PATT_GEN_MODE bit */
    result = hyPhy20gVarFieldWrite (&val, modeMsk, patType);
    if(result) return result;

    /* Configure PATT_GEN_INV bit */
    result = hyPhy20gVarFieldWrite (&val, invMsk, invPatt);
    if(result) return result;

    /* set PATT_GEN_RST = 0 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* write the PATT_GEN_EN bit to 1 seperately */
    /* set PATT_GEN_EN = 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, addr, enMsk, 0x1);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gXfiSerdesPattGenCfg */

/*******************************************************************************
**
**  hyPhy20gXfiSerdesPattGenErrInj
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function will inject a single error on the Pattern
**                  Generator within the XFI Serdes subsystem.  The error is
**                  injected by inverting the 8th bit of the next byte to be
**                  sent.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - XFI interface 0
**                                1 - XFI interface 1
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gXfiSerdesPattGenErrInj(struct file *fileHndl, UINT4 linkId)
{
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;

    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;

    /* setup register address and bit offset */
    addr = HYPHY20G_SERDES_XFI_TOP_REG_PAT_GNRTR_CFG;
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(addr, linkId);
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_PAT_GNRTR_CFG_PATT_GEN_ERR_INJ_OFF;

    /* bit is not self-clearing so clear before injecting error */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* set ERR_INJ bit to '1' */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gXfiSerdesPattGenErrInj */



/*******************************************************************************
**
**  hyPhy20gPifSerdesPattMonCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Pattern Monitor within the
**                  Pif Serdes subsystem.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - PIF Interface 0
**                                1 - PIF Interface 1
**                                ..
**                                7 - PIF Interface 7
**                  patType     - 0 - PRBS5 Pattern
**                                1 - PRBS7 Pattern
**                                2 - PRBS11 Pattern
**                                3 - PRBS15 Pattern
**                                4 - PRBS23 Pattern
**                                5 - PRBS31 Pattern
**                  invPatt     - 0 - PRBS pattern is not inverted
**                              - 1 - PRBS pattern is inverted
**                  enable      - 0 - PRBS Monitor is disabled
**                              - 1 - PRBS Monitor is enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifSerdesPattMonCfg(struct file *fileHndl, UINT4 linkId,
                                 UINT4 patType, UINT4 invPatt, UINT4 enable)
{
    INT4 result = 0;
    UINT4 addr;
    UINT4 val = 0;
    UINT4 enMsk, modeMsk, invMsk, rstMsk;

    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((patType > 5)||(invPatt > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    enMsk = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_PAT_MON_CFG_PATT_MON_EN_MSK;
    modeMsk = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_PAT_MON_CFG_PATT_MON_MODE_MSK;
    invMsk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_1_INV_REC_DATA_MSK;
    rstMsk = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_PAT_MON_CFG_PATT_MON_RST_MSK;


    addr = HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_1;
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(addr, linkId);

    /* Configure PATT_MON_INV bit */
    result = hyPhy20gRegFieldWrite (fileHndl, addr, invMsk, invPatt);
    if(result) return result;

    addr = HYPHY20G_SERDES_PIF_TXRXSLICE_REG_PAT_MON_CFG;
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(addr, linkId);

    /* Disable Pattern Monitor before configuring */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* set PATT_MON_RST = 1 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x1);
    if(result) return result;

    /* set PATT_MON_EN = 0 */
    result = hyPhy20gVarFieldWrite (&val, enMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* exit function if disabling */
    if(enable == 0){
        return HYPHY20G_SUCCESS;
    }

    /* Configure PAT_MON_CFG register */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* set PATT_MON_EN = 1 */
    result = hyPhy20gVarFieldWrite (&val, enMsk, 0x1);
    if(result) return result;

    /* Configure PATT_MON_MODE bit */
    result = hyPhy20gVarFieldWrite (&val, modeMsk, patType);
    if(result) return result;

    /* set PATT_MON_RST = 0 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifSerdesPattMonCfg */

/*******************************************************************************
**
**  hyPhy20gPifSerdesPattMonResync
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function will issue a 'resync' command to the
**                  Pattern Monitor within the Pif Serdes subsystem.
**
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - PIF Interface 0
**                                1 - PIF Interface 1
**                                ..
**                                7 - PIF Interface 7
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifSerdesPattMonResync(struct file *fileHndl, UINT4 linkId)
{
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;

    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;

    /* setup register address and bit offset */
    addr = HYPHY20G_SERDES_PIF_TXRXSLICE_REG_PAT_MON_CFG;
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(addr, linkId);
    bitoffset = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_PAT_MON_CFG_PATT_MON_RESYNC_OFF;

    /* bit is not self-clearing so clear before re-sync */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* Set RESYNC bit to '1' */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifSerdesPattMonResync */

/*******************************************************************************
**
**  hyPhy20gPifSerdesPattMonStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function checks the status of the Pattern Monitor
**                  within the Pif Serdes subsystem.
**
**                  To reset the ZERO_DET_V/I status bits after a sychronization, 
**                  the monitor must be disabled and then re-enabled using 
**                  hyPhy20gPifSerdesPattMonCfg.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - PIF Interface 0
**                                1 - PIF Interface 1
**                                ..
**                                7 - PIF Interface 7
**                  intClear    - 1 - all *_I interrupt bits accessed by this
**                                    function will be cleared by writing them
**                                    to '1'
**                              - 0 - all *_I interrupt bits accessed by this
**                                    function will retain their values if
**                                    WCIMODE is used.
**                  errCntUpdate- 0 - retrieve counter without performing
**                                    local counter accumulation
**                                1 - retrieve counter after performing local
**                                    counter accumulation
**                  *pStatusV  - Pointer to storage for event state indication
**                                Bit 31:3 - Reserved
**                                Bit 2    - PATT_MON_ZERO_DET_V
**                                Bit 1    - PATT_MON_SYNC_V
**                                Bit 0    - PATT_MON_ERR_V
**                  *pStatusI  - Pointer to storage for event indication bits
**                                Bit 31:3 - Reserved
**                                Bit 2    - PATT_MON_ZERO_DET_I
**                                Bit 1    - PATT_MON_SYNC_I
**                                Bit 0    - PATT_MON_ERR_I
**                  *errorCount - Pointer to storage for 32-bit PRBS Error count
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifSerdesPattMonStatus(struct file *fileHndl,UINT4 linkId,
                                    UINT4 intClear, UINT4 errCntUpdate,
                                    UINT4 *pStatusV, UINT4 *pStatusI,
                                    UINT4 *errorCount)
{
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 bitmsk;
    UINT4 reg_val = 0;
    UINT4 zeroDetV, zeroDetI;
    UINT4 syncV, syncI;
    UINT4 errV, errI;
    UINT4 pattMonStat = 0, pattMonValue = 0;

    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (errCntUpdate > 1 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Get PRBS error count */
    addr = HYPHY20G_SERDES_PIF_TXRXSLICE_REG_PAT_MON_BIT_ERR_CNT;
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(addr, linkId);

    if (errCntUpdate) {
    		/* Trigger the counter update */
    		result = sysHyPhy20gWrite(fileHndl, addr, 0x0);
    		if (result) return result;
    }

    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;
    *errorCount = reg_val;

    /* Get PRBS Value bits */
    addr = HYPHY20G_SERDES_PIF_TXRXSLICE_REG_INT_VAL;
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(addr, linkId);
    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;

    /* read out PATT_MON_ZERO_DET_V */
    bitmsk = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_INT_VAL_PATT_MON_ZERO_DET_V_MSK;
    bitoffset = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_INT_VAL_PATT_MON_ZERO_DET_V_OFF;
    zeroDetV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out PATT_MON_SYNC_V */
    bitmsk = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_INT_VAL_PATT_MON_SYNC_V_MSK;
    bitoffset = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_INT_VAL_PATT_MON_SYNC_V_OFF;
    syncV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out PATT_MON_ERR_V */
    bitmsk = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_INT_VAL_PATT_MON_ERR_V_MSK;
    bitoffset = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_INT_VAL_PATT_MON_ERR_V_OFF;
    errV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* return results */
    pattMonValue = (zeroDetV << 2) | (syncV << 1) | errV;
    *pStatusV = pattMonValue;

    /* Get PRBS Status bits */
    addr = HYPHY20G_SERDES_PIF_TXRXSLICE_REG_INT_STAT;
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(addr, linkId);
    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;

    /* read out ZERO_DET_I */
    bitmsk = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_INT_STAT_PATT_MON_ZERO_DET_I_MSK;
    bitoffset = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_INT_STAT_PATT_MON_ZERO_DET_I_OFF;
    zeroDetI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out SYNC_I */
    bitmsk = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_INT_STAT_PATT_MON_SYNC_I_MSK;
    bitoffset = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_INT_STAT_PATT_MON_SYNC_I_OFF;
    syncI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out ERR_I */
    bitmsk = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_INT_STAT_PATT_MON_ERR_I_MSK;
    bitoffset = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_INT_STAT_PATT_MON_ERR_I_OFF;
    errI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* return results */
    pattMonStat = (zeroDetI << 2) | (syncI << 1) | errI;
    *pStatusI = pattMonStat;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }


    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifSerdesPattMonStatus */



/*******************************************************************************
**
**  hyPhy20gPifSerdesPattGenCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Pattern Generator within the
**                  Pif Serdes subsystem.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - PIF Interface 0
**                                1 - PIF Interface 1
**                                ..
**                                7 - PIF Interface 7
**                  patType     - 0 - PRBS5 Pattern
**                                1 - PRBS7 Pattern
**                                2 - PRBS11 Pattern
**                                3 - PRBS15 Pattern
**                                4 - PRBS23 Pattern
**                                5 - PRBS31 Pattern
**                  invPatt     - 0 - PRBS pattern is not inverted
**                              - 1 - PRBS pattern is inverted
**                  enable      - 0 - PRBS Generator is disabled
**                              - 1 - PRBS Generator is enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifSerdesPattGenCfg(struct file *fileHndl, UINT4 linkId,
                                 UINT4 patType, UINT4 invPatt, UINT4 enable)
{
    INT4 result = 0;
    UINT4 addr;
    UINT4 val = 0;
    UINT4 enMsk, modeMsk, invMsk, rstMsk;

    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (patType > 5) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((invPatt > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    addr = HYPHY20G_SERDES_PIF_TXRXSLICE_REG_PAT_GNRTR_CFG;
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(addr, linkId);

    enMsk = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_PAT_GNRTR_CFG_PATT_GEN_EN_MSK;
    modeMsk = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_PAT_GNRTR_CFG_PATT_GEN_MODE_MSK;
    invMsk = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_PAT_GNRTR_CFG_PATT_GEN_INV_MSK;
    rstMsk = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_PAT_GNRTR_CFG_PATT_GEN_RST_MSK;

    /* Disable Pattern Generator before configuring */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* set PATT_GEN_RST = 1 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x1);
    if(result) return result;

    /* set PATT_GEN_EN = 0 */
    result = hyPhy20gVarFieldWrite (&val, enMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* exit function if disabling */
    if(enable == 0){
        return HYPHY20G_SUCCESS;
    }

    /* Configure PAT_GNRTR_CFG register */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* Configure PATT_GEN_MODE bit */
    result = hyPhy20gVarFieldWrite (&val, modeMsk, patType);
    if(result) return result;

    /* Configure PATT_GEN_INV bit */
    result = hyPhy20gVarFieldWrite (&val, invMsk, invPatt);
    if(result) return result;

    /* set PATT_GEN_RST = 0 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* write the PATT_GEN_EN bit to 1 seperately */
    /* set PATT_GEN_EN = 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, addr, enMsk, 0x1);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifSerdesPattGenCfg */

/*******************************************************************************
**
**  hyPhy20gPifSerdesPattGenErrInj
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function will inject a single error on the Pattern
**                  Generator within the Pif Serdes subsystem.  The error is
**                  injected by inverting the 8th bit of the next byte to be
**                  sent.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - PIF Interface 0
**                                1 - PIF Interface 1
**                                ..
**                                7 - PIF Interface 7
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifSerdesPattGenErrInj(struct file *fileHndl, UINT4 linkId)
{
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;

    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;

    /* setup register address and bit offset */
    addr = HYPHY20G_SERDES_PIF_TXRXSLICE_REG_PAT_GNRTR_CFG;
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(addr, linkId);
    bitoffset = HYPHY20G_SERDES_PIF_TXRXSLICE_BIT_PAT_GNRTR_CFG_PATT_GEN_ERR_INJ_OFF;

    /* bit is not self-clearing so clear before injecting error */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* set ERR_INJ bit to '1' */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifSerdesPattGenErrInj */



/*******************************************************************************
**
**  hyPhy20gBackplaneSerdesPattMonCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Pattern Monitor within the
**                  Backplane Serdes subsystem.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - Backplane Interface 0
**                                1 - Backplane Interface 1
**                                ..
**                                31 - Backplane Interface 31
**                  patType     - 0 - PRBS5 Pattern
**                                1 - PRBS7 Pattern
**                                2 - PRBS11 Pattern
**                                3 - PRBS15 Pattern
**                                4 - PRBS23 Pattern
**                                5 - PRBS31 Pattern
**                  invPatt     - 0 - PRBS pattern is not inverted
**                              - 1 - PRBS pattern is inverted
**                  enable      - 0 - PRBS Monitor is disabled
**                              - 1 - PRBS Monitor is enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gBackplaneSerdesPattMonCfg(struct file *fileHndl, UINT4 linkId,
                                       UINT4 patType, UINT4 invPatt,
                                       UINT4 enable)
{
    INT4 result = 0;
    UINT4 addr;
    UINT4 val = 0;
    UINT4 enMsk, modeMsk, invMsk, rstMsk;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
	
    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((patType > 5)||(invPatt > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */

    enMsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_PAT_MON_CFG_PATT_MON_EN_MSK;
    modeMsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_PAT_MON_CFG_PATT_MON_MODE_MSK;
    invMsk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_1_INV_REC_DATA_MSK;
    rstMsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_PAT_MON_CFG_PATT_MON_RST_MSK;


    addr = HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_1;
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(addr, linkId);

    /* Configure PATT_MON_INV bit */
    result = hyPhy20gRegFieldWrite (fileHndl, addr, invMsk, invPatt);
    if(result) return result;

    addr = HYPHY20G_SERDES_BP_TXRXSLICE_REG_PAT_MON_CFG;
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(addr, linkId);

    /* Disable Pattern Monitor before configuring */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* set PATT_MON_RST = 1 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x1);
    if(result) return result;

    /* set PATT_MON_EN = 0 */
    result = hyPhy20gVarFieldWrite (&val, enMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* exit function if disabling */
    if(enable == 0){
        return HYPHY20G_SUCCESS;
    }

    /* Configure PAT_MON_CFG register */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* set PATT_MON_EN = 1 */
    result = hyPhy20gVarFieldWrite (&val, enMsk, 0x1);
    if(result) return result;

    /* Configure PATT_MON_MODE bit */
    result = hyPhy20gVarFieldWrite (&val, modeMsk, patType);
    if(result) return result;

    /* set PATT_MON_RST = 0 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gBackplaneSerdesPattMonCfg */

/*******************************************************************************
**
**  hyPhy20gBackplaneSerdesPattMonResync
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function will issue a 'resync' command to the
**                  Pattern Monitor within the Backplane Serdes subsystem.
**
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - Backplane Interface 0
**                                1 - Backplane Interface 1
**                                ..
**                                31 - Backplane Interface 31
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gBackplaneSerdesPattMonResync(struct file *fileHndl, UINT4 linkId)
{
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
	
    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;

    /* setup register address and bit offset */
    addr = HYPHY20G_SERDES_BP_TXRXSLICE_REG_PAT_MON_CFG;
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(addr, linkId);
    bitoffset = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_PAT_MON_CFG_PATT_MON_RESYNC_OFF;

    /* bit is not self-clearing so clear before re-sync */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* Set RESYNC bit to '1' */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gBackplaneSerdesPattMonResync */

/*******************************************************************************
**
**  hyPhy20gBackplaneSerdesPattMonStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function checks the status of the Pattern Monitor
**                  within the Backplane Serdes subsystem.
**
**                  To reset the ZERO_DET_V/I status bits after a sychronization, 
**                  the monitor must be disabled and then re-enabled using 
**                  hyPhy20gBackplaneSerdesPattMonCfg.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - Backplane Interface 0
**                                1 - Backplane Interface 1
**                                ..
**                                31 - Backplane Interface 31
**                  intClear    - 1 - all *_I interrupt bits accessed by this
**                                    function will be cleared by writing them
**                                    to '1'
**                              - 0 - all *_I interrupt bits accessed by this
**                                    function will retain their values if
**                                    WCIMODE is used.
**                  errCntUpdate- 0 - retrieve counter without performing
**                                    local counter accumulation
**                                1 - retrieve counter after performing local
**                                    counter accumulation
**                  *pStatusV  - Pointer to storage for event state indication
**                                Bit 31:3 - Reserved
**                                Bit 2    - PATT_MON_ZERO_DET_V
**                                Bit 1    - PATT_MON_SYNC_V
**                                Bit 0    - PATT_MON_ERR_V
**                  *pStatusI  - Pointer to storage for event indication bits
**                                Bit 31:3 - Reserved
**                                Bit 2    - PATT_MON_ZERO_DET_I
**                                Bit 1    - PATT_MON_SYNC_I
**                                Bit 0    - PATT_MON_ERR_I
**                  *errorCount - Pointer to storage for 32-bit PRBS Error count
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gBackplaneSerdesPattMonStatus(struct file *fileHndl,UINT4 linkId,
                                          UINT4 intClear, UINT4 errCntUpdate,
                                          UINT4 *pStatusV, UINT4 *pStatusI,
                                          UINT4 *errorCount)
{
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 bitmsk;
    UINT4 reg_val = 0;
    UINT4 zeroDetV, zeroDetI;
    UINT4 syncV, syncI;
    UINT4 errV, errI;
    UINT4 pattMonStat = 0, pattMonValue = 0;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
	
    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (errCntUpdate > 1 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Get PRBS error count */
    addr = HYPHY20G_SERDES_BP_TXRXSLICE_REG_PAT_MON_BIT_ERR_CNT;
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(addr, linkId);

    if (errCntUpdate) {
    		/* Trigger the counter update */
    		result = sysHyPhy20gWrite(fileHndl, addr, 0x0);
    		if (result) return result;
    }

    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;
    *errorCount = reg_val;

    /* Get PRBS Value bits */
    addr = HYPHY20G_SERDES_BP_TXRXSLICE_REG_INT_VAL;
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(addr, linkId);
    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;

    /* read out PATT_MON_ZERO_DET_V */
    bitmsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_INT_VAL_PATT_MON_ZERO_DET_V_MSK;
    bitoffset = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_INT_VAL_PATT_MON_ZERO_DET_V_OFF;
    zeroDetV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out PATT_MON_SYNC_V */
    bitmsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_INT_VAL_PATT_MON_SYNC_V_MSK;
    bitoffset = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_INT_VAL_PATT_MON_SYNC_V_OFF;
    syncV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out PATT_MON_ERR_V */
    bitmsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_INT_VAL_PATT_MON_ERR_V_MSK;
    bitoffset = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_INT_VAL_PATT_MON_ERR_V_OFF;
    errV = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* return results */
    pattMonValue = (zeroDetV << 2) | (syncV << 1) | errV;
    *pStatusV = pattMonValue;

    /* Get PRBS Status bits */
    addr = HYPHY20G_SERDES_BP_TXRXSLICE_REG_INT_STAT;
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(addr, linkId);
    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;

    /* read out ZERO_DET_I */
    bitmsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_INT_STAT_PATT_MON_ZERO_DET_I_MSK;
    bitoffset = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_INT_STAT_PATT_MON_ZERO_DET_I_OFF;
    zeroDetI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out SYNC_I */
    bitmsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_INT_STAT_PATT_MON_SYNC_I_MSK;
    bitoffset = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_INT_STAT_PATT_MON_SYNC_I_OFF;
    syncI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* read out ERR_I */
    bitmsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_INT_STAT_PATT_MON_ERR_I_MSK;
    bitoffset = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_INT_STAT_PATT_MON_ERR_I_OFF;
    errI = mHYPHY20G_GET_FIELD4(reg_val, bitmsk, bitoffset);

    /* return results */
    pattMonStat = (zeroDetI << 2) | (syncI << 1) | errI;
    *pStatusI = pattMonStat;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gBackplaneSerdesPattMonStatus */


/*******************************************************************************
**
**  hyPhy20gBackplaneSerdesPattGenCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Pattern Generator within the
**                  Backplane Serdes subsystem.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - Backplane Interface 0
**                                1 - Backplane Interface 1
**                                ..
**                                31 - Backplane Interface 31
**                  patType     - 0 - PRBS5 Pattern
**                                1 - PRBS7 Pattern
**                                2 - PRBS11 Pattern
**                                3 - PRBS15 Pattern
**                                4 - PRBS23 Pattern
**                                5 - PRBS31 Pattern
**                  invPatt     - 0 - PRBS pattern is not inverted
**                              - 1 - PRBS pattern is inverted
**                  enable      - 0 - PRBS Generator is disabled
**                              - 1 - PRBS Generator is enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gBackplaneSerdesPattGenCfg(struct file *fileHndl, UINT4 linkId,
                                       UINT4 patType, UINT4 invPatt,
                                       UINT4 enable)
{
    INT4 result = 0;
    UINT4 addr;
    UINT4 val = 0;
    UINT4 enMsk, modeMsk, invMsk, rstMsk;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
	
    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (patType > 5) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((invPatt > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    addr = HYPHY20G_SERDES_BP_TXRXSLICE_REG_PAT_GNRTR_CFG;
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(addr, linkId);

    enMsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_PAT_GNRTR_CFG_PATT_GEN_EN_MSK;
    modeMsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_PAT_GNRTR_CFG_PATT_GEN_MODE_MSK;
    invMsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_PAT_GNRTR_CFG_PATT_GEN_INV_MSK;
    rstMsk = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_PAT_GNRTR_CFG_PATT_GEN_RST_MSK;

    /* Disable Pattern Generator before configuring */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* set PATT_GEN_RST = 1 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x1);
    if(result) return result;

    /* set PATT_GEN_EN = 0 */
    result = hyPhy20gVarFieldWrite (&val, enMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* exit function if disabling */
    if(enable == 0){
        return HYPHY20G_SUCCESS;
    }

    /* Configure PAT_GNRTR_CFG register */
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* Configure PATT_GEN_MODE bit */
    result = hyPhy20gVarFieldWrite (&val, modeMsk, patType);
    if(result) return result;

    /* Configure PATT_GEN_INV bit */
    result = hyPhy20gVarFieldWrite (&val, invMsk, invPatt);
    if(result) return result;

    /* set PATT_GEN_RST = 0 */
    result = hyPhy20gVarFieldWrite (&val, rstMsk, 0x0);
    if(result) return result;

    /* write value to the register */
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* write the PATT_GEN_EN bit to 1 seperately */
    /* set PATT_GEN_EN = 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, addr, enMsk, 0x1);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gBackplaneSerdesPattGenCfg */

/*******************************************************************************
**
**  hyPhy20gBackplaneSerdesPattGenErrInj
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function will inject a single error on the Pattern
**                  Generator within the Backplane Serdes subsystem.  The error is
**                  injected by inverting the 8th bit of the next byte to be
**                  sent.
**
**  INPUTS:         *fileHndl   - base address
**                  linkId      - 0 - Backplane Interface 0
**                                1 - Backplane Interface 1
**                                ..
**                                31 - Backplane Interface 31
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gBackplaneSerdesPattGenErrInj(struct file *fileHndl, UINT4 linkId)
{
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
	
    /* argument checking */
    if (linkId > HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;

    /* setup register address and bit offset */
    addr = HYPHY20G_SERDES_BP_TXRXSLICE_REG_PAT_GNRTR_CFG;
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(addr, linkId);
    bitoffset = HYPHY20G_SERDES_BP_TXRXSLICE_BIT_PAT_GNRTR_CFG_PATT_GEN_ERR_INJ_OFF;

    /* bit is not self-clearing so clear before injecting error */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* set ERR_INJ bit to '1' */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gBackplaneSerdesPattGenErrInj */

/*******************************************************************************
**
**  hyPhy20gSfiSerdesSyncStatus
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function checks the synchronization status of the
**                  SFI4-2 SERDES.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - SFI Serdes Slice instance
**                                0 - SFI1
**                                1 - SFI2
**                  intClear    - 1 - all *_I interrupt bits accessed by this
**                                    function will be cleared by writing them
**                                    to '1'
**                              - 0 - all *_I interrupt bits accessed by this
**                                    function will retain their values if
**                                    WCIMODE is used.
**                  *pStatusV  - Pointer to storage for event state indication
**                                Bit 31:2 - Reserved
**                                Bit  1:0 - SFI_SYNC_ERR_V[1:0]
**                  *pStatusI  - Pointer to storage for event indication bits
**                                Bit 31:2 - Reserved
**                                Bit  1:0 - SFI_SYNC_ERR_I[1:0]
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSfiSerdesSyncStatus(struct file *fileHndl, UINT4 slice,
                                 UINT4 intClear, UINT4 *pStatusV,
                                 UINT4 *pStatusI)
{
    UINT4 devId, revId, status;
    UINT4 addr, msk;
    INT4 result;
    
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
	
    /* argument checking */
    if (slice > 1 || intClear > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_PGMRCLK_REG_SYNC_ERR_V;
    msk = HYPHY20G_PGMRCLK_BIT_SYNC_ERR_V_SFI_SYNC_ERR_V_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, pStatusV);
    if (result) return result;

    addr = HYPHY20G_PGMRCLK_REG_SYNC_ERR_I;
    msk = HYPHY20G_PGMRCLK_BIT_SYNC_ERR_I_SFI_SYNC_ERR_I_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, pStatusI);
    if (result) return result;

    if(intClear == 1) {
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, msk);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSfiSerdesSyncStatus */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/* end of file */
