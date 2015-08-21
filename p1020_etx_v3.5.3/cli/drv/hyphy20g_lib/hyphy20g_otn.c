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
**  FILE        :
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
#include "hyphy20g_otn.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
extern INT4 hyPhy20gOtnOhRamCreateCtxt(struct file *fileHndl);
INT4 otnHyPhy20gEfecDeviceInit(struct file *fileHndl, UINT4 enable);
INT4 otnHyPhy20gJatInit(struct file *fileHndl, UINT4 slice, UINT4 mode);
INT4 otnHyPhy20gEfecInit(struct file *fileHndl, UINT4 slice, UINT4 mode);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gOtu2DeviceInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the OTU2 and ODTU12 framers for
**                  slice 0 and 1 to be in low power - power savings mode or out
**                  of low power mode and ready for individual configuration.
**
**                  In addition the block provides initialization for the I.4
**                  and I.7 FEC blocks for each slice.  When enabling It ensures
**                  that they are out of device level low power mode, reset to
**                  default values and that block level power savings for each
**                  slice are enabled.  When disabling it ensures that they are
**                  in low power mode at the device level.
**
**                  This function impacts both slice 0 and slice 1.  It is the
**                  caller's responsibility to ensure that this function is not
**                  called with enable = 0 while a slice is intended to be
**                  processing user data.
**
**                  hyPhy20gHostMsgInit must be called prior to calling this
**                  function.
**
**                  This function must be called prior to calling either
**                  hyPhy20gOtu2Init or hyPhy20gOdtu12Init to enable a resource
**                  for a desired mode.
**
**  INPUTS:         *fileHndl  - base address
**                  enable     - Operation Mode of OTU2 Slices 0 and 1
**                               0 - In low power modes
**                               1 - Out of low power mode and reset to
**                                   default values
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**
*******************************************************************************/
INT4 hyPhy20gOtu2DeviceInit(struct file *fileHndl, UINT4 enable)
{
    INT4 result;
    UINT4 addr, msk, val;
    
    /* init context memory */
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt or pDevCtxt->pCpbCtxt is NULL.");

    /* set lowpwr bit to 1 */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                             HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OTN_OTU2_LOWPWR_OFF,
                             1);
    if(result) return result;

    /* reset at top level */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_2_OTN_OTU2_RST_OFF,
                                 1);
    if(result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_2_OTN_OTU2_RST_OFF,
                                 0);
    if(result) return result;

    /* terminate function if disabling subsystem */
    if(enable == 0){
        /* place I.4 and I.7 FEC in device level low power mode */
        result = otnHyPhy20gEfecDeviceInit(fileHndl, 0);
        if(result) return result;

        return HYPHY20G_SUCCESS;
    }

    /* remove lowpwr if subsystem to be enabled */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                             HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OTN_OTU2_LOWPWR_OFF,
                             0);
    if(result) return result;

    /* place I.4 and I.7 FEC in block level out of low power mode so that these
       resources are available for each slice */
    result = otnHyPhy20gEfecDeviceInit(fileHndl, 1);
    if(result) return result;

    /* perform FW initialization of OTU2 Framer Subsystem */
    result = hostMsgHyPhy20gHostMsgCmdCfgOtnSetInitState(fileHndl);
    //if(result) return result;

    /* apply non-default settings for ZONE_CHK_EN and ZONE_CHK_PERIOD*/
    addr = HYPHY20G_OTU2_TOP_REG_SYS_SIDE_DAT_SEL;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    msk = HYPHY20G_OTU2_TOP_BIT_SYS_SIDE_DAT_SEL_ZONE_CHK_EN_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 0x0);
    if(result) return result;

    msk = HYPHY20G_OTU2_TOP_BIT_SYS_SIDE_DAT_SEL_ZONE_CHK_PERIOD_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 0x0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;
        
    /* Create context to store OTN OH RAM resource */
    result = hyPhy20gOtnOhRamCreateCtxt(fileHndl);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtu2DeviceInit */


