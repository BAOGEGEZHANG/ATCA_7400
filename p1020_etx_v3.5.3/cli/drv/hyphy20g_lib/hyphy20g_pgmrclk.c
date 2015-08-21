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
**  FILE        : hyphy20g_pgmrclk..c
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
#include "hyphy20g_pgmrclk.h"

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
**  hyPhy20gPgmrclkInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function initializes the PGMRCLK subsystem so that it may
**                be further configured with triggering mechanism.
**
**
**  INPUTS:       *fileHndl     - base address
**                 enable       - 0 - PGMRCLK subsystem disabled
**                                1 - PGMRCLK subsystem enabled
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPgmrclkInit(struct file *fileHndl, UINT4 enable)
{
    /* variable declaration */
    UINT4 swRegAddr, swRegOff;
    UINT4 pwrRegAddr, pwrRegOff;
    INT4 result;
    
    /* argument checking */
    if(enable > 1){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    swRegAddr = HYPHY20G_TOP_REG_SYS_SW_RST_3;
    swRegOff = HYPHY20G_TOP_BIT_SYS_SW_RST_3_PGMRCLK_RST_OFF;
    pwrRegAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_3;
    pwrRegOff = HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_PGMRCLK_LOWPWR_OFF;
    
    /* enable operations */
    if (enable==1){
    /* reset top level subsystem and place into low power mode */
        result = sysHyPhy20gBitWrite(fileHndl, swRegAddr, swRegOff, 1); 
        if(result) return result;           
        result = sysHyPhy20gBitWrite(fileHndl, pwrRegAddr, pwrRegOff, 1); 
        if(result) return result;    

        result = sysHyPhy20gBitWrite(fileHndl, swRegAddr, swRegOff, 0); 
        if(result) return result;
        result = sysHyPhy20gBitWrite(fileHndl, pwrRegAddr, pwrRegOff, 0); 
        if(result) return result;      
    } else {
        /* disable operations */
        result = sysHyPhy20gBitWrite(fileHndl, swRegAddr, swRegOff, 1); 
        if(result) return result;           
        result = sysHyPhy20gBitWrite(fileHndl, pwrRegAddr, pwrRegOff, 1); 
        if(result) return result;           
    }         
                                  
    return HYPHY20G_SUCCESS;    
 
} /* hyPhy20gPgmrclkInit */


/*******************************************************************************
**
**  hyPhy20gPgmrclkTrigTypeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the device level PGMRCLK subsystem
**                counter triggering mechanism.
**
**                Automatic triggering applies to all counter types specified
**                for this mode.  hyPhy20gPgmrclkAutoTrigCfg must be called
**                to configure the specific mode of automatic counter 
**                triggering.
**
**                PGMRCLK register space triggering applies to one or more
**                counter accumulators.  hyPhy20gPgmrclkManTrig may be called
**                to initiate counter accumulation for one or more subsystems
**                configured for this mode of operation.
**
**
**  INPUTS:       *fileHndl     - base address
**                 counter      - Subsystem counter accumulation mode to 
**                                configure
**                                31 - 9 RESERVED
**                                8 GFP1
**                                7 GFP0
**                                6 SONET
**                                5 VCAT
**                                4 OTN
**                                3 ENET
**                                2 ESSI
**                                1 PIF
**                                0 ODUKPP
**                                Selecting RESERVED values returns 
**                                HYPHY20G_ERR_INVALID_ARG
**                                Bit 3 ENET should be set to 1 if FW
**                                is operating on LSCI/HSCI subsystems.
**                 mode         - Counter accumulation mode:
**                                0 - automatic triggering (including device
**                                    PMON_SYNC_I pin triggering)
**                                1 - PGMRCLK register space triggering
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gPgmrclkTrigTypeCfg(struct file *fileHndl, UINT4 counter, 
                                UINT4 mode)
{
    /* variable declaration */
    UINT4 addr;
    UINT4 off;
    INT4  result;

    /* Argument check */
    if ((counter > 8) || (mode > 1))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine register addresses */
    addr = HYPHY20G_PGMRCLK_REG_LCLK_CTL;
    
    /* Determine bit offset to configure */
    off = counter + HYPHY20G_PGMRCLK_BIT_LCLK_CTL_LCLK_SUBSYS_CTRL_OFF;
    
    /* configure the LCLK_SUBSYSTEM_CTRL bit for the requested resource */
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, mode); 
    if (result) return result;       

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPgmrclkTrigTypeCfg */

