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
**  FILE        : hyphy20g_dcsu.c
**
**  $Date: 2012-03-19 14:09:18 +0800 (Mon, 19 Mar 2012) $
**
**  $Revision: 21071 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include    "hyphy20g_dcsu.h"

/** external functions **/

/** external data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 dcsuHyPhy20gOtnDcsuInit(struct file *fileHndl);
INT4 dcsuHyPhy20gPtpDcsuInit(struct file *fileHndl);
INT4 dcsuHyPhy20gDdrDcsuInit(struct file *fileHndl);
INT4 dcsuHyPhy20gQdrDcsuInit(struct file *fileHndl);
INT4 dcsuHyPhy20gSdhDcsuInit(struct file *fileHndl);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gDcsuInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    DSCU initialization. Prior to configuring any DCSU block,
**                  the device power supplies and REFCLK input signal must
**                  be stable.
**
**                  After the device comes out of hardware reset, each DCSU
**                  must only be initialized once.
**
**  INPUTS:         *fileHndl - base address
**                  block     - 0 - DDR DCSU
**                              1 - PTP DCSU
**                              2 - OTN DCSU
**                              3 - QDR DCSU
**                              4 - SDH DCSU
**
**  NOTE:           For META 20G devices, the value of block as below:
**                  block     - 0 - GFP DCSU
**                              1 - PTP DCSU
**                              2 - OTN DCSU
**                              3 - CPB DCSU
**                              4 - SDH DCSU
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gDcsuInit(struct file *fileHndl, UINT4 block)
{
    INT4 result;
    
    /* Check input args */
    if(block > 4) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    switch (block) {
        case 0:
            /* DDR DCSU */
            result = dcsuHyPhy20gDdrDcsuInit(fileHndl);
            if (result) return result;
            break;
        case 1:
            /* PTP DCSU */
            result = dcsuHyPhy20gPtpDcsuInit(fileHndl);
            if (result) return result;
            break;
        case 2:
            /* OTN DCSU */
            result = dcsuHyPhy20gOtnDcsuInit(fileHndl);
            if (result) return result;
            break;
        case 3:
            /* QDR DCSU */
            result = dcsuHyPhy20gQdrDcsuInit(fileHndl);
            if (result) return result;
            break;
        case 4:
            /* SDH DCSU */
            result = dcsuHyPhy20gSdhDcsuInit(fileHndl);
            if (result) return result;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;
        
} /* hyPhy20gDcsuInit */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/*******************************************************************************
**
**  dcsuHyPhy20gSdhDcsuInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    SDH DSCU initialization. Prior to configuring any DCSU
**                  block, the device power supplies and REFCLK input signal
**                  must be stable.
**
**  INPUTS:         *fileHndl - base address
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 dcsuHyPhy20gSdhDcsuInit(struct file *fileHndl)
{
    UINT4 preDivBy;
    UINT4 extFbClkEn, extFbModSel;
    UINT4 fbIntParam, mIn, fbIntFracB;
    UINT4 extFbIntDiv, extFbNum, extFbDen;
    UINT4 postDivBy;
    UINT4 addr, val, msk;
    INT4 result;
    
    /* Configuration values for each block */
    preDivBy = 0x9;
    /* SDH DCSU */
    extFbClkEn  = 0x0;
    extFbModSel = 0x1;
    fbIntParam  = 0x27; /* 39 */
    mIn         = 0x0;
    fbIntFracB  = 0x1;
    extFbIntDiv = 0x27; /* 39 */
    extFbNum    = 0x0;
    extFbDen    = 0xFF; /* 255 */
    postDivBy   = 0x100;
    
    /* (1) Wait 10 us */
    result = sysHyPhy20gUsDelay(10);
    if (result) return result;
    
    
    /* (2) Configure bits */
    /* Configure PRE_DIV_BY */
    addr = HYPHY20G_DCSU_SDH_REG_REF_CLK_A_OR_B_SEL;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, preDivBy, 
                       HYPHY20G_DCSU_SDH_BIT_REF_CLK_A_OR_B_SEL_PRE_DIV_BY_MSK,
                       HYPHY20G_DCSU_SDH_BIT_REF_CLK_A_OR_B_SEL_PRE_DIV_BY_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    
    /* Configure EXT_FBCLK_EN, EXT_FB_MOD_SEL and EXT_FB_INT_DIV */
    addr = HYPHY20G_DCSU_SDH_REG_EXT_FB_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbClkEn, 
                             HYPHY20G_DCSU_SDH_BIT_EXT_FB_CFG_EXT_FBCLK_EN_MSK,
                             HYPHY20G_DCSU_SDH_BIT_EXT_FB_CFG_EXT_FBCLK_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, extFbModSel,
                           HYPHY20G_DCSU_SDH_BIT_EXT_FB_CFG_EXT_FB_MOD_SEL_MSK,
                           HYPHY20G_DCSU_SDH_BIT_EXT_FB_CFG_EXT_FB_MOD_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, extFbIntDiv,
                           HYPHY20G_DCSU_SDH_BIT_EXT_FB_CFG_EXT_FB_INT_DIV_MSK,
                           HYPHY20G_DCSU_SDH_BIT_EXT_FB_CFG_EXT_FB_INT_DIV_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* FB_INTPARAM and FB_INT_FRACB */
    addr = HYPHY20G_DCSU_SDH_REG_INT_PAR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, fbIntParam, 
                          HYPHY20G_DCSU_SDH_BIT_INT_PAR_FB_INTPARAM_MSK,
                          HYPHY20G_DCSU_SDH_BIT_INT_PAR_FB_INTPARAM_OFF);
    val = mHYPHY20G_SET_FIELD4(val, fbIntFracB,
                         HYPHY20G_DCSU_SDH_BIT_INT_PAR_FB_INT_FRACB_MSK,
                         HYPHY20G_DCSU_SDH_BIT_INT_PAR_FB_INT_FRACB_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* M_IN */
    addr = HYPHY20G_DCSU_SDH_REG_MODULATION_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, mIn, 
                               HYPHY20G_DCSU_SDH_BIT_MODULATION_CFG_M_IN_MSK,
                               HYPHY20G_DCSU_SDH_BIT_MODULATION_CFG_M_IN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
  
    /* EXT_FB_NUMERATOR */
    addr = HYPHY20G_DCSU_SDH_REG_EXT_FB_NUMR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbNum,
                        HYPHY20G_DCSU_SDH_BIT_EXT_FB_NUMR_EXT_FB_NUMERATOR_MSK, 
                        HYPHY20G_DCSU_SDH_BIT_EXT_FB_NUMR_EXT_FB_NUMERATOR_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    
    /* EXT_FB_DENOMINATOR */
    addr = HYPHY20G_DCSU_SDH_REG_EXT_FB_DENM;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbDen,
                      HYPHY20G_DCSU_SDH_BIT_EXT_FB_DENM_EXT_FB_DENOMINATOR_MSK, 
                      HYPHY20G_DCSU_SDH_BIT_EXT_FB_DENM_EXT_FB_DENOMINATOR_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    
    /* POST_DIV_BY[7:0], POST_DIV_BY[15:8] */
    addr = HYPHY20G_DCSU_SDH_REG_PT_DIVIDER_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, postDivBy, 
                           HYPHY20G_DCSU_SDH_BIT_PT_DIVIDER_CFG_POSTDIV_BY_MSK, 
                           HYPHY20G_DCSU_SDH_BIT_PT_DIVIDER_CFG_POSTDIV_BY_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* (3) Wait 50 ms */
    result = sysHyPhy20gMsDelay(50);    
    if (result) return result;

    /* (4) Set DIV2_EN to 1 */
    addr = HYPHY20G_DCSU_SDH_REG_DCO_CLK_DIV_EN;
    msk = HYPHY20G_DCSU_SDH_BIT_DCO_CLK_DIV_EN_DIV2_EN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if(result) return result;

    /* (5) Set ARSTB to 1 */
    addr = HYPHY20G_DCSU_SDH_REG_DCO_CTL;
    msk = HYPHY20G_DCSU_SDH_BIT_DCO_CTL_ARSTB_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if(result) return result;

  
    /* (6) Wait 2 ms */
    result = sysHyPhy20gMsDelay(2);
    if (result) return result;
    
    
    /* (7) Toggle the POST_UPDATE_EN bit to transition from 0 to 1 to 0 */
    addr = HYPHY20G_DCSU_SDH_REG_PT_DIVIDER_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 1, 
                       HYPHY20G_DCSU_SDH_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_MSK, 
                       HYPHY20G_DCSU_SDH_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 0, 
                       HYPHY20G_DCSU_SDH_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_MSK, 
                       HYPHY20G_DCSU_SDH_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    
    /* (8) Poll LOCK_V bit for the respective DCSU to ensure lock has occurred.
           If lock has not occurred a HW fault can be declared as an external
           condition for DCSU initialization has not been met. */
    addr = HYPHY20G_DCSU_SDH_REG_INT_VAL;
    result = sysHyPhy20gPollBit(fileHndl, addr, 
                                HYPHY20G_DCSU_SDH_BIT_INT_VAL_LOCK_V_MSK,
                                HYPHY20G_DCSU_SDH_BIT_INT_VAL_LOCK_V_MSK, 1);
    if (result) return result;
    
    
    /* (9) Set the CLK_BYPASS[1:0] bits to 0 to enable the generated DCSU clock 
           to propagate to downstream blocks. */
    addr = HYPHY20G_DCSU_SDH_REG_CLK_BYP_EN;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 0,
                            HYPHY20G_DCSU_SDH_BIT_CLK_BYP_EN_CLK_BYPASS_EN_MSK, 
                            HYPHY20G_DCSU_SDH_BIT_CLK_BYP_EN_CLK_BYPASS_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
        
    return HYPHY20G_SUCCESS;
        
} /* dcsuHyPhy20gSdhDcsuInit */

