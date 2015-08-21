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
**  MODULE      : HyPHY 20G LSCI Subsystem Configuration Code
**
**  FILE        : hyphy20g_lsci.c
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
#include "hyphy20g_lsci.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 lsciHyPhy20gLsciInitCtxt(struct file *fileHndl, sHYPHY20G_LSCI_CTXT *pCtxt);

INT4 lsciHyPhy20gLsciFeGeBlockInit(struct file *fileHndl, UINT4 slice,
                                   UINT4 enable);

INT4 lsciHyPhy20gLsciFeGeDataPathCfg(struct file *fileHndl, UINT4 slice);

INT4 lsciHyPhy20gLsciGeGfpTDataPathCfg(struct file *fileHndl, UINT4 slice);

INT4 lsciHyPhy20gLsciFcDataPathCfg(struct file *fileHndl, UINT4 slice);

INT4 lsciHyPhy20gLsciCbrDataPathCfg(struct file *fileHndl, UINT4 slice);

INT4 lsciHyPhy20gLsciFcCbrLcvErrInsCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 dir, UINT4 mode, UINT4 err_patt_n,
                                       UINT4 err_patt_p);

INT4 lsciHyPhy20gLsciFcCbrUsrPattCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 dir, UINT4 olSeqHi, UINT4 olSeqLow,
                                     UINT4 pattDisp);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/
/*******************************************************************************
**
**  hyPhy20gLsciInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes a LSCI slice at the device level 
**                  and configures it to one of the major operating modes:
**                      1 - MAC Terminated (FE/GE, SGMII FE/GE)
**                      2 - GE GFP-T (Transparent GE)
**                      3 - FC 4G or lower rate (including FICON)
**                      4 - ESCON
**                      5 - Low-Speed Byte-aligned CBR (STS-48/12/3
**                          asynchronous transparent)
**                      6 - Other Low-Speed Byte-aligned CBR
**                      7 - Low-Speed 8b10b encoded CBR with TX_RD_INV_EN
**                      8 - Low-Speed 8b10b encoded CBR with TX_RD_INV_DIS
**
**                  Alternatively, this function can be called to disable the
**                  LSCI subsystem.
**
**                  This function controls FW command for MSTATX statistics
**                  collection.  hyPhy20gHostMsgInit must have been called
**                  prior to calling this function.
**
**                  In low-speed 8b10b CBR mode, there is an additional 
**                  disparity inversion logic that is available.  The disparity 
**                  inversion logic controls the disparity insertion when switching
**                  from NOS insertion to data stream and also ensures that no 
**                  disparity errors are detected after deassertion of DLOLB.
**
**                  So split the Low-Speed 8b10b encoded CBR mode to 
**                  Low-Speed 8b10b encoded CBR with TX_RD_INV_EN(mode 7) and
**                  Low-Speed 8b10b encoded CBR with TX_RD_INV_DIS(mode 8).
**
**                  For clients such as FC and GE that are sensitive to disparity, 
**                  mode 8 should be used.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**                  mode      - Operation mode of LSCI slice
**                              0 - Disable the LSCI slice
**                              1 - MAC Terminated (FE/GE, SGMII FE/GE)
**                              2 - GE GFP-T (Transparent GE)
**                              3 - FC 4G or lower rate (including FICON)
**                              4 - ESCON
**                              5 - Low-Speed Byte-aligned CBR (STS-48/12/3
**                                  asynchronous transparent)
**                              6 - Other Low-Speed Byte-aligned CBR
**                              7 - Low-Speed 8b10b encoded CBR with TX_RD_INV_EN
**                              8 - Low-Speed 8b10b encoded CBR with TX_RD_INV_DIS
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gLsciInit(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    UINT4 devId, revId, status;
    INT4 result;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 addr, val, lowpwrOff, rstOff;
    UINT4 pmmLpwr, fcCbrLpwr, feGeLpwr, pmmRst, fcCbrRst, feGeRst, lsciRst;
    UINT4 refClkSel, enetMdSel, pmmMdSel, sysMdSel, lnMdSel;
    UINT4 statEn;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 ||
        mode > LSCI_TRAFFIC_MAX) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");



    if (mode == HYPHY20G_DISABLE){           /* disable operations */
       
        result = hyPhy20gEnetComLsciInit(fileHndl, slice, mode);
        if(result) return result;    	

        /* reset at top level */
        rstOff = HYPHY20G_TOP_BIT_SYS_SW_RST_2_ENET_LSCI_RST_0_OFF + slice;
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                 rstOff, 1);
        if(result) return result;
        	                             	
    	  /* set lowpwr bit to 1 */
        lowpwrOff = HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_ENET_LSCI_LOWPWR_0_OFF + slice;
        result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                                 lowpwrOff, 1);
        if(result) return result;

        /* free memory used by LSCI subsystem */
        result = hyPhy20gLsciDestroyCtxt(fileHndl, slice);
        if(result) return result;

#ifndef HWACCESSMODE_USB
        
        /* unconditionally disable FW MSTATX PMON statistics collection */
        result = hyPhy20gHostMsgCmdCfgPmonEnetStatColCtrl(fileHndl, 1,
                                                          slice, 0);
        if(result) return result;
#endif

        return HYPHY20G_SUCCESS;
    }
    
    /* set lowpwr bit to 1 */
    lowpwrOff = HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_ENET_LSCI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                                 lowpwrOff, 1);
    if(result) return result;

    /* reset at top level */
    rstOff = HYPHY20G_TOP_BIT_SYS_SW_RST_2_ENET_LSCI_RST_0_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                 rstOff, 1);
    if(result) return result;
    	              
    /*
     * enable operations
     */
    /* allocate and initialize memory used by LSCI subsystem */
    result = hyPhy20gLsciCreateCtxt(fileHndl, slice, mode);
    if(result) return result;

    /* remove at top level lowpwr if subsystem to be enabled */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                                 lowpwrOff, 0);
    if(result) return result;

    /* remove LSCI slice reset at top level */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                 rstOff, 0);
    if(result) return result;

    /* configure LSCI Top level register. */
    if (mode == LSCI_MAC_TEMINATION) {
        pmmLpwr        = 1;
        fcCbrLpwr      = 1;
        feGeLpwr       = 0;
        pmmRst         = 1;
        fcCbrRst       = 1;
        feGeRst        = 0;
        lsciRst        = 0;
        refClkSel      = 0;
        enetMdSel      = 0;
        pmmMdSel       = 0;
        sysMdSel       = 0;
        lnMdSel        = 0;
        statEn         = 1;
    } else if (mode == LSCI_GE_GFPT_MAPPING) {
        pmmLpwr        = 0;
        fcCbrLpwr      = 1;
        feGeLpwr       = 0;
        pmmRst         = 0;
        fcCbrRst       = 1;
        feGeRst        = 0;
        lsciRst        = 0;
        refClkSel      = 1;
        enetMdSel      = 1;
        pmmMdSel       = 0;
        sysMdSel       = 1;
        lnMdSel        = 0;
        statEn         = 0;
    } else {
        pmmLpwr        = 0;
        fcCbrLpwr      = 0;
        feGeLpwr       = 1;
        pmmRst         = 0;
        fcCbrRst       = 0;
        feGeRst        = 1;
        lsciRst        = 0;
        refClkSel      = 1;
        enetMdSel      = 0;
        pmmMdSel       = 1;
        sysMdSel       = 1;
        lnMdSel        = 1;
        statEn         = 0;
    }

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_TOP_REG_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, pmmLpwr,
            HYPHY20G_LSCI_TOP_BIT_CFG_PMM_LOWPWR_MSK,
            HYPHY20G_LSCI_TOP_BIT_CFG_PMM_LOWPWR_OFF);
    val = mHYPHY20G_SET_FIELD4(val, fcCbrLpwr,
            HYPHY20G_LSCI_TOP_BIT_CFG_FC_CBR_LOWPWR_MSK,
            HYPHY20G_LSCI_TOP_BIT_CFG_FC_CBR_LOWPWR_OFF);
    val = mHYPHY20G_SET_FIELD4(val, feGeLpwr,
            HYPHY20G_LSCI_TOP_BIT_CFG_ENET_FEGE_LOWPWR_MSK,
            HYPHY20G_LSCI_TOP_BIT_CFG_ENET_FEGE_LOWPWR_OFF);
    val = mHYPHY20G_SET_FIELD4(val, pmmRst,
            HYPHY20G_LSCI_TOP_BIT_CFG_PMM_RST_MSK,
            HYPHY20G_LSCI_TOP_BIT_CFG_PMM_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, fcCbrRst,
            HYPHY20G_LSCI_TOP_BIT_CFG_FC_CBR_RST_MSK,
            HYPHY20G_LSCI_TOP_BIT_CFG_FC_CBR_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, feGeRst,
            HYPHY20G_LSCI_TOP_BIT_CFG_ENET_FEGE_RST_MSK,
            HYPHY20G_LSCI_TOP_BIT_CFG_ENET_FEGE_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, lsciRst,
            HYPHY20G_LSCI_TOP_BIT_CFG_LSCI_RST_MSK,
            HYPHY20G_LSCI_TOP_BIT_CFG_LSCI_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, refClkSel,
            HYPHY20G_LSCI_TOP_BIT_CFG_REFCLK334_SEL_MSK,
            HYPHY20G_LSCI_TOP_BIT_CFG_REFCLK334_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, enetMdSel,
            HYPHY20G_LSCI_TOP_BIT_CFG_ENET_MODE_SEL_MSK,
            HYPHY20G_LSCI_TOP_BIT_CFG_ENET_MODE_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, pmmMdSel,
            HYPHY20G_LSCI_TOP_BIT_CFG_PMM_MODE_SEL_MSK,
            HYPHY20G_LSCI_TOP_BIT_CFG_PMM_MODE_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, sysMdSel,
            HYPHY20G_LSCI_TOP_BIT_CFG_SYS_MODE_SEL_MSK,
            HYPHY20G_LSCI_TOP_BIT_CFG_SYS_MODE_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, lnMdSel,
            HYPHY20G_LSCI_TOP_BIT_CFG_LINE_MODE_SEL_MSK,
            HYPHY20G_LSCI_TOP_BIT_CFG_LINE_MODE_SEL_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    if (mode == LSCI_MAC_TEMINATION) {
        result = lsciHyPhy20gLsciFeGeBlockInit(fileHndl, slice, 1);
        if(result) return result;

        result = lsciHyPhy20gLsciFeGeDataPathCfg(fileHndl, slice);
        if(result) return result;

        /* max frame size to 9618 bytes for jumbo frames */
        result = hyPhy20gLsciFeGeSetFrameSizes(fileHndl, slice,
                    0x2592, 0x00, 0x2592, 0x00);
        if(result) return result;

    } else if (mode == LSCI_GE_GFPT_MAPPING) {
        result = lsciHyPhy20gLsciFeGeBlockInit(fileHndl, slice, 1);
        if(result) return result;

        result = lsciHyPhy20gLsciGeGfpTDataPathCfg(fileHndl, slice);
        if(result) return result;
    } else if ((mode == LSCI_FC_4G) || (mode == LSCI_ESCON)){
        result = lsciHyPhy20gLsciFcDataPathCfg(fileHndl, slice);
        if(result) return result;
    } else {
        result = lsciHyPhy20gLsciCbrDataPathCfg(fileHndl, slice);
        if(result) return result;
    }

    result = hyPhy20gEnetComLsciInit(fileHndl, slice, mode);
    if(result) return result;

#ifndef HWACCESSMODE_USB
    /* enable of disable FW MSTATX PMON statistics collection based on mode */
    result = hyPhy20gHostMsgCmdCfgPmonEnetStatColCtrl(fileHndl, 1, slice,
                                                      statEn);
    if(result) return result;
#endif

    return HYPHY20G_SUCCESS;
} /* hyPhy20gLsciInit */