/*******************************************************************************
**
**  hyPhy20gPgmrclkAutoTrigCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the automatic counter trigger
**                accumulation mechanism.  This mechanism allows for device pin
**                PMON_SYNC_I to align the counter mechanism or for PMON_SYNC_I
**                to override the automatic counter accumulation mechanism.
**
**                PMON_SYNC_I can override the automatic counter accumulation
**                mechanism when the automatic accumulation interval is much
**                greater than the period HW will assert the PMON_SYNC_I pin.
**                In such cases, the internal counter will reset each time a
**                PMON_SYNC_I rising edge is detected.
**
**                The function hyPhy20gPgmrclkAutoTrigSwResync may be used to 
**                resynchronize the counter interval.
**
**                The PMON_SYNC_I masking window control argument pinWindow is
**                used to ensure that no two PMON_SYNC_I rising edges are
**                detected within close proximity.  This will help improve 
**                PMON_SYNC_I signal quality and avoid aliasing of counter
**                events should a double pulse edge be detected.
**
**
**  INPUTS:       *fileHndl     - base address
**                 interval     - Automatic Counter Accumulation Interval with
**                                units of 125 usecond increments
**                                Legal range 0x5 to 0x3FFF
**                                Default value 8000 (corresponding to 1 second)
**                 pinCtrl      - Control the influence of PMON_SYNC_I input pin
**                                1 - Enabled, PMON_SYNC_I processed
**                                0 - Disabled, PMON_SYNC_I not processed
**                 pinWindow    - Controls width of PMON_SYNC_I masking window.
**                                The window size is in units of 311 MHz clock
**                                cycles and is relative to the mask window 
**                                center.
**                                0  - +(1+0)/-(1+3) clock cycle (special case)
**                                1  - +(12+0)/-(12+3) clock cycle
**                                2 to 14 
**                                   - +(12*n+0)/-(12*n+3) (where n = 2 - 14) 
**                                     clock cycle
**                                15 - +(12*15+0)/-(12*15+3) clock cycle
**                                Only valid when pinCtrl set to 1
**                 windowCtrl   - Response to PMON_SYNC_I sample point relative
**                                to expected receive location:
**                                1 - always realign masking window to 
**                                    PMON_SYNC_I event.
**                                0 - do not realign to PMON_SYNC_I event
**                                    provided it occurs within the masking
**                                    window.
**                                Only valid when pinCtrl set to 1
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPgmrclkAutoTrigCfg(struct file *fileHndl, UINT4 interval,  
                                UINT4 pinCtrl, UINT4 pinWindow, 
                                UINT4 windowCtrl)
{
    /* variable declaration */
    UINT4 addr, val, msk;
    UINT4 synciMsk, cntrEnMsk, maskWdwMsk;
    INT4  result;
    
    /* Argument check */
    if ((interval < 5) || (interval > 16383) || (pinCtrl > 1) || 
        (pinWindow > 15) || (windowCtrl > 1))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Configure SYNC_INTRVAL */
    addr = HYPHY20G_PGMRCLK_REG_SYNC_CIRCUIT_INTRVL_CTL;
    msk = HYPHY20G_PGMRCLK_BIT_SYNC_CIRCUIT_INTRVL_CTL_SYNC_INTVAL_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, interval);
    if (result) return result;   
    
    /* Configure SYNCI_DIS, CNTR_ENABLE, MASK_WDW */
    addr = HYPHY20G_PGMRCLK_REG_IN_LCLK_SYNC_CTL;
    
    synciMsk = HYPHY20G_PGMRCLK_BIT_IN_LCLK_SYNC_CTL_SYNCI_DIS_MSK;
    cntrEnMsk = HYPHY20G_PGMRCLK_BIT_IN_LCLK_SYNC_CTL_CNTR_ENABLE_MSK;
    maskWdwMsk = HYPHY20G_PGMRCLK_BIT_IN_LCLK_SYNC_CTL_MASK_WDW_MSK;
    
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, synciMsk, 1-pinCtrl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, cntrEnMsk, windowCtrl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, maskWdwMsk, pinWindow);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;    

        
    return HYPHY20G_SUCCESS;

} /* hyPhy20gPgmrclkAutoTrigCfg */