/*******************************************************************************
**
**  dcsuHyPhy20gDdrDcsuInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    DDR DSCU initialization. Prior to configuring any DCSU
**                  block, the device power supplies and REFCLK input signal
**                  must be stable.
**
**  INPUTS:         *fileHndl - base address
**
**  NOTE:           For META 20G devices, this function is used for GFP DCSU.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 dcsuHyPhy20gDdrDcsuInit(struct file *fileHndl)
{
    UINT4 preDivBy;
    UINT4 extFbClkEn, extFbModSel;
    UINT4 fbIntParam, mIn, fbIntFracB;
    UINT4 extFbIntDiv, extFbNum, extFbDen;
    UINT4 postDivBy;
    UINT4 addr, val, msk;
    INT4 result;
    
    /* Configuration values for each block */
    preDivBy = 0x9;
    /* DDR DCSU */
    extFbClkEn  = 0x0;
    extFbModSel = 0x1;
    fbIntParam  = 0;
    mIn         = 0x13E8145; /* 20873541 */
    fbIntFracB  = 0;
    extFbIntDiv = 0x32; /* 50 */
    extFbNum    = 0x6B; /* 107 */
    extFbDen    = 0xF3; /* 243 */
    postDivBy   = 0x301;
    
    /* (1) Wait 10 us */
    result = sysHyPhy20gUsDelay(10);
    if (result) return result;
    
    
    /* (2) Configure bits */
    /* Configure PRE_DIV_BY */
    addr = HYPHY20G_DCSU_DDR_REG_REF_CLK_A_OR_B_SEL;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, preDivBy, 
                       HYPHY20G_DCSU_DDR_BIT_REF_CLK_A_OR_B_SEL_PRE_DIV_BY_MSK,
                       HYPHY20G_DCSU_DDR_BIT_REF_CLK_A_OR_B_SEL_PRE_DIV_BY_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    
    /* Configure EXT_FBCLK_EN, EXT_FB_MOD_SEL and EXT_FB_INT_DIV */
    addr = HYPHY20G_DCSU_DDR_REG_EXT_FB_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbClkEn, 
                             HYPHY20G_DCSU_DDR_BIT_EXT_FB_CFG_EXT_FBCLK_EN_MSK,
                             HYPHY20G_DCSU_DDR_BIT_EXT_FB_CFG_EXT_FBCLK_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, extFbModSel,
                           HYPHY20G_DCSU_DDR_BIT_EXT_FB_CFG_EXT_FB_MOD_SEL_MSK,
                           HYPHY20G_DCSU_DDR_BIT_EXT_FB_CFG_EXT_FB_MOD_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, extFbIntDiv,
                           HYPHY20G_DCSU_DDR_BIT_EXT_FB_CFG_EXT_FB_INT_DIV_MSK,
                           HYPHY20G_DCSU_DDR_BIT_EXT_FB_CFG_EXT_FB_INT_DIV_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* FB_INTPARAM and FB_INT_FRACB */
    addr = HYPHY20G_DCSU_DDR_REG_INT_PAR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, fbIntParam, 
                          HYPHY20G_DCSU_DDR_BIT_INT_PAR_FB_INTPARAM_MSK,
                          HYPHY20G_DCSU_DDR_BIT_INT_PAR_FB_INTPARAM_OFF);
    val = mHYPHY20G_SET_FIELD4(val, fbIntFracB,
                         HYPHY20G_DCSU_DDR_BIT_INT_PAR_FB_INT_FRACB_MSK,
                         HYPHY20G_DCSU_DDR_BIT_INT_PAR_FB_INT_FRACB_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* M_IN */
    addr = HYPHY20G_DCSU_DDR_REG_MODULATION_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, mIn, 
                               HYPHY20G_DCSU_DDR_BIT_MODULATION_CFG_M_IN_MSK,
                               HYPHY20G_DCSU_DDR_BIT_MODULATION_CFG_M_IN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
  
    /* EXT_FB_NUMERATOR */
    addr = HYPHY20G_DCSU_DDR_REG_EXT_FB_NUMR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbNum,
                        HYPHY20G_DCSU_DDR_BIT_EXT_FB_NUMR_EXT_FB_NUMERATOR_MSK, 
                        HYPHY20G_DCSU_DDR_BIT_EXT_FB_NUMR_EXT_FB_NUMERATOR_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    
    /* EXT_FB_DENOMINATOR */
    addr = HYPHY20G_DCSU_DDR_REG_EXT_FB_DENM;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbDen,
                      HYPHY20G_DCSU_DDR_BIT_EXT_FB_DENM_EXT_FB_DENOMINATOR_MSK, 
                      HYPHY20G_DCSU_DDR_BIT_EXT_FB_DENM_EXT_FB_DENOMINATOR_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    
    /* POST_DIV_BY[7:0], POST_DIV_BY[15:8] */
    addr = HYPHY20G_DCSU_DDR_REG_PT_DIVIDER_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, postDivBy, 
                           HYPHY20G_DCSU_DDR_BIT_PT_DIVIDER_CFG_POSTDIV_BY_MSK, 
                           HYPHY20G_DCSU_DDR_BIT_PT_DIVIDER_CFG_POSTDIV_BY_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    /* (3) Wait 50 ms */
    result = sysHyPhy20gMsDelay(50);    
    if (result) return result;

    /* (4) Set DIV2_EN to 1 */
    addr = HYPHY20G_DCSU_DDR_REG_DCO_CLK_DIV_EN;
    msk = HYPHY20G_DCSU_DDR_BIT_DCO_CLK_DIV_EN_DIV2_EN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if(result) return result;

    /* (5) Set ARSTB to 1 */
    addr = HYPHY20G_DCSU_DDR_REG_DCO_CTL;
    msk = HYPHY20G_DCSU_DDR_BIT_DCO_CTL_ARSTB_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if(result) return result;

  
    /* (6) Wait 2 ms */
    result = sysHyPhy20gMsDelay(2);
    if (result) return result;
    
    
    /* (7) Toggle the POST_UPDATE_EN bit to transition from 0 to 1 to 0 */
    addr = HYPHY20G_DCSU_DDR_REG_PT_DIVIDER_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 1, 
                       HYPHY20G_DCSU_DDR_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_MSK, 
                       HYPHY20G_DCSU_DDR_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 0, 
                       HYPHY20G_DCSU_DDR_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_MSK, 
                       HYPHY20G_DCSU_DDR_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    
    /* (8) Poll LOCK_V bit for the respective DCSU to ensure lock has occurred.
           If lock has not occurred a HW fault can be declared as an external
           condition for DCSU initialization has not been met. */
    addr = HYPHY20G_DCSU_DDR_REG_INT_VAL;
    result = sysHyPhy20gPollBit(fileHndl, addr, 
                                HYPHY20G_DCSU_DDR_BIT_INT_VAL_LOCK_V_MSK,
                                HYPHY20G_DCSU_DDR_BIT_INT_VAL_LOCK_V_MSK, 1);
    if (result) return result;
    
    
    /* (9) Set the CLK_BYPASS[1:0] bits to 0 to enable the generated DCSU clock 
           to propagate to downstream blocks. */
    addr = HYPHY20G_DCSU_DDR_REG_CLK_BYP_EN;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 0,
                            HYPHY20G_DCSU_DDR_BIT_CLK_BYP_EN_CLK_BYPASS_EN_MSK, 
                            HYPHY20G_DCSU_DDR_BIT_CLK_BYP_EN_CLK_BYPASS_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
        
    return HYPHY20G_SUCCESS;
        
} /* dcsuHyPhy20gDdrDcsuInit */