/*******************************************************************************
**
**  hyPhy20gLsciDataPathEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables the Rx and Tx channel
**                  data flow separately for a specified LSCI slice.
**
**                  This function must be called in order to enable traffic
**                  after the specified LSCI slice has initialized and payload
**                  specific configuration has done.
**
**                  To enable data flow decently, user must firstly enable
**                  the transmit data flow, and then enable the receive
**                  data flow as a part of the last configuration stage.
**
**                  To disable data flow decently, user must firstly disable
**                  the receive data flow, and then disable the transmit
**                  data flow as a part of the last configuration stage.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**                  dir       - data flow direction
**                              0 - enable/disable the transmit data flow
**                                  (from the CPB to the SFP)
**                              1 - enable/disable the receive data flow
**                                  (from the SFP to the CPB)
**                  enable    - 0 - disable
**                              1 - enabled for normal data path
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciDataPathEnable(struct file *fileHndl, UINT4 slice,
                                UINT4 dir, UINT4 enable)
{
    INT4 result;
    UINT4 addr;
    UINT4 mode;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > HYPHY20G_LSCI_NUM_SLICE - 1) || (dir > 1) ||
        (enable > 1)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = lsciHyPhy20gLsciCtxtGetMode(fileHndl, slice, &mode);
    if(result) return result;

    if (mode == LSCI_MAC_TEMINATION) {

        if (dir == HYPHY20G_TX_DIRECTION) {
            /* enable/disable transmit data path */
            addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TMAC_REG_CFG_1, slice);
            result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_TMAC_ENBL_OFF, enable);
            if(result) return result;

        } else {
            /* enable/disable receive data path */
            addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_RMAC_REG_CFG_1, slice);
            result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_RMAC_ENBL_OFF, enable);
            if(result) return result;

        }

    } else if (mode == LSCI_GE_GFPT_MAPPING) {

        if (dir == HYPHY20G_TX_DIRECTION) {
            /* enable/disable transmit data path */
            addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_TX_CFG, slice);
            result = sysHyPhy20gBitWrite(fileHndl, addr,
                        HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_SOFT_RESET_OFF,
                        (enable == 0)?1:0);
            if(result) return result;

            result = hyPhy20gLsciPmmMonEnable(fileHndl, slice, enable);
            if(result) return result;

        } else {
            /* enable/disable receive data path */
            result = hyPhy20gLsciPmmMkrEnable(fileHndl, slice, enable);
            if(result) return result;

            addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_RX_CFG, slice);
            result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_SOFT_RESET_OFF,
                (enable == 0)?1:0);
            if(result) return result;
        }

    } else if ((mode == LSCI_FC_4G) ||
               (mode == LSCI_ESCON) ||
               (mode == LSCI_BYTEALIGED_CBR_STS) ||
               (mode == LSCI_BYTEALIGED_CBR) ||
               (mode == LSCI_8B10B_TX_RD_INV_EN_CBR) ||
               (mode == LSCI_8B10B_TX_RD_INV_DIS_CBR)) {

        if (dir == HYPHY20G_TX_DIRECTION) {
            /* enable/disable transmit data path */
            addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_TX_CFG, slice);
            result = sysHyPhy20gBitWrite(fileHndl, addr,
                        HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_SOFT_RST_OFF,
                        (enable == 0)?1:0);
            if(result) return result;

            result = hyPhy20gLsciPmmMonEnable(fileHndl, slice, enable);
            if(result) return result;

        } else {
            /* enable/disable receive data path */
            result = hyPhy20gLsciPmmMkrEnable(fileHndl, slice, 1);
            if(result) return result;

            addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_RX_CFG, slice);
            result = sysHyPhy20gBitWrite(fileHndl, addr,
                        HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_SOFT_RST_OFF,
                        (enable == 0)?1:0);
            if(result) return result;
        }

    } else {
        /* should never run into this place */
        return HYPHY20G_ERR_INVALID_ARG;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gLsciDataPathEnable */


/*******************************************************************************
**
**  hyPhy20gLsciFeGeModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the ENET_FEGE block in LSCI
**                  subsystem for MAC Terminated traffic operation mode.
**
**                  When LSCI slice is configured in MAC Terminated
**                  (FE/GE, SGMII FE/GE) mode, this function
**                  must be called after hyPhy20gLsciInit, but prior to
**                  hyPhy20gLsciDataPathEnable (enable operation).
**
**                  Along with the major ENET_FEGE block operating mode, this
**                  function also configures a specific auto-negotiation mode.
**
**                  For GE MAC Terminated mode, user can select:
**                      - disable auto-negotiation feature;
**                      - enable auto-negotiation and only exchange base page;
**                      - enable auto-negotiation and exchange both base page
**                        and next page.
**
**                  For FE MAC Terminated mode, auto-negotiation feature
**                  is not available, because of no such definition in
**                  IEEE 802.3 Clause 24 (100 BASE-X).
**
**                  For SGMII FE and GE MAC Terminated modes, user can select:
**                      - enable auto-negotiation with device auto ACK, which
**                        exactly implements auto-negotiation state machine
**                        defined in figure 37-6, IEEE 802.3.
**                      - enable auto-negotiation with manual ACK, which enables
**                        SW intervening to transfer the ACK.
**
**                  When the LSCI is configured in the GE MAC Terminated mode,
**                  it is user's responsibility to select a proper
**                  auto-negotiation mode to match settings of the peer device.
**                  When auto-negotiation is enabled (mode == 0 or 5),
**                  to change pause capability of local device in the
**                  auto-negotiation base page register, call
**                  hyPhy20gLsciGeAnBasePageCfg function prior to calling
**                  hyPhy20gLsciFeGeModeCfg.
**
**                  When configured in the SGMII FE or GE Terminated
**                  mode, the LSCI along with SERDES is consider as MAC layer,
**                  while it interfaces with external PHY device.
**                  In this case, user can select auto-negotiation with device
**                  automatical or manual ACKNOWLEDGE through the 'mode'
**                  parameter (mode '3', '4', '6' and '7') of this function.
**                    1) If Control Information (defined in Table 1,
**                    "Serial-GMII Specification") between MAC and exteranl PHY
**                    device are explicitly configured to match with each other,
**                    user can select the auto-negotiation auto ACK mode.
**                    2) If Control Information setting between MAC
**                    and exteranl PHY device does not match, or
**                    whether matching is unknown, user must select
**                    the auto-negotiation maunal ACK mode, and then call
**                    hyPhy20gLsciSgmiiAnBpAbilityCompare API to compare PHY's
**                    Control Information with MAC's setting.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**                  mode      - Operation mode of LSCI slice level
**                              0 - GE MAC Terminated - auto-negotiation
**                                  enabled, with base page exchange only;
**                              1 - FE MAC Terminated;
**                              2 - SGMII GE MAC Terminated, with auto
**                                  Acknowledge in auto-negotiation processing;
**                              3 - SGMII FE MAC Terminated, with auto
**                                  Acknowledge in auto-negotiation processing;
**                              4 - GE MAC Terminated - auto-negotiation
**                                  disabled;
**                              5 - GE MAC Terminated - auto-negotiation
**                                  enabled, with next page exchange;
**                              6 - SGMII GE MAC Terminated, with manual
**                                  Acknowledge in auto-negotiation processing;
**                              7 - SGMII FE MAC Terminated, with manual
**                                  Acknowledge in auto-negotiation processing;
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gLsciFeGeModeCfg(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    UINT4 devId, revId, status;  
    INT4 result;
    UINT4 traffic;
    UINT4 addr;
    UINT4 bitOff;
    UINT4 msk;
    UINT4 txSwapVal;
    UINT4 fegeMdVal;
    UINT4 l1rppMdVal;
    UINT4 linkTimeVal;
    UINT4 anEnblVal;
    UINT4 locNextPgVal;
    UINT4 l1tppMdVal;
    UINT4 manAckVal = 0;
    UINT4 regData;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 ||
        mode > LSCI_SGMII_FE_MAC_TEMINATION_MANACK) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* retrieve slice mode from context */
    result = lsciHyPhy20gLsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;

    /* confirm mode before processing */
    if (traffic != LSCI_MAC_TEMINATION) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /*
     * unconditionally disable L1RPP and L1TPP block prior to configuration
     */
    /* disable L1RPP block */
    addr = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_CFG_AND_STAT;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    bitOff = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_L1RPP_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitOff, 0);
    if(result) return result;

    /* disable L1TPP block */
    addr = HYPHY20G_LSCI_ENET_FEGE_L1TPP_REG_CFG;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    bitOff = HYPHY20G_LSCI_ENET_FEGE_L1TPP_BIT_CFG_L1TPP_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitOff, 0);
    if(result) return result;

    /*
     * init configuration values
     */
    switch (mode) {
        case LSCI_GE_MAC_TEMINATION:
            txSwapVal    = 1;
            fegeMdVal    = 0x1;
            l1rppMdVal   = 1;
            linkTimeVal  = 0x0000;
            anEnblVal    = 1;
            locNextPgVal = 0;
            l1tppMdVal   = 0x1;
            manAckVal    = 0;
            break;
        case LSCI_FE_MAC_TEMINATION:
            txSwapVal    = 0;
            fegeMdVal    = 0x2;
            l1rppMdVal   = 0x7;
            linkTimeVal  = 0x1388;
            anEnblVal    = 0;
            locNextPgVal = 0;
            l1tppMdVal   = 0x5;
            break;
        case LSCI_SGMII_GE_MAC_TEMINATION:
            txSwapVal    = 1;
            fegeMdVal    = 0x3;
            l1rppMdVal   = 1;
            linkTimeVal  = 0x0000;
            anEnblVal    = 1;
            locNextPgVal = 0;
            l1tppMdVal   = 0x1;
            manAckVal    = 0;
            break;
        case LSCI_SGMII_FE_MAC_TEMINATION:
            txSwapVal    = 1;
            fegeMdVal    = 0x4;
            l1rppMdVal   = 1;
            linkTimeVal  = 0x0000;
            anEnblVal    = 1;
            locNextPgVal = 0;
            l1tppMdVal   = 0x1;
            manAckVal    = 0;
            break;
        case LSCI_GE_MAC_TEMINATION_AN_DISABLE:
            txSwapVal    = 1;
            fegeMdVal    = 0x1;
            l1rppMdVal   = 1;
            linkTimeVal  = 0x0000;
            anEnblVal    = 0;
            locNextPgVal = 0;
            l1tppMdVal   = 0x1;
            manAckVal    = 0;
            break;
        case LSCI_GE_MAC_TEMINATION_AN_NEXTPG:
            txSwapVal    = 1;
            fegeMdVal    = 0x1;
            l1rppMdVal   = 1;
            linkTimeVal  = 0x0000;
            anEnblVal    = 1;
            locNextPgVal = 1;
            l1tppMdVal   = 0x1;
            manAckVal    = 0;
            break;
        case LSCI_SGMII_GE_MAC_TEMINATION_MANACK:
            txSwapVal    = 1;
            fegeMdVal    = 0x3;
            l1rppMdVal   = 1;
            linkTimeVal  = 0x0000;
            anEnblVal    = 1;
            locNextPgVal = 0;
            l1tppMdVal   = 0x1;
            manAckVal    = 1;
            break;
        case LSCI_SGMII_FE_MAC_TEMINATION_MANACK:
            txSwapVal    = 1;
            fegeMdVal    = 0x4;
            l1rppMdVal   = 1;
            linkTimeVal  = 0x0000;
            anEnblVal    = 1;
            locNextPgVal = 0;
            l1tppMdVal   = 0x1;
            manAckVal    = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /*
     * configure ENET_FEGE Top Level register
     */
    addr = HYPHY20G_LSCI_TOP_REG_CFG;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    bitOff = HYPHY20G_LSCI_TOP_BIT_CFG_TX_10B_SWAP_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitOff, txSwapVal);
    if(result) return result;

    /*
     * configure ENET_FEGE Top Level register
     */
    /* configure the mode in which this FEGE Ethernet Channel is to operate */
    addr = HYPHY20G_LSCI_ENET_FEGE_TOP_REG_MD_CFG;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk  = HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_FEGE_MODE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, fegeMdVal);
    if(result) return result;

    /*
     * configure L1RPP block
     */
    addr = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_CFG_AND_STAT;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_MODE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, l1rppMdVal);
    if(result) return result;

    msk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_MANUAL_ACK_EN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, manAckVal);
    if(result) return result;

    /* configure LINK TIME for all SGMII modes to meet 1.6ms 
       requirement in the SGMII spec */
    if (mode == LSCI_SGMII_GE_MAC_TEMINATION        ||
        mode == LSCI_SGMII_FE_MAC_TEMINATION        ||
        mode == LSCI_SGMII_GE_MAC_TEMINATION_MANACK ||
        mode == LSCI_SGMII_FE_MAC_TEMINATION_MANACK) {
        addr = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_DBG_LNK_TMR_OVERRIDE;
        addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
        msk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_DBG_LNK_TMR_OVERRIDE_LINK_TIMER_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x30E);
        if(result) return result;
    }
    
    /* configure LINK TIME DURATION for the FE mode */
    if (mode == LSCI_FE_MAC_TEMINATION) {
        addr = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_LNK_TM_DUR;
        addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
        msk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_LNK_TM_DUR_LINK_TIME_DUR_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, linkTimeVal);
        if(result) return result;
    }

    /* enable/disable the Auto-Negotiation feature */
    addr = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_MII_CTL;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    bitOff = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_CTL_AN_ENABLE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitOff, anEnblVal);
    if(result) return result;

    if (anEnblVal) {
        /* configure MII AUTO-NEGOTIATION ADVERTISEMENT register (base page) */
        addr = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_MII_AUTO_NEG_AD;
        addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
        regData = 0x0000;

        if (mode == LSCI_SGMII_GE_MAC_TEMINATION        ||
            mode == LSCI_SGMII_FE_MAC_TEMINATION        ||
            mode == LSCI_SGMII_GE_MAC_TEMINATION_MANACK ||
            mode == LSCI_SGMII_FE_MAC_TEMINATION_MANACK) {

            msk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_BIT0_MSK;
            result = hyPhy20gVarFieldWrite(&regData, msk, 1);
            if(result) return result;

        } else if (mode == LSCI_GE_MAC_TEMINATION ||
                   mode == LSCI_GE_MAC_TEMINATION_AN_NEXTPG) {

            msk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_NEXT_PAGE_MSK;
            result = hyPhy20gVarFieldWrite(&regData, msk, locNextPgVal);
            if(result) return result;

            msk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_HALF_DUPLEX_MSK;
            result = hyPhy20gVarFieldWrite(&regData, msk, 0);
            if(result) return result;

            msk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_FULL_DUPLEX_MSK;
            result = hyPhy20gVarFieldWrite(&regData, msk, 1);
            if(result) return result;
        }

        DBG_PRINT("%s, %s, %d, value 0x%x to write to base page register.\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)regData);

        result = sysHyPhy20gWrite(fileHndl, addr, regData);
        if(result) return result;

        /* load the base page setting to the core */
        addr = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_CFG_AND_STAT;
        addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
        msk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_AN_ABLTY_ADV_UPD_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
        if(result) return result;

        /* Wait for AN_ABLTY_ADV_UPD bit to self-cleared */
        result = sysHyPhy20gPollBit(fileHndl, addr, msk,
                                    0, HYPHY20G_DELAY_20USEC);
        if(result) return result;
    }

    /*
     * configure L1TPP block
     */
    addr = HYPHY20G_LSCI_ENET_FEGE_L1TPP_REG_CFG;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk  = HYPHY20G_LSCI_ENET_FEGE_L1TPP_BIT_CFG_MODE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, l1tppMdVal);
    if(result) return result;

    /* enable L1TPP block */
    bitOff = HYPHY20G_LSCI_ENET_FEGE_L1TPP_BIT_CFG_L1TPP_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitOff, 1);
    if(result) return result;

    /* enable L1RPP block */
    addr = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_CFG_AND_STAT;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    bitOff = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_L1RPP_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitOff, 1);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciFeGeModeCfg */




/*******************************************************************************
**
**  hyPhy20gLsciGeAnNextPageCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function must be called to configure auto-negotiation
**                  NEXT PAGE TRANSMIT register if local next page ability
**                  (LOC_NEXT_PAGE bit) is enabled.
**
**                  It is user's responsibility to ensure this function is only
**                  used in GE MAC Terminated mode and auto-negotiation is
**                  enabled with next page exchange.
**
**                  It is user's responsibility to ensure this function is
**                  called after AN_STATE bit reaches the state '0x5' while
**                  the AN_COMPLETE bit is b'0. Otherwise this function can
**                  go into infinite loop.
**
**                  When the LSCI slice is configured in GE MAC Terminated mode
**                  and auto-negotiation is enabled with next page exchange,
**                  user must call this API to initate next page exchanges,
**                  and complete the auto-negotiation.
**
**                  The 'nextPg' parameter can set to b'1 to initate
**                  N-rounds of next page exchange as user desired, but
**                  user must call this API with 'nextPg' == 0, and with the
**                  "NULL Message Code" ('msgType' == 1 and 'msgCode' == 0x1)
**                  to end next page transmission and to complete the
**                  auto-negotiation.
**
**                  Refer to IEEE 802.3 2000 clause 28 for details of the next
**                  page register fields definition and the next page exchange.
**
**                  Refer to IEEE 802.3 2000 Annex 28C for details of message
**                  code field encoding.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**                  nextPg    - whether request next page transmission
**                              0 - no subsequent next pages required
**                              1 - request next page transmission
**                  msgType   - determine the page type
**                              0 - Unformatted Page
**                              1 - Message Page
**                  ack2      - whether local device has ability to comply
**                              with the message
**                              0 - local device cannot comply with message
**                              1 - local device will comply with message
**                  msgCode   - 11-bits data that is sent in the next page
**                              When msgType == 0, it is Unformatted Code
**                              Fields, which can be set to arbitrary value;
**                              When msgType == 1, it is formatted pages that
**                              carry a predefined Message Code which is
**                              enumerated in IEEE 802.3 2000 / Annex 28C.
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciGeAnNextPageCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 nextPg, UINT4 msgType, UINT4 ack2,
                                 UINT4 msgCode)
{
    INT4 result;
    UINT4 stateAddr;
    UINT4 stateMsk;
    UINT4 stateVal;
    UINT4 completeAddr;
    UINT4 completeOff;
    UINT4 completeVal;
    UINT4 nextPgAddr;
    UINT4 nextPgMsk;
    UINT4 msgTypeMsk;
    UINT4 ack2Msk;
    UINT4 msgCodeMsk;
    UINT4 regData;
    const UINT4 AN_NOT_COMPLETE = 0;
    const UINT4 AN_STATE_COMPLETE_ACK = 5;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 ||
        nextPg > 1  ||
        msgType > 1 ||
        ack2 > 1    ||
        msgCode > 0x07FF) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    stateAddr      = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_DBG_SM_STAT;
    stateAddr      = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(stateAddr, slice);
    stateMsk       = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_DBG_SM_STAT_AN_STATE_MSK;
    completeAddr   = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_MII_STAT;
    completeAddr   = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(completeAddr, slice);
    completeOff    = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_STAT_AN_COMPLETE_OFF;

    /* poll until AN_COMPLETE bit is 0x0 and AN_STATE is 0x5 */
    do {
        /* Wait */
        result = sysHyPhy20gMsDelay(500);
        if(result) return result;

        result = sysHyPhy20gBitRead(fileHndl, completeAddr,
                                     completeOff, &completeVal);
        if(result) return result;

        result = hyPhy20gRegFieldRead(fileHndl, stateAddr, stateMsk, &stateVal);
        if(result) return result;

    } while (!(completeVal == AN_NOT_COMPLETE &&
             stateVal == AN_STATE_COMPLETE_ACK));

    nextPgAddr = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_MII_AUTO_NEG_NXT_PG_TX;
    nextPgAddr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(nextPgAddr, slice);
    nextPgMsk  = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_NXT_PG_TX_LOC_NP_NEXT_PAGE_MSK;
    msgTypeMsk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_NXT_PG_TX_LOC_NP_MSG_PAGE_MSK;
    ack2Msk    = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_NXT_PG_TX_LOC_NP_ACK2_MSK;
    msgCodeMsk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_NXT_PG_TX_LOC_NP_CODE_FIELD_MSK;

    /* configure MII AUTO-NEGOTIATION NEXT PAGE TRANSMIT register */
    result = sysHyPhy20gRead(fileHndl, nextPgAddr, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, nextPgMsk, nextPg);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, msgTypeMsk, msgType);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, ack2Msk, ack2);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&regData, msgCodeMsk, msgCode);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, nextPgAddr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciGeAnNextPageCfg */


/*******************************************************************************
**
**  hyPhy20gLsciSgmiiAnBpAbilityCompare
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function is used in SGMII AUTO-NEGOTIATION mode, when
**                  the HyPHY 20G device acts as MAC, and interfaces with a
**                  external PHY device.
**
**                  This function compares local device (MAC) ability with
**                  link partner (PHY) ability conveyed in Control Information,
**                  which is observed through AUTO-NEGOTIATION LINK PARTNER
**                  BASE PAGE ABILITY register (register 0x48840A4).
**                  The MAC and PHY Control Information register encoding are
**                  defined in Table 1, "Serial-GMII Specification".
**
**                  This comparison applies in the SGMII FE and SGMII GE modes
**                  when the manual ACKNOWLEDGE feature is enabled (see the
**                  mode '6' and '7' of hyPhy20gLsciFeGeModeCfg API).
**
**                  If the ability between MAC and PHY is matched, this function
**                  will manually enable device's ACKNOWLEDGE insertion
**                  (bit 14 - ACK), which allows AUTO-NEGOTIATION state
**                  machine to move forward from ABILITY_DETECT_STATE to
**                  ACKNOWLEDGE_DETECT.
**
**                  If the ability between MAC and PHY mis-matches, this
**                  function will return a specific ability mismatch reason,
**                  and doesn't insert ACKNOWLEDGE (bit 14 - ACK).
**                  In this case, AUTO-NEGOTIATION state machine will remain
**                  in the ABILITY_DETECT_STATE, and AUTO-NEGOTIATION can not
**                  complete. Therefore, consequential action is required to
**                  re-configure either MAC or PHY, depending on the specific
**                  mismatch reason.
**                  Note, both the ability mismatch reason and corresponding
**                  consequential action are vendor specific. This function's
**                  implementation is PMC-Sierra specific.
**
**                  The following comparison result and consequential action
**                  are defined as PMC-Sierra's specific policy:
**
**                  0) HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_MATCH
**                     PHY's ability encoding is standard compliant,
**                     and matches with MAC's ability;
**                     consequentially insert ACKNOWLEDGE in MAC
**                     MAC (ENET_FEGE block);
**                     hyPhy20gLsciSgmiiAnBpAbilityCompare API implemented
**                     this consequential action;
**
**                  1) HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_NOT_DEFINED
**                     PHY's ability encoding is not standard compliant, like
**                     the bit 0 is not b'1;
**                     consequentially re-configure the PHY;
**                     this operation is not a part of HyPHY 20G device
**                     configuration;
**
**                  2) HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_SPEED_MM
**                     MAC speed setting (1000 Mbps or 100 Mbps) mis-matchs
**                     with the PHY's;
**                     consequentially re-configure the MAC (ENET_FEGE block)
**                     speed setting, with the assumption that MAC's speed
**                     setting should match with the PHY's;
**                     hyPhy20gLsciFeGeModeCfg API can be used to
**                     re-configure the speed setting;
**
**                  3) HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_DPLXMODE_MM
**                     PHY duplex mode setting mis-matchs with MAC
**                     (ENET_FEGE block), which supports only full duplex mode;
**                     consequentially re-configure the PHY;
**                     this operation is not a part of HyPHY 20G device
**                     configuration;
**
**                  4) HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_LINKDOWN
**                     PHY link down state is received;
**                     consequentially re-configure the PHY to "link up" state;
**                     this operation is not a part of HyPHY 20G device
**                     configuration;
**
**                  User must call this function only when:
**                    1) ENET_FEGE block is configured in mode '6' or '7'
**                       by hyPhy20gLsciFeGeModeCfg API, and
**                    2) AUTO_NEG_LP_ABLTY_UPD_I bit is asserting.
**
**                  It is user's responsibility to re-configure either MAC
**                  or PHY if their ability mis-match.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**
**  OUTPUTS:        *mmCode   - pointer to storage for returned comparison
**                              results.
**                              Each bit of the return value corresponds to
**                              a specific mismatch reason:
**                              bit 0 - NOT_DEFINED
**                              bit 1 - SPEED_MM
**                              bit 2 - DPLXMODE_MM
**                              bit 3 - LINKDOWN
**                              And the bit value definition is:
**                              0 - no the specific mismatch or failure
**                              1 - detect the specific mismatch or failure
**                              The 4 all zero bits consider as matched.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gLsciSgmiiAnBpAbilityCompare(struct file *fileHndl, UINT4 slice,
                                         UINT4 *mmCode)
{
    INT4 result;
    UINT4 addr;
    UINT4 ackMsk;
    UINT4 updOff;
    UINT4 updMsk;
    UINT4 regData;
    UINT4 lnkStatMsk;
    UINT4 rsvd0Msk;
    UINT4 dplxMdMsk;
    UINT4 speedMsk;
    UINT4 rsvd1Msk;
    UINT4 lnkStatVal;
    UINT4 rsvd0Val;
    UINT4 dplxMdVal;
    UINT4 speedVal;
    UINT4 rsvd1Val;
    UINT4 fegeMdVal;
    UINT4 feGeMdMsk;
    UINT4 locSpeedVal;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (mmCode == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Retrieve ENET_FEGE mode and check pre-conditions */
    addr = HYPHY20G_LSCI_ENET_FEGE_TOP_REG_MD_CFG;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    feGeMdMsk = HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_FEGE_MODE_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, addr, feGeMdMsk, &fegeMdVal);
    if(result) return result;

    switch (fegeMdVal) {
        case 0x3:
            locSpeedVal  = 0x2;
            break;
        case 0x4:
            locSpeedVal  = 0x1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Retrieve AUTO-NEGOTIATION LINK PARTNER BASE PAGE ABILITY value */
    addr = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_MII_AUTO_NEG_LNK_PRT_BAS_PG_ABL;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    lnkStatMsk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_LINK_STATUS_MSK;
    rsvd0Msk   = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_RSVD_0_MSK;
    dplxMdMsk  = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_DUPLEX_MODE_MSK;
    speedMsk   = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_SPEED_MSK;
    rsvd1Msk   = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_RSVD_1_MSK;

    result = hyPhy20gVarFieldRead(regData, lnkStatMsk, &lnkStatVal);
    if(result) return result;

    result = hyPhy20gVarFieldRead(regData, rsvd0Msk, &rsvd0Val);
    if(result) return result;

    result = hyPhy20gVarFieldRead(regData, dplxMdMsk, &dplxMdVal);
    if(result) return result;

    result = hyPhy20gVarFieldRead(regData, speedMsk, &speedVal);
    if(result) return result;

    result = hyPhy20gVarFieldRead(regData, rsvd1Msk, &rsvd1Val);
    if(result) return result;

    /* init mismatch return code to 'HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_MATCH' */
    *mmCode = HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_MATCH;

    /* update mismatch return code if there is any condition matched */
    if (rsvd1Val == 0) {
        *mmCode |= HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_NOT_DEFINED;
    }

    if (lnkStatVal == 0) {
        *mmCode |= HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_LINKDOWN;
    }

    if (dplxMdVal != 1) {
        *mmCode |= HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_DPLXMODE_MM;
    }

    if (speedVal != locSpeedVal) {
        *mmCode |= HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_SPEED_MM;
    }

    /*
     * return if there is any mismatch detected
     */
    if (*mmCode != HYPHY20G_LSCI_AUTO_NEG_LP_ABLTY_MATCH) {
        return HYPHY20G_SUCCESS;
    }

    /*
     * enable device's ACKNOWLEDGE insertion
     */
    ackMsk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_ACK_MSK;
    updOff = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_AN_ABLTY_ADV_UPD_OFF;
    updMsk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_AN_ABLTY_ADV_UPD_MSK;

    /* write ACKNOWLEDGE, bit-14 to b'1 in AUTO-NEGOTIATION
       ADVERTISEMENT register (base page) */
    addr = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_MII_AUTO_NEG_AD;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&regData, ackMsk, 1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    /* load the new base page value to the L1RPP core */
    addr = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_CFG_AND_STAT;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr, updOff, 1);
    if(result) return result;

    /* Wait for AN_ABLTY_ADV_UPD bit to self-cleared */
    result = sysHyPhy20gPollBit(fileHndl, addr, updMsk,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciSgmiiAnBpAbilityCompare */


/*******************************************************************************
**
**  hyPhy20gLsciFeGeTmacCfg1
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure TMAC block Configuration 1 register within an
**                  ENET_FEGE slice.
**
**                  Refer to 0x4886000 + 0x08000*A(A=0:15) for further details
**                  on the functionalities configured with this function.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gLsciInit but prior to hyPhy20gLsciDataPathEnable.
**
**                  The default values provided in this description are
**                  relative to the state of the device after calling
**                  hyPhy20gLsciInit and hyPhy20gLsciFeGeModeCfg.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**                  crcCkEn   - enable/disable checking the FCS field
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  padAddEn  - select the pad insertion:
**                              00 - no pad insertion
**                              01 - insert pad and 4-byte CRC when the
**                                   MAC frame is short but at least 14 bytes in
**                                   length (including DA,SA and Type/Length).
**                              10 - insert pad and 4-byte CRC when the
**                                   MAC frame is short (no 14 bytes in
**                                   length requirement)
**                              Default setting b'00
**                  crcAddEn  - enable/disable 32-bit CRC append
**                              0 - disable
**                              1 - enable
**                              Default setting 0
**                  minFrmCkEn - enable/disable checking the minimum frame length
**                               0 - disable
**                               1 - enable
**                               Default setting 1
**                  truncEn   - enable/disable truncating the MAC frame
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  maxFrmCkEn - enable/disable checking the maximum frame length
**                               0 - disable
**                               1 - enable
**                               Default setting 1
**                  ibufHthrEn - enable/disable outgoing flow control of
**                               IBUF_HTHR request
**                               0 - disable
**                               1 - enable
**                               Default setting 1
**                  xonEn     - enable/disable sending an XON PAUSE
**                              control frame
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  rmacPauseEn - enable/disable pause request upon the
**                                completion of the current packet transmission
**                                0 - disable
**                                1 - enable
**                                Default setting 1
**                  ebufErrEn - enable/disable checking EBUF_ERR from the
**                              upstream and passes it along with packet data
**                              to downstream.
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciFeGeTmacCfg1(struct file *fileHndl, UINT4 slice,
                              UINT4 crcCkEn, UINT4 padAddEn,
                              UINT4 crcAddEn, UINT4 minFrmCkEn,
                              UINT4 truncEn, UINT4 maxFrmCkEn,
                              UINT4 ibufHthrEn, UINT4 xonEn, UINT4 rmacPauseEn,
                              UINT4 ebufErrEn)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 ||
        crcCkEn > 1     ||
        padAddEn > 2    ||
        crcAddEn > 1    ||
        minFrmCkEn > 1  ||
        truncEn > 1     ||
        maxFrmCkEn > 1  ||
        ibufHthrEn > 1  ||
        xonEn > 1       ||
        rmacPauseEn > 1 ||
        ebufErrEn > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TMAC_REG_CFG_1, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, crcCkEn,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_CRC_CK_MSK,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_CRC_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, padAddEn,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_PAD_ADD_EN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_PAD_ADD_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, crcAddEn,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_CRC_ADD_EN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_CRC_ADD_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, minFrmCkEn,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_MIN_FRM_CK_MSK,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_MIN_FRM_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, truncEn,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_TRUNC_EN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_TRUNC_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, maxFrmCkEn,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_MAX_FRM_CK_MSK,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_MAX_FRM_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, ibufHthrEn,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_IBUF_HTHR_EN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_IBUF_HTHR_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, xonEn,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_XON_EN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_XON_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, rmacPauseEn,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_RMAC_PAUSE_EN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_RMAC_PAUSE_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, ebufErrEn,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_EBUF_ERR_EN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_EBUF_ERR_EN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciFeGeTmacCfg1 */

/*******************************************************************************
**
**  hyPhy20gLsciFeGeTmacCfg2
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure TMAC block Configuration 2 register within an
**                  ENET_FEGE slice.
**
**                  Refer to 0x4886004 + 0x08000*A(A=0:15) for further details
**                  on the functionalities configured with this function.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gLsciInit but prior to hyPhy20gLsciDataPathEnable.
**
**                  The default values provided in this description are
**                  relative to the state of the device after calling
**                  hyPhy20gLsciInit and hyPhy20gLsciFeGeModeCfg.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**                  sfdEn     - enable/disable insertion of the SFD field
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  pambLen   - specify length in octet of the Preamble field
**                              b'0000 -  0 octet  (full duplex)
**                              b'0001 -  1 octet
**                              b'0010 -  2 octets
**                              ......
**                              b'1111 - 15 octets
**                              Default setting b'0111
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciFeGeTmacCfg2(struct file *fileHndl, UINT4 slice,
                              UINT4 sfdEn, UINT4 pambLen)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 ||
        sfdEn > 1       ||
        pambLen > 15) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TMAC_REG_CFG_2, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, sfdEn,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_2_SFD_EN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_2_SFD_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, pambLen,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_2_PAMB_LEN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_2_PAMB_LEN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciFeGeTmacCfg2 */

/*******************************************************************************
**
**  hyPhy20gLsciFeGeTxIpgCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure the length in octet of the Inter-packet Gap. The
**                  configuration is applied to the TMAC block.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gLsciInit but prior to hyPhy20gLsciDataPathEnable.
**
**                  The default values provided in this description are
**                  relative to the state of the device after calling
**                  hyPhy20gLsciInit and hyPhy20gLsciFeGeModeCfg.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**                  txIPG1    - Transmit InterPacket in bit time
**                              Legal range 0 to 15
**                               0 - 128 in bit time
**                               1 -   8 in bit time
**                               2 -  16 in bit time
**                               ......  in bit time
**                              15 - 120 in bit time
**                              Default setting b'1100
**                  txIPG2    - Transmit InterPacket in bit time
**                              Legal range 0 to 15
**                               0 - 128 in bit time
**                               1 -   8 in bit time
**                               2 -  16 in bit time
**                               ......  in bit time
**                              15 - 120 in bit time
**                              Default setting b'1000
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciFeGeTxIpgCfg(struct file *fileHndl, UINT4 slice,
                              UINT4 txIPG1, UINT4 txIPG2)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 ||
        txIPG1 > 15     ||
        txIPG2 > 15) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TMAC_REG_INTR_PAC_GAP_CFG_2, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, txIPG1,
        HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_INTR_PAC_GAP_CFG_2_FIPG1_LEN_MSK,
        HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_INTR_PAC_GAP_CFG_2_FIPG1_LEN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, txIPG2,
        HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_INTR_PAC_GAP_CFG_2_FIPG2_LEN_MSK,
        HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_INTR_PAC_GAP_CFG_2_FIPG2_LEN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciFeGeTxIpgCfg */


/*******************************************************************************
**
**  hyPhy20gLsciFeGeRmacCfg1
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure RMAC block Configuration 1 register within an
**                  ENET_FEGE slice.
**
**                  Refer to 0x4884100 + 0x08000*A(A=0:15) for further details
**                  on the functionalities configured with this function.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gLsciInit but prior to hyPhy20gLsciDataPathEnable.
**
**                  The default values provided in this description are
**                  relative to the state of the device after calling
**                  hyPhy20gLsciInit and hyPhy20gLsciFeGeModeCfg.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**                  preambleCkEn - enable/disable preamble check
**                                 0 - disable
**                                 1 - enable
**                                 Default setting 0
**                  tagAwareEn - enable/disable Tagging Extension Aware
**                               0 - disable
**                               1 - enable
**                               Default setting 1
**                  inrangeDisEn - enable/disable in-range length error discard
**                                 0 - disable
**                                 1 - enable
**                                 (if inrangeCk is non-zero)
**                                 Default setting 1
**                  inrangeCk - In-range length error checking configuration
**                              b'00 - no checks performed
**                              b'01 - Reserved
**                              b'10 - Reserved
**                              b'11 - Mark frame with in-range length error
**                                     if:
**                                     length/type field contains a value from
**                                     46 to 1500 and the length/type
**                                     field != MAC payload size; OR
**                                     length/type field < 46 AND the MAC
**                                     payload size is > 46.
**                              Default setting b'11
**                  maxFrmTrnEn - enable/disable maximum frame length truncate
**                                0 - disable
**                                1 - enable
**                                Default setting 1
**                  maxFrmDisEn - enable/disable maximum frame length discard
**                                0 - disable
**                                1 - enable
**                                Default setting 1
**                  maxFrmCkEn - enable/disable maximum frame length check
**                               0 - disable
**                               1 - enable
**                               Default setting 1
**                  minFrmDisEn
**                            - enable/disable minimum frame length discard
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  minFrmCkEn - enable/disable minimum frame length check
**                               0 - disable
**                               1 - enable
**                               Default setting 1
**                  crcDisEn  - enable/disable CRC error discard
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  crcCkEn   - enable/disable CRC check
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciFeGeRmacCfg1(struct file *fileHndl, UINT4 slice,
                              UINT4 preambleCkEn, UINT4 tagAwareEn,
                              UINT4 inrangeDisEn, UINT4 inrangeCk,
                              UINT4 maxFrmTrnEn, UINT4 maxFrmDisEn,
                              UINT4 maxFrmCkEn, UINT4 minFrmDisEn,
                              UINT4 minFrmCkEn, UINT4 crcDisEn,
                              UINT4 crcCkEn)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 ||
        preambleCkEn > 1    ||
        tagAwareEn > 1      ||
        inrangeDisEn > 1    ||
        inrangeCk > 3       ||
        maxFrmTrnEn > 1     ||
        maxFrmDisEn > 1     ||
        maxFrmCkEn > 1      ||
        minFrmDisEn > 1     ||
        minFrmCkEn > 1      ||
        crcDisEn > 1        ||
        crcCkEn > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (inrangeCk == 1 || inrangeCk == 2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_RMAC_REG_CFG_1, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, preambleCkEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_PAMB_CK_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_PAMB_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, tagAwareEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_TAG_AWARE_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_TAG_AWARE_OFF);

    val = mHYPHY20G_SET_FIELD4(val, inrangeDisEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_INRANGE_DIS_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_INRANGE_DIS_OFF);

    val = mHYPHY20G_SET_FIELD4(val, inrangeCk,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_INRANGE_CK_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_INRANGE_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, maxFrmTrnEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_TRN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_TRN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, maxFrmDisEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_DIS_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_DIS_OFF);

    val = mHYPHY20G_SET_FIELD4(val, maxFrmCkEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_CK_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, minFrmDisEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_MIN_FRM_DIS_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_MIN_FRM_DIS_OFF);

    val = mHYPHY20G_SET_FIELD4(val, minFrmCkEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_MIN_FRM_CK_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_MIN_FRM_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, crcDisEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_CRC_DIS_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_CRC_DIS_OFF);

    val = mHYPHY20G_SET_FIELD4(val, crcCkEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_CRC_CK_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_CRC_CK_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciFeGeRmacCfg1 */

/*******************************************************************************
**
**  hyPhy20gLsciFeGeRmacCfg2
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure RMAC block Configuration 2 register within an
**                  ENET_FEGE slice.
**
**                  Refer to 0x4884104 + 0x08000*A(A=0:15) for further details
**                  on the functionalities configured with this function.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gLsciInit but prior to hyPhy20gLsciDataPathEnable.
**
**                  The default values provided in this description are
**                  relative to the state of the device after calling
**                  hyPhy20gLsciInit and hyPhy20gLsciFeGeModeCfg.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**                  interPktL1En - 0 - disable interpacket Layer 1 error report
**                                 1 - enable interpacket Layer 1 error report
**                                 Default setting 0
**                  padStripEn - 0 - don't pad
**                               1 - pad strip non-errored packets
**                               Note: rxCRCStrip must also be set when pad
**                                     stripping is enabled
**                               Default setting 0
**                  crcStripEn - 0 - don't strip received CRC from forwarded frame
**                               1 - strip rxed CRC from forwarded frame
**                               Default setting 0
**                  carrMonEn - Carrier information processing
**                              0 - monitors for false carrier and
**                                  errored packet
**                              1 - in addition to false carrier and errored
**                                  packet, monitors for L1RPP_ER input
**                              Default setting 0
**                              Refer to CARRIER_EN bit in register description
**                              for further details.
**                  pauseFrmEn - enable/disable Pause Control frame reception
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  pauseFrmDropEn - 0 - forward PAUSE frames
**                                   1 - drop PAUSE frames
**                                   Default setting 0
**                  ctlFrmDropEn - 0 - forward control frames (except PAUSE 
**                                     frames)
**                                 1 - drop control frames (except PAUSE frames)
**                                 Default setting 0
**                  desAddrDropEn - 0 - forward all MAC frames containing 
**                                      a Unicast address
**                                  1 - forward only if destination address 
**                                      field contains a Unicast address and 
**                                      the destination destination field 
**                                      matches the station address
**                                  Default setting 0
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciFeGeRmacCfg2(struct file *fileHndl, UINT4 slice,
                              UINT4 interPktL1En, 
                              UINT4 padStripEn, UINT4 crcStripEn,
                              UINT4 carrMonEn, UINT4 pauseFrmEn,
                              UINT4 pauseFrmDropEn, UINT4 ctlFrmDropEn,
                              UINT4 desAddrDropEn)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 ||
        interPktL1En > 1    ||
        padStripEn > 1      ||
        crcStripEn > 1      ||
        carrMonEn > 1       ||
        pauseFrmEn > 1      ||
        pauseFrmDropEn > 1  ||
        ctlFrmDropEn > 1    ||
        desAddrDropEn > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_RMAC_REG_CFG_2, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, interPktL1En,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_INTERPKT_L1_EN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_INTERPKT_L1_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, padStripEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_PAD_STRIP_EN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_PAD_STRIP_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, crcStripEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_CRC_STRIP_EN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_CRC_STRIP_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, carrMonEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_CARRIER_EN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_CARRIER_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, pauseFrmEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_INPAUSE_EN_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_INPAUSE_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, pauseFrmDropEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_PAUSEFRM_DROP_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_PAUSEFRM_DROP_OFF);

    val = mHYPHY20G_SET_FIELD4(val, ctlFrmDropEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_CTRLFRM_DROP_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_CTRLFRM_DROP_OFF);

    val = mHYPHY20G_SET_FIELD4(val, desAddrDropEn,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_DA_DROP_MSK,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_DA_DROP_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciFeGeRmacCfg2 */

/*******************************************************************************
**
**  hyPhy20gLsciFeGeSetStationAddr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Changes the channel station MAC address in the RMAC
**                  and TMAC blocks. This function requires that traffic on the
**                  channel to be stopped.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gLsciInit but prior to hyPhy20gLsciDataPathEnable.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**                  hiSA      - Station Address High
**                              Legal range 0 to 255
**                  midSA     - Station Address Mid
**                              Legal range 0 to 255
**                  lowSA     - Station Address Low
**                              Legal range 0 to 255
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciFeGeSetStationAddr(struct file *fileHndl, UINT4 slice,
                                    UINT4 hiSA, UINT4 midSA, UINT4 lowSA)
{
    INT4 result;
    UINT4 addr;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 ||
        hiSA > 0xFFFF ||
        midSA > 0xFFFF ||
        lowSA > 0xFFFF) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Write Station Address to RMAC */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_RMAC_REG_STN_ADR_HI_WD_CFG, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_STN_ADR_HI_WD_CFG_STA_MSK, hiSA);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_RMAC_REG_STN_ADR_MID_WD_CFG, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_STN_ADR_MID_WD_CFG_STA_MSK, midSA);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_RMAC_REG_STN_ADR_LOW_WD_CFG, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_STN_ADR_LOW_WD_CFG_STA_MSK, lowSA);
    if(result) return result;

    /* Write Station Address to TMAC */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TMAC_REG_STN_ADR_HI_WD_CFG, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_STN_ADR_HI_WD_CFG_STA_MSK, hiSA);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TMAC_REG_STN_ADR_MID_WD_CFG, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_STN_ADR_MID_WD_CFG_STA_MSK, midSA);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TMAC_REG_STN_ADR_LOW_WD_CFG, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_STN_ADR_LOW_WD_CFG_STA_MSK, lowSA);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciFeGeSetStationAddr */

/*******************************************************************************
**
**  hyPhy20gLsciFeGeSetFrameSizes
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function writes new min/max frame sizes for the MAC in
**                  the RMAC and TMAC blocks.
**
**                  Set Maximum Frame Size to 9618 bytes for jumbo frames;
**                  Set Maximum Frame Size to 9622 bytes for VLAN_DOUBLE_TAG
**                  jumbo frames.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gLsciInit but prior to hyPhy20gLsciDataPathEnable.
**
**  INPUTS:         fileHndl  - device handle
**                  txMaxFrameSize
**                            - the maximum MAC frame length in octet from the
**                              first byte of Destination Address to the last
**                              byte of Frame Check Sequence
**                              legal range (txMinFrameSize+4) - 0xFFFC
**                  txMinFrameSize
**                            - minimum MAC frame length in octet from the
**                              first byte of Destination Address to the last
**                              byte of Frame Check Sequence
**                              legal range 0x16 - (txMaxFrameSize-4)
**                  rxMaxFrameSize
**                            - the value used during comparison for maximum
**                              length received frame errors within the RMAC.
**                              this value should not be excess 0xFFFB
**                              The frame length in bytes (comprising the DA,
**                              SA, Length/Type, MAC Client Data,
**                              Pad and FCS fields)
**                  rxMinFrameSize
**                            - the value used during comparison for minimum
**                              length received frame errors within the RMAC.
**                              The frame length in bytes (comprising the DA,
**                              SA, Length/Type, MAC Client Data,
**                              Pad and FCS fields)
**                              0  - 64 octets
**                              1  -  1 octets
**                              2  -  2 octets
**                              ......
**                              63 - 63 octets
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciFeGeSetFrameSizes(struct file *fileHndl, UINT4 slice,
                                   UINT4 txMaxFrameSize, UINT4 txMinFrameSize,
                                   UINT4 rxMaxFrameSize, UINT4 rxMinFrameSize)
{
    INT4 result;
    UINT4 addr;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 ||
        rxMaxFrameSize > HYPHY20G_LSCI_ENET_FEGE_RXMAXFRMSIZE_UPPERLT ||
        rxMinFrameSize > HYPHY20G_LSCI_ENET_FEGE_RXMINFRMSIZE_UPPERLT ||
        txMinFrameSize + 4 > txMaxFrameSize ||
        txMaxFrameSize > HYPHY20G_LSCI_ENET_FEGE_TXMAXFRMSIZE_UPPERLT - 1) {
		return HYPHY20G_ERR_INVALID_ARG;
    }

    if (txMinFrameSize <= HYPHY20G_LSCI_ENET_FEGE_TXMINFRMSIZE_LOWERLT) {
        DBG_PRINT("%s, %s, %d, strongly recommend that txMinFrameSize set larger than 0x16. Otherwise It may bring unspecified effects.\n",__FILE__, __FUNCTION__, __LINE__);
	}

    /* Write max and min frame size to RMAC */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_RMAC_REG_MAX_FRM_LEN_CFG, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_MAX_FRM_LEN_CFG_MAX_FRM_LEN_MSK,
                rxMaxFrameSize);
    if(result) return result;

    addr = HYPHY20G_LSCI_ENET_FEGE_RMAC_REG_CFG_2;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_MIN_FRM_LEN_MSK,
                rxMinFrameSize);
    if(result) return result;

    /* Write max and min frame size to TMAC */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TMAC_REG_MAX_FRM_LEN_CFG, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_STN_ADR_LOW_WD_CFG_STA_MSK,
                txMaxFrameSize);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TMAC_REG_MIN_FRM_LEN_CFG, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_MAX_FRM_LEN_CFG_MAX_FRM_LEN_MSK,
                txMinFrameSize);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciFeGeSetFrameSizes */


