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
**  MODULE      : HyPHY 20G Configuration Code
**
**  FILE        :  hyphy20g_serdes.h
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :  Contains specific definitions for hyphy20g serdes
**
**  NOTES       :
**
**
*******************************************************************************/

#ifndef _HYPHY20G_SERDES_H
#define _HYPHY20G_SERDES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hyphy20g.h"
#include "hyphy20g_util_acb.h"
#include "hyphy20g_strs.h"
#include "hyphy20g_hw.h"
#include "hyphy20g_hostmsg.h"

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/

#define HYPHY20G_SERDES_NPHASES_SFP                                             12
#define HYPHY20G_SERDES_NPHASES_SGMII                                           12
#define HYPHY20G_SERDES_NPHASES_XFI                                             6
#define HYPHY20G_SERDES_NPHASES_PIF                                             3
#define HYPHY20G_SERDES_NPHASES_BACKPLANE                                       3
#define HYPHY20G_SERDES_NCOMPS_SFP                                              1
#define HYPHY20G_SERDES_NCOMPS_SGMII                                            1
#define HYPHY20G_SERDES_NCOMPS_XFI                                              9
#define HYPHY20G_SERDES_NCOMPS_PIF                                              9
#define HYPHY20G_SERDES_NCOMPS_BACKPLANE                                        9

#define HYPHY20G_SERDES_NLINKS_PER_SFP_CSU                                      1
#define HYPHY20G_SERDES_NLINKS_PER_XFI_CSU                                      1
#define HYPHY20G_SERDES_NLINKS_PER_PIF_CSU                                      8
#define HYPHY20G_SERDES_NLINKS_PER_BACKPLANE_CSU                                16

#define HYPHY20G_SERDES_ADC_CAL_REPEAT_TOL                                      2
#define HYPHY20G_SERDES_ADC_CAL_COUNT_MAX                                       5

#define HYPHY20G_SERDES_GRP_MAX_NUM                                             6
#define HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM                                  15
#define HYPHY20G_SERDES_GRP_SFI4_LNK_ID_MAX_NUM                                 1
#define HYPHY20G_SERDES_GRP_SGMII_LNK_ID_MAX_NUM                                0
#define HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM                                  1
#define HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM                                  7
#define HYPHY20G_SERDES_GRP_PCIE_LNK_ID_MAX_NUM                                 3
#define HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM                            31
#define HYPHY20G_SERDES_GRP_BACKPLANE_CSU_ID_MAX_NUM                            1

#define HYPHY20G_SERDES_GRP_SFP_LNK_MAX_NUM (HYPHY20G_SERDES_GRP_SFP_LNK_ID_MAX_NUM + 1)
#define HYPHY20G_SERDES_GRP_XFI_LNK_MAX_NUM  (HYPHY20G_SERDES_GRP_XFI_LNK_ID_MAX_NUM + 1)
#define HYPHY20G_SERDES_GRP_BACKPLANE_LNK_MAX_NUM (HYPHY20G_SERDES_GRP_BACKPLANE_LNK_ID_MAX_NUM + 1)
#define HYPHY20G_SERDES_GRP_PIF_LNK_MAX_NUM (HYPHY20G_SERDES_GRP_PIF_LNK_ID_MAX_NUM + 1)

#define HYPHY20G_SERDES_LNK_MAX_NUM           (HYPHY20G_SERDES_GRP_SFP_LNK_MAX_NUM +      \
                                              HYPHY20G_SERDES_GRP_XFI_LNK_MAX_NUM +       \
                                              HYPHY20G_SERDES_GRP_BACKPLANE_LNK_MAX_NUM + \
                                              HYPHY20G_SERDES_GRP_PIF_LNK_MAX_NUM)        