/*******************************************************************************
**
**  dcsuHyPhy20gQdrDcsuInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    QDR DSCU initialization. Prior to configuring any DCSU
**                  block, the device power supplies and REFCLK input signal
**                  must be stable.
**
**  INPUTS:         *fileHndl - base address
**
**  NOTE:           For META 20G devices, this function is used for CPB DCSU.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 dcsuHyPhy20gQdrDcsuInit(struct file *fileHndl)
{
    UINT4 preDivBy;
    UINT4 extFbClkEn, extFbModSel;
    UINT4 fbIntParam, mIn, fbIntFracB;
    UINT4 extFbIntDiv, extFbNum, extFbDen;
    UINT4 postDivBy;
    UINT4 addr, val, msk;
    INT4 result;
    
    /* Configuration values for each block */
    preDivBy = 0x9;
    /* QDR DCSU */
    extFbClkEn  = 0x0;
    extFbModSel = 0x1;
    fbIntParam  = 0;
    mIn         = 0x13E8145; /* 20873541 */
    fbIntFracB  = 0;
    extFbIntDiv = 0x32; /* 50 */
    extFbNum    = 0x6B; /* 107 */
    extFbDen    = 0xF3; /* 243 */
    postDivBy   = 0x001;
           
    
    /* (1) Wait 10 us */
    result = sysHyPhy20gUsDelay(10);
    if (result) return result;
    
    
    /* (2) Configure bits */
    /* Configure PRE_DIV_BY */
    addr = HYPHY20G_DCSU_QDR_REG_REF_CLK_A_OR_B_SEL;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, preDivBy, 
                       HYPHY20G_DCSU_QDR_BIT_REF_CLK_A_OR_B_SEL_PRE_DIV_BY_MSK,
                       HYPHY20G_DCSU_QDR_BIT_REF_CLK_A_OR_B_SEL_PRE_DIV_BY_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    
    /* Configure EXT_FBCLK_EN, EXT_FB_MOD_SEL and EXT_FB_INT_DIV */
    addr = HYPHY20G_DCSU_QDR_REG_EXT_FB_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbClkEn, 
                             HYPHY20G_DCSU_QDR_BIT_EXT_FB_CFG_EXT_FBCLK_EN_MSK,
                             HYPHY20G_DCSU_QDR_BIT_EXT_FB_CFG_EXT_FBCLK_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, extFbModSel,
                           HYPHY20G_DCSU_QDR_BIT_EXT_FB_CFG_EXT_FB_MOD_SEL_MSK,
                           HYPHY20G_DCSU_QDR_BIT_EXT_FB_CFG_EXT_FB_MOD_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, extFbIntDiv,
                           HYPHY20G_DCSU_QDR_BIT_EXT_FB_CFG_EXT_FB_INT_DIV_MSK,
                           HYPHY20G_DCSU_QDR_BIT_EXT_FB_CFG_EXT_FB_INT_DIV_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* FB_INTPARAM and FB_INT_FRACB */
    addr = HYPHY20G_DCSU_QDR_REG_INT_PAR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, fbIntParam, 
                          HYPHY20G_DCSU_QDR_BIT_INT_PAR_FB_INTPARAM_MSK,
                          HYPHY20G_DCSU_QDR_BIT_INT_PAR_FB_INTPARAM_OFF);
    val = mHYPHY20G_SET_FIELD4(val, fbIntFracB,
                         HYPHY20G_DCSU_QDR_BIT_INT_PAR_FB_INT_FRACB_MSK,
                         HYPHY20G_DCSU_QDR_BIT_INT_PAR_FB_INT_FRACB_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* M_IN */
    addr = HYPHY20G_DCSU_QDR_REG_MODULATION_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, mIn, 
                               HYPHY20G_DCSU_QDR_BIT_MODULATION_CFG_M_IN_MSK,
                               HYPHY20G_DCSU_QDR_BIT_MODULATION_CFG_M_IN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
  
    /* EXT_FB_NUMERATOR */
    addr = HYPHY20G_DCSU_QDR_REG_EXT_FB_NUMR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbNum,
                        HYPHY20G_DCSU_QDR_BIT_EXT_FB_NUMR_EXT_FB_NUMERATOR_MSK, 
                        HYPHY20G_DCSU_QDR_BIT_EXT_FB_NUMR_EXT_FB_NUMERATOR_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    
    /* EXT_FB_DENOMINATOR */
    addr = HYPHY20G_DCSU_QDR_REG_EXT_FB_DENM;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbDen,
                      HYPHY20G_DCSU_QDR_BIT_EXT_FB_DENM_EXT_FB_DENOMINATOR_MSK, 
                      HYPHY20G_DCSU_QDR_BIT_EXT_FB_DENM_EXT_FB_DENOMINATOR_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    
    /* POST_DIV_BY[7:0], POST_DIV_BY[15:8] */
    addr = HYPHY20G_DCSU_QDR_REG_PT_DIVIDER_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, postDivBy, 
                           HYPHY20G_DCSU_QDR_BIT_PT_DIVIDER_CFG_POSTDIV_BY_MSK, 
                           HYPHY20G_DCSU_QDR_BIT_PT_DIVIDER_CFG_POSTDIV_BY_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* (3) Wait 50 ms */
    result = sysHyPhy20gMsDelay(50);    
    if (result) return result;

    /* (4) Set DIV2_EN to 1 */
    addr = HYPHY20G_DCSU_QDR_REG_DCO_CLK_DIV_EN;
    msk = HYPHY20G_DCSU_QDR_BIT_DCO_CLK_DIV_EN_DIV2_EN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if(result) return result;

    /* (5) Set ARSTB to 1 */
    addr = HYPHY20G_DCSU_QDR_REG_DCO_CTL;
    msk = HYPHY20G_DCSU_QDR_BIT_DCO_CTL_ARSTB_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if(result) return result;

  
    /* (6) Wait 2 ms */
    result = sysHyPhy20gMsDelay(2);
    if (result) return result;
    
    
    /* (7) Toggle the POST_UPDATE_EN bit to transition from 0 to 1 to 0 */
    addr = HYPHY20G_DCSU_QDR_REG_PT_DIVIDER_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 1, 
                       HYPHY20G_DCSU_QDR_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_MSK, 
                       HYPHY20G_DCSU_QDR_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 0, 
                       HYPHY20G_DCSU_QDR_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_MSK, 
                       HYPHY20G_DCSU_QDR_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    
    /* (8) Poll LOCK_V bit for the respective DCSU to ensure lock has occurred.
           If lock has not occurred a HW fault can be declared as an external
           condition for DCSU initialization has not been met. */
    addr = HYPHY20G_DCSU_QDR_REG_INT_VAL;
    result = sysHyPhy20gPollBit(fileHndl, addr, 
                                HYPHY20G_DCSU_QDR_BIT_INT_VAL_LOCK_V_MSK,
                                HYPHY20G_DCSU_QDR_BIT_INT_VAL_LOCK_V_MSK, 1);
    if (result) return result;
    
    
    /* (9) Set the CLK_BYPASS[1:0] bits to 0 to enable the generated DCSU clock 
           to propagate to downstream blocks. */
    addr = HYPHY20G_DCSU_QDR_REG_CLK_BYP_EN;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 0,
                            HYPHY20G_DCSU_QDR_BIT_CLK_BYP_EN_CLK_BYPASS_EN_MSK, 
                            HYPHY20G_DCSU_QDR_BIT_CLK_BYP_EN_CLK_BYPASS_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
        
    return HYPHY20G_SUCCESS;
        
} /* dcsuHyPhy20gQdrDcsuInit */