/*******************************************************************************
**
**  hyPhy20gLsciGeGfpTModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs payload specific configuration
**                  for a LSCI slice configured for GE_GFPT mode.
**
**                  Used for GE GFP-T mode.
**
**                  This function is no longer used.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - LSCI slice processing resource
**                                    slice number 0 - 15
**                  rxResyncCnt     - number of valid characters must be
**                                    received to reduce the LOS counter by one.
**                                    Legal range 0x0 to 0xF
**                                  - Default value 0x4
**                  rxLosCnt        - number of invalid characters must be
**                                    received to enter the
**                                    LOSS-OF-SYNCHRONIZATION state.
**                                    Legal range 0x0 to 0xF
**                                  - Default value 0x4
**                  rxSyncCnt       - number of synchronization characters that
**                                    must be received correctly without
**                                    intervening errors to enter the
**                                    SYNCHRONIZATION-ACQUIRED state.
**                                    Legal range 0x0 to 0xF
**                                  - Default value 0x3
**                  txResyncCnt     - number of valid characters that must be
**                                    received from Central Packet Buffer to
**                                    reduce the LOS counter by one.
**                                    Legal range 0x0 to 0xF
**                                  - Default value 0x4
**                  txLosCnt        - number of invalid characters that must be
**                                    received from Central Packet Buffer to
**                                    enter the LOSS-OF-SYNCHRONIZATION state.
**                                    Legal range 0x0 to 0xF
**                                  - Default value 0x4
**                  txSyncCnt       - number of synchronization characters that
**                                    must be received correctly from system
**                                    interface without intervening errors to
**                                    enter the SYNCHRONIZATION-ACQUIRED state.
**                                    Legal range 0x0 to 0xF
**                                  - Default value 0x3
**                  disInvEn        - enable/disable the disparity inversion
**                                    function
**                                    0 - disable
**                                    1 - enable
**                                  - Default value 0x3
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciGeGfpTModeCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 rxResyncCnt, UINT4 rxLosCnt,
                               UINT4 rxSyncCnt,
                               UINT4 txResyncCnt, UINT4 txLosCnt,
                               UINT4 txSyncCnt, UINT4 disInvEn)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 rxCfgAddr = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_RX_CFG;
    UINT4 txCfgAddr = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_TX_CFG;
    UINT4 devId, revId, status;

    /* This API is no longer used. Return SUCCESS */
    return HYPHY20G_SUCCESS;
    
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 ||
        rxResyncCnt > 0xF ||
        rxLosCnt > 0xF ||
        rxSyncCnt > 0xF ||
        txResyncCnt > 0xF ||
        txLosCnt > 0xF ||
        txSyncCnt > 0xF ||
        disInvEn > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* GE_GFPT RX Configuration */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(rxCfgAddr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, rxResyncCnt,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_RESYNC_COUNT_MSK,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_RESYNC_COUNT_OFF);
    val = mHYPHY20G_SET_FIELD4(val, rxLosCnt,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_LOS_COUNT_MSK,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_LOS_COUNT_OFF);
    val = mHYPHY20G_SET_FIELD4(val, rxSyncCnt,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_SYNC_COUNT_MSK,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_SYNC_COUNT_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* GE_GFPT TX Configuration */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(txCfgAddr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, txResyncCnt,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_RESYNC_COUNT_MSK,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_RESYNC_COUNT_OFF);
    val = mHYPHY20G_SET_FIELD4(val, txLosCnt,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_LOS_COUNT_MSK,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_LOS_COUNT_OFF);
    val = mHYPHY20G_SET_FIELD4(val, txSyncCnt,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_SYNC_COUNT_MSK,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_SYNC_COUNT_OFF);
    val = mHYPHY20G_SET_FIELD4(val, disInvEn,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_DISP_INVERSION_EN_MSK,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_DISP_INVERSION_EN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciGeGfpTModeCfg */





/*******************************************************************************
**
**  hyPhy20gLsciCbrModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This dummy function may performs payload specific
**                  configuration for a slice configured for CBR
**                  mode in further.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciCbrModeCfg(struct file *fileHndl, UINT4 slice)
{   
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* TBD */

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciCbrModeCfg */

/*******************************************************************************
**
**  hyPhy20gLsciCbrLcvErrInsCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function sets the user configured 10-bit patterns and
**                controls whether or not they will be substituted for
**                received characters with encoding or running disparity erros.
**
**                This function is only applicable to 8b10b encoded CBR clients.
**
**                This function can optionally be called after
**                hyPhy20gLsciInit but prior to hyPhy20gLsciDataPathEnable.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - LSCI slice processing resource
**                                slice number 0 - 15
**                 dir          - data flow direction
**                                0 - transmit data flow
**                                    from the CPB to the SFP
**                                1 - receive data flow
**                                    from the SFP to the CPB
**                 mode         - data stream overwriting operation
**                                0 - characters with encoding or running
**                                    disparity errors will NOT be replaced
**                                    with the user configured 10 bit patterns
**                                1 - characters with encoding or running
**                                    disparity errors will be replaced
**                                    with the user configured 10 bit patterns
**                 err_patt_n   - user configured pattern to be optionally
**                                sent out.
**                                Default 0x0F1
**                 err_patt_p   - user configured pattern to be optionally
**                                sent out.
**                                Default 0x30E
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciCbrLcvErrInsCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 dir, UINT4 mode, UINT4 err_patt_n,
                                 UINT4 err_patt_p)
{
    INT4 result;
    UINT4 traffic;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 || dir > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (mode > 2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = lsciHyPhy20gLsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic < LSCI_BYTEALIGED_CBR_STS || traffic > LSCI_8B10B_TX_RD_INV_DIS_CBR) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* call private function */
    result = lsciHyPhy20gLsciFcCbrLcvErrInsCfg(fileHndl, slice, dir, mode,
                                               err_patt_n, err_patt_p);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gLsciCbrLcvErrInsCfg */


/*******************************************************************************
**
**  hyPhy20gLsciFcModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures ordered set in the 
**                  transmit path.
**
**                  This function also selects one of word alignment mode:
**                  1) Continuous-mode, in which the receiver is allowed to
**                  re-establish byte and word alignment at any point in the
**                  incoming bit stream.
**                  2) Explicit-mode, in which the receiver is allowed to to
**                  re-establish alignment under controlled circumstances,
**                  for example, in the loss of synchronization state.
**
**                  Used for FC-4G and subrate FC modes.
**
**                  This function can optionally be called after
**                  hyPhy20gLsciInit but prior to hyPhy20gLsciDataPathEnable.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**                  osCfgMsk  - configuration mask to control the ordered set 
**                              handling in the Tx path:
**                                bit 3 - configure RDY_UNKWN_EN bit
**                                bit 2 - configure UNKWN_OS_FW_EN bit
**                                bit 1 - configure ARB_INS_EN bit
**                                bit 0 - configure ARB_DEL_EN bit
**                              RDY_UNKWN_EN
**                              0 - R_RDY and VC_RDY will be treated as 
**                                  recognized primitive signals;
**                              1 - R_RDY and VC_RDY will be treated as 
**                                  unrecognized ordered sets;
**                              UNKWN_OS_FW_EN
**                              0 - Unknown ordered set cannot be considered as
**                                  a fill word during insertion and deletion;
**                              1 - Unknown ordered set can be considered as 
**                                  a fill word during insertion and deletion;
**                              ARB_INS_EN
**                              0 - ARB not to be considered as a fill word for
**                                  insertion
**                              1 - ARB can be considered as a fill word for
**                                  insertion
**                              ARB_DEL_EN
**                              0 - ARB not to be considered as a fill word for
**                                  deletion
**                              1 - ARB can be considered as a fill word for
**                                  deletion
**                              Default setting b'0000
**                  arbLsbMsk - defines the mask used to qualify the 
**                              least-significant 20-bits of ARB ordered set
**                              with respect to value in ARB_VALUE;
**                              bit x (19:0) of ARB_VALUE_EN:
**                              0 - The ARB_VALUE[x] bit is used in ARB 
**                                  operations;
**                              1 - The ARB_VALUE[x] bit is not used in ARB 
**                                  operations;
**                              Default setting 0xFFFFF
**                              Note: the 'arbLsbMsk' should be only used when
**                                    ARB_DEL_EN or ARB_INS_EN bit is b'1;
**                  arbLsbVal - defines the least-significant 20-bits value of 
**                              ARB ordered set
**                              Default setting 0xAC6B1
**                  rxAlignMd - Receive fibre channel ordered-set/delimeter
**                              alignment mode.
**                              0 - Explicit-mode (default)
**                              1 - Continuous-mode
**                  txAlignMd - Transmit fibre channel ordered-set/delimeter
**                              alignment mode.
**                              0 - Explicit-mode
**                              1 - Continuous-mode (default)
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciFcModeCfg(struct file *fileHndl, UINT4 slice, UINT4 osCfgMsk,
                           UINT4 arbLsbMsk, UINT4 arbLsbVal, UINT4 rxAlignMd, 
                           UINT4 txAlignMd)
{

    UINT4 devId, revId, status; 
    INT4 result;
    UINT4 addr, val;
    UINT4 offset;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 ||
        osCfgMsk > 0xF      || 
        arbLsbMsk > 0xFFFFF ||
        arbLsbVal > 0xFFFFF ||
        rxAlignMd > 1       ||
        txAlignMd > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* configure ordered set control register */
    addr = HYPHY20G_LSCI_FC_CBR_REG_OS_CTL;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gWrite(fileHndl, addr, osCfgMsk);
    if(result) return result;

    /* configure mask of least-significant 20-bits of ARB ordered set */
    addr = HYPHY20G_LSCI_FC_CBR_REG_ARB_VAL_EN;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gWrite(fileHndl, addr, arbLsbMsk);
    if(result) return result;

    /* configure least-significant 20-bits of ARB ordered set */
    addr = HYPHY20G_LSCI_FC_CBR_REG_ARB_VAL;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gWrite(fileHndl, addr, arbLsbVal);
    if(result) return result;

    /* configure RX_FC_CONT_ALIGN_MODE */
    addr = HYPHY20G_LSCI_FC_CBR_REG_RX_CFG;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, rxAlignMd,
                    HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_FC_CONT_ALIGN_MODE_MSK,
                    HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_FC_CONT_ALIGN_MODE_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* configure TX_FC_CONT_ALIGN_MODE */
    addr   = HYPHY20G_LSCI_FC_CBR_REG_TX_CFG;
    addr   = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    offset = HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_FC_CONT_ALIGN_MODE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, txAlignMd);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gLsciFcModeCfg */


/*******************************************************************************
**
**  hyPhy20gLsciFcLcvErrInsCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function sets the user configured 10-bit patterns and
**                controls whether or not they will be substituted for for
**                received characters with encoding or running disparity erros.
**
**                By default when calling hyPhy20gLsciInit 10 bit and 20 bit
**                patterns are configured to be standards compliant.  This
**                function allows for variation from the standards based
**                requirements.
**
**                10 bit patterns replace errored code words.  20 bit patterns
**                overwrite the data stream in the event of LOS.
**
**                For enable operation, this funciton will set
**                a specific type of pattern value and make the
**                new configured pattern value take effect.
**
**                This function can optionally be called after
**                hyPhy20gLsciInit but prior to hyPhy20gLsciDataPathEnable.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - LSCI slice processing resource
**                                slice number 0 - 15
**                 dir          - data flow direction
**                                0 - transmit data flow
**                                    from the CPB to the SFP
**                                1 - receive data flow
**                                    from the SFP to the CPB
**                 mode         - data stream overwriting operation
**                                0 - characters with encoding or running
**                                    disparity errors will NOT be replaced
**                                    with the user configured 10 bit patterns
**                                1 - characters with encoding or running
**                                    disparity errors will be replaced
**                                    with the user configured 10 bit patterns
**                 err_patt_n   - user configured pattern to be optionally
**                                sent out.
**                                Default 0x0F1
**                 err_patt_p   - user configured pattern to be optionally
**                                sent out.
**                                Default 0x30E
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciFcLcvErrInsCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                                UINT4 mode, UINT4 err_patt_n, UINT4 err_patt_p)
{
    INT4 result;
    UINT4 traffic;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 || dir > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (mode > 2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = lsciHyPhy20gLsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic != LSCI_FC_4G) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* call private function */
    result = lsciHyPhy20gLsciFcCbrLcvErrInsCfg(fileHndl, slice, dir, mode,
                                               err_patt_n, err_patt_p);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gLsciFcLcvErrInsCfg */

/*******************************************************************************
**
**  hyPhy20gLsciEsconModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This dummy function may performs payload specific
**                  configuration for a slice configured for ESCON
**                  mode in further.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciEsconModeCfg(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* TBD */

    return HYPHY20G_SUCCESS;
} /* hyPhy20gLsciEsconModeCfg */


/*******************************************************************************
**
**  hyPhy20gLsciEsconLcvErrInsCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function sets the user configured 10-bit patterns and
**                controls whether or not they will be substituted for for
**                received characters with encoding or running disparity erros.
**
**                This function can optionally be called after
**                hyPhy20gLsciInit but prior to hyPhy20gLsciDataPathEnable.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - LSCI slice processing resource
**                                slice number 0 - 15
**                 dir          - data flow direction
**                                0 - transmit data flow
**                                    from the CPB to the SFP
**                                1 - receive data flow
**                                    from the SFP to the CPB
**                 mode         - data stream overwriting operation
**                                0 - characters with encoding or running
**                                    disparity errors will NOT be replaced
**                                    with the user configured 10 bit patterns
**                                1 - characters with encoding or running
**                                    disparity errors will be replaced
**                                    with the user configured 10 bit patterns
**                 err_patt_n   - user configured pattern to be optionally
**                                sent out.
**                                Default 0x0F1
**                 err_patt_p   - user configured pattern to be optionally
**                                sent out.
**                                Default 0x30E
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciEsconLcvErrInsCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 dir, UINT4 mode, UINT4 err_patt_n,
                                   UINT4 err_patt_p)
{
    INT4 result;
    UINT4 traffic;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 || dir > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    if (mode > 2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = lsciHyPhy20gLsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic != LSCI_ESCON) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* call private function */
    result = lsciHyPhy20gLsciFcCbrLcvErrInsCfg(fileHndl, slice, dir, mode,
                                               err_patt_n, err_patt_p);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gLsciEsconLcvErrInsCfg */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gLsciCreateCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates and intializes memory required to operate
