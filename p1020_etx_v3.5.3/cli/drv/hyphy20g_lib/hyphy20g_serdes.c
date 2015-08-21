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
**  FILE        : hyphy20g_serdes.c
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
#include "hyphy20g_serdes.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 serdesHyPhy20gSerdesSetAdcCalCtxt(struct file *fileHndl, UINT4 intf,
                                       UINT4 linkID, UINT4 valid, UINT4 calMode,
                                       UINT4 calReq, UINT4 calOk, UINT4 pisoEnb,
                                       UINT4 adsmIntDiv);

INT4 serdesHyPhy20gSerdesGetAdcCalCtxt(struct file *fileHndl, UINT4 intf,
                                       UINT4 linkID, UINT4 *pValid,
                                       UINT4 *pCalMode, UINT4 *pCalReq,
                                       UINT4 *pCalOk, UINT4 *pPisoEnb,
                                       UINT4 *pAdsmIntDiv);

INT4 serdesHyPhy20gSerdesSetMskAdcCalCtxt(struct file *fileHndl, UINT4 intf,
                                          UINT4 linkID, UINT4 valid,
                                          UINT4 calMode, UINT4 calReq,
                                          UINT4 calOk, UINT4 pisoEnb,
                                          UINT4 adsmIntDiv, UINT4 setMsk);

INT4 serdesHyPhy20gSerdesSetAdcCalOffsetCtxt(struct file *fileHndl, UINT4 intf,
                                             UINT4 linkID, UINT4 phase,
                                             UINT4 comp, UINT4 offset);

INT4 serdesHyPhy20gSerdesGetAdcCalOffsetCtxt(struct file *fileHndl, UINT4 intf,
                                             UINT4 linkID, UINT4 phase,
                                             UINT4 comp, UINT4 *pOffset);

INT4 serdesHyPhy20gSerdesSetModeSelCtxt(struct file *fileHndl, UINT4 intf,
                                        UINT4 linkID, UINT4 valid,
                                        UINT4 mapType, UINT4 accumRef0);

INT4 serdesHyPhy20gSerdesGetModeSelCtxt(struct file *fileHndl, UINT4 intf,
                                       UINT4 linkID, UINT4 *pValid,
                                       UINT4 *pMapType, UINT4 *pAccumRef0);

INT4 serdesHyPhy20gSerdesSetMskModeSelCtxt(struct file *fileHndl, UINT4 intf,
                                           UINT4 linkID, UINT4 valid,
                                           UINT4 mapType, UINT4 accumRef0,
                                           UINT4 setMsk);

INT4 serdesHyPhy20gSerdesSetEqCtxt(struct file *fileHndl, UINT4 intf,
                                   UINT4 linkID, UINT4 valid, UINT4 eqTap1,
                                   UINT4 eqTap2, UINT4 eqTap3,
                                   UINT4 timingEqTap);

INT4 serdesHyPhy20gSerdesGetEqCtxt(struct file *fileHndl, UINT4 intf,
                                   UINT4 linkID, UINT4 *pValid,
                                   UINT4 *pEqTap1, UINT4 *pEqTap2,
                                   UINT4 *pEqTap3, UINT4 *pTimingEqTap);

INT4 serdesHyPhy20gSerdesSetMskEqCtxt(struct file *fileHndl, UINT4 intf,
                                      UINT4 linkID, UINT4 valid, UINT4 eqTap1,
                                      UINT4 eqTap2, UINT4 eqTap3,
                                      UINT4 timingEqTap, UINT4 setMsk);

INT4 utilHyPhy20gXfiSerdesADCCalibrate(struct file *fileHndl, UINT4 linkID);

INT4 utilHyPhy20gSfpSerdesCsuCal(struct file *fileHndl, UINT4 linkID,
                                 UINT4 *freq);

INT4 utilHyPhy20gXfiSerdesCsuCal(struct file *fileHndl, UINT4 linkID,
                                 UINT4 *freq);

INT4 utilHyPhy20gBackplaneSerdesCsuCal(struct file *fileHndl, UINT4 csu_id,
                                       UINT4 *freq);

INT4 utilHyPhy20gPifSerdesCsuCal(struct file *fileHndl, UINT4 *freq);

INT4 utilHyPhy20gSerdesRxcpgen(struct file *fileHndl, UINT4 linkID,
                               UINT4 serdesGrp);

INT4 utilHyPhy20gSerdesValidateSfpRates(struct file *fileHndl, UINT4 linkMsk);

INT4 utilHyPhy20gSerdesPrepCalibration(struct file *fileHndl, UINT4 intf,
                                       UINT4 linkMsk);

INT4 utilHyPhy20gSerdesIntfMskPrepCalibration(struct file *fileHndl,
                                              UINT4 sfpMsk, UINT4 xfiMsk,
                                              UINT4 bpMsk, UINT4 pifMsk);

INT4 utilHyPhy20gSerdesCalibrateOnePhase(struct file *fileHndl, UINT4 intf,
                                         UINT4 linkMsk, UINT4 phase);

INT4 utilHyPhy20gSerdesIntfMskCalibrateOnePhase(struct file *fileHndl,
                                                UINT4 sfpMsk, UINT4 sfpPhase,
                                                UINT4 xfiMsk, UINT4 xfiPhase,
                                                UINT4 bpMsk, UINT4 bpPhase,
                                                UINT4 pifMsk, UINT4 pifPhase);

INT4 utilHyPhy20gSerdesCheckAdc(struct file *fileHndl, UINT4 intf,
                                UINT4 linkMsk, UINT4 calCount);

INT4 utilHyPhy20gSerdesIntfMskCheckAdc(struct file *fileHndl, UINT4 sfpMsk,
                                       UINT4 sfpCalCnt, UINT4 xfiMsk,
                                       UINT4 xfiCalCnt, UINT4 bpMsk,
                                       UINT4 bpCalCnt, UINT4 pifMsk,
                                       UINT4 pifCalCnt);

INT4 utilHyPhy20gSfpSerdesSubtRefsFrmOffsets(struct file *fileHndl,
                                             UINT4 linkID, UINT4 *pValid);

INT4 utilHyPhy20gXfiBpPifSerdesSubtRefsFrmOffsets(struct file *fileHndl,
                                                  UINT4 intf, UINT4 linkID,
                                                  UINT4 *pValid);

INT4 utilHyPhy20gSerdesRestore(struct file *fileHndl, UINT4 intf,
                               UINT4 linkMsk);

INT4 utilHyPhy20gSerdesIntfMskRestore(struct file *fileHndl, UINT4 sfpMsk,
                                      UINT4 xfiMsk, UINT4 bpMsk, UINT4 pifMsk);

INT4 utilHyPhy20gSerdesAdcSingleRun(struct file *fileHndl, UINT4 sfpMsk,
                                    UINT4 xfiMsk, UINT4 bpMsk, UINT4 pifMsk);

INT4 utilHyPhy20gSerdesAdcCalibrate(struct file *fileHndl, UINT4 intf,
                                    UINT4 linkID, UINT4 adcCal);

INT4 utilHyPhy20gSerdesGetMaxLinks(struct file *fileHndl, UINT4 intf,
                                   UINT4 *pMaxLinks);

INT4 utilHyPhy20gSerdesGetBackplaneCsuId(struct file *fileHndl, UINT4 linkID,
                                         UINT4 *pCsuID);

INT4 utilHyPhy20gSerdesGetMaxNPhaseNComp(struct file *fileHndl, UINT4 intf,
                                         UINT4 *pNPhases, UINT4 *pNComps);

INT4 utilHyPhy20gSerdesRxFifoReCenter(struct file *fileHndl, UINT4 intf,
                                      UINT4 linkID);

INT4 utilHyPhy20gSerdesCalcTxSwing(struct file *fileHndl, UINT4 preEmp,
                                   UINT4 swing, UINT4 *pTxDenCore,
                                   UINT4 *pTxSwngCtrlCore);

INT4 utilHyPhy20gWriteXfiBpPifAdcMemory(struct file *fileHndl, UINT4 intf,
                                UINT4 linkID);

INT4 utilHyPhy20gWriteSfpAdcMemory(struct file *fileHndl, UINT4 linkID);

INT4 utilHyPhy20gSetAdcThreshold(struct file *fileHndl, UINT4 intf,
                                 UINT4 linkID, UINT4 mapType, UINT4 scale);

INT4 utilHyPhy20gAdcModeSelect(struct file *fileHndl, UINT4 intf, UINT4 linkID,
                               UINT4 mode);

INT4 utilHyPhy20gSerdesLnSync(struct file *fileHndl, UINT4 serdesGrp);

INT4 utilHyPhy20gSerdesSaveModeSel(struct file *fileHndl, UINT4 intf,
                                   UINT4 linkID);

INT4 utilHyPhy20gSerdesSaveEq(struct file *fileHndl, UINT4 intf, UINT4 linkID);

INT4 utilHyPhy20gSerdesEqCfg(struct file *fileHndl, UINT4 intf, UINT4 linkID,
                             UINT4 mode);
                             
INT4 utilHyPhy20gSerdesInitLwPwr(struct file *fileHndl, UINT4 intf,
                                    UINT4 linkId);                           

static INT4 utilHyPhy20gHexStrToByte(char *pStr, UINT4 numChar, UINT4 *data);

INT4 utilHyPhy20gSerdesLoadFactoryCalCtxt(struct file *fileHndl, UINT4 intf, 
                                          UINT4 linkId, void *pCtxtBuf, 
                                          UINT4 *pParsedSize);
UINT4 utilHyPhy20gSerdesGetCtxtSize(sHYPHY20G_SERDES_CTXT *pSerdesCtxt);
UINT4 utilHyPhy20gSerdesExtractCtxt(sHYPHY20G_SERDES_CTXT *pSerdesCtxt, 
                                    void *pCtxtBuf);
UINT4 utilHyPhy20gSerdesRestoreCtxt(sHYPHY20G_SERDES_CTXT *pSerdesCtxt, 
                                    void *pCtxtBuf);
INT4 utilHyPhy20gSerdesRxcpgenForceInit(struct file *fileHndl, UINT4 linkID,
                               UINT4 serdesGrp);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gSfpRateCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the SFP Serdes subsystem rate
**                  for a given link.
**
**                  hyPhy20gSfpSerdesInit must be called prior to this
**                  function.
**
**                  ADC offset calibration should only be performed on links
**                  with floating inputs or with active receive data traffic.
**                  hyPhy20gSerdesADCCalibrate is also available to perform
**                  calibration across multiple SERDES interfaces and links.
**
**                  ADC offset calibration should NOT be performed on rates
**                  lower than 1.5Gbps.  Calibrating links with rates lower
**                  than 1.5Gbps will result in the error
**                  HYPHY20G_ERR_SERDES_ADC_SFP_RATE_CHECK.
**
**                  hyPhy20gSerdesLoadFactoryCal must be called prior to
**                  calling this function with adcCal = 2.
**
**                  When enable is 0, the low level serdes csu, rx and tx
**                  circuits are put in reset.
**
**  INPUTS:         *fileHndl - base address
**                  linkID    - 0 - SFP interface 0
**                              1 - SFP interface 1
**                              ..
**                              15 - SFP interface 15
**                  rate      - 0 - FE 125M
**                              1 - GE 1.25G
**                              2 - E3 34.368M
**                              3 - T3 44.736M
**                              4 - OC-1 51.84M (not supported)
**                              5 - OC-3 155.52M
**                              6 - OC-12 622.08M
**                              7 - OC-48 2.488G
**                              8 - OC-51 2.643G (not supported)
**                              9 - OC-60 3.11G (not supported)
**                              10 - ODU1
**                              11 - OTU1
**                              12 - FC12
**                              13 - FC25
**                              14 - FC50
**                              15 - FC100
**                              16 - FC200
**                              17 - FC400
**                              18 - CLO-ETR 16M
**                              19 - FDDI 125M
**                              20 - ESCON
**                              21 - ISC 1.0625G
**                              22 - ISC3 2.125G
**                              23 - Video 143M
**                              24 - Video 177M
**                              25 - Video SD-SDI 270M
**                              26 - Video SD-SDI 360M
**                              27 - Video 540M
**                              28 - Video HD-SDI 1.485G
**                              29 - Video HD-SDI (1.001) 1.483G
**                              30 - Video 3GHD-SDI 2.97G
**                              31 - Video 2.967M
**                              32 - DV6000 2.38G
**                              33 - Firewire 100
**                              34 - Firewire 200
**                              35 - CPRI 614M
**                              36 - CPRI 1.228G
**                              37 - CPRI 2.457G
**                              38 - CPRI 3.072G
**                              39 - CPRI 4.915G
**                              40 - DVB-ASI 270M
**                              41 - InfiniBand 5G
**                  adcCal    - 0 - do not perform ADC offset calibration
**                              1 - perform ADC offset calibration
**                              2 - use ADC calibrated offsets from SW context
**                              3 - defer ADC offset calibration (use
**                                  hyPhy20gSerdesADCCalibrate later)
**                  enable    - 0 - disable SFP subsystem
**                              1 - enable SFP subsystem
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_SERDES_ADSM_LOCK
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SERDES_ADC_SFP_RATE_CHECK
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_SERDES_ADC_CALIBRATION_FAILED
**
*******************************************************************************/
INT4 hyPhy20gSfpRateCfg(struct file *fileHndl, UINT4 linkID, UINT4 rate,
                        UINT4 adcCal, UINT4 enable)
{
    /* local variables */
    UINT4 devId, revId, status;
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 reg_msk;
    UINT4 val;
    UINT4 converge_freq;
    UINT4 rxpath_bus_width;
    UINT4 txpath_bus_width;
    UINT4 adsm_clk_cnt_max;
    UINT4 adsm_frac_div;
    UINT4 adsm_int_div;
    UINT4 adsm_denomin;
    UINT4 rx_path_div;
    UINT4 piso_div_enable;
    UINT4 piso_ctrl;
    UINT4 csu_mode;
    UINT4 vco_select;
    UINT4 acb_pll_s_max;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (linkID > HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (rate > HYPHY20G_SFP_SERDES_RATES_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (adcCal > 3) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;
    reg_msk = 0;
    converge_freq = 0;
    DBG_PRINT("%s, %s, %d, entering SfpRateCfg \n",__FILE__, __FUNCTION__, __LINE__);

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkID];
    HYPHY20G_ASSERT((pSerdesCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* initialize rate specific settings */
    switch(rate){
        case HYPHY20G_SFP_SERDES_FE_125:
            rxpath_bus_width = 0x01;
            txpath_bus_width = 0x01;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 2520033;
            adsm_int_div = 32;
            adsm_denomin = 16777206;
            rx_path_div = 0x05;
            piso_div_enable = 0x01;
            piso_ctrl = 0x04;
            csu_mode = 0x01;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_GE_1250:
            rxpath_bus_width = 0x01;
            txpath_bus_width = 0x01;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 2520033;
            adsm_int_div = 32;
            adsm_denomin = 16777206;
            rx_path_div = 0x02;
            piso_div_enable = 0x01;
            piso_ctrl = 0x01;
            csu_mode = 0x00;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_E3_34_368:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 4805300;
            adsm_int_div = 28;
            adsm_denomin = 16777125;
            rx_path_div = 0x07;
            piso_div_enable = 0x01;
            piso_ctrl = 0x06;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_T3_44_736:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 10314825;
            adsm_int_div = 27;
            adsm_denomin = 16777125;
            rx_path_div = 0x06;
            piso_div_enable = 0x01;
            piso_ctrl = 0x05;
            csu_mode = 0x02;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        /* HYPHY20G_SFP_SERDES_OC1_51_84 not supported
        case HYPHY20G_SFP_SERDES_OC1_51_84:
            rxpath_bus_width = 0x01;
            txpath_bus_width = 0x01;
            adsm_clk_cnt_max = 0;
            adsm_frac_div = 0;
            adsm_int_div = 0;
            adsm_denomin = 0;
            rx_path_div = 0;
            piso_div_enable = 0;
            piso_ctrl = 0;
            csu_mode = 0;
            vco_select = 0;
            acb_pll_s_max = 0x0;
        */
        break;
        case HYPHY20G_SFP_SERDES_OC3_155_52:
            rxpath_bus_width = 0x00;
            txpath_bus_width = 0x00;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 0;
            adsm_int_div = 32;
            adsm_denomin = 16777215;
            rx_path_div = 0x05;
            piso_div_enable = 0x01;
            piso_ctrl = 0x04;
            csu_mode = 0x00;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_OC12_622_08:
            rxpath_bus_width = 0x00;
            txpath_bus_width = 0x00;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 0;
            adsm_int_div = 32;
            adsm_denomin = 16777215;
            rx_path_div = 0x03;
            piso_div_enable = 0x01;
            piso_ctrl = 0x02;
            csu_mode = 0x00;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_OC48_2488_32:
            if (linkID % 2 == 0) {
                rxpath_bus_width = 0x00;
                txpath_bus_width = 0x00;
                adsm_clk_cnt_max = 4000;
                adsm_frac_div = 0;
                adsm_int_div = 32;
                adsm_denomin = 16777215;
                rx_path_div = 0x01;
                piso_div_enable = 0x01;
                piso_ctrl = 0x00;
                csu_mode = 0x00;
                vco_select = 0x01;
                acb_pll_s_max = 0xc;
            } else {
                rxpath_bus_width = 0x00;
                txpath_bus_width = 0x00;
                adsm_clk_cnt_max = 4000;
                adsm_frac_div = 0;
                adsm_int_div = 24;
                adsm_denomin = 16777215;
                rx_path_div = 0x00;
                piso_div_enable = 0x00;
                piso_ctrl = 0x00;
                csu_mode = 0x02;
                vco_select = 0x00;
                acb_pll_s_max = 0xc;
            }
        break;
        /* HYPHY20G_SFP_SERDES_OC51_2643_84 not supported
        case HYPHY20G_SFP_SERDES_OC51_2643_84:
            rxpath_bus_width = 0x01;
            txpath_bus_width = 0x01;
            adsm_clk_cnt_max = 0;
            adsm_frac_div = 0;
            adsm_int_div = 0;
            adsm_denomin = 0;
            rx_path_div = 0;
            piso_div_enable = 0;
            piso_ctrl = 0;
            csu_mode = 0;
            vco_select = 0;
            acb_pll_s_max = 0x0;
        break;
        */
        /* HYPHY20G_SFP_SERDES_OC60_3110_4 not supported
        case HYPHY20G_SFP_SERDES_OC60_3110_4:
            rxpath_bus_width = 0x01;
            txpath_bus_width = 0x01;
            adsm_clk_cnt_max = 0;
            adsm_frac_div = 0;
            adsm_int_div = 0;
            adsm_denomin = 0;
            rx_path_div = 0;
            piso_div_enable = 0;
            piso_ctrl = 0;
            csu_mode = 0;
            vco_select = 0;
            acb_pll_s_max = 0x0;
        break;
        */
        case HYPHY20G_SFP_SERDES_ODU1_2498_775:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 2255760;
            adsm_int_div = 32;
            adsm_denomin = 16777215;
            rx_path_div = 0x01;
            piso_div_enable = 0x01;
            piso_ctrl = 0x00;
            csu_mode = 0x00;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_OTU1_2666_057:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 4793490;
            adsm_int_div = 34;
            adsm_denomin = 16777215;
            rx_path_div = 0x01;
            piso_div_enable = 0x01;
            piso_ctrl = 0x00;
            csu_mode = 0x00;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_FC12_132_813:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 5497310;
            adsm_int_div = 27;
            adsm_denomin = 16776720;
            rx_path_div = 0x05;
            piso_div_enable = 0x01;
            piso_ctrl = 0x04;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_FC25_265_525:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 5497310;
            adsm_int_div = 27;
            adsm_denomin = 16776720;
            rx_path_div = 0x04;
            piso_div_enable = 0x01;
            piso_ctrl = 0x03;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_FC50_531_25:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 5497310;
            adsm_int_div = 27;
            adsm_denomin = 16776720;
            rx_path_div = 0x03;
            piso_div_enable = 0x01;
            piso_ctrl = 0x02;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_FC100_1062_5:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 5497310;
            adsm_int_div = 27;
            adsm_denomin = 16776720;
            rx_path_div = 0x02;
            piso_div_enable = 0x01;
            piso_ctrl = 0x01;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_FC200_2125:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 5497310;
            adsm_int_div = 27;
            adsm_denomin = 16776720;
            rx_path_div = 0x01;
            piso_div_enable = 0x01;
            piso_ctrl = 0x00;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_FC400_4250:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 5497310;
            adsm_int_div = 27;
            adsm_denomin = 16776720;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_CLOETR_16:
            rxpath_bus_width = 0x00;
            txpath_bus_width = 0x00;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 5661444;
            adsm_int_div = 26;
            adsm_denomin = 16777206;
            rx_path_div = 0x08;
            piso_div_enable = 0x01;
            piso_ctrl = 0x07;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_FDDI_125:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 2520033;
            adsm_int_div = 32;
            adsm_denomin = 16777206;
            rx_path_div = 0x05;
            piso_div_enable = 0x01;
            piso_ctrl = 0x04;
            csu_mode = 0x01;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_ESCON_200:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 226520;
            adsm_int_div = 30;
            adsm_denomin = 262116;
            rx_path_div = 0x04;
            piso_div_enable = 0x01;
            piso_ctrl = 0x03;
            csu_mode = 0x02;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_ISC_106_25:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 5497310;
            adsm_int_div = 27;
            adsm_denomin = 16776720;
            rx_path_div = 0x02;
            piso_div_enable = 0x01;
            piso_ctrl = 0x01;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_ISC3_212_5:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 5497310;
            adsm_int_div = 27;
            adsm_denomin = 16776720;
            rx_path_div = 0x01;
            piso_div_enable = 0x01;
            piso_ctrl = 0x00;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_VIDEO_143_182:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 7734210;
            adsm_int_div = 29;
            adsm_denomin = 16767000;
            rx_path_div = 0x05;
            piso_div_enable = 0x01;
            piso_ctrl = 0x04;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_VIDEO_177:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 5281713;
            adsm_int_div = 27;
            adsm_denomin = 16777206;
            rx_path_div = 0x04;
            piso_div_enable = 0x01;
            piso_ctrl = 0x03;
            csu_mode = 0x02;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_VIDEO_270:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 13048945;
            adsm_int_div = 27;
            adsm_denomin = 16777215;
            rx_path_div = 0x04;
            piso_div_enable = 0x01;
            piso_ctrl = 0x03;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_VIDEO_360:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 13048945;
            adsm_int_div = 27;
            adsm_denomin = 16777215;
            rx_path_div = 0x03;
            piso_div_enable = 0x01;
            piso_ctrl = 0x02;
            csu_mode = 0x02;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_VIDEO_540:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 13048945;
            adsm_int_div = 27;
            adsm_denomin = 16777215;
            rx_path_div = 0x03;
            piso_div_enable = 0x01;
            piso_ctrl = 0x02;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_HD_SDI_1485:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 10835275;
            adsm_int_div = 28;
            adsm_denomin = 16777200;
            rx_path_div = 0x01;
            piso_div_enable = 0x01;
            piso_ctrl = 0x00;
            csu_mode = 0x02;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_VIDEO_1483_516:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 10354999;
            adsm_int_div = 28;
            adsm_denomin = 16776942;
            rx_path_div = 0x01;
            piso_div_enable = 0x01;
            piso_ctrl = 0x00;
            csu_mode = 0x02;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_3G_HD_SDI_2970:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 10835275;
            adsm_int_div = 28;
            adsm_denomin = 16777200;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            csu_mode = 0x02;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_VIDEO_2967_033:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 10354999;
            adsm_int_div = 28;
            adsm_denomin = 16776942;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            csu_mode = 0x02;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_DV6000_2380:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 10183695;
            adsm_int_div = 30;
            adsm_denomin = 16777206;
            rx_path_div = 0x01;
            piso_div_enable = 0x01;
            piso_ctrl = 0x00;
            csu_mode = 0x00;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_FWIRE100_98_304:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 5716650;
            adsm_int_div = 30;
            adsm_denomin = 16777125;
            rx_path_div = 0x05;
            piso_div_enable = 0x01;
            piso_ctrl = 0x04;
            csu_mode = 0x02;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_FWIRE200_196_608:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 5716650;
            adsm_int_div = 30;
            adsm_denomin = 16777125;
            rx_path_div = 0x04;
            piso_div_enable = 0x01;
            piso_ctrl = 0x03;
            csu_mode = 0x02;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_CPRI_614_4:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 10149174;
            adsm_int_div = 31;
            adsm_denomin = 16777206;
            rx_path_div = 0x03;
            piso_div_enable = 0x01;
            piso_ctrl = 0x02;
            csu_mode = 0x00;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_CPRI_1228_8:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 10149174;
            adsm_int_div = 31;
            adsm_denomin = 16777206;
            rx_path_div = 0x02;
            piso_div_enable = 0x01;
            piso_ctrl = 0x01;
            csu_mode = 0x00;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_CPRI_2457_6:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 10149174;
            adsm_int_div = 31;
            adsm_denomin = 16777206;
            rx_path_div = 0x01;
            piso_div_enable = 0x01;
            piso_ctrl = 0x00;
            csu_mode = 0x00;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_CPRI_3072:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 10563426;
            adsm_int_div = 29;
            adsm_denomin = 16777206;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            csu_mode = 0x02;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_CPRI_4915_2:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 10149174;
            adsm_int_div = 31;
            adsm_denomin = 16777206;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            csu_mode = 0x00;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_DVB_ASI_270:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 13048945;
            adsm_int_div = 27;
            adsm_denomin = 16777215;
            rx_path_div = 0x04;
            piso_div_enable = 0x01;
            piso_ctrl = 0x03;
            csu_mode = 0x00;
            vco_select = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SFP_SERDES_INFINIBAND_5000:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 2520033;
            adsm_int_div = 32;
            adsm_denomin = 16777206;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            csu_mode = 0x00;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        default:
            /* redundant to initial argument checking */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /********************/
    /* reset sequencing */
    /********************/
    /* hold CSU AnalogRSTB */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_CSU_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* set CSU CTRL[11] for feedback divider */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_CSU_CTRL_OFF + 11;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* hold rxpatgen analog blocks in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold rxpatgen vco - RXCPGEN_CTRL[3] in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_CTRL_OFF + 3;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold ADC DACs in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_DAC_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold rxpatgen blocks in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold block in digital reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_DIGITAL_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold ADC in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold PISO in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold ADSM in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* enable CSU (default) */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_CSU_ENB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* turn off CSU AnalogTM (default) */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_CSU_ATMSB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    result = sysHyPhy20gMsDelay (1);
    if (result) return result;
    DBG_PRINT("%s, %s, %d, finished SfpRateCfg reset sequence \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of reset sequence */

    if (enable == 0) {
        DBG_PRINT("%s, %s, %d, enable is 0, SfpRateCfg link held in reset\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_SUCCESS;
    }

    /********************/
    /* default settings */
    /********************/
    /* enable majority filter in cdru */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_1_FILT_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* program equalizer loop filter coeff */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_3, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_3_COEFF_D_TEP_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x0C);
    if (result) return result;

    /* set LOS_ENABLE to 1 */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_4, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_4_LOS_ENABLE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
    if (result) return result;


    /* enable isolated pulse detector in cdru */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_1_IPD_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* enable soft TED in cdru */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_1_TED_SEL_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* disable ADSM seed */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_11, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_SEED_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* set ADSM_CLKO_POLARITY bit */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_12, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_12_ADSM_CLKO_POLARITY_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* set CSU_CTRL[8:6] bit */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    reg_msk = (0x0007 << 22);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x03);
    if (result) return result;

    /* set CSU_CTRL[14:13] bit */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    reg_msk = (0x0003 << 29);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
    if (result) return result;

    /* end of default settings */

    /*************************/
    /* lane/rate configs     */
    /*************************/
    /* config rx_path_div */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_RX_PATH_DIV_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rx_path_div);
    if (result) return result;

    /* program tx and rx bus widths */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_TXPATH_BUS_WIDTH_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txpath_bus_width);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_RXPATH_BUS_WIDTH_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rxpath_bus_width);
    if (result) return result;

    /* config piso - bypass divider, set divide ratio */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_DIV_ENABLE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, piso_div_enable);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_CTRL_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, piso_ctrl);
    if (result) return result;

    /* program ADSM int, frac and denomin settings */
    /* see rate specific init lookup table for all the different rates */
    /* note that ADSM_INT_DIV is split into lower and upper fields */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_10, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_INT_DIV_OFF;
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_INT_DIV_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_int_div);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_10, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_FRAC_DIV_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_frac_div);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_9, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_9_ADSM_DENOMIN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_denomin);
    if (result) return result;

    /* config csu_ctrl[3:2] */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    reg_msk = (0x0003 << 18);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
    if (result) return result;

    /* config txrxcs_mode[0:1] (may not be necessary for SFP - uses CSU_mode bits for post divider)
       addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
       reg_msk = (0x0003 << 28);
       result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txrxcs_mode);
       if (result) return result;  */

    /* config csu_mode[21:19] */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    reg_msk = (0x0007 << 19);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, csu_mode);
    if (result) return result;

    /* config csu_mode[18:17] */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    reg_msk = (0x0003 << 17);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x02);
    if (result) return result;

    /* config csu_mode[16] */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_7_CSU_MODE_OFF + 16;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, vco_select);
    if (result) return result;

    /* config csu_mode[15:10] - set to default will be calibrated */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    reg_msk = (0x003F << 10);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x2D);
    if (result) return result;

    result = sysHyPhy20gMsDelay (1);
    if (result) return result;

    /* enable ADSM again (may not be necessary as we reset again in cal loop)*/
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    result = sysHyPhy20gMsDelay (1);
    if (result) return result;
    DBG_PRINT("%s, %s, %d, finished SfpRateCfg lane config sequence \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of Lane config */

    /*************************/
    /* CSU calibration       */
    /*************************/
    /* set clock count max */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_12, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_12_ADSM_CLK_CNT_MAX_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_clk_cnt_max);
    if (result) return result;

     /* set lock counter */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_LOCK_CNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 16);
    if (result) return result;

    /* call CSU calibration function */
    result = utilHyPhy20gSfpSerdesCsuCal(fileHndl, linkID, &converge_freq);
    if (result) return result;

    /* set CSU AnalogRSTB */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_CSU_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* reset ADSM */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* update CSU vco center freq with new convergence */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    reg_msk = (0x003F << 10);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, converge_freq);
    if (result) return result;

    result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_10USEC);
    if (result) return result;

    /* enable ADSM */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    DBG_PRINT("%s, %s, %d, finished SfpRateCfg csu cal routine \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of CSU calibration routine */

    /***************************/
    /* reset enable sequencing */
    /***************************/
    result = sysHyPhy20gMsDelay (1);
    if (result) return result;

    /* check ADSM_LOCK */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_11, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_LOCK_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, addr, reg_msk, &val);
    if (result) return result;
    if (val != 0x1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADSM_LOCK\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_SERDES_ADSM_LOCK;
    }

    /* enable rxpatgen blocks */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* minimum 1us delay */
    result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_5USEC);
    if (result) return result;

    /* enable rxpatgen analog blocks */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* 1 ms delay */
   	result = sysHyPhy20gMsDelay (1);
    if (result) return result;

    /* enable rxpatgen vco */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_CTRL_OFF + 3;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    result = sysHyPhy20gMsDelay (1);
    if (result) return result;
    	
    /* Disable rxpatgen analog blocks */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;
    
    result = sysHyPhy20gMsDelay (1);
    if (result) return result;	
    	
    /* enable rxpatgen analog blocks */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;
	
	result = sysHyPhy20gMsDelay (1);
    if (result) return result;	
    	

    /* enable ADC */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* enable ADC DACs */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_DAC_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    result = sysHyPhy20gMsDelay (1);
    if (result) return result;

    /* enable digital reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_DIGITAL_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* enable PISO */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    DBG_PRINT("%s, %s, %d, finished SfpRateCfg reset enable sequence \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of reset enable sequence */

    /* end of CSU config */

    /* check rxcpgen lock status */
    result = utilHyPhy20gSerdesRxcpgen(fileHndl, linkID,
                                       HYPHY20G_SERDES_GRP_SFP);
    if (result) return result;

    /**************/
    /* ACB CONFIG */
    /**************/
    /* ACB config to nominal using ACB util functions   */
    result = hyPhy20gSfpAcbInit(fileHndl, linkID, 1);
    if (result) return result;

    /* config PLL_S_MAX */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_TOP_REG_ACB_FRC_WAVE_AND_CFG0_TO_CFG3, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_FRC_WAVE_AND_CFG0_TO_CFG3_PLL_S_MAX_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, acb_pll_s_max);
    if (result) return result;
    /* end ACB config */

    /* toggle ADC reference to start */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_16, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_REFERENCE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_16, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_REFERENCE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* run ADC calibration (offset cal and mode sel) */
    result = utilHyPhy20gSerdesAdcCalibrate(fileHndl, HYPHY20G_SERDES_SFP_INTF,
                                            linkID, adcCal);
    if (result) return result;

    /* re-center RX FIFO */
    result = utilHyPhy20gSerdesRxFifoReCenter(fileHndl, HYPHY20G_SERDES_SFP_INTF,
                                              linkID);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSfpRateCfg */


/*******************************************************************************
**
**  hyPhy20gSfpSerdesInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables an SFP Serdes link by taking it out
**                  of reset or disables it by putting it in reset.
**
**                  After calling hyPhy20gSfpSerdesInit with enable = 1 for a
**                  linkID, hyPhy20gSfpRateCfg should be called to configure
**                  rate specific settings.
**
**                  The link can also optionally be put into a low power state.
**                  After putting the link in low power state, to re-enable the
**                  link, the SerdesInit API with enable = 1 should be called
**                  with the subsequent RateCfg API called to configure the
**                  rate specific settings
**
**  INPUTS:         *fileHndl - base address
**                  linkID    - 0 - SFP interface 0
**                              1 - SFP interface 1
**                              ..
**                              15 - SFP interface 15
**                  enable    - 0 - disable SFP subsystem
**                              1 - enable SFP subsystem
**                              2 - SFP subsystem in low power
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 hyPhy20gSfpSerdesInit(struct file *fileHndl, UINT4 linkID, UINT4 enable)
{
    /* variable declaration */
    UINT4 addr, rstBitOff;
    INT4 result;

    /* argument checking */
    if (linkID > 15 || enable > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    rstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_LINE_SFP_RST_0_OFF +
                linkID;
    addr = HYPHY20G_TOP_REG_SYS_SW_RST_4;

    /* unconditionally apply device level reset bit */
    result = sysHyPhy20gBitWrite(fileHndl, addr, rstBitOff, 1);
    if (result) return result;
    if (enable == 0 || enable == 2) {
        /* free memory used by SERDES subsystem */
        result = serdesHyPhy20gSerdesDestroyCtxt(fileHndl,
                                                 HYPHY20G_SERDES_SFP_INTF,
                                                 linkID);
        if (result) return result;

        if (enable == 2) {
            /* call util function to put interface in low power mode */
            result = utilHyPhy20gSerdesInitLwPwr(fileHndl, HYPHY20G_SERDES_SFP_INTF,
                                                 linkID);
            if (result) return result;        
        }
        
        return HYPHY20G_SUCCESS;
    }

    /* if enabling clear device level reset bit */
    result = sysHyPhy20gBitWrite(fileHndl, addr, rstBitOff, 0);
    if (result) return result;

    if (enable == 1) {
        /* allocate and initialize memory used by SERDES subsystem */
        result = serdesHyPhy20gSerdesCreateCtxt(fileHndl, HYPHY20G_SERDES_SFP_INTF,
                                                linkID);
        if (result) return result;
    }
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSfpSerdesInit */


/*******************************************************************************
**
**  hyPhy20gSfpSfi2SerdesModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects between the SFP Serdes or SFI4-2
**                  modes for the shared interface.
**
**
**  INPUTS:         *fileHndl - base address
**                  mode      - 0 - SFP Serdes
**                              1 - SFI4-2
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSfpSfi2SerdesModeCfg(struct file *fileHndl, UINT4 mode)
{
    UINT4 tsSdataEn = 0, txTestSel = 0, rxlosMode = 0;
    UINT4 addr, bitoffset, x;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    switch(mode){
        case 0: /* SFP Serdes */
            tsSdataEn = 1;
            txTestSel = 0;
            rxlosMode = 0;
            break;
        case 1: /* SFI4-2 */
            tsSdataEn = 0;
            txTestSel = 1;
            rxlosMode = 1;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    /* take SFP ports out of device level reset and
       placed shared SFP TX pins in bypass and RX pins using rxlos_mode[0] */
    for (x=0; x < 17; x++) {
        if((mode == 1)&&(x <16)){
            /* Take logic associated with SFI data pin out of reset */
            result = hyPhy20gSfpSerdesInit(fileHndl, x, 1);
            if (result) return result;
        }
        if (x == 16){
            /* SERDES_LINE_SFP_RST bit 16 corresponding to the SFI TX CLK */
            bitoffset = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_LINE_SFP_RST_0_OFF + x;
            addr = HYPHY20G_TOP_REG_SYS_SW_RST_4;

            /* unconditionally apply device level reset bit */
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;
            if (mode == 1){
                /* remove device level reset bit */
                result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
                if (result) return result;
            }
        }
        if (!((mode == 0)&&(x==16))){
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, x);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_TX_SDATA_EN_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, tsSdataEn);
            if (result) return result;

            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_TX_TEST_SEL_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, txTestSel);
            if (result) return result;

            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, x);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_RXLOS_MODE_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, rxlosMode);
            if (result) return result;
        }
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSfpSfi2SerdesModeCfg */

/*******************************************************************************
**
**  hyPhy20gXfiRefClkInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the XFI interface by taking
**                  it out of reset or disables it by putting it in reset.
**                  After hyPhy20gXfiRefClkInit is called,  
**                  hyPhy20gXfiSfi1TxClkCfg or hyPhy20gXfiSfi2TxClkCfg must  
**                  be used to configure the tx clocking mode.
**                  Then hyPhy20gXfiRateCfg for link 0/1 is used to initialize
**                  the CSU shared between SFI4 #1 and XFI #0.
**                   
**                   
**
**  INPUTS:         *fileHndl - base address
**                  linkID    - 0 - XFI interface 0
**                              1 - XFI interface 1

**                  enable    - 0 - disable XFI subsystem
**                              1 - enable XFI subsystem
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gXfiRefClkInit(struct file *fileHndl, UINT4 linkID, UINT4 enable)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;

    if(linkID>1){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;

    /* unconditionally apply device level pwrdn bit */
    /* ref */
    addr = HYPHY20G_PGMRCLK_REG_SFI_IFS_PWR_DOWN_CTL;
    bitoffset = HYPHY20G_PGMRCLK_BIT_SFI_IFS_PWR_DOWN_CTL_SFI_XFI_X_REF_PWRDN_OFF(linkID);
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    if (enable == 0) {
        return HYPHY20G_SUCCESS;
    }

    /* if enabling clear device level pwrdn bit */
    /* ref */
    addr = HYPHY20G_PGMRCLK_REG_SFI_IFS_PWR_DOWN_CTL;
    bitoffset = HYPHY20G_PGMRCLK_BIT_SFI_IFS_PWR_DOWN_CTL_SFI_XFI_X_REF_PWRDN_OFF(linkID);
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gXfiSfiInit */


/*******************************************************************************
**
**  hyPhy20gSfi1Init
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the SFI4-1 interface by taking
**                  it out of reset or disables it by putting it in reset.
**                  After hyPhy20gSfi1Init is called, hyPhy20gXfiSfi1TxClkCfg
**                  must be used to configure the tx clocking mode.
**                  Then hyPhy20gXfiRateCfg for link 0 is used to initialize
**                  the CSU shared between SFI4 #1 and XFI #0
**
**
**  INPUTS:         *fileHndl - base address
**                  enable    - 0 - disable SFI4-1 subsystem
**                              1 - enable SFI4-1 subsystem
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSfi1Init(struct file *fileHndl, UINT4 enable)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
	
    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;

    /* unconditionally apply device level pwrdn bit */
    /* ref */
    addr = HYPHY20G_PGMRCLK_REG_SFI_IFS_PWR_DOWN_CTL;
    bitoffset = HYPHY20G_PGMRCLK_BIT_SFI_IFS_PWR_DOWN_CTL_SFI_XFI_REF_PWRDN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* clk */
    bitoffset = HYPHY20G_PGMRCLK_BIT_SFI_IFS_PWR_DOWN_CTL_SFI_CLK_DATA_PWRDN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    if (enable == 0) {
        return HYPHY20G_SUCCESS;
    }

    /* if enabling clear device level pwrdn bit */
    /* ref */
    addr = HYPHY20G_PGMRCLK_REG_SFI_IFS_PWR_DOWN_CTL;
    bitoffset = HYPHY20G_PGMRCLK_BIT_SFI_IFS_PWR_DOWN_CTL_SFI_XFI_REF_PWRDN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* clk */
    bitoffset = HYPHY20G_PGMRCLK_BIT_SFI_IFS_PWR_DOWN_CTL_SFI_CLK_DATA_PWRDN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSfi1Init */

/*******************************************************************************
**
**  hyPhy20gXfiSfi1TxClkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the reference clock usage for the
**                  SFI1_TXCLK_P/N pins.  In master mode the pins are outputs
**                  and the div parameter must be set to divide down the
**                  output of the internal CSU reference.  The internal CSU
**                  reference has already been internally divided by 16 prior
**                  to the further division with the div argument.
**
**                  In slave mode the pins are inputs and the div parameter is
**                  not processed.
**
**                  In master mode the SFI1_TXCLK_P/N pins may be used by
**                  an external XFI or SFI4.1 module.
**
**                  IMPORTANT NOTE ONLY 155.52 MHz REFCLK_B supported at this
**                  time.  It will be updated in a future release.
**
**
**  INPUTS:         *fileHndl - base address
**                  mode      - 0 - clock slave with source div/16
**                                - divided freq <= 155.52 (SFI4.1 #1)
**                              1 - clock master
**                                - using REFCLK_A 155.52 MHz(SFI4.1 or XFI)
**                              2 - clock master
**                                - using REFCLK_B 155.52 MHz(SFI4.1 or XFI)
**                              3 - clock master
**                                - using REFCLK_B 156.25 MHz(SFI4.1 or XFI)
**                              4 - clock slave with source div/16
**                                - divided freq > 155.52 (SFI4.1 #1)
**                              5 - clock slave with source div/64
**                                - divided freq <= 155.52 (SFI4.1 #1)
**                              6 - clock slave with source div/64
**                                - divided freq > 155.52 (SFI4.1 #1)
**                  div       - Post Divider setting for XFI CSU
**                              Typically set to reduce the output frequency for
**                              the requirements of external SFI module.
**                              For XFI mode set to 0.
**                              0 - div by 1 (bypass) (XFI or SFI with /16
**                                                     clock requirement)
**                              1 - div by 2
**                              2 - div by 3
**                              3 - div by 4 (SFI with /64 clock requirment)
**                              4 - div by 5
**                              5 - div by 6
**                              6 - div by 7
**                              7 - div by 8
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gXfiSfi1TxClkCfg(struct file *fileHndl, UINT4 mode, UINT4 div)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 bitmsk;
    UINT4 linkID;
    UINT4 csuClkSrc;

    /* argument checking */
    if (div > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (mode > 6) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;
    bitmsk = 0;
    csuClkSrc = 0;
    /* for SFI1 */
    linkID = 0;

    if (mode == 3) return HYPHY20G_ERR_NO_SW_CONTENT;

    /* default bit config*/
    /* REFCLK to come from SFI1_TXCLK_SRC_P/N */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CFG_2, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_CFG_2_REFCLK_SEL_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* configure SFI1_TXCLK_P/N clock source */
    // SFI_TXCLK_SRC_SEL
    // Selects between device input SFI_TXCLK_SRC and divided clock of XFI CSU as a driver for device outpur pin SFI_TXCLK
    // When set to logic:
    // 1: SFI_TXCLK_SRC is selected.
    // 0: divided clock from the XFI CSU is selected
    // divclk16 (divided by N) is selected from the input
    // The device input can either be from SFI-4 interface 1 or 2 depending on the interface selected
    /* configure the SFI1_TXCLK_P/N clock to the divided clock from the XFI CSU */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CFG_2, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_CFG_2_SFI_TXCLK_SRC_SEL_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* master mode - by default SFI1_TXCLK_P/N uses CSU, just setup divider */
    if ((mode == 1)||(mode == 2)) {
        if (mode == 1) csuClkSrc = 0;
        if (mode == 2) csuClkSrc = 1;

        /* change CSU clock source to the REFCLK coming from REFCLK_A or REFCLK_B*/
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_1, linkID);
        bitmsk = 0x3 << 16;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, csuClkSrc);
        if (result) return result;

        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CFG_1, linkID);
        bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_CFG_1_DIV16CLK_INT_DIV_BY_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, div);
        if (result) return result;

        /* enable CSU */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        return HYPHY20G_SUCCESS;
    }

    // TXRXCS_MODE
    // TXRXCS_MODE[1:0]:CSU reference clock select.
    // 0b00 = device input reference clock REFCLK_A is selected
    // 0b01 = device input reference clock REFCLK_B is selected
    // 0b10 = device input SFI_TXCLK_SRC clock is selected. When TXRXCS_MODE[1:0]=0b10 then the REFCLK_SEL register bit must be set to '1'.
    //
    // REFCLK_A is connected to REFCLK1 of the CSU, REFCLK_B is connected to REFCLK2.
    // For REFCLK_C input of the CSU we can select between SFI_TXCLK_SRC or REFCLK_A based on REFCLK_SEL register bt.


    /* slave mode - note clk div is a don't care in slave mode */
    /* change CSU clock source to the REFCLK coming from SFI1_TXCLK_SRC_P/N */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_1, linkID);
    bitmsk = 0x3 << 16;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, 0x02);
    if (result) return result;



    // REFCLK_INT_DIV_BY
    // Controls the reference clock integer divider ratio
    // When set to logic:
    // 000: Integer divided by 1 (bypass)
    // 001: Integer divide by 2
    // 010: Integer divide by 3
    // 011: Integer divide by 4
    // 100: Integer divide by 5
    // 101: Integer divide by 6
    // 110: Integer divide by 7
    // 111: Integer divide by 8
    if(mode == 0) div = 3;  // Integer divide by 4
    if(mode == 4) div = 7;  // Integer divide by 8
    if(mode == 5) div = 3;  // Integer divide by 4
    if(mode == 6) div = 7;  // Integer divide by 8

    /* divide down the txclk_src reference freq to a value that is <= 155.52 MHz */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CFG_1, linkID);
    bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_CFG_1_REFCLK_INT_DIV_BY_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, div);
    if (result) return result;

    /* enable CSU */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_ENB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gXfiSfi1TxClkCfg */

/*******************************************************************************
**
**  hyPhy20gSfi2Init
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the SFI4-2 interface by taking
**                  it out of reset or disables it by putting it in reset.
**                  After hyPhy20gSfi2Init is called, hyPhy20gXfiSfi2TxClkCfg
**                  must be used to configure the tx clocking mode.
**                  Then hyPhy20gXfiRateCfg for link 1 is used to initialize
**                  the CSU shared between SFI4 #2 and XFI #1
**
**
**  INPUTS:         *fileHndl - base address
**                  enable    - 0 - disable SFI4-2 subsystem
**                              1 - enable SFI4-2 subsystem
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSfi2Init(struct file *fileHndl, UINT4 enable)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
	
    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;

    /* unconditionally apply device level pwrdn bit */
    /* ref */
    addr = HYPHY20G_PGMRCLK_REG_SFI_IFS_PWR_DOWN_CTL;
    bitoffset = HYPHY20G_PGMRCLK_BIT_SFI_IFS_PWR_DOWN_CTL_SFI2_XFI_REF_PWRDN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* clk */
    bitoffset = HYPHY20G_PGMRCLK_BIT_SFI_IFS_PWR_DOWN_CTL_SFI2_CLK_PWRDN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    if (enable == 0) {
        return HYPHY20G_SUCCESS;
    }

    /* if enabling clear device level pwrdn bit */
    /* ref */
    addr = HYPHY20G_PGMRCLK_REG_SFI_IFS_PWR_DOWN_CTL;
    bitoffset = HYPHY20G_PGMRCLK_BIT_SFI_IFS_PWR_DOWN_CTL_SFI2_XFI_REF_PWRDN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* clk */
    bitoffset = HYPHY20G_PGMRCLK_BIT_SFI_IFS_PWR_DOWN_CTL_SFI2_CLK_PWRDN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSfi2Init */

/*******************************************************************************
**
**  hyPhy20gXfiSfi2TxClkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the reference clock usage for the
**                  SFI2_TXCLK_P/N pins.  In master mode the pins are outputs
**                  and the div parameter must be set to divide down the
**                  output of the internal CSU reference.  The internal CSU
**                  reference has already been internally divided by 16 prior
**                  to the further division with the div argument.
**
**                  In slave mode the pins are inputs and the div parameter is
**                  not processed.
**
**                  In master mode the SFI2_TXCLK_P/N pins may be used by
**                  an external XFI or SFI4.1 module.
**
**                  IMPORTANT NOTE ONLY 155.52 MHz REFCLK_B supported at this
**                  time.  It will be updated in a future release.
**
**
**  INPUTS:         *fileHndl - base address
**                  mode      - 0 - clock slave with source div/16
**                                - divided freq <= 155.52 (SFI4.1 #2)
**                              1 - clock master
**                                - using REFCLK_A 155.52 MHz(SFI4.1 or XFI)
**                              2 - clock master
**                                - using REFCLK_B 155.52 MHz(SFI4.1 or XFI)
**                              3 - clock master
**                                - using REFCLK_B 156.25 MHz(SFI4.1 or XFI)
**                              4 - clock slave with source div/16
**                                - divided freq > 155.52 (SFI4.1 #2)
**                              5 - clock slave with source div/64
**                                - divided freq <= 155.52 (SFI4.1 #2)
**                              6 - clock slave with source div/64
**                                - divided freq > 155.52 (SFI4.1 #2)
**                  div       - Post Divider setting for XFI CSU
**                              Typically set to reduce the output frequency for
**                              the requirements of external SFI module.
**                              For XFI mode set to 0.
**                              0 - div by 1 (bypass) (XFI or SFI with /16 clock
**                                                     requirement)
**                              1 - div by 2
**                              2 - div by 3
**                              3 - div by 4 (SFI with /64 clock requirment)
**                              4 - div by 5
**                              5 - div by 6
**                              6 - div by 7
**                              7 - div by 8
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NO_SW_CONTENT
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gXfiSfi2TxClkCfg(struct file *fileHndl, UINT4 mode, UINT4 div)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 bitmsk;
    UINT4 linkID;
    UINT4 csuClkSrc;

    /* argument checking */
    if (div > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (mode > 6) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;
    bitmsk = 0;
    csuClkSrc = 0;
    /* for SFI2 */
    linkID = 1;

    if (mode == 3) return HYPHY20G_ERR_NO_SW_CONTENT;

    /* default bit config*/
    /* REFCLK to come from SFI1_TXCLK_SRC_P/N */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CFG_2, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_CFG_2_REFCLK_SEL_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* configure SFI2_TXCLK_P/N clock source */
    // SFI_TXCLK_SRC_SEL
    // Selects between device input SFI_TXCLK_SRC and divided clock of XFI CSU as a driver for device outpur pin SFI_TXCLK
    // When set to logic:
    // 1: SFI_TXCLK_SRC is selected.
    // 0: divided clock from the XFI CSU is selected
    // divclk16 (divided by N) is selected from the input
    // The device input can either be from SFI-4 interface 1 or 2 depending on the interface selected
    /* configure the SFI1_TXCLK_P/N clock to the divided clock from the XFI CSU */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CFG_2, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_TOP_BIT_CFG_2_SFI_TXCLK_SRC_SEL_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* master mode - by default SFI1_TXCLK_P/N uses CSU, just setup divider */
    if ((mode == 1)||(mode == 2)) {
        if (mode == 1) csuClkSrc = 0;
        if (mode == 2) csuClkSrc = 1;

        /* change CSU clock source to the REFCLK coming from REFCLK_A or REFCLK_B*/
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_1, linkID);
        bitmsk = 0x3 << 16;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, csuClkSrc);
        if (result) return result;

        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CFG_1, linkID);
        bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_CFG_1_DIV16CLK_INT_DIV_BY_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, div);
        if (result) return result;

        /* enable CSU */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        return HYPHY20G_SUCCESS;
    }

    // TXRXCS_MODE
    // TXRXCS_MODE[1:0]:CSU reference clock select.
    // 0b00 = device input reference clock REFCLK_A is selected
    // 0b01 = device input reference clock REFCLK_B is selected
    // 0b10 = device input SFI_TXCLK_SRC clock is selected. When TXRXCS_MODE[1:0]=0b10 then the REFCLK_SEL register bit must be set to '1'.
    //
    // REFCLK_A is connected to REFCLK1 of the CSU, REFCLK_B is connected to REFCLK2.
    // For REFCLK_C input of the CSU we can select between SFI_TXCLK_SRC or REFCLK_A based on REFCLK_SEL register bt.


    /* slave mode - note clk div is a don't care in slave mode */
    /* change CSU clock source to the REFCLK coming from SFI1_TXCLK_SRC_P/N */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_1, linkID);
    bitmsk = 0x3 << 16;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, 0x02);
    if (result) return result;



    // REFCLK_INT_DIV_BY
    // Controls the reference clock integer divider ratio
    // When set to logic:
    // 000: Integer divided by 1 (bypass)
    // 001: Integer divide by 2
    // 010: Integer divide by 3
    // 011: Integer divide by 4
    // 100: Integer divide by 5
    // 101: Integer divide by 6
    // 110: Integer divide by 7
    // 111: Integer divide by 8
    if(mode == 0) div = 3;  // Integer divide by 4
    if(mode == 4) div = 7;  // Integer divide by 8
    if(mode == 5) div = 3;  // Integer divide by 4
    if(mode == 6) div = 7;  // Integer divide by 8

    /* divide down the txclk_src reference freq to a value that is <= 155.52 MHz */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CFG_1, linkID);
    bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_CFG_1_REFCLK_INT_DIV_BY_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, div);
    if (result) return result;

    /* enable CSU */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_ENB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gXfiSfi2TxClkCfg */

/*******************************************************************************
**
**  hyPhy20gSgmiiRateCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the SGMII Serdes subsystem rate.
**                  When enable is 0, the low level serdes csu, rx and tx
**                  circuits are put in reset.
**
**
**  INPUTS:         *fileHndl - base address
**                  rate      - 0 - FE 100M
**                              1 - GE 1000M
**                  enable    - 0 - disable SGMII subsystem
**                              1 - enable SGMII subsystem
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_CSU_LOCK
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gSgmiiRateCfg(struct file *fileHndl, UINT4 rate, UINT4 enable)
{
    /* local variables */
    INT4 result;
    UINT4 linkID;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 reg_msk;
    UINT4 val;
    UINT4 converge_freq;
    UINT4 rxpath_bus_width;
    UINT4 txpath_bus_width;
    UINT4 adsm_clk_cnt_max;
    UINT4 adsm_frac_div;
    UINT4 adsm_int_div;
    UINT4 adsm_denomin;
    UINT4 rx_path_div;
    UINT4 piso_div_enable;
    UINT4 piso_ctrl;
    UINT4 csu_mode;
    UINT4 vco_select;
    UINT4 acb_pll_s_max;

    /* argument checking */
    if (rate > HYPHY20G_SGMII_SERDES_RATES_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    linkID = 17;
    addr = 0;
    bitoffset = 0;
    reg_msk = 0;
    converge_freq = 0;
    DBG_PRINT("%s, %s, %d, entering SgmiiRateCfg \n",__FILE__, __FUNCTION__, __LINE__);

    /* initialize rate specific settings */
    switch(rate){
        case HYPHY20G_SGMII_SERDES_FE_125:
            rxpath_bus_width = 0x01;
            txpath_bus_width = 0x01;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 2520033;
            adsm_int_div = 32;
            adsm_denomin = 16777206;
            rx_path_div = 0x05;
            piso_div_enable = 0x01;
            piso_ctrl = 0x04;
            csu_mode = 0x01;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_SGMII_SERDES_GE_1250:
            rxpath_bus_width = 0x01;
            txpath_bus_width = 0x01;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 2520033;
            adsm_int_div = 32;
            adsm_denomin = 16777206;
            rx_path_div = 0x02;
            piso_div_enable = 0x01;
            piso_ctrl = 0x01;
            csu_mode = 0x00;
            vco_select = 0x01;
            acb_pll_s_max = 0xc;
        break;
        default:
            /* redundant to initial argument checking */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /********************/
    /* reset sequencing */
    /********************/
    /* hold CSU AnalogRSTB */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_CSU_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* set CSU CTRL[11] for feedback divider */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_CSU_CTRL_OFF + 11;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* hold rxpatgen analog blocks in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold rxpatgen vco - RXCPGEN_CTRL[3] in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_CTRL_OFF + 3;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold ADC DACs in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_DAC_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold rxpatgen blocks in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold block in digital reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_DIGITAL_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold ADC in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold PISO in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold ADSM in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* enable CSU (default) */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_CSU_ENB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* turn off CSU AnalogTM (default) */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_CSU_ATMSB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    result = sysHyPhy20gMsDelay (1);
    if (result) return result;
    DBG_PRINT("%s, %s, %d, finished SgmiiRateCfg reset sequence \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of reset sequence */

    if (enable == 0) {
        DBG_PRINT("%s, %s, %d, enable is 0, SgmiiRateCfg link held in reset\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_SUCCESS;
    }

    /********************/
    /* default settings */
    /********************/
    /* enable majority filter in cdru */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_1_FILT_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* program equalizer loop filter coeff */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_3, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_3_COEFF_D_TEP_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x0C);
    if (result) return result;

    /* enable isolated pulse detector in cdru */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_1_IPD_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* enable soft TED in cdru */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_1_TED_SEL_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* disable ADSM seed */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_11, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_SEED_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* set ADSM_CLKO_POLARITY bit */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_12, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_12_ADSM_CLKO_POLARITY_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);

    /* set CSU_CTRL[8:6] bit */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    reg_msk = (0x0007 << 22);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x03);
    if (result) return result;

    /* set CSU_CTRL[14:13] bit */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    reg_msk = (0x0003 << 29);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
    if (result) return result;

    /* end of default settings */

    /*************************/
    /* lane/rate configs     */
    /*************************/
    /* config rx_path_div */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_RX_PATH_DIV_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rx_path_div);
    if (result) return result;

    /* program tx and rx bus widths */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_TXPATH_BUS_WIDTH_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txpath_bus_width);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_RXPATH_BUS_WIDTH_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rxpath_bus_width);
    if (result) return result;

    /* config piso - bypass divider, set divide ratio, reset then enable divider */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_DIV_ENABLE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, piso_div_enable);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_CTRL_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, piso_ctrl);
    if (result) return result;

    /* program ADSM int, frac and denomin settings */
    /* see rate specific init lookup table for all the different rates */
    /* note that ADSM_INT_DIV is split into lower and upper fields */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_10, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_INT_DIV_OFF;
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_INT_DIV_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_int_div);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_10, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_FRAC_DIV_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_frac_div);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_9, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_9_ADSM_DENOMIN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_denomin);
    if (result) return result;

    /* config csu_ctrl[3:2] */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    reg_msk = (0x0003 << 18);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
    if (result) return result;

    /* config txrxcs_mode[0:1] (may not be necessary for SFP - uses CSU_mode bits for post divider)
       addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
       reg_msk = (0x0003 << 28);
       result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txrxcs_mode);
       if (result) return result;  */

    /* config csu_mode[21:19] */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    reg_msk = (0x0007 << 19);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, csu_mode);
    if (result) return result;

    /* config csu_mode[18:17] */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    reg_msk = (0x0003 << 17);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x02);
    if (result) return result;

    /* config csu_mode[16] */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_7_CSU_MODE_OFF + 16;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, vco_select);
    if (result) return result;

    result = sysHyPhy20gMsDelay (1);
    if (result) return result;

    /* enable ADSM again (may not be necessary as we reset again in cal loop)*/
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    result = sysHyPhy20gMsDelay (1);
    if (result) return result;
    DBG_PRINT("%s, %s, %d, finished SgmiiRateCfg lane config sequence \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of Lane config */

    /*************************/
    /* CSU calibration       */
    /*************************/
    /* set clock count max */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_12, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_12_ADSM_CLK_CNT_MAX_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_clk_cnt_max);
    if (result) return result;

     /* set lock counter */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_LOCK_CNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 16);
    if (result) return result;

    /* call CSU calibration function */
    result = utilHyPhy20gSfpSerdesCsuCal(fileHndl, linkID, &converge_freq);
    if (result) return result;

    /* set CSU AnalogRSTB */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_CSU_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* hold CSU CTRL[11] for feedback divider */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_CSU_CTRL_OFF + 11;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* reset ADSM */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* update CSU vco center freq with new convergence */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    reg_msk = (0x003F << 10);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, converge_freq);
    if (result) return result;

    result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_10USEC);
    if (result) return result;

    /* enable ADSM */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    DBG_PRINT("%s, %s, %d, finished SGMIIRateCfg csu cal routine \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of CSU calibration routine */

    /***************************/
    /* reset enable sequencing */
    /***************************/
    result = sysHyPhy20gMsDelay (1);
    if (result) return result;

    /* check ADSM_LOCK */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_11, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_LOCK_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, addr, reg_msk, &val);
    if (result) return result;
    if (val != 0x1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_CSU_LOCK\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_CSU_LOCK;
    }

    /* enable rxpatgen vco */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_CTRL_OFF + 3;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* enable rxpatgen blocks */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* minimum 1us delay */
    result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_5USEC);
    if (result) return result;

    /* enable rxpatgen analog blocks */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* minimum 2us delay */
    result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_5USEC);
    if (result) return result;

    /* enable ADC */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* enable ADC DACs */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_DAC_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    result = sysHyPhy20gMsDelay (1);
    if (result) return result;

    /* enable digital reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_DIGITAL_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* enable PISO */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    DBG_PRINT("%s, %s, %d, finished SgmiiRateCfg reset enable sequence \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of reset enable sequence */

    /* end of CSU config */

    /* check rxcpgen lock status */
    result = utilHyPhy20gSerdesRxcpgen(fileHndl, linkID,
                                       HYPHY20G_SERDES_GRP_SGMII);
    if (result) return result;

    /**************/
    /* ACB CONFIG */
    /**************/
    /* ACB config to nominal using ACB util functions   */
    result = hyPhy20gSfpAcbInit(fileHndl, linkID, 1);
    if (result) return result;

    /* config PLL_S_MAX */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_TOP_REG_ACB_FRC_WAVE_AND_CFG0_TO_CFG3, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_FRC_WAVE_AND_CFG0_TO_CFG3_PLL_S_MAX_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, acb_pll_s_max);
    if (result) return result;
    /* end ACB config */

    /* toggle ADC reference to start */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_16, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_REFERENCE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_16, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_REFERENCE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* re-center RX FIFO */
    result = utilHyPhy20gSerdesRxFifoReCenter(fileHndl, HYPHY20G_SERDES_SFP_INTF,
                                              linkID);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSgmiiRateCfg */

/*******************************************************************************
**
**  hyPhy20gSgmiiSerdesInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables an SGMII Serdes link by taking it out
**                  of reset or disables it by putting it in reset.
**
**                  After calling hyPhy20gSgmiiSerdesInit with enable = 1 for a
**                  linkID, hyPhy20gSgmiiRateCfg should be called to configure
**                  rate specific settings.
**
**
**  INPUTS:         *fileHndl - base address
**                  enable    - 0 - disable SGMII subsystem
**                              1 - enable SGMII subsystem
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSgmiiSerdesInit(struct file *fileHndl, UINT4 enable)
{
    /* variable declaration */
    UINT4 addr, rstBitOff;
    INT4 result;

    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    rstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_LINE_SFP_RST_17_OFF;
    addr = HYPHY20G_TOP_REG_SYS_SW_RST_4;

    /* unconditionally apply device level reset bit */
    result = sysHyPhy20gBitWrite(fileHndl, addr, rstBitOff, 1);
    if (result) return result;
    if (enable == 0) {
        return HYPHY20G_SUCCESS;
    }
    
    /* if enabling clear device level reset bit */
    result = sysHyPhy20gBitWrite(fileHndl, addr, rstBitOff, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSgmiiSerdesInit */

/*******************************************************************************
**
**  hyPhy20gXfiRateCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the XFI Serdes subsystem rate
**                  for a given link.
**
**                  hyPhy20gXfiSerdesInit must be called prior to this
**                  function.
**
**                  hyPhy20gXfi1TxClkCfg or hyPhy20gXfi2TxClkCfg (for Link ID 0
**                  and 1 respectivtely)must be called to specify a slave mode
**                  timing source prior to calling this API if rate is to be
**                  set for 'SFI4.1 Slave Input Clock'
**
**                  ADC calibration should only be performed on links with
**                  floating inputs or with active receive data traffic.  ADC
**                  calibration can be deferred using adcCal == 3, and when
**                  ready, hyPhy20gSerdesADCCalibrate can be called to perform
**                  calibration across multiple SERDES interfaces and links.
**                  When adcCal == 2, hyPhy20gSerdesLoadFactoryCal must be
**                  called before this function.
**
**                  Mode selection and equalization are performed together and
**                  they should only be performed with active receive data
**                  traffic.  Mode selection and equalization can be deferred
**                  using eqMode == 3, and when ready, hyPhy20gSerdesADCCalibrate
**                  can be called to perform the operation.  If firmware based
**                  equalization is used, hyPhy20gHostMsgInit must be called
**                  prior to this function.
**
**                  When enable is 0, the low level serdes csu, rx and tx
**                  circuits are put in reset.
**
**
**  INPUTS:         *fileHndl - base address
**                  linkID    - 0 - XFI interface 0
**                              1 - XFI interface 1
**                  rate      - 0 - OC-48 2.488G
**                              1 - OC-192 9.953G
**                              2 - 10GE-LAN 10.3125G
**                              3 - 10GE-WAN 9.953G
**                              4 - OTU2
**                              5 - OTU2 Gsup43 7.1 of 10GE-LAN
**                              6 - OTU2 Gsup43 7.2 of 10GE-LAN
**                              7 - OTU2 Gsup43 7.1 of FC-1200
**                              8 - OTU2 Gsup43 7.2 of FC-1200
**                              9 - ODU2
**                              10 - ODU1e
**                              11 - ODU2e
**                              12 - ODU1f
**                              13 - ODU2f
**                              14 - FC8G
**                              15 - FC10G
**                              16 - SFI4.1 Slave Input Clock
**                              17 - CPRI 9.8304G
**                  adcCal    - 0 - do not perform ADC calibration
**                              1 - perform ADC calibration
**                              2 - perform ADC calibration using offsets in
**                                  memory (call hyPhy20gSerdesLoadFactoryCal
**                                  before this function)
**                              3 - defer ADC calibration, equalization
**                                  will not be performed (intended for calling
**                                  hyPhy20gSerdesADCCalibrate at a later point)
**                  eqMode    - 0 - hardcoded mode selection and equalization
**                                  values
**                              1 - perform mode selection and FW equalization
**                              2 - apply mode selection and equalization
**                                  using settings in memory (call
**                                  hyPhy20gSerdesLoadFactoryCal before this
**                                  function)
**                              3 - defer mode selection and FW equalization
**                                  (intended for calling
**                                   hyPhy20gSerdesADCCalibrate at a later
**                                   point)
**                  enable    - 0 - disable XFI subsystem
**                              1 - enable XFI subsystem
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_SERDES_ADSM_LOCK
**                  HYPHY20G_ERR_SERDES_CSU_INACTIVE
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SERDES_ADC_SFP_RATE_CHECK
**                  HYPHY20G_ERR_SERDES_ADC_CALIBRATION_FAILED
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gXfiRateCfg(struct file *fileHndl, UINT4 linkID, UINT4 rate,
                        UINT4 adcCal, UINT4 eqMode, UINT4 enable)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset, bitmsk, val, csuDiv, slaveMode = 0;
    UINT4 reg_msk;
    UINT4 converge_freq;
    UINT4 rxpath_bus_width;
    UINT4 txpath_bus_width;
    UINT4 adsm_clk_cnt_max;
    UINT4 adsm_frac_div;
    UINT4 adsm_int_div;
    UINT4 adsm_denomin;
    UINT4 rx_path_div;
    UINT4 piso_div_enable;
    UINT4 piso_ctrl;
    UINT4 txrxcs_mode;
    UINT4 i;
    UINT4 csu_config;
    UINT4 coeff_d_tep;
    UINT4 acb_pll_s_max;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (linkID > HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (rate > HYPHY20G_XFI_SERDES_RATES_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((adcCal > 3) || (eqMode > 3)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;
    reg_msk = 0;
    converge_freq = 0;
    DBG_PRINT("%s, %s, %d, entering XfiRateCfg \n",__FILE__, __FUNCTION__, __LINE__);

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkID];
    HYPHY20G_ASSERT((pSerdesCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Start SFI4.1 slave mode CSU initialization */
    if(rate == HYPHY20G_XFI_SERDES_SFI4_1_SLAVE) {
        /* treat rate as OC192 */
        slaveMode = 1;
        rate = HYPHY20G_XFI_SERDES_OC192_9953_28;

        /* confirm CSU source is from slave input */
        /* confirm TXRXCS_MODE[1:0] is set to 0x02 */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_1, linkID);
        bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_1_TXRXCS_MODE_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, addr, bitmsk, &val);
        if (result) return result;

        if((val & 0x3) != 0x2) return HYPHY20G_ERR_INVALID_PRECONFIG;

        /* confirm SFI_TXCLK_SRC is set to 0 */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CFG_2, linkID);
        bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_CFG_2_SFI_TXCLK_SRC_SEL_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, addr, bitmsk, &val);
        if (result) return result;

        if(val != 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

        /* recover REFCLK_INT_DIV to determine if ADSM_INT_DIV setting needs to be modified */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CFG_1, linkID);
        bitmsk = HYPHY20G_SERDES_XFI_TOP_BIT_CFG_1_REFCLK_INT_DIV_BY_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, addr, bitmsk, &csuDiv);
        if (result) return result;

    } /* end SFI4.1 slave mode CSU initialization */

    /* initialize rate specific settings */
    switch(rate){
        case HYPHY20G_XFI_SERDES_OC48_2488_32:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 0;
            adsm_int_div = 32;
            adsm_denomin = 16777215;
            rx_path_div = 0x02;
            piso_div_enable = 0x01;
            piso_ctrl = 0x01;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_OC192_9953_28:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 0;
            adsm_int_div = 32;
            adsm_denomin = 16777215;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_10GE_LAN_10312_5:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 2598508;
            adsm_int_div = 33;
            adsm_denomin = 16775424;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_10GE_WAN_9953_28:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 0;
            adsm_int_div = 32;
            adsm_denomin = 16777215;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_OTU2_10709:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 7220546;
            adsm_int_div = 34;
            adsm_denomin = 16777151;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_OTU2_GSUP43_7_1_11096:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 11024600;
            adsm_int_div = 35;
            adsm_denomin = 16381440;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_OTU2_GSUP43_7_2_11049:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 8769915;
            adsm_int_div = 35;
            adsm_denomin = 16765056;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_OTU2_FC1200_11317_6:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 6436171;
            adsm_int_div = 36;
            adsm_denomin = 16654464;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_OTU2_FC1200_11270_1:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 3917680;
            adsm_int_div = 36;
            adsm_denomin = 16773120;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_ODU2_10037_3:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 4530496;
            adsm_int_div = 32;
            adsm_denomin = 16776993;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_ODU1e_10355_8:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 4718857;
            adsm_int_div = 33;
            adsm_denomin = 16039296;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_ODU2e_10399_5:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 6942793;
            adsm_int_div = 33;
            adsm_denomin = 15971904;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_ODU1f_10562_9:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 16072738;
            adsm_int_div = 33;
            adsm_denomin = 16740864;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_ODU2f_10607_5:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 1693540;
            adsm_int_div = 34;
            adsm_denomin = 16381440;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_FC8G_8500:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 2676837;
            adsm_int_div = 34;
            adsm_denomin = 16772832;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x01;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_FC10G_10518_75:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 13722258;
            adsm_int_div = 33;
            adsm_denomin = 16775424;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        case HYPHY20G_XFI_SERDES_CPRI_9830_4:
            rxpath_bus_width = 0x03;
            txpath_bus_width = 0x03;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 10149174;
            adsm_int_div = 31;
            adsm_denomin = 16777206;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            txrxcs_mode = 0x00;
            acb_pll_s_max = 0xc;
        break;
        default:
            /* redundant to initial argument checking */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* program ADSM_INT_DIV to 64 for slave mode when reference > 155.52 MHz*/
    if((slaveMode == 1)&&((csuDiv==1)||(csuDiv==7))){
        adsm_int_div = 64;
    }
    
    
    csu_config = 1;
    for (i = 0; i < 3 && csu_config == 1; i++) {
        /********************/
        /* reset sequencing */
        /********************/
        /* hold CSU AnalogRSTB */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_ARSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
       	if (result) return result;

        /* hold CSU CTRL[11] for feedback divider*/
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_CTRL_OFF + 11;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
       	if (result) return result;

        /* hold rxpatgen analog blocks in reset */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_ARSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
       	if (result) return result;

        /* read back VCO setting to check if CSU is configured */
        /* if VCO setting is non-zero, CSU has already been configured once */
        /* if already configured once, the following reset bits should not be disabled on */
        /* subsequent line rate configurations */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_9, linkID);
        reg_msk = (0x003F << 10);
        result = hyPhy20gRegFieldRead(fileHndl, addr, reg_msk, &val);
       	if (result) return result;
        
        if (val == 0x0) { 
          /* hold rxpatgen vco = ctrl[3] in reset */
          addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
          bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_CTRL_OFF + 3;
         	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
         	if (result) return result;
  
          /* hold ADC DACs in reset */
          addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_3, linkID);
          bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_3_ADC_DAC_RSTB_OFF;
        	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
         	if (result) return result;
  
          /* hold rxpatgen blocks reset */
          addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
          bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_RSTB_OFF;
         	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
         	if (result) return result;
  
          /* hold block in digital reset */
          addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
          bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_DIGITAL_RSTB_OFF;
         	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
         	if (result) return result;
  
          /* hold ADC in reset */
          addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_3, linkID);
          bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_3_ADC_RSTB_OFF;
         	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
         	if (result) return result;
        } else { 	  
          /* hold block in digital reset */
          addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
          bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_DIGITAL_RSTB_OFF;
         	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
         	if (result) return result;
        }	  

        /* hold PISO in reset */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_RSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
       	if (result) return result;

        /* hold ADSM in reset */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
       	if (result) return result;

        /* enable CSU (default) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_ENB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
       	if (result) return result;

        /* turn off CSU AnalogTM (default) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_ATMSB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
       	if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished XfiRateCfg reset sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of reset sequence */

        if (enable == 0) {
            DBG_PRINT("%s, %s, %d, enable is 0, XfiRateCfg link held in reset\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_SUCCESS;
        }

        /********************/
        /* default settings */
        /********************/

        /* disable ADSM seed */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_13, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_SEED_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
       	if (result) return result;

        /* enable OC_ENB (default) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_OC_ENB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
       	if (result) return result;

        /* program OC_INTERVAL to 0xFFFF - required non-default setting */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_8_OC_INTERVAL_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0xFFFF);

        /* set CSU_CTRL[8:6] bit */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        reg_msk = (0x0007 << 18);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x07);
       	if (result) return result;

        /* set CSU_CTRL[14:13] bit */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        reg_msk = (0x0003 << 25);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
       	if (result) return result;

        /* set CSU_MODE[7:0] bits */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_9, linkID);
        reg_msk = (0x0007F << 0);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x07);
       	if (result) return result;
       	  
        /* set CSU_MODE[31:27] bits */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_9, linkID);
        reg_msk = (0x0001F << 27);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x08);
       	if (result) return result;
       	  
        /* end of default settings */

        /*************************/
        /* lane/rate configs     */
        /*************************/

        /* config rx_path_div */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_RX_PATH_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rx_path_div);
        if (result) return result;

        /* program tx and rx bus widths */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_TXPATH_BUS_WIDTH_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txpath_bus_width);
        if (result) return result;

        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_RXPATH_BUS_WIDTH_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rxpath_bus_width);
        if (result) return result;

        /* config piso - bypass divider, set divide ratio */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_DIV_ENABLE_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, piso_div_enable);
       	if (result) return result;

        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_CTRL_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, piso_ctrl);
        if (result) return result;

        /* program ADSM int, frac and denomin settings */
        /* see rate specific init lookup table for all the different rates */
        /* note that ADSM_INT_DIV is split into lower and upper fields */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_12, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_12_ADSM_INT_DIV_OFF;
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_12_ADSM_INT_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_int_div);
        if (result) return result;

        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_12, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_12_ADSM_FRAC_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_frac_div);
        if (result) return result;

        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_11, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_11_ADSM_DENOMIN_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_denomin);
        if (result) return result;

        /* config csu_ctrl[3:2] */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        reg_msk = (0x0003 << 14);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
        if (result) return result;

        /* config txrxcs_mode[5:4] */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_1, linkID);
        reg_msk = (0x0003 << 20);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txrxcs_mode);
        if (result) return result;

        /* config csu_mode[21:19] (may not be necessary for XFI - uses txrxcs_mode bits for post divider)
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_9, linkID);
        reg_msk = (0x0007 << 19);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, csu_mode);
        if (result) return result;      */

        /* config csu_mode[15:10] - set to default will be calibrated */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_9, linkID);
        reg_msk = (0x003F << 10);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x26);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* enable ADSM again (may not be necessary as we reset again in cal loop)*/
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
       	if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished XfiRateCfg lane config sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of Lane config */

        /*************************/
        /* CSU calibration       */
        /*************************/
        /* program ADSM clk_cnt_max settings */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_14, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_14_ADSM_CLK_CNT_MAX_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_clk_cnt_max);
        if (result) return result;

        /* set lock counter */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_LOCK_CNT_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x10);
        if (result) return result;

        /* call XFI CSU calibration function */
        result = utilHyPhy20gXfiSerdesCsuCal(fileHndl, linkID, &converge_freq);
        if (result) return result;

        /* set CSU AnalogRSTB */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_ARSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
       	if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* hold ADSM in reset */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
       	if (result) return result;

        /* update CSU vco center freq with new convergence */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_9, linkID);
        reg_msk = (0x003F << 10);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, converge_freq);
        if (result) return result;

        /* enable ADSM */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
       	if (result) return result;

        DBG_PRINT("%s, %s, %d, finished XfiRateCfg csu cal routine \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of CSU calibration routine */

        /***************************/
        /* reset enable sequencing */
        /***************************/
        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* check ADSM_LOCK */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_13, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_LOCK_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, addr, reg_msk, &val);
        if (result) return result;
        if (val != 0x1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADSM_LOCK\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_SERDES_ADSM_LOCK;
        }

        /* enable rxpatgen vco = ctrl[3] */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_CTRL_OFF + 3;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
       	if (result) return result;

        /* enable rxpatgen blocks */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_RSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
       	if (result) return result;

        result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_5USEC);
        if (result) return result;

        /* enable rxpatgen analog blocks in reset */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_ARSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
       	if (result) return result;

        result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_5USEC);
        if (result) return result;

        /* enable ADC */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_3_ADC_RSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
       	if (result) return result;

        /* enable ADC DACs */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_3_ADC_DAC_RSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
       	if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* enable digital reset */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_DIGITAL_RSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
       	if (result) return result;

        /* enable PISO */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_RSTB_OFF;
       	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
       	if (result) return result;

        /**********************/
        /* check CSU activity */
        /**********************/
   	    /* toggle CSU_ACTIVE_DTCT_RSTB */
   	    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CSU_ACTIVE_DTCT_RST, linkID);
   	    reg_msk = HYPHY20G_SERDES_XFI_TOP_BIT_CSU_ACTIVE_DTCT_RST_CSU_ACTIVE_DTCT_RSTB_MSK;
   	    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0);
   	    if (result) return result;

   	    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 1);
   	    if (result) return result;

        /* wait a minimum of 1/566MHz */
        result = sysHyPhy20gUsDelay (1);
        if (result) return result;

        /* read CSU_ACTIVE_DTCT */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CSU_ACTIVE_DTCT, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_TOP_BIT_CSU_ACTIVE_DTCT_CSU_ACTIVE_DTCT_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, addr, reg_msk, &val);
        if (result) return result;
        /* end check CSU activity */

        /* re-configure CSU if there is no clock activity */
       	csu_config = (val == 1) ? 0 : 1;
    }
    if (csu_config == 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_CSU_INACTIVE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_SERDES_CSU_INACTIVE;
    }

    DBG_PRINT("%s, %s, %d, finished XfiRateCfg reset enable sequence \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of reset enable sequence */

    /* CSU Config complete */

    /* check rxcpgen lock status */
    result = utilHyPhy20gSerdesRxcpgen(fileHndl, linkID,
                                       HYPHY20G_SERDES_GRP_XFI);
    if (result) return result;

    /**************/
    /* ACB CONFIG */
    /**************/
    /* config PLL_S_MAX */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_ACB_CTL, linkID);
    reg_msk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CTL_PLL_S_MAX_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, acb_pll_s_max);
    if (result) return result;
    /* end ACB config */

    /* toggle ADC reference to start */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_REFERENCE_OFF;
   	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
   	if (result) return result;

    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_REFERENCE_OFF;
   	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
   	if (result) return result;

    /* adc offset correction */
    result = utilHyPhy20gSerdesAdcCalibrate(fileHndl,
                                            HYPHY20G_SERDES_XFI_INTF,
                                            linkID, adcCal);
    if (result) return result;

    /* mode selection */
    result = utilHyPhy20gAdcModeSelect(fileHndl, HYPHY20G_SERDES_XFI_INTF,
                                       linkID, eqMode);
    if (result) return result;

    /* program equalizer loop filter coeff (hardcoded now, should be set in fw ) */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_3, linkID);
    reg_msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_3_COEFF_D_TEP_MSK;
    coeff_d_tep = (rate == HYPHY20G_XFI_SERDES_OTU2_FC1200_11317_6) ? 0x0E : 0x0A;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, coeff_d_tep);
    if (result) return result;

    /* program equalizer threshold clip detector (default, hardcoded now, should be set in fw) */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_4, linkID);
    reg_msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_TH_CLIP_DETECT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x12);
    if (result) return result;

    /* set LOS_ENABLE to 1 */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_4, linkID);
    reg_msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_LOS_ENABLE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
    if (result) return result;

    /* equalization */
    result = utilHyPhy20gSerdesEqCfg(fileHndl, HYPHY20G_SERDES_XFI_INTF, linkID,
                                     eqMode);
    if (result) return result;

    /* re-center RX FIFO */
    result = utilHyPhy20gSerdesRxFifoReCenter(fileHndl, HYPHY20G_SERDES_XFI_INTF,
                                              linkID);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gXfiRateCfg */

/*******************************************************************************
**
**  hyPhy20gXfiSerdesInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables the XFI Serdes subsystem link by
**                  taking it out of reset or disables it by putting it in
**                  reset.
**
**                  After calling hyPhy20gXfiSerdesInit with enable = 1 for a
**                  linkID, hyPhy20gXfiRateCfg should be called to configure
**                  rate specific settings.
**
**                  For RevB and RevC of the device, disabling the serdes by  
**                  calling this function with enable = 0 should not be used 
**                  unless both ports are being disabled.
**                  Instead, hyPhy20gXfiRateCfg should be used for all 
**                  subsequent re-configurations of the linkID.
**
**                  The link can also optionally be put into a low power state.
**                  After putting the link in low power state, to re-enable the
**                  link, the SerdesInit API with enable = 1 should be called
**                  with the subsequent RateCfg API called to configure the
**                  rate specific settings
**
**  INPUTS:         *fileHndl - base address
**                  interface - 0 - XFI interface 0
**                              1 - XFI interface 1
**                  enable    - 0 - disable XFI subsystem
**                              1 - enable XFI subsystem
**                              2 - XFI subsystem in low power
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 hyPhy20gXfiSerdesInit(struct file *fileHndl, UINT4 linkID, UINT4 enable)
{
    /* variable declaration */
    UINT4 addr, rstBitOff;
    INT4 result;

    /* argument checking */
    if (linkID > 1 || enable > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    rstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_LINE_XFI_RST_0_OFF +
                linkID;
    addr = HYPHY20G_TOP_REG_SYS_SW_RST_4;

    /* unconditionally apply device level reset bit */
    result = sysHyPhy20gBitWrite(fileHndl, addr, rstBitOff, 1);
    if (result) return result;
    if (enable == 0 || enable == 2) {
        /* free memory used by SERDES subsystem */
        result = serdesHyPhy20gSerdesDestroyCtxt(fileHndl,
                                                 HYPHY20G_SERDES_XFI_INTF,
                                                 linkID);
        if (result) return result;
        
        if (enable == 2) {
            /* call util function to put interface in low power mode */
            result = utilHyPhy20gSerdesInitLwPwr(fileHndl, HYPHY20G_SERDES_XFI_INTF,
                                                 linkID);
            if (result) return result;
        }

        return HYPHY20G_SUCCESS;
    }


    /* if enabling clear device level reset bit */
    result = sysHyPhy20gBitWrite(fileHndl, addr, rstBitOff, 0);
    if (result) return result;

    if (enable == 1) {
        /* allocate and initialize memory used by SERDES subsystem */
        result = serdesHyPhy20gSerdesCreateCtxt(fileHndl, HYPHY20G_SERDES_XFI_INTF,
                                                linkID);
        if (result) return result;
    }
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gXfiSerdesInit */


/*******************************************************************************
**
**  hyPhy20gPifRateCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the PIF Serdes subsystem rate
**                  for a given link.
**
**                  hyPhy20gPifSerdesInit must be called prior to this
**                  function.
**
**                  ADC calibration should only be performed on links with
**                  floating inputs or with active receive data traffic.
**                  Note that data integrity will be compromised on all PIF
**                  links during ADC offset calibration.  Mode selection will
**                  only be performed upon successful offset calibration.
**                  ADC calibration can be deferred using adcCal == 3, and when
**                  ready, hyPhy20gSerdesADCCalibrate can be called to perform
**                  calibration across multiple SERDES interfaces and links.
**                  When adcCal == 2, call hyPhy20gSerdesLoadFactoryCal before
**                  this function.
**
**                  Equalization is selectable between hardcoded values or
**                  fw based equalization.  If firmware based equalization is
**                  used, hyPhy20gHostMsgInit must have been called prior to
**                  calling this function.
**
**                  When enable is 0, the low level serdes csu, rx and tx
**                  circuits are put in reset.
**
**
**  INPUTS:         *fileHndl - base address
**                  linkID    - 0 - PIF Interface 0
**                              1 - PIF Interface 1
**                              ..
**                              7 - PIF Interface 7
**                  rate      - 0 - Interlaken 6.25G
**                              1 - XAUI 3.125G
**                              2 - Interlaken 3.125G
**                  adcCal    - 0 - do not perform ADC calibration
**                              1 - perform ADC calibration
**                              2 - perform ADC calibration using offsets in
**                                  memory (call hyPhy20gSerdesLoadFactoryCal
**                                  before this function)
**                              3 - defer ADC calibration, equalization
**                                  will not be performed (intended for calling
**                                  hyPhy20gSerdesADCCalibrate at a later point)
**                  eqMode    - 0 - hardcoded mode selection and equalization
**                                  values
**                              1 - perform mode selection and FW equalization
**                              2 - apply mode selection and equalization
**                                  using settings in memory (call
**                                  hyPhy20gSerdesLoadFactoryCal before this
**                                  function)
**                              3 - defer mode selection and FW equalization
**                                  (intended for calling
**                                   hyPhy20gSerdesADCCalibrate at a later
**                                   point)
**                  enable    - 0 - disable PIF subsystem
**                              1 - enable PIF subsystem
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SERDES_ADC_SFP_RATE_CHECK
**                  HYPHY20G_ERR_SERDES_ADC_CALIBRATION_FAILED
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gPifRateCfg(struct file *fileHndl, UINT4 linkID, UINT4 rate,
                        UINT4 adcCal, UINT4 eqMode, UINT4 enable)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 reg_msk;
    UINT4 converge_freq;
    UINT4 csu_enable;
    UINT4 rxpath_bus_width;
    UINT4 txpath_bus_width;
    UINT4 adsm_clk_cnt_max;
    UINT4 adsm_frac_div;
    UINT4 adsm_int_div;
    UINT4 adsm_denomin;
    UINT4 rx_path_div;
    UINT4 piso_div_enable;
    UINT4 piso_ctrl;
    UINT4 csu_mode;
    UINT4 core_clk1_sel;
    UINT4 div4_div_en;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (linkID > HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (rate > HYPHY20G_PIF_SERDES_RATES_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((adcCal > 3) || (eqMode > 3)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;
    reg_msk = 0;
    converge_freq = 0;
    DBG_PRINT("%s, %s, %d, entering PifRateCfg \n",__FILE__, __FUNCTION__, __LINE__);

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkID];
    HYPHY20G_ASSERT((pSerdesCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* initialize rate specific settings */
    switch(rate){
        case HYPHY20G_PIF_SERDES_INTERLAKEN_6250:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 48910;
            adsm_int_div = 40;
            adsm_denomin = 260496;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            csu_mode = 0x00;
            core_clk1_sel = 0;
            div4_div_en = 1;
        break;
        case HYPHY20G_PIF_SERDES_XAUI_3125:
            rxpath_bus_width = 0x01;
            txpath_bus_width = 0x01;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 0;
            adsm_int_div = 40;
            adsm_denomin = 10;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            csu_mode = 0x03;
            core_clk1_sel = 1;
            div4_div_en = 0;
        break;
        case HYPHY20G_PIF_SERDES_INTERLAKEN_3125:
            rxpath_bus_width = 0x02;
            txpath_bus_width = 0x02;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 48910;
            adsm_int_div = 40;
            adsm_denomin = 260496;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            csu_mode = 0x03;
            core_clk1_sel = 1;
            div4_div_en = 0;
        break;
        default:
            /* redundant to initial argument checking */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* check to see if csu is enabled yet, CSU is default to 1.  if 1, enable csu */
    addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_2;
    bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_ENB_OFF;
    result = sysHyPhy20gBitRead(fileHndl, addr, bitoffset, &csu_enable);
    if (result) return result;

    if (csu_enable == 1) {
        /* csu is not enabled, enable csu using this link */
        /********************/
        /* reset sequencing */
        /********************/
        /* enable csu */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_2;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* enable rxpatgen */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* hold csu analog blocks in reset */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_2;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* setup divider */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_2;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_CTRL_OFF + 11;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* hold rxpatgen analog blocks in reset */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold rxpatgen vco - RXCPGEN_CTRL[3] in reset */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold ADC DACs in reset */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_2_ADC_DAC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold rxpatgen blocks in reset */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold csu block in digital reset */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_13;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_13_DIGITAL_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold block in digital reset */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_8, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_8_DIGITAL_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold ADC in reset */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_ADC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold PISO in reset */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold adsm in reset */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_10;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished PifRateCfg reset sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of reset sequence */

        if (enable == 0) {
            DBG_PRINT("%s, %s, %d, enable is 0, PifRateCfg link held in reset\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_SUCCESS;
        }

        /********************/
        /* default settings */
        /********************/
        /* disable repeater and terminators */
        addr = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_REG_REP_TER_CTL;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_BIT_REP_TER_CTL_RST_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_REG_REP_TER_CTL;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_BIT_REP_TER_CTL_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* config terminator block for csu */
        addr = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_REG_REP_TER_CTL;
        reg_msk = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_BIT_REP_TER_CTL_TERM_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x06);
        if (result) return result;

        /* disable ADSM seed */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_5;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_SEED_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* program OC_INTERVAL to 0xFFFF - required non-default setting */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_11;
        reg_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_11_OC_INTERVAL_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0xFFFF);
        if (result) return result;

        /* set CSU_CTRL[8:6] bit */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_2;
        reg_msk = (0x0007 << 18);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x03);
        if (result) return result;

        /* set CSU_CTRL[14:13] bit */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_2;
        reg_msk = (0x0003 << 25);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
        if (result) return result;

        /* Configuration settings to generate Interlaken Core clock */
        /* 6.25G mode  CORE_CLK1_SEL = 0
                       DIV4_DIV_EN = 1
           3.125G Mode CORE_CLK1_SEL = 1
                       DIV4_DIV_EN = 0
        */
        /* set CORE_CLK1_SEL */
        addr = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_REG_CLK_SEL;
        reg_msk = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_BIT_CLK_SEL_CORE_CLK1_SEL_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, core_clk1_sel);
        if (result) return result;
        /* set DIV4_DIV_EN */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_13;
        reg_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_13_DIV4_DIV_EN_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, div4_div_en);
        if (result) return result;
        /* end Configuration settings to generate Interlaken Core clock */

        /* end of default settings */

        /*************************/
        /* lane/rate configs     */
        /*************************/
        /* if XAUI rate selected, use refclk b */
        if (rate == HYPHY20G_PIF_SERDES_XAUI_3125){
            addr = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_REG_CLK_SEL;
            bitoffset = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_BIT_CLK_SEL_REFCLK_SEL_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;
        }

        /* config rx_path_div */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_8_RX_PATH_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rx_path_div);
        if (result) return result;

        /* program tx and rx bus widths */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_8_TXPATH_BUS_WIDTH_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txpath_bus_width);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_8_RXPATH_BUS_WIDTH_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rxpath_bus_width);
        if (result) return result;

        /* config piso - bypass divider, set divide ratio */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_DIV_ENABLE_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, piso_div_enable);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
        reg_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_CTRL_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, piso_ctrl);
        if (result) return result;

        /* program ADSM int, frac and denomin settings */
        /* see rate specific init lookup table for all the different rates */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_5;
        reg_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_INT_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_int_div);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_6;
        reg_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_6_ADSM_FRAC_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_frac_div);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_9;
        reg_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_9_ADSM_DENOMIN_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_denomin);
        if (result) return result;

        /* config cs_mode[4:2] post divider */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_1;
        reg_msk = (0x0007 << 18);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, csu_mode);
        if (result) return result;

        /* config csu_ctrl[3:2] */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_2;
        reg_msk = (0x0003 << 14);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
        if (result) return result;

        /* config csu_mode[15:10] - set to default will be calibrated */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_3;
        reg_msk = (0x003F << 10);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x2D);
        if (result) return result;

        /* config csu_mode[5:0] charge pump */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_3;
        reg_msk = (0x0003F << 0);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x03);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* enable ADSM again (may not be necessary as we reset again in cal loop)*/
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_10;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished PifRateCfg lane config sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of Lane config */

        /*************************/
        /* CSU calibration       */
        /*************************/
        /* set clock count max */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_8;
        reg_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_8_ADSM_CLK_CNT_MAX_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_clk_cnt_max);
        if (result) return result;

        /* set lock counter */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_5;
        reg_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_LOCK_CNT_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 16);
        if (result) return result;

        /* call CSU calibration function */
        result = utilHyPhy20gPifSerdesCsuCal(fileHndl, &converge_freq);
        if (result) return result;

        /* set CSU AnalogRSTB */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_2;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* hold CSU CTRL[11] for feedback divider */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_2;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_CTRL_OFF + 11;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* reset ADSM */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_10;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* update CSU vco center freq with new convergence */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_3;
        reg_msk = (0x003F << 10);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, converge_freq);
        if (result) return result;

        /* enable ADSM */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_10;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        DBG_PRINT("%s, %s, %d, finished BackplaneRateCfg csu cal routine \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of CSU calibration routine */

        /***************************/
        /* reset enable sequencing */
        /***************************/
        result = sysHyPhy20gMsDelay (50);
        if (result) return result;

        {
            UINT4 val = 0;
            addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_2;
            bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_LOCK_STATUS_OFF;
            result = sysHyPhy20gBitRead(fileHndl, addr, bitoffset, &val);
            if (result) return result;

            if (!val) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_LOCK_STATUS_OFF is b'0!\n",__FILE__, __FUNCTION__, __LINE__);
            }
        }

        /* RXCPGEN Initialization Starting*/
        /* RXCPGEN_CTRL[3] - disable VCO */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* RXCPGEN_ENB - Enable RXCPGEN */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /*result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_100USEC);
        if (result) return result;  */
       result = sysHyPhy20gMsDelay (5);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

       result = sysHyPhy20gMsDelay (5);
        if (result) return result;

        /* enable PLL so VCO control voltage goes to max */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

       result = sysHyPhy20gMsDelay (5);
        if (result) return result;

        /* RXCPGEN_CTRL[3] - enable VCO */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (5);
        if (result) return result;

        /* VCO control voltage = reset voltage */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (5);
        if (result) return result;

        /* let PLL lock */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (10);
        if (result) return result;

        /* RXCPGEN Initialization Completed!*/


        /* enable ADC */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_ADC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable ADC DACs */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_2_ADC_DAC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* enable csu digital reset */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_13;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_13_DIGITAL_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable digital reset */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_8, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_8_DIGITAL_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable PISO */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished PifRateCfg reset enable sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of reset enable sequence */
    } else {
        /* csu is already enabled, only configure rate info for linkID*/
        /********************/
        /* reset sequencing */
        /********************/
        /* RXCPGEN Initialization Starting*/
        /* RXCPGEN_CTRL[3] - disable VCO */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* RXCPGEN_ENB - Enable RXCPGEN */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (5);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

       result = sysHyPhy20gMsDelay (5);
        if (result) return result;


        /* enable PLL so VCO control voltage goes to max */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

       result = sysHyPhy20gMsDelay (5);
        if (result) return result;

        /* RXCPGEN_CTRL[3] - enable VCO */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (5);
        if (result) return result;

        /* VCO control voltage = reset voltage */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (5);
        if (result) return result;

        /* let PLL lock */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (10);
        if (result) return result;

        /* RXCPGEN Initialization Completed!*/

        /* hold ADC DACs in reset */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_2_ADC_DAC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold block in digital reset */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_8, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_8_DIGITAL_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold ADC in reset */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_ADC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold PISO in reset */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished PifRateCfg TxRx only reset sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of reset sequence */

        if (enable == 0) {
            DBG_PRINT("%s, %s, %d, enable is 0, PifRateCfg link held in reset\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_SUCCESS;
        }

        /*************************/
        /* lane/rate configs     */
        /*************************/
        /* if XAUI rate selected, use refclk b */
        if (rate == HYPHY20G_PIF_SERDES_XAUI_3125){
            addr = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_REG_CLK_SEL;
            bitoffset = HYPHY20G_SERDES_PIF_CSU_HYPHY_CSU_BIT_CLK_SEL_REFCLK_SEL_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;
        }

        /* config rx_path_div */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_8_RX_PATH_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rx_path_div);
        if (result) return result;

        /* program tx and rx bus widths */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_8_TXPATH_BUS_WIDTH_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txpath_bus_width);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_8_RXPATH_BUS_WIDTH_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rxpath_bus_width);
        if (result) return result;

        /* config piso - bypass divider, set divide ratio */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_DIV_ENABLE_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, piso_div_enable);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
        reg_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_CTRL_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, piso_ctrl);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished PifRateCfg TxRx only lane config sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of Lane config */

        /* enable ADC */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_ADC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable ADC DACs */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_2_ADC_DAC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* enable digital reset */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_8, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_8_DIGITAL_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable PISO */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished PifRateCfg TxRx only reset enable sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of reset enable sequence */
    }

    result = sysHyPhy20gMsDelay (10);
    if (result) return result;

    /* synch XAUI lanes */
    if (rate == HYPHY20G_PIF_SERDES_XAUI_3125) {
        result = utilHyPhy20gSerdesLnSync(fileHndl, HYPHY20G_SERDES_GRP_PIF);
        if (result) return result;
    }

    /* check rxcpgen lock status */
    result = utilHyPhy20gSerdesRxcpgenForceInit(fileHndl, linkID,
                                       HYPHY20G_SERDES_GRP_PIF);
    if (result) return result;

    /* toggle ADC reference to start */
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_9, linkID);
    bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_9_OCC_ADC_REFERENCE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_9, linkID);
    bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_9_OCC_ADC_REFERENCE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* adc offset correction */
    result = utilHyPhy20gSerdesAdcCalibrate(fileHndl,
                                            HYPHY20G_SERDES_PIF_INTF,
                                            linkID, adcCal);
    if (result) return result;

    /* mode selection */
    result = utilHyPhy20gAdcModeSelect(fileHndl, HYPHY20G_SERDES_PIF_INTF,
                                       linkID, eqMode);
    if (result) return result;

    /* program equalizer loop filter coeff (hardcoded now, should be set in fw ) */
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_3, linkID);
    reg_msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_3_COEFF_D_TEP_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x16);
    if (result) return result;

    /* program equalizer threshold clip detector (hardcoded now, should be set in fw ) */
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_4, linkID);
    reg_msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_4_TH_CLIP_DETECT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x12);
    if (result) return result;

    /* set LOS_ENABLE to 1 */
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_4, linkID);
    reg_msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_4_LOS_ENABLE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x1);
    if (result) return result;

    /* equalization */
    result = utilHyPhy20gSerdesEqCfg(fileHndl, HYPHY20G_SERDES_PIF_INTF, linkID,
                                     eqMode);
    if (result) return result;

    /* re-center RX FIFO */
    result = utilHyPhy20gSerdesRxFifoReCenter(fileHndl, HYPHY20G_SERDES_PIF_INTF,
                                              linkID);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifRateCfg */

/*******************************************************************************
**
**  hyPhy20gPifSerdesInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables a PIF Serdes link by taking it out of
**                  reset or disables it by putting it in reset.
**
**                  This function additionally controls the operations of
**                  PIF CSU device level reset.  It will clear the reset bit if
**                  any PIF SERDES is brought out of reset and it will set the
**                  reset bit if all PIF SERDES are put in reset state.
**
**                  After calling hyPhy20gPifSerdesInit with enable = 1 for a
**                  linkID, hyPhy20gPifRateCfg must be called for the same
**                  linkID prior to calling hyPhy20gPifSerdesInit for another
**                  linkID.
**
**                  The link can also optionally be put into a low power state.
**                  After putting the link in low power state, to re-enable the
**                  link, the SerdesInit API with enable = 1 should be called
**                  with the subsequent RateCfg API called to configure the
**                  rate specific settings
**
**  INPUTS:         *fileHndl - base address
**                  linkID    - 0 - PIF Serdes Link 0
**                              1 - PIF Serdes Link 1
**                              ..
**                              7 - PIF Serdes Link 7
**                  enable    - 0 - disable PIF Serdes Link
**                              1 - enable PIF Serdes Link
**                              2 - PIF Serdes Link in low power
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 hyPhy20gPifSerdesInit(struct file *fileHndl, UINT4 linkID, UINT4 enable)
{
    /* variable declaration */
    UINT4 addr, regData, serdesRstBitOff, csuRstBitOff;
    UINT4 msk;
    INT4 result;

    /* argument checking */
    if (linkID > 7 || enable > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    serdesRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_SYS_RST_0_OFF +
                      linkID;
    csuRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_SYS_CSU_RST_OFF;
    addr = HYPHY20G_TOP_REG_SYS_SW_RST_4;

    /* unconditionally apply device level reset bit */
    result = sysHyPhy20gBitWrite(fileHndl, addr, serdesRstBitOff, 1);
    if (result) return result;

    if (enable == 0 || enable == 2) {
        /* free memory used by SERDES subsystem */
        result = serdesHyPhy20gSerdesDestroyCtxt(fileHndl,
                                                 HYPHY20G_SERDES_PIF_INTF,
                                                 linkID);
        if (result) return result;

        /* test to determine if all reset bits are set, if so
           write CSU reset bit to 1 */
        result = sysHyPhy20gRead(fileHndl, addr, &regData);
        if (result) return result;

        msk = 0xFF << serdesRstBitOff;
        if ((regData & msk)== msk){
            result = sysHyPhy20gBitWrite(fileHndl, addr, csuRstBitOff, 1);
            if (result) return result;
        }
        
        if (enable == 2) {
            /* call util function to put interface in low power mode */
            result = utilHyPhy20gSerdesInitLwPwr(fileHndl, HYPHY20G_SERDES_PIF_INTF,
                                                 linkID);
            if (result) return result;        
        }

        return HYPHY20G_SUCCESS;
    }

    /* if enabling clear device level reset bit */
    result = sysHyPhy20gBitWrite(fileHndl, addr, serdesRstBitOff, 0);
    if (result) return result;

    /* unconditionally clear CSU reset bit */
    result = sysHyPhy20gBitWrite(fileHndl, addr, csuRstBitOff, 0);
    if (result) return result;

    if (enable == 1) {
        /* allocate and initialize memory used by SERDES subsystem */
        result = serdesHyPhy20gSerdesCreateCtxt(fileHndl, HYPHY20G_SERDES_PIF_INTF,
                                                linkID);
        if (result) return result;
    }
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gPIFSerdesInit */

/*******************************************************************************
**
**  hyPhy20gBackplaneRateCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Backplane/ESSI Serdes
**                  subsystem rate for a given link.
**
**                  hyPhy20gBackplaneSerdesInit must be called prior to this
**                  function.
**
**                  ADC calibration should only be performed on links with
**                  floating inputs or with active receive data traffic.
**                  Note that data integrity will be compromised on all links
**                  sharing the same CSU during ADC offset calibration.  ADC
**                  calibration can be deferred using adcCal == 3, and when
**                  ready, hyPhy20gSerdesADCCalibrate can be called to perform
**                  calibration across multiple SERDES interfaces and links.
**                  When adcCal == 2, call hyPhy20gSerdesLoadFactoryCal before
**                  this function.
**
**                  Equalization is selectable between hardcoded values or
**                  fw based equalization.  If firmware based equalization is
**                  used, hyPhy20gHostMsgInit must have been called prior to
**                  calling this function.
**
**                  When enable is 0, the low level serdes csu, rx and tx
**                  circuits are put in reset.
**
**
**  INPUTS:         *fileHndl - base address
**                  linkID    - 0 - Backplane Interface 0
**                              1 - Backplane Interface 1
**                              ..
**                              31 - Backplane Interface 31
**                  rate      - 0 - Interlaken 2.488G
**                              1 - Interlaken 3.125G
**                              2 - ESSI 2.488G
**                              3 - CBRI 3.3333333G
**                  adcCal    - 0 - do not perform ADC calibration
**                              1 - perform ADC calibration
**                              2 - perform ADC calibration using offsets in
**                                  memory (call hyPhy20gSerdesLoadFactoryCal
**                                  before this function)
**                              3 - defer ADC calibration, equalization
**                                  will not be performed (intended for calling
**                                  hyPhy20gSerdesADCCalibrate at a later point)
**                  eqMode    - 0 - hardcoded mode selection and equalization
**                                  values
**                              1 - perform mode selection and FW equalization
**                              2 - apply mode selection and equalization
**                                  using settings in memory (call
**                                  hyPhy20gSerdesLoadFactoryCal before this
**                                  function)
**                              3 - defer mode selection and FW equalization
**                                  (intended for calling
**                                   hyPhy20gSerdesADCCalibrate at a later
**                                   point)
**                  enable    - 0 - disable Backplane subsystem
**                              1 - enable Backplane subsystem
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SERDES_ADC_SFP_RATE_CHECK
**                  HYPHY20G_ERR_SERDES_ADC_CALIBRATION_FAILED
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gBackplaneRateCfg(struct file *fileHndl, UINT4 linkID, UINT4 rate,
                              UINT4 adcCal, UINT4 eqMode, UINT4 enable)
{
    /* local variables */
    UINT4 devId, revId, status;
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 reg_msk;
    UINT4 converge_freq;
    UINT4 csu_id;
    UINT4 csu_enable;
    UINT4 rxpath_bus_width;
    UINT4 txpath_bus_width;
    UINT4 adsm_clk_cnt_max;
    UINT4 adsm_frac_div;
    UINT4 adsm_int_div;
    UINT4 adsm_denomin;
    UINT4 rx_path_div;
    UINT4 piso_div_enable;
    UINT4 piso_ctrl;
    UINT4 csu_mode;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (linkID > HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (rate > HYPHY20G_BACKPLANE_SERDES_RATES_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((adcCal > 3) || (eqMode > 3)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;
    reg_msk = 0;
    converge_freq = 0;
    DBG_PRINT("%s, %s, %d, entering BackplaneRateCfg \n",__FILE__, __FUNCTION__, __LINE__);

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkID];
    HYPHY20G_ASSERT((pSerdesCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* initialize local variables for setup */
    /* csu[0] corresponds to links 0 to 7 and 16 to 23  */
    /* csu[1] corresponds to links 8 to 15 and 24 to 31 */

    if (((linkID >= 0) && (linkID <= 7)) || ((linkID >= 16) && (linkID <= 23))) {
        csu_id = 0;
    } else {
        if (((linkID >= 8) && (linkID <= 15)) || ((linkID >= 24) && (linkID <= 31))) {
            csu_id = 1;
        } else {
            return HYPHY20G_ERR_INVALID_ARG;
        }
    }
    /* initialize rate specific settings */
    switch(rate){
        case HYPHY20G_BACKPLANE_SERDES_INTERLAKEN_2488_32:
            rxpath_bus_width = 0x01;
            txpath_bus_width = 0x01;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 0;
            adsm_int_div = 40;
            adsm_denomin = 10;
            rx_path_div = 0x01;
            piso_div_enable = 0x01;
            piso_ctrl = 0x00;
            csu_mode = 0x01;
        break;
        case HYPHY20G_BACKPLANE_SERDES_INTERLAKEN_3125:
            rxpath_bus_width = 0x01;
            txpath_bus_width = 0x01;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 48910;
            adsm_int_div = 40;
            adsm_denomin = 260496;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            csu_mode = 0x03;
        break;
        case HYPHY20G_BACKPLANE_SERDES_ESSI_2488_32:
            rxpath_bus_width = 0x00;
            txpath_bus_width = 0x00;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 0;
            adsm_int_div = 40;
            adsm_denomin = 10;
            rx_path_div = 0x01;
            piso_div_enable = 0x01;
            piso_ctrl = 0x00;
            csu_mode = 0x01;
        break;
        case HYPHY20G_BACKPLANE_SERDES_CBRI_3333_333_3:
            rxpath_bus_width = 0x01;
            txpath_bus_width = 0x01;
            adsm_clk_cnt_max = 4000;
            adsm_frac_div = 224711;
            adsm_int_div = 42;
            adsm_denomin = 259200;
            rx_path_div = 0x00;
            piso_div_enable = 0x00;
            piso_ctrl = 0x00;
            csu_mode = 0x03;
        break;
        default:
            /* redundant to initial argument checking */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* check to see if csu is enabled yet, CSU is default to 1.  if 1, enable csu */
    addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_2, csu_id);
    bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_ENB_OFF;
    result = sysHyPhy20gBitRead(fileHndl, addr, bitoffset, &csu_enable);
    if (result) return result;

    if (csu_enable == 1) {
        /* csu is not enabled, enable csu for this bank of links while configuring rate */
        /********************/
        /* reset sequencing */
        /********************/
        /* enable csu */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_2, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* enable rxpatgen */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* hold csu analog blocks in reset */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_2, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* setup divider */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_2, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_CTRL_OFF + 11;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* hold rxpatgen analog blocks in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold rxpatgen vco - RXCPGEN_CTRL[3] in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold ADC DACs in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_2_ADC_DAC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold rxpatgen blocks in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold csu block in digital reset */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_13, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_13_DIGITAL_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold block in digital reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_8, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_DIGITAL_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold ADC in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_ADC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold PISO in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold adsm in reset */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_10, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished BackplaneRateCfg reset sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of reset sequence */

        if (enable == 0) {
            DBG_PRINT("%s, %s, %d, enable is 0, BackplaneRateCfg link held in reset\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_SUCCESS;
        }

        /********************/
        /* default settings */
        /********************/
        /* disable repeater and terminators */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_HYPHY_CSU_REG_REP_TER_CTL, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_HYPHY_CSU_BIT_REP_TER_CTL_RST_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_HYPHY_CSU_REG_REP_TER_CTL, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_HYPHY_CSU_BIT_REP_TER_CTL_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* config terminator block for csu */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_HYPHY_CSU_REG_REP_TER_CTL, csu_id);
        reg_msk = HYPHY20G_SERDES_ESSI_CSU_HYPHY_CSU_BIT_REP_TER_CTL_TERM_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x06);
        if (result) return result;

        if (adcCal == 0) {
            /* config linear mode (hardcoded for PMC EVBD test board, should
               call ADCModeSel util) */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_11, linkID);
            reg_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_11_ACCUM_REF_SEL_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x02);
            if (result) return result;
        } else {
            /* program ADC MODE selection to Table 1 and run mode selection algo
               after ADC calibration */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_11, linkID);
            reg_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_11_ACCUM_REF_SEL_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x0);
            if (result) return result;
        }

        /* disable ADSM seed */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_5, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_SEED_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* program OC_INTERVAL to 0xFFFF - required non-default setting */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_11, csu_id);
        reg_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_11_OC_INTERVAL_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0xFFFF);
        if (result) return result;

        /* set CSU_CTRL[8:6] bit */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_2, csu_id);
        reg_msk = (0x0007 << 18);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x03);
        if (result) return result;

        /* set CSU_CTRL[14:13] bit */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_2, csu_id);
        reg_msk = (0x0003 << 25);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
        if (result) return result;

        /* end of default settings */

        /*************************/
        /* lane/rate configs     */
        /*************************/
        /* config rx_path_div */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_RX_PATH_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rx_path_div);
        if (result) return result;

        /* program tx and rx bus widths */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_TXPATH_BUS_WIDTH_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txpath_bus_width);
        if (result) return result;

        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_RXPATH_BUS_WIDTH_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rxpath_bus_width);
        if (result) return result;

        /* config piso - bypass divider, set divide ratio */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_DIV_ENABLE_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, piso_div_enable);
        if (result) return result;

        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
        reg_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_CTRL_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, piso_ctrl);
        if (result) return result;

        /* program ADSM int, frac and denomin settings */
        /* see rate specific init lookup table for all the different rates */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_5, csu_id);
        reg_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_INT_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_int_div);
        if (result) return result;

        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_6, csu_id);
        reg_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_6_ADSM_FRAC_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_frac_div);
        if (result) return result;

        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_9, csu_id);
        reg_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_9_ADSM_DENOMIN_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_denomin);
        if (result) return result;

        /* config cs_mode[4:2] post divider */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_1, csu_id);
        reg_msk = (0x0007 << 18);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, csu_mode);
        if (result) return result;

        /* config csu_ctrl[3:2] */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_2, csu_id);
        reg_msk = (0x0003 << 14);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
        if (result) return result;

        /* config csu_mode[15:10] - set to default will be calibrated */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_3, csu_id);
        reg_msk = (0x003F << 10);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x2D);
        if (result) return result;

        /* config csu_mode[5:0] charge pump */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_3, csu_id);
        reg_msk = (0x0003F << 0);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x03);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* enable ADSM again (may not be necessary as we reset again in cal loop)*/
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_10, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished BackplaneRateCfg lane config sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of Lane config */

        /*************************/
        /* CSU calibration       */
        /*************************/
        /* set clock count max */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_8, csu_id);
        reg_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_8_ADSM_CLK_CNT_MAX_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, HYPHY20G_SERDES_BACKPLANE_ADSM_CLK_CNT_MAX_VAL);
        if (result) return result;

         /* set lock counter */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_5, csu_id);
        reg_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_LOCK_CNT_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 16);
        if (result) return result;

        /* call CSU calibration function */
        result = utilHyPhy20gBackplaneSerdesCsuCal(fileHndl, csu_id, &converge_freq);
        if (result) return result;

        /* set CSU AnalogRSTB */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_2, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* hold CSU CTRL[11] for feedback divider */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_2, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_CTRL_OFF + 11;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* reset ADSM */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_10, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* update CSU vco center freq with new convergence */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_3, csu_id);
        reg_msk = (0x003F << 10);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, converge_freq);
        if (result) return result;

        /* enable ADSM */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_10, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        DBG_PRINT("%s, %s, %d, finished BackplaneRateCfg csu cal routine \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of CSU calibration routine */

        /***************************/
        /* reset enable sequencing */
        /***************************/
        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /** RXCPGEN Initialization started! **/

        /* hold rxpcgen vco - RXCPGEN_CTRL[3] in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold rxpcgen analog blocks in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold rxpcgen blocks in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* Disable RXCPGEN */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* Enable RXCPGEN */       
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_5USEC);
        if (result) return result;

        /* Take rxpcgen blocks out of reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* Toggle RXCPGEN Analog Reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /** RXCPGEN Initalization Completed! **/

        /* enable ADC */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_ADC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable ADC DACs */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_2_ADC_DAC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* enable csu digital reset */
        addr = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_13, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_13_DIGITAL_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable digital reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_8, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_DIGITAL_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable PISO */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished BackplaneRateCfg reset enable sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of reset enable sequence */
    } else {
        /* csu is already enabled, only configure rate info for linkID*/
        /********************/
        /* reset sequencing */
        /********************/

        /** RXCPGEN Initialization started! **/

        /* hold rxpcgen vco - RXCPGEN_CTRL[3] in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold rxpcgen analog blocks in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold rxpcgen blocks in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* Disable RXCPGEN */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* Enable RXCPGEN */       
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* Take rxpcgen blocks out of reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* Toggle RXCPGEN Analog Reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /** RXCPGEN Initalization Completed! **/

        /* hold ADC DACs in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_2_ADC_DAC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;


        /* hold block in digital reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_8, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_DIGITAL_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold ADC in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_ADC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold PISO in reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished BackplaneRateCfg TxRx only reset sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of reset sequence */

        if (enable == 0) {
            DBG_PRINT("%s, %s, %d, enable is 0, BackplaneRateCfg link held in reset\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_SUCCESS;
        }

        /*************************/
        /* lane/rate configs     */
        /*************************/
        /* config rx_path_div */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_RX_PATH_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rx_path_div);
        if (result) return result;

        /* program tx and rx bus widths */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_TXPATH_BUS_WIDTH_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txpath_bus_width);
        if (result) return result;

        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_RXPATH_BUS_WIDTH_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rxpath_bus_width);
        if (result) return result;

        /* config piso - bypass divider, set divide ratio */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_DIV_ENABLE_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, piso_div_enable);
        if (result) return result;

        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
        reg_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_CTRL_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, piso_ctrl);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished BackplaneRateCfg TxRx only lane config sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of Lane config */

        /***************************/
        /* reset enable sequencing */
        /***************************/
        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* enable rxpatgen vco - RXCPGEN_CTRL[3] */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable rxpatgen blocks */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gUsDelay (HYPHY20G_DELAY_5USEC);
        if (result) return result;

        /* enable rxpatgen analog blocks */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_5USEC);
        if (result) return result;

        /* enable ADC */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_ADC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable ADC DACs */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_2_ADC_DAC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* enable digital reset */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_8, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_DIGITAL_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable PISO */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished BackplaneRateCfg TxRx only reset enable sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of reset enable sequence */
    }

    /* check rxcpgen lock status */
    result = utilHyPhy20gSerdesRxcpgen(fileHndl, linkID,
                                       HYPHY20G_SERDES_GRP_BACKPLANE);
    if (result) return result;

    /* toggle ADC reference to start */
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_9, linkID);
    bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_9_OCC_ADC_REFERENCE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_9, linkID);
    bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_9_OCC_ADC_REFERENCE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* run ADC calibration (offset cal and mode sel) */
    result = utilHyPhy20gSerdesAdcCalibrate(fileHndl,
                                            HYPHY20G_SERDES_BACKPLANE_INTF,
                                            linkID, adcCal);
    if (result) return result;

    /* mode selection */
    result = utilHyPhy20gAdcModeSelect(fileHndl, HYPHY20G_SERDES_BACKPLANE_INTF,
                                       linkID, eqMode);
    if (result) return result;

    /* program equalizer loop filter coeff (hardcoded now, should be set in fw, default value) */
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_3, linkID);
    reg_msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_3_COEFF_D_TEP_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x16);
    if (result) return result;

    /* program equalizer threshold clip detector (hardcoded now, should be set in fw, default value ) */
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_4, linkID);
    reg_msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_4_TH_CLIP_DETECT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x12);
    if (result) return result;

    /* Set LOS_ENABLE to 1 */
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_4, linkID);
    reg_msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_4_LOS_ENABLE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
    if (result) return result;


    /* equalization */
    result = utilHyPhy20gSerdesEqCfg(fileHndl, HYPHY20G_SERDES_BACKPLANE_INTF,
                                     linkID, eqMode);
    if (result) return result;

    /* re-center RX FIFO */
    result = utilHyPhy20gSerdesRxFifoReCenter(fileHndl,
                                        HYPHY20G_SERDES_BACKPLANE_INTF, linkID);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gBackplaneRateCfg */
/*******************************************************************************
**
**  hyPhy20gBackplaneSerdesInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables a Backplane Serdes link by taking it
**                  out of reset or disables it by putting it in reset.
**
**                  This function additionally controls the operations of
**                  Backplane CSU device level reset.  It will clear the reset
**                  bit if any Backplane SERDES is brought out of reset and it
**                  will set the reset bit if all Backplane SERDES are put in
**                  reset state.
**
**                  After calling hyPhy20gBackplaneSerdesInit with enable = 1
**                  for a linkID, hyPhy20gBackplaneSerdesRateCfg must be called
**                  for the same linkID prior to calling
**                  hyPhy20gBackplaneSerdesInit for another linkID.
**
**                  The link can also optionally be put into a low power state.
**                  After putting the link in low power state, to re-enable the
**                  link, the SerdesInit API with enable = 1 should be called
**                  with the subsequent RateCfg API called to configure the
**                  rate specific settings
**
**  INPUTS:         *fileHndl - base address
**                  linkID    - 0 - Backplane Serdes Link 0
**                              1 - Backplane Serdes Link 1
**                              ..
**                              31 - Backplane Serdes Link 31
**                  enable    - 0 - disable Backplane Serdes Link
**                              1 - enable Backplane Serdes Link
**                              2 - Backplane Serdes Link in low power
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 hyPhy20gBackplaneSerdesInit(struct file *fileHndl, UINT4 linkID,
                                 UINT4 enable)
{
    /* variable declaration */
    UINT4 serdesAddr, csuAddr, regData, csuRstBitOff;
    UINT4 serdesRstBitOff = 0;
    UINT4 rstBitRange;
    INT4 result;

    /* argument checking */
    if (linkID > HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM || enable > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    serdesAddr = HYPHY20G_TOP_REG_SYS_SW_RST_5;
    /* links 0 to 7 and 16 to 23 corresponds to back0[0:15] */
    /* links 8 to 15 and 24 to 31 corresponds to back1[0:15] */
    if ((linkID >= 0) && (linkID <= 7)) {
        serdesRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_5_SERDES_BACK0_RST_0_OFF
                      + linkID;
    }
    if ((linkID >= 8) && (linkID <= 15)) {
        serdesRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_5_SERDES_BACK0_RST_0_OFF
                      + linkID + 8;
    }
    if ((linkID >= 16) && (linkID <= 23)) {
        serdesRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_5_SERDES_BACK0_RST_0_OFF
                      + linkID - 8;
    }
    if ((linkID >= 24) && (linkID <= 31)) {
        serdesRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_5_SERDES_BACK0_RST_0_OFF
                      + linkID;
    }

    csuAddr = HYPHY20G_TOP_REG_SYS_SW_RST_4;
    if (((linkID >= 0) && (linkID <= 7)) || ((linkID >= 16) && (linkID <= 23))) {
        csuRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_BACK0_CSU_RST_OFF;
    } else {
        csuRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_BACK1_CSU_RST_OFF;
    }

    /* unconditionally apply device level reset bit */
    result = sysHyPhy20gBitWrite(fileHndl, serdesAddr, serdesRstBitOff, 1);
    if (result) return result;

    if (enable == 0 || enable == 2) {
        /* free memory used by SERDES subsystem */
        result = serdesHyPhy20gSerdesDestroyCtxt(fileHndl,
                                                 HYPHY20G_SERDES_BACKPLANE_INTF,
                                                 linkID);
        if (result) return result;

        /* test to determine if all reset bits are set, if so
           write CSU reset bit to 1 */
        result = sysHyPhy20gRead(fileHndl, serdesAddr, &regData);
        if (result) return result;
        /* links 0 to 7 and 16 to 23 corresponds to back0[0:15] */
        /* links 8 to 15 and 24 to 31 corresponds to back1[0:15] */
        if (((linkID >= 0) && (linkID <= 7)) || ((linkID >= 16) && (linkID <= 23))) {
          rstBitRange = 0xFFFF;
        } else {
          rstBitRange = 0xFFFF0000;
        }
        if ((regData & rstBitRange)== (rstBitRange)){
            result = sysHyPhy20gBitWrite(fileHndl, csuAddr, csuRstBitOff, 1);
            if (result) return result;
        }

        if (enable == 2) {
            /* call util function to put interface in low power mode */
            result = utilHyPhy20gSerdesInitLwPwr(fileHndl, 
                                                 HYPHY20G_SERDES_BACKPLANE_INTF,
                                                 linkID);
            if (result) return result;        
        }

        return HYPHY20G_SUCCESS;
    }

    /* if enabling clear device level reset bit */
    result = sysHyPhy20gBitWrite(fileHndl, serdesAddr, serdesRstBitOff, 0);
    if (result) return result;

    /* unconditionally clear CSU reset bit */
    result = sysHyPhy20gBitWrite(fileHndl, csuAddr, csuRstBitOff, 0);
    if (result) return result;

    if (enable == 1) {
        /* allocate and initialize memory used by SERDES subsystem */
        result = serdesHyPhy20gSerdesCreateCtxt(fileHndl, 
                                                HYPHY20G_SERDES_BACKPLANE_INTF,
                                                linkID);
        if (result) return result;
    }
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gBackplaneSerdesInit */

/*******************************************************************************
**
**  hyPhy20gXfiSerdesTxCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the XFI Serdes Tx pre-emphasis and
**                  swing control.
**                  Pre-emphasis can be set from 0 to 15.
**                  Swing can be set to the range of 3 to 48 minus 2x the
**                  pre-emphasis units.
**
**  INPUTS:         *fileHndl   - base address
**                  linkID      - 0 - XFI Serdes Link 0
**                                1 - XFI Serdes Link 1
**                  preEmp      - 0 - 0dB
**                                1 - 0.357dB
**                                2 - 0.736dB
**                                3 - 1.13dB
**                                4 - 1.55dB
**                                5 - 1.98dB
**                                6 - 2.45dB
**                                7 - 2.94dB
**                                8 - 3.46dB
**                                9 - 4.01dB
**                                10 - 4.61dB
**                                11 - 5.24dB
**                                12 - 5.94dB
**                                13 - 6.69dB
**                                14 - 7.52dB
**                                15 - 8.42dB
**                  swing       - 3..48
**                              - swing + 2*preEmp <= 48
**                  enable      - 0 - disables tx pre-emphasis
**                                1 - enables tx pre-emphasis
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gXfiSerdesTxCfg(struct file *fileHndl, UINT4 linkID,
                            UINT4 preEmp, UINT4 swing, UINT4 enable)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 bitmsk;
    UINT4 den_core;
    UINT4 sw_ctrl_core;
    UINT4 val;

    /* argument checking */
    if (enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (linkID > HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (preEmp > 15) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((swing < 3) || (swing > 48)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (((swing + (2*preEmp)) < 1)  || ((swing + (2*preEmp)) > 48)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;
    bitmsk = 0;
    den_core = 0;
    sw_ctrl_core = 0;

    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_PRE1_ENABLE_OFF;
    /* unconditionally disable pre-emp */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;
    if (enable == 0) {
        preEmp = 0; 
    }

    /* if enabling pre-emp, configure bits */
    /* set pre-emphasis level */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_6, linkID);
    bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_6_TX_DPENI1_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, preEmp);
    if (result) return result;

    /* calculate swing adjustment values */
    result = utilHyPhy20gSerdesCalcTxSwing(fileHndl, preEmp, swing, &den_core,
                                           &sw_ctrl_core);
    if (result) return result;

    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_5, linkID);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    /* set DEN_CORE */
    bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_5_TX_DEN_CORE_MSK;
    result = hyPhy20gVarFieldWrite(&val, bitmsk, den_core);
    if (result) return result;

    /* set SWING_CTRL_CORE */
    bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_5_TX_SWING_CTRL_CORE_MSK;
    result = hyPhy20gVarFieldWrite(&val, bitmsk, sw_ctrl_core);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* turn on pre-emphasis */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_PRE1_ENABLE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gXfiSerdesTxCfg */

/*******************************************************************************
**
**  hyPhy20gSfpSerdesTxCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the SFP Serdes Tx swing and
**                  pre-emphasis control.
**                  Pre-emphasis can be set from 0 to 15.
**                  Swing can be set to the range of 3 to 48 minus 2x the
**                  pre-emphasis units.
**
**  INPUTS:         *fileHndl   - base address
**                  linkID    - 0 - SFP interface 0
**                              1 - SFP interface 1
**                              ..
**                              15 - SFP interface 15
**                  preEmp      - 0 - 0dB
**                                1 - 0.357dB
**                                2 - 0.736dB
**                                3 - 1.13dB
**                                4 - 1.55dB
**                                5 - 1.98dB
**                                6 - 2.45dB
**                                7 - 2.94dB
**                                8 - 3.46dB
**                                9 - 4.01dB
**                                10 - 4.61dB
**                                11 - 5.24dB
**                                12 - 5.94dB
**                                13 - 6.69dB
**                                14 - 7.52dB
**                                15 - 8.42dB
**                  swing       - 3..48
**                              - swing + 2*preEmp <= 48
**                  enable      - 0 - disables tx pre-emphasis
**                                1 - enables tx pre-emphasis
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSfpSerdesTxCfg(struct file *fileHndl, UINT4 linkID,
                            UINT4 preEmp, UINT4 swing, UINT4 enable)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 bitmsk;
    UINT4 den_core;
    UINT4 sw_ctrl_core;
    UINT4 val;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((linkID > HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM) && (linkID != 17)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (preEmp > 15) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((swing < 3) || (swing > 48)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (((swing + (2*preEmp)) < 1)  || ((swing + (2*preEmp)) > 48)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;
    bitmsk = 0;
    den_core = 0;
    sw_ctrl_core = 0;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_PRE1_ENABLE_OFF;
    /* unconditionally disable pre-emp */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;
    if (enable == 0) {
		preEmp = 0; /*If Tx Pre-emphasis is disabled, preEmp is set to zero*/
    }

    /* if enabling pre-emp, configure bits */
    /* set pre-emphasis level */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_5, linkID);
    bitmsk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_5_TX_DPENI1_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, preEmp);
    if (result) return result;

    /* calculate swing adjustment values */
    result = utilHyPhy20gSerdesCalcTxSwing(fileHndl, preEmp, swing, &den_core,
                                           &sw_ctrl_core);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_4, linkID);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    /* set DEN_CORE */
    bitmsk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_4_TX_DEN_CORE_MSK;
    result = hyPhy20gVarFieldWrite(&val, bitmsk, den_core);
    if (result) return result;

    /* set SWING_CTRL_CORE */
    bitmsk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_4_TX_SWING_CTRL_CORE_MSK;
    result = hyPhy20gVarFieldWrite(&val, bitmsk, sw_ctrl_core);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* turn on pre-emphasis */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_PRE1_ENABLE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSfpSerdesTxCfg */

/*******************************************************************************
**
**  hyPhy20gSgmiiSerdesTxCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Sgmii Serdes Tx swing and
**                  de-emphasis control.
**                  Pre-emphasis can be set from 0 to 15.
**                  Swing can be set to the range of 3 to 48 minus 2x the
**                  pre-emphasis units.
**
**  INPUTS:         *fileHndl   - base address
**                  preEmp      - 0 - 0dB
**                                1 - 0.357dB
**                                2 - 0.736dB
**                                3 - 1.13dB
**                                4 - 1.55dB
**                                5 - 1.98dB
**                                6 - 2.45dB
**                                7 - 2.94dB
**                                8 - 3.46dB
**                                9 - 4.01dB
**                                10 - 4.61dB
**                                11 - 5.24dB
**                                12 - 5.94dB
**                                13 - 6.69dB
**                                14 - 7.52dB
**                                15 - 8.42dB
**                  swing       - 3..48
**                              - swing + 2*preEmp <= 48
**                  enable      - 0 - disables tx pre-emphasis
**                                1 - enables tx pre-emphasis
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSgmiiSerdesTxCfg(struct file *fileHndl, UINT4 preEmp,
                              UINT4 swing, UINT4 enable)
{
    UINT4 linkID;
    INT4 result;

    linkID = 17;

    result = hyPhy20gSfpSerdesTxCfg(fileHndl, linkID, preEmp, swing,
                                    enable);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSgmiiSerdesTxCfg */


/*******************************************************************************
**
**  hyPhy20gBackplaneSerdesTxCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the Backplane Serdes Tx
**                  swing and de-emphasis control.
**                  Pre-emphasis can be set from 0 to 15.
**                  Swing can be set to the range of 3 to 48 minus 2x the
**                  pre-emphasis units.
**
**  INPUTS:         *fileHndl   - base address
**                  linkID    - 0 - Backplane Interface 0
**                              1 - Backplane Interface 1
**                              ..
**                              31 - Backplane Interface 31
**                  preEmp      - 0 - 0dB
**                                1 - 0.357dB
**                                2 - 0.736dB
**                                3 - 1.13dB
**                                4 - 1.55dB
**                                5 - 1.98dB
**                                6 - 2.45dB
**                                7 - 2.94dB
**                                8 - 3.46dB
**                                9 - 4.01dB
**                                10 - 4.61dB
**                                11 - 5.24dB
**                                12 - 5.94dB
**                                13 - 6.69dB
**                                14 - 7.52dB
**                                15 - 8.42dB
**                  swing       - 3..48
**                              - swing + 2*preEmp <= 48
**                  enable      - 0 - disables tx pre-emphasis
**                                1 - enables tx pre-emphasis
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gBackplaneSerdesTxCfg(struct file *fileHndl, UINT4 linkID,
                                  UINT4 preEmp, UINT4 swing, UINT4 enable)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 bitmsk;
    UINT4 den_core;
    UINT4 sw_ctrl_core;
    UINT4 val;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (linkID > HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (preEmp > 15) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((swing < 3) || (swing > 48)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (((swing + (2*preEmp)) < 1)  || ((swing + (2*preEmp)) > 48)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;
    bitmsk = 0;
    den_core = 0;
    sw_ctrl_core = 0;

    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
    bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_PRE1_ENABLE_OFF;
    /* unconditionally disable pre-emp */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;
    if (enable == 0) {
        preEmp = 0; 
    }

    /* if enabling pre-emp, configure bits */
    /* set pre-emphasis level */
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_5, linkID);
    bitmsk = HYPHY20G_SERDES_BP_C8_BIT_CFG_5_TX_DPENI1_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, preEmp);
    if (result) return result;

    /* calculate swing adjustment values */
    result = utilHyPhy20gSerdesCalcTxSwing(fileHndl, preEmp, swing, &den_core,
                                           &sw_ctrl_core);
    if (result) return result;

    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_6, linkID);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    /* set DEN_CORE */
    bitmsk = HYPHY20G_SERDES_BP_C8_BIT_CFG_6_TX_DEN_CORE_MSK;
    result = hyPhy20gVarFieldWrite(&val, bitmsk, den_core);
    if (result) return result;

    /* set SWING_CTRL_CORE */
    bitmsk = HYPHY20G_SERDES_BP_C8_BIT_CFG_6_TX_SWING_CTRL_CORE_MSK;
    result = hyPhy20gVarFieldWrite(&val, bitmsk, sw_ctrl_core);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* turn on pre-emphasis */
    addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
    bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_PRE1_ENABLE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gBackplaneSerdesTxCfg */

/*******************************************************************************
**
**  hyPhy20gPifSerdesTxCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the PIF Serdes Tx swing and
**                  de-emphasis control.
**                  Pre-emphasis can be set from 0 to 15.
**                  Swing can be set to the range of 3 to 48 minus 2x the
**                  pre-emphasis units.
**
**  INPUTS:         *fileHndl   - base address
**                  linkID    - 0 - PIF Interface 0
**                              1 - PIF Interface 1
**                              ..
**                              7 - PIF Interface 7
**                  preEmp      - 0 - 0dB
**                                1 - 0.357dB
**                                2 - 0.736dB
**                                3 - 1.13dB
**                                4 - 1.55dB
**                                5 - 1.98dB
**                                6 - 2.45dB
**                                7 - 2.94dB
**                                8 - 3.46dB
**                                9 - 4.01dB
**                                10 - 4.61dB
**                                11 - 5.24dB
**                                12 - 5.94dB
**                                13 - 6.69dB
**                                14 - 7.52dB
**                                15 - 8.42dB
**                  swing       - 3..48
**                              - swing + 2*preEmp <= 48
**                  enable      - 0 - disables tx pre-emphasis
**                                1 - enables tx pre-emphasis
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gPifSerdesTxCfg(struct file *fileHndl, UINT4 linkID,
                            UINT4 preEmp, UINT4 swing, UINT4 enable)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 bitmsk;
    UINT4 den_core;
    UINT4 sw_ctrl_core;
    UINT4 val;

    /* argument checking */
    if (enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (linkID > HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (preEmp > 15) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((swing < 3) || (swing > 48)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (((swing + (2*preEmp)) < 1)  || ((swing + (2*preEmp)) > 48)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;
    bitmsk = 0;
    den_core = 0;
    sw_ctrl_core = 0;

    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
    bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_PRE1_ENABLE_OFF;
    /* unconditionally disable pre-emp */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;
    if (enable == 0) {
		preEmp = 0;
    }

    /* if enabling pre-emp, configure bits */
    /* set pre-emphasis level */
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_5, linkID);
    bitmsk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_5_TX_DPENI1_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, preEmp);
    if (result) return result;

    /* calculate swing adjustment values */
    result = utilHyPhy20gSerdesCalcTxSwing(fileHndl, preEmp, swing, &den_core,
                                           &sw_ctrl_core);
    if (result) return result;

    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_6, linkID);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    /* set DEN_CORE */
    bitmsk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_6_TX_DEN_CORE_MSK;
    result = hyPhy20gVarFieldWrite (&val, bitmsk, den_core);
    if (result) return result;

    /* set SWING_CTRL_CORE */
    bitmsk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_6_TX_SWING_CTRL_CORE_MSK;
    result = hyPhy20gVarFieldWrite (&val, bitmsk, sw_ctrl_core);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* turn on pre-emphasis */
    addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
    bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_PRE1_ENABLE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gPifSerdesTxCfg */

/*******************************************************************************
**
**  hyPhy20gSfpCustomRateCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the SFP Serdes subsystem rate.
**
**
**  INPUTS:         *fileHndl       - base address
**                  linkID          - 0 - SFP interface 0
**                                    1 - SFP interface 1
**                                    ..
**                                    15 - SFP interface 15
**                  busWidth        - 0 - 8 bit
**                                    1 - 10 bit
**                                    2 - 16 bit
**                                    3 - 20 bit
**                  adsmFracDiv     - 23-bit PLL multiplier nominator
**                  adsmIntDiv      - 8-bit PLL multiplier integer
**                  adsmDenomin     - 23-bit PLL multiplier denominator
**                                    Mult = INT_DIV + (FRAC_DIV + FRAC_DIV_MOD)/DENOMIN
**                  rxPathDiv       - RX PATH decimation factor
**                                    0 - Div 1
**                                    1 - Div 2
**                                    2 - Div 4
**                                    3 - Div 8
**                                    4 - Div 16
**                                    5 - Div 32
**                                    6 - Div 64
**                                    7 - Div 128
**                                    8 - Div 256
**                  pisoDivEnable   - Enable clock divider in the PISO
**                                    0 - Divider bypassed
**                                    1 - Divider enabled
**                  pisoCtrl        - PISO clock divide ratio
**                                    0 - Decimation by 2
**                                    1 - Decimation by 4
**                                    2 - Decimation by 8
**                                    3 - Decimation by 16
**                                    4 - Decimation by 32
**                                    5 - Decimation by 64
**                                    6 - Decimation by 128
**                                    7 - Decimation by 256
**                  postDiv         - Selects post-divider ratio in PLL to generate CLKHR(B)
**                                    0 - Div 2
**                                    1 - Div 2.5
**                                    2 - Div 3
**                                    3 - Div 3.5
**                  vcoSelect       - 0 - Slower VCO (4G 4.65G)
**                                    1 - Faster VCO (4.65G 5.4G)
**                  enable          - 1 - enable SFP subsystem
**                                    0 - disable SFP subsystem
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_SERDES_ADSM_LOCK
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gSfpCustomRateCfg(struct file *fileHndl, UINT4 linkID, UINT4 busWidth,
                              UINT4 adsmFracDiv, UINT4 adsmIntDiv, UINT4 adsmDenomin,
                              UINT4 rxPathDiv, UINT4 pisoDivEnable, UINT4 pisoCtrl,
                              UINT4 postDiv, UINT4 vcoSelect, UINT4 enable)
{
    /* local variables */
    UINT4 devId, revId, status;
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 reg_msk;
    UINT4 converge_freq;
    UINT4 rxpath_bus_width;
    UINT4 txpath_bus_width;
    UINT4 adsm_clk_cnt_max;
    UINT4 adsm_frac_div;
    UINT4 adsm_int_div;
    UINT4 adsm_denomin;
    UINT4 rx_path_div;
    UINT4 piso_div_enable;
    UINT4 piso_ctrl;
    UINT4 csu_mode;
    UINT4 vco_select;
    UINT4 acb_pll_s_max;
    UINT4 val;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (linkID > HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (busWidth > 3) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (adsmFracDiv > 16777215) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (adsmIntDiv > 255) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (adsmDenomin > 16777215) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (rxPathDiv > 8) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pisoDivEnable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pisoCtrl > 7) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (postDiv > 3) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (vcoSelect > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;
    reg_msk = 0;
    converge_freq = 0;
    DBG_PRINT("%s, %s, %d, entering SfpRateCfg \n",__FILE__, __FUNCTION__, __LINE__);

    /* initialize rate specific settings */
    rxpath_bus_width = busWidth;
    txpath_bus_width = busWidth;
    adsm_clk_cnt_max = 4000;
    adsm_frac_div = adsmFracDiv;
    adsm_int_div = adsmIntDiv;
    adsm_denomin = adsmDenomin;
    rx_path_div = rxPathDiv;
    piso_div_enable = pisoDivEnable;
    piso_ctrl = pisoCtrl;
    csu_mode = postDiv;
    vco_select = vcoSelect;
    acb_pll_s_max = 0xc;

    /********************/
    /* reset sequencing */
    /********************/
    /* hold CSU AnalogRSTB */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_CSU_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* set CSU CTRL[11] for feedback divider */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_CSU_CTRL_OFF + 11;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* hold rxpatgen analog blocks in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold rxpatgen vco - RXCPGEN_CTRL[3] in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_CTRL_OFF + 3;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold ADC DACs in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_DAC_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold rxpatgen blocks in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold block in digital reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_DIGITAL_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold ADC in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold PISO in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* hold ADSM in reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* enable CSU (default) */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_CSU_ENB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* turn off CSU AnalogTM (default) */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_CSU_ATMSB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    result = sysHyPhy20gMsDelay (1);
    if (result) return result;
    DBG_PRINT("%s, %s, %d, finished SfpRateCfg reset sequence \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of reset sequence */

    if (enable == 0) {
        DBG_PRINT("%s, %s, %d, enable is 0, SfpRateCfg link held in reset\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_SUCCESS;
    }

    /********************/
    /* default settings */
    /********************/
    /* enable majority filter in cdru */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_1_FILT_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* program equalizer loop filter coeff */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_3, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_3_COEFF_D_TEP_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x0C);
    if (result) return result;

	/* set LOS_ENABLE to 1 */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_4, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_4_LOS_ENABLE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
    if (result) return result;

    /* enable isolated pulse detector in cdru */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_1_IPD_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* enable soft TED in cdru */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_1_TED_SEL_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* disable ADSM seed */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_11, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_SEED_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* set ADSM_CLKO_POLARITY bit */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_12, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_12_ADSM_CLKO_POLARITY_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* set CSU_CTRL[8:6] bit */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    reg_msk = (0x0007 << 22);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x03);
    if (result) return result;

    /* set CSU_CTRL[14:13] bit */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    reg_msk = (0x0003 << 29);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
    if (result) return result;

    /* end of default settings */

    /*************************/
    /* lane/rate configs     */
    /*************************/
    /* config rx_path_div */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_RX_PATH_DIV_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rx_path_div);
    if (result) return result;

    /* program tx and rx bus widths */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_TXPATH_BUS_WIDTH_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txpath_bus_width);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_RXPATH_BUS_WIDTH_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rxpath_bus_width);
    if (result) return result;

    /* config piso - bypass divider, set divide ratio */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_DIV_ENABLE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, piso_div_enable);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_CTRL_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, piso_ctrl);
    if (result) return result;

    /* program ADSM int, frac and denomin settings */
    /* see rate specific init lookup table for all the different rates */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_10, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_INT_DIV_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_int_div);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_10, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_FRAC_DIV_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_frac_div);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_9, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_9_ADSM_DENOMIN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_denomin);
    if (result) return result;

    /* config csu_ctrl[3:2] */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    reg_msk = (0x0003 << 18);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
    if (result) return result;

    /* config txrxcs_mode[0:1] (may not be necessary for SFP - uses CSU_mode bits for post divider)
       addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
       reg_msk = (0x0003 << 28);
       result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txrxcs_mode);
       if (result) return result;  */

    /* config csu_mode[21:19] */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    reg_msk = (0x0007 << 19);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, csu_mode);
    if (result) return result;

    /* config csu_mode[18:17] */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    reg_msk = (0x0003 << 17);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x02);
    if (result) return result;

    /* config csu_mode[16] */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_7_CSU_MODE_OFF + 16;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, vco_select);
    if (result) return result;

    /* config csu_mode[15:10] - set to default will be calibrated */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    reg_msk = (0x003F << 10);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x2D);
    if (result) return result;

    result = sysHyPhy20gMsDelay (1);
    if (result) return result;

    /* enable ADSM again (may not be necessary as we reset again in cal loop)*/
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    result = sysHyPhy20gMsDelay (1);
    if (result) return result;
    DBG_PRINT("%s, %s, %d, finished SfpRateCfg lane config sequence \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of Lane config */

    /*************************/
    /* CSU calibration       */
    /*************************/
    /* set clock count max */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_12, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_12_ADSM_CLK_CNT_MAX_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_clk_cnt_max);
    if (result) return result;

     /* set lock counter */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_LOCK_CNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 16);
    if (result) return result;

    /* call CSU calibration function */
    result = utilHyPhy20gSfpSerdesCsuCal(fileHndl, linkID, &converge_freq);
    if (result) return result;

    /* set CSU AnalogRSTB */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_CSU_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* reset ADSM */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* update CSU vco center freq with new convergence */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
    reg_msk = (0x003F << 10);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, converge_freq);
    if (result) return result;

    result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_10USEC);
    if (result) return result;

    /* enable ADSM */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    DBG_PRINT("%s, %s, %d, finished SfpRateCfg csu cal routine \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of CSU calibration routine */

    /***************************/
    /* reset enable sequencing */
    /***************************/
    result = sysHyPhy20gMsDelay (1);
    if (result) return result;

    /* check ADSM_LOCK */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_11, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_LOCK_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, addr, reg_msk, &val);
    if (result) return result;
    if (val != 0x1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADSM_LOCK\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_SERDES_ADSM_LOCK;
    }

    /* enable rxpatgen vco */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_CTRL_OFF + 3;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* enable rxpatgen blocks */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* minimum 1us delay */
    result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_5USEC);
    if (result) return result;

    /* enable rxpatgen analog blocks */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* minimum 2us delay */
    result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_5USEC);
    if (result) return result;

    /* enable ADC */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* enable ADC DACs */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_DAC_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    result = sysHyPhy20gMsDelay (1);
    if (result) return result;

    /* enable digital reset */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_DIGITAL_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* enable PISO */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    DBG_PRINT("%s, %s, %d, finished SfpRateCfg reset enable sequence \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of reset enable sequence */

    /* end of CSU config */

    /* check rxcpgen lock status */
    result = utilHyPhy20gSerdesRxcpgen(fileHndl, linkID,
                                       HYPHY20G_SERDES_GRP_SFP);
    if (result) return result;

    /**************/
    /* ACB CONFIG */
    /**************/
    /* ACB config to nominal using ACB util functions   */
    result = hyPhy20gSfpAcbInit(fileHndl, linkID, 1);
    if (result) return result;

    /* config PLL_S_MAX */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_TOP_REG_ACB_FRC_WAVE_AND_CFG0_TO_CFG3, linkID);
    reg_msk = HYPHY20G_SERDES_SFP_TOP_BIT_ACB_FRC_WAVE_AND_CFG0_TO_CFG3_PLL_S_MAX_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, acb_pll_s_max);
    if (result) return result;
    /* end ACB config */

    /* toggle ADC reference to start */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_16, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_REFERENCE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_16, linkID);
    bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_REFERENCE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* defer ADC calibration (calibrate explicitly later) */
    result = utilHyPhy20gSerdesAdcCalibrate(fileHndl, HYPHY20G_SERDES_SFP_INTF,
                                            linkID, 3);
    if (result) return result;

    /* re-center RX FIFO */
    result = utilHyPhy20gSerdesRxFifoReCenter(fileHndl, HYPHY20G_SERDES_SFP_INTF,
                                              linkID);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSfpCustomRateCfg */

/*******************************************************************************
**
**  hyPhy20gXfiCustomRateCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the XFI Serdes subsystem.  
**
**
**  INPUTS:         *fileHndl       - base address
**                  linkID          - 0 - XFI interface 0
**                                    1 - XFI interface 1
**                  busWidth        - 0 - Reserved
**                                    1 - Reserved
**                                    2 - 32 bit
**                                    3 - 40 bit
**                  adsmFracDiv     - 23-bit PLL multiplier nominator
**                  adsmIntDiv      - 8-bit PLL multiplier integer
**                  adsmDenomin     - 23-bit PLL multiplier denominator
**                                    Mult = INT_DIV + (FRAC_DIV + FRAC_DIV_MOD)/DENOMIN
**                  rxPathDiv       - RX PATH decimation factor
**                                    0 - Div 1
**                                    1 - Div 2
**                                    2 - Div 4
**                                    3 - Div 8
**                                    4 - Div 16
**                  pisoDivEnable   - Enable clock divider in the PISO
**                                    0 - Divider bypassed
**                                    1 - Divider enabled
**                  pisoCtrl        - PISO clock divide ratio
**                                    0 - Decimation by 2
**                                    1 - Decimation by 4
**                                    2 - Decimation by 8
**                                    3 - Decimation by 16
**                                    4 - Decimation by 32
**                                    5 - Decimation by 64
**                                    6 - Decimation by 128
**                                    7 - Decimation by 256
**                  postDiv         - Controls the divide ratio of the VCO clock used to generate HRCLK
**                                    0 - Div 2
**                                    1 - Div 2.5
**                                    2 - Div 3
**                  enable          - 1 - enable XFI subsystem
**                                    0 - disable XFI subsystem
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_SERDES_ADSM_LOCK
**                  HYPHY20G_ERR_SERDES_CSU_INACTIVE
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gXfiCustomRateCfg(struct file *fileHndl, UINT4 linkID, UINT4 busWidth,
                              UINT4 adsmFracDiv, UINT4 adsmIntDiv, UINT4 adsmDenomin,
                              UINT4 rxPathDiv, UINT4 pisoDivEnable, UINT4 pisoCtrl,
                              UINT4 postDiv, UINT4 enable)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 reg_msk;
    UINT4 converge_freq;
    UINT4 rxpath_bus_width;
    UINT4 txpath_bus_width;
    UINT4 adsm_clk_cnt_max;
    UINT4 adsm_frac_div;
    UINT4 adsm_int_div;
    UINT4 adsm_denomin;
    UINT4 rx_path_div;
    UINT4 piso_div_enable;
    UINT4 piso_ctrl;
    UINT4 txrxcs_mode;
    UINT4 i;
    UINT4 csu_config;
    UINT4 val;
    UINT4 acb_pll_s_max;

    /* argument checking */
    if (linkID > HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (busWidth < 2 || busWidth > 3) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (adsmFracDiv > 16777215) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (adsmIntDiv > 255) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (adsmDenomin > 16777215) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (rxPathDiv > 4) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pisoDivEnable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (pisoCtrl > 7) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (postDiv > 2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    addr = 0;
    bitoffset = 0;
    reg_msk = 0;
    converge_freq = 0;
    DBG_PRINT("%s, %s, %d, entering XfiRateCfg \n",__FILE__, __FUNCTION__, __LINE__);

    /* initialize rate specific settings */
    rxpath_bus_width = busWidth;
    txpath_bus_width = busWidth;
    adsm_clk_cnt_max = 4000;
    adsm_frac_div = adsmFracDiv;
    adsm_int_div = adsmIntDiv;
    adsm_denomin = adsmDenomin;
    rx_path_div = rxPathDiv;
    piso_div_enable = pisoDivEnable;
    piso_ctrl = pisoCtrl;
    txrxcs_mode = postDiv;
    acb_pll_s_max = 0xc;

    csu_config = 1;
    for (i = 0; i < 3 && csu_config == 1; i++) {
        /********************/
        /* reset sequencing */
        /********************/
        /* hold CSU AnalogRSTB */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold CSU CTRL[11] for feedback divider*/
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_CTRL_OFF + 11;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* hold rxpatgen analog blocks in reset */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* read back VCO setting to check if CSU is configured */
        /* if VCO setting is non-zero, CSU has already been configured once */
        /* if already configured once, the following reset bits should not be disabled on */
        /* subsequent line rate configurations */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_9, linkID);
        reg_msk = (0x003F << 10);
        result = hyPhy20gRegFieldRead(fileHndl, addr, reg_msk, &val);
       	if (result) return result;
        
        if (val == 0x0) { 
          /* hold rxpatgen vco = ctrl[3] in reset */
          addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
          bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_CTRL_OFF + 3;
         	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
         	if (result) return result;
  
          /* hold ADC DACs in reset */
          addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_3, linkID);
          bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_3_ADC_DAC_RSTB_OFF;
        	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
         	if (result) return result;
  
          /* hold rxpatgen blocks reset */
          addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
          bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_RSTB_OFF;
         	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
         	if (result) return result;
  
          /* hold block in digital reset */
          addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
          bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_DIGITAL_RSTB_OFF;
         	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
         	if (result) return result;
  
          /* hold ADC in reset */
          addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_3, linkID);
          bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_3_ADC_RSTB_OFF;
         	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
         	if (result) return result;
        } else { 	  
          /* hold block in digital reset */
          addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
          bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_DIGITAL_RSTB_OFF;
         	result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
         	if (result) return result;
        }	  

        /* hold PISO in reset */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* hold ADSM in reset */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* enable CSU (default) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* turn off CSU AnalogTM (default) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_ATMSB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished XfiRateCfg reset sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of reset sequence */

        if (enable == 0) {
            DBG_PRINT("%s, %s, %d, enable is 0, XfiRateCfg link held in reset\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_SUCCESS;
        }

        /********************/
        /* default settings */
        /********************/
        /* program ADC MODE selection (hardcoded now, should call utilADCModeSel ) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_20, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_20_ACCUM_REF_SEL_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x02);
        if (result) return result;

        /* program non-linear map (default, hardcoded now, should call utilADCModeSel) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_1, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_1_NON_LINEAR_MAP_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* program equalizer tap (hardcoded now, should be set in fw ) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_2, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_2_TIMING_EQ_TAP_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
        if (result) return result;

        /* program equalizer loop filter coeff (hardcoded now, should be set in fw ) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_3, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_3_COEFF_D_TEP_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x0A);
        if (result) return result;

        /* program equalizer tap 3 (hardcoded now, should be set in fw ) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_4, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_EQ_TAP_3_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
        if (result) return result;

        /* program equalizer tap 2 (hardcoded now, should be set in fw ) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_4, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_EQ_TAP_2_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
        if (result) return result;

        /* program equalizer tap 1 (hardcoded now, should be set in fw ) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_4, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_EQ_TAP_1_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
        if (result) return result;

        /* program equalizer threshold clip detector (hardcoded now, should be set in fw ) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_4, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_TH_CLIP_DETECT_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x12);
        if (result) return result;

        /* program equalizer threshold clip detector (hardcoded now, should be set in fw ) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_4, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_LOS_ENABLE_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
        if (result) return result;


        /* disable ADSM seed */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_13, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_SEED_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable OC_ENB (default) */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_OC_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* program OC_INTERVAL to 0xFFFF - required non-default setting */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_8, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_8_OC_INTERVAL_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0xFFFF);

        /* set CSU_CTRL[8:6] bit */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        reg_msk = (0x0007 << 18);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x07);
        if (result) return result;

        /* set CSU_CTRL[14:13] bit */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        reg_msk = (0x0003 << 25);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
        if (result) return result;

        /* set CSU_MODE[7:0] bits */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_9, linkID);
        reg_msk = (0x0007F << 0);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x07);
        if (result) return result;
          
        /* set CSU_MODE[31:27] bits */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_9, linkID);
        reg_msk = (0x0001F << 27);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x08);
       	if (result) return result;

        /* end of default settings */

        /*************************/
        /* lane/rate configs     */
        /*************************/

        /* config rx_path_div */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_RX_PATH_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rx_path_div);
        if (result) return result;

        /* program tx and rx bus widths */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_TXPATH_BUS_WIDTH_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txpath_bus_width);
        if (result) return result;

        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_RXPATH_BUS_WIDTH_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, rxpath_bus_width);
        if (result) return result;

        /* config piso - bypass divider, set divide ratio */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_DIV_ENABLE_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, piso_div_enable);
        if (result) return result;

        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_CTRL_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, piso_ctrl);
        if (result) return result;

        /* program ADSM int, frac and denomin settings */
        /* see rate specific init lookup table for all the different rates */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_12, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_12_ADSM_INT_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_int_div);
        if (result) return result;

        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_12, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_12_ADSM_FRAC_DIV_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_frac_div);
        if (result) return result;

        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_11, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_11_ADSM_DENOMIN_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_denomin);
        if (result) return result;

        /* config csu_ctrl[3:2] */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        reg_msk = (0x0003 << 14);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x01);
        if (result) return result;

        /* config txrxcs_mode[5:4] */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_1, linkID);
        reg_msk = (0x0003 << 20);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, txrxcs_mode);
        if (result) return result;

        /* config csu_mode[21:19] (may not be necessary for XFI - uses txrxcs_mode bits for post divider)
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_9, linkID);
        reg_msk = (0x0007 << 19);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, csu_mode);
        if (result) return result;      */

        /* config csu_mode[15:10] - set to default will be calibrated */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_9, linkID);
        reg_msk = (0x003F << 10);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x26);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* enable ADSM again (may not be necessary as we reset again in cal loop)*/
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;
        DBG_PRINT("%s, %s, %d, finished XfiRateCfg lane config sequence \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of Lane config */

        /*************************/
        /* CSU calibration       */
        /*************************/
        /* program ADSM clk_cnt_max settings */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_14, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_14_ADSM_CLK_CNT_MAX_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, adsm_clk_cnt_max);
        if (result) return result;

        /* set lock counter */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_LOCK_CNT_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0x10);
        if (result) return result;

        /* call XFI CSU calibration function */
        result = utilHyPhy20gXfiSerdesCsuCal(fileHndl, linkID, &converge_freq);
        if (result) return result;

        /* set CSU AnalogRSTB */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* hold ADSM in reset */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* update CSU vco center freq with new convergence */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_9, linkID);
        reg_msk = (0x003F << 10);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, converge_freq);
        if (result) return result;

        /* enable ADSM */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        DBG_PRINT("%s, %s, %d, finished XfiRateCfg csu cal routine \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of CSU calibration routine */

        /***************************/
        /* reset enable sequencing */
        /***************************/
        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* check ADSM_LOCK */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_13, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_LOCK_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, addr, reg_msk, &val);
        if (result) return result;
        if (val != 0x1) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADSM_LOCK\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_SERDES_ADSM_LOCK;
        }

        /* enable rxpatgen vco = ctrl[3] */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable rxpatgen blocks */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_5USEC);
        if (result) return result;

        /* enable rxpatgen analog blocks in reset */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_5USEC);
        if (result) return result;

        /* enable ADC */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_3_ADC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable ADC DACs */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_3_ADC_DAC_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* enable digital reset */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_DIGITAL_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* enable PISO */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /**********************/
        /* check CSU activity */
        /**********************/
        /* toggle CSU_ACTIVE_DTCT_RSTB */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CSU_ACTIVE_DTCT_RST, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_TOP_BIT_CSU_ACTIVE_DTCT_RST_CSU_ACTIVE_DTCT_RSTB_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 0);
        if (result) return result;

        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, 1);
        if (result) return result;

        /* wait a minimum of 1/566MHz */
        result = sysHyPhy20gUsDelay (1);
        if (result) return result;

        /* read CSU_ACTIVE_DTCT */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_CSU_ACTIVE_DTCT, linkID);
        reg_msk = HYPHY20G_SERDES_XFI_TOP_BIT_CSU_ACTIVE_DTCT_CSU_ACTIVE_DTCT_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, addr, reg_msk, &val);
        if (result) return result;
        /* end check CSU activity */

        /* re-configure CSU if there is no clock activity */
        csu_config = (val == 1) ? 0 : 1;
    }
    if (csu_config == 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_CSU_INACTIVE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_SERDES_CSU_INACTIVE;
    }

    DBG_PRINT("%s, %s, %d, finished XfiRateCfg reset enable sequence \n",__FILE__, __FUNCTION__, __LINE__);
    /* end of reset enable sequence */

    /* CSU Config complete */

    /* check rxcpgen lock status */
    result = utilHyPhy20gSerdesRxcpgen(fileHndl, linkID,
                                       HYPHY20G_SERDES_GRP_XFI);
    if (result) return result;

    /**************/
    /* ACB CONFIG */
    /**************/
    /* ACB config to nominal using ACB util functions */
    result = hyPhy20gXfiAcbInit(fileHndl, linkID, 1);
    if (result) return result;

    /* config PLL_S_MAX */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_TOP_REG_ACB_CTL, linkID);
    reg_msk = HYPHY20G_SERDES_XFI_TOP_BIT_ACB_CTL_PLL_S_MAX_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, acb_pll_s_max);
    if (result) return result;
    /* end ACB config */

    /* toggle ADC reference to start */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_REFERENCE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_REFERENCE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;

    /* re-center RX FIFO */
    result = utilHyPhy20gSerdesRxFifoReCenter(fileHndl, HYPHY20G_SERDES_XFI_INTF,
                                              linkID);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gXfiCustomRateCfg */

/*******************************************************************************
**
**  hyPhy20gSerdesADCCalibrate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs ADC offset calibration and mode
**                  selection across multiple SERDES interfaces and links.
**                  ADC offset calibration should only be performed on links
**                  with floating inputs or with active receive data traffic.
**
**                  *RateCfg functions of the corresponding SERDES interfaces
**                  for links to be calibrated must be called prior to this
**                  function.
**
**                  For SFP interfaces, calibration should NOT be performed with
**                  rates lower than 1.5Gbps.  Calibrating links with rates
**                  lower than 1.5Gbps will result in the error
**                  HYPHY20G_ERR_SERDES_ADC_SFP_RATE_CHECK.  Mode selection is
**                  not valid on SFP interfaces and thus will not be performed.
**
**                  When performing ADC offset calibration on a PIF link,
**                  data integrity on all PIF links will be compromised.
**
**                  When performing ADC offset calibration on a Backplane link,
**                  data integrity on all links sharing the same CSU will be
**                  compromised.
**
**                  Calibration results are return by pSfpCalFail, pXfiCalFail,
**                  pBpCalFail and pPifCalFail.
**                  Values:
**                      0 - ADC calibration successful
**                      1 - ADC calibration failure
**
**
**  INPUTS:         *fileHndl    - base address
**                  sfpMsk       - Mask of SFP links to calibrate
**                                     bit 0 : SFP #0
**                                     bit 1 : SFP #1
**                                     ..
**                                     bit 15: SFP #15
**                  xfiMsk       - Mask of XFI links to calibrate
**                                     bit 0 : XFI #0
**                                     bit 1 : XFI #1
**                  bpMsk        - Mask of backplane links to calibrate
**                                     bit 0 : link #0
**                                     bit 1 : link #1
**                                     ..
**                                     bit 31: link #31
**                  pifMsk       - Mask of PIF links to calibrate
**                                     bit 0 : link #0
**                                     bit 1 : link #1
**                                     ..
**                                     bit 7 : link #7
**                  eqMode       - 0 - do not perform mode selection and
**                                     equalization
**                                 1 - perform mode selection and FW
**                                     equalization
**                  *pSfpCalFail - Pointer to storage for SFP calibration
**                                 failures
**                                     bit 0 : SFP #0
**                                     bit 1 : SFP #1
**                                     ..
**                                     bit 15: SFP #15
**                  *pXfiCalFail - Pointer to storage for XFI calibration
**                                 failures
**                                     bit 0 : XFI #0
**                                     bit 1 : XFI #1
**                  *pBpCalFail  - Pointer to storage for Backplane calibration
**                                 failures
**                                     bit 0 : link #0
**                                     bit 1 : link #1
**                                     ..
**                                     bit 31: link #31
**                  *pPifCalFail - Pointer to storage for PIF calibration
**                                 failures
**                                     bit 0 : link #0
**                                     bit 1 : link #1
**                                     ..
**                                     bit 7 : link #7
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SERDES_ADC_SFP_RATE_CHECK
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSerdesADCCalibrate(struct file *fileHndl, UINT4 sfpMsk,
                                UINT4 xfiMsk, UINT4 bpMsk, UINT4 pifMsk,
                                UINT4 eqMode, UINT4 *pSfpCalFail,
                                UINT4 *pXfiCalFail, UINT4 *pBpCalFail,
                                UINT4 *pPifCalFail)
{
    UINT4 intf;
    UINT4 maxLinks, link;
    UINT4 cfgMsk;
    UINT4 calCount = 0;
    UINT4 devValid, devCalMode, devCalReq, devCalOk;
    UINT4 devPisoEnb, devAdsmIntDiv;
    UINT4 mskLink, continueCal;
    UINT4 fwIntf = 0;
    UINT4 fwEqIteration = 0;
    UINT4 dummy = 0;
    UINT4 *pCalFail;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((sfpMsk > 0xFFFF) || (xfiMsk > 0x3) || (bpMsk > 0xFFFFFFFF) ||
        (pifMsk > 0xFF) || (eqMode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure calibrate request flag */
    for (intf = 0; intf < 4; intf++) {
        result = utilHyPhy20gSerdesGetMaxLinks(fileHndl, intf, &maxLinks);
        if (result) return result;

        switch (intf) {
            case HYPHY20G_SERDES_SFP_INTF:
                /* SFP */
                cfgMsk = sfpMsk;
                break;
            case HYPHY20G_SERDES_XFI_INTF:
                /* XFI */
                cfgMsk = xfiMsk;
                break;
            case HYPHY20G_SERDES_BACKPLANE_INTF:
                /* Backplane */
                cfgMsk = bpMsk;
                break;
            case HYPHY20G_SERDES_PIF_INTF:
                /* PIF */
                cfgMsk = pifMsk;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        if (cfgMsk != 0) {
            for (link = 0; link < maxLinks; link++) {
                mskLink = (cfgMsk >> link) & 0x1;
                if (mskLink == 0x1) {
                    /* check state of calMode - should be invalid, dynamic or
                       deferred */
                    result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl, intf,
                                                               link,
                                                               &devValid,
                                                               &devCalMode,
                                                               &devCalReq,
                                                               &devCalOk,
                                                               &devPisoEnb,
                                                               &devAdsmIntDiv);
                    if (result) return result;

                    if (devCalMode != HYPHY20G_SERDES_DYNAMIC_CAL_MODE
                        && devCalMode != HYPHY20G_SERDES_DEFER_CAL_MODE) {
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
                        return HYPHY20G_ERR_INVALID_PRECONFIG;
                    }

                    /* set calibration request flag */
                    result = serdesHyPhy20gSerdesSetMskAdcCalCtxt(fileHndl,
                                                                  intf, link,
                                                                  dummy, dummy,
                                                                  1, dummy,
                                                                  dummy, dummy,
                                                                  0x4);
                    if (result) return result;
                }
            }
        }
    }

    /* Validate SFP rates */
    if (sfpMsk != 0) {
        result = utilHyPhy20gSerdesValidateSfpRates(fileHndl, sfpMsk);
        if (result) return result;
    }

    /* Prepare links for calibration */
    result = utilHyPhy20gSerdesIntfMskPrepCalibration(fileHndl, sfpMsk, xfiMsk,
                                                      bpMsk, pifMsk);
    if (result) return result;

    /* Wait >= 5us */
    result = sysHyPhy20gUsDelay(5);
    if (result) return result;

    /* First pass at calibration */
    result = utilHyPhy20gSerdesAdcSingleRun(fileHndl, sfpMsk, xfiMsk, bpMsk,
                                            pifMsk);
    if (result) return result;

    /* Check results */
    result = utilHyPhy20gSerdesIntfMskCheckAdc(fileHndl, sfpMsk, calCount,
                                               xfiMsk, calCount, bpMsk,
                                               calCount, pifMsk, calCount);
    if (result) return result;

    /* Count how many times we have calibrated */
    calCount++;

    do {
        /* Run calibration again */
        result = utilHyPhy20gSerdesAdcSingleRun(fileHndl, sfpMsk, xfiMsk, bpMsk,
                                                pifMsk);
        if (result) return result;

        /* Check results */
        result = utilHyPhy20gSerdesIntfMskCheckAdc(fileHndl, sfpMsk, calCount,
                                                   xfiMsk, calCount, bpMsk,
                                                   calCount, pifMsk, calCount);
        if (result) return result;

        /* Increment count */
        calCount++;

        /* determine if there are any links left to calibrate */
        continueCal = 0;
        for (intf = 0; intf < 4; intf++) {
            result = utilHyPhy20gSerdesGetMaxLinks(fileHndl, intf, &maxLinks);
            if (result) return result;

            switch (intf) {
                case HYPHY20G_SERDES_SFP_INTF:
                    /* SFP */
                    cfgMsk = sfpMsk;
                    break;
                case HYPHY20G_SERDES_XFI_INTF:
                    /* XFI */
                    cfgMsk = xfiMsk;
                    break;
                case HYPHY20G_SERDES_BACKPLANE_INTF:
                    /* Backplane */
                    cfgMsk = bpMsk;
                    break;
                case HYPHY20G_SERDES_PIF_INTF:
                    /* PIF */
                    cfgMsk = pifMsk;
                    break;
                default:
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_INVALID_ARG;
            }

            if (cfgMsk != 0) {
                for (link = 0; link < maxLinks; link++) {
                    mskLink = (cfgMsk >> link) & 0x1;
                    if (mskLink == 1) {
                        result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl,
                                                                   intf, link,
                                                                   &devValid,
                                                                   &devCalMode,
                                                                   &devCalReq,
                                                                   &devCalOk,
                                                                   &devPisoEnb,
                                                                   &devAdsmIntDiv);
                        if (result) return result;

                        if (devCalReq == 1 && devCalOk == 0) {
                            continueCal = 1;
                        }
                    }
                }
            }
        }
    } while (calCount < HYPHY20G_SERDES_ADC_CAL_COUNT_MAX && continueCal == 1);

    /* Check calibration results */
    for (intf = 0; intf < 4; intf++) {
        result = utilHyPhy20gSerdesGetMaxLinks(fileHndl, intf, &maxLinks);
        if (result) return result;

        switch (intf) {
            case HYPHY20G_SERDES_SFP_INTF:
                /* SFP */
                cfgMsk = sfpMsk;
                pCalFail = pSfpCalFail;
                break;
            case HYPHY20G_SERDES_XFI_INTF:
                /* XFI */
                cfgMsk = xfiMsk;
                pCalFail = pXfiCalFail;
                break;
            case HYPHY20G_SERDES_BACKPLANE_INTF:
                /* Backplane */
                cfgMsk = bpMsk;
                pCalFail = pBpCalFail;
                break;
            case HYPHY20G_SERDES_PIF_INTF:
                /* PIF */
                cfgMsk = pifMsk;
                pCalFail = pPifCalFail;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        *pCalFail = 0;
        for (link = 0; link < maxLinks; link++) {
            mskLink = (cfgMsk >> link) & 0x1;
            if (mskLink == 0x1) {
                /* check calOk flag */
                result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl, intf,
                                                           link, &devValid,
                                                           &devCalMode, &devCalReq,
                                                           &devCalOk, &devPisoEnb,
                                                           &devAdsmIntDiv);
                if (result) return result;

                if (devCalReq != 1) {
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_INVALID_PRECONFIG;
                }
                if (devCalOk == 0) {
                    /* calibration failed */
                    *pCalFail = *pCalFail | (0x1 << link);
                }
            }
        }
    }

    /* Restore */
    result = utilHyPhy20gSerdesIntfMskRestore(fileHndl, sfpMsk, xfiMsk, bpMsk,
                                              pifMsk);
    if (result) return result;

    /* Wait >= 5us */
    result = sysHyPhy20gUsDelay(5);
    if (result) return result;

    /* Re-center RX FIFO */
    for (intf = 0; intf < 4; intf++) {
        result = utilHyPhy20gSerdesGetMaxLinks(fileHndl, intf, &maxLinks);
        if (result) return result;

        switch (intf) {
            case HYPHY20G_SERDES_SFP_INTF:
                /* SFP */
                cfgMsk = sfpMsk;
                pCalFail = pSfpCalFail;
                break;
            case HYPHY20G_SERDES_XFI_INTF:
                /* XFI */
                cfgMsk = xfiMsk;
                pCalFail = pXfiCalFail;
                break;
            case HYPHY20G_SERDES_BACKPLANE_INTF:
                /* Backplane */
                cfgMsk = bpMsk;
                pCalFail = pBpCalFail;
                break;
            case HYPHY20G_SERDES_PIF_INTF:
                /* PIF */
                cfgMsk = pifMsk;
                pCalFail = pPifCalFail;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        for (link = 0; link < maxLinks; link++) {
            mskLink = (cfgMsk >> link) & 0x1;
            if (mskLink == 0x1) {
                result = utilHyPhy20gSerdesRxFifoReCenter(fileHndl, intf, link);
                if (result) return result;
            }
        }
    }

    /* Check calibration results and run mode selection and equalization if
       calibration is successful.  Mode selection and equalization are not
       applicable for SFP. */
    for (intf = 0; intf < 4; intf++) {
        if (intf != HYPHY20G_SERDES_SFP_INTF) {

            result = utilHyPhy20gSerdesGetMaxLinks(fileHndl, intf, &maxLinks);
            if (result) return result;

            switch (intf) {
                case HYPHY20G_SERDES_XFI_INTF:
                    /* XFI */
                    cfgMsk = xfiMsk;
                    pCalFail = pXfiCalFail;
                    fwIntf = 0;
                    fwEqIteration = 100;
                    break;
                case HYPHY20G_SERDES_BACKPLANE_INTF:
                    /* Backplane */
                    cfgMsk = bpMsk;
                    pCalFail = pBpCalFail;
                    fwIntf = 3;
                    fwEqIteration = 150;
                    break;
                case HYPHY20G_SERDES_PIF_INTF:
                    /* PIF */
                    cfgMsk = pifMsk;
                    pCalFail = pPifCalFail;
                    fwIntf = 1;
                    fwEqIteration = 150;
                    break;
                default:
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_INVALID_ARG;
            }

            for (link = 0; link < maxLinks; link++) {
                mskLink = (cfgMsk >> link) & 0x1;
                if (mskLink == 0x1) {
                    /* check calOk flag */
                    result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl, intf,
                                                               link, &devValid,
                                                               &devCalMode,
                                                               &devCalReq,
                                                               &devCalOk,
                                                               &devPisoEnb,
                                                               &devAdsmIntDiv);
                    if (result) return result;

                    if (devCalOk == 1 && eqMode == 1) {
                        /* run ADC mode selection and FW equalization */
                        result = utilHyPhy20gAdcModeSelect(fileHndl, intf, link,
                                                           eqMode);
                        if (result) return result;

                        result = utilHyPhy20gSerdesEqCfg(fileHndl, intf, link,
                                                         eqMode);
                        if (result) return result;
                    }
                }
            }
        }
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSerdesADCCalibrate */

/*******************************************************************************
**
**  hyPhy20gSerdesLoadFactoryCal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function loads the factory calibrated ADC offsets from
**                  a text file into SW context memory.  ALL links on an
**                  interface must be initialized with *SerdesInit before this
**                  function can be called.
**
**                  Text file format:
**                  1) Offset, mode select and EQ values for ALL links on an
**                     interface must be included in the text file.
**                  2) Values must be represented in hexidecimal starting with
**                     0x or 0X.
**                  3) ADC offset values must be in the range of 0x0 to 0xFF.
**                  4) ADC offset values must be padded out by zeroes to
**                     represent 1 byte (ie. 6 is represented by 0x06).
**                  5) Mode select values:
**                         - NON_LINEAR_MAP value must be in the range of
**                           0x0 to 0x1.
**                         - ACCUM_REF0 value must be in the range of
**                           0x00000000 to 0xFFFFFFFF, pad out values to 32-bit
**                  6) Equalization values:
**                         - EQ_TAP_1, EQ_TAP_2, EQ_TAP_3 and TIMING_EQ_TAP
**                           must be in the range of 0x0 to 0xF.
**                  7) Comments start with #.
**                  8) A carriage return must immediately follow an offset
**                     value.
**                  9) Order of offset values per link [phase = 0 to p,
**                     comp = 0 to c] are as follows:
**                            [phase=0, comp=0]
**                            [phase=0, comp=1]
**                             ...
**                            [phase=0, comp=c-1]
**                            [phase=0, comp=c]
**                            [phase=1, comp=0]
**                             ...
**                             ...
**                            [phase=p, comp=c-1]
**                            [phase=p, comp=c]
**
**                     where: SFP       - p = 11, c = 0
**                            XFI       - p = 5,  c = 8
**                            Backplane - p = 2,  c = 8
**                            PIF       - p = 2,  c = 8
**
**                 10) Text file order
**                         i) Offset values
**                        ii) Mode selection values
**                       iii) Equalization values
**
**  INPUTS:         *fileHndl - base address
**                  intf      - SERDES interface
**                                  0 - SFP
**                                  1 - XFI
**                                  2 - Backplane
**                                  3 - PIF
**                  *fread    - path to factory calibration file
**
**  NOTE:
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_EQ
**                  HYPHY20G_ERR_SERDES_ADC_FACTORY_LOAD
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSerdesLoadFactoryCal(struct file *fileHndl, UINT4 intf,
                                  char *fread)
{
    struct file *f = NULL;
    UINT4 bEof;
    UINT4 length_read;
    UINT4 length;
    UINT4 nPhases;
    UINT4 nComps;
    UINT4 phase;
    UINT4 comp;
    UINT4 linkID;
    UINT4 maxLinkID;
    UINT4 offset;
    UINT4 endOfLine;
    UINT4 rdNumBytes;
    UINT4 ldOffsetsDone;
    UINT4 nonLinMap;
    UINT4 accumRef0;
    UINT4 ldModeSelDone;
    UINT4 tap;
    UINT4 ldEqDone;
    UINT4 modeSelEqLinkID = 0;
    UINT4 count = 0;
    UINT4 i;
    UINT8 filePosOffset;
    char buffer[11];
    char hexStr[11];
    INT4 result;

    /* argument checking */
    if (intf > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize variables */
    rdNumBytes = 5; /* read 5 bytes at a time first */
    bEof = 0;
    filePosOffset = 0;
    length = 0;
    phase = 0;
    comp = 0;
    linkID = 0;
    ldOffsetsDone = 0;
    ldModeSelDone = 0;
    ldEqDone = 0;
    result = utilHyPhy20gSerdesGetMaxLinks(fileHndl, intf, &maxLinkID);
    if (result) return result;

    /* retrieve nPhase and nComp based on interface type */
    result = utilHyPhy20gSerdesGetMaxNPhaseNComp(fileHndl, intf, &nPhases,
                                                 &nComps);
    if (result) return result;

    /* Open the file for reading */
    f = sysHyPhy20gFileOpen(fread);
    if (f == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* read contents from file */
    while (bEof == 0) {
        /* read rdNumOffsetBytes from file */
        result = sysHyPhy20gFileReadBytes(f, buffer, rdNumBytes,
                                          &filePosOffset, &length_read);
        if (result) {
            sysHyPhy20gFileClose(f);
            return result;
        }

        /* check for end of file */
        bEof = (length_read == 0) ? 1 : 0;

        if (bEof == 0) {
            /* increment file pointer position */
            length = length + length_read;
            filePosOffset = length;

            /* determine if line is a comment, if so ignore full line */
            if (buffer[0] == '#') {
                endOfLine = 0;
                while (endOfLine == 0 && bEof == 0) {
                    /* read one byte at a time until a carriage return is found */
                    result = sysHyPhy20gFileReadBytes(f, buffer, 1,
                                                      &filePosOffset,
                                                      &length_read);
                    if (result) {
                        sysHyPhy20gFileClose(f);
                        return result;
                    }
                    bEof = (length_read == 0) ? 1 : 0;

                    if (bEof == 0) {
                        /* increment file pointer position */
                        length = length + length_read;
                        filePosOffset = length;
                        buffer[length_read] = '\0';

                        if (buffer[0] == '\n') {
                            endOfLine = 1;
                        }

                        DBG_PRINT("JC_DEBUG: comment line, read_length = %ld, buffer = %s\n", length_read, buffer);
                    }
                }

            /* load offset values (# of items = nPhases * nComps)
            **   [phase0, comp0]
            **   [phase0, comp1]
            **   ...
            */
            } else if ((ldOffsetsDone == 0) && (length_read == 5) &&
                       (buffer[0] == '0') &&
                       (buffer[1] == 'x' || buffer[1] == 'X') &&
                       (buffer[4] == '\n')) {

                buffer[length_read-1] = '\0';
                DBG_PRINT("JC_DEBUG LOAD OFFSETS: read file, read_length = %ld, buffer = %s\n", length_read, buffer);

                hexStr[0] = buffer[2];
                hexStr[1] = buffer[3];
                hexStr[2] = '\0';
                result = utilHyPhy20gHexStrToByte(hexStr, 2, &offset);
                if (result) {
                    sysHyPhy20gFileClose(f);
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS;
                }
                DBG_PRINT("JC_DEBUG: linkID = %ld, phase = %ld, comp = %ld, offset = 0x%lx (%lu)\n", linkID, phase, comp, offset, offset);

                /* check read value and other parameters */
                if (offset > 255 || linkID >= maxLinkID || phase >= nPhases ||
                    comp >= nComps) {

                    /* invalid values detected, reset SW context */
                    for (i = 0; i < maxLinkID; i++) {
                        result = serdesHyPhy20gSerdesSetMskAdcCalCtxt(
                                   fileHndl, intf, i, 0,
                                   HYPHY20G_INVALID_VAL_32BIT, 0, 0, 0, 0, 0x2);
                        if (result) {
                            sysHyPhy20gFileClose(f);
                            return result;
                        }

                        for (phase = 0; phase < nPhases; phase++) {
                            for (comp = 0; comp < nComps; comp++) {
                                /* set offset to 0 */
                                result = serdesHyPhy20gSerdesSetAdcCalOffsetCtxt(
                                           fileHndl, intf, i, phase, comp,
                                           0);
                                if (result) {
                                    sysHyPhy20gFileClose(f);
                                    return result;
                                }
                            }
                        }
                    }

                    sysHyPhy20gFileClose(f);
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS;
                }

                /* store offset into SW context */
                result = serdesHyPhy20gSerdesSetAdcCalOffsetCtxt(fileHndl, intf,
                                                                 linkID, phase,
                                                                 comp, offset);
                if (result) {
                    sysHyPhy20gFileClose(f);
                    return result;
                }

                if ((phase == (nPhases-1) && comp == (nComps-1))) {
                    /* prepare to load mode select parameters */
                    ldOffsetsDone = 1;
                    rdNumBytes = 4;
                    count = 0;
                    modeSelEqLinkID = linkID;

                    linkID++;
                    phase = 0;
                    comp = 0;
                } else if (comp == (nComps-1)) {
                    phase++;
                    comp = 0;
                } else {
                    comp++;
                }

            /* load mode selection values (# of items = 2)
            **   NON_LINEAR_MAP
            **   ACCUM_REF0
            */
            } else if (ldOffsetsDone == 1 && ldModeSelDone == 0) {
                DBG_PRINT("JC_DEBUG LOAD MODE SEL:\n");

                if ((count == 0) && (length_read == 4) && (buffer[0] == '0') &&
                    (buffer[1] == 'x' || buffer[1] == 'X') &&
                    (buffer[3] == '\n')) {
                    DBG_PRINT("JC_DEBUG: loading NON_LINEAR_MAP\n");

                    hexStr[0] = buffer[2];
                    hexStr[1] = '\0';
                    result = utilHyPhy20gHexStrToByte(hexStr, 1, &nonLinMap);
                    if (result) {
                        sysHyPhy20gFileClose(f);
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL\n",__FILE__, __FUNCTION__, __LINE__);
                        return HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL;
                    }

                    DBG_PRINT("JC_DEBUG: linkID = %ld, non_linear_map = 0x%lx(%lu)\n", modeSelEqLinkID, nonLinMap, nonLinMap);

                    /* check read value and other parameters */
                    if (nonLinMap > 1) {
                        sysHyPhy20gFileClose(f);
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL\n",__FILE__, __FUNCTION__, __LINE__);
                        return HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL;
                    }

                    /* store value into software context */
                    result = serdesHyPhy20gSerdesSetMskModeSelCtxt(fileHndl,
                                        intf, modeSelEqLinkID, 0, nonLinMap,
                                        0, 0x2);
                    if (result) {
                        sysHyPhy20gFileClose(f);
                        return result;
                    }

                    rdNumBytes = 11;
                    count++;

                } else if ((count == 1) && (length_read == 11) &&
                           (buffer[0] == '0') &&
                           (buffer[1] == 'x' || buffer[1] == 'X') &&
                           (buffer[10] == '\n')) {
                    DBG_PRINT("JC_DEBUG: loading ACCUM_REF0\n");

                    for (i = 0; i < 8; i++) {
                        hexStr[i] = buffer[i+2];
                    }
                    hexStr[8] = '\0';
                    result = utilHyPhy20gHexStrToByte(hexStr, 8, &accumRef0);
                    if (result) {
                        sysHyPhy20gFileClose(f);
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL\n",__FILE__, __FUNCTION__, __LINE__);
                        return HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL;
                    }

                    DBG_PRINT("JC_DEBUG: linkID = %ld, accumRef0 = 0x%lx (%lu)\n", modeSelEqLinkID, accumRef0, accumRef0);

                    /* check read value and other parameters */
                    if (accumRef0 > 0xFFFFFFFF) {
                        sysHyPhy20gFileClose(f);
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL\n",__FILE__, __FUNCTION__, __LINE__);
                        return HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL;
                    }

                    /* store value into software context */
                    result = serdesHyPhy20gSerdesSetMskModeSelCtxt(fileHndl,
                                   intf, modeSelEqLinkID, 0, 0, accumRef0, 0x4);
                    if (result) {
                        sysHyPhy20gFileClose(f);
                        return result;
                    }

                    rdNumBytes = 4;
                    ldModeSelDone = 1;
                    count = 0;
                }

            /* load equalization values  (# of items = 4)
            **   EQ_TAP_1
            **   EQ_TAP_2
            **   EQ_TAP_3
            **   TIMING_EQ_TAP
            */
            } else if (ldOffsetsDone == 1 && ldModeSelDone == 1 && ldEqDone == 0) {
                DBG_PRINT("JC_DEBUG EQUALIZATION:\n");

                if ((count < 4) && (length_read == 4) && (buffer[0] == '0') &&
                    (buffer[1] == 'x' || buffer[1] == 'X') &&
                    (buffer[3] == '\n')) {
                    DBG_PRINT("JC_DEBUG: loading EQ_TAP_%lu\n", count);

                    hexStr[0] = buffer[2];
                    hexStr[1] = '\0';
                    result = utilHyPhy20gHexStrToByte(hexStr, 1, &tap);
                    if (result) {
                        sysHyPhy20gFileClose(f);
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_INVALID_EQ\n",__FILE__, __FUNCTION__, __LINE__);
                        return HYPHY20G_ERR_SERDES_ADC_INVALID_EQ;
                    }

                    DBG_PRINT("JC_DEBUG: linkID = %ld, tap[%lu] = 0x%lx (%ld)\n", modeSelEqLinkID, count, tap, tap);

                    /* check read value and other parameters */
                    if (tap > 0xF) {
                        sysHyPhy20gFileClose(f);
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_INVALID_EQ\n",__FILE__, __FUNCTION__, __LINE__);
                        return HYPHY20G_ERR_SERDES_ADC_INVALID_EQ;
                    }

                    /* store value into software context */
                    switch (count) {
                        case 0:
                            result = serdesHyPhy20gSerdesSetMskEqCtxt(fileHndl,
                                   intf, modeSelEqLinkID, 0, tap, 0, 0, 0, 0x2);
                            break;
                        case 1:
                            result = serdesHyPhy20gSerdesSetMskEqCtxt(fileHndl,
                                   intf, modeSelEqLinkID, 0, 0, tap, 0, 0, 0x4);
                            break;
                        case 2:
                            result = serdesHyPhy20gSerdesSetMskEqCtxt(fileHndl,
                                   intf, modeSelEqLinkID, 0, 0, 0, tap, 0, 0x8);
                            break;
                        case 3:
                            result = serdesHyPhy20gSerdesSetMskEqCtxt(fileHndl,
                                  intf, modeSelEqLinkID, 0, 0, 0, 0, tap, 0x10);
                            break;
                        default:
                            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_INVALID_EQ\n",__FILE__, __FUNCTION__, __LINE__);
                            return HYPHY20G_ERR_SERDES_ADC_INVALID_EQ;
                    }
                    if (result) {
                        sysHyPhy20gFileClose(f);
                        return result;
                    }

                    count++;

                    if (count == 4) {
                        ldEqDone = 1;
                    }
                }

            } else {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_FACTORY_LOAD\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_SERDES_ADC_FACTORY_LOAD;
            }
        }

        if (ldOffsetsDone == 1 && ldModeSelDone == 1 && ldEqDone == 1) {
            /* reset for next link */
            DBG_PRINT("Reset for next link\n");
            ldOffsetsDone = 0;
            ldModeSelDone = 0;
            ldEqDone = 0;
            rdNumBytes = 5;
        }

    } /* end of while */

    if (linkID < maxLinkID || phase != 0 || comp != 0) {
        DBG_PRINT("JC_DEBUG: linkID = %ld, phase = %ld, comp = %ld\n", linkID, phase, comp);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_FACTORY_LOAD\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_SERDES_ADC_FACTORY_LOAD;
    }

    /* configure SW context after successful load */
    for (i = 0; i < maxLinkID; i++) {
        result = serdesHyPhy20gSerdesSetMskAdcCalCtxt(fileHndl, intf, i, 0,
                             HYPHY20G_SERDES_FACTORY_CAL_MODE, 0, 0, 0, 0, 0x2);
        if (result) return result;
    }

    /* close the file */
    sysHyPhy20gFileClose(f);

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSerdesLoadFactoryCal */

/*******************************************************************************
**
**  hyPhy20gSerdesDbgPrintCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Converts two char hex sequence into a byte
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 0 - SFP
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkID   - SERDES link
**                                 SFP       - 0 to 15
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**
**  OUTPUTS:        None     - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSerdesDbgPrintCtxt(struct file *fileHndl, UINT4 intf,
                                UINT4 linkID)
{
    UINT4 nPhases, nComps;
    UINT4 phase, comp;
    UINT4 offset;
    UINT4 valid, calMode, calReq, calOk, pisoEnb, adsmIntDiv;
    UINT4 mapType, accumRef0;
    UINT4 eqTap1, eqTap2, eqTap3, timingEqTap;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl, intf, linkID,
                                               &valid, &calMode, &calReq,
                                               &calOk, &pisoEnb, &adsmIntDiv);
    if (result) return result;
    DBG_PRINT("valid = %ld, calMode = %ld, calReq = %ld, calOk = %ld, pisoEnb = %ld, adsmIntDiv = 0x%lx\n", valid, calMode, calReq, calOk, pisoEnb, adsmIntDiv);

    result = utilHyPhy20gSerdesGetMaxNPhaseNComp(fileHndl, intf, &nPhases,
                                                 &nComps);
    if (result) return result;

    /* Print offset values */
    for (phase = 0; phase < nPhases; phase++) {
        for (comp = 0; comp < nComps; comp++) {
            result = serdesHyPhy20gSerdesGetAdcCalOffsetCtxt(fileHndl, intf,
                                                             linkID, phase,
                                                             comp, &offset);
            if (result) return result;

            DBG_PRINT("phase = %ld, comp = %ld, offset = 0x%lx\n", phase, comp, offset);
        }
    }

    /* Print mode select values */
    result = serdesHyPhy20gSerdesGetModeSelCtxt(fileHndl, intf, linkID, &valid,
                                                &mapType, &accumRef0);
    if (result) return result;
    DBG_PRINT("valid = %ld\n", valid);
    DBG_PRINT("mapType = %ld\n", mapType);
    DBG_PRINT("accumRef0 = 0x%lx\n", accumRef0);

    /* Print equalization values */
    result = serdesHyPhy20gSerdesGetEqCtxt(fileHndl, intf, linkID, &valid,
                                           &eqTap1, &eqTap2, &eqTap3,
                                           &timingEqTap);
    if (result) return result;
    DBG_PRINT("valid = %ld\n", valid);
    DBG_PRINT("eqTap1 = 0x%lx\n", eqTap1);
    DBG_PRINT("eqTap2 = 0x%lx\n", eqTap2);
    DBG_PRINT("eqTap3 = 0x%lx\n", eqTap3);
    DBG_PRINT("timingEqTap = 0x%lx\n", timingEqTap);

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSerdesDbgPrintCtxt */

/*******************************************************************************
**
**  hyPhy20gSerdesModeSelectCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure will configure mode selection on the specified
**                  link.  Note that valid receive signal must be present for
**                  proper configuration.  This function is only valid for
**                  XFI, Backplane and PIF interfaces.  Not applicable to SFP.
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkID   - SERDES link
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**                  mode     - 0 - hardcoded mode selection
**                             1 - perform mode selection
**                             2 - use mode selection values from SW context
**
**  OUTPUTS:        None     - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSerdesModeSelectCfg(struct file *fileHndl, UINT4 intf,
                                 UINT4 linkID, UINT4 mode)
{
    INT4 result;

    /* argument check */
    if ((intf < 1 || intf > 3) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7) ||
        (mode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* run mode selection */
    result = utilHyPhy20gAdcModeSelect(fileHndl, intf, linkID, mode);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSerdesModeSelAndEqCfg */

/*******************************************************************************
**
**  hyPhy20gSerdesManualModeSelectCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure will manually configure mode selection on the 
**                  specified link to the values specified and save those 
**                  values into the software context.  Note that valid receive 
**                  signal must be present for proper configuration.  
**                  This function is only valid for
**                  XFI, Backplane and PIF interfaces.  Not applicable to SFP.
**                  Values will not be checked for proper operation.
**                  The manual values entered should be previously tested or 
**                  derived from the driver using hyPhy20gSerdesModeSelectCfg
**                  API and retrieved using hyPhy20gSerdesDbgPrintCtxt.
**
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkID   - SERDES link
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**                  accumRef0Val - 32 bit value 0x0 - 0xFFFFFFFF
**                  nonLinMapVal - 0 - linear map selection
**                                 1 - non-linear map selection
**
**
**  OUTPUTS:        None     - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSerdesManualModeSelectCfg(struct file *fileHndl, UINT4 intf,
                                       UINT4 linkID, UINT4 accumRef0Val,
                                       UINT4 nonLinMapVal)
{
    INT4 result;
    UINT4 accumRefSelReg, accumRefSelMsk;
    UINT4 nonLinMapReg, nonLinMapMsk, nonLinMap;
    UINT4 accumRef0Reg, accumRef0Msk, accumRef0;
    
    /* argument check */
    if ((intf < 1 || intf > 3) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((nonLinMapVal > 1) || (accumRef0Val > 0xFFFFFFFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    switch (intf) {
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            accumRefSelReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_20, linkID);
            accumRefSelMsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_20_ACCUM_REF_SEL_MSK;

            nonLinMapReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_1, linkID);
            nonLinMapMsk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_1_NON_LINEAR_MAP_MSK;
            nonLinMap = nonLinMapVal;

            accumRef0Reg = HYPHY20G_SERDES_XFI_CORE_REG_CFG_21;
            accumRef0Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                                         accumRef0Reg, linkID);
            accumRef0Msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_21_ACCUM_REF0_MSK;
            accumRef0 = accumRef0Val;
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            accumRefSelReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_11, linkID);
            accumRefSelMsk = HYPHY20G_SERDES_BP_C8_BIT_CFG_11_ACCUM_REF_SEL_MSK;

            nonLinMapReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_1, linkID);
            nonLinMapMsk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_1_NON_LINEAR_MAP_MSK;
            nonLinMap = nonLinMapVal;

            accumRef0Reg = HYPHY20G_SERDES_BP_C8_REG_CFG_12;
            accumRef0Reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                                         accumRef0Reg, linkID);
            accumRef0Msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_12_ACCUM_REF0_MSK;
            accumRef0 = accumRef0Val;
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            accumRefSelReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_11, linkID);
            accumRefSelMsk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_11_ACCUM_REF_SEL_MSK;

            nonLinMapReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_1, linkID);
            nonLinMapMsk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_1_NON_LINEAR_MAP_MSK;
            nonLinMap = nonLinMapVal;

            accumRef0Reg = HYPHY20G_SERDES_PIF_C8_REG_CFG_12;
            accumRef0Reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                                         accumRef0Reg, linkID);
            accumRef0Msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_12_ACCUM_REF0_MSK;
            accumRef0 = accumRef0Val;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* configure ACCUM_REF_SEL to 0 */
    result = hyPhy20gRegFieldWrite(fileHndl, accumRefSelReg,
                                   accumRefSelMsk, 0);
    if (result) return result;

    /* configure manual value of NON_LINEAR_MAP */
    result = hyPhy20gRegFieldWrite(fileHndl, nonLinMapReg, nonLinMapMsk,
                                   nonLinMap);
    if (result) return result;

    /* configure manual value of ACCUM_REF0 */
    result = hyPhy20gRegFieldWrite(fileHndl, accumRef0Reg, accumRef0Msk,
                                   accumRef0);
    if (result) return result;

    /* update device to use new threshold map */
    result = utilHyPhy20gWriteXfiBpPifAdcMemory(fileHndl, intf, linkID);
    if (result) return result;

    /* save current mode selection in SW context */
    result = utilHyPhy20gSerdesSaveModeSel(fileHndl, intf, linkID);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSerdesManualModeSelectCfg */

/*******************************************************************************
**
**  hyPhy20gSerdesEqCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure will configure equalization settings on the
**                  specified link.  Note that valid receive signal must be
**                  present for proper configuration.  This function is
**                  only valid for XFI, Backplane and PIF interfaces.  Not
**                  applicable to SFP.
**
**                  hyPhy20gHostMsgInit must be called before this function
**                  when performing FW equalization (mode == 1).
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkID   - SERDES link
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**                  mode     - 0 - hardcoded equalization
**                             1 - perform FW equalization
**                             2 - use equalization values from SW context
**
**  OUTPUTS:        None     - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSerdesEqCfg(struct file *fileHndl, UINT4 intf, UINT4 linkID,
                         UINT4 mode)
{
    INT4 result;

    /* argument check */
    if ((intf < 1 || intf > 3) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7) ||
        (mode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* run mode selection */
    result = utilHyPhy20gSerdesEqCfg(fileHndl, intf, linkID, mode);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSerdesEqCfg */

/*******************************************************************************
**
**  hyPhy20gSerdesManualEqCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure will manually configure equalization settings 
**                  on the specified link to the values specified and save 
**                  those values into the software context.  Note that 
**                  valid receive signal must be present for proper 
**                  configuration.  
**                  This function is only valid for XFI, Backplane and PIF 
**                  interfaces.  Not applicable to SFP.  
**                  Values will not be checked for proper operation.
**                  The manual values entered should be previously tested or 
**                  derived from FW using hyPhy20gSerdesEqCfg API and retrieved
**                  using hyPhy20gSerdesDbgPrintCtxt.
**
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkID   - SERDES link
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**                  timingEqTapVal - 0-8
**                  eqTap1Val      - 0-8
**                  eqTap2Val      - 0-8
**                  eqTap3Val      - 0-8
**
**  OUTPUTS:        None     - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSerdesManualEqCfg(struct file *fileHndl, UINT4 intf, UINT4 linkID,
                               UINT4 timingEqTapVal, UINT4 eqTap1Val, 
                               UINT4 eqTap2Val, UINT4 eqTap3Val)
{
    INT4 result;
    UINT4 eqTapReg, eqTap1Msk, eqTap2Msk, eqTap3Msk = 0;
    UINT4 eqTap1, eqTap2, eqTap3 = 0;
    UINT4 timingEqTapReg, timingEqTapMsk, timingEqTap = 0;
    
    /* argument check */
    if ((intf < 1 || intf > 3) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((timingEqTapVal > 8) || (eqTap1Val > 8) ||
        (eqTap2Val > 8) || (eqTap3Val > 8)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    switch (intf) {
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            eqTapReg = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_4;
            eqTapReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(eqTapReg,
                                                                  linkID);
            eqTap1Msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_EQ_TAP_1_MSK;
            eqTap2Msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_EQ_TAP_2_MSK;
            eqTap3Msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_EQ_TAP_3_MSK;

            timingEqTapReg = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_2;
            timingEqTapReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                                       timingEqTapReg, linkID);
            timingEqTapMsk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_2_TIMING_EQ_TAP_MSK;

            timingEqTap = timingEqTapVal;
            eqTap1 = eqTap1Val;
            eqTap2 = eqTap2Val;
            eqTap3 = eqTap3Val;
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            eqTapReg = HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_4;
            eqTapReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(eqTapReg,
                                                                 linkID);
            eqTap1Msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_1_MSK;
            eqTap2Msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_2_MSK;
            eqTap3Msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_3_MSK;

            timingEqTapReg = HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_2;
            timingEqTapReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                                       timingEqTapReg, linkID);
            timingEqTapMsk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_2_TIMING_EQ_TAP_MSK;

            timingEqTap = timingEqTapVal;
            eqTap1 = eqTap1Val;
            eqTap2 = eqTap2Val;
            eqTap3 = eqTap3Val;
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            eqTapReg = HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_4;
            eqTapReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(eqTapReg,
                                                                  linkID);
            eqTap1Msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_1_MSK;
            eqTap2Msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_2_MSK;
            eqTap3Msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_3_MSK;

            timingEqTapReg = HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_2;
            timingEqTapReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                                       timingEqTapReg, linkID);
            timingEqTapMsk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_2_TIMING_EQ_TAP_MSK;

            timingEqTap = timingEqTapVal;
            eqTap1 = eqTap1Val;
            eqTap2 = eqTap2Val;
            eqTap3 = eqTap3Val;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }


    /**
     ** manual equalization
     **/
    
    /* program equalizer tap */
    result = hyPhy20gRegFieldWrite(fileHndl, timingEqTapReg, timingEqTapMsk,
                                   timingEqTap);
    if (result) return result;

    /* program equalizer tap 1 */
    result = hyPhy20gRegFieldWrite(fileHndl, eqTapReg, eqTap1Msk, eqTap1);
    if (result) return result;

    /* program equalizer tap 2 */
    result = hyPhy20gRegFieldWrite(fileHndl, eqTapReg, eqTap2Msk, eqTap2);
    if (result) return result;

    /* program equalizer tap 3 */
    result = hyPhy20gRegFieldWrite(fileHndl, eqTapReg, eqTap3Msk, eqTap3);
    if (result) return result;

    /* save equalization settings in SW context */
    result = utilHyPhy20gSerdesSaveEq(fileHndl, intf, linkID);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSerdesManualEqCfg */

/*******************************************************************************
**
**  hyPhy20gSerdesGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the size of SFP/XFI/BackPlane/PIF 
**                  SERDES context.
**
**
**  INPUTS:         fileHndl - device handle
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The size of SFP/XFI/BackPlane/PIF SERDES context.             
**
*******************************************************************************/
UINT4 hyPhy20gSerdesGetCtxtSize(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 size = 0;
    
    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    /* no valid driver context */
    if (pDevCtxt->valid != HYPHY20G_VALID_SIGN) {
        DBG_PRINT("%s, %s, %d, no valid driver context\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return size;
    }

    /* place the version info at the first field inside the context buffer, 
    so that any driver version could easily find it */
    size += sizeof(pDevCtxt->version);

    size += serdesHyPhy20gSerdesGetCtxtSize(fileHndl);

    /* include end header flag in the buffer */
    size += sizeof(UINT4);

    return size;
} /* hyPhy20gSerdesGetCtxtSize */

/*******************************************************************************
**
**  hyPhy20gSerdesExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the SFP/XFI/BackPlane/PIF SERDES context into
**                  pre-allocated buffer.
**
**
**  INPUTS:         fileHndl - device handle
**                  pCtxtBuf - pre-allocated buffer for SFP/XFI/BackPlane/PIF
**                             SERDES context extraction
**
**  OUTPUTS:        pExtractedSize - the context size(in byte) has been 
**                                   extracted into the buffer
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSerdesExtractCtxt(struct file *fileHndl, void *pCtxtBuf, 
                               UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;
    UINT4 *p4;
    UINT4 offset = 0, len = 0;
    
    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    /* no valid driver context */
    if (pDevCtxt->valid != HYPHY20G_VALID_SIGN) {
        DBG_PRINT("%s, %s, %d, no valid driver context\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_SUCCESS;
    }

    /* place the version info at the first field inside the context buffer, 
    so that any driver version could easily find it */
    p4 = (UINT4 *)((UINT1 *)pCtxtBuf + offset);
    *p4 = pDevCtxt->version;
    offset += sizeof(*p4);

    /* extract SERDES context */
    result = serdesHyPhy20gSerdesExtractCtxt(fileHndl, 
                            ((UINT1 *)pCtxtBuf + offset), &len);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
                    "%s", "serdesHyPhy20gSerdesExtractCtxt failed.");
    offset += len;
    
    /* insert end header flag in the buffer */
    p4 = (UINT4 *)((UINT1 *)pCtxtBuf + offset);
    *p4 = 0;
    offset += sizeof(*p4);

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pExtractedSize = offset;
    return HYPHY20G_SUCCESS;
} /* hyPhy20gSerdesExtractCtxt */
    
/*******************************************************************************
**
**  hyPhy20gSerdesRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the SFP/XFI/BackPlane/PIF SERDES context
**                  into the driver context memory.
**
**
**  INPUTS:         fileHndl - device handle
**                  pCtxtBuf - pre-stored SFP/XFI/BackPlane/PIF 
**                             SERDES context for restoration
**
**  OUTPUTS:        pParsedSize - the data size(in byte) has been parsed
**                                in the buffer for restoration
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_CTXT_CORRUPTED
**                  HYPHY20G_FAILURE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSerdesRestoreCtxt(struct file *fileHndl, void *pCtxtBuf, 
                               UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    
    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    /* no valid driver context */
    if (pDevCtxt->valid != HYPHY20G_VALID_SIGN) {
        DBG_PRINT("%s, %s, %d, Driver context not valid\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* retrieve the version code of the pre-stored context */
    /*
    **p4 = (UINT4 *)pCtxtBuf;
    **preStoredVersion = *p4;
    */
    /* [TBD]: if the sHYPHY20G_SERDES_CTXT structure is modified in a future version,
    the modifier should also update the code here to verify backwards-compatibility
    of context restoration */

    pCtxtBuf = (void *)((UINT1 *)pCtxtBuf + sizeof(pDevCtxt->version));
    
    return serdesHyPhy20gSerdesRestoreCtxt(fileHndl, pCtxtBuf, pParsedSize);
} /* hyPhy20gSerdesRestoreCtxt */

/*******************************************************************************
**
**  hyPhy20gSerdesLoadCtxtFactoryCal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the SFP/XFI/BackPlane/PIF SERDES context per link
**                  into the driver context memory.
**
**                  Prior to calling this API, *SerdesInit must be called for
**                  the link that this API is loading factory calibration for.
**                  
**                  To restore valid context for the specified link, the ADC
**                  Factory Calibration information of this link must have been 
**                  extracted into a buffer that can be read by this API.
**
**                  This function will parse through the entire SERDES context
**                  information stored in the buffer and find the specified link
**                  offset and load factory calibration context information 
**                  of the link only if valid data exists in the stored buffer.
**
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                              0 - SFP
**                              1 - XFI
**                              2 - Backplane
**                              3 - PIF
**                  linkId   - SERDES link
**                              SFP       - 0 to 15
**                              XFI       - 0 to 1
**                              Backplane - 0 to 31
**                              PIF       - 0 to 7
**                  pCtxtBuf - pre-stored Sfp/Xfi/BackPlane/Pif 
**                             SERDES context for restoration
**
**  OUTPUTS:        pParsedSize - the data size(in byte) has been parsed
**                                in the buffer for restoration
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_CTXT_CORRUPTED
**                  HYPHY20G_FAILURE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSerdesLoadCtxtFactoryCal(struct file *fileHndl, UINT4 intf, 
                                      UINT4 linkId, void *pCtxtBuf, 
                                      UINT4 *pParsedSize)
{ 
    sHYPHY20G_CTXT *pDevCtxt;
    
    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    /* no valid driver context */
    if (pDevCtxt->valid != HYPHY20G_VALID_SIGN) {
        DBG_PRINT("%s, %s, %d, Driver context not valid\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* retrieve the version code of the pre-stored context */
    /*
    **p4 = (UINT4 *)pCtxtBuf;
    **preStoredVersion = *p4;
    */
    /* [TBD]: if the sHYPHY20G_SERDES_CTXT structure is modified in a future version,
    the modifier should also update the code here to verify backwards-compatibility
    of context restoration */

    pCtxtBuf = (void *)((UINT1 *)pCtxtBuf + sizeof(pDevCtxt->version));

    return utilHyPhy20gSerdesLoadFactoryCalCtxt(fileHndl, intf, linkId, 
                                                pCtxtBuf, pParsedSize);      
}

/*******************************************************************************
**
**  hyPhy20gSerdesRxFifoReCenter
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function re-centers the receive SERDES FIFO for the
**                  specified interface link.
**
**  INPUTS:         fileHndl   - device handle
**                  intf       - SERDES interface
**                                   0 - SFP
**                                   1 - XFI
**                                   2 - Backplane
**                                   3 - PIF
**                  linkID     - Link ID
**                                   0 to 15 - SFP Interface 0 to 15
**                                   17      - SFP SGMII 17
**                                   0 to 1  - XFI Interface 0 to 1
**                                   0 to 31 - Backplane Interface 0 to 31
**                                   0 to 7  - PIF Interface 0 to 7
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSerdesRxFifoReCenter(struct file *fileHndl, UINT4 intf,
                                      UINT4 linkID)
{
    INT4 result;

    result = utilHyPhy20gSerdesRxFifoReCenter(fileHndl, intf, linkID);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSerdesRxFifoReCenter */
                           
/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/*******************************************************************************
**
**  serdesHyPhy20gSerdesCreateCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates and intializes memory required to operate
**                  the SERDES.
**
**                  References this memory to the device level structure
**                  pointed to with *pDevCtxt.
**
**                  Function serdesHyPhy20gSerdesDestroyCtxt recovers this
**                  memory and set the device level reference to NULL.
**
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 0 - SFP
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkID   - SERDES link
**                                 SFP       - 0 to 15
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesCreateCtxt(struct file *fileHndl, UINT4 intf,
                                    UINT4 linkID)
{
    UINT4 size = sizeof(sHYPHY20G_SERDES_CTXT);
    UINT4 offsetSize;
    UINT4 destroyCtxt = 0;
    UINT4 phase, nPhases, comp, nComps;
    UINT4 valid, calMode, calReq, calOk, pisoEnb, adsmIntDiv;
    sHYPHY20G_SERDES_ADC_CAL_CTXT *pAdcCalCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;

//    DBG_PRINT("%s, %s, %d, intf = %lu, linkID = %lu\n",__FILE__, __FUNCTION__, __LINE__, intf, linkID);

    /* argument check */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* If serdes context is not a NULL pointer,
       destroy it first to prevent memory leak */
    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            nPhases = HYPHY20G_SERDES_NPHASES_SFP;
            nComps = HYPHY20G_SERDES_NCOMPS_SFP;
            destroyCtxt = (pDevCtxt->pSfpSerdesCtxt[linkID] != NULL) ? 1 : 0;
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            nPhases = HYPHY20G_SERDES_NPHASES_XFI;
            nComps = HYPHY20G_SERDES_NCOMPS_XFI;
            destroyCtxt = (pDevCtxt->pXfiSerdesCtxt[linkID] != NULL) ? 1 : 0;
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            nPhases = HYPHY20G_SERDES_NPHASES_BACKPLANE;
            nComps = HYPHY20G_SERDES_NCOMPS_BACKPLANE;
            destroyCtxt = (pDevCtxt->pBackplaneSerdesCtxt[linkID] != NULL) ?
                          1 : 0;
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            nPhases = HYPHY20G_SERDES_NPHASES_PIF;
            nComps = HYPHY20G_SERDES_NCOMPS_PIF;
            destroyCtxt = (pDevCtxt->pPifSerdesCtxt[linkID] != NULL) ? 1 : 0;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;

    }
    if (destroyCtxt) {
        serdesHyPhy20gSerdesDestroyCtxt(fileHndl, intf, linkID);
    }

    offsetSize = sizeof(UINT4)*nPhases*nComps;

    /* allocate memory for and associate with each SERDES context */
    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            pDevCtxt->pSfpSerdesCtxt[linkID] = (sHYPHY20G_SERDES_CTXT *) sysHyPhy20gMemAlloc(size);
            if (pDevCtxt->pSfpSerdesCtxt[linkID] == NULL) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_MEM_ALLOC;
            }

            pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkID];
            pAdcCalCtxt = &(pSerdesCtxt->adcCalCtxt);
            pAdcCalCtxt->pOffset = (UINT4 *) sysHyPhy20gMemAlloc(offsetSize);
            if (pAdcCalCtxt->pOffset == NULL) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
                sysHyPhy20gMemFree(pDevCtxt->pSfpSerdesCtxt[linkID]);
                return HYPHY20G_ERR_MEM_ALLOC;
            }
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            pDevCtxt->pXfiSerdesCtxt[linkID] = (sHYPHY20G_SERDES_CTXT *) sysHyPhy20gMemAlloc(size);
            if (pDevCtxt->pXfiSerdesCtxt[linkID] == NULL) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_MEM_ALLOC;
            }

            pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkID];
            pAdcCalCtxt = &(pSerdesCtxt->adcCalCtxt);
            pAdcCalCtxt->pOffset = (UINT4 *) sysHyPhy20gMemAlloc(offsetSize);
            if (pAdcCalCtxt->pOffset == NULL) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
                sysHyPhy20gMemFree(pDevCtxt->pXfiSerdesCtxt[linkID]);
                return HYPHY20G_ERR_MEM_ALLOC;
            }
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            pDevCtxt->pBackplaneSerdesCtxt[linkID] = (sHYPHY20G_SERDES_CTXT *) sysHyPhy20gMemAlloc(size);
            if (pDevCtxt->pBackplaneSerdesCtxt[linkID] == NULL) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_MEM_ALLOC;
            }

            pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkID];
            pAdcCalCtxt = &(pSerdesCtxt->adcCalCtxt);
            pAdcCalCtxt->pOffset = (UINT4 *) sysHyPhy20gMemAlloc(offsetSize);
            if (pAdcCalCtxt->pOffset == NULL) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
                sysHyPhy20gMemFree(pDevCtxt->pBackplaneSerdesCtxt[linkID]);
                return HYPHY20G_ERR_MEM_ALLOC;
            }
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            pDevCtxt->pPifSerdesCtxt[linkID] = (sHYPHY20G_SERDES_CTXT *) sysHyPhy20gMemAlloc(size);
            if (pDevCtxt->pPifSerdesCtxt[linkID] == NULL) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_MEM_ALLOC;
            }

            pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkID];
            pAdcCalCtxt = &(pSerdesCtxt->adcCalCtxt);
            pAdcCalCtxt->pOffset = (UINT4 *) sysHyPhy20gMemAlloc(offsetSize);
            if (pAdcCalCtxt->pOffset == NULL) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_MEM_ALLOC\n",__FILE__, __FUNCTION__, __LINE__);
                sysHyPhy20gMemFree(pDevCtxt->pPifSerdesCtxt[linkID]);
                return HYPHY20G_ERR_MEM_ALLOC;
            }
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    pAdcCalCtxt->sizeOffset = offsetSize;

    /* initialize context */
    valid      = HYPHY20G_VALID_VAL_32BIT;
    calMode    = HYPHY20G_INVALID_VAL_32BIT;
    calReq     = 0;
    calOk      = 0;
    pisoEnb    = 0;
    adsmIntDiv = 0;
    result = serdesHyPhy20gSerdesSetAdcCalCtxt(fileHndl, intf, linkID, valid,
                                               calMode, calReq, calOk, pisoEnb,
                                               adsmIntDiv);
    if (result) {
        serdesHyPhy20gSerdesDestroyCtxt(fileHndl, intf, linkID);
        return result;
    }

    for (phase = 0; phase < nPhases; phase++) {
        for (comp = 0; comp < nComps; comp++) {
            result = serdesHyPhy20gSerdesSetAdcCalOffsetCtxt(fileHndl, intf,
                                                             linkID, phase,
                                                             comp, 0);
            if (result) {
                serdesHyPhy20gSerdesDestroyCtxt(fileHndl, intf, linkID);
                return result;
            }
        }
    }

    return HYPHY20G_SUCCESS;
} /* serdesHyPhy20gSerdesCreateCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesDestroyCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset context of the SERDES subsystem.
**
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 0 - SFP
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkID   - SERDES link
**                                 SFP       - 0 to 15
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesDestroyCtxt(struct file *fileHndl, UINT4 intf,
                                     UINT4 linkID)
{
    sHYPHY20G_SERDES_ADC_CAL_CTXT *pAdcCalCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

//    DBG_PRINT("%s, %s, %d, intf = %lu, linkID = %lu\n",__FILE__, __FUNCTION__, __LINE__, intf, linkID);

    /* argument check */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            if (pDevCtxt->pSfpSerdesCtxt[linkID] == NULL) {
                DBG_PRINT("%s, %s, %d, returning success\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_SUCCESS;
            } else {
                pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkID];
                pAdcCalCtxt = &(pSerdesCtxt->adcCalCtxt);
                sysHyPhy20gMemFree(pAdcCalCtxt->pOffset);
                sysHyPhy20gMemFree(pDevCtxt->pSfpSerdesCtxt[linkID]);
            }
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            if(pDevCtxt->pXfiSerdesCtxt[linkID] == NULL){
                DBG_PRINT("%s, %s, %d, returning success\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_SUCCESS;
            } else {
                pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkID];
                pAdcCalCtxt = &(pSerdesCtxt->adcCalCtxt);
                sysHyPhy20gMemFree(pAdcCalCtxt->pOffset);
                sysHyPhy20gMemFree(pDevCtxt->pXfiSerdesCtxt[linkID]);
            }
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            if(pDevCtxt->pBackplaneSerdesCtxt[linkID] == NULL){
                DBG_PRINT("%s, %s, %d, returning success\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_SUCCESS;
            } else {
                pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkID];
                pAdcCalCtxt = &(pSerdesCtxt->adcCalCtxt);
                sysHyPhy20gMemFree(pAdcCalCtxt->pOffset);
                sysHyPhy20gMemFree(pDevCtxt->pBackplaneSerdesCtxt[linkID]);
            }
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            if(pDevCtxt->pPifSerdesCtxt[linkID] == NULL){
                DBG_PRINT("%s, %s, %d, returning success\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_SUCCESS;
            } else {
                pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkID];
                pAdcCalCtxt = &(pSerdesCtxt->adcCalCtxt);
                sysHyPhy20gMemFree(pAdcCalCtxt->pOffset);
                sysHyPhy20gMemFree(pDevCtxt->pPifSerdesCtxt[linkID]);
            }
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;

} /* serdesHyPhy20gSerdesDestroyCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesSetAdcCalCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Sets SERDES ADC calibration context elements.
**
**
**  INPUTS:         fileHndl        - device handle
**                  intf            - SERDES interface
**                                        0 - SFP
**                                        1 - XFI
**                                        2 - Backplane
**                                        3 - PIF
**                  linkID          - SERDES link
**                                        SFP       - 0 to 15
**                                        XFI       - 0 to 1
**                                        Backplane - 0 to 31
**                                        PIF       - 0 to 7
**                  valid           - Validity of context:
**                                        HYPHY20G_VALID_VAL_32BIT - valid
**                                        Others - invalid
**                  calMode         - Calibration mode
**                                        HYPHY20G_SERDES_FACTORY_CAL_MODE = 0
**                                        HYPHY20G_SERDES_DYNAMIC_CAL_MODE = 1
**                  calReq          - Calibration request
**                                        HYPHY20G_SERDES_NO_CAL = 0
**                                        HYPHY20G_SERDES_RUN_CAL = 1
**                  calOk           - Calibration result
**                                        HYPHY20G_SERDES_CAL_FAIL = 0
**                                        HYPHY20G_SERDES_CAL_SUCCESS = 1
**                  pisoEnb         - PISO_ENB value before calibration
**                  adsmIntDiv      - ADSM_INT_DIV value before calibration
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesSetAdcCalCtxt(struct file *fileHndl, UINT4 intf,
                                       UINT4 linkID, UINT4 valid, UINT4 calMode,
                                       UINT4 calReq, UINT4 calOk, UINT4 pisoEnb,
                                       UINT4 adsmIntDiv)
{
    sHYPHY20G_SERDES_ADC_CAL_CTXT *pAdcCalCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7) ||
        (pisoEnb > 1) || (adsmIntDiv > 0xFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* generate pointer to interface level context information for this link */
    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkID];
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    HYPHY20G_ASSERT((pSerdesCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* generate pointer to ADC calibration context information */
    pAdcCalCtxt = &(pSerdesCtxt->adcCalCtxt);

    /* set channel context */
    pAdcCalCtxt->valid = valid;
    pAdcCalCtxt->calMode = calMode;
    pAdcCalCtxt->calReq = calReq;
    pAdcCalCtxt->calOk = calOk;
    pAdcCalCtxt->pisoEnb = pisoEnb;
    pAdcCalCtxt->adsmIntDiv = adsmIntDiv;

    return HYPHY20G_SUCCESS;

} /* serdesHyPhy20gSerdesSetAdcCalCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesGetAdcCalCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Gets SERDES ADC calibration context elements.
**
**
**  INPUTS:         fileHndl        - device handle
**                  intf            - SERDES interface
**                                        0 - SFP
**                                        1 - XFI
**                                        2 - Backplane
**                                        3 - PIF
**                  linkID          - SERDES link
**                                        SFP       - 0 to 15
**                                        XFI       - 0 to 1
**                                        Backplane - 0 to 31
**                                        PIF       - 0 to 7
**                  *pValid         - Pointer to storage for validity of
**                                    context
**                                        HYPHY20G_VALID_VAL_32BIT - valid
**                                        Others - invalid
**                  *pCalMode       - Pointer to storage for calibration mode
**                                        HYPHY20G_SERDES_FACTORY_CAL_MODE = 0
**                                        HYPHY20G_SERDES_DYNAMIC_CAL_MODE = 1
**                  *pCalReq        - Pointer to storage for  calibration
**                                    request
**                                        HYPHY20G_SERDES_NO_CAL = 0
**                                        HYPHY20G_SERDES_RUN_CAL = 1
**                  *pCalOk         - Pointer to storage for calibration result
**                                        HYPHY20G_SERDES_CAL_FAIL = 0
**                                        HYPHY20G_SERDES_CAL_SUCCESS = 1
**                  *pPisoEnb       - Pointer to storage for PISO_ENB value
**                                    before calibration
**                  *pAdsmIntDiv    - Pointer to storage for ADSM_INT_DIV value
**                                    before calibration
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesGetAdcCalCtxt(struct file *fileHndl, UINT4 intf,
                                       UINT4 linkID, UINT4 *pValid,
                                       UINT4 *pCalMode, UINT4 *pCalReq,
                                       UINT4 *pCalOk, UINT4 *pPisoEnb,
                                       UINT4 *pAdsmIntDiv)
{
    sHYPHY20G_SERDES_ADC_CAL_CTXT *pAdcCalCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* generate pointer to interface level context information for this link */
    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkID];
            DBG_PRINT("%s, %s, %d, HYPHY20G_SERDES_SFP_INTF\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkID];
            DBG_PRINT("%s, %s, %d, HYPHY20G_SERDES_XFI_INTF\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkID];
            DBG_PRINT("%s, %s, %d, HYPHY20G_SERDES_BACKPLANE_INTF\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkID];
            DBG_PRINT("%s, %s, %d, HYPHY20G_SERDES_PIF_INTF\n",__FILE__, __FUNCTION__, __LINE__);
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    HYPHY20G_ASSERT((pSerdesCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSerdesCtxt is NULL.");

    /* generate pointer to ADC calibration context information */
    pAdcCalCtxt = &(pSerdesCtxt->adcCalCtxt);

    /* set channel context */
    *pValid = pAdcCalCtxt->valid;
    DBG_PRINT("%s, %s, %d, pValid = %li\n",__FILE__, __FUNCTION__, __LINE__, *pValid);
    *pCalMode = pAdcCalCtxt->calMode;
    DBG_PRINT("%s, %s, %d, pCalMode = %li\n",__FILE__, __FUNCTION__, __LINE__, *pCalMode);
    *pCalReq = pAdcCalCtxt->calReq;
    DBG_PRINT("%s, %s, %d, pCalReq = %li\n",__FILE__, __FUNCTION__, __LINE__, *pCalReq);
    *pCalOk = pAdcCalCtxt->calOk;
    DBG_PRINT("%s, %s, %d, pCalOk = %li\n",__FILE__, __FUNCTION__, __LINE__, *pCalOk);
    *pPisoEnb = pAdcCalCtxt->pisoEnb;
    DBG_PRINT("%s, %s, %d, pPisoEnb = %li\n",__FILE__, __FUNCTION__, __LINE__, *pPisoEnb);
    *pAdsmIntDiv = pAdcCalCtxt->adsmIntDiv;
    DBG_PRINT("%s, %s, %d, pAdsmIntDiv = %li\n",__FILE__, __FUNCTION__, __LINE__, *pAdsmIntDiv);

    return HYPHY20G_SUCCESS;

} /* serdesHyPhy20gSerdesGetAdcCalCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesSetMskAdcCalCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Sets one or more parameters of SERDES ADC offset calibration
**                  context as controlled by setMsk argument.
**
**
**  INPUTS:         fileHndl        - device handle
**                  intf            - SERDES interface
**                                        0 - SFP
**                                        1 - XFI
**                                        2 - Backplane
**                                        3 - PIF
**                  linkID          - SERDES link
**                                        SFP       - 0 to 15
**                                        XFI       - 0 to 1
**                                        Backplane - 0 to 31
**                                        PIF       - 0 to 7
**                  valid           - Validity of context:
**                                        HYPHY20G_VALID_VAL_32BIT - valid
**                                        Others - invalid
**                  calMode         - Calibration mode
**                                        HYPHY20G_SERDES_FACTORY_CAL_MODE = 0
**                                        HYPHY20G_SERDES_DYNAMIC_CAL_MODE = 1
**                  calReq          - Calibration request
**                                        HYPHY20G_SERDES_NO_CAL = 0
**                                        HYPHY20G_SERDES_RUN_CAL = 1
**                  calOk           - Calibration result
**                                        HYPHY20G_SERDES_CAL_FAIL = 0
**                                        HYPHY20G_SERDES_CAL_SUCCESS = 1
**                  pisoEnb         - PISO_ENB value before calibration
**                  adsmIntDiv      - ADSM_INT_DIV value before calibration
**                  setMsk          - Mask of fields to update
**                                        bit 0: valid
**                                        bit 1: calMode
**                                        bit 2: calReq
**                                        bit 3: calOk
**                                        bit 4: pisoEnb
**                                        bit 5: adsmIntDiv
**                                    Values:
**                                        0 - do not set field
**                                        1 - set field
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesSetMskAdcCalCtxt(struct file *fileHndl, UINT4 intf,
                                          UINT4 linkID, UINT4 valid,
                                          UINT4 calMode, UINT4 calReq,
                                          UINT4 calOk, UINT4 pisoEnb,
                                          UINT4 adsmIntDiv, UINT4 setMsk)
{
    UINT4 devValid, devCalMode, devCalReq, devCalOk;
    UINT4 devPisoEnb, devAdsmIntDiv;
    INT4 result;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7) ||
        (setMsk == 0x10 && pisoEnb > 1) ||
        (setMsk == 0x20 && adsmIntDiv > 0xFF) || (setMsk > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl, intf, linkID,
                                               &devValid, &devCalMode,
                                               &devCalReq, &devCalOk,
                                               &devPisoEnb, &devAdsmIntDiv);
    if (result) return result;

    /* update arguments based on setMsk */
    if (setMsk & 0x1) {
        devValid = valid;
    }
    if (setMsk & 0x2) {
        devCalMode = calMode;
    }
    if (setMsk & 0x4) {
        devCalReq = calReq;
    }
    if (setMsk & 0x8) {
        devCalOk = calOk;
    }
    if (setMsk & 0x10) {
        devPisoEnb = pisoEnb;
    }
    if (setMsk & 0x20) {
        devAdsmIntDiv = adsmIntDiv;
    }

    /* update context settings */
    result = serdesHyPhy20gSerdesSetAdcCalCtxt(fileHndl, intf, linkID, devValid,
                                               devCalMode, devCalReq, devCalOk,
                                               devPisoEnb, devAdsmIntDiv);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* serdesHyPhy20gSerdesSetMskAdcCalCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesSetAdcCalOffsetCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Sets SERDES ADC offset calibration offset context element.
**
**
**  INPUTS:         fileHndl        - device handle
**                  intf            - SERDES interface
**                                        0 - SFP
**                                        1 - XFI
**                                        2 - Backplane
**                                        3 - PIF
**                  linkID          - SERDES link
**                                        SFP       - 0 to 15
**                                        XFI       - 0 to 1
**                                        Backplane - 0 to 31
**                                        PIF       - 0 to 7
**                  phase           - Phase number (base-0)
**                  comp            - Comp number (base-0)
**                  offset          - ADC calibration offset
**                                    Legal range 0-0xFF
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesSetAdcCalOffsetCtxt(struct file *fileHndl, UINT4 intf,
                                             UINT4 linkID, UINT4 phase,
                                             UINT4 comp, UINT4 offset)
{
    UINT4 nPhases, nComps, idx;
    UINT4 *pOffset;
    sHYPHY20G_SERDES_ADC_CAL_CTXT *pAdcCalCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7) ||
        (offset > 0xFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            nPhases = HYPHY20G_SERDES_NPHASES_SFP;
            nComps = HYPHY20G_SERDES_NCOMPS_SFP;
            pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            nPhases = HYPHY20G_SERDES_NPHASES_XFI;
            nComps = HYPHY20G_SERDES_NCOMPS_XFI;
            pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            nPhases = HYPHY20G_SERDES_NPHASES_BACKPLANE;
            nComps = HYPHY20G_SERDES_NCOMPS_BACKPLANE;
            pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            nPhases = HYPHY20G_SERDES_NPHASES_PIF;
            nComps = HYPHY20G_SERDES_NCOMPS_PIF;
            pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkID];
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;

    }
    if ((phase > nPhases) || (comp > nComps)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    HYPHY20G_ASSERT((pSerdesCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* generate pointer to ADC calibration context information */
    pAdcCalCtxt = &(pSerdesCtxt->adcCalCtxt);
    pOffset = pAdcCalCtxt->pOffset;

    /* set offset context */
    idx = (phase*nComps) + comp;
    *(pOffset+idx) = offset;

    return HYPHY20G_SUCCESS;

} /* serdesHyPhy20gSerdesSetAdcCalOffsetCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesGetAdcCalOffsetCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Gets SERDES ADC offset calibration offset context element.
**
**
**  INPUTS:         fileHndl        - device handle
**                  intf            - SERDES interface
**                                        0 - SFP
**                                        1 - XFI
**                                        2 - Backplane
**                                        3 - PIF
**                  linkID          - SERDES link
**                                        SFP       - 0 to 15
**                                        XFI       - 0 to 1
**                                        Backplane - 0 to 31
**                                        PIF       - 0 to 7
**                  phase           - Phase number (base-0)
**                  comp            - Comp number (base-0)
**                  *pOffset        - Pointer to storage for ADC calibration
**                                    offset
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesGetAdcCalOffsetCtxt(struct file *fileHndl, UINT4 intf,
                                             UINT4 linkID, UINT4 phase,
                                             UINT4 comp, UINT4 *pOffset)
{
    UINT4 nPhases, nComps, idx;
    UINT4 *devOffset;
    sHYPHY20G_SERDES_ADC_CAL_CTXT *pAdcCalCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            nPhases = HYPHY20G_SERDES_NPHASES_SFP;
            nComps = HYPHY20G_SERDES_NCOMPS_SFP;
            pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            nPhases = HYPHY20G_SERDES_NPHASES_XFI;
            nComps = HYPHY20G_SERDES_NCOMPS_XFI;
            pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            nPhases = HYPHY20G_SERDES_NPHASES_BACKPLANE;
            nComps = HYPHY20G_SERDES_NCOMPS_BACKPLANE;
            pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            nPhases = HYPHY20G_SERDES_NPHASES_PIF;
            nComps = HYPHY20G_SERDES_NCOMPS_PIF;
            pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkID];
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;

    }
    if ((phase > nPhases) || (comp > nComps)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    HYPHY20G_ASSERT((pSerdesCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* generate pointer to ADC calibration context information */
    pAdcCalCtxt = &(pSerdesCtxt->adcCalCtxt);
    devOffset = pAdcCalCtxt->pOffset;

    /* set offset context */
    idx = (phase*nComps) + comp;
    *pOffset = *(devOffset+idx);

    return HYPHY20G_SUCCESS;

} /* serdesHyPhy20gSerdesGetAdcCalOffsetCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesSetModeSelCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Sets SERDES mode select context elements.
**
**
**  INPUTS:         fileHndl        - device handle
**                  intf            - SERDES interface
**                                        0 - SFP
**                                        1 - XFI
**                                        2 - Backplane
**                                        3 - PIF
**                  linkID          - SERDES link
**                                        SFP       - 0 to 15
**                                        XFI       - 0 to 1
**                                        Backplane - 0 to 31
**                                        PIF       - 0 to 7
**                  valid           - Validity of context:
**                                        HYPHY20G_VALID_VAL_32BIT - valid
**                                        Others - invalid
**                  mapType         - Mapping type
**                                        HYPHY20G_SERDES_LINEAR_MAP
**                                        HYPHY20G_SERDES_NONLINEAR_MAP
**                  accumRef0       - ACCUM_REF0 value
**                                    Legal range 0x0 to 0xFFFFFFFF
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesSetModeSelCtxt(struct file *fileHndl, UINT4 intf,
                                        UINT4 linkID, UINT4 valid,
                                        UINT4 mapType, UINT4 accumRef0)
{
    sHYPHY20G_SERDES_MODE_SEL_CTXT *pModeSelCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7) ||
        (mapType > HYPHY20G_SERDES_MAX_MAP_TYPE) || (accumRef0 > 0xFFFFFFFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* generate pointer to interface level context information for this link */
    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkID];
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    HYPHY20G_ASSERT((pSerdesCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* generate pointer to ADC calibration context information */
    pModeSelCtxt = &(pSerdesCtxt->modeSelCtxt);

    /* set channel context */
    pModeSelCtxt->valid = valid;
    pModeSelCtxt->mapType = mapType;
    pModeSelCtxt->accumRef0 = accumRef0;

    return HYPHY20G_SUCCESS;

} /* serdesHyPhy20gSerdesSetModeSelCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesGetModeSelCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Gets SERDES mode select context elements.
**
**
**  INPUTS:         fileHndl        - device handle
**                  intf            - SERDES interface
**                                        0 - SFP
**                                        1 - XFI
**                                        2 - Backplane
**                                        3 - PIF
**                  linkID          - SERDES link
**                                        SFP       - 0 to 15
**                                        XFI       - 0 to 1
**                                        Backplane - 0 to 31
**                                        PIF       - 0 to 7
**                  *pValid         - Pointer to storage for validity of
**                                    context
**                  *pMapType       - Pointer to storage for mapping type
**                  *pAccumRef0     - Pointer to storage for ACCUM_REF0 value
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesGetModeSelCtxt(struct file *fileHndl, UINT4 intf,
                                       UINT4 linkID, UINT4 *pValid,
                                       UINT4 *pMapType, UINT4 *pAccumRef0)
{
    sHYPHY20G_SERDES_MODE_SEL_CTXT *pModeSelCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* generate pointer to interface level context information for this link */
    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkID];
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    HYPHY20G_ASSERT((pSerdesCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSerdesCtxt is NULL.");

    /* generate pointer to ADC calibration context information */
    pModeSelCtxt = &(pSerdesCtxt->modeSelCtxt);

    /* set channel context */
    *pValid = pModeSelCtxt->valid;
    *pMapType = pModeSelCtxt->mapType;
    *pAccumRef0 = pModeSelCtxt->accumRef0;

    return HYPHY20G_SUCCESS;

} /* serdesHyPhy20gSerdesGetModeSelCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesSetMskModeSelCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Sets one or more parameters of SERDES mode select context
**                  as controlled by setMsk argument.
**
**
**  INPUTS:         fileHndl        - device handle
**                  intf            - SERDES interface
**                                        0 - SFP
**                                        1 - XFI
**                                        2 - Backplane
**                                        3 - PIF
**                  linkID          - SERDES link
**                                        SFP       - 0 to 15
**                                        XFI       - 0 to 1
**                                        Backplane - 0 to 31
**                                        PIF       - 0 to 7
**                  valid           - Validity of context:
**                                        HYPHY20G_VALID_VAL_32BIT - valid
**                                        Others - invalid
**                  mapType         - Mapping type
**                                        HYPHY20G_SERDES_LINEAR_MAP
**                                        HYPHY20G_SERDES_NONLINEAR_MAP
**                  accumRef0       - ACCUM_REF0 value
**                                    Legal range 0x0 to 0xFFFFFFFF
**                  setMsk          - Mask of fields to update
**                                        bit 0: valid
**                                        bit 1: mapType
**                                        bit 2: accumRef0
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesSetMskModeSelCtxt(struct file *fileHndl, UINT4 intf,
                                           UINT4 linkID, UINT4 valid,
                                           UINT4 mapType, UINT4 accumRef0,
                                           UINT4 setMsk)
{
    UINT4 devValid, devMapType, devAccumRef0;
    INT4 result;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7) ||
        (mapType > HYPHY20G_SERDES_MAX_MAP_TYPE) || (accumRef0 > 0xFFFFFFFF) ||
        (setMsk > 0x7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = serdesHyPhy20gSerdesGetModeSelCtxt(fileHndl, intf, linkID,
                                                &devValid, &devMapType,
                                                &devAccumRef0);
    if (result) return result;

    /* update arguments based on setMsk */
    if (setMsk & 0x1) {
        devValid = valid;
    }
    if (setMsk & 0x2) {
        devMapType = mapType;
    }
    if (setMsk & 0x4) {
        devAccumRef0 = accumRef0;
    }

    /* update context settings */
    result = serdesHyPhy20gSerdesSetModeSelCtxt(fileHndl, intf, linkID,
                                                devValid, devMapType,
                                                devAccumRef0);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* serdesHyPhy20gSerdesSetMskModeSelCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesSetEqCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Sets SERDES equalization context elements.
**
**
**  INPUTS:         fileHndl        - device handle
**                  intf            - SERDES interface
**                                        0 - SFP
**                                        1 - XFI
**                                        2 - Backplane
**                                        3 - PIF
**                  linkID          - SERDES link
**                                        SFP       - 0 to 15
**                                        XFI       - 0 to 1
**                                        Backplane - 0 to 31
**                                        PIF       - 0 to 7
**                  valid           - Validity of context:
**                                        HYPHY20G_VALID_VAL_32BIT - valid
**                                        Others - invalid
**                  eqTap1          - EQ_TAP_1 value
**                                    Legal range 0x0 to 0xF
**                  eqTap2          - EQ_TAP_2 value
**                                    Legal range 0x0 to 0xF
**                  eqTap3          - EQ_TAP_3 value
**                                    Legal range 0x0 to 0xF
**                  timingEqTap     - TIMING_EQ_TAP value
**                                    Legal range 0x0 to 0xF
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesSetEqCtxt(struct file *fileHndl, UINT4 intf,
                                   UINT4 linkID, UINT4 valid, UINT4 eqTap1,
                                   UINT4 eqTap2, UINT4 eqTap3,
                                   UINT4 timingEqTap)
{
    sHYPHY20G_SERDES_EQ_CTXT *pEqCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7) ||
        (eqTap1 > 0xF) || (eqTap2 > 0xF) || (eqTap3 > 0xF) ||
        (timingEqTap > 0xF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* generate pointer to interface level context information for this link */
    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkID];
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    HYPHY20G_ASSERT((pSerdesCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* generate pointer to ADC calibration context information */
    pEqCtxt = &(pSerdesCtxt->eqCtxt);

    /* set channel context */
    pEqCtxt->valid = valid;
    pEqCtxt->eqTap1 = eqTap1;
    pEqCtxt->eqTap2 = eqTap2;
    pEqCtxt->eqTap3 = eqTap3;
    pEqCtxt->timingEqTap = timingEqTap;

    return HYPHY20G_SUCCESS;

} /* serdesHyPhy20gSerdesSetEqCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesGetEqCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Gets SERDES mode select context elements.
**
**
**  INPUTS:         fileHndl        - device handle
**                  intf            - SERDES interface
**                                        0 - SFP
**                                        1 - XFI
**                                        2 - Backplane
**                                        3 - PIF
**                  linkID          - SERDES link
**                                        SFP       - 0 to 15
**                                        XFI       - 0 to 1
**                                        Backplane - 0 to 31
**                                        PIF       - 0 to 7
**                  *pValid         - Pointer to storage for validity of
**                                    context
**                  *pEqTap1        - Pointer to storage for EQ_TAP_1 value
**                  *pEqTap2        - Pointer to storage for EQ_TAP_2 value
**                  *pEqTap3        - Pointer to storage for EQ_TAP_3 value
**                  *pTimingEqTap   - Pointer to storage for TIMING_EQ_TAP value
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesGetEqCtxt(struct file *fileHndl, UINT4 intf,
                                   UINT4 linkID, UINT4 *pValid,
                                   UINT4 *pEqTap1, UINT4 *pEqTap2,
                                   UINT4 *pEqTap3, UINT4 *pTimingEqTap)
{
    sHYPHY20G_SERDES_EQ_CTXT *pEqCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* generate pointer to interface level context information for this link */
    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkID];
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkID];
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    HYPHY20G_ASSERT((pSerdesCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pSerdesCtxt is NULL.");

    /* generate pointer to ADC calibration context information */
    pEqCtxt = &(pSerdesCtxt->eqCtxt);

    /* set channel context */
    *pValid = pEqCtxt->valid;
    *pEqTap1 = pEqCtxt->eqTap1;
    *pEqTap2 = pEqCtxt->eqTap2;
    *pEqTap3 = pEqCtxt->eqTap3;
    *pTimingEqTap = pEqCtxt->timingEqTap;

    return HYPHY20G_SUCCESS;

} /* serdesHyPhy20gSerdesGetEqCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesSetMskEqCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Sets one or more parameters of SERDES equalization context
**                  as controlled by setMsk argument.
**
**  INPUTS:         fileHndl        - device handle
**                  intf            - SERDES interface
**                                        0 - SFP
**                                        1 - XFI
**                                        2 - Backplane
**                                        3 - PIF
**                  linkID          - SERDES link
**                                        SFP       - 0 to 15
**                                        XFI       - 0 to 1
**                                        Backplane - 0 to 31
**                                        PIF       - 0 to 7
**                  valid           - Validity of context:
**                                        HYPHY20G_VALID_VAL_32BIT - valid
**                                        Others - invalid
**                  eqTap1          - EQ_TAP_1 value
**                                    Legal range 0x0 to 0xF
**                  eqTap2          - EQ_TAP_2 value
**                                    Legal range 0x0 to 0xF
**                  eqTap3          - EQ_TAP_3 value
**                                    Legal range 0x0 to 0xF
**                  timingEqTap     - TIMING_EQ_TAP value
**                                    Legal range 0x0 to 0xF
**                  setMsk          - Mask of fields to update
**                                        bit 0: valid
**                                        bit 1: eqTap1
**                                        bit 2: eqTap2
**                                        bit 3: eqTap3
**                                        bit 4: timingEqTap
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesSetMskEqCtxt(struct file *fileHndl, UINT4 intf,
                                      UINT4 linkID, UINT4 valid, UINT4 eqTap1,
                                      UINT4 eqTap2, UINT4 eqTap3,
                                      UINT4 timingEqTap, UINT4 setMsk)
{
    UINT4 devValid, devEqTap1, devEqTap2, devEqTap3, devTimingEqTap;
    INT4 result;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7) ||
        (eqTap1 > 0xF) || (eqTap2 > 0xF) || (eqTap3 > 0xF) ||
        (timingEqTap > 0xF) || (setMsk > 0x1F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = serdesHyPhy20gSerdesGetEqCtxt(fileHndl, intf, linkID, &devValid,
                                           &devEqTap1, &devEqTap2, &devEqTap3,
                                           &devTimingEqTap);
    if (result) return result;

    /* update arguments based on setMsk */
    if (setMsk & 0x1) {
        devValid = valid;
    }
    if (setMsk & 0x2) {
        devEqTap1 = eqTap1;
    }
    if (setMsk & 0x4) {
        devEqTap2 = eqTap2;
    }
    if (setMsk & 0x8) {
        devEqTap3 = eqTap3;
    }
    if (setMsk & 0x10) {
        devTimingEqTap = timingEqTap;
    }

    /* update context settings */
    result = serdesHyPhy20gSerdesSetEqCtxt(fileHndl, intf, linkID, devValid,
                                           devEqTap1, devEqTap2, devEqTap3,
                                           devTimingEqTap);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* serdesHyPhy20gSerdesSetMskEqCtxt */

/*******************************************************************************
**
**  utilHyPhy20gXfiSerdesADCCalibrate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calibration procedure for correcting ADC offsets.
**                  Improves jitter performance if calibration is successfull.
**                  Does not affect datapath if calibration fails, datapath is
**                  still usable with higher jitter.
**
**
**  INPUTS:         fileHndl  - device handle
**                  serdesGrp -
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gXfiSerdesADCCalibrate(struct file *fileHndl, UINT4 linkID)
{
    /* local variables */
    INT4 result;
    UINT4 x, y;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 bitmsk;
    UINT4 reg_val;
    UINT4 cal_count;
    UINT4 consistency;
    UINT4 pisoenb_val;
    UINT4 adsmintdiv_val;
    UINT4 curr_offset_val;
    UINT4 accum_ref_val;
    UINT4 sum_offsets, systematic_offset, divider, remainder;
    UINT4 offset[HYPHY20G_SERDES_NPHASES_XFI][HYPHY20G_SERDES_NCOMPS_XFI];
    UINT4 ref[HYPHY20G_SERDES_NCOMPS_XFI];

    /* argument checking */
    if (linkID > HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    result = 0;
    x = 0;
    y = 0;
    addr = 0;
    bitoffset = 0;
    bitmsk = 0;
    reg_val = 0;
    cal_count = 0;
    consistency = 0;
    pisoenb_val = 0;
    adsmintdiv_val = 0;
    curr_offset_val = 0;
    accum_ref_val = 0;
    sum_offsets = 0;
    systematic_offset = 0;
    divider = 0;
    remainder = 0;
    for (x=0; x < HYPHY20G_SERDES_NPHASES_XFI; x++) {
        for (y=0; y < HYPHY20G_SERDES_NCOMPS_XFI; y++) {
            offset[x][y] = 0;
        }
    }
    DBG_PRINT("%s, %s, %d, entering XfiADCCalibrate \n",__FILE__, __FUNCTION__, __LINE__);

    /* read and store current piso_enb bit to restore later */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_ENB_OFF;
    result = sysHyPhy20gBitRead(fileHndl, addr, bitoffset, &pisoenb_val);
    if (result) return result;

    /* put piso in rst */
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* read and store current adsm_int_div value to restore later */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_12, linkID);
    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;
    adsmintdiv_val = mHYPHY20G_GET_FIELD4(reg_val,
                                          HYPHY20G_SERDES_XFI_CORE_BIT_CFG_12_ADSM_INT_DIV_MSK,
                                          HYPHY20G_SERDES_XFI_CORE_BIT_CFG_12_ADSM_INT_DIV_OFF);

    /* put a freq offset into the ADSM of ADSM_INT_DIV-1 */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_12, linkID);
    bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_12_ADSM_INT_DIV_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, (adsmintdiv_val - 1));
    if (result) return result;

    /* toggle ADSM_RSTB */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_5USEC);
    if (result) return result;

    /* repeat calibration until consistent or max tries */
    while ((consistency == 0) && (cal_count <= HYPHY20G_SERDES_ADC_CAL_COUNT_MAX)) {
        DBG_PRINT("%s, %s, %d, consistency = %u, calibration count = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (consistency), (unsigned int) (cal_count));

        /* begin actual calibration */
        for (x=0; x < HYPHY20G_SERDES_NPHASES_XFI; x++) {
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_2, linkID);
            bitmsk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_2_SIPO_BIST_SEL_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, (HYPHY20G_SERDES_NPHASES_XFI - (x + 1)));
            if (result) return result;

            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
            bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_CAL_PHASE_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, x);
            if (result) return result;

            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_CALIBRATE_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            DBG_PRINT("%s, %s, %d, Calibrating for nphase = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (x));

            result = sysHyPhy20gMsDelay(48);
            if (result) return result;
        }
        DBG_PRINT("%s, %s, %d, finished actual calibration \n",__FILE__, __FUNCTION__, __LINE__);
        /* end of calibration */


        /* read offsets and do a check against tolerance to make sure calibration is successful */
        consistency = 1;
        /* put ADC into bypass */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_BYPASS_EN_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        for (x=0; x < HYPHY20G_SERDES_NPHASES_XFI; x++) {
            for (y=0; y < HYPHY20G_SERDES_NCOMPS_XFI; y++) {
                /* write addr */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
                bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_A_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, ((x*16) + y));
                if (result) return result;

                /* trigger rd */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
                bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_RD_OFF;
                result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
                if (result) return result;
                result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
                if (result) return result;

                /* read value */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
                result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
                if (result) return result;
                curr_offset_val = mHYPHY20G_GET_FIELD4(reg_val,
                                                       HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_Q_MSK,
                                                       HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_Q_OFF);
                DBG_PRINT("%s, %s, %d, curr offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (curr_offset_val), (unsigned int) (x), (unsigned int) (y));
                DBG_PRINT("%s, %s, %d, prev offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (offset[x][y]), (unsigned int) (x), (unsigned int) (y));

                /* if abs (offset value-curr offset value is > tolerance) */
                if (curr_offset_val > offset[x][y]) {
                    if ((curr_offset_val - offset[x][y]) > HYPHY20G_SERDES_ADC_CAL_REPEAT_TOL) {
                        consistency = 0;
                    }
                } else {
                    if ((offset[x][y] - curr_offset_val) > HYPHY20G_SERDES_ADC_CAL_REPEAT_TOL) {
                        consistency = 0;
                    }
                }

                offset[x][y] = curr_offset_val;
                DBG_PRINT("%s, %s, %d, consistency = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (consistency), (unsigned int) (x), (unsigned int) (y));

            } /* end for y */
        } /* end for x */

        /* exit ADC bypass */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_BYPASS_EN_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* end of offset read check */

        cal_count++;
    } /* end while */

    /* remove freq offset and return to original config */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_12, linkID);
    bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_12_ADSM_INT_DIV_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, adsmintdiv_val);
    if (result) return result;

     /* toggle ADSM_RSTB */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* restore piso_enb bit */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
    bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_ENB_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, pisoenb_val);
    if (result) return result;

    if (consistency == 1) {
        /* calibration was consistent */
        DBG_PRINT("%s, %s, %d, calibration consistent \n",__FILE__, __FUNCTION__, __LINE__);

        /* offset adjustment routine */
        DBG_PRINT("%s, %s, %d, ADC offset adjustment routine \n",__FILE__, __FUNCTION__, __LINE__);

        /* read adc_mode */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_20, linkID);
        result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
        if (result) return result;
        accum_ref_val = mHYPHY20G_GET_FIELD4(reg_val,
                                             HYPHY20G_SERDES_XFI_CORE_BIT_CFG_20_ACCUM_REF_SEL_MSK,
                                             HYPHY20G_SERDES_XFI_CORE_BIT_CFG_20_ACCUM_REF_SEL_OFF);

        switch(accum_ref_val){
            case HYPHY20G_SERDES_ADC_MODE_A:
                /* adc_mode "A" */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_21, linkID);
            break;
            case HYPHY20G_SERDES_ADC_MODE_B:
                /* adc_mode "B" */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_22, linkID);
            break;
            case HYPHY20G_SERDES_ADC_MODE_C:
                /* adc_mode "C" */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_23, linkID);
            break;
            case HYPHY20G_SERDES_ADC_MODE_D:
                /* adc_mode "D" */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_24, linkID);
            break;
            default:
                /* invalid read value */
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }
        /* setup reference */
        result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
        if (result) return result;
        ref[3] = reg_val & 0x000000FF;
        ref[2] = (reg_val & 0x0000FF00) >> 8;
        ref[1] = (reg_val & 0x00FF0000) >> 16;
        ref[0] = (reg_val & 0xFF000000) >> 24;
        DBG_PRINT("%s, %s, %d, ref[0] = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (ref[0]));
        DBG_PRINT("%s, %s, %d, ref[1] = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (ref[1]));
        DBG_PRINT("%s, %s, %d, ref[2] = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (ref[2]));
        DBG_PRINT("%s, %s, %d, ref[3] = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (ref[3]));

        /* calculate systematic offset */
        for (x=0; x < HYPHY20G_SERDES_NPHASES_XFI; x++) {
            for (y=0; y < ((HYPHY20G_SERDES_NCOMPS_XFI-1)/2); y++) {
                /* offsets are 8-bit two's complement */
                offset[x][y] = offset[x][y] + ref[y];
                if (offset[x][y] > 255) {
                    /* convert back to 8 bit two's complement from 32bit UINT4 */
                    offset[x][y] = offset[x][y] - 256;
                }
                offset[x][(HYPHY20G_SERDES_NCOMPS_XFI - (y + 1))] =
                        offset[x][(HYPHY20G_SERDES_NCOMPS_XFI - (y + 1))] - ref[y];
                if (offset[x][(HYPHY20G_SERDES_NCOMPS_XFI - (y + 1))] > 0x7FFFFFFF) {
                    /* convert back to 8 bit two's complement from 32bit UINT4 */
                    offset[x][(HYPHY20G_SERDES_NCOMPS_XFI - (y + 1))] = 256 - ((~offset[x][(HYPHY20G_SERDES_NCOMPS_XFI - (y + 1))])+1);
                }
                DBG_PRINT("%s, %s, %d, offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (offset[x][y]), (unsigned int) (x), (unsigned int) (y));
                DBG_PRINT("%s, %s, %d, offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (offset[x][(HYPHY20G_SERDES_NCOMPS_XFI - (y + 1))]), (unsigned int) (x), (unsigned int) ((HYPHY20G_SERDES_NCOMPS_XFI - (y + 1))));
            }
            for (y=0; y < HYPHY20G_SERDES_NCOMPS_XFI; y++) {
                /* offsets are 8-bit two's complement */
                if (offset[x][y] > 127) {
                    /* offset is negative */
                    sum_offsets = sum_offsets - (((~offset[x][y])&0xFF)+1);
                } else {
                    sum_offsets = sum_offsets + offset[x][y];
                }
                DBG_PRINT("%s, %s, %d, sum_offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (sum_offsets), (unsigned int) (x), (unsigned int) (y));
            }
        }

        /* integer divide and round systematic offset */
        divider = HYPHY20G_SERDES_NPHASES_XFI * HYPHY20G_SERDES_NCOMPS_XFI;
        systematic_offset = sum_offsets*1000;
        systematic_offset = systematic_offset/divider;

        remainder = systematic_offset % 1000;

        systematic_offset = systematic_offset/1000;

        if (remainder >= 500) {
            /* round up due to integer divide */
            systematic_offset = systematic_offset+1;
        }
        DBG_PRINT("%s, %s, %d, systematic offset = %u \n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (systematic_offset));


        /* subtract systematic offset from all */
        for (x=0; x < HYPHY20G_SERDES_NPHASES_XFI; x++) {
            for (y=0; y < HYPHY20G_SERDES_NCOMPS_XFI; y++) {
                /* offsets are 8-bit two's complement */
                offset[x][y] = offset[x][y]-systematic_offset;
                if (offset[x][y] > 0x7FFFFFFF) {
                    /* convert back to 8 bit two's complement from 32bit UINT4 */
                    offset[x][y] = 256 - ((~offset[x][y])+1);
                }
                DBG_PRINT("%s, %s, %d, offset-systematic_offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (offset[x][y]), (unsigned int) (x), (unsigned int) (y));

            }
        }

        /* write the new offset value into adc */
        /* put ADC into bypass */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_BYPASS_EN_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* read adc_mode */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_20, linkID);
        result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
        if (result) return result;
        accum_ref_val = mHYPHY20G_GET_FIELD4(reg_val,
                                             HYPHY20G_SERDES_XFI_CORE_BIT_CFG_20_ACCUM_REF_SEL_MSK,
                                             HYPHY20G_SERDES_XFI_CORE_BIT_CFG_20_ACCUM_REF_SEL_OFF);

        switch(accum_ref_val){
            case HYPHY20G_SERDES_ADC_MODE_A:
                /* adc_mode "A" */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_21, linkID);
            break;
            case HYPHY20G_SERDES_ADC_MODE_B:
                /* adc_mode "B" */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_22, linkID);
            break;
            case HYPHY20G_SERDES_ADC_MODE_C:
                /* adc_mode "C" */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_23, linkID);
            break;
            case HYPHY20G_SERDES_ADC_MODE_D:
                /* adc_mode "D" */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_24, linkID);
            break;
            default:
                /* invalid read value */
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }
        /* setup reference */
        result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
        if (result) return result;
        ref[3] = reg_val & 0x000000FF;
        ref[2] = (reg_val & 0x0000FF00) >> 8;
        ref[1] = (reg_val & 0x00FF0000) >> 16;
        ref[0] = (reg_val & 0xFF000000) >> 24;

        for (x=0; x < HYPHY20G_SERDES_NPHASES_XFI; x++) {
            for (y=0; y < ((HYPHY20G_SERDES_NCOMPS_XFI-1)/2); y++) {
                /* write addr */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
                bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_A_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, ((x*16) + y));
                if (result) return result;

                /* write data */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
                bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_D_MSK;
                reg_val = offset[x][y] - ref[y];
                if (reg_val > 0x7FFFFFFF) {
                        /* convert back to 8 bit two's complement from 32bit UINT4 */
                        reg_val = 256 - ((~reg_val)+1);
                }
                result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, reg_val);
                if (result) return result;
                DBG_PRINT("%s, %s, %d, updated offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (reg_val), (unsigned int) (x), (unsigned int) (y));

                /* trigger write */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
                bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_WR_OFF;
                result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
                if (result) return result;
                result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
                if (result) return result;

                /* write addr */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
                bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_A_MSK;
                result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, ((x*16) + (HYPHY20G_SERDES_NCOMPS_XFI - (y + 1))));
                if (result) return result;

                /* write data */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
                bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_D_MSK;
                reg_val = offset[x][(HYPHY20G_SERDES_NCOMPS_XFI - (y + 1))] + ref[y];
                if (reg_val > 255) {
                        /* convert back to 8 bit two's complement from 32bit UINT4 */
                        reg_val = reg_val - 256;
                }
                result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, reg_val);
                if (result) return result;
                DBG_PRINT("%s, %s, %d, updated offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (reg_val), (unsigned int) (x), (unsigned int) (HYPHY20G_SERDES_NCOMPS_XFI - (y + 1)));

                /* trigger write */
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
                bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_WR_OFF;
                result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
                if (result) return result;
                result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
                if (result) return result;
            }

            y = (HYPHY20G_SERDES_NCOMPS_XFI - 1)/2;

            /* write addr */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
            bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_A_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, ((x*16) + y));
            if (result) return result;

            /* write data */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
            bitmsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_D_MSK;
            result = hyPhy20gRegFieldWrite(fileHndl, addr, bitmsk, (offset[x][y]));
            if (result) return result;
            DBG_PRINT("%s, %s, %d, updated offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (offset[x][y]), (unsigned int) (x), (unsigned int) (y));

            /* trigger write */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_WR_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;
        }

        /* exit ADC bypass */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
        bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_BYPASS_EN_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;
    } else {
        /* calibration failed */
        DBG_PRINT("%s, %s, %d, calibration failed \n",__FILE__, __FUNCTION__, __LINE__);

        /* reset offset array */
        for (x=0; x < HYPHY20G_SERDES_NPHASES_XFI; x++) {
            for (y=0; y < HYPHY20G_SERDES_NCOMPS_XFI; y++) {
                offset[x][y] = 0;
            }
        }
    } /* end calibration failed */


    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gXfiSerdesADCCalibrate */


/*******************************************************************************
**
**  utilHyPhy20gSfpSerdesCsuCal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    None
**
**
**
**  INPUTS:         fileHndl  - device handle
**                  serdesGrp -
**                  serdesLnkID -
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSfpSerdesCsuCal(struct file *fileHndl, UINT4 linkID,
                                 UINT4 *freq)
{
    /* local variables */
    INT4 y;
    INT4 result;
    UINT4 addr = 0;
    UINT4 bitoffset = 0;
    UINT4 reg_msk = 0;
    UINT4 bit_val;
    UINT4 reg_val, curr_count;
    UINT4 prev_count_diff;
    UINT4 curr_count_diff;
    UINT4 curr_freq;
    UINT4 opt_freq;
    UINT4 found_opt;
    UINT4 adsm_clk_cnt_max;
    UINT4 repeat;
    UINT4 curr_count_chk;
    UINT4 sysclk_consist;

    /* argument checking */
    if ((linkID > HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM) && (linkID != 17)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* extract ADSM_CLK_CNT_MAX setting */
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_12, linkID);
    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;
    adsm_clk_cnt_max = mHYPHY20G_GET_FIELD4(reg_val, HYPHY20G_SERDES_SFP_CORE_BIT_CFG_12_ADSM_CLK_CNT_MAX_MSK,
                                            HYPHY20G_SERDES_SFP_CORE_BIT_CFG_12_ADSM_CLK_CNT_MAX_OFF);

    /* initialize local variables */
    y = 0;
    bit_val = 0;
    reg_val = 0;
    curr_count = 0;
    prev_count_diff = 4000;
    curr_count_diff = adsm_clk_cnt_max;
    curr_freq = 0;
    opt_freq = 0;
    found_opt = 0;
    repeat = 0;
    curr_count_chk = 0;
    sysclk_consist = 0;

    /* loop to repeat calibration loop if it fails the first time */
    while (repeat <= 1) {
        /* repeat for 0b000000 to 0b111111 binary search for vco center freq */
        while (curr_freq <= 63 && found_opt == 0) {
            /* reset ADSM */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_10USEC);
            if (result) return result;

            /* set CSU_MODE[15:10] - VCO center freq */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
            reg_msk = (0x003F << 10);
            result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, curr_freq);
            if (result) return result;

            result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_10USEC);
            if (result) return result;

            /* enable ADSM */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* reset ADSM sysclk cnt valid indicator */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_11, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_SYSCLK_RD_ACK_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            /* not self-clearing so clear for next use */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_11, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_SYSCLK_RD_ACK_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            /* poll ADSM sysclk cnt valid for "1" for a max of 8000 polls */
            y = 0;
            bit_val = 0;
            while ((y <= HYPHY20G_SERDES_MAX_SYSCLK_POLL) && (bit_val != 0x1)) {
                addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_11, linkID);
                bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_SYSCLK_CNT_VALID_OFF;
                result = sysHyPhy20gBitRead(fileHndl, addr, bitoffset, &bit_val);
                if (result) return result;

                y++; /* increment poll loop counter */
            } /* end while */

            /* repeat to ensure a valid clk indicator */
            /* reset ADSM sysclk cnt valid indicator */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_11, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_SYSCLK_RD_ACK_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            /* not self-clearing so clear for next use */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_11, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_SYSCLK_RD_ACK_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            /* poll ADSM sysclk cnt valid for "1" for a max of 8000 polls */
            y = 0;
            bit_val = 0;
            curr_count = 0;
            while ((y <= HYPHY20G_SERDES_MAX_SYSCLK_POLL) && (bit_val != 0x1)) {
                addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_11, linkID);
                bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_SYSCLK_CNT_VALID_OFF;
                result = sysHyPhy20gBitRead(fileHndl, addr, bitoffset, &bit_val);
                if (result) return result;

                /* if the count is valid, save the closest value to converge the center freq */
                /* algorithm should select the first freq that gives SYSCLK_CNT < CLK_CNT_MAX/2 */
                if (bit_val == 0x1) {
                    sysclk_consist = 0;
                    while (sysclk_consist == 0) {
                        addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CFG_11, linkID);
                        result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
                        if (result) return result;
                        curr_count = mHYPHY20G_GET_FIELD4(reg_val, HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_SYSCLK_CNT_MSK,
                                                         HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_SYSCLK_CNT_OFF);
                        DBG_PRINT("%s, %s, %d, vco = %u, sysclk count = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) curr_freq, (unsigned int) curr_count);

                        /* read again to ensure consistent */
                        result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
                        if (result) return result;
                        curr_count_chk = mHYPHY20G_GET_FIELD4(reg_val, HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_SYSCLK_CNT_MSK,
                                                         HYPHY20G_SERDES_SFP_CORE_BIT_CFG_11_ADSM_SYSCLK_CNT_OFF);
                        DBG_PRINT("%s, %s, %d, vco = %u, sysclk count check = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) curr_freq, (unsigned int) curr_count_chk);

                        if ((curr_count - curr_count_chk) == 0) {
                            sysclk_consist = 1;
                        }
                    } /* end while */

                    /* calculate the count difference */
                    if (curr_count > (adsm_clk_cnt_max/2)) {
                        curr_count_diff = adsm_clk_cnt_max - curr_count;
                        DBG_PRINT("%s, %s, %d, counter difference is = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) curr_count_diff);
                    } else {
                        curr_count_diff = curr_count;
                        DBG_PRINT("%s, %s, %d, counter difference is = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) curr_count_diff);
                    }

                    if (prev_count_diff > curr_count_diff) {
                        prev_count_diff = curr_count_diff;
                    } else {
                        if (found_opt == 0) {
                            opt_freq = curr_freq - 1;
                            found_opt = 1;
                            DBG_PRINT("%s, %s, %d, Found optimal freq = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) opt_freq);
                        }
                    }
                } /* end if for bit_val */

                y++;

            } /* end while */

            curr_freq++; /* increment binary search */
        } /* end while */

        /* return the optimal setting if found, otherwise repeat calibration */
        if (found_opt == 1) {
            if ((curr_count != 0) && ((opt_freq != 0) && (opt_freq != 63))) {
                *freq = opt_freq;
                DBG_PRINT("%s, %s, %d, optimal vco setting was found! \n",__FILE__, __FUNCTION__, __LINE__);
                repeat = repeat + 2; /* skip out of repeat loop */
            } else {
                DBG_PRINT("%s, %s, %d, optimal vco setting was found as 0 or 63 \n",__FILE__, __FUNCTION__, __LINE__);
                DBG_PRINT("%s, %s, %d, repeating calibration \n",__FILE__, __FUNCTION__, __LINE__);
                *freq = 0;
                /* reset variables */
                found_opt = 0;
                curr_count = 0;
                prev_count_diff = 4000;
                curr_count_diff = adsm_clk_cnt_max;
                curr_freq = 0;
                repeat++;

                DBG_PRINT("%s, %s, %d, curr_count was found as 0 toggle dig_csu_loop_rstb \n",__FILE__, __FUNCTION__, __LINE__);
                addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
                bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_DIG_CSU_LOOP_RSTB_OFF;
                result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
                if (result) return result;
                result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
                if (result) return result;

            }
        } else {
            DBG_PRINT("%s, %s, %d, optimal vco setting not found \n",__FILE__, __FUNCTION__, __LINE__);
            DBG_PRINT("%s, %s, %d, repeating calibration \n",__FILE__, __FUNCTION__, __LINE__);
            *freq = 0;
            /* reset variables */
            curr_count = 0;
            prev_count_diff = 4000;
            curr_count_diff = adsm_clk_cnt_max;
            curr_freq = 0;
            repeat++;
        }
    } /* end of repeat loop */

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSfpSerdesCsuCal */

/*******************************************************************************
**
**  utilHyPhy20gXfiSerdesCsuCal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    None
**
**
**
**  INPUTS:         fileHndl  - device handle
**                              serdesGrp -
**                              serdesLnkID -
**                              freq -
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gXfiSerdesCsuCal(struct file *fileHndl, UINT4 linkID,
                                 UINT4 *freq)
{
    /* local variables */
    INT4 y;
    INT4 result;
    UINT4 addr = 0;
    UINT4 bitoffset = 0;
    UINT4 reg_msk = 0;
    UINT4 bit_val;
    UINT4 reg_val, curr_count;
    UINT4 prev_count_diff;
    UINT4 curr_count_diff;
    UINT4 curr_freq;
    UINT4 opt_freq;
    UINT4 found_opt;
    UINT4 adsm_clk_cnt_max;
    UINT4 repeat;
    UINT4 curr_count_chk;
    UINT4 sysclk_consist;

    /* argument checking */
    if (linkID > HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* extract ADSM_CLK_CNT_MAX setting */
    addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_14, linkID);
    result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
    if (result) return result;
    adsm_clk_cnt_max = mHYPHY20G_GET_FIELD4(reg_val, HYPHY20G_SERDES_XFI_CORE_BIT_CFG_14_ADSM_CLK_CNT_MAX_MSK,
                                            HYPHY20G_SERDES_XFI_CORE_BIT_CFG_14_ADSM_CLK_CNT_MAX_OFF);


    /* initialize local variables */
    y = 0;
    bit_val = 0;
    reg_val = 0;
    curr_count = 0;
    prev_count_diff = 4000;
    curr_count_diff = adsm_clk_cnt_max;
    curr_freq = 0;
    opt_freq = 0;
    found_opt = 0;
    repeat = 0;
    curr_count_chk = 0;
    sysclk_consist = 0;

    /* loop to repeat calibration loop if it fails the first time */
    while (repeat <= 1) {
        /* repeat for 0b000000 to 0b111111 binary search for vco center freq */
        while (curr_freq <= 63 && found_opt == 0) {
            /* reset ADSM */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_10USEC);
            if (result) return result;

            /* set CSU_MODE[15:10] - VCO center freq */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_9, linkID);
            reg_msk = (0x003F << 10);
            result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, curr_freq);
            if (result) return result;

            result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_10USEC);
            if (result) return result;

            /* enable ADSM */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* reset ADSM sysclk cnt valid indicator */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_13, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_SYSCLK_RD_ACK_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            /* not self-clearing so clear for next use */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_13, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_SYSCLK_RD_ACK_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            /* poll ADSM sysclk cnt valid for "1" for a max of 8000 polls */
            y = 0;
            bit_val = 0;
            while ((y <= HYPHY20G_SERDES_MAX_SYSCLK_POLL) && (bit_val != 0x1)) {
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_13, linkID);
                bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_SYSCLK_CNT_VALID_OFF;
                result = sysHyPhy20gBitRead(fileHndl, addr, bitoffset, &bit_val);
                if (result) return result;

                y++; /* increment poll loop counter */
            } /* end while */

            /* repeat to ensure a valid clk indicator */
            /* reset ADSM sysclk cnt valid indicator */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_13, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_SYSCLK_RD_ACK_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            /* not self-clearing so clear for next use */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_13, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_SYSCLK_RD_ACK_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            /* poll ADSM sysclk cnt valid for "1" for a max of 8000 polls */
            y = 0;
            bit_val = 0;
            curr_count = 0;
            while ((y <= HYPHY20G_SERDES_MAX_SYSCLK_POLL) && (bit_val != 0x1)) {
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_13, linkID);
                bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_SYSCLK_CNT_VALID_OFF;
                result = sysHyPhy20gBitRead(fileHndl, addr, bitoffset, &bit_val);
                if (result) return result;

                /* if the count is valid, save the closest value to converge the center freq */
                /* algorithm should select the first freq that gives SYSCLK_CNT < CLK_CNT_MAX/2 */
                if (bit_val == 0x1) {
                    sysclk_consist = 0;
                    while (sysclk_consist == 0) {
                        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_13, linkID);
                        result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
                        if (result) return result;
                        curr_count = mHYPHY20G_GET_FIELD4(reg_val, HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_SYSCLK_CNT_MSK,
                                                          HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_SYSCLK_CNT_OFF);
                        /* DBG_PRINT("%s, %s, %d, vco = %u, sysclk count = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) *(curr_freq), (unsigned int) *(curr_count));   */

                        /* read again to ensure consistent */
                        result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
                        if (result) return result;
                        curr_count_chk = mHYPHY20G_GET_FIELD4(reg_val, HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_SYSCLK_CNT_MSK,
                                                          HYPHY20G_SERDES_XFI_CORE_BIT_CFG_13_ADSM_SYSCLK_CNT_OFF);
                        DBG_PRINT("%s, %s, %d, vco = %u, sysclk count check = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) curr_freq, (unsigned int) curr_count_chk);
                        if ((curr_count - curr_count_chk) == 0) {
                            sysclk_consist = 1;
                        }
                    } /* end while */

                    /* calculate the count difference */
                    if (curr_count > (adsm_clk_cnt_max/2)) {
                        curr_count_diff = adsm_clk_cnt_max - curr_count;
                        DBG_PRINT("%s, %s, %d, counter difference is = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)curr_count_diff);
                    } else {
                        curr_count_diff = curr_count;
                        DBG_PRINT("%s, %s, %d, counter difference is = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)curr_count_diff);
                    }

                    if (prev_count_diff > curr_count_diff) {
                        prev_count_diff = curr_count_diff;
                    } else {
                        if (found_opt == 0) {
                            if(curr_freq > 0){ // avoid getting a negative value
                                opt_freq = curr_freq - 1;
                            }
                            found_opt = 1;
                            DBG_PRINT("%s, %s, %d, Found optimal freq = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)curr_freq);
                        }
                    }
                } /* end if for bit_val */

                y++;

            } /* end while */

            curr_freq++; /* increment binary search */
        } /* end while */


        /* return the optimal setting if found, otherwise */
        if (found_opt == 1) {
            if ((curr_count != 0) && ((opt_freq != 0) && (opt_freq != 63))) {
                *freq = opt_freq;
                DBG_PRINT("%s, %s, %d, optimal xfi vco setting was found! \n",__FILE__, __FUNCTION__, __LINE__);
                repeat = repeat + 2; /* skip out of repeat loop */
            } else {
                DBG_PRINT("%s, %s, %d, optimal vco setting was found as 0 or 63 \n",__FILE__, __FUNCTION__, __LINE__);
                DBG_PRINT("%s, %s, %d, repeating calibration \n",__FILE__, __FUNCTION__, __LINE__);
                *freq = 0;
                /* reset variables */
                found_opt = 0;
                curr_count = 0;
                prev_count_diff = 4000;
                curr_count_diff = adsm_clk_cnt_max;
                curr_freq = 0;
                repeat++;

                DBG_PRINT("%s, %s, %d, curr_count was found as 0 toggle dig_csu_loop_rstb \n",__FILE__, __FUNCTION__, __LINE__);
                addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_7, linkID);
                bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_7_DIG_CSU_LOOP_RSTB_OFF;
                result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
                if (result) return result;
                result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
                if (result) return result;

            }
        } else {
            DBG_PRINT("%s, %s, %d, optimal xfi vco setting not found \n",__FILE__, __FUNCTION__, __LINE__);
            DBG_PRINT("%s, %s, %d, repeating calibration \n",__FILE__, __FUNCTION__, __LINE__);
            *freq = 0;
            /* reset variables */
            curr_count = 0;
            prev_count_diff = 4000;
            curr_count_diff = adsm_clk_cnt_max;
            curr_freq = 0;
            repeat++;
        }
    } /* end while repeat <=1 */

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gXfiSerdesCsuCal */

/*******************************************************************************
**
**  utilHyPhy20gBackplaneSerdesCsuCal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    None
**
**
**
**  INPUTS:         fileHndl  - device handle
**                              serdesGrp -
**                              serdesLnkID -
**                              freq -
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gBackplaneSerdesCsuCal(struct file *fileHndl, UINT4 csu_id,
                                       UINT4 *freq)
{
    /* local variables */
    INT4 y;
    INT4 result;
    UINT4 addr = 0;
    UINT4 bitoffset = 0;
    UINT4 reg_msk = 0;
    UINT4 bit_val;
    UINT4 reg_val, curr_count;
    UINT4 prev_count_diff;
    UINT4 curr_count_diff;
    UINT4 curr_freq;
    UINT4 opt_freq;
    UINT4 found_opt;

    /* argument checking */
    if (csu_id > HYPHY20G_SERDES_GRP_BACKPLANE_CSU_ID_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    y = 0;
    bit_val = 0;
    reg_val = 0;
    curr_count = 0;
    prev_count_diff = 4000;
    curr_count_diff = HYPHY20G_SERDES_BACKPLANE_ADSM_CLK_CNT_MAX_VAL;
    curr_freq = 0;
    opt_freq = 0;
    found_opt = 0;

    /* repeat for 0b000000 to 0b111111 binary search for vco center freq */
    while (curr_freq <= 63 && found_opt == 0) {
        /* reset ADSM */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_10, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_10USEC);
        if (result) return result;

        /* set CSU_MODE[15:10] - VCO center freq */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_3, csu_id);
        reg_msk = (0x003F << 10);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, curr_freq);
        if (result) return result;

        result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_10USEC);
        if (result) return result;

        /* enable ADSM */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_10, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* reset ADSM sysclk cnt valid indicator */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_7, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_RD_ACK_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* not self-clearing so clear for next use */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_7, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_RD_ACK_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* poll ADSM sysclk cnt valid for "1" for a max of 8000 polls */
        y = 0;
        bit_val = 0;
        while ((y <= HYPHY20G_SERDES_MAX_SYSCLK_POLL) && (bit_val != 0x1)) {
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_7, csu_id);
            bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_CNT_VALID_OFF;
            result = sysHyPhy20gBitRead(fileHndl, addr, bitoffset, &bit_val);
            if (result) return result;

            y++; /* increment poll loop counter */
        } /* end while */

        /* repeat to ensure a valid clk indicator */
        /* reset ADSM sysclk cnt valid indicator */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_7, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_RD_ACK_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* not self-clearing so clear for next use */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_7, csu_id);
        bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_RD_ACK_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* poll ADSM sysclk cnt valid for "1" for a max of 8000 polls */
        y = 0;
        bit_val = 0;
        curr_count = 0;
        while ((y <= HYPHY20G_SERDES_MAX_SYSCLK_POLL) && (bit_val != 0x1)) {
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_7, csu_id);
            bitoffset = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_CNT_VALID_OFF;
            result = sysHyPhy20gBitRead(fileHndl, addr, bitoffset, &bit_val);
            if (result) return result;

            /* if the count is valid, save the closest value to converge the center freq */
            /* algorithm should select the first freq that gives SYSCLK_CNT < CLK_CNT_MAX/2 */
            if (bit_val == 0x1) {
                addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_7, csu_id);
                result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
                if (result) return result;
                curr_count = mHYPHY20G_GET_FIELD4(reg_val, HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_CNT_MSK,
                                                 HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_CNT_OFF);
                /* DBG_PRINT("%s, %s, %d, vco = %u, sysclk count = %u\n",__FILE__, __FUNCTION__, __LINE__, *(curr_freq), *(curr_count)); */

                /* calculate the count difference */
                if (curr_count > (HYPHY20G_SERDES_BACKPLANE_ADSM_CLK_CNT_MAX_VAL/2)) {
                    curr_count_diff = HYPHY20G_SERDES_BACKPLANE_ADSM_CLK_CNT_MAX_VAL - curr_count;
                    /* DBG_PRINT("%s, %s, %d, counter difference is = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) *(curr_count_diff));  */
                } else {
                    curr_count_diff = curr_count;
                    /* DBG_PRINT("%s, %s, %d, counter difference is = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) *(curr_count_diff));  */
                }

                if (prev_count_diff > curr_count_diff) {
                    prev_count_diff = curr_count_diff;
                } else {
                    if (found_opt == 0) {
                        opt_freq = curr_freq - 1;
                        found_opt = 1;
                        /* DBG_PRINT("%s, %s, %d, Found optimal freq = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) *(curr_freq));  */
                    }
                }
            } /* end if for bit_val */

            y++;

        } /* end while */

    curr_freq++; /* increment binary search */
    } /* end while */

    /* return the optimal setting if found, otherwise */
    if (found_opt == 1) {
        *freq = opt_freq;
        DBG_PRINT("%s, %s, %d, optimal vco setting was found! \n",__FILE__, __FUNCTION__, __LINE__);
    } else {
        DBG_PRINT("%s, %s, %d, optimal vco setting not found \n",__FILE__, __FUNCTION__, __LINE__);
        *freq = 64;
    }
    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gBackplaneSerdesCsuCal */


/*******************************************************************************
**
**  utilHyPhy20gPifSerdesCsuCal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    None
**
**
**
**  INPUTS:         fileHndl  - device handle
**                              serdesGrp -
**                              serdesLnkID -
**                              freq -
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gPifSerdesCsuCal(struct file *fileHndl, UINT4 *freq)
{
    /* local variables */
    INT4 y;
    INT4 result;
    UINT4 addr = 0;
    UINT4 bitoffset = 0;
    UINT4 reg_msk = 0;
    UINT4 bit_val;
    UINT4 reg_val, curr_count;
    UINT4 prev_count_diff;
    UINT4 curr_count_diff;
    UINT4 curr_freq;
    UINT4 opt_freq;
    UINT4 found_opt;

    /* argument checking */

    /* initialize local variables */
    y = 0;
    bit_val = 0;
    reg_val = 0;
    curr_count = 0;
    prev_count_diff = 4000;
    curr_count_diff = HYPHY20G_SERDES_PIF_ADSM_CLK_CNT_MAX_VAL;
    curr_freq = 0;
    opt_freq = 0;
    found_opt = 0;

    /* repeat for 0b000000 to 0b111111 binary search for vco center freq */
    while (curr_freq <= 63 && found_opt == 0) {
        /* reset ADSM */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_10;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_10USEC);
        if (result) return result;

        /* set CSU_MODE[15:10] - VCO center freq */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_3;
        reg_msk = (0x003F << 10);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, curr_freq);
        if (result) return result;

        result = sysHyPhy20gUsDelay(HYPHY20G_DELAY_10USEC);
        if (result) return result;

        /* enable ADSM */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_10;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* reset ADSM sysclk cnt valid indicator */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_7;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_RD_ACK_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* not self-clearing so clear for next use */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_7;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_RD_ACK_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* poll ADSM sysclk cnt valid for "1" for a max of 8000 polls */
        y = 0;
        bit_val = 0;
        while ((y <= HYPHY20G_SERDES_MAX_SYSCLK_POLL) && (bit_val != 0x1)) {
            addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_7;
            bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_CNT_VALID_OFF;
            result = sysHyPhy20gBitRead(fileHndl, addr, bitoffset, &bit_val);
            if (result) return result;

            y++; /* increment poll loop counter */
        } /* end while */

        /* repeat to ensure a valid clk indicator */
        /* reset ADSM sysclk cnt valid indicator */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_7;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_RD_ACK_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* not self-clearing so clear for next use */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_7;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_RD_ACK_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        /* poll ADSM sysclk cnt valid for "1" for a max of 8000 polls */
        y = 0;
        bit_val = 0;
        curr_count = 0;
        while ((y <= HYPHY20G_SERDES_MAX_SYSCLK_POLL) && (bit_val != 0x1)) {
            addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_7;
            bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_CNT_VALID_OFF;
            result = sysHyPhy20gBitRead(fileHndl, addr, bitoffset, &bit_val);
            if (result) return result;

            /* if the count is valid, save the closest value to converge the center freq */
            /* algorithm should select the first freq that gives SYSCLK_CNT < CLK_CNT_MAX/2 */
            if (bit_val == 0x1) {
                addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_7;
                result = sysHyPhy20gRead(fileHndl, addr, &reg_val);
                if (result) return result;
                curr_count = mHYPHY20G_GET_FIELD4(reg_val, HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_CNT_MSK,
                                                 HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_7_ADSM_SYSCLK_CNT_OFF);
                /* DBG_PRINT("%s, %s, %d, vco = %u, sysclk count = %u\n",__FILE__, __FUNCTION__, __LINE__, *(curr_freq), *(curr_count)); */

                /* calculate the count difference */
                if (curr_count > (HYPHY20G_SERDES_PIF_ADSM_CLK_CNT_MAX_VAL/2)) {
                    curr_count_diff = HYPHY20G_SERDES_PIF_ADSM_CLK_CNT_MAX_VAL - curr_count;
                    /* DBG_PRINT("%s, %s, %d, counter difference is = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) *(curr_count_diff));  */
                } else {
                    curr_count_diff = curr_count;
                    /* DBG_PRINT("%s, %s, %d, counter difference is = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) *(curr_count_diff));  */
                }

                if (prev_count_diff > curr_count_diff) {
                    prev_count_diff = curr_count_diff;
                } else {
                    if (found_opt == 0) {
                        opt_freq = curr_freq - 1;
                        found_opt = 1;
                        /* DBG_PRINT("%s, %s, %d, Found optimal freq = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) *(curr_freq));  */
                    }
                }
            } /* end if for bit_val */

            y++;

        } /* end while */

    curr_freq++; /* increment binary search */
    } /* end while */

    /* return the optimal setting if found, otherwise */
    if (found_opt == 1) {
        *freq = opt_freq;
        DBG_PRINT("%s, %s, %d, optimal vco setting was found! \n",__FILE__, __FUNCTION__, __LINE__);
    } else {
        DBG_PRINT("%s, %s, %d, optimal vco setting not found \n",__FILE__, __FUNCTION__, __LINE__);
        *freq = 64;
    }
    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gPifSerdesCsuCal */

/*******************************************************************************
**
**  utilHyPhy20gSerdesRxcpgen
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This util checks to see if the RXCPGEN is locked.  If not
**                  locked, it will run the workaround.
**                  If after the workaround and the RXCPGEN is still not
**                  locked, declare a failure and send back error code.
**
**  INPUTS:         fileHndl  - device handle
**                  linkID -
**                  serdesGrp -
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesRxcpgen(struct file *fileHndl, UINT4 linkID,
                               UINT4 serdesGrp)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 bit_msk;

    /* argument checking */
    if (serdesGrp > HYPHY20G_SERDES_GRP_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if ((serdesGrp == HYPHY20G_SERDES_GRP_SFP) && (linkID > HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((serdesGrp == HYPHY20G_SERDES_GRP_SGMII) && (linkID != 17)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((serdesGrp == HYPHY20G_SERDES_GRP_XFI) && (linkID > HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((serdesGrp == HYPHY20G_SERDES_GRP_PIF) && (linkID > HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((serdesGrp == HYPHY20G_SERDES_GRP_BACKPLANE) && (linkID > HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    addr = 0;
    bitoffset = 0;
    bit_msk = 0;

    /******************************/
    /* SFP and SGMII Workaround   */
    /******************************/
    if ((serdesGrp == HYPHY20G_SERDES_GRP_SFP) || (serdesGrp == HYPHY20G_SERDES_GRP_SGMII)) {
        /* check RXCPGEN lock bit, poll for up to 100 ms */
        addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
        bit_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_LOCK_STATUS_MSK;
        result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);

        /* if locked, no need to run workaround or if dev access, return err */
        if ((result == HYPHY20G_ERR_DEV_ACCESS) || (result == HYPHY20G_SUCCESS)) {
            return result;
        }
        /* if not locked, run workaround routine */
        if (result == HYPHY20G_ERR_POLL_TIMEOUT) {
            /* RXCPGEN_CTRL[3] - disable VCO */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

						addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ENB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

          
            /* Disable ARSTB */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;   	
            	  
    	    /* Disable RSTB */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            /* enable PLL so VCO control voltage goes to max */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* RXCPGEN_CTRL[3] - enable VCO */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

						result = sysHyPhy20gMsDelay (1);
      		  if (result) return result;

            /* VCO control voltage = reset voltage */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* let PLL lock */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* check lock again and return */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bit_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_LOCK_STATUS_MSK;
            result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);
            return result;
        }
    }

    /********************/
    /* XFI Workaround   */
    /********************/
    if (serdesGrp == HYPHY20G_SERDES_GRP_XFI) {
        /* check RXCPGEN lock bit, poll for up to 100 ms */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
        bit_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_LOCK_STATUS_MSK;
        result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);

        /* if locked, no need to run workaround or if dev access, return err */
        if ((result == HYPHY20G_ERR_DEV_ACCESS) || (result == HYPHY20G_SUCCESS)) {
            return result;
        }
        /* if not locked, run workaround routine */
        if (result == HYPHY20G_ERR_POLL_TIMEOUT) {
            /* RXCPGEN_CTRL[3] - disable VCO */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_ENB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            /* enable PLL so VCO control voltage goes to max */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* RXCPGEN_CTRL[3] - enable VCO */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* VCO control voltage = reset voltage */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* let PLL lock */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* check lock again and return */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bit_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_LOCK_STATUS_MSK;
            result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);
            return result;
        }
    }

    /********************/
    /* PIF Workaround   */
    /********************/
    if (serdesGrp == HYPHY20G_SERDES_GRP_PIF) {
        /* check RXCPGEN lock bit, poll for up to 100 ms */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bit_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_LOCK_STATUS_MSK;
        result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);

        /* if locked, no need to run workaround or if dev access, return err */
        if ((result == HYPHY20G_ERR_DEV_ACCESS) || (result == HYPHY20G_SUCCESS)) {
            return result;
        }
        /* if not locked, run workaround routine */
        if (result == HYPHY20G_ERR_POLL_TIMEOUT) {
            /* RXCPGEN_CTRL[3] - disable VCO */
            addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            /* RXCPGEN_ENB - Enable RXCPGEN */      
            addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            /* enable PLL so VCO control voltage goes to max */
            addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* RXCPGEN_CTRL[3] - enable VCO */
            addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* VCO control voltage = reset voltage */
            addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* let PLL lock */
            addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* check lock again and return */
            addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
            bit_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_LOCK_STATUS_MSK;
            result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);
            return result;
        }
    }

    /**************************/
    /* Backplane Workaround   */
    /**************************/
    if (serdesGrp == HYPHY20G_SERDES_GRP_BACKPLANE) {
        /* check RXCPGEN lock bit, poll for up to 100 ms */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bit_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_LOCK_STATUS_MSK;
        result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);

        /* if locked, no need to run workaround or if dev access, return err */
        if ((result == HYPHY20G_ERR_DEV_ACCESS) || (result == HYPHY20G_SUCCESS)) {
            return result;
        }
        /* if not locked, run workaround routine */
        if (result == HYPHY20G_ERR_POLL_TIMEOUT) {
            /** RXCPGEN Initialization started! **/

            /* hold rxpcgen vco - RXCPGEN_CTRL[3] in reset */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            /* hold rxpcgen analog blocks in reset */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            /* hold rxpcgen blocks in reset */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            /* Disable RXCPGEN */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            /* Enable RXCPGEN */       
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* Take rxpcgen blocks out of reset */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;	

            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* Toggle RXCPGEN Analog Reset */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /** RXCPGEN Initalization Completed! **/

            /* check lock again and return */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bit_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_LOCK_STATUS_MSK;
            result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);
            return result;
        }
    }
    /* should never execute this */
    return HYPHY20G_ERR_INVALID_ARG;
} /* utilHyPhy20gSerdesRxcpgen */

/*******************************************************************************
**
**  utilHyPhy20gSerdesRxcpgenForceInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This util checks to see if the RXCPGEN is locked.  If not
**                  locked, it will run the workaround.
**                  If after the workaround and the RXCPGEN is still not
**                  locked, declare a failure and send back error code.
**
**  INPUTS:         fileHndl  - device handle
**                  linkID -
**                  serdesGrp -
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesRxcpgenForceInit(struct file *fileHndl, UINT4 linkID,
                               UINT4 serdesGrp)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 bit_msk;
    UINT4 reg_msk;

    DBG_PRINT("%s, %s, %d, \n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if (serdesGrp > HYPHY20G_SERDES_GRP_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if ((serdesGrp == HYPHY20G_SERDES_GRP_SFP) && (linkID > HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((serdesGrp == HYPHY20G_SERDES_GRP_SGMII) && (linkID != 17)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((serdesGrp == HYPHY20G_SERDES_GRP_XFI) && (linkID > HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((serdesGrp == HYPHY20G_SERDES_GRP_PIF) && (linkID > HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((serdesGrp == HYPHY20G_SERDES_GRP_BACKPLANE) && (linkID > HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize local variables */
    addr = 0;
    bitoffset = 0;
    bit_msk = 0;
    reg_msk = 0;

    /******************************/
    /* SFP and SGMII Workaround   */
    /******************************/
    if ((serdesGrp == HYPHY20G_SERDES_GRP_SFP) || (serdesGrp == HYPHY20G_SERDES_GRP_SGMII)) {
        /* check RXCPGEN lock bit, poll for up to 100 ms */
        addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
        bit_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_LOCK_STATUS_MSK;
        result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);

        /* if locked, no need to run workaround or if dev access, return err */
        if ((result == HYPHY20G_ERR_DEV_ACCESS) || (result == HYPHY20G_SUCCESS)) {
            return result;
        }
        /* if not locked, run workaround routine */
        if (result == HYPHY20G_ERR_POLL_TIMEOUT) {
            /* RXCPGEN_CTRL[3] - disable VCO */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

						addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ENB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

          
            /* Disable ARSTB */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;   	
            	  
    	    /* Disable RSTB */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            /* enable PLL so VCO control voltage goes to max */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* RXCPGEN_CTRL[3] - enable VCO */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

						result = sysHyPhy20gMsDelay (1);
      		  if (result) return result;

            /* VCO control voltage = reset voltage */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* let PLL lock */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bitoffset = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* check lock again and return */
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkID);
            bit_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_LOCK_STATUS_MSK;
            result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);
            return result;
        }
    }

    /********************/
    /* XFI Workaround   */
    /********************/
    if (serdesGrp == HYPHY20G_SERDES_GRP_XFI) {
        /* check RXCPGEN lock bit, poll for up to 100 ms */
        addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
        bit_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_LOCK_STATUS_MSK;
        result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);

        /* if locked, no need to run workaround or if dev access, return err */
        if ((result == HYPHY20G_ERR_DEV_ACCESS) || (result == HYPHY20G_SUCCESS)) {
            return result;
        }
        /* if not locked, run workaround routine */
        if (result == HYPHY20G_ERR_POLL_TIMEOUT) {
            /* RXCPGEN_CTRL[3] - disable VCO */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_ENB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            /* enable PLL so VCO control voltage goes to max */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* RXCPGEN_CTRL[3] - enable VCO */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* VCO control voltage = reset voltage */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* let PLL lock */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bitoffset = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* check lock again and return */
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkID);
            bit_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_LOCK_STATUS_MSK;
            result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);
            return result;
        }
    }

    /********************/
    /* PIF Workaround   */
    /********************/
    if (serdesGrp == HYPHY20G_SERDES_GRP_PIF) {
        /* RXCPGEN_CTRL[3] - disable VCO */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* RXCPGEN_ENB - Enable RXCPGEN */      
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        /* adding delay */
        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* enable PLL so VCO control voltage goes to max */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* RXCPGEN_CTRL[3] - enable VCO */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* VCO control voltage = reset voltage */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        /* let PLL lock */
        addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkID);
        bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

        result = sysHyPhy20gMsDelay (1);
        if (result) return result;

        return HYPHY20G_SUCCESS;
    }

    /**************************/
    /* Backplane Workaround   */
    /**************************/
    if (serdesGrp == HYPHY20G_SERDES_GRP_BACKPLANE) {
        /* check RXCPGEN lock bit, poll for up to 100 ms */
        addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
        bit_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_LOCK_STATUS_MSK;
        result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);

        /* if locked, no need to run workaround or if dev access, return err */
        if ((result == HYPHY20G_ERR_DEV_ACCESS) || (result == HYPHY20G_SUCCESS)) {
            return result;
        }
        /* if not locked, run workaround routine */
        if (result == HYPHY20G_ERR_POLL_TIMEOUT) {
            /** RXCPGEN Initialization started! **/

            /* hold rxpcgen vco - RXCPGEN_CTRL[3] in reset */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            /* hold rxpcgen analog blocks in reset */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            /* hold rxpcgen blocks in reset */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            /* Disable RXCPGEN */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            /* Enable RXCPGEN */       
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* Take rxpcgen blocks out of reset */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;	

            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_CTRL_OFF + 3;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /* Toggle RXCPGEN Analog Reset */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bitoffset = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ARSTB_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
            if (result) return result;

            result = sysHyPhy20gMsDelay (1);
            if (result) return result;

            /** RXCPGEN Initalization Completed! **/

            /* check lock again and return */
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkID);
            bit_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_LOCK_STATUS_MSK;
            result = sysHyPhy20gPollBit(fileHndl, addr, bit_msk, bit_msk, 200000);
            return result;
        }
    }
    /* should never execute this */
    return HYPHY20G_ERR_INVALID_ARG;
} /* utilHyPhy20gSerdesRxcpgenForceInit */

/*******************************************************************************
**
**  utilHyPhy20gSerdesValidateSfpRates
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function checks the data rates of SFP links that have
**                  calReq set to 1 for ADC calibration.  Data rates less
**                  than 1.5Gbps do not need to be calibrated.
**
**                  Mask values:
**                      0 - do not check rate
**                      1 - check rate
**
**  INPUTS:         fileHndl - device handle
**                  linkMsk  - Mask of SFP links to check
**                                 0x0 to 0xFFFF
**                                     bit 0 : SFP #0
**                                     bit 1 : SFP #1
**                                     ..
**                                     bit 15: SFP #15
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SERDES_ADC_SFP_RATE_CHECK
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesValidateSfpRates(struct file *fileHndl, UINT4 linkMsk)
{
    UINT4 reg, val, msk, i;
    UINT4 maxLinks, mskBit, linkID, intf;
    UINT4 adsmIntDiv_val, adsmFracDiv_val, adsmDenom_val;
    UINT4 rxPathDiv_val, csuMode_val;
    UINT4 vco_rate_a, vco_rate_b;
    UINT4 pow2RxPathDiv, rxpcgen_div, dsp_div_1000, hr_div_1000;
    UINT4 line_rate_top, line_rate_bot, line_rate;
    UINT4 dummy, calReq, calMode;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if (linkMsk > 0xFFFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (linkMsk == 0) {
        return HYPHY20G_SUCCESS;
    }

    intf = HYPHY20G_SERDES_SFP_INTF;

    result = utilHyPhy20gSerdesGetMaxLinks(fileHndl, intf, &maxLinks);
    if (result) return result;

    /* Configure links in mask */
    for (linkID = 0; linkID < maxLinks; linkID++) {
        mskBit = (linkMsk >> linkID) & 0x1;

        if (mskBit == 1) {
            /* check link state */
            result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl, intf, linkID,
                                                       &dummy, &calMode,
                                                       &calReq, &dummy, &dummy,
                                                       &dummy);
            if (result) return result;

            if ((calMode != HYPHY20G_SERDES_DYNAMIC_CAL_MODE &&
                calMode != HYPHY20G_SERDES_DEFER_CAL_MODE) || calReq == 0) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }

            if (calReq == 1) {
                /* check sfp rate */
                /* Equation:

                   VCO_RATE = 155.52e6 * (ADSM_INT_DIV[SFP_CSU] +
                              ADSM_FRAC_DIV[SFP_CSU] / ADSM_DENOMIN[SFP_CSU]);
                   RXCPGEN_DIV = min(2^RX_PATH_DIV[SFP_CSU],8)
                   DSP_DIV = 2^RX_PATH_DIV[SFP_CSU] / RXCPGEN_DIV
                   HR_DIV = 2 + CSU_MODE[SFP_CSU][20:19]/2
                   LINE_RATE =  VCO_RATE * 2 / DSP_DIV / RXPCGEN_DIV / 2 / HR_DIV

                   if LINE_RATE < 1.5e9, don't need to calibrate
                */

                reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_SFP_CORE_REG_CFG_10, linkID);
                result = sysHyPhy20gRead(fileHndl, reg, &val);
                if (result) return result;

                /* retrieve ADSM_INT_DIV */
                msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_INT_DIV_MSK;
                result = hyPhy20gVarFieldRead(val, msk, &adsmIntDiv_val);
                if (result) return result;

                /* retrieve ADSM_FRAC_DIV */
                msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_FRAC_DIV_MSK;
                result = hyPhy20gVarFieldRead(val, msk, &adsmFracDiv_val);
                if (result) return result;

                /* retrieve ADSM_DENOMIN */
                reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                                    HYPHY20G_SERDES_SFP_CORE_REG_CFG_9, linkID);
                msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_9_ADSM_DENOMIN_MSK;
                result = hyPhy20gRegFieldRead(fileHndl, reg, msk,
                                              &adsmDenom_val);
                if (result) return result;

                /* retrieve RX_PATH_DIV */
                reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkID);
                msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_RX_PATH_DIV_MSK;
                result = hyPhy20gRegFieldRead(fileHndl, reg, msk,
                                              &rxPathDiv_val);
                if (result) return result;

                /* retrieve CSU_MODE[20:19] */
                reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_SFP_CORE_REG_CONF_7, linkID);
                msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_7_CSU_MODE_MSK;
                result = hyPhy20gRegFieldRead(fileHndl, reg, msk, &val);
                if (result) return result;
                csuMode_val = (val >> 19) & 0x3;
                DBG_PRINT("%s, %s, %d, csuMode_val = %lu\n",__FILE__, __FUNCTION__, __LINE__, csuMode_val);

                vco_rate_a = (adsmFracDiv_val*1000)/adsmDenom_val;
                vco_rate_b = adsmIntDiv_val*1000;
                DBG_PRINT("%s, %s, %d, vco_rate_a = %lu, vco_rate_b = %lu\n",__FILE__, __FUNCTION__, __LINE__, vco_rate_a, vco_rate_b);

                /* make sure we don't overflow UINT4 */
                if (rxPathDiv_val > 22) {
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_VAR_OVERFLOW\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_VAR_OVERFLOW;
                }
                pow2RxPathDiv = 1;
                for (i = 1; i <= rxPathDiv_val; i++) {
                    pow2RxPathDiv = pow2RxPathDiv*2;
                }
                rxpcgen_div = (pow2RxPathDiv < 8) ? pow2RxPathDiv : 8;
                dsp_div_1000 = (pow2RxPathDiv*1000)/rxpcgen_div;
                hr_div_1000 = 2*1000 + (csuMode_val*1000)/2;

                DBG_PRINT("%s, %s, %d, pow2RxPathDiv = %lu, rxpcgen_div = %lu, dsp_div_1000 = %lu, hr_div_1000 = %lu\n",__FILE__, __FUNCTION__, __LINE__, pow2RxPathDiv, rxpcgen_div, dsp_div_1000, hr_div_1000);

                line_rate_top = (vco_rate_a + vco_rate_b)*2*15552;
                line_rate_bot = dsp_div_1000*rxpcgen_div*hr_div_1000/1000;
                line_rate = line_rate_top/line_rate_bot;
                DBG_PRINT("%s, %s, %d, line_rate = %lu\n",__FILE__, __FUNCTION__, __LINE__, line_rate);

                if (line_rate < 150000) {
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_SFP_RATE_CHECK\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_SERDES_ADC_SFP_RATE_CHECK;
                }
            }
        }
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesValidateSfpRates */

/*******************************************************************************
**
**  utilHyPhy20gSerdesPrepCalibration
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Preparation procedure for calibrating the ADC offsets for
**                  a set of links.  Stops the transmitters and forces the CSUs
**                  to be asynchronous.
**
**                  Mask values:
**                      0 - do not prep link for calibration
**                      1 - prep link calibration
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 0 - SFP
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkMsk  - Mask of links to prep calibration
**                                 SFP       - 0x0 to 0x3FFFF
**                                             bit 0 : SFP #0
**                                             bit 1 : SFP #1
**                                             ..
**                                             bit 15: SFP #15
**                                 XFI       - 0x0 to 0x3
**                                             bit 0 : XFI #0
**                                             bit 1 : XFI #1
**                                 Backplane - 0x0 to 0xFFFFFFFF
**                                             bit 0 : link #0
**                                             bit 1 : link #1
**                                             ..
**                                             bit 31: link #31
**                                 PIF       - 0x0 to 0xFF
**                                             bit 0 : link #0
**                                             bit 1 : link #1
**                                             ..
**                                             bit 7 : link #7
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesPrepCalibration(struct file *fileHndl, UINT4 intf,
                                       UINT4 linkMsk)
{
    UINT4 maxLinks, mskBit, linkID;
    UINT4 piso_reg, adsmIntDiv_reg, adsmRstb_reg;
    UINT4 piso_msk, adsmIntDiv_msk, adsmRstb_msk;
    UINT4 pisoEnb_val, asdmIntDiv_val;
    UINT4 val, csu_id, adsm0Done, adsm1Done, adsmConfig;
    UINT4 dummy, calReq, calMode, calOk;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((intf > 3) || (intf == 0 && linkMsk > 0xFFFF) ||
        (intf == 1 && linkMsk > 0x3) || (intf == 2 && linkMsk > 0xFFFFFFFF) ||
        (intf == 3 && linkMsk > 0xFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (linkMsk == 0) {
        return HYPHY20G_SUCCESS;
    }

    result = utilHyPhy20gSerdesGetMaxLinks(fileHndl, intf, &maxLinks);
    if (result) return result;

    /* Configure links in mask */
    for (linkID = 0; linkID < maxLinks; linkID++) {
        mskBit = (linkMsk >> linkID) & 0x1;

        if (mskBit == 1) {
            switch (intf) {
                case HYPHY20G_SERDES_SFP_INTF:
                    /* SFP */
                    piso_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
                    piso_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_ENB_MSK;

                    adsmIntDiv_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_SFP_CORE_REG_CFG_10, linkID);
                    adsmIntDiv_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_INT_DIV_MSK;

                    adsmRstb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
                    adsmRstb_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_MSK;
                    break;
                case HYPHY20G_SERDES_XFI_INTF:
                    /* XFI */
                    piso_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                    HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
                    piso_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_ENB_MSK;

                    adsmIntDiv_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_12, linkID);
                    adsmIntDiv_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_12_ADSM_INT_DIV_MSK;

                    adsmRstb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
                    adsmRstb_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_MSK;
                    break;
                case HYPHY20G_SERDES_BACKPLANE_INTF:
                    /* Backplane */
                    piso_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                       HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
                    piso_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_ENB_MSK;

                    result = utilHyPhy20gSerdesGetBackplaneCsuId(fileHndl,
                                                                 linkID,
                                                                 &csu_id);
                    if (result) return result;

                    adsmIntDiv_reg = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(
                         HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_5, csu_id);
                    adsmIntDiv_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_INT_DIV_MSK;

                    adsmRstb_reg = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(
                        HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_10, csu_id);
                    adsmRstb_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_MSK;
                    break;
                case HYPHY20G_SERDES_PIF_INTF:
                    /* PIF */
                    piso_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
                    piso_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_ENB_MSK;

                    adsmIntDiv_reg = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_5;
                    adsmIntDiv_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_INT_DIV_MSK;

                    adsmRstb_reg = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_10;
                    adsmRstb_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_MSK;
                    break;
                default:
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_INVALID_ARG;
            }

            /* check link state */
            result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl, intf, linkID,
                                                       &dummy, &calMode,
                                                       &calReq, &dummy, &dummy,
                                                       &dummy);
            if (result) return result;

            if ((calMode != HYPHY20G_SERDES_DYNAMIC_CAL_MODE &&
                calMode != HYPHY20G_SERDES_DEFER_CAL_MODE) || calReq == 0) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }

            /* read and store current piso_enb bit to restore later */
            result = sysHyPhy20gRead(fileHndl, piso_reg, &val);
            if (result) return result;
            result = hyPhy20gVarFieldRead(val, piso_msk, &pisoEnb_val);
            if (result) return result;

            /* put piso in rst */
            result = hyPhy20gVarFieldWrite(&val, piso_msk, 1);
            if (result) return result;
            result = sysHyPhy20gWrite(fileHndl, piso_reg, val);
            if (result) return result;

            calOk = 0;
            if ((intf == HYPHY20G_SERDES_SFP_INTF) ||
                (intf == HYPHY20G_SERDES_XFI_INTF)) {
                /* read and store current adsm_int_div value to restore later */
                result = sysHyPhy20gRead(fileHndl, adsmIntDiv_reg, &val);
                if (result) return result;
                result = hyPhy20gVarFieldRead(val, adsmIntDiv_msk,
                                              &asdmIntDiv_val);
                if (result) return result;

                /* put a freq offset into the ADSM of ADSM_INT_DIV-1 */
                result = hyPhy20gVarFieldWrite(&val, adsmIntDiv_msk,
                                               (asdmIntDiv_val - 1));
                if (result) return result;
                result = sysHyPhy20gWrite(fileHndl, adsmIntDiv_reg, val);
                if (result) return result;

                /* toggle ADSM_RSTB */
                result = hyPhy20gRegFieldWrite(fileHndl, adsmRstb_reg,
                                               adsmRstb_msk, 0);
                if (result) return result;
                result = hyPhy20gRegFieldWrite(fileHndl, adsmRstb_reg,
                                               adsmRstb_msk, 1);
                if (result) return result;

                /* store values in SW context */
                result = serdesHyPhy20gSerdesSetMskAdcCalCtxt(fileHndl, intf,
                                                              linkID, dummy,
                                                              dummy, dummy,
                                                              calOk, pisoEnb_val,
                                                              asdmIntDiv_val,
                                                              0x38);
                if (result) return result;
            } else {
                /* store values in SW context */
                result = serdesHyPhy20gSerdesSetMskAdcCalCtxt(fileHndl, intf,
                                                              linkID, dummy,
                                                              dummy, dummy,
                                                              calOk, pisoEnb_val,
                                                              dummy,
                                                              0x18);
                if (result) return result;
            }
        }
    }

    /* ADSM registers are not per link basis for Backplane and PIF,
       therefore we only need to configure once for a set of links. */
    adsm0Done = 0;
    adsm1Done = 0;
    if ((intf == HYPHY20G_SERDES_BACKPLANE_INTF) ||
        (intf == HYPHY20G_SERDES_PIF_INTF)) {

        for (linkID = 0; linkID < maxLinks; linkID++) {
            mskBit = (linkMsk >> linkID) & 0x1;

            if (mskBit == 1) {
                switch (intf) {
                    case HYPHY20G_SERDES_BACKPLANE_INTF:
                        /* Backplane */
                        result = utilHyPhy20gSerdesGetBackplaneCsuId(fileHndl,
                                                                     linkID,
                                                                     &csu_id);
                        if (result) return result;

                        if (csu_id == 0) {
                            adsmConfig = (adsm0Done == 0) ? 1 : 0;
                        } else {
                            adsmConfig = (adsm1Done == 0) ? 1 : 0;
                        }

                        adsmIntDiv_reg = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(
                         HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_5, csu_id);
                        adsmIntDiv_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_INT_DIV_MSK;

                        adsmRstb_reg = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(
                        HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_10, csu_id);
                        adsmRstb_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_MSK;
                        break;
                    case HYPHY20G_SERDES_PIF_INTF:
                        /* PIF */
                        adsmConfig = (adsm0Done == 0) ? 1 : 0;

                        adsmIntDiv_reg = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_5;
                        adsmIntDiv_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_INT_DIV_MSK;

                        adsmRstb_reg = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_10;
                        adsmRstb_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_MSK;
                        break;
                    default:
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                        return HYPHY20G_ERR_INVALID_ARG;
                }

                if (adsmConfig == 1) {
                    /* read and store current adsm_int_div value to restore later */
                    result = sysHyPhy20gRead(fileHndl, adsmIntDiv_reg, &val);
                    if (result) return result;
                    result = hyPhy20gVarFieldRead(val, adsmIntDiv_msk,
                                                  &asdmIntDiv_val);
                    if (result) return result;

                    /* put a freq offset into the ADSM of ADSM_INT_DIV-1 */
                    result = hyPhy20gVarFieldWrite(&val, adsmIntDiv_msk,
                                                   (asdmIntDiv_val - 1));
                    if (result) return result;
                    result = sysHyPhy20gWrite(fileHndl, adsmIntDiv_reg, val);
                    if (result) return result;

                    /* toggle ADSM_RSTB */
                    result = hyPhy20gRegFieldWrite(fileHndl, adsmRstb_reg,
                                                   adsmRstb_msk, 0);
                    if (result) return result;
                    result = hyPhy20gRegFieldWrite(fileHndl, adsmRstb_reg,
                                                   adsmRstb_msk, 1);
                    if (result) return result;

                    /* store values in SW context */
                    result = serdesHyPhy20gSerdesSetMskAdcCalCtxt(fileHndl, intf,
                                                                  linkID, dummy,
                                                                  dummy, dummy,
                                                                  dummy, dummy,
                                                                  asdmIntDiv_val,
                                                                  0x20);
                    if (result) return result;

                    switch (intf) {
                        case HYPHY20G_SERDES_BACKPLANE_INTF:
                            /* Backplane */
                            result = utilHyPhy20gSerdesGetBackplaneCsuId(
                                                     fileHndl, linkID, &csu_id);
                            if (result) return result;

                            if (csu_id == 0) {
                                adsm0Done = 1;
                            } else {
                                adsm1Done = 1;
                            }
                            break;
                        case HYPHY20G_SERDES_PIF_INTF:
                            /* PIF */
                            adsm0Done = 1;
                            break;
                        default:
                            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                            return HYPHY20G_ERR_INVALID_ARG;
                    }
                }
            }
        }
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesPrepCalibration */

/*******************************************************************************
**
**  utilHyPhy20gSerdesIntfMskPrepCalibration
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Preparation procedure for calibrating the ADC offsets for
**                  interfaces.  Stops the transmitters and forces the CSUs
**                  to be asynchronous.
**
**                  Mask values:
**                      0 - do not calibrate link
**                      1 - calibrate link
**
**  INPUTS:         fileHndl - device handle
**                  sfpMsk   - Mask of SFP links to calibrate
**                                 bit 0 : SFP #0
**                                 bit 1 : SFP #1
**                                 ..
**                                 bit 15: SFP #15
**                  xfiMsk   - Mask of XFI links to calibrate
**                                 bit 0 : XFI #0
**                                 bit 1 : XFI #1
**                  bpMsk    - Mask of backplane links to calibrate
**                                 bit 0 : link #0
**                                 bit 1 : link #1
**                                 ..
**                                 bit 31: link #31
**                  pifMsk   - Mask of PIF links to calibrate
**                                 bit 0 : link #0
**                                 bit 1 : link #1
**                                 ..
**                                 bit 7 : link #7
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesIntfMskPrepCalibration(struct file *fileHndl,
                                              UINT4 sfpMsk, UINT4 xfiMsk,
                                              UINT4 bpMsk, UINT4 pifMsk)
{
    UINT4 intf;
    UINT4 cfgMsk;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((sfpMsk > 0xFFFF) || (xfiMsk > 0x3) || (bpMsk > 0xFFFFFFFF) ||
        (pifMsk > 0xFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (intf = 0; intf < 4; intf++) {
        switch (intf) {
            case HYPHY20G_SERDES_SFP_INTF:
                /* SFP */
                cfgMsk = sfpMsk;
                break;
            case HYPHY20G_SERDES_XFI_INTF:
                /* XFI */
                cfgMsk = xfiMsk;
                break;
            case HYPHY20G_SERDES_BACKPLANE_INTF:
                /* Backplane */
                cfgMsk = bpMsk;
                break;
            case HYPHY20G_SERDES_PIF_INTF:
                /* PIF */
                cfgMsk = pifMsk;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        if (cfgMsk != 0) {
            result = utilHyPhy20gSerdesPrepCalibration(fileHndl, intf, cfgMsk);
            if (result) return result;
        }
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesIntfMskPrepCalibration */

/*******************************************************************************
**
**  utilHyPhy20gSerdesCalibrateOnePhase
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure for calibrating a single XFI/SFP/PIF/ESSI
**                  phase for a set of links.
**
**                  Mask values:
**                      0 - do not calibrate link
**                      1 - calibrate link
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 0 - SFP
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkMsk  - Mask of links to prep calibration
**                                 SFP       - 0x0 to 0xFFFF
**                                             bit 0 : SFP #0
**                                             bit 1 : SFP #1
**                                             ..
**                                             bit 15: SFP #15
**                                 XFI       - 0x0 to 0x3
**                                             bit 0 : XFI #0
**                                             bit 1 : XFI #1
**                                 Backplane - 0x0 to 0xFFFFFFFF
**                                             bit 0 : link #0
**                                             bit 1 : link #1
**                                             ..
**                                             bit 31: link #31
**                                 PIF       - 0x0 to 0xFF
**                                             bit 0 : link #0
**                                             bit 1 : link #1
**                                             ..
**                                             bit 7 : link #7
**                  phaseNum - Phase number
**
**  OUTPUTS:        None     - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesCalibrateOnePhase(struct file *fileHndl, UINT4 intf,
                                         UINT4 linkMsk, UINT4 phase)
{
    UINT4 maxLinks, mskBit, linkID;
    UINT4 sipoBistSel_reg, occCalPhase_reg, occAdcCal_reg;
    UINT4 sipoBistSel_msk, occCalPhase_msk, occAdcCal_msk;
    UINT4 dummy, calReq, calMode, calOk;
    UINT4 nPhases, nComps;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((intf > 3) || (intf == 0 && linkMsk > 0xFFFF) ||
        (intf == 1 && linkMsk > 0x3) || (intf == 2 && linkMsk > 0xFFFFFFFF) ||
        (intf == 3 && linkMsk > 0xFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (linkMsk == 0) {
        return HYPHY20G_SUCCESS;
    }

    result = utilHyPhy20gSerdesGetMaxLinks(fileHndl, intf, &maxLinks);
    if (result) return result;

    result = utilHyPhy20gSerdesGetMaxNPhaseNComp(fileHndl, intf, &nPhases,
                                                 &nComps);
    if (result) return result;

    /* Configure links in mask */
    for (linkID = 0; linkID < maxLinks; linkID++) {
        mskBit = (linkMsk >> linkID) & 0x1;

        if (mskBit == 1) {
            result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl, intf, linkID,
                                                       &dummy, &calMode, &calReq,
                                                       &calOk, &dummy, &dummy);
            if (result) return result;

            if (calMode != HYPHY20G_SERDES_DYNAMIC_CAL_MODE &&
                calMode != HYPHY20G_SERDES_DEFER_CAL_MODE) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            if (calReq == 0 || calOk == 1) {
                return HYPHY20G_SUCCESS;
            }

            switch (intf) {
                case HYPHY20G_SERDES_SFP_INTF:
                    /* SFP */
                    sipoBistSel_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                             HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_CFG_2, linkID);
                    sipoBistSel_msk = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_CFG_2_SIPO_BIST_SEL_MSK;

                    occCalPhase_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_SFP_CORE_REG_CFG_17, linkID);
                    occCalPhase_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_17_OCC_CAL_PHASE_MSK;

                    occAdcCal_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_SFP_CORE_REG_CFG_16, linkID);
                    occAdcCal_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_CALIBRATE_MSK;
                    break;
                case HYPHY20G_SERDES_XFI_INTF:
                    /* XFI */
                    sipoBistSel_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                          HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_2, linkID);
                    sipoBistSel_msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_2_SIPO_BIST_SEL_MSK;

                    occCalPhase_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
                    occCalPhase_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_CAL_PHASE_MSK;

                    occAdcCal_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
                    occAdcCal_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_CALIBRATE_MSK;
                    break;
                case HYPHY20G_SERDES_BACKPLANE_INTF:
                    /* Backplane */
                    sipoBistSel_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_2, linkID);
                    sipoBistSel_msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_2_SIPO_BIST_SEL_MSK;

                    occCalPhase_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                       HYPHY20G_SERDES_BP_C8_REG_CFG_9, linkID);
                    occCalPhase_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_9_OCC_CAL_PHASE_MSK;

                    occAdcCal_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                       HYPHY20G_SERDES_BP_C8_REG_CFG_9, linkID);
                    occAdcCal_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_9_OCC_ADC_CALIBRATE_MSK;
                    break;
                case HYPHY20G_SERDES_PIF_INTF:
                    /* PIF */
                    sipoBistSel_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                           HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_2, linkID);
                    sipoBistSel_msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_2_SIPO_BIST_SEL_MSK;

                    occCalPhase_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_PIF_C8_REG_CFG_9, linkID);
                    occCalPhase_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_9_OCC_CAL_PHASE_MSK;

                    occAdcCal_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_PIF_C8_REG_CFG_9, linkID);
                    occAdcCal_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_9_OCC_ADC_CALIBRATE_MSK;
                    break;
                default:
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_INVALID_ARG;
            }

            result = hyPhy20gRegFieldWrite(fileHndl, sipoBistSel_reg,
                                           sipoBistSel_msk,
                                           (nPhases - (phase + 1)));
            if (result) return result;

            result = hyPhy20gRegFieldWrite(fileHndl, occCalPhase_reg,
                                           occCalPhase_msk, phase);
            if (result) return result;

            result = hyPhy20gRegFieldWrite(fileHndl, occAdcCal_reg,
                                           occAdcCal_msk, 0);
            if (result) return result;
            result = hyPhy20gRegFieldWrite(fileHndl, occAdcCal_reg,
                                           occAdcCal_msk, 1);
            if (result) return result;
            result = hyPhy20gRegFieldWrite(fileHndl, occAdcCal_reg,
                                           occAdcCal_msk, 0);
            if (result) return result;
        }
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesCalibrateOnePhase */

/*******************************************************************************
**
**  utilHyPhy20gSerdesIntfMskCalibrateOnePhase
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure for calibrating a XFI/SFP/PIF/ESSI phase for a
**                  set of interface links.
**
**                  Mask values:
**                      0 - do not calibrate link
**                      1 - calibrate link
**
**  INPUTS:         fileHndl - device handle
**                  sfpMsk   - Mask of SFP links to calibrate
**                                 bit 0 : SFP #0
**                                 bit 1 : SFP #1
**                                 ..
**                                 bit 15: SFP #15
**                  sfpPhase - Phase number for masked SFP links
**                  xfiMsk   - Mask of XFI links to calibrate
**                                 bit 0 : XFI #0
**                                 bit 1 : XFI #1
**                  xfiPhase - Phase number for masked XFI links
**                  bpMsk    - Mask of backplane links to calibrate
**                                 bit 0 : link #0
**                                 bit 1 : link #1
**                                 ..
**                                 bit 31: link #31
**                  bpPhase  - Phase number for masked backplane links
**                  pifMsk   - Mask of PIF links to calibrate
**                                 bit 0 : link #0
**                                 bit 1 : link #1
**                                 ..
**                                 bit 7 : link #7
**                  pifPhase - Phase number for masked PIF links
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesIntfMskCalibrateOnePhase(struct file *fileHndl,
                                                UINT4 sfpMsk, UINT4 sfpPhase,
                                                UINT4 xfiMsk, UINT4 xfiPhase,
                                                UINT4 bpMsk, UINT4 bpPhase,
                                                UINT4 pifMsk, UINT4 pifPhase)
{
    UINT4 intf;
    UINT4 cfgMsk;
    UINT4 phase;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((sfpMsk > 0xFFFF) || (xfiMsk > 0x3) || (bpMsk > 0xFFFFFFFF) ||
        (pifMsk > 0xFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (intf = 0; intf < 4; intf++) {
        switch (intf) {
            case HYPHY20G_SERDES_SFP_INTF:
                /* SFP */
                cfgMsk = sfpMsk;
                phase = sfpPhase;
                break;
            case HYPHY20G_SERDES_XFI_INTF:
                /* XFI */
                cfgMsk = xfiMsk;
                phase = xfiPhase;
                break;
            case HYPHY20G_SERDES_BACKPLANE_INTF:
                /* Backplane */
                cfgMsk = bpMsk;
                phase = bpPhase;
                break;
            case HYPHY20G_SERDES_PIF_INTF:
                /* PIF */
                cfgMsk = pifMsk;
                phase = pifPhase;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        if (cfgMsk != 0) {
            result = utilHyPhy20gSerdesCalibrateOnePhase(fileHndl, intf, cfgMsk,
                                                         phase);
            if (result) return result;
        }
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesIntfMskCalibrateOnePhase */

/*******************************************************************************
**
**  utilHyPhy20gSerdesCheckAdc
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure reads the ADC memories and checks the results
**                  of a set of links.  If the results are consistent with
**                  previous results, the final offsets are calculated.
**
**                  Mask values:
**                      0 - do not calibrate link
**                      1 - calibrate link
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 0 - SFP
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkMsk  - Mask of links to prep calibration
**                                 SFP       - 0x0 to 0xFFFF
**                                             bit 0 : SFP #0
**                                             bit 1 : SFP #1
**                                             ..
**                                             bit 15: SFP #15
**                                 XFI       - 0x0 to 0x3
**                                             bit 0 : XFI #0
**                                             bit 1 : XFI #1
**                                 Backplane - 0x0 to 0xFFFFFFFF
**                                             bit 0 : link #0
**                                             bit 1 : link #1
**                                             ..
**                                             bit 31: link #31
**                                 PIF       - 0x0 to 0xFF
**                                             bit 0 : link #0
**                                             bit 1 : link #1
**                                             ..
**                                             bit 7 : link #7
**                  calCount - Calibration count
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesCheckAdc(struct file *fileHndl, UINT4 intf,
                                UINT4 linkMsk, UINT4 calCount)
{
    UINT4 maxLinks, mskBit, linkID;
    UINT4 occAdc_reg;
    UINT4 occAdcBypassEn_msk, occAdcA_msk, occAdcRd_msk, occAdcQ_msk;
    UINT4 dummy, calReq, calMode, calOk;
    UINT4 phase, nPhases, comp, nComps, consistency;
    UINT4 curr_offset_val, prev_offset_val, offset;
    UINT4 valid, consistency_2;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((intf > 3) || (intf == 0 && linkMsk > 0xFFFF) ||
        (intf == 1 && linkMsk > 0x3) || (intf == 2 && linkMsk > 0xFFFFFFFF) ||
        (intf == 3 && linkMsk > 0xFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = utilHyPhy20gSerdesGetMaxLinks(fileHndl, intf, &maxLinks);
    if (result) return result;

    result = utilHyPhy20gSerdesGetMaxNPhaseNComp(fileHndl, intf, &nPhases,
                                                 &nComps);
    if (result) return result;

    /* Configure links in mask */
    for (linkID = 0; linkID < maxLinks; linkID++) {
        mskBit = (linkMsk >> linkID) & 0x1;

        if (mskBit == 1) {
            result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl, intf, linkID,
                                                       &dummy, &calMode, &calReq,
                                                       &calOk, &dummy, &dummy);
            if (result) return result;

            if (calMode != HYPHY20G_SERDES_DYNAMIC_CAL_MODE &&
                calMode != HYPHY20G_SERDES_DEFER_CAL_MODE) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            if (calReq == 0 || calOk == 1) {
                return HYPHY20G_SUCCESS;
            }

            switch (intf) {
                case HYPHY20G_SERDES_SFP_INTF:
                    /* SFP */
                    occAdc_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_SFP_CORE_REG_CFG_16, linkID);
                    occAdcBypassEn_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_BYPASS_EN_MSK;
                    occAdcA_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_A_MSK;
                    occAdcRd_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_RD_MSK;
                    occAdcQ_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_Q_MSK;
                    break;
                case HYPHY20G_SERDES_XFI_INTF:
                    /* XFI */
                    occAdc_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
                    occAdcBypassEn_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_BYPASS_EN_MSK;
                    occAdcA_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_A_MSK;
                    occAdcRd_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_RD_MSK;
                    occAdcQ_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_Q_MSK;
                    break;
                case HYPHY20G_SERDES_BACKPLANE_INTF:
                    /* Backplane */
                    occAdc_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                       HYPHY20G_SERDES_BP_C8_REG_CFG_9, linkID);
                    occAdcBypassEn_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_9_OCC_ADC_BYPASS_EN_MSK;
                    occAdcA_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_9_OCC_ADC_A_MSK;
                    occAdcRd_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_9_OCC_ADC_RD_MSK;
                    occAdcQ_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_9_OCC_ADC_Q_MSK;
                    break;
                case HYPHY20G_SERDES_PIF_INTF:
                    /* PIF */
                    occAdc_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_PIF_C8_REG_CFG_9, linkID);
                    occAdcBypassEn_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_9_OCC_ADC_BYPASS_EN_MSK;
                    occAdcA_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_9_OCC_ADC_A_MSK;
                    occAdcRd_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_9_OCC_ADC_RD_MSK;
                    occAdcQ_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_9_OCC_ADC_Q_MSK;
                    break;
                default:
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_INVALID_ARG;
            }

            consistency = 1;
            /* put ADC into bypass */
            result = hyPhy20gRegFieldWrite(fileHndl, occAdc_reg,
                                           occAdcBypassEn_msk, 1);
            if (result) return result;

            for (phase = 0; phase < nPhases; phase++) {
                for (comp = 0; comp < nComps; comp++) {
                    /* write addr */
                    result = hyPhy20gRegFieldWrite(fileHndl, occAdc_reg,
                                                   occAdcA_msk,
                                                   ((phase*16) + comp));
                    if (result) return result;

                    /* trigger rd */
                    result = hyPhy20gRegFieldWrite(fileHndl, occAdc_reg,
                                                   occAdcRd_msk, 1);
                    if (result) return result;
                    result = hyPhy20gRegFieldWrite(fileHndl, occAdc_reg,
                                                   occAdcRd_msk, 0);
                    if (result) return result;

                    /* read value */
                    result = hyPhy20gRegFieldRead(fileHndl, occAdc_reg,
                                                  occAdcQ_msk,
                                                  &curr_offset_val);
                    if (result) return result;

                    /* DEBUG */
                    result = serdesHyPhy20gSerdesGetAdcCalOffsetCtxt(fileHndl,
                                   intf, linkID, phase, comp, &prev_offset_val);
                    if (result) return result;
                    DBG_PRINT("%s, %s, %d, curr offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (curr_offset_val), (unsigned int) (phase), (unsigned int) (comp));
                    DBG_PRINT("%s, %s, %d, prev offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (prev_offset_val), (unsigned int) (phase), (unsigned int) (comp));
                    /* End DEBUG */

                    if (calCount < 1) {
                        consistency = 0;
                    }

                    if (curr_offset_val > 127) {
                        /* equation: offset = prev_offset - curr_offset */
                        offset = prev_offset_val + (((~curr_offset_val)&0xFF)+1);
                    } else {
                        offset = prev_offset_val - curr_offset_val;
                    }

                    if (offset > 0x7FFFFFFF) {
                        /* convert back to 8 bit two's complement from 32bit UINT4 */
                        offset = 256 - ((~offset)+1);
                    } else if (offset > 0xFF) {
                        /* rolling over */
                        offset = offset - 256;
                    }
                    DBG_PRINT("%s, %s, %d, abs offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (offset), (unsigned int) (phase), (unsigned int) (comp));

                    /* if abs (offset value-curr offset value is > tolerance) */
                    if (offset > 127) {
                        if ((((~offset)&0xFF)+1) > HYPHY20G_SERDES_ADC_CAL_REPEAT_TOL) {
                            DBG_PRINT("%s, %s, %d, setting consistency to 0\n",__FILE__, __FUNCTION__, __LINE__);
                            consistency = 0;
                        }
                    } else {
                        if (offset > HYPHY20G_SERDES_ADC_CAL_REPEAT_TOL) {
                            DBG_PRINT("%s, %s, %d, setting consistency to 0\n",__FILE__, __FUNCTION__, __LINE__);
                            consistency = 0;
                        }
                    }

                    result = serdesHyPhy20gSerdesSetAdcCalOffsetCtxt(fileHndl,
                                    intf, linkID, phase, comp, curr_offset_val);
                    if (result) return result;
                    DBG_PRINT("%s, %s, %d, consistency = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (consistency), (unsigned int) (phase), (unsigned int) (comp));
                }
            }

            /* exit ADC bypass */
            result = hyPhy20gRegFieldWrite(fileHndl, occAdc_reg,
                                           occAdcBypassEn_msk, 0);
            if (result) return result;

            if (consistency == 1) {
                if (intf == HYPHY20G_SERDES_SFP_INTF) {
                    /* SFP */
                    result = utilHyPhy20gSfpSerdesSubtRefsFrmOffsets(fileHndl,
                                                                linkID, &valid);
                    if (result) return result;
                } else {
                    result = utilHyPhy20gXfiBpPifSerdesSubtRefsFrmOffsets(
                                                fileHndl, intf, linkID, &valid);
                    if (result) return result;
                }
                consistency_2 = valid;
            } else {
                consistency_2 = consistency;
            }

            if ((calCount == (HYPHY20G_SERDES_ADC_CAL_COUNT_MAX-1)) &&
                (consistency_2 == 0)) {
                /* calibration failed, reset offsets to zero */
                for (phase = 0; phase < nPhases; phase++) {
                    for (comp = 0; comp < nComps; comp++) {
                        result = serdesHyPhy20gSerdesSetAdcCalOffsetCtxt(
                                        fileHndl, intf, linkID, phase, comp, 0);
                        if (result) return result;
                    }
                }
            }
        }
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesCheckAdc */

/*******************************************************************************
**
**  utilHyPhy20gSerdesIntfMskCheckAdc
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure reads the ADC memories and checks the results
**                  of a set of links.  If the results are consistent with
**                  previous results, the final offsets are calculated.
**
**  INPUTS:         fileHndl  - device handle
**                  sfpMsk    - Mask of SFP links to calibrate
**                                  bit 0 : SFP #0
**                                  bit 1 : SFP #1
**                                  ..
**                                  bit 15: SFP #15
**                  sfpCalCnt - Calibration count for masked SFP links
**                  xfiMsk    - Mask of XFI links to calibrate
**                                  bit 0 : XFI #0
**                                  bit 1 : XFI #1
**                  xfiCalCnt - Calibration count for masked XFI links
**                  bpMsk     - Mask of backplane links to calibrate
**                                  bit 0 : link #0
**                                  bit 1 : link #1
**                                  ..
**                                  bit 31: link #31
**                  bpCalCnt  - Calibration count for masked backplane links
**                  pifMsk    - Mask of PIF links to calibrate
**                                  bit 0 : link #0
**                                  bit 1 : link #1
**                                  ..
**                                  bit 7 : link #7
**                  pifCalCnt - Calibration count for masked PIF links
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesIntfMskCheckAdc(struct file *fileHndl, UINT4 sfpMsk,
                                       UINT4 sfpCalCnt, UINT4 xfiMsk,
                                       UINT4 xfiCalCnt, UINT4 bpMsk,
                                       UINT4 bpCalCnt, UINT4 pifMsk,
                                       UINT4 pifCalCnt)
{
    UINT4 intf;
    UINT4 cfgMsk;
    UINT4 calCount;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((sfpMsk > 0xFFFF) || (xfiMsk > 0x3) || (bpMsk > 0xFFFFFFFF) ||
        (pifMsk > 0xFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (intf = 0; intf < 4; intf++) {
        switch (intf) {
            case HYPHY20G_SERDES_SFP_INTF:
                /* SFP */
                cfgMsk = sfpMsk;
                calCount = sfpCalCnt;
                break;
            case HYPHY20G_SERDES_XFI_INTF:
                /* XFI */
                cfgMsk = xfiMsk;
                calCount = xfiCalCnt;
                break;
            case HYPHY20G_SERDES_BACKPLANE_INTF:
                /* Backplane */
                cfgMsk = bpMsk;
                calCount = bpCalCnt;
                break;
            case HYPHY20G_SERDES_PIF_INTF:
                /* PIF */
                cfgMsk = pifMsk;
                calCount = pifCalCnt;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        if (cfgMsk != 0) {
            result = utilHyPhy20gSerdesCheckAdc(fileHndl, intf, cfgMsk,
                                                calCount);
            if (result) return result;
        }
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesIntfMskCheckAdc */

/*******************************************************************************
**
**  utilHyPhy20gSfpSerdesSubtRefsFrmOffsets
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure subtracts the references to calculate the offsets
**                  for a single SFP link.
**
**  INPUTS:         fileHndl - device handle
**                  linkID   - SFP SERDES link
**                             Legal range 0 to 15
**                  *pValid  - 0 - Offsets are invalid
**                             1 - Offsets are valid
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSfpSerdesSubtRefsFrmOffsets(struct file *fileHndl,
                                             UINT4 linkID, UINT4 *pValid)
{
    UINT4 intf, nPhases, phase, nComps, comp;
    UINT4 sum_offsets, valid_offsets, divider, systematic_offset, remainder;
    UINT4 pos_sum_offsets, convertToPos;
    UINT4 adcCalOk;
    UINT4 offset;
    UINT4 dummy = 0;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if (linkID > 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    intf = HYPHY20G_SERDES_SFP_INTF; /* SFP */
    result = utilHyPhy20gSerdesGetMaxNPhaseNComp(fileHndl, intf, &nPhases,
                                                 &nComps);
    if (result) return result;

    DBG_PRINT("%s, %s, %d, ADC offset adjustment routine \n",__FILE__, __FUNCTION__, __LINE__);

    sum_offsets = 0;
    valid_offsets = 1;
    /* calculate systematic offset */
    for (phase = 0; phase < nPhases; phase++) {
        for (comp = 0; comp < nComps; comp++) {
            /* offsets are 8-bit two's complement */
            result = serdesHyPhy20gSerdesGetAdcCalOffsetCtxt(fileHndl, intf,
                                                  linkID, phase, comp, &offset);
            if (result) return result;

            if (offset > 127) {
                /* offset is negative */
                sum_offsets = sum_offsets - (((~offset)&0xFF)+1);
            } else {
                sum_offsets = sum_offsets + offset;
            }

            /* check offset value */
            if (offset > 0x7F) {
                if ((((~offset)&0xFF)+1) > 18) {
                    /* offset too large */
                    valid_offsets = 0;
                }
            } else {
                if (offset > 18) {
                    /* offset too large */
                    valid_offsets = 0;
                }
            }

            if (sum_offsets > 0x7FFFFFFF) {
                /* convert back to 8 bit two's complement from 32bit UINT4 */
                sum_offsets = 256 - ((~sum_offsets)+1);
            } else if (sum_offsets > 0xFF) {
                /* rolling over */
                sum_offsets = sum_offsets - 256;
            }
            DBG_PRINT("%s, %s, %d, sum_offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (sum_offsets), (unsigned int) (phase), (unsigned int) (comp));
        }
    }

    if (valid_offsets == 0) {
        /* calibration failed */
        adcCalOk = 0;
        *pValid = 0;
        DBG_PRINT("%s, %s, %d, valid_offsets = %lu \n",__FILE__, __FUNCTION__, __LINE__, valid_offsets);
    } else {
        adcCalOk = 1;
        *pValid = 1;

        /* if sum_offsets is negative, temporarily convert to positive domain */
        convertToPos = (sum_offsets > 127) ? 1 : 0;
        pos_sum_offsets = (convertToPos == 1) ? (((~sum_offsets)&0xFF)+1) : sum_offsets;
        DBG_PRINT("%s, %s, %d, sum_offsets = %u, pos_sum_offsets = %u \n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (sum_offsets), (unsigned int) (pos_sum_offsets));

        /* integer divide and round systematic offset */
        divider = nPhases * nComps;
        systematic_offset = pos_sum_offsets*1000;
        systematic_offset = systematic_offset/divider;

        remainder = systematic_offset % 1000;

        systematic_offset = systematic_offset/1000;

        if (remainder >= 500) {
            /* round up due to integer divide */
            systematic_offset = systematic_offset+1;
        }

        /* convert back to 2's complement if necessary */
        systematic_offset = (convertToPos == 1) ?
                            ((~systematic_offset) + 1) : systematic_offset;

        if (systematic_offset > 127) {
            /* convert back to 8 bit from 32bit UINT4 */
            systematic_offset = 256 - ((~systematic_offset)+1);
        }
        DBG_PRINT("%s, %s, %d, systematic offset = %u \n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (systematic_offset));

        /* subtract systematic offset from all */
        for (phase = 0; phase < nPhases; phase++) {
            for (comp = 0; comp < nComps; comp++) {
                /* offsets are 8-bit two's complement */
                result = serdesHyPhy20gSerdesGetAdcCalOffsetCtxt(fileHndl, intf,
                                                  linkID, phase, comp, &offset);
                if (result) return result;

                if (systematic_offset > 127) {
                    /* equation: offset = offset - systematic_offset */
                    offset = offset + (((~systematic_offset)&0xFF)+1);
                } else {
                    offset = offset - systematic_offset;
                }

                if (offset > 0x7FFFFFFF) {
                    /* convert back to 8 bit two's complement from 32bit UINT4 */
                    offset = 256 - ((~offset)+1);
                } else if (offset > 0xFF) {
                    /* rolling over */
                    offset = offset - 256;
                }
                DBG_PRINT("%s, %s, %d, offset-systematic_offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (offset), (unsigned int) (phase), (unsigned int) (comp));
            }
        }
    }

    /* update SW context */
    result = serdesHyPhy20gSerdesSetMskAdcCalCtxt(fileHndl, intf, linkID, dummy,
                                                  dummy, dummy, adcCalOk, dummy,
                                                  dummy, 0x8);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSfpSerdesSubtRefsFrmOffsets */

/*******************************************************************************
**
**  utilHyPhy20gXfiBpPifSerdesSubtRefsFrmOffsets
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure subtracts the references to calculate the offsets
**                  for a single XFI, Backplane or PIF link.
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkID   - SERDES link
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**                  *pValid  - 0 - Offsets are invalid
**                             1 - Offsets are valid
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gXfiBpPifSerdesSubtRefsFrmOffsets(struct file *fileHndl,
                                                  UINT4 intf, UINT4 linkID,
                                                  UINT4 *pValid)
{
    UINT4 nPhases, phase, nComps, comp;
    UINT4 accumRefSel_reg;
    UINT4 accumRefSel_msk, accum_ref_val;
    UINT4 adcMode_reg, adcModeA_reg, adcModeB_reg, adcModeC_reg, adcModeD_reg;
    UINT4 adcMode_msk, adcModeA_msk, adcModeB_msk, adcModeC_msk, adcModeD_msk;
    UINT4 sum_offsets, valid_offsets, divider, systematic_offset, remainder;
    UINT4 pos_sum_offsets, convertToPos;
    UINT4 adcCalOk;
    UINT4 ref[4], offset;
    UINT4 reg_val;
    UINT4 dummy = 0;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((intf < 1) || (intf > 3) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = utilHyPhy20gSerdesGetMaxNPhaseNComp(fileHndl, intf, &nPhases,
                                                 &nComps);
    if (result) return result;

    switch (intf) {
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            accumRefSel_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_20, linkID);
            accumRefSel_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_20_ACCUM_REF_SEL_MSK;

            adcModeA_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_21, linkID);
            adcModeA_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_21_ACCUM_REF0_MSK;

            adcModeB_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_22, linkID);
            adcModeB_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_22_ACCUM_REF1_MSK;

            adcModeC_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_23, linkID);
            adcModeC_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_23_ACCUM_REF2_MSK;

            adcModeD_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_24, linkID);
            adcModeD_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_24_ACCUM_REF3_MSK;
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            accumRefSel_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_BP_C8_REG_CFG_11, linkID);
            accumRefSel_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_11_ACCUM_REF_SEL_MSK;

            adcModeA_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_BP_C8_REG_CFG_12, linkID);
            adcModeA_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_12_ACCUM_REF0_MSK;

            adcModeB_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_BP_C8_REG_CFG_13, linkID);
            adcModeB_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_13_ACCUM_REF1_MSK;

            adcModeC_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_BP_C8_REG_CFG_14, linkID);
            adcModeC_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_14_ACCUM_REF2_MSK;

            adcModeD_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_BP_C8_REG_CFG_15, linkID);
            adcModeD_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_15_ACCUM_REF3_MSK;
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            accumRefSel_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                     HYPHY20G_SERDES_PIF_C8_REG_CFG_11, linkID);
            accumRefSel_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_11_ACCUM_REF_SEL_MSK;

            adcModeA_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                     HYPHY20G_SERDES_PIF_C8_REG_CFG_12, linkID);
            adcModeA_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_12_ACCUM_REF0_MSK;

            adcModeB_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                     HYPHY20G_SERDES_PIF_C8_REG_CFG_13, linkID);
            adcModeB_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_13_ACCUM_REF1_MSK;

            adcModeC_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                     HYPHY20G_SERDES_PIF_C8_REG_CFG_14, linkID);
            adcModeC_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_14_ACCUM_REF2_MSK;

            adcModeD_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                     HYPHY20G_SERDES_PIF_C8_REG_CFG_15, linkID);
            adcModeD_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_15_ACCUM_REF3_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    DBG_PRINT("%s, %s, %d, ADC offset adjustment routine \n",__FILE__, __FUNCTION__, __LINE__);

    /* read adc_mode - only valid for XFI, Backplane and PIF */
    result = hyPhy20gRegFieldRead(fileHndl, accumRefSel_reg, accumRefSel_msk,
                                  &accum_ref_val);
    if (result) return result;

    switch(accum_ref_val){
        case HYPHY20G_SERDES_ADC_MODE_A:
            /* adc_mode "A" */
            adcMode_reg = adcModeA_reg;
            adcMode_msk = adcModeA_msk;
        break;
        case HYPHY20G_SERDES_ADC_MODE_B:
            /* adc_mode "B" */
            adcMode_reg = adcModeB_reg;
            adcMode_msk = adcModeB_msk;
        break;
        case HYPHY20G_SERDES_ADC_MODE_C:
            /* adc_mode "C" */
            adcMode_reg = adcModeC_reg;
            adcMode_msk = adcModeC_msk;
        break;
        case HYPHY20G_SERDES_ADC_MODE_D:
            /* adc_mode "D" */
            adcMode_reg = adcModeD_reg;
            adcMode_msk = adcModeD_msk;
        break;
        default:
            /* invalid read value */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }
    /* setup reference */
    result = hyPhy20gRegFieldRead(fileHndl, adcMode_reg, adcMode_msk, &reg_val);
    if (result) return result;
    ref[3] = reg_val & 0x000000FF;
    ref[2] = (reg_val & 0x0000FF00) >> 8;
    ref[1] = (reg_val & 0x00FF0000) >> 16;
    ref[0] = (reg_val & 0xFF000000) >> 24;
    DBG_PRINT("%s, %s, %d, ref[0] = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (ref[0]));
    DBG_PRINT("%s, %s, %d, ref[1] = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (ref[1]));
    DBG_PRINT("%s, %s, %d, ref[2] = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (ref[2]));
    DBG_PRINT("%s, %s, %d, ref[3] = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (ref[3]));

    sum_offsets = 0;
    valid_offsets = 1;
    /* calculate systematic offset */
    for (phase = 0; phase < nPhases; phase++) {
        for (comp = 0; comp < ((nComps-1)/2); comp++) {
            /* offsets are 8-bit two's complement */
            result = serdesHyPhy20gSerdesGetAdcCalOffsetCtxt(fileHndl, intf,
                                                  linkID, phase, comp, &offset);
            if (result) return result;
            offset = offset + ref[comp];
            if (offset > 255) {
                /* convert back to 8 bit two's complement from 32bit UINT4 */
                offset = offset - 256;
            }
            result = serdesHyPhy20gSerdesSetAdcCalOffsetCtxt(fileHndl, intf,
                                                             linkID, phase,
                                                             comp, offset);
            if (result) return result;

            /* check offset value */
            if (offset > 0x7F) {
                if ((((~offset)&0xFF)+1) > 18) {
                    /* offset too large */
                    valid_offsets = 0;
                }
            } else {
                if (offset > 18) {
                    /* offset too large */
                    valid_offsets = 0;
                }
            }
            DBG_PRINT("%s, %s, %d, offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (offset), (unsigned int) (phase), (unsigned int) (comp));


            result = serdesHyPhy20gSerdesGetAdcCalOffsetCtxt(fileHndl, intf,
                                 linkID, phase, (nComps - (comp + 1)), &offset);
            if (result) return result;
            offset = offset - ref[comp];
            if (offset > 0x7FFFFFFF) {
                /* convert back to 8 bit two's complement from 32bit UINT4 */
                offset = 256 - (~offset + 1);
            }
            result = serdesHyPhy20gSerdesSetAdcCalOffsetCtxt(fileHndl, intf,
                                  linkID, phase, (nComps - (comp + 1)), offset);
            if (result) return result;

            /* check offset value */
            if (offset > 0x7F) {
                if ((((~offset)&0xFF)+1) > 18) {
                    /* offset too large */
                    valid_offsets = 0;
                }
            } else {
                if (offset > 18) {
                    /* offset too large */
                    valid_offsets = 0;
                }
            }
            DBG_PRINT("%s, %s, %d, offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (offset), (unsigned int) (phase), (unsigned int) ((nComps - (comp + 1))));
        }
        for (comp = 0; comp < nComps; comp++) {
            /* offsets are 8-bit two's complement */
            result = serdesHyPhy20gSerdesGetAdcCalOffsetCtxt(fileHndl, intf,
                                                  linkID, phase, comp, &offset);
            if (result) return result;

            if (offset > 127) {
                /* offset is negative */
                sum_offsets = sum_offsets - (((~offset)&0xFF)+1);
            } else {
                sum_offsets = sum_offsets + offset;
            }

            if (sum_offsets > 0x7FFFFFFF) {
                /* convert back to 8 bit two's complement from 32bit UINT4 */
                sum_offsets = 256 - ((~sum_offsets)+1);
            } else if (sum_offsets > 0xFF) {
                /* rolling over */
                sum_offsets = sum_offsets - 256;
            }
            DBG_PRINT("%s, %s, %d, sum_offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (sum_offsets), (unsigned int) (phase), (unsigned int) (comp));
        }
    }

    if (valid_offsets == 0) {
        /* calibration failed */
        adcCalOk = 0;
        *pValid = 0;
        DBG_PRINT("%s, %s, %d, valid_offsets = %lu \n",__FILE__, __FUNCTION__, __LINE__, valid_offsets);
    } else {
        adcCalOk = 1;
        *pValid = 1;

        /* if sum_offsets is negative, temporarily convert to positive domain */
        convertToPos = (sum_offsets > 127) ? 1 : 0;
        pos_sum_offsets = (convertToPos == 1) ? (((~sum_offsets)&0xFF)+1) : sum_offsets;
        DBG_PRINT("%s, %s, %d, sum_offsets = %u, pos_sum_offsets = %u \n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (sum_offsets), (unsigned int) (pos_sum_offsets));

        /* integer divide and round systematic offset */
        divider = nPhases * nComps;
        systematic_offset = pos_sum_offsets*1000;
        systematic_offset = systematic_offset/divider;

        remainder = systematic_offset % 1000;

        systematic_offset = systematic_offset/1000;

        if (remainder >= 500) {
            /* round up due to integer divide */
            systematic_offset = systematic_offset+1;
        }

        /* convert back to 2's complement if necessary */
        systematic_offset = (convertToPos == 1) ?
                            ((~systematic_offset) + 1) : systematic_offset;

        if (systematic_offset > 127) {
            /* convert back to 8 bit from 32bit UINT4 */
            systematic_offset = 256 - ((~systematic_offset)+1);
        }
        DBG_PRINT("%s, %s, %d, systematic offset = %u \n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (systematic_offset));

        /* subtract systematic offset from all */
        for (phase = 0; phase < nPhases; phase++) {
            for (comp = 0; comp < nComps; comp++) {
                /* offsets are 8-bit two's complement */
                result = serdesHyPhy20gSerdesGetAdcCalOffsetCtxt(fileHndl, intf,
                                                  linkID, phase, comp, &offset);
                if (result) return result;

                if (systematic_offset > 127) {
                    /* equation: offset = offset - systematic_offset */
                    offset = offset + (((~systematic_offset)&0xFF)+1);
                } else {
                    offset = offset - systematic_offset;
                }

                if (offset > 0x7FFFFFFF) {
                    /* convert back to 8 bit two's complement from 32bit UINT4 */
                    offset = 256 - ((~offset)+1);
                } else if (offset > 0xFF) {
                    /* rolling over */
                    offset = offset - 256;
                }
                DBG_PRINT("%s, %s, %d, offset-systematic_offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (offset), (unsigned int) (phase), (unsigned int) (comp));
            }
        }
    }

    /* update SW context */
    result = serdesHyPhy20gSerdesSetMskAdcCalCtxt(fileHndl, intf, linkID, dummy,
                                                  dummy, dummy, adcCalOk, dummy,
                                                  dummy, 0x8);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gXfiBpPifSerdesSubtRefsFrmOffsets */

/*******************************************************************************
**
**  utilHyPhy20gSerdesRestore
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure re-configures links back the way they were before
**                  ADC offset calibration for a set of links.
**
**                  Mask values:
**                      0 - do not calibrate link
**                      1 - calibrate link
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 0 - SFP
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkMsk  - Mask of links to prep calibration
**                                 SFP       - 0x0 to 0xFFFF
**                                             bit 0 : SFP #0
**                                             bit 1 : SFP #1
**                                             ..
**                                             bit 15: SFP #15
**                                 XFI       - 0x0 to 0x3
**                                             bit 0 : XFI #0
**                                             bit 1 : XFI #1
**                                 Backplane - 0x0 to 0xFFFFFFFF
**                                             bit 0 : link #0
**                                             bit 1 : link #1
**                                             ..
**                                             bit 31: link #31
**                                 PIF       - 0x0 to 0xFF
**                                             bit 0 : link #0
**                                             bit 1 : link #1
**                                             ..
**                                             bit 7 : link #7
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesRestore(struct file *fileHndl, UINT4 intf,
                               UINT4 linkMsk)
{
    UINT4 maxLinks, mskBit, linkID;
    UINT4 piso_reg, adsmIntDiv_reg, adsmRstb_reg;
    UINT4 piso_msk, adsmIntDiv_msk, adsmRstb_msk;
    UINT4 pisoEnb_val, adsmIntDiv_val;
    UINT4 csu_id, adsm0Done, adsm1Done, adsmConfig;
    UINT4 dummy, calReq, calMode;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((intf > 3) || (intf == 0 && linkMsk > 0xFFFF) ||
        (intf == 1 && linkMsk > 0x3) || (intf == 2 && linkMsk > 0xFFFFFFFF) ||
        (intf == 3 && linkMsk > 0xFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = utilHyPhy20gSerdesGetMaxLinks(fileHndl, intf, &maxLinks);
    if (result) return result;

    /* Configure links in mask */
    for (linkID = 0; linkID < maxLinks; linkID++) {
        mskBit = (linkMsk >> linkID) & 0x1;

        if (mskBit == 1) {
            switch (intf) {
                case HYPHY20G_SERDES_SFP_INTF:
                    /* SFP */
                    piso_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkID);
                    piso_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_ENB_MSK;

                    adsmIntDiv_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_SFP_CORE_REG_CFG_10, linkID);
                    adsmIntDiv_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_10_ADSM_INT_DIV_MSK;

                    adsmRstb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkID);
                    adsmRstb_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_MSK;
                    break;
                case HYPHY20G_SERDES_XFI_INTF:
                    /* XFI */
                    piso_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                    HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkID);
                    piso_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_ENB_MSK;

                    adsmIntDiv_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_12, linkID);
                    adsmIntDiv_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_12_ADSM_INT_DIV_MSK;

                    adsmRstb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkID);
                    adsmRstb_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_MSK;
                    break;
                case HYPHY20G_SERDES_BACKPLANE_INTF:
                    /* Backplane */
                    piso_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                       HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkID);
                    piso_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_ENB_MSK;

                    result = utilHyPhy20gSerdesGetBackplaneCsuId(fileHndl,
                                                                 linkID,
                                                                 &csu_id);
                    if (result) return result;

                    adsmIntDiv_reg = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(
                         HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_5, csu_id);
                    adsmIntDiv_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_INT_DIV_MSK;

                    adsmRstb_reg = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(
                        HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_10, csu_id);
                    adsmRstb_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_MSK;
                    break;
                case HYPHY20G_SERDES_PIF_INTF:
                    /* PIF */
                    piso_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkID);
                    piso_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_ENB_MSK;

                    adsmIntDiv_reg = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_5;
                    adsmIntDiv_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_INT_DIV_MSK;

                    adsmRstb_reg = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_10;
                    adsmRstb_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_MSK;
                    break;
                default:
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                    return HYPHY20G_ERR_INVALID_ARG;
            }

            /* check link state */
            result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl, intf, linkID,
                                                       &dummy, &calMode,
                                                       &calReq, &dummy,
                                                       &dummy, &dummy);
            if (result) return result;

            if (calMode != HYPHY20G_SERDES_DYNAMIC_CAL_MODE &&
                calMode != HYPHY20G_SERDES_DEFER_CAL_MODE) {
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
            if (calReq == 0) {
                return HYPHY20G_SUCCESS;
            }

            /* update sw context */
            dummy = 0;
            calReq = 0;
            result = serdesHyPhy20gSerdesSetMskAdcCalCtxt(fileHndl, intf,
                                                          linkID, dummy, dummy,
                                                          calReq, dummy, dummy,
                                                          dummy, 0x4);
            if (result) return result;

            /* retrieve sw context */
            result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl, intf, linkID,
                                                       &dummy, &dummy, &dummy,
                                                       &dummy, &pisoEnb_val,
                                                       &adsmIntDiv_val);
            if (result) return result;

            /* restore piso_enb bit */
            result = hyPhy20gRegFieldWrite(fileHndl, piso_reg, piso_msk,
                                           pisoEnb_val);
            if (result) return result;

            if ((intf == HYPHY20G_SERDES_SFP_INTF) ||
                (intf == HYPHY20G_SERDES_XFI_INTF)) {
                /* restore asdm_int_div */
                result = hyPhy20gRegFieldWrite(fileHndl, adsmIntDiv_reg,
                                               adsmIntDiv_msk, adsmIntDiv_val);
                if (result) return result;

                /* toggle ADSM_RSTB */
                result = hyPhy20gRegFieldWrite(fileHndl, adsmRstb_reg,
                                               adsmRstb_msk, 0);
                if (result) return result;
                result = hyPhy20gRegFieldWrite(fileHndl, adsmRstb_reg,
                                               adsmRstb_msk, 1);
                if (result) return result;
            }
        }
    }

    /* ADSM registers are not per link basis for Backplane and PIF,
       therefore we only need to configure once for a set of links. */
    adsm0Done = 0;
    adsm1Done = 0;
    if ((intf == HYPHY20G_SERDES_BACKPLANE_INTF) ||
        (intf == HYPHY20G_SERDES_PIF_INTF)) {

        for (linkID = 0; linkID < maxLinks; linkID++) {
            mskBit = (linkMsk >> linkID) & 0x1;

            if (mskBit == 1) {
                switch (intf) {
                    case HYPHY20G_SERDES_BACKPLANE_INTF:
                        /* Backplane */
                        result = utilHyPhy20gSerdesGetBackplaneCsuId(fileHndl,
                                                                     linkID,
                                                                     &csu_id);
                        if (result) return result;

                        if (csu_id == 0) {
                            adsmConfig = (adsm0Done == 0) ? 1 : 0;
                        } else {
                            adsmConfig = (adsm1Done == 0) ? 1 : 0;
                        }

                        adsmIntDiv_reg = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(
                         HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_5, csu_id);
                        adsmIntDiv_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_INT_DIV_MSK;

                        adsmRstb_reg = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(
                        HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_10, csu_id);
                        adsmRstb_msk = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_MSK;
                        break;
                    case HYPHY20G_SERDES_PIF_INTF:
                        /* PIF */
                        adsmConfig = (adsm0Done == 0) ? 1 : 0;

                        adsmIntDiv_reg = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_5;
                        adsmIntDiv_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_5_ADSM_INT_DIV_MSK;

                        adsmRstb_reg = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_10;
                        adsmRstb_msk = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_MSK;
                        break;
                    default:
                        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                        return HYPHY20G_ERR_INVALID_ARG;
                }

                if (adsmConfig == 1) {
                    /* restore asdm_int_div */
                    result = hyPhy20gRegFieldWrite(fileHndl, adsmIntDiv_reg,
                                                   adsmIntDiv_msk,
                                                   adsmIntDiv_val);
                    if (result) return result;

                    /* toggle ADSM_RSTB */
                    result = hyPhy20gRegFieldWrite(fileHndl, adsmRstb_reg,
                                                   adsmRstb_msk, 0);
                    if (result) return result;
                    result = hyPhy20gRegFieldWrite(fileHndl, adsmRstb_reg,
                                                   adsmRstb_msk, 1);
                    if (result) return result;

                    switch (intf) {
                        case HYPHY20G_SERDES_BACKPLANE_INTF:
                            /* Backplane */
                            result = utilHyPhy20gSerdesGetBackplaneCsuId(
                                                     fileHndl, linkID, &csu_id);
                            if (result) return result;

                            if (csu_id == 0) {
                                adsm0Done = 1;
                            } else {
                                adsm1Done = 1;
                            }
                            break;
                        case HYPHY20G_SERDES_PIF_INTF:
                            /* PIF */
                            adsm0Done = 1;
                            break;
                        default:
                            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                            return HYPHY20G_ERR_INVALID_ARG;
                    }
                }
            }
        }
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesRestore */

/*******************************************************************************
**
**  utilHyPhy20gSerdesIntfMskRestore
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure re-configures links back the way they were before
**                  ADC offset calibration for a set of links.
**
**  INPUTS:         fileHndl - device handle
**                  sfpMsk   - Mask of SFP links to calibrate
**                                 bit 0 : SFP #0
**                                 bit 1 : SFP #1
**                                 ..
**                                 bit 15: SFP #15
**                  xfiMsk   - Mask of XFI links to calibrate
**                                 bit 0 : XFI #0
**                                 bit 1 : XFI #1
**                  bpMsk    - Mask of backplane links to calibrate
**                                 bit 0 : link #0
**                                 bit 1 : link #1
**                                 ..
**                                 bit 31: link #31
**                  pifMsk   - Mask of PIF links to calibrate
**                                 bit 0 : link #0
**                                 bit 1 : link #1
**                                 ..
**                                 bit 7 : link #7
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesIntfMskRestore(struct file *fileHndl, UINT4 sfpMsk,
                                      UINT4 xfiMsk, UINT4 bpMsk, UINT4 pifMsk)
{
    UINT4 intf;
    UINT4 cfgMsk;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((sfpMsk > 0xFFFF) || (xfiMsk > 0x3) || (bpMsk > 0xFFFFFFFF) ||
        (pifMsk > 0xFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (intf = 0; intf < 4; intf++) {
        switch (intf) {
            case HYPHY20G_SERDES_SFP_INTF:
                /* SFP */
                cfgMsk = sfpMsk;
                break;
            case HYPHY20G_SERDES_XFI_INTF:
                /* XFI */
                cfgMsk = xfiMsk;
                break;
            case HYPHY20G_SERDES_BACKPLANE_INTF:
                /* Backplane */
                cfgMsk = bpMsk;
                break;
            case HYPHY20G_SERDES_PIF_INTF:
                /* PIF */
                cfgMsk = pifMsk;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        if (cfgMsk != 0) {
            result = utilHyPhy20gSerdesRestore(fileHndl, intf, cfgMsk);
            if (result) return result;
        }
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesIntfMskRestore */

/*******************************************************************************
**
**  utilHyPhy20gSerdesAdcSingleRun
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure for calibrating specified links in parallel
**                  across all interface.
**
**  INPUTS:         fileHndl - device handle
**                  sfpMsk   - Mask of SFP links to calibrate
**                                 bit 0 : SFP #0
**                                 bit 1 : SFP #1
**                                 ..
**                                 bit 15: SFP #15
**                  xfiMsk   - Mask of XFI links to calibrate
**                                 bit 0 : XFI #0
**                                 bit 1 : XFI #1
**                  bpMsk    - Mask of backplane links to calibrate
**                                 bit 0 : link #0
**                                 bit 1 : link #1
**                                 ..
**                                 bit 31: link #31
**                  pifMsk   - Mask of PIF links to calibrate
**                                 bit 0 : link #0
**                                 bit 1 : link #1
**                                 ..
**                                 bit 7 : link #7
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesAdcSingleRun(struct file *fileHndl, UINT4 sfpMsk,
                                    UINT4 xfiMsk, UINT4 bpMsk, UINT4 pifMsk)
{
    UINT4 sfpPhase, xfiPhase, bpPhase, pifPhase;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((sfpMsk > 0xFFFF) || (xfiMsk > 0x3) || (bpMsk > 0xFFFFFFFF) ||
        (pifMsk > 0xFF)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* calibrate ADCs on selected links */

    /* process 1 */
    sfpPhase = xfiPhase = bpPhase = pifPhase = 0;
    result = utilHyPhy20gSerdesIntfMskCalibrateOnePhase(fileHndl, sfpMsk,
                                                        sfpPhase, xfiMsk,
                                                        xfiPhase, bpMsk,
                                                        bpPhase, pifMsk,
                                                        pifPhase);
    if (result) return result;
    result = sysHyPhy20gMsDelay(5);
    if (result) return result;


    /* process 2 */
    sfpPhase = 1;
    result = utilHyPhy20gSerdesIntfMskCalibrateOnePhase(fileHndl, sfpMsk,
                                                        sfpPhase, 0, 0, 0, 0,
                                                        0, 0);
    if (result) return result;
    result = sysHyPhy20gMsDelay(5);
    if (result) return result;


    /* process 3 */
    sfpPhase = 2;
    xfiPhase = 1;
    result = utilHyPhy20gSerdesIntfMskCalibrateOnePhase(fileHndl, sfpMsk,
                                                        sfpPhase, xfiMsk,
                                                        xfiPhase, 0, 0, 0, 0);
    if (result) return result;
    result = sysHyPhy20gMsDelay(5);
    if (result) return result;


    /* process 4 */
    sfpPhase = 3;
    result = utilHyPhy20gSerdesIntfMskCalibrateOnePhase(fileHndl, sfpMsk,
                                                        sfpPhase, 0, 0, 0, 0,
                                                        0, 0);
    if (result) return result;
    result = sysHyPhy20gMsDelay(5);
    if (result) return result;


    /* process 5 */
    sfpPhase = 4;
    xfiPhase = 2;
    bpPhase = 1;
    pifPhase = 1;
    result = utilHyPhy20gSerdesIntfMskCalibrateOnePhase(fileHndl, sfpMsk,
                                                        sfpPhase, xfiMsk,
                                                        xfiPhase, bpMsk,
                                                        bpPhase, pifMsk,
                                                        pifPhase);
    if (result) return result;
    result = sysHyPhy20gMsDelay(5);
    if (result) return result;


    /* process 6 */
    sfpPhase = 5;
    result = utilHyPhy20gSerdesIntfMskCalibrateOnePhase(fileHndl, sfpMsk,
                                                        sfpPhase, 0, 0, 0, 0,
                                                        0, 0);
    if (result) return result;
    result = sysHyPhy20gMsDelay(5);
    if (result) return result;


    /* process 7 */
    sfpPhase = 6;
    xfiPhase = 3;
    result = utilHyPhy20gSerdesIntfMskCalibrateOnePhase(fileHndl, sfpMsk,
                                                        sfpPhase, xfiMsk,
                                                        xfiPhase, 0, 0, 0, 0);
    if (result) return result;
    result = sysHyPhy20gMsDelay(5);
    if (result) return result;


    /* process 8 */
    sfpPhase = 7;
    result = utilHyPhy20gSerdesIntfMskCalibrateOnePhase(fileHndl, sfpMsk,
                                                        sfpPhase, 0, 0, 0, 0,
                                                        0, 0);
    if (result) return result;
    result = sysHyPhy20gMsDelay(5);
    if (result) return result;


    /* process 9 */
    sfpPhase = 8;
    xfiPhase = 4;
    bpPhase = 2;
    pifPhase = 2;
    result = utilHyPhy20gSerdesIntfMskCalibrateOnePhase(fileHndl, sfpMsk,
                                                        sfpPhase, xfiMsk,
                                                        xfiPhase, bpMsk,
                                                        bpPhase, pifMsk,
                                                        pifPhase);
    if (result) return result;
    result = sysHyPhy20gMsDelay(5);
    if (result) return result;


    /* process 10 */
    sfpPhase = 9;
    result = utilHyPhy20gSerdesIntfMskCalibrateOnePhase(fileHndl, sfpMsk,
                                                        sfpPhase, 0, 0, 0, 0,
                                                        0, 0);
    if (result) return result;
    result = sysHyPhy20gMsDelay(5);
    if (result) return result;


    /* process 11 */
    sfpPhase = 10;
    xfiPhase = 5;
    result = utilHyPhy20gSerdesIntfMskCalibrateOnePhase(fileHndl, sfpMsk,
                                                        sfpPhase, xfiMsk,
                                                        xfiPhase, 0, 0, 0, 0);
    if (result) return result;
    result = sysHyPhy20gMsDelay(5);
    if (result) return result;


    /* process 12 */
    sfpPhase = 11;
    result = utilHyPhy20gSerdesIntfMskCalibrateOnePhase(fileHndl, sfpMsk,
                                                        sfpPhase, 0, 0, 0, 0,
                                                        0, 0);
    if (result) return result;
    result = sysHyPhy20gMsDelay(5);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesAdcSingleRun */

/*******************************************************************************
**
**  utilHyPhy20gSerdesAdcCalibrate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function is to be called from *RateCfg APIs to
**                  calibrate a single link.
**
**                  Function updates the SERDES software context parameters,
**                  calMode and calReq, based on desired ADC calibration mode,
**                  adcCal, then runs hyPhy20gADCCalibrate if necessary.
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 0 - SFP
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkID   - SERDES link
**                                 SFP       - 0 to 15
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**                  adcCal   - 0 - do not perform ADC calibration
**                             1 - perform ADC calibration
**                             2 - use ADC calibration offset from SW context
**                             3 - defer ADC offset calibration (use
**                                 hyPhy20gSerdesADCCalibrate later)
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SERDES_ADC_SFP_RATE_CHECK
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_SERDES_ADC_CALIBRATION_FAILED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesAdcCalibrate(struct file *fileHndl, UINT4 intf,
                                    UINT4 linkID, UINT4 adcCal)
{
    UINT4 calMode, calReq;
    UINT4 calMsk, calFail;
    UINT4 setMskCtxt;
    UINT4 dummy = 0;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((intf > 3) || (intf == 0 && linkID > 15) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7) ||
        (adcCal > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* update SERDES ADC calibration SW context */
    if (adcCal == 0) {
        /* no calibration */
        calMode = HYPHY20G_SERDES_NO_CAL_MODE;
        calReq = 0;
        setMskCtxt = 0x6;
        dummy = 0;
    } else if (adcCal == 1) {
        /* run calibration */
        calMode = HYPHY20G_SERDES_DYNAMIC_CAL_MODE;
        calReq = 1;
        setMskCtxt = 0x6;
        dummy = 0;
    } else if (adcCal == 2) {
        /* factory calibration - check calMode */
        result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl, intf, linkID,
                                                   &dummy, &calMode, &dummy,
                                                   &dummy, &dummy, &dummy);
        if (result) return result;

        if (calMode != HYPHY20G_SERDES_FACTORY_CAL_MODE) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }

        calReq = 0;
        setMskCtxt = 0x0;
        dummy = 0;
    } else {
        /* deferred calibration */
        calMode = HYPHY20G_SERDES_DEFER_CAL_MODE;
        calReq = 1;
        setMskCtxt = 0x6;
        dummy = 0;
    }
    result = serdesHyPhy20gSerdesSetMskAdcCalCtxt(fileHndl, intf, linkID, dummy,
                                                  calMode, calReq, dummy, dummy,
                                                  dummy, setMskCtxt);
    if (result) return result;

    if (adcCal == 1) {
        /* run calibration */
        calMsk = 0x1 << linkID;

        switch (intf) {
            case HYPHY20G_SERDES_SFP_INTF:
                /* SFP */
                result = hyPhy20gSerdesADCCalibrate(fileHndl, calMsk, 0, 0, 0,
                                                    0, &calFail, &dummy, &dummy,
                                                    &dummy);
                if (result) return result;
                break;
            case HYPHY20G_SERDES_XFI_INTF:
                /* XFI */
                result = hyPhy20gSerdesADCCalibrate(fileHndl, 0, calMsk, 0, 0,
                                                    0, &dummy, &calFail, &dummy,
                                                    &dummy);
                if (result) return result;
                break;
            case HYPHY20G_SERDES_BACKPLANE_INTF:
                /* Backplane */
                result = hyPhy20gSerdesADCCalibrate(fileHndl, 0, 0, calMsk, 0,
                                                    0, &dummy, &dummy, &calFail,
                                                    &dummy);
                if (result) return result;
                break;
            case HYPHY20G_SERDES_PIF_INTF:
                /* PIF */
                result = hyPhy20gSerdesADCCalibrate(fileHndl, 0, 0, 0, calMsk,
                                                    0, &dummy, &dummy, &dummy,
                                                    &calFail);
                if (result) return result;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }

        if (calFail != 0) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_CALIBRATION_FAILED\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_SERDES_ADC_CALIBRATION_FAILED;
        }
    } else if (adcCal == 2) {
        /* factory calibration */
        switch (intf) {
            case HYPHY20G_SERDES_SFP_INTF:
                /* SFP */
                result = utilHyPhy20gWriteSfpAdcMemory(fileHndl, linkID);
                if (result) return result;
                break;
            case HYPHY20G_SERDES_XFI_INTF:
            case HYPHY20G_SERDES_BACKPLANE_INTF:
            case HYPHY20G_SERDES_PIF_INTF:
                /* XFI, Backplane and PIF */

                /* apply offsets */
                result = utilHyPhy20gWriteXfiBpPifAdcMemory(fileHndl, intf,
                                                            linkID);
                if (result) return result;

                /* set context calOk to 1 */
                result = serdesHyPhy20gSerdesSetMskAdcCalCtxt(fileHndl, intf,
                                                              linkID, dummy,
                                                              dummy, dummy,
                                                              1, dummy, dummy,
                                                              0x8);
                if (result) return result;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesAdcCalibrate */

/*******************************************************************************
**
**  utilHyPhy20gSerdesGetMaxLinks
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function returns the maximum number of links
**                  for a specified interface.
**
**  INPUTS:         fileHndl   - device handle
**                  intf       - SERDES interface
**                                   0 - SFP
**                                   1 - XFI
**                                   2 - Backplane
**                                   3 - PIF
**                  *pMaxLinks - pointer to storage for maximum links
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesGetMaxLinks(struct file *fileHndl, UINT4 intf,
                                   UINT4 *pMaxLinks)
{
    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if (intf > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            *pMaxLinks = HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM + 1;
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            *pMaxLinks = HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM + 1;
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            *pMaxLinks = HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM + 1;
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            *pMaxLinks = HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM + 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesGetMaxLinks */

/*******************************************************************************
**
**  utilHyPhy20gSerdesGetBackplaneCsuId
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function returns the CSU ID associated with the
**                  backplane link.
**
**  INPUTS:         fileHndl   - device handle
**                  linkID     - Backplane link
**                               Legal range 0 to 31
**                  *pCsuID    - pointer to storage for CSU ID
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesGetBackplaneCsuId(struct file *fileHndl, UINT4 linkID,
                                         UINT4 *pCsuID)
{
    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if (linkID > 31) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* csu[0] corresponds to links 0 to 7 and 16 to 23  */
    /* csu[1] corresponds to links 8 to 15 and 24 to 31 */
    if (((linkID >= 0) && (linkID <= 7)) ||
        ((linkID >= 16) && (linkID <= 23))) {
        *pCsuID = 0;
    } else {
        *pCsuID = 1;
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesGetBackplaneCsuId */

/*******************************************************************************
**
**  utilHyPhy20gSerdesGetMaxNPhaseNComp
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function returns the maximum NPHASES and NCOMPS
**                  required for each interface.
**
**  INPUTS:         fileHndl   - device handle
**                  intf       - SERDES interface
**                                   0 - SFP
**                                   1 - XFI
**                                   2 - Backplane
**                                   3 - PIF
**                  *pNPhases  - pointer to storage for NPHASES
**                  *pNComps   - pointer to storage for NCOMPS
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesGetMaxNPhaseNComp(struct file *fileHndl, UINT4 intf,
                                         UINT4 *pNPhases, UINT4 *pNComps)
{
    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if (intf > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            *pNPhases = HYPHY20G_SERDES_NPHASES_SFP;
            *pNComps = HYPHY20G_SERDES_NCOMPS_SFP;
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            *pNPhases = HYPHY20G_SERDES_NPHASES_XFI;
            *pNComps = HYPHY20G_SERDES_NCOMPS_XFI;
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            *pNPhases = HYPHY20G_SERDES_NPHASES_BACKPLANE;
            *pNComps = HYPHY20G_SERDES_NCOMPS_BACKPLANE;
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            *pNPhases = HYPHY20G_SERDES_NPHASES_PIF;
            *pNComps = HYPHY20G_SERDES_NCOMPS_PIF;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesGetMaxNPhaseNComp */

/*******************************************************************************
**
**  utilHyPhy20gSerdesRxFifoReCenter
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function re-centers the receive SERDES FIFO for the
**                  specified interface link.
**
**  INPUTS:         fileHndl   - device handle
**                  intf       - SERDES interface
**                                   0 - SFP
**                                   1 - XFI
**                                   2 - Backplane
**                                   3 - PIF
**                  linkID     - Link ID
**                                   0 to 15 - SFP Interface 0 to 15
**                                   17      - SFP SGMII 17
**                                   0 to 1  - XFI Interface 0 to 1
**                                   0 to 31 - Backplane Interface 0 to 31
**                                   0 to 7  - PIF Interface 0 to 7
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesRxFifoReCenter(struct file *fileHndl, UINT4 intf,
                                      UINT4 linkID)
{
    UINT4 addr, msk;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((intf > 3) || (intf == 0 && (linkID == 16 || linkID > 17)) ||
        (intf == 1 && linkID > 1) || (intf == 2 && linkID > 31) ||
        (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (intf) {
        case 0:
            /* SFP */
            addr = HYPHY20G_SERDES_SFP_DELTA8_CDRU_REG_FIFO;
            addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(addr, linkID);
            msk = HYPHY20G_SERDES_SFP_DELTA8_CDRU_BIT_FIFO_FIFO_SW_RESET_MSK;
            break;
        case 1:
            /* XFI */
            addr = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_FIFO;
            addr = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(addr, linkID);
            msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_FIFO_FIFO_SW_RESET_MSK;
            break;
        case 2:
            /* Backplane */
            addr = HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_FIFO;
            addr = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(addr, linkID);
            msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_FIFO_FIFO_SW_RESET_MSK;
            break;
        case 3:
            /* PIF */
            addr = HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_FIFO;
            addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(addr, linkID);
            msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_FIFO_FIFO_SW_RESET_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* toggle FIFO_SW_RESET bit */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if (result) return result;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesRxFifoReCenter */

/*******************************************************************************
**
**  utilHyPhy20gSerdesCalcTxSwing
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function calculates the output swing of the
**                  transmitter.
**
**  INPUTS:         fileHndl         - device handle
**                  preEmp           - 0 - 0dB
**                                     1 - 0.357dB
**                                     2 - 0.736dB
**                                     3 - 1.13dB
**                                     4 - 1.55dB
**                                     5 - 1.98dB
**                                     6 - 2.45dB
**                                     7 - 2.94dB
**                                     8 - 3.46dB
**                                     9 - 4.01dB
**                                     10 - 4.61dB
**                                     11 - 5.24dB
**                                     12 - 5.94dB
**                                     13 - 6.69dB
**                                     14 - 7.52dB
**                                     15 - 8.42dB
**                  swing            - 3..48
**                                     swing + 2*preEmp <= 48
**                  *pTxDenCore      - pointer to storage for TX_DEN_CORE
**                  *pTxSwngCtrlCore - pointer to storage for TX_SWING_CTRL_CORE
**
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesCalcTxSwing(struct file *fileHndl, UINT4 preEmp,
                                   UINT4 swing, UINT4 *pTxDenCore,
                                   UINT4 *pTxSwngCtrlCore)
{
    UINT4 pWeight[13] = {3,1,2,4,8,1,2,4,8,1,2,4,8};
    UINT4 pSwingU[13];
    UINT4 pConst[4];
    UINT4 pDeEmpU[13];
    UINT4 pDenCore[13];
    UINT4 pCtrlCore[13];
    UINT4 pConstShift = 5;
    UINT4 tmp, i;
    UINT4 sumInputs;
    UINT4 maxSwingUBit;
    UINT4 sumSwingU;
    UINT4 sumSwingU_4To0, sumSwingU_8To0;
    UINT4 maxConstBit;
    UINT4 sumDeEmpU;
    UINT4 bit;

    /* argument checking */
    if (preEmp > 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if ((swing < 3) || (swing > 48)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (((swing + (2*preEmp)) < 1)  || ((swing + (2*preEmp)) > 48)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Calculate DEN_CORE[0] */
    sumInputs = preEmp + swing;
    pDenCore[0] = (sumInputs >= pWeight[0]) ? 1 : 0;
    pSwingU[0] = pDenCore[0] * pWeight[0];

    /* Calculate DEN_CORE[4] */
    tmp = sumInputs - pSwingU[0];
    pDenCore[4] = (tmp >= pWeight[4]) ? 1 : 0;
    pSwingU[4] = pDenCore[4] * pWeight[4];

    /* Calculate DEN_CORE[3:1] */
    maxSwingUBit = 4;
    for (bit = 3; bit >= 1; bit--) {
        sumSwingU = 0;
        for (i = bit+1; i <= maxSwingUBit; i++) {
            sumSwingU = sumSwingU + pSwingU[i];
        }
        pDenCore[bit] = ((tmp-sumSwingU) >= pWeight[bit]) ? 1 : 0;
        pSwingU[bit] = pDenCore[bit] * pWeight[bit];
    }

    /* Calculate DEN_CORE[8] */
    pConst[8-pConstShift] = (preEmp >= pWeight[8]) ? 1 : 0;
    sumSwingU_4To0 = 0;
    for (i = 0; i <= 4; i++) {
        sumSwingU_4To0 = sumSwingU_4To0 + pSwingU[i];
    }
    pDenCore[8] = (((sumInputs - sumSwingU_4To0) >= pWeight[8]) &&
                     pConst[8-pConstShift] < 1) ? 1 : 0;
    pSwingU[8] = pDenCore[8] * pWeight[8];
    pDeEmpU[8] = pWeight[8] * pConst[8-pConstShift];

    /* Calculate DEN_CORE[7:5] */
    maxConstBit = 8;
    for (bit = 7; bit >= 5; bit--) {
        sumDeEmpU = 0;
        for (i = bit+1; i <= maxConstBit; i++) {
            sumDeEmpU = sumDeEmpU + pDeEmpU[i];
        }
        pConst[bit-pConstShift] = ((preEmp - sumDeEmpU) >= pWeight[bit]) ? 1 : 0;
        pDeEmpU[bit] = pWeight[bit] * pConst[bit-pConstShift];

        sumSwingU = 0;
        maxSwingUBit = 8;
        for (i = bit+1; i <= maxSwingUBit; i++) {
            sumSwingU = sumSwingU + pSwingU[i];
        }
        pDenCore[bit] = (((sumInputs - sumSwingU_4To0 - sumSwingU) >= pWeight[bit])
                           && pConst[bit-pConstShift] < 1)? 1 : 0;
        pSwingU[bit] = pDenCore[bit] * pWeight[bit];
    }

    /* Calculate DEN_CORE[12] */
    sumSwingU_8To0 = 0;
    for (i = 0; i <= 8; i++) {
        sumSwingU_8To0 = sumSwingU_8To0 + pSwingU[i];
    }
    pDenCore[12] = ((sumInputs - sumSwingU_8To0) >= pWeight[12]) ? 1 : 0;
    pSwingU[12] = pDenCore[12] * pWeight[12];

    /* Calculate DEN_CORE[11:9] */
    maxSwingUBit = 12;
    for (bit = 11; bit >= 9; bit--) {
        sumSwingU = 0;
        for (i = bit+1; i <= maxSwingUBit; i++) {
            sumSwingU = sumSwingU + pSwingU[i];
        }
        pDenCore[bit] = ((sumInputs - sumSwingU_8To0 - sumSwingU) >= pWeight[bit])
                          ? 1 : 0;
        pSwingU[bit] = pDenCore[bit] * pWeight[bit];
    }

    for (bit = 0; bit <= 12; bit++) {
        if (bit >= 5 && bit <= 8) {
            pCtrlCore[bit] = 0x1 - pConst[bit-pConstShift] - pDenCore[bit];
        } else {
            pCtrlCore[bit] = 0x1 - pDenCore[bit];
        }
    }

    for (i = 0; i <= 12; i++){
        *pTxDenCore |= (pDenCore[i] << i) ;
        *pTxSwngCtrlCore |= (pCtrlCore[i] << i);
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesCalcTxSwing */

/*******************************************************************************
**
**  utilHyPhy20gWriteXfiBpPifAdcMemory
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure for updating the ADC memory based on calibrated
**                  offsets saved in software context.  Only valid for XFI,
**                  Backplane or PIF interfaces.
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkID   - SERDES link
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gWriteXfiBpPifAdcMemory(struct file *fileHndl, UINT4 intf,
                                UINT4 linkID)
{
    UINT4 nPhases, phase, nComps, comp;
    UINT4 accumRefSel_reg;
    UINT4 accumRefSel_msk, accum_ref_val;
    UINT4 adcMode_reg, adcModeA_reg, adcModeB_reg, adcModeC_reg, adcModeD_reg;
    UINT4 adcMode_msk, adcModeA_msk, adcModeB_msk, adcModeC_msk, adcModeD_msk;
    UINT4 occAdc_reg;
    UINT4 occAdcBypass_msk, occAdcA_msk, occAdcD_msk, occAdcWr_msk;
    UINT4 ref[4];
    UINT4 offset;
    UINT4 reg_val;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((intf < 1) || (intf > 3) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = utilHyPhy20gSerdesGetMaxNPhaseNComp(fileHndl, intf, &nPhases,
                                                 &nComps);
    if (result) return result;

    switch (intf) {
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            accumRefSel_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_20, linkID);
            accumRefSel_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_20_ACCUM_REF_SEL_MSK;

            adcModeA_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_21, linkID);
            adcModeA_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_21_ACCUM_REF0_MSK;

            adcModeB_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_22, linkID);
            adcModeB_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_22_ACCUM_REF1_MSK;

            adcModeC_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_23, linkID);
            adcModeC_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_23_ACCUM_REF2_MSK;

            adcModeD_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_24, linkID);
            adcModeD_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_24_ACCUM_REF3_MSK;

            occAdc_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
            occAdcBypass_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_BYPASS_EN_MSK;
            occAdcA_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_A_MSK;
            occAdcD_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_D_MSK;
            occAdcWr_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_WR_MSK;
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            accumRefSel_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_BP_C8_REG_CFG_11, linkID);
            accumRefSel_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_11_ACCUM_REF_SEL_MSK;

            adcModeA_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_BP_C8_REG_CFG_12, linkID);
            adcModeA_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_12_ACCUM_REF0_MSK;

            adcModeB_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_BP_C8_REG_CFG_13, linkID);
            adcModeB_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_13_ACCUM_REF1_MSK;

            adcModeC_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_BP_C8_REG_CFG_14, linkID);
            adcModeC_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_14_ACCUM_REF2_MSK;

            adcModeD_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_BP_C8_REG_CFG_15, linkID);
            adcModeD_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_15_ACCUM_REF3_MSK;

            occAdc_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                       HYPHY20G_SERDES_BP_C8_REG_CFG_9, linkID);
            occAdcBypass_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_9_OCC_ADC_BYPASS_EN_MSK;
            occAdcA_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_9_OCC_ADC_A_MSK;
            occAdcD_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_9_OCC_ADC_D_MSK;
            occAdcWr_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_9_OCC_ADC_WR_MSK;
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            accumRefSel_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                     HYPHY20G_SERDES_PIF_C8_REG_CFG_11, linkID);
            accumRefSel_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_11_ACCUM_REF_SEL_MSK;

            adcModeA_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                     HYPHY20G_SERDES_PIF_C8_REG_CFG_12, linkID);
            adcModeA_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_12_ACCUM_REF0_MSK;

            adcModeB_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                     HYPHY20G_SERDES_PIF_C8_REG_CFG_13, linkID);
            adcModeB_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_13_ACCUM_REF1_MSK;

            adcModeC_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                     HYPHY20G_SERDES_PIF_C8_REG_CFG_14, linkID);
            adcModeC_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_14_ACCUM_REF2_MSK;

            adcModeD_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                     HYPHY20G_SERDES_PIF_C8_REG_CFG_15, linkID);
            adcModeD_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_15_ACCUM_REF3_MSK;

            occAdc_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                      HYPHY20G_SERDES_PIF_C8_REG_CFG_9, linkID);
            occAdcBypass_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_9_OCC_ADC_BYPASS_EN_MSK;
            occAdcA_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_9_OCC_ADC_A_MSK;
            occAdcD_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_9_OCC_ADC_D_MSK;
            occAdcWr_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_9_OCC_ADC_WR_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* read adc_mode */
    result = hyPhy20gRegFieldRead(fileHndl, accumRefSel_reg, accumRefSel_msk,
                                  &accum_ref_val);
    if (result) return result;

    switch(accum_ref_val){
        case HYPHY20G_SERDES_ADC_MODE_A:
            /* adc_mode "A" */
            adcMode_reg = adcModeA_reg;
            adcMode_msk = adcModeA_msk;
        break;
        case HYPHY20G_SERDES_ADC_MODE_B:
            /* adc_mode "B" */
            adcMode_reg = adcModeB_reg;
            adcMode_msk = adcModeB_msk;
        break;
        case HYPHY20G_SERDES_ADC_MODE_C:
            /* adc_mode "C" */
            adcMode_reg = adcModeC_reg;
            adcMode_msk = adcModeC_msk;
        break;
        case HYPHY20G_SERDES_ADC_MODE_D:
            /* adc_mode "D" */
            adcMode_reg = adcModeD_reg;
            adcMode_msk = adcModeD_msk;
        break;
        default:
            /* invalid read value */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* enter bypass mode */
    result = hyPhy20gRegFieldWrite(fileHndl, occAdc_reg, occAdcBypass_msk, 1);
    if (result) return result;

    /* setup reference */
    result = hyPhy20gRegFieldRead(fileHndl, adcMode_reg, adcMode_msk, &reg_val);
    if (result) return result;
    ref[3] = reg_val & 0x000000FF;
    ref[2] = (reg_val & 0x0000FF00) >> 8;
    ref[1] = (reg_val & 0x00FF0000) >> 16;
    ref[0] = (reg_val & 0xFF000000) >> 24;
    DBG_PRINT("%s, %s, %d, ref[0] = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (ref[0]));
    DBG_PRINT("%s, %s, %d, ref[1] = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (ref[1]));
    DBG_PRINT("%s, %s, %d, ref[2] = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (ref[2]));
    DBG_PRINT("%s, %s, %d, ref[3] = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (ref[3]));

    /* calculate systematic offset */
    for (phase = 0; phase < nPhases; phase++) {
        for (comp = 0; comp < nComps; comp++) {

            result = sysHyPhy20gRead(fileHndl, occAdc_reg, &reg_val);
            if (result) return result;

            /* configure OCC_ADC_A */
            result = hyPhy20gVarFieldWrite(&reg_val, occAdcA_msk,
                                           (phase*16) + comp);
            if (result) return result;

            /* offsets are 8-bit two's complement */
            result = serdesHyPhy20gSerdesGetAdcCalOffsetCtxt(fileHndl, intf,
                                                  linkID, phase, comp, &offset);
            if (result) return result;
            DBG_PRINT("%s, %s, %d, prev offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (offset), (unsigned int) (phase), (unsigned int) (comp));

            if (comp < 4) {
                /* subract ref from offset */
                offset = offset - ref[comp];
                if (offset > 0x7FFFFFFF) {
                    /* convert back to 8-bit two's complement from 32-bit */
                    offset = 256 - ((~offset)+1);
                } else if (offset > 0xFF) {
                    /* rolling over */
                    offset = offset - 256;
                }
            } else if (comp == 4) {
                /* do nothing for: offset = offset - 0*/
            } else {
                /* add ref to offset */
                offset = offset + ref[comp-((comp-4)*2)];
                if (offset > 255) {
                    /* convert back to 8 bit two's complement from 32bit */
                    offset = offset - 256;
                }
            }
            DBG_PRINT("%s, %s, %d, curr offset = %u for nphase = %u, comp = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (offset), (unsigned int) (phase), (unsigned int) (comp));

            /* configure OCC_ADC_D */
            result = hyPhy20gVarFieldWrite(&reg_val, occAdcD_msk, offset);
            if (result) return result;

            /* write OCC_ADC_A, OCC_ADC_D to register */
            result = sysHyPhy20gWrite(fileHndl, occAdc_reg, reg_val);
            if (result) return result;

            /* trigger write */
            result = hyPhy20gRegFieldWrite(fileHndl, occAdc_reg, occAdcWr_msk,
                                           1);
            if (result) return result;

            /* clear OCC_ADC_WR */
            result = hyPhy20gRegFieldWrite(fileHndl, occAdc_reg, occAdcWr_msk,
                                           0);
            if (result) return result;
        }
    }

    /* exit bypass mode */
    result = hyPhy20gRegFieldWrite(fileHndl, occAdc_reg, occAdcBypass_msk, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gWriteXfiBpPifAdcMemory */

/*******************************************************************************
**
**  utilHyPhy20gWriteSfpAdcMemory
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure for updating the ADC memory based on calibrated
**                  offsets saved in software context.  Only valid for SFP
**                  interface.
**
**  INPUTS:         fileHndl - device handle
**                  linkID   - SFP SERDES link
**                             Legal range 0 to 15
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gWriteSfpAdcMemory(struct file *fileHndl, UINT4 linkID)
{
    UINT4 nPhases, phase, nComps, comp;
    UINT4 occAdc_reg;
    UINT4 occAdcBypass_msk, occAdcA_msk, occAdcD_msk, occAdcWr_msk;
    UINT4 offset;
    UINT4 reg_val;
    UINT4 intf;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if (linkID > 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* initialize variables */
    intf = HYPHY20G_SERDES_SFP_INTF;
    occAdc_reg = HYPHY20G_SERDES_SFP_CORE_REG_CFG_16;
    occAdc_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(occAdc_reg, linkID);
    occAdcBypass_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_BYPASS_EN_MSK;
    occAdcA_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_A_MSK;
    occAdcD_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_D_MSK;
    occAdcWr_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CFG_16_OCC_ADC_WR_MSK;

    result = utilHyPhy20gSerdesGetMaxNPhaseNComp(fileHndl, intf, &nPhases,
                                                 &nComps);
    if (result) return result;

    /* enter bypass mode */
    result = hyPhy20gRegFieldWrite(fileHndl, occAdc_reg, occAdcBypass_msk, 1);
    if (result) return result;

    /* calculate systematic offset */
    for (phase = 0; phase < nPhases; phase++) {
        for (comp = 0; comp < nComps; comp++) {

            result = sysHyPhy20gRead(fileHndl, occAdc_reg, &reg_val);
            if (result) return result;

            /* configure OCC_ADC_A */
            result = hyPhy20gVarFieldWrite(&reg_val, occAdcA_msk,
                                           (phase*16) + comp);
            if (result) return result;

            /* offsets are 8-bit two's complement */
            result = serdesHyPhy20gSerdesGetAdcCalOffsetCtxt(fileHndl, intf,
                                                  linkID, phase, comp, &offset);
            if (result) return result;
            DBG_PRINT("%s, %s, %d, nphase = %u, comp = %u, offset = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) (phase), (unsigned int) (comp), (unsigned int) (offset));

            /* configure OCC_ADC_D */
            result = hyPhy20gVarFieldWrite(&reg_val, occAdcD_msk, offset);
            if (result) return result;

            /* write OCC_ADC_A, OCC_ADC_D to register */
            result = sysHyPhy20gWrite(fileHndl, occAdc_reg, reg_val);
            if (result) return result;

            /* trigger write */
            result = hyPhy20gRegFieldWrite(fileHndl, occAdc_reg, occAdcWr_msk,
                                           1);
            if (result) return result;

            /* clear OCC_ADC_WR */
            result = hyPhy20gRegFieldWrite(fileHndl, occAdc_reg, occAdcWr_msk,
                                           0);
            if (result) return result;
        }
    }

    /* exit bypass mode */
    result = hyPhy20gRegFieldWrite(fileHndl, occAdc_reg, occAdcBypass_msk, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gWriteSfpAdcMemory */

/*******************************************************************************
**
**  utilHyPhy20gSetAdcThreshold
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure configures the linear or non-linear ADC threshold
**                  settings based on the scaling factor.  This function is
**                  only valid for XFI, Backplane and PIF interfaces.  Not
**                  available for SFP because mode selection is not applicable.
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkID   - SERDES link
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**                  mapType  - mapping type for ADC threshold
**                                 0 - linear ADC threshold map
**                                 1 - non-linear ADC threshold map
**                  scale    - scaling factor
**                                 1 to 8 - linear
**                                 1 to 5 - non-linear
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSetAdcThreshold(struct file *fileHndl, UINT4 intf,
                                 UINT4 linkID, UINT4 mapType, UINT4 scale)
{
    UINT4 mapReg, nonLinMapMsk;
    UINT4 accumRef0Reg, accumRef0Msk, accumRef0;
    UINT4 scaleFactor;
    UINT4 i;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((intf < 1) || (intf > 3) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7) ||
        (mapType > 1) || (scale < 1) || (mapType == 0 && scale > 8) ||
        (mapType == 1 && scale > 5)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (intf) {
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            mapReg = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_1;
            mapReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(mapReg, linkID);
            nonLinMapMsk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_1_NON_LINEAR_MAP_MSK;

            accumRef0Reg = HYPHY20G_SERDES_XFI_CORE_REG_CFG_21;
            accumRef0Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                                          accumRef0Reg, linkID);
            accumRef0Msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_21_ACCUM_REF0_MSK;
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            mapReg = HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_1;
            mapReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(mapReg, linkID);
            nonLinMapMsk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_1_NON_LINEAR_MAP_MSK;

            accumRef0Reg = HYPHY20G_SERDES_BP_C8_REG_CFG_12;
            accumRef0Reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                                          accumRef0Reg, linkID);
            accumRef0Msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_12_ACCUM_REF0_MSK;
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            mapReg = HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_1;
            mapReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(mapReg, linkID);
            nonLinMapMsk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_1_NON_LINEAR_MAP_MSK;

            accumRef0Reg = HYPHY20G_SERDES_PIF_C8_REG_CFG_12;
            accumRef0Reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                                          accumRef0Reg, linkID);
            accumRef0Msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_12_ACCUM_REF0_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure NON_LINEAR_MAP */
    result = hyPhy20gRegFieldWrite(fileHndl, mapReg, nonLinMapMsk, mapType);
    if (result) return result;

    /* calculate scales */
    accumRef0 = 0;
    for (i = 0; i < 4; i++) {
        if (mapType == HYPHY20G_SERDES_LINEAR_MAP) {
            /* linear map scale factors: 1 2 3 4 */
            scaleFactor = i + 1;
        } else {
            /* non-linear map scale factors: 1 2 4 6 */
            scaleFactor = (i == 0) ? 1 : i*2;
        }

        accumRef0 |= (((scaleFactor * scale) & 0xFF) << (i*8));
    }

    /* configure ACCUM_REF0 */
    result = hyPhy20gRegFieldWrite(fileHndl, accumRef0Reg, accumRef0Msk,
                                   accumRef0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSetAdcThreshold */

/*******************************************************************************
**
**  utilHyPhy20gAdcModeSelect
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure will find the optimal ADC threshold settings
**                  using linear and non-linear ADC maps.  This function is
**                  only valid for XFI, Backplane and PIF interfaces.  Not
**                  available for SFP because mode selection is not applicable.
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkID   - SERDES link
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**                  mode     - 0 - hardcoded mode selection
**                             1 - perform mode selection
**                             2 - use mode selection values from SW context
**                             3 - defer mode selection (use
**                                 hyPhy20gSerdesADCCalibrate later)
**
**  OUTPUTS:        None     - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 utilHyPhy20gAdcModeSelect(struct file *fileHndl, UINT4 intf, UINT4 linkID,
                               UINT4 mode)
{
    UINT4 iterations, goal, scale, cal;
    UINT4 linDist, nonLinDist, newDist;
    UINT4 linScale, nonLinScale;
    UINT4 mapType;
    UINT4 dummy;
    UINT4 ctxtValid, ctxtCalOk;
    UINT4 dataBusReg, dataBusMsk;
    UINT4 capReqReg, capReqMsk;
    UINT4 numDataBusReg, dataBusWidth;
    UINT4 numSamples, numSamplesRemain, numSamplesPerReg;
    UINT4 maxScale, maxNum;
    UINT4 posbins, negbins, negEdge, posEdge, negMiddle, posMiddle;
    UINT4 ratio, posRatio, negRatio;
    UINT4 addr, val, sample;
    UINT4 goalUps, linDistUps, nonLinDistUps;
    UINT4 i, j, k;
    UINT4 accumRefSelReg, accumRefSelMsk, accumRefSel;
    UINT4 nonLinMapReg, nonLinMapMsk, nonLinMap;
    UINT4 accumRef0Reg, accumRef0Msk, accumRef0;
    UINT4 occAdcRefReg, occAdcRefMsk;
    UINT4 defaultAccumRef0;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((intf < 1) || (intf > 3) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7) ||
        (mode > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }


    /**
     ** deferred mode selection
     **/
    if (mode == 3) {
        DBG_PRINT("Deferred mode selection\n");

        /* save current mode selection in SW context */
        result = utilHyPhy20gSerdesSaveModeSel(fileHndl, intf, linkID);
        if (result) return result;

        return HYPHY20G_SUCCESS;
    }


    /* constants */
    iterations = 50;            /* number of captures, [54:0] per capture */
    goal       = 25;            /* ideal percent of signal outside registers */

    defaultAccumRef0 = 0x40302010;  /* default ACCUM_REF0 value */

    /* instantiate variables */
    if (intf == HYPHY20G_SERDES_XFI_INTF) {
        mapType = HYPHY20G_SERDES_NONLINEAR_MAP;    /* only do non-linear map */
    } else {
        mapType = HYPHY20G_SERDES_LINEAR_MAP;       /* start with linear map */
    }
    scale = 1;                          /* start with scale of 1 */
    cal   = 1;                          /* calibration flag */
    linDist = nonLinDist = 100;         /* minimum percentage distances:
                                           distance = abs(current_percentage -
                                                          goal) */
    linScale = nonLinScale = 1;         /* ideal scaling factor */

    goalUps = goal*1000;
    linDistUps = linDist*1000;
    nonLinDistUps = nonLinDist*1000;

    switch (intf) {
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            accumRefSelReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_20, linkID);
            accumRefSelMsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_20_ACCUM_REF_SEL_MSK;
            accumRefSel = 0x02;

            nonLinMapReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_1, linkID);
            nonLinMapMsk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_1_NON_LINEAR_MAP_MSK;
            nonLinMap = 0;

            dataBusReg = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_INTERPOLATOR_DAT_BUS_SAMPLE_0_TO_5;
            dataBusReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(dataBusReg,
                                                                   linkID);

            capReqReg = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CPT_INTERPOLATOR_STAT_REQ;
            capReqReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(capReqReg,
                                                                  linkID);
            capReqMsk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CPT_INTERPOLATOR_STAT_REQ_CAPTURE_REQ_MSK;

            accumRef0Reg = HYPHY20G_SERDES_XFI_CORE_REG_CFG_21;
            accumRef0Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                                         accumRef0Reg, linkID);
            accumRef0Msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_21_ACCUM_REF0_MSK;

            occAdcRefReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_XFI_CORE_REG_CFG_18, linkID);
            occAdcRefMsk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_18_OCC_ADC_REFERENCE_MSK;

            numDataBusReg = 10;         /* number of registers for DATA_BUS */
            dataBusWidth = 5;           /* number of bits per sample */
            numSamples = 55;            /* number of samples */
            numSamplesRemain = 1;       /* number of samples in last register */
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            accumRefSelReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_11, linkID);
            accumRefSelMsk = HYPHY20G_SERDES_BP_C8_BIT_CFG_11_ACCUM_REF_SEL_MSK;
            accumRefSel = 0x02;

            nonLinMapReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_1, linkID);
            nonLinMapMsk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_1_NON_LINEAR_MAP_MSK;
            nonLinMap = 0;

            dataBusReg = HYPHY20G_SERDES_BP_CDRU_REG_INTERPOLATOR_DAT_BUS_SAMPLE_0_TO_5;
            dataBusReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(dataBusReg,
                                                                  linkID);

            capReqReg = HYPHY20G_SERDES_BP_CDRU_REG_CPT_INTERPOLATOR_STAT_REQ;
            capReqReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(capReqReg,
                                                                  linkID);
            capReqMsk = HYPHY20G_SERDES_BP_CDRU_BIT_CPT_INTERPOLATOR_STAT_REQ_CAPTURE_REQ_MSK;

            accumRef0Reg = HYPHY20G_SERDES_BP_C8_REG_CFG_12;
            accumRef0Reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                                         accumRef0Reg, linkID);
            accumRef0Msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_12_ACCUM_REF0_MSK;

            occAdcRefReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_BP_C8_REG_CFG_9, linkID);
            occAdcRefMsk = HYPHY20G_SERDES_BP_C8_BIT_CFG_9_OCC_ADC_REFERENCE_MSK;

            numDataBusReg = 6;          /* number of registers for DATA_BUS */
            dataBusWidth = 5;           /* number of bits per sample */
            numSamples = 33;            /* number of samples */
            numSamplesRemain = 3;       /* number of samples in last register */
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            accumRefSelReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_11, linkID);
            accumRefSelMsk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_11_ACCUM_REF_SEL_MSK;
            accumRefSel = 0x02;

            nonLinMapReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_1, linkID);
            nonLinMapMsk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_1_NON_LINEAR_MAP_MSK;
            nonLinMap = 0;

            dataBusReg = HYPHY20G_SERDES_PIF_CDRU_REG_INTERPOLATOR_DAT_BUS_SAMPLE_0_TO_5;
            dataBusReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(dataBusReg,
                                                                   linkID);

            capReqReg = HYPHY20G_SERDES_PIF_CDRU_REG_CPT_INTERPOLATOR_STAT_REQ;
            capReqReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(capReqReg,
                                                                  linkID);
            capReqMsk = HYPHY20G_SERDES_PIF_CDRU_BIT_CPT_INTERPOLATOR_STAT_REQ_CAPTURE_REQ_MSK;

            accumRef0Reg = HYPHY20G_SERDES_PIF_C8_REG_CFG_12;
            accumRef0Reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                                         accumRef0Reg, linkID);
            accumRef0Msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_12_ACCUM_REF0_MSK;

            occAdcRefReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_9, linkID);
            occAdcRefMsk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_9_OCC_ADC_REFERENCE_MSK;

            numDataBusReg = 6;          /* number of registers for DATA_BUS */
            dataBusWidth = 5;           /* number of bits per sample */
            numSamples = 33;            /* number of samples */
            numSamplesRemain = 3;       /* number of samples in last register */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }


    /**
     ** hardcoded mode selection
     **/
    if (mode == 0) {
        DBG_PRINT("Hardcoded mode selection\n");

        /* configure ACCUM_REF_SEL */
        result = hyPhy20gRegFieldWrite(fileHndl, accumRefSelReg,
                                       accumRefSelMsk, accumRefSel);
        if (result) return result;

        /* configure NON_LINEAR_MAP */
        result = hyPhy20gRegFieldWrite(fileHndl, nonLinMapReg, nonLinMapMsk,
                                       nonLinMap);
        if (result) return result;

        /* configure default value of ACCUM_REF0 */
        result = hyPhy20gRegFieldWrite(fileHndl, accumRef0Reg, accumRef0Msk,
                                       defaultAccumRef0);
        if (result) return result;

        /* update device to use new threshold map */
        result = utilHyPhy20gWriteXfiBpPifAdcMemory(fileHndl, intf, linkID);
        if (result) return result;

        /* save current mode selection in SW context */
        result = utilHyPhy20gSerdesSaveModeSel(fileHndl, intf, linkID);
        if (result) return result;

        return HYPHY20G_SUCCESS;
    }


    /* for other modes, configure ACCUM_REF_SEL to 0 */
    result = hyPhy20gRegFieldWrite(fileHndl, accumRefSelReg,
                                   accumRefSelMsk, 0);
    if (result) return result;


    /**
     ** factory mode selection
     **/
     if (mode == 2) {
        DBG_PRINT("Factory mode selection\n");

        /* check SW context for valid mode selection values */
        result = serdesHyPhy20gSerdesGetModeSelCtxt(fileHndl, intf, linkID,
                                                    &ctxtValid, &nonLinMap,
                                                    &accumRef0);
        if (result) return result;

        if (ctxtValid != HYPHY20G_VALID_VAL_32BIT) {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL;
        }

        /* configure ACCUM_REF0 */
        result = hyPhy20gRegFieldWrite(fileHndl, accumRef0Reg, accumRef0Msk,
                                       accumRef0);
        if (result) return result;

        /* configure NON_LINEAR_MAP */
        result = hyPhy20gRegFieldWrite(fileHndl, nonLinMapReg, nonLinMapMsk,
                                       nonLinMap);
        if (result) return result;

        /* update device to use new threshold map */
        result = utilHyPhy20gWriteXfiBpPifAdcMemory(fileHndl, intf, linkID);
        if (result) return result;

    	return HYPHY20G_SUCCESS;
    }


    /**
     ** dynamic mode selection
     **/
    DBG_PRINT("Dynamic mode selection\n");

    /* check SW context for valid offsets -> use calOk */
    result = serdesHyPhy20gSerdesGetAdcCalCtxt(fileHndl, intf, linkID,
                                               &ctxtValid, &dummy, &dummy,
                                               &ctxtCalOk, &dummy, &dummy);
    if (result) return result;

    if (ctxtValid != HYPHY20G_VALID_VAL_32BIT || ctxtCalOk != 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS;
    }



    dataBusMsk = 0;
    for (i = 0; i < dataBusWidth; i++) {
        dataBusMsk |= (0x1 << i);
    }
    numSamplesPerReg = 32/dataBusWidth;

    while (cal == 1) {

        /* configure threshold map */
        result = utilHyPhy20gSetAdcThreshold(fileHndl, intf, linkID, mapType,
                                             scale);
        if (result) return result;

        /* update device to use new threshold map */
        result = utilHyPhy20gWriteXfiBpPifAdcMemory(fileHndl, intf, linkID);
        if (result) return result;

        if (mapType == HYPHY20G_SERDES_LINEAR_MAP) {
            /* set variables for linear mapping */
            maxScale = HYPHY20G_SERDES_LINEAR_MAX_SCALE;
            posbins = HYPHY20G_SERDES_LINEAR_POS_BINS;
            negbins = HYPHY20G_SERDES_LINEAR_NEG_BINS;
        } else {
            /* set variables non-linear mapping */
            maxScale = HYPHY20G_SERDES_NONLINEAR_MAX_SCALE;
            posbins = HYPHY20G_SERDES_NONLINEAR_POS_BINS;
            negbins = HYPHY20G_SERDES_NONLINEAR_NEG_BINS;
        }
        negEdge = 0;
        posEdge = 0;
        negMiddle = 0;
        posMiddle = 0;

        /* loop through captures */
        for (i = 0; i < iterations; i++) {
            /* read all registers containing DATA_BUS samples, extract each
               sample and place into bins */

            /* trigger capture into registers */
            result = hyPhy20gRegFieldWrite(fileHndl, capReqReg, capReqMsk, 1);
            if (result) return result;

            result = sysHyPhy20gUsDelay(1);
            if (result) return result;

            for (j = 0; j < numDataBusReg; j++) {
                addr = dataBusReg + 0x4*j;
                result = sysHyPhy20gRead(fileHndl, addr, &val);
                if (result) return result;

                if ((numSamplesRemain == 0) || (j < numDataBusReg-1)) {
                    maxNum = numSamplesPerReg;
                } else {
                    maxNum = numSamplesRemain;
                }

                for (k = 0; k < maxNum; k++) {
                    sample = (val >> (k*dataBusWidth)) & dataBusMsk;

                    /* sort samples into bins */
                    if (sample == negbins) {
                        negEdge++;
                    } else if (sample == posbins) {
                        posEdge++;
                    } else if (sample < 0xF) {
                        posMiddle++;
                    } else {
                        negMiddle++;
                    }
                }
            }
        }
        DBG_PRINT("negEdge = %lu, posEdge = %lu, negMiddle = %lu, posMiddle = %lu\n", negEdge, posEdge, negMiddle, posMiddle);

        if ((posEdge + posMiddle) != 0 && (negEdge + negMiddle) != 0) {
            /* calculate ratio, multiply by 1000 to increase resolution */
            posRatio = (1000*100*posEdge) / (posEdge + posMiddle);
            negRatio = (1000*100*negEdge) / (negEdge + negMiddle);

            /* find polarity */
            ratio = (posRatio > negRatio) ? posRatio : negRatio;
            DBG_PRINT("posRatio = %lu, negRatio = %lu\n", posRatio, negRatio);

            /* compare with goal */
            newDist = (goalUps > ratio) ? (goalUps - ratio) : (ratio - goalUps);
            DBG_PRINT("mapType = %lu, scale = %lu, ratio = %lu, newDist = %lu\n", mapType, scale, ratio, newDist);

            /* determine which mapping is closest to goal */
            if (mapType == HYPHY20G_SERDES_LINEAR_MAP) {
                /* linear mapping */
                if (newDist < linDistUps) {
                    DBG_PRINT("previous: linDistUps = %lu, linScale = %lu\n", linDistUps, linScale);
                    linDistUps = newDist;
                    linScale = scale;
                    DBG_PRINT("new: linDistUps = %lu, linScale = %lu\n", linDistUps, linScale);
                }
            } else {
                /* non-linear mapping */
                if (newDist < nonLinDistUps) {
                    DBG_PRINT("previous: nonLinDistUps = %lu, nonLinScale = %lu\n", nonLinDistUps, nonLinScale);
                    nonLinDistUps = newDist;
                    nonLinScale = scale;
                    DBG_PRINT("new: nonLinDistUps = %lu, nonLinScale = %lu\n", nonLinDistUps, nonLinScale);
                }
            }
        } else {
            DBG_PRINT("skipping ratio check\n");
        }

        /* switch to non-linear map or end calibration */
        if (scale == maxScale) {
            if (mapType == HYPHY20G_SERDES_LINEAR_MAP) {
                scale = 0;
                mapType = HYPHY20G_SERDES_NONLINEAR_MAP;
            } else {
                cal = 0;
            }
        }

        scale++;
    }

    /* use the mapping that is closest to goal */
    if ((linDistUps < nonLinDistUps) && (intf != HYPHY20G_SERDES_XFI_INTF)) {
        mapType = HYPHY20G_SERDES_LINEAR_MAP;
        scale = linScale;
        DBG_PRINT("chosen linear map, scale = %ld\n", scale);
    } else {
        mapType = HYPHY20G_SERDES_NONLINEAR_MAP;
        scale = nonLinScale;
        DBG_PRINT("chosen non-linear map, scale = %ld\n", scale);
    }

    /* update threshold values */
    result = utilHyPhy20gSetAdcThreshold(fileHndl, intf, linkID, mapType,
                                         scale);
    if (result) return result;

    /* update device to use new threshold map */
    result = utilHyPhy20gWriteXfiBpPifAdcMemory(fileHndl, intf, linkID);
    if (result) return result;

    /* save current mode selection in SW context */
    result = utilHyPhy20gSerdesSaveModeSel(fileHndl, intf, linkID);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gAdcModeSelect */

/*******************************************************************************
**
**  utilHyPhy20gHexStrToByte
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Converts char hex sequence into a UINT4 byte value
**
**  INPUTS:         *pStr   - string to convert
**                  numChar - number of characters in pStr (excluding null
**                            character)
**                  *data   - pointer to storage for conversion result
**
**  OUTPUTS:        None    - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_MODE_SEL
**                  HYPHY20G_ERR_SERDES_ADC_INVALID_OFFSETS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
static INT4 utilHyPhy20gHexStrToByte(char *pStr, UINT4 numChar, UINT4 *data)
{
    UINT4 shift;
    UINT4 i;

    *data = 0;

    /* argument checking */
    if (numChar > 8 || pStr[numChar] != '\0') {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    for (i = 0; i < numChar; i++) {
        shift = (numChar - i - 1) * 4;

        if (( 'a' <= pStr[i] ) && ( pStr[i] <= 'f' )) {
          /* convert hex letter */
          *data |= (pStr[i] - 'a' + 0xa) << shift;
       } else if (( 'A' <= pStr[i] ) && ( pStr[i] <= 'F' )) {
          /* convert hex letter */
          *data |= (pStr[i] - 'A' + 0xa) << shift;
       } else if (('0' <= pStr[i]) && (pStr[i] <= '9' )) {
          /* convert number */
          *data |= (pStr[i] - '0' ) << shift;
       } else {
          /* invalid char */
          DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_VAR_OVERFLOW\n",__FILE__, __FUNCTION__, __LINE__);
          return HYPHY20G_ERR_VAR_OVERFLOW;
       }
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gHexStrToByte */

/*******************************************************************************
**
**  utilHyPhy20gSerdesLnSync
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This util performs a lane synchronization.
**
**
**  INPUTS:         fileHndl  - device handle
**                  serdesGrp -
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesLnSync(struct file *fileHndl, UINT4 serdesGrp)
{
    /* local variables */
    INT4 result;
    UINT4 addr;
    UINT4 bitoffset;
    UINT4 linkId;
    UINT4 slice;

    /* argument checking */
    if (serdesGrp > HYPHY20G_SERDES_GRP_MAX_NUM) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (serdesGrp != HYPHY20G_SERDES_GRP_PIF) {
        return HYPHY20G_ERR_INVALID_ARG;
		}

    /* initialize local variables */
    addr = 0;
    bitoffset = 0;
    linkId = 0;
    slice = 0;

    /*******************/
    /* PIF Lane Sync   */
    /*******************/
    if (serdesGrp == HYPHY20G_SERDES_GRP_PIF) {
        /* turn off all piso fifos for all lanes  */
        for (linkId = 0; linkId <= HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM; linkId++) {
						/* disable PISO */
        		addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkId);
        		bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_RSTB_OFF;
        		result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        		if (result) return result;
        }

				result = sysHyPhy20gUsDelay (1);
        if (result) return result;

        /* reset CSU clock to PISO */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_1;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_1_RST_SYNC_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        if (result) return result;

				result = sysHyPhy20gUsDelay (1);
        if (result) return result;

        /* XAUI specific */
        for (slice = 0; slice < 2; slice++) {
            addr = HYPHY20G_XAUI_MTSB_SLICE_REG_OFFSET(HYPHY20G_XAUI_TOP_REG_SW_RST,
                                                       slice);
            result = hyPhy20gRegFieldWrite(fileHndl, addr,
                                  HYPHY20G_XAUI_TOP_BIT_SW_RST_XTEF_RST_MSK, 1);
            if(result) return result;
            result = hyPhy20gRegFieldWrite(fileHndl, addr,
                                  HYPHY20G_XAUI_TOP_BIT_SW_RST_XTEF_RST_MSK, 0);
            if(result) return result;
        }/* XAUI specific */

        /* turn on all piso fifos for all lanes  */
        for (linkId = 0; linkId <= HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM ; linkId++) {
						/* enable PISO */
        		addr = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkId);
        		bitoffset = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_RSTB_OFF;
        		result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
        		if (result) return result;
        }

				result = sysHyPhy20gUsDelay (1);
        if (result) return result;

        /* start CSU clock to PISO */
        addr = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_1;
        bitoffset = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_1_RST_SYNC_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 0);
        if (result) return result;

				return HYPHY20G_SUCCESS;
    }
    /* should never execute this */
    return HYPHY20G_ERR_INVALID_ARG;
} /* utilHyPhy20gSerdesLnSync */

/*******************************************************************************
**
**  utilHyPhy20gSerdesSaveModeSel
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Saves SERDES mode select register settings into SW
**                  context.
**
**
**  INPUTS:         fileHndl        - device handle
**                  intf            - SERDES interface
**                                        0 - Reserved
**                                        1 - XFI
**                                        2 - Backplane
**                                        3 - PIF
**                  linkID          - SERDES link
**                                        XFI       - 0 to 1
**                                        Backplane - 0 to 31
**                                        PIF       - 0 to 7
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesSaveModeSel(struct file *fileHndl, UINT4 intf,
                                   UINT4 linkID)
{
    UINT4 dummy = 0;
    UINT4 nonLinMapReg, nonLinMapMsk, nonLinMapVal = 0;
    UINT4 accumRef0Reg, accumRef0Msk, accumRef0Val = 0;
    INT4 result;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf < 0) || (intf > 3) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* generate pointer to interface level context information for this link */
    switch (intf) {
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            nonLinMapReg = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_1;
            nonLinMapReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                                         nonLinMapReg, linkID);
            nonLinMapMsk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_1_NON_LINEAR_MAP_MSK;

            accumRef0Reg = HYPHY20G_SERDES_XFI_CORE_REG_CFG_21;
            accumRef0Reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                                         accumRef0Reg, linkID);
            accumRef0Msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_21_ACCUM_REF0_MSK;
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            nonLinMapReg = HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_1;
            nonLinMapReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                                         nonLinMapReg, linkID);
            nonLinMapMsk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_1_NON_LINEAR_MAP_MSK;

            accumRef0Reg = HYPHY20G_SERDES_BP_C8_REG_CFG_12;
            accumRef0Reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                                         accumRef0Reg, linkID);
            accumRef0Msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_12_ACCUM_REF0_MSK;
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            nonLinMapReg = HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_1;
            nonLinMapReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                                         nonLinMapReg, linkID);
            nonLinMapMsk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_1_NON_LINEAR_MAP_MSK;

            accumRef0Reg = HYPHY20G_SERDES_PIF_C8_REG_CFG_12;
            accumRef0Reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                                         accumRef0Reg, linkID);
            accumRef0Msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_12_ACCUM_REF0_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* read NON_LINEAR_MAP value from register */
    result = hyPhy20gRegFieldRead(fileHndl, nonLinMapReg, nonLinMapMsk,
                                  &nonLinMapVal);
    if (result) return result;

    /* read ACCUM_REF0 value from register */
    result = hyPhy20gRegFieldRead(fileHndl, accumRef0Reg, accumRef0Msk,
                                  &accumRef0Val);
    if (result) return result;

    /* store NON_LINEAR_MAP and ACCUM_REF0 in SW context */
    result = serdesHyPhy20gSerdesSetMskModeSelCtxt(fileHndl, intf, linkID,
                                                   dummy, nonLinMapVal,
                                                   accumRef0Val, 0x6);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesSaveModeSel */

/*******************************************************************************
**
**  utilHyPhy20gSerdesSaveEq
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Saves SERDES equalization register settings into SW
**                  context.
**
**
**  INPUTS:         fileHndl        - device handle
**                  intf            - SERDES interface
**                                        0 - Reserved
**                                        1 - XFI
**                                        2 - Backplane
**                                        3 - PIF
**                  linkID          - SERDES link
**                                        XFI       - 0 to 1
**                                        Backplane - 0 to 31
**                                        PIF       - 0 to 7
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesSaveEq(struct file *fileHndl, UINT4 intf, UINT4 linkID)
{
    UINT4 value = 0;
    UINT4 dummy = 0;
    UINT4 eqTapReg, eqTap1Msk, eqTap2Msk, eqTap3Msk = 0;
    UINT4 eqTap1Val, eqTap2Val, eqTap3Val = 0;
    UINT4 timingEqTapReg, timingEqTapMsk, timingEqTapVal = 0;
    INT4 result;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf < 0) || (intf > 3) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* generate pointer to interface level context information for this link */
    switch (intf) {
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            eqTapReg = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_4;
            eqTapReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(eqTapReg,
                                                                  linkID);
            eqTap1Msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_EQ_TAP_1_MSK;
            eqTap2Msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_EQ_TAP_2_MSK;
            eqTap3Msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_EQ_TAP_3_MSK;

            timingEqTapReg = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_2;
            timingEqTapReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                                       timingEqTapReg, linkID);
            timingEqTapMsk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_2_TIMING_EQ_TAP_MSK;
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            eqTapReg = HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_4;
            eqTapReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(eqTapReg,
                                                                 linkID);
            eqTap1Msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_1_MSK;
            eqTap2Msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_2_MSK;
            eqTap3Msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_3_MSK;

            timingEqTapReg = HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_2;
            timingEqTapReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                                       timingEqTapReg, linkID);
            timingEqTapMsk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_2_TIMING_EQ_TAP_MSK;
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            eqTapReg = HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_4;
            eqTapReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(eqTapReg,
                                                                  linkID);
            eqTap1Msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_1_MSK;
            eqTap2Msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_2_MSK;
            eqTap3Msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_3_MSK;

            timingEqTapReg = HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_2;
            timingEqTapReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                                       timingEqTapReg, linkID);
            timingEqTapMsk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_2_TIMING_EQ_TAP_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* read EQ_TAP_1, EQ_TAP_2 and EQ_TAP_3 values from register */
    result = sysHyPhy20gRead(fileHndl, eqTapReg, &value);
    if (result) return result;

    result = hyPhy20gVarFieldRead(value, eqTap1Msk, &eqTap1Val);
    if (result) return result;

    result = hyPhy20gVarFieldRead(value, eqTap2Msk, &eqTap2Val);
    if (result) return result;

    result = hyPhy20gVarFieldRead(value, eqTap3Msk, &eqTap3Val);
    if (result) return result;

    /* read TIMING_EQ_TAP values from register */
    result = hyPhy20gRegFieldRead(fileHndl, timingEqTapReg, timingEqTapMsk,
                                  &timingEqTapVal);
    if (result) return result;

    /* store values in SW context */
    result = serdesHyPhy20gSerdesSetMskEqCtxt(fileHndl, intf, linkID, dummy,
                                              eqTap1Val, eqTap2Val, eqTap3Val,
                                              timingEqTapVal, 0x1E);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesSaveEq */

/*******************************************************************************
**
**  utilHyPhy20gSerdesEqCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Procedure will find configure the equalization settings
**                  based on selected mode.  This function is only valid for
**                  XFI, Backplane and PIF interfaces.  Not available for SFP
**                  because equalization is not applicable.
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkID   - SERDES link
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**                  mode     - 0 - hardcoded equalization
**                             1 - perform FW equalization
**                             2 - use equalization values from SW context
**                             3 - defer equalization (use
**                                 hyPhy20gSerdesADCCalibrate later)
**
**  OUTPUTS:        None     - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesEqCfg(struct file *fileHndl, UINT4 intf, UINT4 linkID,
                             UINT4 mode)
{
    UINT4 eqTapReg, eqTap1Msk, eqTap2Msk, eqTap3Msk = 0;
    UINT4 eqTap1, eqTap2, eqTap3 = 0;
    UINT4 timingEqTapReg, timingEqTapMsk, timingEqTap = 0;
    UINT4 fwIntf, fwIter;
    UINT4 ctxtValid;
    INT4 result;

//    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if ((intf < 0) || (intf > 3) || (intf == 1 && linkID > 1) ||
        (intf == 2 && linkID > 31) || (intf == 3 && linkID > 7) ||
        (mode > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (intf) {
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            eqTapReg = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_4;
            eqTapReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(eqTapReg,
                                                                  linkID);
            eqTap1Msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_EQ_TAP_1_MSK;
            eqTap2Msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_EQ_TAP_2_MSK;
            eqTap3Msk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_4_EQ_TAP_3_MSK;

            timingEqTapReg = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_REG_CFG_2;
            timingEqTapReg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                                                       timingEqTapReg, linkID);
            timingEqTapMsk = HYPHY20G_SERDES_XFI_CENTAUR16_CDRU_BIT_CFG_2_TIMING_EQ_TAP_MSK;

            timingEqTap = 0x01;
            eqTap1 = 0x01;
            eqTap2 = 0x01;
            eqTap3 = 0x01;

            fwIntf = 0;
            fwIter = 1000;
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            eqTapReg = HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_4;
            eqTapReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(eqTapReg,
                                                                 linkID);
            eqTap1Msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_1_MSK;
            eqTap2Msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_2_MSK;
            eqTap3Msk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_3_MSK;

            timingEqTapReg = HYPHY20G_SERDES_BP_CDRU_REG_CENTAUR8_CFG_2;
            timingEqTapReg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                                       timingEqTapReg, linkID);
            timingEqTapMsk = HYPHY20G_SERDES_BP_CDRU_BIT_CENTAUR8_CFG_2_TIMING_EQ_TAP_MSK;

            timingEqTap = 0x07;
            eqTap1 = 0x07;
            eqTap2 = 0x07;
            eqTap3 = 0x07;

            fwIntf = 3;
            fwIter = 1000;
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            eqTapReg = HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_4;
            eqTapReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(eqTapReg,
                                                                  linkID);
            eqTap1Msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_1_MSK;
            eqTap2Msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_2_MSK;
            eqTap3Msk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_4_EQ_TAP_3_MSK;

            timingEqTapReg = HYPHY20G_SERDES_PIF_CDRU_REG_CENTAUR8_CFG_2;
            timingEqTapReg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                                                       timingEqTapReg, linkID);
            timingEqTapMsk = HYPHY20G_SERDES_PIF_CDRU_BIT_CENTAUR8_CFG_2_TIMING_EQ_TAP_MSK;

            timingEqTap = 0x02;
            eqTap1 = 0x02;
            eqTap2 = 0x02;
            eqTap3 = 0x02;

            fwIntf = 1;
            fwIter = 1000;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }


    /**
     ** hardcoded equalization
     **/
    if (mode == 0) {
        DBG_PRINT("Hardcoded equalization\n");

        /* program equalizer tap */
        result = hyPhy20gRegFieldWrite(fileHndl, timingEqTapReg, timingEqTapMsk,
                                       timingEqTap);
        if (result) return result;

        /* program equalizer tap 3 */
        result = hyPhy20gRegFieldWrite(fileHndl, eqTapReg, eqTap1Msk, eqTap1);
        if (result) return result;

        /* program equalizer tap 2 (hardcoded now, should be set in fw ) */
        result = hyPhy20gRegFieldWrite(fileHndl, eqTapReg, eqTap2Msk, eqTap2);
        if (result) return result;

        /* program equalizer tap 1 (hardcoded now, should be set in fw ) */
        result = hyPhy20gRegFieldWrite(fileHndl, eqTapReg, eqTap3Msk, eqTap3);
        if (result) return result;
    }


    /**
     ** FW equalization
     **/
    if (mode == 1) {
        DBG_PRINT("FW equalization\n");

        result = hyPhy20gHostMsgCmdCfgSerdesSetEquTap(fileHndl, fwIntf, linkID,
                                                      fwIter);
        if (result) return result;
    }


    /**
     ** Factory equalization
     **/
    if (mode == 2) {
        DBG_PRINT("Factory equalization\n");

        /* retrieve SW context equalization values */
        result = serdesHyPhy20gSerdesGetEqCtxt(fileHndl, intf, linkID,
                                               &ctxtValid, &eqTap1, &eqTap2,
                                               &eqTap3, &timingEqTap);
        if (result) return result;

        /* program equalizer tap */
        result = hyPhy20gRegFieldWrite(fileHndl, timingEqTapReg, timingEqTapMsk,
                                       timingEqTap);
        if (result) return result;

        /* program equalizer tap 3 */
        result = hyPhy20gRegFieldWrite(fileHndl, eqTapReg, eqTap1Msk, eqTap1);
        if (result) return result;

        /* program equalizer tap 2 (hardcoded now, should be set in fw ) */
        result = hyPhy20gRegFieldWrite(fileHndl, eqTapReg, eqTap2Msk, eqTap2);
        if (result) return result;

        /* program equalizer tap 1 (hardcoded now, should be set in fw ) */
        result = hyPhy20gRegFieldWrite(fileHndl, eqTapReg, eqTap3Msk, eqTap3);
        if (result) return result;
    }

    /**
     ** Deferred mode - do nothing
     **/
    DBG_PRINT("Deferred equalization\n");

    /* save equalization settings in SW context */
    result = utilHyPhy20gSerdesSaveEq(fileHndl, intf, linkID);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesEqCfg */

/*******************************************************************************
**
**  utilHyPhy20gSerdesInitLwPwr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function is to be called from *SerdesInit APIs to
**                  configure the link for lowest power settings.
**
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                                 0 - SFP
**                                 1 - XFI
**                                 2 - Backplane
**                                 3 - PIF
**                  linkId   - SERDES link
**                                 SFP       - 0 to 15
**                                 XFI       - 0 to 1
**                                 Backplane - 0 to 31
**                                 PIF       - 0 to 7
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesInitLwPwr(struct file *fileHndl, UINT4 intf,
                                 UINT4 linkId)
{
    UINT4 piso_enb_reg, piso_enb_off = 0;
    UINT4 piso_rstb_reg, piso_rstb_off = 0;
    UINT4 adc_rstb_reg, adc_rstb_off = 0;
    UINT4 adc_dac_enb_reg, adc_dac_enb_off = 0;
    UINT4 adc_dac_rstb_reg, adc_dac_rstb_off = 0;
    UINT4 adc_enb_reg, adc_enb_off = 0;
    UINT4 rxlos_enb_reg, rxlos_enb_off = 0;
    UINT4 rxcpgen_enb_reg, rxcpgen_enb_off = 0;
    UINT4 rxcpgen_rstb_reg, rxcpgen_rstb_off = 0;
    UINT4 tx_dpeni1_reg, tx_dpeni1_msk = 0;
    UINT4 tx_den_core_reg, tx_den_core_msk = 0;
    UINT4 tx_sw_reg, tx_sw_msk = 0;
    UINT4 badj_enb_reg, badj_enb_off = 0;
    UINT4 csu_enb_reg, csu_enb_off = 0;
    UINT4 adsm_rstb_reg, adsm_rstb_off = 0;

    UINT4 csu_id = 0;
    UINT4 all_pt_chk = 0;
    UINT4 bit_val = 0;

    INT4 x;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((intf > 3) || (intf == 0 && linkId > 15) || (intf == 1 && linkId > 1) ||
        (intf == 2 && linkId > 31) || (intf == 3 && linkId > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
        
    switch (intf) {
        case HYPHY20G_SERDES_SFP_INTF:
            /* SFP */
            piso_enb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                           HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkId);
            piso_enb_off = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_ENB_OFF;

            piso_rstb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkId);
            piso_rstb_off = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_PISO_RSTB_OFF;

            adc_rstb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                           HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkId);
            adc_rstb_off = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_RSTB_OFF;

            adc_dac_enb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                              HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkId);
            adc_dac_enb_off = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_ENB_OFF;

            adc_dac_rstb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                               HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkId);
            adc_dac_rstb_off = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_DAC_RSTB_OFF;

            adc_enb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                          HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkId);
            adc_enb_off = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_ADC_ENB_OFF;

            rxlos_enb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_SFP_CORE_REG_CONF_2, linkId);
            rxlos_enb_off = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_2_RXLOS_ENB_OFF;

            rxcpgen_enb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                              HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkId);
            rxcpgen_enb_off = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_ENB_OFF;

            rxcpgen_rstb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                               HYPHY20G_SERDES_SFP_CORE_REG_CONF_3, linkId);
            rxcpgen_rstb_off = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_3_RXCPGEN_RSTB_OFF;

            tx_dpeni1_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_SFP_CORE_REG_CONF_5, linkId);
            tx_dpeni1_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_5_TX_DPENI1_MSK;

            tx_den_core_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                              HYPHY20G_SERDES_SFP_CORE_REG_CONF_4, linkId);
            tx_den_core_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_4_TX_DEN_CORE_MSK;

            tx_sw_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                        HYPHY20G_SERDES_SFP_CORE_REG_CONF_4, linkId);
            tx_sw_msk = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_4_TX_SWING_CTRL_CORE_MSK;

            badj_enb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                           HYPHY20G_SERDES_SFP_CORE_REG_CONF_1, linkId);
            badj_enb_off = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_1_BADJ_ENB_OFF;

            csu_enb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                          HYPHY20G_SERDES_SFP_CORE_REG_CONF_6, linkId);
            csu_enb_off = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_6_CSU_ENB_OFF;

            adsm_rstb_reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_SFP_CORE_REG_CONF_8, linkId);
            adsm_rstb_off = HYPHY20G_SERDES_SFP_CORE_BIT_CONF_8_ADSM_RSTB_OFF;
            break;
        case HYPHY20G_SERDES_XFI_INTF:
            /* XFI */
            piso_enb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                           HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkId);
            piso_enb_off = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_ENB_OFF;

            piso_rstb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkId);
            piso_rstb_off = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_PISO_RSTB_OFF;

            adc_rstb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                           HYPHY20G_SERDES_XFI_CORE_REG_CFG_3, linkId);
            adc_rstb_off = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_3_ADC_RSTB_OFF;

            adc_dac_enb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                              HYPHY20G_SERDES_XFI_CORE_REG_CFG_3, linkId);
            adc_dac_enb_off = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_3_ADC_DAC_ENB_OFF;

            adc_dac_rstb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                               HYPHY20G_SERDES_XFI_CORE_REG_CFG_3, linkId);
            adc_dac_rstb_off = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_3_ADC_DAC_RSTB_OFF;

            adc_enb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                          HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkId);
            adc_enb_off = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_ADC_ENB_OFF;

            rxlos_enb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_XFI_CORE_REG_CFG_3, linkId);
            rxlos_enb_off = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_3_RXLOS_ENB_OFF;

            rxcpgen_enb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                              HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkId);
            rxcpgen_enb_off = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_ENB_OFF;

            rxcpgen_rstb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                               HYPHY20G_SERDES_XFI_CORE_REG_CFG_4, linkId);
            rxcpgen_rstb_off = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_4_RXCPGEN_RSTB_OFF;

            tx_dpeni1_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_XFI_CORE_REG_CFG_6, linkId);
            tx_dpeni1_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_6_TX_DPENI1_MSK;

            tx_den_core_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                              HYPHY20G_SERDES_XFI_CORE_REG_CFG_5, linkId);
            tx_den_core_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_5_TX_DEN_CORE_MSK;

            tx_sw_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                        HYPHY20G_SERDES_XFI_CORE_REG_CFG_5, linkId);
            tx_sw_msk = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_5_TX_SWING_CTRL_CORE_MSK;

            badj_enb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                           HYPHY20G_SERDES_XFI_CORE_REG_CFG_2, linkId);
            badj_enb_off = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_2_BADJ_ENB_OFF;

            csu_enb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                          HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkId);
            csu_enb_off = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_CSU_ENB_OFF;

            adsm_rstb_reg = HYPHY20G_SERDES_XFI_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_XFI_CORE_REG_CFG_10, linkId);
            adsm_rstb_off = HYPHY20G_SERDES_XFI_CORE_BIT_CFG_10_ADSM_RSTB_OFF;
            break;
        case HYPHY20G_SERDES_BACKPLANE_INTF:
            /* Backplane */
            /* links 0 to 7 and 16 to 23 corresponds to csu0 */
            /* links 8 to 15 and 24 to 31 corresponds to csu1 */
            if (((linkId >= 0) && (linkId <= 7)) || ((linkId >= 16) && (linkId <= 23))) {
                csu_id = 0;
            } else {
                if (((linkId >= 8) && (linkId <= 15)) || ((linkId >= 24) && (linkId <= 31))) {
                    csu_id = 1;
                } else {
                    return HYPHY20G_ERR_INVALID_ARG;
                }
            }
            piso_enb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                           HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkId);
            piso_enb_off = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_ENB_OFF;

            piso_rstb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkId);
            piso_rstb_off = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_PISO_RSTB_OFF;

            adc_rstb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                           HYPHY20G_SERDES_BP_C8_REG_CFG_1, linkId);
            adc_rstb_off = HYPHY20G_SERDES_BP_C8_BIT_CFG_1_ADC_RSTB_OFF;

            adc_dac_enb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                              HYPHY20G_SERDES_BP_C8_REG_CFG_2, linkId);
            adc_dac_enb_off = HYPHY20G_SERDES_BP_C8_BIT_CFG_2_ADC_DAC_ENB_OFF;

            adc_dac_rstb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                               HYPHY20G_SERDES_BP_C8_REG_CFG_2, linkId);
            adc_dac_rstb_off = HYPHY20G_SERDES_BP_C8_BIT_CFG_2_ADC_DAC_RSTB_OFF;

            adc_enb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                          HYPHY20G_SERDES_BP_C8_REG_CFG_2, linkId);
            adc_enb_off = HYPHY20G_SERDES_BP_C8_BIT_CFG_2_ADC_ENB_OFF;

            rxlos_enb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_BP_C8_REG_CFG_2, linkId);
            rxlos_enb_off = HYPHY20G_SERDES_BP_C8_BIT_CFG_2_RXLOS_ENB_OFF;

            rxcpgen_enb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                              HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkId);
            rxcpgen_enb_off = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;

            rxcpgen_rstb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                               HYPHY20G_SERDES_BP_C8_REG_CFG_3, linkId);
            rxcpgen_rstb_off = HYPHY20G_SERDES_BP_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;

            tx_dpeni1_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_BP_C8_REG_CFG_5, linkId);
            tx_dpeni1_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_5_TX_DPENI1_MSK;

            tx_den_core_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                              HYPHY20G_SERDES_BP_C8_REG_CFG_6, linkId);
            tx_den_core_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_6_TX_DEN_CORE_MSK;

            tx_sw_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                        HYPHY20G_SERDES_BP_C8_REG_CFG_6, linkId);
            tx_sw_msk = HYPHY20G_SERDES_BP_C8_BIT_CFG_6_TX_SWING_CTRL_CORE_MSK;

            badj_enb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                           HYPHY20G_SERDES_BP_C8_REG_CFG_8, linkId);
            badj_enb_off = HYPHY20G_SERDES_BP_C8_BIT_CFG_8_BADJ_ENB_OFF;

            csu_enb_reg = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(
                          HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_2, csu_id);
            csu_enb_off = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_ENB_OFF;

            adsm_rstb_reg = HYPHY20G_SERDES_ESSI_CSU_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_REG_CFG_10, csu_id);
            adsm_rstb_off = HYPHY20G_SERDES_ESSI_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
            break;
        case HYPHY20G_SERDES_PIF_INTF:
            /* PIF */
            piso_enb_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                           HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkId);
            piso_enb_off = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_ENB_OFF;

            piso_rstb_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkId);
            piso_rstb_off = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_PISO_RSTB_OFF;

            adc_rstb_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                           HYPHY20G_SERDES_PIF_C8_REG_CFG_1, linkId);
            adc_rstb_off = HYPHY20G_SERDES_PIF_C8_BIT_CFG_1_ADC_RSTB_OFF;

            adc_dac_enb_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                              HYPHY20G_SERDES_PIF_C8_REG_CFG_2, linkId);
            adc_dac_enb_off = HYPHY20G_SERDES_PIF_C8_BIT_CFG_2_ADC_DAC_ENB_OFF;

            adc_dac_rstb_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                               HYPHY20G_SERDES_PIF_C8_REG_CFG_2, linkId);
            adc_dac_rstb_off = HYPHY20G_SERDES_PIF_C8_BIT_CFG_2_ADC_DAC_RSTB_OFF;

            adc_enb_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                          HYPHY20G_SERDES_PIF_C8_REG_CFG_2, linkId);
            adc_enb_off = HYPHY20G_SERDES_PIF_C8_BIT_CFG_2_ADC_ENB_OFF;

            rxlos_enb_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_PIF_C8_REG_CFG_2, linkId);
            rxlos_enb_off = HYPHY20G_SERDES_PIF_C8_BIT_CFG_2_RXLOS_ENB_OFF;

            rxcpgen_enb_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                              HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkId);
            rxcpgen_enb_off = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_ENB_OFF;

            rxcpgen_rstb_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                               HYPHY20G_SERDES_PIF_C8_REG_CFG_3, linkId);
            rxcpgen_rstb_off = HYPHY20G_SERDES_PIF_C8_BIT_CFG_3_RXCPGEN_RSTB_OFF;

            tx_dpeni1_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_PIF_C8_REG_CFG_5, linkId);
            tx_dpeni1_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_5_TX_DPENI1_MSK;

            tx_den_core_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                              HYPHY20G_SERDES_PIF_C8_REG_CFG_6, linkId);
            tx_den_core_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_6_TX_DEN_CORE_MSK;

            tx_sw_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                        HYPHY20G_SERDES_PIF_C8_REG_CFG_6, linkId);
            tx_sw_msk = HYPHY20G_SERDES_PIF_C8_BIT_CFG_6_TX_SWING_CTRL_CORE_MSK;

            badj_enb_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                           HYPHY20G_SERDES_PIF_C8_REG_CFG_8, linkId);
            badj_enb_off = HYPHY20G_SERDES_PIF_C8_BIT_CFG_8_BADJ_ENB_OFF;

            csu_enb_reg = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_2;
            csu_enb_off = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_2_CSU_ENB_OFF;

            adsm_rstb_reg = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_REG_CFG_10;
            adsm_rstb_off = HYPHY20G_SERDES_PIF_CSU_NGS_CSU_6G_BIT_CFG_10_ADSM_RSTB_OFF;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }
 
    /* disable piso */
    result = sysHyPhy20gBitWrite(fileHndl, piso_enb_reg, piso_enb_off, 1);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, piso_rstb_reg, piso_rstb_off, 0);
    if (result) return result;

    /* disable rx path */
    result = sysHyPhy20gBitWrite(fileHndl, adc_rstb_reg, adc_rstb_off, 0);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, adc_dac_enb_reg, adc_dac_enb_off, 1);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, adc_dac_rstb_reg, adc_dac_rstb_off, 0);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, adc_enb_reg, adc_enb_off, 1);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, rxlos_enb_reg, rxlos_enb_off, 1);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, rxcpgen_enb_reg, rxcpgen_enb_off, 1);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, rxcpgen_rstb_reg, rxcpgen_rstb_off, 0);
    if (result) return result;

    /* disable tx path */
    result = hyPhy20gRegFieldWrite(fileHndl, tx_dpeni1_reg, tx_dpeni1_msk, 0x0);
    if (result) return result;

    result = hyPhy20gRegFieldWrite(fileHndl, tx_den_core_reg, tx_den_core_msk, 0x0000);
    if (result) return result;

    result = hyPhy20gRegFieldWrite(fileHndl, tx_sw_reg, tx_sw_msk, 0x0000);
    if (result) return result;

    /* disable rclk */
    result = sysHyPhy20gBitWrite(fileHndl, badj_enb_reg, badj_enb_off, 1);
    if (result) return result;

    /* disable csu */
    if ((intf == HYPHY20G_SERDES_SFP_INTF) || (intf == HYPHY20G_SERDES_XFI_INTF))  {
        result = sysHyPhy20gBitWrite(fileHndl, csu_enb_reg, csu_enb_off, 1);
        if (result) return result;
        
        result = sysHyPhy20gBitWrite(fileHndl, adsm_rstb_reg, adsm_rstb_off, 0);
        if (result) return result;
    } else if (intf == HYPHY20G_SERDES_BACKPLANE_INTF) {
        /* need to check all links for the csu are in low power mode */
        all_pt_chk = 1;
        /* links 0 to 7 and 16 to 23 corresponds to csu0 */
        /* links 8 to 15 and 24 to 31 corresponds to csu1 */
        if (csu_id == 0) {
            for (x = 0; x <= 7; x++) {
                /* check PISO_ENB */
                piso_enb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                               HYPHY20G_SERDES_BP_C8_REG_CFG_1, x);                
                result = sysHyPhy20gBitRead(fileHndl, piso_enb_reg, piso_enb_off, &bit_val);
                if (result) return result;
                /* if any of the PISO_ENB are not set, csu can not be put into low power */
                if (bit_val == 0x0) {
                    all_pt_chk = 0;
                    break;
                }
            }
            if(all_pt_chk==1){
                for (x = 16; x <=23; x++) {
                    /* check PISO_ENB */
                    piso_enb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_BP_C8_REG_CFG_1, x);                 
                    result = sysHyPhy20gBitRead(fileHndl, piso_enb_reg, piso_enb_off, &bit_val);
                    if (result) return result;
                    /* if any of the PISO_ENB are not set, csu can not be put into low power */
                    if (bit_val == 0x0) {
                        all_pt_chk = 0;
                        break;
                    }
                }
            }
        } else if (csu_id == 1) {
            for (x = 8; x <= 15; x++) {
                /* check PISO_ENB */
                piso_enb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                               HYPHY20G_SERDES_BP_C8_REG_CFG_1, x);                 
                result = sysHyPhy20gBitRead(fileHndl, piso_enb_reg, piso_enb_off, &bit_val);
                if (result) return result;
                /* if any of the PISO_ENB are not set, csu can not be put into low power */
                if (bit_val == 0x0) {
                    all_pt_chk = 0;
                    break;
                }
            }
            if(all_pt_chk==1){
                for (x = 24; x <= 31; x++) {
                    /* check PISO_ENB */
                    piso_enb_reg = HYPHY20G_SERDES_BP_MTSB_SLICE_REG_OFFSET(
                                   HYPHY20G_SERDES_BP_C8_REG_CFG_1, x);                 
                    result = sysHyPhy20gBitRead(fileHndl, piso_enb_reg, piso_enb_off, &bit_val);
                    if (result) return result;
                    /* if any of the PISO_ENB are not set, csu can not be put into low power */
                    if (bit_val == 0x0) {
                        all_pt_chk = 0;
                        break;
                    }
                }
            }
        }
        if (all_pt_chk == 1) {
            result = sysHyPhy20gBitWrite(fileHndl, csu_enb_reg, csu_enb_off, 1);
            if (result) return result;
            
            result = sysHyPhy20gBitWrite(fileHndl, adsm_rstb_reg, adsm_rstb_off, 0);
            if (result) return result;            
        }    
    } else if (intf == HYPHY20G_SERDES_PIF_INTF) {
        /* need to check all links for the csu are in low power mode */
        all_pt_chk = 1;
        for (x = 0; x <= HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM; x++) {
            /* check PISO_ENB */
            piso_enb_reg = HYPHY20G_SERDES_PIF_MTSB_SLICE_REG_OFFSET(
                            HYPHY20G_SERDES_PIF_C8_REG_CFG_1, x);                
            result = sysHyPhy20gBitRead(fileHndl, piso_enb_reg, piso_enb_off, &bit_val);
            if (result) return result;
            /* if any of the PISO_ENB are not set, csu can not be put into low power */
            if (bit_val == 0x0) {
                all_pt_chk = 0;
                break;
            }
        }
        if (all_pt_chk == 1) {
            result = sysHyPhy20gBitWrite(fileHndl, csu_enb_reg, csu_enb_off, 1);
            if (result) return result;
            
            result = sysHyPhy20gBitWrite(fileHndl, adsm_rstb_reg, adsm_rstb_off, 0);
            if (result) return result;            
        }    
    } else {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gSerdesInitLwPwr */

/*******************************************************************************
**
**  utilHyPhy20gSerdesGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Get per-link context size.
**
**
**  INPUTS:         pSerdesCtxt - per-link context
**
**  OUTPUTS:        None
**
**  RETURNS:        The per-link context size
**
*******************************************************************************/
UINT4 utilHyPhy20gSerdesGetCtxtSize(sHYPHY20G_SERDES_CTXT *pSerdesCtxt)
{
    UINT4  size = 0;

    /* include ctxt header size */
    size += utilHyPhy20gGetCtxtHdrSize();

    if (pSerdesCtxt != NULL) {
        /* calculate the first level structure size of Serdes subsystem */
        size += sizeof(sHYPHY20G_SERDES_CTXT);

        /* calculate the second level context size */
        /* include ctxt header size */
        size += utilHyPhy20gGetCtxtHdrSize();
        size += pSerdesCtxt->adcCalCtxt.sizeOffset;
    }
    return size;
}/* utilHyPhy20gSerdesGetCtxtSize */

/*******************************************************************************
**
**  utilHyPhy20gSerdesExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the Sfp/Xfi/BackPlane/Pif SERDES context into
**                  pre-allocated buffer.
**
**
**  INPUTS:         pSerdesCtxt - per-link context
**                  pCtxtBuf - pre-allocated buffer for Sfp/Xfi/BackPlane/Pif 
**                  SERDES context extraction
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The extracted length
**
*******************************************************************************/
UINT4 utilHyPhy20gSerdesExtractCtxt(sHYPHY20G_SERDES_CTXT *pSerdesCtxt, 
                                    void *pCtxtBuf)
{
    UINT4 rtLen;
    UINT4 offset = 0, len = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), offset, 
        "%s", "pCtxtBuf is NULL.");
    
    offset += sizeof(sHYPHY20G_CTXT_HDR);
    
    if (pSerdesCtxt != NULL) {
        /* extract the first level context of Serdes subsystem */
        sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
                        (void *)(pSerdesCtxt), sizeof(sHYPHY20G_SERDES_CTXT));
        offset += sizeof(sHYPHY20G_SERDES_CTXT);

        /* extract the second level context */
        /* insert ctxt buffer header */
        len = sizeof(sHYPHY20G_CTXT_HDR) + pSerdesCtxt->adcCalCtxt.sizeOffset;
        rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                        HYPHY_SERDES_OFFSET_CTXT_ID, HYPHY_NO_INDEX);
        HYPHY20G_ASSERT((rtLen > 0), offset, 
            "%s", "pCtxtBuf is NULL.");  
        offset += sizeof(sHYPHY20G_CTXT_HDR);
        
        sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
                    (void *)(pSerdesCtxt->adcCalCtxt.pOffset), 
                    pSerdesCtxt->adcCalCtxt.sizeOffset);
        offset += pSerdesCtxt->adcCalCtxt.sizeOffset;
    }
    return offset;
}/* utilHyPhy20gSerdesExtractCtxt */

/*******************************************************************************
**
**  utilHyPhy20gSerdesRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the Sfp/Xfi/BackPlane/Pif Serdes context into
**                  driver memory.
**
**
**  INPUTS:         pSerdesCtxt - per-link context
**                  pCtxtBuf - pre-stored Sfp/Xfi/BackPlane/Pif Serdes context
**                  for restoration
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The parsed length
**
*******************************************************************************/
UINT4 utilHyPhy20gSerdesRestoreCtxt(sHYPHY20G_SERDES_CTXT *pSerdesCtxt, 
                                    void *pCtxtBuf)
{
    UINT4 offset = 0, len = 0;
    UINT4 *pOffset = NULL;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), offset, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pSerdesCtxt != NULL), offset, 
        "%s", "pSerdesCtxt is NULL.");

    /* backup the useful pOffset value */
    pOffset = pSerdesCtxt->adcCalCtxt.pOffset;

    /* restore the first level context for Serdes subsystem */
    /* offset pCtxtBuf to figure out the payload position */
    offset += sizeof(sHYPHY20G_CTXT_HDR);

    len = sizeof(sHYPHY20G_SERDES_CTXT);
    sysHyPhy20gMemCpy((void *)(pSerdesCtxt), 
					((UINT1 *)pCtxtBuf + offset), len);

    /* finish restore the first level context for Serdes subsystem */
    pSerdesCtxt->adcCalCtxt.pOffset = pOffset;
    offset += sizeof(sHYPHY20G_SERDES_CTXT);

    /* restore the second level context */
    /* offset pCtxtBuf to figure out the payload position */
    offset += sizeof(sHYPHY20G_CTXT_HDR);

    sysHyPhy20gMemCpy((void *)(pSerdesCtxt->adcCalCtxt.pOffset), 
					((UINT1 *)pCtxtBuf + offset), pSerdesCtxt->adcCalCtxt.sizeOffset);
    offset += pSerdesCtxt->adcCalCtxt.sizeOffset;

    return offset;
}/* utilHyPhy20gSerdesRestoreCtxt */

/*******************************************************************************
**
**  utilHyPhy20gSerdesLoadFactoryCalCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the Sfp/Xfi/BackPlane/Pif SERDES context per link
**                  into the driver context memory.
**
**
**  INPUTS:         fileHndl - device handle
**                  intf     - SERDES interface
**                              0 - SFP
**                              1 - XFI
**                              2 - Backplane
**                              3 - PIF
**                  linkId   - SERDES link
**                              SFP       - 0 to 15
**                              XFI       - 0 to 1
**                              Backplane - 0 to 31
**                              PIF       - 0 to 7
**                  pCtxtBuf - pre-stored Sfp/Xfi/BackPlane/Pif 
**                             SERDES context for restoration
**
**  OUTPUTS:        pParsedSize -the data size(in byte) has been parsed
**                  in the buffer for restoration
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_CTXT_CORRUPTED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSerdesLoadFactoryCalCtxt(struct file *fileHndl, UINT4 intf, 
                                              UINT4 linkId, void *pCtxtBuf, 
                                              UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    INT4 result;
    UINT4 index = 0, offset = 0;
    
    if (pCtxtBuf == NULL || 
        pParsedSize == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* argument check */
    if ((intf > HYPHY20G_SERDES_PIF_INTF) || 
        (intf == HYPHY20G_SERDES_SFP_INTF && 
        linkId > HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM) || 
        (intf == HYPHY20G_SERDES_XFI_INTF && 
        linkId > HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM) ||
        (intf == HYPHY20G_SERDES_BACKPLANE_INTF && 
        linkId > HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM) || 
        (intf == HYPHY20G_SERDES_PIF_INTF && 
        linkId > HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (intf == HYPHY20G_SERDES_SFP_INTF) {
        index = linkId;
    } else if (intf == HYPHY20G_SERDES_XFI_INTF) {
        index = linkId + HYPHY20G_SERDES_XFI_LNK_OFFSET;
    } else if (intf == HYPHY20G_SERDES_BACKPLANE_INTF) {
        index = linkId + HYPHY20G_SERDES_BACKPLANE_LNK_OFFSET;
    } else {
        index = linkId + HYPHY20G_SERDES_PIF_LNK_OFFSET;
    }

    /* try to retrieve the serdes[index] header in the pre-stored context buffer */
    pCtxtBufHdr = utilHyPhy20gSearchCtxtHdr(pCtxtBuf, 
                                HYPHY_SERDES_CTXT_ID, index);
    if (pCtxtBufHdr == NULL) {
        DBG_PRINT("%s, %s, %d, can not find HYPHY_SERDES_CTXT_ID hdr\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    pCtxtBuf = (void *)pCtxtBufHdr;

    if (pCtxtBufHdr->len == sizeof(sHYPHY20G_CTXT_HDR)) {
        offset += sizeof(sHYPHY20G_CTXT_HDR);
    } else if (pCtxtBufHdr->len > sizeof(sHYPHY20G_CTXT_HDR)) {
        /* create Serdes context */
        result = serdesHyPhy20gSerdesCreateCtxt(fileHndl, intf, linkId);
        if (result) {
            DBG_PRINT("%s, %s, %d, serdesHyPhy20gSerdesCreateCtxt failed\n", 
                        __FILE__, __FUNCTION__, __LINE__);
            *pParsedSize = 0;
            return result;
        }

        /* retrieve device context */
        pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
        /* confirm device context pointer is valid before processing */
        HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

        if (intf == HYPHY20G_SERDES_SFP_INTF) {
            pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkId];
        } else if (intf == HYPHY20G_SERDES_XFI_INTF) {
            pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkId];
        } else if (intf == HYPHY20G_SERDES_BACKPLANE_INTF) {
            pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkId];
        } else {
            pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkId];
        }
            
        offset += utilHyPhy20gSerdesRestoreCtxt(pSerdesCtxt, ((UINT1 *)pCtxtBuf + offset));

        /* configure SW context after successful load */
        result = serdesHyPhy20gSerdesSetMskAdcCalCtxt(fileHndl, intf, linkId, 0,
                                 HYPHY20G_SERDES_FACTORY_CAL_MODE, 0, 0, 0, 0, 0x2);
        if (result) {
            serdesHyPhy20gSerdesDestroyCtxt(fileHndl, intf, linkId);
            return result;
        }
    } else {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_CORRUPTED\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        *pParsedSize = 0;
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pParsedSize = offset;

    return HYPHY20G_SUCCESS;
}/* utilHyPhy20gSerdesLoadFactoryCalCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the size of Sfp/Xfi/BackPlane/Pif 
**                  SERDES context.
**
**
**  INPUTS:         fileHndl      - device handle
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The size of Sfp/Xfi/BackPlane/Pif 
**                  SERDES context.
**
*******************************************************************************/
UINT4 serdesHyPhy20gSerdesGetCtxtSize(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    UINT4 i = 0, linkId = 0, size = 0;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    DBG_PRINT("%s, %s, %d, serdes offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, size);

    /* serdes context size */
    for (i = 0; i < HYPHY20G_SERDES_LNK_MAX_NUM; i++) {
        if (i < HYPHY20G_SERDES_XFI_LNK_OFFSET) {
            /* SFP serdes link */
            linkId = i;
            pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkId];
        } else if (i < HYPHY20G_SERDES_BACKPLANE_LNK_OFFSET) {
            /* XFI serdes link */
            linkId = i - HYPHY20G_SERDES_XFI_LNK_OFFSET;
            pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkId];
        } else if (i < HYPHY20G_SERDES_PIF_LNK_OFFSET) {
            /* BackPlane serdes link */
            linkId = i - HYPHY20G_SERDES_BACKPLANE_LNK_OFFSET;
            pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkId];
        } else {
            /* PIF serdes link */
            linkId = i - HYPHY20G_SERDES_PIF_LNK_OFFSET;
            pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkId];
        }
        
        /* serdes pre-link context size */
        size += utilHyPhy20gSerdesGetCtxtSize(pSerdesCtxt);
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, size);

    return size;
}/* serdesHyPhy20gSerdesGetCtxtSize */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the Sfp/Xfi/BackPlane/Pif SERDES context into
**                  pre-allocated buffer.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-allocated buffer for Sfp/Xfi/BackPlane/Pif
**                  SERDES context extraction
**
**  OUTPUTS:        pExtractedSize -the context size(in byte) has been extracted
**                  into the buffer
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesExtractCtxt(struct file *fileHndl, 
                                     void *pCtxtBuf, UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    UINT4 rtLen;
    UINT4 i = 0, linkId = 0, offset = 0, len = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pExtractedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pExtractedSize is NULL.");

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");


    DBG_PRINT("%s, %s, %d, serdes offset 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    /* extract serdes context */
    for (i = 0; i < HYPHY20G_SERDES_LNK_MAX_NUM; i++) {
        if (i < HYPHY20G_SERDES_XFI_LNK_OFFSET) {
            /* SFP serdes link */
            linkId = i;
            pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkId];
        } else if (i < HYPHY20G_SERDES_BACKPLANE_LNK_OFFSET) {
            /* XFI serdes link */
            linkId = i - HYPHY20G_SERDES_XFI_LNK_OFFSET;
            pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkId];
        } else if (i < HYPHY20G_SERDES_PIF_LNK_OFFSET) {
            /* BackPlane serdes link */
            linkId = i - HYPHY20G_SERDES_BACKPLANE_LNK_OFFSET;
            pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkId];
        } else {
            /* PIF serdes link */
            linkId = i - HYPHY20G_SERDES_PIF_LNK_OFFSET;
            pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkId];
        }
        
        /* extract pre-link serdes context */
        /* insert ctxt buffer header */
        len = (pSerdesCtxt == NULL) ? sizeof(sHYPHY20G_CTXT_HDR) : 
                (sizeof(sHYPHY20G_CTXT_HDR) + sizeof(sHYPHY20G_SERDES_CTXT));
        rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                            HYPHY_SERDES_CTXT_ID, i);
        HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
            "%s", "pCtxtBuf is NULL.");  

        offset += utilHyPhy20gSerdesExtractCtxt(pSerdesCtxt, ((UINT1 *)pCtxtBuf + offset));
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pExtractedSize = offset;

    return HYPHY20G_SUCCESS;
}/* serdesHyPhy20gSerdesExtractCtxt */

/*******************************************************************************
**
**  serdesHyPhy20gSerdesRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the Sfp/Xfi/BackPlane/Pif SERDES context
**                  into the driver context memory.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-stored Sfp/Xfi/BackPlane/Pif 
**                  SERDES context for restoration
**
**  OUTPUTS:        pParsedSize -the data size(in byte) has been parsed
**                  in the buffer for restoration
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_CTXT_CORRUPTED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 serdesHyPhy20gSerdesRestoreCtxt(struct file *fileHndl, 
                                     void *pCtxtBuf, UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_SERDES_CTXT *pSerdesCtxt;
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    INT4 result;
    UINT4 i = 0, offset = 0, linkId = 0, intf = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pParsedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pParsedSize is NULL.");

    /* try to retrieve the sfpSerdes[0] header in the pre-stored context buffer */
    pCtxtBufHdr = utilHyPhy20gSearchCtxtHdr(pCtxtBuf, 
                                HYPHY_SERDES_CTXT_ID, 0);
    if (pCtxtBufHdr == NULL) {
        DBG_PRINT("%s, %s, %d, can not find HYPHY_SERDES_CTXT_ID hdr\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    pCtxtBuf = (void *)pCtxtBufHdr;

    DBG_PRINT("%s, %s, %d, serdes offset 0x%lx 0x%p\n", 
                __FILE__, __FUNCTION__, __LINE__, offset, ((UINT1 *)pCtxtBuf + offset));

    /* restore serdes context */
    for (i = 0; i < HYPHY20G_SERDES_LNK_MAX_NUM; i++) {
        /* restore pre-link serdes context */
        /* check if the payload is valid */
        if (pCtxtBufHdr->len == sizeof(sHYPHY20G_CTXT_HDR)) {
            offset += sizeof(sHYPHY20G_CTXT_HDR);
        } else if (pCtxtBufHdr->len > sizeof(sHYPHY20G_CTXT_HDR)) {
            if (i < HYPHY20G_SERDES_XFI_LNK_OFFSET) {
                /* SFP serdes link */
                linkId = i;
                intf = HYPHY20G_SERDES_SFP_INTF;
            } else if (i < HYPHY20G_SERDES_BACKPLANE_LNK_OFFSET) {
                /* XFI serdes link */
                linkId = i - HYPHY20G_SERDES_XFI_LNK_OFFSET;
                intf = HYPHY20G_SERDES_XFI_INTF;
            } else if (i < HYPHY20G_SERDES_PIF_LNK_OFFSET) {
                /* BackPlane serdes link */
                linkId = i - HYPHY20G_SERDES_BACKPLANE_LNK_OFFSET;
                intf = HYPHY20G_SERDES_BACKPLANE_INTF;
            } else {
                /* PIF serdes link */
                linkId = i - HYPHY20G_SERDES_PIF_LNK_OFFSET;
                intf = HYPHY20G_SERDES_PIF_INTF;
            }
            
            /* create Serdes context */
            result = serdesHyPhy20gSerdesCreateCtxt(fileHndl, intf, linkId);
            if (result) {
                DBG_PRINT("%s, %s, %d, serdesHyPhy20gSerdesCreateCtxt failed\n", 
                            __FILE__, __FUNCTION__, __LINE__);
		        /* just abort recovery and flag error to the master hyPhy20gRestoreCtxt() function,
		        and call hyPhy20gDestroyCtxt to do clean up work */
                *pParsedSize = 0;
                return result;
            }

            /* retrieve device context */
            pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
            /* confirm device context pointer is valid before processing */
            HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                        "%s", "pDevCtxt is NULL.");

            if (intf == HYPHY20G_SERDES_SFP_INTF) {
                pSerdesCtxt = pDevCtxt->pSfpSerdesCtxt[linkId];
            } else if (intf == HYPHY20G_SERDES_XFI_INTF) {
                pSerdesCtxt = pDevCtxt->pXfiSerdesCtxt[linkId];
            } else if (intf == HYPHY20G_SERDES_BACKPLANE_INTF) {
                pSerdesCtxt = pDevCtxt->pBackplaneSerdesCtxt[linkId];
            } else {
                pSerdesCtxt = pDevCtxt->pPifSerdesCtxt[linkId];
            }
            offset += utilHyPhy20gSerdesRestoreCtxt(pSerdesCtxt, ((UINT1 *)pCtxtBuf + offset));
        } else {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_CORRUPTED\n", 
                        __FILE__, __FUNCTION__, __LINE__);
            *pParsedSize = 0;
            return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
        }
        /* move to next ctxt buffer header */
        pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx 0x%p\n", 
                __FILE__, __FUNCTION__, __LINE__, offset, ((UINT1 *)pCtxtBuf + offset));

    *pParsedSize = offset;

    return HYPHY20G_SUCCESS;
}/* serdesHyPhy20gSerdesRestoreCtxt */

/********************************************************************************
**
**  sfpSerdesHyPhy20gSfpSerdesAuditCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    the function audit the SFP serdes subCTXT.
**
**
**  INPUTS:         fileHndl            - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_FAILURE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sfpSerdesHyPhy20gSfpSerdesAuditCtxt(struct file *fileHndl)
{
    INT4 result;
    UINT4 linkID,addr, rstBitOff, linkRst;
    UINT4 devId, revId, status;    
    sHYPHY20G_SERDES_ADC_CAL_CTXT *pAdcCalCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                    "%s", "hyPhy20gCheckRealRev return error.");

    /* the meta chip don't need care about this subCtxt, so skip checking it*/
    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_SUCCESS);
    
    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    
    
    /* the below check rule mainly come from hyPhy20gSfpSerdesInit function */
    addr = HYPHY20G_TOP_REG_SYS_SW_RST_4;

    for (linkID=0; linkID<=HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM; linkID++)
    {
        rstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_LINE_SFP_RST_0_OFF +
                    linkID;

        /* check link level reset bit */
        result = sysHyPhy20gBitRead(fileHndl, addr, rstBitOff, &linkRst);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 

        /*serdes link disabled */
        if (linkRst == 1)
        {
            if (pDevCtxt->pSfpSerdesCtxt[linkID]!=NULL)
            {
                DBG_PRINT("%s, %s, %d, pDevCtxt->pSfpSerdesCtxt[%u] != NULL when LINE_SFP_RST=1.\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)linkID);
                return HYPHY20G_FAILURE;
            }
            else
                continue;
        }

        /*serdes link enabled */
        if (pDevCtxt->pSfpSerdesCtxt[linkID]!=NULL)
        {
            pAdcCalCtxt=&(pDevCtxt->pSfpSerdesCtxt[linkID]->adcCalCtxt);
            if ((pAdcCalCtxt->calMode>HYPHY20G_SERDES_DEFER_CAL_MODE)||(pAdcCalCtxt->calReq>HYPHY20G_SERDES_RUN_CAL)
                ||(pAdcCalCtxt->calOk>HYPHY20G_SERDES_CAL_SUCCESS)||(pAdcCalCtxt->pisoEnb>1)
                ||(pAdcCalCtxt->adsmIntDiv>0xFF)||(*(pAdcCalCtxt->pOffset)>0xFF))
            {
                DBG_PRINT("%s, %s, %d, pDevCtxt->pSfpSerdesCtxt[%u]->adcCalCtxt members value is wrong.\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)linkID);
                return HYPHY20G_FAILURE;
            }
        }
    }
    
    return HYPHY20G_SUCCESS;
    
} /* sfpSerdesHyPhy20gSfpSerdesAuditCtxt */

/********************************************************************************
**
**  xfiSerdesHyPhy20gXfiSerdesAuditCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    the function audit the XFI serdes subCTXT.
**
**
**  INPUTS:         fileHndl            - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_FAILURE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 xfiSerdesHyPhy20gXfiSerdesAuditCtxt(struct file *fileHndl)
{
    INT4 result;
    UINT4 linkID,addr, rstBitOff, linkRst;
    sHYPHY20G_SERDES_ADC_CAL_CTXT *pAdcCalCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    
    /* the below check rule mainly come from hyPhy20gXfiSerdesInit function */
    addr = HYPHY20G_TOP_REG_SYS_SW_RST_4;

    for (linkID=0; linkID<=HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM; linkID++)
    {
        rstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_LINE_XFI_RST_0_OFF +
                    linkID;

        /* check link level reset bit */
        result = sysHyPhy20gBitRead(fileHndl, addr, rstBitOff, &linkRst);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 

        /*serdes link disabled */
        if (linkRst == 1)
        {
            if (pDevCtxt->pXfiSerdesCtxt[linkID]!=NULL)
            {
                DBG_PRINT("%s, %s, %d, pDevCtxt->pXfiSerdesCtxt[%u] != NULL when LINE_XFI_RST=1.\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)linkID);
                return HYPHY20G_FAILURE;
            }
            else
                continue;
        }

        /*serdes link enabled */
        if (pDevCtxt->pXfiSerdesCtxt[linkID]!=NULL)
        {
            pAdcCalCtxt=&(pDevCtxt->pXfiSerdesCtxt[linkID]->adcCalCtxt);
            if ((pAdcCalCtxt->calMode>HYPHY20G_SERDES_DEFER_CAL_MODE)||(pAdcCalCtxt->calReq>HYPHY20G_SERDES_RUN_CAL)
                ||(pAdcCalCtxt->calOk>HYPHY20G_SERDES_CAL_SUCCESS)||(pAdcCalCtxt->pisoEnb>1)
                ||(pAdcCalCtxt->adsmIntDiv>0xFF)||(*(pAdcCalCtxt->pOffset)>0xFF))
            {
                DBG_PRINT("%s, %s, %d, pDevCtxt->pXfiSerdesCtxt[%u]->adcCalCtxt members value is wrong.\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)linkID);
                return HYPHY20G_FAILURE;
            }
        }
    }
    
    return HYPHY20G_SUCCESS;
    
}/* xfiSerdesHyPhy20gXfiSerdesAuditCtxt */


/********************************************************************************
**
**  bkpSerdesHyPhy20gBkpSerdesAuditCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    the function audit the Backplabe serdes subCTXT.
**
**
**  INPUTS:         fileHndl            - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_FAILURE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 bkpSerdesHyPhy20gBkpSerdesAuditCtxt(struct file *fileHndl)
{
    INT4 result;
    UINT4 linkID, serdesAddr, csuAddr, serdesRstBitOff=0, csuRstBitOff, serdesRst, csuRst;
    UINT4 devId, revId, status;
    sHYPHY20G_SERDES_ADC_CAL_CTXT *pAdcCalCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                    "%s", "hyPhy20gCheckRealRev return error.");

    /* the meta chip don't need care about this subCtxt, so skip checking it*/
    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_SUCCESS);

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");
    
    /*the below check rule mainly come from hyPhy20gBackplaneSerdesInit */
    serdesAddr = HYPHY20G_TOP_REG_SYS_SW_RST_5;

    for (linkID=0; linkID<=HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM; linkID++)
    {
        /* links 0 to 7 and 16 to 23 corresponds to back0[0:15] */
        /* links 8 to 15 and 24 to 31 corresponds to back1[0:15] */
        if (linkID <= 7) {
            serdesRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_5_SERDES_BACK0_RST_0_OFF
                          + linkID;
        }
        if ((linkID >= 8) && (linkID <= 15)) {
            serdesRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_5_SERDES_BACK0_RST_0_OFF
                          + linkID + 8;
        }
        if ((linkID >= 16) && (linkID <= 23)) {
            serdesRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_5_SERDES_BACK0_RST_0_OFF
                          + linkID - 8;
        }
        if ((linkID >= 24) && (linkID <= 31)) {
            serdesRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_5_SERDES_BACK0_RST_0_OFF
                          + linkID;
        }

        /* check link level reset bit */
        result = sysHyPhy20gBitRead(fileHndl, serdesAddr, serdesRstBitOff, &serdesRst);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 

        /*serdes link disabled */
        if (serdesRst == 1)
        {
            if (pDevCtxt->pBackplaneSerdesCtxt[linkID]!=NULL)
            {
                DBG_PRINT("%s, %s, %d, pDevCtxt->pBackplaneSerdesCtxt[%u] != NULL when BACKx_RST=1.\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)linkID);
                return HYPHY20G_FAILURE;
            }
            else
                continue;
        }

        /*serdes link enabled */
        csuAddr = HYPHY20G_TOP_REG_SYS_SW_RST_4;
        if ((linkID <= 7) || ((linkID >= 16) && (linkID <= 23))) 
        {
            csuRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_BACK0_CSU_RST_OFF;
        } 
        else 
        {
            csuRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_BACK1_CSU_RST_OFF;
        }

        /* check CSU reset bit */
        result = sysHyPhy20gBitRead(fileHndl, csuAddr, csuRstBitOff, &csuRst);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 

        if (csuRst != 0)
        {
            DBG_PRINT("%s, %s, %d, %uth backplane serdes link BACKx_CSU_RST=1 when BACKx_RST=0\n",
                __FILE__, __FUNCTION__, __LINE__, (unsigned int)linkID);
            return HYPHY20G_FAILURE;
        }
        
        if (pDevCtxt->pBackplaneSerdesCtxt[linkID]!=NULL)
        {
            pAdcCalCtxt=&(pDevCtxt->pBackplaneSerdesCtxt[linkID]->adcCalCtxt);
            if ((pAdcCalCtxt->calMode>HYPHY20G_SERDES_DEFER_CAL_MODE)||(pAdcCalCtxt->calReq>HYPHY20G_SERDES_RUN_CAL)
                ||(pAdcCalCtxt->calOk>HYPHY20G_SERDES_CAL_SUCCESS)||(pAdcCalCtxt->pisoEnb>1)
                ||(pAdcCalCtxt->adsmIntDiv>0xFF)||(*(pAdcCalCtxt->pOffset)>0xFF))
            {
                DBG_PRINT("%s, %s, %d, pDevCtxt->pBackplaneSerdesCtxt[%u]->adcCalCtxt members value is wrong.\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)linkID);
                return HYPHY20G_FAILURE;
            }
        }
    }
    
    return HYPHY20G_SUCCESS;
    
}/* bkpSerdesHyPhy20gBkpSerdesAuditCtxt */

/********************************************************************************
**
**  pifSerdesHyPhy20gPifSerdesAuditCtxt 
**  ___________________________________________________________________________
**
**  DESCRIPTION:    function audit the PIF serdes subCTXT.
**
**
**  INPUTS:         fileHndl            - device handle
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_FAILURE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 pifSerdesHyPhy20gPifSerdesAuditCtxt (struct file *fileHndl)
{
    INT4 result;
    UINT4 linkID, serdesAddr, csuAddr, serdesRstBitOff, csuRstBitOff, serdesRst, csuRst;
    sHYPHY20G_SERDES_ADC_CAL_CTXT *pAdcCalCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    /* the below check rule mainly come from hyPhy20gPifSerdesInit */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    

    serdesAddr = HYPHY20G_TOP_REG_SYS_SW_RST_4;
    
    for (linkID=0; linkID<=HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM; linkID++)
    {
        serdesRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_SYS_RST_0_OFF+ linkID;

        /* check link level reset bit */
        result = sysHyPhy20gBitRead(fileHndl, serdesAddr, serdesRstBitOff, &serdesRst);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead operation fail.");    

        /*serdes link disabled */
        if (serdesRst == 1)
        {
            if (pDevCtxt->pPifSerdesCtxt[linkID]!=NULL)
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
            else
                continue;
        }

        /*serdes link enabled */
        csuAddr = HYPHY20G_TOP_REG_SYS_SW_RST_4;
        csuRstBitOff = HYPHY20G_TOP_BIT_SYS_SW_RST_4_SERDES_SYS_CSU_RST_OFF;

        /* check CSU reset bit */
        result = sysHyPhy20gBitRead(fileHndl, csuAddr, csuRstBitOff, &csuRst);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 

        if (csuRst != 0)
        {
            DBG_PRINT("%s, %s, %d, %uth backplane serdes link SYS_CSU_RST=1 when SYS_RST=0\n",
                __FILE__, __FUNCTION__, __LINE__, (unsigned int)linkID);
            return HYPHY20G_FAILURE;
        }
                
        if (pDevCtxt->pPifSerdesCtxt[linkID]!=NULL)
        {
            pAdcCalCtxt=&(pDevCtxt->pPifSerdesCtxt[linkID]->adcCalCtxt);
            if ((pAdcCalCtxt->calMode>HYPHY20G_SERDES_DEFER_CAL_MODE)||(pAdcCalCtxt->calReq>HYPHY20G_SERDES_RUN_CAL)||
                (pAdcCalCtxt->calOk>HYPHY20G_SERDES_CAL_SUCCESS)||(pAdcCalCtxt->pisoEnb>1)||
                (pAdcCalCtxt->adsmIntDiv>0xFF)||(*(pAdcCalCtxt->pOffset)>0xFF))
            {
                DBG_PRINT("%s, %s, %d, pDevCtxt->pPifSerdesCtxt[%u]->adcCalCtxt members value is wrong.\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)linkID);
                return HYPHY20G_FAILURE;
            }
        }
    }
    
    return HYPHY20G_SUCCESS;
    
}/* pifSerdesHyPhy20gPifSerdesAuditCtxt */

/* end of file */