/*******************************************************************************
**
**  dcsuHyPhy20gPtpDcsuInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    PTP DSCU initialization. Prior to configuring any DCSU
**                  block, the device power supplies and REFCLK input signal
**                  must be stable.
**
**  INPUTS:         *fileHndl - base address
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 dcsuHyPhy20gPtpDcsuInit(struct file *fileHndl)
{
    UINT4 preDivBy;
    UINT4 extFbClkEn, extFbModSel;
    UINT4 fbIntParam, mIn, fbIntFracB;
    UINT4 extFbIntDiv, extFbNum, extFbDen;
    UINT4 postDivBy;
    UINT4 addr, val, msk;
    INT4 result;
    
    /* Configuration values for each block */
    preDivBy = 0x9;
    /* PTP DCSU */
    extFbClkEn  = 0x1;
    extFbModSel = 0x1;
    fbIntParam  = 0;
    mIn         = 0x0;
    fbIntFracB  = 0;
    extFbIntDiv = 0x2F; /* 47 */
    extFbNum    = 0x39ADD37F; /* 967693183 */
    extFbDen    = 0xFFFFFEDC; /* 4294967004 */
    postDivBy   = 0x005;
    
    /* (1) Wait 10 us */
    result = sysHyPhy20gUsDelay(10);
    if (result) return result;
    
    
    /* (2) Configure bits */
    /* Configure PRE_DIV_BY */
    addr = HYPHY20G_DCSU_PTP_REG_REF_CLK_A_OR_B_SEL;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, preDivBy, 
                       HYPHY20G_DCSU_PTP_BIT_REF_CLK_A_OR_B_SEL_PRE_DIV_BY_MSK,
                       HYPHY20G_DCSU_PTP_BIT_REF_CLK_A_OR_B_SEL_PRE_DIV_BY_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    
    /* Configure EXT_FBCLK_EN, EXT_FB_MOD_SEL and EXT_FB_INT_DIV */
    addr = HYPHY20G_DCSU_PTP_REG_EXT_FB_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbClkEn, 
                             HYPHY20G_DCSU_PTP_BIT_EXT_FB_CFG_EXT_FBCLK_EN_MSK,
                             HYPHY20G_DCSU_PTP_BIT_EXT_FB_CFG_EXT_FBCLK_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, extFbModSel,
                           HYPHY20G_DCSU_PTP_BIT_EXT_FB_CFG_EXT_FB_MOD_SEL_MSK,
                           HYPHY20G_DCSU_PTP_BIT_EXT_FB_CFG_EXT_FB_MOD_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, extFbIntDiv,
                           HYPHY20G_DCSU_PTP_BIT_EXT_FB_CFG_EXT_FB_INT_DIV_MSK,
                           HYPHY20G_DCSU_PTP_BIT_EXT_FB_CFG_EXT_FB_INT_DIV_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* FB_INTPARAM and FB_INT_FRACB */
    addr = HYPHY20G_DCSU_PTP_REG_INT_PAR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, fbIntParam, 
                          HYPHY20G_DCSU_PTP_BIT_INT_PAR_FB_INTPARAM_MSK,
                          HYPHY20G_DCSU_PTP_BIT_INT_PAR_FB_INTPARAM_OFF);
    val = mHYPHY20G_SET_FIELD4(val, fbIntFracB,
                         HYPHY20G_DCSU_PTP_BIT_INT_PAR_FB_INT_FRACB_MSK,
                         HYPHY20G_DCSU_PTP_BIT_INT_PAR_FB_INT_FRACB_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* M_IN */
    addr = HYPHY20G_DCSU_PTP_REG_MODULATION_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, mIn, 
                               HYPHY20G_DCSU_PTP_BIT_MODULATION_CFG_M_IN_MSK,
                               HYPHY20G_DCSU_PTP_BIT_MODULATION_CFG_M_IN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
  
    /* EXT_FB_NUMERATOR */
    addr = HYPHY20G_DCSU_PTP_REG_EXT_FB_NUMR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbNum,
                        HYPHY20G_DCSU_PTP_BIT_EXT_FB_NUMR_EXT_FB_NUMERATOR_MSK, 
                        HYPHY20G_DCSU_PTP_BIT_EXT_FB_NUMR_EXT_FB_NUMERATOR_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    
    /* EXT_FB_DENOMINATOR */
    addr = HYPHY20G_DCSU_PTP_REG_EXT_FB_DENM;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbDen,
                      HYPHY20G_DCSU_PTP_BIT_EXT_FB_DENM_EXT_FB_DENOMINATOR_MSK, 
                      HYPHY20G_DCSU_PTP_BIT_EXT_FB_DENM_EXT_FB_DENOMINATOR_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    
    /* POST_DIV_BY[7:0], POST_DIV_BY[15:8] */
    addr = HYPHY20G_DCSU_PTP_REG_PT_DIVIDER_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, postDivBy, 
                           HYPHY20G_DCSU_PTP_BIT_PT_DIVIDER_CFG_POSTDIV_BY_MSK, 
                           HYPHY20G_DCSU_PTP_BIT_PT_DIVIDER_CFG_POSTDIV_BY_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    /* (3) Wait 50 ms */
    result = sysHyPhy20gMsDelay(50);    
    if (result) return result;

    /* (4) Set DIV2_EN to 1 */
    addr = HYPHY20G_DCSU_PTP_REG_DCO_CLK_DIV_EN;
    msk = HYPHY20G_DCSU_PTP_BIT_DCO_CLK_DIV_EN_DIV2_EN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if(result) return result;

    /* (5) Set ARSTB to 1 */
    addr = HYPHY20G_DCSU_PTP_REG_DCO_CTL;
    msk = HYPHY20G_DCSU_PTP_BIT_DCO_CTL_ARSTB_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if(result) return result;

  
    /* (6) Wait 2 ms */
    result = sysHyPhy20gMsDelay(2);
    if (result) return result;
    
    
    /* (7) Toggle the POST_UPDATE_EN bit to transition from 0 to 1 to 0 */
    addr = HYPHY20G_DCSU_PTP_REG_PT_DIVIDER_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 1, 
                       HYPHY20G_DCSU_PTP_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_MSK, 
                       HYPHY20G_DCSU_PTP_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 0, 
                       HYPHY20G_DCSU_PTP_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_MSK, 
                       HYPHY20G_DCSU_PTP_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    
    /* (8) Poll LOCK_V bit for the respective DCSU to ensure lock has occurred.
           If lock has not occurred a HW fault can be declared as an external
           condition for DCSU initialization has not been met. */
    addr = HYPHY20G_DCSU_PTP_REG_INT_VAL;
    result = sysHyPhy20gPollBit(fileHndl, addr, 
                                HYPHY20G_DCSU_PTP_BIT_INT_VAL_LOCK_V_MSK,
                                HYPHY20G_DCSU_PTP_BIT_INT_VAL_LOCK_V_MSK, 1);
    if (result) return result;
    
    
    /* (9) Set the CLK_BYPASS[1:0] bits to 0 to enable the generated DCSU clock 
           to propagate to downstream blocks. */
    addr = HYPHY20G_DCSU_PTP_REG_CLK_BYP_EN;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 0,
                            HYPHY20G_DCSU_PTP_BIT_CLK_BYP_EN_CLK_BYPASS_EN_MSK, 
                            HYPHY20G_DCSU_PTP_BIT_CLK_BYP_EN_CLK_BYPASS_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
        
    return HYPHY20G_SUCCESS;
        
} /* dcsuHyPhy20gPtpDcsuInit */