#define HYPHY20G_SERDES_XFI_LNK_OFFSET        HYPHY20G_SERDES_GRP_SFP_LNK_MAX_NUM
#define HYPHY20G_SERDES_BACKPLANE_LNK_OFFSET  (HYPHY20G_SERDES_GRP_SFP_LNK_MAX_NUM +   \
                                              HYPHY20G_SERDES_GRP_XFI_LNK_MAX_NUM)
#define HYPHY20G_SERDES_PIF_LNK_OFFSET        (HYPHY20G_SERDES_GRP_SFP_LNK_MAX_NUM +   \
                                              HYPHY20G_SERDES_GRP_XFI_LNK_MAX_NUM +    \
                                              HYPHY20G_SERDES_GRP_BACKPLANE_LNK_MAX_NUM)

#define HYPHY20G_SFP_SERDES_RATES_MAX_NUM                                       41
#define HYPHY20G_SFI4_SERDES_RATES_MAX_NUM                                      0
#define HYPHY20G_SGMII_SERDES_RATES_MAX_NUM                                     1
#define HYPHY20G_XFI_SERDES_RATES_MAX_NUM                                       17
#define HYPHY20G_PIF_SERDES_RATES_MAX_NUM                                       2
#define HYPHY20G_PCIE_SERDES_RATES_MAX_NUM                                      0
#define HYPHY20G_BACKPLANE_SERDES_RATES_MAX_NUM                                 3

#define HYPHY20G_SERDES_ADC_CLIP_RATE_SAMPLE_SIZE                               30000
#define HYPHY20G_SERDES_SFP_ADSM_CLK_CNT_MAX_VAL                                4000
#define HYPHY20G_SERDES_MAX_SYSCLK_POLL                                         8000
#define HYPHY20G_SERDES_XFI_ADSM_CLK_CNT_MAX_VAL                                4000
#define HYPHY20G_SERDES_BACKPLANE_ADSM_CLK_CNT_MAX_VAL                          4000
#define HYPHY20G_SERDES_PIF_ADSM_CLK_CNT_MAX_VAL                                4000

#define HYPHY20G_SERDES_SFP_INTF                                                0
#define HYPHY20G_SERDES_XFI_INTF                                                1
#define HYPHY20G_SERDES_BACKPLANE_INTF                                          2
#define HYPHY20G_SERDES_PIF_INTF                                                3
#define HYPHY20G_SERDES_NO_CAL_MODE                                             0
#define HYPHY20G_SERDES_DYNAMIC_CAL_MODE                                        1
#define HYPHY20G_SERDES_FACTORY_CAL_MODE                                        2
#define HYPHY20G_SERDES_DEFER_CAL_MODE                                          3
#define HYPHY20G_SERDES_NO_CAL                                                  0
#define HYPHY20G_SERDES_RUN_CAL                                                 1
#define HYPHY20G_SERDES_CAL_FAIL                                                0
#define HYPHY20G_SERDES_CAL_SUCCESS                                             1

#define HYPHY20G_SERDES_LINEAR_MAP                                              0
#define HYPHY20G_SERDES_NONLINEAR_MAP                                           1
#define HYPHY20G_SERDES_MAX_MAP_TYPE                                            1       /* maximum value for mapping type */
#define HYPHY20G_SERDES_LINEAR_MIN_SCALE                                        1
#define HYPHY20G_SERDES_LINEAR_MAX_SCALE                                        8
#define HYPHY20G_SERDES_LINEAR_POS_BINS                                         0x9     /* 2's complement =  9 */
#define HYPHY20G_SERDES_LINEAR_NEG_BINS                                         0x17    /* 2's complement = -9 */
#define HYPHY20G_SERDES_NONLINEAR_MIN_SCALE                                     1
#define HYPHY20G_SERDES_NONLINEAR_MAX_SCALE                                     5
#define HYPHY20G_SERDES_NONLINEAR_POS_BINS                                      0xE     /* 2's complement =  14 */
#define HYPHY20G_SERDES_NONLINEAR_NEG_BINS                                      0x12    /* 2's complement = -14 */