/*******************************************************************************
**
**  hyPhy20gPgmrclkAutoTrigEnbl
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function enables or disables the automatic counter
**                accumulation mechanism.
**
**                This function should be called after the automatic accumuation
**                mechanism has been configured with hyPhy20gPgmrclkAutoTrigCfg.
**
**  INPUTS:       *fileHndl     - base address
**                 enable       - Enabling of automatic counter accumulation
**                                1 - Enabled
**                                0 - Disabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPgmrclkAutoTrigEnbl(struct file *fileHndl, UINT4 enable)
{
    /* variable declaration */
    UINT4 addr, off;
    INT4 result;
    
    /* Argument check */
    if ( enable >1 )
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Determine register addresses */
    addr = HYPHY20G_PGMRCLK_REG_SYS_LCLK_CTL;
    /* Determine bit offset to configure */ 
    off = HYPHY20G_PGMRCLK_BIT_SYS_LCLK_CTL_AUTO_LCLKOUT_EN_OFF;
    
    /* configure the AUTO_LCLKOUT_EN bit for the requested resource */
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, enable);
    if(result) return result;
        
    return HYPHY20G_SUCCESS;
                                 
} /* hyPhy20gPgmrclkAutoTrigEnbl */

/*******************************************************************************
**
**  hyPhy20gPgmrclkAutoTrigSwResync
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function forces a resynchronization of the automatic
**                counter accumulation interval via registers.
**
**                Counter data extracted for the accumulation event immediately
**                following this call may be corrupted.
**
**
**  INPUTS:       *fileHndl     - base address
**                 
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPgmrclkAutoTrigSwResync(struct file *fileHndl)
{
    /* variable declaration */
    INT4 result;
    UINT4 addr, off;
    
    /* Determine register addresses and bit offset to configure */
    addr=HYPHY20G_PGMRCLK_REG_SYNC_CIRCUIT_INTRVL_CTL;
    off=HYPHY20G_PGMRCLK_BIT_SYNC_CIRCUIT_INTRVL_CTL_INTERVAL_RST_OFF;

    /* configure the INTERVAL_RST bit */
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, 1);        
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPgmrclkAutoTrigSwResync */

/*******************************************************************************
**
**  hyPhy20gPgmrclkManTrig
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function initiates PGMRCLK statistic counter accumulation
**                using the device register triggering mechanism.  It will
**                initiate counter accumulation for the PGMRCLK accumulators
**                specified by a user mask.
**
**                The function will poll the TIP bits for each accumulator
**                specified by the user mask.   An error will be returned
**                if a TIP bit does not clear.
**
**                Register triggering mechanism is only valid for accumulators 
**                that have been enabled for such using 
**                hyPhy20gPgmrclkTrigTypeCfg.
**
**  INPUTS:       *fileHndl     - base address
**                 msk          - Mask of accumulators to initiate
**                                Bit set to 0 - do not trigger accumulation
**                                Bit set to 1 - trigger accumulation
**                                31 - 9 RESERVED
**                                8 GFP1
**                                7 GFP0
**                                6 SONET
**                                5 VCAT
**                                4 OTN
**                                3 ENET
**                                2 ESSI
**                                1 PIF
**                                0 ODUKPP
**                                RESERVED bits are not processed they, are 
**                                interpreted as being set to 0.
**                                Bit 3 ENET should not be triggered if FW
**                                is operating on LSCI/HSCI subsystems.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_TIP_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gPgmrclkManTrig(struct file *fileHndl, UINT4 msk)
{
    /* variable declaration */
    UINT4 addr, plsgenMsk;
    UINT4 tipCsdBits, val;
    UINT4 iterations;
    INT4  result;
    
    /* Argument check */
    if ( msk > 0x1FF )
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

     /* Trigger counter accumulation with LCLK_SUBSYS_PLSGEN */
    addr = HYPHY20G_PGMRCLK_REG_LCLK_PLS_GEN;
    plsgenMsk = HYPHY20G_PGMRCLK_BIT_LCLK_PLS_GEN_LCLK_SUBSYS_PLSGEN_MSK;
    
    result = hyPhy20gRegFieldWrite(fileHndl, addr, plsgenMsk, msk);
    if (result) return result;
    
    /* Monitor TIP_CSD to confirm counter accumulation has completed */

    /* Determine TIP_CSD bits that need to be monitored based on 
       subsystems requested to be transferred */
    tipCsdBits = 0; 
    /* ESSI case */
    if ( msk & 0x4 ){
        /* monitor TIP_CSD
           bit 10 - ESSI
           bit  9 - DMI */
        tipCsdBits |= 0x600;
    }
    /* ENET case */
    if ( msk & 0x8 ){
        /* monitor TIP_CSD
           bit  5 - Ethernet */
        tipCsdBits |= 0x20;
    }
    /* VCAT case */
    if ( msk & 0x20){
        /* monitor TIP_CSD
           bit 0 - VCAT */
        tipCsdBits |= 0x1;
    }
    /* SONET case */
    if ( msk & 0x40){
        /* monitor TIP_CSD
           bit 8 - SONET Framers
           bit 7 - SONET HOPP */
        tipCsdBits |= 0x180;
    }
    /* GFP 0 case */
    if ( msk & 0x80){
        /* monitor TIP_CSD
           bit 2 - GFP0_FRM 
           bit 1 - GFP0_TRA */
        tipCsdBits |= 0x6;
    }
    /* GFP 1 case */
    if ( msk & 0x100){
        /* monitor TIP_CSD
           bit 4 - GFP1_FRM 
           bit 3 - GFP1_TRA */
        tipCsdBits |= 0x18;
    }
    
    /* Determine TIP_CSD bits have completed counter transfer. */
    addr = HYPHY20G_PGMRCLK_REG_TIP_CONSOLIDATION;

    /* Poll TIP_CSD bits for the requested subsystems till they are clear
       and return success otherwise if 1 second exceeded return error */
    iterations = 0;
    while(1) {
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if(result) return result;

        if ((val&tipCsdBits) == 0) {
            return HYPHY20G_SUCCESS;
        }
        
        /* fail if TIP bits not cleared within one second */
        if (iterations == 1000000) {
            return HYPHY20G_ERR_TIP_TIMEOUT;
        }
        
        /* delay 1 us before next evaluation */
        result = sysHyPhy20gUsDelay(1);
        if (result) return result;   
        iterations++; 
    }

} /* hyPhy20gPgmrclkManTrig */



