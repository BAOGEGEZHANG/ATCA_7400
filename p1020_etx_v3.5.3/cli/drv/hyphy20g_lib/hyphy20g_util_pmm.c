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
**  FILE        : hyphy20g_util_pmm.c
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
#include "hyphy20g_util_pmm.h"

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
**  hyPhy20gHsciPmmInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes a PMM instance associated
**                  with the HSCI subsystem.
**
**                  It configures the required packet size for the specified
**                  application and other default settings.
**
**                  For signals that are transparently mapped to ODU2/2e/2f
**                  through OTNMA, asynchronous mapping is only applicable to
**                  STS-192 payload type.
**
**                  For cases where payload is synchronously demapped from
**                  OTNMD there will be no dithered packets generated in
**                  CPB from the OTNMD, the ACB is relying on PMM rate 
**                  indication from the CBRB PMM transferring the data to VCAT
**                  subsystem.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - HSCI PMM instance
**                                    0 to 1
**                  payloadType     - Featured application of the HSCI
**                                    instance that requires packetization of
**                                    CBR stream in CPB block:
**                                     0 - Disabled (held in RESET)
**                                         required for MAC terminated clients
**                                     1 - 8G FC
**                                     2 - 10G FC
**                                     3 - Gsup43 Clause 7.1 and 7.2
**                                         10GE transparent
**                                     4 - STS-192 transparent
**                                     5 - CPRI 9.8304 Gbps
**                  timingMode         Transmit timing mode for associated Tx
**                                     Serdes:
**                                     0 - Adaptive FIFO
**                                         source data stream received from
**                                         CPB non dithered packet source:
**                                         - GFP-T AA demapped 8b10b CBR clients
**                                         - 8G FC
**                                         - CPRI 9.8304 Gbps
**                                     1 - Source C phase from CBRB PMM
**                                         source data stream sync demapped from
**                                         ODU2e/2f/1e/1f applicable to
**                                         CBR 10G FC and 10GE
**                                         Not valid for 8G FC mode and CPRI
**                                         9.8304 Gbps
**                                     2 - Source C phase from HSCI PMM
**                                         source data stream received from
**                                         CPB dithered packet source:
**                                         - sync/async demapped STS-192
**                                         - GFP-T FA demapped 8b10b CBR clients
**                                           applicable to 8G FC transported as
**                                           8b10b CBR client
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciPmmInit(struct file *fileHndl, UINT4 slice, UINT4 payloadType,
                         UINT4 timingMode)
{
    /* variable declaration */
    UINT4 addr;
    UINT4 fcCbrBusWidthMsk = HYPHY20G_HSCI_FC_CBR_BIT_CFG_BUS_WIDTH_MSK;
    UINT4 pmmRstMsk = HYPHY20G_HSCI_TOP_BIT_CFG_PMM_RST_MSK;
    UINT4 hsciCfgReg;
    UINT4 mkrPktPeriodMsk =
                         HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_PERIOD_MSK;
    UINT4 mkrLosDisMsk =
                        HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_LOS_DISABLE_MSK;
    UINT4 mkrLosDisFcCbrMsk =
                 HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_LOS_DISABLE_FC_CBR_MSK;
    UINT4 mkrPktSzVarMsk =
                       HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_SIZE_VAR_MSK;
    UINT4 dsmTypeMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_DSM_TYPE_MSK;
    UINT4 mkrDithEnMsk =
                        HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_DITH_ENABLE_MSK;
    UINT4 mkrEnMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_ENABLE_MSK;
    UINT4 mkrCfg1Reg;
    UINT4 mkrPktSzMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_2_MKR_PKT_SIZE_MSK;
    UINT4 busWidthMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_2_BUS_WIDTH_MSK;
    UINT4 mkrCfg2Reg;
    UINT4 mkrDsmDenMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_3_MKR_DSM_DENOM_MSK;
    UINT4 mkrCfg3Reg;
    UINT4 clkCtrlEnMsk =
                   HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_ADPV_CLK_CTRL_ENABLE_MSK;
    UINT4 monLpBkMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_LOOPBACK_MSK;
    UINT4 monLpTmMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_LOOPTIME_MSK;
    UINT4 monCntEnMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_CNT_ENABLE_MSK;
    UINT4 monEnMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_ENABLE_MSK;
    UINT4 monCfg1Reg;
    UINT4 monPktSzMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_2_MON_PKT_SIZE_MSK;
    UINT4 predDenMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_2_PREDICTOR_DENOM_MSK;
    UINT4 predNumMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_2_PREDICTOR_NUMER_MSK;
    UINT4 monCfg2Reg;
    UINT4 mkrPktPeriodVal, mkrLosDisVal, mkrLosDisFcCbrVal;
    UINT4 mkrPktSzVarVal, dsmTypeVal, mkrDithEnVal, mkrEnVal;
    UINT4 mkrPktSzVal, busWidthVal;
    UINT4 mkrDsmDenVal;
    UINT4 clkCtrlEnVal, monLpBkVal, monLpTmVal, monCntEnVal, monEnVal;
    UINT4 monPktSzVal, predDenVal, predNumVal;
    UINT4 regData;
    UINT4 revId;
    UINT4 rateKbits = 0;
    INT4 result;

    /* argument checking */
    if((slice > 1)||(payloadType > 5)||(timingMode > 2)||
       ((payloadType == 2 || payloadType == 3 || payloadType == 0)&&(timingMode == 2))){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* get device revision */
    result = sysHyPhy20gGetDeviceRevId(fileHndl, &revId);
    if (result) return result;

    hsciCfgReg = HYPHY20G_HSCI_TOP_REG_CFG;
    hsciCfgReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(hsciCfgReg, slice);
    mkrCfg1Reg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_1;
    mkrCfg1Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(mkrCfg1Reg, slice);
    mkrCfg2Reg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_2;
    mkrCfg2Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(mkrCfg2Reg, slice);
    mkrCfg3Reg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_3;
    mkrCfg3Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(mkrCfg3Reg, slice);
    monCfg1Reg = HYPHY20G_HSCI_TOP_REG_PAC_MON_CFG_1;
    monCfg1Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(monCfg1Reg, slice);
    monCfg2Reg = HYPHY20G_HSCI_TOP_REG_PAC_MON_CFG_2;
    monCfg2Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(monCfg2Reg, slice);

    /* generate default settings for non-phase mode CPB packet generation
       settings to be modified on a case by case basis relative to user
       specified application */

    mkrPktPeriodVal = 0;
    mkrLosDisVal = 1;
    mkrLosDisFcCbrVal = 0;
    mkrPktSzVarVal = 0;
    dsmTypeVal = 1;
    mkrDithEnVal = 0;
    mkrEnVal = 0;

    mkrPktSzVal = 0xC8;
    busWidthVal = 0;
    mkrDsmDenVal = 1024;

    clkCtrlEnVal = 0;
    monLpBkVal = 0;
    monLpTmVal = 0;
    monCntEnVal = 0;
    monEnVal = 0;

    monPktSzVal = 0xC8;
    predDenVal = 1;
    predNumVal = 0;

    /* implement configuration settings changes from default */
    switch (payloadType) {
        case 0:
            /*  0 - Disabled (held in RESET)
                    required for MAC terminated clients  */
            break;
        case 1: /* Different values used if GFP-T AA vs GFP-T FA mode used */
            /*  1 - 8G FC   */
            if(timingMode == 1){ /* reject invalid timing mode for payload */
                return HYPHY20G_ERR_INVALID_ARG;
            }
            busWidthVal = 0x0; /* 8b10b encoded */
            rateKbits = 8000000;
            /* phase mode specific configuration parameters
               8G FC potentially transferred in GFP-T FA mode */
            if(timingMode){
                mkrPktPeriodVal = 139;
                mkrPktSzVal = 380;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            } else { /* fifo mode specific settings */
                mkrPktSzVal = 64; /* value required for GFP-T AA mapping */
                monPktSzVal = mkrPktSzVal;
                mkrPktSzVarVal = 1;
                mkrPktPeriodVal = 23;
            }
            mkrLosDisVal = 0;
            mkrLosDisFcCbrVal = 1;
            break;
        case 2:
            /*  2 - 10G FC  */
            if(timingMode == 2){ /* reject invalid timing mode for payload */
                return HYPHY20G_ERR_INVALID_ARG;
            }
            /* 10G FC potentially mapped to ODU1f or ODU2f through OTNMA or 
               looped back to a PMM monitor */
            busWidthVal = 0x1; /* non 8b10b encoded */
            rateKbits = 10518750;
            mkrPktPeriodVal = 119;
            mkrPktSzVal = 503;
            mkrPktSzVarVal = 0;
            dsmTypeVal = 1;
            mkrDithEnVal = 1;
            mkrDsmDenVal = 1024;
            monPktSzVal = mkrPktSzVal;
            predDenVal =1;
            mkrLosDisVal = 1;
            mkrLosDisFcCbrVal = 0;
            break;
        case 3:
            /*  3 - 10GE LAN */
            if(timingMode == 2){ /* reject invalid timing mode for payload */
                return HYPHY20G_ERR_INVALID_ARG;
            }
            /* 10GE LAN potentially mapped to ODU1e or ODU2e through OTNMA 
               or looped back to a PMM monitor */
            busWidthVal = 0x1; /* non 8b10b encoded */
            rateKbits = 10312500;
            mkrPktPeriodVal = 123;
            mkrPktSzVal = 510;
            mkrPktSzVarVal = 0;
            dsmTypeVal = 1;
            mkrDithEnVal = 1;
            mkrDsmDenVal = 1024;
            monPktSzVal = mkrPktSzVal;
            predDenVal =1;
            mkrLosDisVal = 1;
            mkrLosDisFcCbrVal = 0;
            break;
        case 4:
            /* 4 - STS-192 transparent */
            /* STS-192 potentially sync mapped or async mapped to ODU2 through
               OTNMA or looped back to a PMM monitor */
            busWidthVal = 0x1; /* non 8b10b encoded */
            rateKbits = 9953280;
            mkrPktPeriodVal = 127;
            mkrPktSzVal = 508;
            mkrPktSzVarVal =0;
            dsmTypeVal = 1;
            mkrDithEnVal = 1;
            mkrDsmDenVal = 1024;
            monPktSzVal = mkrPktSzVal;
            predDenVal =1;
            mkrLosDisVal = 1;
            mkrLosDisFcCbrVal = 0;
            break;
        case 5:
            /* 5 - CPRI 9.8304 Gbps */
            if(timingMode == 1){ /* reject invalid timing mode for payload */
                return HYPHY20G_ERR_INVALID_ARG;
            }
            busWidthVal = 0x0; /* 8b10b encoded */
            rateKbits = 9830400;
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 100;
                mkrPktSzVal = 316;
                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            } else { /* fifo mode specific settings */
                mkrPktSzVal = 64; /* value required for GFP-T AA mapping */
                monPktSzVal = mkrPktSzVal;
                mkrPktSzVarVal = 1;
                mkrPktPeriodVal = 20;
            }
            mkrLosDisVal = 1;
            mkrLosDisFcCbrVal = 0;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* set PMM_RESET = 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, hsciCfgReg, pmmRstMsk, 1);
    if(result) return result;

    if(payloadType == 0) return HYPHY20G_SUCCESS;

    /* configure bus width for HSCI FC_CBR block. Note the bus width encoding
       is different with bus width register in PMM */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_CFG,
                                               slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, fcCbrBusWidthMsk,
                                   (0x5 - busWidthVal));
    if(result) return result;

    /* configure FC_CBR DLOLB event integration periods - an integration
       period of 100 ms is applied */

    result = hsciHyPhy20gHsciRxFcDlolbIntPer(fileHndl, slice, 100,
                                             rateKbits,
                                             (0x5 - busWidthVal));
    if(result) return result;

    /* configure Mon Cfg Reg 1 */
    result = sysHyPhy20gRead(fileHndl, monCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,  clkCtrlEnMsk,  clkCtrlEnVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,  monLpBkMsk,  monLpBkVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monLpTmMsk, monLpTmVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monCntEnMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monEnMsk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfg1Reg, regData);
    if(result) return result;

    /* configure Mon Cfg Reg 2 */
    result = sysHyPhy20gRead(fileHndl, monCfg2Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monPktSzMsk, monPktSzVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, predDenMsk, predDenVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, predNumMsk, predNumVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfg2Reg, regData);
    if(result) return result;

    /* configure Mkr Cfg Reg 3 */
    result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg3Reg, mkrDsmDenMsk,
                                    mkrDsmDenVal);
    if(result) return result;

    /* configure Mkr Cfg Reg 2 */
    result = sysHyPhy20gRead(fileHndl, mkrCfg2Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktSzMsk, mkrPktSzVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, busWidthMsk, busWidthVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, mkrCfg2Reg, regData);
    if(result) return result;

    /* configure Mkr Cfg Reg 1 */
    result = sysHyPhy20gRead(fileHndl, mkrCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktPeriodMsk, mkrPktPeriodVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrLosDisMsk, mkrLosDisVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrLosDisFcCbrMsk,
                                    mkrLosDisFcCbrVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktSzVarMsk, mkrPktSzVarVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, dsmTypeMsk, dsmTypeVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrDithEnMsk, mkrDithEnVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrEnMsk, mkrEnVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, mkrCfg1Reg, regData);
    if(result) return result;

    /* set PMM_RESET = 0 */
    result = hyPhy20gRegFieldWrite (fileHndl, hsciCfgReg, pmmRstMsk, 0);
    if(result) return result;

    /* remove FC_CBR reset */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_CFG,
                                               slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_FC_CBR_BIT_CFG_SOFT_RST_OFF, 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciPmmInit */

/*******************************************************************************
**
**  hyPhy20gHsciPmmDsmCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs PMM DSM configuration.  It may be
**                  called after hyPhy20gHsciPmmInit and prior to enabling
**                  PMM processing with hyPhy20gHsciPmmMonEnable and
**                  hyPhy20gHsciPmmMkrEnable.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - PMM instance
**                                    0 to 1
**                  dsmMode         - DSM Mode
**                                    0 - 1st order
**                                    1 - MASH11 2nd order
**                                    2 - Standard 2nd order
**                  dsmDenom        - Maker Delta Sigma Modulator denominator
**                                    value, 14 bits, legal range:
**                                    0 to 0x3FFF
**                  predDenom       - Monitor Predictor Delta Sigma Modulator
**                                    denominator  value, 8 bits, legal range:
**                                    0 to 0xFF
**                  predNum         - Monitor Predictor Delta Sigma Modulator
**                                    numerator value, 8 bits, legal range:
**                                    0 to 0xFF
**                  updateMsk       - Mask of DSM related configuration field
**                                    to updated.  Set the corresponding bit
**                                    to 1 to update a field, set to 0 to
**                                    maintain value of configured field:
**                                    bit 0 - update predNum field
**                                    bit 1 - update predDenom field
**                                    bit 2 - update dsmDenom field
**                                    bit 3 - update dsmMode field
**
**
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciPmmDsmCfg(struct file *fileHndl, UINT4 slice, UINT4 dsmMode,
                          UINT4 dsmDenom, UINT4 predDenom, UINT4 predNum,
                          UINT4 updateMsk)
{
    /* variable declaration */
    UINT4 dsmTypeMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_DSM_TYPE_MSK;
    UINT4 mkrCfg1Reg;
    UINT4 mkrDsmDenMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_3_MKR_DSM_DENOM_MSK;
    UINT4 mkrCfg3Reg;
    UINT4 predDenMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_2_PREDICTOR_DENOM_MSK;
    UINT4 predNumMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_2_PREDICTOR_NUMER_MSK;
    UINT4 monCfg2Reg;
    INT4 result;


    /* argument checking */
    if((slice > 1)||(dsmMode > 2)||(dsmDenom > 0x3FFF)||(predDenom > 0xFF)||
       (predNum > 0xFF)||(updateMsk > 0xF)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    mkrCfg1Reg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_1;
    mkrCfg1Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(mkrCfg1Reg, slice);
    mkrCfg3Reg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_3;
    mkrCfg3Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(mkrCfg3Reg, slice);
    monCfg2Reg = HYPHY20G_HSCI_TOP_REG_PAC_MON_CFG_2;
    monCfg2Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(monCfg2Reg, slice);

    /* configure the MKR_DSM_TYPE field if specified */
    if(updateMsk & 0x8){
        result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg1Reg, dsmTypeMsk,
                                        dsmMode);
        if(result) return result;
    }

    /* configure the DSM_DENOM field if specified */
    if(updateMsk & 0x4){
        result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg3Reg, mkrDsmDenMsk,
                                        dsmDenom);
        if(result) return result;
    }

    /* configure the DSM_DENOM field if specified */
    if(updateMsk & 0x2){
        result = hyPhy20gRegFieldWrite (fileHndl, monCfg2Reg, predDenMsk,
                                        predDenom);
        if(result) return result;
    }

    /* configure the DSM_DENOM field if specified */
    if(updateMsk & 0x1){
        result = hyPhy20gRegFieldWrite (fileHndl, monCfg2Reg, predNumMsk,
                                        predNum);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciPmmDsmCfg */




/*******************************************************************************
**
**  hyPhy20gHsciPmmMonEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables the operating mode of the packet
**                  monitor.
**
**                  This function also configures RD_ZONE_START bit in the
**                  FC_CBR trasmit block. This bit can only change when no
**                  egress data trasmit from CPB to FC_CBR block.
**
**                  It will unconditionally set the packet monitor to send
**                  packet size data to ACB blocks unless 'disabled' mode is
**                  selected.
**
**                  When the datapath is loopbacking received data from
**                  companion packet maker block, PMM looptiming will be
**                  selected.  This mode requires the associated ACB to be
**                  configured to use Source C as the timing reference source.
**                  PMM loopback operating mode is not supported for 8G FC
**                  payload types.
**                  8G FC uses transmit rate adaption which is not
**                  supported in PMM loopback.  Transmit rate adaption
**                  loopback is only supported when the data stream is
**                  loopedbacked within CPB as the rate adaption mechanism
**                  uses CPB FIFO depth to make character insertion/deletion
**                  decisions. If these application rates are treated as CBR
**                  clients  PMM loopback may be used.
**
**                  Prior to enabling the PMM loopback (mode == 2), user
**                  must disable and unprovision the corresponding CPB channel,
**                  if it has been previously enabled, to protect the channel
**                  from overflow/underflow within CPB.
**
**                  To switch over Packet Monitor Operating Modes, user must
**                  disable the Packet Maker at first, and enable the
**                  Packet Maker after Packet Monitor mode switch completed
**                  (when the mode == 1 or 2).
**                  hyPhy20gHsciPmmMkrEnable API is used for enabling/disabling
**                  the Packet Maker.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - PMM instance
**                                    0 to 1
**                  mode            - Monitor Operating Mode
**                                    0 - disabled
**                                    1 - enabled for normal data path
**                                    2 - enabled to loopback data received
**                                        from companion packet maker block
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciPmmMonEnable(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    /* variable declaration */
    UINT4 fcCbrCfgReg;
    UINT4 fcTxCfgReg;
    UINT4 preZoneVal;
    UINT4 rdZoneVal     = 0;
    UINT4 rdZoneMsk     = HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_RD_ZONE_START_MSK;
    UINT4 fcCbrRstOff   = HYPHY20G_HSCI_FC_CBR_BIT_CFG_SOFT_RST_OFF;
    UINT4 clkCtrlEnMsk =
                   HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_ADPV_CLK_CTRL_ENABLE_MSK;
    UINT4 monLpBkMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_LOOPBACK_MSK;
    UINT4 monLpTmMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_LOOPTIME_MSK;
    UINT4 monCntEnMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_CNT_ENABLE_MSK;
    UINT4 monEnMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_ENABLE_MSK;
    UINT4 monCfg1Reg;
    UINT4 cntEnableVal = 0;
    UINT4 enableVal = 0;
    UINT4 lpTmVal = 0;
    UINT4 lpBkVal = 0;
    UINT4 clkCtrlEnVal = 0;
    UINT4 regData;
    INT4 result;

    /* argument checking */
    if((slice > 1)||(mode > 2)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (mode) {
        case 0:
            cntEnableVal = 0;
            clkCtrlEnVal = cntEnableVal;
            enableVal = 0;
            lpTmVal = 0;
            lpBkVal = 0;
            rdZoneVal = 2;
            break;
        case 1:
            cntEnableVal = 1;
            clkCtrlEnVal = cntEnableVal;
            enableVal = 1;
            lpTmVal = 0;
            lpBkVal = 0;
            rdZoneVal = 2;
            break;
        case 2:
            cntEnableVal = 1;
            clkCtrlEnVal = cntEnableVal;
            enableVal = 1;
            lpTmVal = 1;
            lpBkVal = 1;
            rdZoneVal = 0;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure RD_ZONE_START bit in the FC_CBR block */
    fcCbrCfgReg = HYPHY20G_HSCI_FC_CBR_REG_CFG;
    fcCbrCfgReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(fcCbrCfgReg, slice);
    result = sysHyPhy20gBitRead(fileHndl, fcCbrCfgReg, fcCbrRstOff, &regData);
    if(result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, fcCbrCfgReg, fcCbrRstOff, 1);
    if(result) return result;

    fcTxCfgReg = HYPHY20G_HSCI_FC_CBR_REG_TX_CFG;
    fcTxCfgReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(fcTxCfgReg, slice);
    result = hyPhy20gRegFieldRead(fileHndl, fcTxCfgReg,
                                  rdZoneMsk, &preZoneVal);
    if(result) return result;

    if(rdZoneVal != preZoneVal) {
        result = hyPhy20gRegFieldWrite(fileHndl, fcTxCfgReg,
                                       rdZoneMsk, rdZoneVal);
        if(result) return result;
    }

    /* restore the previous setting */
    if(!regData) {
        result = sysHyPhy20gBitWrite(fileHndl, fcCbrCfgReg, fcCbrRstOff, 0);
        if(result) return result;
    }

    /* configure Packet Monitor */
    monCfg1Reg = HYPHY20G_HSCI_TOP_REG_PAC_MON_CFG_1;
    monCfg1Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(monCfg1Reg, slice);

    result = sysHyPhy20gRead(fileHndl, monCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monEnMsk, enableVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monCntEnMsk, cntEnableVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monLpTmMsk, lpTmVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monLpBkMsk, lpBkVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, clkCtrlEnMsk, clkCtrlEnVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfg1Reg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciPmmMonEnable */

/*******************************************************************************
**
**  hyPhy20gHsciPmmMkrEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables the operating mode of the packet
**                  maker.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - PMM instance
**                                    0 to 1
**                  mode            - Maker Operating Mode
**                                    0 - disabled
**                                    1 - enable, generate packets toward
**                                        CPB
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciPmmMkrEnable(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    /* variable declaration */
    UINT4 mkrEnMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_ENABLE_MSK;
    UINT4 mkrCfg1Reg;
    INT4 result;

    /* argument checking */
    if((slice > 1)||(mode > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    mkrCfg1Reg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_1;
    mkrCfg1Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(mkrCfg1Reg, slice);

    /* configure the MKR_ENABLE bit as specified */
    result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg1Reg, mkrEnMsk, mode);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciPmmMkrEnable */

/*******************************************************************************
**
**  hyPhy20gLsciPmmInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes a PMM instance associated
**                  with the LSCI subsystem.
**
**                  It configures the requires packet size for the specified
**                  application and other default settings.
**
**                  In addition it configures LSCI BUS_WIDTH settings and
**                  FC_CBR client DLOLB alarm event integration periods.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - LSCI PMM instance
**                                    0 to 15
**                  payloadType     - Featured application of the LSCI
**                                    instance:
**                                     0 - Disabled (held in RESET)
**                                         required for MAC terminated clients
**                                     1 - Transparent GE (1250 Mbps)
**                                     2 - FC-12 (132.8125 Mbps)
**                                     3 - FC-25 (265.625 Mbps)
**                                     4 - FC-50 (531.25 Mbps)
**                                     5 - 1G FC (1.06250 Gbps)
**                                     6 - 2G FC (2.125 Gbps)
**                                     7 - SD-SDI (270 Mbps)
**                                     8 - SD-SDI (360 Mbps)
**                                     9 - HD-SDI (1.485 Gbps)
**                                    10 - 3G-SDI (2.97 Gbps)
**                                    11 - DV-6000 (2.38 Gbps)
**                                    12 - DVB-ASI (270 Mbps)
**                                    13 - Infiniband (2.5 Gbps) (reserved)
**                                    14 - ISC 1 (1.0625 Gbps)
**                                    15 - ISC 3 (2.125 Gbps)
**                                    16 - ESCON (200 Mbps)
**                                    17 - ETR-CLO (16 Mbps)
**                                    18 - FDDI (125 Mbps)
**                                    19 - Optical T3 (44.736 Mbps)
**                                    20 - Optical E3 (34.368 Mbps)
**                                    21 - G-link (196.608 Mbps)
**                                    22 - CPRI (614.4 Mbps)
**                                    23 - CPRI (1228.8 Mbps)
**                                    24 - CPRI (2457.6 Mbps)
**                                    25 - OC-3/STM-1 (155.52 Mbps)
**                                    26 - OC-12/STM-4 (622.08 Mbps)
**                                    27 - OC-48/STM-16 (2488.32 Mbps)
**                                    28 - 4G FC (4.25 Gbps)
**                                    29 - 1.25 Gbps 8b10b GE CBR client
**                                    30 - 132.8125 Mbps 8b10b CBR client
**                                    31 - 265.625 Mbps 8b10b CBR client
**                                    32 - 531.25 Mbps 8b10b CBR client
**                                    33 - 1.06250 Gbps 8b10b CBR client
**                                    34 - 2.125 Gbps 8b10b CBR client
**                                    35 - 4.25 Gbps 8b10b CBR client
**                                    36 - HD-SDI (1.001) 1.483G
**                                    37 - CPRI (4915.2 Mbps)
**                                    38 - InfiniBand (5 Gbps)
**                                    39 - 1.25 Gbps 8b10b non-GE CBR client
**                                    40 - 3G-SDI (NTSC) (2.97Gbps/1.001)
**                  timingMode        Transmit timing mode for associated Tx
**                                    Serdes:
**                                    0 - Adaptive FIFO
**                                        source data stream received from
**                                        CPB non dithered packet source:
**                                        - GFP-T AA demapped 8b10b CBR clients
**                                    1 - Source C phase from LSCI PMM
**                                        source data stream received from
**                                        CPB dithered packet source:
**                                        - sync/async demapped OC-48 from ODU1
**                                        - async demapped OC-12/3 from ODU1
**                                          trib slots
**                                        - GFP-F demapped byte CBR clients
**                                        - GFP-T FA demapped 8b10b CBR clients
**                                    2 - RESERVED
**                                        This timing mode should not be used.
**                                        Sync demapped STS-48 should be treated
**                                        the same as async demapped STS-48.
**                                    3 - Nominal
**                                        Valid for payload types:
**                                        GE, FC-12, FC-25, FC-50, 1G FC, 2G FC,
**                                        4G FC and ESCON these modes rate adapt
**                                        signal in transmit LSCI.
**
**                                    Note:  GE, FC-12, FC-25, FC-50, 1G FC,
**                                        2G FC, 4G FC and ESCON payload types
**                                        will always generate nominal timing
**                                        there ae equivalent 'raw' rates provided
**                                        for using these applications as CBR
**                                        clients.
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciPmmInit(struct file *fileHndl, UINT4 slice, UINT4 payloadType,
                         UINT4 timingMode)
{
    /* variable declaration */
    UINT4 rateKbits = 0;
    UINT4 fcCbrReg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(
                                    HYPHY20G_LSCI_FC_CBR_REG_CFG, slice);
    UINT4 fcCbrBusWidthMsk = HYPHY20G_LSCI_FC_CBR_BIT_CFG_BUS_WIDTH_MSK;
    UINT4 addr;

    UINT4 pmmRstMsk = HYPHY20G_LSCI_TOP_BIT_CFG_PMM_RST_MSK;
    UINT4 lsciCfgReg;
    UINT4 mkrPktPeriodMsk =
                         HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_PERIOD_MSK;
    UINT4 mkrLosDisMsk =
                        HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_LOS_DISABLE_MSK;
    UINT4 mkrLosDisGeGfpTMsk =
                HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_LOS_DISABLE_GE_GFPT_MSK;
    UINT4 mkrLosDisFcCbrMsk =
                 HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_LOS_DISABLE_FC_CBR_MSK;
    UINT4 mkrPktSzVarMsk =
                       HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_SIZE_VAR_MSK;
    UINT4 dsmTypeMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_DSM_TYPE_MSK;
    UINT4 mkrDithEnMsk =
                        HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_DITH_ENABLE_MSK;
    UINT4 mkrEnMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_ENABLE_MSK;
    UINT4 mkrCfg1Reg;
    UINT4 mkrPktSzMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_2_MKR_PKT_SIZE_MSK;
    UINT4 busWidthMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_2_BUS_WIDTH_MSK;
    UINT4 mkrCfg2Reg;
    UINT4 mkrDsmDenMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_3_MKR_DSM_DENOM_MSK;
    UINT4 mkrCfg3Reg;
    UINT4 clkCtrlEnMsk =
                   HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_ADPV_CLK_CTRL_ENABLE_MSK;
    UINT4 monLpBkMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_MON_LOOPBACK_MSK;
    UINT4 monLpTmMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_MON_LOOPTIME_MSK;
    UINT4 monCntEnMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_MON_CNT_ENABLE_MSK;
    UINT4 monEnMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_MON_ENABLE_MSK;
    UINT4 monCfg1Reg;
    UINT4 monPktSzMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_2_MON_PKT_SIZE_MSK;
    UINT4 predDenMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_2_PREDICTOR_DENOM_MSK;
    UINT4 predNumMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_2_PREDICTOR_NUMER_MSK;
    UINT4 monCfg2Reg;
    UINT4 mkrPktPeriodVal, mkrLosDisVal, mkrLosDisGeGfpTVal, mkrLosDisFcCbrVal;
    UINT4 mkrPktSzVarVal, dsmTypeVal, mkrDithEnVal, mkrEnVal;
    UINT4 mkrPktSzVal, busWidthVal;
    UINT4 mkrDsmDenVal;
    UINT4 clkCtrlEnVal, monLpBkVal, monLpTmVal, monCntEnVal, monEnVal;
    UINT4 monPktSzVal, predDenVal, predNumVal;
    UINT4 regData;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 15)||(payloadType > 40)||(timingMode > 3)||
       ((payloadType != 27)&&(timingMode == 2))||
       (((payloadType > 6)&&(payloadType != 16)&&(payloadType != 28))
       &&(timingMode == 3))){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* get device revision */
    result = sysHyPhy20gGetDeviceRevId(fileHndl, &revId);
    if (result) return result;

    lsciCfgReg = HYPHY20G_LSCI_TOP_REG_CFG;
    lsciCfgReg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(lsciCfgReg, slice);
    mkrCfg1Reg = HYPHY20G_LSCI_TOP_REG_PAC_MKR_CFG_1;
    mkrCfg1Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(mkrCfg1Reg, slice);
    mkrCfg2Reg = HYPHY20G_LSCI_TOP_REG_PAC_MKR_CFG_2;
    mkrCfg2Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(mkrCfg2Reg, slice);
    mkrCfg3Reg = HYPHY20G_LSCI_TOP_REG_PAC_MKR_CFG_3;
    mkrCfg3Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(mkrCfg3Reg, slice);
    monCfg1Reg = HYPHY20G_LSCI_TOP_REG_PAC_MON_CFG_1;
    monCfg1Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(monCfg1Reg, slice);
    monCfg2Reg = HYPHY20G_LSCI_TOP_REG_PAC_MON_CFG_2;
    monCfg2Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(monCfg2Reg, slice);

    /* generate default settings for non-phase mode CPB packet generation
       settings to be modified on a case by case basis relative to user
       specified application */

    mkrPktPeriodVal = 0;
    mkrLosDisVal = 1;
    mkrLosDisGeGfpTVal = 0;
    mkrLosDisFcCbrVal = 1;
    mkrPktSzVarVal = 0;
    dsmTypeVal = 1;
    mkrDithEnVal = 0;
    mkrEnVal = 0;

    mkrPktSzVal = 0x40; /* 64 characters required setting for GFP-T AA mode */
    busWidthVal = 0;
    mkrDsmDenVal = 1024;

    clkCtrlEnVal = 0;
    monLpBkVal = 0;
    monLpTmVal = 0;
    monCntEnVal = 0;
    monEnVal = 0;

    monPktSzVal = 0x40; /* 64 characters required setting for GFP-T AA mode */
    predDenVal = 1;
    predNumVal = 0;

    /* for nominal timing applications, configure as per FIFO only mode */
    if(timingMode == 3){
        timingMode = 0;
    }

    /* implement configuration settings changes from default */
    switch (payloadType) {
        case 0:
            /*  0 - Disabled (held in RESET)
                    required for MAC terminated clients  */
            break;
        case 1:
            /*   1 - Transparent GE (1250 Mbps) */
            busWidthVal = 0x4;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 159;
            mkrLosDisGeGfpTVal = 1;
            mkrLosDisFcCbrVal = 0;
            rateKbits = 1250000;
            break;
        case 2:
            /*   2 - FC-12 (132.8125 Mbps)      */
            busWidthVal = 0x2; /* 8b10b encoded */
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 1499;
            rateKbits = 132813;
            break;
        case 3:
            /*   3 - FC-25 (265.625 Mbps)       */
            busWidthVal = 0x2; /* 8b10b encoded */
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 749;
            rateKbits = 265625;
            break;
        case 4:
            /*   4 - FC-50 (531.25 Mbps)        */
            busWidthVal = 0x2; /* 8b10b encoded */
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 375;
            rateKbits = 531250;
            break;
        case 5:
            /*   5 - 1G FC (1.06250 Gbps)       */
            busWidthVal = 0x2; /* 8b10b encoded */
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 187;
            rateKbits = 1061500;

            break;
        case 6:
            /*   6 - 2G FC (2.125 Gbps)         */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 2125000;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 94; /* adaptive FIFO specific configuration parameters */
            break;
        case 7:
            /*   7 - SD-SDI (270 Mbps)          */
            busWidthVal = 0x3; /* non 8b10b encoded */
            rateKbits = 270000;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 590; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 4598;
                mkrPktSzVal = 499;

                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 8:
            /*   8 - SD-SDI (360 Mbps)          */
            busWidthVal = 0x3; /* non 8b10b encoded */
            rateKbits = 360000;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 442; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 3449;
                mkrPktSzVal = 499;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 9:
            /*   9 - HD-SDI (1.485 Gbps)        */
            busWidthVal = 0x3; /* non 8b10b encoded */
            rateKbits = 1485000;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 107; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 836;
                mkrPktSzVal = 499;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 10:
            /*  10 - 3G-SDI (2.97 Gbps)         */
            busWidthVal = 0x3; /* non 8b10b encoded */
            rateKbits = 2970000;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 54; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 408;
                mkrPktSzVal = 487;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 11:
            /*  11 - DV-6000 (2.38 Gbps)        */
            busWidthVal = 0x3; /* non 8b10b encoded */
            rateKbits = 2380000;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 67; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 509;
                mkrPktSzVal = 487;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 12:
            /*  12 - DVB-ASI (270 Mbps)         */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 270000;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 737; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 5115;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 13:
            /*  13 - Infiniband (2.5 Gbps)      */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 2500000;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 80; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 552;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 14:
            /*  14 - ISC 1 (1.0625 Gbps)        */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 1062500;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 187; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 1300;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 15:
            /*  15 - ISC 3 (2.125 Gbps)         */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 2125000;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 94; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 650;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 16:
            /*  16 - ESCON (200 Mbps)           */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 200000;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 1011; /* adaptive FIFO specific configuration parameters */
            break;
        case 17:
            /*  17 - ETR-CLO (16 Mbps)          */
            busWidthVal = 0x3; /* non 8b10b encoded */
            rateKbits = 16000;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 9953; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 73872;
                mkrPktSzVal = 475;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 18:
            /*  18 - FDDI (125 Mbps)            */
            busWidthVal = 0x3; /* non 8b10b encoded */
            rateKbits = 125000;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 1274; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 12118;
                mkrPktSzVal = 487;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 19:
            /*  19 - Optical T3 (44.736 Mbps)   */
            busWidthVal = 0x3; /* non 8b10b encoded */
            rateKbits = 44735;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 3560; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 27755;
                mkrPktSzVal = 499;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 20:
            /*  20 - Optical E3 (34.368 Mbps)   */
            busWidthVal = 0x3; /* non 8b10b encoded */
            rateKbits = 34368;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 4634; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 19621;
                mkrPktSzVal = 271;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 21:
            /*  21 - G-link (196.608 Mbps)      */
            busWidthVal = 0x3; /* non 8b10b encoded */
            rateKbits = 196608;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 810; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 5556;
                mkrPktSzVal = 439;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 22:
            /*  22 - CPRI (614.4 Mbps)          */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 614400;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 324; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 2248;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 23:
            /*  23 - CPRI (1228.8 Mbps)         */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 1228800;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 162; /* adaptive FIFO specific configuration parameters */
             /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 1124;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 24:
            /*  24 - CPRI (2457.6 Mbps)         */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 2457600;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 81; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 562;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 25:
            /*  25 - OC-3/STM-1 (155.52 Mbps)   */
            busWidthVal = 0x5;
            rateKbits = 155520;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 1024; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 7984;
                mkrPktSzVal = 499;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 26:
            /*  26 - OC-12/STM-4 (622.08 Mbps)  */
            busWidthVal = 0x5;
            rateKbits = 622080;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 256; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 1996;
                mkrPktSzVal = 499;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 27:
            /*  27 - OC-48/STM-16 (2488.32 Mbps)*/
            busWidthVal = 0x5;
            rateKbits = 2488320;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 64; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters
               for async demapped OC-48 or OC-48 that has
               been GFP-F transported with dithered packet
               sizes */
            if(timingMode==2){ /* sync mapping/demapping */
                mkrPktPeriodVal = 499;
                mkrPktSzVal = 499;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 0;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            } else if(timingMode==1){ /* async mapping/demapping and phase mode */
                mkrPktPeriodVal = 499;
                mkrPktSzVal = 499;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 28:
            /*  28 - 4G FC (4.25 Gbps) */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 4250000;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 47; /* adaptive FIFO specific configuration parameters */
            break;
        case 29:
            /*  29 - 1.25 Gbps 8b10b GE CBR client */
        case 39:
            /*  39 - 1.25 Gbps 8b10b non-GE CBR client */
            busWidthVal = 0x4; /* 8b10b encoded */
            rateKbits = 1250000;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 159; /* adaptive FIFO specific configuration parameters */
            mkrLosDisGeGfpTVal = 0;
            mkrLosDisFcCbrVal = 0;
            if(timingMode){
                mkrPktPeriodVal = 1105;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 30:
            /*  30 - 132.8125 Mbps 8b10b CBR client */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 132813;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 1499; /* adaptive FIFO specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 10398;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 31:
            /*  31 - 265.625 Mbps 8b10b CBR client */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 265625;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 749; /* adaptive FIFO specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 5199;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 32:
            /*  32 - 531.25 Mbps 8b10b CBR client */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 531250;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 375; /* adaptive FIFO specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 2600;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 33:
            /*  33 - 1.06250 Gbps 8b10b CBR client */
            rateKbits = 1062500;
            busWidthVal = 0x2; /* 8b10b encoded */
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 187; /* adaptive FIFO specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 1300;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 34:
            /*  34 - 2.125 Gbps 8b10b CBR client */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 2125000;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 94; /* adaptive FIFO specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 650;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 35:
            /*  35 - 4.25 Gbps 8b10b CBR client */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 4250000;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 47; /* adaptive FIFO specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 325;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 36:
            /* 36 - HD-SDI (1.001) 1.001G */
            busWidthVal = 0x3; /* non 8b10b encoded */
            rateKbits = 1483516;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 107; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 736;
                mkrPktSzVal = 439;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 37:
            /*  37 - CPRI (4915.2 Mbps)         */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 4915200;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 49; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 281;
                mkrPktSzVal = 444;

                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 38:
            /*  38 - Infiniband (5 Gbps)        */
            busWidthVal = 0x2; /* 8b10b encoded */
            rateKbits = 5000000;
            mkrPktSzVarVal = 1; 
            mkrPktPeriodVal = 42; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 117;
                mkrPktSzVal = 188;
                mkrPktSzVarVal = 1;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        case 40:
            /*  40 - 3G-SDI(NTSC) (2.97 Gbps/1.001) */
            busWidthVal = 0x3; /* non 8b10b encoded */
            rateKbits = 2967033;
            mkrPktSzVarVal = 0; 
            mkrPktPeriodVal = 54; /* adaptive FIFO specific configuration parameters */
            /* phase mode specific configuration parameters */
            if(timingMode){
                mkrPktPeriodVal = 368;
                mkrPktSzVal = 439;

                mkrPktSzVarVal = 0;
                dsmTypeVal = 1;
                mkrDithEnVal = 1;
                mkrDsmDenVal = 1024;
                monPktSzVal = mkrPktSzVal;
                predDenVal =1;
            }
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* set PMM_RESET = 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, lsciCfgReg, pmmRstMsk, 1);
    if(result) return result;

    if(payloadType == 0) return HYPHY20G_SUCCESS;

    if(payloadType != 1) {
        /* configure bus width for LSCI FC_CBR block. Note the bus width
           encoding is different with bus width register in PMM */
        result = hyPhy20gRegFieldWrite(fileHndl, fcCbrReg, fcCbrBusWidthMsk,
                                       (0x5 - busWidthVal));
        if(result) return result;

        /* configure FC_CBR DLOLB event integration periods - an integration
           period of 100 ms is applied */
        result = lsciHyPhy20gLsciRxFcDlolbIntPer(fileHndl, slice, 100,
                                                 rateKbits,
                                                 (0x5 - busWidthVal));
        if(result) return result;

    }
    /* enable GE_RD_INV_EN bit for 1.25 Gbps 8b10b CBR client (GE traffic) */
    if(payloadType == 29) {
        addr = HYPHY20G_LSCI_FC_CBR_REG_TX_CFG;
        addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_GE_RD_INV_EN_OFF, 1);
        if(result) return result;
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_FC_RD_INV_EN_OFF, 0);
        if(result) return result;
    }

    /* configure PMM */

    /* configure Mon Cfg Reg 1 */
    result = sysHyPhy20gRead(fileHndl, monCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,  clkCtrlEnMsk,  clkCtrlEnVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,  monLpBkMsk,  monLpBkVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monLpTmMsk, monLpTmVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monCntEnMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monEnMsk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfg1Reg, regData);
    if(result) return result;

    /* configure Mon Cfg Reg 2 */
    result = sysHyPhy20gRead(fileHndl, monCfg2Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monPktSzMsk, monPktSzVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, predDenMsk, predDenVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, predNumMsk, predNumVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfg2Reg, regData);
    if(result) return result;

    /* configure Mkr Cfg Reg 3 */
    result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg3Reg, mkrDsmDenMsk,
                                    mkrDsmDenVal);
    if(result) return result;

    /* configure Mkr Cfg Reg 2 */
    result = sysHyPhy20gRead(fileHndl, mkrCfg2Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktSzMsk, mkrPktSzVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, busWidthMsk, busWidthVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, mkrCfg2Reg, regData);
    if(result) return result;

    /* configure Mkr Cfg Reg 1 */
    result = sysHyPhy20gRead(fileHndl, mkrCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktPeriodMsk, mkrPktPeriodVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrLosDisMsk, mkrLosDisVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrLosDisGeGfpTMsk,
                                    mkrLosDisGeGfpTVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrLosDisFcCbrMsk,
                                    mkrLosDisFcCbrVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktSzVarMsk, mkrPktSzVarVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, dsmTypeMsk, dsmTypeVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrDithEnMsk, mkrDithEnVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrEnMsk, mkrEnVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, mkrCfg1Reg, regData);
    if(result) return result;

    /* set PMM_RESET = 0 */
    result = hyPhy20gRegFieldWrite (fileHndl, lsciCfgReg, pmmRstMsk, 0);
    if(result) return result;

    /* remove FC_CBR block SOFT_RST for FC or CBR modes */
    if(payloadType != 1) {
        addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(
                                        HYPHY20G_LSCI_FC_CBR_REG_CFG, slice);
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_LSCI_FC_CBR_BIT_CFG_SOFT_RST_OFF, 0);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciPmmInit */


/*******************************************************************************
**
**  hyPhy20gLsciPmmDsmCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs PMM DSM configuration.  It may be
**                  called after hyPhy20gLsciPmmInit and prior to enabling
**                  PMM processing with hyPhy20gLsciPmmMonEnable and
**                  hyPhy20gLsciPmmMkrEnable.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - PMM instance
**                                    0 to 15
**                  dsmMode         - DSM Mode
**                                    0 - 1st order
**                                    1 - MASH11 2nd order
**                                    2 - Standard 2nd order
**                  dsmDenom        - Maker Delta Sigma Modulator denominator
**                                    value, 14 bits, legal range:
**                                    0 to 0x3FFF
**                  predDenom       - Monitor Predictor Delta Sigma Modulator
**                                    denominator value, 8 bits, legal range:
**                                    0 to 0xFF
**                  predNum         - Monitor Predictor Delta Sigma Modulator
**                                    numerator value, 8 bits, legal range:
**                                    0 to 0xFF
**                  updateMsk       - Mask of DSM related configuration field
**                                    to updated.  Set the corresponding bit
**                                    to 1 to update a field, set to 0 to
**                                    maintain value of configured field:
**                                    bit 0 - update predNum field
**                                    bit 1 - update predDenom field
**                                    bit 2 - update dsmDenom field
**                                    bit 3 - update dsmMode field
**
**
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciPmmDsmCfg(struct file *fileHndl, UINT4 slice, UINT4 dsmMode,
                          UINT4 dsmDenom, UINT4 predDenom, UINT4 predNum,
                          UINT4 updateMsk)
{
    /* variable declaration */
    UINT4 dsmTypeMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_DSM_TYPE_MSK;
    UINT4 mkrCfg1Reg;
    UINT4 mkrDsmDenMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_3_MKR_DSM_DENOM_MSK;
    UINT4 mkrCfg3Reg;
    UINT4 predDenMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_2_PREDICTOR_DENOM_MSK;
    UINT4 predNumMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_2_PREDICTOR_NUMER_MSK;
    UINT4 monCfg2Reg;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);


    /* argument checking */
    if((slice > 15)||(dsmMode > 2)||(dsmDenom > 0x3FFF)||(predDenom > 0xFF)||
       (predNum > 0xFF)||(updateMsk > 0xF)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    mkrCfg1Reg = HYPHY20G_LSCI_TOP_REG_PAC_MKR_CFG_1;
    mkrCfg1Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(mkrCfg1Reg, slice);
    mkrCfg3Reg = HYPHY20G_LSCI_TOP_REG_PAC_MKR_CFG_3;
    mkrCfg3Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(mkrCfg3Reg, slice);
    monCfg2Reg = HYPHY20G_LSCI_TOP_REG_PAC_MON_CFG_2;
    monCfg2Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(monCfg2Reg, slice);

    /* configure the MKR_DSM_TYPE field if specified */
    if(updateMsk & 0x8){
        result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg1Reg, dsmTypeMsk,
                                        dsmMode);
        if(result) return result;
    }

    /* configure the DSM_DENOM field if specified */
    if(updateMsk & 0x4){
        result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg3Reg, mkrDsmDenMsk,
                                        dsmDenom);
        if(result) return result;
    }

    /* configure the DSM_DENOM field if specified */
    if(updateMsk & 0x2){
        result = hyPhy20gRegFieldWrite (fileHndl, monCfg2Reg, predDenMsk,
                                        predDenom);
        if(result) return result;
    }

    /* configure the DSM_DENOM field if specified */
    if(updateMsk & 0x1){
        result = hyPhy20gRegFieldWrite (fileHndl, monCfg2Reg, predNumMsk,
                                        predNum);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciPmmDsmCfg */




/*******************************************************************************
**
**  hyPhy20gLsciPmmMonEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables the operating mode of the packet
**                  monitor.
**
**                  hyPhy20gCpbInit must be called prior to calling this API 
**                  to ensure that CPB context has been initialized.
**
**                  This function also configures RD_ZONE_START bit in the
**                  FC_CBR trasmit block. This bit can only change when no
**                  egress data is being transmitted from CPB to FC_CBR block.
**
**                  This function should be called when CPB multicasting is 
**                  performed by provisioning a non-primary CBR read channel 
**                  using a CBR write channel. ZONE1 setting needs to be lowered
**                  by setting RD_ZONE_START to 0 to allow the non-primary read
**                  channel to run. 
**   
**                  When the non-primary CBR read channel is re-established as
**                  a primary channel, this function should be called to set
**                  RD_ZONE_START back to 2 when enabled for normal data path.           
**
**                  It will unconditionally set the packet monitor to send
**                  packet size data to ACB blocks unless 'disabled' mode is
**                  selected.
**
**                  When the datapath is loopbacking received data from
**                  companion packet maker block, PMM looptiming will be
**                  selected.  This mode requires the associated ACB to be
**                  configured to use Source C as the timing reference source.
**                  PMM loopback operating mode is not supported for GE, FC-12,
**                  FC-25, FC-50, 1G FC, 2G FC, 4G FC and ESCON payload types.
**                  These modes use transmit rate adaption which is not
**                  supported in PMM loopback.  Transmit rate adaption
**                  loopback is only supported when the data stream is mapped
**                  back to CPB as the rate adaption mechanism uses CPB FIFO
**                  depth to make character insertion/deletion decisions.
**                  If these application rates are treated as CBR clients
**                  PMM loopback may be used.
**
**                  Prior to enabling the PMM loopback (mode == 2), user
**                  must disable and unprovision the corresponding CPB channel,
**                  if it has been previously enabled, to protect the channel
**                  from overflow/underflow within CPB.
**
**                  To switch over Packet Monitor Operating Modes, user must
**                  disable the Packet Maker at first, and enable the
**                  Packet Maker after Packet Monitor mode switch completed
**                  (when the mode == 1 or 2).
**                  hyPhy20gLsciPmmMkrEnable API is used for enabling/disabling
**                  the Packet Maker.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - PMM instance
**                                    0 to 15
**                  mode            - Monitor Operating Mode
**                                    0 - disabled
**                                    1 - enabled for normal data path
**                                    2 - enabled to loopback data received
**                                        from companion packet maker block
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciPmmMonEnable(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    /* variable declaration */
    UINT4 fcCbrCfgReg;
    UINT4 fcTxCfgReg;
    UINT4 rdZoneVal     = 0;
    UINT4 preZoneVal;
    UINT4 rdZoneMsk     = HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_RD_ZONE_START_MSK;
    UINT4 fcCbrRstOff   = HYPHY20G_LSCI_FC_CBR_BIT_CFG_SOFT_RST_OFF;
    UINT4 clkCtrlEnMsk =
                   HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_ADPV_CLK_CTRL_ENABLE_MSK;
    UINT4 monLpBkMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_MON_LOOPBACK_MSK;
    UINT4 monLpTmMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_MON_LOOPTIME_MSK;
    UINT4 monCntEnMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_MON_CNT_ENABLE_MSK;
    UINT4 monEnMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_MON_ENABLE_MSK;
    UINT4 monCfg1Reg;
    UINT4 cntEnableVal, enableVal, lpTmVal, lpBkVal, clkCtrlEnVal;
    UINT4 regData;
    INT4 result;
    UINT4 devId, revId, status;
	sHYPHY20G_CTXT          *pDevCtxt;
    sHYPHY20G_RD_CHNL_CTXT  *pRdChnl;
    
    /* Retrieve context for CPB */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* confirm the CPB context is valid */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pRdChnl + slice);
	
    /* Return error if API called against META 20G device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 15)||(mode > 2)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (mode) {
        case 0:
            cntEnableVal = 0;
            clkCtrlEnVal = cntEnableVal;
            enableVal = 0;
            lpTmVal = 0;
            lpBkVal = 0;
            rdZoneVal = 2;
            break;
        case 1:
            cntEnableVal = 1;
            clkCtrlEnVal = cntEnableVal;
            enableVal = 1;
            lpTmVal = 0;
            lpBkVal = 0;
            rdZoneVal = 2;
            break;
        case 2:
            cntEnableVal = 1;
            clkCtrlEnVal = cntEnableVal;
            enableVal = 1;
            lpTmVal = 1;
            lpBkVal = 1;
            rdZoneVal = 0;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /*
     * configure RD_ZONE_START bit in the FC_CBR block
     */
    fcCbrCfgReg = HYPHY20G_LSCI_FC_CBR_REG_CFG;
    fcCbrCfgReg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(fcCbrCfgReg, slice);
    result = sysHyPhy20gBitRead(fileHndl, fcCbrCfgReg, fcCbrRstOff, &regData);
    if(result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, fcCbrCfgReg, fcCbrRstOff, 1);
    if(result) return result;

    fcTxCfgReg = HYPHY20G_LSCI_FC_CBR_REG_TX_CFG;
    fcTxCfgReg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(fcTxCfgReg, slice);
    result = hyPhy20gRegFieldRead(fileHndl, fcTxCfgReg,
                                  rdZoneMsk, &preZoneVal);
    if(result) return result;

    if(rdZoneVal != preZoneVal) {
        result = hyPhy20gRegFieldWrite(fileHndl, fcTxCfgReg,
                                       rdZoneMsk, rdZoneVal);
        if(result) return result;
    }
    
    /* If the PMM instance is associated with a CPB non-primary reader
       set RD_ZONE_START to 0 */
    if (pRdChnl->primaryFlag == HYPHY20G_CPB_NON_PRIMARY_RD_CHNL) {
        result = hyPhy20gRegFieldWrite(fileHndl, fcTxCfgReg, rdZoneMsk, 0);
        if(result) return result;
    } 

    /* restore the previous setting */
    if(!regData) {
        result = sysHyPhy20gBitWrite(fileHndl, fcCbrCfgReg, fcCbrRstOff, 0);
        if(result) return result;
    }
    
    /* configure Packet Monitor */
    monCfg1Reg = HYPHY20G_LSCI_TOP_REG_PAC_MON_CFG_1;
    monCfg1Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(monCfg1Reg, slice);

    result = sysHyPhy20gRead(fileHndl, monCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monEnMsk, enableVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monCntEnMsk, cntEnableVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monLpTmMsk, lpTmVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monLpBkMsk, lpBkVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, clkCtrlEnMsk, clkCtrlEnVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfg1Reg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciPmmMonEnable */

/*******************************************************************************
**
**  hyPhy20gLsciPmmMkrEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables the operating mode of the packet
**                  maker.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - PMM instance
**                                    0 to 15
**                  mode            - Maker Operating Mode
**                                    0 - disabled
**                                    1 - enable, generate packets toward
**                                        CPB
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciPmmMkrEnable(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    /* variable declaration */
    UINT4 mkrEnMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_ENABLE_MSK;
    UINT4 mkrCfg1Reg;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 15)||(mode > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    mkrCfg1Reg = HYPHY20G_LSCI_TOP_REG_PAC_MKR_CFG_1;
    mkrCfg1Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(mkrCfg1Reg, slice);

    /* configure the MKR_ENABLE bit as specified */
    result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg1Reg, mkrEnMsk, mode);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciPmmMkrEnable */

/*******************************************************************************
**
**  hyPhy20gOpiPmmInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes an PMM instance associated
**                  with the OPI subsystem.
**
**                  It configures the requires packet size for the specified
**                  ODUk type and other default settings.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - PMM instance
**                                    OTU1 subsystem interface:
**                                    0 to 7 OTU1 framer 0 to 7
**                                    OTU2 subsystem interface:
**                                    8 to 9 OTU2 framer 0 to 1
**                                    VCAT interface:
**                                    10 to 13 VCAT interface 0 (ODU1 mode)
**                                    14 to 17 VCAT interface 1 (ODU1 mode)
**                                    10 VCAT interface 0 (ODU2 mode)
**                                    14 VCAT interface 1 (ODU2 mode)
**                  payloadType     - ODUk format / SONET Internal Loop timing
**                                    0 - ODU1 for (ODU1 mode)
**                                    1 - ODU2 for (ODU2 mode)
**                                    2 - ODU1e for (ODU2 mode)
**                                    3 - ODU2e for (ODU2 mode)
**                                    4 - ODU1f for (ODU2 mode)
**                                    5 - ODU2f for (ODU2 mode)
**                                    6 - SONET Internal Loop timing
**                                    payloadType 1 to 5 supported for slice
**                                    8, 9, 10 and 14.
**                                    payloadType 0 supported for slice 0 to 7
**                                    and 10 to 17.
**                                    payloadType 6 supported for slice 8 and 9
**                                    only valid on Rev C and Rev D devices.
**                  sourceType      - PMM Output Signal Format Type
**                                    0 - Source C of ACB
**                                        (Valid for ODU1e/2e/1f/2f)
**                                    1 - Source B of ACB or
**                                        Pacer ODUKPP for ODU1 or ODU2 signals
**                                        (Valid for ODU1/ODU2 for sync and 
**                                        async mapped clients)
**                                    2 - Pacer ODUKPP for ODU2e/1e signals 
**                                        bound for HSCI subsystem 
**                                        (MAC termination of G.Sup43 7.1/6.2)
**                                    3 - SONET Internal Loop timing
**                                    Must be set to 0 for slice 0 to 9
**                                    Configurable for slice 10 to 17, source
**                                    B is only valid for ODU1 and ODU2 payload
**                                    types.
**                                    sourceType set to 2 only valid for slice
**                                    10 and 14 in ODU2e mode of operation for
**                                    the ODUKPP to HSCI data path
**                                    sourceType set to 3 is only valid for 
**                                    payloadType set to 6
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOpiPmmInit(struct file *fileHndl, UINT4 slice, UINT4 payloadType,
                        UINT4 sourceType)
{
    /* variable declaration */
    sHYPHY20G_CTXT  *pDevCtxt;
    UINT4 devId, revId, status;
    UINT4 pmmRstMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_PMM_RESET_MSK;
    UINT4 mkrPktPeriodMsk =
                          HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_PKT_PERIOD_MSK;
    UINT4 mkrPktSzVarMsk =
                        HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_PKT_SIZE_VAR_MSK;
    UINT4 dsmTypeMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_DSM_TYPE_MSK;
    UINT4 mkrDithEnMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_DITH_ENABLE_MSK;
    UINT4 mkrEnMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_ENABLE_MSK;
    UINT4 mkrCfg1Reg;
    UINT4 mkrPktSzMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_2_MKR_PKT_SIZE_MSK;
    UINT4 mkrTsEnMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_2_MKR_TS_ENABLE_MSK;
    UINT4 mkrCfg2Reg;
    UINT4 mkrDsmDenMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_3_MKR_DSM_DENOM_MSK;
    UINT4 mkrCfg3Reg;
    UINT4 monCntEnMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_1_MON_CNT_ENABLE_MSK;
    UINT4 monEnMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_1_MON_ENABLE_MSK;
    UINT4 monTsEnMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_1_MON_TS_ENABLE_MSK;
    UINT4 monCfg1Reg;
    UINT4 monPktSzMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_2_MON_PKT_SIZE_MSK;
    UINT4 predDenMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_2_PREDICTOR_DENOM_MSK;
    UINT4 predNumMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_2_PREDICTOR_NUMER_MSK;
    UINT4 monCfg2Reg;
    UINT4 mkrPktSz, mkrPktPeriod, monPktSz, predDen, predNum;
    UINT4 predDenMsb = 0, predNumMsb = 0;
    UINT4 predNumMsbMsk, predDenMsbMsk;
    UINT4 monCfg4Reg;
    UINT4 regData;
    UINT4 pacerEnableMsk, pacerEnableVal;
    INT4 result;

    /* argument checking */
    if((slice > 17)||(payloadType > 6)||(sourceType != 0 && slice < 8)||(sourceType > 3)||
       (payloadType == 0 && (slice == 8 || slice == 9))||(payloadType != 0 && (slice < 8 ||
       (slice > 10 && slice < 14)||(slice > 14))) || 
       (sourceType == 2 && (slice != 10 && slice != 14) && (payloadType != 2 && payloadType != 3))||
       (payloadType == 6 && (slice != 8 && slice != 9)) || (payloadType == 6 && sourceType != 3)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* evaluation of compatibility between SW driver and device */
    result = hyPhy20gCheckRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (payloadType == 6 && (revId != HYPHY20G_REV_ID_C && revId != HYPHY20G_REV_ID_D)){
         /* rev B requirement - do nothing */
        return HYPHY20G_SUCCESS;       
    }

    mkrCfg1Reg = HYPHY20G_OPI_TOP_REG_PKT_MKR_CFG_1_OFFSET(slice);
    mkrCfg2Reg = HYPHY20G_OPI_TOP_REG_PKT_MKR_CFG_2_OFFSET(slice);
    mkrCfg3Reg = HYPHY20G_OPI_TOP_REG_PKT_MKR_CFG_3_OFFSET(slice);
    monCfg1Reg = HYPHY20G_OPI_TOP_REG_PKT_MON_CFG_1_OFFSET(slice);
    monCfg2Reg = HYPHY20G_OPI_TOP_REG_PKT_MON_CFG_2_OFFSET(slice);
    

    /* PMM basic settings */
    if(sourceType == 0){ /* 0 - Source C of ACB */
        /* PMM basic settings for OTN Framers and VCAT I/F PMM blocks
           that are being used for source C inputs to ACB */
        switch (payloadType) {
            case 0: /* ODU1 for (ODU1 mode) */
                switch (pDevCtxt->gData.hyphy20gCbrbPacketSizeMode) {
                     case 0: /* standard */    
                        mkrPktSz = 233;
                        mkrPktPeriod = 232;
                        monPktSz = 233;
                        predDen = 1;
                        predNum = 0;
                        break;
                    case 1: /* standard predictor compensated */
                        mkrPktSz = 233;
                        mkrPktPeriod = 232;
                        monPktSz = 233;
                        predDen = 1;
                        predNum = 0;
                        break;
                    case 2: /* custom fabric */
                        mkrPktSz = 113;
                        mkrPktPeriod = 113;
                        monPktSz = 113;
                        predDen = 1;
                        predNum = 0;
                        break;
                    default:
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                break;
            case 1: /* ODU2 for (ODU2 mode) */
                switch (pDevCtxt->gData.hyphy20gCbrbPacketSizeMode) {
                    case 0: /* standard */    
                        mkrPktSz = 230;
                        mkrPktPeriod = 57;
                        monPktSz = 230;
                        predDen = 1;
                        predNum = 0;
                        break;
                    case 1: /* standard predictor compensated */
                        mkrPktSz = 230;
                        mkrPktPeriod = 57;
                        monPktSz = 230;
                        predDen = 1;
                        predNum = 0;
                        break;
                    case 2: /* custom fabric */
                        mkrPktSz = 113;
                        mkrPktPeriod = 28;
                        monPktSz = 113;
                        predDen = 1;
                        predNum = 0;
                        break;
                    default:
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                break;
            case 2: /* ODU1e for (ODU2 mode) */
                switch (pDevCtxt->gData.hyphy20gCbrbPacketSizeMode) {
                    case 0: /* standard */    
                        mkrPktSz = 229;
                        mkrPktPeriod = 55;
                        monPktSz = 229;
                        predDen = 1;
                        predNum = 0;
                        break;
                    case 1: /* standard predictor compensated */
                        mkrPktSz = 204;
                        mkrPktPeriod = 49;
                        monPktSz = 204;
                        predDen = 1;
                        predNum = 0;
                        break;
                    case 2: /* custom fabric */
                        mkrPktSz = 112;
                        mkrPktPeriod = 27;
                        monPktSz = 112;
                        predDen = 1;
                        predNum = 0;
                        break;
                    default:
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                break;
            case 3: /* ODU2e for (ODU2 mode) */
                switch (pDevCtxt->gData.hyphy20gCbrbPacketSizeMode) {
                    case 0: /* standard */    
                        mkrPktSz = 230;
                        mkrPktPeriod = 55;
                        monPktSz = 230;
                        predDen = 1;
                        predNum = 0;
                        break;
                    case 1: /* standard predictor compensated */
                        mkrPktSz = 238;
                        mkrPktPeriod = 57;
                        monPktSz = 238;
                        predDen = 1;
                        predNum = 0;
                        break;
                    case 2: /* custom fabric */
                        mkrPktSz = 113;
                        mkrPktPeriod = 27;
                        monPktSz = 113;
                        predDen = 1;
                        predNum = 0;
                        break;
                    default:
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                break;
            case 4: /* ODU1f for (ODU2 mode) */
                switch (pDevCtxt->gData.hyphy20gCbrbPacketSizeMode) {
                    case 0: /* standard */    
                        mkrPktSz = 225;
                        mkrPktPeriod = 53;
                        monPktSz = 225;
                        predDen = 1;
                        predNum = 0;
                        break;
                    case 1: /* standard predictor compensated */
                        mkrPktSz = 225;
                        mkrPktPeriod = 53;
                        monPktSz = 225;
                        predDen = 1;
                        predNum = 0;
                        break;
                    case 2: /* custom fabric */
                        mkrPktSz = 106;
                        mkrPktPeriod = 25;
                        monPktSz = 106;
                        predDen = 1;
                        predNum = 0;
                        break;
                    default:
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                break;
            case 5: /* ODU2f for (ODU2 mode) */
                switch (pDevCtxt->gData.hyphy20gCbrbPacketSizeMode) {
                    case 0: /* standard */    
                        mkrPktSz = 226;
                        mkrPktPeriod = 53;
                        monPktSz = 226;
                        predDen = 1;
                        predNum = 0;
                        break;
                    case 1: /* standard predictor compensated */
                        mkrPktSz = 243;
                        mkrPktPeriod = 57;
                        monPktSz = 243;
                        predDen = 1;
                        predNum = 0;
                        break;
                    case 2: /* custom fabric */
                        mkrPktSz = 107;
                        mkrPktPeriod =25;
                        monPktSz = 107;
                        predDen = 1;
                        predNum = 0;
                        break;
                    default:
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    } else if (sourceType == 1) { /* Source B of ACB or Pacer ODUKPP for ODU1 or ODU2 signals */
        /* VCAT I/F PMM blocks used for ACB Source B input (sourceType == 1) */
        switch (payloadType) {
            case 0: /* ODU1 for (ODU1 mode) */
                switch (pDevCtxt->gData.hyphy20gCbrbPacketSizeMode) {
                    case 0: /* standard */    
                    case 1: /* standard predictor compensated */
                        mkrPktSz = 233;
                        mkrPktPeriod = 232;
                        monPktSz = 233;
                        predDen = 0x77;
                        predNum = 0x3;
                        break;
                    case 2: /* custom fabric */
                        mkrPktSz = 113;
                        mkrPktPeriod = 113;
                        monPktSz = 113;
                        predDen = 0xEE; /* 238d */
                        predNum = 0xFFFF8F; /* -113d */
                        break;
                    default:
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                break;
            case 1: /* ODU2 for (ODU2 mode) */
                switch (pDevCtxt->gData.hyphy20gCbrbPacketSizeMode) {
                    case 0: /* standard */ 
                    case 1: /* standard predictor compensated */   
                        mkrPktSz = 230;
                        mkrPktPeriod = 57;
                        monPktSz = 230;
                        predDen = 0x4f;
                        predNum = 0x6;
                        break;
                    case 2: /* custom fabric */
                        mkrPktSz = 113;
                        mkrPktPeriod = 28;
                        monPktSz = 113;
                        predDen = 0xED; /* 237d */
                        predNum = 0xD; /* 13d */
                        break;
                    default:
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                break;

            case 2: /* ODU1e for (ODU2 mode) */
            case 3: /* ODU2e for (ODU2 mode) */
            case 4: /* ODU1f for (ODU2 mode) */
            case 5: /* ODU2f for (ODU2 mode) */
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    } else if (sourceType == 2) { /* sourceType == 2 */
             /* Pacer ODUKPP for ODU2e signals bound
                for HSCI subsystem (MAC termination of
                G.Sup43 7.1 or 7.2) */
        switch (payloadType) {
            case 2: /* ODU1e for (ODU2 mode) */
                switch (pDevCtxt->gData.hyphy20gCbrbPacketSizeMode) {
                    case 0: /* standard */    
                        mkrPktSz = 229;
                        mkrPktPeriod = 55;
                        monPktSz = 229;
                        predDen = 112338;
                        predNum = 12902;
                        break;
                    case 1: /* standard predictor compensated */
                        mkrPktSz = 204;
                        mkrPktPeriod = 49;
                        monPktSz = 204;
                        predDen = 0x1B6D2; /* 112338 */
                        predNum = 0x24ec; /* 9452d */
                        break;
                    case 2: /* custom fabric */
                        mkrPktSz = 112;
                        mkrPktPeriod = 27;
                        monPktSz = 112;
                        predDen = 0x1B6D2; /* 112338 */
                        predNum = 0xFF613C; /* -40644d */
                        break;
                    default:
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                break;
            case 3: /* ODU2e for (ODU2 mode) */
                switch (pDevCtxt->gData.hyphy20gCbrbPacketSizeMode) {
                    case 0: /* standard */    
                        mkrPktSz = 230;
                        mkrPktPeriod = 55;
                        monPktSz = 230;
                        predDen = 133668;
                        predNum = 19540;
                        break;
                    case 1: /* standard predictor compensated */
                        mkrPktSz = 238;
                        mkrPktPeriod = 57;
                        monPktSz = 238;
                        predDen = 0x20A24; /* 133668d */
                        predNum = 0xFF913C; /* -28356d */
                        break;
                    case 2: /* custom fabric */
                        mkrPktSz = 113;
                        mkrPktPeriod = 27;
                        monPktSz = 113;
                        predDen = 0x20A24; /* 133668d */
                        predNum = 0x54F0; /* 21744d */  
                        break;
                    default:
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                }
                break;
            case 0: /* ODU1 for (ODU1 mode) */
            case 1: /* ODU2 for (ODU2 mode) */ 
            case 4: /* ODU1f for (ODU2 mode) */
            case 5: /* ODU2f for (ODU2 mode) */
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    } else { /* sourceType == 3 SONET Internal Loop timing */
        mkrPktSz = 0x3F;
        mkrPktPeriod = 0x3F;
        monPktSz = 0x3F;
        predDen = 1; /* 1d */
        predNum = 0; /* 0d */       
    }
    
    /* If PREDICTOR_DENOM or PREDICTOR_NUMER is greater than 255, extended 
       registers are used */ 
    if (predNum > 0xFF) {
        predNumMsb = predNum >> 8;
        predNum &= 0xFF;        
    }
    if (predDen > 0xFF) {
        predDenMsb = predDen >> 8;
        predDen &= 0xFF;        
    }    
   
    /* set PMM_RESET = 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg1Reg, pmmRstMsk, 1);
    if(result) return result;

    /* configure Mon Cfg Reg 1 */
    result = sysHyPhy20gRead(fileHndl, monCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monCntEnMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monEnMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monTsEnMsk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfg1Reg, regData);
    if(result) return result;

    /* configure Mon Cfg Reg 2 */
    result = sysHyPhy20gRead(fileHndl, monCfg2Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monPktSzMsk, monPktSz);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, predDenMsk, predDen);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, predNumMsk, predNum);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfg2Reg, regData);
    if(result) return result;
      
    /* configure Mon Cfg Reg 4 */            
    predNumMsbMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_4_PREDICTOR_NUMER_MSK;                 
    predDenMsbMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_4_PREDICTOR_DENOM_MSK;
    monCfg4Reg = HYPHY20G_OPI_TOP_REG_PKT_MON_CFG_4_OFFSET(slice);

    result = sysHyPhy20gRead(fileHndl, monCfg4Reg, &regData);
    if(result) return result; 

    result = hyPhy20gVarFieldWrite (&regData, predDenMsbMsk, predDenMsb);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, predNumMsbMsk, predNumMsb);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfg4Reg, regData);
    if(result) return result;
    
    /* configure Mkr Cfg Reg 3 */
    result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg3Reg, mkrDsmDenMsk,
                                    1024);
    if(result) return result;
        
    /* configure Mkr Cfg Reg 2 */
    result = sysHyPhy20gRead(fileHndl, mkrCfg2Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktSzMsk, mkrPktSz);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrTsEnMsk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, mkrCfg2Reg, regData);
    if(result) return result;

    /* configure Mkr Cfg Reg 1 with PMM_RESET = 0 */
    result = sysHyPhy20gRead(fileHndl, mkrCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, pmmRstMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktPeriodMsk, mkrPktPeriod);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktSzVarMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, dsmTypeMsk, 1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrDithEnMsk, 1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrEnMsk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, mkrCfg1Reg, regData);
    if(result) return result;

    /* For slice 10 and 14 on Rev C and Rev D devices configure PACER_ENABLE_10 and 
       PACER_ENABLE_14 */
    if((revId == HYPHY20G_REV_ID_C || revId == HYPHY20G_REV_ID_D)&&(slice == 10 || slice == 14)){
        pacerEnableMsk = (slice == 10) ? 
                         HYPHY20G_OPI_TOP_BIT_SERDES_BACKPLANE_MD_PACER_ENABLE_10_MSK:
                         HYPHY20G_OPI_TOP_BIT_SERDES_BACKPLANE_MD_PACER_ENABLE_14_MSK;
        pacerEnableVal = (payloadType < 2 || payloadType > 5) ? 0:1;
        result = hyPhy20gRegFieldWrite(fileHndl, 
                                       HYPHY20G_OPI_TOP_REG_SERDES_BACKPLANE_MD,
                                       pacerEnableMsk,
                                       pacerEnableVal);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOpiPmmInit */


/*******************************************************************************
**
**  hyPhy20gOpiPmmDsmCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs PMM DSM configuration.  It may be
**                  called after hyPhy20gOpiPmmInit and prior to enabling
**                  PMM processing with hyPhy20gOpiPmmMonEnable and
**                  hyPhy20gOpiPmmMkrEnable.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - PMM instance
**                                    OTU1 subsystem interface:
**                                    0 to 7 OTU1 framer 0 to 7
**                                    OTU2 subsystem interface:
**                                    8 to 9 OTU2 framer 0 to 1
**                                    VCAT interface:
**                                    10 to 13 VCAT interface 0 (ODU1 mode)
**                                    14 to 17 VCAT interface 1 (ODU1 mode)
**                                    10 VCAT interface 0 (ODU2 mode)
**                                    14 VCAT interface 1 (ODU2 mode)
**                  dsmMode         - DSM Mode
**                                    0 - 1st order
**                                    1 - MASH11 2nd order
**                                    2 - Standard 2nd order
**                  dsmDenom        - Maker Delta Sigma Modulator denominator
**                                    value, 14 bits, legal range:
**                                    0 to 0x3FFF
**                  predDenom       - Monitor Predictor Delta Sigma Modulator
**                                    denominator  value, 8 bits, legal range:
**                                    0 to 0xFF
**                  predNum         - Monitor Predictor Delta Sigma Modulator
**                                    numerator value, 8 bits, legal range:
**                                    0 to 0xFF
**                  updateMsk       - Mask of DSM related configuration field
**                                    to updated.  Set the corresponding bit
**                                    to 1 to update a field, set to 0 to
**                                    maintain value of configured field:
**                                    bit 0 - update predNum field
**                                    bit 1 - update predDenom field
**                                    bit 2 - update dsmDenom field
**                                    bit 3 - update dsmMode field
**
**
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOpiPmmDsmCfg(struct file *fileHndl, UINT4 slice, UINT4 dsmMode,
                          UINT4 dsmDenom, UINT4 predDenom, UINT4 predNum,
                          UINT4 updateMsk)
{
    /* variable declaration */
    UINT4 dsmTypeMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_DSM_TYPE_MSK;
    UINT4 mkrCfg1Reg;
    UINT4 mkrDsmDenMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_3_MKR_DSM_DENOM_MSK;
    UINT4 mkrCfg3Reg;
    UINT4 predDenMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_2_PREDICTOR_DENOM_MSK;
    UINT4 predNumMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_2_PREDICTOR_NUMER_MSK;
    UINT4 monCfg2Reg;
    INT4 result;

    /* argument checking */
    if((slice > 17)||(dsmMode > 2)||(dsmDenom > 0x3FFF)||(predDenom > 0xFF)||
       (predNum > 0xFF)||(updateMsk > 0xF)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    mkrCfg1Reg = HYPHY20G_OPI_TOP_REG_PKT_MKR_CFG_1_OFFSET(slice);
    mkrCfg3Reg = HYPHY20G_OPI_TOP_REG_PKT_MKR_CFG_3_OFFSET(slice);
    monCfg2Reg = HYPHY20G_OPI_TOP_REG_PKT_MON_CFG_2_OFFSET(slice);

    /* configure the MKR_DSM_TYPE field if specified */
    if(updateMsk & 0x8){
        result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg1Reg, dsmTypeMsk,
                                        dsmMode);
        if(result) return result;
    }

    /* configure the DSM_DENOM field if specified */
    if(updateMsk & 0x4){
        result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg3Reg, mkrDsmDenMsk,
                                        dsmDenom);
        if(result) return result;
    }

    /* configure the PREDICTOR_DENOM field if specified */
    if(updateMsk & 0x2){
        result = hyPhy20gRegFieldWrite (fileHndl, monCfg2Reg, predDenMsk,
                                        predDenom);
        if(result) return result;
    }

    /* configure the PREDICTOR_NUMER field if specified */
    if(updateMsk & 0x1){
        result = hyPhy20gRegFieldWrite (fileHndl, monCfg2Reg, predNumMsk,
                                        predNum);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOpiPmmDsmCfg */


/*******************************************************************************
**
**  hyPhy20gOpiPmmTmStmpCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs PMM timestamp related processing. It
**                  may be called after hyPhy20gOpiPmmInit and prior to
**                  enabling PMM processing with hyPhy20gOpiPmmMonEnable and
**                  hyPhy20gOpiPmmMkrEnable.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - PMM instance
**                                    OTU1 subsystem interface:
**                                    0 to 7 OTU1 framer 0 to 7
**                                    OTU2 subsystem interface:
**                                    8 to 9 OTU2 framer 0 to 1
**                                    VCAT interface:
**                                    10 to 13 VCAT interface 0 (ODU1 mode)
**                                    14 to 17 VCAT interface 1 (ODU1 mode)
**                                    10 VCAT interface 0 (ODU2 mode)
**                                    14 VCAT interface 1 (ODU2 mode)
**                  tsGenEn         - enables generation of timestamp at the
**                                    packet maker
**                                    0 - disabled
**                                    1 - enabled
**                  csiAlarmVal     - Alarm value to enter into Timestamp CSI
**                                    field when signal fail detected by packet
**                                    maker, legal range:
**                                    0 to 0x1F
**                                    0x1 is the default value transmit when
**                                    no upstream fail present.
**                  csiAlarmTh      - Timestamp CSI alarm threshold to signal
**                                    fail to downstream blocks at packet
**                                    monitor.  Legal range:
**                                    0 to 0x1B
**                                    Downstream alarm triggered when the
**                                    received CSI is greater than this value
**                                    less than 0x1C.
**                                    In a non-alarm state CSI is reported as
**                                    0x1
**                  monSeqChkEn     - Enables dummy packet insertion in the
**                                    event of a lost sequence number.  A single
**                                    missing packet will be replaces with a
**                                    dummy packet of all 0s payload.
**                                    Timestamps must be enabled for this feature
**                                    to be relevant.
**                                    0 - Disabled
**                                    1 - Enabled
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOpiPmmTmStmpCfg(struct file *fileHndl, UINT4 slice, UINT4 tsGenEn,
                             UINT4 csiAlarmVal, UINT4 csiAlarmTh,
                             UINT4 monSeqChkEn)
{
    /* variable declaration */
    UINT4 mkrTsEnblMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_2_MKR_TS_ENABLE_MSK;
    UINT4 mkrCsiMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_2_MKR_CSI_MSK;
    UINT4 mkrCfgReg;
    UINT4 monCsiMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_1_MON_CSI_MSK;
    UINT4 monTsEnblMsk =HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_1_MON_TS_ENABLE_MSK;
    UINT4 monSeqChkEnMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_1_MON_SEQ_CHK_EN_MSK;
    UINT4 monCfgReg;
    UINT4 regData;
    INT4 result;

    /* argument checking */
    if((slice > 17)||(tsGenEn > 1)||(csiAlarmVal > 0x1F)||(csiAlarmTh > 0x1B)||
       (monSeqChkEn > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    mkrCfgReg = HYPHY20G_OPI_TOP_REG_PKT_MKR_CFG_2_OFFSET(slice);
    monCfgReg = HYPHY20G_OPI_TOP_REG_PKT_MON_CFG_1_OFFSET(slice);

    /* configure MKR_CSI and MKR_TS_ENABLE bits */

    result = sysHyPhy20gRead(fileHndl, mkrCfgReg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrTsEnblMsk, tsGenEn);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrCsiMsk, csiAlarmVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, mkrCfgReg, regData);
    if(result) return result;

    /* configure MON_CSI, MON_TS_ENABLE and MON_SEQ_CHK_EN bits */

    result = sysHyPhy20gRead(fileHndl, monCfgReg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monTsEnblMsk, tsGenEn);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monCsiMsk, csiAlarmTh);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monSeqChkEnMsk, monSeqChkEn);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfgReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOpiPmmTmStmpCfg */


/*******************************************************************************
**
**  hyPhy20gOpiPmmTmStmpTrig
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables PMM Monitor to suppress CBRB FIFO
**                  reads in order to generate a delay within the CBRB FIFO
**                  specified by MON_TS_ADDER field (configured with
**                  tsAddr argument).  When the delay is accumulated
**                  the PMM Monitor will read data from the CBRB FIFO.
**
**                  The intention of this feature is to align ODUk streams
**                  by monitoring the timestamps on the packetized ODUk
**                  elements.  The alignment is required to remove skew
**                  from fabric in order to maintain alignment of VCAT
**                  streams.
**
**                  Delay is generated as follows:
**                  1) MON_TS_TRIGGER transitions from 0 to 1 (triggered by
**                     calling this API)
**                  2) PMM Monitor stops reading data from CBRB and generates
**                     0s on it's output stream
**                  3) The last received timestamp is added to MON_TS_ADDER
**                  4) The local timestamp counter is monitored till it
**                     reaches value calculated in 3
**                  5) PMM
**
**                  MON_TS_ADDER is specified in units of 3.215 nsecond steps.
**                  It must be set to a value greater than the maximum latency
**                  of the ODUk streams that are being processed.  It needs to
**                  be set to the same value for all links that require skew to
**                  be removed.
**
**                  If MON_TS_ADDER set to a value less than the delay present
**                  on an ODUk stream, the delay generated will be 125 us -
**                  MON_TS_ADDER.
**
**                  CBRB FIFO size is limited, for latency requirements
**                  exceeding CBRB depth, the system side device must be able
**                  to provide buffering.  Data streams from OTU Framers or
**                  VCAT subsystem do not provide this additional buffering.
**                  In order to deskew these ODUk streams, they must pass
**                  through the fabric.
**
**                  It may be called after user data is actively flowing
**                  through the CBRB.  If the CBRB underflows, it must
**                  be triggered again after the recovered data stream is
**                  flowing through the CBRB.  Triggering prior to the
**                  data stream being reestablished will use the last
**                  timestamp received, and will have no coherence to
**                  aligning the new incoming ODUk data.
**
**                  This functionality is mutually exclusive with CBRB FIFO
**                  Centering mechanism discussed in hyPhy20gCbrbOtnZoneChnlCfg.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - PMM instance
**                                    OTU1 subsystem interface:
**                                    0 to 7 OTU1 framer 0 to 7
**                                    OTU2 subsystem interface:
**                                    8 to 9 OTU2 framer 0 to 1
**                                    VCAT interface:
**                                    10 to 13 VCAT interface 0 (ODU1 mode)
**                                    14 to 17 VCAT interface 1 (ODU1 mode)
**                                    10 VCAT interface 0 (ODU2 mode)
**                                    14 VCAT interface 1 (ODU2 mode)
**                  tsAddr          - Configures the MON_TS_ADDER field. Used
**                                    to specify the maximum fabric delay. See
**                                    description.
**                                    Legal values 0 to 38879
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOpiPmmTmStmpTrig(struct file *fileHndl, UINT4 slice, UINT4 tsAddr)
{

    /* variable declaration */
    UINT4 monTsAdderMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_1_MON_TS_ADDER_MSK;
    UINT4 monTsTrigMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_1_MON_TS_TRIGGER_MSK;
    UINT4 monCfgReg;
    UINT4 regData;
    INT4 result;

    /* argument checking */
    if((slice > 17)||(tsAddr > 38879)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    monCfgReg = HYPHY20G_OPI_TOP_REG_PKT_MON_CFG_1_OFFSET(slice);

    /* configure MON_TS_ADDER bits and set MON_TS_TRIGGER to 0 */
    result = sysHyPhy20gRead(fileHndl, monCfgReg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monTsAdderMsk, tsAddr);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monTsTrigMsk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfgReg, regData);
    if(result) return result;

    /* set MON_TS_TRIGGER to 1 */
    result = hyPhy20gRegFieldWrite(fileHndl, monCfgReg, monTsTrigMsk, 1);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOpiPmmTmStmpTrig */

/*******************************************************************************
**
**  hyPhy20gOpiPmmForceCsi
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function forces the CSI value in appended timestamps
**                  to the value specified in csiAlarmVal of
**                  hyPhy20gOpiPmmTmStmpCfg.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - PMM instance
**                                    OTU1 subsystem interface:
**                                    0 to 7 OTU1 framer 0 to 7
**                                    OTU2 subsystem interface:
**                                    8 to 9 OTU2 framer 0 to 1
**                                    VCAT interface:
**                                    10 to 13 VCAT interface 0 (ODU1 mode)
**                                    14 to 17 VCAT interface 1 (ODU1 mode)
**                                    10 VCAT interface 0 (ODU2 mode)
**                                    14 VCAT interface 1 (ODU2 mode)
**                  forceCsi        - forces generation of the configured CSI
**                                    value:
**                                    0 - disabled CSI is reported as per
**                                        upstream alarm status
**                                    1 - enabled CSI value configured in
**                                        csiAlarmVal of
**                                        hyPhy20gOpiPmmTmStmpCfg reported
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOpiPmmForceCsi(struct file *fileHndl, UINT4 slice, UINT4 forceCsi)
{
    /* variable declaration */
    UINT4 mkrCfgReg;
    UINT4 csiMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_2_MKR_FORCE_CSI_MSK;
    INT4 result;

    /* argument checking */
    if((slice > 17)||(forceCsi > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    mkrCfgReg = HYPHY20G_OPI_TOP_REG_PKT_MKR_CFG_2_OFFSET(slice);

    /* configure the FORCE_CSI bit as  specified */
    result = hyPhy20gRegFieldWrite (fileHndl, mkrCfgReg, csiMsk, forceCsi);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOpiPmmForceCsi */

/*******************************************************************************
**
**  hyPhy20gOpiPmmMonEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables the operating mode of the packet
**                  monitor.
**
**                  It will unconditionally set the packet monitor to send
**                  packet size data to ACB blocks unless 'disabled' mode is
**                  selected.
**
**                  When the datapath is enabled to loopback received data from
**                  companion packet maker block, PMM looptiming will be
**                  selected.
**
**                  When mode 3 is specified, PREDICTOR_NUMBERATOR and
**                  PREDICTOR_DENOM will be forced to predictor enabled format
**                  setings for the PMM packet size being used. This setting 
**                  applies to OPI ports towards OTU1 framers or ODUKPP OTU1 
**                  framers that are bound for timeslots of OTU2 subsystem 
**                  ODTU12 MUX timeslot.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - PMM instance
**                                    OTU1 subsystem interface:
**                                    0 to 7 OTU1 framer 0 to 7
**                                    OTU2 subsystem interface:
**                                    8 to 9 OTU2 framer 0 to 1
**                                    VCAT interface:
**                                    10 to 13 VCAT interface 0 (ODU1 mode)
**                                    14 to 17 VCAT interface 1 (ODU1 mode)
**                                    10 VCAT interface 0 (ODU2 mode)
**                                    14 VCAT interface 1 (ODU2 mode)
**                  mode            - Monitor Operating Mode
**                                    0 - disabled
**                                    1 - enabled for normal data path
**                                    2 - enabled to loopback data received
**                                        from companion packet maker block
**                                    3 - enabled for normal data path for an
**                                        ODU1 destined to ODTU12 MUX.
**                                    4 - enabled for SONET internal loop timing
**                                        processing  
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOpiPmmMonEnable(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    /* variable declaration */
    sHYPHY20G_CTXT  *pDevCtxt;
    UINT4 devId, revId, status;
    UINT4 monCfgReg, regData, monCfg2Reg, monCfg4Reg;
    UINT4 enableMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_1_MON_ENABLE_MSK;
    UINT4 cntEnableMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_1_MON_CNT_ENABLE_MSK;
    UINT4 lpTmMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_1_MON_LOOPTIME_MSK;
    UINT4 lpBkMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_1_MON_LOOPBACK_MSK;
    UINT4 predDenMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_2_PREDICTOR_DENOM_MSK;
    UINT4 predNumMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_2_PREDICTOR_NUMER_MSK;
    UINT4 cntEnableVal, enableVal;
    UINT4 predDen, predNum, predNumMsb = 0, predDenMsb = 0;
    UINT4 predNumMsbMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_4_PREDICTOR_NUMER_MSK;
    UINT4 predDenMsbMsk = HYPHY20G_OPI_TOP_BIT_PKT_MON_CFG_4_PREDICTOR_DENOM_MSK;
    UINT4 lpTmVal = 0;
    UINT4 lpBkVal = 0;
    INT4 result;

    /* argument checking */
    if((slice > 17)||(mode > 4)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* evaluation of compatibility between SW driver and device */
    result = hyPhy20gCheckRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (mode == 4 && (revId != HYPHY20G_REV_ID_C && revId != HYPHY20G_REV_ID_D)) {
        /* invalid mode rev B do nothing */
        return HYPHY20G_SUCCESS;
    }

    /* set PREDICTOR_DENOM and PREDICTOR_NUMER
       for an ODU1 targetting ODTU12 MUX */
    if(mode == 3){
        switch (pDevCtxt->gData.hyphy20gCbrbPacketSizeMode) {
            case 0: /* standard */    
            case 1: /* standard predictor compensated */
                predDen = 0x77;
                predNum = 0x3;
                break;
            case 2: /* custom fabric */
                predDen = 0xEE; /* 238d */
                predNum = 0xFFFF8F; /* -113d */
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
        /* If PREDICTOR_DENOM or PREDICTOR_NUMER is greater than 255, extended 
           registers are used */ 
        if (predNum > 0xFF) {
            predNumMsb = predNum >> 8;
            predNum &= 0xFF;        
        }
        if (predDen > 0xFF) {
            predDenMsb = predDen >> 8;
            predDen &= 0xFF;        
        }
        /* Write MSB and LSB of PREDICTOR_NUMER and PREDICTOR_DENOM */
        /* configure Mon Cfg Reg 4 */            
        monCfg4Reg = HYPHY20G_OPI_TOP_REG_PKT_MON_CFG_4_OFFSET(slice);
    
        result = sysHyPhy20gRead(fileHndl, monCfg4Reg, &regData);
        if(result) return result; 
    
        result = hyPhy20gVarFieldWrite (&regData, predDenMsbMsk, predDenMsb);
        if(result) return result;
    
        result = hyPhy20gVarFieldWrite (&regData, predNumMsbMsk, predNumMsb);
        if(result) return result;
    
        result = sysHyPhy20gWrite(fileHndl, monCfg4Reg, regData);
        if(result) return result;

        /* configure Mon Cfg Reg 2 */
        monCfg2Reg = HYPHY20G_OPI_TOP_REG_PKT_MON_CFG_2_OFFSET(slice);

        result = sysHyPhy20gRead(fileHndl, monCfg2Reg, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, predDenMsk, predDen);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData, predNumMsk, predNum);
        if(result) return result;

        result = sysHyPhy20gWrite(fileHndl, monCfg2Reg, regData);
        if(result) return result;
    }

    switch (mode) {
        case 0: /* 0 - disabled */
            cntEnableVal = 0;
            enableVal = 0;
            lpTmVal = 0;
            lpBkVal = 0;
            break;
        case 1: /* 1 - enabled for normal data path */
            cntEnableVal = 1;
            enableVal = 1;
            lpTmVal = 0;
            lpBkVal = 0;
            break;
        case 2: /* 2 - enabled to loopback data received from companion packet maker block */
            cntEnableVal = 1;
            enableVal = 1;
            lpTmVal = 1;
            lpBkVal = 1;
            break;
        case 3: /* 3 - enabled for normal data path for an ODU1 destined to ODTU12 MUX. */
            cntEnableVal = 1;
            enableVal = 1;
            lpTmVal = 0;
            lpBkVal = 0;
            break;
        case 4: /* 4 - enabled for SONET internal loop timing processing */
            cntEnableVal = 1;
            enableVal = 1;
            lpTmVal = 1;
            lpBkVal = 0;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }


    monCfgReg = HYPHY20G_OPI_TOP_REG_PKT_MON_CFG_1_OFFSET(slice);

    result = sysHyPhy20gRead(fileHndl, monCfgReg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, enableMsk, enableVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, cntEnableMsk, cntEnableVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, lpTmMsk, lpTmVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, lpBkMsk, lpBkVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfgReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOpiPmmMonEnable */

/*******************************************************************************
**
**  hyPhy20gOpiPmmMkrEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables the operating mode of the packet
**                  maker.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - PMM instance
**                                    OTU1 subsystem interface:
**                                    0 to 7 OTU1 framer 0 to 7
**                                    OTU2 subsystem interface:
**                                    8 to 9 OTU2 framer 0 to 1
**                                    VCAT interface:
**                                    10 to 13 VCAT interface 0 (ODU1 mode)
**                                    14 to 17 VCAT interface 1 (ODU1 mode)
**                                    10 VCAT interface 0 (ODU2 mode)
**                                    14 VCAT interface 1 (ODU2 mode)
**                  mode            - Maker Operating Mode
**                                    0 - disabled
**                                    1 - enable, generate packets toward
**                                        CBRB or process packets for SONET
**                                        internal loop timing
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOpiPmmMkrEnable(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    /* variable declaration */
    UINT4 mkrCfgReg;
    UINT4 enableMsk = HYPHY20G_OPI_TOP_BIT_PKT_MKR_CFG_1_MKR_ENABLE_MSK;
    INT4 result;

    /* argument checking */
    if((slice > 17)||(mode > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    mkrCfgReg = HYPHY20G_OPI_TOP_REG_PKT_MKR_CFG_1_OFFSET(slice);

    /* configure the MKR_ENABLE bit as  specified */
    result = hyPhy20gRegFieldWrite (fileHndl, mkrCfgReg, enableMsk, mode);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOpiPmmMkrEnable */

/*******************************************************************************
**
**  hyPhy20gLsciPmmAnyRateInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes a PMM instance associated
**                  with the LSCI subsystem.
**
**                  It is specifically provided to configure any rate CBR
**                  clients for mapping to FA GFP-T or GFP-F.  These CBR clients
**                  pass through FC_CBR block and are used to generate phase
**                  mode timing for the ACB associated with transmit Serdes.
**
**                  The function is used in place of hyPhy20gLsciPmmInit for
**                  initializing an LSCI PMM.  To disable a LSCI PMM instance
**                  use hyPhy20gLsciPmmInit.
**
**                  Should an any rate CBR client use CPB fifo depth to control
**                  the transmit Serdes rate, then hyPhy20gLsciPmmInit may be
**                  called with timingModes set to 0 (for adaptive FIFO).
**
**                  It configures the requires packet size for the specified
**                  application and other required settings.
**
**                  In addition it configures LSCI BUS_WIDTH settings and
**                  FC_CBR client DLOLB alarm event integration periods.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - LSCI PMM instance
**                                    0 to 15
**                  rateKbits       - CBR Client rate in Kbps
**                                    Legal Range:
**                                    16000 to 5000000
**                  encoding        - CBR Client encoding:
**                                    0 - non-8b10b encoded
**                                    1 - 8b10b encoded
**                  busWidth        - Internal Bus Width used
**                                    FC_CBR supported values:
**                                    0 - 8 bit (valid for encoding set to 0)
**                                    1 - 10 bit (valid for encoding set to 1)
**                                    2 - 16 bit (valid for encoding set to 0)
**                                    3 - 20 bit (valid for encoding set to 1)
**                                    Must be set to same value as busWidth
**                                    of hyPhy20gSfpCustomRateCfg
**                  size            - Packet Size value to be used by PMM
**                                    Legal range non-8b10b client Rev B
**                                    67 to 511
**                                    Legal range 8b10b client Rev B
**                                    60 to 507
**                  period          - Packet Period to be used by PMM
**                                    Legal range
**                                    1 to 65536
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciPmmAnyRateInit(struct file *fileHndl, UINT4 slice,
                                UINT4 rateKbits, UINT4 encoding, UINT4 busWidth,
                                UINT4 size, UINT4 period)
{
    /* variable declaration */
    UINT4 fcCbrReg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(
                                    HYPHY20G_LSCI_FC_CBR_REG_CFG, slice);
    UINT4 fcCbrBusWidthMsk = HYPHY20G_LSCI_FC_CBR_BIT_CFG_BUS_WIDTH_MSK;
    UINT4 addr;

    UINT4 pmmRstMsk = HYPHY20G_LSCI_TOP_BIT_CFG_PMM_RST_MSK;
    UINT4 lsciCfgReg;
    UINT4 mkrPktPeriodMsk =
                         HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_PERIOD_MSK;
    UINT4 mkrLosDisMsk =
                        HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_LOS_DISABLE_MSK;
    UINT4 mkrLosDisGeGfpTMsk =
                HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_LOS_DISABLE_GE_GFPT_MSK;
    UINT4 mkrLosDisFcCbrMsk =
                 HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_LOS_DISABLE_FC_CBR_MSK;
    UINT4 mkrPktSzVarMsk =
                       HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_SIZE_VAR_MSK;
    UINT4 dsmTypeMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_DSM_TYPE_MSK;
    UINT4 mkrDithEnMsk =
                        HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_DITH_ENABLE_MSK;
    UINT4 mkrEnMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_ENABLE_MSK;
    UINT4 mkrCfg1Reg;
    UINT4 mkrPktSzMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_2_MKR_PKT_SIZE_MSK;
    UINT4 busWidthMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_2_BUS_WIDTH_MSK;
    UINT4 mkrCfg2Reg;
    UINT4 mkrDsmDenMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_3_MKR_DSM_DENOM_MSK;
    UINT4 mkrCfg3Reg;
    UINT4 clkCtrlEnMsk =
                   HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_ADPV_CLK_CTRL_ENABLE_MSK;
    UINT4 monLpBkMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_MON_LOOPBACK_MSK;
    UINT4 monLpTmMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_MON_LOOPTIME_MSK;
    UINT4 monCntEnMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_MON_CNT_ENABLE_MSK;
    UINT4 monEnMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_MON_ENABLE_MSK;
    UINT4 monCfg1Reg;
    UINT4 monPktSzMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_2_MON_PKT_SIZE_MSK;
    UINT4 predDenMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_2_PREDICTOR_DENOM_MSK;
    UINT4 predNumMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_2_PREDICTOR_NUMER_MSK;
    UINT4 monCfg2Reg;
    UINT4 mkrPktPeriodVal, mkrLosDisVal, mkrLosDisGeGfpTVal, mkrLosDisFcCbrVal;
    UINT4 mkrPktSzVarVal, dsmTypeVal, mkrDithEnVal, mkrEnVal;
    UINT4 mkrPktSzVal, busWidthVal;
    UINT4 mkrDsmDenVal;
    UINT4 clkCtrlEnVal, monLpBkVal, monLpTmVal, monCntEnVal, monEnVal;
    UINT4 monPktSzVal, predDenVal, predNumVal;
    UINT4 regData;
    UINT4 devRevId;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 15)||(rateKbits < 16000)||(rateKbits > 5000000)||
       (encoding > 1)||(busWidth > 3)||
       ((encoding == 0)&&((busWidth == 1)||(busWidth == 3)))||
       ((encoding == 1)&&((busWidth == 0)||(busWidth == 2)))||
       ((encoding == 0)&&(size < 67))||
       ((encoding == 1)&&(size < 60))||
       (period > 65536)||(period < 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* get device revision */
    result = sysHyPhy20gGetDeviceRevId(fileHndl, &devRevId);
    if (result) return result;

    /* argument check upper range of packet size based on revId */
    if(((devRevId == 0)&&(encoding == 0)&&(size > 254))||
       ((devRevId == 1)&&(encoding == 0)&&(size > 511))||
       ((devRevId == 0)&&(encoding == 1)&&(size > 251))||
       ((devRevId == 1)&&(encoding == 1)&&(size > 507))){
        return HYPHY20G_ERR_INVALID_ARG;
    }


    lsciCfgReg = HYPHY20G_LSCI_TOP_REG_CFG;
    lsciCfgReg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(lsciCfgReg, slice);
    mkrCfg1Reg = HYPHY20G_LSCI_TOP_REG_PAC_MKR_CFG_1;
    mkrCfg1Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(mkrCfg1Reg, slice);
    mkrCfg2Reg = HYPHY20G_LSCI_TOP_REG_PAC_MKR_CFG_2;
    mkrCfg2Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(mkrCfg2Reg, slice);
    mkrCfg3Reg = HYPHY20G_LSCI_TOP_REG_PAC_MKR_CFG_3;
    mkrCfg3Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(mkrCfg3Reg, slice);
    monCfg1Reg = HYPHY20G_LSCI_TOP_REG_PAC_MON_CFG_1;
    monCfg1Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(monCfg1Reg, slice);
    monCfg2Reg = HYPHY20G_LSCI_TOP_REG_PAC_MON_CFG_2;
    monCfg2Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(monCfg2Reg, slice);

    /* generate default settings for phase mode CPB packet generation
       using supplied parameters */
    mkrLosDisVal = 1;
    mkrLosDisGeGfpTVal = 0;
    mkrLosDisFcCbrVal = 1;
    mkrEnVal = 0;

    clkCtrlEnVal = 0;
    monLpBkVal = 0;
    monLpTmVal = 0;
    monCntEnVal = 0;
    monEnVal = 0;

    dsmTypeVal = 1;
    mkrDithEnVal = 1;
    mkrDsmDenVal = 1024;
    predDenVal =1;
    predNumVal = 0;
    mkrPktPeriodVal = period;
    mkrPktSzVal = size;
    monPktSzVal = mkrPktSzVal;
    mkrPktSzVarVal = encoding;

    /* determine busWidth value for local usage code */
    switch (busWidth) {
        case 0: /* 8 bit */
            busWidthVal = 5;
            break;
        case 1: /* 10 bit */
            busWidthVal = 4;
            break;
        case 2: /* 16 bit */
            busWidthVal = 3;
            break;
        case 3: /* 20 bit */
            busWidthVal = 2;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }


    /* set PMM_RESET = 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, lsciCfgReg, pmmRstMsk, 1);
    if(result) return result;

    /* configure bus width for LSCI FC_CBR block. Note the bus width
       encoding is different with bus width register in PMM */
    result = hyPhy20gRegFieldWrite(fileHndl, fcCbrReg, fcCbrBusWidthMsk,
                                   (0x5 - busWidthVal));
    if(result) return result;

    /* configure FC_CBR DLOLB event integration periods - an integration
       period of 100 ms is applied */
    result = lsciHyPhy20gLsciRxFcDlolbIntPer(fileHndl, slice, 100,
                                             rateKbits,
                                             (0x5 - busWidthVal));
    if(result) return result;


    /* configure PMM */

    /* configure Mon Cfg Reg 1 */
    result = sysHyPhy20gRead(fileHndl, monCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,  clkCtrlEnMsk,  clkCtrlEnVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,  monLpBkMsk,  monLpBkVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monLpTmMsk, monLpTmVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monCntEnMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monEnMsk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfg1Reg, regData);
    if(result) return result;

    /* configure Mon Cfg Reg 2 */
    result = sysHyPhy20gRead(fileHndl, monCfg2Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monPktSzMsk, monPktSzVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, predDenMsk, predDenVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, predNumMsk, predNumVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfg2Reg, regData);
    if(result) return result;

    /* configure Mkr Cfg Reg 3 */
    result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg3Reg, mkrDsmDenMsk,
                                    mkrDsmDenVal);
    if(result) return result;

    /* configure Mkr Cfg Reg 2 */
    result = sysHyPhy20gRead(fileHndl, mkrCfg2Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktSzMsk, mkrPktSzVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, busWidthMsk, busWidthVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, mkrCfg2Reg, regData);
    if(result) return result;

    /* configure Mkr Cfg Reg 1 */
    result = sysHyPhy20gRead(fileHndl, mkrCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktPeriodMsk, mkrPktPeriodVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrLosDisMsk, mkrLosDisVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrLosDisGeGfpTMsk,
                                    mkrLosDisGeGfpTVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrLosDisFcCbrMsk,
                                    mkrLosDisFcCbrVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktSzVarMsk, mkrPktSzVarVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, dsmTypeMsk, dsmTypeVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrDithEnMsk, mkrDithEnVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrEnMsk, mkrEnVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, mkrCfg1Reg, regData);
    if(result) return result;

    /* set PMM_RESET = 0 */
    result = hyPhy20gRegFieldWrite (fileHndl, lsciCfgReg, pmmRstMsk, 0);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(
                                    HYPHY20G_LSCI_FC_CBR_REG_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_FC_CBR_BIT_CFG_SOFT_RST_OFF, 0);
    if(result) return result;


    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciPmmAnyRateInit */

/*******************************************************************************
**
**  hyPhy20gLsciPmmSet
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This funtion sets PMM maker/monitor packet size and period
**                  for LSCI PMMs. 
**                  This function may be called after hyPhy20gLsciPmmInit 
**                  or hyPhy20gLsciPmmAnyRateInit. 
**
**                  Note that, at most time, hyPhy20gLsciPmmInit have already
**                  set best-fit packet size and period values for each payload
**                  type, thus no need to call this function to configure these
**                  parameters again. 
**                  Use this function to change these parameters only when the
**                  existing settings can not work.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI PMM instance
**                              0 to 15
**                  monitorPktSize
**                            - PMM monitor packet size.
**                  makerPktSize
**                            - PMM maker packet size.
**                  makerPktPeriod
**                            - PMM maker packet period.
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciPmmSet(struct file *fileHndl, UINT4 slice, 
                            UINT4 monitorPktSize, UINT4 makerPktSize, 
                            UINT4 makerPktPeriod)
{
    UINT4 monCfg1Reg, monCfg2Reg, monPktSzMsk, monEnMsk;
    UINT4 mkrCfg1Reg, mkrCfg2Reg, mkrPktSzMsk, mkrPktPeriodMsk, mkrDithEnMsk, mkrEnMsk;
    UINT4 regData; 
    UINT4 pmm_monitor_pkt_size, pmm_maker_pkt_size, pmm_maker_pkt_period;
    INT4 result;
    
    /* argument checking */
    if((slice > 15)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pmm_monitor_pkt_size = monitorPktSize;
    pmm_maker_pkt_size = makerPktSize;
    pmm_maker_pkt_period = makerPktPeriod;

    /* Config PMM monitor and maker */
    mkrCfg1Reg = HYPHY20G_LSCI_TOP_REG_PAC_MKR_CFG_1;
    mkrCfg1Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(mkrCfg1Reg, slice);
    mkrCfg2Reg = HYPHY20G_LSCI_TOP_REG_PAC_MKR_CFG_2;
    mkrCfg2Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(mkrCfg2Reg, slice);

    monCfg1Reg = HYPHY20G_LSCI_TOP_REG_PAC_MON_CFG_1;
    monCfg1Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(monCfg1Reg, slice);
    monCfg2Reg = HYPHY20G_LSCI_TOP_REG_PAC_MON_CFG_2;
    monCfg2Reg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(monCfg2Reg, slice);

    monEnMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_1_MON_ENABLE_MSK;
    monPktSzMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MON_CFG_2_MON_PKT_SIZE_MSK;
    mkrEnMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_ENABLE_MSK;
    mkrPktSzMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_2_MKR_PKT_SIZE_MSK;
    mkrPktPeriodMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_PERIOD_MSK;
    mkrDithEnMsk = HYPHY20G_LSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_DITH_ENABLE_MSK;

    /* disable PMM monitor and maker */
    result = hyPhy20gRegFieldWrite(fileHndl, monCfg1Reg, monEnMsk, 0);
    if(result) return( result );
    result = hyPhy20gRegFieldWrite(fileHndl, mkrCfg1Reg, mkrEnMsk, 0);
    if(result) return( result );

    /* configure Mon Cfg Reg 2 */
    result = hyPhy20gRegFieldWrite(fileHndl, monCfg2Reg, monPktSzMsk, pmm_monitor_pkt_size);
    if(result) return( result );

    /* configure Mkr Cfg Reg 2 */
    result = hyPhy20gRegFieldWrite(fileHndl, mkrCfg2Reg, mkrPktSzMsk, pmm_maker_pkt_size);
    if(result) return( result );

    /* configure Mkr Cfg Reg 1 */
    result = sysHyPhy20gRead(fileHndl, mkrCfg1Reg, &regData);
    if(result) return( result );

    result = hyPhy20gVarFieldWrite (&regData, mkrPktPeriodMsk, pmm_maker_pkt_period);
    if(result) return( result );

    result = hyPhy20gVarFieldWrite (&regData, mkrDithEnMsk, 0x1);
    if(result) return( result );

    result = sysHyPhy20gWrite(fileHndl, mkrCfg1Reg, regData);
    if(result) return( result );

    /* enable PMM monitor and maker */
    result = hyPhy20gRegFieldWrite(fileHndl, monCfg1Reg, monEnMsk, 1);
    if(result) return( result );
    result = hyPhy20gRegFieldWrite(fileHndl, mkrCfg1Reg, mkrEnMsk, 1);
    if(result) return( result );

    return HYPHY20G_SUCCESS;
}

/*******************************************************************************
**
**  hyPhy20gHsciPmmAnyRateInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes a PMM instance associated
**                  with the HSCI subsystem.
**
**                  It is specifically provided to configure any rate CBR
**                  clients for mapping to FA GFP-T or GFP-F.  These CBR clients
**                  pass through FC_CBR block and are used to generate phase
**                  mode timing for the ACB associated with transmit Serdes.
**
**                  The function is used in place of hyPhy20gHsciPmmInit for
**                  initializing an HSCI PMM.  To disable a HSCI PMM instance
**                  use hyPhy20gHsciPmmInit.
**
**                  Should an any rate CBR client use CPB fifo depth to control
**                  the transmit Serdes rate, then hyPhy20gHsciPmmInit may be
**                  called with timingMode set for adaptive FIFO.
**
**                  It configures the requires packet size for the specified
**                  application and other required settings.
**
**                  In addition it configures HSCI BUS_WIDTH settings and
**                  FC_CBR client DLOLB alarm event integration periods.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - HSCI PMM instance
**                                    0 to 1
**                  rateKbits       - CBR Client rate in Kbps
**                                    Legal Range:
**                                    8000000 to 11320000
**                  encoding        - CBR Client encoding:
**                                    0 - non-8b10b encoded
**                                    1 - 8b10b encoded
**                  size            - Packet Size value to be used by PMM
**                                    Legal range non-8b10b client Rev B
**                                    67 to 511
**                                    Legal range 8b10b client Rev B
**                                    60 to 507
**                  period          - Packet Period to be used by PMM
**                                    Legal range
**                                    1 to 65536
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciPmmAnyRateInit(struct file *fileHndl, UINT4 slice,
                                UINT4 rateKbits, UINT4 encoding, UINT4 size,
                                UINT4 period)
{
    /* variable declaration */
    UINT4 addr;
    UINT4 fcCbrBusWidthMsk = HYPHY20G_HSCI_FC_CBR_BIT_CFG_BUS_WIDTH_MSK;
    UINT4 pmmRstMsk = HYPHY20G_HSCI_TOP_BIT_CFG_PMM_RST_MSK;
    UINT4 hsciCfgReg;
    UINT4 mkrPktPeriodMsk =
                         HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_PERIOD_MSK;
    UINT4 mkrLosDisMsk =
                        HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_LOS_DISABLE_MSK;
    UINT4 mkrLosDisFcCbrMsk =
                 HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_LOS_DISABLE_FC_CBR_MSK;
    UINT4 mkrPktSzVarMsk =
                       HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_SIZE_VAR_MSK;
    UINT4 dsmTypeMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_DSM_TYPE_MSK;
    UINT4 mkrDithEnMsk =
                        HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_DITH_ENABLE_MSK;
    UINT4 mkrEnMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_ENABLE_MSK;
    UINT4 mkrCfg1Reg;
    UINT4 mkrPktSzMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_2_MKR_PKT_SIZE_MSK;
    UINT4 busWidthMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_2_BUS_WIDTH_MSK;
    UINT4 mkrCfg2Reg;
    UINT4 mkrDsmDenMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_3_MKR_DSM_DENOM_MSK;
    UINT4 mkrCfg3Reg;
    UINT4 clkCtrlEnMsk =
                   HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_ADPV_CLK_CTRL_ENABLE_MSK;
    UINT4 monLpBkMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_LOOPBACK_MSK;
    UINT4 monLpTmMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_LOOPTIME_MSK;
    UINT4 monCntEnMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_CNT_ENABLE_MSK;
    UINT4 monEnMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_ENABLE_MSK;
    UINT4 monCfg1Reg;
    UINT4 monPktSzMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_2_MON_PKT_SIZE_MSK;
    UINT4 predDenMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_2_PREDICTOR_DENOM_MSK;
    UINT4 predNumMsk = HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_2_PREDICTOR_NUMER_MSK;
    UINT4 monCfg2Reg;
    UINT4 mkrPktPeriodVal, mkrLosDisVal, mkrLosDisFcCbrVal;
    UINT4 mkrPktSzVarVal, dsmTypeVal, mkrDithEnVal, mkrEnVal;
    UINT4 mkrPktSzVal, busWidthVal;
    UINT4 mkrDsmDenVal;
    UINT4 clkCtrlEnVal, monLpBkVal, monLpTmVal, monCntEnVal, monEnVal;
    UINT4 monPktSzVal, predDenVal, predNumVal;
    UINT4 regData;
    UINT4 devRevId;
    INT4 result;

    /* argument checking */
    if((slice > 15)||(rateKbits < 8000000)||(rateKbits > 11320000)||
       (encoding > 1)||
       ((encoding == 0)&&(size < 67))||
       ((encoding == 1)&&(size < 60))||
       (period > 65536)||(period < 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* get device revision */
    result = sysHyPhy20gGetDeviceRevId(fileHndl, &devRevId);
    if (result) return result;

    /* argument check upper range of packet size based on revId */
    if(((devRevId == 0)&&(encoding == 0)&&(size > 254))||
       ((devRevId == 1)&&(encoding == 0)&&(size > 511))||
       ((devRevId == 0)&&(encoding == 1)&&(size > 251))||
       ((devRevId == 1)&&(encoding == 1)&&(size > 507))){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    hsciCfgReg = HYPHY20G_HSCI_TOP_REG_CFG;
    hsciCfgReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(hsciCfgReg, slice);
    mkrCfg1Reg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_1;
    mkrCfg1Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(mkrCfg1Reg, slice);
    mkrCfg2Reg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_2;
    mkrCfg2Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(mkrCfg2Reg, slice);
    mkrCfg3Reg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_3;
    mkrCfg3Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(mkrCfg3Reg, slice);
    monCfg1Reg = HYPHY20G_HSCI_TOP_REG_PAC_MON_CFG_1;
    monCfg1Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(monCfg1Reg, slice);
    monCfg2Reg = HYPHY20G_HSCI_TOP_REG_PAC_MON_CFG_2;
    monCfg2Reg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(monCfg2Reg, slice);

    /* generate default settings for non-phase mode CPB packet generation
       settings to be modified on a case by case basis relative to user
       specified application */


    mkrLosDisVal = 1;
    mkrLosDisFcCbrVal = 1;
    mkrEnVal = 0;

    clkCtrlEnVal = 0;
    monLpBkVal = 0;
    monLpTmVal = 0;
    monCntEnVal = 0;
    monEnVal = 0;

    mkrPktPeriodVal = period;
    mkrPktSzVal = size;
    monPktSzVal = mkrPktSzVal;

    mkrPktSzVarVal = encoding;
    dsmTypeVal = 1;
    mkrDithEnVal = 1;
    mkrDsmDenVal = 1024;
    predDenVal =1;
    predNumVal = 0;

    /* determine busWidth value for local usage code */
    switch (encoding) {
        case 0: /* non 8b10b encoded */
            busWidthVal = 1; /* 32 bits */
            break;
        case 1: /* 8b10b encoded */
            busWidthVal = 0; /* 40 bits */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* set PMM_RESET = 1 */
    result = hyPhy20gRegFieldWrite (fileHndl, hsciCfgReg, pmmRstMsk, 1);
    if(result) return result;

    /* configure bus width for HSCI FC_CBR block. Note the bus width encoding
       is different with bus width register in PMM */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_CFG,
                                               slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, fcCbrBusWidthMsk,
                                   (0x5 - busWidthVal));
    if(result) return result;

    /* configure FC_CBR DLOLB event integration periods - an integration
       period of 100 ms is applied */
    result = hsciHyPhy20gHsciRxFcDlolbIntPer(fileHndl, slice, 100,
                                             rateKbits,
                                             (0x5 - busWidthVal));
    if(result) return result;

    /* configure Mon Cfg Reg 1 */
    result = sysHyPhy20gRead(fileHndl, monCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,  clkCtrlEnMsk,  clkCtrlEnVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,  monLpBkMsk,  monLpBkVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monLpTmMsk, monLpTmVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monCntEnMsk, 0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monEnMsk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfg1Reg, regData);
    if(result) return result;

    /* configure Mon Cfg Reg 2 */
    result = sysHyPhy20gRead(fileHndl, monCfg2Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, monPktSzMsk, monPktSzVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, predDenMsk, predDenVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, predNumMsk, predNumVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, monCfg2Reg, regData);
    if(result) return result;

    /* configure Mkr Cfg Reg 3 */
    result = hyPhy20gRegFieldWrite (fileHndl, mkrCfg3Reg, mkrDsmDenMsk,
                                    mkrDsmDenVal);
    if(result) return result;

    /* configure Mkr Cfg Reg 2 */
    result = sysHyPhy20gRead(fileHndl, mkrCfg2Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktSzMsk, mkrPktSzVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, busWidthMsk, busWidthVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, mkrCfg2Reg, regData);
    if(result) return result;

    /* configure Mkr Cfg Reg 1 */
    result = sysHyPhy20gRead(fileHndl, mkrCfg1Reg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktPeriodMsk, mkrPktPeriodVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrLosDisMsk, mkrLosDisVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrLosDisFcCbrMsk,
                                    mkrLosDisFcCbrVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrPktSzVarMsk, mkrPktSzVarVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, dsmTypeMsk, dsmTypeVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrDithEnMsk, mkrDithEnVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, mkrEnMsk, mkrEnVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, mkrCfg1Reg, regData);
    if(result) return result;

    /* set PMM_RESET = 0 */
    result = hyPhy20gRegFieldWrite (fileHndl, hsciCfgReg, pmmRstMsk, 0);
    if(result) return result;

    /* remove FC_CBR reset */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_CFG,
                                               slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_FC_CBR_BIT_CFG_SOFT_RST_OFF, 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciPmmAnyRateInit */

/*******************************************************************************
**
**  hyPhy20gHsciPmmSet
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function sets packet size and period parameters for 
**                  HSCI PMMs.
**                  This function should be called after hyPhy20gHsciPmmInit 
**                  or hyPhy20gHsciPmmAnyRateInit. 
**
**                  Note that, at most time, hyPhy20gHsciPmmInit have already
**                  set best-fit packet size and period values for each payload
**                  type, thus no need to call this function to configure these
**                  parameters again. 
**                  Use this function to change these parameters only when the
**                  existing settings can not work.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI PMM instance
**                              0 to 1
**                  monitorPktSize
**                            - PMM monitor packet size.
**                  makerPktSize
**                            - PMM maker packet size.
**                  makerPktPeriod
**                            - PMM maker packet period.
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciPmmSet(struct file *fileHndl, UINT4 slice, 
                            UINT4 monitorPktSize, UINT4 makerPktSize, 
                            UINT4 makerPktPeriod)
{
    UINT4 cfgReg;
    UINT4 pmm_monitor_pkt_size, pmm_maker_pkt_size, pmm_maker_pkt_period;
    INT4 result;
    
    /* argument checking */
    if ((slice > 1)){
        return( HYPHY20G_ERR_INVALID_ARG );
    }

    pmm_monitor_pkt_size = monitorPktSize;
    pmm_maker_pkt_size = makerPktSize;
    pmm_maker_pkt_period = makerPktPeriod;

    /* Config PMM monitor and maker */
    /* disalbe maker and monitor */
    cfgReg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_1;
    cfgReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cfgReg, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, cfgReg, HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_ENABLE_MSK, 0);
    if(result) return(result);
    
    cfgReg = HYPHY20G_HSCI_TOP_REG_PAC_MON_CFG_1;
    cfgReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cfgReg, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, cfgReg, HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_ENABLE_MSK, 0);
    if(result) return(result);
    
    /* maker packet period */
    cfgReg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_1;
    cfgReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cfgReg, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, cfgReg, HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_PKT_PERIOD_MSK, pmm_maker_pkt_period);
    if(result) return(result);

    /* maker packet size */
    cfgReg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_2;
    cfgReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cfgReg, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, cfgReg, HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_2_MKR_PKT_SIZE_MSK, pmm_maker_pkt_size);
    if(result) return(result);

    /* monitor packet size */
    cfgReg = HYPHY20G_HSCI_TOP_REG_PAC_MON_CFG_2;
    cfgReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cfgReg, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, cfgReg, HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_2_MON_PKT_SIZE_MSK, pmm_monitor_pkt_size);
    if(result) return(result);

    /* enalbe maker and monitor */
    cfgReg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_1;
    cfgReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cfgReg, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, cfgReg, HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_ENABLE_MSK, 1);
    if(result) return(result);

    cfgReg = HYPHY20G_HSCI_TOP_REG_PAC_MKR_CFG_1;
    cfgReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cfgReg, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, cfgReg, HYPHY20G_HSCI_TOP_BIT_PAC_MKR_CFG_1_MKR_DITH_ENABLE_MSK, 1);
    if(result) return(result);
    
    cfgReg = HYPHY20G_HSCI_TOP_REG_PAC_MON_CFG_1;
    cfgReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cfgReg, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, cfgReg, HYPHY20G_HSCI_TOP_BIT_PAC_MON_CFG_1_MON_ENABLE_MSK, 1);
    if(result) return(result);

    return HYPHY20G_SUCCESS;
}

/*******************************************************************************
**
**  hyPhy20gOpiPmmOtu2DataSourceCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the line side timing and data source
**                for an OPI PMM slice associated with one of the two OTU2
**                Framers.  In normal operation timing and data is received from
**                an OTU2 Framer.  For internally loop-timing a POS Tx Serdes
**                the OPI PMM processes the recovered Rx Serdes timing rate
**                that has been routed to the PGMRCLK interface. In this case
**                the OTU2 framer generates a clock signal to the OPI PMM at
**                1/4 of the recovered clock frequency.  This recovered clock
**                frequency is processed bo the OPI PMM and used to generate
**                the Tx Serdes rate for the POS signal.
**
**                For POS transmit timing source hyPhy20gPgmrclkClkSrcCfg must 
**                be called to configure XFI 0 or XFI 1 as the recovered clock
**                source for PGMRCLK ports 1 to 4.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - OTU2 Framer Slice
**                                0 - OTU2 Framer Slice 0 / OPI PMM Slice 8
**                                1 - OTU2 Framer Slice 1 / OPI PMM Slice 9
**                 mode         - Operating Mode
**                                0 - Normal OTU2 Framer
**                                1 - POS Tx Timing Source with PGMRCLK[0] 
**                                2 - POS Tx Timing Source with PGMRCLK[1] 
**                                3 - POS Tx Timing Source with PGMRCLK[2] 
**                                4 - POS Tx Timing Source with PGMRCLK[3] 
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOpiPmmOtu2DataSourceCfg(struct file *fileHndl, UINT4 slice, 
                                     UINT4 mode)
{
    /* variable declaration */
    UINT4 devId, revId, status;
    UINT4 otnAddr, pgmrclkAddr;
    UINT4 pgmrclkSelMuxMsk, sysclkSelMuxMsk, validMsk;
    UINT4 pgmrclkSelMuxVal, sysclkSelMuxVal, validVal;
    INT4  result;

    /* Argument check */
    if ((slice > 2) || (mode > 4))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* evaluation of compatibility between SW driver and device */
    result = hyPhy20gCheckRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (revId == HYPHY20G_REV_ID_B) {
        /* rev B requirement - do nothing */
        return HYPHY20G_SUCCESS;
    }

    otnAddr = HYPHY20G_OPI_TOP_REG_SERDES_BACKPLANE_MD;
    pgmrclkAddr = HYPHY20G_PGMRCLK_REG_OTU2_RX_CLK_MUX_1_SEL;

    switch (slice) {
        case 0:
            pgmrclkSelMuxMsk = HYPHY20G_PGMRCLK_BIT_OTU2_RX_CLK_MUX_1_SEL_OTU2_RX_CLK_MUX_0_1_SEL_1_MSK;
            sysclkSelMuxMsk = HYPHY20G_OPI_TOP_BIT_SERDES_BACKPLANE_MD_OTU2_RX_CLK_MUX_0_2_SEL_MSK;
            validMsk = HYPHY20G_OPI_TOP_BIT_SERDES_BACKPLANE_MD_OTU2_RX_SYS_VALID0_MSK;
            break;
        case 1:
            pgmrclkSelMuxMsk = HYPHY20G_PGMRCLK_BIT_OTU2_RX_CLK_MUX_1_SEL_OTU2_RX_CLK_MUX_1_1_SEL_1_MSK;
            sysclkSelMuxMsk = HYPHY20G_OPI_TOP_BIT_SERDES_BACKPLANE_MD_OTU2_RX_CLK_MUX_1_2_SEL_MSK;
            validMsk = HYPHY20G_OPI_TOP_BIT_SERDES_BACKPLANE_MD_OTU2_RX_SYS_VALID1_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (mode) {
        case 0:
            pgmrclkSelMuxVal = 0;
            sysclkSelMuxVal = 0;
            validVal = 0;
            break;
        case 1:
            pgmrclkSelMuxVal = 0;
            sysclkSelMuxVal = 1;
            validVal = 1;
            break;
        case 2:
            pgmrclkSelMuxVal = 1;
            sysclkSelMuxVal = 1;
            validVal = 1;
            break;
        case 3:
            pgmrclkSelMuxVal = 2;
            sysclkSelMuxVal = 1;
            validVal = 1;
            break;
        case 4:
            pgmrclkSelMuxVal = 3;
            sysclkSelMuxVal = 1;
            validVal = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
        
    }
      
    result = hyPhy20gRegFieldWrite(fileHndl, otnAddr, validMsk, validVal);
    if(result) return result;    

    result = hyPhy20gRegFieldWrite(fileHndl, otnAddr, sysclkSelMuxMsk, sysclkSelMuxVal);
    if(result) return result; 

    result = hyPhy20gRegFieldWrite(fileHndl, pgmrclkAddr, pgmrclkSelMuxMsk, pgmrclkSelMuxVal);
    if(result) return result; 

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOpiPmmOtu2DataSourceCfg */


/* end of file */