/*******************************************************************************
**
**  hyPhy20gOtu2Init
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the major operational mode of an
**                  OTU2 Framer resource.  For OTU2 Frames with I.4 or I.7 FEC,
**                  the EFEC block will also be configured.
**      
**                  The line side subsystem connected to the OTU2 framer must be
**                  configured before this function is called otherwise it will 
**                  return a HYPHY20G_ERR_INVALID_PRECONFIG error.
**
**                  The major operational modes are:
**                     OTU2 Line Side Frame with no FEC
**                     OTU2 Line Side Frame with G.709 FEC
**                     OTU2 Line Side Frame with I.4 FEC
**                     OTU2 Line Side Frame with I.7 FEC
**                     ODU2 Line Side Frames
**                     Disabled
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTU2 Framer instance:
**                               0 - OTU2 Framer 0
**                               1 - OTU2 Framer 1
**                  mode       - Operation Mode of OTU2 Framer slice
**                               Modes 0 to 4 apply reset to the OTU2 framer
**                               resetting all configuration associated with 
**                               the OTU2 Framer.
**                               Modes 6 to 10 do not apply reset to the OTU2
**                               framer allowing change of the line frame type
**                               without impacting other framer configuration.
**                               0 - OTU2 Line Side Frame with no FEC (reset)
**                               1 - OTU2 Line Side Frame with G.709 FEC (reset)
**                               2 - OTU2 Line Side Frame with I.4 FEC (reset)
**                               3 - OTU2 Line Side Frame with I.7 FEC (reset)
**                               4 - ODU2 Line Side Frame (reset)
**                               5 - Disabled
**                               6 - OTU2 Line Side Frame with no FEC (no reset)
**                               7 - OTU2 Line Side Frame with G.709 FEC
**                                   (no reset)
**                               8 - OTU2 Line Side Frame with I.4 FEC(no reset)
**                               9 - OTU2 Line Side Frame with I.7 FEC(no reset)
**                               10 - ODU2 Line Side Frame (no reset)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtu2Init(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    INT4 result;
    UINT4 addr, val, msk;
    UINT4 swRstOff, sFecSelOff;
    UINT4 fasOff;
    UINT4 devId, revId, status; 
    UINT4 otuFrmType  = 0; /* 0: ODU frames, 1: OTU frames */ 
    UINT4 fecBlanking = 0;
    UINT4 externalFec = 0;
    UINT4 rxFecEn     = 0;
    UINT4 txExtFec    = 0;
    UINT4 rxExtFec    = 0;
    UINT4 txScrEn     = 1;
    UINT4 rxScrEn     = 1;
    UINT4 scrPreFec   = 0;
    UINT4 scrPostFec  = 0;
    UINT4 otu2FailEn  = 0;
    UINT4 sFecSelVal  = 0; /* 0: I.4 FEC Mode, 1: I.7 FEC Mode */
    UINT4 efecInitVal = 2; /* FEC algorithm disabled */   
    
    /* argument checking */
    if (slice > 1 || mode > 10) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* evaluation of compatibility between SW driver and device */
    result = hyPhy20gCheckRev(fileHndl, &devId, &revId, &status);
    if(result) return result;
        
    swRstOff = HYPHY20G_OTU2_TOP_BIT_CFG_SW_RST_OTU2_FRAMER_OFF + slice;
    sFecSelOff = HYPHY20G_OTU2_TOP_BIT_CFG_STRONG_FEC_SEL_OFF + slice;

    addr = HYPHY20G_OTU2_TOP_REG_CFG;
    /* disable operation */
    if (mode == 5) {
        /* hold OTU2_FRAMER block in reset */
        result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 1);
        if (result) return result;
        return HYPHY20G_SUCCESS;
    }

    /* ensure OTU2_FRAMER block has been reset */
    if (mode < 5) {
        /* apply reset only if specified by mode */
        result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 1);
        if (result) return result;
    }
    result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 0);
    if (result) return result;
    
    switch(mode)
    {
        case 0: /* OTU2 Line Side Frame with no FEC (reset) */
        case 6: /* OTU2 Line Side Frame with no FEC (no reset) */
            otuFrmType  = 1;
            fecBlanking = 1;
            otu2FailEn  = 1;
            break;
        case 1: /* OTU2 Line Side Frame with G.709 FEC (reset) */
        case 7: /* OTU2 Line Side Frame with G.709 FEC (no reset) */
            otuFrmType  = 1;
            rxFecEn     = 1;
            otu2FailEn  = 1;
            break;
        case 2: /* OTU2 Line Side Frame with I.4 FEC (reset) */
        case 8: /* OTU2 Line Side Frame with I.4 FEC (no reset) */
            otuFrmType  = 1;
            externalFec = 1;
            rxFecEn     = 1;
            txExtFec    = 1;
            rxExtFec    = 1;
            otu2FailEn  = 1;
            sFecSelVal  = 0; /* I.4 FEC Mode */
            efecInitVal = 0; /* I.4 FEC Mode */
            break;
        case 3: /* OTU2 Line Side Frame with I.7 FEC (reset) */
        case 9: /* OTU2 Line Side Frame with I.7 FEC (no reset) */
            otuFrmType  = 1;
            externalFec = 1;
            rxFecEn     = 1;
            txExtFec    = 1;
            rxExtFec    = 1;
            txScrEn     = 0;
            rxScrEn     = 0;
            scrPreFec   = 1;
            scrPostFec  = 1;
            otu2FailEn  = 1;
            sFecSelVal  = 1; /* I.7 FEC Mode */
            efecInitVal = 1; /* I.7 FEC Mode */
            break;
        case 4: /* ODU2 Line Side Frame (reset) */
        case 10: /* ODU2 Line Side Frame (no reset) */
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /*
     * configure the Tx OTU2 framer
     */

    /* for Rev C and Rev D devices configure OTU_FAIL_ENABLE */
    if (revId == HYPHY20G_REV_ID_C || revId == HYPHY20G_REV_ID_D) {
        addr = HYPHY20G_OTU2_TOP_REG_GEN_PURP;
        msk = (1 << HYPHY20G_OTU2_TOP_BIT_GEN_PURP_OTU_FAIL_ENABLE_OFF) << slice;
        
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, otu2FailEn);
        if (result) return result;
    }
        
    /* configure TX_SCRAMBLE_ENABLE, SCRAMBLE_PRE_FEC, OTU_FRAME_TYPE,
       FEC_BLANKING and TX_EXTERNAL_FEC */
    addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_GLBL_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_TX_SCRAMBLE_ENABLE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, txScrEn);
    if(result) return result;

    msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_SCRAMBLE_PRE_FEC_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, scrPreFec);
    if(result) return result;

    msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_OTU_FRAME_TYPE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, otuFrmType);
    if(result) return result;

    msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_FEC_BLANKING_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, fecBlanking);
    if(result) return result;

    msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_TX_EXTERNAL_FEC_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, txExtFec);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /*
     * configure the Rx OTU2 framer
     */

    /* configure RX_SCRAMBLE_ENABLE, SCRAMBLE_POST_FEC, OTUK_FRAME_TYPE,
       FEC_ENABLE and RX_EXTERNAL_FEC */
    addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    msk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_SCRAMBLE_ENABLE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, rxScrEn);
    if(result) return result;

    msk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_SCRAMBLE_POST_FEC_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, scrPostFec);
    if(result) return result;

    msk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_OTUK_FRAME_TYPE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, otuFrmType);
    if(result) return result;

    msk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_EXTERNAL_FEC_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, rxExtFec);
    if(result) return result;

    msk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_FEC_ENABLE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, rxFecEn);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* Strong FEC specific configuration */    
    /* Set STRONG_FEC_SEL to default value */
    addr = HYPHY20G_OTU2_TOP_REG_CFG;
    result = sysHyPhy20gBitWrite(fileHndl, addr, sFecSelOff, sFecSelVal);
    if (result) return result;
    /* hold external FEC blocks in reset */
    result = otnHyPhy20gEfecInit(fileHndl, slice, efecInitVal);
    if (result) return result;    
        
    /* apply non-default settings to block */
    /* configure RX_FIFO_HIGH_THRESHOLD_VALUE to 0x5e (94d) */
    addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_OPU_FIFO_HI_TH;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk =
   HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_FIFO_HI_TH_RX_FIFO_HIGH_THRESHOLD_VALUE_MSK;
    val = 0x5e;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure RX_FIFO_LOW_THRESHOLD_VALUE to 0x28 (40d) */
    addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_OPU_FIFO_LOW_TH;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk =
   HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_FIFO_LOW_TH_RX_FIFO_LOW_THRESHOLD_VALUE_MSK;
    val = 0x28;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure TX_ERR_SYNC_MODE to 1 */
    addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_CNT_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_CNT_CTL_TX_ERR_SYNC_MODE_MSK;
    val = 0x1;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure TX_FIFO_HIGH_THRESHOLD_VALUE to 0x60 (96d) */
    addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_OPU_FIFO_HI_TH;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk =
   HYPHY20G_OTU2_FRM_BIT_TX_OTN_OPU_FIFO_HI_TH_TX_FIFO_HIGH_THRESHOLD_VALUE_MSK;
    val = 0x60;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure RX_ERR_SYNC_MODE to 1 */
    addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
    val = 0x1;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* set OTU2_OVERWRITE_FAS[n] to 1 and OTU2_OVERWRITE_MFAS[n] to 0 */
    addr = HYPHY20G_OTU2_TOP_REG_RX_OVERWRITE_FAS_MFAS;
    fasOff = HYPHY20G_OTU2_TOP_BIT_RX_OVERWRITE_FAS_MFAS_OTU2_OVERWRITE_FAS_OFF;
    
    msk = 1 << slice << fasOff;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x1);
    if (result) return result;
        
    /* set AUTO_OTUK_AIS_IAE to 0 */
    addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_AUTO_AIS_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_OTUK_AIS_IAE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0);
    if (result) return result;
    
    /* check if the write was actually performed */
    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &val);
    if (result) return result;
    
    /* raise an INVALID_PRECONFIG error if the register write test fails */
    if (val != 0) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG: Line side clock source not present\n"
                  ,__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    
    /* set AUTO_ODUK_AIS_FIFO_ERROR to 1 */ 
    msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_ODUK_AIS_FIFO_ERROR_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if (result) return result;

    /* set MIN_AUTO_AIS_INSERT_MULTIFRAME_COUNT to 1 */
    msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_AUTO_AIS_CTL_MIN_AUTO_AIS_INSERT_MULTIFRAME_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if (result) return result;
        
    /* set TX_IAE_MULTIFRAME_COUNT to 16 to comply to G.798 */
    addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_ODU_TCMI_AUTO_BDI_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_IAE_MULTIFRAME_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x10);
    if (result) return result;
    
    /* set RX_IAE_MULTIFRAME_COUNT to 16 to comply to G.798 */
    addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_IAE_MULTIFRAME_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x10);
    if (result) return result;
            
    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtu2Init */


/*******************************************************************************
**
**  hyPhy20gOdtu12Init
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the major operational mode of an
**                ODTU12 Multiplexer resource.
**
**                The major operational modes are:
**                   ODTU12 associated with a lineside OTU2
**                   ODTU12 associated with a lineside ODU2
**                   Disabled
**
**  INPUTS:       *fileHndl  - base address
**                slice      - ODTU12 Multiplexer instance:
**                             0 - ODTU12 Framer 0
**                             1 - ODTU12 Framer 1
**                mode       - Operation Mode of ODTU12 Multiplexer slice
**                             0 - ODTU12 associated with a lineside OTU2
**                             1 - ODTU12 associated with a lineside ODU2
**                             2 - Disabled
**
**  OUTPUTS:      None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdtu12Init(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    /* variable declaration */
    UINT4 devId, revId, status;
    INT4 result;
    UINT4 addr, msk, val;
    UINT4 swRstOff;
    

    /* argument checking */
    if (slice > 1 || mode > 2 ) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    swRstOff = HYPHY20G_OTU2_TOP_BIT_CFG_SW_RST_ODTU12_MUX_OFF + slice;

    /* disable operation */
    if (mode == 2) {
        /* hold ODTU12_MUX block in reset */
        addr = HYPHY20G_OTU2_TOP_REG_CFG;
        result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 1);
        if (result) return result;
        return HYPHY20G_SUCCESS;
    }

    /*
     * enable operation
     */
    /* ensure ODTU12_MUX block has been reset */
    addr = HYPHY20G_OTU2_TOP_REG_CFG;
    result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 1);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 0);
    if (result) return result;


    /*  Configure Register 4800804H: TX MX GLOBAL CONTROL */
    addr = HYPHY20G_OTU2_ODTU12_REG_TX_MX_GLBL_CTL;
    addr = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* configure line side ODU2/OTU2 frame type TX_OTU_FRAME_TYPE */
    msk = HYPHY20G_OTU2_ODTU12_BIT_TX_MX_GLBL_CTL_TX_OTU_FRAME_TYPE_MSK;

    result = hyPhy20gVarFieldWrite (&val, msk, 0x1);
    if(result) return result;

    /* apply non-default settings */
    /* set TX_ERR_SYNC_MODE to 1 */
    msk = HYPHY20G_OTU2_ODTU12_BIT_TX_MX_GLBL_CTL_TX_ERR_SYNC_MODE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 0x1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* Configure Register 4800A00H: RX MX GLOBAL CONTROL */
    /* apply non-default settings */
    addr = HYPHY20G_OTU2_ODTU12_REG_RX_MX_GLBL_CTL;
    addr = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(addr, slice);
    
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;
    
    /* configure line side ODU2/OTU2 frame type RX_OTU_FRAME_TYPE to 0 */
    msk = HYPHY20G_OTU2_ODTU12_BIT_RX_MX_GLBL_CTL_RX_OTU_FRAME_TYPE_MSK;
    
    result = hyPhy20gVarFieldWrite (&val, msk, 0x0);
    if(result) return result;
    
    /* set RX_ERR_SYNC_MODE to 1 */
    msk = HYPHY20G_OTU2_ODTU12_BIT_RX_MX_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
    result = hyPhy20gVarFieldWrite (&val, msk, 0x1);
    if(result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* configure TX_FIFO_LOW_THESHOLD_VALUE to value of 17 for Rev B devices */
    result = hyPhy20gCheckRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (revId == HYPHY20G_REV_ID_B) {
        addr = HYPHY20G_OTU2_ODTU12_REG_TX_MX_FIFO_LOW_TH;
        addr = HYPHY20G_OTU2_ODTU12_TSB_SLICE_REG_OFFSET(addr, slice);
        msk = HYPHY20G_OTU2_ODTU12_BIT_TX_MX_FIFO_LOW_TH_TX_FIFO_LOW_THRESHOLD_VALUE_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x11);
        if(result) return result;        
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdtu12Init */

/*******************************************************************************
**
**  hyPhy20gOtu1DeviceInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the OTU1 framers for
**                  slice 0 to 7 to be in low power - power savings mode or out
**                  of low power mode and ready for individual configuration.
**
**                  This function impacts all slices 0 to 7.  It is the
**                  caller's responsibility to ensure that this function is not
**                  called with enable = 0 while a slice is intended to be
**                  processing user data.
**
**                  This function must be called prior to calling either
**                  hyPhy20gOtu1Init to enable a resource for a desired mode.
**
**  INPUTS:         *fileHndl  - base address
**                  enable     - Operation Mode of OTU1 Slices 0 and 1
**                               0 - In low power modes
**                               1 - Out of low power mode and reset to
**                                   default values
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtu1DeviceInit(struct file *fileHndl, UINT4 enable)
{
    INT4 result;

    /* argument checking */
    if (enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* set lowpwr bit to 1 */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                             HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OTN_OTU1_LOWPWR_OFF,
                             1);
    if(result) return result;

    /* reset at top level */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_2_OTN_OTU1_RST_OFF,
                                 1);
    if(result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_2_OTN_OTU1_RST_OFF,
                                 0);
    if(result) return result;

    /* terminate function if disabling subsystem */
    if(enable == 0){
        return HYPHY20G_SUCCESS;
    }

    /* remove lowpwr if subsystem to be enabled */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                             HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OTN_OTU1_LOWPWR_OFF,
                             0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtu1DeviceInit */