/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/
/******************************************************************************/
/* TEMPORARY DEFINES                                                          */
/******************************************************************************/

/******************************************************************************/
/* ENUMERATED TYPES                                                           */
/******************************************************************************/
typedef enum {
    HYPHY20G_SERDES_GRP_SFP         = 0,
    HYPHY20G_SERDES_GRP_SFI4        = 1,
    HYPHY20G_SERDES_GRP_SGMII       = 2,
    HYPHY20G_SERDES_GRP_XFI         = 3,
    HYPHY20G_SERDES_GRP_PIF         = 4,
    HYPHY20G_SERDES_GRP_PCIE        = 5,
    HYPHY20G_SERDES_GRP_BACKPLANE   = 6
} eHYPHY20G_SERDES_GRP_NAME;

typedef enum {
    HYPHY20G_SFP_SERDES_FE_125          = 0,
    HYPHY20G_SFP_SERDES_GE_1250         = 1,
    HYPHY20G_SFP_SERDES_E3_34_368       = 2,
    HYPHY20G_SFP_SERDES_T3_44_736       = 3,
    HYPHY20G_SFP_SERDES_OC1_51_84       = 4,
    HYPHY20G_SFP_SERDES_OC3_155_52      = 5,
    HYPHY20G_SFP_SERDES_OC12_622_08     = 6,
    HYPHY20G_SFP_SERDES_OC48_2488_32    = 7,
    HYPHY20G_SFP_SERDES_OC51_2643_84    = 8,
    HYPHY20G_SFP_SERDES_OC60_3110_4     = 9,
    HYPHY20G_SFP_SERDES_ODU1_2498_775   = 10,
    HYPHY20G_SFP_SERDES_OTU1_2666_057   = 11,
    HYPHY20G_SFP_SERDES_FC12_132_813    = 12,
    HYPHY20G_SFP_SERDES_FC25_265_525    = 13,
    HYPHY20G_SFP_SERDES_FC50_531_25     = 14,
    HYPHY20G_SFP_SERDES_FC100_1062_5    = 15,
    HYPHY20G_SFP_SERDES_FC200_2125      = 16,
    HYPHY20G_SFP_SERDES_FC400_4250      = 17,
    HYPHY20G_SFP_SERDES_CLOETR_16       = 18,
    HYPHY20G_SFP_SERDES_FDDI_125        = 19,
    HYPHY20G_SFP_SERDES_ESCON_200       = 20,
    HYPHY20G_SFP_SERDES_ISC_106_25      = 21,
    HYPHY20G_SFP_SERDES_ISC3_212_5      = 22,
    HYPHY20G_SFP_SERDES_VIDEO_143_182   = 23,
    HYPHY20G_SFP_SERDES_VIDEO_177       = 24,
    HYPHY20G_SFP_SERDES_VIDEO_270       = 25, /* SD-SDI */
    HYPHY20G_SFP_SERDES_VIDEO_360       = 26, /* SD-SDI */
    HYPHY20G_SFP_SERDES_VIDEO_540       = 27,
    HYPHY20G_SFP_SERDES_HD_SDI_1485     = 28,
    HYPHY20G_SFP_SERDES_VIDEO_1483_516  = 29, /* HD-SDI 1.001 */
    HYPHY20G_SFP_SERDES_3G_HD_SDI_2970  = 30,
    HYPHY20G_SFP_SERDES_VIDEO_2967_033  = 31,
    HYPHY20G_SFP_SERDES_DV6000_2380     = 32,
    HYPHY20G_SFP_SERDES_FWIRE100_98_304 = 33,
    HYPHY20G_SFP_SERDES_FWIRE200_196_608 = 34,
    HYPHY20G_SFP_SERDES_CPRI_614_4      = 35,
    HYPHY20G_SFP_SERDES_CPRI_1228_8     = 36,
    HYPHY20G_SFP_SERDES_CPRI_2457_6     = 37,
    HYPHY20G_SFP_SERDES_CPRI_3072       = 38,
    HYPHY20G_SFP_SERDES_CPRI_4915_2     = 39,
    HYPHY20G_SFP_SERDES_DVB_ASI_270     = 40,
    HYPHY20G_SFP_SERDES_INFINIBAND_5000 = 41
} eHYPHY20G_SFP_SERDES_RATES;

