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
**  FILE        : hyphy20g_gfp.c
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
#include    "hyphy20g_gfp.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 gfpHyPhy20gGfpScbsFreeSpace(struct file *fileHndl,
                                    sHYPHY20G_GFP_SLICE_CTXT *pGfpSliceCtxt,
                                    UINT4 *unusedRows, UINT4 *firstUnused);

INT4 gfpHyPhy20gGfpScbsGrpShift(struct file *fileHndl,
                                    sHYPHY20G_GFP_SLICE_CTXT *pGfpSliceCtxt,
                                    UINT4 grpId, UINT4 shift, UINT4 direction);

INT4 gfpHyPhy20gGfpCreateSliceCtxt(struct file *fileHndl, UINT4 slice, 
                                    UINT4 noInitSchd, UINT4 noInitRdChnl);

INT4 gfpHyPhy20gGfpDestroyScbsRowCtxt(struct file *fileHndl,
                                      sHYPHY20G_GFP_SCBS_ROW_CTXT *pGfpRowCtxt);

INT4 gfpHyPhy20gGfpGenerateSchdData( struct file *fileHndl, UINT4 slice,
                                     UINT4 grpId, UINT4 compress, UINT4 *pNumRows,
                                     UINT4 *pCalData);

INT4 gfpHyPhy20gGfpSetChnlCtxt(struct file *fileHndl, UINT4 slice, UINT4 chnl,
                               UINT4 valid, UINT4 mode, UINT4 numSb,
                               UINT4 cpbPktLen, UINT4 gfpEncapLen);

INT4 gfpHyPhy20gGfpSetMskChnlCtxt(struct file *fileHndl, UINT4 slice,
                                  UINT4 chnl, UINT4 valid, UINT4 mode,
                                  UINT4 numSb, UINT4 cpbPktLen,
                                  UINT4 gfpEncapLen, UINT4 setMsk);

INT4 gfpHyPhy20gGfpGetChnlCtxt(struct file *fileHndl, UINT4 slice, UINT4 chnl,
                               UINT4 *pValid, UINT4 *pMode, UINT4 *pNumSb,
                               UINT4 *pCpbPktLen, UINT4 *pGfpEncapLen);

INT4 gfpHyPhy20gGfpGenerateRateAwareSchdData( struct file *fileHndl,
                                              UINT4 slice, UINT4 chnlId,
                                              UINT4 addRemove, UINT4 grpId,
                                              UINT4 mode, UINT4 numSb,
                                              UINT4 cpbPktLen, UINT4 gfpEncapLen,
                                              UINT4 compress, UINT4 *pNumRows,
                                              UINT4 *pCalData, UINT4 *pOptimal);

INT4 gfpHyPhy20gGrpCalcVcatGrpRate(UINT4 memType, UINT4 numMem, UINT4 *rate);

INT4 gfpHyPhy20gGfpEvalChnlCtxt(struct file *fileHndl, UINT4 slice, UINT4 chnl,
                                UINT4 rate, UINT4 *pMode, UINT4 *pFrmRate,
                                UINT4 *pLineRate, UINT4 *pLineRateUnits);

INT4 gfpHyPhy20gGfpGetVcatGrpRate(struct file *fileHndl, UINT4 slice, UINT4 grp,
                                  UINT4 *pRate);

PRIVATE INT4 gfpHyPhy20gGfpAuditSchdCtxt(struct file *fileHndl, UINT4 slice);
PRIVATE INT4 gfpHyPhy20gGfpAuditGfpRowCtxt(struct file *fileHndl, UINT4 slice);
PRIVATE INT4 gfpHyPhy20gGfpAuditGfpChnlCtxt(struct file *fileHndl, UINT4 slice);
/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gGfpInit
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Initializes one of the two available GFP subsystem slices
**                  for operation in either transparent mode, GFP/HDLC mode, or
**                  a mix of both.  This function should only be called after
**                  the respective VCAT slice has been enabled using either
**                  hyPhy20gVcatLcasAInit or hyPhy20gVcatLcasBInit.  As well,
**                  hyPhy20gCpbInit must be called prior to calling this 
**                  function.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              enable      - 0 - Disable Subsystem
**                          - 1 - Enable Subsystem
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**
*******************************************************************************/
INT4 hyPhy20gGfpInit(struct file *fileHndl, UINT4 slice, UINT4 enable)
{
    UINT4 gfpFrmCfg = 0;
    UINT4 gfpTraCfg = 0;
    UINT4 regAddr = 0;
    UINT4 offset = 0;
    UINT4 mask = 0;
    UINT4 sonetEnValue = 0;
    UINT4 i;
    INT4 result;

    eHYPHY20G_SCBS_SCHD_ID schId;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt;

    /* argument checking */
    if ((slice > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to GFP level context information for this slice */
    if (slice == 0) {
        pGfpCtxt = pDevCtxt->pGfpACtxt;
        schId = HYPHY20G_GFP_TX_SCHD_0;
    } else {
        pGfpCtxt = pDevCtxt->pGfpBCtxt;
        schId = HYPHY20G_GFP_TX_SCHD_1;
    }

    /* Reset GFP susbystem at the top level.  */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_3;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_3_GFP0_FRM_RST_OFF + 2*slice;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;
    }

    offset++;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;
    }

    /* Apply Top Level Low Power setting */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_3;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_GFP0_FRM_LOWPWR_OFF + 2*slice;
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
    }

    offset++;
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
    }

    if (enable) {
        /* Perform Block level reset of GFP_10G_FRM / RXDP64 / TXDP64 */
        regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);

        result = sysHyPhy20gRead(fileHndl, regAddr, &gfpFrmCfg);
        if (result) return result;

        mask = HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpFrmCfg, mask, 1);
        if (result) return result;

        mask = HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_RXDP64_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpFrmCfg, mask, 1);
        if (result) return result;

        mask = HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_TXDP64_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpFrmCfg, mask, 1);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, gfpFrmCfg);
        if (result) return result;

        result = sysHyPhy20gRead(fileHndl, regAddr, &gfpFrmCfg);
        if (result) return result;

        mask = HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpFrmCfg, mask, 0);
        if (result) return result;

        mask = HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_RXDP64_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpFrmCfg, mask, 0);
        if (result) return result;

        mask = HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_TXDP64_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpFrmCfg, mask, 0);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, gfpFrmCfg);
        if (result) return result;

        /* Perform block level reset of GFP_10G_TRA / OTNMA / OTNMD /
                                                            RXGFPT / TXGFPT */
        regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);

        result = sysHyPhy20gRead(fileHndl, regAddr, &gfpTraCfg);
        if (result) return result;

        mask = HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpTraCfg, mask, 1);
        if (result) return result;

        mask = HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_OTNMA_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpTraCfg, mask, 1);
        if (result) return result;

        mask = HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_OTNMD_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpTraCfg, mask, 1);
        if (result) return result;

        mask = HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_TXGFPT_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpTraCfg, mask, 1);
        if (result) return result;

        mask = HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_RXGFPT_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpTraCfg, mask, 1);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, gfpTraCfg);
        if (result) return result;

        result = sysHyPhy20gRead(fileHndl, regAddr, &gfpTraCfg);
        if (result) return result;

        mask = HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpTraCfg, mask, 0);
        if (result) return result;

        mask = HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_OTNMA_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpTraCfg, mask, 0);
        if (result) return result;

        mask = HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_OTNMD_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpTraCfg, mask, 0);
        if (result) return result;

        mask = HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_TXGFPT_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpTraCfg, mask, 0);
        if (result) return result;

        mask = HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_RXGFPT_SFTWRST_MSK;
        result = hyPhy20gVarFieldWrite(&gfpTraCfg, mask, 0);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, gfpTraCfg);
        if (result) return result;

        /* Set XOFF in TXDP to '1' for all groups */
        regAddr = HYPHY20G_GFP_FRM_TXDP_REG_TX_DIS_OFFSET(0);
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;
        regAddr = HYPHY20G_GFP_FRM_TXDP_REG_TX_DIS_OFFSET(1);
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFFFF);
        if (result) return result;


        /* Enable TXDP64 / RXDP64 / TXGFPT / RXGFPT Blocks */
        regAddr = HYPHY20G_GFP_FRM_TXDP_TSB_BASE;
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_GFP_FRM_TXDP_BIT_CFG_TXDP_EN_OFF,
                                1);
        if (result) return result;

        regAddr = HYPHY20G_GFP_FRM_RXDP_TSB_BASE;
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_GFP_FRM_RXDP_BIT_CFG_RXDP64_EN_OFF,
                                1);
        if (result) return result;

        regAddr = HYPHY20G_GFP_TRA_TXGFPT_MTSB_BASE;
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                            HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_CFG_TXGFPT_EN_OFF,
                                1);
        if (result) return result;

        regAddr = HYPHY20G_GFP_TRA_RXGFPT_TSB_BASE;
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0x30E1);
        if (result) return result;

        /* Initialize RXDP GROUP Memory using SELF_INITIALIZE bit*/
        for (i = 0; i<HYPHY20G_GFP_TX_SCHD_MAX_GRP; i++) {
            result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x0, i, 0x2,
                                    0x0);
            if (result) return result;
            result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x0, i, 0x0,
                                    0x0);
            if (result) return result;
        }

       /* Initialize RXDP Channel Memory using SELF_INITIALIZE bit*/
        for (i = 0; i<HYPHY20G_GFP_NUM_CPBID; i++) {
            result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x20, i, 0x1,
                                    0x0);
            if (result) return result;
            result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x20, i, 0x20006,
                                    0x0);
            if (result) return result;
        }

        /* Configure RXDP GFP_SYSCLK_SEL (0 for OTN mode, 1 for SONET mode) */
        regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
        result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                        &sonetEnValue);
        if (result) return result;

        regAddr = HYPHY20G_PGMRCLK_REG_GFP_SYSCLK_SEL;
        mask = (slice == 0)? HYPHY20G_PGMRCLK_BIT_GFP_SYSCLK_SEL_GFP_0_SYSCLK_SEL_MSK:
        HYPHY20G_PGMRCLK_BIT_GFP_SYSCLK_SEL_GFP_1_SYSCLK_SEL_MSK;
        
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, 
                                       mask, sonetEnValue);
        if (result) return result;

        /* Initialize PMON blocks by doing a dummy read */
        /* TXGFPT PMON init*/
        regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_IND_PMON_CTL_AND_STAT;
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        offset = HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_PMON_CTL_AND_STAT_PMON_INDIRECT_REQ_BUSY_OFF;
        mask = HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_IND_PMON_CTL_AND_STAT_PMON_INDIRECT_REQ_BUSY_MSK;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
        result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                        HYPHY20G_DELAY_20USEC);
        if (result) return result;

        /* RXGFPT PMON init*/
        regAddr = HYPHY20G_GFP_TRA_RXGFPT_REG_IND_PMON_CTL;
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        offset = HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_PMON_CTL_PMON_BUSY_OFF;
        mask = HYPHY20G_GFP_TRA_RXGFPT_BIT_IND_PMON_CTL_PMON_BUSY_MSK;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
        result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                        HYPHY20G_DELAY_20USEC);
        if (result) return result;

        /* TXDP PMON init */
        regAddr = HYPHY20G_GFP_FRM_TXDP_REG_IND_PMON_CTL;
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        offset = HYPHY20G_GFP_FRM_TXDP_BIT_IND_PMON_CTL_PMON_BUSY_OFF;
        mask = HYPHY20G_GFP_FRM_TXDP_BIT_IND_PMON_CTL_PMON_BUSY_MSK;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
        result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                        HYPHY20G_DELAY_20USEC);
        if (result) return result;

        /* RXDP PMON init */
        regAddr = HYPHY20G_GFP_FRM_RXDP_REG_PMON_LATCH_CTL;
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        offset = HYPHY20G_GFP_FRM_RXDP_BIT_PMON_LATCH_CTL_PMON_LATCH_TRIG_OFF;
        mask = HYPHY20G_GFP_FRM_RXDP_BIT_PMON_LATCH_CTL_PMON_LATCH_TRIG_MSK;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
        result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                        HYPHY20G_DELAY_20USEC);
        if (result) return result;


        /* Create GFP level context for this slice */
        result = gfpHyPhy20gGfpCreateSliceCtxt(fileHndl, slice, 0, 0);
        if (result) return result;


    } else {
        /* Disable subsystem */
        /* Destroy GFP level context for this slice */
        result = gfpHyPhy20gGfpDestroySliceCtxt(fileHndl, slice);
        if (result) return result;
    }
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpInit */

/*******************************************************************************
**
**  hyPhy20gGfpGfpGrpInit
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Initializes and configures the group parameters of a GFP
**                  group within the GFP subsystem.  This procedure should only
**                  be called if there are no channels currently assigned to
**                  this group.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              grpId       - grpId of the group being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              pFcsStrip   - Controls whether payload FCS should be stripped
**                            from Rx frames destined for the FDF interface.
**                          - 0 - do not strip pFcs
**                          - 1 - strip pFCS
**              enable      - 1 - Group Enabled
**                          - 0 - Group held in reset
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpGfpGrpInit(struct file *fileHndl, UINT4 slice, UINT4 grpId,
                           UINT4 pFcsStrip, UINT4 enable)
{
    UINT4 regAddr = 0;
    UINT4 gfpFrmRst = 0;
    UINT4 gfpTraRst = 0;
    UINT4 rxdpPageAddr0Word0 = 0;
    UINT4 rxdpPageAddr0Word1 = 0;
    UINT4 modeOffset = 0;
    UINT4 modeMask = 0;
    UINT4 revBitOffset = 0;
    UINT4 revBitMask = 0;
    UINT4 procChanId = 0;

    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_SCHD_GRP_CTXT *pGrpCtxt;

    /* Configure Indirect Variable settings */
    /* RXDP indirect setting initialization */
    UINT4 RXDP_PAGE_ADDRESS_00000_CHAN_EN = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_SELF_INITIALIZE = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_MODE = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_DESCRMBL = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_HDLC_PFCS_CHK = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_PFCS_MODE = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_PFCS_INIT = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_PFCS_INV = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_PFCS_REV_PLDBIT = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_PFCS_REV_FCSBIT = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_PFCS_REV_FCSBYTE = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_DATA_REV_IBIT = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_DATA_REV_OBIT = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_FDF_FCS_STRIP = pFcsStrip;
    UINT4 RXDP_PAGE_ADDRESS_00000_DELTA_FDO_STRIP = 0x1;
    UINT4 RXDP_PAGE_ADDRESS_00000_CHEC_ECC_MIN_PLD_CHK = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_THEC_CHK_MAX_PLD_CHK = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_THEC_ECC_TRNSP_MODE = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_EHEC_CHK = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_CNT_MAXFL = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_EHEC_ECC = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_DC_BALANCE = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_FDF_STRIP = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_RADP_BYTE = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_MINFL = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_MAXFL = 0xFFFF;

    /* argument checking */
    if ((slice > 1)||(grpId > 0x3F)||(pFcsStrip > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }


    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check that Group is not currently active */
    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Grab pointer to GFP Slice context*/
    if (slice==0) {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpACtxt);
    } else {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpBCtxt);
    }

    /* Confirm GFP Slice Context is valid */
    if(pGfpCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Grab the scheduler data for the GFP SCBS being configured */
    pSchd = &(pGfpCtxt->schd);

    /* Grab the group data for the GFP SCBS being configured */
    pGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)(pSchd->pScbsGrpCtxt + grpId);

    if ((pGrpCtxt->startAddr != pGrpCtxt->endAddr) ||
            (pGrpCtxt->startAddr != HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW))
        return HYPHY20G_ERR_INVALID_PRECONFIG;


    /* Tx Direction Group Config */
    /* TXDP64 Group Reset */
    regAddr = HYPHY20G_GFP_FRM_TXDP_REG_GRP_RST_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
    if (result) return result;

    /* TXDP XOFF */
    regAddr = HYPHY20G_GFP_FRM_TXDP_REG_TX_DIS_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
        if (result) return result;
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
        if (result) return result;
    }
    /* TXDP CMF Processor Channel ID */
    regAddr = HYPHY20G_GFP_FRM_TXDP_REG_PROC_CHNL_ID_OFFSET(grpId/4);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &procChanId);
    if (result) return result;

    modeOffset = 8*(grpId%4) + 7;
    modeMask = 0x1 << modeOffset;
    result = hyPhy20gVarFieldWrite(&procChanId, modeMask, 0);
    if (result) return result;

    revBitOffset = 8*(grpId%4) + 6;
    revBitMask = 0x1 << revBitOffset;
    result = hyPhy20gVarFieldWrite(&procChanId, revBitMask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, procChanId);
    if (result) return result;

    /* TXGFPT Group Reset */
    regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_GRP_RST_GROUPS_31_TO_0;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    if (grpId > 31)
        regAddr = regAddr + 4;
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
        if (result) return result;
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
        if (result) return result;
    }


    /* RXDP64 Config */
    /* Set CHAN_EN to '0' */
    result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x0, grpId, 0x0, 0x0);
    if (result) return result;

    /* Set VCAT_CHN_RST */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_VCAT_GRP_RST_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
    if (result) return result;
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
        if (result) return result;
    }

    /* Configure PAGE_ADDRESS_00000 indirect registers */
    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_GRP_EN_MSK,
        RXDP_PAGE_ADDRESS_00000_CHAN_EN);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_GRP_SELF_INIT_MSK,
        RXDP_PAGE_ADDRESS_00000_SELF_INITIALIZE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_MODE_MSK,
        RXDP_PAGE_ADDRESS_00000_MODE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_DESCRMBL_MSK,
        RXDP_PAGE_ADDRESS_00000_DESCRMBL);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_HDLC_PFCS_CHK_MSK,
        RXDP_PAGE_ADDRESS_00000_HDLC_PFCS_CHK);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_PFCS_MODE_MSK,
        RXDP_PAGE_ADDRESS_00000_PFCS_MODE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_PFCS_INIT_MSK,
        RXDP_PAGE_ADDRESS_00000_PFCS_INIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_PFCS_INV_MSK,
        RXDP_PAGE_ADDRESS_00000_PFCS_INV);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_PFCS_REV_PLDBIT_MSK,
        RXDP_PAGE_ADDRESS_00000_PFCS_REV_PLDBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_PFCS_REV_FCSBIT_MSK,
        RXDP_PAGE_ADDRESS_00000_PFCS_REV_FCSBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_PFCS_REV_FCSBYTE_MSK,
        RXDP_PAGE_ADDRESS_00000_PFCS_REV_FCSBYTE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_DATA_REV_IBIT_MSK,
        RXDP_PAGE_ADDRESS_00000_DATA_REV_IBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_DATA_REV_OBIT_MSK,
        RXDP_PAGE_ADDRESS_00000_DATA_REV_OBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_FDF_FCS_STRIP_MSK,
        RXDP_PAGE_ADDRESS_00000_FDF_FCS_STRIP);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_DELTA_FDO_STRIP_MSK,
        RXDP_PAGE_ADDRESS_00000_DELTA_FDO_STRIP);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_CHEC_ECC_MIN_PLD_CHK_MSK,
        RXDP_PAGE_ADDRESS_00000_CHEC_ECC_MIN_PLD_CHK);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_THEC_CHK_MAX_PLD_CHK_MSK,
        RXDP_PAGE_ADDRESS_00000_THEC_CHK_MAX_PLD_CHK);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_THEC_ECC_TRNSP_MODE_MSK,
        RXDP_PAGE_ADDRESS_00000_THEC_ECC_TRNSP_MODE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_EHEC_CHK_MSK,
        RXDP_PAGE_ADDRESS_00000_EHEC_CHK);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_CNT_MAXFL_MSK,
        RXDP_PAGE_ADDRESS_00000_CNT_MAXFL);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_EHEC_ECC_MSK,
        RXDP_PAGE_ADDRESS_00000_EHEC_ECC);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_DC_BALANCE_MSK,
        RXDP_PAGE_ADDRESS_00000_DC_BALANCE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_FDF_STRIP_MSK,
        RXDP_PAGE_ADDRESS_00000_FDF_STRIP);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word1,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_RADP_BYTE_MSK,
        RXDP_PAGE_ADDRESS_00000_RADP_BYTE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word1,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_MINFL_MSK,
        RXDP_PAGE_ADDRESS_00000_MINFL);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word1,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_MAXFL_MSK,
        RXDP_PAGE_ADDRESS_00000_MAXFL);
    if (result) return result;

    result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x0, grpId, rxdpPageAddr0Word0,
                                    rxdpPageAddr0Word1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpGfpGrpInit */



/*******************************************************************************
**
**  hyPhy20gGfpFChInitRx
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Initializes and configures the Rx portion of a GFP-F channel
**                  within the GFP subsystem.   By default, extension header is
**                  not enabled.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - CPB ID of the channel being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              pauseMode   - Controls handling of received Pause Frames
**                          - 0 - Pause time is NOT extracted from pause frames.
**                                All pause frames are written into CPB.
**                          - 1 - Pause time is extracted from pause frames
**                                and passed to Tx.  All pause frames are
**                                written into CPB
**                          - 2 - Pause time is extracted from pause frames
**                                and passed to Tx.  Pause frames are NOT
**                                written into CPB
**                          - 3 - Pause time is NOT extracted from pause frames.
**                                Pause frames are NOT written into CPB.
**             fdfStripMode - Controls which fields are stripped from frames
**                            destined for the FDF Fifo.
**                          - 0 - strip nothing
**                          - 1 - strip Type Header
**                          - 2 - strip Extension Header, Type Header
**             fdoHdrStrip  - Controls which headers are stripped from frames
**                            destined for the FDO interface (i.e. RXGFPT)
**                          - 0 - strip nothing
**                          - 1 - strip Type Header
**                          - 2 - strip Extension Header, Type Header
**             fdoFcsMode   - Controls which type of FCS checking and FCS
**                            stripping to perform on incoming frames destined
**                            for the FDO interface (i.e. RXGFPT)
**                          - 0 - No FCS checking, strip nothing
**                          - 1 - N/A, Don't use;
**                          - 2 - pFCS checking, strip nothing
**                          - 3 - pFCS checking, strip pFCS
**                          - 4 - eFCS checking (ingoring preamble & SFD)
**                                strip nothing
**                          - 5 - eFCS checking (ingoring preamble & SFD)
**                                strip eFCS
**                          - 6 - eFCS checking (including preamble & SFD)
**                                strip nothing
**                          - 7 - eFCS checking (including preamble & SFD)
**                                strip eFCS
**          preambleSize    - For ethernet traffic, selects size of preamble
**                            (Can be left at '0' for non-ethernet traffic)
**                          - 0 - Three bytes of preamble, 1 byte of SFD
**                          - 1 - Seven bytes of preamble, 1 byte of SFD
**                          - 2 - Eleven bytes of preamble, 1 byte of SFD
**                          - 3 - Thirteen bytes of preamble, 1 byte of SFD
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpFChInitRx(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                          UINT4 pauseMode, UINT4 fdfStripMode, UINT4 fdoHdrStrip,
                          UINT4 fdoFcsMode, UINT4 preambleSize)
{
    UINT4 regAddr = 0;
    UINT4 gfpFrmRst = 0;
    UINT4 gfpTraRst = 0;
    UINT4 rxdpPageAddr32Word0 = 0;
    UINT4 rxdpPageAddr32Word1 = 0;
    INT4 result;

    /* Configure Indirect Variable settings */
    /* RXDP indirect setting initialization */

    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_SELF_INITIALIZE = 0;
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_ENABLE = 0x0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_FILTER = 0x0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_FDF_FORMAT = 0x0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_FDO_FORMAT = 0x0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_EFCS_STRIP = 0x0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_PREAMBLE_SFD_MODE = preambleSize;
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_CNT_UDF_FCS = 0;
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_PU_MIS_DRP_CNT_EN = 0;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(pauseMode > 3)||(fdfStripMode > 2)||
        (fdoHdrStrip > 2)||(fdoFcsMode > 7)||(preambleSize > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Configure RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_ENABLE variable */
    if ((pauseMode == 1) || (pauseMode == 2)){
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_ENABLE = 0x1;
    } else {
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_ENABLE = 0x0;
    }

    /* Configure RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_FILTER variable */
    if (pauseMode > 1) {
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_FILTER = 0x1;
    } else {
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_FILTER = 0x0;
    }


    /* Configure RXDP_PAGE_ADDRESS_10000_CPBID_FDF_FORMAT variable */
    if (fdfStripMode == 0) {
        RXDP_PAGE_ADDRESS_10000_CPBID_FDF_FORMAT = 0x2;
    } else if ((fdfStripMode == 1)) {
        RXDP_PAGE_ADDRESS_10000_CPBID_FDF_FORMAT = 0x1;
    } else {
        RXDP_PAGE_ADDRESS_10000_CPBID_FDF_FORMAT = 0x0;
    }

    /* Configure RXDP_PAGE_ADDRESS_10000_CPBID_FDO_FORMAT variable */
    if (fdoHdrStrip == 0) {
        RXDP_PAGE_ADDRESS_10000_CPBID_FDO_FORMAT = 0x2;
    } else if (fdoHdrStrip == 1)  {
        RXDP_PAGE_ADDRESS_10000_CPBID_FDO_FORMAT = 0x1;
    } else {
        RXDP_PAGE_ADDRESS_10000_CPBID_FDO_FORMAT = 0x0;
    }

    /* Configure RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE and
        RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP variables */
    switch (fdoFcsMode) {
        case 0:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x0;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x0;
            break;
        case 2:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x1;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x0;
            break;
        case 3:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x1;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x1;
            break;
        case 4:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x2;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x0;
            break;
        case 5:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x2;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x1;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_EFCS_STRIP = 0x1;
            break;
        case 6:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x3;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x0;
            break;
        case 7:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x3;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x1;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_EFCS_STRIP = 0x1;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }



    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* RxGFPT Config */
    /* Set CHAN_EN to '0' */
    regAddr = HYPHY20G_GFP_TRA_RXGFPT_REG_CHNL_EN_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 0);
    if (result) return result;

    /* Toggle CHAN_RESET */
    regAddr = HYPHY20G_GFP_TRA_RXGFPT_REG_CHNL_RST_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 1);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 0);
    if (result) return result;

    /* Configure GFP_INTF_MODE to '1' for GFP-F mode */
    regAddr = HYPHY20G_GFP_TRA_RXGFPT_REG_IF_MD_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 1);
    if (result) return result;

    /* RxDP64 Config */
    /* Toggle CPBID_CHN_RST */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CPBID_CHNL_RST_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 1);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 0);
    if (result) return result;

    /* Configure PAGE_ADDRESS_10000 indirect registers */
    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_CHN_SELF_INIT_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_SELF_INITIALIZE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_PAUSE_ENABLE_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_ENABLE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_PAUSE_FILTER_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_FILTER);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_FDF_FORMAT_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_FDF_FORMAT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_FDO_FORMAT_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_FDO_FORMAT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_GFP_FCS_CHK_MODE_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_GFP_FDO_PFCS_STRIP_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_PREAMBLE_SFD_MODE_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_PREAMBLE_SFD_MODE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_CNT_UDF_FCS_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_CNT_UDF_FCS);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_PU_MIS_DRP_CNT_EN_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_PU_MIS_DRP_CNT_EN);
    if (result) return result;
        
    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word1,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_GFP_FDO_EFCS_STRIP_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_EFCS_STRIP);
    if (result) return result;
    
    result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x20, cpbId, 
                                          rxdpPageAddr32Word0,
                                          rxdpPageAddr32Word1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpFChInitRx */


/*******************************************************************************
**
**  hyPhy20gGfpTChInitRx
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Initializes and configures the Rx portion of a GFP-T channel
**                  within the GFP subsystem.   By default, extension header is
**                  not enabled.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - CPBID of the channel being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              alignment   - 0 - Aribtrary Alignment Mode
**                          - 1 - Frame Alignment Mode
**              geClient    - 0 - Any client other than GbE
**                          - 1 - GbE client
**              pauseMode   - Controls handling of received Pause Frames
**                          - 0 - Pause time is NOT extracted from pause frames.
**                                All pause frames are written into CPB.
**                          - 1 - Pause time is extracted from pause frames
**                                and passed to Tx.  All pause frames are
**                                written into CPB
**                          - 2 - Pause time is extracted from pause frames
**                                and passed to Tx.  Pause frames are NOT
**                                written into CPB
**                          - 3 - Pause time is NOT extracted from pause frames.
**                                Pause frames are NOT written into CPB.
**             fdfStripMode - Controls which fields are stripped from frames
**                            destined for the FDF Fifo.
**                          - 0 - strip nothing
**                          - 1 - strip Type Header
**                          - 2 - strip Extension Header, Type Header
**             fdoHdrStrip  - Controls which headers are stripped from frames
**                            destined for the FDO interface (i.e. RXGFPT)
**                          - 0 - strip nothing
**                          - 1 - strip Type Header
**                          - 2 - strip Extension Header, Type Header
**             fdoFcsMode   - Controls which type of FCS checking and FCS
**                            stripping to perform on incoming frames destined
**                            for the FDO interface (i.e. RXGFPT)
**                          - 0 - No FCS checking, strip nothing
**                          - 1 - N/A. Don't use.
**                          - 2 - pFCS checking, strip nothing
**                          - 3 - pFCS checking, strip pFCS
**                          - 4 - eFCS checking (ingoring preamble & SFD)
**                                strip nothing
**                          - 5 - eFCS checking (ingoring preamble & SFD)
**                                strip eFCS
**                          - 6 - eFCS checking (including preamble & SFD)
**                                strip nothing
**                          - 7 - eFCS checking (including preamble & SFD)
**                                strip eFCS
**          preambleSize    - For ethernet traffic, selects size of preamble
**                            (Can be left at '0' for non-ethernet traffic)
**                          - 0 - Three bytes of preamble, 1 byte of SFD
**                          - 1 - Seven bytes of preamble, 1 byte of SFD
**                          - 2 - Eleven bytes of preamble, 1 byte of SFD
**                          - 3 - Thirteen bytes of preamble, 1 byte of SFD
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpTChInitRx(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 alignment, UINT4 geClient,
                                 UINT4 pauseMode, UINT4 fdfStripMode,
                                 UINT4 fdoHdrStrip, UINT4 fdoFcsMode,
                                 UINT4 preambleSize)
{
    UINT4 regAddr = 0;
    UINT4 gfpFrmRst = 0;
    UINT4 gfpTraRst = 0;
    UINT4 rxgfptIndChanCfgReg0 = 0;
    UINT4 rxdpPageAddr32Word0 = 0;
    INT4 result;


    /* Configure Indirect Variable settings */
    /* RxDP64 indirect setting initialization */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_SELF_INITIALIZE = 0;
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_ENABLE = 0x0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_FILTER = 0x0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_FDF_FORMAT = 0x0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_FDO_FORMAT = 0x0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_PREAMBLE_SFD_MODE = preambleSize;
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_CNT_UDF_FCS = 0;
    UINT4 RXDP_PAGE_ADDRESS_10000_CPBID_PU_MIS_DRP_CNT_EN = 0;

    /* RxGFPT indirect setting initialization */
    UINT4 RXGFPT_IND_CHAN_CONFIG_AA_MODE_WATCH = geClient;
    UINT4 RXGFPT_IND_CHAN_CONFIG_ERROR_BEHAVE_CONFIG = 0;
    UINT4 RXGFPT_IND_CHAN_CONFIG_AA_MODE_PACKET_LENGTH = 0x10;
    UINT4 RXGFPT_IND_CHAN_CONFIG_CRC_ERROR_CORRECTION = 1;
    UINT4 RXGFPT_IND_CHAN_CONFIG_GFPT_ALIGNMENT_MODE = 1;
    if (alignment) {
        RXGFPT_IND_CHAN_CONFIG_GFPT_ALIGNMENT_MODE = 0;
    }

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(alignment > 1)||(geClient > 1)||(pauseMode > 3)||(fdfStripMode > 2)||(fdoHdrStrip > 2)||(fdoFcsMode > 7)||(preambleSize > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Configure RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_ENABLE variable */
    if ((pauseMode == 1) || (pauseMode == 2)){
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_ENABLE = 0x1;
    } else {
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_ENABLE = 0x0;
    }

    /* Configure RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_FILTER variable */
    if (pauseMode > 1) {
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_FILTER = 0x1;
    } else {
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_FILTER = 0x0;
    }

    /* Configure RXDP_PAGE_ADDRESS_10000_CPBID_FDF_FORMAT variable */
    if (fdfStripMode == 0){
        RXDP_PAGE_ADDRESS_10000_CPBID_FDF_FORMAT = 0x2;
    } else if ((fdfStripMode == 1)) {
        RXDP_PAGE_ADDRESS_10000_CPBID_FDF_FORMAT = 0x1;
    } else {
        RXDP_PAGE_ADDRESS_10000_CPBID_FDF_FORMAT = 0x0;
    }

    /* Configure RXDP_PAGE_ADDRESS_10000_CPBID_FDO_FORMAT variable */
    if (fdoHdrStrip == 0) {
        RXDP_PAGE_ADDRESS_10000_CPBID_FDO_FORMAT = 0x2;
    } else if (fdoHdrStrip == 1)  {
        RXDP_PAGE_ADDRESS_10000_CPBID_FDO_FORMAT = 0x1;
    } else {
        RXDP_PAGE_ADDRESS_10000_CPBID_FDO_FORMAT = 0x0;
    }

    /* Configure RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE and
        RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP variables */
    switch (fdoFcsMode) {
        case 0:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x0;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x0;
            break;
        case 2:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x1;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x0;
            break;
        case 3:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x1;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x1;
            break;
        case 4:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x2;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x0;
            break;
        case 5:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x2;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x1;
            break;
        case 6:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x3;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x0;
            break;
        case 7:
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE = 0x3;
            RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP = 0x1;
            break;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* RxGFPT Config */
    /* Set CHAN_EN to '0' */
    regAddr = HYPHY20G_GFP_TRA_RXGFPT_REG_CHNL_EN_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 0);
    if (result) return result;

    /* Toggle CHAN_RESET */
    regAddr = HYPHY20G_GFP_TRA_RXGFPT_REG_CHNL_RST_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 1);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 0);
    if (result) return result;

    /* Configure GFP_INTF_MODE to '0' for GFP-T mode */
    regAddr = HYPHY20G_GFP_TRA_RXGFPT_REG_IF_MD_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 0);
    if (result) return result;

    /* Configure IND_CHAN_CONFIG indirect registers */
    result = hyPhy20gVarFieldWrite(&rxgfptIndChanCfgReg0,
        HYPHY20G_GFP_TRA_RXGFPT_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_AA_MODE_WATCH_MSK,
        RXGFPT_IND_CHAN_CONFIG_AA_MODE_WATCH);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxgfptIndChanCfgReg0,
        HYPHY20G_GFP_TRA_RXGFPT_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_ERROR_BEHAVE_CONFIG_MSK,
        RXGFPT_IND_CHAN_CONFIG_ERROR_BEHAVE_CONFIG);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxgfptIndChanCfgReg0,
        HYPHY20G_GFP_TRA_RXGFPT_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_AA_MODE_PACKET_LENGTH_MSK,
        RXGFPT_IND_CHAN_CONFIG_AA_MODE_PACKET_LENGTH);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxgfptIndChanCfgReg0,
        HYPHY20G_GFP_TRA_RXGFPT_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_CRC_ERROR_CORRECTION_MSK,
        RXGFPT_IND_CHAN_CONFIG_CRC_ERROR_CORRECTION);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxgfptIndChanCfgReg0,
        HYPHY20G_GFP_TRA_RXGFPT_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_GFPT_ALIGNMENT_MODE_MSK,
        RXGFPT_IND_CHAN_CONFIG_GFPT_ALIGNMENT_MODE);
    if (result) return result;

    result = gfptHyPhy20gRXGFPTChanIndWrite(fileHndl, slice, cpbId, rxgfptIndChanCfgReg0);
    if (result) return result;

    /* RXDP64 Config */
    /* Toggle CPBID_CHN_RST */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CPBID_CHNL_RST_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 1);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 0);
    if (result) return result;

    /* Configure PAGE_ADDRESS_10000 indirect registers */
    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_CHN_SELF_INIT_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_SELF_INITIALIZE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_PAUSE_ENABLE_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_ENABLE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_PAUSE_FILTER_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_PAUSE_FILTER);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_FDF_FORMAT_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_FDF_FORMAT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_FDO_FORMAT_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_FDO_FORMAT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_GFP_FCS_CHK_MODE_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FCS_CHK_MODE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_GFP_FDO_PFCS_STRIP_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_GFP_FDO_PFCS_STRIP);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_PREAMBLE_SFD_MODE_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_PREAMBLE_SFD_MODE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_CNT_UDF_FCS_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_CNT_UDF_FCS);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr32Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_100000_IBIT_CPBID_BASED_COM_CFG_PU_MIS_DRP_CNT_EN_MSK,
        RXDP_PAGE_ADDRESS_10000_CPBID_PU_MIS_DRP_CNT_EN);
    if (result) return result;

    result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x20, cpbId, rxdpPageAddr32Word0,
                                    0x0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpTChInitRx */




/*******************************************************************************
**
**  hyPhy20gGfpFChInitTx
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Initializes and configures the Tx portion of a GFP-F channel
**                  within the GFP subsystem. By default, extension header is
**                  not enabled.
**
**                  In Rev B device, the value '2' of gSup43Mode parameter is
**                  used for the MAC terminated G.Sup43 mapping, in which TXDP
**                  in GFP subsystem is connected to HSCI line side through
**                  the MAC_GSUP43 block in ENET_COM subsystem.
**
**                  For Rev B device, the value '1' of 
**                  gSup43Mode parameter is also used for G.Sup43 Clause 7.3 
**                  Terminating application bypass MAC mode, in which TXDP in 
**                  GFP subsystem is connected to the corresponding CPB channel.
**
**                  When using gSup43Mode set to 1 or 2 for 7.3 modes of
**                  operation, the following applies:
**                  - Bit 8 of upi field should be set to 1.  
**                  - If gSup43Mode is set to 2 the UPI source is specified 
**                  using hyPhy20gEnetComGsup43C73ModeCfg API.  
**                  - If gSup43Mode is set to 1 and the data source is from 
**                  HSCI, UPI is specified using hyPhy20gHsciGsup43C73ModeCfg 
**                  API.  
**                  - If gSup43Mode is set to 1 and the data source is from PIF
**                  the source of UPI is extracted from UPI data contained in
**                  the TYPE field of PIF data transfers.
** 
**
**  INPUTS:     *fileHndl  - base address
**              slice      - 0 - Slice 'A'
**                         - 1 - Slice 'B'
**              cpbId      - CPBID of the channel being configured
**                         - valid values are 0x0 - 0x3F (0-63)
**              upi        - Bit 8 UPI source
**                            0 - TXDP
**                            1 - Upstream (received in length field from PIF or
**                                generated in GSUP43 block for 7.3 application)
**                           Bit  7:0 UPI Value
**                            - If upi bit 8 is 0, the value 0x0 to 0xFF 
**                              generated in UPI field of GFP frame.
**                            - If upi bit 8 is 1, the field setting does not 
**                              impact the UPI field of the generated GFP frame.
**              gSup43Mode - 2 - G.Sup43 Clause 6.2 Terminating application or
**                               G.Sup43 Clause 7.3 Terminating application
**                               through MAC mode
**                         - 1 - G.Sup43 Clause 7.3 Terminating application 
**                               bypass MAC or
**                               G.Sup43 Clause 7.3 Transponder application or
**                               G.Sup43 Clause 6.2 Transponder application                           
**                         - 0 - Any Other Mapping Mode in which data directly
**                               come from a CPB channel
**              pfcsMode   - Controls options for Payload FCS Insertion
**                         - 0 - No payload FCS insertion
**                         - 1 - Payload FCS insertion enabled
**                         - 2 - Calculate & insert eFCS after 4 received bytes
**                         - 3 - Calculate & insert eFCS after 8 received bytes
**                         - 4 - Calculate & insert eFCS after 12 received bytes
**                         - 5 - Calculate & insert eFCS after 16 received bytes
**                         - 6 - Calculate & insert eFCS for all bytes
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gGfpFChInitTx(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                          UINT4 upi, UINT4 gSup43Mode, UINT4 pfcsMode)
{
    UINT4 regAddr = 0;
    UINT4 gfpFrmRst = 0;
    UINT4 gfpTraRst = 0;
    UINT4 txgfptRpChanCfgReg0 = 0;
    UINT4 txdpIndChanCfgReg0 = 0;
    UINT4 txdpIndChanCfgReg1 = 0;
    UINT4 txdpIndChanCfgReg2 = 0;
    UINT4 cpbGfpConfig;
    UINT4 mask;
    UINT4 offset1;
    UINT4 offset2;
    UINT4 upiVal; 
    UINT4 upiSrc;
    INT4 result;

    /* Configure Indirect Variable settings */
    /* TxGFPT indirect setting initialization */
    UINT4 TXGFPT_RP_CHAN_CONFIG_NUM_SB = 0x0;
    UINT4 TXGFPT_RP_CHAN_CONFIG_MODE = 0x2;

    /* TXDP indirect setting initialization */
    UINT4 TXDP_IND_CHAN_CONFIG_UPI_SEL = 0x0; /* will be configured below */
    UINT4 TXDP_IND_CHAN_CONFIG_FCS_INS_POS = 0x0; /* will be configured below */
    UINT4 TXDP_IND_CHAN_CONFIG_TYPE_ADCT_DAT = 0x0; /* will be configured below */
    UINT4 TXDP_IND_CHAN_CONFIG_EHDR_EN = 0;    
    UINT4 TXDP_IND_CHAN_CONFIG_ABORT_TYPE = 0;
    UINT4 TXDP_IND_CHAN_CONFIG_FCS_INS = 0x0; /* will be configured below */
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_MODE = 1;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_INIT = 1;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_INV = 1;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_CRPT_INV = 1;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_REV_IBIT = 0;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_REV_OBIT = 0;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_REV_OBYTE = 0;
    UINT4 TXDP_IND_CHAN_CONFIG_FLAG_IDLE = 0;
    UINT4 TXDP_IND_CHAN_CONFIG_FDI_DATA_REV = 0;
    
    /* Configure TXDP_IND_CHAN_CONFIG_UPI_SEL variable */
    upiSrc = upi & 0x100;
    upiSrc = upiSrc >> 8;
    if (upiSrc == 1) { /* UPI source is from upstream */
        TXDP_IND_CHAN_CONFIG_UPI_SEL = 1;
    } else { /* UPI source is from TXDP */
        TXDP_IND_CHAN_CONFIG_UPI_SEL = 0;
    }

    /* Extract UPI value from upi argument */
    upiVal = upi & 0xFF;
    
    /* Configure TXDP_IND_CHAN_CONFIG_TYPE_ADCT_DAT variable if source of UPI
       is from TXDP */
    if (TXDP_IND_CHAN_CONFIG_UPI_SEL == 0) { 
        if (pfcsMode == 1) {
            TXDP_IND_CHAN_CONFIG_TYPE_ADCT_DAT = 0x1000 + upiVal;
        } else {
            TXDP_IND_CHAN_CONFIG_TYPE_ADCT_DAT = upiVal;
        }
    } else {
        if (pfcsMode == 1) {
            TXDP_IND_CHAN_CONFIG_TYPE_ADCT_DAT = 0x1000;
        }
    }    

    /* Configure TXDP_IND_CHAN_CONFIG_FCS_INS_POS variable */
    if ((pfcsMode > 1) && (pfcsMode < 6)){
        TXDP_IND_CHAN_CONFIG_FCS_INS_POS = pfcsMode - 2;
    }

    /* Configure TXDP_IND_CHAN_CONFIG_FCS_INS variable */
    if (pfcsMode < 2) {
        TXDP_IND_CHAN_CONFIG_FCS_INS = pfcsMode;
    } else if (pfcsMode == 6) {
        TXDP_IND_CHAN_CONFIG_FCS_INS = 0x3;
        TXDP_IND_CHAN_CONFIG_PFCS_REV_IBIT = 0x1;
        TXDP_IND_CHAN_CONFIG_PFCS_REV_OBIT = 0x1;
    } else {
        TXDP_IND_CHAN_CONFIG_FCS_INS = 0x2;
        TXDP_IND_CHAN_CONFIG_PFCS_REV_IBIT = 0x1;
        TXDP_IND_CHAN_CONFIG_PFCS_REV_OBIT = 0x1;
    }

    
    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(upi > 0x1FF)||(gSup43Mode > 2)||
        (pfcsMode > 0x6)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* TXDP64 Config */
    /* Configure IND_CHAN_CONFIG indirect registers */
    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg0,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_UPI_SEL_MSK,
        TXDP_IND_CHAN_CONFIG_UPI_SEL);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg0,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_FCS_INS_POS_MSK,
        TXDP_IND_CHAN_CONFIG_FCS_INS_POS);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg1,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_TYPE_ADCT_DAT_MSK,
        TXDP_IND_CHAN_CONFIG_TYPE_ADCT_DAT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_EHDR_EN_MSK,
        TXDP_IND_CHAN_CONFIG_EHDR_EN);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_ABORT_TYPE_MSK,
        TXDP_IND_CHAN_CONFIG_ABORT_TYPE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_FCS_INS_MSK,
        TXDP_IND_CHAN_CONFIG_FCS_INS);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_MODE_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_MODE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_INIT_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_INIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_INV_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_INV);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_CRPT_INV_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_CRPT_INV);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_REV_IBIT_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_REV_IBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_REV_OBIT_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_REV_OBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_REV_OBYTE_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_REV_OBYTE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_FLAG_IDLE_MSK,
        TXDP_IND_CHAN_CONFIG_FLAG_IDLE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_FDI_DATA_REV_MSK,
        TXDP_IND_CHAN_CONFIG_FDI_DATA_REV);
    if (result) return result;
    
    result = gfpfHyPhy20gTXDPChanIndWrite(fileHndl, slice, cpbId, txdpIndChanCfgReg0,
                                    txdpIndChanCfgReg1, txdpIndChanCfgReg2);
    if (result) return result;

    /* Configure RP_CHAN_CONFIG indirect registers */
    result = hyPhy20gVarFieldWrite(&txgfptRpChanCfgReg0,
        HYPHY20G_GFP_TRA_TXGFPT_TOP_RP_CHAN_CFG_IBIT_REQ_PROC_CHNL_CFG_NUM_SB_MSK,
        TXGFPT_RP_CHAN_CONFIG_NUM_SB);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txgfptRpChanCfgReg0,
        HYPHY20G_GFP_TRA_TXGFPT_TOP_RP_CHAN_CFG_IBIT_REQ_PROC_CHNL_CFG_MODE_MSK,
        TXGFPT_RP_CHAN_CONFIG_MODE);
    if (result) return result;

    result = gfptHyPhy20gTXGFPTReqProChanIndWrite(fileHndl, slice, cpbId, txgfptRpChanCfgReg0, 0x0);
    if (result) return result;

    /* Configure GFP_RD_THRESH and GFP_RD_VALID_SQL in CPB */
    regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &cpbGfpConfig);

    mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
    result = hyPhy20gVarFieldWrite(&cpbGfpConfig, mask, 0x1A);
    if (result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_VALID_SQL_MSK;
    result = hyPhy20gVarFieldWrite(&cpbGfpConfig, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, cpbGfpConfig);
    if (result) return result;

    /* TXGFPT Config */
    regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_CFG;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    offset1 = HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_CFG_ENET_DATAPATH_EN_OFF;
    offset2 = HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_CFG_REQ_PROC_BYPASS_OFF;
    if (gSup43Mode == 2) { /* source of data is from ENET_COM */
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset1, 1);
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset2, 1);
    } else { /* data is from CPB */
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset1, 0);
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset2, 0);
    }
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpFChInitTx */




/*******************************************************************************
**
**  hyPhy20gGfpTChInitTx
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Initializes and configures the Tx portion of a GFP-T channel
**                  within the GFP subsystem.   By default, extension header is
**                  not enabled.
**
**  INPUTS:     *fileHndl  - base address
**              slice      - 0 - Slice 'A'
**                         - 1 - Slice 'B'
**              cpbId      - CPBID of the channel being configured
**                         - valid values are 0x0 - 0x3F (0-63)
**              numSb      - Number of superblocks per GFP-T frame
**                         - Valid values are 0x1 - 0x7F (1-127)
**              alignment  - 0 - Aribtrary Alignment Mode
**                         - 1 - Frame Alignment Mode
**              upi        - UPI value to use for GFP data frames
**                         - Valid values are 0x0 - 0xFF 
**              pfcsMode   - Controls options for Payload FCS Insertion
**                         - 0 - No payload FCS insertion
**                         - 1 - Payload FCS insertion enabled
**                         - 2 - Calculate & insert eFCS after 4 received bytes
**                         - 3 - Calculate & insert eFCS after 8 received bytes
**                         - 4 - Calculate & insert eFCS after 12 received bytes
**                         - 5 - Calculate & insert eFCS after 16 received bytes
**                         - 6 - Calculate & insert eFCS for all bytes
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpTChInitTx(struct file *fileHndl, UINT4 slice,
                          UINT4 cpbId, UINT4 numSb, UINT4 alignment,
                          UINT4 upi, UINT4 pfcsMode)
{
    UINT4 regAddr = 0;
    UINT4 gfpFrmRst = 0;
    UINT4 gfpTraRst = 0;
    UINT4 txgfptRpChanCfgReg0 = 0;
    UINT4 txgfptRegsChanCfgReg0 = 0;
    UINT4 txdpIndChanCfgReg0 = 0;
    UINT4 txdpIndChanCfgReg1 = 0;
    UINT4 txdpIndChanCfgReg2 = 0;
    UINT4 cpbGfpConfig;
    UINT4 mask;
    INT4 result;


    /* Configure Indirect Variable settings */
    /* TxGFPT indirect setting initialization */
    UINT4 TXGFPT_RP_CHAN_CONFIG_NUM_SB = numSb;
    UINT4 TXGFPT_RP_CHAN_CONFIG_MODE = alignment;

    UINT4 TXGFPT_TXGFPT_REGS_PLI = 67*TXGFPT_RP_CHAN_CONFIG_NUM_SB;

    /* TXDP indirect setting initialization */
    UINT4 TXDP_IND_CHAN_CONFIG_UPI_SEL = 0x0;
    UINT4 TXDP_IND_CHAN_CONFIG_FCS_INS_POS = 0; /* will be configured below */
    UINT4 TXDP_IND_CHAN_CONFIG_TYPE_ADCT_DAT = 0x0; /* will be configured below */
    UINT4 TXDP_IND_CHAN_CONFIG_EHDR_EN = 0;  
    UINT4 TXDP_IND_CHAN_CONFIG_ABORT_TYPE = 0;
    UINT4 TXDP_IND_CHAN_CONFIG_FCS_INS = 0; /* will be configured below */
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_MODE = 1;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_INIT = 1;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_INV = 1;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_CRPT_INV = 1;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_REV_IBIT = 0;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_REV_OBIT = 0;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_REV_OBYTE = 0;
    UINT4 TXDP_IND_CHAN_CONFIG_FLAG_IDLE = 0;
    UINT4 TXDP_IND_CHAN_CONFIG_FDI_DATA_REV = 0;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(numSb > 0x7F)||(alignment > 1)||
        (upi > 0xFF)||(pfcsMode > 6)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Configure TXDP_IND_CHAN_CONFIG_TYPE_ADCT_DAT variable */
    if (pfcsMode == 1) {
        TXDP_IND_CHAN_CONFIG_TYPE_ADCT_DAT = 0x1000 + upi;
    } else {
        TXDP_IND_CHAN_CONFIG_TYPE_ADCT_DAT = upi;
    }

    /* Configure TXDP_IND_CHAN_CONFIG_FCS_INS_POS variable */
    if ((pfcsMode > 1) && (pfcsMode < 6)){
        TXDP_IND_CHAN_CONFIG_FCS_INS_POS = pfcsMode - 2;
    }

    /* Configure TXDP_IND_CHAN_CONFIG_FCS_INS variable */
    if (pfcsMode < 2) {
        TXDP_IND_CHAN_CONFIG_FCS_INS = pfcsMode;
    } else if (pfcsMode == 6) {
        TXDP_IND_CHAN_CONFIG_FCS_INS = 0x3;
    } else {
        TXDP_IND_CHAN_CONFIG_FCS_INS = 0x2;
    }



    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure RP_CHAN_CONFIG indirect registers */
    result = hyPhy20gVarFieldWrite(&txgfptRpChanCfgReg0,
        HYPHY20G_GFP_TRA_TXGFPT_TOP_RP_CHAN_CFG_IBIT_REQ_PROC_CHNL_CFG_NUM_SB_MSK,
        TXGFPT_RP_CHAN_CONFIG_NUM_SB);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txgfptRpChanCfgReg0,
        HYPHY20G_GFP_TRA_TXGFPT_TOP_RP_CHAN_CFG_IBIT_REQ_PROC_CHNL_CFG_MODE_MSK,
        TXGFPT_RP_CHAN_CONFIG_MODE);
    if (result) return result;

    result = gfptHyPhy20gTXGFPTReqProChanIndWrite(fileHndl, slice, cpbId, txgfptRpChanCfgReg0, 0x0);
    if (result) return result;

    /* Configure TXGFPT_REGS indirect registers */
    result = hyPhy20gVarFieldWrite(&txgfptRegsChanCfgReg0,
        HYPHY20G_GFP_TRA_TXGFPT_TOP_CHAN_CONFIG_IBIT_CHNL_CFG_PLI_MSK,
        TXGFPT_TXGFPT_REGS_PLI);
    if (result) return result;

    result = gfptHyPhy20gTXGFPTChanIndWrite(fileHndl, slice, cpbId, txgfptRegsChanCfgReg0, 0x0);
    if (result) return result;

    /* TXDP64 Config */
    /* Configure IND_CHAN_CONFIG indirect registers */
    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg0,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_UPI_SEL_MSK,
        TXDP_IND_CHAN_CONFIG_UPI_SEL);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg0,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_FCS_INS_POS_MSK,
        TXDP_IND_CHAN_CONFIG_FCS_INS_POS);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg1,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_TYPE_ADCT_DAT_MSK,
        TXDP_IND_CHAN_CONFIG_TYPE_ADCT_DAT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_EHDR_EN_MSK,
        TXDP_IND_CHAN_CONFIG_EHDR_EN);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_ABORT_TYPE_MSK,
        TXDP_IND_CHAN_CONFIG_ABORT_TYPE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_FCS_INS_MSK,
        TXDP_IND_CHAN_CONFIG_FCS_INS);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_MODE_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_MODE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_INIT_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_INIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_INV_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_INV);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_CRPT_INV_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_CRPT_INV);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_REV_IBIT_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_REV_IBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_REV_OBIT_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_REV_OBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_REV_OBYTE_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_REV_OBYTE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_FLAG_IDLE_MSK,
        TXDP_IND_CHAN_CONFIG_FLAG_IDLE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_FDI_DATA_REV_MSK,
        TXDP_IND_CHAN_CONFIG_FDI_DATA_REV);
    if (result) return result;

    result = gfpfHyPhy20gTXDPChanIndWrite(fileHndl, slice, cpbId, txdpIndChanCfgReg0,
                                    txdpIndChanCfgReg1, txdpIndChanCfgReg2);
    if (result) return result;

    /* Configure GFP_RD_THRESH and GFP_RD_VALID_SQL in CPB */
    regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &cpbGfpConfig);

    mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
    result = hyPhy20gVarFieldWrite(&cpbGfpConfig, mask, 0x1A);
    if (result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_VALID_SQL_MSK;
    result = hyPhy20gVarFieldWrite(&cpbGfpConfig, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, cpbGfpConfig);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpTChInitTx */




/*******************************************************************************
**
**  hyPhy20gGfpHdlcGrpChInitTx
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Initializes and configures the Tx group and channel
**                  parameters of an HDLC link within the GFP subsystem.  (Only
**                  available for slice 'A' of the GFP subsystem, with SONET/SDH
**                  mapping).  By default, prepend bytes are not enabled.  This
**                  procedure should only be called if there are no channels
**                  currently assigned to this group.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B' (Only available if fullRate = 1)
**              fullRate    - 0 - non-OC-192c traffic.
**                          - 1 - OC-192c POS traffic (Vcat subsystem will be
**                              bypassed.  Must use grpId of 0)
**              grpId       - Group ID of the group being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              cpbId       - CPBID of the channel being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              encaps      - 0 - PPP encapsulation
**                          - 1 - HDLC encapsulated Ethernet
**                          - 2 - LAPS encapsulated Ethernet
**              pfcsMode    - Selects whether to use 16 or 32 bit CRC for HDLC
**                            FCS detection.  Only applies when encaps = 0 or 1.
**                          - 0 - CRC16 FCS
**                          - 1 - CRC32 FCS
**              abortType   - Defines abort type to be used for incorrect
**                            SOP/EOP errors
**                          - 0 - NO abort indication
**                          - 1 - Abort with 0x7D7E, do not append FCS
**                          - 2 - Invert FCS
**              minFlags    - Minimum number of flag bytes inserted between
**                            packets.  2^minFlags will be inserted.
**                          - Valid values are 0-7.
**              enable      - 1 - Group Enabled
**                          - 0 - Group held in reset
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpHdlcGrpChInitTx (struct file *fileHndl, UINT4 slice,
                                 UINT4 fullRate, UINT4 grpId, UINT4 cpbId,
                                 UINT4 encaps, UINT4 pfcsMode, UINT4 abortType,
                                 UINT4 minFlags, UINT4 enable)
{
    UINT4 regAddr = 0;
    UINT4 gfpFrmRst = 0;
    UINT4 gfpTraRst = 0;
    UINT4 modeOffset = 0;
    UINT4 modeMask = 0;
    UINT4 revBitOffset = 0;
    UINT4 revBitMask = 0;
    UINT4 procChanId = 0;
    UINT4 txgfptRpChanCfgReg0 = 0;
    UINT4 txdpIndChanCfgReg0 = 0;
    UINT4 txdpIndChanCfgReg1 = 0;
    UINT4 txdpIndChanCfgReg2 = 0;
    UINT4 cpbGfpConfig;
    UINT4 mask;
    UINT4 offset;  
    UINT4 gfpSysclkVal, sonetEnVal, txPosVcatBVal;    
    INT4 result;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_SCHD_GRP_CTXT *pGrpCtxt;

    /* Configure Indirect Variable settings */
    /* TxGFPT indirect setting initialization */
    UINT4 TXGFPT_RP_CHAN_CONFIG_NUM_SB = 0x0;
    UINT4 TXGFPT_RP_CHAN_CONFIG_MODE = 0x2;

    /* TXDP indirect setting initialization */
    UINT4 TXDP_IND_CHAN_CONFIG_UPI_SEL = 0x0;
    UINT4 TXDP_IND_CHAN_CONFIG_FCS_INS_POS = 0x0;
    UINT4 TXDP_IND_CHAN_CONFIG_TYPE_ADCT_DAT = 0x0;
    UINT4 TXDP_IND_CHAN_CONFIG_EHDR_EN = 0;
    
    UINT4 TXDP_IND_CHAN_CONFIG_ABORT_TYPE = abortType;
    UINT4 TXDP_IND_CHAN_CONFIG_FCS_INS = 0x1;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_MODE = 0x0; /* will be configured below */
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_INIT = 1;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_INV = 1;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_CRPT_INV = 1;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_REV_IBIT = 0; /* will be configured below */
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_REV_OBYTE = 0;
    UINT4 TXDP_IND_CHAN_CONFIG_FLAG_IDLE = minFlags;
    UINT4 TXDP_IND_CHAN_CONFIG_FDI_DATA_REV = 0;
    UINT4 TXDP_IND_CHAN_CONFIG_PFCS_REV_OBIT = 1;
    if (encaps == 2) {
        TXDP_IND_CHAN_CONFIG_PFCS_REV_OBIT = 0;
    }

    /* Configure TXDP_IND_CHAN_CONFIG_PFCS_MODE variable */
    if (encaps == 2) {
        TXDP_IND_CHAN_CONFIG_PFCS_MODE = 0x1;
    } else {
        TXDP_IND_CHAN_CONFIG_PFCS_MODE = pfcsMode;
    }

    /* Configure TXDP_IND_CHAN_CONFIG_PFCS_REV_IBIT variable */
    if (encaps == 2) {
        TXDP_IND_CHAN_CONFIG_PFCS_REV_IBIT = 0;
    } else {
        TXDP_IND_CHAN_CONFIG_PFCS_REV_IBIT = 1;
    }

    /* argument checking */
    if ((slice > 1)||(fullRate > 1)||(grpId > 0x3F)||(cpbId > 0x3F)||(encaps > 2)||(pfcsMode > 1)||(abortType > 2)||(minFlags > 7)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if ((slice == 1)&&(fullRate == 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if ((fullRate == 1)&&(grpId > 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check that Group is not currently active */
    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Grab pointer to GFP Slice context*/
    if (slice==0) {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpACtxt);
    } else {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpBCtxt);
    }

    /* Confirm GFP Slice Context is valid */
    if(pGfpCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Grab the scheduler data for the GFP SCBS being configured */
    pSchd = &(pGfpCtxt->schd);

    /* Grab the group data for the GFP SCBS being configured */
    pGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)(pSchd->pScbsGrpCtxt + grpId);

    if ((pGrpCtxt->startAddr != pGrpCtxt->endAddr) ||
            (pGrpCtxt->startAddr != HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW))
        return HYPHY20G_ERR_INVALID_PRECONFIG;


    /* Tx Direction Group Config */
    /* TXDP64 Group Reset */
    regAddr = HYPHY20G_GFP_FRM_TXDP_REG_GRP_RST_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
    if (result) return result;

    /* TXDP XOFF */
    regAddr = HYPHY20G_GFP_FRM_TXDP_REG_TX_DIS_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
        if (result) return result;
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
        if (result) return result;
    }

    /* TXDP CMF Processor Channel ID */
    regAddr = HYPHY20G_GFP_FRM_TXDP_REG_PROC_CHNL_ID_OFFSET(grpId/4);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &procChanId);
    if (result) return result;

    modeOffset = 8*(grpId%4) + 7;
    modeMask = 0x1 << modeOffset;
    result = hyPhy20gVarFieldWrite(&procChanId, modeMask, 1);
    if (result) return result;

    revBitOffset = 8*(grpId%4) + 6;
    revBitMask = 0x1 << revBitOffset;
    result = hyPhy20gVarFieldWrite(&procChanId, revBitMask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, procChanId);
    if (result) return result;

    /* TXGFPT Group Reset */
    regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_GRP_RST_GROUPS_31_TO_0;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    if (grpId > 31)
        regAddr = regAddr + 4;

    if (enable == 0){
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
        if (result) return result;        
    }

    /* Tx Direction Channel Config */
    /* Configure RP_CHAN_CONFIG indirect registers */
    result = hyPhy20gVarFieldWrite(&txgfptRpChanCfgReg0,
        HYPHY20G_GFP_TRA_TXGFPT_TOP_RP_CHAN_CFG_IBIT_REQ_PROC_CHNL_CFG_NUM_SB_MSK,
        TXGFPT_RP_CHAN_CONFIG_NUM_SB);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txgfptRpChanCfgReg0,
        HYPHY20G_GFP_TRA_TXGFPT_TOP_RP_CHAN_CFG_IBIT_REQ_PROC_CHNL_CFG_MODE_MSK,
        TXGFPT_RP_CHAN_CONFIG_MODE);
    if (result) return result;

    result = gfptHyPhy20gTXGFPTReqProChanIndWrite(fileHndl, slice, cpbId, txgfptRpChanCfgReg0, 0x0);
    if (result) return result;

    /* TXDP64 Config */
    /* Configure IND_CHAN_CONFIG indirect registers */
    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg0,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_UPI_SEL_MSK,
        TXDP_IND_CHAN_CONFIG_UPI_SEL);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg0,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_FCS_INS_POS_MSK,
        TXDP_IND_CHAN_CONFIG_FCS_INS_POS);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg1,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_TYPE_ADCT_DAT_MSK,
        TXDP_IND_CHAN_CONFIG_TYPE_ADCT_DAT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_EHDR_EN_MSK,
        TXDP_IND_CHAN_CONFIG_EHDR_EN);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_ABORT_TYPE_MSK,
        TXDP_IND_CHAN_CONFIG_ABORT_TYPE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_FCS_INS_MSK,
        TXDP_IND_CHAN_CONFIG_FCS_INS);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_MODE_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_MODE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_INIT_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_INIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_INV_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_INV);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_CRPT_INV_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_CRPT_INV);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_REV_IBIT_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_REV_IBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_REV_OBIT_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_REV_OBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_PFCS_REV_OBYTE_MSK,
        TXDP_IND_CHAN_CONFIG_PFCS_REV_OBYTE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_FLAG_IDLE_MSK,
        TXDP_IND_CHAN_CONFIG_FLAG_IDLE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&txdpIndChanCfgReg2,
        HYPHY20G_GFP_FRM_TXDP_IND_CHAN_CFG_IBIT_IND_CHNL_COM_CFG_FDI_DATA_REV_MSK,
        TXDP_IND_CHAN_CONFIG_FDI_DATA_REV);
    if (result) return result;

    result = gfpfHyPhy20gTXDPChanIndWrite(fileHndl, slice, cpbId, txdpIndChanCfgReg0,
                                    txdpIndChanCfgReg1, txdpIndChanCfgReg2);
    if (result) return result;

    /* Configure GFP_RD_THRESH and GFP_RD_VALID_SQL in CPB */
    regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &cpbGfpConfig);
    if (result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
    result = hyPhy20gVarFieldWrite(&cpbGfpConfig, mask, 0x1A);
    if (result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_VALID_SQL_MSK;
    result = hyPhy20gVarFieldWrite(&cpbGfpConfig, mask, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, cpbGfpConfig);
    if (result) return result;

    /* Configure TX_POS_VCATB bit as a function of fullRate data will go 
       to VCAT subsystem or directly to SONET_LINE */
    regAddr = HYPHY20G_GFP_FRM_TOP_REG_MSTR_CTL;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_TX_POS_VCATB_OFF;

    txPosVcatBVal = (enable&&fullRate) ? 1:0;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, txPosVcatBVal);        
    if (result) return result;           
        
    /* retrieve SONET_EN value */
    regAddr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                    HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                    &sonetEnVal);
    if (result) return result;    

    /* Set GFP_0_SYSCLK or GFP_1_SYSCLK as a function of the downstream
       data path */        
    gfpSysclkVal = (txPosVcatBVal == 1) ? 1:sonetEnVal;
 
    regAddr = HYPHY20G_PGMRCLK_REG_GFP_SYSCLK_SEL;
    mask = (slice == 0)? HYPHY20G_PGMRCLK_BIT_GFP_SYSCLK_SEL_GFP_0_SYSCLK_SEL_MSK:
    HYPHY20G_PGMRCLK_BIT_GFP_SYSCLK_SEL_GFP_1_SYSCLK_SEL_MSK;
    
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, 
                                   mask, gfpSysclkVal);
    if (result) return result;  
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpHdlcGrpChInitTx */

/*******************************************************************************
**
**  hyPhy20gGfpHdlcGrpChInitRx
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Initializes and configures the Rx group and channel
**                  parameters of an HDLC link within the GFP subsystem.  (Only
**                  available for slice 'A' of the GFP subsystem, with SONET/SDH
**                  mapping).  By default, prepend bytes are not enabled.  This
**                  procedure should only be called if there are no channels
**                  currently assigned to this group.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B' (Only available if fullRate = 1)
**              fullRate    - 0 - non-OC-192c traffic.
**                          - 1 - OC-192c POS traffic (Vcat subsystem will be
**                              bypassed.  Must use grpId of 0)
**              grpId       - Group ID of the group being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              cpbId       - CPBID of the channel being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              encaps      - 0 - PPP encapsulation
**                          - 1 - HDLC encapsulated Ethernet
**                          - 2 - LAPS encapsulated Ethernet
**              pfcsMode    - Selects whether to use 16 or 32 bit CRC for HDLC
**                            FCS detection.  Only applies when encaps = 0 or 1.
**                          - 0 - CRC16 FCS
**                          - 1 - CRC32 FCS
**              pFcsStrip   - Controls whether payload FCS should be stripped
**                            from frames destined for the FDF interface.
**                          - 0 - do not strip pFcs
**                          - 1 - strip pFCS
**              fdoByteStrip - Controls the number of bytes stripped from HDLC
**                             frames destined for the FDO interface.
**                           - Valid values 0-31 (0x0-0x1F)
**              fdfByteStrip - Controls the number of bytes stripped from HDLC
**                             frames destined for the FDF interface.
**                           - Valid values 0-31 (0x0-0x1F)
**              enable      - 1 - Group Enabled
**                          - 0 - Group held in reset
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpHdlcGrpChInitRx (struct file *fileHndl, UINT4 slice,
                                 UINT4 fullRate, UINT4 grpId, UINT4 cpbId,
                                 UINT4 encaps, UINT4 pfcsMode,
                                 UINT4 pFcsStrip, UINT4 fdoByteStrip,
                                 UINT4 fdfByteStrip, UINT4 enable, 
                                 UINT4 payloadScram)
{
    UINT4 regAddr = 0;
    UINT4 gfpFrmRst = 0;
    UINT4 gfpTraRst = 0;
    UINT4 rxdpPageAddr0Word0 = 0;
    UINT4 rxdpPageAddr0Word1 = 0;
    UINT4 vcidCidVal = 0;
    UINT4 offset;
    INT4 result;

    /* Configure Indirect Variable settings */
    /* RXDP indirect setting initialization */
    UINT4 RXDP_PAGE_ADDRESS_00000_CHAN_EN = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_SELF_INITIALIZE = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_MODE = 1;
    //UINT4 RXDP_PAGE_ADDRESS_00000_DESCRMBL = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_DESCRMBL = payloadScram;
    UINT4 RXDP_PAGE_ADDRESS_00000_HDLC_PFCS_CHK = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_PFCS_MODE = pfcsMode;
    UINT4 RXDP_PAGE_ADDRESS_00000_PFCS_INIT = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_PFCS_INV = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_PFCS_REV_PLDBIT = 0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_00000_PFCS_REV_FCSBYTE = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_DATA_REV_IBIT = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_DATA_REV_OBIT = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_FDF_FCS_STRIP = pFcsStrip;
    UINT4 RXDP_PAGE_ADDRESS_00000_DELTA_FDO_STRIP = fdoByteStrip;
    UINT4 RXDP_PAGE_ADDRESS_00000_CHEC_ECC_MIN_PLD_CHK = 1;
    UINT4 RXDP_PAGE_ADDRESS_00000_THEC_CHK_MAX_PLD_CHK = 0x1;
    UINT4 RXDP_PAGE_ADDRESS_00000_THEC_ECC_TRNSP_MODE = 0; /* will be configured below */
    UINT4 RXDP_PAGE_ADDRESS_00000_EHEC_CHK = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_CNT_MAXFL = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_EHEC_ECC = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_DC_BALANCE = 0;
    UINT4 RXDP_PAGE_ADDRESS_00000_FDF_STRIP = fdfByteStrip;
    UINT4 RXDP_PAGE_ADDRESS_00000_RADP_BYTE = 0xDD;
    UINT4 RXDP_PAGE_ADDRESS_00000_MINFL = 0x0;
    UINT4 RXDP_PAGE_ADDRESS_00000_MAXFL = 0xFFFF;
    UINT4 RXDP_PAGE_ADDRESS_00000_PFCS_REV_FCSBIT = 1; /* will be configured below */
    if (encaps == 2) {
        RXDP_PAGE_ADDRESS_00000_PFCS_REV_FCSBIT = 0;
    }

    /* Configure RXDP_PAGE_ADDRESS_00000_THEC_ECC_TRNSP_MODE variable */
    if (encaps < 2) {
        RXDP_PAGE_ADDRESS_00000_THEC_ECC_TRNSP_MODE = 0;
    } else {
        RXDP_PAGE_ADDRESS_00000_THEC_ECC_TRNSP_MODE = 1;
    }

    /* Configure RXDP_PAGE_ADDRESS_00000_PFCS_REV_PLDBIT variable */
    if (encaps == 2) {
        RXDP_PAGE_ADDRESS_00000_PFCS_REV_PLDBIT = 0;
    } else {
        RXDP_PAGE_ADDRESS_00000_PFCS_REV_PLDBIT = 1;
    }

    /* argument checking */
    if ((slice > 1)||(fullRate > 1)||(grpId > 0x3F)||(cpbId > 0x3F)||(encaps > 2)||(pfcsMode > 1)||(pFcsStrip > 1)||(fdoByteStrip > 0x1F)||(fdfByteStrip > 0x1F)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if ((slice == 1)&&(fullRate == 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if ((fullRate == 1)&&(grpId > 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check that channel is not currently active */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CPBID_LKUP_TBL_OFFSET(cpbId);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_GFP_FRM_RXDP_BIT_CPBID_LKUP_TBL_VCID_CID_VAL_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &vcidCidVal);
    if (result) return result;

    if (vcidCidVal == 1)
        return HYPHY20G_ERR_INVALID_PRECONFIG;


    /* Rx Direction Group Config */
    /* RXDP64 Config */
    /* Set CHAN_EN to '0' */
    result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x0, grpId, 0x0, 0x0);
    if (result) return result;

    /* VCAT_CHN_RST */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_VCAT_GRP_RST_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
    if (result) return result;
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
        if (result) return result;
    }


    /* Configure PAGE_ADDRESS_00000 indirect registers */
    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_GRP_EN_MSK,
        RXDP_PAGE_ADDRESS_00000_CHAN_EN);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_GRP_SELF_INIT_MSK,
        RXDP_PAGE_ADDRESS_00000_SELF_INITIALIZE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_MODE_MSK,
        RXDP_PAGE_ADDRESS_00000_MODE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_DESCRMBL_MSK,
        RXDP_PAGE_ADDRESS_00000_DESCRMBL);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_HDLC_PFCS_CHK_MSK,
        RXDP_PAGE_ADDRESS_00000_HDLC_PFCS_CHK);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_PFCS_MODE_MSK,
        RXDP_PAGE_ADDRESS_00000_PFCS_MODE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_PFCS_INIT_MSK,
        RXDP_PAGE_ADDRESS_00000_PFCS_INIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_PFCS_INV_MSK,
        RXDP_PAGE_ADDRESS_00000_PFCS_INV);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_PFCS_REV_PLDBIT_MSK,
        RXDP_PAGE_ADDRESS_00000_PFCS_REV_PLDBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_PFCS_REV_FCSBIT_MSK,
        RXDP_PAGE_ADDRESS_00000_PFCS_REV_FCSBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_PFCS_REV_FCSBYTE_MSK,
        RXDP_PAGE_ADDRESS_00000_PFCS_REV_FCSBYTE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_DATA_REV_IBIT_MSK,
        RXDP_PAGE_ADDRESS_00000_DATA_REV_IBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_DATA_REV_OBIT_MSK,
        RXDP_PAGE_ADDRESS_00000_DATA_REV_OBIT);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_FDF_FCS_STRIP_MSK,
        RXDP_PAGE_ADDRESS_00000_FDF_FCS_STRIP);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_DELTA_FDO_STRIP_MSK,
        RXDP_PAGE_ADDRESS_00000_DELTA_FDO_STRIP);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_CHEC_ECC_MIN_PLD_CHK_MSK,
        RXDP_PAGE_ADDRESS_00000_CHEC_ECC_MIN_PLD_CHK);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_THEC_CHK_MAX_PLD_CHK_MSK,
        RXDP_PAGE_ADDRESS_00000_THEC_CHK_MAX_PLD_CHK);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_THEC_ECC_TRNSP_MODE_MSK,
        RXDP_PAGE_ADDRESS_00000_THEC_ECC_TRNSP_MODE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_EHEC_CHK_MSK,
        RXDP_PAGE_ADDRESS_00000_EHEC_CHK);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_CNT_MAXFL_MSK,
        RXDP_PAGE_ADDRESS_00000_CNT_MAXFL);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_EHEC_ECC_MSK,
        RXDP_PAGE_ADDRESS_00000_EHEC_ECC);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_DC_BALANCE_MSK,
        RXDP_PAGE_ADDRESS_00000_DC_BALANCE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_FDF_STRIP_MSK,
        RXDP_PAGE_ADDRESS_00000_FDF_STRIP);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word1,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_RADP_BYTE_MSK,
        RXDP_PAGE_ADDRESS_00000_RADP_BYTE);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word1,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_MINFL_MSK,
        RXDP_PAGE_ADDRESS_00000_MINFL);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&rxdpPageAddr0Word1,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_MAXFL_MSK,
        RXDP_PAGE_ADDRESS_00000_MAXFL);
    if (result) return result;

    result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x0, grpId, rxdpPageAddr0Word0,
                                    rxdpPageAddr0Word1);
    if (result) return result;

    /* Rx Channel Config */
    /* RxGFPT Config */
    /* Set CHAN_EN to '0' */
    regAddr = HYPHY20G_GFP_TRA_RXGFPT_REG_CHNL_EN_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 0);
    if (result) return result;

    /* Toggle CHAN_RESET */
    regAddr = HYPHY20G_GFP_TRA_RXGFPT_REG_CHNL_RST_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 1);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 0);
    if (result) return result;

    /* Configure GFP_INTF_MODE to '1' for HDLC mode */
    regAddr = HYPHY20G_GFP_TRA_RXGFPT_REG_IF_MD_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 1);
    if (result) return result;

    /* RXDP64 Config */
    /* Toggle CPBID_CHN_RST */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CPBID_CHNL_RST_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 1);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 0);
    if (result) return result;

    /* Configure RX_POS_VCATB bit */
    regAddr = HYPHY20G_GFP_FRM_TOP_REG_MSTR_CTL;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    offset = HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_RX_POS_VCATB_OFF;
    if (fullRate) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, enable);
        if (result) return result;
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpHdlcGrpChInitRx */


/*******************************************************************************
**
**  hyPhy20gGfpOtu2TranspSliceInit
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Initializes and configures an OTU2-based slice within the
**                  OTNMA / OTNMD of the GFP subsystem for transparent OTN
**                  mapping.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              mode        - 0 - Asynchronous mode
**                          - 1 - Synchronous mode
**              fixStuffEn  - 0 - 64 fixed stuff (FS) bytes are NOT added in
**                                columns 1905 to 1920
**                          - 1 - 64 fixed stuff (FS) bytes are added in
**                                columns 1905 to 1920 - Set to '1' for OC-192
**                                mappings or Gsup43 Clause 7.1.  Otherwise, set
**                                to '0'.
**              enable      - 0 - Disable slice
**                          - 1 - Enable slice
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpOtu2TranspSliceInit(struct file *fileHndl, UINT4 slice,
                                    UINT4 mode, UINT4 fixStuffEn, UINT4 enable)
{
    UINT4 regAddr = 0;
    UINT4 gfpTraRst = 0;
    UINT4 otnmaCfg = 0;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(mode > 1)||(fixStuffEn > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure OTNMA */
    /* Set SLICE_EN to '0' before configuring block */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(0);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_SLICE_EN_OFF,
                                0);
    if (result) return result;

    /* Configure OTNMA Mapper Configuration */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_OTN_MAP_CFG;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &otnmaCfg);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_VCAT_TO_CPB_REQ_DELAY_MSK,
        HYPHY20G_OTNMA_VCAT_TO_CPB_REQ_DELAY);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_CPB_DATA_DELAY_MSK,
        HYPHY20G_OTNMA_CPB_DATA_DELAY);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_VCAT_DATA_DELAY_MSK,
        HYPHY20G_OTNMA_VCAT_DATA_DELAY);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_FIFO_CENTER_MODE_MSK,
        0x1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_FIFO_CENTER_EN_MSK,
        0x1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_OPU2_FIXED_STUFF_EN_MSK,
        fixStuffEn);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_OTU_RATE_SEL_MSK,
        0x0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, otnmaCfg);
    if (result) return result;

    /* Configure OTNMA Slice Configuration */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(0);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_SLICE_SYNC_SEL_OFF,
                            mode);
    if (result) return result;

    /* OTNMA set SLICE_EN */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(0);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_SLICE_EN_OFF,
                                enable);
    if (result) return result;

    /* Configure OTNMD */
    /* Set SLICE_EN to '0'  */
    regAddr = HYPHY20G_GFP_TRA_OTNMD_REG_SLC_CFG_OFFSET(0);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMD_BIT_SLC_CFG_SLICE_EN_OFF,
                                0);
    if (result) return result;

    /* Configure OTNMD Demapper Configuration */
    regAddr = HYPHY20G_GFP_TRA_OTNMD_REG_OTN_DEMAPPER_CFG;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, HYPHY20G_OTNMD_DATA_DELAY);
    if (result) return result;

    /* OTNMD Set SLICE_EN */
    regAddr = HYPHY20G_GFP_TRA_OTNMD_REG_SLC_CFG_OFFSET(0);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMD_BIT_SLC_CFG_SLICE_EN_OFF,
                                enable);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpOtu2TranspSliceInit */

/*******************************************************************************
**
**  hyPhy20gGfpOtu2TranspChTxAdd
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Enables a CPBID to VCAT GID mapping within the OTNMA
**                  for a client channel carried transparently within an OTU2.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - CPBID of the channel being added
**                          - valid values are 0x0 - 0x3F (0-63)
**              grpId       - VCAT group ID to assign this channel to
**                          - valid values are 0x0 - 0x3F (0-63)
**              mappingType - 0 - 10GE --> ODU1e
**                          - 1 - 10GE --> ODU2e
**                          - 2 - 10G FC --> ODU1f
**                          - 3 - 10G FC --> ODU2f
**                          - 4 - SONET / SDH --> ODU2
**                          - 5 - Other
**              pktSize     - Configures the nominal packet size to use for
**                            the OTNMA.  This value should match the
**                            value being used by the HSCI.
**              Application Specific recommended values for pktSize are:
**              508 bytes - STS-192 async or sync demapped from ODU2
**              510 bytes - 10GE LAN sync demapped from ODU1e/2e
**              503 bytes - 10G FC sync demapped from ODU1f/2f
**
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gGfpOtu2TranspChTxAdd(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 grpId,
                                 UINT4 mappingType, UINT4 pktSize)
{
    UINT4 regAddr = 0;
    UINT4 gfpTraRst = 0;
    UINT4 otnmaGrpCfg = 0;
    UINT4 otnmaWdogCfg = 0;
    UINT4 numFecColumns = 0;
    UINT4 numRowsNplus1Fecs = 0;
    UINT4 numRowsNFecs = 0;
    UINT4 timePeriod = 0;
    UINT4 syncMode = 0;
    UINT4 offset = 0;
    UINT4 otnmaSchCfg;
    UINT4 sliceCfg;
    UINT4 cpbGfpConfig;
    UINT4 mask;
    UINT4 i;
    UINT4 sliceEn;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(grpId > 0x3F)||(mappingType > 5)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check SLICE_EN */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(0);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_SLICE_EN_OFF,
                                &sliceEn);
    if (result) return result;
    if (sliceEn == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;


    /* Configure OTNMA */
    /* Set VCAT_GRP_EN to '0' before configuring  */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(grpId);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_VCAT_GRP_EN_OFF,
                0);
    if (result) return result;

    /* Configure OTNMA VCAT Group Configuration */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(grpId);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &otnmaGrpCfg);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaGrpCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_CPB_CHN_ID_VCAT_GRP_ID_MSK,
        cpbId);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaGrpCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_VCAT_GRP_NOM_PKT_SIZE_MSK,
        pktSize);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, otnmaGrpCfg);
    if (result) return result;

    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(0);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_SLICE_SYNC_SEL_OFF,
                            &syncMode);
    if (result) return result;

    /* Configure OTNMA Watchdog Timer Configuration */
    if ((syncMode == 1 || (syncMode == 0))&&(mappingType < 5)) {
        switch (mappingType) {
            case 0: /* 10GE --> ODU1e */
                numFecColumns = 32;
                numRowsNplus1Fecs = 5;
                numRowsNFecs = 3;
                break;
            case 1: /* 10GE --> ODU2e */
                numFecColumns = 28;
                numRowsNplus1Fecs = 8;
                numRowsNFecs = 9;
                break;
            case 2: /* 10G FC --> ODU1f */
                numFecColumns = 13;
                numRowsNplus1Fecs = 4;
                numRowsNFecs = 13;
                break;
            case 3: /* 10G FC --> ODU2f */
                numFecColumns = 9;
                numRowsNplus1Fecs = 1;
                numRowsNFecs = 5;
                break;
            case 4: /* SONET / SDH --> ODU2 */
                numFecColumns = 64;
                numRowsNplus1Fecs = 0;
                numRowsNFecs = 0;
                break;
        }
        timePeriod = HYPHY20G_GFP_OTNMA_10G_WATCHDOG_TIME_PERIOD;
		
        regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_SPEC_WTCHDG_FEC_CFG_OFFSET(0);
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gRead(fileHndl, regAddr, &otnmaWdogCfg);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&otnmaWdogCfg,
            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_SPEC_WTCHDG_FEC_CFG_NUM_ROWS_N_PLUS1_FECS_MSK,
            numRowsNplus1Fecs);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&otnmaWdogCfg,
            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_SPEC_WTCHDG_FEC_CFG_NUM_ROWS_N_FECS_MSK,
            numRowsNFecs);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&otnmaWdogCfg,
            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_SPEC_WTCHDG_FEC_CFG_WATCHDOG_TIME_PERIOD_MSK,
            timePeriod);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&otnmaWdogCfg,
            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_SPEC_WTCHDG_FEC_CFG_WATCHDOG_TIMER_GRPID_MSK,
            grpId);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, otnmaWdogCfg);
        if (result) return result;

        regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(0);
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gRead(fileHndl, regAddr, &sliceCfg);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&sliceCfg,
            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_NUM_FEC_COLUMNS_MSK,
            numFecColumns);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, sliceCfg);
        if (result) return result;

        /* If group ID is 0, set other 3 watchdog slices to have non-zero Grp ID */
        if (grpId == 0) {
            for (i = 1; i<=3; i++) {
                regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_SPEC_WTCHDG_FEC_CFG_OFFSET(i);
                regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0x3F);
                if (result) return result;
            }
       }


    }

    /* Configure GRP_FAIL_PATTERN_SEL */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_ORD_SET_CFG_1;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_GFP_TRA_OTNMA_BIT_ORD_SET_CFG_1_GRP_FAIL_PATTERN_SEL_OFF;

    if (mappingType < 2) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    }

    /* Configure GFP_RD_THRESH and GFP_RD_VALID_SQL in CPB */
    regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &cpbGfpConfig);
    if (result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
    result = hyPhy20gVarFieldWrite(&cpbGfpConfig, mask, 0x1);
    if (result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_VALID_SQL_MSK;
    result = hyPhy20gVarFieldWrite(&cpbGfpConfig, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, cpbGfpConfig);
    if (result) return result;

    /* Configure OTNMA Tx Scheduler */
    for (i = 0; i<HYPHY20G_GFP_NUM_VCATID; i++) {
        regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(i);
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);

        offset = HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_SCH_GRP_EN_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;
    }

    for (i = 0; i<HYPHY20G_GFP_NUM_VCATID; i++) {
        regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(i);
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);

        result = sysHyPhy20gRead(fileHndl, regAddr, &otnmaSchCfg);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&otnmaSchCfg,
            HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_SCH_GRP_MSK,
            grpId);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&otnmaSchCfg,
            HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_SCH_GRP_EN_MSK,
            1);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, otnmaSchCfg);
        if (result) return result;
    }

    /* OTNMA set VCAT_GRP_EN to '1' */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(grpId);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_VCAT_GRP_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;


    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpOtu2TranspChTxAdd */

/*******************************************************************************
**
**  hyPhy20gGfpOtu2TranspChTxDelete
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Removes a CPBID to VCAT GID mapping within the OTNMA
**                  for a client channel carried transparently within an OTU2.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              grpId       - VCAT group ID being used by the channel
**                          - valid values are 0x0 - 0x3F (0-63)
**
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpOtu2TranspChTxDelete(struct file *fileHndl, UINT4 slice,
                                     UINT4 grpId)
{
    UINT4 regAddr = 0;
    UINT4 gfpTraRst = 0;
    UINT4 offset;
    UINT4 sliceEn;
    UINT4 i;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(grpId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check SLICE_EN */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(0);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_SLICE_EN_OFF,
                                &sliceEn);
    if (result) return result;
    if (sliceEn == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure OTNMA */
    /* Set VCAT_GRP_EN to '0'  */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(grpId);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_VCAT_GRP_EN_OFF,
                0);
    if (result) return result;

    /* Configure OTNMA Tx Scheduler */
    for (i = 0; i<HYPHY20G_GFP_NUM_VCATID; i++) {
        regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(i);
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);

        offset = HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_SCH_GRP_EN_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpOtu2TranspChTxDelete */

/*******************************************************************************
**
**  hyPhy20gGfpOtu2TranspChRxAdd
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Enables a CPBID to VCAT GID mapping within the OTNMD
**                  for a client channel carried transparently within an OTU2.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - CPBID of the channel being added
**                          - valid values are 0x0 - 0x3F (0-63)
**              grpId       - VCAT group ID to assign this channel to
**                          - valid values are 0x0 - 0x3F (0-63)
**              pktSize     - Configures the nominal packet size to use for
**                            the OTNMD.  This value should match the
**                            value being used by the HSCI.
**              Application Specific recommended values for pktSize are:
**              508 bytes - STS-192 async or sync demapped from ODU2
**              510 bytes - 10GE LAN sync demapped from ODU1e/2e
**              503 bytes - 10G FC sync demapped from ODU1f/2f
**
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpOtu2TranspChRxAdd(struct file *fileHndl, UINT4 slice,
                                  UINT4 cpbId, UINT4 grpId, UINT4 pktSize)
{
    UINT4 regAddr = 0;
    UINT4 gfpTraRst = 0;
    UINT4 otnmdGrpCfg = 0;
    UINT4 sliceEn;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(grpId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check SLICE_EN */
    regAddr = HYPHY20G_GFP_TRA_OTNMD_REG_SLC_CFG_OFFSET(0);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMD_BIT_SLC_CFG_SLICE_EN_OFF,
                                &sliceEn);
    if (result) return result;
    if (sliceEn == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;


    /* Configure OTNMD */
    /* Configure OTNMD VCAT Group Configuration */
    regAddr = HYPHY20G_GFP_TRA_OTNMD_REG_VCAT_LCAS_GRP_CFG_OFFSET(grpId);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &otnmdGrpCfg);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmdGrpCfg,
        HYPHY20G_GFP_TRA_OTNMD_BIT_VCAT_LCAS_GRP_CFG_CPB_CHN_ID_VCAT_GRP_ID_MSK,
        cpbId);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmdGrpCfg,
        HYPHY20G_GFP_TRA_OTNMD_BIT_VCAT_LCAS_GRP_CFG_VCAT_GRP_NOM_PKT_SIZE_MSK,
        pktSize);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, otnmdGrpCfg);
    if (result) return result;


    /* Enable group in RVCPO */
    /* Set GRP_ID_N_INHIBIT to '0' */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_GRP_ID_INH_0 + 4*(grpId/32);
#ifdef HWACCESSMODE_USB
/* emulation */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
#else
/* EVBD */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
#endif
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpOtu2TranspChRxAdd */

/*******************************************************************************
**
**  hyPhy20gGfpOtu2TranspChRxDelete
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Disables a CPBID to VCAT GID mapping within the OTNMD
**                  for a client channel carried transparently within an OTU2.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              grpId       - VCAT group ID being used by this channel
**                          - valid values are 0x0 - 0x3F (0-63)
**
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpOtu2TranspChRxDelete(struct file *fileHndl, UINT4 slice,
                                     UINT4 grpId)
{
    UINT4 regAddr = 0;
    UINT4 gfpTraRst = 0;
    UINT4 sliceEn;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(grpId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check SLICE_EN */
    regAddr = HYPHY20G_GFP_TRA_OTNMD_REG_SLC_CFG_OFFSET(0);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMD_BIT_SLC_CFG_SLICE_EN_OFF,
                                &sliceEn);
    if (result) return result;
    if (sliceEn == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;


    /* Disable group in RVCPO */
    /* Set GRP_ID_N_INHIBIT to '1' */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_GRP_ID_INH_0 + 4*(grpId/32);
#ifdef HWACCESSMODE_USB
/* emulation */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
#else
/* EVBD */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
#endif
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpOtu2TranspChRxDelete */


/*******************************************************************************
**
**  hyPhy20gGfpOtu1TranspSliceInit
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Initializes and configures an OTU1-based slice within the
**                  OTNMA / OTNMD of the GFP subsystem for transparent OTN
**                  mapping.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              otu1Num     - Specifies which of the 4 OTU1 links in the OTNMA /
**                            OTNMD to configure
**                          - valid values are 0-3
**              mode        - 0 - Asynchronous mode
**                          - 1 - Synchronous mode
**              enable      - 0 - Disable slice
**                          - 1 - Enable slice
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpOtu1TranspSliceInit(struct file *fileHndl, UINT4 slice,
                                 UINT4 otu1Num, UINT4 mode, UINT4 enable)
{
    UINT4 regAddr = 0;
    UINT4 gfpTraRst = 0;
    UINT4 otnmaCfg = 0;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(otu1Num > 3)||(mode > 1)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure OTNMA */
    /* Set SLICE_EN to '0' before configuring block */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(otu1Num);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_SLICE_EN_OFF,
                                0);
    if (result) return result;

    /* Configure OTNMA Mapper Configuration */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_OTN_MAP_CFG;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &otnmaCfg);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_VCAT_TO_CPB_REQ_DELAY_MSK,
        HYPHY20G_OTNMA_VCAT_TO_CPB_REQ_DELAY);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_CPB_DATA_DELAY_MSK,
        HYPHY20G_OTNMA_CPB_DATA_DELAY);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_VCAT_DATA_DELAY_MSK,
        HYPHY20G_OTNMA_VCAT_DATA_DELAY);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_FIFO_CENTER_MODE_MSK,
        0x1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_FIFO_CENTER_EN_MSK,
        0x1);
    if (result) return result;


    result = hyPhy20gVarFieldWrite(&otnmaCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_OTN_MAP_CFG_OTU_RATE_SEL_MSK,
        0x1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, otnmaCfg);
    if (result) return result;

    /* Configure OTNMA Slice Configuration */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(otu1Num);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_SLICE_SYNC_SEL_OFF,
                            mode);
    if (result) return result;

    /* OTNMA set SLICE_EN */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(otu1Num);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_SLICE_EN_OFF,
                                enable);
    if (result) return result;


    /* Configure OTNMD */
    /* Set SLICE_EN '0' as part of link init routine */
    regAddr = HYPHY20G_GFP_TRA_OTNMD_REG_SLC_CFG_OFFSET(otu1Num);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMD_BIT_SLC_CFG_SLICE_EN_OFF,
                                0);
    if (result) return result;

    /* Configure OTNMD Demapper Configuration */
    regAddr = HYPHY20G_GFP_TRA_OTNMD_REG_OTN_DEMAPPER_CFG;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, HYPHY20G_OTNMD_DATA_DELAY);
    if (result) return result;

    /* OTNMD Set SLICE_EN */
    regAddr = HYPHY20G_GFP_TRA_OTNMD_REG_SLC_CFG_OFFSET(otu1Num);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMD_BIT_SLC_CFG_SLICE_EN_OFF,
                                enable);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpOtu1TranspSliceInit */

/*******************************************************************************
**
**  hyPhy20gGfpOtu1TranspChTxAdd
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Enables a CPBID to VCAT GID mapping within the OTNMA
**                  for a client channel carried transparently within an OTU1.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              otu1Num     - Specifies which of the 4 OTU1 links in the OTNMA
**                            to configure
**                          - valid values are 0-3
**              cpbId       - CPBID of the channel being added
**                          - valid values are 0x0 - 0x3F (0-63)
**              grpId       - VCAT group ID to assign this channel to
**                          - valid values are 0x0 - 0x3F (0-63)
**              pktSize     - Configures the nominal packet byte size to use for
**                            the OTNMA.  This value should match the
**                            value being used by the LSCI/HSCI.
**              Application Specific recommended values for pktSize are:
**              499 bytes - Rev B OC-48/12/3 sync/async demapped
**
**
**
**  COMMENTS:   grpID of 0x3F should NOT be used for channels being mapped
**              in the VCAT subsystem using TS mode.
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gGfpOtu1TranspChTxAdd(struct file *fileHndl, UINT4 slice,
                                 UINT4 otu1Num, UINT4 cpbId, UINT4 grpId,
                                 UINT4 pktSize)
{
    UINT4 regAddr = 0;
    UINT4 gfpTraRst = 0;
    UINT4 otnmaGrpCfg = 0;
    UINT4 otnmaWdogCfg = 0;
    UINT4 numFecColumns = 0;
    UINT4 numRowsNplus1Fecs = 0;
    UINT4 numRowsNFecs = 0;
    UINT4 timePeriod = 0;
    UINT4 syncMode = 0;
    UINT4 offset = 0;
    UINT4 otu1ModeCfg = 0;
    UINT4 otnmaSchCfg;
    UINT4 sliceCfg;
    UINT4 cpbGfpConfig;
    UINT4 sliceEn;
    UINT4 mask;
    UINT4 mode;
    UINT4 group;
    UINT4 tsModeCfg;
    UINT4 i;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(otu1Num > 3)||(cpbId > 0x3F)||(grpId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check SLICE_EN */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(otu1Num);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_SLICE_EN_OFF,
                                &sliceEn);
    if (result) return result;
    if (sliceEn == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check TVCPO SLICE_MODE */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_OTU1_MD_SLICES_CFG;
#ifdef HWACCESSMODE_USB
/* emulation */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
#else
/* EVBD */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
#endif
    result = sysHyPhy20gRead(fileHndl, regAddr, &otu1ModeCfg);
    if (result) return result;
    mask = 0x3 << (4+(2*otu1Num));
    offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_SLICE0_MODE_OFF + 2*otu1Num;
    mode = mHYPHY20G_GET_FIELD4(otu1ModeCfg, mask, offset);
    if (mode > 0x1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure OTNMA */
    /* Set VCAT_GRP_EN to '0' before configuring  */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(grpId);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_VCAT_GRP_EN_OFF,
                0);
    if (result) return result;

    /* Configure OTNMA VCAT Group Configuration */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(grpId);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &otnmaGrpCfg);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaGrpCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_CPB_CHN_ID_VCAT_GRP_ID_MSK,
        cpbId);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmaGrpCfg,
        HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_VCAT_GRP_NOM_PKT_SIZE_MSK,
        pktSize);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, otnmaGrpCfg);
    if (result) return result;

    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(otu1Num);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_SLICE_SYNC_SEL_OFF,
                            &syncMode);
    if (result) return result;

    /* Configure OTNMA Watchdog Timer Configuration */
    if (syncMode == 1 || syncMode == 0) {
        numFecColumns = 68;
        numRowsNplus1Fecs = 3;
        numRowsNFecs = 7;
        /* Using fixed numbers below to avoid floating point arithmetic */
        timePeriod = HYPHY20G_GFP_OTNMA_2G5_WATCHDOG_TIME_PERIOD;

        regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_SPEC_WTCHDG_FEC_CFG_OFFSET(otu1Num);
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gRead(fileHndl, regAddr, &otnmaWdogCfg);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&otnmaWdogCfg,
            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_SPEC_WTCHDG_FEC_CFG_NUM_ROWS_N_PLUS1_FECS_MSK,
            numRowsNplus1Fecs);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&otnmaWdogCfg,
            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_SPEC_WTCHDG_FEC_CFG_NUM_ROWS_N_FECS_MSK,
            numRowsNFecs);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&otnmaWdogCfg,
            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_SPEC_WTCHDG_FEC_CFG_WATCHDOG_TIME_PERIOD_MSK,
            timePeriod);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&otnmaWdogCfg,
            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_SPEC_WTCHDG_FEC_CFG_WATCHDOG_TIMER_GRPID_MSK,
            grpId);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, otnmaWdogCfg);
        if (result) return result;

        regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_SLC_CFG_OFFSET(otu1Num);
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gRead(fileHndl, regAddr, &sliceCfg);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&sliceCfg,
            HYPHY20G_GFP_TRA_OTNMA_BIT_SLC_CFG_NUM_FEC_COLUMNS_MSK,
            numFecColumns);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, sliceCfg);
        if (result) return result;
    }

    /* Configure GRP_FAIL_PATTERN_SEL */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_ORD_SET_CFG_1;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_GFP_TRA_OTNMA_BIT_ORD_SET_CFG_1_GRP_FAIL_PATTERN_SEL_OFF;

    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);

    /* Configure GFP_RD_THRESH and GFP_RD_VALID_SQL in CPB */
    regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &cpbGfpConfig);
    if (result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
    result = hyPhy20gVarFieldWrite(&cpbGfpConfig, mask, 0x1);
    if (result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_VALID_SQL_MSK;
    result = hyPhy20gVarFieldWrite(&cpbGfpConfig, mask, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, cpbGfpConfig);
    if (result) return result;

    /* Configure OTNMA Tx Scheduler */
    if (mode == 0x1) {
        for (i = 0; i<16; i++) {
            regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(4*i+otu1Num);
            regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);

            offset = HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_SCH_GRP_EN_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
            if (result) return result;
        }

        for (i = 0; i<16; i++) {
            regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(4*i+otu1Num);
            regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);

            result = sysHyPhy20gRead(fileHndl, regAddr, &otnmaSchCfg);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&otnmaSchCfg,
                HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_SCH_GRP_MSK,
                grpId);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&otnmaSchCfg,
                HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_SCH_GRP_EN_MSK,
                1);
            if (result) return result;

            result = sysHyPhy20gWrite(fileHndl, regAddr, otnmaSchCfg);
            if (result) return result;
        }
    } else {
        for (i = 0; i<16; i++) {
            regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_SLC_0_GRP_15_CFG_OFFSET(i)+0x40*otu1Num;
#ifdef HWACCESSMODE_USB
/* emulation */
            regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
#else
/* EVBD */
            regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
#endif
            result = sysHyPhy20gRead(fileHndl, regAddr, &tsModeCfg);
            mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_GROUP_ID_MSK;
            offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_GROUP_ID_OFF;
            group = mHYPHY20G_GET_FIELD4(tsModeCfg, mask, offset);
            if (group == grpId) {
                regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(4*i+otu1Num);
                regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);

                offset = HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_SCH_GRP_EN_OFF;
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
                if (result) return result;

                result = sysHyPhy20gRead(fileHndl, regAddr, &otnmaSchCfg);
                if (result) return result;

                result = hyPhy20gVarFieldWrite(&otnmaSchCfg,
                    HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_SCH_GRP_MSK,
                    grpId);
                if (result) return result;

                result = hyPhy20gVarFieldWrite(&otnmaSchCfg,
                    HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_SCH_GRP_EN_MSK,
                    1);
                if (result) return result;

                result = sysHyPhy20gWrite(fileHndl, regAddr, otnmaSchCfg);
                if (result) return result;
            }
        }
    }

    /* OTNMA set VCAT_GRP_EN to '1' */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(grpId);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_VCAT_GRP_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;


    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpOtu1TranspChTxAdd */

/*******************************************************************************
**
**  hyPhy20gGfpOtu1TranspChTxDelete
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Removes a CPBID to VCAT GID mapping within the OTNMA
**                  for a client channel carried transparently within an OTU1.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              otu1Num     - Specifies which of the 4 OTU1 links in the OTNMA
**                            to configure
**                          - valid values are 0-3
**              grpId       - VCAT group ID being used by the channel
**                          - valid values are 0x0 - 0x3F (0-63)
**
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpOtu1TranspChTxDelete(struct file *fileHndl, UINT4 slice,
                                     UINT4 otu1Num, UINT4 grpId)
{
    UINT4 regAddr = 0;
    UINT4 gfpTraRst = 0;
    UINT4 mask;
    UINT4 offset;
    UINT4 mode;
    UINT4 otu1ModeCfg;
    UINT4 tsModeCfg;
    UINT4 group;
    UINT4 i;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(otu1Num > 3)||(grpId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check TVCPO SLICE_MODE */
    regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_OTU1_MD_SLICES_CFG;
#ifdef HWACCESSMODE_USB
/* emulation */
     regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
#else
/* EVBD */
     regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
#endif
    result = sysHyPhy20gRead(fileHndl, regAddr, &otu1ModeCfg);
    if (result) return result;
    mask = 0x3 << (4+(2*otu1Num));
    offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_SLICE0_MODE_OFF + 2*otu1Num;
    mode = mHYPHY20G_GET_FIELD4(otu1ModeCfg, mask, offset);
    if (mode > 0x1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure OTNMA */
    /* Set VCAT_GRP_EN to '0'  */
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(grpId);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr,
                HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_VCAT_GRP_EN_OFF,
                0);
    if (result) return result;

    /* Configure OTNMA Tx Scheduler */
    if (mode == 0x1) {
        for (i = 0; i<16; i++) {
            regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(4*i+otu1Num);
            regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);

            offset = HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_SCH_GRP_EN_OFF;
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
            if (result) return result;
        }

    } else {
        for (i = 0; i<16; i++) {
            regAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_SLC_0_GRP_15_CFG_OFFSET(i)+0x40*otu1Num;
#ifdef HWACCESSMODE_USB
/* emulation */
            regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
#else
/* EVBD */
            regAddr = HYPHY20G_VCAT_LCAS_TVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
#endif
            result = sysHyPhy20gRead(fileHndl, regAddr, &tsModeCfg);
            mask = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_GROUP_ID_MSK;
            offset = HYPHY20G_VCAT_LCAS_TVCPO_BIT_SLC_0_GRP_15_CFG_SLICE0_GROUP_ID_OFF;
            group = mHYPHY20G_GET_FIELD4(tsModeCfg, mask, offset);
            if (group == grpId) {
                regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(4*i+otu1Num);
                regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);

                offset = HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_SCH_GRP_EN_OFF;
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
                if (result) return result;

            }
        }
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpOtu1TranspChTxDelete */

/*******************************************************************************
**
**  hyPhy20gGfpOtu1TranspChRxAdd
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Enables a CPBID to VCAT GID mapping within the OTNMD
**                  for a client channel carried transparently within an OTU1.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              otu1Num     - Specifies which of the 4 OTU1 links in the
**                            OTNMD to configure
**                          - valid values are 0-3
**              cpbId       - CPBID of the channel being added
**                          - valid values are 0x0 - 0x3F (0-63)
**              grpId       - VCAT group ID to assign this channel to
**                          - valid values are 0x0 - 0x3F (0-63)
**              pktSize     - Configures the nominal packet size to use for
**                            the OTNMD.  This value should match the
**                            value being used by the LSCI/HSCI.
**              Application Specific recommended values for pktSize are:
**              200 bytes - OC-48 sync demapped
**              499 bytes - Rev B OC-48/12/3 async demapped
**
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpOtu1TranspChRxAdd(struct file *fileHndl, UINT4 slice,
                                 UINT4 otu1Num, UINT4 cpbId, UINT4 grpId,
                                 UINT4 pktSize)
{
    UINT4 regAddr = 0;
    UINT4 gfpTraRst = 0;
    UINT4 otnmdGrpCfg = 0;
    UINT4 sliceEn;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(otu1Num > 3)||(cpbId > 0x3F)||(grpId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check SLICE_EN */
    regAddr = HYPHY20G_GFP_TRA_OTNMD_REG_SLC_CFG_OFFSET(otu1Num);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                                HYPHY20G_GFP_TRA_OTNMD_BIT_SLC_CFG_SLICE_EN_OFF,
                                &sliceEn);
    if (result) return result;
    if (sliceEn == 0) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure OTNMD */
    /* Configure OTNMD VCAT Group Configuration */
    regAddr = HYPHY20G_GFP_TRA_OTNMD_REG_VCAT_LCAS_GRP_CFG_OFFSET(grpId);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &otnmdGrpCfg);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmdGrpCfg,
        HYPHY20G_GFP_TRA_OTNMD_BIT_VCAT_LCAS_GRP_CFG_CPB_CHN_ID_VCAT_GRP_ID_MSK,
        cpbId);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&otnmdGrpCfg,
        HYPHY20G_GFP_TRA_OTNMD_BIT_VCAT_LCAS_GRP_CFG_VCAT_GRP_NOM_PKT_SIZE_MSK,
        pktSize);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, otnmdGrpCfg);
    if (result) return result;


    /* Enable group in RVCPO */
    /* Set GRP_ID_N_INHIBIT to '0' */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_GRP_ID_INH_0 + 4*(grpId/32);
#ifdef HWACCESSMODE_USB
/* emulation */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
#else
/* EVBD */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
#endif
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpOtu1TranspChRxAdd */

/*******************************************************************************
**
**  hyPhy20gGfpOtu1TranspChRxDelete
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Disables a CPBID to VCAT GID mapping within the OTNMD
**                  for a client channel carried transparently within an OTU1.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              grpId       - VCAT group ID being used by this channel
**                          - valid values are 0x0 - 0x3F (0-63)
**
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpOtu1TranspChRxDelete(struct file *fileHndl, UINT4 slice,
                                      UINT4 grpId)
{
    UINT4 regAddr = 0;
    UINT4 gfpTraRst = 0;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(grpId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;


    /* Disable group in RVCPO */
    /* Set GRP_ID_N_INHIBIT to '1' */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_GRP_ID_INH_0 + 4*(grpId/32);
#ifdef HWACCESSMODE_USB
/* emulation */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
#else
/* EVBD */
    regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
#endif
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpOtu1TranspChRxDelete */



/*******************************************************************************
**
**  hyPhy20gGfpFExiFilterCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the EXI Match filter within the RXDP64 for
**                  processing incoming GFP frames for a given group.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              grpId       - grpId of the group being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              priority    - Selects which of the 4 stages of filtering to
**                            configure.  Stage 0 has the highest priority, and
**                            stage 3 has the lowest priority.
**                          - Valid values are 0-3
**              exiMatch    - Field to be matched to incoming frame's EXI field.
**                          - Valid values are 0-2
**              ehdrLen     - Assumed length of extension header when exiMatch
**                            is true.
**                          - Valid values are TBD
**              fdfSend     - Selects whether matched frames should also be
**                            forwarded to the FDF interface for uP access
**                          - 0 - Do not send matching frames to FDF
**                          - 1 - Send matching frames to FDF
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpFExiFilterCfg(struct file *fileHndl, UINT4 slice,
                              UINT4 grpId, UINT4 priority, UINT4 exiMatch,
                              UINT4 ehdrLen, UINT4 fdfSend)
{
    UINT4 regAddr = 0;
    UINT4 gfpFrmRst = 0;
    UINT4 gfpTraRst = 0;
    UINT4 rxdpVcatChRst = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(grpId > 0x3F)||(priority > 0x3)||(exiMatch > 2)||(ehdrLen > 0x3C)||(fdfSend > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check RXDP64 VCAT_CHN_RST and MODE settings */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_VCAT_GRP_RST_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr, grpId%32, &rxdpVcatChRst);
    if (result) return result;
    if (rxdpVcatChRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x0, grpId, &reg0, &reg1);
    if (result) return result;
    mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_MODE_MSK;
    offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_MODE_OFF;

    if (mHYPHY20G_GET_FIELD4(reg0, mask, offset) != 0)
        return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure EXI Filter Parameters */
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x1, grpId, &reg0, &reg1);
    if (result) return result;

    switch (priority) {
        case 0:
            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_00001_IBIT_GFP_EXI_MTCH_AND_EXTN_HDR_SZ_EXH_EXI0_MSK,
                exiMatch);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_00001_IBIT_GFP_EXI_MTCH_AND_EXTN_HDR_SZ_EXH_LTH0_MSK,
                ehdrLen);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_00001_IBIT_GFP_EXI_MTCH_AND_EXTN_HDR_SZ_EXH_FDF0_MSK,
                fdfSend);
            if (result) return result;
            break;
        case 1:
            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_00001_IBIT_GFP_EXI_MTCH_AND_EXTN_HDR_SZ_EXH_EXI1_MSK,
                exiMatch);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_00001_IBIT_GFP_EXI_MTCH_AND_EXTN_HDR_SZ_EXH_LTH1_MSK,
                ehdrLen);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_00001_IBIT_GFP_EXI_MTCH_AND_EXTN_HDR_SZ_EXH_FDF1_MSK,
                fdfSend);
            if (result) return result;
            break;

        case 2:
            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_00001_IBIT_GFP_EXI_MTCH_AND_EXTN_HDR_SZ_EXH_EXI2_MSK,
                exiMatch);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_00001_IBIT_GFP_EXI_MTCH_AND_EXTN_HDR_SZ_EXH_LTH2_MSK,
                ehdrLen);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_00001_IBIT_GFP_EXI_MTCH_AND_EXTN_HDR_SZ_EXH_FDF2_MSK,
                fdfSend);
            if (result) return result;
            break;

        case 3:
            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_00001_IBIT_GFP_EXI_MTCH_AND_EXTN_HDR_SZ_EXH_EXI3_MSK,
                exiMatch);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_00001_IBIT_GFP_EXI_MTCH_AND_EXTN_HDR_SZ_EXH_LTH3_MSK,
                ehdrLen);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_00001_IBIT_GFP_EXI_MTCH_AND_EXTN_HDR_SZ_EXH_FDF3_MSK,
                fdfSend);
            if (result) return result;
            break;
    }

    result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x1, grpId, reg0, reg1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpFExiFilterCfg */

/*******************************************************************************
**
**  hyPhy20gGfpFPtiUpiFilterCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the PTI/UPI Match filter within the RXDP64 for
**                  processing incoming GFP frames.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - CPBID of the channel being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              priority    - Selects which of the 4 stages of filtering to
**                            configure.  Stage 0 has the highest priority, and
**                            stage 3 has the lowest priority.
**                          - Valid values are 0-3
**              upiMatch    - Field to be matched to incoming frame's UPI field.
**                          - Valid values are TBD
**              ptiMatch    - Field to be matched to incoming frame's PTI field.
**                          - Valid values are TBD
**              ptiEn       - Enables PTI checking
**                          - 0 - PTI match is always assumed to be true
**                          - 1 - PTI match is based on incoming frame
**              upiEn       - Enables UPI checking
**                          - 0 - UPI match is always assumed to be true
**                          - 1 - UPI match is based on incoming frame
**              fdfSend     - Selects whether matched frames should also be
**                            forwarded to the FDF interface for uP access
**                          - 0 - Do not send matching frames to FDF
**                          - 1 - Send matching frames to FDF
**              dropEn      - Selects whether matched frames should be dropped
**                          - 0 - Do not drop matching frames
**                          - 1 - Drop Matching Frames
**              csfEn       - Selects whether matched frames should should cause
**                            a CSFI interrupt to be raised
**                          - 0 - Do not raise interrupt for matching frames
**                          - 1 - rais interrupt for Matching Frames
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpFPtiUpiFilterCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 priority, UINT4 upiMatch,
                                 UINT4 ptiMatch, UINT4 ptiEn, UINT4 upiEn,
                                 UINT4 fdfSend, UINT4 dropEn, UINT4 csfEn)
{
    UINT4 regAddr = 0;
    UINT4 gfpFrmRst = 0;
    UINT4 gfpTraRst = 0;
    UINT4 rxdpCpbidChRst = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(priority > 0x3)||(upiMatch > 0xFF)||(ptiMatch > 0x7)||(ptiEn > 1)||(upiEn > 1)||(fdfSend > 1)||(dropEn > 1)||(csfEn > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check RXDP64 CPBID_CHN_RST setting */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CPBID_CHNL_RST_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr, cpbId%32, &rxdpCpbidChRst);
    if (result) return result;
    if (rxdpCpbidChRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure PTI/UPI Filter Parameters */
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x21, cpbId, &reg0, &reg1);
    if (result) return result;

    switch (priority) {
        case 0:
            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTIUPI_FDF0_MSK,
                fdfSend);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTIUPI_DROP0_MSK,
                dropEn);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTIUPI_CSF0_MSK,
                csfEn);
            if (result) return result;

            if (ptiEn) {
                result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTI0_ENB_MSK,
                0);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTI0_ENB_MSK,
                1);
                if (result) return result;
            }

            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTI0_MSK,
                ptiMatch);
            if (result) return result;

            if (upiEn) {
                result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_UPI0_ENB_MSK,
                0);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_UPI0_ENB_MSK,
                1);
                if (result) return result;
            }

            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_UPI0_MSK,
                upiMatch);
            if (result) return result;
            break;

        case 1:
            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTIUPI_FDF1_MSK,
                fdfSend);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTIUPI_DROP1_MSK,
                dropEn);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTIUPI_CSF1_MSK,
                csfEn);
            if (result) return result;

            if (ptiEn) {
                result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTI1_ENB_MSK,
                0);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTI1_ENB_MSK,
                1);
                if (result) return result;
            }

            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTI1_MSK,
                ptiMatch);
            if (result) return result;

            if (upiEn) {
                result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_UPI1_ENB_MSK,
                0);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_UPI1_ENB_MSK,
                1);
                if (result) return result;
            }

            result = hyPhy20gVarFieldWrite(&reg0,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_UPI1_MSK,
                upiMatch);
            if (result) return result;
            break;

        case 2:
            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTIUPI_FDF2_MSK,
                fdfSend);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTIUPI_DROP2_MSK,
                dropEn);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTIUPI_CSF2_MSK,
                csfEn);
            if (result) return result;

            if (ptiEn) {
                result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTI2_ENB_MSK,
                0);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTI2_ENB_MSK,
                1);
                if (result) return result;
            }

            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTI2_MSK,
                ptiMatch);
            if (result) return result;

            if (upiEn) {
                result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_UPI2_ENB_MSK,
                0);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_UPI2_ENB_MSK,
                1);
                if (result) return result;
            }

            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_UPI2_MSK,
                upiMatch);
            if (result) return result;
            break;

        case 3:
            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTIUPI_FDF3_MSK,
                fdfSend);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTIUPI_DROP3_MSK,
                dropEn);
            if (result) return result;

            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTIUPI_CSF3_MSK,
                csfEn);
            if (result) return result;

            if (ptiEn) {
                result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTI3_ENB_MSK,
                0);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTI3_ENB_MSK,
                1);
                if (result) return result;
            }

            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_PTI3_MSK,
                ptiMatch);
            if (result) return result;

            if (upiEn) {
                result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_UPI3_ENB_MSK,
                0);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_UPI3_ENB_MSK,
                1);
                if (result) return result;
            }

            result = hyPhy20gVarFieldWrite(&reg1,
                HYPHY20G_GFP_FRM_RXDP_PG_ADR_100001_IBIT_PTI_UPI_FLT_UPI3_MSK,
                upiMatch);
            if (result) return result;
            break;
    }

    result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x21, cpbId, reg0, reg1);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpFPtiUpiFilterCfg */

/*******************************************************************************
**
**  hyPhy20gGfpGenericFilterCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the Generic Match filter within the RXDP64 for
**                  processing incoming GFP/HDLC frames for a given group.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              grpId       - group ID of the group being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              genFilter   - 0 - Generic Filter 0
**                          - 1 - Generic Filter 1 (used only for HDLC)
**              index       - Index to byte # of frame where start of 4-byte
**                            filter is to be applied.
**                          - Valid values are 0-8
**                          - Value of 15 will disable the filter
**              data        - Specifies a 4-byte value to be compared against
**                            incoming frames
**                          - Valid values are 0x00000000 - 0xFFFFFFFF
**              include     - Specifies the bits that should be included in
**                            the matching algorithm (bit positions set to '0' will be
**                            excluded from the matching algorithm).
**                          - Valid values are 0x00000000 - 0xFFFFFFFF
**              reverse     - Selects whether result of filter match should be
**                            reversed
**                          - 0 - Do not reverse the filter match result
**                          - 1 - Reverse the filter match result
**              fdoSend     - Selects whether matched frames should be
**                            forwarded to the FDO interface for further
**                            datapath processing
**                          - 0 - Do not send matching frames to FDO
**                          - 1 - Send matching frames to FDO
**              fdfSend     - Selects whether matched frames should be
**                            forwarded to the FDF interface for uP access
**                          - 0 - Do not send matching frames to FDF
**                          - 1 - Send matching frames to FDF
**              dropEn      - Selects whether matched frames should be dropped
**                          - 0 - Do not drop matching frames
**                          - 1 - Drop Matching Frames
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpGenericFilterCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 grpId, UINT4 genFilter, UINT4 index,
                                 UINT4 data, UINT4 include, UINT4 reverse,
                                 UINT4 fdoSend, UINT4 fdfSend, UINT4 dropEn)
{
    UINT4 regAddr = 0;
    UINT4 gfpFrmRst = 0;
    UINT4 gfpTraRst = 0;
    UINT4 rxdpVcatChRst = 0;
    UINT4 maskByte = 0;
    UINT4 cfgReg0 = 0;
    UINT4 cfgReg1 = 0;
    UINT4 reg0 = 0;
    UINT4 reg1 = 0;
    UINT4 mask = 0;
    UINT4 offset = 0;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(grpId > 0x3F)||(genFilter > 1)||(index > 15)||(reverse > 1)||(fdoSend > 1)||(fdfSend > 1)||(dropEn > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if ((index > 8)&&(index < 15)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* pre-config checking */
    /* check GFP10_FRM_SFTWRST and GFP10_TRA_SFTWRST in GFP top level */
    regAddr = HYPHY20G_GFP_FRM_MTSB_BASE;
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_GFP10_FRM_SFTWRST_OFF,
                        &gfpFrmRst);
    if (result) return result;
    if (gfpFrmRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    regAddr = HYPHY20G_GFP_TRA_MTSB_BASE;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr,
                        HYPHY20G_GFP_TRA_TOP_BIT_MSTR_CTL_GFP10_TRA_SFTWRST_OFF,
                        &gfpTraRst);
    if (result) return result;
    if (gfpTraRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* check RXDP64 VCAT_CHN_RST settings */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_VCAT_GRP_RST_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr, grpId%32, &rxdpVcatChRst);
    if (result) return result;
    if (rxdpVcatChRst == 1) return HYPHY20G_ERR_INVALID_PRECONFIG;

    /* Configure Generic Filter Control Parameters */
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x2, grpId, &reg0, &reg1);
    if (result) return result;

    if (genFilter == 0) {
        /* Configure Generic Filter0 Control Parameters */
        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00010_IBIT_GNRC_FLTR_0_1_CTL_GF0_FDO_MSK,
            fdoSend);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00010_IBIT_GNRC_FLTR_0_1_CTL_GF0_FDF_MSK,
            fdfSend);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00010_IBIT_GNRC_FLTR_0_1_CTL_GF0_DROP_MSK,
            dropEn);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00010_IBIT_GNRC_FLTR_0_1_CTL_GF0_REV_MSK,
            reverse);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00010_IBIT_GNRC_FLTR_0_1_CTL_GF0_IDX_MSK,
            index);
        if (result) return result;

        result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x2, grpId, reg0, reg1);
        if (result) return result;

        /* Configure Generic Filter0 Data Parameters */
        result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x3, grpId, &reg0, &reg1);
        if (result) return result;


        maskByte = mHYPHY20G_GET_FIELD4(include, (0xFF << 24), 24);
        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_MASK0_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(include, (0xFF << 16), 16);
        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_MASK1_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(data, (0xFF << 24), 24);
        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_BYTE0_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(data, (0xFF << 16), 16);
        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_BYTE1_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(include, (0xFF << 8), 8);
        result = hyPhy20gVarFieldWrite(&reg1,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_MASK2_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(include, (0xFF << 0), 0);
        result = hyPhy20gVarFieldWrite(&reg1,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_MASK3_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(data, (0xFF << 8), 8);
        result = hyPhy20gVarFieldWrite(&reg1,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_BYTE2_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(data, (0xFF << 0), 0);
        result = hyPhy20gVarFieldWrite(&reg1,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_BYTE3_MSK,
            maskByte);
        if (result) return result;

        result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x3, grpId, reg0, reg1);
        if (result) return result;

    } else {
        /* Confirm group is configured for HDLC */
        result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x0, grpId, &cfgReg0, &cfgReg1);
        if (result) return result;
        mask = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_MODE_MSK;
        offset = HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_MODE_OFF;

        if (mHYPHY20G_GET_FIELD4(cfgReg0, mask, offset) != 1)
            return HYPHY20G_ERR_INVALID_PRECONFIG;

        /* Configure Generic Filter1 Control Parameters */
        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00010_IBIT_GNRC_FLTR_0_1_CTL_GF1_FDO_MSK,
            fdoSend);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00010_IBIT_GNRC_FLTR_0_1_CTL_GF1_FDF_MSK,
            fdfSend);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00010_IBIT_GNRC_FLTR_0_1_CTL_GF1_DROP_MSK,
            dropEn);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00010_IBIT_GNRC_FLTR_0_1_CTL_GF1_REV_MSK,
            reverse);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00010_IBIT_GNRC_FLTR_0_1_CTL_GF1_IDX_MSK,
            index);
        if (result) return result;

        result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x2, grpId, reg0, reg1);
        if (result) return result;

        /* Configure Generic Filter1 Data Parameters */
        result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x4, grpId, &reg0, &reg1);
        if (result) return result;


        maskByte = mHYPHY20G_GET_FIELD4(include, (0xFF << 24), 24);
        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_MASK0_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(include, (0xFF << 16), 16);
        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_MASK1_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(data, (0xFF << 24), 24);
        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_BYTE0_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(data, (0xFF << 16), 16);
        result = hyPhy20gVarFieldWrite(&reg0,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_BYTE1_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(include, (0xFF << 8), 8);
        result = hyPhy20gVarFieldWrite(&reg1,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_MASK2_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(include, (0xFF << 0), 0);
        result = hyPhy20gVarFieldWrite(&reg1,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_MASK3_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(data, (0xFF << 8), 8);
        result = hyPhy20gVarFieldWrite(&reg1,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_BYTE2_MSK,
            maskByte);
        if (result) return result;

        maskByte = mHYPHY20G_GET_FIELD4(data, (0xFF << 0), 0);
        result = hyPhy20gVarFieldWrite(&reg1,
            HYPHY20G_GFP_FRM_RXDP_PG_ADR_00011_IBIT_GEN_FLT_0_DAT_GF0_BYTE3_MSK,
            maskByte);
        if (result) return result;

        result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x4, grpId, reg0, reg1);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpGenericFilterCfg */


/*******************************************************************************
**
**  hyPhy20gGfpTxChAdd
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the calendar-based scheduler (SCBS) within the
**                  TxGFPT block to add a particular GFP/HDLC
**                  CPBID channel to a particular outgoing VCAT group.
**
**                  This procedure also enables GFP multiplexing when
**                  multiple GFP channels with unique CIDs are mapped to the
**                  same Group ID.
**
**                  This function is used mutually exclusively with
**                  hyPhy20gGfpRateAwareTxChAdd.
**
**                  The user must ensure that hyPhy20gCpbChnlProv or
**                  hyPhy20gCpbNonPrimRdChnlProv has been called for the CPB
**                  channel prior to calling this function.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - CPBID of the channel being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              grpId       - VCAT group ID to assign this channel to
**                          - valid values are 0x0 - 0x3F (0-63)
**              chType      - 0 - GFP-F Multiple Channels per group or
**                                single channel with data rate less than 50%
**                                of group rate.
**                          - 1 - GFP-T (FA) Multiple Channels per group or
**                                single channel with data rate less than 50%
**                                of group rate.
**                          - 2 - HDLC Channel with channel data rate less
**                                than 50% of group rate.
**                          - 3 - GFP-F Single Channel per group with
**                                channel data rate greater than 50% of 
**                                group rate.
**                          - 4 - HDLC Channel with channel data rate greater
**                                than 50% of group rate.
**                          - 5 - GFP-T (FA) Single Channel per group with
**                                channel data rate greater than 50% of 
**                                group rate.
**                          - 6 - GFP-T (AA)
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_EVALUATING_SCHD
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpTxChAdd(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 grpId, UINT4 chType)
{
    INT4 result;
    UINT4 regAddr = 0;
    UINT4 calSize = 0;
    UINT4 size = 0;
    UINT4 grpToModifyStart = 0;
    UINT4 grpToModifyEnd = 0;
    UINT4 rowsToShift = 0;
    UINT4 unusedRows = 0;
    UINT4 firstUnused = 0;
    UINT4 firstRow = 0;
    UINT4 lastRow = 0;
    UINT4 numRows = 0;
    UINT4 currGrpId = 0;
    UINT4 currGrpStart = 0;
    UINT4 cpbChOffset = 0;
    UINT4 firstCh = 0;
    UINT4 grpStart = 0;
    UINT4 chIdOffset = 0;
    UINT4 chIdMask = 0;
    UINT4 procChanId = 0;
    UINT4 disableSnglChnl = 0;
    UINT4 chGrpLut = 0;
    UINT4 snglChEn = 0;
    UINT4 i = 0;
    UINT4 mask = 0;
    UINT4 compress = 0;
    UINT4 cpbGfpConfig = 0;
    UINT4 off = 0;
    UINT4 enetPath;

    UINT4 *pDummyCalMem;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_SCHD_GRP_CTXT *pGrpCtxt;
    sHYPHY20G_GFP_SCBS_ROW_CTXT *pCurrGrpId;
    sHYPHY20G_GFP_SCBS_ROW_CTXT *pGfpRowCtxt;
    sHYPHY20G_SCHD_GRP_CTXT *pCurrGrpCtxt;
    sHYPHY20G_RD_CHNL_CTXT *pRdChnlCtxt = NULL;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(grpId > 0x3F)||(chType > 6)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

   /* Create a dummy calendar page for later */
    calSize = HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW * HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW;
    size = calSize * sizeof(UINT4);
    pDummyCalMem = (UINT4 *)sysHyPhy20gMemAlloc(size);
    if(pDummyCalMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* Grab pointer to GFP Slice context*/
    if (slice==0) {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpACtxt);
    } else {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpBCtxt);
    }

    /* Confirm GFP Slice Context is valid */
    if(pGfpCtxt == NULL){
        sysHyPhy20gMemFree(pDummyCalMem);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Grab the scheduler data for the GFP SCBS being configured */
    pSchd = &(pGfpCtxt->schd);


    /* Grab the group data for the GFP SCBS being configured */
    pGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)(pSchd->pScbsGrpCtxt + grpId);


    /* For the group being modified, find the current number of
       rows assigned to the group */
    grpToModifyStart = pGrpCtxt->startAddr;
    grpToModifyEnd = pGrpCtxt->endAddr;

    /* Calculate number of additional rows that the group will need*/
    if (grpToModifyStart == 0x7F) {
        /* Group is not currently in use - assign 1 row for 1st channel */
        rowsToShift = 0;
    } else {
        if (grpToModifyStart == grpToModifyEnd) {
            /*Group contains 1 channel. Add 3 rows for a 2-channel group */
            rowsToShift = 3;
        } else {
            /*Group contains multiple channels. Add 2 rows for the new channel*/
            rowsToShift = 2;
        }
    }


    /* Determine the amount of free space available in the SCBS */
    result = gfpHyPhy20gGfpScbsFreeSpace(fileHndl, pGfpCtxt,
                                         &unusedRows, &firstUnused);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    /* Check to make sure that there is enough free space in the SCBS */
    if (unusedRows < rowsToShift) {
        sysHyPhy20gMemFree(pDummyCalMem);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check to make sure that there is enough free space in the SCBS */
    if ((unusedRows == 0)&&(rowsToShift == 0)) {
        sysHyPhy20gMemFree(pDummyCalMem);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* check to see whether all 127 rows of SCBS will now be in use */
    if (unusedRows == rowsToShift) {
      rowsToShift--;
      compress = 1;
    }

    /* Shift down all groups below the group being modified to make space
       for the new channel data */
    if(rowsToShift == 0) {
        /* Group is currently not configured.  Add to the end of the scheduler */
        firstRow = firstUnused;
        lastRow = firstUnused;
        grpToModifyStart = firstUnused;
        grpToModifyEnd = firstUnused;
     } else {
        firstRow = grpToModifyEnd;
        lastRow = firstUnused - 1;
     }


    /* Read ENET_DATAPATH_EN to determine the source of data to GFP */
    regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_CFG;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    off = HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_CFG_ENET_DATAPATH_EN_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, off, &enetPath);
    
    if (result) {
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }
    
    /* Check CPB context only if the source of data to GFP is from CPB */   
    if (enetPath != 1) { /* source of data from CPB */  
        /* Grab pointer to CPB Rd Channel Context */
        cpbChOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET
                    + slice*HYPHY20G_CPB_GFPA_WR_SCHD_MAX_CHNL + cpbId;
        pRdChnlCtxt = (sHYPHY20G_RD_CHNL_CTXT *)
                                       (pDevCtxt->pCpbCtxt->pRdChnl + cpbChOffset);
        
         /* Check to make sure channel is not already part of another group */
        if (pRdChnlCtxt->gfpGrpId != HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER) {
            sysHyPhy20gMemFree(pDummyCalMem);
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
        
         /* Check to make sure channel has been provisioned in CPB */
        if (pRdChnlCtxt->state == HYPHY20G_CPB_CHNL_START) {
            sysHyPhy20gMemFree(pDummyCalMem);
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
    }
    
    /* Configure 'Single Channel LUT' parameters */
    switch (chType) {
        case 0: /* GFP-F Multiple Channels */
        case 3: /* GFP-F Single Channel mapped into full 2.5G or 10G payload */
          /* GFP-F Channel.  Use Single Channel Scheduling mode only
             if group is currently empty */
          if (rowsToShift == 0) {

              regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_TO_LUT_GROUPS_3_0 + 0x4*(grpId/4);
              regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              result = sysHyPhy20gRead(fileHndl, regAddr, &chGrpLut);

              mask = 0x3F << (8*(grpId%4));
              result = hyPhy20gVarFieldWrite(&chGrpLut, mask, cpbId);
              if (result) {
                sysHyPhy20gMemFree(pDummyCalMem);
                return result;
              }
              
              result = sysHyPhy20gWrite(fileHndl, regAddr, chGrpLut);
              if (result) {
                sysHyPhy20gMemFree(pDummyCalMem);
                return result;
              }

              regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_EN_GROUPS_31_TO_0 + 0x4*(grpId/32);
              regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
              if (result) {
                sysHyPhy20gMemFree(pDummyCalMem);
                return result;
              }

              /* Configure GFP_RD_THRESH in CPB */
              regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
              mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
              if (chType == 3) {
                  result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0xF);
                  if (result) {
                    sysHyPhy20gMemFree(pDummyCalMem);
                    return result;
                  } 
              } else {
                  result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x1A);
                  if (result) {
                    sysHyPhy20gMemFree(pDummyCalMem);
                    return result;
                  }
              }
          } else {
              disableSnglChnl = 1;
              /* Configure GFP_RD_THRESH in CPB */
              regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
              result = sysHyPhy20gRead(fileHndl, regAddr, &cpbGfpConfig);
              mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
              result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x1A);
              if (result) {
                sysHyPhy20gMemFree(pDummyCalMem);
                return result;
              }     
          }
          /* Configure GFP_RD_OUTPUT_LENGTH
             This bit must be set for GFP-F and OTN mapping modes.  This bit
             must be cleared for GFP-T and HDLC mapping modes */
          regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
          mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_OUTPUT_LENGTH_MSK;
          result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 1);
          if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
          }
          break;
      case 1: /* GFP-T (FA) Multiple Channels */
      case 5: /* GFP-T (FA) Single Channel per group */
      case 6: /* GFP-T (AA) */
          /* GFP-T Channel.  Always use SCBS Scheduling Mode */
          regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_EN_GROUPS_31_TO_0 + 0x4*(grpId/32);
          regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
          result = sysHyPhy20gBitRead(fileHndl, regAddr, grpId%32, &snglChEn);
          if (result) {
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }
          if (snglChEn == 1) {
              sysHyPhy20gMemFree(pDummyCalMem);
              return HYPHY20G_ERR_INVALID_PRECONFIG;
          }
          
          /* Configure GFP_RD_THRESH in CPB */
          regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
          mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
          if (chType == 1) {
              result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x1A);
              if (result) {
                  sysHyPhy20gMemFree(pDummyCalMem);
                  return result;
              }
          } else if (chType == 5) {
              result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0xF);
              if (result) {
                  sysHyPhy20gMemFree(pDummyCalMem);
                  return result;
              }
          } else {
              result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x1);
              if (result) {
                  sysHyPhy20gMemFree(pDummyCalMem);
                  return result;
              }
          }   
          /* Configure GFP_RD_OUTPUT_LENGTH
             This bit must be set for GFP-F and OTN mapping modes.  This bit
             must be cleared for GFP-T and HDLC mapping modes */
          regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
          mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_OUTPUT_LENGTH_MSK;
          result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0);
          if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
          }  
        break;
      case 2: /* HDLC Channel non-full bandwidth */
      case 4: /* HDLC Channel mapped into full 2.5G or 10G payload */
          /* HDLC Channel.  Always use Single Channel Scheduling Mode */
          regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_EN_GROUPS_31_TO_0 + 0x4*(grpId/32);
          regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
          result = sysHyPhy20gBitRead(fileHndl, regAddr, grpId%32, &snglChEn);
          if (result) {
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }
          if (snglChEn == 1) {
              sysHyPhy20gMemFree(pDummyCalMem);
              return HYPHY20G_ERR_INVALID_PRECONFIG;
          }
          regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_TO_LUT_GROUPS_3_0 + 0x4*(grpId/4);
          regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
          result = sysHyPhy20gRead(fileHndl, regAddr, &chGrpLut);

          mask = 0x3F << (8*(grpId%4));
          result = hyPhy20gVarFieldWrite(&chGrpLut, mask, cpbId);
          if (result) {
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }

          result = sysHyPhy20gWrite(fileHndl, regAddr, chGrpLut);
          if (result) {
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }

          regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_EN_GROUPS_31_TO_0 + 0x4*(grpId/32);
          regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
          result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
          if (result) {
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }
          /* Configure GFP_RD_THRESH in CPB */
          regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
          mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
          result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0xF);
          if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
          }
          /* Configure GFP_RD_OUTPUT_LENGTH
             This bit must be set for GFP-F and OTN mapping modes.  This bit
             must be cleared for GFP-T and HDLC mapping modes */
          regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
          mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_OUTPUT_LENGTH_MSK;
          result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0);
          if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
          }
          break;
    }
    
    /* Perform only if CPB context exists */
    if (pRdChnlCtxt != NULL) { /* data is from CPB */        
        /* Add GRPID to channel-based device context */
        pRdChnlCtxt->gfpGrpId = grpId;
    }
        
    while (lastRow > firstRow) {
        pCurrGrpId = (sHYPHY20G_GFP_SCBS_ROW_CTXT *)&(pGfpCtxt->pGfpRowCtxt[lastRow]);
        currGrpId = pCurrGrpId->grpId;

        pCurrGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)
                            (pSchd->pScbsGrpCtxt + currGrpId);
        currGrpStart = pCurrGrpCtxt->startAddr;

        result = gfpHyPhy20gGfpScbsGrpShift(fileHndl, pGfpCtxt,
                                            currGrpId, rowsToShift, 0);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
        lastRow = currGrpStart - 1;
    }

    /* Get scheduler data for this group with the new addition */
    result = gfpHyPhy20gGfpGenerateSchdData(fileHndl, slice, grpId, compress,
                                            &numRows, pDummyCalMem);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    /* Update group pointers for new group size */
    grpToModifyEnd = grpToModifyEnd + rowsToShift;

    result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, grpId,
                                           grpToModifyStart, grpToModifyEnd);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    /* update GFP Row Context data for the group being modified */
    for (i = grpToModifyStart; i<=grpToModifyEnd; i++) {
        pGfpRowCtxt = (sHYPHY20G_GFP_SCBS_ROW_CTXT *)&(pGfpCtxt->pGfpRowCtxt[i]);
        pGfpRowCtxt->grpId = grpId;
    }


    /* Update Calendar Data */
    result = utilHyPhy20gScbsUpdateCalRows(fileHndl, pSchd, numRows, 0,
                                           grpToModifyStart, pDummyCalMem);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }


    /* TxDP Config   */
    /* Update PROC_CHAN_ID with a valid channel from the group */
    grpStart = pGrpCtxt->startAddr;
    if (grpStart != HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW) {
        result = utilHyPhy20gScbsReadSchdCal(fileHndl, pSchd, grpStart, 0, &firstCh);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }

        regAddr = HYPHY20G_GFP_FRM_TXDP_REG_PROC_CHNL_ID_OFFSET(grpId/4);
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gRead(fileHndl, regAddr, &procChanId);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }

        chIdOffset = 8*(grpId%4);
        chIdMask = 0x3F << chIdOffset;
        result = hyPhy20gVarFieldWrite(&procChanId, chIdMask, firstCh);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }

        result = sysHyPhy20gWrite(fileHndl, regAddr, procChanId);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
    }

    /* TXDP64 - Make sure that GRP_RST is '1' */
    regAddr = HYPHY20G_GFP_FRM_TXDP_REG_GRP_RST_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    /* TxGFPT Config   */
    /* Set GRP_RST to '0' */
    regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_GRP_RST_GROUPS_31_TO_0;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    if (grpId > 31)
        regAddr = regAddr + 4;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    /* Set CHAN_EN to '1' */
    regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_CHNL_EN_CHANNELS_31_TO_0;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    if (cpbId > 31)
        regAddr = regAddr + 4;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 1);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }


    if (disableSnglChnl) {
          regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_EN_GROUPS_31_TO_0 + 0x4*(grpId/32);
          regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
          result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
          if (result){
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }
    }

    sysHyPhy20gMemFree(pDummyCalMem);

    return HYPHY20G_SUCCESS;
}  /* hyPhy20gGfpTxChAdd */


/*******************************************************************************
**
**  hyPhy20gGfpRateAwareTxChAdd
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the calendar-based scheduler (SCBS) within the
**                  TxGFPT block to add a particular GFP/HDLC
**                  CPBID channel to a particular outgoing VCAT group.
**
**                  This procedure also enables GFP multiplexing when
**                  multiple GFP channels with unique CIDs are mapped to the
**                  same Group ID.
**
**                  This function is used mutually exclusively with
**                  hyPhy20gGfpTxChAdd.  This rate aware version will
**                  ensure that GFP channel multiplexing of CBR clients will
**                  have sufficient line bandwidth and SCBS resources to
**                  accept the addition of the channel, otherwise it will be
**                  rejected.
**
**                  For GFP-F packet mode, this function will not test for the
**                  interface rate and it will use the same SCBS content
**                  generation as hyPhy20gGfpTxChAdd.
**
**                  This function will reject the mixture of GFP-F Packet mode
**                  channels with CBR mode channels if channel multiplexing is
**                  being configured.  The group type will be determined based
**                  on the mode of the first added channel.
**
**                  The user must ensure that hyPhy20gCpbChnlProv or
**                  hyPhy20gCpbNonPrimRdChnlProv has been called for the CPB
**                  channel prior to calling this function.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - CPBID of the channel being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              grpId       - VCAT group ID to assign this channel to
**                          - valid values are 0x0 - 0x3F (0-63)
**              mode        - GFP Channel mode
**                            1 - HYPHY20G_GFP_T_AA
**                            2 - HYPHY20G_GFP_T_FA, channel multiplexed or
**                                single channel less than 50% of group rate
**                            3 - HYPHY20G_GFP_F_PKT
**                            4 - HYPHY20G_GFP_F_PHASE, channel multiplexed or
**                                single channel less than 50% of group rate
**                            5 - HDLC Channel with channel data rate less
**                                than 50% of possible group rate.
**                            6 - HYPHY20G_GFP_F_PHASE Single Channel per group
**                                with channel data rate greater than 50% of 
**                                group rate.
**                            7 - HDLC Channel with channel data rate greater
**                                than 50% of group rate.
**                            8 - HYPHY20G_GFP_T_FA Single Channel per group 
**                                with channel data rate greater than 50% of 
**                                group rate.
**              numSb       - For HYPHY20G_GFP_T_AA or HYPHY20G_GFP_T_FA
**                            set to the number of superblock used for
**                            GFP-T encapsulation.  Otherwise set to 0.
**              cpbPktLen   - Packet Length in CPB:
**                            - GFP-F Packet mode use min
**                            packet length
**                            - GFP-T and GFP-F phase mode use
**                            nominal PMM packet size
**              gfpEncapLen - Number bytes added by GFP subsystem:
**                            Header + Extension Header + FCS
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_INVALID_GFP_CHNL_MODES
**                  HYPHY20G_ERR_ZERO_DIVIDE
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_EVALUATING_SCHD
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpRateAwareTxChAdd(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 grpId, UINT4 mode,
                                 UINT4 numSb, UINT4 cpbPktLen,
                                 UINT4 gfpEncapLen)
{
    INT4 result;
    UINT4 regAddr = 0;
    UINT4 calSize = 0;
    UINT4 size = 0;
    UINT4 grpToModifyStart = 0;
    UINT4 grpToModifyEnd = 0;
    UINT4 rowsToShift = 0;
    UINT4 unusedRows = 0;
    UINT4 firstUnused = 0;
    UINT4 firstRow = 0;
    UINT4 lastRow = 0;
    UINT4 numRows = 0;
    UINT4 currGrpId = 0;
    UINT4 currGrpStart = 0;
    UINT4 cpbChOffset = 0;
    UINT4 firstCh = 0;
    UINT4 grpStart = 0;
    UINT4 chIdOffset = 0;
    UINT4 chIdMask = 0;
    UINT4 procChanId = 0;
    UINT4 disableSnglChnl = 0;
    UINT4 chGrpLut = 0;
    UINT4 snglChEn = 0;
    UINT4 i = 0;
    UINT4 mask = 0;
    UINT4 compress = 0;
    UINT4 pOptimal;
    UINT4 chType;
    UINT4 off = 0;
    UINT4 enetPath = 0;    
    UINT4 genMode = 0;

    UINT4 *pDummyCalMem;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_SCHD_GRP_CTXT *pGrpCtxt;
    sHYPHY20G_GFP_SCBS_ROW_CTXT *pCurrGrpId;
    sHYPHY20G_GFP_SCBS_ROW_CTXT *pGfpRowCtxt;
    sHYPHY20G_SCHD_GRP_CTXT *pCurrGrpCtxt;
    sHYPHY20G_RD_CHNL_CTXT *pRdChnlCtxt = NULL;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(grpId > 0x3F)||(mode > 8)||(mode < 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

   /* Create a dummy calendar page for later */
    calSize = HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW * HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW;
    size = calSize * sizeof(UINT4);
    pDummyCalMem = (UINT4 *)sysHyPhy20gMemAlloc(size);
    if(pDummyCalMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    /* Grab pointer to GFP Slice context*/
    if (slice==0) {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpACtxt);
    } else {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpBCtxt);
    }

    /* Confirm GFP Slice Context is valid */
    if(pGfpCtxt == NULL){
        sysHyPhy20gMemFree(pDummyCalMem);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Grab the scheduler data for the GFP SCBS being configured */
    pSchd = &(pGfpCtxt->schd);


    /* Grab the group data for the GFP SCBS being configured */
    pGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)(pSchd->pScbsGrpCtxt + grpId);


    /* For the group being modified, find the current number of
       rows assigned to the group */
    grpToModifyStart = pGrpCtxt->startAddr;
    grpToModifyEnd = pGrpCtxt->endAddr;

    /* Calculate number of additional rows that the group will need*/
    if (grpToModifyStart == 0x7F) {
        /* Group is not currently in use - assign 1 row for 1st channel */
        rowsToShift = 0;
    } else {
        if (grpToModifyStart == grpToModifyEnd) {
            /*Group contains 1 channel. Add 3 rows for a 2-channel group */
            rowsToShift = 3;
        } else {
            /*Group contains multiple channels. Add 2 rows for the new channel*/
            rowsToShift = 2;
        }
    }


    /* Determine the amount of free space available in the SCBS */
    result = gfpHyPhy20gGfpScbsFreeSpace(fileHndl, pGfpCtxt,
                                         &unusedRows, &firstUnused);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    /* Check to make sure that there is enough free space in the SCBS */
    if (unusedRows < rowsToShift) {
        sysHyPhy20gMemFree(pDummyCalMem);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check to make sure that there is enough free space in the SCBS */
    if ((unusedRows == 0)&&(rowsToShift == 0)) {
        sysHyPhy20gMemFree(pDummyCalMem);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* check to see whether all 127 rows of SCBS will now be in use */
    if (unusedRows == rowsToShift) {
      rowsToShift--;
      compress = 1;
    }

    /* Shift down all groups below the group being modified to make space
       for the new channel data */
    if(rowsToShift == 0) {
        /* Group is currently not configured.  Add to the end of the scheduler */
        firstRow = firstUnused;
        lastRow = firstUnused;
        grpToModifyStart = firstUnused;
        grpToModifyEnd = firstUnused;
     } else {
        firstRow = grpToModifyEnd;
        lastRow = firstUnused - 1;
     }

    /* Read ENET_DATAPATH_EN to determine the source of data to GFP */
    regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_CFG;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    off = HYPHY20G_GFP_TRA_TXGFPT_TOP_BIT_CFG_ENET_DATAPATH_EN_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, off, &enetPath);
    if (result) {
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }
    
    /* Check the CPB context only if the source of data to GFP is from CPB */    
    if (enetPath != 1) {
        /* Grab pointer to CPB Rd Channel Context */
        cpbChOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET
                    + slice*HYPHY20G_CPB_GFPA_WR_SCHD_MAX_CHNL + cpbId;
        pRdChnlCtxt = (sHYPHY20G_RD_CHNL_CTXT *)
                                       (pDevCtxt->pCpbCtxt->pRdChnl + cpbChOffset);
        
         /* Check to make sure channel is not already part of another group */
        if (pRdChnlCtxt->gfpGrpId != HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER) {
            sysHyPhy20gMemFree(pDummyCalMem);
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
        
         /* Check to make sure channel has been provisioned in CPB */
        if (pRdChnlCtxt->state == HYPHY20G_CPB_CHNL_START) {
            sysHyPhy20gMemFree(pDummyCalMem);
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;
        }   
    }

    /* relate mode to chType */
    switch (mode) {
        case 0:
            sysHyPhy20gMemFree(pDummyCalMem);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 1: /* HYPHY20G_GFP_T_AA */
            chType = 6; /* GFP-T (AA) Channel */
            break;
        case 2: /* HYPHY20G_GFP_T_FA Multiple Channels */
            chType = 1; /* GFP-T Multiple Channels */
            break;
        case 3: /* HYPHY20G_GFP_F_PKT */
        case 4: /* HYPHY20G_GFP_F_PHASE */
            chType = 0; /* GFP-F Multiple Channels */
            break;
        case 5: /* HDLC (single channel non-full bandwidth) */
        case 6: /* GFP-F (single channel full rate) */
        case 7: /* HDLC (single channel full rate) */
        case 8: /* GFP-T (FA) (single channel) */
            chType = mode - 3; /* 2 - HDLC Channel non-full bandwidth
                                  3 - GFP-F Single Channel mapped into full 2.5G or 10G payload
                                  4 - HDLC Channel mapped into full 2.5G or 10G payload
                                  5 - GFP-T (FA) Single Channel per group */
            break;
        default:
            sysHyPhy20gMemFree(pDummyCalMem);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Configure 'Single Channel LUT' parameters */
    switch (chType) {
        case 0: /* GFP-F Channel */
        case 3: /* GFP-F Single Channel mapped into full 2.5G or 10G payload */
          /* GFP-F Channel.  Use Single Channel Scheduling mode only
             if group is currently empty */
          if (rowsToShift == 0) {

              regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_TO_LUT_GROUPS_3_0 + 0x4*(grpId/4);
              regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              result = sysHyPhy20gRead(fileHndl, regAddr, &chGrpLut);

              mask = 0x3F << (8*(grpId%4));
              result = hyPhy20gVarFieldWrite(&chGrpLut, mask, cpbId);
              if (result){
                 sysHyPhy20gMemFree(pDummyCalMem);
                 return result;
              }

              result = sysHyPhy20gWrite(fileHndl, regAddr, chGrpLut);
              if (result){
                 sysHyPhy20gMemFree(pDummyCalMem);
                 return result;
              }

              regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_EN_GROUPS_31_TO_0 + 0x4*(grpId/32);
              regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
              result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
              if (result){
                 sysHyPhy20gMemFree(pDummyCalMem);
                 return result;
              }

              /* Configure GFP_RD_THRESH in CPB */
              regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
              mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
              if (chType == 3) {
                  result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0xF);
                  if (result) {
                    sysHyPhy20gMemFree(pDummyCalMem);
                    return result;
                  }
              } else {
                  result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x1A);
                  if (result) {
                    sysHyPhy20gMemFree(pDummyCalMem);
                    return result;
                  }
              }
          } else {
              disableSnglChnl = 1;
              /* Configure GFP_RD_THRESH in CPB */
              regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
              mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
              result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x1A);
              if (result) {
                sysHyPhy20gMemFree(pDummyCalMem);
                return result;
              }
          }
          /* Configure GFP_RD_OUTPUT_LENGTH
             This bit must be set for GFP-F and OTN mapping modes.  This bit
             must be cleared for GFP-T and HDLC mapping modes */
          regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
          mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_OUTPUT_LENGTH_MSK;
          result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 1);
          if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
          }
          break;
      case 1: /* GFP-T (FA) Multiple Channels */
      case 5: /* GFP-T (FA) single channel per group */
      case 6: /* GFP-T (AA) Channel */
          /* GFP-T Channel.  Always use SCBS Scheduling Mode */
          regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_EN_GROUPS_31_TO_0 + 0x4*(grpId/32);
          regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
          result = sysHyPhy20gBitRead(fileHndl, regAddr, grpId%32, &snglChEn);
          if (result){
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }
          if (snglChEn == 1) {
              /* if SINGLE_CHAN_GRP_EN set to 1 implies mixed group
                 with GFP-T FA and GFP-F Phase mode members.  In these
                 cases this field must be set to 0. */
              disableSnglChnl = 1;

          }
          
          /* Configure GFP_RD_THRESH in CPB */
          if (chType == 1) { 
              regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
              mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
              result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x1A);
              if (result) {
                sysHyPhy20gMemFree(pDummyCalMem);
                return result;
              }
          } else if (chType == 5) {
              regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
              mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
              result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0xF);
              if (result) {
                sysHyPhy20gMemFree(pDummyCalMem);
                return result;
              }
          } else {
              regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
              mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
              result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x1);
              if (result) {
                sysHyPhy20gMemFree(pDummyCalMem);
                return result;
              }
          }
          /* Configure GFP_RD_OUTPUT_LENGTH
             This bit must be set for GFP-F and OTN mapping modes.  This bit
             must be cleared for GFP-T and HDLC mapping modes */
          regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
          mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_OUTPUT_LENGTH_MSK;
          result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0);
          if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
          }
        break;
      case 2: /* HDLC Channel non-full bandwidth */
      case 4: /* HDLC Channel mapped into full 2.5G or 10G payload */
          /* HDLC Channel.  Always use Single Channel Scheduling Mode */
          regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_EN_GROUPS_31_TO_0 + 0x4*(grpId/32);
          regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
          result = sysHyPhy20gBitRead(fileHndl, regAddr, grpId%32, &snglChEn);
          if (result){
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }
          if (snglChEn == 1) {
              sysHyPhy20gMemFree(pDummyCalMem);
              DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;
          }

          regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_TO_LUT_GROUPS_3_0 + 0x4*(grpId/4);
          regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
          result = sysHyPhy20gRead(fileHndl, regAddr, &chGrpLut);

          mask = 0x3F << (8*(grpId%4));
          result = hyPhy20gVarFieldWrite(&chGrpLut, mask, cpbId);
          if (result){
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }

          result = sysHyPhy20gWrite(fileHndl, regAddr, chGrpLut);
          if (result){
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }

          regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_EN_GROUPS_31_TO_0 + 0x4*(grpId/32);
          regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
          result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
          if (result){
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }

          /* Configure GFP_RD_THRESH in CPB */
          regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
          mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_THRESH_MSK;
          result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0xF);
          if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
          }
          /* Configure GFP_RD_OUTPUT_LENGTH
             This bit must be set for GFP-F and OTN mapping modes.  This bit
             must be cleared for GFP-T and HDLC mapping modes */
          regAddr = HYPHY20G_CPB_CPB_REG_GFP_PRT_CFG_OFFSET(cpbId, slice);
          mask = HYPHY20G_CPB_CPB_BIT_GFP_PRT_CFG_GFP_RD_OUTPUT_LENGTH_MSK;
          result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0);
          if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
          }
          break;
    }

    /* Perform only if CPB context exists */
    if (pRdChnlCtxt != NULL) { /* data is from CPB */
        /* Add GRPID to channel-based device context */
        pRdChnlCtxt->gfpGrpId = grpId;
    }

    /* Get scheduler data for this group with the new addition */
    /* argument mode must be adjusted for usage of mode in this API */
    /* relate mode to chType */
    switch (mode) {
        /* gfpHyPhy20gGfpGenerateRateAwareSchdData required mode:
                  mode        - For addRemove set to 1:
                                GFP Channel mode
                                HYPHY20G_GFP_UNUSED = 0
                                HYPHY20G_GFP_T_AA = 1
                                HYPHY20G_GFP_T_FA = 2
                                HYPHY20G_GFP_F_PKT = 3
                                HYPHY20G_GFP_F_PHASE = 4
                                HYPHY20G_GFP_OTHER = 5
        */
        case 0:
            sysHyPhy20gMemFree(pDummyCalMem);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 1: /* HYPHY20G_GFP_T_AA */
            genMode = 1; /* HYPHY20G_GFP_T_AA = 1 */
            break;
        case 2: /* HYPHY20G_GFP_T_FA Multiple Channels */
            genMode = 2; /* HYPHY20G_GFP_T_FA = 2 */
            break;
        case 3: /* HYPHY20G_GFP_F_PKT */
            genMode = 3; /* HYPHY20G_GFP_F_PKT = 3 */
            break;
        case 4: /* HYPHY20G_GFP_F_PHASE */
            genMode = 4; /* HYPHY20G_GFP_F_PHASE = 4 */
            break;
        case 5: /* HDLC (single channel non-full bandwidth) */
            genMode = 5; /* HYPHY20G_GFP_OTHER = 5 */
            break;
        case 6: /* GFP-F (single channel full rate) */
            genMode = 3; /* HYPHY20G_GFP_F_PKT = 3 */
            break;
        case 7: /* HDLC (single channel full rate) */
            genMode = 5; /* HYPHY20G_GFP_OTHER = 5 */
            break;
        case 8: /* GFP-T (FA) (single channel) */
            genMode = 2; /* HYPHY20G_GFP_T_FA = 2 */
            break;
        default:
            sysHyPhy20gMemFree(pDummyCalMem);
            return HYPHY20G_ERR_INVALID_ARG;
    }
    result = gfpHyPhy20gGfpGenerateRateAwareSchdData( fileHndl,
                                                      slice, cpbId, 1,
                                                      grpId, genMode,
                                                      numSb, cpbPktLen,
                                                      gfpEncapLen, compress,
                                                      &numRows, pDummyCalMem,
                                                      &pOptimal);
    
    if (result){
        /* Remove grpId from the channel context */
        if (pRdChnlCtxt != NULL) {
            pRdChnlCtxt->gfpGrpId = HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER;
        }
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    while (lastRow > firstRow) {
        pCurrGrpId = (sHYPHY20G_GFP_SCBS_ROW_CTXT *)&(pGfpCtxt->pGfpRowCtxt[lastRow]);
        currGrpId = pCurrGrpId->grpId;

        pCurrGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)
                            (pSchd->pScbsGrpCtxt + currGrpId);
        currGrpStart = pCurrGrpCtxt->startAddr;

        result = gfpHyPhy20gGfpScbsGrpShift(fileHndl, pGfpCtxt,
                                            currGrpId, rowsToShift, 0);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
        lastRow = currGrpStart - 1;
    }

    /* Update group pointers for new group size */
    grpToModifyEnd = grpToModifyEnd + rowsToShift;

    result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, grpId,
                                           grpToModifyStart, grpToModifyEnd);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    /* update GFP Row Context data for the group being modified */
    for (i = grpToModifyStart; i<=grpToModifyEnd; i++) {
        pGfpRowCtxt = (sHYPHY20G_GFP_SCBS_ROW_CTXT *)&(pGfpCtxt->pGfpRowCtxt[i]);
        pGfpRowCtxt->grpId = grpId;
    }


    /* Update Calendar Data */
    result = utilHyPhy20gScbsUpdateCalRows(fileHndl, pSchd, numRows, 0,
                                           grpToModifyStart, pDummyCalMem);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }


    /* TxDP Config   */
    /* Update PROC_CHAN_ID with a valid channel from the group */
    grpStart = pGrpCtxt->startAddr;
    if (grpStart != HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW) {
        result = utilHyPhy20gScbsReadSchdCal(fileHndl, pSchd, grpStart, 0, &firstCh);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }

        regAddr = HYPHY20G_GFP_FRM_TXDP_REG_PROC_CHNL_ID_OFFSET(grpId/4);
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gRead(fileHndl, regAddr, &procChanId);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }

        chIdOffset = 8*(grpId%4);
        chIdMask = 0x3F << chIdOffset;
        result = hyPhy20gVarFieldWrite(&procChanId, chIdMask, firstCh);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }

        result = sysHyPhy20gWrite(fileHndl, regAddr, procChanId);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
    }

    /* TXDP64 - Make sure that GRP_RST is '1' */
    regAddr = HYPHY20G_GFP_FRM_TXDP_REG_GRP_RST_OFFSET(grpId/32);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    /* TxGFPT Config   */
    /* Set GRP_RST to '0' */
    regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_GRP_RST_GROUPS_31_TO_0;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    if (grpId > 31)
        regAddr = regAddr + 4;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    /* Set CHAN_EN to '1' */
    regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_CHNL_EN_CHANNELS_31_TO_0;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    if (cpbId > 31)
        regAddr = regAddr + 4;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 1);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    if (disableSnglChnl) {
          regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_EN_GROUPS_31_TO_0 + 0x4*(grpId/32);
          regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
          result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
          if (result){
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }
    }

    sysHyPhy20gMemFree(pDummyCalMem);

    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRateAwareTxChAdd */

/*******************************************************************************
**
**  hyPhy20gGfpRateAwareTxChDelete
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the calendar-based scheduler (SCBS) within the
**                  TxGFPT block to remove a particular GFP/HDLC
**                  CPBID channel from a particular outgoing VCAT group.
**
**                  For Channel Multiplexing of CBR clients, the SCBS will
**                  attempt to be populated to ensure the calculated frame
**                  rate for each CBR client will be met.
**
**                  VCAT bandwidth may decrease for an operational group if
**                  bandwidth is deprovisioned. For CBR Channel Multiplexing,
**                  it may be possible that the generated SCBS is suboptimal
**                  and will not guarantee the frame rate requirements of
**                  the application.
**
**                  The quality of the scheduler will be returned with the
**                  *pOptimal argument.  Suboptimal SCBS population may result
**                  in CPB FIFO overflows in turn corrupting one or more CBR
**                  streams associated with the channel multiplexed VCAT group.
**
**                  This function is used mutually exclusively with
**                  hyPhy20gGfpTxChDelete.
**
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - CPBID of the channel being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              grpId       - VCAT group ID to assign this channel to
**                          - valid values are 0x0 - 0x3F (0-63)
**             *pOptimal    - Pointer to storage for the optimized state of
**                            SCBS contents:
**                            0 - Optimal.  SCBS meets frame rate request
**                                requirements of channels.
**                            Other - Sub-Optimal.  SCBS does not guarantee
**                                frame rate request requirements of channels.
**
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_INVALID_GFP_CHNL_MODES
**                  HYPHY20G_ERR_ZERO_DIVIDE
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_EVALUATING_SCHD
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpRateAwareTxChDelete(struct file *fileHndl, UINT4 slice,
                                    UINT4 cpbId, UINT4 grpId, UINT4 *pOptimal)
{
    INT4 result;
    UINT4 regAddr = 0;
    UINT4 calSize = 0;
    UINT4 size = 0;
    UINT4 grpToModifyStart = 0;
    UINT4 grpToModifyEnd = 0;
    UINT4 grpToModifyNewEnd = 0;
    UINT4 grpToModifyRows = 0;
    UINT4 rowsToRemove = 0;
    UINT4 firstRow = 0;
    UINT4 lastRow = 0;
    UINT4 firstUnused = 0;
    UINT4 unusedRows = 0;
    UINT4 numRows = 0;
    UINT4 currGrpId = 0;
    UINT4 currGrpStart = 0;
    UINT4 currGrpEnd = 0;
    UINT4 cpbChOffset = 0;
    UINT4 grpStart = 0;
    UINT4 firstCh = 0;
    UINT4 chIdOffset = 0;
    UINT4 chIdMask = 0;
    UINT4 procChanId = 0;
    UINT4 addition = 1;

    UINT4 grpEmpty = 0;
    UINT4 i = 0;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_GFP_SCBS_ROW_CTXT *pCurrGrpId;
    sHYPHY20G_SCHD_GRP_CTXT *pCurrGrpCtxt;
    sHYPHY20G_SCHD_GRP_CTXT *pGrpCtxt;
    sHYPHY20G_RD_CHNL_CTXT *pRdChnlCtxt;
    sHYPHY20G_GFP_SCBS_ROW_CTXT *pGfpRowCtxt;
    UINT4 *pDummyCalMem;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(grpId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

   /* Create a dummy calendar page for later & Initialize to all 0xFF */
    calSize = HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW * HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW;
    size = calSize * sizeof(UINT4);
    pDummyCalMem = (UINT4 *)sysHyPhy20gMemAlloc(size);
    if(pDummyCalMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    for (i = 0; i<calSize; i++) {
        *(pDummyCalMem + i) = HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER;
    }

    /* Grab pointer to GFP Slice context*/
    if (slice==0) {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpACtxt);
    } else {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpBCtxt);
    }

    /* Confirm GFP Slice Context is valid */
    if(pGfpCtxt == NULL){
        sysHyPhy20gMemFree(pDummyCalMem);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Grab the scheduler data for the GFP SCBS being configured */
    pSchd = &(pGfpCtxt->schd);

    /* Grab the group data for the GFP SCBS being configured */
    pGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)(pSchd->pScbsGrpCtxt + grpId);

    /* For the group being modified, find the current number of
       rows assigned to the group */
    grpToModifyStart = pGrpCtxt->startAddr;
    grpToModifyEnd = pGrpCtxt->endAddr;
    grpToModifyRows = pGrpCtxt->grpNumRows + 1;

    /* Calculate number of additional rows that the group will need*/
    if (grpToModifyStart == 0x7F) {
        /* Group is not currently in use - this is an error condition */
        sysHyPhy20gMemFree(pDummyCalMem);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    } else {
        if (grpToModifyRows == 1) {
            /* Group contains 1 channel.  Group will be deleted */
            rowsToRemove = 1;
            grpEmpty = 1;
        } else {
            if (grpToModifyRows == 4) {
                /* Group contains 2 channels. Remove 3 rows for the
                   deleted channel */
                rowsToRemove = 3;
            } else {
                if (grpToModifyRows == 3) {
                    /* Group contains 2 channels that were added when the SCBS was full.
                    Remove 2 rows for the deleted channel */
                    rowsToRemove = 3;
                } else {
                    if (grpToModifyRows%2) {
                        /* Group contains multiple channels that were added when the SCBS
                        was full. Remove 1 row for the deleted channel */
                        rowsToRemove = 1;
                    } else {
                        /* Group contains multiple channels. Remove 2 rows for the
                        deleted channel */
                        rowsToRemove = 2;
                    }
                }
            }
        }
    }

    /* Remove GRPID from channel-based device context */
    cpbChOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET
                + slice*HYPHY20G_CPB_GFPA_WR_SCHD_MAX_CHNL + cpbId;

    pRdChnlCtxt = (sHYPHY20G_RD_CHNL_CTXT *)
                                        (pDevCtxt->pCpbCtxt->pRdChnl + cpbChOffset);

    /* Check to make sure channel currently is part of grpId */
    if (pRdChnlCtxt->gfpGrpId != grpId) {
        sysHyPhy20gMemFree(pDummyCalMem);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Remove grpId from the channel context */
    pRdChnlCtxt->gfpGrpId = HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER;

    /* Get scheduler data for this group with the channel removed */
    result = gfpHyPhy20gGfpGenerateRateAwareSchdData(fileHndl, slice, cpbId, 0,
                                                     grpId, 0, 0, 0, 0, 0,
                                                     &numRows, pDummyCalMem,
                                                     pOptimal);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    /* Update Calendar Data */
    result = utilHyPhy20gScbsUpdateCalRows(fileHndl, pSchd, grpToModifyRows, 0,
                                                grpToModifyStart, pDummyCalMem);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

   /* Update group pointers for new group size */
    if (grpEmpty) {
        if (grpToModifyEnd == 0) {
            grpToModifyNewEnd = 0;
            addition = 0;
        } else {
            grpToModifyNewEnd = grpToModifyEnd-1;
        }
        result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, grpId,
           HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW, HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
    } else {
        grpToModifyNewEnd = grpToModifyEnd - rowsToRemove;
        result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, grpId,
                                            grpToModifyStart, grpToModifyNewEnd);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
    }

    /* update GFP Row Context data for the group being modified */
    for (i = grpToModifyNewEnd+addition; i<=grpToModifyEnd; i++) {
        pGfpRowCtxt = (sHYPHY20G_GFP_SCBS_ROW_CTXT *)
                                                &(pGfpCtxt->pGfpRowCtxt[i]);
        pGfpRowCtxt->grpId = 0xFF;
    }

    /* Shift up all groups below the group being modified to condense SCBS
        calendar */
    result = gfpHyPhy20gGfpScbsFreeSpace(fileHndl, pGfpCtxt,
                                                    &unusedRows, &firstUnused);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }


    firstRow = grpToModifyEnd + 1;
    if (firstUnused == 0) {
        lastRow = firstUnused;
    } else {
        lastRow = firstUnused - 1;
    }
    while (lastRow >= firstRow) {
        pCurrGrpId = (sHYPHY20G_GFP_SCBS_ROW_CTXT *)
                                        &(pGfpCtxt->pGfpRowCtxt[firstRow]);
        currGrpId = pCurrGrpId->grpId;
        pCurrGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)
                            (pSchd->pScbsGrpCtxt + currGrpId);
        currGrpStart = pCurrGrpCtxt->startAddr;
        currGrpEnd = pCurrGrpCtxt->endAddr;

        result = gfpHyPhy20gGfpScbsGrpShift(fileHndl, pGfpCtxt,
                                                        currGrpId, rowsToRemove, 1);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
        firstRow = currGrpEnd + 1;
    }


    /* Disable channel and possibly group in TXDP64 / TXGFPT */
    /* TXDP64 - set GRP_RST to '0' if the Group is empty*/
    if (grpEmpty) {
        regAddr = HYPHY20G_GFP_FRM_TXDP_REG_GRP_RST_OFFSET(grpId/32);
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
    }
    /* Update PROC_CHAN_ID with a valid channel from the group */
    grpStart = pGrpCtxt->startAddr;
    if (grpStart != HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW) {
        result = utilHyPhy20gScbsReadSchdCal(fileHndl, pSchd, grpStart, 0, &firstCh);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }

        regAddr = HYPHY20G_GFP_FRM_TXDP_REG_PROC_CHNL_ID_OFFSET(grpId/4);
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gRead(fileHndl, regAddr, &procChanId);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }

        chIdOffset = 8*(grpId%4);
        chIdMask = 0x3F << chIdOffset;
        result = hyPhy20gVarFieldWrite(&procChanId, chIdMask, firstCh);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }

        result = sysHyPhy20gWrite(fileHndl, regAddr, procChanId);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
    }



    /* TXGFPT - set GRP_RST to '1' if the group is empty */
    if (grpEmpty) {
        regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_GRP_RST_GROUPS_31_TO_0;
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        if (grpId > 31)
            regAddr = regAddr + 4;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
    }

    /* TXGFPT - set CHAN_EN to '0' */
    regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_CHNL_EN_CHANNELS_31_TO_0;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    if (cpbId > 31)
        regAddr = regAddr + 4;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 0);
    if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }

    if (grpEmpty) {
        regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_EN_GROUPS_31_TO_0 + 0x4*(grpId/32);
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
        
        /* Clear GRP_TO_CHAN_LUT_n */
        regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_TO_LUT_GROUPS_3_0 + 0x4*(grpId/4);
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, (0x1F << ((grpId % 4)*8)), 0);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
    }

    sysHyPhy20gMemFree(pDummyCalMem);
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpRateAwareTxChDelete */

/*******************************************************************************
**
**  hyPhy20gGfpRxChAdd
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the Rx data path for channel add
**
**                  This procedure also enables GFP de-multiplexing when
**                  multiple GFP channels with unique CIDs are mapped to the
**                  same GID.
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - CPBID of the channel being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              cid         - CID value to use for channel (only applies for
**                            gfp based traffic).  Valid values are 0x0 - 0xFF
**              grpId       - VCAT group ID to assign this channel to
**                          - valid values are 0x0 - 0x3F (0-63)
**              chSrc       - 0 - SONET
**                          - 1 - OTN
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpRxChAdd(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                                UINT4 cid, UINT4 grpId, UINT4 chSrc)
{

    INT4 result;
    UINT4 mask = 0;
    UINT4 offset;
    UINT4 vcid_mask = 0;
    UINT4 cid_mask = 0;
    UINT4 regAddr = 0;
    UINT4 reg0;
    UINT4 reg1;
    UINT4 vcid_cid = 0;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(cid > 0xFF)||(grpId > 0x3F)||(chSrc > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }


    /* Enable channel in RXDP64 / RXGFPT */
    /* RXDP64 - Set VCID_CID field */
    cid_mask = (0xFF<<0);
    vcid_mask = (0x3F<<8);
    result = hyPhy20gVarFieldWrite(&vcid_cid, vcid_mask, grpId);
    if (result) return result;
    result = hyPhy20gVarFieldWrite(&vcid_cid, cid_mask, cid);
    if (result) return result;

    mask = HYPHY20G_GFP_FRM_RXDP_BIT_CPBID_LKUP_TBL_VCID_CID_VAL_MSK;
    offset = HYPHY20G_GFP_FRM_RXDP_BIT_CPBID_LKUP_TBL_VCID_CID_VAL_OFF;
    result = hyPhy20gVarFieldWrite(&vcid_cid, mask, 0);
    if (result) return result;

    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CPBID_LKUP_TBL_OFFSET(cpbId);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, vcid_cid);
    if (result) return result;

    /* RXDP64 - Set VAL field to 1*/
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;

    /* RXDP64 - set CHAN_EN to '1' for this group*/
    result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x0, grpId,
                                                    &reg0, &reg1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&reg0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_GRP_EN_MSK,
        1);
    if (result) return result;

    result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x0, grpId,
                                                    reg0, reg1);
    if (result) return result;

    /* RXGFPT - set CHAN_EN to '1' */
    regAddr = HYPHY20G_GFP_TRA_RXGFPT_REG_CHNL_EN_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 1);
    if (result) return result;


    switch (chSrc) {
        case 0:
            /* Enable group in RVCP192 */
            /* Set GRP_INHIBIT to '0'  */
            regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_GRP_INH_OFFSET(grpId/32);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
            if (result) return result;
            break;
        case 1:
            /* Enable group in RVCPO */
            /* Set GRP_ID_N_INHIBIT to '0'  */
            regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_GRP_ID_INH_0 + 4*(grpId/32);

#ifdef HWACCESSMODE_USB
/* emulation */
            regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
#else
/* EVBD */
            regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
#endif
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
            if (result) return result;
            break;
    }
    return HYPHY20G_SUCCESS;

}  /* hyPhy20gGfpRxChAdd */


/*******************************************************************************
**
**  hyPhy20gGfpTxChDelete
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the calendar-based scheduler (SCBS) within the
**                  TxGFPT block to remove a particular GFP/HDLC
**                  CPBID channel from a particular outgoing VCAT group.
**
**                  This function is used mutually exclusively with
**                  hyPhy20gGfpRateAwareTxChDelete.
**
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - CPBID of the channel being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              grpId       - VCAT group ID to assign this channel to
**                          - valid values are 0x0 - 0x3F (0-63)
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_EVALUATING_SCHD
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpTxChDelete(struct file *fileHndl, UINT4 slice,
                                 UINT4 cpbId, UINT4 grpId)
{
    INT4 result;
    UINT4 regAddr = 0;
    UINT4 calSize = 0;
    UINT4 size = 0;
    UINT4 grpToModifyStart = 0;
    UINT4 grpToModifyEnd = 0;
    UINT4 grpToModifyNewEnd = 0;
    UINT4 grpToModifyRows = 0;
    UINT4 rowsToRemove = 0;
    UINT4 firstRow = 0;
    UINT4 lastRow = 0;
    UINT4 firstUnused = 0;
    UINT4 unusedRows = 0;
    UINT4 numRows = 0;
    UINT4 currGrpId = 0;
    UINT4 currGrpStart = 0;
    UINT4 currGrpEnd = 0;
    UINT4 cpbChOffset = 0;
    UINT4 grpStart = 0;
    UINT4 firstCh = 0;
    UINT4 chIdOffset = 0;
    UINT4 chIdMask = 0;
    UINT4 procChanId = 0;
    UINT4 addition = 1;

    UINT4 grpEmpty = 0;
    UINT4 i = 0;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_GFP_SCBS_ROW_CTXT *pCurrGrpId;
    sHYPHY20G_SCHD_GRP_CTXT *pCurrGrpCtxt;
    sHYPHY20G_SCHD_GRP_CTXT *pGrpCtxt;
    sHYPHY20G_RD_CHNL_CTXT *pRdChnlCtxt;
    sHYPHY20G_GFP_SCBS_ROW_CTXT *pGfpRowCtxt;
    UINT4 *pDummyCalMem;

    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(grpId > 0x3F)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

   /* Create a dummy calendar page for later & Initialize to all 0xFF */
    calSize = HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW * HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW;
    size = calSize * sizeof(UINT4);
    pDummyCalMem = (UINT4 *)sysHyPhy20gMemAlloc(size);
    if(pDummyCalMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    for (i = 0; i<calSize; i++) {
        *(pDummyCalMem + i) = HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER;
    }

    /* Grab pointer to GFP Slice context*/
    if (slice==0) {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpACtxt);
    } else {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpBCtxt);
    }

    /* Confirm GFP Slice Context is valid */
    if(pGfpCtxt == NULL){
        sysHyPhy20gMemFree(pDummyCalMem);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Grab the scheduler data for the GFP SCBS being configured */
    pSchd = &(pGfpCtxt->schd);

    /* Grab the group data for the GFP SCBS being configured */
    pGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)(pSchd->pScbsGrpCtxt + grpId);

    /* For the group being modified, find the current number of
       rows assigned to the group */
    grpToModifyStart = pGrpCtxt->startAddr;
    grpToModifyEnd = pGrpCtxt->endAddr;
    grpToModifyRows = pGrpCtxt->grpNumRows + 1;

    /* Calculate number of additional rows that the group will need*/
    if (grpToModifyStart == 0x7F) {
        /* Group is not currently in use - this is an error condition */
        sysHyPhy20gMemFree(pDummyCalMem);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    } else {
        if (grpToModifyRows == 1) {
            /* Group contains 1 channel.  Group will be deleted */
            rowsToRemove = 1;
            grpEmpty = 1;
        } else {
            if (grpToModifyRows == 4) {
                /* Group contains 2 channels. Remove 3 rows for the
                   deleted channel */
                rowsToRemove = 3;
            } else {
                if (grpToModifyRows == 3) {
                    /* Group contains 2 channels that were added when the SCBS was full.
                    Remove 2 rows for the deleted channel */
                    rowsToRemove = 3;
                } else {
                    if (grpToModifyRows%2) {
                        /* Group contains multiple channels that were added when the SCBS
                        was full. Remove 1 row for the deleted channel */
                        rowsToRemove = 1;
                    } else {
                        /* Group contains multiple channels. Remove 2 rows for the
                        deleted channel */
                        rowsToRemove = 2;
                    }
                }
            }
        }
    }

    /* Remove GRPID from channel-based device context */
    cpbChOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET
                + slice*HYPHY20G_CPB_GFPA_WR_SCHD_MAX_CHNL + cpbId;

    pRdChnlCtxt = (sHYPHY20G_RD_CHNL_CTXT *)
                                        (pDevCtxt->pCpbCtxt->pRdChnl + cpbChOffset);

    /* Check to make sure channel currently is part of grpId */
    if (pRdChnlCtxt->gfpGrpId != grpId) {
        sysHyPhy20gMemFree(pDummyCalMem);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Remove grpId from the channel context */
    pRdChnlCtxt->gfpGrpId = HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER;

    /* Get scheduler data for this group with the channel removed */
    result = gfpHyPhy20gGfpGenerateSchdData(fileHndl, slice, grpId, 0,
                                                    &numRows, pDummyCalMem);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    /* Update Calendar Data */
    result = utilHyPhy20gScbsUpdateCalRows(fileHndl, pSchd, grpToModifyRows, 0,
                                                grpToModifyStart, pDummyCalMem);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

   /* Update group pointers for new group size */
    if (grpEmpty) {
        if (grpToModifyEnd == 0) {
            grpToModifyNewEnd = 0;
            addition = 0;
        } else {
            grpToModifyNewEnd = grpToModifyEnd-1;
        }
        result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, grpId,
           HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW, HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
    } else {
        grpToModifyNewEnd = grpToModifyEnd - rowsToRemove;
        result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, grpId,
                                            grpToModifyStart, grpToModifyNewEnd);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
    }

    /* update GFP Row Context data for the group being modified */
    for (i = grpToModifyNewEnd+addition; i<=grpToModifyEnd; i++) {
        pGfpRowCtxt = (sHYPHY20G_GFP_SCBS_ROW_CTXT *)
                                                &(pGfpCtxt->pGfpRowCtxt[i]);
        pGfpRowCtxt->grpId = 0xFF;
    }

    /* Shift up all groups below the group being modified to condense SCBS
        calendar */
    result = gfpHyPhy20gGfpScbsFreeSpace(fileHndl, pGfpCtxt,
                                                    &unusedRows, &firstUnused);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }


    firstRow = grpToModifyEnd + 1;
    if (firstUnused == 0) {
        lastRow = firstUnused;
    } else {
        lastRow = firstUnused - 1;
    }
    while (lastRow >= firstRow) {
        pCurrGrpId = (sHYPHY20G_GFP_SCBS_ROW_CTXT *)
                                        &(pGfpCtxt->pGfpRowCtxt[firstRow]);
        currGrpId = pCurrGrpId->grpId;
        pCurrGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)
                            (pSchd->pScbsGrpCtxt + currGrpId);
        currGrpStart = pCurrGrpCtxt->startAddr;
        currGrpEnd = pCurrGrpCtxt->endAddr;

        result = gfpHyPhy20gGfpScbsGrpShift(fileHndl, pGfpCtxt,
                                                        currGrpId, rowsToRemove, 1);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
        firstRow = currGrpEnd + 1;
    }

    /* TXGFPT - set CHAN_EN to '0' */
    regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_CHNL_EN_CHANNELS_31_TO_0;
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    if (cpbId > 31)
        regAddr = regAddr + 4;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 0);
    if (result){
        sysHyPhy20gMemFree(pDummyCalMem);
        return result;
    }

    if (grpEmpty) {
          regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_EN_GROUPS_31_TO_0 + 0x4*(grpId/32);
          regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
          result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
          if (result){
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }

          /* Clear GRP_TO_CHAN_LUT_n */
          regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_SNGL_CHNL_GRP_TO_LUT_GROUPS_3_0 + 0x4*(grpId/4);
          regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
          result = hyPhy20gRegFieldWrite(fileHndl, regAddr, (0x1F << ((grpId % 4)*8)), 0);
          if (result){
              sysHyPhy20gMemFree(pDummyCalMem);
              return result;
          }
    }

    /* TXGFPT - set GRP_RST to '1' if the group is empty */
    if (grpEmpty) {
        regAddr = HYPHY20G_GFP_TRA_TXGFPT_TOP_REG_GRP_RST_GROUPS_31_TO_0;
        regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        if (grpId > 31)
            regAddr = regAddr + 4;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
    }

    /* Disable channel and possibly group in TXDP64 / TXGFPT */
    /* TXDP64 - set GRP_RST to '0' if the Group is empty*/
    if (grpEmpty) {
        regAddr = HYPHY20G_GFP_FRM_TXDP_REG_GRP_RST_OFFSET(grpId/32);
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 0);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
    }
    /* Update PROC_CHAN_ID with a valid channel from the group */
    grpStart = pGrpCtxt->startAddr;
    if (grpStart != HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW) {
        result = utilHyPhy20gScbsReadSchdCal(fileHndl, pSchd, grpStart, 0, &firstCh);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }

        regAddr = HYPHY20G_GFP_FRM_TXDP_REG_PROC_CHNL_ID_OFFSET(grpId/4);
        regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gRead(fileHndl, regAddr, &procChanId);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }

        chIdOffset = 8*(grpId%4);
        chIdMask = 0x3F << chIdOffset;
        result = hyPhy20gVarFieldWrite(&procChanId, chIdMask, firstCh);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }

        result = sysHyPhy20gWrite(fileHndl, regAddr, procChanId);
        if (result){
            sysHyPhy20gMemFree(pDummyCalMem);
            return result;
        }
    }

    sysHyPhy20gMemFree(pDummyCalMem);
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpTxChDelete */


/*******************************************************************************
**
**  hyPhy20gGfpRxChDelete
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    Configures the Rx data path for channel removal
**
**
**  INPUTS:     *fileHndl   - base address
**              slice       - 0 - Slice 'A'
**                          - 1 - Slice 'B'
**              cpbId       - CPBID of the channel being configured
**                          - valid values are 0x0 - 0x3F (0-63)
**              grpId       - VCAT group ID to assign this channel to
**                          - valid values are 0x0 - 0x3F (0-63)
**              chSrc       - 0 - SONET
**                          - 1 - OTN
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_MUTEX_AQUIRE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpRxChDelete(struct file *fileHndl, UINT4 slice, UINT4 cpbId,
                                UINT4 grpId, UINT4 chSrc)
{

    INT4 result;
    UINT4 regAddr;
    UINT4 grpEmpty = 0;
    UINT4 reg0;
    UINT4 reg1;
    UINT4 offset;
    UINT4  cpbChOffset;


    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_SCHD_GRP_CTXT *pGrpCtxt;
    sHYPHY20G_RD_CHNL_CTXT *pRdChnlCtxt;


    /* argument checking */
    if ((slice > 1)||(cpbId > 0x3F)||(grpId > 0x3F)||(chSrc > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }


    /* Determine if Group is now empty */

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
            "%s", "pDevCtxt is NULL.");

    /* Grab pointer to GFP Slice context*/
    if (slice==0) {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpACtxt);
    } else {
        pGfpCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)(pDevCtxt->pGfpBCtxt);
    }

    /* Confirm GFP Slice Context is valid */
    if(pGfpCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Grab the scheduler data for the GFP SCBS being configured */
    pSchd = &(pGfpCtxt->schd);

    /* Grab the group data for the GFP SCBS being configured */
    pGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)(pSchd->pScbsGrpCtxt + grpId);

    if (pGrpCtxt->startAddr == pGrpCtxt->endAddr) {
        /* Group has either no channels or 1 channel currently assigned */
        if (pGrpCtxt->startAddr == HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW) {
            /* Group has no channels assigned */
            grpEmpty = 1;
        } else {
            /* group has 1 channel assigned.  If this is the same channel
               being removed, then the group will be empty */
            cpbChOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET
                        + slice*HYPHY20G_CPB_GFPA_WR_SCHD_MAX_CHNL + cpbId;
            pRdChnlCtxt = (sHYPHY20G_RD_CHNL_CTXT *)
                                   (pDevCtxt->pCpbCtxt->pRdChnl + cpbChOffset);
            if (pRdChnlCtxt->gfpGrpId == grpId) {
                grpEmpty = 1;
            }
        }
    }

    /* Disable channel in RXDP64 / RXGFPT */
    /* RXGFPT - set CHAN_EN to '0' */
    regAddr = HYPHY20G_GFP_TRA_RXGFPT_REG_CHNL_EN_OFFSET(cpbId/32);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, cpbId%32, 0);
    if (result) return result;

    /* RXDP64 - Set VAL field to 0x0 */
    regAddr = HYPHY20G_GFP_FRM_RXDP_REG_CPBID_LKUP_TBL_OFFSET(cpbId);
    regAddr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_GFP_FRM_RXDP_BIT_CPBID_LKUP_TBL_VCID_CID_VAL_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* RXDP64 - Set VCID_CID field to 0x0 */
    result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
    if (result) return result;

    /* RXDP64 - set GRP_EN to '0' if the group is empty */
    if (grpEmpty) {
        result = gfpfHyPhy20gRXDPChanIndRead(fileHndl, slice, 0x0, grpId,
                                                        &reg0, &reg1);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&reg0,
        HYPHY20G_GFP_FRM_RXDP_PG_ADR_00000_IBIT_GFP_HDLC_CHNL_CFG_GRP_EN_MSK,
        0);
        if (result) return result;
        result = gfpfHyPhy20gRXDPChanIndWrite(fileHndl, slice, 0x0, grpId,
                                                        reg0, reg1);
        if (result) return result;
    }

    switch (chSrc) {
        case 0:
            if (grpEmpty) {
                /* disable group in RVCP192 */
                /* Set GRP_INHIBIT to '1'  */
                regAddr = HYPHY20G_VCAT_LCAS_RVCP_REG_GRP_INH_OFFSET(grpId/32);
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
                if (result) return result;
            }
            break;
        case 1:
            if (grpEmpty) {
                /* disable group in RVCPO */
                /* Set GRP_ID_N_INHIBIT to '1'  */
                regAddr = HYPHY20G_VCAT_LCAS_RVCPO_REG_GRP_ID_INH_0 + 4*(grpId/32);
 #ifdef HWACCESSMODE_USB
/* emulation */
                regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, 1);
#else
/* EVBD */
                regAddr = HYPHY20G_VCAT_LCAS_RVCPO_TSB_SLICE_REG_OFFSET(regAddr, slice);
#endif
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, grpId%32, 1);
                if (result) return result;
            }
            break;
    }
    return HYPHY20G_SUCCESS;

}  /* hyPhy20gGfpRxChDelete */

/*******************************************************************************
**
**  hyPhy20gGfpSonetSysClkCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function configures the system reference clock for
**                  GFP slices operating in SONET mode.
**
**                  When the data path is in POS internal loop timing mode,
**                  system reference clock is derived from OC192 clock.
**                  The default reference clock in this API is REFCLK311.
**
**                  Prior to calling this API, hyPhy20gGfpHdlcGrpChInitTx must
**                  be called to set up HDLC link traffic source within the GFP 
**                  subsystem.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - 0 - Slice 'A'
**                              - 1 - Slice 'B'
**                  timingMode  - 0 - Internal 311MHZ reference clock
**                              - 1 - OC192[0] internally recovered XFI clock
**                              - 2 - OC192[1] internally recovered XFI clock      
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpSonetSysClkCfg(struct file *fileHndl, UINT4 slice, 
                               UINT4 timingMode)
{
    UINT4 addr;
    UINT4 off;
    UINT4 mask;
    UINT4 posVal;
    UINT4 sysClkVal;
    UINT4 sonetEnVal;
    INT4 result;
    
    /* argument checking */
    if (slice > 1 || timingMode > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Retreive TX_POS_VCATB bit to determine if data is in SONET mode */
    addr = HYPHY20G_GFP_FRM_TOP_REG_MSTR_CTL;
    addr = HYPHY20G_GFP_FRM_MTSB_SLICE_REG_OFFSET(addr, slice);

    off = HYPHY20G_GFP_FRM_TOP_BIT_MSTR_CTL_TX_POS_VCATB_OFF;
    
    result = sysHyPhy20gBitRead(fileHndl, addr, off, &posVal);
    if (result) return result; 

    if (posVal == 0) { 
        /* if not from SONET_LN then retreive SONET_EN to determine if data is 
           in SONET mode */
        addr = HYPHY20G_VCAT_LCAS_TOP_REG_CFG;
        result = sysHyPhy20gBitRead(fileHndl, addr,
                                    HYPHY20G_VCAT_LCAS_TOP_BIT_CFG_SONET_EN_OFF,
                                    &sonetEnVal);
        if (result) return result; 
            
        if (sonetEnVal != 1) {
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }    
    }
           
    /* Configure GFP_0_SYSCLK_SEL or GFP_1_SYSCLK_SEL */
    addr = HYPHY20G_PGMRCLK_REG_GFP_SYSCLK_SEL;
    mask = (slice == 0)? HYPHY20G_PGMRCLK_BIT_GFP_SYSCLK_SEL_GFP_0_SYSCLK_SEL_MSK:
    HYPHY20G_PGMRCLK_BIT_GFP_SYSCLK_SEL_GFP_1_SYSCLK_SEL_MSK;    
    sysClkVal = (timingMode == 1)? 2:3;    
    
    if (timingMode == 0) {
        result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, 1);
        if (result) return result;
    } else {
        result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, sysClkVal);
        if (result) return result;
    }  
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpSonetSysClkCfg */

/*******************************************************************************
**
**  hyPhy20gGfpTxPayloadSrcCfg
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function allows for the forceful insertion of 
**                  non-inverted PN-11 data into the OPU payload region. When
**                  enabled data is still read from the read FIFO at the normal 
**                  rate but that data is discarded and the PN-11 sequence is 
**                  used instead.
**
**                  Note that calling this function after enabling VCAT 
**                  groups / slices can result in some transient undefined   
**                  behavior.
**
**  INPUTS:         *fileHndl   - Base address
**                  slice       - 0 - Slice 'A'
**                              - 1 - Slice 'B'
**                  grpId       - VCAT group ID to set the payload source for. 
**                                Valid range is 0-63
**                  mode        - 0 - Disable PN-11 generation for grpId
**                              - 1 - Enable PN-11 generation for grpId
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gGfpTxPayloadSrcCfg (struct file *fileHndl, UINT4 slice, 
                                UINT4 grpId, UINT4 mode)
{
    UINT4 regAddr;
    UINT4 forcePn11Msk;
    INT4 result;
    
    if (slice > 1 || grpId > 63 || mode > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
        
    regAddr = HYPHY20G_GFP_TRA_OTNMA_REG_VCAT_LCAS_GRP_CFG_OFFSET(grpId);
    regAddr = HYPHY20G_GFP_TRA_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    forcePn11Msk = HYPHY20G_GFP_TRA_OTNMA_BIT_VCAT_LCAS_GRP_CFG_FORCE_PN11_DATA_MSK;
    
    /* Configure FORCE_PN11_DATA */
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, forcePn11Msk, mode);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;
} /* hyPhy20gGfpTxPayloadSrcCfg */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/*******************************************************************************
**
**  gfpHyPhy20gGfpScbsFreeSpace
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function determines whether the number of unused rows
**                  in the SCBS and the location of the first unused row.
**
**  INPUTS:     *fileHndl       - base address
**              *pGfpSliceCtxt   - pointer to pDevCtxt->pGfpACtxt or
**                                           pDevCtxt->pGfpBCtxt
**              *unusedRows     - Number of unused rows
**              *firstUnused    - Address of first unused row
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpHyPhy20gGfpScbsFreeSpace(struct file *fileHndl,
                                    sHYPHY20G_GFP_SLICE_CTXT *pGfpSliceCtxt,
                                    UINT4 *unusedRows, UINT4 *firstUnused)
{
    UINT4 bottomRow = 0;
    UINT4 topRow = 0;
    UINT4 i = 0;

    sHYPHY20G_GFP_SCBS_ROW_CTXT *pCurrGrpId;

    bottomRow = HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW;
    topRow = 0;


    for (i = bottomRow; i>topRow; i--) {
        pCurrGrpId = (sHYPHY20G_GFP_SCBS_ROW_CTXT *)
                                        &(pGfpSliceCtxt->pGfpRowCtxt[i - 1]);
        if (pCurrGrpId->grpId != 0xFF) {
            *unusedRows = HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW + 1 - i;
            *firstUnused = i;
            i = 1;
        } else {
            if (i == 1) {
                *unusedRows = HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW;
                *firstUnused = 0;
            }
        }
    }

    return HYPHY20G_SUCCESS;
} /* gfpHyPhy20gGfpScbsFreeSpace */


/*******************************************************************************
**
**  gfpHyPhy20gGfpScbsGrpShift
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function shifts the location of a group's calendar data
**                  within the TxGFPT SCBS down by a specified number of columns
**
**  INPUTS:     *fileHndl       - base address
**              *pGfpSliceCtxt   - pointer to pDevCtxt->pGfpACtxt or
**                                           pDevCtxt->pGfpBCtxt
**              grpId           - groupID that needs to be shifted
**              shift           - Number of rows to shift the group
**              direction       - Direction to shift a group
**                              - 1 - up (shifted towards start of table)
**                              - 0 - down (shifted towards bottom of table)
**
**  OUTPUTS:    None
**
**  RETURNS:        HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpHyPhy20gGfpScbsGrpShift(struct file *fileHndl,
                                    sHYPHY20G_GFP_SLICE_CTXT *pGfpSliceCtxt,
                                    UINT4 grpId, UINT4 shift, UINT4 direction)
{
    INT4 result;
    UINT4 currStart = 0;
    UINT4 currEnd = 0;
    UINT4 calSize = 0;
    UINT4 size = 0;
    UINT4 newStart = 0;
    UINT4 newEnd = 0;
    UINT4 currGrpSize = 0;
    UINT4 i,j;

    sHYPHY20G_SCHD_GRP_CTXT *pScbsGrpCtxt;
    sHYPHY20G_GFP_SCBS_ROW_CTXT *pGfpRowCtxt;
    sHYPHY20G_SCHD_CTXT *pSchd;
    UINT4 *pDummyCalMem;
    UINT4 *pOnLineCalData;
    UINT4 *pOnlineCalMemCopy;

    /* Create a dummy calendar page with all 0xFFFFFFFF */
    calSize = HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW * HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW;
    size = calSize * sizeof(UINT4);
    pDummyCalMem = (UINT4 *)sysHyPhy20gMemAlloc(size);
    if(pDummyCalMem == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    for (i = 0; i<calSize; i++) {
        *(pDummyCalMem + i) = HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER;
    }


    /* grab scheduler data */
    pSchd = &(pGfpSliceCtxt->schd);
    pOnLineCalData = (UINT4 *) (pSchd->pOnLineCalMem);

    /* Create a copy of online calendar page */
    pOnlineCalMemCopy = (UINT4 *)sysHyPhy20gMemAlloc(size);
    if(pOnlineCalMemCopy == NULL) {
        sysHyPhy20gMemFree(pDummyCalMem);
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    for (i=0; i <HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW; i++) {
        for(j=0; j < HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW; j++) {
            *(pOnlineCalMemCopy + i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j) =
                 *(pOnLineCalData + i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + j);
        }
    }

    /* Grab group context data  */
    pScbsGrpCtxt = (sHYPHY20G_SCHD_GRP_CTXT *)
                                (pSchd->pScbsGrpCtxt + grpId);


    /* Grab current group start/end addresses */
    currStart = pScbsGrpCtxt->startAddr;
    currEnd = pScbsGrpCtxt->endAddr;
    currGrpSize = pScbsGrpCtxt->grpNumRows + 1;

    /* Shift the group contents up/down by the required number of rows */
    if (direction) {
    /* shift up */
        /* Adjust the start address to include the newly deleted rows above the group
           being shifted */
        newStart = currStart - shift;
        newEnd = currEnd - shift;
        result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, grpId,
                                                            newStart, currEnd);
        if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            sysHyPhy20gMemFree(pOnlineCalMemCopy);
            return result;
        }

        /* Write the new group contents in to the scheduler */
        result = utilHyPhy20gScbsUpdateCalRows(fileHndl, pSchd, currGrpSize,
                    currStart, currStart-shift, pOnlineCalMemCopy);
        if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            sysHyPhy20gMemFree(pOnlineCalMemCopy);
            return result;
        }

        /* Adjust the end address to reflect the new values*/
        result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, grpId,
                                                            newStart, newEnd);
        if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            sysHyPhy20gMemFree(pOnlineCalMemCopy);
            return result;
        }

        /* For each row that is no longer being used by the group, set the
           calendar entry to 0xFFFFFFFF */
        result = utilHyPhy20gScbsUpdateCalRows(fileHndl, pSchd, shift,
                    0, currEnd-shift + 1, pDummyCalMem);
        if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            sysHyPhy20gMemFree(pOnlineCalMemCopy);
            return result;
        }

        /* update GFP Row Context data for all the rows that have been
           modified */
        for (i = newStart; i<=newEnd; i++) {
            pGfpRowCtxt = (sHYPHY20G_GFP_SCBS_ROW_CTXT *)
                                                &(pGfpSliceCtxt->pGfpRowCtxt[i]);
            pGfpRowCtxt->grpId = grpId;
        }
        for (i = (newEnd+1); i<=(currEnd); i++) {
            pGfpRowCtxt = (sHYPHY20G_GFP_SCBS_ROW_CTXT *)
                                                &(pGfpSliceCtxt->pGfpRowCtxt[i]);
            pGfpRowCtxt->grpId = 0xFF;
        }

    } else {
    /* shift down */
        /* Adjust the end address to include the newly added rows below the group
           being shifted */
        newStart = currStart + shift;
        newEnd = currEnd + shift;
        result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, grpId,
                                                            currStart, newEnd);
        if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            sysHyPhy20gMemFree(pOnlineCalMemCopy);
            return result;
        }

        result = utilHyPhy20gScbsUpdateCalRows(fileHndl, pSchd, currGrpSize,
                    currStart, currStart+shift, pOnlineCalMemCopy);
        if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            sysHyPhy20gMemFree(pOnlineCalMemCopy);
            return result;
        }

        /* Adjust the start address to reflect the new values*/
        result = utilHyPhy20gScbsWriteSchdCtxt(fileHndl, pSchd, grpId,
                                                            newStart, newEnd);
        if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            sysHyPhy20gMemFree(pOnlineCalMemCopy);
            return result;
        }

        /* For each row that is no longer being used by the group, set the
           calendar entry to 0xFFFFFFFF */
        result = utilHyPhy20gScbsUpdateCalRows(fileHndl, pSchd, shift,
                    0, currStart, pDummyCalMem);
        if (result) {
            sysHyPhy20gMemFree(pDummyCalMem);
            sysHyPhy20gMemFree(pOnlineCalMemCopy);
            return result;
        }

        /* update GFP Row Context data for all the rows that have been
           modified */
        for (i = newStart; i<=newEnd; i++) {
             pGfpRowCtxt = (sHYPHY20G_GFP_SCBS_ROW_CTXT *)
                                                &(pGfpSliceCtxt->pGfpRowCtxt[i]);
            pGfpRowCtxt->grpId = grpId;
        }
        for (i = currStart; i<(currStart+shift); i++) {
            pGfpRowCtxt = (sHYPHY20G_GFP_SCBS_ROW_CTXT *)
                                                &(pGfpSliceCtxt->pGfpRowCtxt[i]);
            pGfpRowCtxt->grpId = 0xFF;
        }
    }
    sysHyPhy20gMemFree(pDummyCalMem);
    sysHyPhy20gMemFree(pOnlineCalMemCopy);

    return HYPHY20G_SUCCESS;
} /* gfpHyPhy20gGfpScbsGrpShift */

/*******************************************************************************
**
**  gfpHyPhy20gGfpCreateSliceCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates memory for tracking slice context.
**                  Before allocating memory, the CPB context must have been 
**                  created.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - 0 - Slice 'A'
**                                  - 1 - Slice 'B'
**                  noInitSchd      - 0 - initialize the TxGFPT scheduler
**                                  - 1 - not initialize the TxGFPT scheduler
**                  noInitRdChnl    - 0 - initialize CPB RdChannel context
**                                  - 1 - not initialize CPB RdChannel context
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpHyPhy20gGfpCreateSliceCtxt(struct file *fileHndl, UINT4 slice, 
                                    UINT4 noInitSchd, UINT4 noInitRdChnl)
{
    UINT4 size;
    UINT4 cpbChOffset;
    INT4 result;

    eHYPHY20G_SCBS_SCHD_ID schId;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_CHNL_CTXT  *pGfpChnlCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpSliceCtxt;
    sHYPHY20G_RD_CHNL_CTXT *pRdChnlCtxt;

    UINT4 i = 0;
    
    /* argument checking */
    if (slice > 1 || noInitSchd > 1 || noInitRdChnl > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Ensure that CPB context has been created */
    if(pDevCtxt->pCpbCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* allocate memory to GFP level context information for this slice */
    /* allocate memory */
    size = sizeof(sHYPHY20G_GFP_SLICE_CTXT);

    pGfpSliceCtxt = (sHYPHY20G_GFP_SLICE_CTXT *)sysHyPhy20gMemAlloc(size);

    if(pGfpSliceCtxt == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    if (slice == 0) {
        /* if structure already exists destroy previous to prevent memory
           leak */
        if(pDevCtxt->pGfpACtxt != NULL){
            gfpHyPhy20gGfpDestroySliceCtxt(fileHndl, 0);
        }

        pDevCtxt->pGfpACtxt = pGfpSliceCtxt;
    } else {
        /* if structure already exists destroy previous to prevent memory
           leak */
        if(pDevCtxt->pGfpBCtxt != NULL){
            gfpHyPhy20gGfpDestroySliceCtxt(fileHndl, 1);
        }

        pDevCtxt->pGfpBCtxt = pGfpSliceCtxt;
    }

    /* initialize the valid bit of Slice context to indicate invalid */
    pGfpSliceCtxt->valid = HYPHY20G_VALID_VAL_32BIT;
    pGfpSliceCtxt->maxGfpChnl = HYPHY20G_GFP_MAX_NUM_CHNL;
    pSchd = &(pGfpSliceCtxt->schd);
    pSchd->pOnLineCalMem = NULL;
    pSchd->pOffLineCalMem = NULL;
    pSchd->pScbsGrpCtxt = NULL;
    pSchd->valid = HYPHY20G_INVALID_VAL_32BIT;

    if (noInitSchd == 0) {
        /* initialize the TxGFPT scheduler */
        schId = (slice == 0) ? (HYPHY20G_GFP_TX_SCHD_0) : (HYPHY20G_GFP_TX_SCHD_1);
        
        result = utilHyPhy20gScbsInitSchd(fileHndl, pSchd, schId,
                          HYPHY20G_GFP_SCBS_NUM_ROWS,
                          HYPHY20G_SCBS_SCHD_BONUS_MODE_DISABLE);
        if (result) {
            gfpHyPhy20gGfpDestroySliceCtxt(fileHndl, slice);
            return result;
        }
    }
    
    /* set grpId to 0xFF for all rows of GFP context*/
    for (i = 0; i <= HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW; i++) {
        pGfpSliceCtxt->pGfpRowCtxt[i].grpId = HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER;
    }

    if (noInitRdChnl == 0) {
        /* set grpId to 0xFF for all rows of CPB RdChannel context*/
        for (i = 0; i < HYPHY20G_GFP_TX_SCHD_MAX_CHNL; i++) {
            cpbChOffset = HYPHY20G_CPB_GFPA_PORT_OFFSET
                                    + slice*HYPHY20G_CPB_GFPA_WR_SCHD_MAX_CHNL + i;
            pRdChnlCtxt = (sHYPHY20G_RD_CHNL_CTXT *)
                                            (pDevCtxt->pCpbCtxt->pRdChnl + cpbChOffset);
            pRdChnlCtxt->gfpGrpId = HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER;
        }
    }
    
    /* Initialize the channel context */
    /* allocate memory for GFP Channel Context */
    size = sizeof(sHYPHY20G_GFP_CHNL_CTXT);
    for (i = 0; i < HYPHY20G_GFP_MAX_NUM_CHNL; i++) {
        pGfpSliceCtxt->pGfpChnlCtxt[i] =
                           (sHYPHY20G_GFP_CHNL_CTXT *)sysHyPhy20gMemAlloc(size);
        if(pGfpSliceCtxt->pGfpChnlCtxt[i] == NULL) {
            gfpHyPhy20gGfpDestroySliceCtxt(fileHndl, slice);
            return HYPHY20G_ERR_MEM_ALLOC;
        }
    }

    /* provide default settings */
    for(i=0; i < HYPHY20G_GFP_MAX_NUM_CHNL; i++){
        pGfpChnlCtxt = (sHYPHY20G_GFP_CHNL_CTXT *)(pGfpSliceCtxt->pGfpChnlCtxt[i]);
        pGfpChnlCtxt->valid = HYPHY20G_VALID_VAL_32BIT;
        pGfpChnlCtxt->mode = HYPHY20G_GFP_UNUSED;
        pGfpChnlCtxt->numSb = 0;
        pGfpChnlCtxt->cpbPktLen = 0;
        pGfpChnlCtxt->gfpEncapLen = 0;
    }

    return HYPHY20G_SUCCESS;

} /* gfpHyPhy20gGfpCreateSliceCtxt */

/*******************************************************************************
**
**  gfpHyPhy20gGfpDestroySliceCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset context of the Gfp subsystem.  Deallocates memory
**                  used for storing this information.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - 0 - Slice 'A'
**                                  - 1 - Slice 'B'
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpHyPhy20gGfpDestroySliceCtxt(struct file *fileHndl,
                                    UINT4 slice)
{
    UINT4 i;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpSliceCtxt;
    sHYPHY20G_SCHD_CTXT *pSchd;

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* allocate memory to GFP level context information for this slice */
    if (slice == 0) {
        pGfpSliceCtxt = pDevCtxt->pGfpACtxt;
    } else {
        pGfpSliceCtxt = pDevCtxt->pGfpBCtxt;
    }

    if(pGfpSliceCtxt == NULL){
        /* Nothing to destroy */
      return HYPHY20G_SUCCESS;
    }

    pSchd = &(pGfpSliceCtxt->schd);
    /* destroy scheduler structures */
    utilHyPhy20gScbsDestroySchd(pSchd);

    for (i = 0; i < HYPHY20G_GFP_MAX_NUM_CHNL; i++) {
        if(pGfpSliceCtxt->pGfpChnlCtxt[i] != NULL) {
            sysHyPhy20gMemFree(pGfpSliceCtxt->pGfpChnlCtxt[i]);
        }
    }

    /* Release pGfpACtxt/pGfpBCtxt pointer */
    if (slice == 0) {
       sysHyPhy20gMemFree(pDevCtxt->pGfpACtxt);
    } else {
       sysHyPhy20gMemFree(pDevCtxt->pGfpBCtxt);
    }
    return HYPHY20G_SUCCESS;

} /* gfpHyPhy20gGfpDestroySliceCtxt */



/*******************************************************************************
**
**  gfpHyPhy20gGfpDestroyScbsRowCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Deallocates memory used for storing scheduler row
**                  context.
**
**  INPUTS:         fileHndl        - device handle
**                  pGfpRowCtxt     - pointer to row context to be allocated
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpHyPhy20gGfpDestroyScbsRowCtxt(struct file *fileHndl,
                                      sHYPHY20G_GFP_SCBS_ROW_CTXT *pGfpRowCtxt)
{

    if(pGfpRowCtxt == NULL){
        /* Nothing to destroy */
      return HYPHY20G_SUCCESS;
    }

    /* deallocate memory associated with slice structure */
    sysHyPhy20gMemFree(pGfpRowCtxt);

    return HYPHY20G_SUCCESS;

} /* gfpHyPhy20gGfpDestroyScbsRowCtxt */

/*******************************************************************************
**
**  gfpHyPhy20gGfpSetChnlCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Sets GFP channel context elements.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - GFP Slice
**                                    0 - GFP A
**                                    1 - GFP B
**                  chnl            - GFP Channel
**                                    0 to 63
**                  valid           - Validity of context:
**                                    HYPHY20G_VALID_VAL_32BIT - valid
**                                    Others - invalid
**                  mode            - GFP Channel mode
**                                    HYPHY20G_GFP_UNUSED = 0
**                                    HYPHY20G_GFP_T_AA = 1
**                                    HYPHY20G_GFP_T_FA = 2
**                                    HYPHY20G_GFP_F_PKT = 3
**                                    HYPHY20G_GFP_F_PHASE = 4
**                                    HYPHY20G_GFP_OTHER = 5
**                  numSb           - For HYPHY20G_GFP_T_AA or HYPHY20G_GFP_T_FA
**                                    set to the number of superblock used for
**                                    GFP-T encapsulation.  Otherwise set to 0.
**                  cpbPktLen       - Packet Length in CPB:
**                                    - GFP-T and GFP-F phase mode use
**                                    nominal PMM packet size
**                                    - GFP-F Packet mode does not process this
**                                    field
**                                    set to any value in range 0 to 9600
**                  gfpEncapLen     - Number bytes added by GFP subsystem:
**                                    Header + Extension Header + FCS
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpHyPhy20gGfpSetChnlCtxt(struct file *fileHndl, UINT4 slice, UINT4 chnl,
                               UINT4 valid, UINT4 mode, UINT4 numSb,
                               UINT4 cpbPktLen, UINT4 gfpEncapLen)
{

    sHYPHY20G_GFP_CHNL_CTXT *pGfpChnlCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpSliceCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if ((slice > 1)||(chnl > 63)||(mode > HYPHY20G_GFP_OTHER)||(numSb > 127)||
        (cpbPktLen > 512 && (mode == HYPHY20G_GFP_T_AA ||
         mode == HYPHY20G_GFP_T_FA)) ||
        (cpbPktLen > 9600 && mode == HYPHY20G_GFP_F_PHASE)||
        (cpbPktLen > 9600 && mode == HYPHY20G_GFP_F_PKT)||
        (gfpEncapLen > (4 + 64 + 4))) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    /* Where (4 + 64 + 4) represents 4 byte GFP header with TYPE bytes, 64
       byte extension header, 4 bytes for 32 bit FCS */

    DBG_PRINT("%s, %s, %d, chnl = %u, valid = %u, mode = %u, numSb = %u, cpbPktLen = %u, gfpEncapLen = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)chnl, (unsigned int)valid, (unsigned int)mode, (unsigned int)numSb, (unsigned int)cpbPktLen, (unsigned int)gfpEncapLen);

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to GFP level context information for this slice */
    if (slice == 0) {
        pGfpSliceCtxt = pDevCtxt->pGfpACtxt;
    } else {
        pGfpSliceCtxt = pDevCtxt->pGfpBCtxt;
    }
    HYPHY20G_ASSERT((pGfpSliceCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to channel context requested */
    pGfpChnlCtxt = (sHYPHY20G_GFP_CHNL_CTXT*)(pGfpSliceCtxt->pGfpChnlCtxt[chnl]);

    /* set channel context */
    pGfpChnlCtxt->valid = HYPHY20G_VALID_VAL_32BIT;
    pGfpChnlCtxt->mode = (eHYPHY20G_GFP_CHNL_MODE) mode;
    pGfpChnlCtxt->numSb = numSb;
    pGfpChnlCtxt->cpbPktLen = cpbPktLen;
    pGfpChnlCtxt->gfpEncapLen = gfpEncapLen;

    DBG_PRINT("%s, %s, %d, chnl = %u, pGfpChnlCtxt->valid = %u, pGfpChnlCtxt->mode = %u, pGfpChnlCtxt->numSb = %u, pGfpChnlCtxt->cpbPktLen = %u, pGfpChnlCtxt->gfpEncapLen = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)chnl, (unsigned int)pGfpChnlCtxt->valid, (unsigned int)pGfpChnlCtxt->mode, (unsigned int)pGfpChnlCtxt->numSb, (unsigned int)pGfpChnlCtxt->cpbPktLen, (unsigned int)pGfpChnlCtxt->gfpEncapLen);

    return HYPHY20G_SUCCESS;

} /* gfpHyPhy20gGfpSetChnlCtxt */

/*******************************************************************************
**
**  gfpHyPhy20gGfpSetMskChnlCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Sets one or more parameters of GFP channel context as
**                  controlled by setMsk argument.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - GFP Slice
**                                    0 - GFP A
**                                    1 - GFP B
**                  chnl            - GFP Channel
**                                    0 to 63
**                  valid           - Validity of context:
**                                    HYPHY20G_VALID_VAL_32BIT - valid
**                                    Others - invalid
**                  mode            - GFP Channel mode
**                                    HYPHY20G_GFP_UNUSED = 0
**                                    HYPHY20G_GFP_T_AA = 1
**                                    HYPHY20G_GFP_T_FA = 2
**                                    HYPHY20G_GFP_F_PKT = 3
**                                    HYPHY20G_GFP_F_PHASE = 4
**                                    HYPHY20G_GFP_OTHER = 5
**                  numSb           - For HYPHY20G_GFP_T_AA or HYPHY20G_GFP_T_FA
**                                    set to the number of superblock used for
**                                    GFP-T encapsulation.  Otherwise set to 0.
**                  cpbPktLen       - Packet Length in CPB:
**                                    - GFP-F Packet mode use min
**                                    packet length
**                                    - GFP-T and GFP-F phase mode use
**                                    nominal PMM packet size
**                  gfpEncapLen     - Number bytes added by GFP subsystem:
**                                    Header + Extension Header + FCS
**                  setMsk          - Mask of fields to update
**                                    bit 0: valid
**                                    bit 1: mode
**                                    bit 2: numSb
**                                    bit 3: cpbPktLen
**                                    bit 4: gfpEncapLen
**                                    Values:
**                                    0 - do not set field
**                                    1 - set field
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpHyPhy20gGfpSetMskChnlCtxt(struct file *fileHndl, UINT4 slice,
                                  UINT4 chnl, UINT4 valid, UINT4 mode,
                                  UINT4 numSb, UINT4 cpbPktLen,
                                  UINT4 gfpEncapLen, UINT4 setMsk)
{

    UINT4 devValid, devMode, devNumSb, devCpbPktLen, devGfpEncapLen;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(chnl > 63)||(setMsk > 0x1F)||
        (mode > HYPHY20G_GFP_OTHER && setMsk & 0x2)||
        (numSb > 127 && setMsk & 0x4)||
        (cpbPktLen > 512 && (mode == HYPHY20G_GFP_T_AA ||
         mode == HYPHY20G_GFP_T_FA || mode == HYPHY20G_GFP_F_PHASE) &&
         setMsk & 0x8)||
        (cpbPktLen > 9600 && mode == HYPHY20G_GFP_F_PKT && setMsk & 0x8)||
        (gfpEncapLen > (4 + 64 + 4) && setMsk & gfpEncapLen)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    /* Where (4 + 64 + 4) represents 4 byte GFP header with TYPE bytes, 64
       byte extension header, 4 bytes for 32 bit FCS */

    /* retrieve context settings */
    result = gfpHyPhy20gGfpGetChnlCtxt(fileHndl, slice, chnl, &devValid,
                                       &devMode, &devNumSb, &devCpbPktLen,
                                       &devGfpEncapLen);
    if (result) return result;

    /* specify values to update from arguments based on setMsk */
    if(setMsk & 0x1){ /* bit 0: valid */
        devValid = valid;
    }
    if(setMsk & 0x2){ /* bit 1: mode */
        devMode = mode;
    }
    if(setMsk & 0x4){ /* bit 2: numSb */
        devNumSb = numSb;
    }
    if(setMsk & 0x8){ /* bit 3: cpbPktLen */
        devCpbPktLen = cpbPktLen;
    }
    if(setMsk & 0x10){ /* bit 4: gfpEncapLen */
        devGfpEncapLen = gfpEncapLen;
    }

    /* update context settings */
    result = gfpHyPhy20gGfpSetChnlCtxt(fileHndl, slice, chnl, devValid,
                                       devMode, devNumSb, devCpbPktLen,
                                       devGfpEncapLen);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* gfpHyPhy20gGfpSetMskChnlCtxt */

/*******************************************************************************
**
**  gfpHyPhy20gGfpGetChnlCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Gets GFP channel context elements.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - GFP Slice
**                                    0 - GFP A
**                                    1 - GFP B
**                  chnl            - GFP Channel
**                                    0 to 63
**                 *pValid          - Pointer to storage for:
**                                    Validity of context:
**                                    HYPHY20G_VALID_VAL_32BIT - valid
**                                    Others - invalid
**                 *pMode           - Pointer to storage for:
**                                    GFP Channel mode
**                                    HYPHY20G_GFP_UNUSED = 0
**                                    HYPHY20G_GFP_T_AA = 1
**                                    HYPHY20G_GFP_T_FA = 2
**                                    HYPHY20G_GFP_F_PKT = 3
**                                    HYPHY20G_GFP_F_PHASE = 4
**                                    HYPHY20G_GFP_OTHER = 5
**                 *pNumSb          - Pointer to storage for:
**                                    For HYPHY20G_GFP_T_AA or HYPHY20G_GFP_T_FA
**                                    Set to the number of superblock used for
**                                    GFP-T encapsulation.  Otherwise set to 0.
**                 *pCpbPktLen      - Pointer to storage for:
**                                    Packet Length in CPB:
**                                    - GFP-F Packet mode use min
**                                    packet length
**                                    - GFP-T and GFP-F phase mode use
**                                    nominal PMM packet size
**                 *pGfpEncapLen    - Pointer to storage for:
**                                    Number bytes added by GFP subsystem:
**                                    Header + Extension Header + FCS
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpHyPhy20gGfpGetChnlCtxt(struct file *fileHndl, UINT4 slice, UINT4 chnl,
                               UINT4 *pValid, UINT4 *pMode, UINT4 *pNumSb,
                               UINT4 *pCpbPktLen, UINT4 *pGfpEncapLen)
{

    sHYPHY20G_GFP_CHNL_CTXT *pGfpChnlCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpSliceCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if ((slice > 1)||(chnl > 63)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to GFP level context information for this slice */
    if (slice == 0) {
        pGfpSliceCtxt = pDevCtxt->pGfpACtxt;
    } else {
        pGfpSliceCtxt = pDevCtxt->pGfpBCtxt;
    }
    HYPHY20G_ASSERT((pGfpSliceCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* Generate pointer to channel context requested */
    pGfpChnlCtxt = (sHYPHY20G_GFP_CHNL_CTXT*)(pGfpSliceCtxt->pGfpChnlCtxt[chnl]);

    /* Get channel context */
    *pValid = pGfpChnlCtxt->valid;
    *pMode = pGfpChnlCtxt->mode;
    *pNumSb = pGfpChnlCtxt->numSb;
    *pCpbPktLen = pGfpChnlCtxt->cpbPktLen;
    *pGfpEncapLen = pGfpChnlCtxt->gfpEncapLen;
    DBG_PRINT("%s, %s, %d, *pValid = %u, *pMode = %u, *pNumSb = %u, *pCpbPktLen = %u, *pGfpEncapLen = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*pValid, (unsigned int)*pMode, (unsigned int)*pNumSb, (unsigned int)*pCpbPktLen, (unsigned int)*pGfpEncapLen);

    return HYPHY20G_SUCCESS;

} /* gfpHyPhy20gGfpGetChnlCtxt */

/*******************************************************************************
**
**  gfpHyPhy20gGfpGenerateSchdData
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Evaluates the channel context for all CPBID channels used in
**                  in a VCAT group, determines the number of SCBS rows required
**                  generates the scheduler calendar memory contents on a best
**                  effort even distribution.
**
**                  This function uses the gfpGrpId parameter of
**                  sHYPHY20G_RD_CHNL_CTXT structure to identify which CPBIDs
**                  are associated with the group.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - GFP instance
**                              0 - GFP A
**                              1 - GFP B
**                  grpId     - VCAT group ID to have calendar data evaluated
**                              0 to 63
**                  compress  - GFP instance
**                              0 - use 2 rows per channel for the grpId
**                              1 - use 1 less row than normally for the grpId
**                 *pNumRows  - pointer to the number of calendar rows required
**                              by the group.  Range 0 to 126.
**                 *pCalData  - pointer to formatted scheduler calendar data
**                              function expects pointer to memory of size
**                              size = 4 * HYPHY20G_GFP_SCBS_NUM_ROWS *
**                                     sizeof(UINT4);
**
**
**  OUTPUTS:        None
**
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_EVALUATING_SCHD
**
*******************************************************************************/
INT4 gfpHyPhy20gGfpGenerateSchdData( struct file *fileHndl, UINT4 slice,
                                     UINT4 grpId, UINT4 compress, UINT4 *pNumRows,
                                     UINT4 *pCalData)
{
    sHYPHY20G_RD_CHNL_CTXT *pRdChnl;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_CPB_CTXT *pCpbCtxt;
    UINT4 chnlOffset;
    UINT4 totalChnlRate = 0, numCalChnl = 0;
    UINT4 lowestChnlRate, maxChnlRate;
    UINT4 chnlToCalMultFactor;
    UINT4 workingRate, workingEntries, chnlRate, scaleEntries;
    UINT4 numCalEntries, populateChnl, populateRate;
    UINT4 startIndex, offset, downOffset, upOffset, calSpace, calSingles;
    UINT4 i, j, k, size;
    UINT4 lowestIndex = 0, lowestRate;
    /* alloate memory for local use by function */
    UINT4 *pChnlNum, *pRawChnlNum, *pChnlRate, *pRawChnlRate;
    UINT4 *pLinearCal;
    UINT4 *pValid;

    /* verify pointer to global cpb data */
    sHYPHY20G_CTXT *pDevCtxt;


    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pCpbCtxt = pDevCtxt->pCpbCtxt;
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pCpbCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* reference scheduler for CPB GFP A or GFP B Read Interface Group */
    switch(slice){
        case 0:
            /* GFP A Read Scheduler */
            pSchd = pCpbCtxt->pSchdCtxt + GRP_A_RD_INTF_GRP;
            break;
        case 1:
            /* GFP B Read Scheduler */
            pSchd = pCpbCtxt->pSchdCtxt + GRP_B_RD_INTF_GRP;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

//NEED TO CHECK THAT CPB CONTEXT IS NOT NULL!!!!

    size = sizeof(UINT4)*(HYPHY20G_GFP_NUM_CPBID) +
           sizeof(UINT4)*(HYPHY20G_GFP_NUM_CPBID) +
           sizeof(UINT4)*(HYPHY20G_GFP_NUM_CPBID) +
           sizeof(UINT4)*(HYPHY20G_GFP_NUM_CPBID) +
           sizeof(UINT4)*((HYPHY20G_GFP_SCBS_NUM_ROWS)*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW)+
           sizeof(UINT4)*(HYPHY20G_GFP_NUM_CPBID);


    pChnlNum = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if(pChnlNum == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pRawChnlNum = (UINT4 *)(pChnlNum + HYPHY20G_GFP_NUM_CPBID);
    pChnlRate = (UINT4 *)(pRawChnlNum + HYPHY20G_GFP_NUM_CPBID);
    pRawChnlRate = (UINT4 *)(pChnlRate + HYPHY20G_GFP_NUM_CPBID);
    pLinearCal = (UINT4 *)(pRawChnlRate + HYPHY20G_GFP_NUM_CPBID);
    pValid = (UINT4 *)(pLinearCal + (HYPHY20G_GFP_SCBS_NUM_ROWS*
                         HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW));


    /* initialize channel data pointers */
    pRdChnl = NULL;

    /**************************************************************************/
    /*  EXTRACT CHANNEL RATE DATA FOR LOCAL USE BY FUNCTION                   */
    /**************************************************************************/

    if(slice){
        chnlOffset = HYPHY20G_GFP_GFPB_CPB_PORT_OFFSET;
    } else {
        chnlOffset = HYPHY20G_GFP_GFPA_CPB_PORT_OFFSET;
    }

    /* extract channel context for making calendar population decisions */
    /* Interface Group Contains Read Channels/Ports */
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pRdChnl +
                                         chnlOffset);

    j = 0;
    lowestChnlRate = 0xFFFFFFFF;
    maxChnlRate = 0;
    for (i=0; i < HYPHY20G_GFP_NUM_CPBID; i++) {
         if((pRdChnl->valid == HYPHY20G_VALID_SIGN)&&
           (pRdChnl->gfpGrpId == grpId)){

            *(pRawChnlNum + j) = pRdChnl->chnlId;
            *(pRawChnlRate + j) = pRdChnl->rate;
            totalChnlRate += *(pRawChnlRate+j);
            if (lowestChnlRate > *(pRawChnlRate + j)){
                lowestChnlRate = *(pRawChnlRate + j);
            }
            if (maxChnlRate < *(pRawChnlRate + j)) {
                maxChnlRate = *(pRawChnlRate + j);
            }
            numCalChnl++;
            j++;
        }
        pRdChnl++;
    }

// j or numCalChnl identifies how many channels are associated with the group
// we have a shorter list.
// so we know we have 0 to j-1 channels in the group.

    /* default set the return structure to all NULL addresses */


    /* determine number of calendar rows required */
    /* 1 row is assigned to a group with 1 CPBID.  2 rows are assigned to
       each CPBID within a group if there are greater than 1 CPBID.  0 rows
       assigned to a group with no CPBIDs associated with it. */
    if(numCalChnl == 0){
        *pNumRows = 0;
    } else if(numCalChnl == 1) {
        *pNumRows = 1;
    } else {
        if(numCalChnl == 64){
            *pNumRows = 127;
        } else {
            if (compress) {
                *pNumRows = (2 * numCalChnl) - 1;
            } else {
                *pNumRows = 2 * numCalChnl;
            }
        }
    }

    /**************************************************************************/
    /*  PROVIDE DATA FOR SIMPLEST POPULATION CASE                             */
    /**************************************************************************/

    /* populate scheduler content will NULL address */
    for (i=0; i < HYPHY20G_GFP_SCBS_NUM_ROWS; i++) {
        // GB - Looking to see if calling function has allocated adequate memory for pCalData
        for (j=0; j < HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW; j++){
            *(pCalData + (HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW*i) + j) =
                HYPHY20G_CPB_SCHD_NULL_CHNL;
        }
    }
    /* return if no CPBID in group */
    if(numCalChnl == 0){
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_SUCCESS;
    }

    /* return channel number in first row entries if single channel */
    if(numCalChnl == 1){
        for (j=0; j < HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW; j++){
            *(pCalData + j) = *pRawChnlNum;
        }
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_SUCCESS;
    }

    /**************************************************************************/
    /*  PUT CHANNEL RATE DATA IN ORDER FROM LOWEST TO HIGHEST RATE            */
    /**************************************************************************/

    /* sort chnlNum and chnlRate so that they are ordered from lowest to
       highest chnlRate */
    for(i=0; i < HYPHY20G_GFP_NUM_CPBID; i++){
        if(i < numCalChnl){
            *(pValid + i) = 1;
        } else {
            *(pValid + i) = 0;
        }
    }
    lowestRate = 0xFFFFFFFF;
    for(j=0; j < numCalChnl; j++) {
        /* find lowest valid in set */
        for(i=0; i < numCalChnl; i++) {
            if((*(pRawChnlRate + i) < lowestRate)&& *(pValid + i)){
                lowestRate = *(pRawChnlRate + i);
                lowestIndex = i;
            }
        }
        *(pChnlRate + j) = *(pRawChnlRate + lowestIndex);
        *(pChnlNum + j) = *(pRawChnlNum + lowestIndex);
        *(pValid + lowestIndex) = 0;
        lowestRate = 0xFFFFFFFF;
    }



    /* retrieve scheduler context to determine size of scheduler calendar
       needing to be populated */
    numCalEntries = HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW * *pNumRows;

    /**************************************************************************/
    /*  DEBUG PRINT INITIAL DATA                                              */
    /**************************************************************************/
    DBG_PRINT("%s, %s, %d, Initial Rate Data: with numCalEntries = %u, numCalChnl = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) numCalEntries, (unsigned int) numCalChnl);
    for(i=0; i < numCalChnl; i++){
        if(*(pChnlRate + i) != 0){
    DBG_PRINT("%s, %s, %d, chnl = %u, rate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) *(pChnlNum + i), (unsigned int) *(pChnlRate + i));
        }
    }

    /******************************************************************************/
    /*  END DEBUG PRINT INITIAL DATA                                              */
    /******************************************************************************/

    DBG_PRINT("%s, %s, %d, Got to here: numCalEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) numCalEntries);
    if (numCalChnl > numCalEntries){
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_ERR_SCHD_CAL_SIZE;
    }

    /**************************************************************************/
    /*  SCALE CALENDAR ENTRIES                                                */
    /**************************************************************************/

  /* ensure totalChnlRate > numCalEntries */
    if (totalChnlRate < numCalEntries){
        if (totalChnlRate == 0) {
            sysHyPhy20gMemFree(pChnlNum);
            return HYPHY20G_ERR_EVALUATING_SCHD;
        }
        chnlToCalMultFactor = (numCalEntries/totalChnlRate)+1;

        for (i=0; i < numCalChnl; i++){

            *(pChnlRate + i) *= chnlToCalMultFactor;

        }
        totalChnlRate *= chnlToCalMultFactor;
    }

    /* scale required calendar entries so that they can be contained in
       the number of available calendar entries ensuring that at least one
       entry is present for a channel*/

    workingRate = totalChnlRate;
    workingEntries = numCalEntries;
    for(i=0; i < numCalChnl; i++){
        if(i == (numCalChnl-1)){
            /* ensure last channel consumes remainder of calendar entries */
            *(pChnlRate+i) = workingEntries;

        } else {
            chnlRate = *(pChnlRate+i);
            chnlRate *= 100;
            workingRate *= 100;
            workingEntries *= 100;

            /* confirm workingRate is not 0. */
            HYPHY20G_ASSERT((workingRate != 0), HYPHY20G_ERR_ZERO_DIVIDE,
                            "%s", "workingRate is 0.");
            scaleEntries = (chnlRate*workingEntries)/workingRate;
            if ((scaleEntries%100) > 49){
                DBG_PRINT("%s, %d, got here\n",__FUNCTION__, __LINE__);
                scaleEntries = (scaleEntries/100) + 1;
            } else {
                DBG_PRINT("%s, %d, got here\n",__FUNCTION__, __LINE__);
                scaleEntries = scaleEntries/100;
             }
            /* ensure a minimum of 1 entry */
            if(scaleEntries == 0){
                scaleEntries = 1;
            }

            /* update results */
            workingRate = workingRate/100;
            workingRate -= *(pChnlRate+i);
            workingEntries = workingEntries/100;
            workingEntries -= scaleEntries;
            *(pChnlRate+i) = scaleEntries;
        }
    }



    DBG_PRINT("%s, %s, %d, Scaled Calendar Data\n",__FILE__, __FUNCTION__, __LINE__);
    /**************************************************************************/
    /*  DEBUG PRINT INITIAL DATA                                              */
    /**************************************************************************/
    DBG_PRINT("%s, %s, %d, Initial Rate Data: with numCalEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) numCalEntries);
    for(i=0; i < numCalChnl; i++){
        if(*(pChnlRate + i) != 0){
    DBG_PRINT("%s, %s, %d, chnl = %u, rate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) *(pChnlNum + i), (unsigned int) *(pChnlRate + i));
        }
    }

    /**************************************************************************/
    /*  END DEBUG PRINT INITIAL DATA                                          */
    /**************************************************************************/


    /**************************************************************************/
    /*  EVENLY DISTRIBUTE CALENDAR ENTRIES                                    */
    /**************************************************************************/

    /* populate the calendar so that entries are roughly evenly distributed */
    for(i=0; i < numCalEntries; i++){
        *(pLinearCal + i) = HYPHY20G_CPB_SCHD_NULL_CHNL;
    }
    DBG_PRINT("%s, %s, %d, Got to startIndex = 0\n",__FILE__, __FUNCTION__, __LINE__);

    startIndex = 0;
    for(i=0; i < numCalChnl; i++){

        populateRate = *(pChnlRate + i);
        populateChnl = *(pChnlNum + i);
        /* identify the required number of calendar entries between each
           occurance of this channel number */
        calSpace = numCalEntries/populateRate;
        calSingles = numCalEntries%populateRate;
        for(j=0; j < populateRate; j++){
            /* identify optimal distribution */
            if (j == 0){
                offset = startIndex;
            } else {
                if(calSingles){
                    offset = startIndex + (j * calSpace) + 1;
                    calSingles--;
                } else {
                    offset = startIndex + (j * calSpace);
                }
            }
            if(offset >= numCalEntries){
                offset = numCalEntries - 1;
            }

            /* attempt to apply optimal location and if not jitter to nearest
               available calendar entry */
            if(*(pLinearCal + offset) == HYPHY20G_CPB_SCHD_NULL_CHNL) {
                *(pLinearCal + offset) = populateChnl;
            } else {
                /* determine number up to next entry */
                k = offset + 1;
                upOffset = 0xFFFF;
                while ((k < numCalEntries)&&(upOffset==0xFFFF)){
                    if(*(pLinearCal + k) == HYPHY20G_CPB_SCHD_NULL_CHNL){
                        upOffset = k;
                    }
                    k++;
                }
                /* determine number down to next entry */
                k = offset - 1;
                downOffset = 0xFFFF;
                while (((k >= 0)&&(k < numCalEntries))&&(downOffset==0xFFFF)){
                    if(*(pLinearCal + k) == HYPHY20G_CPB_SCHD_NULL_CHNL){
                        downOffset = k;
                    }
                    k--;
                }
                /* populate channel number to closest entry */
                if((offset - downOffset)>(upOffset - offset)){
                    /* upOffset is smaller or equal */
                    *(pLinearCal + upOffset) = populateChnl;
                } else {
                    /* downOffset is smaller */
                    *(pLinearCal + downOffset) = populateChnl;
                }
            }
        }
        startIndex++;
    }
DBG_PRINT("%s, %s, %d, Got to here\n",__FILE__, __FUNCTION__, __LINE__);

    /**************************************************************************/
    /*  FORMAT CALENDAR ENTRY DATA TO ROWS OF SCBS                            */
    /**************************************************************************/

    /* apply the linear calendar data to the device scheduler calendar
       format */
    j = 0;
    for(i = 0; i < *pNumRows; i++){
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW)) =
            *(pLinearCal + j);
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW + 1)) =
            *(pLinearCal + (j+1));
        j += 2;
    }

    for(i=0; i < *pNumRows; i++){
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW + 2)) =
            *(pLinearCal + j);
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW + 3)) =
            *(pLinearCal + (j+1));
        j += 2;
    }

    /* free allocated memory used by local function */
    sysHyPhy20gMemFree(pChnlNum);
    return HYPHY20G_SUCCESS;

} /* gfpHyPhy20gGfpGenerateSchdData */

/*******************************************************************************
**
**  gfpHyPhy20gGfpGenerateRateAwareSchdData
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Evaluates the channel context for all CPBID channels used in
**                  in a VCAT group:
**                  - evaluates SCBS rows available for group
**                  - generates SCBS number of entries per CPB channel such that
**                    required frame rate is met
**                  - evaluates if there are enough entries to support the
**                    required frame rate of the CPB channel, if not rejects
**                    addition
**                  - formats these entries so they are evenly distributed on a
**                    best effort basis
**
**                  This function uses the gfpGrpId parameter of
**                  sHYPHY20G_RD_CHNL_CTXT structure to identify which CPBIDs
**                  are associated with the group.
**
**                  This function uses the sHYPHY20G_GFP_CHNL_CTXT structure
**                  to retrieve context related to the GFP channel in order to
**                  calculate the minimum entries required to support the frame
**                  rate based scheduling.
**
**  INPUTS:         fileHndl    - device handle
**                  slice       - GFP instance
**                                0 - GFP A
**                                1 - GFP B
**                  chnlId      - GFP Channel to be added
**                                0 to 63
**                  addRemove   - Operation to be performed on chnlId
**                                0 - Remove chnlId from VCAT Group
**                                1 - Add chnlId to VCAT Group
**                  grpId       - VCAT group ID to have calendar data evaluated
**                                0 to 63
**                  mode        - For addRemove set to 1:
**                                GFP Channel mode
**                                HYPHY20G_GFP_UNUSED = 0
**                                HYPHY20G_GFP_T_AA = 1
**                                HYPHY20G_GFP_T_FA = 2
**                                HYPHY20G_GFP_F_PKT = 3
**                                HYPHY20G_GFP_F_PHASE = 4
**                                HYPHY20G_GFP_OTHER = 5
**                  numSb       - For addRemove set to 1:
**                                For HYPHY20G_GFP_T_AA or HYPHY20G_GFP_T_FA
**                                set to the number of superblock used for
**                                GFP-T encapsulation.  Otherwise set to 0.
**                  cpbPktLen   - For addRemove set to 1:
**                                Packet Length in CPB:
**                                - GFP-F Packet mode use min
**                                packet length
**                                - GFP-T and GFP-F phase mode use
**                                nominal PMM packet size
**                  gfpEncapLen - For addRemove set to 1:
**                                Number bytes added by GFP subsystem:
**                                Header + Extension Header + FCS
**                  compress    - GFP instance
**                                0 - use 2 rows per channel for the grpId
**                                1 - use 1 less row than normally for the grpId
**                 *pNumRows    - pointer to the number of calendar rows
**                                required by the group.  Range 0 to 126.
**                 *pCalData    - pointer to formatted scheduler calendar data
**                                function expects pointer to memory of size
**                                size = 4 * HYPHY20G_GFP_SCBS_NUM_ROWS *
**                                       sizeof(UINT4);
**                 *pOptimal    - For addRemove set to 0:
**                                Pointer to storage for the optimized state of
**                                SCBS contents:
**                                0 - Optimal.  SCBS meets frame rate request
**                                    requirements of channels.
**                                Other - Sub-Optimal.  SCBS does not guarantee
**                                    frame rate request requirements of
**                                    channels.
**
**
**  OUTPUTS:        None
**
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_VAR_OVERFLOW
**                  HYPHY20G_ERR_SCHD_BW
**                  HYPHY20G_ERR_INVALID_GFP_CHNL_MODES
**                  HYPHY20G_ERR_ZERO_DIVIDE
**                  HYPHY20G_ERR_SCHD_CAL_SIZE
**                  HYPHY20G_ERR_SCHD_ALGO_FAIL
**                  HYPHY20G_ERR_EVALUATING_SCHD
**
*******************************************************************************/
INT4 gfpHyPhy20gGfpGenerateRateAwareSchdData( struct file *fileHndl,
                                              UINT4 slice, UINT4 chnlId,
                                              UINT4 addRemove, UINT4 grpId,
                                              UINT4 mode, UINT4 numSb,
                                              UINT4 cpbPktLen, UINT4 gfpEncapLen,
                                              UINT4 compress, UINT4 *pNumRows,
                                              UINT4 *pCalData, UINT4 *pOptimal)
{
    sHYPHY20G_RD_CHNL_CTXT *pRdChnl;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_CPB_CTXT *pCpbCtxt;
    UINT4 chnlOffset;
    UINT4 numCalChnl = 0;
    UINT4 lowestChnlRate, maxChnlRate;
    UINT4 devValid, devMode, devNumSb, devCpbPktLen, devGfpEncapLen, modeMsk;
    UINT4 totalLineRate, totalFrmRate, evalChnlRate, evalChnlNum;
     UINT4 frmRate, lineRate, evalLineRateUnits, reject, cbrModeMsk, gfpFpktMsk;
    UINT4 availLineRate = 0;
    UINT4 workingEntries;
    UINT4 numCalEntries, populateChnl, populateRate;
    UINT4 calSingles, calSpace;
    UINT4 denToKbps, lineRateUnits;
    UINT4 i, j, k, size;
    UINT4 lowestIndex = 0, lowestRate;
    UINT4 idleLineRate, idleReqRate, vcatReqRate, calEntryReqRate, numRows;
    UINT4 totalMinEntries, entryRate;
    UINT4 minEntries, swap, tmp, assignEntries;
    UINT4 numEntriesReqd, numEntriesStole = 0, numEntriesAvail;
    UINT4 numEmptyLoc, newNumEmptyLoc, halfNumCalEntries, updatedNumCalChnl;
    UINT4 splitLowRate, sum, emptyLocOffset, linearCaloffset;
    UINT4 memType, numMem, txBitsPerScbsChnlReq;

    /* alloate memory for local use by function */
    UINT4 *pChnlNum, *pRawChnlNum, *pChnlRate, *pRawChnlRate, *pFrmRate;
    UINT4 *pAssignEntries, *pMinEntries, *pEmptyLoc;
    UINT4 *pLinearCal;
    UINT4 *pValid;
    INT4 result;

    /* verify pointer to global cpb data */
    sHYPHY20G_CTXT *pDevCtxt;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* retrieve pointer to device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pCpbCtxt = pDevCtxt->pCpbCtxt;
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pCpbCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* reference scheduler for CPB GFP A or GFP B Read Interface Group */
    switch(slice){
        case 0:
            /* GFP A Read Scheduler */
            pSchd = pCpbCtxt->pSchdCtxt + GRP_A_RD_INTF_GRP;
            break;
        case 1:
            /* GFP B Read Scheduler */
            pSchd = pCpbCtxt->pSchdCtxt + GRP_B_RD_INTF_GRP;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pSchd != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* allocate memory for local use by function */
    size = sizeof(UINT4)*(HYPHY20G_GFP_NUM_CPBID+1) +
           sizeof(UINT4)*(HYPHY20G_GFP_NUM_CPBID) +
           sizeof(UINT4)*(HYPHY20G_GFP_NUM_CPBID) +
           sizeof(UINT4)*(HYPHY20G_GFP_NUM_CPBID) +
           sizeof(UINT4)*((HYPHY20G_GFP_SCBS_NUM_ROWS)*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW)+
           sizeof(UINT4)*(HYPHY20G_GFP_NUM_CPBID) +
           sizeof(UINT4)*(HYPHY20G_GFP_NUM_CPBID) +
           sizeof(UINT4)*(HYPHY20G_GFP_NUM_CPBID) +
           sizeof(UINT4)*(HYPHY20G_GFP_NUM_CPBID+1);


    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    pChnlNum = (UINT4 *) sysHyPhy20gMemAlloc(size);
    if(pChnlNum == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }
    pRawChnlNum = (UINT4 *)(pChnlNum + HYPHY20G_GFP_NUM_CPBID + 1);
    pChnlRate = (UINT4 *)(pRawChnlNum + HYPHY20G_GFP_NUM_CPBID);
    pRawChnlRate = (UINT4 *)(pChnlRate + HYPHY20G_GFP_NUM_CPBID);
    pLinearCal = (UINT4 *)(pRawChnlRate + HYPHY20G_GFP_NUM_CPBID);
    pValid = (UINT4 *)(pLinearCal + (HYPHY20G_GFP_SCBS_NUM_ROWS*
                         HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW));
    pEmptyLoc    = (UINT4 *)(pValid + HYPHY20G_GFP_NUM_CPBID);
    pMinEntries  = (UINT4 *)(pEmptyLoc + HYPHY20G_GFP_NUM_CPBID);
    pAssignEntries = (UINT4 *)(pMinEntries + HYPHY20G_GFP_NUM_CPBID);

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* initially set the pointed to memory for the scheduling status */
    *pOptimal = 0;


    /**************************************************************************/
    /*  EXTRACT CHANNEL RATE DATA FOR LOCAL USE BY FUNCTION                   */
    /**************************************************************************/

    if(slice){
        chnlOffset = HYPHY20G_GFP_GFPB_CPB_PORT_OFFSET;
    } else {
        chnlOffset = HYPHY20G_GFP_GFPA_CPB_PORT_OFFSET;
    }

    /* initialize channel data pointers */
    pRdChnl = NULL;

    /* extract channel context for making calendar population decisions */
    /* Interface Group Contains Read Channels/Ports */
    pRdChnl = (sHYPHY20G_RD_CHNL_CTXT *)(pDevCtxt->pCpbCtxt->pRdChnl +
                                         chnlOffset);

    j = 0;
    lowestChnlRate = 0xFFFFFFFF;
    maxChnlRate = 0;
    for (i=0; i < HYPHY20G_GFP_NUM_CPBID; i++) {
         if((pRdChnl->valid == HYPHY20G_VALID_SIGN)&&
           (pRdChnl->gfpGrpId == grpId)){

            *(pRawChnlNum + j) = pRdChnl->chnlId;
            *(pRawChnlRate + j) = pRdChnl->rate;
            if (lowestChnlRate > *(pRawChnlRate + j)){
                lowestChnlRate = *(pRawChnlRate + j);
            }
            if (maxChnlRate < *(pRawChnlRate + j)) {
                maxChnlRate = *(pRawChnlRate + j);
            }
            numCalChnl++;
            j++;
        }
        pRdChnl++;
    }

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* determine number of calendar rows required */
    /* 1 row is assigned to a group with 1 CPBID.  2 rows are assigned to
       each CPBID within a group if there are greater than 1 CPBID.  0 rows
       assigned to a group with no CPBIDs associated with it. */
    if(numCalChnl == 0){
        *pNumRows = 0;
    } else if(numCalChnl == 1) {
        *pNumRows = 1;
    } else {
        if(numCalChnl == 64){
            *pNumRows = 127;
        } else {
            if (compress) {
                *pNumRows = (2 * numCalChnl) - 1;
            } else {
                *pNumRows = 2 * numCalChnl;
            }
        }
    }


    /**************************************************************************/
    /*  PROVIDE DATA FOR SIMPLEST POPULATION CASE                             */
    /**************************************************************************/

    /* populate scheduler content will NULL address */
    for (i=0; i < HYPHY20G_GFP_SCBS_NUM_ROWS; i++) {
        for (j=0; j < HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW; j++){
            *(pCalData + (HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW*i) + j) =
                HYPHY20G_CPB_SCHD_NULL_CHNL;
        }
    }
    /* return if no CPBID in group */
    if(numCalChnl == 0){
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_SUCCESS;
    }

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /**************************************************************************/
    /*  PUT CHANNEL RATE DATA IN ORDER FROM LOWEST TO HIGHEST RATE            */
    /**************************************************************************/
    /* set valid context for each used channel */
    for(i=0; i < HYPHY20G_GFP_NUM_CPBID; i++){
        if(i < numCalChnl){
            *(pValid + i) = 1;
        } else {
            *(pValid + i) = 0;
        }
    }

    /* sort */
    lowestRate = 0xFFFFFFFF;
    for(j=0; j < numCalChnl; j++) {
        /* find lowest valid in set */
        for(i=0; i < numCalChnl; i++) {
            if((*(pRawChnlRate + i) < lowestRate)&& *(pValid + i)){
                lowestRate = *(pRawChnlRate + i);
                lowestIndex = i;
            }
        }
        *(pChnlRate + j) = *(pRawChnlRate + lowestIndex);
        *(pChnlNum + j) = *(pRawChnlNum + lowestIndex);
        *(pValid + lowestIndex) = 0;
        lowestRate = 0xFFFFFFFF;
    }

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /**************************************************************************/
    /* extract and store GFP chnl context of channel to be added              */
    /* if the addition is not successful, the context needs to be restored to */
    /* these values                                                           */
    /**************************************************************************/
    result = gfpHyPhy20gGfpGetChnlCtxt(fileHndl, slice, chnlId, &devValid,
                                       &devMode, &devNumSb, &devCpbPktLen,
                                       &devGfpEncapLen);
    if(result){
        sysHyPhy20gMemFree(pChnlNum);
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
        return result;
    }

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /**************************************************************************/
    /* update GFP chnl context for the channel to be added or removed         */
    /**************************************************************************/
    if(addRemove==1){
        DBG_PRINT("%s, %s, %d gfpHyPhy20gGfpSetChnlCtxt for addRemove == 1\n",__FILE__, __FUNCTION__, __LINE__);
        result = gfpHyPhy20gGfpSetChnlCtxt(fileHndl, slice, chnlId, devValid,
                                           mode, numSb, cpbPktLen, gfpEncapLen);
    }
    if(addRemove==0){
        DBG_PRINT("%s, %s, %d gfpHyPhy20gGfpSetChnlCtxt for addRemove == 0\n",__FILE__, __FUNCTION__, __LINE__);
        result = gfpHyPhy20gGfpSetChnlCtxt(fileHndl, slice, chnlId, devValid,
                                           HYPHY20G_GFP_UNUSED, 0, 0, 0);
    }
    if(result){
        if(addRemove==1){
            /* return channel context */
            gfpHyPhy20gGfpSetChnlCtxt(fileHndl, slice, chnlId, devValid,
                                      devMode, devNumSb, devCpbPktLen,
                                      devGfpEncapLen);
        }
        sysHyPhy20gMemFree(pChnlNum);
        DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
        return result;
    }

    /**************************************************************************/
    /* return channel number in first row entries if single channel           */
    /* - implement here as GFP Channel ctxt has been updated                  */
    /**************************************************************************/
    if(numCalChnl == 1){
        for (j=0; j < HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW; j++){
            *(pCalData + j) = *pRawChnlNum;
        }
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_SUCCESS;
    }


    /**************************************************************************/
    /*  Determine frame request rate and payload rate of frame data for       */
    /*  channels in group                                                     */
    /**************************************************************************/

    /* repurpose memory used to store raw channel data to store frame rate */
    pFrmRate = pRawChnlRate;
    modeMsk = 0;
    totalLineRate = 0;
    totalFrmRate = 0;
    lineRateUnits = 0; /* 0 - bps, 1 - 10*bps */
    denToKbps = 1000; /* for lineRateUnits == 0 /1000 to get kbps */
    for(i=0; i < numCalChnl; i++){
        /* retrieve the mode, frmRate and lineRate for the channel */
        evalChnlRate = *(pChnlRate + i);
        evalChnlNum = *(pChnlNum + i);
        result = gfpHyPhy20gGfpEvalChnlCtxt(fileHndl, slice, evalChnlNum,
                                            evalChnlRate, &mode, &frmRate,
                                            &lineRate, &evalLineRateUnits);
        if(result){
            gfpHyPhy20gGfpSetChnlCtxt(fileHndl, slice, chnlId, devValid,
                                      devMode, devNumSb, devCpbPktLen,
                                      devGfpEncapLen);
            sysHyPhy20gMemFree(pChnlNum);
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
            return result;
        }
        DBG_PRINT("%s, %s, %d, evalChnlNum = %u, evalChnlRate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) evalChnlNum, (unsigned int) evalChnlRate);
        DBG_PRINT("%s, %s, %d, mode = %u, frmRate = %u, lineRate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) mode, (unsigned int) frmRate, (unsigned int) lineRate);
        /* collect GFP modes used by channel in interface */
        modeMsk |= (1 << mode);

        /* accumulate total line rate for CBR clients */
        if(mode == HYPHY20G_GFP_T_AA || mode == HYPHY20G_GFP_T_FA ||
           mode == HYPHY20G_GFP_F_PHASE){
            if ((totalLineRate + lineRate > totalLineRate) && (lineRateUnits == 0) && (evalLineRateUnits == 0)){
                totalLineRate += lineRate;
                DBG_PRINT("%s, %s, %d, totalLineRate = %u, lineRateUnits = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)totalLineRate, (unsigned int) lineRateUnits);
            } else {
                if (lineRateUnits == 0){
                    totalLineRate = totalLineRate / 10;
                    lineRateUnits = 1;
                    denToKbps = 100; /* for lineRateUnits == 1 /100 to get kbps */
                }
                if (evalLineRateUnits == 0) { /* lineRate units in bps */
                    totalLineRate += (lineRate/10);
                } else { /* lineRateUnits in 10x bps */
                    totalLineRate += lineRate;
                }
                DBG_PRINT("%s, %s, %d, totalLineRate = %u, lineRateUnits = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)totalLineRate, (unsigned int) lineRateUnits);
            }
        }

        /* store frame rate and generate total frame rate */
        if(0xFFFFFFFF - frmRate < totalFrmRate){
            sysHyPhy20gMemFree(pChnlNum);
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_VAR_OVERFLOW\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_VAR_OVERFLOW;
        }
        *(pFrmRate + i) = frmRate;
        totalFrmRate += frmRate;
    }

    /**************************************************************************/
    /*  A) FOR CBR CALCULATE THE INTERFACE RATE REQUIRED                      */
    /*  B) DETERMINE IF THERE IS A MODE VIOLATION - MIXTURE OF GFP-F PACKET   */
    /*     AND CBR                                                            */
    /**************************************************************************/
    /* for greater than one channel of CBR (not "GFP-F Packet" mode) VCAT
       payload rate cannot be oversubscribed. The available interface rate
       will be compared with the GFP data rate of each channel */

    /* evaluate if channel addition to be rejected due to incompatible channel
       mode mix or insufficient VCAT bandwidth for CBR clients */
    reject = 0;

    cbrModeMsk = (1 << HYPHY20G_GFP_T_AA) + (1 << HYPHY20G_GFP_T_FA) +
                 (1 << HYPHY20G_GFP_F_PHASE);
    gfpFpktMsk = (1 << HYPHY20G_GFP_F_PKT);

    /* evaluate if there is a mixture of GFP-F Packet and CBR GFP channels */
    if((cbrModeMsk & modeMsk)&&(gfpFpktMsk & modeMsk)){
        reject = 1;
    }

    /* evaluate if cbr channels are oversubscribed */
    if((modeMsk & cbrModeMsk)&&(reject == 0)){
        /* retrieve available VCAT payload bandwidth in kbps */
        result = gfpHyPhy20gGfpGetVcatGrpRate(fileHndl, slice, grpId,
                                              &availLineRate);
        /* reject if multiplexed channels are oversubscribed */
        /* availLineRate in kbps, totalLineRate in bps or 10*bps */
        if( availLineRate < (totalLineRate/denToKbps +
           (totalLineRate%denToKbps ? 1:0))){
            reject = 2;
            /* configure totalLineRate for successful execution of
               future code */
            totalLineRate = availLineRate;
            totalLineRate *= denToKbps;
        }
    }

    if((reject || result) && addRemove){
        /* reset context associated with channel to be added */
        gfpHyPhy20gGfpSetChnlCtxt(fileHndl, slice, chnlId, devValid,
                                  devMode, devNumSb, devCpbPktLen,
                                  devGfpEncapLen);
        sysHyPhy20gMemFree(pChnlNum);
        if(result){
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
            return result;
        }
        if (reject == 2){
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_BW\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_BW;
        }
        /* reject == 1 case */
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_GFP_CHNL_MODES\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_GFP_CHNL_MODES;
    }

    if((reject || result) && !addRemove){
        *pOptimal = 1;
    }
    /* end of...
       evaluate if channel addition to be rejected due to incompatible channel
       mode mix or insufficient VCAT bandwidth for CBR clients */

    /**************************************************************************/
    /*  FOR GFP-F PACKET CLIENTS POPULATE AS PER TRADITIONAL METHOD AND EXIT  */
    /**************************************************************************/
    if(gfpFpktMsk & modeMsk){
        result = gfpHyPhy20gGfpGenerateSchdData( fileHndl, slice, grpId, compress,
                                                 &numRows, pLinearCal);
        /* transfer pLinearCal to pCalData for use by calling function */
        for(i = 0; i < HYPHY20G_GFP_SCBS_NUM_ROWS*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW; i++){
            *(pCalData + i) = *(pLinearCal + i);
        }
        *pNumRows = numRows;
        if (result){
            /* reset context associated with channel to be added */
            if(addRemove){
                gfpHyPhy20gGfpSetChnlCtxt(fileHndl, slice, chnlId, devValid,
                                          devMode, devNumSb, devCpbPktLen,
                                          devGfpEncapLen);
            }
            sysHyPhy20gMemFree(pChnlNum);
            DBG_PRINT("%s, %s, %d, Error Code %d\n",__FILE__, __FUNCTION__, __LINE__, (int)result);
            return result;
        }
        sysHyPhy20gMemFree(pChnlNum);
        return HYPHY20G_SUCCESS;
    }

    /* determine number of calendar entries available for population */
    numCalEntries = HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW * *pNumRows;

    /**************************************************************************/
    /* Determine the frame request rate per SCBS calendar entry for           */
    /* the VCAT group                                                         */
    /* a request to SCBS requires 4 system level clocks to return a result    */
    /* VCAT group will make a request every 32 bits at line data rate         */
    /* for a VCAT group request 1, 2, 3 or 4 IDLE may be generated on the     */
    /* if no data available in CPB based on the ratio of system clock to line */
    /* line clock                                                             */
    /**************************************************************************/
    /* retrieve system clock rate */
    /* div32 of line rate 
       OTN - 334.66 MHz
       SONET - 311.04 MHz */
    result = vcatHyPhy20gVcatGetGrpRate(fileHndl, slice, grpId, &memType, &numMem);
    if(result) {
        sysHyPhy20gMemFree(pChnlNum);
        return result;
    }
    /* compare the vcat payload rate with the rate threshold for providing
       1, 2, 3 or 4 idles per SCBS channel request */
    if(memType > 6){ /* OTN */
        if( availLineRate <= 2772800 ){
            txBitsPerScbsChnlReq = 32; /* 1 GFP idle */
        } else if( availLineRate <= 5354560 ){
            txBitsPerScbsChnlReq = 64; /* 2 GFP idle */
        } else if( availLineRate <= 8031840 ){
            txBitsPerScbsChnlReq = 96; /* 3 GFP idle */
        } else {
            txBitsPerScbsChnlReq = 128; /* 4 GFP idle */
        }
    } else { /* SONET */
        if( availLineRate <= 2488320 ){
            txBitsPerScbsChnlReq = 32; /* 1 GFP idle */
        } else if( availLineRate <= 4976640 ){
            txBitsPerScbsChnlReq = 64; /* 2 GFP idle */
        } else if( availLineRate <= 7464960 ){
            txBitsPerScbsChnlReq = 96; /* 3 GFP idle */
        } else {
            txBitsPerScbsChnlReq = 128; /* 4 GFP idle */
        }
    }

    DBG_PRINT("%s, %s, %d, txBitsPerScbsChnlReq = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) txBitsPerScbsChnlReq);

    /* idleLineRate in kbps */
    if (availLineRate < 0xFFFFFFFF/denToKbps) {
        /* use available precision of totalLineRate */
        idleLineRate = availLineRate*denToKbps - totalLineRate; /* units specified by lineRateUnits */
        /* generate idleReqRate in units of requests/sec */
        idleReqRate = idleLineRate / txBitsPerScbsChnlReq + (idleLineRate%txBitsPerScbsChnlReq ? 1:0);
        if(lineRateUnits==1){
            idleReqRate *= 10;
        }
    } else { /* perform calculation using units of kbps */
        totalLineRate = totalLineRate/denToKbps + (totalLineRate%denToKbps ? 1:0);
        idleLineRate = availLineRate - totalLineRate; /* units of kbps */
        idleReqRate = idleLineRate / txBitsPerScbsChnlReq + (idleLineRate%txBitsPerScbsChnlReq ? 1:0);
        /* generate idleReqRate in units of requests/sec */
        idleReqRate *= 1000;
    }

    /* vcatReqRate is the rate that SCBS entries are requested to change from
       one CPBID to another for the group */
    vcatReqRate = totalFrmRate + idleReqRate; /* units of requests/sec */

    DBG_PRINT("%s, %s, %d, availLineRate = %u, totalLineRate = %u, lineRateUnits = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) availLineRate, (unsigned int) totalLineRate, (unsigned int) lineRateUnits);
    DBG_PRINT("%s, %s, %d, idleLineRate = %u, totalFrmRate = %u, vcatReqRate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) idleReqRate, (unsigned int) totalFrmRate, (unsigned int) vcatReqRate);

    /* return error if numCalEntries == 0 */
    if (numCalEntries == 0) {
        if(addRemove){
            /* reset context associated with channel to be added */
            gfpHyPhy20gGfpSetChnlCtxt(fileHndl, slice, chnlId, devValid,
                                      devMode, devNumSb, devCpbPktLen,
                                      devGfpEncapLen);
        }
        sysHyPhy20gMemFree(pChnlNum);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_ZERO_DIVIDE\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_ZERO_DIVIDE;
    }
    calEntryReqRate = vcatReqRate / numCalEntries;
    DBG_PRINT("%s, %s, %d, calEntryReqRate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) calEntryReqRate);


    /**************************************************************************/
    /*  DEBUG PRINT INITIAL DATA                                              */
    /**************************************************************************/
    DBG_PRINT("%s, %s, %d, Initial Rate Data: with numCalEntries = %u, numCalChnl = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) numCalEntries, (unsigned int) numCalChnl);
    for(i=0; i < numCalChnl; i++){
        if(*(pChnlRate + i) != 0){
            DBG_PRINT("%s, %s, %d, chnl = %u, chnlRate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) *(pChnlNum + i), (unsigned int) *(pChnlRate + i));
            DBG_PRINT("%s, %s, %d, chnl = %u, frmRate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) *(pChnlNum + i), (unsigned int) *(pFrmRate + i));
        }
    }

    /**************************************************************************/
    /*  END DEBUG PRINT INITIAL DATA                                          */
    /**************************************************************************/

    DBG_PRINT("%s, %s, %d, Got to here: numCalEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) numCalEntries);
    if (numCalChnl > numCalEntries){
        if(addRemove){
            /* reset context associated with channel to be added */
            gfpHyPhy20gGfpSetChnlCtxt(fileHndl, slice, chnlId, devValid,
                                      devMode, devNumSb, devCpbPktLen,
                                      devGfpEncapLen);
        }
        sysHyPhy20gMemFree(pChnlNum);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_CAL_SIZE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_SCHD_CAL_SIZE;
    }

    /**************************************************************************/
    /*  DETERMINE NUMBER OF SCBS CALENDAR ENTRIES REQUIRED TO SUPPORT EACH    */
    /*  CHANNEL                                                               */
    /**************************************************************************/

    /* determine the number of calendar entries starting at maximum possible */
/*
    numCalRows = maxCalRows;
    findNumCalRows = 1;
*/
    totalMinEntries = 0;

    /* check for failing conditions */
/*
    if (numCalRows == 0){
        sysHyPhy20gMemFree(pChnlNum);
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_ALGO_FAIL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_ALGO_FAIL;
    }
*/

    entryRate = calEntryReqRate;

    /* determine the minimum entries required to meet frame request rate for
       each channel */
    for(j=0; j < numCalChnl; j++) {
        frmRate = *(pFrmRate + j);
        if (entryRate == 0) {
            if (addRemove) {
                /* reset context associated with channel to be added */
                gfpHyPhy20gGfpSetChnlCtxt(fileHndl, slice, chnlId, devValid,
                                          devMode, devNumSb, devCpbPktLen,
                                          devGfpEncapLen);
            }
            sysHyPhy20gMemFree(pChnlNum);
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_ZERO_DIVIDE\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_ZERO_DIVIDE;
        }
        minEntries = (frmRate/entryRate) + (frmRate%entryRate ? 1:0);

        /* store to array */
        *(pMinEntries + j) = minEntries;
        /* store accumulate entries required by calendar */
        totalMinEntries += minEntries;
    }

    /* determine if totalMinEntries <= numCalEntries */
    if(totalMinEntries > numCalEntries){
        if (addRemove) {
            /* reset context associated with channel to be added */
            gfpHyPhy20gGfpSetChnlCtxt(fileHndl, slice, chnlId, devValid,
                                      devMode, devNumSb, devCpbPktLen,
                                      devGfpEncapLen);
            sysHyPhy20gMemFree(pChnlNum);
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_EVALUATING_SCHD\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_EVALUATING_SCHD;
        } else {
            *pOptimal = 2;
        }
    }

    /******************************************************************************/
    /*  DEBUG PRINT INITIAL DATA                                                  */
    /******************************************************************************/
    DBG_PRINT("%s, %s, %d, Initial Rate Data: with numCalEntries = %u numCalChnl = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)numCalEntries, (unsigned int)numCalChnl);
    for(i=0; i < numCalChnl; i++){
        if(*(pChnlRate + i) != 0){
            DBG_PRINT("%s, %s, %d, chnl = %u, rate = %u, minEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)*(pChnlRate + i), (unsigned int)*(pMinEntries + i));
        }
    }

    /* order entries from high-to-low based on minEntries  */
    do {
        swap = 0;
        for(i=0; i < numCalChnl-1; i++) {
            j = i + 1;
            if (*(pMinEntries + j) > *(pMinEntries + i)) {
                /* swap */
                tmp = *(pMinEntries + i);
                *(pMinEntries + i) = *(pMinEntries + j);
                *(pMinEntries + j) = tmp;

                tmp = *(pChnlNum + i);
                *(pChnlNum + i) = *(pChnlNum + j);
                *(pChnlNum + j) = tmp;

                swap = 1;
            }
        }
    } while (swap);

    /**************************************************************************/
    /*  SCALE CALENDAR ENTRIES (SUBOPTIMAL CASE)                              */
    /*  Note: The below algorithm will generate SCBS data for the case that   */
    /*  totalMinEntries > numCalEntries                                       */
    /*  It will:                                                              */
    /*          scale entries proprtionately to minEntries                    */
    /*          steal entries from scaled channels to ensure each channel     */
    /*          appears in the calendar at least once                         */
    /**************************************************************************/
    if(*pOptimal == 2 && addRemove == 0){
        workingEntries = numCalEntries;
        for(i=0; i < numCalChnl; i++){
            /* scale from highest to lowest rate channel */
            minEntries = *(pMinEntries + i);
            /* scale entries rounding up */

            if(totalMinEntries == 0){
                sysHyPhy20gMemFree(pChnlNum);
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_ALGO_FAIL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_ALGO_FAIL;
            }

            assignEntries = minEntries*numCalEntries/totalMinEntries +
                            (minEntries*numCalEntries%totalMinEntries != 0 ? 1:0 );

            if(assignEntries > workingEntries){
                DBG_PRINT("%s, %s, %d, Require entry steal\n",__FILE__, __FUNCTION__, __LINE__);
                /* steal entries from previously scaled channels to ensure new channel has
                   at least one entry */
                numEntriesReqd = 1;
                numEntriesStole = 0;
                DBG_PRINT("%s, %s, %d, chnl = %u, Needs entries numEntriesStole = %u, numEntriesReqd = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)numEntriesStole, (unsigned int)numEntriesReqd);
                for(j=0; j < i; j++){
                    DBG_PRINT("%s, %s, %d, steal entry for loop\n",__FILE__, __FUNCTION__, __LINE__);
                    if(numEntriesReqd > numEntriesStole){
                        DBG_PRINT("%s, %s, %d,  if(numEntriesReqd < numEntriesStole\n)",__FILE__, __FUNCTION__, __LINE__);
                        if(*(pAssignEntries + j) > 1){
                            numEntriesAvail = *(pAssignEntries + j) - 1;
                            if(numEntriesAvail){
                                if(numEntriesReqd > numEntriesAvail + numEntriesStole){
                                    /* steal all entries */
                                    DBG_PRINT("%s, %s, %d, Steal All Entries\n",__FILE__, __FUNCTION__, __LINE__);
                                    *(pAssignEntries + j) = *(pMinEntries + j);
                                    numEntriesStole += numEntriesAvail;
                                } else {
                                    /* steal balance of entries needed */
                                    DBG_PRINT("%s, %s, %d, steel entries needed\n",__FILE__, __FUNCTION__, __LINE__);
                                    *(pAssignEntries+j) -= (numEntriesReqd-numEntriesStole);
                                    numEntriesStole += (numEntriesReqd-numEntriesStole);
                                }
                            }
                        }
                    }
                }
                if(numEntriesStole < numEntriesReqd){
                    sysHyPhy20gMemFree(pChnlNum);
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_BW\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_BW;
                }
                workingEntries += numEntriesStole;
            }
            DBG_PRINT("%s, %s, %d, chnl = %u, numEntriesStole = %u, workingEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)numEntriesStole, (unsigned int)workingEntries);
            workingEntries -= assignEntries;
            *(pAssignEntries + i) = assignEntries;
            DBG_PRINT("%s, %s, %d, chnl = %u, *(pAssignEntries + i) = %u, workingEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)*(pAssignEntries + i), (unsigned int)workingEntries);
        }
    }

    /**************************************************************************/
    /*  SCALE CALENDAR ENTRIES (OPTIMAL CASE)                                 */
    /*  Note: The below algorithm will terminate successfully provided        */
    /*  totalMinEntries <= numCalEntries                                      */
    /**************************************************************************/
    /* scale required calendar entries relative to the sum of minimum required
       entries - increase the number of entries for channels to ensure that
       scheduler does not have empty entries - ensure that minEntries per
       channel is maintained */
    if(*pOptimal != 2){
        workingEntries = numCalEntries;
        for(i=0; i < numCalChnl; i++){
            minEntries = *(pMinEntries + i);
            /* scale entries rounding up */

            if(totalMinEntries == 0){
                sysHyPhy20gMemFree(pChnlNum);
                DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_ALGO_FAIL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_ALGO_FAIL;
            }
            assignEntries = minEntries*numCalEntries/totalMinEntries +
                            (minEntries*numCalEntries%totalMinEntries != 0 ? 1:0 );
            DBG_PRINT("%s, %s, %d, i = %u calculation of assignEntries = %u \n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)i, (unsigned int)assignEntries);
            /* for the last channel ensure that round up of assignEntries does not
               prevent completion of subsequent code */
            if(numCalChnl - 1 == i){
                DBG_PRINT("%s, %s, %d, Last Channel Assign Balance of Entries to avoid rounding error\n",__FILE__, __FUNCTION__, __LINE__);
                assignEntries = workingEntries;
            }

            if(assignEntries < minEntries){
                DBG_PRINT("%s, %s, %d, Increase Entries to meet minimum required\n",__FILE__, __FUNCTION__, __LINE__);
                assignEntries = minEntries;
            }
            if(assignEntries > workingEntries){
                DBG_PRINT("%s, %s, %d, Require entry steal\n",__FILE__, __FUNCTION__, __LINE__);
                /* steal entries from previously scaled channels */
                numEntriesReqd = assignEntries - workingEntries;
                numEntriesStole = 0;
                DBG_PRINT("%s, %s, %d, chnl = %u, Needs entries numEntriesStole = %u, numEntriesReqd = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)numEntriesStole, (unsigned int)numEntriesReqd);
                for(j=0; j < i; j++){
                    DBG_PRINT("%s, %s, %d, steal entry for loop\n",__FILE__, __FUNCTION__, __LINE__);
                    if(numEntriesReqd > numEntriesStole){
                        DBG_PRINT("%s, %s, %d,  if(numEntriesReqd < numEntriesStole\n)",__FILE__, __FUNCTION__, __LINE__);
                        if(*(pAssignEntries + j) > *(pMinEntries + j)){
                            numEntriesAvail = *(pAssignEntries + j) - *(pMinEntries + j);
                            if (numEntriesAvail){
                                if(numEntriesReqd > numEntriesAvail + numEntriesStole){
                                    /* steal all entries */
                                    DBG_PRINT("%s, %s, %d, Steal All Entries\n",__FILE__, __FUNCTION__, __LINE__);
                                    *(pAssignEntries+j) = *(pMinEntries + j);
                                    numEntriesStole += numEntriesAvail;
                                } else {
                                    /* steal balance of entries needed */
                                    DBG_PRINT("%s, %s, %d, steel entries needed\n",__FILE__, __FUNCTION__, __LINE__);
                                    *(pAssignEntries+j) -= (numEntriesReqd-numEntriesStole);
                                    numEntriesStole += (numEntriesReqd-numEntriesStole);
                                }
                            }
                        }
                    }
                }
                if(numEntriesStole < numEntriesReqd){
                    sysHyPhy20gMemFree(pChnlNum);
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_BW\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_BW;
                }
                workingEntries += numEntriesStole;
            }
            DBG_PRINT("%s, %s, %d, chnl = %u, numEntriesStole = %u, workingEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)numEntriesStole, (unsigned int)workingEntries);
            workingEntries -= assignEntries;
            *(pAssignEntries + i) = assignEntries;
            DBG_PRINT("%s, %s, %d, chnl = %u, *(pAssignEntries + i) = %u, workingEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)*(pAssignEntries + i), (unsigned int)workingEntries);

        }
    }

    DBG_PRINT("%s, %s, %d, Scaled Calendar Data\n",__FILE__, __FUNCTION__, __LINE__);
    /**************************************************************************/
    /*  DEBUG PRINT INITIAL DATA                                              */
    /**************************************************************************/
    DBG_PRINT("%s, %s, %d, Initial Rate Data: with numCalEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) numCalEntries);
    for(i=0; i < numCalChnl; i++){
        if(*(pFrmRate + i) != 0){
        DBG_PRINT("%s, %s, %d, chnl = %u, frmRate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int) *(pChnlNum + i), (unsigned int) *(pFrmRate + i));
        }
    }
    /**************************************************************************/
    /*  END DEBUG PRINT INITIAL DATA                                          */
    /**************************************************************************/

    /**************************************************************************/
    /*  EVENLY DISTRIBUTE CALENDAR ENTRIES                                    */
    /**************************************************************************/

    /* initially populate the full calendar with NULL channel */
    for(i=0; i < numCalEntries; i++){
        *(pLinearCal + i) = HYPHY20G_CPB_SCHD_NULL_CHNL;
        *(pEmptyLoc + i) = i;
    }
    numEmptyLoc = newNumEmptyLoc = numCalEntries;

    /* test for entry greater than 50% calendar length and split into two
       entries */
    halfNumCalEntries = numCalEntries/2;
    updatedNumCalChnl = numCalChnl; /* this index requires the extra element
                                       when memory allocated for pChnlNum
                                       and pAssignEntries */

    for (i = 0; i < numCalChnl; i++) {
        if (*(pAssignEntries + i) > halfNumCalEntries) {
            /* split into two entries */
            splitLowRate = *(pAssignEntries + i) - halfNumCalEntries;
            *(pAssignEntries + updatedNumCalChnl) = splitLowRate;
            *(pChnlNum + updatedNumCalChnl) = *(pChnlNum + i);
            updatedNumCalChnl++;

            *(pAssignEntries + i) = halfNumCalEntries;
        }
    }
    DBG_PRINT("%s, %s, %d, numCalChnl = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)numCalChnl);
    numCalChnl = updatedNumCalChnl;
    DBG_PRINT("%s, %s, %d, numCalChnl = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)numCalChnl);

    /* order entries from high-to-low based on channel rate  */
    do {
        swap = 0;
        for(i=0; i < numCalChnl-1; i++) {
            j = i + 1;
            if (*(pAssignEntries + j) > *(pAssignEntries + i)) {
                /* swap */
                tmp = *(pAssignEntries + i);
                *(pAssignEntries + i) = *(pAssignEntries + j);
                *(pAssignEntries + j) = tmp;

                tmp = *(pChnlNum + i);
                *(pChnlNum + i) = *(pChnlNum + j);
                *(pChnlNum + j) = tmp;

                swap = 1;
            }
        }
    } while (swap);

    /******************************************************************************/
    /*  DEBUG PRINT ORDERED SCALED DATA                                           */
    /******************************************************************************/
    DBG_PRINT("%s, %s, %d, Ordered Final Rate Data: with numCalEntries = %u, numCalChnl = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)numCalEntries, (unsigned int)numCalChnl);
    sum = 0;
    for(i=0; i < numCalChnl; i++){
        if(*(pAssignEntries + i) != 0){
        /* this prints excess information */
        DBG_PRINT("%s, %s, %d, chnl = %u, assignedEntries = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)*(pChnlNum + i), (unsigned int)*(pAssignEntries + i));
            sum = sum + *(pAssignEntries + i);
        }
    }
    DBG_PRINT("%s, %s, %d, Final Rate Sum = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)sum);
    /******************************************************************************/
    /*  END DEBUG PRINT ORDERED SCALED DATA                                       */
    /******************************************************************************/

    /* STRICT POPULATION ALGO */
    for (i=0; i < numCalChnl; i++) {
        DBG_PRINT("DEBUG_JC 1: numEmptyLoc = %lu, newNumEmptyLoc = %lu\n", numEmptyLoc, newNumEmptyLoc);

        populateRate = *(pAssignEntries + i);
        populateChnl = *(pChnlNum + i);
        /* identify the required number of calendar entries between each
           occurance of this channel number */
        if(populateRate == 0){
            sysHyPhy20gMemFree(pChnlNum);
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_SCHD_ALGO_FAIL\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_SCHD_ALGO_FAIL;
        }
        calSpace = numEmptyLoc/populateRate;
        calSingles = numEmptyLoc%populateRate;
        if (calSpace == 0) {
            DBG_PRINT("DEBUG_JC 1a: calSpace forced to 1\n");
            calSpace = 1;
        }

        DBG_PRINT("DEBUG_JC 2: LINE %d\n", __LINE__);
        emptyLocOffset = 0;
        for (j=0; j < populateRate; j++) {
            /* fetch location of empty entry, alternating between start and
               end of list */
            if(j==0) {
                emptyLocOffset = 0;
            } else {
                if(calSingles){
                    emptyLocOffset += (calSpace+1);
                    calSingles--;
                } else {
                    emptyLocOffset += calSpace;
                }
            }
            linearCaloffset = *(pEmptyLoc + emptyLocOffset);

            /*pPopulate calendar with channel number */
           *(pLinearCal + linearCaloffset) = populateChnl;

           /* remove empty location from list */
           *(pEmptyLoc + emptyLocOffset) = 0xFFFF;
           newNumEmptyLoc--;
        }

        DBG_PRINT("DEBUG_JC 3: LINE %d\n", __LINE__);
        /* push all empty locations to front of list while maintaining order */
        do {
            swap = 0;
            for(j=0; j < numCalEntries-1; j++) {
                k = j + 1;
                if (*(pEmptyLoc + k) < *(pEmptyLoc + j)) {
                    /* swap */
                    tmp = *(pEmptyLoc + j);
                    *(pEmptyLoc + j) = *(pEmptyLoc + k);
                    *(pEmptyLoc + k) = tmp;

                    swap = 1;
                }
            }
        } while (swap);

        numEmptyLoc = newNumEmptyLoc;
    }

    DBG_PRINT("%s, %s, %d, Format Calendar Entry Data to Rows of SCBS\n",__FILE__, __FUNCTION__, __LINE__);


    /**************************************************************************/
    /*  FORMAT CALENDAR ENTRY DATA TO ROWS OF SCBS                            */
    /**************************************************************************/

    /* apply the linear calendar data to the device scheduler calendar
       format */
    j = 0;
    for(i = 0; i < *pNumRows; i++){
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW)) =
            *(pLinearCal + j);
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW + 1)) =
            *(pLinearCal + (j+1));
        j += 2;
    }

    for(i=0; i < *pNumRows; i++){
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW + 2)) =
            *(pLinearCal + j);
        *(pCalData + (i*HYPHY20G_CPB_SCHD_CAL_ENTRY_PER_ROW + 3)) =
            *(pLinearCal + (j+1));
        j += 2;
    }

    /* free allocated memory used by local function */
    sysHyPhy20gMemFree(pChnlNum);
    return HYPHY20G_SUCCESS;

} /* gfpHyPhy20gGfpGenerateRateAwareSchdData */
/*******************************************************************************
**
**  gfpHyPhy20gGrpCalcVcatGrpRate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    For a given VCAT interface type and number of members
**                  returns the payload rate in Mbps.
**
**  INPUTS:         memType         - VCAT Group member type as per
**                                    eHYPHY20G_VCAT_GRP_TYPE
**                  numMem          - Number of members in VCAT Group
**                 *rate            - Pointer to storage for VCAT Group payload
**                                    rate in kbps
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpHyPhy20gGrpCalcVcatGrpRate(UINT4 memType, UINT4 numMem, UINT4 *rate)
{
    UINT4 sts1Rate, rawRate, stsN;

    /* argument checking */
    if((memType > 9)||
       (memType == HYPHY20G_VCAT_GRP_STS1 && numMem > 192)||
       (memType == HYPHY20G_VCAT_GRP_STS3c && numMem > 64)||
       (memType == HYPHY20G_VCAT_GRP_STS12 && numMem > 16)||
       (memType == HYPHY20G_VCAT_GRP_STS24 && numMem > 8)||
       (memType == HYPHY20G_VCAT_GRP_STS48 && numMem > 4)||
       (memType == HYPHY20G_VCAT_GRP_STS192 && numMem > 1)||
       (memType == HYPHY20G_VCAT_GRP_TS && numMem > 16)||
       (memType == HYPHY20G_VCAT_GRP_ODU1 && numMem > 4)||
       (memType == HYPHY20G_VCAT_GRP_ODU2 && numMem > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sts1Rate = 51840; /* STS-1 rate in Kbps */

    switch (memType){
        case 0: /* HYPHY20G_VCAT_GRP_UNUSED */
            *rate = 0;
            break;
        case 1: /* HYPHY20G_VCAT_GRP_STS1 */
            /* For STS-1 84/90 columns used for payload. */
            rawRate = sts1Rate;
            rawRate *= 84;
            rawRate = rawRate/90 + ((rawRate%90 > 44)?1:0); /* rounded value */
            rawRate *= numMem;
            *rate = rawRate;
            break;
        case 2: /* HYPHY20G_VCAT_GRP_STS3c */
            /* For STS-Nc ratio of payload envelope to total frame
               N*87 - (1 + (N/3 - 1)) / (N*90) */
            stsN = 3;
            rawRate = sts1Rate * stsN;
            rawRate *= stsN*87 - 1 - (stsN/3 - 1);
            rawRate = rawRate/(stsN*90) +
                      (rawRate%(stsN*90) > (stsN*45-1)?1:0);
            rawRate *= numMem;
            *rate = rawRate;
            break;
        case 3: /* HYPHY20G_VCAT_GRP_STS12 */
            /* For STS-Nc ratio of payload envelope to total frame
               N*87 - (1 + (N/3 - 1)) / (N*90) */
            stsN = 12;
            rawRate = sts1Rate * stsN;
            rawRate *= stsN*87 - 1 - (stsN/3 - 1);
            rawRate = rawRate/(stsN*90) +
                      (rawRate%(stsN*90) > (stsN*45-1)?1:0);
            rawRate *= numMem;
            *rate = rawRate;
            break;
        case 4: /* HYPHY20G_VCAT_GRP_STS24 */
            /* For STS-Nc ratio of payload envelope to total frame
               N*87 - (1 + (N/3 - 1)) / (N*90) */
            stsN = 24;
            rawRate = sts1Rate * stsN;
            rawRate *= stsN*87 - 1 - (stsN/3 - 1);
            rawRate = rawRate/(stsN*90) +
                      (rawRate%(stsN*90) > (stsN*45-1)?1:0);
            rawRate *= numMem;
            *rate = rawRate;
            break;
        case 5: /* HYPHY20G_VCAT_GRP_STS48 */
            /* For STS-Nc ratio of payload envelope to total frame
               N*87 - (1 + (N/3 - 1)) / (N*90) */
            stsN = 48;
            rawRate = sts1Rate * stsN;
            rawRate *= stsN*87 - 1 - (stsN/3 - 1);
            rawRate = rawRate/(stsN*90) +
                      (rawRate%(stsN*90) > (stsN*45-1)?1:0);
            rawRate *= numMem;
            *rate = rawRate;
            break;
        case 6: /* HYPHY20G_VCAT_GRP_STS192 */
            /* For STS-Nc ratio of payload envelope to total frame
               N*87 - (1 + (N/3 - 1)) / (N*90) */
            stsN = 192;
            rawRate = sts1Rate * stsN;
            rawRate *= stsN*87 - 1 - (stsN/3 - 1);
            rawRate = rawRate/(stsN*90) +
                      (rawRate%(stsN*90) > (stsN*45-1)?1:0);
            rawRate *= numMem;
            *rate = rawRate;
            break;
        case 7: /* HYPHY20G_VCAT_GRP_TS */
            rawRate = 155520 * numMem;
            *rate = rawRate;
            break;
        case 8: /* HYPHY20G_VCAT_GRP_ODU1 */
            *rate = 2488320 * numMem; /* 2.48832 Gbps */
            break;
        case 9: /* HYPHY20G_VCAT_GRP_ODU2 */
            *rate = 9995300; /* 9.9953 Gbps */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
            break;
    }

    DBG_PRINT("%s, %s, %d, memType = %u, numMem = %u, *rate = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)memType, (unsigned int)numMem, (unsigned int)*rate);

    return HYPHY20G_SUCCESS;

} /* gfpHyPhy20gGrpCalcVcatGrpRate */

/*******************************************************************************
**
**  gfpHyPhy20gGfpEvalChnlCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Evaluates GFP Channel Context to retrieve/calculation the
**                  GFP channel mode, GFP frame rate and the GFP line rate.
**
**                  Used for evaluating channel addition to TXGFPT SCBS.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice         - GFP Slice
**                                  0 - GFP A
**                                  1 - GFP B
**                  chnl          - GFP Channel
**                                  0 to 63
**                  rate          - chnlRate for CPBID associated with GFP
**                                  channel
**                 *pMode         - Pointer to storage for:
**                                  GFP Channel mode
**                                  HYPHY20G_GFP_UNUSED = 0
**                                  HYPHY20G_GFP_T_AA = 1
**                                  HYPHY20G_GFP_T_FA = 2
**                                  HYPHY20G_GFP_F_PKT = 3
**                                  HYPHY20G_GFP_F_PHASE = 4
**                                  HYPHY20G_GFP_OTHER = 5
**                 *pFrmRate      - Pointer to storage for:
**                                  Calculated GFP Channel Frame Rate
**                                  For GFP-F Packet mode this will return
**                                  input rate value.
**                 *pLineRate     - Pointer to storage for:
**                                  Calculated GFP Channel Line Rate
**                                  For GFP-F Packet mode this will return
**                                  input rate value.
**                 *pLineRateUnits - Pointer to storage for:
**                                   The units of the *pLineRate parameter
**                                   0 - bps
**                                   1 - 10x bps
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_ZERO_DIVIDE
**                  HYPHY20G_ERR_VAR_OVERFLOW
**
*******************************************************************************/
INT4 gfpHyPhy20gGfpEvalChnlCtxt(struct file *fileHndl, UINT4 slice, UINT4 chnl,
                                UINT4 rate, UINT4 *pMode, UINT4 *pFrmRate,
                                UINT4 *pLineRate, UINT4 *pLineRateUnits)
{
    UINT4 valid, mode, numSb, cpbPktLen, gfpEncapLen;
    UINT4 num8b10bChar, bitsPerFrame, bitsPerGfpFrame;
    INT4 result;

    DBG_PRINT("Executing %s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((slice > 1) || (chnl > 63) || (rate > 12000)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve the GFP channel context */
    result = gfpHyPhy20gGfpGetChnlCtxt(fileHndl, slice, chnl, &valid,
                                       &mode, &numSb, &cpbPktLen,
                                       &gfpEncapLen);
    if (result) return result;

    *pMode = mode;

    if (mode == HYPHY20G_GFP_F_PKT) {
        DBG_PRINT("Executing %s, %s, %d if (mode == HYPHY20G_GFP_F_PKT)\n",__FILE__, __FUNCTION__, __LINE__);
        *pFrmRate = rate*1000;
        *pLineRate = rate*1000;
        return HYPHY20G_SUCCESS;
    }

    /**************************************************************************/
    /*  CALCULATE THE FRAME RATE FOR A CHANNEL                                */
    /**************************************************************************/
    switch (mode) {
        case 0: /* GFP channel unconfigured */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 1: /* GFP-T AA mode */
            /* frame rate in kbits/sec */
            num8b10bChar = 64 * numSb;
            bitsPerFrame = num8b10bChar * 10;
            bitsPerGfpFrame = (numSb * 67 + gfpEncapLen) * 8;
            break;
        case 2: /* GFP-T FA mode */
            /* frame rate in kbits/sec */
            num8b10bChar = cpbPktLen;
            bitsPerFrame = num8b10bChar * 10;
            bitsPerGfpFrame = (numSb * 67 + gfpEncapLen) * 8;
            break;
        case 3: /* GFP-F Packet mode */
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
            break;
        case 4: /* GFP-F Phase mode */
            bitsPerFrame = cpbPktLen * 8;
            bitsPerGfpFrame = (cpbPktLen + gfpEncapLen) * 8;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
            break;
    }
    /* return error if bitsPerFrame == 0 */
    if (bitsPerFrame == 0) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_ZERO_DIVIDE\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_ZERO_DIVIDE;
    }
    /* frame rate in bits/sec */
    if(rate < 4294) {
        *pFrmRate = rate*1000000 / bitsPerFrame + ((rate*1000000)%bitsPerFrame ? 1:0);
    } else {
        /* "rate * 10000000" exceeds 32 bit container round value to nearest 10s */
        *pFrmRate = rate*100000 / bitsPerFrame + ((rate*100000)%bitsPerFrame ? 1:0);
        if(*pFrmRate > (*pFrmRate) * 10){
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_VAR_OVERFLOW\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_VAR_OVERFLOW;
        }
        *pFrmRate = (*pFrmRate) * 10;
    }
    /* exceed 32 bit number if 0xFFFFFFFF / bitsPerGfpFrame < *pFrmRate */
    if(0xFFFFFFFF/bitsPerGfpFrame < *pFrmRate){
        *pFrmRate = (*pFrmRate)/10 + (((*pFrmRate)%10) ? 1:0);
        if(0xFFFFFFFF/bitsPerGfpFrame < *pFrmRate){
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_VAR_OVERFLOW\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_VAR_OVERFLOW;
        }
        *pLineRate = (*pFrmRate) * bitsPerGfpFrame;
        *pLineRateUnits = 1;
        return HYPHY20G_SUCCESS;
    }
    /* line rate in bits/sec */
    *pLineRate = (*pFrmRate) * bitsPerGfpFrame;
    *pLineRateUnits = 0;

    return HYPHY20G_SUCCESS;

} /* gfpHyPhy20gGfpEvalChnlCtxt */




/*******************************************************************************
**
**  gfpHyPhy20gGfpGetVcatGrpRate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Retrieve VCAT Group Payload rate in kbps.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - VCAT slice to access:
**                                    0 - VCAT A
**                                    1 - VCAT B
**                  grp             - VCAT Group ID to access:
**                                    0 to 63
**                 *pRate           - Pointer to storage for VCAT group rate
**                                    (kbps)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpHyPhy20gGfpGetVcatGrpRate(struct file *fileHndl, UINT4 slice, UINT4 grp,
                                  UINT4 *pRate)
{
    UINT4 memType, numMem, rate;
    INT4 result;

    /* argument checking */
    if ((slice > 1)||(grp > 63)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Get VCAT Group Payload Rate associated with channel */
    result = vcatHyPhy20gVcatGetGrpRate(fileHndl, slice, grp, &memType,
                                        &numMem);
    if (result) return result;

    DBG_PRINT("%s, %s, %d, slice = %u, grp = %u, memType = %u, numMem = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)slice, (unsigned int)grp, (unsigned int)memType, (unsigned int)numMem);

    /* Retrieve VCAT Group Payload Rate in kbps */
    result = gfpHyPhy20gGrpCalcVcatGrpRate(memType, numMem, &rate);
    if (result) return result;

    *pRate = rate;

    return HYPHY20G_SUCCESS;

} /* gfpHyPhy20gGfpGetVcatGrpRate */

/*******************************************************************************
**
**  gfpHyPhy20gGfpGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the size of GfpA and GfpB context.
**
**
**  INPUTS:         fileHndl      - device handle
**
**  OUTPUTS:        None 
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The size of GfpA and GfpB context
**
*******************************************************************************/
UINT4 gfpHyPhy20gGfpGetCtxtSize(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt;
    UINT4 slice = 0, size = 0, i = 0;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    /* calculate each slice context size */
    for (slice = 0; slice < HYPHY20G_GFP_NUM_SLICE; slice++) {
        DBG_PRINT("%s, %s, %d, slice %ld 1st offset 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, size);

        /* include ctxt header size */
        size += sizeof(sHYPHY20G_CTXT_HDR);
		
        pGfpCtxt = (slice == 0) ? (pDevCtxt->pGfpACtxt) : (pDevCtxt->pGfpBCtxt);
        if (pGfpCtxt != NULL) {
        	/* calculate the first level structure size of GFP subsystem */
            size += sizeof(sHYPHY20G_GFP_SLICE_CTXT);

            DBG_PRINT("%s, %s, %d, slice %ld 2nd scbs offset 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, slice, size);
            
            /* calculate the second level size of SCBS */
            /* calculate pGfpCtxt->schd.pScbsGrpCtxt, 
            pGfpCtxt->schd.pOnLineCalMem, 
            and pGfpCtxt->schd.pOffLineCalMem context size */
            size += utilHyPhy20gScbsGetSchdCtxtSize((sHYPHY20G_SCHD_CTXT *)
                                                    &(pGfpCtxt->schd));

            DBG_PRINT("%s, %s, %d, slice %ld 2nd GfpChnlCtxt offset 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, slice, size);
            
            /* calculate the second level size of channel context */
            for (i = 0; i < pGfpCtxt->maxGfpChnl; i++) {
                /* include ctxt header size */
                size += utilHyPhy20gGetCtxtHdrSize();
                size += sizeof(sHYPHY20G_GFP_CHNL_CTXT);
            }
        }
        DBG_PRINT("%s, %s, %d, slice %ld offset end 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, size);
    }
    return size;
}/* gfpHyPhy20gGfpGetCtxtSize */

/*******************************************************************************
**
**  gfpHyPhy20gGfpExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the GfpA and GfpB context into pre-allocated buffer.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-allocated buffer for GfpA and GfpB 
**                  context extraction
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
INT4 gfpHyPhy20gGfpExtractCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt;
    INT4 result;
    UINT4 rtLen;
    UINT4 offset = 0, len = 0, i = 0, slice = 0;
    
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

    /* extract each slice context */
    for (slice = 0; slice < HYPHY20G_GFP_NUM_SLICE; slice++) {
        DBG_PRINT("%s, %s, %d, slice %ld 1st offset 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, offset);

        pGfpCtxt = (slice == 0) ? (pDevCtxt->pGfpACtxt) : (pDevCtxt->pGfpBCtxt);
        
		/* insert ctxt buffer header */
        len = (pGfpCtxt == NULL) ? sizeof(sHYPHY20G_CTXT_HDR) : 
				(sizeof(sHYPHY20G_CTXT_HDR) + sizeof(sHYPHY20G_GFP_SLICE_CTXT));
        rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                            HYPHY_GFP_CTXT_ID, slice);
        HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
            "%s", "pCtxtBuf is NULL.");  
        offset += sizeof(sHYPHY20G_CTXT_HDR);

        if (pGfpCtxt != NULL) {
	        /* extract the first level context of Gfp# subsystem */
	        /* copy the payload into the buffer */
	        sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
	                        (void *)pGfpCtxt, sizeof(sHYPHY20G_GFP_SLICE_CTXT));
	        offset += sizeof(sHYPHY20G_GFP_SLICE_CTXT);


	        DBG_PRINT("%s, %s, %d, slice %ld 2nd scbs offset 0x%lx\n", 
	                    __FILE__, __FUNCTION__, __LINE__, slice, offset);

	        /* extract the second level context of SCBS */
	        /* extract pGfpCtxt->schd.pScbsGrpCtxt, 
	        pGfpCtxt->schd.pOnLineCalMem, 
	        and pGfpCtxt->schd.pOffLineCalMem context */
	        result = utilHyPhy20gScbsExtractSchdCtxt((sHYPHY20G_SCHD_CTXT *)
	                        &(pGfpCtxt->schd), ((UINT1 *)pCtxtBuf + offset), &len);
	        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
	                        "%s", "utilHyPhy20gScbsExtractSchdCtxt failed.");
	        offset += len;


	        DBG_PRINT("%s, %s, %d, slice %ld 2nd GfpChnlCtxt offset 0x%lx\n", 
	                    __FILE__, __FUNCTION__, __LINE__, slice, offset);

	        /* extract the second level context of GfpChnlCtxt */
	        for (i = 0; i < pGfpCtxt->maxGfpChnl; i++ ) {
	            /* insert ctxt buffer header */
	            len = sizeof(sHYPHY20G_CTXT_HDR) +  
	                sizeof(sHYPHY20G_GFP_CHNL_CTXT);
	            rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
	                                        HYPHY_GFP_CHNL_CTXT_ID, i);
	            HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
	                "%s", "pCtxtBuf is NULL.");  
	            offset += sizeof(sHYPHY20G_CTXT_HDR);

	            /* copy the payload into the buffer */
	            sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
	                (void *)(pGfpCtxt->pGfpChnlCtxt[i]), sizeof(sHYPHY20G_GFP_CHNL_CTXT));
	            offset += sizeof(sHYPHY20G_GFP_CHNL_CTXT);
	        }
		}
        DBG_PRINT("%s, %s, %d, slice %ld offset end 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, offset);
    }

    *pExtractedSize = offset;
    
    return HYPHY20G_SUCCESS;
}/* gfpHyPhy20gGfpExtractCtxt */

/*******************************************************************************
**
**  gfpHyPhy20gGfpRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the GfpA and GfpB context into the driver
**                  context memory.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-stored GfpA and GfpB context for restoration
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
INT4 gfpHyPhy20gGfpRestoreCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_GFP_SLICE_CTXT  *pGfpCtxt;
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    INT4 result;
    UINT4 offset = 0, len = 0, i = 0, slice = 0;
    
    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pParsedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pParsedSize is NULL.");

    /* try to retrieve the GfpA ctxt header in the pre-stored context buffer */
    pCtxtBufHdr = utilHyPhy20gSearchCtxtHdr(pCtxtBuf, 
                                HYPHY_GFP_CTXT_ID, 0);
    if (pCtxtBufHdr == NULL) {
        DBG_PRINT("%s, %s, %d, can not find HYPHY_GFP_CTXT_ID hdr\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    pCtxtBuf = (void *)pCtxtBufHdr;
    
    /* restore each slice context */
    for (slice = 0; slice < HYPHY20G_GFP_NUM_SLICE; slice++) {
        DBG_PRINT("%s, %s, %d, slice %ld 1st offset 0x%lx 0x%p\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, offset, pCtxtBuf);
        
        /* check if the payload is valid */
        if (pCtxtBufHdr->len == sizeof(sHYPHY20G_CTXT_HDR)) {
            offset += sizeof(sHYPHY20G_CTXT_HDR);
        } else if (pCtxtBufHdr->len > sizeof(sHYPHY20G_CTXT_HDR)) {
            /* create Gfp# context */
            result = gfpHyPhy20gGfpCreateSliceCtxt(fileHndl, slice, 1, 1);
            if (result) {
                DBG_PRINT("%s, %s, %d, GfpCreateSliceCtxt failed for slice %ld\n", 
                            __FILE__, __FUNCTION__, __LINE__, slice);
                *pParsedSize = 0;
                return result;
            }

            /* retrieve device context */
            pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
            /* confirm device context pointer is valid before processing */
            HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                            "%s", "pDevCtxt is NULL.");

            pGfpCtxt = (slice == 0)?(pDevCtxt->pGfpACtxt):(pDevCtxt->pGfpBCtxt);

            /* restore the first level context for GFP# subsystem */
            /* offset pCtxtBuf to figure out the payload position */
            offset += sizeof(sHYPHY20G_CTXT_HDR);
            
            /* not copy the unuseful pointer value from the pre-stored data */
            len = sizeof(sHYPHY20G_GFP_SLICE_CTXT) - 
                    sizeof(pGfpCtxt->pGfpChnlCtxt);
            sysHyPhy20gMemCpy((void *)(pGfpCtxt), 
                            ((UINT1 *)pCtxtBuf + offset), len);

            /* finish restore the first level context for GFPx subsystem */
            len = sizeof(sHYPHY20G_GFP_SLICE_CTXT);
            offset += len;


            DBG_PRINT("%s, %s, %d, slice %ld 2nd SCBS offset 0x%lx 0x%p\n", 
                        __FILE__, __FUNCTION__, __LINE__, slice, offset, ((UINT1 *)pCtxtBuf + offset));

            /* restore the second level context of SCBS */
            /* restore pGfpCtxt->schd.pScbsGrpCtxt, 
            pGfpCtxt->schd.pOnLineCalMem, 
            and pGfpCtxt->schd.pOffLineCalMem context */
            result = utilHyPhy20gScbsRestoreSchdCtxt(&(pGfpCtxt->schd), 
                                            ((UINT1 *)pCtxtBuf + offset), &len);
            if (result) {
                DBG_PRINT("%s, %s, %d, utilHyPhy20gScbsRestoreSchdCtxt failed\n", 
                            __FILE__, __FUNCTION__, __LINE__);
				/* just abort recovery and flag error to the master hyPhy20gRestoreCtxt() function,
				and call hyPhy20gDestroyCtxt to do clean up work */
                *pParsedSize = 0;
                return result;
            }
            offset += len;


            DBG_PRINT("%s, %s, %d, slice %ld 2nd GfpChnlCtxt offset 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, slice, offset);

            /* restore the second level context of GfpChnlCtxt */
            for (i = 0; i < pGfpCtxt->maxGfpChnl; i++ ) {
                /* try to retrieve the pGfpChnlCtxt ctxt header in the pre-stored context buffer */
                pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
                if (pCtxtBufHdr->id != HYPHY_GFP_CHNL_CTXT_ID) {
					/* just abort recovery and flag error to the master hyPhy20gRestoreCtxt() function,
					and call hyPhy20gDestroyCtxt to do clean up work */
                    *pParsedSize = 0;
                    return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
                }
                HYPHY20G_ASSERT((pCtxtBufHdr->len >= sizeof(sHYPHY20G_CTXT_HDR)), 
                                HYPHY20G_ERR_DEV_CTXT_CORRUPTED,
                                "%s", "HYPHY20G_ERR_DEV_CTXT_CORRUPTED.");

                /* offset pCtxtBuf to figure out the payload position */
                offset += sizeof(sHYPHY20G_CTXT_HDR);

                sysHyPhy20gMemCpy((void *)(pGfpCtxt->pGfpChnlCtxt[i]), 
                    ((UINT1 *)pCtxtBuf + offset), sizeof(sHYPHY20G_GFP_CHNL_CTXT));

                /* offset to next ctxt buffer header */
                offset += sizeof(sHYPHY20G_GFP_CHNL_CTXT);
            }
        } else {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_CORRUPTED\n", 
                        __FILE__, __FUNCTION__, __LINE__);
			/* just abort recovery and flag error to the master hyPhy20gRestoreCtxt() function,
			and call hyPhy20gDestroyCtxt to do clean up work */
            *pParsedSize = 0;
            return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
        }
        DBG_PRINT("%s, %s, %d, slice %ld offset end 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, offset);

        /* move to next ctxt buffer header */
        pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
    }
    
    *pParsedSize = offset;

    return HYPHY20G_SUCCESS;
}/* gfpHyPhy20gGfpRestoreCtxt */

/*******************************************************************************
**
**  gfpHyPhy20gGfpAuditSchdCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit SCHD data in GFP subCtxt. 
**
**                  PRIVATE function called by gfpHyPhy20gGfpAuditCtxt.
**
**  INPUTS:         fileHndl - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_FAILURE  
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
PRIVATE INT4 gfpHyPhy20gGfpAuditSchdCtxt(struct file *fileHndl, UINT4 slice)
{  
    UINT4 schId;
    INT4 result;
    sHYPHY20G_CTXT *pDevCtxt = NULL;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt = NULL;
    sHYPHY20G_SCHD_CTXT *pSchdCtxt = NULL;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT(((pDevCtxt != NULL)&&(slice<2)), HYPHY20G_FAILURE,
                    "%s", "((pDevCtxt==NULL)||(slice>1))");    

    if (slice == 0)
        pGfpCtxt = pDevCtxt->pGfpACtxt;
    else
        pGfpCtxt = pDevCtxt->pGfpBCtxt;

    if (pGfpCtxt==NULL)  
    {
        DBG_PRINT("%s, %s, %d, %uth Gfp slice pGfpCtxt==NULL.\n",
            __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice);
        return HYPHY20G_FAILURE;
    }
    
    pSchdCtxt= &(pGfpCtxt->schd);

    if (slice == 0) {
        schId = HYPHY20G_GFP_TX_SCHD_0;
    } else {
        schId = HYPHY20G_GFP_TX_SCHD_1;
    }

    if (pSchdCtxt->schdId != schId)
    {
        DBG_PRINT("%s, %s, %d, %uth GFP Slice pSchdCtxt->schdId != schId\n",
            __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice);
        return HYPHY20G_FAILURE;
    }

    result=utilHyPhy20gScbsAuditSchdCtxt(fileHndl,pSchdCtxt);
    if (result != HYPHY20G_SUCCESS) 
    {
        DBG_PRINT("%s, %s, %d, %uth GFP Slice utilHyPhy20gScbsAuditSchdCtxt(fileHndl,pSchdCtxt) fail.\n",
            __FILE__, __FUNCTION__, __LINE__,(unsigned int)slice);
        return HYPHY20G_FAILURE;
    }
    DBG_PRINT("%s, %s, %d, utilHyPhy20gScbsAuditSchdCtxt(fileHndl,pSchdCtxt) SUCCESS in gfpHyPhy20gGfpAuditSchdCtxt(fileHndl,%u).\n",
        __FILE__, __FUNCTION__, __LINE__,(unsigned int)slice);
    
	return HYPHY20G_SUCCESS;
} /* gfpHyPhy20gGfpAuditSchdCtxt */

/*******************************************************************************
**
**  gfpHyPhy20gAuditGfpRowCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit pGfpRowCtxt data in GFP subCtxt.
**
**                  PRIVATE function called by gfpHyPhy20gGfpAuditCtxt.
**
**  INPUTS:         fileHndl - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_FAILURE  
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
PRIVATE INT4 gfpHyPhy20gGfpAuditGfpRowCtxt(struct file *fileHndl, UINT4 slice)
{
    INT4 i;
    sHYPHY20G_CTXT *pDevCtxt = NULL;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt = NULL;
    sHYPHY20G_GFP_SCBS_ROW_CTXT *pGfpRowCtxt = NULL;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT(((pDevCtxt != NULL)&&(slice<2)), HYPHY20G_FAILURE,
                    "%s", "((pDevCtxt==NULL)||(slice>1))");    

    if (slice == 0)
        pGfpCtxt = pDevCtxt->pGfpACtxt;
    else
        pGfpCtxt = pDevCtxt->pGfpBCtxt;

    if (pGfpCtxt==NULL)  
    {
        DBG_PRINT("%s, %s, %d, %uth Gfp slice pGfpCtxt==NULL.\n",
            __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice);
        return HYPHY20G_FAILURE;
    }
    
    for (i = 0; i<=HYPHY20G_GFP_TX_SCHD_MAX_NUM_ROW; i++)
    {
        pGfpRowCtxt= &(pGfpCtxt->pGfpRowCtxt[i]);

        if ((pGfpRowCtxt->grpId!=HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER)&&(pGfpRowCtxt->grpId>=HYPHY20G_GFP_SCHD_MAX_GRP))
        {
            DBG_PRINT("%s, %s, %d, %uth Gfp slice %uth pGfpRowCtxt->grpId(%u) value wrong.\n",
                __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice,(unsigned int)i,
                (unsigned int)(pGfpRowCtxt->grpId));
            return HYPHY20G_FAILURE;
        }
    }

    return HYPHY20G_SUCCESS;

} /* gfpHyPhy20gAuditGfpRowCtxt */

/*******************************************************************************
**
**  gfpHyPhy20gGfpAuditGfpChnlCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit pGfpChnlCtxt data in GFP subCtxt.
**
**                  PRIVATE function called by gfpHyPhy20gGfpAuditCtxt.
**
**  INPUTS:         fileHndl - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_FAILURE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
PRIVATE INT4 gfpHyPhy20gGfpAuditGfpChnlCtxt(struct file *fileHndl, UINT4 slice)
{     
    UINT4 i;
    
    sHYPHY20G_CTXT *pDevCtxt = NULL;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt = NULL;
    sHYPHY20G_GFP_CHNL_CTXT *pGfpChnlCtxt = NULL;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT(((pDevCtxt != NULL)&&(slice<2)), HYPHY20G_FAILURE,
                    "%s", "((pDevCtxt==NULL)||(slice>1))");    
        
    if (slice == 0)
        pGfpCtxt = pDevCtxt->pGfpACtxt;
    else
        pGfpCtxt = pDevCtxt->pGfpBCtxt;

    if (pGfpCtxt==NULL)  
    {
        DBG_PRINT("%s, %s, %d, %uth Gfp slice pGfpCtxt==NULL.\n",
            __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice);
        return HYPHY20G_FAILURE;
    }

    for (i=0; i<HYPHY20G_GFP_SCHD_MAX_GRP; i++)
    {
        pGfpChnlCtxt = pGfpCtxt->pGfpChnlCtxt[i];

        if (pGfpChnlCtxt==NULL)  
        {
            DBG_PRINT("%s, %s, %d, %uth Gfp slice pGfpCtxt->pGfpChnlCtxt[%u]==NULL in gfpHyPhy20gGfpAuditGfpChnlCtxt.\n",
                        __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice ,(unsigned int)i);
            return HYPHY20G_FAILURE;
        }
        if ((pGfpChnlCtxt->valid != HYPHY20G_VALID_VAL_32BIT)
            ||(pGfpChnlCtxt->mode > HYPHY20G_GFP_OTHER)
            ||(pGfpChnlCtxt->numSb > 127)
            ||(pGfpChnlCtxt->cpbPktLen > HYPHY20G_GFP_MAX_PACKET_LEN)
            ||(pGfpChnlCtxt->gfpEncapLen > HYPHY20G_GFP_MAX_ENCAP_HEAD_LEN)
           )
        {
            DBG_PRINT("%s, %s, %d, %uth Gfp slice pGfpChnlCtxt[%u] members value wrong.\n",
    			__FILE__, __FUNCTION__, __LINE__, (unsigned int)slice, (unsigned int)i );
            return HYPHY20G_FAILURE;
        }
        DBG_PRINT("%s, %s, %d, %uth Gfp slice pGfpCtxt->pGfpChnlCtxt[%u] check SUCCESS.\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice, (unsigned int)i );
    }
    
	return HYPHY20G_SUCCESS;

} /* cpbHyPhy20gCpbAuditScbsCtxt */


/*******************************************************************************
**
**  gfpHyPhy20gGfpAuditCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit GfpA and GfpB subCtxt 
**
**                  PRIVATE function called by the hyPhy20gAuditCtxt
**
**  INPUTS:         fileHndl - base address
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        HYPHY20G_FAILURE  
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 gfpHyPhy20gGfpAuditCtxt(struct file *fileHndl)
{
    INT4 result;
    sHYPHY20G_CTXT *pDevCtxt = NULL;
    sHYPHY20G_GFP_SLICE_CTXT *pGfpCtxt=NULL;
    UINT4 gfpFrmLowPwr,gfpTraLowPwr,slice=0, regAddr, offset;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt==NULL");    
    
    for (slice =0; slice<2; slice++)
    {
        if (slice==0)
            pGfpCtxt = pDevCtxt->pGfpACtxt;
        else
            pGfpCtxt = pDevCtxt->pGfpBCtxt;

        regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_3;
        offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_3_GFP0_FRM_LOWPWR_OFF+slice*2;

        result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &gfpFrmLowPwr);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 
        
        result = sysHyPhy20gBitRead(fileHndl, regAddr, offset+1, &gfpTraLowPwr);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 
        
        if (gfpFrmLowPwr!=gfpTraLowPwr)
        {
            DBG_PRINT("%s, %s, %d, %uth GFP slice gfpFrmLowPwr(%u)!=gfpTraLowPwr(%u). \n",
                __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice,
                (unsigned int)gfpFrmLowPwr, (unsigned int)gfpTraLowPwr );
            return HYPHY20G_FAILURE;
        }

        /*the GFP slice disabled */
        if (gfpFrmLowPwr == 1)
        {
            if (pGfpCtxt!=NULL)
            {
                DBG_PRINT("%s, %s, %d, %uth GFP slice (pGfpCtxt!=NULL) when GFP_FRM_LOWPWR=1.\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice);
                return HYPHY20G_FAILURE;
            }
            else
                continue;
        }

        /*the GFP slice enabled */
        if (pGfpCtxt->valid!=HYPHY20G_VALID_VAL_32BIT)
        {
            DBG_PRINT("%s, %s, %d, %uth pGfpCtxt->valid!=HYPHY20G_VALID_VAL_32BIT when it is enabled\n",
                __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice);
            return HYPHY20G_FAILURE;
        }

        result = gfpHyPhy20gGfpAuditSchdCtxt(fileHndl, slice);
        if (result != HYPHY20G_SUCCESS) 
        {
            return result;
        }
        DBG_PRINT("%s, %s, %d, %uth GFP slice gfpHyPhy20gGfpAuditSchdCtxt(fileHndl, slice) SUCCESS in gfpHyPhy20gGfpAuditCtxt\n",
                __FILE__, __FUNCTION__, __LINE__,(unsigned int)slice);
        
        result = gfpHyPhy20gGfpAuditGfpRowCtxt(fileHndl, slice);
        if (result != HYPHY20G_SUCCESS) 
        {
            return result;
        }
        DBG_PRINT("%s, %s, %d, %uth GFP slice gfpHyPhy20gGfpAuditGfpRowCtxt(fileHndl, slice) SUCCESS in gfpHyPhy20gGfpAuditCtxt\n",
            __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice);

        result = gfpHyPhy20gGfpAuditGfpChnlCtxt(fileHndl, slice);
        if (result != HYPHY20G_SUCCESS) 
        {
            return result;
        }
        DBG_PRINT("%s, %s, %d, %uth GFP slice gfpHyPhy20gGfpAuditGfpChnlCtxt(fileHndl, slice) SUCCESS in gfpHyPhy20gGfpAuditCtxt\n",
            __FILE__, __FUNCTION__, __LINE__,(unsigned int)slice);

    }
    return HYPHY20G_SUCCESS;
} /* gfpHyPhy20gGfpAuditCtxt */

/* end of file */