/*******************************************************************************
**
**  hyPhy20gOtu1Init
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures an OTU1 Framer Slice for the
**                  line side signal type.
**
**                  Mode 0 to 2 and 6 to 8 are associated with line side SFP 
**                  via SONET_LINE.
**                  Mode 3, 4, 9, and 10 are associated with line side ODTU12
**                  operation.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTU1 Framer instance:
**                               0 to 7 OTU1 Framer 1 to 8
**                  mode       - Operation Mode of OTU1 Framer slice
**                               Modes 0 to 4 apply reset to the OTU1 framer 
**                               resetting all configuration associated with the
**                               OTU1 Framer
**                               Modes 6 to 10 do not apply reset to the OTU1
**                               framer allowing change of the line frame type
**                               without impacting other framer configuration.
**                               0 - OTU1 Line Side Frame with no FEC (reset)
**                               1 - OTU1 Line Side Frame with G.709 FEC (reset)
**                               2 - ODU1 Line Side Frame (reset)
**                               3 - ODU1 Line Side to ODTU12 MX / OTU2 #0
**                                   (reset)
**                               4 - ODU1 Line Side to ODTU12 MX / OTU2 #1
**                                   (reset)
**                               5 - Disabled
**                               6 - OTU1 Line Side Frame with no FEC (no reset)
**                               7 - OTU1 Line Side Frame with G.709 FEC 
**                                   (no reset)
**                               8 - ODU1 Line Side Frame (no reset)
**                               9 - ODU1 Line Side to ODTU12 MX / OTU2 #0
**                                   (no reset)
**                               10 - ODU1 Line Side to ODTU12 MX / OTU2 #1
**                                   (no reset)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtu1Init(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    INT4 result;
    UINT4 addr, val, msk;
    UINT4 swRstOff;
    UINT4 txClkCfgMsk, rxClkCfgMsk;
    UINT4 fasOff;
    UINT4 devId, revId, status;
    UINT4 otuFrmType  = 0;
    UINT4 fecBlanking = 0;
    UINT4 rxFecEn     = 0;
    UINT4 txClkCfg    = 0;
    UINT4 rxClkCfg    = 0;
    UINT4 otuFailEn   = 0;

    /* argument checking */
    if (slice > 7 || mode > 10) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* evaluation of compatibility between SW driver and device */
    result = hyPhy20gCheckRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    addr = HYPHY20G_OTU1_TOP_REG_CFG;
    swRstOff = HYPHY20G_OTU1_TOP_BIT_CFG_SW_RST_OTU1_FRAMER_OFF + slice;
    /* disable operation */
    if (mode == 5) {
        /* hold OTU1_FRAMER block in reset */
        result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 1);
        if (result) return result;
        return HYPHY20G_SUCCESS;
    }

    /*
     * enable operation
     */
    /* ensure OTU1_FRAMER block is reset */
    if (mode < 5) { 
        /* apply reset only if specified by mode */
        result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 1);
        if (result) return result;
    }
    result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 0);
    if (result) return result;

    /*
     * configure an OTU1 Framer Slice
     */
    switch(mode)
    {
        case 0: /* OTU1 Line Side Frame with no FEC (reset) */
        case 6: /* OTU1 Line Side Frame with no FEC (no reset) */
            otuFrmType  = 1;
            otuFailEn   = 1;
            fecBlanking = 1;
            break;
        case 1: /* OTU1 Line Side Frame with G.709 FEC (reset) */
        case 7: /* OTU1 Line Side Frame with G.709 FEC (no reset) */
            otuFrmType  = 1;
            rxFecEn     = 1;
            otuFailEn   = 1;
            break;
        case 2: /* ODU1 Line Side Frame (reset) */
        case 8: /* ODU1 Line Side Frame (no reset) */
            fecBlanking = 1;
            break;
        case 3: /* ODU1 Line Side to ODTU12 MX / OTU2 #0 (reset) */
        case 9: /* ODU1 Line Side to ODTU12 MX / OTU2 #0 (no reset) */
            txClkCfg    = 1;
            rxClkCfg    = 1;
            break;
        case 4:  /* ODU1 Line Side to ODTU12 MX / OTU2 #1 (reset) */
        case 10: /* ODU1 Line Side to ODTU12 MX / OTU2 #1 (no reset) */
            txClkCfg    = 2;
            rxClkCfg    = 2;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* identify line clock configuration register addresses */
    switch(slice)
    {
        case 0:
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG0_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG0_MSK;
            break;

        case 1:
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG1_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG1_MSK;
            break;

        case 2:
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG2_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG2_MSK;
            break;

        case 3:
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG3_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG3_MSK;
            break;

        case 4:
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG4_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG4_MSK;
            break;

        case 5:
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG5_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG5_MSK;
            break;

        case 6:
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG6_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG6_MSK;
            break;

        case 7:
             txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG7_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG7_MSK;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* for Rev C and Rev D devices configure OTU_FAIL_ENABLE field */
    if (revId == HYPHY20G_REV_ID_C || revId == HYPHY20G_REV_ID_D) {
        addr = HYPHY20G_OTU1_TOP_REG_GEN_PURP;
        msk = (1 << HYPHY20G_OTU1_TOP_BIT_GEN_PURP_OTU_FAIL_ENABLE_OFF) << slice; 
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, otuFailEn);
        if (result) return result;
    }
     
    /* configure line side clock source so that OTU1 framer registers may
       be written */
    /* Set the OTU1_TX_LINE_CLK_CFGn[1:0] */
    addr = HYPHY20G_OTU1_TOP_REG_CLK_CONFIGURATION0;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, txClkCfgMsk, txClkCfg);
    if (result) return result;

    /* Set the OTU1_RX_LINE_CLK_CFGn[1:0] */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, rxClkCfgMsk, rxClkCfg);
    if (result) return result;

    /* Set the OTU2 Level TX_LINK_CLK_CFGn[1:0] */
    addr = HYPHY20G_OTU2_TOP_REG_OTU1_CLK_CFG;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, txClkCfgMsk, txClkCfg);
    if (result) return result;

    /* Set the OTU2 Level RX_LINK_CLK_CFGn[1:0] */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, rxClkCfgMsk, rxClkCfg);
    if (result) return result;

    /*
     * configure the Tx OTU1 framer
     */
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                HYPHY20G_OTU1_FRM_REG_TX_OTN_GLBL_CTL, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result != HYPHY20G_SUCCESS) return result;

    val = mHYPHY20G_SET_FIELD4(val, otuFrmType,
            HYPHY20G_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_OTU_FRAME_TYPE_MSK,
            HYPHY20G_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_OTU_FRAME_TYPE_OFF);

    val = mHYPHY20G_SET_FIELD4(val, fecBlanking,
            HYPHY20G_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_FEC_BLANKING_MSK,
            HYPHY20G_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_FEC_BLANKING_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result != HYPHY20G_SUCCESS) return result;

    /*
     * configure the Rx OTU1 framer
     */
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                HYPHY20G_OTU1_FRM_REG_RX_OTN_GLBL_CTL, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result != HYPHY20G_SUCCESS) return result;

    val = mHYPHY20G_SET_FIELD4(val, otuFrmType,
            HYPHY20G_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_OTUK_FRAME_TYPE_MSK,
            HYPHY20G_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_OTUK_FRAME_TYPE_OFF);

    val = mHYPHY20G_SET_FIELD4(val, rxFecEn,
            HYPHY20G_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_FEC_ENABLE_MSK,
            HYPHY20G_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_FEC_ENABLE_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result != HYPHY20G_SUCCESS) return result;

    /* apply non-default settings to block */
    /* configure RX_FIFO_HIGH_THRESHOLD_VALUE to 0x190 (400d) */
    addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_OPU_FIFO_HI_TH;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk =
   HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_FIFO_HI_TH_RX_FIFO_HIGH_THRESHOLD_VALUE_MSK;
    val = 0x190;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure RX_FIFO_LOW_THRESHOLD_VALUE to 0xc8 (200d) */
    addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_OPU_FIFO_LOW_TH;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk =
   HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_FIFO_LOW_TH_RX_FIFO_LOW_THRESHOLD_VALUE_MSK;
    val = 0xc8;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure TX_ERR_SYNC_MODE to 1 */
    addr = HYPHY20G_OTU1_FRM_REG_TX_OTN_CNT_CTL;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_CNT_CTL_TX_ERR_SYNC_MODE_MSK;
    val = 0x1;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure RX_ERR_SYNC_MODE to 1 */
    addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_GLBL_CTL;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
    val = 0x1;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* set OTU1_OVERWRITE_FAS[n] to 1 and OTU1_OVERWRITE_MFAS[n] to 0 */
    addr = HYPHY20G_OTU1_TOP_REG_RX_OVERWRITE_FAS_MFAS;
    fasOff = HYPHY20G_OTU1_TOP_BIT_RX_OVERWRITE_FAS_MFAS_OTU1_OVERWRITE_FAS_OFF;

    msk = 1 << slice << fasOff;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x1);
    if (result) return result;
        
    /* set N_LOF_LOF_COUNT to 61 (0x3D) to ensure LOF entered 3 ms after OOF */
    addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_FA_FAU_CTL_1;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_FAU_CTL_1_N_LOF_LOF_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x3D);
    if (result) return result;

    /* set LOF_N_LOF_COUNT to 61 (0x3D) to ensure LOF entered 3 ms after OOF */
    addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_FA_FAU_CTL_2;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_FAU_CTL_2_LOF_N_LOF_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x3D);
    if (result) return result;
    
    /* set N_LOM_LOM_COUNT to 61 (0x3D) to ensure LOM entered 3 ms after OOM */
    addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_FA_MFAU_CTL_1;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_FA_MFAU_CTL_1_N_LOM_LOM_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x3D); 
    if (result) return result;
     
    /* set AUTO_OTUK_AIS_IAE to 0 */  
    addr = HYPHY20G_OTU1_FRM_REG_TX_OTN_AUTO_AIS_CTL;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_OTUK_AIS_IAE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0);
    if (result) return result;
        
    /* set AUTO_ODUK_AIS_FIFO_ERROR to 1 */
    msk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_ODUK_AIS_FIFO_ERROR_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if (result) return result;
        
    /* set MIN_AUTO_AIS_INSERT_MULTIFRAME_COUNT to 1 */
    msk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_AUTO_AIS_CTL_MIN_AUTO_AIS_INSERT_MULTIFRAME_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if (result) return result; 
    
    /* set TX_IAE_MULTIFRAME_COUNT to 16 to comply to G.798 */  
    addr = HYPHY20G_OTU1_FRM_REG_TX_OTN_ODU_TCMI_AUTO_BDI_CTL;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_IAE_MULTIFRAME_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x10);
    if (result) return result;
         
    /* set RX_IAE_MULTIFRAME_COUNT to 16 to comply to G.798 */ 
    addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_IAE_MULTIFRAME_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x10);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtu1Init */