/* don't need SFI as it's LVDS
typedef enum {
    HYPHY20G_SFI4_LVDS                  = 0,
    HYPHY20G_SFI4_LVDS777               = 1
} eHYPHY20G_SFI4_SERDES_RATES;
*/

typedef enum {
    HYPHY20G_SGMII_SERDES_FE_125         = 0,
    HYPHY20G_SGMII_SERDES_GE_1250        = 1
} eHYPHY20G_SGMII_SERDES_RATES;

typedef enum {
    HYPHY20G_XFI_SERDES_OC48_2488_32                = 0,
    HYPHY20G_XFI_SERDES_OC192_9953_28               = 1,
    HYPHY20G_XFI_SERDES_10GE_LAN_10312_5            = 2,
    HYPHY20G_XFI_SERDES_10GE_WAN_9953_28            = 3,
    HYPHY20G_XFI_SERDES_OTU2_10709                  = 4,
    HYPHY20G_XFI_SERDES_OTU2_GSUP43_7_1_11096       = 5,
    HYPHY20G_XFI_SERDES_OTU2_GSUP43_7_2_11049       = 6,
    HYPHY20G_XFI_SERDES_OTU2_FC1200_11317_6         = 7,
    HYPHY20G_XFI_SERDES_OTU2_FC1200_11270_1         = 8,
    HYPHY20G_XFI_SERDES_ODU2_10037_3                = 9,
    HYPHY20G_XFI_SERDES_ODU1e_10355_8               = 10,
    HYPHY20G_XFI_SERDES_ODU2e_10399_5               = 11,
    HYPHY20G_XFI_SERDES_ODU1f_10562_9               = 12,
    HYPHY20G_XFI_SERDES_ODU2f_10607_5               = 13,
    HYPHY20G_XFI_SERDES_FC8G_8500                   = 14,
    HYPHY20G_XFI_SERDES_FC10G_10518_75              = 15,
    HYPHY20G_XFI_SERDES_SFI4_1_SLAVE                = 16,
    HYPHY20G_XFI_SERDES_CPRI_9830_4                 = 17
} eHYPHY20G_XFI_SERDES_RATES;

typedef enum {
    HYPHY20G_PIF_SERDES_INTERLAKEN_6250             = 0,
    HYPHY20G_PIF_SERDES_XAUI_3125                   = 1,
    HYPHY20G_PIF_SERDES_INTERLAKEN_3125             = 2
} eHYPHY20G_PIF_SERDES_RATES;

typedef enum {
    HYPHY20G_BACKPLANE_SERDES_INTERLAKEN_2488_32    = 0,
    HYPHY20G_BACKPLANE_SERDES_INTERLAKEN_3125       = 1,
    HYPHY20G_BACKPLANE_SERDES_ESSI_2488_32          = 2,
    HYPHY20G_BACKPLANE_SERDES_CBRI_3333_333_3       = 3
} eHYPHY20G_BACKPLANE_SERDES_RATES;

typedef enum {
    HYPHY20G_SERDES_ADC_MODE_A      = 0,
    HYPHY20G_SERDES_ADC_MODE_B      = 1,
    HYPHY20G_SERDES_ADC_MODE_C      = 2,
    HYPHY20G_SERDES_ADC_MODE_D      = 3
} eHYPHY20G_SERDES_ADC_MODE_TYPE;

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                 */
/******************************************************************************/

INT4 hyPhy20gSfpRateCfg(struct file *fileHndl,
                        UINT4 linkID,
                        UINT4 rate,
                        UINT4 adcCal,
                        UINT4 enable);
