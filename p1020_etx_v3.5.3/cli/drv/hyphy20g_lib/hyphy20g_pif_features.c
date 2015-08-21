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
**  FILE        : hyphy20g_pif_features.c
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
#include "hyphy20g_pif_features.h"

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
**  hyPhy20gPifIntlknSerdesTestCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the PIF Interlaken Serdes test 
**                generator or detector.  This feature allows one of 6 PRBS 
**                signals or a fixed programmable pattern to be generated or 
**                detected.  
**
**                For programmable pattern generation, up to 3 - 32 bit patterns  
**                may be generated or detected.  The programmable pattern is  
**                formed as follows:
**
**                Pattern A * Repetition A, followed by Pattern B * Repetition B 
**                followed by Pattern C * Repetition C.
**
**                When Repetion N(A, B or C) is set to 0, Pattern N is not
**                included in the expected pattern stream.
**
**                This function must be called when the PIF instance is in 
**                Major Bidirection State == 1.  It takes effect when the PIF
**                instance is transitioned to Major Bidirection State == 2.
**
**
**  INPUTS:       *fileHndl     - base address
**                 dir          - Direction of travel associated with counters:
**                                0 - Transmit PRBS Generator
**                                1 - Receive PRBS Detector
**                 mode         - Test pattern generation mode
**                                0 - Disabled
**                                1 - PRBS
**                                2 - Programmable Pattern
**                 prbs         - For mode == 1, PRBS type:
**                                0 - PRBS31
**                                1 - PRBS23
**                                2 - PRBS7
**                                3 - PRBS9
**                                4 - PRBS11
**                                5 - PRBS15
**                                6 - PRBS29
**                 patternA     - For mode == 2 32 bit pattern to be repeated  
**                                repA times
**                                0 - 6 (refer to prbs argument for definition)
**                 patternB     - For mode == 2 32 bit pattern to be repeated  
**                                repB times
**                                0 - 6 (refer to prbs argument for definition)
**                 patternC     - For mode == 2 32 bit pattern to be repeated  
**                                repC times
**                                0 - 6 (refer to prbs argument for definition)
**                 repA         - For mode == 2 number of time to repeat 
**                                patternA.  If not to be repeated set to 0.
**                                0 - 0xFF
**                 repB         - For mode == 2 number of time to repeat 
**                                patternB.  If not to be repeated set to 0.
**                                0 - 0xFF
**                 repC         - For mode == 2 number of time to repeat 
**                                patternC.  If not to be repeated set to 0.
**                                0 - 0xFF