/*******************************************************************************
**
**  hyPhy20gOtu2SysDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the bidirectional system side data
**                  path for an OTU2 Framer Slice.  Will configure the JAT_622E
**                  for CBR10G SONET system side payloads.
**
**                  If OPI subsystem is operational, hyPhy20gCbrbVcatOtnChnlCfg
**                  needs to be called with enable set to 0 for the OTN Framer
**                  slices that are used for serial system side data.
**
**                  The system side data sources are:
**                     CBRB
**                     ODTU12
**                     CBR10G from SONET subsystem
**                     CBR10G from HSCI subsystem - G.Sup43 7.1 Compliant
**                     CBR10G from HSCI subsystem - G.Sup43 7.2 Compliant
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTU2 Framer instance:
**                               0 - OTU2 Framer 0
**                               1 - OTU2 Framer 1
**                  source     - Operation Mode of OTU2 Framer slice
**                               0 - CBRB
**                               1 - ODTU12
**                               2 - CBR10G from SONET subsystem
**                               3 - CBR10G from HSCI subsystem - G.Sup43 7.1
**                                   Compliant (ODU2e or ODU2f rate signals)
**                               4 - CBR10G from HSCI subsystem - G.Sup43 7.2
**                                   Compliant (ODU1e or ODU1f rate signals)
**                               5 - CBR10G from HSCI subsystem - STS-192 rate
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOtu2SysDataPathCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 source)
{
    INT4 result;
    UINT4 addr, msk;
    UINT4 off, val;
    UINT4 dataSelMsk, txClkCfgOff, rxClkCfgOff;
    UINT4 dataSel, txClkCfg, txStrMode, txMapping, txStuffEn;
    UINT4 rxClkCfg, rxStrMode, rxMapping, rxStuffEn;
    UINT4 jatEn;
    UINT4 otuBlank, oduBlank, opuBlank;
    UINT4 insFas, insMfas;
    UINT4 txFracDivCtrlReg = 0, rxFracDivCtrlReg =0, txFracDivNumMsk = 0;
    UINT4 txFracDivDenMsk = 0, rxFracDivNumMsk = 0, rxFracDivDenMsk = 0;
    UINT4 txFracDivNumVal = 1, txFracDivDenVal = 1, rxFracDivNumVal = 1;
    UINT4 rxFracDivDenVal = 1;

    /* argument checking */
    if (slice > 1 || source > 5) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (slice == 0) {
        dataSelMsk = HYPHY20G_OTU2_TOP_BIT_SYS_SIDE_DAT_SEL_OTU2_SYS_DATA_SEL0_MSK;
        txClkCfgOff = HYPHY20G_OTU2_TOP_BIT_CLK_CFG_OTU2_TX_SYS_CLK_CFG_OFF;
        rxClkCfgOff = HYPHY20G_OTU2_TOP_BIT_CLK_CFG_OTU2_RX_SYS_CLK_CFG_OFF;
        txFracDivCtrlReg = HYPHY20G_OTU2_TOP_REG_TX_GE_FRAC_DIV_CTRL0;
        rxFracDivCtrlReg = HYPHY20G_OTU2_TOP_REG_RX_GE_FRAC_DIV_CTRL0;
        txFracDivNumMsk = HYPHY20G_OTU2_TOP_BIT_TX_GE_FRAC_DIV_CTRL0_TX_GE_FRAC_NUMER0_MSK;
        txFracDivDenMsk = HYPHY20G_OTU2_TOP_BIT_TX_GE_FRAC_DIV_CTRL0_TX_GE_FRAC_DENOM0_MSK;
        rxFracDivNumMsk = HYPHY20G_OTU2_TOP_BIT_RX_GE_FRAC_DIV_CTRL0_RX_GE_FRAC_NUMER0_MSK;
        rxFracDivDenMsk = HYPHY20G_OTU2_TOP_BIT_RX_GE_FRAC_DIV_CTRL0_RX_GE_FRAC_DENOM0_MSK;
    } else {
        dataSelMsk = HYPHY20G_OTU2_TOP_BIT_SYS_SIDE_DAT_SEL_OTU2_SYS_DATA_SEL1_MSK;
        txClkCfgOff = HYPHY20G_OTU2_TOP_BIT_CLK_CFG_OTU2_TX_SYS_CLK_CFG_OFF + 1;
        rxClkCfgOff = HYPHY20G_OTU2_TOP_BIT_CLK_CFG_OTU2_RX_SYS_CLK_CFG_OFF + 1;
        txFracDivCtrlReg = HYPHY20G_OTU2_TOP_REG_TX_GE_FRAC_DIV_CTRL1;
        rxFracDivCtrlReg = HYPHY20G_OTU2_TOP_REG_RX_GE_FRAC_DIV_CTRL1;
        txFracDivNumMsk = HYPHY20G_OTU2_TOP_BIT_TX_GE_FRAC_DIV_CTRL1_TX_GE_FRAC_NUMER1_MSK;
        txFracDivDenMsk = HYPHY20G_OTU2_TOP_BIT_TX_GE_FRAC_DIV_CTRL1_TX_GE_FRAC_DENOM1_MSK;
        rxFracDivNumMsk = HYPHY20G_OTU2_TOP_BIT_RX_GE_FRAC_DIV_CTRL1_RX_GE_FRAC_NUMER1_MSK;
        rxFracDivDenMsk = HYPHY20G_OTU2_TOP_BIT_RX_GE_FRAC_DIV_CTRL1_RX_GE_FRAC_DENOM1_MSK;
    }

    switch(source)
    {
        /* CBRB */
        case 0:
            dataSel     = 0;
            txClkCfg    = 0;
            txStrMode   = 1;
            txMapping   = 0;
            rxClkCfg    = 0;
            rxStrMode   = 1;
            rxMapping   = 0;

            otuBlank = oduBlank = opuBlank = 0;
            txStuffEn = rxStuffEn = 0;

            jatEn       = 0;
            break;
        /* ODTU12 */
        case 1:
            dataSel     = 3;
            txClkCfg    = 0;
            txStrMode   = 1;
            txMapping   = 0;
            rxClkCfg    = 0;
            rxStrMode   = 1;
            rxMapping   = 0;

            otuBlank = oduBlank = opuBlank = 0;
            txStuffEn = rxStuffEn = 0;

            jatEn       = 0;
            break;
        /* CBR10G from SONET subsystem */
        case 2:
            dataSel     = 1;
            txClkCfg    = 1;
            txStrMode   = 0;
            txMapping   = 1;
            rxClkCfg    = 1;
            rxStrMode   = 0;
            rxMapping   = 1;

            otuBlank = oduBlank = opuBlank = 0;
            txStuffEn = rxStuffEn = 1;

            jatEn       = 1;
            break;
        /* CBR10G from HSCI subsystem G.Sup 7.1 */
        case 3:
            dataSel     = 2;
            txClkCfg    = 0;
            txStrMode   = 0;
            txMapping   = 0;
            rxClkCfg    = 0;
            rxStrMode   = 0;
            rxMapping   = 0;
            otuBlank = oduBlank = opuBlank = 0;
            txStuffEn = rxStuffEn = 1;

            /* Tx G.Sup43 clause 7.1: 63108/65535 */
            txFracDivNumVal = 63108;
            txFracDivDenVal = 65535;
            /* Rx G.Sup43 clause 7.1: 63121/65535 */
            rxFracDivNumVal = 63121;
            rxFracDivDenVal = 65535;

            jatEn       = 0;
            break;
        /* CBR10G from HSCI subsystem G.Sup 7.2 */
        case 4:
            dataSel     = 2;
            txClkCfg    = 0;
            txStrMode   = 0;
            txMapping   = 0;
            rxClkCfg    = 0;
            rxStrMode   = 0;
            rxMapping   = 0;
            otuBlank = oduBlank = opuBlank = 0;
            txStuffEn = rxStuffEn = 0;

            /* Tx G.Sup43 clause 7.2: 63108/65535 */
            txFracDivNumVal = 63108;
            txFracDivDenVal = 65535;
            /* Rx G.Sup43 clause 7.2: 63121/65535 */
            rxFracDivNumVal = 63121;
            rxFracDivDenVal = 65535;

            jatEn       = 0;
            break;
        /* CBR10G from HSCI subsystem - STS-192 rate */
        case 5:
            dataSel     = 2;
            txClkCfg    = 0;
            txStrMode   = 0;
            txMapping   = 0;
            rxClkCfg    = 0;
            rxStrMode   = 0;
            rxMapping   = 0;
            otuBlank = oduBlank = opuBlank = 0;
            txStuffEn = rxStuffEn = 1;

            /* Tx STS-192: 237/255 */
            txFracDivNumVal = 237;
            txFracDivDenVal = 255;
            /* Rx STS-192: 240/255 */
            rxFracDivNumVal = 240;
            rxFracDivDenVal = 255;

            jatEn       = 0;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    insFas = insMfas = 1;
    /* Configure JAT_622E */
    result = otnHyPhy20gJatInit(fileHndl, slice, jatEn);
    if (result) return result;

    /* Set OTU2_SYS_DATA_SEL[n] */
    addr = HYPHY20G_OTU2_TOP_REG_SYS_SIDE_DAT_SEL;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, dataSelMsk, dataSel);
    if (result) return result;

    /* Set the OTU2_TX_SYS_CLK_CFG[n] */
    addr = HYPHY20G_OTU2_TOP_REG_CLK_CFG;
    result = sysHyPhy20gBitWrite(fileHndl, addr, txClkCfgOff, txClkCfg);
    if (result) return result;

    /* Set the OTU2_RX_SYS_CLK_CFG[n] */
    result = sysHyPhy20gBitWrite(fileHndl, addr, rxClkCfgOff, rxClkCfg);
    if (result) return result;

    /* Set the TX_STREAMING_MODE */
    addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_GLBL_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_TX_STREAMING_MODE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, txStrMode);
    if (result) return result;

    /* Set the TX_ASYNC_MAPPING */
    addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_OPU_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_OTU2_FRM_BIT_TX_OTN_OPU_CTL_TX_ASYNC_MAPPING_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, txMapping);
    if (result) return result;

    /* Configure Blanking */
    addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_FA_OTU_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_OTU2_FRM_BIT_TX_OTN_FA_OTU_CTL_OTU_BLANKING_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, otuBlank);
    if (result) return result;

    addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_ODU_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_OTU2_FRM_BIT_TX_OTN_ODU_CTL_ODU_BLANKING_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, oduBlank);
    if (result) return result;

    addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_OPU_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_OTU2_FRM_BIT_TX_OTN_OPU_CTL_OPU_BLANKING_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, opuBlank);
    if (result) return result;

    /* Configure Tx Insert FAS/MFAS */
    addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_FA_OTU_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_OTU2_FRM_BIT_TX_OTN_FA_OTU_CTL_INSERT_FAS_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, insFas);
    if (result) return result;

    off = HYPHY20G_OTU2_FRM_BIT_TX_OTN_FA_OTU_CTL_INSERT_MFAS_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, insMfas);
    if (result) return result;

    /* Set the TX_STUFF_ENABLE */
    addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_OPU_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_OTU2_FRM_BIT_TX_OTN_OPU_CTL_TX_STUFF_ENABLE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, txStuffEn);
    if (result) return result;

    /* Set the RX_STREAMING_MODE */
    addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_STREAMING_MODE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, rxStrMode);
    if (result) return result;

    /* Set the RX_ASYNC_MAPPING */
    addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_OPU_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_CTL_RX_ASYNC_MAPPING_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, rxMapping);
    if (result) return result;

    /* Set the RX_STUFF_ENABLE */
    addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_OPU_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_OTU2_FRM_BIT_RX_OTN_OPU_CTL_RX_STUFF_ENABLE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, rxStuffEn);
    if (result) return result;

    /* configure TX 10GE Fractional Divider Control n */
    result = sysHyPhy20gRead(fileHndl, txFracDivCtrlReg, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, txFracDivNumMsk, txFracDivNumVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, txFracDivDenMsk, txFracDivDenVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, txFracDivCtrlReg, val);
    if(result) return result;

    /* configure RX 10GE Fractional Divider Control n */
    result = sysHyPhy20gRead(fileHndl, rxFracDivCtrlReg, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, rxFracDivNumMsk, rxFracDivNumVal);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, rxFracDivDenMsk, rxFracDivDenVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, rxFracDivCtrlReg, val);
    if(result) return result;

    /* additional setting if souce is CBR10G from SONET/HSCI subsystem */
    if (source == 2 || source == 3 || source == 4 || source == 5) {
        /* Reset Rx FIFO */
        addr = HYPHY20G_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
        addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        off = HYPHY20G_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_FIFO_RESET_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, off, 1);
        if (result) return result;

        /* Reset Tx FIFO */
        addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_GLBL_CTL;
        addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        off = HYPHY20G_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_TX_FIFO_RESET_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, off, 1);
        if (result) return result;
    }

    /* if ODTU12 system side data source set AUTO_ODUK_AIS_LOS to 0
       otherwise set to 1 */
    val = 1;
    if(source == 1) {
        val = 0;
    }
    addr = HYPHY20G_OTU2_FRM_REG_TX_OTN_AUTO_AIS_CTL;
    addr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_ODUK_AIS_LOS_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtu2SysDataPathCfg */


