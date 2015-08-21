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
**  FILE        : hyphy20g_util_acb.c
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
#include "hyphy20g_util_acb.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 utilHyPhy20gSfpAcbGetMode(struct file *fileHndl, UINT4 slice, UINT4 *pMode);

INT4 utilHyPhy20gSfpAcbGetPmmSource(struct file *fileHndl, UINT4 slice,
                                    UINT4 *pPmmSource);

INT4 utilHyPhy20gXfiAcbGetMode(struct file *fileHndl, UINT4 slice, UINT4 *pMode);

INT4 utilHyPhy20gXfiAcbGetPmmSource(struct file *fileHndl, UINT4 slice,
                                    UINT4 *pPmmSource);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/


/*******************************************************************************
**
**  hyPhy20gSfpAcbInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs default initialization of the ACB
**                  block and configures it to provide a nominal timing signal
**                  to SERDES.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - ACB Instance 1:1 mapped to SFP ports
**                                    0 to 15
**                                    17 Management Port SFP
**                  enable          - ACB Enable
**                                    0 - Disabled
**                                    1 - Enabled
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSfpAcbInit(struct file *fileHndl, UINT4 slice, UINT4 enable)
{
    /* variable declaration */
    UINT4 acbCfg1Reg, acbRstMsk, acbGainMsk, fbWordSzMsk, odu2ModMsk;
    UINT4 enableFifoCentMsk, enableBIntMsk, enableFifoAdaptMsk;
    UINT4 enablePmmAdaptMsk;
    UINT4 acbCfg2Reg, lpGainMsk, cntNMsk;
    UINT4 acbCfg3Reg, cntDMsk;
    UINT4 acbCfg5Reg, cPktPerMsk, cPktSzVarMsk, cPktSz;
    UINT4 cfg0To3Reg, cfg0Msk;
    UINT4 sfpCfg8Reg, adsmFracDivModSelMsk;
    UINT4 sfpCfg6Reg, sfpCfg6Data, txPathBusWidth, fbWordSzVal;
    UINT4 regData;
    INT4 result;

    /* argument checking */
    if(((slice > 15)&&(slice!=17))||(enable > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* set FB_WORDSIZE[1:0] as a function of TXPATH_BUS_WIDTH
       TXPATH_BUS_WIDTH set to 0x0 or 0x1 (08/10bit) set FB_WORDSIZE to 0x00
       TXPATH_BUS_WIDTH set to 0x2 or 0x3 (16/20bit) set FB_WORDSIZE to 0x01  */

    sfpCfg6Reg = HYPHY20G_SERDES_SFP_CORE_REG_CONF_6;
    sfpCfg6Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(sfpCfg6Reg, slice);

    result = sysHyPhy20gRead(fileHndl, sfpCfg6Reg, &sfpCfg6Data);
    if(result) return result;

    txPathBusWidth = mHYPHY20G_GET_FIELD4(sfpCfg6Data,
                      HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_TXPATH_BUS_WIDTH_MSK,
                      HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_TXPATH_BUS_WIDTH_OFF);

    if(txPathBusWidth > 0x3){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    switch (txPathBusWidth) {
        case 0:
            fbWordSzVal = 0x0;
            break;
        case 1:
            fbWordSzVal = 0x0;
            break;
        case 2:
            fbWordSzVal = 0x1;
            break;
        case 3:
            fbWordSzVal = 0x1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* set fbWordSzVal in a case statement */

    acbCfg1Reg = HYPHY20G_SERDES_SFP_TOP_REG_ACB_CFG_1;
    acbCfg1Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(acbCfg1Reg, slice);

    acbRstMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ACB_RST_MSK;
    acbGainMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ACB_GAIN_MSK;
    fbWordSzMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_FB_WORDSIZE_MSK;
    odu2ModMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ODU2_MOD_MSK;
    enableFifoCentMsk =
                     HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_FIFO_CENT_MSK;
    enableBIntMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_B_INT_MSK;
    enableFifoAdaptMsk =
                    HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_FIFO_ADAPT_MSK;
    enablePmmAdaptMsk =
                     HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_PMM_ADAPT_MSK;


    acbCfg2Reg = HYPHY20G_SERDES_SFP_TOP_REG_ACB_CFG_2;
    acbCfg2Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(acbCfg2Reg, slice);

    lpGainMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_2_LP_GAIN_MSK;
    cntNMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_2_CNT_N_MSK;

    acbCfg3Reg = HYPHY20G_SERDES_SFP_TOP_REG_ACB_CFG_3;
    acbCfg3Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(acbCfg3Reg, slice);

    cntDMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_3_CNT_D_MSK;

    acbCfg5Reg = HYPHY20G_SERDES_SFP_TOP_REG_ACB_CFG_5;
    acbCfg5Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(acbCfg5Reg, slice);

    cPktPerMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_5_C_PKT_PERIOD_MSK;
    cPktSzVarMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_5_C_PKT_SIZE_VAR_MSK;
    cPktSz = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_5_C_PKT_SIZE_MSK;

    cfg0To3Reg = HYPHY20G_SERDES_SFP_TOP_REG_ACB_FRC_WAVE_AND_CFG0_TO_CFG3;
    cfg0To3Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(cfg0To3Reg, slice);

    cfg0Msk =
             HYPHY20G_SERDES_SFP_TOP_BIT_ACB_FRC_WAVE_AND_CFG0_TO_CFG3_CFG0_MSK;

    sfpCfg8Reg = HYPHY20G_SERDES_SFP_CORE_REG_CONF_8;
    sfpCfg8Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(sfpCfg8Reg, slice);

    adsmFracDivModSelMsk =
                    HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_FRACDIVMOD_SEL_MSK;

    /* set the ACB_RST bit to 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, acbCfg1Reg, acbRstMsk, 1);
    if(result) return result;

    /* return success if disabling slice */
    if(enable == 0){
        return HYPHY20G_SUCCESS;
    }

    /* Cfg1to4 Reg */
    /* set CFG0 = 0*/
    /* note CFG0 = 3 for FIFO mode - need clarification */
    result = sysHyPhy20gRead(fileHndl, cfg0To3Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, cfg0Msk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, cfg0To3Reg, regData);
    if(result) return result;

    /* Cfg Reg 5 */
    /* set C_PKT_PERIOD = 0, C_PKT_SIZE_VAR = 0 and C_PKT_SIZE = 0 */
    result = sysHyPhy20gRead(fileHndl, acbCfg5Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, cPktPerMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, cPktSzVarMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, cPktSz, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, acbCfg5Reg, regData);
    if(result) return result;

    /* Cfg Reg 3 */
    /* set CNT_D = 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, acbCfg3Reg, cntDMsk, 1);
    if(result) return result;

    /* Cfg Reg 2 */
    /* set LP_GAIN =  0xF */
    /* set CNT_N = 0 */
    result = sysHyPhy20gRead(fileHndl, acbCfg2Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, lpGainMsk, 0xF);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, cntNMsk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, acbCfg2Reg, regData);
    if(result) return result;

    /* Cfg Reg 1 */
/* set ACB_GAIN to 0x8 (note FIFO mode used an ACB_GAIN of 0xB - need clarification) */
    /* set ENABLE_FIFO_CENT = 0, ENABLE_B_INT = 0, ENABLE_FIFO_ADAPT = 0, ENABLE_PMM_ADAPT = 0 */
    /* set the ACB_RST bit to 0 */
    result = sysHyPhy20gRead(fileHndl, acbCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, acbRstMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, acbGainMsk, 8);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, fbWordSzMsk, fbWordSzVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, odu2ModMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enableFifoCentMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enableBIntMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enableFifoAdaptMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enablePmmAdaptMsk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, acbCfg1Reg, regData);
    if(result) return result;

    /* SFP Cfg 8 Reg*/
    /* set ADSM_FRACDIVMOD_SEL = 0x2 */
    result = sysHyPhy20gRead(fileHndl, sfpCfg8Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, adsmFracDivModSelMsk, 0x2);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, sfpCfg8Reg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSfpAcbInit */


/*******************************************************************************
**
**  hyPhy20gSfpAcbModeSel
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configured the operation mode of the ACB
**                  block.  It may be called after the block is initialized with
**                  hyPhy20gSfpAcbInit.
**
**                  Prior to calling this function hyPhy20gSfpAcbCfgGain may be
**                  called to adjust gain parameters associated with the PMM
**                  block.
**
**                  This function will identify PMM block associated with C
**                  ACB input and retrieve packet length data from that PMM
**                  block for application to ACB packet length related fields.
**                  For modes set to other than `Nominal mode`,the PMM 
**                  associated with source C must be initialized and the 
**                  routing of SFP source C input from the associated PMM 
**                  must be configured prior to making this call. Refer to:   
**                      - hyPhy20gLsciPmmInit(or hyPhy20gOpiPmmInit)
**                      - hyPhy20gSonetLnSfpSrcCSel
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - ACB Instance 1:1 mapped to SFP ports
**                                    0 to 15
**                  mode            - ACB Processing Mode:
**                                    0 - Nominal mode
**                                    1 - Phase mode using source C
**                                    2 - Phase mode using source B and C
**                                    3 - FIFO mode
**                  lineFrm         - Frame type transferred on Line
**                                    0 - non-OTU1
**                                    1 - OTU1
**                  bRate           - Rate of B packet size data source
**                                    0 - ODU1 stream
**                                    1 - ODU2 stream
**                                    Only valid for mode = 2
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NO_SW_CONTENT
**
*******************************************************************************/
INT4 hyPhy20gSfpAcbModeSel(struct file *fileHndl, UINT4 slice, UINT4 mode,
                           UINT4 lineFrm, UINT4 bRate)
{
    /* variable declaration */
    UINT4 devId, revId, status;
    UINT4 pmmSource, monPktSz, mkrPktSz, mkrPktSzVar, mkrPktPeriod;
    UINT4 acbCfg1Reg;
    UINT4 odu2ModMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ODU2_MOD_MSK;
    UINT4 enableFifoCentMsk =
                     HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_FIFO_CENT_MSK;
    UINT4 enableBIntMsk =
                         HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_B_INT_MSK;
    UINT4 enableFifoAdaptMsk =
                    HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_FIFO_ADAPT_MSK;
    UINT4 enablePmmAdaptMsk =
                     HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_PMM_ADAPT_MSK;
    UINT4 enableFifoCent = 0;
    UINT4 enablePmmAdapt = 0;
    UINT4 enableFifoAdapt = 0;
    UINT4 enableBInt = 0;
    UINT4 acbCfg2Reg;
    UINT4 cntNMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_2_CNT_N_MSK;
    UINT4 acbCfg3Reg;
    UINT4 cntDMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_3_CNT_D_MSK;
    UINT4 acbCfg5Reg;
    UINT4 cPktPerMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_5_C_PKT_PERIOD_MSK;
    UINT4 cPktSzVarMsk =
                       HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_5_C_PKT_SIZE_VAR_MSK;
    UINT4 cPktSz = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_5_C_PKT_SIZE_MSK;
    UINT4 cntNVal = 0;
    UINT4 cntDVal = 0;
    UINT4 regData;
    UINT4 period, remainder, intDivision, scalePeriod;    
    INT4 result;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 15)||(mode > 3)||(lineFrm > 1)||(bRate > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* for non-nominal modes recover PMM packet size and bus width data
       for application to ACB block */
    if (mode > 0){
        acbCfg5Reg = HYPHY20G_SERDES_SFP_TOP_REG_ACB_CFG_5;
        acbCfg5Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(acbCfg5Reg,
                                                               slice);
        /* identify PMM source */
        result = utilHyPhy20gSfpAcbGetPmmSource(fileHndl, slice, &pmmSource);
        if(result) return result;
        /* retrieve PMM context data */
        result = utilHyPhy20gSfpAcbGetPmmPktData(fileHndl, slice, pmmSource,
                                                 &monPktSz, &mkrPktSz,
                                                 &mkrPktSzVar, &mkrPktPeriod);
        if(result) return result;
        result = sysHyPhy20gRead(fileHndl, acbCfg5Reg, &regData);
        if(result) return result;

        /* modify C_PKT_PERIOD setting if adaptive FIFO timing is being used */
        if(mode==3){
            if(mkrPktSzVar==1){ /* 8b10b CBR client */
                /* add ~10% to the PMM packet period */
                mkrPktPeriod = mkrPktPeriod + mkrPktPeriod/10;
            } else { /* non-8b10b CBR client */
                /* return error if mkrPktSz == 0 */
                if (mkrPktSz == 0) {
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_ZERO_DIVIDE\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_ZERO_DIVIDE;
                }
                /* scale PMM packet period for an ~4K byte packet */
                mkrPktPeriod = mkrPktPeriod*(12000/mkrPktSz + (12000%mkrPktSz)? 1:0);
                period = mkrPktPeriod;
                remainder = (12000%mkrPktSz != 0)?1:0;
                intDivision = 12000/mkrPktSz;
                scalePeriod = period*(intDivision + remainder);
                mkrPktPeriod = scalePeriod;
             }
            /* if C_PKT_PERIOD exceeds 16 bits, truncate to 0xFFFF */
            if(mkrPktPeriod > 0xFFFF) mkrPktPeriod = 0xFFFF;
        }

        result = hyPhy20gVarFieldWrite (&regData, cPktPerMsk, mkrPktPeriod);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cPktSzVarMsk, mkrPktSzVar);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cPktSz, mkrPktSz);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, acbCfg5Reg, regData);
        if(result) return result;

    }

    acbCfg1Reg = HYPHY20G_SERDES_SFP_TOP_REG_ACB_CFG_1;
    acbCfg1Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(acbCfg1Reg, slice);
    acbCfg2Reg = HYPHY20G_SERDES_SFP_TOP_REG_ACB_CFG_2;
    acbCfg2Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(acbCfg2Reg, slice);
    acbCfg3Reg = HYPHY20G_SERDES_SFP_TOP_REG_ACB_CFG_3;
    acbCfg3Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(acbCfg3Reg, slice);

    /* lineFrm configures default setting for gains CNT_N and CNT_D */
    if (mode == 1 || mode == 2) {
        switch (lineFrm) {
            case 0:
                if (pmmSource < 9){
                /* settings for LSCI PMM providing source C to SFP ACB or OTN I/F
                   CBRB PMM providing source C to SFP ACB with ODU1 frames being
                   transferred on the line */
                    cntNVal = 0;
                    cntDVal = 1;
                } else if (pmmSource > 9){
                /* settings for VCAT I/F CBRB PMM providing source C to SFP ACB with
                   data connection to SFP from LSCI.  this mode supports synchronous
                   demapping of STS-48 from ODU1 */
                    cntNVal = 0xFFFFFF;  /* -1d */
                    cntDVal = 0xEE; /* 238d */
                } else if (pmmSource == 9){
                /* settings for case where ACB source C is associated with ENET_COM
                   PMM */
                    return HYPHY20G_ERR_NO_SW_CONTENT;
                } else {
                    return HYPHY20G_ERR_INVALID_ARG;
                }
                break;
            case 1:
                /* settings OTN I/F CBRB PMM providing source C to SFP ACB with
                   OTU1 frames being transferred on the line */
                cntNVal = 0x10;
                cntDVal = 0xFF;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }

        /* set the CNT_N field */
        result = hyPhy20gRegFieldWrite (fileHndl, acbCfg2Reg, cntNMsk, cntNVal);
        if(result) return result;

        /* set the CNT_D field */
        result = hyPhy20gRegFieldWrite (fileHndl, acbCfg3Reg, cntDMsk, cntDVal);
        if(result) return result;
    }

    /* mode field setting specification */
    switch (mode) {
        case 0: /* 0 - Nominal mode */
            enablePmmAdapt = 0;
            enableFifoAdapt = 0;
            enableBInt = 0;
            break;
        case 1: /* 1 - Phase mode using source C */
            enablePmmAdapt = 1;
            enableFifoAdapt = 0;
            enableBInt = 0;
            break;
        case 2: /* 2 - Phase mode using source B and C */
            enablePmmAdapt = 1;
            enableFifoAdapt = 0;
            enableBInt = 1;
            break;
        case 3: /* 3 - FIFO mode */
            enablePmmAdapt = 0;
            enableFifoAdapt = 1;
            enableBInt = 0;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /*ENABLE_FIFO_CENT bit should always set to 0 for all modes*/
    enableFifoCent = 0;

    result = sysHyPhy20gRead(fileHndl, acbCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enableFifoCentMsk,
                                    enableFifoCent);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enablePmmAdaptMsk,
                                    enablePmmAdapt);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enableFifoAdaptMsk,
                                    enableFifoAdapt);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enableBIntMsk, enableBInt);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, odu2ModMsk, bRate);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, acbCfg1Reg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSfpAcbModeSel */

/*******************************************************************************
**
**  hyPhy20gSfpAcbCfgGain
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function modifies the default gain settings as applied
**                  with hyPhy20gSfpAcbInit.
**
**                  This function will test operating mode of the device and
**                  only proceed if the block is enabled and operating in
**                  nominal mode.
**
**                  ACB_GAIN, LP_GAIN and CFG0 to CFG3 may maintain default
**                  settings for ACB phase mode timing (excluding some video
**                  applications),
**
**                  For adaptive FIFO ACB
**                  timing the following settings may be used:
**                  ACB_GAIN 0x8
**                  LP_GAIN 0xF
**                  CFG0 0x3
**
**                  For phase mode timing the following values are used in
**                  hyPhy20gSfpAcbInit:
**                  ACB_GAIN 0x8
**                  LP_GAIN 0xF
**                  CFG0 0x0
**
**                  The following video applications can use the above
**                  settings with ACB_GAIN set to 0x4:
**                  - Video HD-SDI 1.485G
**                  - Video HD-SDI (1.001) 1.483G
**                  - Video 3GHD-SDI 2.97G
**                  - Video 2.967M
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - ACB Instance 1:1 mapped to SFP ports
**                                    0 to 15
**                  acbGain         - ACB_GAIN field setting.
**                  lpGain          - LP_GAIN field setting.
**                  cfg0            - CFG0 field setting.
**                  cfg1            - CFG1 field setting.OBSOLETE, NO
**                                    DEVICE REGISTER CHANGE BASED ON THIS ARG
**                  cfg2            - CFG2 field setting.OBSOLETE, NO
**                                    DEVICE REGISTER CHANGE BASED ON THIS ARG
**                  cfg3            - CFG3 field setting.OBSOLETE, NO
**                                    DEVICE REGISTER CHANGE BASED ON THIS ARG
**                  updateMsk       - Mask of ACB gain related configuration
**                                    fields to be updated.  Set the
**                                    corresponding bit to 1 to update a field,
**                                    set to 0 to maintain value of configured
**                                    field:
**                                    bit 0 - update cfg3 field
**                                    bit 1 - update cfg2 field
**                                    bit 2 - update cfg1 field
**                                    bit 3 - update cfg0 field
**                                    bit 4 - update lpGain field
**                                    bit 5 - update acbGain field
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSfpAcbCfgGain(struct file *fileHndl, UINT4 slice, UINT4 acbGain,
                           UINT4 lpGain,UINT4 cfg0, UINT4 cfg1, UINT4 cfg2,
                           UINT4 cfg3, UINT4 updateMsk)
{
    /* variable declaration */
    UINT4 cfg1Reg;
    UINT4 acbGainMsk = 0;
    UINT4 cfg2Reg;
    UINT4 lpGainMsk = 0;
    UINT4 cfg0To3Reg;
    UINT4 cfg0Msk = 0;
    UINT4 value, regData;
    INT4 result;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 15)||(acbGain > 15)||
       (lpGain > 15)||
       (cfg0 > 15)||
       (cfg1 > 15)||
       (cfg2 > 15)||
       (cfg3 > 15)||
       (updateMsk > 0x3F)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* test to confirm block in nominal mode before proceeding */
    result = utilHyPhy20gSfpAcbGetMode(fileHndl, slice, &value);
    if(value){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* register accessing parameters */
    cfg1Reg = HYPHY20G_SERDES_SFP_TOP_REG_ACB_CFG_1;
    cfg1Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(cfg1Reg, slice);

    acbGainMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ACB_GAIN_MSK;

    cfg2Reg = HYPHY20G_SERDES_SFP_TOP_REG_ACB_CFG_2;
    cfg2Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(cfg2Reg, slice);

    lpGainMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_2_LP_GAIN_MSK;

    cfg0To3Reg = HYPHY20G_SERDES_SFP_TOP_REG_ACB_FRC_WAVE_AND_CFG0_TO_CFG3;
    cfg0To3Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(cfg0To3Reg, slice);

    cfg0Msk =
             HYPHY20G_SERDES_SFP_TOP_BIT_ACB_FRC_WAVE_AND_CFG0_TO_CFG3_CFG0_MSK;

    /* configure the ACB_GAIN field if specified */
    if(updateMsk & 0x20){
        result = hyPhy20gRegFieldWrite (fileHndl, cfg1Reg, acbGainMsk, acbGain);
        if(result) return result;
    }

    /* configure the LP_GAIN field if specified */
    if(updateMsk & 0x10){
        result = hyPhy20gRegFieldWrite (fileHndl, cfg2Reg, lpGainMsk, lpGain);
        if(result) return result;
    }

    /* configure CFG0 to CFG3 values */
    result = sysHyPhy20gRead(fileHndl, cfg0To3Reg, &regData);
    if(result) return result;

    /* configure the CFG0 field if specified */
    if(updateMsk & 0x8){
        result = hyPhy20gVarFieldWrite (&regData, cfg0Msk, cfg0);
        if(result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, cfg0To3Reg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSfpAcbCfgGain */

/*******************************************************************************
**
**  hyPhy20gXfiAcbInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs default initialization of the ACB
**                  block and configures it to provide a nominal timing signal
**                  to SERDES.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - ACB Instance 1:1 mapped to XFI ports
**                                    0 to 1
**                  enable          - ACB Enable
**                                    0 - Disabled
**                                    1 - Enabled
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gXfiAcbInit(struct file *fileHndl, UINT4 slice, UINT4 enable)
{
    /* variable declaration */
    UINT4 acbCfg1Reg, acbRstMsk, acbGainMsk, fbWordSzMsk, odu2ModMsk;
    UINT4 enableFifoCentMsk, enableBIntMsk, enableFifoAdaptMsk;
    UINT4 enablePmmAdaptMsk;
    UINT4 acbCfg2Reg, lpGainMsk, cntNMsk;
    UINT4 acbCfg3Reg, cntDMsk;
    UINT4 acbCfg5Reg, cPktPerMsk, cPktSzVarMsk, cPktSz;
    UINT4 cfg0To3Reg, cfg0Msk;
    UINT4 xfiCfg10Reg, adsmFracDivModSelMsk;
    UINT4 xfiCfg2Reg, acbPllFbClkSelMsk;
    UINT4 regData;
    INT4 result;

    /* argument checking */
    if((slice > 15)||(enable > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    acbCfg1Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CFG_1;
    acbCfg1Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(acbCfg1Reg, slice);

    acbRstMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ACB_RST_MSK;
    acbGainMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ACB_GAIN_MSK;
    fbWordSzMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_FB_WORDSIZE_MSK;
    odu2ModMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_OTU2_MOD_MSK;
    enableFifoCentMsk =
                     HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_FIFO_CENT_MSK;
    enableBIntMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_B_INT_MSK;
    enableFifoAdaptMsk =
                    HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_FIFO_ADAPT_MSK;
    enablePmmAdaptMsk =
                     HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_PMM_ADAPT_MSK;


    acbCfg2Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CFG_2;
    acbCfg2Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(acbCfg2Reg, slice);

    lpGainMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_2_LP_GAIN_MSK;
    cntNMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_2_CNT_N_MSK;

    acbCfg3Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CFG_3;
    acbCfg3Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(acbCfg3Reg, slice);

    cntDMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_3_CNT_D_MSK;

    acbCfg5Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CFG_5;
    acbCfg5Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(acbCfg5Reg, slice);

    cPktPerMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_5_C_PKT_PERIOD_MSK;
    cPktSzVarMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_5_C_PKT_SIZE_VAR_MSK;
    cPktSz = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_5_C_PKT_SIZE_MSK;

    cfg0To3Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CTL;
    cfg0To3Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(cfg0To3Reg, slice);

    cfg0Msk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CTL_CFG0_MSK;

    xfiCfg10Reg = HYPHY20G_SERDES_XFI_CORE_REG_CFG_10;
    xfiCfg10Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(xfiCfg10Reg, slice);

    adsmFracDivModSelMsk =
                    HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_FRACDIVMOD_SEL_MSK;

    xfiCfg2Reg = HYPHY20G_SERDES_XFI_TOP_REG_CFG_2;
    xfiCfg2Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(xfiCfg2Reg, slice);
    acbPllFbClkSelMsk = HYPHY20G_SERDES_XFI_TOP_BIT_CFG_2_ACB_PLL_FBCLK_SEL_MSK;

    /* set the ACB_RST bit to 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, acbCfg1Reg, acbRstMsk, 1);
    if(result) return result;

    /* return success if disabling slice */
    if(enable == 0){
        return HYPHY20G_SUCCESS;
    }

    /* XFI Cfg 10 Reg*/
    /* set ADSM_FRACDIVMOD_SEL = 0x2 */
    result = sysHyPhy20gRead(fileHndl, xfiCfg10Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, adsmFracDivModSelMsk, 0x2);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, xfiCfg10Reg, regData);
    if(result) return result;

    /* Cfg1to4 Reg */
    /* set CFG3 = 0, CFG2 = 0, CFG1 = 0 and CFG0 = 0*/
    /* note CFG0 = 3 for FIFO mode - need clarification */
    result = sysHyPhy20gRead(fileHndl, cfg0To3Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, cfg0Msk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, cfg0To3Reg, regData);
    if(result) return result;

    /* Cfg Reg 5 */
    /* set C_PKT_PERIOD = 0, C_PKT_SIZE_VAR = 0 and C_PKT_SIZE = 0 */
    result = sysHyPhy20gRead(fileHndl, acbCfg5Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, cPktPerMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, cPktSzVarMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, cPktSz, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, acbCfg5Reg, regData);
    if(result) return result;

    /* Cfg Reg 3 */
    /* set CNT_D = 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, acbCfg3Reg, cntDMsk, 1);
    if(result) return result;

    /* Cfg Reg 2 */
    /* set LP_GAIN =  0xF */
    /* set CNT_N = 0 */
    result = sysHyPhy20gRead(fileHndl, acbCfg2Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, lpGainMsk, 0xF);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, cntNMsk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, acbCfg2Reg, regData);
    if(result) return result;

    /* Cfg Reg 1 */
/* set ACB_GAIN to 0x8 (note FIFO mode used an ACB_GAIN of 0xB - need clarification) */
    /* set FB_WORDSIZE[1:0] to 0x3 for all XFI clients */
    /* set ENABLE_FIFO_CENT = 0, ENABLE_B_INT = 0, ENABLE_FIFO_ADAPT = 0, ENABLE_PMM_ADAPT = 0 */
    /* set the ACB_RST bit to 0 */
    result = sysHyPhy20gRead(fileHndl, acbCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, acbRstMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, acbGainMsk, 8);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, fbWordSzMsk, 0x3);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, odu2ModMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enableFifoCentMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enableBIntMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enableFifoAdaptMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enablePmmAdaptMsk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, acbCfg1Reg, regData);
    if(result) return result;

    /* XFI Top Cfg Register 2 set ACB_PLL_FBCLK_SEL to 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, xfiCfg2Reg, acbPllFbClkSelMsk, 1);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gXfiAcbInit */

/*******************************************************************************
**
**  hyPhy20gXfiAcbModeSel
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configured the operation mode of the ACB
**                  block.  It may be called after the block is initialized with
**                  hyPhy20gXfiAcbInit.
**
**                  Prior to calling this function hyPhy20gXfiAcbCfgGain may be
**                  called to adjust gain parameters associated with the PMM
**                  block.
**
**                  This function will identify PMM block associated with C
**                  ACB input and retrieve packet length data from that PMM
**                  block for application to ACB packet length related fields.
**                  For modes set to other than `Nominal mode`,the PMM 
**                  associated with source C must be initialized and the 
**                  routing of XFP source C input from the associated PMM 
**                  must be configured prior to making this call. Refer to:   
**                      - hyPhy20gHsciPmmInit(or hyPhy20gOpiPmmInit)
**                      - hyPhy20gSonetLnXfiSrcCSel
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - ACB Instance 1:1 mapped to XFI ports
**                                    0 to 1
**                  mode            - ACB Processing Mode:
**                                    0 - Nominal mode
**                                    1 - Phase mode using source C
**                                    2 - Phase mode using source B and C
**                                    3 - FIFO mode
**                  lineFrm         - Frame type transferred on Line for mode
**                                    set to 1 or 2.  For mode 0 or 3 set to 0.
**                                    0  - non-ODUk/OTUk
**                                    1  - OTU2
**                                    2  - OTU2e
**                                    3  - OTU1e
**                                    4  - OTU2f
**                                    5  - OTU1f
**                                    6  - ODU2
**                                    7  - ODU2e
**                                    8  - ODU1e
**                                    9  - ODU2f
**                                    10 - ODU1f
**                                    11 - HSCI related XFI with Source C
**                                         from CBRB PMM transferring
**                                         ODU1e or ODU1f data
**                                         synchronous demapping application
**                                    12 - HSCI related XFI with Source C
**                                         from CBRB PMM transferring
**                                         ODU2e or ODU2f data (only valid for
**                                         mode == 1)
**                                         synchronous demapping application
**                                    13 - OPI PMM being used for POS internal
**                                         loop timing (only valid for
**                                         mode == 1)
**
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gXfiAcbModeSel(struct file *fileHndl, UINT4 slice, UINT4 mode,
                           UINT4 lineFrm)
{
    /* variable declaration */
    UINT4 pmmSource, monPktSz, mkrPktSz, mkrPktSzVar, mkrPktPeriod;
    UINT4 acbCfg1Reg;
    UINT4 odu2ModMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_OTU2_MOD_MSK;
    UINT4 enableFifoCentMsk =
                     HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_FIFO_CENT_MSK;
    UINT4 enableBIntMsk =
                         HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_B_INT_MSK;
    UINT4 enableFifoAdaptMsk =
                    HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_FIFO_ADAPT_MSK;
    UINT4 enablePmmAdaptMsk =
                     HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_PMM_ADAPT_MSK;
    UINT4 acbCfg2Reg;
    UINT4 cntNMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_2_CNT_N_MSK;
    UINT4 acbCfg3Reg;
    UINT4 cntDMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_3_CNT_D_MSK;
    UINT4 acbCfg5Reg;
    UINT4 cPktPerMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_5_C_PKT_PERIOD_MSK;
    UINT4 cPktSzVarMsk =
                       HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_5_C_PKT_SIZE_VAR_MSK;
    UINT4 cPktSz = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_5_C_PKT_SIZE_MSK;
    UINT4 enableFifoCent = 0;
    UINT4 enablePmmAdapt = 0;
    UINT4 enableFifoAdapt = 0;
    UINT4 enableBInt = 0;
    UINT4 cntNVal = 0;
    UINT4 cntDVal = 0;
    UINT4 regData;
    UINT4 period, remainder, intDivision, scalePeriod;  
    INT4 result;

    /* argument checking */
    if((slice > 1)||(mode > 3)||(lineFrm > 13)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    acbCfg1Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CFG_1;
    acbCfg1Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(acbCfg1Reg, slice);
    acbCfg2Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CFG_2;
    acbCfg2Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(acbCfg2Reg, slice);
    acbCfg3Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CFG_3;
    acbCfg3Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(acbCfg3Reg, slice);
    acbCfg5Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CFG_5;
    acbCfg5Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(acbCfg5Reg, slice);

    /* ACB packet length configuration */
    if (mode > 0){
        acbCfg5Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CFG_5;
        acbCfg5Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(acbCfg5Reg,
                                                               slice);
        /* identify PMM source */
        result = utilHyPhy20gXfiAcbGetPmmSource(fileHndl, slice, &pmmSource);
        if(result) return result;
        /* retrieve PMM context data */
        result = utilHyPhy20gXfiAcbGetPmmPktData(fileHndl, pmmSource,
                                                 &monPktSz, &mkrPktSz,
                                                 &mkrPktSzVar, &mkrPktPeriod);
        if(result) return result;
        result = sysHyPhy20gRead(fileHndl, acbCfg5Reg, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cPktPerMsk, mkrPktPeriod);
        if(result) return result;

        /* modify C_PKT_PERIOD setting if adaptive FIFO timing is being used */
        if(mode==3){
            if(mkrPktSzVar==1){ /* 8b10b CBR client */
                /* add ~10% to the PMM packet period */
                mkrPktPeriod = mkrPktPeriod + mkrPktPeriod/10;
            } else { /* non-8b10b CBR client */
                /* return error if mkrPktSz == 0 */
                if (mkrPktSz == 0) {
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_ZERO_DIVIDE\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_ZERO_DIVIDE;
                }
                /* scale PMM packet period for an ~4K byte packet */
                mkrPktPeriod = mkrPktPeriod*(12000/mkrPktSz + (12000%mkrPktSz)? 1:0);
                period = mkrPktPeriod;
                remainder = (12000%mkrPktSz != 0)?1:0;
                intDivision = 12000/mkrPktSz;
                scalePeriod = period*(intDivision + remainder);
                mkrPktPeriod = scalePeriod;
             }
            /* if C_PKT_PERIOD exceeds 16 bits, truncate to 0xFFFF */
            if(mkrPktPeriod > 0xFFFF) mkrPktPeriod = 0xFFFF;
        }

        result = hyPhy20gVarFieldWrite (&regData, cPktSzVarMsk, mkrPktSzVar);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, cPktSz, mkrPktSz);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, acbCfg5Reg, regData);
        if(result) return result;

    }

    /* CNT_N and CNT_D specification as a function of lineFrm */
    if (mode == 1 || mode == 2) {
        switch (lineFrm) {
            case 0: /* 0  - non-ODUk/OTUk */
                cntNVal = 0;
                cntDVal = 0x1;
                break;
            case 1: /* 1  - OTU2          */
                /* case if data sourced from HSCI slice 0 or 1 */
                if((pmmSource == 2)||(pmmSource == 3)){
                    cntNVal = 0x12;
                    cntDVal = 0xFF;
                /* case if data sourced from OTU2 slice 0 or 1 */
                } else {
                    cntNVal = 0x10;
                    cntDVal = 0xFF;
                }
                break;
            case 2: /* 2  - OTU2e         */
                /* case if data sourced from HSCI slice 0 or 1 */
                if((pmmSource == 2)||(pmmSource == 3)){
                    cntNVal = 0x12;
                    cntDVal = 0xFF;
                /* case if data sourced from OTU2 slice 0 or 1 */
                } else {
                    cntNVal = 0x10;
                    cntDVal = 0xFF;
                }
                break;
            case 3: /* 3  - OTU1e         */
                /* case if data sourced from HSCI slice 0 or 1 */
                if((pmmSource == 2)||(pmmSource == 3)){
                    cntNVal = 0x11;
                    cntDVal = 0xFF;
                /* case if data sourced from OTU2 slice 0 or 1 */
                } else {
                    cntNVal = 0x10;
                    cntDVal = 0xFF;
                }
                break;
            case 4: /* 4  - OTU2f         */
                /* case if data sourced from HSCI slice 0 or 1 */
                if((pmmSource == 2)||(pmmSource == 3)){
                    cntNVal = 0x12;
                    cntDVal = 0xFF;
                /* case if data sourced from OTU2 slice 0 or 1 */
                } else {
                    cntNVal = 0x10;
                    cntDVal = 0xFF;
                }
                break;
            case 5: /* 5  - OTU1f         */
                /* case if data sourced from HSCI slice 0 or 1 */
                if((pmmSource == 2)||(pmmSource == 3)){
                    cntNVal = 0x11;
                    cntDVal = 0xFF;
                /* case if data sourced from OTU2 slice 0 or 1 */
                } else {
                    cntNVal = 0x10;
                    cntDVal = 0xFF;
                }
                break;
            case 6: /* 6  - ODU2          */
                /* case if data sourced from HSCI slice 0 or 1 */
                if((pmmSource == 2)||(pmmSource == 3)){
                    cntNVal = 0x2;
                    cntDVal = 0xEF;
                /* case if data sourced from OTU2 slice 0 or 1 */
                } else {
                    cntNVal = 0x0;
                    cntDVal = 0x1;
                }
                break;
            case 7: /* 7  - ODU2e         */
                /* case if data sourced from HSCI slice 0 or 1 */
                if((pmmSource == 2)||(pmmSource == 3)){
                    cntNVal = 0x2;
                    cntDVal = 0xEF;
                /* case if data sourced from OTU2 slice 0 or 1 */
                } else {
                    cntNVal = 0x0;
                    cntDVal = 0x1;
                }
                break;
            case 8: /* 8  - ODU1e         */
                /* case if data sourced from HSCI slice 0 or 1 */
                if((pmmSource == 2)||(pmmSource == 3)){
                    cntNVal = 0x1;
                    cntDVal = 0xEF;
                /* case if data sourced from OTU2 slice 0 or 1 */
                } else {
                    cntNVal = 0x0;
                    cntDVal = 0x1;
                }
                break;
            case 9: /* 9  - ODU2f         */
                /* case if data sourced from HSCI slice 0 or 1 */
                if((pmmSource == 2)||(pmmSource == 3)){
                    cntNVal = 0x2;
                    cntDVal = 0xEF;
                /* case if data sourced from OTU2 slice 0 or 1 */
                } else {
                    cntNVal = 0x0;
                    cntDVal = 0x1;
                }
                break;
            case 10: /* 10 - ODU1f         */
                /* case if data sourced from HSCI slice 0 or 1 */
                if((pmmSource == 2)||(pmmSource == 3)){
                    cntNVal = 0x1;
                    cntDVal = 0xEF;
                /* case if data sourced from OTU2 slice 0 or 1 */
                } else {
                    cntNVal = 0x0;
                    cntDVal = 0x1;
                }
                break;
            case 11: /* 11 - HSCI related XFI with Source C
                             from CBRB PMM transferring
                             ODU1e or ODU1f data */
                /* case if data sourced from CBRB PMM */
                cntNVal = 0xFFFFFF;  /* -1d */
                cntDVal = 0xEE; /* 238d */
                break;
            case 12: /* 12 - HSCI related XFI with Source C
                             from CBRB PMM transferring
                             ODU2, ODU2e or ODU2f data */
                /* case if data sourced from CBRB PMM */
                cntNVal = 0xFFFFFE;  /* -2d */
                cntDVal = 0xED; /* 237d */
                break;
            case 13: /* 13 - OPI PMM being used for POS internal
                             loop timing (only valid for mode == 1) */
                cntNVal = 3;  /* 3d */
                cntDVal = 4; /* 4d */
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }

        /* set the CNT_N field */
        result = hyPhy20gRegFieldWrite (fileHndl, acbCfg2Reg, cntNMsk, cntNVal);
        if(result) return result;

        /* set the CNT_D field */
        result = hyPhy20gRegFieldWrite (fileHndl, acbCfg3Reg, cntDMsk, cntDVal);
        if(result) return result;
    }

    /* field setting specification for mode */
    switch (mode) {
        case 0:
            enablePmmAdapt = 0;
            enableFifoAdapt = 0;
            enableBInt = 0;
            break;
        case 1:
            enablePmmAdapt = 1;
            enableFifoAdapt = 0;
            enableBInt = 0;
            break;
        case 2:
            enablePmmAdapt = 1;
            enableFifoAdapt = 0;
            enableBInt = 1;
            break;
        case 3:
            enablePmmAdapt = 0;
            enableFifoAdapt = 1;
            enableBInt = 0;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /*ENABLE_FIFO_CENT bit should always set to 0 for all modes*/
    enableFifoCent = 0;

    /* set fields of Cfg 1 Reg */
    result = sysHyPhy20gRead(fileHndl, acbCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enableFifoCentMsk,
                                    enableFifoCent);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enablePmmAdaptMsk,
                                    enablePmmAdapt);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enableFifoAdaptMsk,
                                    enableFifoAdapt);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enableBIntMsk, enableBInt);
    if(result) return result;

    /* hard code as for an XFI interface source B must be ODU2 rate */
    result = hyPhy20gVarFieldWrite (&regData, odu2ModMsk, 1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, acbCfg1Reg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gXfiAcbModeSel */

/*******************************************************************************
**
**  hyPhy20gXfiAcbCfgGain
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function modifies the default gain settings as applied
**                  with hyPhy20gXfiAcbInit.
**
**                  This function will test operating mode of the device and
**                  only proceed if the block is enabled and operating in
**                  nominal mode.
**
**                  ACB_GAIN, LP_GAIN and CFG0 to CFG3 may maintain default
**                  settings for ACB phase mode timing,
**
**                  For adaptive FIFO ACB
**                  timing the following settings may be used:
**                  ACB_GAIN 0x8
**                  LP_GAIN 0xF
**                  CFG0 0x3
**
**                  For phase mode timing the following values are used in
**                  hyPhy20gSfpAcbInit:
**                  ACB_GAIN 0x8
**                  LP_GAIN 0xF
**                  CFG0 0x0
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - ACB Instance 1:1 mapped to XFI ports
**                                    0 to 1
**                  acbGain         - ACB_GAIN field setting.
**                  lpGain          - LP_GAIN field setting.
**                  cfg0            - CFG0 field setting.
**                  cfg1            - CFG1 field setting.  OBSOLETE, NO
**                                    DEVICE REGISTER CHANGE BASED ON THIS ARG
**                  cfg2            - CFG2 field setting.  OBSOLETE, NO
**                                    DEVICE REGISTER CHANGE BASED ON THIS ARG
**                  cfg3            - CFG3 field setting.  OBSOLETE, NO
**                                    DEVICE REGISTER CHANGE BASED ON THIS ARG
**                  updateMsk       - Mask of ACB gain related configuration
**                                    fields to be updated.  Set the
**                                    corresponding bit to 1 to update a field,
**                                    set to 0 to maintain value of configured
**                                    field:
**                                    bit 0 - update cfg3 field
**                                    bit 1 - update cfg2 field
**                                    bit 2 - update cfg1 field
**                                    bit 3 - update cfg0 field
**                                    bit 4 - update lpGain field
**                                    bit 5 - update acbGain field
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gXfiAcbCfgGain(struct file *fileHndl, UINT4 slice, UINT4 acbGain,
                           UINT4 lpGain,UINT4 cfg0, UINT4 cfg1, UINT4 cfg2,
                           UINT4 cfg3, UINT4 updateMsk)
{
    /* variable declaration */
    UINT4 cfg1Reg;
    UINT4 acbGainMsk = 0;
    UINT4 cfg2Reg;
    UINT4 lpGainMsk = 0;
    UINT4 cfg0To3Reg;
    UINT4 cfg0Msk = 0;
    UINT4 value, regData;
    INT4 result;

    /* argument checking */
    if((slice > 15)||(acbGain > 15)||
       (lpGain > 15)||
       (cfg0 > 15)||
       (cfg1 > 15)||
       (cfg2 > 15)||
       (cfg3 > 15)||
       (updateMsk > 0x3F)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* test to confirm block in nominal mode before proceeding */
    result = utilHyPhy20gXfiAcbGetMode(fileHndl, slice, &value);
    if(value){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* register accessing parameters */
    cfg1Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CFG_1;
    cfg1Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(cfg1Reg, slice);

    acbGainMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ACB_GAIN_MSK;

    cfg2Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CFG_2;
    cfg2Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(cfg2Reg, slice);

    lpGainMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_2_LP_GAIN_MSK;

    cfg0To3Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CTL;
    cfg0To3Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(cfg0To3Reg, slice);

    cfg0Msk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CTL_CFG0_MSK;

    /* configure the ACB_GAIN field if specified */
    if(updateMsk & 0x20){
        result = hyPhy20gRegFieldWrite (fileHndl, cfg1Reg, acbGainMsk, acbGain);
        if(result) return result;
    }

    /* configure the LP_GAIN field if specified */
    if(updateMsk & 0x10){
        result = hyPhy20gRegFieldWrite (fileHndl, cfg2Reg, lpGainMsk, lpGain);
        if(result) return result;
    }

    /* configure CFG0 to CFG3 values */
    result = sysHyPhy20gRead(fileHndl, cfg0To3Reg, &regData);
    if(result) return result;

    /* configure the CFG0 field if specified */
    if(updateMsk & 0x8){
        result = hyPhy20gVarFieldWrite (&regData, cfg0Msk, cfg0);
        if(result) return result;
    }


    result = sysHyPhy20gWrite(fileHndl, cfg0To3Reg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gXfiAcbCfgGain */



/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/*******************************************************************************
**
**  utilHyPhy20gSfpAcbGetMode
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves ACB data to determine the operating
**                  state of the ACB.
**
**                  The ACB is generating a nominal clock if the returned value
**                  is 0.  For non-zero values the state of the block can be
**                  decoded from the pMode data.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - ACB Instance 1:1 mapped to SFP ports
**                                    0 to 15
**                  *pMode          - pointer to 4 bit mode information
**                                    bit 0 ENABLE_PMM_ADAPT value
**                                    bit 1 ENABLE_FIFO_ADAPT value
**                                    bit 2 ENABLE_B_INT value
**                                    bit 3 ACB_RST value
**                                    The block is in nominal mode if all bits
**                                    are set to 0.
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSfpAcbGetMode(struct file *fileHndl, UINT4 slice, UINT4 *pMode)
{
    /* variable declaration */
    UINT4 acbCfg1Reg, acbRstMsk, enableBIntMsk;
    UINT4 enableFifoAdaptMsk, enablePmmAdaptMsk;
    UINT4 acbRstOff, enableBIntOff, enableFifoAdaptOff, enablePmmAdaptOff;
    UINT4 regData;
    INT4 result;

    /* argument checking */
    if(slice > 15){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    acbCfg1Reg = HYPHY20G_SERDES_SFP_TOP_REG_ACB_CFG_1;
    acbCfg1Reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(acbCfg1Reg, slice);

    acbRstMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ACB_RST_MSK;
    enableBIntMsk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_B_INT_MSK;
    enableFifoAdaptMsk =
                    HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_FIFO_ADAPT_MSK;
    enablePmmAdaptMsk =
                     HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_PMM_ADAPT_MSK;
    acbRstOff = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ACB_RST_OFF;
    enableBIntOff = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_B_INT_OFF;
    enableFifoAdaptOff =
                    HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_FIFO_ADAPT_OFF;
    enablePmmAdaptOff =
                     HYPHY20G_SERDES_SFP_TOP_BIT_ACB_CFG_1_ENABLE_PMM_ADAPT_OFF;


    /* retrieve ACB block mode data */
    result = sysHyPhy20gRead(fileHndl, acbCfg1Reg, &regData);
    if(result) return result;

    *pMode = 0;
    /* ENABLE_PMM_ADAPT value */
    if(mHYPHY20G_GET_FIELD4(regData, enablePmmAdaptMsk, enablePmmAdaptOff)){
        *pMode |= 0x1;
    }
    /* ENABLE_FIFO_ADAPT value */
    if(mHYPHY20G_GET_FIELD4(regData, enableFifoAdaptMsk, enableFifoAdaptOff)){
        *pMode |= 0x2;
    }
    /* ENABLE_B_INT value */
    if(mHYPHY20G_GET_FIELD4(regData, enableBIntMsk, enableBIntOff)){
        *pMode |= 0x4;
    }
    /* ACB_RST value */
    if(mHYPHY20G_GET_FIELD4(regData, acbRstMsk, acbRstOff)){
        *pMode |= 0x8;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gSfpAcbGetMode */


/*******************************************************************************
**
**  utilHyPhy20gSfpAcbGetPmmSource
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function identifies the PMM block associated with
**                  ACB source C input.
**
**                  The source is identifed by recovering SONET_LINE clock
**                  configuration context.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - ACB Instance 1:1 mapped to SFP ports
**                                    0 to 15
**                  *pPmmSource     - pointer to PMM connected to ACB source C
**                                    input
**                                    0 to 7 - OPI OTU1 Framer PMMs 0 to 7
**                                    8 - LSCI block with the same slice index
**                                        as the SERDES/ACB block
**                                    9 - ENET_COM
**                                    10 to 17 - OPI VCAT I/F PMMs 0 to 7
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSfpAcbGetPmmSource(struct file *fileHndl, UINT4 slice,
                                    UINT4 *pPmmSource)
{
    /* variable declaration */
    UINT4 clkCtrlReg = 0;
    UINT4 sfpCkctlcSelMsk = 0;
    UINT4 sfpCkctlcSelOff = 0;
    UINT4 regData;
    UINT4 addr, msk, txCkctlMuxSelVal;
    UINT4 pmmSource;
    INT4 result;

    /* argument checking */
    if(slice > 15){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Retrieve TX_CKCTL_MUX_SEL to identify if LSCI is ACB PMM source */
    addr = HYPHY20G_SERDES_SFP_TOP_REG_CFG_1;
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_SERDES_SFP_TOP_BIT_CFG_1_TX_CKCTL_MUX_SEL_OFF;

    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &txCkctlMuxSelVal);
    if(result) return result;

    if(slice > 9){
        clkCtrlReg = HYPHY20G_SONET_LINE_TOP_REG_SFP_CLK_CTL_1;
        sfpCkctlcSelOff = 5*(slice - 10);
        sfpCkctlcSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_CLK_CTL_1_SFP_CKCTLC_SEL10_MSK;
        sfpCkctlcSelMsk = sfpCkctlcSelMsk << sfpCkctlcSelOff;
    } else if (slice < 4) {
        clkCtrlReg = HYPHY20G_SONET_LINE_TOP_REG_XFI_CLK_CTL;
        sfpCkctlcSelOff = (5*slice);
        sfpCkctlcSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_XFI_CLK_CTL_SFP_CKCTLC_SEL0_MSK;
        sfpCkctlcSelMsk = sfpCkctlcSelMsk << sfpCkctlcSelOff;
    } else {
        clkCtrlReg = HYPHY20G_SONET_LINE_TOP_REG_SFP_CLK_CTL_0;
        sfpCkctlcSelOff = 5*(slice - 4);
        sfpCkctlcSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_CLK_CTL_0_SFP_CKCTLC_SEL4_MSK;
        sfpCkctlcSelMsk = sfpCkctlcSelMsk << sfpCkctlcSelOff;
    }

    result = sysHyPhy20gRead(fileHndl, clkCtrlReg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldRead(regData, sfpCkctlcSelMsk, &pmmSource);
    if(result) return result;

    if(pmmSource != 8 && txCkctlMuxSelVal == 1){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    *pPmmSource = pmmSource;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gSfpAcbGetPmmSource */


/*******************************************************************************
**
**  utilHyPhy20gSfpAcbGetPmmPktData
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves PMM packet size data.
**
**                  The packet size data is retrieved from the PMM block.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - SFP ACB slice making request for PMM data
**                                    0 to 15
**                  pmmAddr         - PMM block to retrieve packet size data
**                                    from:
**                                    0 to 7 - OPI OTU1 Framer PMMs 0 to 7
**                                    8 - LSCI block with the same slice index
**                                        as the SERDES/ACB block
**                                    9 - ENET_COM
**                                    10 to 17 - OPI VCAT I/F PMMs 0 to 7
**
**                  *pMonPktSz     - pointer to packet monitor MON_PKT_SIZE
**                                   setting.
**                  *pMkrPktSz     - pointer to packet maker MKR_PKT_SIZE
**                                   setting.
**                  *pMkrPktSzVar  - pointer to packet maker MKR_PKT_SIZE_VAR
**                                   setting.
**                  *pMkrPktPeriod - pointer to packet maker MKR_PKT_PERIOD
**                                   setting.
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSfpAcbGetPmmPktData(struct file *fileHndl, UINT4 slice,
                                     UINT4 pmmAddr, UINT4 *pMonPktSz,
                                     UINT4 *pMkrPktSz, UINT4 *pMkrPktSzVar,
                                     UINT4 *pMkrPktPeriod)
{
    /* variable declaration */
    UINT4 monReg = 0;
    UINT4 mkrCfgReg1 = 0;
    UINT4 mkrCfgReg2 = 0;
    UINT4 monPktSzOff = 0;
    UINT4 monPktSzMsk = 0;
    UINT4 mkrPktSzOff = 0;
    UINT4 mkrPktSzMsk = 0;
    UINT4 mkrPktSzVarOff = 0;
    UINT4 mkrPktSzVarMsk = 0;
    UINT4 mkrPktPeriodOff = 0;
    UINT4 mkrPktPeriodMsk = 0;
    UINT4 regData;
    INT4 result;

    /* argument checking */
    if((slice > 15)||(pmmAddr > 17)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if(pmmAddr < 8 || pmmAddr > 9){
        /* OPI OTU1 Framer PMM or OPI ODUKPP VCAT I/F PMM */
        monReg = HYPHY20G_OPI_TOP_REG_PKT_MON_CFG_2_OFFSET(pmmAddr);
        monPktSzOff = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_2_MON_PKT_SIZE_OFF;
        monPktSzMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_2_MON_PKT_SIZE_MSK;

        mkrCfgReg1 = HYPHY20G_OPI_TOP_REG_PKT_MKR_CFG_1_OFFSET(pmmAddr);
        mkrPktPeriodOff = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_PKT_PERIOD_OFF;
        mkrPktPeriodMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_PKT_PERIOD_MSK;
        mkrPktSzVarOff =
                        HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_PKT_SIZE_VAR_OFF;
        mkrPktSzVarMsk =
                        HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_PKT_SIZE_VAR_MSK;

        mkrCfgReg2 = HYPHY20G_OPI_TOP_REG_PKT_MKR_CFG_2_OFFSET(pmmAddr);
        mkrPktSzOff = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_2_MKR_PKT_SIZE_OFF;
        mkrPktSzMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_2_MKR_PKT_SIZE_MSK;

    } else if(pmmAddr == 8) {
        /* LSCI PMM */
        monReg = HYPHY20G_LSCI_TOP_REG_PAC_MON_CFG_2;
        monReg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(monReg, slice);
        monPktSzOff = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_2_MON_PKT_SIZE_OFF;
        monPktSzMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_2_MON_PKT_SIZE_MSK;

        mkrCfgReg1 = HYPHY20G_LSCI_TOP_REG_PAC_MKR_CFG_1;
        mkrCfgReg1 = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(mkrCfgReg1, slice);
        mkrPktPeriodOff =
                         HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_PERIOD_OFF;
        mkrPktPeriodMsk =
                         HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_PERIOD_MSK;
        mkrPktSzVarOff =
                       HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_SIZE_VAR_OFF;
        mkrPktSzVarMsk =
                       HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_SIZE_VAR_MSK;

        mkrCfgReg2 = HYPHY20G_LSCI_TOP_REG_PAC_MKR_CFG_2;
        mkrCfgReg2 = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(mkrCfgReg2, slice);
        mkrPktSzOff = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_2_MKR_PKT_SIZE_OFF;
        mkrPktSzMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_2_MKR_PKT_SIZE_MSK;
    } else {
        /* ENET_COM PMM */
        // temporary return as ENET_COM PMM configuration requirements
        // being explored
        return HYPHY20G_ERR_NO_SW_CONTENT;
    }

    result = sysHyPhy20gRead(fileHndl, monReg, &regData);
    if(result) return result;

    *pMonPktSz = mHYPHY20G_GET_FIELD4(regData, monPktSzMsk, monPktSzOff);

    result = sysHyPhy20gRead(fileHndl, mkrCfgReg1, &regData);
    if(result) return result;

    *pMkrPktSzVar = mHYPHY20G_GET_FIELD4(regData, mkrPktSzVarMsk,
                                        mkrPktSzVarOff);
    *pMkrPktPeriod = mHYPHY20G_GET_FIELD4(regData, mkrPktPeriodMsk,
                                        mkrPktPeriodOff);

    result = sysHyPhy20gRead(fileHndl, mkrCfgReg2, &regData);
    if(result) return result;

    *pMkrPktSz = mHYPHY20G_GET_FIELD4(regData, mkrPktSzMsk, mkrPktSzOff);

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gSfpAcbGetPmmPktData */

/*******************************************************************************
**
**  utilHyPhy20gXfiAcbGetMode
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves ACB data to determine the operating
**                  state of the ACB.
**
**                  The ACB is generating a nominal clock if the returned value
**                  is 0.  For non-zero values the state of the block can be
**                  decoded from the pMode data.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - ACB Instance 1:1 mapped to XFI ports
**                                    0 to 1
**                  *pMode          - pointer to 4 bit mode information
**                                    bit 0 ENABLE_PMM_ADAPT value
**                                    bit 1 ENABLE_FIFO_ADAPT value
**                                    bit 2 ENABLE_B_INT value
**                                    bit 3 ACB_RST value
**                                    The block is in nominal mode if all bits
**                                    are set to 0.
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gXfiAcbGetMode(struct file *fileHndl, UINT4 slice, UINT4 *pMode)
{
    /* variable declaration */
    UINT4 acbCfg1Reg, acbRstMsk, enableBIntMsk;
    UINT4 enableFifoAdaptMsk, enablePmmAdaptMsk;
    UINT4 acbRstOff, enableBIntOff, enableFifoAdaptOff, enablePmmAdaptOff;
    UINT4 regData;
    INT4 result;

    /* argument checking */
    if(slice > 1){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    acbCfg1Reg = HYPHY20G_SERDES_XFI_TOP_REG_ACB_CFG_1;
    acbCfg1Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(acbCfg1Reg, slice);

    acbRstMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ACB_RST_MSK;
    enableBIntMsk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_B_INT_MSK;
    enableFifoAdaptMsk =
                    HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_FIFO_ADAPT_MSK;
    enablePmmAdaptMsk =
                     HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_PMM_ADAPT_MSK;
    acbRstOff = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ACB_RST_OFF;
    enableBIntOff = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_B_INT_OFF;
    enableFifoAdaptOff =
                    HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_FIFO_ADAPT_OFF;
    enablePmmAdaptOff =
                     HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CFG_1_ENABLE_PMM_ADAPT_OFF;


    /* retrieve ACB block mode data */
    result = sysHyPhy20gRead(fileHndl, acbCfg1Reg, &regData);
    if(result) return result;

    *pMode = 0;
    /* ENABLE_PMM_ADAPT value */
    if(mHYPHY20G_GET_FIELD4(regData, enablePmmAdaptMsk, enablePmmAdaptOff)){
        *pMode |= 0x1;
    }
    /* ENABLE_FIFO_ADAPT value */
    if(mHYPHY20G_GET_FIELD4(regData, enableFifoAdaptMsk, enableFifoAdaptOff)){
        *pMode |= 0x2;
    }
    /* ENABLE_B_INT value */
    if(mHYPHY20G_GET_FIELD4(regData, enableBIntMsk, enableBIntOff)){
        *pMode |= 0x4;
    }
    /* ACB_RST value */
    if(mHYPHY20G_GET_FIELD4(regData, acbRstMsk, acbRstOff)){
        *pMode |= 0x8;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gXfiAcbGetMode */

/*******************************************************************************
**
**  utilHyPhy20gXfiAcbGetPmmSource
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function identifies the PMM block associated with
**                  ACB source C input.
**
**                  The source is identifed by recovering SONET_LINE clock
**                  configuration context.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - ACB Instance 1:1 mapped to XFI ports
**                                    0 to 1
**                  *pPmmSource     - pointer to PMM connected to ACB source C
**                                    input
**                                    0 - OTU2 #0
**                                    1 - OTU2 #1
**                                    2 - HSCI #2
**                                    3 - HSCI #3
**                                    4 - ENET_COM
**                                    5 - ODU2 VCAT I/F #0
**                                    6 - ODU2 VCAT I/F #1
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gXfiAcbGetPmmSource(struct file *fileHndl, UINT4 slice,
                                    UINT4 *pPmmSource)
{
    /* variable declaration */
    UINT4 clkCtrlReg = HYPHY20G_SONET_LINE_TOP_REG_XFI_CLK_CTL;
    UINT4 xfiCkctlcSelMsk =
                    HYPHY20G_SONET_LINE_TOP_BIT_XFI_CLK_CTL_XFI_CKCTLC_SEL0_MSK;
    UINT4 xfiCkctlcSelOff =
                    HYPHY20G_SONET_LINE_TOP_BIT_XFI_CLK_CTL_XFI_CKCTLC_SEL0_OFF;
    UINT4 regData;
    INT4 result;

    /* argument checking */
    if(slice > 1){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if(slice){
        xfiCkctlcSelMsk =
                    HYPHY20G_SONET_LINE_TOP_BIT_XFI_CLK_CTL_XFI_CKCTLC_SEL1_MSK;
        xfiCkctlcSelOff =
                    HYPHY20G_SONET_LINE_TOP_BIT_XFI_CLK_CTL_XFI_CKCTLC_SEL1_OFF;

    }

    result = sysHyPhy20gRead(fileHndl, clkCtrlReg, &regData);
    if(result) return result;

    *pPmmSource = mHYPHY20G_GET_FIELD4(regData, xfiCkctlcSelMsk,
                                      xfiCkctlcSelOff);

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gXfiAcbGetPmmSource */


/*******************************************************************************
**
**  utilHyPhy20gXfiAcbGetPmmPktData
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves PMM packet size data.
**
**                  The packet size data is retrieved from the PMM block.
**
**  INPUTS:         fileHndl        - device handle
**                  pmmAddr         - PMM block to retrieve packet size data
**                                    from:
**                                    0 - OTU2 #0
**                                    1 - OTU2 #1
**                                    2 - HSCI #0
**                                    3 - HSCI #1
**                                    4 - ENET_COM
**                                    5 - ODU2 VCAT I/F #0
**                                    6 - ODU2 VCAT I/F #1
**                  *pMonPktSz     - pointer to packet monitor MON_PKT_SIZE
**                                   setting.
**                  *pMkrPktSz     - pointer to packet maker MKR_PKT_SIZE
**                                   setting.
**                  *pMkrPktSzVar  - pointer to packet maker MKR_PKT_SIZE_VAR
**                                   setting.
**                  *pMkrPktPeriod - pointer to packet maker MKR_PKT_PERIOD
**                                   setting.
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 utilHyPhy20gXfiAcbGetPmmPktData(struct file *fileHndl,
                                     UINT4 pmmAddr, UINT4 *pMonPktSz,
                                     UINT4 *pMkrPktSz, UINT4 *pMkrPktSzVar,
                                     UINT4 *pMkrPktPeriod)
{
    /* variable declaration */
    UINT4 monReg = 0;
    UINT4 mkrCfgReg1 = 0;
    UINT4 mkrCfgReg2 = 0;
    UINT4 monPktSzOff = 0;
    UINT4 monPktSzMsk = 0;
    UINT4 mkrPktSzOff = 0;
    UINT4 mkrPktSzMsk = 0;
    UINT4 mkrPktSzVarOff = 0;
    UINT4 mkrPktSzVarMsk = 0;
    UINT4 mkrPktPeriodOff = 0;
    UINT4 mkrPktPeriodMsk = 0;
    UINT4 opiPmmAddr = 0;
    UINT4 regData;
    INT4 result;

    /* argument checking */
    if(pmmAddr > 6){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if(pmmAddr < 2 || pmmAddr > 4){
        if(pmmAddr < 2){
            opiPmmAddr = pmmAddr+8;
        } else {
            opiPmmAddr = 10 + 4*(pmmAddr - 5);
        }
        monReg = HYPHY20G_OPI_TOP_REG_PKT_MON_CFG_2_OFFSET(opiPmmAddr);
        monPktSzOff = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_2_MON_PKT_SIZE_OFF;
        monPktSzMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_2_MON_PKT_SIZE_MSK;

        mkrCfgReg1 = HYPHY20G_OPI_TOP_REG_PKT_MKR_CFG_1_OFFSET(opiPmmAddr);
        mkrPktPeriodOff = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_PKT_PERIOD_OFF;
        mkrPktPeriodMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_PKT_PERIOD_MSK;
        mkrPktSzVarOff =
                        HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_PKT_SIZE_VAR_OFF;
        mkrPktSzVarMsk =
                        HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_PKT_SIZE_VAR_MSK;

        mkrCfgReg2 = HYPHY20G_OPI_TOP_REG_PKT_MKR_CFG_2_OFFSET(opiPmmAddr);
        mkrPktSzOff = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_2_MKR_PKT_SIZE_OFF;
        mkrPktSzMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_2_MKR_PKT_SIZE_MSK;

    } else if (pmmAddr < 4) {
        monReg = HYPHY20G_HSCI_TOP_REG_PAC_MON_CFG_2;
        monReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(monReg, pmmAddr-2);
        monPktSzOff = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_2_MON_PKT_SIZE_OFF;
        monPktSzMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_2_MON_PKT_SIZE_MSK;

        mkrCfgReg1 = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_1;
        mkrCfgReg1 = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(mkrCfgReg1, pmmAddr-2);
        mkrPktPeriodOff =
                         HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_PERIOD_OFF;
        mkrPktPeriodMsk =
                         HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_PERIOD_MSK;
        mkrPktSzVarOff =
                       HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_SIZE_VAR_OFF;
        mkrPktSzVarMsk =
                       HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_SIZE_VAR_MSK;

        mkrCfgReg2 = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_2;
        mkrCfgReg2 = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(mkrCfgReg2, pmmAddr-2);
        mkrPktSzOff = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_2_MKR_PKT_SIZE_OFF;
        mkrPktSzMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_2_MKR_PKT_SIZE_MSK;
    } else {
        // temporary return as ENET_COM PMM configuration requirements
        // being explored
        return HYPHY20G_ERR_NO_SW_CONTENT;
    }

    result = sysHyPhy20gRead(fileHndl, monReg, &regData);
    if(result) return result;

    *pMonPktSz = mHYPHY20G_GET_FIELD4(regData, monPktSzMsk, monPktSzOff);

    result = sysHyPhy20gRead(fileHndl, mkrCfgReg1, &regData);
    if(result) return result;

    *pMkrPktSzVar = mHYPHY20G_GET_FIELD4(regData, mkrPktSzVarMsk,
                                        mkrPktSzVarOff);
    *pMkrPktPeriod = mHYPHY20G_GET_FIELD4(regData, mkrPktPeriodMsk,
                                        mkrPktPeriodOff);

    result = sysHyPhy20gRead(fileHndl, mkrCfgReg2, &regData);
    if(result) return result;

    *pMkrPktSz = mHYPHY20G_GET_FIELD4(regData, mkrPktSzMsk, mkrPktSzOff);

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gXfiAcbGetPmmPktData */

/* end of file */