**                  the LSCI subsystem.
**
**                  References this memory to the device level structure
**                  pointed to with *pDevCtxt.
**
**                  Function hyPhy20gLsciDestroyCtxt free this memory and
**                  set the LSCI subsystem reference to NULL.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice value range : 0 - 15
**                  traffic   - LSCI traffic type
**                              traffic value range : 1 - 8
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciCreateCtxt(struct file *fileHndl, UINT4 slice, UINT4 traffic)
{
    sHYPHY20G_LSCI_CTXT *pLsciCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 size;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > HYPHY20G_LSCI_NUM_SLICE - 1) ||
        (traffic > LSCI_TRAFFIC_MAX) || (traffic < LSCI_MAC_TEMINATION)) 
    {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* If pLsciCtxt[slice] is not a NULL pointer,
       destroy it first to prevent memory leak */
    if (pDevCtxt->pLsciCtxt[slice] != NULL) {
        hyPhy20gLsciDestroyCtxt(fileHndl, slice);
    }

    /* allocate memory for and associate with pDevCtxt */
    size = sizeof(sHYPHY20G_LSCI_CTXT);
    pDevCtxt->pLsciCtxt[slice] = (sHYPHY20G_LSCI_CTXT *) sysHyPhy20gMemAlloc(size);

    if(pDevCtxt->pLsciCtxt[slice] == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    DBG_PRINT("LSCI slice #%ld Context memory allocated:addr=0x%x size=%ld\n",
              slice, (unsigned int)pDevCtxt->pLsciCtxt[slice], size);

    pLsciCtxt = pDevCtxt->pLsciCtxt[slice];

    /* initialize variables */
    pLsciCtxt->valid            = HYPHY20G_VALID_SIGN;
    pLsciCtxt->traffic          = traffic;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gLsciCreateCtxt */

/*******************************************************************************
**
**  hyPhy20gLsciDestroyCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset context of the LSCI subsystem.
**
**                  This function should be called when TBD
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number : 0 - 15
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gLsciDestroyCtxt(struct file *fileHndl, UINT4 slice)
{
    sHYPHY20G_CTXT *pDevCtxt;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > HYPHY20G_LSCI_NUM_SLICE - 1)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    if(pDevCtxt->pLsciCtxt[slice] == NULL){
        DBG_PRINT("%s, %s, %d, returning success as already NULL\n",__FILE__, __FUNCTION__, __LINE__);
    	return HYPHY20G_SUCCESS;
    }

    sysHyPhy20gMemFree(pDevCtxt->pLsciCtxt[slice]);
    DBG_PRINT("%s, %s, %d, returning success after free\n",__FILE__, __FUNCTION__, __LINE__);
    return HYPHY20G_SUCCESS;
    
} /* hyPhy20gLsciDestroyCtxt */

/*******************************************************************************
**
**  lsciHyPhy20gLsciCtxtGetMode
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves the configuration mode from 
**                  context of the LSCI slice
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**
**  OUTPUTS:        *pMode    - a pointer to the memory where store the
**                              return back value
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 lsciHyPhy20gLsciCtxtGetMode(struct file *fileHndl, UINT4 slice,
                                 UINT4 *pMode)
{
    sHYPHY20G_LSCI_CTXT *pLsciCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    HYPHY20G_ASSERT((pMode != NULL) , HYPHY20G_ERR_NULL_PTR_PASSED,
                    "pMode is NULL.");
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL) && (pDevCtxt->pLsciCtxt[slice] != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "pDevCtxt or (pDevCtxt->pLsciCtxt + %ld) is NULL.", slice);

    pLsciCtxt = pDevCtxt->pLsciCtxt[slice];
    *pMode = pLsciCtxt->traffic;

    HYPHY20G_ASSERT((*pMode <= LSCI_TRAFFIC_MAX)&&(*pMode >= LSCI_MAC_TEMINATION), 
                        HYPHY20G_ERR_INVALID_MODE,"*pMode is %ld.", *pMode);

    return HYPHY20G_SUCCESS;

} /* lsciHyPhy20gLsciCtxtGetMode */

/*******************************************************************************
**
**  lsciHyPhy20gLsciFeGeBlockInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Initializes the ENET_FEGE block at the block level.
**
**                  Alternatively, this function can be called to disable the
**                  ENET_FEGE block.
**
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - ENET_FEGE slice processing resource in the LSCI
**                              slice number 0 - 15
**                  enable    - 1 - enable the FEGE block
**                              0 - disable the FEGE block
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 lsciHyPhy20gLsciFeGeBlockInit(struct file *fileHndl, UINT4 slice,
                                   UINT4 enable)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 traffic;

    /* argument checking */
    if ((slice > HYPHY20G_LSCI_NUM_SLICE - 1) || (enable > 1)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = lsciHyPhy20gLsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic > LSCI_GE_GFPT_MAPPING) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }
    /* put the ENET_FEGE block in reset */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_TOP_REG_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_TOP_BIT_CFG_ENET_FEGE_RST_OFF, 1);
    if(result) return result;

    if (enable == HYPHY20G_DISABLE) {           /* disable operations */
        return HYPHY20G_SUCCESS;
    }

    /*
     * enable operations
     */
    /* remove the ENET_FEGE block reset */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_TOP_REG_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_TOP_BIT_CFG_ENET_FEGE_RST_OFF, 0);
    if(result) return result;

    /*
     * configure LSCI Top level registers
     */
    /* For the MAC terminated mode, use the default low-power value;
       For the GE_GFPT mode, must set low-power value as required */
    if (traffic == LSCI_GE_GFPT_MAPPING) {
        /* set MAC_LOWPWR to 1 and GE_GFPT_LOWPWR to 0 */
        addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TOP_REG_MD_CFG, slice);
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_MAC_LOWPWR_OFF,
                    1);
        if(result) return result;
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_GE_GFPT_LOWPWR_OFF,
                    0);
        if(result) return result;
    }

    /* remove sub-block reset */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TOP_REG_SW_RST, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    if (traffic == LSCI_GE_GFPT_MAPPING) {
        /* remove the reset to the useful sub-blocks for
           GE_GFPT_MAPPING mode */
        val = mHYPHY20G_SET_FIELD4(val, 0x6BE, (0x07FF << 0), 0);
    } else {        /* operations for all MAC terminated traffic */
        /* remove the reset to the useful sub-blocks for other modes */
        val = mHYPHY20G_SET_FIELD4(val, 0x500, (0x07FF << 0), 0);
    }

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* lsciHyPhy20gLsciFeGeBlockInit */



/*******************************************************************************
**
**  lsciHyPhy20gLsciFeGeDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure data path in FEGE block for FE/GE MAC Terminated
**                  and SGMII FE/GE MAC Terminated traffics.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - ENET_FEGE slice processing resource in the LSCI
**                              slice number 0 - 15
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 lsciHyPhy20gLsciFeGeDataPathCfg(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 addr, val = 0;
    UINT4 traffic;

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = lsciHyPhy20gLsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic != LSCI_MAC_TEMINATION) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /*
     * configure ENET_FEGE Top Level register
     */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TOP_REG_MD_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 0x2,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_IPG_RED_ON_THRESH_MSK,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_IPG_RED_ON_THRESH_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x1,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_IPG_RED_OFF_THRESH_MSK,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_IPG_RED_OFF_THRESH_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x2,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_PAUSE_ON_THRESH_MSK,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_PAUSE_ON_THRESH_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x1,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_PAUSE_OFF_THRESH_MSK,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_PAUSE_OFF_THRESH_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_UNIDIRECTIONAL_MODE_MSK,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_UNIDIRECTIONAL_MODE_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /*
     * configure L1RPP block
     */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_CFG_AND_STAT, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_AUTO_SENSE_MSK,
            HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_AUTO_SENSE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_L1RPP_EN_MSK,
            HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_L1RPP_EN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /*
     * configure RMAC block
     */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_RMAC_REG_CFG_1, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_INRANGE_DIS_OFF,
                1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_TRN_OFF,
                1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_TRANSPARENT_OFF,
                0);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_ETHER_MODE_OFF,
                1);
    if(result) return result;
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_1_INRANGE_CK_MSK, 0x3);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_RMAC_REG_CFG_2, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_INPAUSE_EN_OFF,
                1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_RMAC_BIT_CFG_2_PAUSEFRM_DROP_OFF,
                1);
    if(result) return result;

    /*
     * configure TMAC block
     */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TMAC_REG_CFG_1, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_IBUF_HTHR_EN_OFF,
                1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_RMAC_PAUSE_EN_OFF,
                1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_EBUF_ERR_EN_OFF,
                1);
    if(result) return result;

    /* don't enable tx path at this time */
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_TMAC_BIT_CFG_1_TMAC_ENBL_OFF,
                0);
    if(result) return result;

    /*
     * configure L1TPP block
     */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_L1TPP_REG_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_L1TPP_BIT_CFG_RD_ALIGN_EN_OFF,
                1);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* lsciHyPhy20gLsciFeGeDataPathCfg */

/*******************************************************************************
**
**  lsciHyPhy20gLsciGeGfpTDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure data path in PMM and FEGE block
**                  for GE GFP-T traffic.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - ENET_FEGE slice processing resource in the LSCI
**                              slice number 0 - 15
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 lsciHyPhy20gLsciGeGfpTDataPathCfg(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 addr, val = 0;
    UINT4 traffic;

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = lsciHyPhy20gLsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic != LSCI_GE_GFPT_MAPPING) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /*
     * Init PMM block
     */
    result = hyPhy20gLsciPmmInit(fileHndl, slice, 1, 0);
    if(result) return result;

    /*
     * configure ENET_FEGE Top Level register
     */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_TOP_REG_MD_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_UNIDIRECTIONAL_MODE_MSK,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_UNIDIRECTIONAL_MODE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x5,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_FEGE_MODE_MSK,
            HYPHY20G_LSCI_ENET_FEGE_TOP_BIT_MD_CFG_FEGE_MODE_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;


    /*
     * configure L1RPP block
     */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_CFG_AND_STAT, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_AUTO_SENSE_MSK,
            HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_AUTO_SENSE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_L1RPP_EN_MSK,
            HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_L1RPP_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x1,
            HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_MODE_MSK,
            HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_MODE_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_MII_CTL, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_CTL_AN_ENABLE_OFF, 0);
    if(result) return result;

    /* enable L1RPP_EN at the last step of ENET_FEGE L1RPP configuration */
    addr = HYPHY20G_LSCI_ENET_FEGE_L1RPP_REG_CFG_AND_STAT;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_L1RPP_EN_OFF,
                1);
    if(result) return result;

    /*
     * configure GE_GFPT block
     */
    /* put SOFT_RST into reset */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_CFG_SOFT_RESET_OFF,
                1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_CFG_SOFT_RESET_OFF,
                0);
    if(result) return result;

    /* set the *_patt_update bits to b'1*/
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_RX_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_10BIT_PATT_UPDATE_OFF,
                1);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_RX_20_BIT_PAT, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_20_BIT_PAT_RX_20BIT_PATT_UPDATE_OFF,
                1);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_TX_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_10BIT_PATT_UPDATE_OFF,
                1);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_TX_20_BIT_PAT, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_20_BIT_PAT_TX_20BIT_PATT_UPDATE_OFF,
                1);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_RX_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_SOFT_RESET_OFF,
                1);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_TX_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_SOFT_RESET_OFF,
                1);
    if(result) return result;

    /* configure the ingress path */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_RX_CFG, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_10BIT_PATTERN_MSK,
                0x0F1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_10BIT_PATT_EN_OFF,
                1);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_RX_20_BIT_PAT, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_20_BIT_PAT_RX_20BIT_PATTERN_MSK,
                0x7A1E8);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_20_BIT_PAT_RX_20BIT_PATT_EN_OFF,
                1);
    if(result) return result;

    /* configure the egress path */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_TX_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_CLK_ADAPT_FIFO_RESET_OFF,
                1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_CLK_ADAPT_FIFO_RESET_OFF,
                0);
    if(result) return result;

    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_10BIT_PATTERN_MSK,
                0x0F1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_10BIT_PATT_EN_OFF,
                1);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_TX_20_BIT_PAT, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_20_BIT_PAT_TX_20BIT_PATTERN_MSK,
                0x7A1E8);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_20_BIT_PAT_TX_20BIT_PATT_EN_OFF,
                1);
    if(result) return result;

    /* configure TX Rate Adaptation Delete/insert */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_TX_RT_ADPT_DEL_INTRVL, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_RT_ADPT_DEL_INTRVL_TXGE_GFPT_DELETE_INTERVAL_MSK,
                0x017E);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_TX_RT_ADPT_INS_INTRVL, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_RT_ADPT_INS_INTRVL_TXGE_GFPT_INSERT_INTERVAL_MSK,
                0x017E);
    if(result) return result;

 
    /* GE_GFPT TX Configuration */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_TX_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;
	
    /* Set the DISP_INVERSION_EN bit to 1 */
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_DISP_INVERSION_EN_MSK,
            HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_DISP_INVERSION_EN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* lsciHyPhy20gLsciGeGfpTDataPathCfg */