/*******************************************************************************
**
**  hyPhy20gOtu1SysDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the bidirectional system side data
**                  path for an OTU1 Framer Slice.  Will configure the JAT_622E
**                  for CBR2G5 SONET system side payloads.
**
**                  If OPI subsystem is operational, the following operations
**                  must be performed for OTN Framer slices that received CBR2G5
**                  serial data:
**                  -   hyPhy20gCbrbVcatOtnChnlCfg needs to be called with
**                      enable set to 0 for the OTN Framer slices that are used
**                      for serial system side data.
**                  -   hyPhy20gCbrbOtnZoneChnlCfg needs to be called with
**                      zoneLow = 0, zoneMid = 1, zoneHigh = "don`t care"
**
**                  The system side data sources are:
**                     CBRB
**                     CBR2G5 from SONET subsystem
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTU1 Framer instance:
**                               0 to 7 - OTU1 Framer 0 to 7
**                  source     - Operation Mode of OTU2 Framer slice
**                               0 - CBRB
**                               1 - CBR2G5 from SONET subsystem
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtu1SysDataPathCfg(struct file *fileHndl, UINT4 slice,
						        UINT4 source)
{
    INT4 result;
    UINT4 addr;
    UINT4 dataSelOff, txClkCfgOff, rxClkCfgOff;
    UINT4 dataSel, txClkCfg, txStrMode, txMapping;
    UINT4 rxClkCfg, rxStrMode, rxMapping;
    UINT4 jatEn;
    UINT4 otuBlank, oduBlank, opuBlank;
    UINT4 insFas, insMfas;

    /* argument checking */
    if (slice > 7 || source > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    dataSelOff  = HYPHY20G_OTU1_TOP_BIT_SYS_SIDE_DAT_SEL_OTU1_SYS_DATA_SEL_OFF +
                    slice;
    txClkCfgOff = HYPHY20G_OTU1_TOP_BIT_CLK_CFG_1_OTU1_TX_SYS_CLK_CFG_OFF +
                    slice;
    rxClkCfgOff = HYPHY20G_OTU1_TOP_BIT_CLK_CFG_1_OTU1_RX_SYS_CLK_CFG_OFF +
                    slice;
    switch(source)
    {
        case 0:
            dataSel     = 0;
            txClkCfg    = 0;
            txStrMode   = 1;
            txMapping   = 0;
            rxClkCfg    = 0;
            rxStrMode   = 1;
            rxMapping   = 0;

            otuBlank = oduBlank = opuBlank = 0;

            jatEn       = 0;
            break;
        case 1:
            jatEn       = 1;

            dataSel     = 1;
            txClkCfg    = 1;
            txStrMode   = 0;
            txMapping   = 1;
            rxClkCfg    = 1;
            rxStrMode   = 0;
            rxMapping   = 1;

            otuBlank = oduBlank = opuBlank = 0;

            jatEn       = 1;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    insFas = insMfas = 1;
    /* Configure JAT_622E */
    result = otnHyPhy20gJatInit(fileHndl, slice + 2, jatEn);
    if (result) return result;

    /* Set OTU1_SYS_DATA_SEL[n] */
    addr = HYPHY20G_OTU1_TOP_REG_SYS_SIDE_DAT_SEL;
    result = sysHyPhy20gBitWrite(fileHndl, addr, dataSelOff, dataSel);
    if (result) return result;

    /* Set the OTU1_TX_SYS_CLK_CFG[n] */
    addr = HYPHY20G_OTU1_TOP_REG_CLK_CFG_1;
    result = sysHyPhy20gBitWrite(fileHndl, addr, txClkCfgOff, txClkCfg);
    if (result) return result;

    /* Set the OTU1_RX_SYS_CLK_CFG[n] */
    result = sysHyPhy20gBitWrite(fileHndl, addr, rxClkCfgOff, rxClkCfg);
    if (result) return result;

    /* Set the TX_STREAMING_MODE */
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                    HYPHY20G_OTU1_FRM_REG_TX_OTN_GLBL_CTL, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_TX_STREAMING_MODE_OFF,
                    txStrMode);
    if (result) return result;

    /* Set the TX_ASYNC_MAPPING */
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                    HYPHY20G_OTU1_FRM_REG_TX_OTN_OPU_CTL, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_OTU1_FRM_BIT_TX_OTN_OPU_CTL_TX_ASYNC_MAPPING_OFF,
                    txMapping);
    if (result) return result;

    /* Configure Blanking */
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                    HYPHY20G_OTU1_FRM_REG_TX_OTN_FA_OTU_CTL, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_OTU1_FRM_BIT_TX_OTN_FA_OTU_CTL_OTU_BLANKING_OFF,
                    otuBlank);
    if (result) return result;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                    HYPHY20G_OTU1_FRM_REG_TX_OTN_ODU_CTL, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_OTU1_FRM_BIT_TX_OTN_ODU_CTL_ODU_BLANKING_OFF,
                    oduBlank);
    if (result) return result;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                    HYPHY20G_OTU1_FRM_REG_TX_OTN_OPU_CTL, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_OTU1_FRM_BIT_TX_OTN_OPU_CTL_OPU_BLANKING_OFF,
                    opuBlank);
    if (result) return result;

    /* Configure Tx Insert FAS/MFAS */
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                    HYPHY20G_OTU1_FRM_REG_TX_OTN_FA_OTU_CTL, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_OTU1_FRM_BIT_TX_OTN_FA_OTU_CTL_INSERT_FAS_OFF,
                    insFas);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_OTU1_FRM_BIT_TX_OTN_FA_OTU_CTL_INSERT_MFAS_OFF,
                    insMfas);
    if (result) return result;

    /* Set the RX_STREAMING_MODE */
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                    HYPHY20G_OTU1_FRM_REG_RX_OTN_GLBL_CTL, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_STREAMING_MODE_OFF,
                    rxStrMode);
    if (result) return result;

    /* Set the RX_ASYNC_MAPPING */
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                    HYPHY20G_OTU1_FRM_REG_RX_OTN_OPU_CTL, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_OTU1_FRM_BIT_RX_OTN_OPU_CTL_RX_ASYNC_MAPPING_OFF,
                    rxMapping);
    if (result) return result;

    /* additional setting if souce is CBR2G5 from SONET subsystem */
    if (source == 1) {
        /* Reset the Rx FIFO */
        addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                        HYPHY20G_OTU1_FRM_REG_RX_OTN_GLBL_CTL, slice);
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                        HYPHY20G_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_FIFO_RESET_OFF,
                        1);
        if (result) return result;
        /* Reset the Tx FIFO */
        addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                        HYPHY20G_OTU1_FRM_REG_TX_OTN_GLBL_CTL, slice);
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                        HYPHY20G_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_TX_FIFO_RESET_OFF,
                        1);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtu1SysDataPathCfg */