/*******************************************************************************
**
**  dcsuHyPhy20gOtnDcsuInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    OTN DSCU initialization. Prior to configuring any DCSU
**                  block, the device power supplies and REFCLK input signal
**                  must be stable.
**
**  INPUTS:         *fileHndl - base address
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 dcsuHyPhy20gOtnDcsuInit(struct file *fileHndl)
{
    UINT4 preDivBy;
    UINT4 extFbClkEn, extFbModSel;
    UINT4 fbIntParam, mIn, fbIntFracB;
    UINT4 extFbIntDiv, extFbNum, extFbDen;
    UINT4 postDivBy;
    UINT4 addr, val, msk;
    INT4 result;
    
    /* Configuration values for each block */
    preDivBy = 0x9;
    /* OTN DCSU */
    extFbClkEn  = 0x1;
    extFbModSel = 0x1;
    fbIntParam  = 0;
    mIn         = 0;
    fbIntFracB  = 0;
    extFbIntDiv = 0x2A; /* 42 */
    extFbNum    = 0x9; /* 9 */
    extFbDen    = 0xED; /* 237 */
    postDivBy   = 0x301;
            
    
    /* (1) Wait 10 us */
    result = sysHyPhy20gUsDelay(10);
    if (result) return result;
    
    
    /* (2) Configure bits */
    /* Configure PRE_DIV_BY */
    addr = HYPHY20G_DCSU_OTN_REG_REF_CLK_A_OR_B_SEL;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, preDivBy, 
                       HYPHY20G_DCSU_OTN_BIT_REF_CLK_A_OR_B_SEL_PRE_DIV_BY_MSK,
                       HYPHY20G_DCSU_OTN_BIT_REF_CLK_A_OR_B_SEL_PRE_DIV_BY_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    
    /* Configure EXT_FBCLK_EN, EXT_FB_MOD_SEL and EXT_FB_INT_DIV */
    addr = HYPHY20G_DCSU_OTN_REG_EXT_FB_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbClkEn, 
                             HYPHY20G_DCSU_OTN_BIT_EXT_FB_CFG_EXT_FBCLK_EN_MSK,
                             HYPHY20G_DCSU_OTN_BIT_EXT_FB_CFG_EXT_FBCLK_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, extFbModSel,
                           HYPHY20G_DCSU_OTN_BIT_EXT_FB_CFG_EXT_FB_MOD_SEL_MSK,
                           HYPHY20G_DCSU_OTN_BIT_EXT_FB_CFG_EXT_FB_MOD_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, extFbIntDiv,
                           HYPHY20G_DCSU_OTN_BIT_EXT_FB_CFG_EXT_FB_INT_DIV_MSK,
                           HYPHY20G_DCSU_OTN_BIT_EXT_FB_CFG_EXT_FB_INT_DIV_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* FB_INTPARAM and FB_INT_FRACB */
    addr = HYPHY20G_DCSU_OTN_REG_INT_PAR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, fbIntParam, 
                          HYPHY20G_DCSU_OTN_BIT_INT_PAR_FB_INTPARAM_MSK,
                          HYPHY20G_DCSU_OTN_BIT_INT_PAR_FB_INTPARAM_OFF);
    val = mHYPHY20G_SET_FIELD4(val, fbIntFracB,
                         HYPHY20G_DCSU_OTN_BIT_INT_PAR_FB_INT_FRACB_MSK,
                         HYPHY20G_DCSU_OTN_BIT_INT_PAR_FB_INT_FRACB_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* M_IN */
    addr = HYPHY20G_DCSU_OTN_REG_MODULATION_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, mIn, 
                               HYPHY20G_DCSU_OTN_BIT_MODULATION_CFG_M_IN_MSK,
                               HYPHY20G_DCSU_OTN_BIT_MODULATION_CFG_M_IN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
  
    /* EXT_FB_NUMERATOR */
    addr = HYPHY20G_DCSU_OTN_REG_EXT_FB_NUMR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbNum,
                        HYPHY20G_DCSU_OTN_BIT_EXT_FB_NUMR_EXT_FB_NUMERATOR_MSK, 
                        HYPHY20G_DCSU_OTN_BIT_EXT_FB_NUMR_EXT_FB_NUMERATOR_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    
    /* EXT_FB_DENOMINATOR */
    addr = HYPHY20G_DCSU_OTN_REG_EXT_FB_DENM;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, extFbDen,
                      HYPHY20G_DCSU_OTN_BIT_EXT_FB_DENM_EXT_FB_DENOMINATOR_MSK, 
                      HYPHY20G_DCSU_OTN_BIT_EXT_FB_DENM_EXT_FB_DENOMINATOR_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    
    /* POST_DIV_BY[7:0], POST_DIV_BY[15:8] */
    addr = HYPHY20G_DCSU_OTN_REG_PT_DIVIDER_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, postDivBy, 
                           HYPHY20G_DCSU_OTN_BIT_PT_DIVIDER_CFG_POSTDIV_BY_MSK, 
                           HYPHY20G_DCSU_OTN_BIT_PT_DIVIDER_CFG_POSTDIV_BY_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* (3) Wait 50 ms */
    result = sysHyPhy20gMsDelay(50);    
    if (result) return result;

    /* (4) Set DIV2_EN to 1 */
    addr = HYPHY20G_DCSU_OTN_REG_DCO_CLK_DIV_EN;
    msk = HYPHY20G_DCSU_OTN_BIT_DCO_CLK_DIV_EN_DIV2_EN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if(result) return result;

    /* (5) Set ARSTB to 1 */
    addr = HYPHY20G_DCSU_OTN_REG_DCO_CTL;
    msk = HYPHY20G_DCSU_OTN_BIT_DCO_CTL_ARSTB_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if(result) return result;

  
    /* (6) Wait 2 ms */
    result = sysHyPhy20gMsDelay(2);
    if (result) return result;
    
    
    /* (7) Toggle the POST_UPDATE_EN bit to transition from 0 to 1 to 0 */
    addr = HYPHY20G_DCSU_OTN_REG_PT_DIVIDER_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 1, 
                       HYPHY20G_DCSU_OTN_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_MSK, 
                       HYPHY20G_DCSU_OTN_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 0, 
                       HYPHY20G_DCSU_OTN_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_MSK, 
                       HYPHY20G_DCSU_OTN_BIT_PT_DIVIDER_CFG_POST_UPDATE_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    
    /* (8) Poll LOCK_V bit for the respective DCSU to ensure lock has occurred.
           If lock has not occurred a HW fault can be declared as an external
           condition for DCSU initialization has not been met. */
    addr = HYPHY20G_DCSU_OTN_REG_INT_VAL;
    result = sysHyPhy20gPollBit(fileHndl, addr, 
                                HYPHY20G_DCSU_OTN_BIT_INT_VAL_LOCK_V_MSK,
                                HYPHY20G_DCSU_OTN_BIT_INT_VAL_LOCK_V_MSK, 1);
    if (result) return result;
    
    
    /* (9) Set the CLK_BYPASS[1:0] bits to 0 to enable the generated DCSU clock 
           to propagate to downstream blocks. */
    addr = HYPHY20G_DCSU_OTN_REG_CLK_BYP_EN;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 0,
                            HYPHY20G_DCSU_OTN_BIT_CLK_BYP_EN_CLK_BYPASS_EN_MSK, 
                            HYPHY20G_DCSU_OTN_BIT_CLK_BYP_EN_CLK_BYPASS_EN_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
        
    return HYPHY20G_SUCCESS;
        
} /* dcsuHyPhy20gOtnDcsuInit */

/* end of file */