/*******************************************************************************
**
**  lsciHyPhy20gLsciFcDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures FC_CBR block
**                  for FC 4G and ESCON traffic.
**
**                  This function also configures default Offline sequences
**                  FC and ESCON.
**
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - FC_CBR slice processing resource in the LSCI
**                              slice number 0 - 15
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 lsciHyPhy20gLsciFcDataPathCfg(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 addr, reg_msk, val, traffic;
    UINT4 cfgMode, busWidth, olSeqHi, olSeqLow, pattDisp, rdInvEn;
    UINT4 txAlignMd;
    const UINT4 NOS_PN11_SEL = 1;
    UINT4 FC_RD_INV_EN;
    UINT4 TX_RD_INV_EN;
    UINT4 UNKWN_OS_FW_EN;

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = lsciHyPhy20gLsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic != LSCI_FC_4G && traffic != LSCI_ESCON) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    switch (traffic) {
        case LSCI_FC_4G:
            /* 0x00: for FC traffic */
            cfgMode     = 0x00;
            busWidth    = 0x03; /* 20-bit */
            /* FC OLS sequence (K28.5 D21.1 D10.4 D21.2) */
            olSeqHi     = 0x3E; /* USR_PATT[39:32] */
            olSeqLow    = 0xAA954AA5;   /* USR_PATT[31:0] */
            pattDisp    = 0;    /* not used for FC mode, set to 0 */
            rdInvEn     = 0;    /* no effect in FC mode */
            txAlignMd   = 1;
            FC_RD_INV_EN   = 1;
            TX_RD_INV_EN   = 0;
            UNKWN_OS_FW_EN = 1;
            break;
        case LSCI_ESCON:
            /* 0x01: for ESCON traffic */
            cfgMode     = 0x01;
            busWidth    = 0x03; /* 20-bit */
            /* ESCON Offline sequence function (K28.5 D24.2 K28.5 D24.2) */
            olSeqHi     = 0x3E; /* USR_PATT[39:32] */
            olSeqLow    = 0x8C53E8C5;   /* USR_PATT[31:0] */
            pattDisp    = 0x05;
            rdInvEn     = 1;
            txAlignMd   = 0;    /* must be b'0 in ESCON mode */
            FC_RD_INV_EN   = 0;
            TX_RD_INV_EN   = 1;
            UNKWN_OS_FW_EN = 0;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /*
     * configure FC_CBR Top Level
     */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_CFG, slice);

    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_FC_CBR_BIT_CFG_SOFT_RST_OFF, 1);
    if(result) return result;

    reg_msk = HYPHY20G_LSCI_FC_CBR_BIT_CFG_MODE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, cfgMode);
    if(result) return result;

    reg_msk = HYPHY20G_LSCI_FC_CBR_BIT_CFG_BUS_WIDTH_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, reg_msk, busWidth);
    if(result) return result;

    /*
     * configure FC_CBR Rx Path
     */
    result = lsciHyPhy20gLsciFcCbrUsrPattCfg(fileHndl, slice, HYPHY20G_RX_DIRECTION,
                olSeqHi, olSeqLow, 0);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_RX_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* defaulting LCV_ERR_INS to insert user_config 10-bit pattern
        update config via hyPhy20gLsciFcLcvErrInsCfg */
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_LCV_ERR_INS_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_LCV_ERR_INS_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, NOS_PN11_SEL,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_NOS_PN11_SEL_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_NOS_PN11_SEL_OFF);
    /* reserved bit */
    val = mHYPHY20G_SET_FIELD4(val, 0, (0x0001 << 3), 3);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_RX_DLO_EN, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_ASD_DLOLB_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_ASD_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_RX_LOSS_SYNC_DLOLB_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_RX_LOSS_SYNC_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_RX_DRL_DLOLB_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_RX_DRL_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x7,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_RX_DLOLB_INT_PER_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_RX_DLOLB_INT_PER_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /*
     * configure FC_CBR Tx Path
     */
    result = lsciHyPhy20gLsciFcCbrUsrPattCfg(fileHndl, slice,
                            HYPHY20G_TX_DIRECTION, olSeqHi, olSeqLow, pattDisp);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_TX_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, txAlignMd,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_FC_CONT_ALIGN_MODE_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_FC_CONT_ALIGN_MODE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, FC_RD_INV_EN,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_FC_RD_INV_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_FC_RD_INV_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, TX_RD_INV_EN,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_RD_INV_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_RD_INV_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_LCV_ERR_INS_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_LCV_ERR_INS_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_PN11_INV_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_PN11_INV_OFF);
    val = mHYPHY20G_SET_FIELD4(val, NOS_PN11_SEL,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_NOS_PN11_SEL_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_NOS_PN11_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0, (0x0001 << 3), 3);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_TX_DLO_EN, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_CLK_ADAPT_FIFO_UNDR_DLOLB_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_CLK_ADAPT_FIFO_UNDR_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_LCV_DLOLB_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_LCV_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_LOSS_SYNC_DLOLB_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_LOSS_SYNC_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_DRL_DLOLB_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_DRL_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x7,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_DLOLB_INT_PER_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_DLOLB_INT_PER_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* set insert and delete interval */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_INS_DEL_INTRVL, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 0x1000,
            HYPHY20G_LSCI_FC_CBR_BIT_INS_DEL_INTRVL_INS_INTERVAL_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_INS_DEL_INTRVL_INS_INTERVAL_OFF);

    val = mHYPHY20G_SET_FIELD4(val, 0x1000,
            HYPHY20G_LSCI_FC_CBR_BIT_INS_DEL_INTRVL_DEL_INTERVAL_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_INS_DEL_INTRVL_DEL_INTERVAL_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_OS_CTL, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* treat unrecognized ordered sets as fill-words */
    val = mHYPHY20G_SET_FIELD4(val, UNKWN_OS_FW_EN,
            HYPHY20G_LSCI_FC_CBR_BIT_OS_CTL_UNKWN_OS_FW_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_OS_CTL_UNKWN_OS_FW_EN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* lsciHyPhy20gLsciFcDataPathCfg */

/*******************************************************************************
**
**  lsciHyPhy20gLsciCbrDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure data path in PMM and FC_CBR block
**                  for CBR traffics.
**
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - FC_CBR slice processing resource in the LSCI
**                              slice number 0 - 15
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 lsciHyPhy20gLsciCbrDataPathCfg(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 addr, val, traffic;
    UINT4 cfgMode = 0x2, errIns, byteAlign;
    UINT4 dlolbEn;
    UINT4 NOS_PN11_SEL = 0;

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = lsciHyPhy20gLsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic < LSCI_BYTEALIGED_CBR_STS || traffic > LSCI_8B10B_TX_RD_INV_DIS_CBR) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (traffic) {
        case LSCI_BYTEALIGED_CBR_STS:
        case LSCI_BYTEALIGED_CBR:
            errIns = byteAlign = 0;
            dlolbEn = 0;
            NOS_PN11_SEL = 1;
            break;
        case LSCI_8B10B_TX_RD_INV_EN_CBR:
        case LSCI_8B10B_TX_RD_INV_DIS_CBR:
            errIns = byteAlign = 1;
            dlolbEn = 1;
            NOS_PN11_SEL = 0;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /*
     * configure FC_CBR Top Level
     */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_CFG, slice);

    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_LSCI_FC_CBR_BIT_CFG_SOFT_RST_OFF, 1);
    if(result) return result;

    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_LSCI_FC_CBR_BIT_CFG_MODE_MSK, cfgMode);
    if(result) return result;

    /*
     * configure FC_CBR Rx Path
     */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_RX_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, errIns,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_LCV_ERR_INS_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_LCV_ERR_INS_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, NOS_PN11_SEL,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_NOS_PN11_SEL_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_NOS_PN11_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, byteAlign, (0x0001 << 3), 3);
    val = mHYPHY20G_SET_FIELD4(val, byteAlign,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_ENABLE_BYTE_ALIGN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_ENABLE_BYTE_ALIGN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_RX_DLO_EN, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_ASD_DLOLB_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_ASD_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, dlolbEn,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_RX_LOSS_SYNC_DLOLB_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_RX_LOSS_SYNC_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, dlolbEn,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_RX_DRL_DLOLB_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_RX_DRL_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x7,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_RX_DLOLB_INT_PER_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_RX_DLOLB_INT_PER_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /*
     * configure FC_CBR Tx Path
     */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_TX_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_FC_CONT_ALIGN_MODE_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_FC_CONT_ALIGN_MODE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_FC_RD_INV_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_FC_RD_INV_EN_OFF);
    
    if (traffic == LSCI_8B10B_TX_RD_INV_DIS_CBR)
    {
        val = mHYPHY20G_SET_FIELD4(val, 0,
                HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_RD_INV_EN_MSK,
                HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_RD_INV_EN_OFF);
    }
    else
    {
        val = mHYPHY20G_SET_FIELD4(val, 1,
                HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_RD_INV_EN_MSK,
                HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_RD_INV_EN_OFF);
    }
    
    val = mHYPHY20G_SET_FIELD4(val, errIns,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_LCV_ERR_INS_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_LCV_ERR_INS_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, NOS_PN11_SEL,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_NOS_PN11_SEL_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_NOS_PN11_SEL_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_LSCI_FC_CBR_REG_TX_DLO_EN, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_CLK_ADAPT_FIFO_UNDR_DLOLB_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_CLK_ADAPT_FIFO_UNDR_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, dlolbEn,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_LOSS_SYNC_DLOLB_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_LOSS_SYNC_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, dlolbEn,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_DRL_DLOLB_EN_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_DRL_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x7,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_DLOLB_INT_PER_MSK,
            HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_DLOLB_INT_PER_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* lsciHyPhy20gLsciCbrDataPathCfg */

/*******************************************************************************
**
**  lsciHyPhy20gLsciFcLcvErrInsCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function sets the user configured 10-bit patterns and
**                controls whether or not they will be substituted for for
**                received characters with encoding or running disparity erros.
**
**                This private function is called by the FC, ESCON or CBR
**                public function which in turn is called after
**                hyPhy20gLsciInit but prior to hyPhy20gLsciDataPathEnable.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - LSCI slice processing resource
**                                slice number 0 - 15
**                 dir          - data flow direction
**                                0 - transmit data flow
**                                    from the CPB to the SFP
**                                1 - receive data flow
**                                    from the SFP to the CPB
**                 mode         - data stream overwriting operation
**                                0 - characters with encoding or running
**                                    disparity errors will NOT be replaced
**                                    with the user configured 10 bit patterns
**                                1 - characters with encoding or running
**                                    disparity errors will be replaced
**                                    with the user configured 10 bit patterns
**                 err_patt_n   - user configured pattern to be optionally
**                                sent out.
**                                Default 0x0F1
**                 err_patt_p   - user configured pattern to be optionally
**                                sent out.
**                                Default 0x30E
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 lsciHyPhy20gLsciFcCbrLcvErrInsCfg(struct file *fileHndl, UINT4 slice,
                                       UINT4 dir, UINT4 mode, UINT4 err_patt_n,
                                       UINT4 err_patt_p)
{
    INT4 result;
    UINT4 traffic;
    UINT4 addr, addr1, addr2, offset1, msk_n, msk_p;

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 || dir > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = lsciHyPhy20gLsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic < LSCI_FC_4G || traffic > LSCI_8B10B_TX_RD_INV_DIS_CBR) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    switch (dir) {
        case HYPHY20G_TX_DIRECTION:
            addr1 = HYPHY20G_LSCI_FC_CBR_REG_TX_CFG;
            addr2 = HYPHY20G_LSCI_FC_CBR_REG_TX_ERR_USR_PAT;
            offset1 = HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_LCV_ERR_INS_EN_OFF;
            msk_n = HYPHY20G_LSCI_FC_CBR_BIT_TX_ERR_USR_PAT_TX_ERR_PATT_N_MSK;
            msk_p = HYPHY20G_LSCI_FC_CBR_BIT_TX_ERR_USR_PAT_TX_ERR_PATT_P_MSK;
            break;
        case HYPHY20G_RX_DIRECTION:
            addr1 = HYPHY20G_LSCI_FC_CBR_REG_RX_CFG;
            addr2 = HYPHY20G_LSCI_FC_CBR_REG_RX_ERR_USR_PAT;
            offset1 = HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_LCV_ERR_INS_EN_OFF;
            msk_n = HYPHY20G_LSCI_FC_CBR_BIT_RX_ERR_USR_PAT_RX_ERR_PATT_N_MSK;
            msk_p = HYPHY20G_LSCI_FC_CBR_BIT_RX_ERR_USR_PAT_RX_ERR_PATT_P_MSK;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure LCV_ERR_INS mode */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr1, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset1, mode);
    if(result) return result;

    if (mode == 1) {
        /* user defined selected, programming values */
        addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr2, slice);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk_n, err_patt_n);
        if(result) return result;

        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk_p, err_patt_p);
        if(result) return result;
    }

 return HYPHY20G_SUCCESS;

} /* lsciHyPhy20gLsciFcCbrLcvErrInsCfg */

/*******************************************************************************
**
**  lsciHyPhy20gLsciFcCbrUsrPattCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configures and updates user pattern and running disparity
**                  for FC_CBR block.
**
**
**
**  INPUTS:         fileHndl   - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**                  dir        - data path direction
**                               0 - pattern in transmit direction
**                                   (from the CPB to the SFP)
**                               1 - pattern in receive direction
**                                   (from the SFP to the CPB)
**                  olSeqHi    - specifies the user defined pattern
**                  olSeqLow   - specifies the user defined pattern
**                  pattDisp   - specifies the running disparity at the end of
**                               each 10-bit user configured pattern
**                               (only useful when dir == 0)
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 lsciHyPhy20gLsciFcCbrUsrPattCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 dir, UINT4 olSeqHi, UINT4 olSeqLow,
                                     UINT4 pattDisp)
{
    INT4 result;
    UINT4 traffic;
    UINT4 addr, bitoffset, addr1, addr2, hiMsk, lowMsk, pattMsk;
    UINT4 upd_msk, upd_off;
    UINT4 softRstVal;

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 || dir > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = lsciHyPhy20gLsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic < LSCI_FC_4G || traffic > LSCI_8B10B_TX_RD_INV_DIS_CBR) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pattMsk = 0;
    switch (dir) {
        case HYPHY20G_TX_DIRECTION:
            addr1   = HYPHY20G_LSCI_FC_CBR_REG_TX_USR_PAT_LSB;
            addr2   = HYPHY20G_LSCI_FC_CBR_REG_TX_CFG;
            lowMsk  = HYPHY20G_LSCI_FC_CBR_BIT_TX_USR_PAT_LSB_TX_USR_PATT_MSK;
            hiMsk   = HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_USR_PATT_MSK;
            pattMsk = HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_USR_PATT_DISP_MSK;
            upd_msk = HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_USR_PATT_UPDATE_MSK;
            upd_off = HYPHY20G_LSCI_FC_CBR_BIT_TX_CFG_TX_USR_PATT_UPDATE_OFF;
            break;
        case HYPHY20G_RX_DIRECTION:
            addr1   = HYPHY20G_LSCI_FC_CBR_REG_RX_USR_PAT_LSB;
            addr2   = HYPHY20G_LSCI_FC_CBR_REG_RX_CFG;
            lowMsk  = HYPHY20G_LSCI_FC_CBR_BIT_RX_USR_PAT_LSB_RX_USR_PATT_MSK;
            hiMsk   = HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_USR_PATT_MSK;
            upd_msk = HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_USR_PATT_UPDATE_MSK;
            upd_off = HYPHY20G_LSCI_FC_CBR_BIT_RX_CFG_RX_USR_PATT_UPDATE_OFF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr1, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, lowMsk, olSeqLow);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr2, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, hiMsk, olSeqHi);
    if(result) return result;

    if ((traffic == LSCI_8B10B_TX_RD_INV_EN_CBR ||
         traffic == LSCI_8B10B_TX_RD_INV_DIS_CBR) &&
         (dir == HYPHY20G_TX_DIRECTION)) {
        result = hyPhy20gRegFieldWrite(fileHndl, addr, pattMsk, pattDisp);
        if(result) return result;
    }

    /* update pattern via USR_PATT_UPDATE bit */
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr2, slice);
    bitoffset = upd_off;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* check the SOFT_RST bit: if set to 0, poll, otherwise don't poll */
    addr = HYPHY20G_LSCI_FC_CBR_REG_CFG;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = hyPhy20gRegFieldRead(fileHndl, addr,
                                  HYPHY20G_LSCI_FC_CBR_BIT_CFG_SOFT_RST_MSK,
                                  &softRstVal);
    if(result) return result;

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr2, slice);
    if (softRstVal == 0) {
        /* poll the update bit which self clears to 0
           when the new pattern has been loaded */
        result = sysHyPhy20gPollBit(fileHndl, addr, upd_msk, 0,
                                    HYPHY20G_DELAY_20USEC);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* lsciHyPhy20gLsciFcCbrUsrPattCfg */

/*******************************************************************************
**
**  lsciHyPhy20gLsciRxFcDlolbIntPer
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function calculates the RX_DLOLB error event integration
**                period and applies it to an LSCI FC_CBR slice.
**
**                This function configures RX_DLOLB_INT_PER and
**                RX_DLOLB_CLK_DIV fields.
**
**                This function performs integer multiplication and division,
**                it assumes 32 bit variables are being used.
**
**                Should the integration period cause register field values to
**                be exceeded, the function will limit the setting to the
**                maximum/minimum values of the register fields.  No
**                notification of this configuration truncation will be passed
**                to system.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - LSCI slice processing resource
**                                slice number 0 - 15
**                 period       - Integration period in mseconds
**                                must be 1 or greater.
**                                maximum value is a function of the
**                                resolution of the intergration period register
**                                fields, the client rate and the bus width
**                                used between the SERDES and LSCI slice.
**                 frequency    - CBR Client frequency in units of kbits/s.
**                                minimum value 0016000 (16 Mbits/s)
**                                maximum value 5000000 (5 Gbits/s)
**                 busWidth     - bus width used between SERDES and LSCI
**                                (using same encoding as BUS_WIDTH field in
**                                 FC_CBR CFG register)
**                                0 - 8 bit
**                                1 - 10 bit
**                                2 - 16 bit
**                                3 - 20 bit
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 lsciHyPhy20gLsciRxFcDlolbIntPer(struct file *fileHndl, UINT4 slice,
                                     UINT4 period, UINT4 frequency,
                                     UINT4 busWidth)
{

    /* variable declaration */
    UINT4 addr, msk;
    UINT4 busWidthInBits, i, num, div, perLim, dlolbIntPer, dlolbClkDiv;
    INT4 result;

    /* argument checking */
    if((slice > 15)||(period == 0)||(period > 0xFFFFFFFF)||
        (frequency < 16000)||(frequency > 5000000)||
        (busWidth > 3)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* The following formulas apply:

       lineClock = frequency/busWidthInBits
       DLOLB_CLK = lineClock/(RX_DLOLB_CLK_DIV+1)
       period = (RX_DLOLB_INT_PER+1)/DLOLB_CLK

       RX_DLOLB_INT_PER[2:1] may take on value 0 to 7

       RX_DLOLB_CLK_DIV[31:0] can be calculate from:

       (RX_DLOLB_CLK_DIV+1) =
       (frequency*period)/(busWidthInBits*(RX_DLOLB_INT_PER+1))
    */

    /* determine number of bits associated with busWidth argument */
    switch (busWidth) {
        case 0:
            busWidthInBits = 8;
            break;
        case 1:
            busWidthInBits = 10;
            break;
        case 2:
            busWidthInBits = 16;
            break;
        case 3:
            busWidthInBits = 20;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine field settings */
    i = 0;
    dlolbIntPer = 0xFFFF;
    /* determine RX_DLOLB_INT_PER so that requested period does not
       overflow 32 bit variables */
    while(i < 8){
        num = 0xFFFFFFFF;
        div = frequency/(i+1);
        div /= busWidthInBits;
        perLim = num/div;
        if(perLim <= period){
            i++;
        } else {
            dlolbIntPer = i;
            i = 8;
        }
    }
    if(dlolbIntPer == 0xFFFF){
        /* value specified by period exceeds the period setting available, in
           this case, maximize the interval */
        dlolbIntPer = 0x7;
        dlolbClkDiv = 0xFFFFFFFF;
    } else {
        /* calculate RX_DLOLB_CLK_DIV so that 32 bit variables are not
           exceeded */
        dlolbClkDiv = frequency/(dlolbIntPer+1);
        dlolbClkDiv /= busWidthInBits;
        dlolbClkDiv *= period;
        dlolbClkDiv--;
    }

    /* write RX_DLOLB_CLK_DIV and RX_DLOLB_INT_PER values to device */
    addr = HYPHY20G_LSCI_FC_CBR_REG_RX_DLO_CLK_CFG;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_CLK_CFG_RX_DLOLB_CLK_DIV_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, dlolbClkDiv);
    if(result) return result;

    addr = HYPHY20G_LSCI_FC_CBR_REG_RX_DLO_EN;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_LSCI_FC_CBR_BIT_RX_DLO_EN_RX_DLOLB_INT_PER_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, dlolbIntPer);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* lsciHyPhy20gLsciRxFcDlolbIntPer */

/*******************************************************************************
**
**  lsciHyPhy20gLsciTxFcDlolbIntPer
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function calculates the TX_DLOLB error event integration
**                period and applies it to an LSCI FC_CBR slice.
**
**                This function configures TX_DLOLB_INT_PER and
**                TX_DLOLB_CLK_DIV fields.
**
**                This function performs integer multiplication and division,
**                it assumes 32 bit variables are being used.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - LSCI slice processing resource
**                                slice number 0 - 15
**                 period       - Integration period in mseconds
**                                must be 1 or greater.
**                                maximum value is a function of the
**                                resolution of the intergration period register
**                                fields 102873 mseconds.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 lsciHyPhy20gLsciTxFcDlolbIntPer(struct file *fileHndl, UINT4 slice,
                                     UINT4 period)
{
    /* variable declaration */
    UINT4 addr, msk;
    UINT4 i, num, div, perLim, dlolbIntPer, dlolbClkDiv, frequency;
    INT4 result;

    /* argument checking */
    if((slice > 15)||(period == 0)||(period > 102873)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* The following formulas apply:

       DLOLB_CLK = 334000 /(TX_DLOLB_CLK_DIV+1)
       period = (TX_DLOLB_INT_PER+1)/DLOLB_CLK

       TX_DLOLB_INT_PER[2:1] may take on value 0 to 7

       TX_DLOLB_CLK_DIV[31:0] can be calculate from:

       (TX_DLOLB_CLK_DIV+1) = (334000*period)/(TX_DLOLB_INT_PER+1))

    */


    /* determine field settings */
    frequency = 334000;
    i = 0;
    dlolbIntPer = 0xFFFF;
    /* determine RX_DLOLB_INT_PER so that requested period does not
       overflow 32 bit variables */
    while(i < 8){
        num = 0xFFFFFFFF;
        div = frequency/(i+1);
        perLim = num/div;
        if(perLim <= period){
            i++;
        } else {
            dlolbIntPer = i;
            i = 8;
        }
    }
    if(dlolbIntPer == 0xFFFF){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    /* calculate TX_DLOLB_CLK_DIV so that 32 bit variables are not
       exceeded */
    dlolbClkDiv = frequency/(dlolbIntPer+1);
    dlolbClkDiv *= period;
    dlolbClkDiv--;

    /* write TX_DLOLB_CLK_DIV and TX_DLOLB_INT_PER values to device */
    addr = HYPHY20G_LSCI_FC_CBR_REG_TX_DLO_CLK_CFG;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_CLK_CFG_TX_DLOLB_CLK_DIV_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, dlolbClkDiv);
    if(result) return result;

    addr = HYPHY20G_LSCI_FC_CBR_REG_TX_DLO_EN;
    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_LSCI_FC_CBR_BIT_TX_DLO_EN_TX_DLOLB_INT_PER_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, dlolbIntPer);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* lsciHyPhy20gLsciTxFcDlolbIntPer */