/*******************************************************************************
**
**  hyPhy20gOtu1LineDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the bidirectional line side data
**                  path for an OTU1 Framer Slice.
**
**                  The line side data sources are:
**                     ODTU12 Multiplexer 0 or 1 timeslot 0 to 3
**                     SONET_LINE (from SFP SERDES)
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTU1 Framer instance:
**                               0 to 7 - OTU1 Framer 0 to 7
**                  source     - Date source for OTU1 Framer
**                               0 - ODTU12 Multiplexer 0 timeslot 0
**                               1 - ODTU12 Multiplexer 0 timeslot 1
**                               2 - ODTU12 Multiplexer 0 timeslot 2
**                               3 - ODTU12 Multiplexer 0 timeslot 3
**                               4 - ODTU12 Multiplexer 1 timeslot 0
**                               5 - ODTU12 Multiplexer 1 timeslot 1
**                               6 - ODTU12 Multiplexer 1 timeslot 2
**                               7 - ODTU12 Multiplexer 1 timeslot 3
**                               8 - SONET_LINE (from SFP SERDES)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtu1LineDataPathCfg(struct file *fileHndl, UINT4 slice,
			                     UINT4 source)
{
    INT4 result;
    UINT4 addr, val, msk;
    UINT4 dataSelMsk, txClkCfgMsk, rxClkCfgMsk;
    UINT4 dataSel = 0, txClkCfg = 0, rxClkCfg = 0;

    /* argument checking */
    if (slice > 7 || source > 8) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    switch(slice)
    {
        case 0:
            dataSelMsk  =
            HYPHY20G_OTU1_TOP_BIT_CRSPNT_CFG_OTU1_FRAMER_DATA_SEL0_MSK;
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG0_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG0_MSK;
            break;

        case 1:
            dataSelMsk  =
            HYPHY20G_OTU1_TOP_BIT_CRSPNT_CFG_OTU1_FRAMER_DATA_SEL1_MSK;
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG1_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG1_MSK;
            break;

        case 2:
            dataSelMsk  =
            HYPHY20G_OTU1_TOP_BIT_CRSPNT_CFG_OTU1_FRAMER_DATA_SEL2_MSK;
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG2_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG2_MSK;
            break;

        case 3:
            dataSelMsk  =
            HYPHY20G_OTU1_TOP_BIT_CRSPNT_CFG_OTU1_FRAMER_DATA_SEL3_MSK;
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG3_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG3_MSK;
            break;

        case 4:
            dataSelMsk  =
            HYPHY20G_OTU1_TOP_BIT_CRSPNT_CFG_OTU1_FRAMER_DATA_SEL4_MSK;
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG4_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG4_MSK;
            break;

        case 5:
            dataSelMsk  =
            HYPHY20G_OTU1_TOP_BIT_CRSPNT_CFG_OTU1_FRAMER_DATA_SEL5_MSK;
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG5_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG5_MSK;
            break;

        case 6:
            dataSelMsk  =
            HYPHY20G_OTU1_TOP_BIT_CRSPNT_CFG_OTU1_FRAMER_DATA_SEL6_MSK;
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG6_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG6_MSK;
            break;

        case 7:
            dataSelMsk  =
            HYPHY20G_OTU1_TOP_BIT_CRSPNT_CFG_OTU1_FRAMER_DATA_SEL7_MSK;
            txClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_TX_LINE_CLK_CFG7_MSK;
            rxClkCfgMsk =
            HYPHY20G_OTU1_TOP_BIT_CLK_CONFIGURATION0_OTU1_RX_LINE_CLK_CFG7_MSK;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    dataSel  = source;

    if (source >= 0 && source <= 3) {
        /* ODTU12 MX A port 0 to 3 */
        txClkCfg = 1;
        rxClkCfg = 1;
    } else if (source >= 4 && source <= 7) {
        /* ODTU12 MX B port 0 to 3 */
        txClkCfg = 2;
        rxClkCfg = 2;
    } else if (source == 8) {
        /* SFP SERDES from SONET_LINE */
        txClkCfg = 0;
        rxClkCfg = 0;
    }

    /* Set OTU1_FRAMER_DATA_SELn[3:0] */
    addr = HYPHY20G_OTU1_TOP_REG_CRSPNT_CFG;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, dataSelMsk, dataSel);
    if (result) return result;

    /* Set the OTU1_TX_LINE_CLK_CFGn[1:0] */
    addr = HYPHY20G_OTU1_TOP_REG_CLK_CONFIGURATION0;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, txClkCfgMsk, txClkCfg);
    if (result) return result;

    /* Set the OTU1_RX_LINE_CLK_CFGn[1:0] */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, rxClkCfgMsk, rxClkCfg);
    if (result) return result;

    /* Set the OTU2 Level TX_LINK_CLK_CFGn[1:0] */
    addr = HYPHY20G_OTU2_TOP_REG_OTU1_CLK_CFG;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, txClkCfgMsk, txClkCfg);
    if (result) return result;

    /* Set the OTU2 Level RX_LINK_CLK_CFGn[1:0] */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, rxClkCfgMsk, rxClkCfg);
    if (result) return result;

    /* configure TX_SCRAMBLE_ENABLE and RX_SCRAMBLE_ENABLE
       for ODTU12 related payloads disable scrambling, otherwise enable
       scrambling */
    val = 1;
    if (source < 8) {
        val = 0;
    }

    addr = HYPHY20G_OTU1_FRM_REG_TX_OTN_GLBL_CTL;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_TX_SCRAMBLE_ENABLE_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    addr = HYPHY20G_OTU1_FRM_REG_RX_OTN_GLBL_CTL;
    addr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_SCRAMBLE_ENABLE_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;


    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtu1LineDataPathCfg */