**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifIntlknSerdesTestCfg(struct file *fileHndl, UINT4 dir, 
                                    UINT4 mode, UINT4 prbs, UINT4 patternA, 
                                    UINT4 patternB, UINT4 patternC, UINT4 repA, 
                                    UINT4 repB, UINT4 repC)
{

    UINT4 regData;
    UINT4 prbsVal;
    UINT4 progEnVal;
    UINT4 prbsEnVal;

    UINT4 ctlReg = HYPHY20G_PIF_INTLKN_CORE_REG_SERDES_TST_CTL_OFFSET(dir);
    UINT4 repCMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_SERDES_TST_CTL_REPC_MSK;
    UINT4 repBMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_SERDES_TST_CTL_REPB_MSK;
    UINT4 repAMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_SERDES_TST_CTL_REPA_MSK;
    UINT4 progEnMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_SERDES_TST_CTL_PROG_EN_MSK;
    UINT4 prbsEnMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_SERDES_TST_CTL_PRBS_EN_MSK;

    UINT4 patAReg = HYPHY20G_PIF_INTLKN_CORE_REG_SERDES_TST_PAT_OFFSET(dir, 0);
    UINT4 patBReg = HYPHY20G_PIF_INTLKN_CORE_REG_SERDES_TST_PAT_OFFSET(dir, 1);
    UINT4 patCReg = HYPHY20G_PIF_INTLKN_CORE_REG_SERDES_TST_PAT_OFFSET(dir, 2);
    UINT4 patMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_SERDES_TST_PAT_TST_PTRN_VALUE_MSK;

    INT4 result;

    /* argument checking */
    /* Note patternA, patternB, patternC not checked in args as legal range
       full range of UINT4 */
    if (dir > 1 || mode > 2 || prbs > 6 || repA > 0xFF || 
        repB > 0xFF || repC > 0xFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* read SERDES TEST CONTROL register */
    result = sysHyPhy20gRead(fileHndl, ctlReg, &regData);
    if(result) return result;

    if(mode == 2){
        /* set REPA, REPB and REPC */
        result = hyPhy20gVarFieldWrite (&regData, repAMsk, repA);
        if(result) return result;   

        result = hyPhy20gVarFieldWrite (&regData, repBMsk, repB);
        if(result) return result; 

        result = hyPhy20gVarFieldWrite (&regData, repCMsk, repC);
        if(result) return result; 

        /* set TST_PTRN_VALUE for Pattern A, B and C */
        result = hyPhy20gRegFieldWrite(fileHndl, patAReg, patMsk, patternA);
        if(result) return result;

        result = hyPhy20gRegFieldWrite(fileHndl, patBReg, patMsk, patternB);
        if(result) return result;

        result = hyPhy20gRegFieldWrite(fileHndl, patCReg, patMsk, patternC);
        if(result) return result;
    } else if(mode == 1) {
        /* set TST_PTRN_VALUE for PRBS pattern */
        switch (prbs) {
            case 0: /* 0 - PRBS31 */           
                prbsVal = 0x90000001;
                break;            
            case 1: /* 1 - PRBS23 */ 
                prbsVal = 0x00840001; 
                break;  
            case 2: /* 2 - PRBS7 */
                prbsVal = 0x000000C1; 
                break;  
            case 3: /* 3 - PRBS9 */             
                prbsVal = 0x00000221; 
                break;            
            case 4: /* 4 - PRBS11 */ 
                prbsVal = 0x00000A01; 
                break;  
            case 5: /* 5 - PRBS15 */
                prbsVal = 0x0000C001; 
                break;  
            case 6: /* 6 - PRBS29 */
                prbsVal = 0x28000001; 
                break; 
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }

        result = hyPhy20gRegFieldWrite(fileHndl, patAReg, patMsk, prbsVal);
        if(result) return result;
    }

    switch (mode) {
        case 0: /* 0 - Disabled */            
            progEnVal = 0;
            prbsEnVal = 0; 
            break;            
        case 1: /* 1 - PRBS */ 
            progEnVal = 0;
            prbsEnVal = 1; 
            break;  
        case 2: /* 2 - Programmable Pattern */
            progEnVal = 1;
            prbsEnVal = 0; 
            break;  
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Set PRBS_EN and PROG_EN fields */
    result = hyPhy20gVarFieldWrite (&regData, progEnMsk, progEnVal);
    if(result) return result; 

    result = hyPhy20gVarFieldWrite (&regData, prbsEnMsk, prbsEnVal);
    if(result) return result; 

    /* write SERDES TEST CONTROL register */
    result = sysHyPhy20gWrite(fileHndl, ctlReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPifIntlknSerdesTestCfg */

/*******************************************************************************
**                                                            
**  hyPhy20gPifIntlknCoreDiagLpbk
**  ___________________________________________________________________________ 
**                                                            
**  DESCRIPTION:    This function enable or disable PIF Interlaken Core 
**                  diagnostic loopback.  The loopback is diagnostic relative to
**                  the CPB.  Data from Tx is looped back to Rx.
**
**                  The block must be in in 'Major Bidirectional State' of 2 in
**                  order for fields accessed by this API to be modified.
**
**                  Use this API with hyPhy20gPifIntlknEnable as follows:
**                  1. call hyPhy20gPifIntlknEnable with enable set to 0
**                  2. call hyPhy20gPifIntlknCoreDiagLpbk with desired loopback 
**                     setting
**                  3. call hyPhy20gPifIntlknEnable with enable set to 1
**                                                            
**                                                            
**  INPUTS:       *fileHndl     - base address                
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
INT4 hyPhy20gPifIntlknCoreDiagLpbk(struct file *fileHndl, UINT4 enable)
{
    /* variable declaration */
    UINT4 regAddr;
    UINT4 txRegAddr;
    UINT4 rxRegAddr;
    UINT4 msk;
    UINT4 dataLbMsk;
    INT4 result;
    
    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Configure transmit control register for loopback feature 
       at 0x495E018 TRANSMIT CONTROL: TX_CTRL */
    regAddr = HYPHY20G_PIF_INTLKN_CORE_REG_TX_CTL;
    msk = HYPHY20G_PIF_INTLKN_CORE_BIT_TX_CTL_TX_DATA_OUT_KILL_MSK;
    
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, enable);
    if (result) return result;
        
    /* Enable or disable loopback feature by configuring 
       0x495E010 + 0x0200 * (m) CONFIGURATION: CFG */
    txRegAddr = HYPHY20G_PIF_INTLKN_CORE_REG_CFG_OFFSET(0);
    rxRegAddr = HYPHY20G_PIF_INTLKN_CORE_REG_CFG_OFFSET(1);
    
    dataLbMsk = HYPHY20G_PIF_INTLKN_CORE_BIT_CFG_DATA_TX_TO_RX_LB_MSK;
        
    result = hyPhy20gRegFieldWrite(fileHndl, txRegAddr, dataLbMsk, enable);
    if (result) return result;
        
    result = hyPhy20gRegFieldWrite(fileHndl, rxRegAddr, dataLbMsk, enable);
    if (result) return result;
    
    /* Enable Core Internal Loopback at Shim 0x495E408 */
    regAddr = HYPHY20G_PIF_INTLKN_SHIM_REG_CORE_LP_BACK;
    msk = HYPHY20G_PIF_INTLKN_SHIM_BIT_CORE_LP_BACK_INTERNAL_LOOP_BACK_MSK;
    
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, enable);
    if (result) return result;
       
    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifIntlknCoreDiagLpbk */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