/*******************************************************************************
**
**  hyPhy20gPgmrclkClkSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the timing source for one of the four
**                PGMRCLK[3:0] output signals.
**
**                The function also configures the clock divider that can reduce
**                the rate of PGMRCLK[3:0] signal.
**
**                A PGRMCLK[3:0] signal cannot exceed 125 MHz.  For sources of 0 
**                to 18, 28-29, and 39 to 75 rate processed by the PGMRCLK is  
**                the resource rate divided by the SERDES bus width.  
**                The divider can be used to reduce this rate so that it does  
**                not exceed 125 MHz.
**
**                Refer to HW Spec Table 'PGMRCLK Output Rates' for more detail
**                on the rate generated on PGMRCLK[3:0] signal for a specific
**                source.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - PGRMCLK[3:0] pin to be configured
**                                0 to 3
**                 source       - Rate source for PGMRCLK[slice]
**                                0      : XFI 0 RCLK
**                                1      : XFI 1 RCLK
**                                2 - 17 : SFP 0 - SFP 15 RCLK
**                                18     : SGMII RCLK
**                                19     : PTP DCSU
**                                20     : QDR DCSU
**                                21     : DDR DCSU
**                                22     : OTN DCSU
**                                23     : SDH DCSU
**                                24     : REFCLK A
**                                25     : REFCLK B
**                                28     : SFI 1 RCLK
**                                29     : SFI 2 RCLK
**                                39 - 54: Recovered clocks for
**                                         SONET framer 0 - SONET framer 15  
**                                55 - 70: SFP 0 - SFP 15 TCLK
**                                71     : SGMII TCLK
**                                72     : XFI 0 TCLK
**                                73     : XFI 1 TCLK
**                                74     : SFI 1 TCLK
**                                75     : SFI 2 TCLK
**                                All other values will set CONFIGURE_DIVIDER 
**                                to "0x0" and Reutrn HYPHY20G_ERR_INVALID_ARG
**                 divider      - Integer divider to reduce PGMRCLK[slice] rate
**                                so that it does not exceed 125 MHz.
**                                0 - divide by 1
**                                1 - divide by 2
**                                2 - divide by 4
**                                3 - divide by 8
**                                4 - divide by 16
**                                5 - divide by 32
**                                6 - divide by 64
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPgmrclkClkSrcCfg(struct file *fileHndl, UINT4 slice, UINT4 source, 
                              UINT4 divider)
{
    /* variable declaration */
    UINT4 addr, msk, div;
    INT4  result;
    
    /* Argument check */
    if ((slice > 3) || (source == 26 || source == 27) || 
        (source > 29 && source < 39 ) || (source > 75) || (divider > 6))
    {   
        addr = HYPHY20G_PGMRCLK_REG_CONFIGURE_DIVIDER;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, 0xFFFF, 0);    
        if(result) return result;
        
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Configure CLK_SEL */
    addr = HYPHY20G_PGMRCLK_REG_CLK_SEL;  

    switch (slice) {
        case 0: /* slice 0 */
            msk = 0x000000FF;
            break;
        case 1: /* slice 1 */
            msk = 0x0000FF00;
            break;
        case 2: /* slice 2 */
            msk = 0x00FF0000;
            break;
        case 3: /* slice 3 */
            msk = 0xFF000000;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, source);    
    if(result) return result;

    /* Configure DIV_BY */ 
    addr = HYPHY20G_PGMRCLK_REG_CONFIGURE_DIVIDER;   

    switch (slice) {
        case 0: /* slice 0 */
            msk = 0x000F;
            break;
        case 1: /* slice 1 */
            msk = 0x00F0;
            break;
        case 2: /* slice 2 */
            msk = 0x0F00;
            break;
        case 3: /* slice 3 */
            msk = 0xF000;
            break;
        
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }
    
    switch (divider) {
        case 0: /* div by 1 */
            div = 0;
            break;            
        case 1: /* div by 2 */
            div = 0x8;
            break;
        case 2: /* div by 4 */
            div = 0x9;
            break;       
        case 3: /* div by 8 */
            div = 0xA;
            break;
        case 4: /* div by 16 */
            div = 0xB;
            break;
        case 5: /* div by 32 */
            div = 0xC;
            break;
        case 6: /* div by 64 */
            div = 0xD;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }   
    
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, div);    
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gPgmrclkClkSrcCfg */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