/*******************************************************************************
**
**  hyPhy20gOtu2SetLoopback
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables one of the OTU2 Framer loopbacks.
**
**                  These loopbacks require that TX and RX serdes rates are
**                  matched in order to pass loopbacked traffic error free.
**
**                  This function ensure only one loopback per slice is enabled
**                  at a time as the loopbacks are not simultaneously supported.
**
**                  When calling function with enable set to 0 all loopbacks
**                  will be disabled independent of the loopType setting.
**
**                  Reserved loopType settings return HYPHY20G_ERR_INVALID_ARG.
**
**  INPUTS:         fileHndl   - device handle
**                  slice      - OTU2 Framer instance:
**                               0 - OTU2 Framer 0
**                               1 - OTU2 Framer 1
**                  loopType   - loopback type
**                               0 - Reserved
**                               1 - TX_TERMINAL_LOOPBACK
**                               2 - Reserved
**                               3 - RX_TERMINAL_LOOPBACK
**                  enable     - enable/disable loopback
**                               0 - disable loopback
**                               1 - enable loopback
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtu2SetLoopback(struct file *fileHndl, UINT4 slice, UINT4 loopType,
                             UINT4 enable)
{
    INT4 result;
    UINT4 rxAddr, txAddr;
    UINT4 rxOffset, txOffset;

    /* argument checking */
    if (slice > 1 || loopType > 3 || enable > 1 ||
        (loopType == 0) || (loopType == 2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    txAddr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(
                                  HYPHY20G_OTU2_FRM_REG_TX_OTN_GLBL_CTL, slice);
    rxAddr = HYPHY20G_OTU2_FRM_TSB_SLICE_REG_OFFSET(
                                  HYPHY20G_OTU2_FRM_REG_RX_OTN_GLBL_CTL, slice);

    txOffset = HYPHY20G_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_TX_TERMINAL_LOOPBACK_OFF;                              
    rxOffset = HYPHY20G_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_TERMINAL_LOOPBACK_OFF;

    /* implement operation */
    switch(loopType)
    {
        case 1:
            result = sysHyPhy20gBitWrite(fileHndl, rxAddr, rxOffset, 0);
            if (result) return result;
            result = sysHyPhy20gBitWrite(fileHndl, txAddr, txOffset, enable);
            if (result) return result;    
            break;
        case 3:
            result = sysHyPhy20gBitWrite(fileHndl, txAddr, txOffset, 0);
            if (result) return result; 
            result = sysHyPhy20gBitWrite(fileHndl, rxAddr, rxOffset, enable);
            if (result) return result;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }
    
    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtu2SetLoopback */

/*******************************************************************************
**
**  hyPhy20gOtu1SetLoopback
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables one of the OTU1 Framer loopbacks.
**
**                  These loopbacks require that TX and RX serdes rates are
**                  matched in order to pass loopbacked traffic error free.
**
**                  This function ensure only one loopback per slice is enabled
**                  at a time as the loopbacks are not simultaneously supported.
**
**                  When calling function with enable set to 0 all loopbacks
**                  will be disabled independent of the loopType setting.
**
**                  Reserved loopType settings return HYPHY20G_ERR_INVALID_ARG.
**
**  INPUTS:         fileHndl   - device handle
**                  slice      - OTU1 Framer instance:
**                               0 to 7 OTU1 Framer 1 to 8
**                  loopType   - loopback type
**                               0 - Reserved
**                               1 - TX_TERMINAL_LOOPBACK
**                               2 - Reserved
**                               3 - RX_TERMINAL_LOOPBACK
**                  enable     - enable/disable loopback
**                               0 - disable loopback
**                               1 - enable loopback
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOtu1SetLoopback(struct file *fileHndl, UINT4 slice, UINT4 loopType,
                             UINT4 enable)
{
    INT4 result;
    UINT4 rxAddr, txAddr;
    UINT4 rxOffset, txOffset;

    /* argument checking */
    if (slice > 7 || loopType > 3 || enable > 1 ||
        (loopType == 0) || (loopType == 2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* variable declaration */ 
    txAddr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                                  HYPHY20G_OTU1_FRM_REG_TX_OTN_GLBL_CTL, slice);
    rxAddr = HYPHY20G_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                                  HYPHY20G_OTU1_FRM_REG_RX_OTN_GLBL_CTL, slice);
                                  
    txOffset = HYPHY20G_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_TX_TERMINAL_LOOPBACK_OFF;
    rxOffset = HYPHY20G_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_TERMINAL_LOOPBACK_OFF;    

    /* implement operation */
    switch(loopType)
    {
        case 1:
            result = sysHyPhy20gBitWrite(fileHndl, rxAddr, rxOffset, 0);
            if (result) return result;
            result = sysHyPhy20gBitWrite(fileHndl, txAddr, txOffset, enable);
            if (result) return result;
            break;
        case 3:
            result = sysHyPhy20gBitWrite(fileHndl, txAddr, txOffset, 0);
            if (result) return result;
            result = sysHyPhy20gBitWrite(fileHndl, rxAddr, rxOffset, enable);
            if (result) return result;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOtu1SetLoopback */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/*******************************************************************************
**
**  otnHyPhy20gEfecDeviceInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the EFEC blocks so that they may
**                  be used for I.4 or I.7 FEC generation on either of the OTU2
**                  framers.
**
**                  This function impacts both slice 0 and 1 of the OTU2 framer
**                  EFEC blocks.
**
**
**  INPUTS:         fileHndl   - device handle
**                  enable     - Enabling of I.4 and I.7 FEC
**                               0 - At device level both slices in low power
**                                   mode
**                               1 - At block level both blocks in low power
**                                   mode.  This mode allows for each OTU2
**                                   framer to optionally be configured with
**                                   I.4 or I.7 FEC.
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 otnHyPhy20gEfecDeviceInit(struct file *fileHndl, UINT4 enable)
{
    INT4 result;
    UINT4 regAddr, regData, slice;

    /* argument checking */
    if (enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* toggle each device level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_2;

    result = sysHyPhy20gRead(fileHndl, regAddr, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                                HYPHY20G_TOP_BIT_SYS_SW_RST_2_OTN_EFEC1_RST_MSK,
                                1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                                HYPHY20G_TOP_BIT_SYS_SW_RST_2_OTN_EFEC0_RST_MSK,
                                1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, regData);
    if(result) return result;

    result = sysHyPhy20gRead(fileHndl, regAddr, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                                HYPHY20G_TOP_BIT_SYS_SW_RST_2_OTN_EFEC1_RST_MSK,
                                0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                                HYPHY20G_TOP_BIT_SYS_SW_RST_2_OTN_EFEC0_RST_MSK,
                                0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, regData);
    if(result) return result;


    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_2;

    /* set device level lowpwr bits to 1 */
    result = sysHyPhy20gRead(fileHndl, regAddr, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                            HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OTN_EFEC1_LOWPWR_MSK,
                            1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                            HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OTN_EFEC0_LOWPWR_MSK,
                            1);
    if(result) return result;

    /* set device level I.4 and I.7 lowpwr bits to 1 */
    result = hyPhy20gVarFieldWrite (&regData,
                            HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OTN_EFEC_LOWPWR_I7_MSK,
                            1);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                            HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OTN_EFEC_LOWPWR_I4_MSK,
                            1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, regData);
    if(result) return result;





    /* if enable = 0 exit */
    if (enable == 0) {
        return HYPHY20G_SUCCESS;
    }

    /* set DISABLE_CLK_FEC_I4 and DISABLE_CLK_FEC_I7 */
    for (slice=0; slice < 2; slice++){
        regAddr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(HYPHY20G_EFEC_REG_CFG,
                                                     slice);

        result = sysHyPhy20gRead(fileHndl, regAddr, &regData);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData,
                                   HYPHY20G_EFEC_BIT_CFG_DISABLE_CLK_FEC_I4_MSK,
                                   1);
        if(result) return result;

        result = hyPhy20gVarFieldWrite (&regData,
                                   HYPHY20G_EFEC_BIT_CFG_DISABLE_CLK_FEC_I7_MSK,
                                   1);
        if(result) return result;


        result = hyPhy20gVarFieldWrite (&regData,
                                     HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I4_MSK,0);
        if(result) return result;
        result = hyPhy20gVarFieldWrite (&regData,
                                     HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I7_MSK,0);
        if(result) return result;


        result = sysHyPhy20gWrite(fileHndl, regAddr, regData);
        if(result) return result;

    }

    /* set device level lowpwr bits to 0 */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_2;
    result = sysHyPhy20gRead(fileHndl, regAddr, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                            HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OTN_EFEC1_LOWPWR_MSK,
                            0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                            HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OTN_EFEC0_LOWPWR_MSK,
                            0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                            HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OTN_EFEC_LOWPWR_I7_MSK,
                            0);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData,
                            HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_OTN_EFEC_LOWPWR_I4_MSK,
                            0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* otnHyPhy20gEfecDeviceInit */

/*******************************************************************************
**
**  otnHyPhy20gJatInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    None
**
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - JAT 622 instance:
**                              0 to 9, where:
**                                  0 to 1 maps to OTU2 framer 0 to 1
**                                  2 to 9 maps to OTU1 framer 0 to 7
**                  mode      - Operation Mode of JAT 622 slice
**                              0 - Disabled
**                              1 - JAT 622 block Enabled
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 otnHyPhy20gJatInit(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    UINT4 addr, off;
    UINT4 swRstOff;
    INT4 result;

    /* argument checking */
    if (slice > 9) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_OTU2_TOP_REG_CFG;
    swRstOff = HYPHY20G_OTU2_TOP_BIT_CFG_SW_RST_JAT_OFF + slice;

    if (mode == 0) {      /* disable operation */
        /* hold OTU2_FRAMER block in reset */
        result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 1);
        if (result) return result;
        return HYPHY20G_SUCCESS;
    }

    /*
     * enable operation
     */
    result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 1);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 0);
    if (result) return result;

    /* enable JAT_622_E block */
    addr = HYPHY20G_OTU2_JAT_TSB_SLICE_REG_OFFSET(HYPHY20G_OTU2_JAT_REG_PLL_CFG,
                                                  slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                                 HYPHY20G_OTU2_JAT_BIT_PLL_CFG_PWRDN_OFF, 0);
    if (result) return result;

    /* Poll the DLL_RUN bit  to ensure it is set to 1 */
    addr = HYPHY20G_OTU2_JAT_REG_DLL_STAT;
    addr = HYPHY20G_OTU2_JAT_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_OTU2_JAT_BIT_DLL_STAT_DLL_RUN_MSK;
    result = sysHyPhy20gPollBit(fileHndl, addr, off, 1, 5000);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* otnHyPhy20gJatInit */


