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
**  FILE        : hyphy20g_opi.c
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
#include "hyphy20g_opi.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 cbrbHyPhy20gCbriUpdateChnlCtxt(struct file *fileHndl, UINT4 slice,
                                    UINT4 chnlNum, UINT4 rate);
INT4 cbrbHyPhy20gCbriUpdateSchd(struct file *fileHndl, UINT4 slice);
PRIVATE INT4 cbriHyPhy20gCbriAuditRdChnlCtxt(struct file *fileHndl, UINT4 slice);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gCbrbPktSizeTypeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the SW driver for the type of
**                  CBRB packets sizes that will be used for device operation.
**
**                  By default standard packet sizes are used.  Standard with
**                  overclocked Predictor Compensation settings are intended
**                  for use in Rev B devices where the device sourcing data
**                  on the CBRI is not pacing the data.
**
**                  Standard and Customer fabric settings assume the system 
**                  side device on the CBRI is pacing the data.
**
**                  This function maintains this state in a global variable.
**                  If the SW driver is modified to support multiple devices,
**                  this API will provide the state used by all devices unless
**                  the implementation is modified.
**
**  INPUTS:         *fileHndl  - base address
**                  type       - 0 - Standard
**                               1 - Standard with overclocked Predictor 
**                                   Compensation
**                               2 - Custom fabric
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbPktSizeTypeCfg(struct file *fileHndl, UINT4 type)
{
    sHYPHY20G_CTXT  *pDevCtxt;

    /* argument checking */
    if(type > 2)
        return HYPHY20G_ERR_INVALID_ARG;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pDevCtxt->gData.hyphy20gCbrbPacketSizeMode = type;

    DBG_PRINT("%s, %s, %d, CBRB Set hyphy20gCbrbPacketSizeMode;  hyphy20gCbrbPacketSizeMode = %u\n",
        __FILE__, __FUNCTION__, __LINE__, 
        (unsigned int)pDevCtxt->gData.hyphy20gCbrbPacketSizeMode);

    return HYPHY20G_SUCCESS;
    
} /* hyPhy20gCbrbPktSizeTypeCfg */

/*******************************************************************************
**
**  hyPhy20gCbrbInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the CBRB subsystem so that it
**                  may be further configured to transfer user data.
**
**                  This function controls the CBRB power savings
**                  OPI_CBRB_LOWPWR bit such that when enable = 0, the block is
**                  placed in low power mode.
**
**                  The block is reset to default values each time the function
**                  is called.
**
**  INPUTS:         *fileHndl  - base address
**                  enable     - 0 - CBRB subsystem disabled
**                               1 - CBRB subsystem enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**
*******************************************************************************/
INT4 hyPhy20gCbrbInit(struct file *fileHndl, UINT4 enable)
{
    /* variable declaration */
    UINT4 regAddr, regData, slice;
    INT4 result;

    /* argument checking */
    if(enable > 1){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* unconditionally place block in lowpwr and reset block registers to
       default */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_2;

    result = sysHyPhy20gRead(fileHndl, regAddr, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                             HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OPI_CBRB_LOWPWR_MSK,
                             1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                           HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OPI_CBRI_LOWPWR_1_MSK,
                           1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                           HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OPI_CBRI_LOWPWR_0_MSK,
                           1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, regData);
    if(result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_2_OPI_RST_OFF,
                                 1);
    if(result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_2_OPI_RST_OFF,
                                 0);
    if(result) return result;

    /* if disabling unconditionally destroy CBRI context */
    if (enable == 0) {
        /* unconditionally destroy scheduler context */
        for (slice=0; slice < 2; slice++){
            hyPhy20gCbriDestroyCtxt(fileHndl, slice);
        }
    }


    /* if enable required take block out of lowpower and reset at block level */
    if(enable == 1){
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                             HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OPI_CBRB_LOWPWR_OFF,
                             0);
        if(result) return result;


        /* toggle block level reset bit */
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_OPI_TOP_REG_SW_RST,
                                   HYPHY20G_OPI_TOP_BIT_SW_RST_CBRB_TSB_RST_OFF,
                                   1);
        if(result) return result;

        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_OPI_TOP_REG_SW_RST,
                                   HYPHY20G_OPI_TOP_BIT_SW_RST_CBRB_TSB_RST_OFF,
                                   0);
        if(result) return result;

    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbInit */


/*******************************************************************************
**
**  hyPhy20gCbrbVcatIntfInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the format of the CBRB ports 10
**                  to 17 associated with the VCAT/ODUKPP interface.  The
**                  ports are initialized in groups of 4.
**
**                  When configured for 4 x ODU1 payload, by default all ports
**                  are configured for access to the ODUKPP subsystem ODU1
**                  slices. hyPhy20gCbrbVcatIntfSrcCfg may be called on a per
**                  link basis to bypass ODUKPP subsystem and connect directly
**                  to VCAT subsystem.
**
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OPI ODUKPP Interface Ports
**                               0 - OPI Ports 10 to 13
**                               1 - OPI Ports 14 to 17
**                  payload    - 0 - 4 x ODU1 (ODUKPP default)
**                               1 - 1 x ODU2
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbVcatIntfInit(struct file *fileHndl, UINT4 slice, UINT4 payload)
{
    /* variable declaration */
    UINT4 rstReg, val, vcatEgrOdu2EnOff;
    UINT4 vcatEgrOdu2EnSliceMsk, ingOdu1EnOff, ingOdu1EnIntfMsk, ingOdu1EnVal;
    INT4 result;

    /* argument checking */
    if((slice > 1)||
       (payload > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    rstReg = HYPHY20G_OPI_TOP_REG_SW_RST;

    result = sysHyPhy20gRead(fileHndl, rstReg, &val);
    if(result) return result;

    vcatEgrOdu2EnOff = HYPHY20G_OPI_TOP_BIT_SW_RST_DM_ODU2_EN_OFF +
                       slice;
    vcatEgrOdu2EnSliceMsk = 1 << vcatEgrOdu2EnOff;

    result = hyPhy20gVarFieldWrite(&val, vcatEgrOdu2EnSliceMsk, payload);
    if(result) return result;

    ingOdu1EnOff = HYPHY20G_OPI_TOP_BIT_SW_RST_DM_UCH_ODU1_EN_OFF;
    ingOdu1EnOff = ingOdu1EnOff + (slice * 4);
    ingOdu1EnIntfMsk = 0xF << ingOdu1EnOff;

    /* if 4 x ODU1 format by default to ODUKPP mode otherwise set to 0 */
    ingOdu1EnVal = (payload==1) ? 0 : 0xF;

    result = hyPhy20gVarFieldWrite(&val, ingOdu1EnIntfMsk, ingOdu1EnVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, rstReg, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbVcatIntfInit */

/*******************************************************************************
**
**  hyPhy20gCbrbVcatIntfSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the data source into the CBRB
**                  for ports 10 to 17 when these interface have been
**                  configured for 4 x ODU1 mode of operation (refer to
**                  hyPhy20gCbrbVcatIntfInit).
**
**
**  INPUTS:         *fileHndl  - base address
**                  chnl       - VCAT_LCAS Subsystem Interface ODU1 channel:
**                               10 - VCAT_LCAS interface 0 ODU1 #1
**                               11 - VCAT_LCAS interface 0 ODU1 #2
**                               12 - VCAT_LCAS interface 0 ODU1 #3
**                               13 - VCAT_LCAS interface 0 ODU1 #4
**                               14 - VCAT_LCAS interface 1 ODU1 #1
**                               15 - VCAT_LCAS interface 1 ODU1 #2
**                               16 - VCAT_LCAS interface 1 ODU1 #3
**                               17 - VCAT_LCAS interface 1 ODU1 #4
**                  source     - 0 - ODU1 from VCAT subsystem
**                               1 - ODU1 from ODUKPP OTU1 Framer
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbVcatIntfSrcCfg(struct file *fileHndl, UINT4 chnl, UINT4 source)
{
    /* variable declaration */
    UINT4 addr, ingOdu1EnOff;
    INT4 result;

    /* argument checking */
    if((chnl > 17)||(chnl < 10)||(source > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_OPI_TOP_REG_SW_RST;
    ingOdu1EnOff = HYPHY20G_OPI_TOP_BIT_SW_RST_DM_UCH_ODU1_EN_OFF;
    ingOdu1EnOff = ingOdu1EnOff + (chnl - 10);

    result = sysHyPhy20gBitWrite(fileHndl, addr, ingOdu1EnOff, source);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbVcatIntfSrcCfg */

/*******************************************************************************
**
**  hyPhy20gCbriIntfInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configure one of the CBRI instances to
**                  transition between 'Major Bidirectional States' of:
**                  1.  Block in reset with all registers in default values
**                  2.  Block in reset with configuration registers maintaining
**                      their last initialized value
**
**                  Setting enable = 1 moves from state 1 to 2.  Setting
**                  enable = 0 allows an interface to move from any state to
**                  state 1.  In state 1, the CBRI block is in low power mode.
**
**                  When entering state 2, the CBRI interface will be
**                  initialized at the OPI top level, Interlaken Shim,
**                  Interlaken Core and SCBS.
**
**                  Prior to enabling the interface using
**                  hyPhy20gCbriIntfEnable, additional initialization must be
**                  performed with:
**                         hyPhy20gCbriTxRateLimitCfg
**                         hyPhy20gCbriFcModeCfg
**                  And optionally:
**                         hyPhy20gCbriTxFcResponseCfg
**                         hyPhy20gCbriFcCalCfg
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - CBRI block:
**                               0 - CBRI 0
**                               1 - CBRI 1
**                  enable     - 0 - disabled
**                               1 - enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gCbriIntfInit(struct file *fileHndl, UINT4 slice, UINT4 enable)
{

    /* variable declaration */
    UINT4 devId, revId, status;
    sHYPHY20G_SCHD_CTXT *pSchd;
    sHYPHY20G_CBRI_CTXT *pCbriCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 lpOff;
    UINT4 rstReg, rstRegData, schedLwRstOff, cbriTsbRstOff;
    UINT4 schdId, length, schdMode;
    UINT4 maxDisMsk, minDisMsk;
    UINT4 val, burstCfgAddr;
    INT4 result;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 1) || (enable > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* RTOS considerations */
    if (enable == 0) {
        /* unconditionally destroy scheduler context */
        result = hyPhy20gCbriDestroyCtxt(fileHndl, slice);
        if(result) return result;
    } else {
        /* create context for use by CBRI instance */
        result = hyPhy20gCbriCreateCtxt(fileHndl, slice);
        if(result) return result;

    }

    /* unconditionally remove low power mode at device level */
    lpOff = (slice == 0) ? HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OPI_CBRI_LOWPWR_0_OFF
                         : HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OPI_CBRI_LOWPWR_1_OFF;

    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                                 lpOff, 0);
    if(result) return result;

    /* unconditionally put block and sub blocks in reset at block level */
    rstReg = HYPHY20G_OPI_TOP_REG_SW_RST;
    schedLwRstOff = HYPHY20G_OPI_TOP_BIT_SW_RST_SCHED_LW_RST_OFF + slice;
    cbriTsbRstOff = HYPHY20G_OPI_TOP_BIT_SW_RST_CBRI_TSB_RST_OFF + slice;

    /* Top level reset bits */
    result = sysHyPhy20gRead(fileHndl, rstReg, &rstRegData);
    if(result) return result;

    /* if top level CBRI reset bit set to 0 put interlaken core into reset */
    if ((rstRegData & (1 << cbriTsbRstOff)) == 0) {
	    /* Interlaken Core Reset bits */
	    result = utilHyPhy20gIntlknCoreEnable(fileHndl, slice,
	                                         HYPHY20G_INTLKN_STATE_INRESET);
	    if(result) return result;
    }

    /* set CBRI and SCBS top level reset bits to 1 */
    rstRegData |= (1 << schedLwRstOff);
    rstRegData |= (1 << cbriTsbRstOff);

    result = sysHyPhy20gWrite(fileHndl, rstReg, rstRegData);
    if(result) return result;

    /* function operation complete if disabling interface set device level
       to low power mode and exit - exit */
    if(enable == 0){
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                                     lpOff, 1);
        if(result) return result;
        return HYPHY20G_SUCCESS;
    }

    /* proceed with enabling and initialization of block and sub blocks */

    /* take blocks out of reset */
    rstRegData &= ~(1 << schedLwRstOff);
    rstRegData &= ~(1 << cbriTsbRstOff);

    result = sysHyPhy20gWrite(fileHndl, rstReg, rstRegData);
    if(result) return result;

    /* Perform Shim Level initialization */
    result = utilHyPhy20gIntlknShimCfg(fileHndl, slice, 1);
    if(result) return result;

    /* Put Interlaken in 'Major Bidirectional State' of 2 */
    result = utilHyPhy20gIntlknCoreEnable(fileHndl, slice,
                                          HYPHY20G_INTLKN_STATE_STANDBY);
    if(result) return result;

    /* apply initial Interlaken Core configuration */
    result = utilHyPhy20gIntlknCoreCfg(fileHndl, slice);
    if(result) return result;

    /* SCBS Initialization */

    /* Access device level context information */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* generate pointer to CBRI context for this slice */
    pCbriCtxt = (sHYPHY20G_CBRI_CTXT *)(pDevCtxt->pCbriCtxt[slice]);
    if(pCbriCtxt == NULL){
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Generate pointer to scheduler context for this slice */
    pSchd = pCbriCtxt->pSchdCtxt;

    schdId = (slice == 0) ? HYPHY20G_OPI_SCHD_0 : HYPHY20G_OPI_SCHD_1;

    length = 2; /* CBRI SCBS requires two rows of calendar entries */
    schdMode = 1; /* BONUS mode disabled */

    result = utilHyPhy20gScbsInitSchd(fileHndl, pSchd,
                                      (eHYPHY20G_SCBS_SCHD_ID)schdId,
                                      length, schdMode);
    if(result) return result;

    /* Non-default setting of TX and RX MIN_DIS/MAX_DIS fields */

    /* Extract BMAX_CNT and BMIN_CNT data */
    maxDisMsk = HYPHY20G_CBRI_CORE_BIT_BRST_CFG_MAX_DIS_MSK;
    minDisMsk = HYPHY20G_CBRI_CORE_BIT_BRST_CFG_MIN_DIS_MSK;

    /* Configure TX direction */
    burstCfgAddr = HYPHY20G_CBRI_CORE_REG_BRST_CFG_OFFSET(0);
    burstCfgAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(burstCfgAddr, slice);
    result = sysHyPhy20gRead(fileHndl, burstCfgAddr, &val);
    if (result) return result;

    /*set Tx MAX_DIS and MIN_DIS to 1 */
    result = hyPhy20gVarFieldWrite(&val, maxDisMsk, 1);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&val, minDisMsk, 1);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, burstCfgAddr, val);
    if (result) return result;


    /* Configure RX direction */
    burstCfgAddr = HYPHY20G_CBRI_CORE_REG_BRST_CFG_OFFSET(1);
    burstCfgAddr = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(burstCfgAddr, slice);
    result = sysHyPhy20gRead(fileHndl, burstCfgAddr, &val);
    if (result) return result;

    /*set Rx MAX_DIS and MIN_DIS to 0 */
    result = hyPhy20gVarFieldWrite(&val, maxDisMsk, 0);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&val, minDisMsk, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, burstCfgAddr, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriIntfInit */


/*******************************************************************************
**
**  hyPhy20gCbriIntfEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configure one of the CBRI instances to
**                  transition between 'Major Bidirectional States' of:
**                  2.  Block in reset with configuration registers maintaining
**                      their last initialized value
**                  3.  Block in operational state with the ability to send/
**                      receive user data or SERDES test patterns.
**
**                  Setting enable = 1 moves from state 2 to 3.  Setting
**                  enable = 0 allows an interface to move from state 3
**                  to state 2.
**
**                  It is assumed that the CBRI instance is in state 2 when
**                  calling this function with enable = 1 and in state 3 when
**                  calling this function with enable = 0.  If these
**                  relationships are not true HYPHY20G_ERR_INVALID_STATE is
**                  returned.
**
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - CBRI block:
**                               0 - CBRI 0
**                               1 - CBRI 1
**                  enable     - 0 - disabled
**                               1 - enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriIntfEnable(struct file *fileHndl, UINT4 slice, UINT4 enable)
{
    /* variable declaration */
    UINT4 state, newState;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((enable > 1)||
       (slice > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine desired state */
    newState = (enable==0) ?
              HYPHY20G_INTLKN_STATE_STANDBY : HYPHY20G_INTLKN_STATE_OPERATIONAL;

    /* retrieve major bidirectional state */
    result = utilHyPhy20gIntlknCoreGetMajBiDirState(fileHndl, slice, &state);
    if(result) return result;

    /* determine if valid state transition */
    if(((newState == HYPHY20G_INTLKN_STATE_STANDBY)&&
       (state == HYPHY20G_INTLKN_STATE_OPERATIONAL))||
       ((newState == HYPHY20G_INTLKN_STATE_OPERATIONAL)&&
       (state == HYPHY20G_INTLKN_STATE_STANDBY))){
        /* Configure Interlaken Core Reset bits */
        result = utilHyPhy20gIntlknCoreEnable(fileHndl, slice, newState);
    } else {
        return HYPHY20G_ERR_INVALID_STATE;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriIntfEnable */

/*******************************************************************************
**
**  hyPhy20gCbriTxRateLimitCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configure the optional transmit rate limiting
**                  feature for one of the CBRI instances.
**
**                  The CBRI block must be 'Major Bidirectional States' of 2
**                  in order for this function to be executed, otherwise
**                  HYPHY20G_ERR_INVALID_STATE will be returned.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - CBRI block:
**                               0 - CBRI 0
**                               1 - CBRI 1
**                  rateLimit  - 0 - disabled no rate limiting applied
**                               1 to 12 - amount of rate limiting applied in
**                                         1 GBPS increments
**                  burstLimit - BurstLimit Parameter setting as described in
**                               Interlaken Protocol Definition.  'The maxiumum
**                               quantity of data that may be burst across the
**                               interface before invoking the rate limiting
**                               logic (bytes)'
**                  byteGran   - Number of credits that are to be subtracted
**                               from the token bucket when an EOP is
**                               encountered.
**                               0 - 1 byte
**                               1 - 2 bytes
**                               2 - 4 bytes
**                               3 - 8 bytes
**                               4 - 16 bytes
**                               5 - 32 bytes
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriTxRateLimitCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 rateLimit, UINT4 burstLimit,
                                UINT4 byteGran)
{
    /* variable declaration */
    UINT4 state;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 1)||
       (rateLimit > 25)||
       (byteGran > 5)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve major bidirectional state */
    result = utilHyPhy20gIntlknCoreGetMajBiDirState(fileHndl, slice, &state);
    if(result) return result;

    /* confirm major bidirection state of 2 or return invalid state error */
    if(state != HYPHY20G_INTLKN_STATE_STANDBY){
        return HYPHY20G_ERR_INVALID_STATE;
    }

    result = utilHyPhy20gIntlknCoreTxRateLimitCfg(fileHndl, slice, rateLimit,
                                                  burstLimit, byteGran);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriTxRateLimitCfg */


/*******************************************************************************
**
**  hyPhy20gCbriTxFcResponseCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configure the way in which a CBRI instance
**                  responds to down stream flow control.  It configures how
**                  the scheduler process a channel after receiving flow control
**                  status of XOFF and if the interface process link level flow
**                  control or not.
**
**                  The function also allows for downstream flow control to be
**                  disregarded alltogether.  To enable this mode set linkMode
**                  and xoffIgnore to 0.
**
**                  There are two modes of operation when a channel receives a
**                  flow control status of XOFF:
**
**                  1.  When XOFF is received for a particular channel, block
**                      the scheduler from selecting that channel until XON is
**                      received for that channel.
**                  2.  When XOFF is received for a particular channel, block
**                      the scheduler from selecting that channel for a
**                      specified number of scheduling opportunities.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - CBRI block:
**                               0 - CBRI 0
**                               1 - CBRI 1
**                  xoffMode   - 0 - mode 1 (see DESCRIPTION)
**                               1 - mode 2 (see DESCRIPTION)
**                  xoffHoldOff- Valid for xoffMode == 1.  Number of scheduling
**                               opportunities that are squelched after XOFF is
**                               received
**                  linkMode   - 0 - link level flow control not processed
**                               1 - link level flow control processed
**                  xoffIgnore - Disable the processing of inband flow control,
**                               process as if all channels always XON
**                               0 - process channel level flow control
**                               1 - ignore channel level flow control
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriTxFcResponseCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 xoffMode, UINT4 xoffHoldOff,
                                 UINT4 linkMode, UINT4 xoffIgnore)
{
    /* variable declaration */
    UINT4 devId, revId, status;
    UINT4 fcCfgReg1, preBufLinkEnOff;
    UINT4 fcCfgReg2, rcvXoffModeSelOff, rcvXoffModeCntMsk, rcvXoffModeCntOff;
    UINT4 fcCfgReg2Data;
    UINT4 shimCfgReg, ignoreFcMsk;
    INT4 result;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 1)||
       (xoffMode > 1)||
       (xoffHoldOff > 255)||
       (linkMode > 1)||
       (xoffIgnore > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PREBUF_LINK_ENABLE setting */
    fcCfgReg1 = HYPHY20G_OPI_TOP_REG_INTLKN_FC_1;
    preBufLinkEnOff = HYPHY20G_OPI_TOP_BIT_INTLKN_FC_1_PREBUF_LINK_ENABLE_OFF +
                      slice;
    result = sysHyPhy20gBitWrite (fileHndl, fcCfgReg1, preBufLinkEnOff,
                                  linkMode);
    if(result) return result;

    /* RCV_XOFF_MODE_SEL and RCV_XOFF_MODE_CNT setting */
    fcCfgReg2 = HYPHY20G_OPI_TOP_REG_INTLKN_FC_2;
    rcvXoffModeSelOff = HYPHY20G_OPI_TOP_BIT_INTLKN_FC_2_RCV_XOFF_MODE_SEL_OFF +
                        slice;
    rcvXoffModeCntOff = HYPHY20G_OPI_TOP_BIT_INTLKN_FC_2_RCV_XOFF_MODE_CNT_OFF;
    rcvXoffModeCntMsk = (slice==0) ? 0xFF : 0xFF00;
    rcvXoffModeCntMsk = rcvXoffModeCntMsk << rcvXoffModeCntOff;

    result = sysHyPhy20gRead(fileHndl, fcCfgReg2, &fcCfgReg2Data);
    if(result) return result;

    if(xoffMode==1){
        /* set RCV_XOFF_MODE to 1 and RCV_XOFF_CNT to user specified value */
        fcCfgReg2Data |= 1 << rcvXoffModeSelOff;
        fcCfgReg2Data &= ~rcvXoffModeCntMsk;
        fcCfgReg2Data |= ~(xoffHoldOff<<(rcvXoffModeCntOff + 8 * slice));
    } else {
        /* set RCV_XOFF_MODE to 0 */
        fcCfgReg2Data &= ~(1 << rcvXoffModeSelOff);
    }

    result = sysHyPhy20gWrite(fileHndl, fcCfgReg2, fcCfgReg2Data);
    if(result) return result;

    /* configure IGNORE_TX_IB_FC */
    shimCfgReg = HYPHY20G_CBRI_SHIM_REG_CFG;
    shimCfgReg = HYPHY20G_CBRI_MTSB_SLICE_REG_OFFSET(shimCfgReg, slice);
    ignoreFcMsk = HYPHY20G_CBRI_SHIM_BIT_CFG_IGNORE_TX_IB_FC_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, shimCfgReg, ignoreFcMsk,
                                   xoffIgnore);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriTxFcResponseCfg */



/*******************************************************************************
**
**  hyPhy20gCbriLaneDisable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function disables one or more of the 8 SERDES interface
**                  lanes of the backplane interface associated with CBRI#0 or
**                  CBRI#1.  The transmit and receive lanes are configured
**                  symmetrically.
**
**                  The CBRI block must be 'Major Bidirectional States' of 2
**                  in order for this function to be executed, otherwise
**                  HYPHY20g_ERR_INVALID_STATE will be returned.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - CBRI block:
**                               0 - CBRI 0
**                               1 - CBRI 1
**                  disableMsk - Mask of lanes to be disabled
**                               Legal range 0 to 0xFF
**                               For CBRI 0 bit index 7:0 corresponds 1:1
**                               to ASSI_P/N/DSSI_P/N[7:0] and
**                               ASSI_P/N/DSSI_P/N[23:16]
**                               For CBRI 1 bit index 7:0 corresponds 1:1
**                               to ASSI_P/N/DSSI_P/N[15:8] and
**                               ASSI_P/N/DSSI_P/N[31:24]
**                               Note setting of 0xFF irrelevant as all lanes
**                               disabled.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriLaneDisable(struct file *fileHndl, UINT4 slice,
                             UINT4 disableMsk)
{
    /* variable declaration */
    UINT4 state;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 1)||(disableMsk > 0xFF)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve major bidirectional state */
    result = utilHyPhy20gIntlknCoreGetMajBiDirState(fileHndl, slice, &state);
    if(result) return result;

    /* confirm major bidirection state of 2 or return invalid state error */
    if(state != HYPHY20G_INTLKN_STATE_STANDBY){
        return HYPHY20G_ERR_INVALID_STATE;
    }

    /* Configure Interlaken Flow Control Mode */
    result = utilHyPhy20gIntlknCoreLaneDisable(fileHndl, slice, disableMsk);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriLaneDisable */

/*******************************************************************************
**
**  hyPhy20gCbriFcModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the in-band Interlaken flow control
**                  reporting method for one of the CBRI instances.
**
**                  The in-band flow control may be programmed to generate
**                  flow control data based on an in-order calendar or on a
**                  programmable calendar.
**
**                  The flow control ports that transfer flow control data
**                  from CBRB to CBRI will be configured so that they are
**                  generating XOFF for the initial flow control state sent
**                  to system side device.
**
**                  When programmable calendar mode of operation is selected,
**                  all programmable calendar entries will be configured with
**                  the NULL flow control port.  It is the user's
**                  responsability to call hyPhy20gCbriFcCalCfg to generate
**                  a meaningful mapping of flow control calendar entries to
**                  flow control ports for their specific application.
**
**                  The CBRI block must be 'Major Bidirectional States' of 2
**                  in order for this function to be executed, otherwise
**                  HYPHY20g_ERR_INVALID_STATE will be returned.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - CBRI block:
**                               0 - CBRI 0
**                               1 - CBRI 1
**                  dir        - 0 - Transmit (data from HyPHY 20G)
**                               1 - Receive (data to HyPHY 20G)
**                  mode       - 0 - In-order flow control calendar
**                               1 - Programmable calendar
**                  ctxt       - For mode == 0:
**                               0 - In-order flow control calendar without link
**                                   level flow control
**                               1 - In-order flow control calendar with link
**                                   level flow control
**                               For mode == 1:
**                               1 to 16 - Number of flow control channel
**                                         entries reported or received in
**                                         Interlaken Idle and Burst Control
**                                         Words.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriFcModeCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                           UINT4 mode, UINT4 ctxt)
{
    /* variable declaration */
    UINT4 devId, revId, status;
    UINT4 state;
    UINT4 fifoTrigEnReg = 0, fifoTrigEnMsk = 0, val = 0;
    UINT4 fcCfgReg = HYPHY20G_OPI_TOP_REG_INTLKN_FC_1;
    UINT4 intlknFcEnblMsk, linkXonOff;
    INT4 result;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 1)||
       (dir > 1)||
       (mode > 1)||
       ((mode == 0)&&(ctxt > 1))||
       ((mode == 1)&&(ctxt > 16)&&(ctxt > 0))){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve major bidirectional state */
    result = utilHyPhy20gIntlknCoreGetMajBiDirState(fileHndl, slice, &state);
    if(result) return result;

    /* confirm major bidirection state of 2 or return invalid state error */
    if(state != HYPHY20G_INTLKN_STATE_STANDBY){
        return HYPHY20G_ERR_INVALID_STATE;
    }

    /* Configure Interlaken Flow Control Mode */
    result = utilHyPhy20gIntlknCoreFcModeCfg(fileHndl, slice, dir, mode, ctxt);
    if(result) return result;

    /* For the receive direction configure initial flow control state. */
    if (dir == 0) {
        /* exit function if transmit direction configured */
        return HYPHY20G_SUCCESS;
    }
    /* set CBRIn_FIFO_TRIG_EN to 1 for all bits associated with the CBRI intf
       this will ensure flow control data received from CBRB rather than
       being internally forced to XON state */
	switch (slice) {
        case 0:
            fifoTrigEnReg = HYPHY20G_OPI_CBRB_REG_FIFO_TRIG_EN_1;
            fifoTrigEnMsk =
                    HYPHY20G_OPI_CBRB_BIT_FIFO_TRIG_EN_1_CBRI1_FIFO_TRIG_EN_MSK;
            break;
        case 1:
            fifoTrigEnReg = HYPHY20G_OPI_CBRB_REG_FIFO_TRIG_EN_2;
            fifoTrigEnMsk =
			        HYPHY20G_OPI_CBRB_BIT_FIFO_TRIG_EN_2_CBRI2_FIFO_TRIG_EN_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    val = 0xFF;

    result = hyPhy20gRegFieldWrite(fileHndl, fifoTrigEnReg, fifoTrigEnMsk, val);
    if(result) return result;

    /* set INTERLAKEN_FC_ENABLE to all 0 for all bits associated with the CBRI
       intf to ensure XOFF state is reported to CBRI flow control ports */
    intlknFcEnblMsk = (slice==1) ? 0xff00:0xff;
    intlknFcEnblMsk = intlknFcEnblMsk <<
                      HYPHY20G_OPI_TOP_BIT_INTLKN_FC_1_INTERLAKEN_FC_ENABLE_OFF;
    val = 0;

    result = hyPhy20gRegFieldWrite(fileHndl, fcCfgReg, intlknFcEnblMsk, val);
    if(result) return result;

    /* set LINK_XON to 0 for the CBRI intf to ensure link level flow
       control port reports XOFF */
    linkXonOff = slice + HYPHY20G_OPI_TOP_BIT_INTLKN_FC_1_LINK_XON_OFF;
    val = 0;
    result = sysHyPhy20gBitWrite(fileHndl, fcCfgReg, linkXonOff, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriFcModeCfg */

/*******************************************************************************
**
**  hyPhy20gCbriFcCalCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the mapping of a flow control
**                  calendar entry to a flow control port for the transmit or
**                  receive calendar of a CBRI instances.
**
**                  This function is only valid for a CBRI instance that have
**                  been configured for programmable calendar mode of FC.
**
**                  More than one calendar entry may be associated with a flow
**                  control port.
**
**                  The CBRI block must be 'Major Bidirectional States' of 2 or
**                  3 in order for this function to be executed, otherwise
**                  HYPHY20G_ERR_INVALID_STATE will be returned.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - CBRI block:
**                               0 - CBRI 0
**                               1 - CBRI 1
**                  dir        - 0 - Transmit (data from HyPHY 20G)
**                               1 - Receive (data to HyPHY 20G)
**                  calAddr    - Calendar address to be configured
**                               0 to 15 - dependent on the length of calendar
**                                         selected when programmable calendar
**                                         mode was configured
**                  fcPort     - Flow control port to be associated with the
**                               calendar address
**                               0 to 7 - Flow control port address
**                               8 - Link Level Flow Control Port
**                               9 - NULL flow control Port (force XOFF to
**                                   calendar entries), only valid for
**                                   flow control data from HyPHY 20G to
**                                   system side device
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_STATE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriFcCalCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                          UINT4 calAddr, UINT4 fcPort)
{
    /* variable declaration */
    UINT4 state;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 1)||
       (dir > 1)||
       (calAddr > 15)||
       ((fcPort > 8)&&(dir == 0))||
       ((fcPort > 9)&&(dir == 1))){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve major bidirectional state */
    result = utilHyPhy20gIntlknCoreGetMajBiDirState(fileHndl, slice, &state);
    if(result) return result;

    /* confirm major bidirection state of 2 or return invalid state error */
    if((state == HYPHY20G_INTLKN_STATE_STANDBY)||
       (state == HYPHY20G_INTLKN_STATE_OPERATIONAL)){
        /* Configure Interlaken Flow Control Calendar Entry */
        result = utilHyPhy20gIntlknCoreFcCalCfg(fileHndl, slice, dir, calAddr,
                                                fcPort);
        if(result) return result;
    } else {
        return HYPHY20G_ERR_INVALID_STATE;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriFcCalCfg */

/*******************************************************************************
**
**  hyPhy20gCbrbOtnZoneChnlCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures ZONE thresholds for CBRB reader
**                  FIFOs related to the OTN subsystem and the ODUKPP Data
**                  Mapper (DM) subsystem.
**
**                  The CBRB FIFO Centering feature is used to force the CBRB
**                  FIFO to center upon startup or after an underrun event.
**                  This will ensure that there is sufficient data in FIFO to
**                  prevent data stream startup up underruns in CBRB, transmit
**                  OTN framers or receive ODUKPP OTN framers.  This feature may
**                  not be used if PMM Monitor is intended to use timestamps to
**                  deskew incoming data.
**
**  INPUTS:         *fileHndl  - base address
**                  rdChnl     - Read channel to be configured
**                             - 0 to 7 OTU1 Framer #0 to #7
**                               8 to 9 OTU2 Framer #0 to #1
**                               10 - ODU2/ODU1 Data Mapper Port
**                               11 to 13 - ODU1 Data Mapper Port
**                               14 - ODU2/ODU1 Data Mapper Port
**                               15 to 17 - ODU1 Data Mapper Port
**                  zoneLow    - low zone threshold
**                  zoneMid    - mid zone threshold
**                  zoneHigh   - high zone threshold
**                               The relationship zoneLow <= zoneMid <= zoneHigh
**                               must be maintained.  Legal range for zone
**                               settings are 0 to 255 words for OTU1 framers
**                                            0 to 511 words for OTU2 framers
**                                            0 to 255 words for DM Port 11 to
**                                                     13 and 15 to 17
**                                            0 to 511 words for DM Port 10 and
**                                                     14
**                  fifoCentEn - CBRB FIFO centering feature enable
**                               0 - Disable FIFO Centering
**                               1 - Enable FIFO Centering
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbOtnZoneChnlCfg(struct file *fileHndl, UINT4 rdChnl,
                                UINT4 zoneLow, UINT4 zoneMid, UINT4 zoneHigh,
                                UINT4 fifoCentEn)
{
    /* variable declaration */
    UINT4 regAddr, regData;
    UINT4 fifoZoneHighMsk, fifoZoneMidMsk, fifoZoneLowMsk, fifoCentEnMsk;
    INT4 result;

    /* argument checking */
    if((rdChnl > 17)||
       (zoneLow > zoneMid)||(zoneLow > zoneHigh)||(zoneMid > zoneHigh)||
       (((rdChnl < 8)||((rdChnl > 10)&&(rdChnl < 14))||((rdChnl > 14)&&
        (rdChnl < 18)))&&(zoneHigh > 255))||
       (((rdChnl == 8)||(rdChnl == 9)||(rdChnl == 10)||(rdChnl == 14))&&
        (zoneHigh > 511))||
       (fifoCentEn > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (rdChnl < 10){
        regAddr = HYPHY20G_OPI_CBRB_REG_OTN_FIFO_ZN_CFG_OFFSET(rdChnl);

        fifoZoneHighMsk = HYPHY20G_OPI_CBRB_BIT_OTN_FIFO_ZN_CFG_OTN_FIFO_ZONE_HIGH_MSK;
        fifoZoneMidMsk = HYPHY20G_OPI_CBRB_BIT_OTN_FIFO_ZN_CFG_OTN_FIFO_ZONE_MID_MSK;
        fifoZoneLowMsk = HYPHY20G_OPI_CBRB_BIT_OTN_FIFO_ZN_CFG_OTN_FIFO_ZONE_LOW_MSK;
        fifoCentEnMsk = HYPHY20G_OPI_CBRB_BIT_OTN_FIFO_ZN_CFG_OTN_FIFO_CENTER_EN_MSK;
    } else {
        regAddr = HYPHY20G_OPI_CBRB_REG_DM_FIFO_ZN_CFG_OFFSET(rdChnl-10);

        fifoZoneHighMsk = HYPHY20G_OPI_CBRB_BIT_DM_FIFO_ZN_CFG_DM_FIFO_ZONE_HIGH_MSK;
        fifoZoneMidMsk = HYPHY20G_OPI_CBRB_BIT_DM_FIFO_ZN_CFG_DM_FIFO_ZONE_MID_MSK;
        fifoZoneLowMsk = HYPHY20G_OPI_CBRB_BIT_DM_FIFO_ZN_CFG_DM_FIFO_ZONE_LOW_MSK;
        fifoCentEnMsk = HYPHY20G_OPI_CBRB_BIT_DM_FIFO_ZN_CFG_DM_FIFO_CENTER_EN_MSK;
    }

    result = sysHyPhy20gRead(fileHndl, regAddr, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, fifoZoneHighMsk, zoneHigh);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, fifoZoneMidMsk, zoneMid);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, fifoZoneLowMsk, zoneLow);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, fifoCentEnMsk, fifoCentEn);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbOtnZoneChnlCfg */


/*******************************************************************************
**
**  hyPhy20gCbrbVcatOtnChnlCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures OTN framers.  The OTN framers
**                  process the ODU-k streams from the CBRB reader FIFOs
**                  destined for the ODUKPP and OTN subsystem.
**
**                  Fail indication to downstream ODUKPP in response to AIS and
**                  OOF will be unconditionally enabled.
**
**                  Fail conditions due to persistance of LOS towards OTN
**                  subsystem will be unconditionally enabled.  3ms default
**                  setting for the interval before declaring this fault
**                  will be applied.
**
**  INPUTS:         *fileHndl  - base address
**                  frmr       - OTN mini framer to be configured
**                               Read channel related mini framers:
**                               0 to 7 OTU1 Framer 0 to 7
**                               8 to 9 OTU2 Framer 0 to 1
**                               10 ODUKPP OTU2 or OTU1 Framer
**                               11 to 13 ODUKPP OTU1 Framers
**                               14 ODUKPP OTU2 or OTU1 Framer
**                               15 to 17 ODUKPP OTU2 or OTU1 Framer
**                               Write channel related mini framers:
**                               18 VCAT_LCAS interface 0, OTU2 or OTU1 #1
**                               19 to 21 VCAT_LCAT interface 0 data towards
**                                        CBRB, OTU1 #2 to #4
**                               22 VCAT_LCAS interface 1, OTU2 or OTU1 #1
**                               23 to 25 VCAT_LCAT interface 1 data towards
**                                        CBRB, OTU1 #2 to #4
**                  enable     - OTN Framer enabling
**                             - 0 disable
**                             - 1 enable
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbVcatOtnChnlCfg(struct file *fileHndl, UINT4 frmr,
                                UINT4 enable)
{
    /* variable declaration */
    UINT4 frmEnReg;
    UINT4 frmReg, regData;
    UINT4 aisPortEnableMsk, oofPortEnableMsk, frmEnableOff;
    UINT4 soakOofTimerMsk, soakOofPortEnableMsk;
    INT4 result;

    /* GB: frmr 18 to 25 may have no downstream processing of their FAIL signals
           therefor PORT_ENABLE is irrelevant to them.  Ajay has indicated that
           these are connected to PMM.  Karl and Ajay to establish the required
           functionality.

           these mini framer could be turned on and off as a function of
           MKR_ENABLE for the VCAT interface related PMM blocks

           it may be more appropriate to move to the PMM functions as
           the framer is transparent in this data path.

           sent email to Ajay to clarify understanding.  provided OOF_STATUS
           issue resolved in rev B, we can share FRM_ENABLE with MKR_ENABLE

    */

    /* argument checking */
    if((enable > 1)||(frmr > 25)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    frmReg = HYPHY20G_OPI_TOP_REG_ALM_CFG_OFFSET(frmr);
    aisPortEnableMsk = HYPHY20G_OPI_TOP_BIT_ALM_CFG_AIS_PORT_ENABLE_MSK;
    oofPortEnableMsk = HYPHY20G_OPI_TOP_BIT_ALM_CFG_OOF_PORT_ENABLE_MSK;
    soakOofPortEnableMsk =
                     HYPHY20G_OPI_TOP_BIT_ALM_CFG_SOAK_OOF_FAIL_PORT_ENABLE_MSK;
    soakOofTimerMsk = HYPHY20G_OPI_TOP_BIT_ALM_CFG_SOAK_OOF_TIMER_MSK;

    result = sysHyPhy20gRead(fileHndl, frmReg, &regData);
    if(result) return result;

    if(frmr > 9){
        /* set AIS_PORT_ENABLE to 1 unconditionally */
        result = hyPhy20gVarFieldWrite (&regData, aisPortEnableMsk, 1);
        if(result) return result;

        /* set OOF_PORT_ENABLE to 1 unconditionally */
        result = hyPhy20gVarFieldWrite (&regData, oofPortEnableMsk, 1);
        if(result) return result;
    } else if (frmr > 7) {
        /* set SOAK_OOF_PORT_ENABLE to 1 unconditionally */
        result = hyPhy20gVarFieldWrite (&regData, soakOofPortEnableMsk, 1);
        if(result) return result;
        /* set SOAK_OOF_TIMER to 249 for 3 ms detection */
        result = hyPhy20gVarFieldWrite (&regData, soakOofTimerMsk, 249);
        if(result) return result;
    } else {
        /* set SOAK_OOF_PORT_ENABLE to 1 unconditionally */
        result = hyPhy20gVarFieldWrite (&regData, soakOofPortEnableMsk, 1);
        if(result) return result;
        /* set SOAK_OOF_TIMER to 61 for 3 ms detection */
        result = hyPhy20gVarFieldWrite (&regData, soakOofTimerMsk, 61);
        if(result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, frmReg, regData);
    if(result) return result;

    /* set FRM_ENABLE based on enable parameter */
    frmEnReg = HYPHY20G_OPI_TOP_REG_FRAMING_CFG;
    frmEnableOff = HYPHY20G_OPI_TOP_BIT_FRAMING_CFG_FRM_ENABLE_OFF;
    frmEnableOff = frmEnableOff + frmr;

    result = sysHyPhy20gBitWrite(fileHndl, frmEnReg, frmEnableOff, enable);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbVcatOtnChnlCfg */



/*******************************************************************************
**
**  hyPhy20gCbrbReaderSrcChnlCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function specifies the CBRB writer channel to be
**                  associated with a CBRB reader FIFO.
**
**                  Multicasting is supported and more than one CBRB reader FIFO
**                  may be configured with the same CBRB writer channel.
**
**  INPUTS:         *fileHndl  - base address
**                  rdChnl     - Read channel to be configured
**                             - 0 to 3 OTU1 #1 to #4 Framer #0
**                               4 to 7 OTU1 #1 to #4 Framer #1
**                               8 to 9 OTU2 Framer #0 to #1
**                               10 ODUKPP OTU2 or OTU1 Framer
**                               11 to 13 ODUKPP OTU1 Framers
**                               14 ODUKPP OTU2 or OTU1 Framer
**                               15 to 17 ODUKPP OTU2 or OTU1 Framer
**                               18 to 25 CBRI #0 ports 0 to 7
**                               26 to 33 CBRI #1 ports 0 to 7
**                  wrChnlIntf - Interface associated with write channel:
**                               0 - OTN or ODUKPP subsystems
**                               1 - CBRI #0
**                               2 - CBRI #1
**                  wrChnl     - For OTN or ODUKPP write channels:
**                               0 to 3 OTU1 #1 to #4 Framer #0
**                               4 to 7 OTU1 #1 to #4 Framer #1
**                               8 to 9 OTU2 Framer #0 to #1
**                               10 ODUKPP OTU2 or OTU1 Framer
**                               11 to 13 ODUKPP OTU1 Framers
**                               14 ODUKPP OTU2 or OTU1 Framer
**                               15 to 17 ODUKPP OTU2 or OTU1 Framer
**                             - For CBRI #0 and CBRI #1 write channels:
**                               0 to 33 - the 'channel number' contained in
**                               Interlaken Burst Control Words that are to
**                               be sent to the reader buffer.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbReaderSrcChnlCfg(struct file *fileHndl, UINT4 rdChnl,
                                  UINT4 wrChnlIntf, UINT4 wrChnl)
{
    /* variable declaration */
    UINT4 regAddr, regData, wrifSrcMsk, wrifSrcMuxMsk;
    UINT4 wrifSrcMuxVal = 0;
    INT4 result;

    /* argument checking */
    if((wrChnl > 33)||(rdChnl > 33)||(wrChnlIntf > 2)||
       ((wrChnlIntf == 0)&&(wrChnl > 17))){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* recover register address */
    switch (rdChnl) {
        case 0:
        case 1:
        case 2:
        case 3:
            regAddr = HYPHY20G_OPI_CBRB_REG_RDR_WR_SRC_1;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            regAddr = HYPHY20G_OPI_CBRB_REG_RDR_WR_SRC_2;
            break;
        case 8:
        case 9:
        case 10:
        case 11:
            regAddr = HYPHY20G_OPI_CBRB_REG_RDR_WR_SRC_3;
            break;
        case 12:
        case 13:
        case 14:
        case 15:
            regAddr = HYPHY20G_OPI_CBRB_REG_RDR_WR_SRC_4;
            break;
        case 16:
        case 17:
            regAddr = HYPHY20G_OPI_CBRB_REG_RDR_WR_SRC_5;
            break;
        case 18:
        case 19:
            regAddr = HYPHY20G_OPI_CBRB_REG_RDR_WR_SRC_5;
            break;
        case 20:
        case 21:
        case 22:
        case 23:
            regAddr = HYPHY20G_OPI_CBRB_REG_RDR_WR_SRC_6;
            break;
        case 24:
        case 25:
            regAddr = HYPHY20G_OPI_CBRB_REG_RDR_WR_SRC_7;
            break;
        case 26:
        case 27:
            regAddr = HYPHY20G_OPI_CBRB_REG_RDR_WR_SRC_7;
            break;
        case 28:
        case 29:
        case 30:
        case 31:
            regAddr = HYPHY20G_OPI_CBRB_REG_RDR_WR_SRC_8;
            break;
        case 32:
        case 33:
            regAddr = HYPHY20G_OPI_CBRB_REG_RDR_WR_SRC_9;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* write interface source registers use common masks for the four channels
       described within each register (excluding the last register with data
       for two channels).  Masks for the first register are used here to
       provide masks for any channel */
    switch (rdChnl%4) {
        case 0:
            wrifSrcMuxMsk =
                     HYPHY20G_OPI_CBRB_BIT_RDR_WR_SRC_1_RDIF_1_WRIF_SRC_MUX_MSK;
            wrifSrcMsk = HYPHY20G_OPI_CBRB_BIT_RDR_WR_SRC_1_RDIF_1_WRIF_SRC_MSK;
            break;
        case 1:
            wrifSrcMuxMsk =
                     HYPHY20G_OPI_CBRB_BIT_RDR_WR_SRC_1_RDIF_2_WRIF_SRC_MUX_MSK;
            wrifSrcMsk = HYPHY20G_OPI_CBRB_BIT_RDR_WR_SRC_1_RDIF_2_WRIF_SRC_MSK;
            break;
        case 2:
            wrifSrcMuxMsk =
                     HYPHY20G_OPI_CBRB_BIT_RDR_WR_SRC_1_RDIF_3_WRIF_SRC_MUX_MSK;
            wrifSrcMsk = HYPHY20G_OPI_CBRB_BIT_RDR_WR_SRC_1_RDIF_3_WRIF_SRC_MSK;
            break;
        case 3:
            wrifSrcMuxMsk =
                     HYPHY20G_OPI_CBRB_BIT_RDR_WR_SRC_1_RDIF_4_WRIF_SRC_MUX_MSK;
            wrifSrcMsk = HYPHY20G_OPI_CBRB_BIT_RDR_WR_SRC_1_RDIF_4_WRIF_SRC_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (wrChnlIntf) {
        case 0:
            wrifSrcMuxVal = 0;
            break;
        case 1:
            wrifSrcMuxVal = 2;
            break;
        case 2:
            wrifSrcMuxVal = 3;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = sysHyPhy20gRead(fileHndl, regAddr, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, wrifSrcMsk, wrChnl);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, wrifSrcMuxMsk, wrifSrcMuxVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbReaderSrcChnlCfg */


/*******************************************************************************
**
**  hyPhy20gCbrbCbriFlowControlCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures flow control related data for a
**                  CBRB CBRI port.
**
**                  For a CBRI flow control port, the destination reader FIFO
**                  number and destination reader FIFO thresholds are
**                  configured.
**
**                  Only one destination FIFO reader may be specified for a
**                  flow control port even though multicasting is supported.
**
**                  A destination FIFO reader may only be associated with a
**                  flow control port once.  Multiple readers cannot map to
**                  the same flow control port.
**
**                  This function must be called before enabling the writer with
**                  hyPhy20gCbrbFifoWriterChnlEnable.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - CBRI port number:
**                               0 - CBRI #0
**                               1 - CBRI #1
**                  fcPort     - Flow control port
**                             - 0 to 7
**                  rdChnl     - destionation reader FIFO channel
**                               0 to 33
**                  trigLow    - Low FIFO trigger threshold
**                               Tne number of empty FIFO locations that must
**                               be exceeded before flow control status
**                               transitions from XOFF to XON
**                               Legal Range:
**                               0 to 128 for rdChnl 18 to 33
**                               0 to 256 for rdChnl 0 to 7, 11 to 13, and 15 to
**                               17
**                               0 to 512 for rdChnl 8 to 10 and 14
**                               trigLow > trigHigh
**                  trigHigh   - High FIFO trigger threshold
**                               Tne number of empty FIFO locations below which
**                               flow control status transitions from XON to
**                               XOFF
**                               Legal Range:
**                               0 to 128 for rdChnl 18 to 33
**                               0 to 256 for rdChnl 0 to 7, 11 to 13, and 15 to
**                               17
**                               0 to 512 for rdChnl 8 to 10 and 14
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbCbriFlowControlCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 fcPort, UINT4 rdChnl, UINT4 trigLow,
                                    UINT4 trigHigh)
{
    /* variable declaration */
    UINT4 fifoTrigSrcReg, fifoTrigThreshReg, regData;
    UINT4 fifoTrigSrcMsk, fifoTrigLowMsk, fifoTrigHighMsk;
    INT4 result;

    /* argument checking */
    if((slice > 1)||(fcPort > 7)||(rdChnl > 33)||
       (trigLow < trigHigh)||
       (trigHigh > 511)||
       ((trigHigh > 255)&&(rdChnl < 8))||
       ((trigHigh > 255)&&((rdChnl < 14)&&(rdChnl > 10)))||
       ((trigHigh > 255)&&((rdChnl < 18)&&(rdChnl > 14)))||
       ((trigHigh > 127)&&(rdChnl > 17))){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* fifoTrigReg is indexed by interlaken flow control port 0 to 7 */
    fifoTrigSrcReg = HYPHY20G_OPI_CBRB_REG_FIFO_TRIG_THR_OFFSET(fcPort);
    /* FIFO_TRIG_SRC bits  5:0 are associated with CBRI #0
                     bits 11:6 are associated with CBRI #1 */
    fifoTrigSrcMsk = (slice==1) ? 0xfc0:0x3f;
    fifoTrigSrcMsk = fifoTrigSrcMsk <<
                     HYPHY20G_OPI_CBRB_BIT_FIFO_TRIG_THR_FIFO_TRIG_SRC_OFF;

    /* FIFO thresholds are indexed by the reader buffer number */
    fifoTrigThreshReg = HYPHY20G_OPI_CBRB_REG_FIFO_TRIG_THR_OFFSET(rdChnl);
    fifoTrigLowMsk = HYPHY20G_OPI_CBRB_BIT_FIFO_TRIG_THR_FIFO_TRIG_LOW_MSK;
    fifoTrigHighMsk = HYPHY20G_OPI_CBRB_BIT_FIFO_TRIG_THR_FIFO_TRIG_HIGH_MSK;

    /* Configure FIFO_TRIG_SRC */
    result = sysHyPhy20gRead(fileHndl, fifoTrigSrcReg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, fifoTrigSrcMsk, rdChnl);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, fifoTrigSrcReg, regData);
    if(result) return result;

    /* Configure FIFO_TRIG_HIGH and FIFO_TRIG_LOW */
    result = sysHyPhy20gRead(fileHndl, fifoTrigThreshReg, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, fifoTrigLowMsk, trigLow);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, fifoTrigHighMsk, trigHigh);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, fifoTrigThreshReg, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbCbriFlowControlCfg */


/*******************************************************************************
**
**  hyPhy20gCbrbFifoReaderChnlEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables a CBRB reader FIFO.
**
**                  For CBRI related ports, rate data is used to appropriately
**                  update the CBRI SCBS scheduler calendar.
**
**                  This function should be called prior to
**                  hyPhy20gCbrbWriterChnlEnable for the first reader FIFO
**                  associated with a writer channel.  For additional multicast
**                  channels this function may be called before or after
**                  hyPhy20gCbrbFifoWriterChnlEnable.
**
**  INPUTS:         *fileHndl  - base address
**                  rdChnl     - Read channel to be configured
**                             - 0 to 3 OTU1 #1 to #4 Framer #0
**                               4 to 7 OTU1 #1 to #4 Framer #1
**                               8 to 9 OTU2 Framer #0 to #1
**                               10 ODUKPP OTU2 or OTU1 Framer
**                               11 to 13 ODUKPP OTU1 Framers
**                               14 ODUKPP OTU2 or OTU1 Framer
**                               15 to 17 ODUKPP OTU2 or OTU1 Framer
**                               18 to 25 CBRI Interface 0 channel 0 to 7
**                               26 to 33 CBRI Interface 1 channel 0 to 7
**                  rate       - 0 - No rate
**                               1 - ODU1
**                               2 - ODU2/2e/1e/2f/1f
**                               Rate must be set to 0 when enable is set to 0.
**                               Rate must not be set to 0 when enable is set to
**                               1.
**                  enable     - 0 disable
**                             - 1 enable
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbFifoReaderChnlEnable(struct file *fileHndl, UINT4 rdChnl,
                                      UINT4 rate, UINT4 enable)
{
    /* variable declaration */
    UINT4 rstReg, rstOff, slice;
    INT4 result;

    /* argument checking */
    if((enable > 1)||(rdChnl > 33)||(rate > 2)||
       ((enable == 0)&&(rate != 0))||
       ((enable == 1)&&(rate == 0))){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if(rdChnl < 32){
        rstReg = HYPHY20G_OPI_CBRB_REG_FIFO_RDR_RST_1;
        rstOff = rdChnl;
    } else {
        rstReg = HYPHY20G_OPI_CBRB_REG_FIFO_RDR_RST_2;
        rstOff = rdChnl - 32;
    }

    if(enable){
        /* unconditionally disable FIFO_READER_RST = 1 */
        result = sysHyPhy20gBitWrite(fileHndl, rstReg, rstOff, 1);
        if(result) return result;
    }

    /* update CBRI scheduler for CBRI reader ports */
    if(rdChnl > 17){
        slice = (rdChnl > 25) ? 1:0;
        /* update context associated with channels used in scheduler */
        result = cbrbHyPhy20gCbriUpdateChnlCtxt(fileHndl, slice, rdChnl, rate);
        if(result) return result;

        /* update scheduler to reflect channels used */
        result = cbrbHyPhy20gCbriUpdateSchd(fileHndl, slice);
        if(result) return result;
    }

    if(!enable){
        /* unconditionally disable FIFO_READER_RST = 1 */
        result = sysHyPhy20gBitWrite(fileHndl, rstReg, rstOff, 1);
        if(result) return result;
    }

    if(enable==1){
        /* set FIFO_READER_RST = 0 */
        result = sysHyPhy20gBitWrite(fileHndl, rstReg, rstOff, 0);
        if(result) return result;

    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbFifoReaderChnlEnable */

/*******************************************************************************
**
**  hyPhy20gCbrbFifoWriterChnlEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables the writer of a CBRB FIFO buffer.
**
**                  This function will always reset the channel when enable = 1
**                  as part of the enabling process.
**
**  INPUTS:         *fileHndl  - base address
**                  rdChnl     - Read FIFO buffer to be configured
**                             - 0 to 3 OTU1 #1 to #4 Framer #0
**                               4 to 7 OTU1 #1 to #4 Framer #1
**                               8 to 9 OTU2 Framer #0 to #1
**                               10 ODUKPP OTU2 or OTU1 Framer
**                               11 to 13 ODUKPP OTU1 Framers
**                               14 ODUKPP OTU2 or OTU1 Framer
**                               15 to 17 ODUKPP OTU2 or OTU1 Framer
**                               18 to 25 CBRI Interface 0 channel 0 to 7
**                               26 to 33 CBRI Interface 1 channel 0 to 7
**                  enable     - 0 disable
**                             - 1 enable
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbFifoWriterChnlEnable(struct file *fileHndl, UINT4 rdChnl,
                                      UINT4 enable)
{
    /* variable declaration */
    UINT4 rstReg, wrifEnReg, rstEnOff;
    INT4 result;

    /* argument checking */
    if((enable > 1)||(rdChnl > 33)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if(rdChnl < 32){
        rstReg = HYPHY20G_OPI_CBRB_REG_FIFO_WR_RST_1;
        wrifEnReg = HYPHY20G_OPI_CBRB_REG_WRIF_EN_1;
        rstEnOff = rdChnl;
    } else {
        rstReg = HYPHY20G_OPI_CBRB_REG_FIFO_WR_RST_2;
        wrifEnReg = HYPHY20G_OPI_CBRB_REG_WRIF_EN_2;
        rstEnOff = rdChnl - 32;
    }

    /* unconditionally disable FIFO_WRITER_RST = 1 and WRIF_EN = 0 */
    result = sysHyPhy20gBitWrite(fileHndl, wrifEnReg, rstEnOff, 0);
    if(result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, rstReg, rstEnOff, 1);
    if(result) return result;


    if(enable==1){
        /* set FIFO_WRITER_RST = 0 and WRIF_EN = 1 */
        result = sysHyPhy20gBitWrite(fileHndl, rstReg, rstEnOff, 0);
        if(result) return result;

        result = sysHyPhy20gBitWrite(fileHndl, wrifEnReg, rstEnOff, 1);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbFifoWriterChnlEnable */


/*******************************************************************************
**
**  hyPhy20gCbrbCbriFlowControlEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables the reporting of
**                  Interlaken flow control status to an upstream device for a
**                  flow control port.
**
**                  When disabled XOFF will be forced on the flow control port.
**                  When enabled the CBRB reader FIFO flow control status
**                  associated with the flow control port (as configured in
**                  hyPhy20gCbrbReaderSrcChnlCfg) will be reported to upstream
**                  device.  When forced the channel will unconditionally report
**                  XON.
**
**                  This function is also used to control link level flow
**                  control status to upstream device.  Link level flow control
**                  can be forced to XON (enable = 1) or XOFF (enable = 0)
**                  state.  Link level flow control is accessed with fcPort = 8.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - CBRI port number:
**                               0 - CBRI #0
**                               1 - CBRI #1
**                  fcPort     - Flow control port
**                             - 0 to 7 for channel level flow control
**                             - 8 for link level flow control
**                  enable     - 0 disable (report XOFF)
**                             - 1 enable (report CBRB Buffer XON/XOFF state)
**                             - 2 force (report XON)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbCbriFlowControlEnable(struct file *fileHndl, UINT4 slice,
                                       UINT4 fcPort, UINT4 enable)
{
    /* variable declaration */
    UINT4 linkXonMsk;
    UINT4 fcEnblMsk;
    UINT4 fcCfgReg = HYPHY20G_OPI_TOP_REG_INTLKN_FC_1;
    UINT4 fifoTrigEnReg, fifoTrigEnMsk, val;
    INT4 result;

    /* argument checking */
    if((enable > 2)||(slice > 1)||(fcPort > 8)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Flow control state bit operation:
       LINK LEVEL FLOW CONTROL:
       LINK_XON = 0 generate XOFF
                = 1 generate XON
       CHANNEL LEVEL FLOW CONTROL:
       CBRIn_FIFO_TRIG_EN[m] = 1 AND
       INTERLAKEN_FC_ENABLE = 0 generate XOFF
                            = 1 generate CBRB reader FIFO flow control status
       CBRIn_FIFO_TRIG_EN[m] = 0 AND
       INTERLAKEN_FC_ENABLE = DONT CARE generate XON */

    /* field generation */
    linkXonMsk = 1 << slice << HYPHY20G_OPI_TOP_BIT_INTLKN_FC_1_LINK_XON_OFF;
    fcEnblMsk = 1 << (slice * 8) << (fcPort) <<
                HYPHY20G_OPI_TOP_BIT_INTLKN_FC_1_INTERLAKEN_FC_ENABLE_OFF;

	switch (slice) {
        case 0:
            fifoTrigEnReg = HYPHY20G_OPI_CBRB_REG_FIFO_TRIG_EN_1;
            fifoTrigEnMsk = 1 <<
                  HYPHY20G_OPI_CBRB_BIT_FIFO_TRIG_EN_1_CBRI1_FIFO_TRIG_EN_OFF <<
                  fcPort;
            break;
        case 1:
            fifoTrigEnReg = HYPHY20G_OPI_CBRB_REG_FIFO_TRIG_EN_2;
            fifoTrigEnMsk = 1 <<
                  HYPHY20G_OPI_CBRB_BIT_FIFO_TRIG_EN_2_CBRI2_FIFO_TRIG_EN_OFF <<
                  fcPort;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configuration settings */
    if (fcPort == 8) {
        val = enable > 0 ? 1:0;
        result = hyPhy20gRegFieldWrite(fileHndl, fcCfgReg, linkXonMsk,
                                       val);
        if(result) return result;
    } else {
        switch (enable){
            case 0:
            case 1:
                /* set INTERLAKEN_FC_ENABLE to generate XOFF or CBRB status
                   based on enable field */
                result = hyPhy20gRegFieldWrite(fileHndl, fcCfgReg, fcEnblMsk,
                                               enable);
                if(result) return result;
                /* set CBRInFIFO_TRIG_EN to 1 so CBRB state propogates and
                   is not forced to XON */
                result = hyPhy20gRegFieldWrite(fileHndl, fifoTrigEnReg,
                                               fifoTrigEnMsk, 1);
                if(result) return result;
                break;
            case 2:
                /* set CBRInFIFO_TRIG_EN to 0 so CBRB state does not propogate
                   and is forced to XON */
                result = hyPhy20gRegFieldWrite(fileHndl, fifoTrigEnReg,
                                               fifoTrigEnMsk, 0);
                break;
            default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbCbriFlowControlEnable */

/*******************************************************************************
**
**  hyPhy20gCbrbCbriRxSerdesSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects which set of 8 backplane SERDES links
**                  are processed by the CBRI instance
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - CBRI port number:
**                               0 - CBRI #0
**                               1 - CBRI #1
**                  rxSrc      - Backplane Serdes Links
**                             - 0 - assi[7:0] for CBRI #0
**                                   assi[15:8] for CBRI #1
**                             - 1 - assi[23:16] for CBRI #0
**                                   assi[31:24] for CBRI #1
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbrbCbriRxSerdesSrcCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 rxSrc)
{
    /* variable declaration */
    UINT4 backEnMsk =
             HYPHY20G_OPI_TOP_BIT_SERDES_BACKPLANE_MD_SERDES_BACK0_CBRI_SEL_MSK;
    UINT4 gpoReg = HYPHY20G_OPI_TOP_REG_SERDES_BACKPLANE_MD;
    INT4 result;

    /* argument checking */
    if((slice > 1)||(rxSrc > 1)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if(slice) backEnMsk =
             HYPHY20G_OPI_TOP_BIT_SERDES_BACKPLANE_MD_SERDES_BACK1_CBRI_SEL_MSK;

    /* set SERDES_BACK1_EN or SERDES_BACK0_EN as specified */
    result = hyPhy20gRegFieldWrite(fileHndl, gpoReg, backEnMsk, rxSrc);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbrbCbriRxSerdesSrcCfg */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gCbriCreateCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates and intializes memory required to operate
**                  the CBRI subsystem.
**
**                  References this memory to the device level structure
**                  pointed to with *pDevCtxt.
**
**                  Function hyPhy20gCbriDestroyCtxt recovers this memory and
**                  set the device level reference to NULL.
**
**  INPUTS:         deviceHndl      - (pointer to) HyPHY 20G Context
**                  slice           - CBRI interface number to have context
**                                    generated:
**                                    0 - CBRI #0
**                                    1 - CBRI #1
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriCreateCtxt(struct file *fileHndl, UINT4 slice)
{
    /* variable declaration */
    sHYPHY20G_CBRI_RD_CHNL_CTXT *pChnlCtxt;
    sHYPHY20G_SCHD_CTXT *pSchdCtxt;
    sHYPHY20G_CBRI_CTXT  *pCbriCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 size, i;

    /* argument checking */
    if(slice > 1){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* If pCbriCtxt is not a NULL pointer,
       destroy it first to prevent memory leak */
    if (pDevCtxt->pCbriCtxt[slice] != NULL) {
        hyPhy20gCbriDestroyCtxt(fileHndl, slice);
    }

    /* allocate memory for and associate with pDevCtxt */
    size = sizeof(sHYPHY20G_CBRI_CTXT);
    pDevCtxt->pCbriCtxt[slice] = (sHYPHY20G_CBRI_CTXT *) sysHyPhy20gMemAlloc(size);

    if(pDevCtxt->pCbriCtxt[slice] == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    DBG_PRINT("CBRI slice #%ld Context memory allocated:addr=0x%x size=%ld\n",
              slice, (unsigned int)pDevCtxt->pCbriCtxt[slice], size);

    pCbriCtxt = pDevCtxt->pCbriCtxt[slice];


    /* allocate memory for the pointed to structures */
    size = sizeof(sHYPHY20G_SCHD_CTXT) +
           sizeof(sHYPHY20G_CBRI_RD_CHNL_CTXT) * HYPHY20G_OPI_SCHD_MAX_CHNL;

    pCbriCtxt->pSchdCtxt = (sHYPHY20G_SCHD_CTXT *) sysHyPhy20gMemAlloc(size);

    DBG_PRINT("CBRI slice #%ld Pointed To Context memory allocated:addr=0x%x size=%ld\n",
              slice, (unsigned int)pDevCtxt->pCbriCtxt[slice], size);

    if(pCbriCtxt->pSchdCtxt == NULL) {
        /* deallocate memory allocated up to this point in function */
        DBG_PRINT("file %s, line %d, - HYPHY20G_ERR_MEM_ALLOC, size=%ld\n",
                  __FILE__, __LINE__, size);
        sysHyPhy20gMemFree(pDevCtxt->pCbriCtxt[slice]);
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    pCbriCtxt->pChnlCtxt =
                      (sHYPHY20G_CBRI_RD_CHNL_CTXT *)(pCbriCtxt->pSchdCtxt + 1);

    pCbriCtxt->maxChnl = HYPHY20G_OPI_SCHD_MAX_CHNL;

    /* reference local pointer to allocated data structure */
    pSchdCtxt = pCbriCtxt->pSchdCtxt;
    pChnlCtxt = pCbriCtxt->pChnlCtxt;

    /* initialize memories */
    if(slice==0){
        pCbriCtxt->baseChnl = HYPHY20G_OPI_CBRI1_CHNL_OFFSET;
    } else {
        pCbriCtxt->baseChnl = HYPHY20G_OPI_CBRI2_CHNL_OFFSET;
    }

    for(i=0; i < HYPHY20G_OPI_SCHD_MAX_CHNL; i++){
        pChnlCtxt->valid = HYPHY20G_VALID_SIGN;
        pChnlCtxt->rate = 0;
        pChnlCtxt++;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriCreateCtxt */

/*******************************************************************************
**
**  hyPhy20gCbriDestroyCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset context of the Cbri subsystem.  Deallocates memory
**                  used for storing this information.
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - CBRI interface number to have context
**                                    destroyed:
**                                    0 - CBRI #0
**                                    1 - CBRI #1
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCbriDestroyCtxt(struct file *fileHndl, UINT4 slice)
{
    /* variable declaration */
    sHYPHY20G_CBRI_CTXT  *pCbriCtxt;
    sHYPHY20G_SCHD_CTXT  *pSchd;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if(slice > 1){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* retrieve CBRI context for appropriate slice */
    pCbriCtxt = pDevCtxt->pCbriCtxt[slice];
    if(pCbriCtxt == NULL){
        /* Nothing to destroy */
    	return HYPHY20G_SUCCESS;
    }

    /* destroy scheduler related structures */
    pSchd = pCbriCtxt->pSchdCtxt;
    if(pSchd != NULL){
        /* destroy scheduler context */
        utilHyPhy20gScbsDestroySchd(pSchd);
    }

    /* destroy memory allocated for pSchdCtxt and pChnlCtxt */
    sysHyPhy20gMemFree(pCbriCtxt->pSchdCtxt);
    /* destroy pCbriCtxt structure for the slice */
    sysHyPhy20gMemFree(pDevCtxt->pCbriCtxt[slice]);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCbriDestroyCtxt */


/*
cbrbHyPhy20gCbriUpdateSchd
    for the slice, analyze the channel data and provide calendar population.

    populate across two rows.

    error if channel data exceeds number of entries in scheduler

    provide another function to update context for the channel for add or
    remove operation

cbrbHyPhy20gCbriUpdateChnlCtxt
    slice, chnlNum, rate;
*/

/*******************************************************************************
**
**  cbrbHyPhy20gCbriUpdateChnlCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Evaluates channel context data for the CBRI slice and
**                  generates calendar data and applies to the scheduler.
**
**                  The function will return an error if the scheduler is
**                  oversubscribed.
**
**                  ODU-1 rate channels receive a minimum of 1 entry, ODU-2
**                  rate (including ODU-2e/2f/1e/1f) receive a minimum of 4
**                  calendar entries.  Unused entries will be padded with
**                  valid channel number.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - CBRI interface number to have context
**                                    destroyed:
**                                    0 - CBRI #0
**                                    1 - CBRI #1
**                  rate            - Channel Data Rate
**                                    0 - No rate (channel being removed)
**                                    1 - ODU1
**                                    2 - ODU2
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cbrbHyPhy20gCbriUpdateChnlCtxt(struct file *fileHndl, UINT4 slice,
                                    UINT4 chnlNum, UINT4 rate)
{
    /* variable declaration */
    sHYPHY20G_CBRI_RD_CHNL_CTXT *pChnlCtxt;
    sHYPHY20G_CBRI_CTXT  *pCbriCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 chnlOffset;

    /* argument checking */
    if((slice > 1)||(rate > 2)||
       ((slice == 0)&&((chnlNum > 25)||(chnlNum < 18)))||
       ((slice == 1)&&((chnlNum > 33)||(chnlNum < 26)))){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* retrieve CBRI context for appropriate slice */
    pCbriCtxt = pDevCtxt->pCbriCtxt[slice];
    if(pCbriCtxt == NULL){
        /* return an error if pointer invalid */
    	return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* normalize channel number to range used by scheduler */
    chnlOffset = chnlNum - pCbriCtxt->baseChnl;

    /* reference data structures within slice */
    pChnlCtxt = pCbriCtxt->pChnlCtxt+chnlOffset;

    pChnlCtxt->rate = rate;

    return HYPHY20G_SUCCESS;

} /* cbrbHyPhy20gCbriUpdateChnlCtxt */

/*******************************************************************************
**
**  cbrbHyPhy20gCbriUpdateSchd
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Evaluates channel context data for the CBRI slice and
**                  generates calendar data and applies to the scheduler.
**
**                  The function will return an error if the scheduler is
**                  oversubscribed.
**
**                  ODU-1 rate channels receive a minimum of 1 entry, ODU-2
**                  rate (including ODU-2e/2f/1e/1f) receive a minimum of 4
**                  calendar entries.  Unused entries will be padded with
**                  valid channel number.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - CBRI interface number to have context
**                                    destroyed:
**                                    0 - CBRI #0
**                                    1 - CBRI #1
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 cbrbHyPhy20gCbriUpdateSchd(struct file *fileHndl, UINT4 slice)
{
    /* variable declaration */
    sHYPHY20G_CBRI_RD_CHNL_CTXT *pChnlCtxt;
    sHYPHY20G_CBRI_CTXT  *pCbriCtxt;
    sHYPHY20G_SCHD_CTXT  *pSchd;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 calData[HYPHY20G_OPI_SCHD_MAX_CHNL];
    UINT4 zeroRateArray[HYPHY20G_OPI_SCHD_MAX_CHNL];
    UINT4 odu1RateArray[HYPHY20G_OPI_SCHD_MAX_CHNL];
    UINT4 odu2RateArray[HYPHY20G_OPI_SCHD_MAX_CHNL];
    UINT4 linearCal[HYPHY20G_OPI_SCHD_MAX_CHNL];
    UINT4 numZeroRate = 0;
    UINT4 numOdu1Rate = 0;
    UINT4 numOdu2Rate = 0;
    UINT4 numEntryPerChnl;
    UINT4 i, j, k;
    INT4 result;

    /* argument checking */
    if(slice > 1){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* retrieve CBRI context for appropriate slice */
    pCbriCtxt = pDevCtxt->pCbriCtxt[slice];
    if(pCbriCtxt == NULL){
        /* return an error if pointer invalid */
    	return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* reference data structures within slice */
    pSchd = pCbriCtxt->pSchdCtxt;
    pChnlCtxt = pCbriCtxt->pChnlCtxt;

    /* recover rate data */
    for(i=0; i < HYPHY20G_OPI_SCHD_MAX_CHNL; i++){
        switch (pChnlCtxt->rate) {

        case HYPHY20G_CBRI_NO_RATE:
            zeroRateArray[numZeroRate] = i;
            numZeroRate++;
            break;
        case HYPHY20G_CBRI_ODU1_RATE:
            odu1RateArray[numOdu1Rate] = i;
            numOdu1Rate++;
            break;
        case HYPHY20G_CBRI_ODU2_RATE:
            odu2RateArray[numOdu2Rate] = i;
            numOdu2Rate++;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
        }
        pChnlCtxt++;
    }

    /* return an error if CBRI interface oversubscribed */
    if((numOdu1Rate + 4*numOdu2Rate > 8)||
       (numOdu2Rate > 2)||(numOdu1Rate > 8)){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* default populate entries with NULL address (0xFF) */
    for(i=0; i < HYPHY20G_OPI_SCHD_MAX_CHNL; i++){
        linearCal[i] = HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER;
        calData[i] = HYPHY20G_SCBS_SCHD_NULL_CHNL_NUMBER;
    }

    /* format data for entry into calendar */

    /* populate calendar for ODU2 rate entries - maximum of 2 ODU2 rate
       channels */
    for(i=0; i < numOdu2Rate; i++){
        /* every other entry receives the channel number */
        switch (i) {
        case 0:
            for(j=0; j < HYPHY20G_OPI_SCHD_MAX_CHNL; j++){
                calData[j] = odu2RateArray[0];
            }
            break;
        case 1:
            calData[1] = odu2RateArray[1];
            calData[3] = odu2RateArray[1];
            calData[5] = odu2RateArray[1];
            calData[7] = odu2RateArray[1];
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
    }

    /* populate calendar for ODU-1 rate entries */
    if (numOdu1Rate) {
        switch (numOdu2Rate) {
        case 0:
            numEntryPerChnl = HYPHY20G_OPI_SCHD_MAX_CHNL/numOdu1Rate;
            k = 0;
            for (i=0; i < numEntryPerChnl; i++) {
                for (j=0; j < numOdu1Rate; j++) {
                    /* populate linear calendar with channel number */
                    linearCal[k] = odu1RateArray[j];
                    k++;
                }
            }

            /* apply the linear calendar data to the device scheduler calendar
               format */
            j = 0;
            for(i = 0; i < pSchd->numRows; i++){
                calData[i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW] = linearCal[j];
                calData[i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + 1] =
                                                                 linearCal[j+1];
                j += 2;
            }
            for(i=0; i < pSchd->numRows; i++){
                calData[i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + 2] =
                                                                   linearCal[j];
                calData[i*HYPHY20G_SCBS_SCHD_CAL_ENTRY_PER_ROW + 3] =
                                                                 linearCal[j+1];
                j += 2;
            }
            break;
        case 1:
            /* ODU2 channel has been entered into all entries, apply one
               entry for each ODU1 rate channel*/
            for(i=0; i < numOdu1Rate; i++){
                calData[i] = odu1RateArray[i];
            }
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
    }

    /* update scheduler calendar with new data */
    result = utilHyPhy20gScbsUpdateCal(fileHndl, pSchd, calData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* cbrbHyPhy20gCbriUpdateSchd */

/*******************************************************************************
**
**  cbriHyPhy20gCbriGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the size of CBRI #0 and #1 context.
**
**
**  INPUTS:         fileHndl      - device handle
**
**  OUTPUTS:        None 
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The size of CBRI #0 and #1 context
**
*******************************************************************************/
UINT4 cbriHyPhy20gCbriGetCtxtSize(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_CBRI_CTXT *pCbriCtxt;
    UINT4 slice = 0, size = 0, i = 0;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    /* extract each slice context */
    for (slice = 0; slice < HYPHY20G_OPI_CBRI_NUM_SLICE; slice++) {
        DBG_PRINT("%s, %s, %d, slice %ld 1st offset 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, size);

        /* include ctxt header size */
        size += sizeof(sHYPHY20G_CTXT_HDR);

        pCbriCtxt = pDevCtxt->pCbriCtxt[slice];
        if (pCbriCtxt != NULL) {
        	/* calculate the first level structure size of CBRI subsystem */
            size += sizeof(sHYPHY20G_CBRI_CTXT);

            DBG_PRINT("%s, %s, %d, slice %ld 2nd SCHD offset 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, slice, size);

            /* calculate the second level size of SCHD */
            /* include the ctxt header size */
            size += utilHyPhy20gGetCtxtHdrSize();
            size += sizeof(sHYPHY20G_SCHD_CTXT);


            DBG_PRINT("%s, %s, %d, slice %ld 2nd rdChnl offset 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, slice, size);

            /* calculate the second level size of CBRI rdChnl */
            /* include the ctxt header size */
            for (i = 0; i < pCbriCtxt->maxChnl; i++) {
                size += utilHyPhy20gGetCtxtHdrSize();
                size += sizeof(sHYPHY20G_CBRI_RD_CHNL_CTXT);
            }

            DBG_PRINT("%s, %s, %d, slice %ld 3rd SCBS offset 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, slice, size);

            /* calculate the third level size of SCBS */
            /* calculate pCbriCtxt->pSchdCtxt->pScbsGrpCtxt, 
            pCbriCtxt->pSchdCtxt->pOnLineCalMem, 
            and pCbriCtxt->pSchdCtxt->pOffLineCalMem context size */
            size += utilHyPhy20gScbsGetSchdCtxtSize(pCbriCtxt->pSchdCtxt);
        }
        DBG_PRINT("%s, %s, %d, slice %ld offset end 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, size);
    }
    return size;
}/* cbriHyPhy20gCbriGetCtxtSize */

/*******************************************************************************
**
**  cbriHyPhy20gCbriExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the CBRI #0 and #1 context into pre-allocated 
**                  buffer.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-allocated buffer for CBRI #0 and #1 
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
INT4 cbriHyPhy20gCbriExtractCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_CBRI_CTXT *pCbriCtxt;
    INT4 result;
    UINT4 rtLen;
    UINT4 slice = 0, offset = 0, len = 0, i = 0;
    
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
    for (slice = 0; slice < HYPHY20G_OPI_CBRI_NUM_SLICE; slice++) {
        DBG_PRINT("%s, %s, %d, slice %ld 1st offset 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, offset);

        pCbriCtxt = pDevCtxt->pCbriCtxt[slice];

        /* insert ctxt buffer header */
		len = (pCbriCtxt == NULL) ? sizeof(sHYPHY20G_CTXT_HDR) : 
				(sizeof(sHYPHY20G_CTXT_HDR) + sizeof(sHYPHY20G_CBRI_CTXT));
        rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                            HYPHY_CBRI_CTXT_ID, slice);
        HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
            "%s", "pCtxtBuf is NULL.");  
        offset += sizeof(sHYPHY20G_CTXT_HDR);

        if (pCbriCtxt != NULL) {
	        /* extract the first level context of CBRI# subsystem */
	        /* copy the pure payload into the buffer */
	        sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
	                        (void *)pCbriCtxt, sizeof(sHYPHY20G_CBRI_CTXT));
	        offset += sizeof(sHYPHY20G_CBRI_CTXT);


	        DBG_PRINT("%s, %s, %d, slice %ld 2nd SCHD offset 0x%lx\n", 
	                    __FILE__, __FUNCTION__, __LINE__, slice, offset);

	        /* extract the second level context of SCHD */
	        len = sizeof(sHYPHY20G_CTXT_HDR) +  
	            sizeof(sHYPHY20G_SCHD_CTXT);

	        /* insert ctxt buffer header */
	        rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
	                                    HYPHY_SCHD_CTXT_ID, HYPHY_NO_INDEX);
	        HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
	            "%s", "pCtxtBuf is NULL.");  
	        offset += sizeof(sHYPHY20G_CTXT_HDR);

	        /* copy the pure payload into the buffer */
	        sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
	                (void *)(pCbriCtxt->pSchdCtxt), sizeof(sHYPHY20G_SCHD_CTXT));
	        offset += sizeof(sHYPHY20G_SCHD_CTXT);


	        DBG_PRINT("%s, %s, %d, slice %ld 2nd rdChnl offset 0x%lx\n", 
	                    __FILE__, __FUNCTION__, __LINE__, slice, offset);

	        /* extract the second level context of CBRI rdChnlCtxt */
	        for (i = 0; i < pCbriCtxt->maxChnl; i++) {
	            len = sizeof(sHYPHY20G_CTXT_HDR) +  
	                sizeof(sHYPHY20G_CBRI_RD_CHNL_CTXT);

	            /* insert ctxt buffer header */
	            rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
	                                        HYPHY_CBRI_RD_CHNL_CTXT_ID, i);
	            HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
	                "%s", "pCtxtBuf is NULL.");  
	            offset += sizeof(sHYPHY20G_CTXT_HDR);

	            /* copy the payload into the buffer */
	            sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
	                (void *)&(pCbriCtxt->pChnlCtxt[i]), sizeof(sHYPHY20G_CBRI_RD_CHNL_CTXT));
	            offset += sizeof(sHYPHY20G_CBRI_RD_CHNL_CTXT);
	        }

	        DBG_PRINT("%s, %s, %d, slice %ld 3rd SCBS offset 0x%lx\n", 
	                    __FILE__, __FUNCTION__, __LINE__, slice, offset);

	        /* extract the third level context of SCBS */
	        /* extract pCbriCtxt->pSchdCtxt->pScbsGrpCtxt, 
	        pCbriCtxt->pSchdCtxt->pOnLineCalMem, 
	        and pCbriCtxt->pSchdCtxt->pOffLineCalMem context */
	        result = utilHyPhy20gScbsExtractSchdCtxt(pCbriCtxt->pSchdCtxt, 
	                                    ((UINT1 *)pCtxtBuf + offset), &len);
	        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), result,
	                        "%s", "utilHyPhy20gScbsExtractSchdCtxt failed.");
	        offset += len;
		}
        DBG_PRINT("%s, %s, %d, slice %ld offset end 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, offset);
    }

    *pExtractedSize = offset;
    
    return HYPHY20G_SUCCESS;
}/* cbriHyPhy20gCbriExtractCtxt */

/*******************************************************************************
**
**  cbriHyPhy20gCbriRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the CBRI #0 and #1 context into the driver
**                  context memory.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-stored CBRI #0 and #1 context for restoration
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
INT4 cbriHyPhy20gCbriRestoreCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_CBRI_CTXT *pCbriCtxt;
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    INT4 result;
    UINT4 offset = 0, len = 0, slice = 0, i = 0;
    
    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pParsedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pParsedSize is NULL.");

    /* try to retrieve the CBRI#0 header in the pre-stored context buffer */
    pCtxtBufHdr = utilHyPhy20gSearchCtxtHdr(pCtxtBuf, 
                                HYPHY_CBRI_CTXT_ID, 0);
    if (pCtxtBufHdr == NULL) {
        DBG_PRINT("%s, %s, %d, can not find HYPHY_CBRI_CTXT_ID hdr\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    pCtxtBuf = (void *)pCtxtBufHdr;
    
    /* restore each slice context */
    for (slice = 0; slice < HYPHY20G_OPI_CBRI_NUM_SLICE; slice++) {
        DBG_PRINT("%s, %s, %d, slice %ld 1st offset 0x%lx 0x%p\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, offset, pCtxtBuf);

        /* check if the payload is valid */
        if (pCtxtBufHdr->len == sizeof(sHYPHY20G_CTXT_HDR)) {
            offset += sizeof(sHYPHY20G_CTXT_HDR);
        } else if (pCtxtBufHdr->len > sizeof(sHYPHY20G_CTXT_HDR)) {
            /* create CBRI context */
            result = hyPhy20gCbriCreateCtxt(fileHndl, slice);
            if (result) {
                DBG_PRINT("%s, %s, %d, CbriCreateCtxt failed for slice %ld\n", 
                            __FILE__, __FUNCTION__, __LINE__, slice);
                *pParsedSize = 0;
                return result;
            }

            /* retrieve device context */
            pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
            /*no failed: confirm device context pointer is valid before processing */
            HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                            "%s", "pDevCtxt is NULL.");

            pCbriCtxt = pDevCtxt->pCbriCtxt[slice];

            /* restore the first level context for CBRI# subsystem */
            /* offset pCtxtBuf to figure out the payload position */
            offset += sizeof(sHYPHY20G_CTXT_HDR);
            
            /* not copy the unuseful pointer value from the pre-stored data */
            len = offsetof(sHYPHY20G_CBRI_CTXT, pSchdCtxt);
            sysHyPhy20gMemCpy((void *)(pCbriCtxt), 
                            ((UINT1 *)pCtxtBuf + offset), len);

            /* finish restore the first level context for CBRI subsystem */
            len = sizeof(sHYPHY20G_CBRI_CTXT);
            offset += len;

            DBG_PRINT("%s, %s, %d, slice %ld 2nd SCHD offset 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, slice, offset);

            /* restore the second level context of SCHD */
            /* try to retrieve the SCHD header in the pre-stored context buffer */
            pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
            if (pCtxtBufHdr->id != HYPHY_SCHD_CTXT_ID) {
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

            /* payload len */
            len = pCtxtBufHdr->len - 
                    sizeof(sHYPHY20G_CTXT_HDR);

            sysHyPhy20gMemCpy((void *)pCbriCtxt->pSchdCtxt, 
                            ((UINT1 *)pCtxtBuf + offset), len);
            offset += len;


            DBG_PRINT("%s, %s, %d, slice %ld 2nd rdChnl offset 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, slice, offset);

            /* restore the second level context of CBRI# rdChnlCtxt */
            for (i = 0; i < pCbriCtxt->maxChnl; i++) {
                /* try to retrieve the pChnlCtxt header in the pre-stored context buffer */
                pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
                if (pCtxtBufHdr->id != HYPHY_CBRI_RD_CHNL_CTXT_ID) {
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

                sysHyPhy20gMemCpy((void *)&(pCbriCtxt->pChnlCtxt[i]), 
                    ((UINT1 *)pCtxtBuf + offset), sizeof(sHYPHY20G_CBRI_RD_CHNL_CTXT));

                /* offset to next ctxt buffer header */
                offset += sizeof(sHYPHY20G_CBRI_RD_CHNL_CTXT);
            }

            DBG_PRINT("%s, %s, %d, slice %ld 3rd SCBS offset 0x%lx\n", 
                        __FILE__, __FUNCTION__, __LINE__, slice, offset);

            /* restore the third level context of SCBS */
            /* restore pCbriCtxt->pSchdCtxt->pScbsGrpCtxt, 
            pCbriCtxt->pSchdCtxt->pOnLineCalMem, 
            and pCbriCtxt->pSchdCtxt->pOffLineCalMem context */
            result = utilHyPhy20gScbsRestoreSchdCtxt(pCbriCtxt->pSchdCtxt, 
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
}/* cbriHyPhy20gCbriRestoreCtxt */

/*******************************************************************************
**
**  cbriHyPhy20gCbriAuditRdChnlCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit the RdChnl data member in CBRI subCtxt 
**
**                  PRIVATE function called by the cbriHyPhy20gCbriAuditCtxt
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
PRIVATE INT4 cbriHyPhy20gCbriAuditRdChnlCtxt(struct file *fileHndl, UINT4 slice)
{
    sHYPHY20G_CBRI_RD_CHNL_CTXT *pChnlCtxt;
    sHYPHY20G_CBRI_CTXT  *pCbriCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 i;
    UINT4 numZeroRate = 0;
    UINT4 numOdu1Rate = 0;
    UINT4 numOdu2Rate = 0;


    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    
    
    pCbriCtxt = pDevCtxt->pCbriCtxt[slice];
    if (pCbriCtxt==NULL)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    /* audit *pChnlCtxt */
    for (i=0;i<HYPHY20G_OPI_SCHD_MAX_CHNL;i++)
    {
        pChnlCtxt=pCbriCtxt->pChnlCtxt + i;
        /*audit according to hyPhy20gCbriCreateCtxt*/
        if (pChnlCtxt->valid != HYPHY20G_VALID_SIGN)
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }

        switch (pChnlCtxt->rate) 
        {
            case HYPHY20G_CBRI_NO_RATE:
                numZeroRate++;
                break;
            case HYPHY20G_CBRI_ODU1_RATE:
                numOdu1Rate++;
                break;
            case HYPHY20G_CBRI_ODU2_RATE:
                numOdu2Rate++;
                break;
            default:
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
        }

    }
    
    /* audit according to cbrbHyPhy20gCbriUpdateSchd */
    if(((numOdu1Rate + 4*numOdu2Rate) > 8)||
       (numOdu2Rate > 2)||(numOdu1Rate > 8))
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_FAILURE;
    }

    return HYPHY20G_SUCCESS;
}/* cbriHyPhy20gCbriAuditRdChnlCtxt */

/*******************************************************************************
**
**  cbriHyPhy20gCbriAuditCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function audit CBRI subCtxt 
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
INT4 cbriHyPhy20gCbriAuditCtxt(struct file *fileHndl)
{
    sHYPHY20G_CBRI_CTXT  *pCbriCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 slice=0,lpOff,lowPower;
    UINT4 devId, revId, status;
    INT4 result;

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

    /* audit pCbriCtxt first level attributes according to hyPhy20gCbriIntfInit,
    hyPhy20gCbriCreateCtxt and hyPhy20gCbriDestroyCtxt functions */
    for (slice =0; slice<2; slice++)
    {
        pCbriCtxt = pDevCtxt->pCbriCtxt[slice];
        lpOff = (slice == 0) ? HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OPI_CBRI_LOWPWR_0_OFF
                             : HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OPI_CBRI_LOWPWR_1_OFF;
        result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                             lpOff, &lowPower);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 

        /* the CBRI slice disabled */
        if (lowPower == 1)
        {
            if (pCbriCtxt!=NULL)
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
            else
                continue;
        }
        /* the CBRI slice enabled */
        if (pCbriCtxt==NULL)
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
        
        if (((slice == 0)&&(pCbriCtxt->baseChnl != HYPHY20G_OPI_CBRI1_CHNL_OFFSET))||
            ((slice == 1)&&(pCbriCtxt->baseChnl != HYPHY20G_OPI_CBRI2_CHNL_OFFSET))||
            (pCbriCtxt->pSchdCtxt==NULL)||(pCbriCtxt->pChnlCtxt==NULL))
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }

        /* audit *pSchdCtxt member */
        result = utilHyPhy20gScbsAuditSchdCtxt(fileHndl,pCbriCtxt->pSchdCtxt);
        if (result != HYPHY20G_SUCCESS)
        {
            return result;
        }
        DBG_PRINT("%s, %s, %d, %uth slice utilHyPhy20gScbsAuditSchdCtxt(fileHndl,pCbriCtxt->pSchdCtxt) SUCCESS in cbriHyPhy20gCbriAuditCtxt.\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)slice);

        /* audit *pChnlCtxt member*/
        result=cbriHyPhy20gCbriAuditRdChnlCtxt(fileHndl, slice);
        if (result != HYPHY20G_SUCCESS) 
        {
            return result;
        }
        DBG_PRINT("%s, %s, %d, %uth slice cbriHyPhy20gCbriAuditRdChnlCtxt(fileHndl, slice) SUCCESS in cbriHyPhy20gCbriAuditCtxt.\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)slice);

    }

    return HYPHY20G_SUCCESS;
} /* cbriHyPhy20gCbriAuditCtxt */

/* end of file */