/*******************************************************************************
**
**  lsciHyPhy20gLsciGeGfpTPattUpdate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure a specific type of pattern value and make the
**                  new configured pattern value taking effect.
**
**                  Programmer must ensure this function called when
**                  corresponding pattern enabled.
**
**                  Used for GE GFP-T mode.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - LSCI slice processing resource
**                              slice number 0 - 15
**                  type      - determine which pattern to be configured
**                              in GE_GFPT block
**                              0 - Rx 10-bit Pattern
**                              1 - Tx 10-bit Pattern
**                              2 - Rx 20-bit Pattern
**                              3 - Tx 20-bit Pattern
**                  pattern   - determine user configurable pattern to replace
**                              errors
**                              0x0 -   0x3FF (type == 0 or 1)
**                              0x0 - 0xFFFFF (type == 2 or 3)
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 lsciHyPhy20gLsciGeGfpTPattUpdate(struct file *fileHndl, UINT4 slice,
                                      UINT4 type, UINT4 pattern)
{
    INT4 result;
    UINT4 addr, pattMsk, updateMsk, updateOff;

    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 || type > 3 ||
        (type < 2 && pattern > 0x3FF) ||
        (type > 1 && pattern > 0xFFFFF)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (type) {
        /* 0 - Rx 10-bit Pattern */
        case 0:
            addr        = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_RX_CFG;
            pattMsk     = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_10BIT_PATTERN_MSK;
            updateMsk   = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_10BIT_PATT_UPDATE_MSK;
            updateOff   = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_CFG_RX_10BIT_PATT_UPDATE_OFF;
            break;
        /* 1 - Tx 10-bit Pattern */
        case 1:
            addr        = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_TX_CFG;
            pattMsk     = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_10BIT_PATTERN_MSK;
            updateMsk   = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_10BIT_PATT_UPDATE_MSK;
            updateOff   = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_CFG_TX_10BIT_PATT_UPDATE_OFF;
            break;
        /* 2 - Rx 20-bit Pattern */
        case 2:
            addr        = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_RX_20_BIT_PAT;
            pattMsk     = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_20_BIT_PAT_RX_20BIT_PATTERN_MSK;
            updateMsk   = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_20_BIT_PAT_RX_20BIT_PATT_UPDATE_MSK;
            updateOff   = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_RX_20_BIT_PAT_RX_20BIT_PATT_UPDATE_OFF;
            break;
        /* 3 - Tx 20-bit Pattern */
        case 3:
            addr        = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_REG_TX_20_BIT_PAT;
            pattMsk     = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_20_BIT_PAT_TX_20BIT_PATTERN_MSK;
            updateMsk   = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_20_BIT_PAT_TX_20BIT_PATT_UPDATE_MSK;
            updateOff   = HYPHY20G_LSCI_ENET_FEGE_GE_GFPT_BIT_TX_20_BIT_PAT_TX_20BIT_PATT_UPDATE_OFF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice);

    /* set the user configured pattern */
    result = hyPhy20gRegFieldWrite (fileHndl, addr, pattMsk, pattern);
    if (result) return result;

    /* toggle to update pattern */
    result = sysHyPhy20gBitWrite(fileHndl, addr, updateOff, 1);
    if(result) return result;

    /* polling the update bit self clear to 0
       when the new pattern has been loaded */
    result = sysHyPhy20gPollBit(fileHndl, addr, updateMsk, 0, 1);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* lsciHyPhy20gLsciGeGfpTPattUpdate */

/*******************************************************************************
**
**  lsciHyPhy20gLsciGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the size of LSCI context.
**
**
**  INPUTS:         fileHndl      - device handle
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The size of LSCI context
**
*******************************************************************************/
UINT4 lsciHyPhy20gLsciGetCtxtSize(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_LSCI_CTXT *pLsciCtxt;
    UINT4 slice = 0, size = 0;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    /* calculate each slice context size */
    for (slice = 0; slice < HYPHY20G_LSCI_NUM_SLICE; slice++) {
        /* include ctxt header size */
        size += utilHyPhy20gGetCtxtHdrSize();

        /* calculate this slice context size */
        pLsciCtxt = pDevCtxt->pLsciCtxt[slice];
        if (pLsciCtxt != NULL) {
            size += sizeof(sHYPHY20G_LSCI_CTXT);
        }
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, size);

    return size;
}/* lsciHyPhy20gLsciGetCtxtSize */

/*******************************************************************************
**
**  lsciHyPhy20gLsciExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the LSCI context into pre-allocated buffer.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-allocated buffer for LSCI context extraction
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
INT4 lsciHyPhy20gLsciExtractCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_LSCI_CTXT *pLsciCtxt;
    UINT4 slice, rtLen;
    UINT4 offset = 0, len = 0;

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
    for (slice = 0; slice < HYPHY20G_LSCI_NUM_SLICE; slice++) {

        /* retrieve this slice context */
        pLsciCtxt = pDevCtxt->pLsciCtxt[slice];

        DBG_PRINT("%s, %s, %d, slice %ld offset 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, offset);
        
		/* insert ctxt buffer header */
        len = (pLsciCtxt == NULL) ? sizeof(sHYPHY20G_CTXT_HDR) : 
                (sizeof(sHYPHY20G_CTXT_HDR) + sizeof(sHYPHY20G_LSCI_CTXT));
        rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                            HYPHY_LSCI_CTXT_ID, slice);
        HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
            "%s", "pCtxtBuf is NULL.");  
        offset += sizeof(sHYPHY20G_CTXT_HDR);
        
        if (pLsciCtxt != NULL) {
            /* copy this slice context into the buffer */
            sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
                            (void *)pLsciCtxt, sizeof(sHYPHY20G_LSCI_CTXT));
            offset += sizeof(sHYPHY20G_LSCI_CTXT);
        }
    }/* end of for slice loop*/

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pExtractedSize = offset;
    
    return HYPHY20G_SUCCESS;
}/* lsciHyPhy20gLsciExtractCtxt */

/*******************************************************************************
**
**  lsciHyPhy20gLsciRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the LSCI context into the driver context memory.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-stored LSCI context for restoration
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
INT4 lsciHyPhy20gLsciRestoreCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_LSCI_CTXT *pLsciCtxt;
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    INT4 result;
    UINT4 offset = 0, len = 0, slice = 0, i = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pParsedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pParsedSize is NULL.");

    /* try to retrieve the lsci[0] header in the pre-stored context buffer */
    pCtxtBufHdr = utilHyPhy20gSearchCtxtHdr(pCtxtBuf, HYPHY_LSCI_CTXT_ID, 0);
    if (pCtxtBufHdr == NULL) {
        DBG_PRINT("%s, %s, %d, can not find HYPHY_LSCI_CTXT_ID hdr\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    pCtxtBuf = (void *)pCtxtBufHdr;

    /* restore each slice context */
    for (slice = 0; slice < HYPHY20G_LSCI_NUM_SLICE; slice++) {

        DBG_PRINT("%s, %s, %d, slice %ld offset 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, offset);

        /* invalid data for this slice */
        if (pCtxtBufHdr->len == sizeof(sHYPHY20G_CTXT_HDR)) {
            offset += sizeof(sHYPHY20G_CTXT_HDR);
        } else if (pCtxtBufHdr->len > sizeof(sHYPHY20G_CTXT_HDR)) {
            /* create this slice context */
            result = hyPhy20gLsciCreateCtxt(fileHndl, slice, 1);
            if (result) {
                DBG_PRINT("%s, %s, %d, CreateCtxt failed for slice %ld\n",
                            __FILE__, __FUNCTION__, __LINE__, slice);

                for (i = 0; i < slice; i++) {
                    hyPhy20gLsciDestroyCtxt(fileHndl, i);
                }
                *pParsedSize = 0;
                return result;
            }

            /* retrieve device context */
            pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
            /* confirm device context pointer is valid before processing */
            HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                            "%s", "pDevCtxt is NULL.");

            /* retrieve this slice context */
            pLsciCtxt = pDevCtxt->pLsciCtxt[slice];

            /* offset pCtxtBuf to figure out the payload position */
            offset += sizeof(sHYPHY20G_CTXT_HDR);
            /* payload len */
            len = pCtxtBufHdr->len - 
                    sizeof(sHYPHY20G_CTXT_HDR);
            
            sysHyPhy20gMemCpy((void *)(pLsciCtxt), 
                            ((UINT1 *)pCtxtBuf + offset), len);
            offset += len;
        } else {
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_DEV_CTXT_CORRUPTED\n", 
                        __FILE__, __FUNCTION__, __LINE__);
            *pParsedSize = 0;
            return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
        }

        /* move to next ctxt buffer header */
        pCtxtBufHdr = (sHYPHY20G_CTXT_HDR *)((UINT1 *)pCtxtBuf + offset);
        
    }/* end of slice loop*/

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);
    
    *pParsedSize = offset;
    
    return HYPHY20G_SUCCESS;
}/* lsciHyPhy20gLsciRestoreCtxt */


/********************************************************************************
**
**  lsciHyPhy20gLsciAuditCtxt 
**  ___________________________________________________________________________
**
**  DESCRIPTION:    the function used to audit the running LSCI subCTXT.
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
INT4 lsciHyPhy20gLsciAuditCtxt (struct file *fileHndl)
{
    INT4 result;
    UINT4 slice, devId, revId, status, lowPwrOff, lowPwr;
    UINT4 rstBit, rstOff;
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_LSCI_CTXT *pLsciCtxt;

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

    for (slice=0; slice<HYPHY20G_LSCI_NUM_SLICE; slice++)
    {
        pLsciCtxt=pDevCtxt->pLsciCtxt[slice];
        lowPwrOff = HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_ENET_LSCI_LOWPWR_0_OFF + slice;
        result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                                     lowPwrOff, &lowPwr);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 

        /* the LSCI slice disabled */
        if (lowPwr == 1)
        {
            if (pLsciCtxt!=NULL)
            {
                DBG_PRINT("%s, %s, %d, %uth slice pLsciCtxt!=NULL when LOWPWR==1\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice );
                return HYPHY20G_FAILURE;
            }
            else
                continue;
        }
        rstOff = HYPHY20G_TOP_BIT_SYS_SW_RST_2_ENET_LSCI_RST_0_OFF + slice;
        result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                     rstOff, &rstBit);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 
        if (rstBit == 1)
        {
            if (pLsciCtxt!=NULL)
            {
                DBG_PRINT("%s, %s, %d, %uth slice pLsciCtxt!=NULL when LSCI_RST == 1\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice );
                return HYPHY20G_FAILURE;
            }
            else
                continue;
        }
        /* the LSCI slice enabled */
        if (pLsciCtxt==NULL)
        {
            DBG_PRINT("%s, %s, %d, %uth slice pLsciCtxt==NULL when this LSCI enabled.\n",
                __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice );
            return HYPHY20G_FAILURE;
        }
        else if ((pLsciCtxt->valid!=HYPHY20G_VALID_SIGN)||
                  (pLsciCtxt->traffic>LSCI_TRAFFIC_MAX)||(pLsciCtxt->traffic<LSCI_MAC_TEMINATION))
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
    }
    
    return HYPHY20G_SUCCESS;
    
}/* lsciHyPhy20gLsciAuditCtxt */
/* end of file */