INT4 hyPhy20gSfpSerdesInit(struct file *fileHndl,
                           UINT4 linkID,
                           UINT4 enable);
INT4 hyPhy20gSfpSfi2SerdesModeCfg(struct file *filehndl,
                                  UINT4 mode);
INT4 hyPhy20gSfi1Init(struct file *fileHndl,
                            UINT4 enable);
INT4 hyPhy20gXfiSfi1TxClkCfg(struct file *fileHndl,
                             UINT4 mode,
                             UINT4 div);
INT4 hyPhy20gSfi2Init(struct file *fileHndl,
                            UINT4 enable);
INT4 hyPhy20gXfiSfi2TxClkCfg(struct file *fileHndl,
                             UINT4 mode,
                             UINT4 div);
INT4 hyPhy20gSgmiiRateCfg(struct file *fileHndl,
                          UINT4 rate,
                          UINT4 enable);
INT4 hyPhy20gSgmiiSerdesInit(struct file *fileHndl,
                             UINT4 enable);
INT4 hyPhy20gXfiRefClkInit(struct file *fileHndl, UINT4 linkID, UINT4 enable);

INT4 hyPhy20gXfiRateCfg(struct file *fileHndl,
                        UINT4 linkID,
                        UINT4 rate,
                        UINT4 adcCal,
                        UINT4 eqMode,
                        UINT4 enable);
INT4 hyPhy20gXfiSerdesInit(struct file *fileHndl,
                           UINT4 linkID,
                           UINT4 enable);
INT4 hyPhy20gPifRateCfg(struct file *fileHndl,
                        UINT4 linkID,
                        UINT4 rate,
                        UINT4 adcCal,
                        UINT4 eqMode,
                        UINT4 enable);
INT4 hyPhy20gPifSerdesInit(struct file *fileHndl,
                           UINT4 linkID,
                           UINT4 enable);
INT4 hyPhy20gBackplaneRateCfg(struct file *fileHndl,
                              UINT4 linkID,
                              UINT4 rate,
                              UINT4 adcCal,
                              UINT4 eqMode,
                              UINT4 enable);
INT4 hyPhy20gBackplaneSerdesInit(struct file *fileHndl,
                                 UINT4 linkID,
                                 UINT4 enable);
INT4 hyPhy20gXfiSerdesTxCfg(struct file *fileHndl,
                            UINT4 linkID,
                            UINT4 preEmp,
                            UINT4 swing,
                            UINT4 enable);
INT4 hyPhy20gSfpSerdesTxCfg(struct file *fileHndl,
                            UINT4 linkID,
                            UINT4 preEmp,
                            UINT4 swing,
                            UINT4 enable);
INT4 hyPhy20gSgmiiSerdesTxCfg(struct file *fileHndl,
                              UINT4 preEmp,
                              UINT4 swing,
                              UINT4 enable);
INT4 hyPhy20gBackplaneSerdesTxCfg(struct file *fileHndl,
                                  UINT4 linkID,
                                  UINT4 preEmp,
                                  UINT4 swing,
                                  UINT4 enable);
INT4 hyPhy20gPifSerdesTxCfg(struct file *fileHndl,
                            UINT4 linkID,
                            UINT4 preEmp,
                            UINT4 swing,
                            UINT4 enable);
INT4 hyPhy20gSfpCustomRateCfg(struct file *fileHndl,
                              UINT4 linkID,
                              UINT4 busWidth,
                              UINT4 adsmFracDiv,
                              UINT4 adsmIntDiv,
                              UINT4 adsmDenomin,
                              UINT4 rxPathDiv,
                              UINT4 pisoDivEnable,
                              UINT4 pisoCtrl,
                              UINT4 postDiv,
                              UINT4 vcoSelect,
                              UINT4 enable);