/*******************************************************************************
**
**  otnHyPhy20gEfecInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the major operational mode of an
**                  EFEC resource.
**
**
**  INPUTS:         fileHndl   - device handle
**                  slice      - EFEC instance:
**                               0 - EFEC 0
**                               1 - EFEC 1
**                  mode       - Operation Mode of OTU2 Framer slice
**                               0 - G.975.1 Appendix I.4 FEC algorithm is used
**                               1 - G.975.1 Appendix I.7 FEC algorithm is used
**                               2 - Disabled
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 otnHyPhy20gEfecInit(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 fecSel = 0, disEnClkI4 = 0, disEnClkI7 = 0, swRstI4 = 0, swRstI7 = 0;

    /* argument checking */
    if (slice > 1 || mode > 2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (mode == 0) {
        fecSel      = 0;
        disEnClkI4  = 0;
        disEnClkI7  = 1;
        swRstI4     = 0;
        swRstI7     = 1;
    } else if (mode == 1) {
        fecSel      = 1;
        disEnClkI4  = 1;
        disEnClkI7  = 0;
        swRstI4     = 1;
        swRstI7     = 0;
    } else if (mode == 2) {
        swRstI4     = 1;
        swRstI7     = 1;
    }

    addr = HYPHY20G_EFEC_TSB_SLICE_REG_OFFSET(HYPHY20G_EFEC_REG_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result != HYPHY20G_SUCCESS) return result;

    /* don't configure these bits for disable operation */
    if (mode != 2) {
        val = mHYPHY20G_SET_FIELD4(val, fecSel,
                HYPHY20G_EFEC_BIT_CFG_STRONG_FEC_SEL_MSK,
                HYPHY20G_EFEC_BIT_CFG_STRONG_FEC_SEL_OFF);
        val = mHYPHY20G_SET_FIELD4(val, disEnClkI4,
                HYPHY20G_EFEC_BIT_CFG_DISABLE_CLK_FEC_I4_MSK,
                HYPHY20G_EFEC_BIT_CFG_DISABLE_CLK_FEC_I4_OFF);
        val = mHYPHY20G_SET_FIELD4(val, disEnClkI7,
                HYPHY20G_EFEC_BIT_CFG_DISABLE_CLK_FEC_I7_MSK,
                HYPHY20G_EFEC_BIT_CFG_DISABLE_CLK_FEC_I7_OFF);
    }

    val = mHYPHY20G_SET_FIELD4(val, swRstI4,
            HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I4_MSK,
            HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I4_OFF);
    val = mHYPHY20G_SET_FIELD4(val, swRstI7,
            HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I7_MSK,
            HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I7_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result != HYPHY20G_SUCCESS) return result;

    sysHyPhy20gMsDelay(1); 
    if (mode == 0) {
        val = mHYPHY20G_SET_FIELD4(val, 1,
                    HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I4_MSK,
                    HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I4_OFF);
    } else if (mode == 1) {
        val = mHYPHY20G_SET_FIELD4(val, 1,
                    HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I7_MSK,
                    HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I7_OFF);
    } else {
        return HYPHY20G_SUCCESS;
    }
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    sysHyPhy20gMsDelay(1); 
    if (mode == 0) {
        val = mHYPHY20G_SET_FIELD4(val, 0,
                    HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I4_MSK,
                    HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I4_OFF);
    } else if (mode == 1) {
        val = mHYPHY20G_SET_FIELD4(val, 0,
                    HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I7_MSK,
                    HYPHY20G_EFEC_BIT_CFG_SW_RST_FEC_I7_OFF);
    } else {
        return HYPHY20G_SUCCESS;
    }
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;

} /* otnHyPhy20gEfecInit */




/* end of file */