INT4 hyPhy20gXfiCustomRateCfg(struct file *fileHndl,
                              UINT4 linkID,
                              UINT4 busWidth,
                              UINT4 adsmFracDiv,
                              UINT4 adsmIntDiv,
                              UINT4 adsmDenomin,
                              UINT4 rxPathDiv,
                              UINT4 pisoDivEnable,
                              UINT4 pisoCtrl,
                              UINT4 postDiv,
                              UINT4 enable);
INT4 hyPhy20gSerdesADCCalibrate(struct file *fileHndl, UINT4 sfpMsk,
                                UINT4 xfiMsk, UINT4 bpMsk, UINT4 pifMsk,
                                UINT4 eqMode, UINT4 *pSfpCalFail,
                                UINT4 *pXfiCalFail, UINT4 *pBpCalFail,
                                UINT4 *pPifCalFail);
INT4 hyPhy20gSerdesLoadFactoryCal(struct file *fileHndl, UINT4 intf,
                                  STRING *fread);
INT4 hyPhy20gSerdesDbgPrintCtxt(struct file *fileHndl, UINT4 intf,
                                UINT4 linkID);
INT4 hyPhy20gSerdesModeSelectCfg(struct file *fileHndl, UINT4 intf,
                                 UINT4 linkID, UINT4 mode);
INT4 hyPhy20gSerdesManualModeSelectCfg(struct file *fileHndl, UINT4 intf,
                                       UINT4 linkID, UINT4 accumRefSelVal,
                                       UINT4 nonLinMapVal);
INT4 hyPhy20gSerdesEqCfg(struct file *fileHndl, UINT4 intf, UINT4 linkID,
                         UINT4 mode);
INT4 hyPhy20gSerdesManualEqCfg(struct file *fileHndl, UINT4 intf, UINT4 linkID,
                               UINT4 timingEqTapVal, UINT4 eqTap1Val, 
                               UINT4 eqTap2Val, UINT4 eqTap3Val);
INT4 hyPhy20gSerdesExtractCtxt(struct file *fileHndl, void *pCtxtBuf, 
                               UINT4 *pExtractedSize);
INT4 hyPhy20gSerdesRestoreCtxt(struct file *fileHndl, void *pCtxtBuf, 
                               UINT4 *pParsedSize);
UINT4 hyPhy20gSerdesGetCtxtSize(struct file *fileHndl);        
INT4 hyPhy20gSerdesLoadCtxtFactoryCal(struct file *fileHndl, UINT4 intf, 
                                      UINT4 linkId, void *pCtxtBuf, 
                                      UINT4 *pParsedSize); 
INT4 hyPhy20gSerdesRxFifoReCenter(struct file *fileHndl, UINT4 intf,
                                      UINT4 linkID);
                         
/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 serdesHyPhy20gSerdesCreateCtxt(struct file *fileHndl, UINT4 intf,
                                    UINT4 linkID);
INT4 serdesHyPhy20gSerdesDestroyCtxt(struct file *fileHndl, UINT4 intf,
                                     UINT4 linkID);
UINT4 serdesHyPhy20gSerdesGetCtxtSize(struct file *fileHndl);
INT4 serdesHyPhy20gSerdesExtractCtxt(struct file *fileHndl, 
                                     void *pCtxtBuf, UINT4 *pExtractedSize);
INT4 serdesHyPhy20gSerdesRestoreCtxt(struct file *fileHndl, 
                                     void *pCtxtBuf, UINT4 *pParsedSize);
INT4 sfpSerdesHyPhy20gSfpSerdesAuditCtxt(struct file *fileHndl);
INT4 xfiSerdesHyPhy20gXfiSerdesAuditCtxt(struct file *fileHndl);
INT4 bkpSerdesHyPhy20gBkpSerdesAuditCtxt(struct file *fileHndl);
INT4 pifSerdesHyPhy20gPifSerdesAuditCtxt (struct file *fileHndl);
#ifdef __cplusplus
}
#endif

#endif /* _HYPHY20G_SERDES_H */
/* end of file */
