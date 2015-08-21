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
**  MODULE      : HyPHY 20G HSCI Subsystem Configuration Code
**
**  FILE        : hyphy20g_hsci.c
**
**  $Date: 2012-12-04 13:25:27 +0800 (Tue, 04 Dec 2012) $
**
**  $Revision: 29458 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_hsci.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 hsciHyPhy20gHsciCreateCtxt(struct file *fileHndl, UINT4 slice, UINT4 mode);

INT4 hsciHyPhy20gHsci10GeBlockInit(struct file *fileHndl, UINT4 slice);

INT4 hsciHyPhy20gHsci10GeDataPathCfg(struct file *fileHndl, UINT4 slice);

INT4 hsciHyPhy20gHsciGsup43DataPathCfg(struct file *fileHndl, UINT4 slice);

INT4 hsciHyPhy20gHsciFcDataPathCfg(struct file *fileHndl, UINT4 slice);

INT4 hsciHyPhy20gHsciCbrDataPathCfg(struct file *fileHndl, UINT4 slice);

INT4 hsciHyPhy20gHsciMacGsup43DataPathCfg(struct file *fileHndl, UINT4 slice);

INT4 hsciHyPhy20gHsciFcCbrUsrPattCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 dir, UINT4 olSeqHi, UINT4 olSeqLow,
                                     UINT4 pattDisp);

INT4 hsciHyPhy20gHsciGsup43RxCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 losInv, UINT4 dscrdZone, UINT4 sigEn,
                                 UINT4 seqEn, UINT4 allDscrd, UINT4 osPrtDis,
                                 UINT4 minOsPrt,UINT4 pktType,UINT4 osType);

INT4 hsciHyPhy20gHsciGsup43TxCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 errDrop, UINT4 pktType, UINT4 osType,
                                 UINT4 forceMinIpg, UINT4 unidirEn,
                                 UINT4 sendIdleEn, UINT4 fifoHmark);

INT4 hsciHyPhy20gHsciRxxgPrmbCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 prmbProc, UINT4 hdrSize);

INT4 hsciHyPhy20gTxElcassPauseTagCfg(struct file *fileHndl, UINT4 slice);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/
/*******************************************************************************
**
**  hyPhy20gHsciInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes a HSCI slice at the device level
**                  and set it to one of major operating modes:
**                      1 - 10GE MAC Terminated (including G.Sup43
**                          Clause 7.1 and 7.2 10GE MAC terminated)
**                      2 - G.Sup43 Clause 7.3 Terminating application
**                          bypass MAC
**                      3 - FC 8G and subrate
**                      4 - High Speed CBR Byte-aligned mode (including
**                          G.Sup43 Clause 7.1 and 7.2 10GE transparent,
**                          STS-192 asynchronous transparent and FC 10G)
**                      5 - High Speed CBR 8b10b encoded mode
**                      6 - G.Sup43 Clause 7.3 Terminating application
**                          through MAC
**                      7 - G.Sup43 Clause 6.2 Terminating application
**                          through MAC
**
**                  The G.Sup43 Clause 7.3 and 6.2 Terminating application
**                  through MAC operating modes are the enhancement feature
**                  and only supported in Rev B device.
**                  In the two applications, the system side of ENET_10G MAC is
**                  the corresponding CPB channel connected to PIF interface;
**                  instead of HSCI GSUP43 and SERDES blocks, the line side of
**                  ENET_10G MAC is MAC_GSUP43 block located in ENET_COM
**                  subsystem and connected to GFP-F subsystem.
**
**                  In G.Sup43 Clause 7.3 Terminating application through
**                  MAC, the preamble along with the Ethernet frames
**                  can be transferred to GFP-F and PIF interface, because
**                  Ethernet Ordered Set information can be preserved by
**                  mapping the Ordered Sets into separate GFP-F frames.
**
**                  In G.Sup43 Clause 6.2 Terminating application through
**                  MAC, the Ethernet 64B/66B line code, preamble, SFD and
**                  IPG are terminated as per IEEE 802.3, because the GFP-F
**                  subsystem is expected to wrap terminated Ethernet data
**                  stream as defined in G.Sup43 Clause 6.2.
**
**                  This function must be called at first to configure
**                  data path depending on the 'mode' parameter.
**
**                  Alternatively, this function can be called to disable the
**                  HSCI subsystem.
**
**                  This function controls FW command for MSTATX statistics
**                  collection. hyPhy20gHostMsgInit must have been called
**                  prior to calling this function.
**
**                  Note: this function must be called after hyPhy20gCpbInit.
**                  
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  mode      - Operation mode of the specific HSCI slice
**                              0 - disable HSCI subsystem
**                              1 - 10GE MAC Terminated (including G.Sup43
**                                  Clause 7.1 and 7.2 10GE MAC terminated)
**                              2 - G.Sup43 Clause 7.3 Terminating application
**                                  bypass MAC
**                              3 - FC 8G and subrate
**                              4 - High Speed CBR Byte-aligned mode (including
**                                  G.Sup43 Clause 7.1 and 7.2 10GE transparent,
**                                  STS-192 asynchronous transparent and FC 10G)
**                              5 - High Speed CBR 8b10b encoded mode
**                                  Enable disparity inverter on the transmit path
**                              6 - G.Sup43 Clause 7.3 Terminating application
**                                  through MAC
**                              7 - G.Sup43 Clause 6.2 Terminating application
**                                  through MAC
**                              8 - High Speed CBR 8b10b encoded mode
**                                  Disable disparity inverter on the transmit path
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
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
INT4 hyPhy20gHsciInit(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    INT4 result;
    UINT4 addr, val, lowpwrOff, rstOff;
    UINT4 pmmLp, fcCbrLp, tenGeLp, pmmRst, fcCbrRst, tenGeRst, hsciTopRst;
    UINT4 macGsup43Md, refClkSel, sysMdSel, lnMdSel;
    UINT4 statEn;
    UINT4 rdOutLenOff;
    UINT4 rdOutLenVal;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        mode > HSCI_TRAFFIC_MAX) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
   
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "pDevCtxt is NULL.");

    /* set lowpwr bit to 1 */
    lowpwrOff = HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_ENET_HSCI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                                 lowpwrOff, 1);
    if(result) return result;

    /* reset at top level */
    rstOff = HYPHY20G_TOP_BIT_SYS_SW_RST_2_ENET_HSCI_RST_0_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                 rstOff, 1);
    if(result) return result;

    if (mode == HYPHY20G_DISABLE){           /* disable operations */
        /* free memory used by HSCI subsystem */
        result = hsciHyPhy20gHsciDestroyCtxt(fileHndl, slice);
        if(result) return result;

#ifndef HWACCESSMODE_USB
        /* unconditionally disable FW MSTATX PMON statistics collection */
        result = hyPhy20gHostMsgCmdCfgPmonEnetStatColCtrl(fileHndl, 0,
                                                          slice, 0);
        if(result) return result;
#endif

        return HYPHY20G_SUCCESS;
    }

    /*
     * enable operations
     */
    /* power up the HSCI slice */
    lowpwrOff = HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_ENET_HSCI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                                 lowpwrOff, 0);
    if(result) return result;

    /* remove HSCI subsystem reset */
    rstOff = HYPHY20G_TOP_BIT_SYS_SW_RST_2_ENET_HSCI_RST_0_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                 rstOff, 0);
    if(result) return result;

    /* allocate and initialize memory */
    result = hsciHyPhy20gHsciCreateCtxt(fileHndl, slice, mode);
    if(result) return result;

    /*
     * configure HSCI slice data path
     */
    /* configure HSCI Top level register. */
    switch(mode)
    {
        case HSCI_10GE_MAC_TEMINATION:
        case HSCI_GSUP43_CLAUSE73:
            pmmLp       = 1;
            fcCbrLp     = 1;
            tenGeLp     = 0;
            pmmRst      = 1;
            fcCbrRst    = 1;
            tenGeRst    = 0;
            hsciTopRst  = 0;
            macGsup43Md = 0;
            refClkSel   = 0;
            sysMdSel    = 0;
            lnMdSel     = 0;
            statEn      = 1;
            break;
        case HSCI_FC_8G:
        case HSCI_HS_CBR_32BIT_MODE:
        case HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_EN:
        case HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_DIS:
            pmmLp       = 0;
            fcCbrLp     = 0;
            tenGeLp     = 1;
            pmmRst      = 0;
            fcCbrRst    = 0;
            tenGeRst    = 1;
            hsciTopRst  = 0;
            macGsup43Md = 0;
            refClkSel   = 1;
            sysMdSel    = 1;
            lnMdSel     = 1;
            statEn      = 0;
            break;
        case HSCI_MAC_GSUP43_CLAUSE73:
        case HSCI_MAC_GSUP43_CLAUSE62:
            pmmLp       = 1;
            fcCbrLp     = 1;
            tenGeLp     = 0;
            pmmRst      = 1;
            fcCbrRst    = 1;
            tenGeRst    = 0;
            hsciTopRst  = 0;
            macGsup43Md = 1; /* Line side interface connected to companion 
                                MAC_GSUP43 in ENET_COM subsystem */
            refClkSel   = 1;
            sysMdSel    = 0;
            lnMdSel     = 0;
            statEn      = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_TOP_REG_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_HSCI_TOP_BIT_CFG_LOS_INVERT_MSK,
            HYPHY20G_HSCI_TOP_BIT_CFG_LOS_INVERT_OFF);
    val = mHYPHY20G_SET_FIELD4(val, pmmLp,
            HYPHY20G_HSCI_TOP_BIT_CFG_PMM_LOWPWR_MSK,
            HYPHY20G_HSCI_TOP_BIT_CFG_PMM_LOWPWR_OFF);
    val = mHYPHY20G_SET_FIELD4(val, fcCbrLp,
            HYPHY20G_HSCI_TOP_BIT_CFG_FC_CBR_LOWPWR_MSK,
            HYPHY20G_HSCI_TOP_BIT_CFG_FC_CBR_LOWPWR_OFF);
    val = mHYPHY20G_SET_FIELD4(val, tenGeLp,
            HYPHY20G_HSCI_TOP_BIT_CFG_ENET_10G_LOWPWR_MSK,
            HYPHY20G_HSCI_TOP_BIT_CFG_ENET_10G_LOWPWR_OFF);
    val = mHYPHY20G_SET_FIELD4(val, pmmRst,
            HYPHY20G_HSCI_TOP_BIT_CFG_PMM_RST_MSK,
            HYPHY20G_HSCI_TOP_BIT_CFG_PMM_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, fcCbrRst,
            HYPHY20G_HSCI_TOP_BIT_CFG_FC_CBR_RST_MSK,
            HYPHY20G_HSCI_TOP_BIT_CFG_FC_CBR_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, tenGeRst,
            HYPHY20G_HSCI_TOP_BIT_CFG_ENET_10G_RST_MSK,
            HYPHY20G_HSCI_TOP_BIT_CFG_ENET_10G_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, hsciTopRst,
            HYPHY20G_HSCI_TOP_BIT_CFG_HSCI_RST_MSK,
            HYPHY20G_HSCI_TOP_BIT_CFG_HSCI_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, macGsup43Md,
            HYPHY20G_HSCI_TOP_BIT_CFG_MAC_GSUP43_MODE_SEL_MSK,
            HYPHY20G_HSCI_TOP_BIT_CFG_MAC_GSUP43_MODE_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, refClkSel,
            HYPHY20G_HSCI_TOP_BIT_CFG_REFCLK334_SEL_MSK,
            HYPHY20G_HSCI_TOP_BIT_CFG_REFCLK334_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, sysMdSel,
            HYPHY20G_HSCI_TOP_BIT_CFG_SYS_MODE_SEL_MSK,
            HYPHY20G_HSCI_TOP_BIT_CFG_SYS_MODE_SEL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, lnMdSel,
            HYPHY20G_HSCI_TOP_BIT_CFG_LINE_MODE_SEL_MSK,
            HYPHY20G_HSCI_TOP_BIT_CFG_LINE_MODE_SEL_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* configure data path based on modes */
    if (mode == HSCI_10GE_MAC_TEMINATION) {
        /* configure ENET_10G top level */
        result = hsciHyPhy20gHsci10GeBlockInit(fileHndl, slice);
        if(result) return result;

        /* configure TXXG and RXXG blocks */
        result = hsciHyPhy20gHsci10GeDataPathCfg(fileHndl, slice);
        if(result) return result;

        /* max frame size to 9618 bytes for jumbo frames */
        result = hyPhy20gHsci10GeLanSetFrameSizes(fileHndl, slice,
                                                  0x2592, 0x3C, 0x2592);
        if(result) return result;

        /* configure HSCI GSUP43 block */
        result = hsciHyPhy20gHsciGsup43DataPathCfg(fileHndl, slice);
        if(result) return result;

    } else if (mode == HSCI_GSUP43_CLAUSE73) {
        result = hsciHyPhy20gHsci10GeBlockInit(fileHndl, slice);
        if(result) return result;

        result = hsciHyPhy20gHsciGsup43DataPathCfg(fileHndl, slice);
        if(result) return result;
    } else if (mode == HSCI_FC_8G) {
        result = hsciHyPhy20gHsciFcDataPathCfg(fileHndl, slice);
        if(result) return result;
    } else if (mode == HSCI_HS_CBR_32BIT_MODE ||
               mode == HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_EN ||
               mode == HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_DIS) {
        result = hsciHyPhy20gHsciCbrDataPathCfg(fileHndl, slice);
        if(result) return result;
    } else if (mode == HSCI_MAC_GSUP43_CLAUSE73 ||
               mode == HSCI_MAC_GSUP43_CLAUSE62) {
        /* configure ENET_10G top level */
        result = hsciHyPhy20gHsci10GeBlockInit(fileHndl, slice);
        if(result) return result;
        /* configure TXXG and RXXG blocks */
        result = hsciHyPhy20gHsciMacGsup43DataPathCfg(fileHndl, slice);
        if(result) return result;

        /* max frame size to 9618 bytes for jumbo frames */
        result = hyPhy20gHsci10GeLanSetFrameSizes(fileHndl, slice,
                                                  0x2592, 0x3C, 0x2592);
        if(result) return result;

        /* configure pause frame tag in HSCI Tx ECLASS block */
        result = hsciHyPhy20gTxElcassPauseTagCfg(fileHndl, slice);
        if(result) return result;

        /* configure ENET_COM GSUP43 block */
        result = hyPhy20gEnetComGsup43Init(fileHndl, slice, mode - 5);
        if(result) return result;
    }

    result = hyPhy20gEnetComHsciInit(fileHndl, slice, mode);
    if(result) return result;

    /* configure corresponding HIGH SPEED client port of CPB: HIGH SPEED client 
       port should prepend "Length" header for MAC_GSUP43 C6.2 and C7.3 modes */
    addr = HYPHY20G_CPB_CPB_REG_HI_SPD_PRT_CFG_OFFSET(slice);
    rdOutLenOff = HYPHY20G_CPB_CPB_BIT_HI_SPD_PRT_CFG_HS_RD_OUTPUT_LENGTH_OFF;    
    rdOutLenVal = (mode == HSCI_MAC_GSUP43_CLAUSE73 ||
                   mode == HSCI_MAC_GSUP43_CLAUSE62) ? 1 : 0;
    result = sysHyPhy20gBitWrite(fileHndl, addr, rdOutLenOff, rdOutLenVal);
    if(result) return result;

#ifndef HWACCESSMODE_USB
    /* enable of disable FW MSTATX PMON statistics collection based on mode */
    result = hyPhy20gHostMsgCmdCfgPmonEnetStatColCtrl(fileHndl, 0, slice,
                                                      statEn);
    if(result) return result;
#endif

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciInit */

/*******************************************************************************
**
**  hyPhy20gHsciSelRefClk 
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function is used to control the source clock to the 
**                  HSCI block.
**
**                  Refclk334 must be used for any datapath as below:
**                  XFP<->OTU1e/2e<->HSCI<->... (Gsup43 Clause 7.1 and 7.2 
**                  with MAC Termination)
**
**                  TCLK_TCLK10 must be used for 10GE LAN (MAC Termination)
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  clkMode   - tranmit clock mode of the specific HSCI slice
**                              0 - TCLK_TCLK10
**                              1 - REFCLK334
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
*/
INT4 hyPhy20gHsciSelRefClk(struct file *fileHndl, UINT4 slice, UINT4 clkMode)
{
    INT4 result;

    /* argument checking */
    if (slice > (HYPHY20G_HSCI_NUM_SLICE - 1) ||
        clkMode > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = sysHyPhy20gBitWrite(fileHndl,
        HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_TOP_REG_CFG,slice), 
        HYPHY20G_HSCI_TOP_BIT_CFG_REFCLK334_SEL_OFF, clkMode);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;
        
}/* hyPhy20gHsciSelRefClk */

/*******************************************************************************
**
**  hyPhy20gHsciDataPathEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables the Rx and Tx channel
**                  data flow separately for a specified HSCI slice.
**
**                  This function must be called in order to enable traffic
**                  after the specified HSCI slice has initialized and payload
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
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  dir       - data flow direction
**                              0 - enable/disable the transmit data flow
**                                  (from the CPB to the XFI/SFI)
**                              1 - enable/disable the receive data flow
**                                  (from the XFI/SFI to the CPB)
**                  enable    - 0 - disable
**                              1 - enabled for normal data path
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gHsciDataPathEnable(struct file *fileHndl, UINT4 slice,
                                UINT4 dir, UINT4 enable)
{
    INT4 result;
    UINT4 mode;
    UINT4 macCfgAddr;
    UINT4 hsciGsup43Addr;
    UINT4 ecomGsup43Addr;
    UINT4 fcCbrAddr;
    UINT4 macEnOff;
    UINT4 hsciGsup43RstOff;
    UINT4 ecomGsup43RstOff;
    UINT4 fcCbrRstOff;
    UINT4 reset;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        dir > 1 || enable > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &mode);
    if(result) return result;

    if (dir == HYPHY20G_TX_DIRECTION) {
        macCfgAddr       = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TXXG_REG_CFG_1, slice);   
        hsciGsup43Addr   = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_TX_CFG_1, slice);
        ecomGsup43Addr   = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(HYPHY20G_ENET_COM_GSUP43_REG_RX_CFG, slice);
        fcCbrAddr        = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_TX_CFG, slice);
        macEnOff         = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_TXEN0_OFF;
        hsciGsup43RstOff = HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_1_TX_SW_RST_OFF;
        ecomGsup43RstOff = HYPHY20G_ENET_COM_GSUP43_BIT_RX_CFG_RX_SW_RST_OFF;
        fcCbrRstOff      = HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_SOFT_RST_OFF;
    } else {
        macCfgAddr       = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_RXXG_REG_CFG_1, slice);
        hsciGsup43Addr   = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_RX_CFG, slice);
        ecomGsup43Addr   = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(HYPHY20G_ENET_COM_GSUP43_REG_TX_CFG_1, slice);
        fcCbrAddr        = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_RX_CFG, slice);
        macEnOff         = HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_RXEN_OFF;
        hsciGsup43RstOff = HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_CFG_RX_SW_RST_OFF;
        ecomGsup43RstOff = HYPHY20G_ENET_COM_GSUP43_BIT_TX_CFG_1_TX_SW_RST_OFF;
        fcCbrRstOff      = HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_SOFT_RST_OFF;
    }

    /* configure HSCI Top level register. */
    reset = (enable == 0) ? 1 : 0;    
    switch(mode)
    {
        case HSCI_10GE_MAC_TEMINATION:
            if (dir == HYPHY20G_TX_DIRECTION) {
                /* TX data path */
                result = sysHyPhy20gBitWrite(fileHndl, hsciGsup43Addr, 
                                             hsciGsup43RstOff, reset);
                if(result) return result;
                
                result = sysHyPhy20gBitWrite(fileHndl, macCfgAddr, 
                                             macEnOff, enable);
                if(result) return result;                
            } else {
                /* RX data path */
                result = sysHyPhy20gBitWrite(fileHndl, macCfgAddr, 
                                             macEnOff, enable);
                if(result) return result;
                
                result = sysHyPhy20gBitWrite(fileHndl, hsciGsup43Addr, 
                                             hsciGsup43RstOff, reset);
                if(result) return result;
            }
            break;
        case HSCI_GSUP43_CLAUSE73:
            result = sysHyPhy20gBitWrite(fileHndl, hsciGsup43Addr, 
                                         hsciGsup43RstOff, reset);
            if(result) return result;
            break;
        case HSCI_FC_8G:
        case HSCI_HS_CBR_32BIT_MODE:
        case HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_EN:
        case HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_DIS:
            if (dir == HYPHY20G_TX_DIRECTION) {
                /* TX data path */
                result = sysHyPhy20gBitWrite(fileHndl, fcCbrAddr, 
                                             fcCbrRstOff, reset);
                if(result) return result;
                
                result = hyPhy20gHsciPmmMonEnable(fileHndl, slice, enable);
                if(result) return result;
            } else {
                /* RX data path */
                result = hyPhy20gHsciPmmMkrEnable(fileHndl, slice, enable);
                if(result) return result;

                result = sysHyPhy20gBitWrite(fileHndl, fcCbrAddr, 
                                             fcCbrRstOff, reset);
                if(result) return result;
            }
            break;
        case HSCI_MAC_GSUP43_CLAUSE73:
            if (dir == HYPHY20G_TX_DIRECTION) {
                /* TX data path */
                result = sysHyPhy20gBitWrite(fileHndl, ecomGsup43Addr, 
                                             ecomGsup43RstOff, reset);
                if(result) return result;

                result = sysHyPhy20gBitWrite(fileHndl, hsciGsup43Addr, 
                                             hsciGsup43RstOff, reset);
                if(result) return result;

                result = sysHyPhy20gBitWrite(fileHndl, macCfgAddr, 
                                             macEnOff, enable);
                if(result) return result;
            } else {
                /* RX data path */
                result = sysHyPhy20gBitWrite(fileHndl, macCfgAddr, 
                                             macEnOff, enable);
                if(result) return result;
                
                result = sysHyPhy20gBitWrite(fileHndl, hsciGsup43Addr, 
                                             hsciGsup43RstOff, reset);
                if(result) return result;
                
                result = sysHyPhy20gBitWrite(fileHndl, ecomGsup43Addr, 
                                             ecomGsup43RstOff, reset);
                if(result) return result;
            }
            break;
        case HSCI_MAC_GSUP43_CLAUSE62:
            if (dir == HYPHY20G_TX_DIRECTION) {
                /* TX data path */
                result = sysHyPhy20gBitWrite(fileHndl, ecomGsup43Addr, 
                                             ecomGsup43RstOff, reset);
                if(result) return result;

                result = sysHyPhy20gBitWrite(fileHndl, hsciGsup43Addr, 
                                             hsciGsup43RstOff, reset);
                if(result) return result;

                result = sysHyPhy20gBitWrite(fileHndl, macCfgAddr, 
                                             macEnOff, enable);
                if(result) return result;
            } else {
                /* RX data path */
                result = sysHyPhy20gBitWrite(fileHndl, macCfgAddr, 
                                             macEnOff, enable);
                if(result) return result;
            }
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciDataPathEnable */


/*******************************************************************************
**
**  hyPhy20gHsci10GeLanTxxgCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure TXXG block Configuration 1 register within an
**                  ENET_10G slice.
**
**                  Refer to 0x4916800 + 0x8000*A(A=0:1) for further details
**                  on the functionalities configured with this function.
**
**                  Used for 10GE MAC Terminated mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gHsciInit but prior to hyPhy20gHsciDataPathEnable
**                  to modify the basic 10GE MAC Termination mode configuration.
**
**                  Default settings provided in argument descriptions are
**                  relative to the device state after hyPhy20gHsciInit has
**                  been called.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI slice 0
**                              1 - HSCI slice 1
**                  ipg       - Back-to-Back Transmit IPG
**                              Legal range 0x00 - 0x3F
**                              Default setting b'001100
**                  ipgReducEn - 0 - disable IPG reduction feature
**                               1 - enable IPG reduction feature
**                               Default setting 1
**                               If set to 0, the Tx MAC generates backpressure
**                               via CPB to the data source to ensure it`s rate
**                               is ethernet compliant.  Rate is achieved by
**                               forcing the interpacket gap.  If set to 1,
**                               it is assumed that the data stream is rate
**                               compliant and IPG are reduced to ensure
**                               CPB FIFO drainage is maximized.
**                  crcAppendEn - 0 - frames transmitted will not have a
**                                    CRC appended
**                                1 - all frames transmitted will have a
**                                    4 byte CRC appended
**                                Default setting 0
**                  frmPadEn - 0 - not pad
**                             1 - TXXG will pad all IEEE 802.3 transmitted
**                                 frames that are less than the minimum size
**                                 up to the minimum size for a valid 802.3
**                                 frame
**                             Default setting 0
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeLanTxxgCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 ipg, UINT4 ipgReducEn,
                                UINT4 crcAppendEn, UINT4 frmPadEn)
{
    INT4 result;
    UINT4 addr, val;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        ipg > 0x3F      ||
        ipgReducEn > 1  ||
        crcAppendEn > 1 ||
        frmPadEn > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }


    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TXXG_REG_CFG_1, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, ipg,
            HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_IPGT_MSK,
            HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_IPGT_OFF);
    val = mHYPHY20G_SET_FIELD4(val, ipgReducEn,
            HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_IPG_REDUC_EN_MSK,
            HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_IPG_REDUC_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, crcAppendEn,
            HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_CRCEN_MSK,
            HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_CRCEN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, frmPadEn,
            HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_PADEN_MSK,
            HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_PADEN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeLanTxxgCfg */


/*******************************************************************************
**
**  hyPhy20gHsci10GeLanRxxgCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure RXXG block Configuration 1 and Configuration 2
**                  registers within an ENET_10G slice.
**
**                  Refer to 0x4914200 + 0x8000*A(A=0:1) and
**                  0x4914200 + 0x8000*A(A=0:1) for further details on the
**                  functionalities configured with this function.
**
**                  Used for 10GE MAC Terminated mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gHsciInit but prior to hyPhy20gHsciDataPathEnable
**                  to modify the basic 10GE MAC Termination mode configuration.
**
**                  Default settings provided in argument descriptions are
**                  relative to the device state after hyPhy20gHsciInit has
**                  been called.
**
**
**  INPUTS:         fileHndl    - device handle
**                  slice       - HSCI slice processing resource
**                                0 - HSCI slice 0
**                                1 - HSCI slice 1
**                  oversizeResp - Respond to Oversize Control Frames
**                                 0 - only pause when 64 byte pause frames are
**                                     received
**                                 1 - pause when an oversize (>=64) byte pause
**                                     frame is received
**                               Default setting 0
**                  mifgEn - 0 - inter-packet gap is not checked
**                           1 - IPG is compared with the programmed Minimum IPG
**                               Default setting 1
**                  noPreamble - 0 - search and strip preamble and SFD byte
**                                  before further packet processing
**                               1 - skip over the search for preamble and SFD
**                               Default setting 0
**                  purePreamble - Pure Preamble
**                                 0 - no preamble checking
**                                 1 - check the preamble field of the packet,
**                                     ensuring a data pattern of 0x55
**                                 Default setting 1
**                  longPreamble - Long Preamble
**                                 0 - discard packets with >11 bytes of preamble
**                                 1 - permit reception of packets with
**                                     any number of preamble
**                               Default setting 0
**                  passAll     - Pass All Receive Frames
**                                0 - process PAUSE Control frames
**                                1 - not process PAUSE Control frames and
**                                    forward the packet
**                                Default setting 0
**                  frmLenCheck - Frame Length Check
**                                0 - don't perform Frame Length Check
**                                1 - compare the actual frame length against
**                                    the value contained in the Type/Length
**                                    field of the MAC header
**                                Default setting 0
**                  passErrFrm - Pass Error Frames
**                               0 - filter all errored frames
**                               1 - forward all errored frames
**                               Default setting 1
**                  passCtlFrm - Frame Length Check
**                               0 - filter MAC Control non-PAUSE frames
**                               1 - forward MAC Control non-PAUSE frames
**                               Default setting 1
**                  contiguousEn - Receive Packet Contiguous
**                                 0 - gaps are allowed
**                                 1 - gaps cause the packet to be dropped
**                                 Default setting 0
**                  crcStrip - Frame Length Check
**                             0 - packet is forwarded with the CRC appended
**                             1 - strip the CRC from the packet
**                             Default setting 0
**                  mifg - Defines the minimum supported inter-frame gap
**                              b'000 -  8  bytes
**                              b'001 -  12 bytes
**                              b'010 -  16 bytes
**                              b'011 -  20 bytes
**                              b'1XX -  0  bytes
**                              Default setting b'100
**                  crcCkDis - controls data path CRC checker
**                             0 - CRC checker is enabled
**                             1 - CRC checker is disabled
**                             Default setting 0
**                  shortPktEn - processing of short packets
**                               0 - minimum frame size is 64 bytes. Packets
**                                   Packets less than 64 bytes will be filtered
**                                   due to minimum length error.
**                               1 - all packets less than 32 bytes are
**                                   considered short packets
**                               Default setting 0
**                  hdrPrmblEn - enables the processing of packets with standard
**                               preamble in the EOS header mode
**                               0 - All packets are considered to have a custom
**                                   header of fixed size equal to the value
**                                   configured in the HDR_SIZE field
**                               1 - The EOS header is scanned for standard
**                                   preamble sequence
**                               Default setting 0
**                  hdrSize - EOS frame delineation header length (in bytes),
**                            which is to be skipped over
**                            Legal range 0 to 0xFF
**                            Default setting 0
**                  cutThruThreshold - Cut-Thru threshold select, sets the 
**                                     forwarding threshold in the internal 
**                                     receive FIFO for initiating packet 
**                                     transfer to the System FIFO interface
**                                     Legal range 0 to 5
**                                     Default setting b'001
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeLanRxxgCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 oversizeResp,
                                UINT4 mifgEn, UINT4 noPreamble,
                                UINT4 purePreamble, UINT4 longPreamble,
                                UINT4 passAll, UINT4 frmLenCheck,
                                UINT4 passErrFrm, UINT4 passCtlFrm,
                                UINT4 contiguousEn, UINT4 crcStrip,
                                UINT4 mifg, UINT4 crcCkDis, UINT4 shortPktEn,
                                UINT4 hdrPrmblEn, UINT4 hdrSize,
                                UINT4 cutThruThreshold)
{
    INT4 result;
    UINT4 addr, val;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        oversizeResp > 1 ||
        mifgEn > 1 ||
        noPreamble > 1 ||
        purePreamble > 1 ||
        longPreamble > 1 ||
        passAll > 1 ||
        frmLenCheck > 1 ||
        passErrFrm > 1 ||
        passCtlFrm > 1 ||
        contiguousEn > 1 ||
        crcStrip > 1 ||
        mifg > 7 ||
        crcCkDis > 1 ||
        shortPktEn > 1 ||
        hdrPrmblEn > 1 ||
        hdrSize > 0xFF ||
        cutThruThreshold > 5) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_RXXG_REG_CFG_1, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, oversizeResp,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_ROCF_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_ROCF_OFF);
    val = mHYPHY20G_SET_FIELD4(val, mifgEn,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_MIFG_EN_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_MIFG_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, noPreamble,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_NO_PRMB_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_NO_PRMB_OFF);
    val = mHYPHY20G_SET_FIELD4(val, purePreamble,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PUREP_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PUREP_OFF);
    val = mHYPHY20G_SET_FIELD4(val, longPreamble,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_LONGP_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_LONGP_OFF);
    val = mHYPHY20G_SET_FIELD4(val, passAll,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PARF_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PARF_OFF);
    val = mHYPHY20G_SET_FIELD4(val, frmLenCheck,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_FLCHK_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_FLCHK_OFF);
    val = mHYPHY20G_SET_FIELD4(val, passErrFrm,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_ERRORS_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_ERRORS_OFF);
    val = mHYPHY20G_SET_FIELD4(val, passCtlFrm,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_CTRL_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_CTRL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, contiguousEn,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_RX_CONTIG_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_RX_CONTIG_OFF);
    val = mHYPHY20G_SET_FIELD4(val, crcStrip,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_CRC_STRIP_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_CRC_STRIP_OFF);
    val = mHYPHY20G_SET_FIELD4(val, mifg,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_MIFG_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_MIFG_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_RXXG_REG_CFG_2, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, crcCkDis,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_2_DIS_CRC_CHK_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_2_DIS_CRC_CHK_OFF);
    val = mHYPHY20G_SET_FIELD4(val, shortPktEn,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_2_SHORTPKT_EN_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_2_SHORTPKT_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, hdrPrmblEn,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_2_HDR_AND_PRMBL_EN_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_2_HDR_AND_PRMBL_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, hdrSize,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_2_HDRSIZE_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_2_HDRSIZE_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_RXXG_REG_CUT_THRU_TH_SEL, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CUT_THRU_TH_SEL_CUT_THRU_THRES_SEL_MSK,
                cutThruThreshold);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeLanRxxgCfg */


/*******************************************************************************
**
**  hyPhy20gHsciRxxgPrmbCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures a specific way to process preamble
**                  in the RXXG block.
**                  
**                  When the prmbProc == 0:
**                    RXXG block will strip out Preamble/SFD;
**                    RXXG block accepts 0-11 bytes of non-0xD5 byte values
**                    followed by SFD byte;
**                  When the prmbProc == 1:
**                    RXXG block will strip out Preamble/SFD;
**                    RXXG block accepts any number of non-0xD5 byte values
**                    followed by SFD byte;
**                  When the prmbProc == 2:
**                    RXXG block will strip out Preamble/SFD;
**                    RXXG block accepts 0-11 bytes of 0x55 byte values
**                    followed by SFD byte;
**                  When the prmbProc == 3:
**                    RXXG block will strip out Preamble/SFD;
**                    RXXG block accepts any number of 0x55 byte values 
**                    followed by SFD byte;
**                  When the prmbProc == 4:
**                    RXXG block passes custom header to downstream;
**                    RXXG block expects all packets to have a custom header 
**                    of fixed size specified by 'hdrSize' parameter;
**                    Here hdrSize == 0 is a special case, and it means RXXG 
**                    block expects no preamble/SFD. 
**                  When the prmbProc == 5:
**                    RXXG block passes custom header to downstream;
**                    RXXG block scans custom header (in the range of byte 1  
**                    to 'hdrSize') for the standard preamble sequence (number
**                    of 0x55 bytes followed by a 0xD5 byte);
**                    If this sequence is found, the DA bytes are counted 
**                    from the byte after the 0xD5 byte; 
**                    If this sequence is not found, the DA bytes are counted 
**                    from the byte after the custom header (header size 
**                    specified by 'hdrSize' parameter); 
**                  
**                  Note: 
**                    The prmbProc == 0 ~ 3 are standard IEEE 802.3 application;
**                    The prmbProc == 4 is custom preamble application;
**                    The prmbProc == 5 supports custom preamble and is 
**                    typically used in EOS application; If the 'hdrSize' > 7 
**                    in this scenario, RXXG supports packets mixed of standard
**                    preamble and custom preamble.  
**
**                  This API function is only used for 10GE MAC Terminated,
**                  G.Sup43 Clause 7.3 Terminating application 
**                  through MAC modes.
**
**                  Note:
**                    HDR_AND_PRMBL_EN bit can only be changed when RXXG block
**                    is disabled (RXEN is b'0).
**
**                  Note:
**                    The register bits configured in this API function 
**                    overlap with certain bits configured in 
**                    hyPhy20gHsci10GeLanRxxgCfg API.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  prmbProc  - determines how RXXG processes preamble; 
**                              refer to API description for details; 
**                              Legal range 0 to 5
**                  hdrSize   - specifies custom header size (in bytes);
**                              When prmbProc == 0 ~ 3, set to 0;
**                              When prmbProc == 4, Legal range 0 to 0xFF;
**                              When prmbProc == 5, Legal range 1 to 0xFF.
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciRxxgPrmbCfg(struct file *fileHndl, UINT4 slice, 
                             UINT4 prmbProc, UINT4 hdrSize)
{
    INT4 result;
    
    result = hsciHyPhy20gHsciRxxgPrmbCfg(fileHndl, slice, prmbProc, hdrSize);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciRxxgPrmbCfg */



/*******************************************************************************
**
**  hyPhy20gHsciGsup43C73ModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures both Tx and Rx GSUP43 blocks in
**                  the HSCI subsystem. This function wraps
**                  hsciHyPhy20gHsciGsup43TxCfg and hsciHyPhy20gHsciGsup43RxCfg
**                  function calls, and not intented to use with them at API
**                  level at the same time.
**
**                  Only used for G.Sup43 Clause 7.3 Terminating application
**                  bypass MAC operating mode (mode '2' in hyPhy20gHsciInit).
**
**                  This function can optionally be called after
**                  hyPhy20gHsciInit but prior to hyPhy20gHsciDataPathEnable
**                  to modify the basic G.Sup43 Clause 7.3 mode configuration.
**
**                  Default settings provided in argument descriptions are
**                  relative to the device state after hyPhy20gHsciInit has
**                  been called.
**
**  INPUTS:         fileHndl   - device handle
**                  slice      - HSCI slice processing resource
**                               0 - HSCI slice 0
**                               1 - HSCI slice 1
**                  txErrDrop  - when incoming packet has an UPI TYPE value
**                               that does not match either TX_PKT_TYPE or
**                               TX_OS_TYPE:
**                               0 - incoming data packet treated as a
**                                   data packet
**                               1 - incoming packet will be dropped
**                               Default setting 1
**                  txPktType  - identifies the transmit data as a 10GE
**                               data frame, as opposed to an ordered set.
**                               this value should be configured to match
**                               the UPI value of the GFP-F frame
**                               Default setting b'11111101
**                  txOsType   - identifies the transmit data as 10GE ordered
**                               set, as opposed to a data packet.
**                               this value should be configured to match
**                               the UPI value of the GFP-F frame
**                               Default setting b'11111110
**                  txMinIpg   - forces the minimum IPG value to 0 bytes
**                               when the downstream packet buffer is full
**                               0 - average minimum IPG is set to 12 bytes
**                                   or 8 bytes
**                               1 - IPG is set to 0 bytes and IPG insertion
**                                   must be controlled by the upstream source
**                               Default setting 0
**                  unidirEn   - enable/disable the unidirectional enable
**                               parameter from IEEE 802.3
**                               0 - Unidirectional operation is disabled
**                               1 - Unidirectional operation is enabled
**                               Default setting 0
**                  sendIdleEn - enable/disable idle generation when RF ordered
**                               sets are detected in the receive PCS
**                               0 - disable idle generation
**                               1 - enable idle generation
**                               Default setting 1
**                  fifoHmark  - identifies the number of 16-byte stored in
**                               the internal FIFO which transfers from
**                               the system interface
**                               Default setting b'00110
**                  losInv     - configures the assertive state of the
**                               LOS input signal
**                               0 - LOS is expected to be asserted when the
**                                   corresponding input signal is logic 1
**                               1 - LOS is expected to be asserted when the
**                                   corresponding input signal is logic 0
**                               Default setting 0
**                  dscrdZone  - sets the fill level of the downstream packet
**                               buffer at which ordered set discard mechanism
**                               is enabled.
**                               b'00 - b'11
**                               Default setting b'10
**                  sigEn      - enables the discard mechanism for signal
**                               ordered sets
**                               0 - Signal ordered sets are not considered in
**                                   the discard rate adaptation mechanism
**                               1 - Signal ordered sets are considered in the
**                                   discard rate adaptation mechanism
**                               Default setting 0
**                  seqEn      - enables the discard mechanism for
**                               sequence ordered sets
**                               0 - Sequence ordered sets are not considered
**                                   in the discard rate adaptation mechanism
**                               1 - Sequence ordered sets are considered
**                                   in the discard rate adaptation mechanism
**                               Default setting 1
**                  allDscrd   - enables the discard of all ordered sets
**                               0 - specific ordered sets in the received
**                                   data path are discarded
**                               1 - All ordered sets in the received data path
**                                   are discarded
**                               Default setting 0
**                  osPrtDis   - disables the function of RX_MIN_OS_RPT[3:0]
**                               when the downstream packet buffer is full
**                               0 - RX_MIN_OS_RPT is always regarded
**                               1 - RX_MIN_OS_RPT is ignored when the
**                                   downstream packer buffer is full
**                               Default setting 0
**                  minOsPrt   - sets the minimum number of consecutive
**                               identical ordered sets on the received
**                               data path that must be detected before they
**                               are eligible for discard
**                               0 - ordered set may be discarded, based on the
**                                   fill-level of the downstream buffer (CPB)
**                               N - the previous N-1 received ordered sets must
**                                   be identical to the current ordered set
**                                   before the current ordered set is
**                                   considered eligible for the discard.
**                               Default setting b'0011
**                  rxPktType  - identifies the received data as an 10GE data
**                               frame as opposed to an ordered set
**                               Default setting b'11111101
**                  rxOsType   - identifies the received data as an 10GE ordered
**                               set as opposed to a data packet
**                               Default setting b'11111110
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciGsup43C73ModeCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 txErrDrop, UINT4 txPktType,
                                  UINT4 txOsType, UINT4 txMinIpg,
                                  UINT4 unidirEn, UINT4 sendIdleEn,
                                  UINT4 fifoHmark,
                                  UINT4 losInv, UINT4 dscrdZone, UINT4 sigEn,
                                  UINT4 seqEn, UINT4 allDscrd, UINT4 osPrtDis,
                                  UINT4 minOsPrt, UINT4 rxPktType,
                                  UINT4 rxOsType)
{
    INT4 result;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = hsciHyPhy20gHsciGsup43TxCfg(fileHndl, slice, txErrDrop, txPktType,
                                         txOsType, txMinIpg, unidirEn, 
                                         sendIdleEn, fifoHmark);
    if(result) return result;

    result = hsciHyPhy20gHsciGsup43RxCfg(fileHndl, slice, losInv, dscrdZone, 
                                         sigEn, seqEn, allDscrd, osPrtDis,
                                         minOsPrt, rxPktType, rxOsType);

    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciGsup43C73ModeCfg */

/*******************************************************************************
**
**  hyPhy20gHsciFc8GModeCfg
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
**                  In order to ensure the RX_BAD_CRC and TX_BAD_CRC counting
**                  correctly, user must configure de-scrable setting
**                  in Tx and Rx framer to match incoming payload's scrabling
**                  state.
**
**                  Used for FC 8G mode.
**
**                  This function can optionally be called after
**                  hyPhy20gHsciInit but prior to hyPhy20gHsciDataPathEnable.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
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
**                  rxDeScr   - controls whether Rx framer de-scramble incoming
**                              frame payload.
**                              0 - don't de-scramble incoming payload (default)
**                              1 - de-scramble payload before CRC computation
**                  txDeScr   - controls whether Tx framer de-scramble incoming
**                              frame payload.
**                              0 - don't de-scramble incoming payload (default)
**                              1 - de-scramble payload before CRC computation
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
INT4 hyPhy20gHsciFc8GModeCfg(struct file *fileHndl, UINT4 slice, UINT4 osCfgMsk,
                             UINT4 arbLsbMsk, UINT4 arbLsbVal, UINT4 rxAlignMd, 
                             UINT4 txAlignMd, UINT4 rxDeScr, UINT4 txDeScr)
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
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        osCfgMsk > 0xF      || 
        arbLsbMsk > 0xFFFFF ||
        arbLsbVal > 0xFFFFF ||
        rxAlignMd > 1       ||
        txAlignMd > 1       ||
        rxDeScr > 1         || 
	    txDeScr > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* configure ordered set control register */
    addr = HYPHY20G_HSCI_FC_CBR_REG_OS_CTL;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gWrite(fileHndl, addr, osCfgMsk);
    if(result) return result;

    /* configure mask of least-significant 20-bits of ARB ordered set */
    addr = HYPHY20G_HSCI_FC_CBR_REG_ARB_VAL_EN;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gWrite(fileHndl, addr, arbLsbMsk);
    if(result) return result;

    /* configure least-significant 20-bits of ARB ordered set */
    addr = HYPHY20G_HSCI_FC_CBR_REG_ARB_VAL;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gWrite(fileHndl, addr, arbLsbVal);
    if(result) return result;

    /* configure RX_FC_CONT_ALIGN_MODE */
    addr = HYPHY20G_HSCI_FC_CBR_REG_RX_CFG;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, rxAlignMd,
                    HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_FC_CONT_ALIGN_MODE_MSK,
                    HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_FC_CONT_ALIGN_MODE_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* configure TX_FC_CONT_ALIGN_MODE */
    addr   = HYPHY20G_HSCI_FC_CBR_REG_TX_CFG;
    addr   = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    offset = HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_FC_CONT_ALIGN_MODE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, txAlignMd);
    if(result) return result;

    /* configure scramble and de-scramble setting for TX and RX Framer */
    addr = HYPHY20G_HSCI_FC_CBR_REG_CFG;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, rxDeScr,
                    HYPHY20G_HSCI_FC_CBR_BIT_CFG_RX_FRM_SCRAM_MSK,
                    HYPHY20G_HSCI_FC_CBR_BIT_CFG_RX_FRM_SCRAM_OFF);

    val = mHYPHY20G_SET_FIELD4(val, txDeScr,
                    HYPHY20G_HSCI_FC_CBR_BIT_CFG_TX_FRM_SCRAM_MSK,
                    HYPHY20G_HSCI_FC_CBR_BIT_CFG_TX_FRM_SCRAM_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciFc8GModeCfg */


/*******************************************************************************
**
**  hyPhy20gHsciCbr10GModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This dummy function may performs payload specific
**                  configuration for a slice configured for High speed CBR
**                  mode (including STS-192, 10GE LAN G.Sup43 7.2 and
**                  10GE LAN G.Sup43 7.3) in further.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciCbr10GModeCfg(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* TBD */

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciCbr10GModeCfg */

/*******************************************************************************
**
**  hyPhy20gHsciCbrUsrPattCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure user pattern and running disparity for FC_CBR
**                  block.
**
**
**  INPUTS:         fileHndl   - device handle
**                  slice      - HSCI slice processing resource
**                               0 - HSCI instance 0
**                               1 - HSCI instance 1
**                  dir        - data flow direction
**                               0 - enable/disable the transmit data flow
**                                   (from the CPB to the SFP)
**                               1 - enable/disable the receive data flow
**                                   (from the SFP to the CPB)
**                  olSeqHi    - specifies the user defined pattern
**                  olSeqLow   - specifies the user defined pattern
**                  pattDisp   - specifies the running disparity at the end of
**                               each 10-bit user configured pattern
**                               (only useful when dir == 0)
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciCbrUsrPattCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 dir, UINT4 olSeqHi, UINT4 olSeqLow,
                               UINT4 pattDisp)
{
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 || dir > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = hsciHyPhy20gHsciFcCbrUsrPattCfg(fileHndl, slice, dir,
                                             olSeqHi, olSeqLow, pattDisp);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciCbrUsrPattCfg */


/*******************************************************************************
**
**  hyPhy20gHsciMacGsup43C73ModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs payload specific configuration 
**                  for a HSCI slice configured for G.Sup43 Clause 7.3 
**                  Terminating application through MAC mode.
**
**                  The MAC G.Sup43 Clause 7.3 data path supports two possible
**                  payloads souring from PIF in Tx data path:
**                    Ethernet Payload (DA, SA, Length/Type, DATA, FCS), and
**                    Extended Ethernet Payload (Preamble, DA, SA, Length/Type,
**                    DATA, FCS).
**
**                  As G.Sup43 spec defined, both Payload and Preamble
**                  must be transparently transported, and Preamble along with
**                  GFP header and SFD are considered as header format, 
**                  the 10GE TMAC (TXXG block) must be properly present
**                  the preamble to downstream, according to upstream (from PIF)
**                  payload type.
**                  This function configures TXXG block basing on upstream 
**                  payload type (specified by 'txPayload' parameter).
**
**                  This function internally enable MAC_GSUP43_MODE_EN bit 
**                  as the last step of configuration.
**
**                  In the Rx path, user must select whether RXXG strip CRC
**                  from packets depending on whether PIF expects packets with   
**                  or without CRC. 
**
**                  When HSCI slice is configured in MAC G.Sup43 Clause 7.3
**                  mode, this function must be called after hyPhy20gHsciInit
**                  but prior to hyPhy20gHsciDataPathEnable (enable operation).
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  txPayload - payload type of packets from PIF (in Tx path)
**                              0 - Ethernet Payload
**                              1 - Extended Ethernet Payload with preamble
**                  txFcsAppend - whether TXXG append CRC to packets
**                              0 - don't append CRC when packets from CPB 
**                                  already have CRC attached;
**                              1 - append CRC if CRC of the packets from CPB
**                  rxFcsStrip  - whether RXXG strip CRC from packets
**                              0 - don't strip CRC from packets 
**                              1 - strip CRC from packets
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciMacGsup43C73ModeCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 txPayload, UINT4 txFcsAppend,
                                     UINT4 rxFcsStrip)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 msk;
    UINT4 hdrSizeVal;
    UINT4 txPrmb;
    UINT4 spreVal;
    UINT4 gsup43MdEnOff;
    UINT4 gsup43MdEnVal;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        txPayload > 1 ||
        txFcsAppend > 1 ||
        rxFcsStrip > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    gsup43MdEnOff = HYPHY20G_HSCI_ENET_10G_TOP_BIT_MAC_GSUP43_CFG_MAC_GSUP43_MODE_EN_OFF;

    /* select configuration value basing on Tx payload type */
    switch(txPayload)
    {
        case 0:
            hdrSizeVal      = 4;
            txPrmb          = 1;    /* prepend preamble in egress */
            spreVal         = 0;    /* prepend preamble in egress */
            break;
        case 1:
            hdrSizeVal      = 0xC;
            txPrmb          = 0;    /* don't prepend preamble in egress */
            spreVal         = 1;    /* don't prepend preamble in egress */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /*
     * configure MAC G.Sup43 related register bits at ENET_10G top level
     */
    addr = HYPHY20G_HSCI_ENET_10G_TOP_REG_MAC_GSUP43_CFG;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);

    /* check if MAC_GSUP43_MODE_EN is enabled */
    result = sysHyPhy20gBitRead(fileHndl, addr, gsup43MdEnOff, &gsup43MdEnVal);
    if(result) return result;

    if (gsup43MdEnVal) {
        /* disable MAC_GSUP43_MODE_EN bit prior to configuration */
        result = sysHyPhy20gBitWrite(fileHndl, addr, gsup43MdEnOff, 0);
        if(result) return result;
    }

    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TOP_BIT_MAC_GSUP43_CFG_TXXG_FCS_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, txFcsAppend);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TOP_BIT_MAC_GSUP43_CFG_TXXG_PRMB_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, txPrmb);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    /*
     * configure TXXG block
     */
    addr = HYPHY20G_HSCI_ENET_10G_TXXG_REG_CFG_1;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_CRCEN_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, txFcsAppend);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_PADEN_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, txFcsAppend);
    if(result) return result;
    
    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_SPRE_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, spreVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    addr = HYPHY20G_HSCI_ENET_10G_TXXG_REG_CFG_2;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_2_HDRSIZE_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, hdrSizeVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    /* enable MAC_GSUP43 mode */
    addr = HYPHY20G_HSCI_ENET_10G_TOP_REG_MAC_GSUP43_CFG;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr, gsup43MdEnOff, 1);
    if(result) return result;

    /*
     * configure RXXG block
     */
    addr = HYPHY20G_HSCI_ENET_10G_RXXG_REG_CFG_1;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_CRC_STRIP_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, rxFcsStrip);
    if(result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciMacGsup43C73ModeCfg */


/*******************************************************************************
**
**  hyPhy20gHsciMacGsup43C62ModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs payload specific configuration
**                  for a HSCI slice configured for G.Sup43 Clause 6.2 
**                  Terminating application through MAC mode.
**
**                  In the Tx path, user must select whether TXXG appends CRC 
**                  to packets depending on whether payloads souring from PIF 
**                  include CRC. 
**
**                  This function internally enable MAC_GSUP43_MODE_EN bit 
**                  as the last step of egress configuration.
**
**                  In the Rx path, user must select whether RXXG strip CRC
**                  from packets depending on whether PIF expects packets with   
**                  or without CRC. 
**
**                  When HSCI slice is configured in MAC G.Sup43 Clause 6.2
**                  mode, this function must be called after hyPhy20gHsciInit
**                  but prior to hyPhy20gHsciDataPathEnable (enable operation).
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  txFcsAppend - whether TXXG append CRC to packets
**                              0 - don't append CRC when packets from CPB 
**                                  already have CRC attached;
**                              1 - append CRC if CRC of the packets from CPB
**                                  was stripped.
**                  rxFcsStrip  - whether RXXG strip CRC from packets
**                              0 - don't strip CRC from packets 
**                              1 - strip CRC from packets
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciMacGsup43C62ModeCfg(struct file *fileHndl, UINT4 slice, 
                                     UINT4 txFcsAppend, UINT4 rxFcsStrip)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 msk;
    UINT4 gsup43MdEnOff;
    UINT4 gsup43MdEnVal;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        txFcsAppend > 1 ||
        rxFcsStrip > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    gsup43MdEnOff = HYPHY20G_HSCI_ENET_10G_TOP_BIT_MAC_GSUP43_CFG_MAC_GSUP43_MODE_EN_OFF;

    /*
     * configure MAC G.Sup43 related register bits at ENET_10G top level
     */
    addr = HYPHY20G_HSCI_ENET_10G_TOP_REG_MAC_GSUP43_CFG;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);

    /* check if MAC_GSUP43_MODE_EN is enabled */
    result = sysHyPhy20gBitRead(fileHndl, addr, gsup43MdEnOff, &gsup43MdEnVal);
    if(result) return result;

    if (gsup43MdEnVal) {
        /* disable MAC_GSUP43_MODE_EN bit prior to configuration */
        result = sysHyPhy20gBitWrite(fileHndl, addr, gsup43MdEnOff, 0);
        if(result) return result;
    }

    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TOP_BIT_MAC_GSUP43_CFG_TXXG_FCS_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, txFcsAppend);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    /*
     * configure TXXG block
     */
    addr = HYPHY20G_HSCI_ENET_10G_TXXG_REG_CFG_1;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_CRCEN_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, txFcsAppend);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_PADEN_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, txFcsAppend);
    if(result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    /* enable MAC_GSUP43 mode */
    addr = HYPHY20G_HSCI_ENET_10G_TOP_REG_MAC_GSUP43_CFG;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr, gsup43MdEnOff, 1);
    if(result) return result;

    /*
     * configure RXXG block
     */
    addr = HYPHY20G_HSCI_ENET_10G_RXXG_REG_CFG_1;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_CRC_STRIP_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, rxFcsStrip);
    if(result) return result;
    
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;
    
} /* hyPhy20gHsciMacGsup43C62ModeCfg */



/*******************************************************************************
**
**  hyPhy20gHsciMacGsup43RxCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs ingress (from GFP-F, through HSCI 
**                  and come into CPB channel) configuration for G.Sup43 Clause  
**                  7.3 and Clause 6.2 Terminating application through MAC mode.
**
**                  This function selects various packet drop or pass in the 
**                  corresponding RXXG and ENET_COM Tx GSUP43 blocks.
**
**                  When HSCI slice is configured in MAC G.Sup43 Clause 7.3 or
**                  Clause 6.2 Terminating application through MAC mode, this 
**                  function can be optionally called after hyPhy20gHsciInit 
**                  but prior to hyPhy20gHsciDataPathEnable (enable operation).
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  txGSup43Drop
**                              - whether ENET_COM Tx GSUP43 block drop packets
**                                that have UPI TYPE value mistached with 
**                                either TX_PKT_TYPE or TX_OS_TYPE.
**                              0 - don't drop error packets
**                              1 - drop error packets
**                  rxxgPassCfg 
**                              - each bit of this parameter controls one type
**                                of frame pass or drop in RXXG block.
**                                  bit 0 - drop/pass control frames
**                                          0 - drop control frames
**                                          1 - pass control frames
**                                  bit 1 - drop/pass error frames
**                                          0 - drop error frames
**                                          1 - pass error frames
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
INT4 hyPhy20gHsciMacGsup43RxCfg(struct file *fileHndl, UINT4 slice, 
                                UINT4 txGSup43Drop, UINT4 rxxgPassCfg)
{
    INT4 result;
    UINT4 mode;
    UINT4 addr;
    UINT4 val;
    UINT4 passErrFrm;
    UINT4 passCtlFrm;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        txGSup43Drop > 1 ||
        rxxgPassCfg > 0x3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &mode);
    if(result) return result;
    /* confirm mode before processing */
    if (mode != HSCI_MAC_GSUP43_CLAUSE73 &&
        mode != HSCI_MAC_GSUP43_CLAUSE62) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    if (mode == HSCI_MAC_GSUP43_CLAUSE73) {
        addr = HYPHY20G_ENET_COM_GSUP43_REG_TX_CFG_1;
        addr = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(addr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_ENET_COM_GSUP43_BIT_TX_CFG_1_TX_ERR_DROP_OFF,
                    txGSup43Drop);
        if(result) return result;
    }

    passErrFrm = (rxxgPassCfg & 0x2) ? 1 : 0;
    passCtlFrm = (rxxgPassCfg & 0x1) ? 1 : 0;
    
    /* configure pass/drop configure bits in the RXXG block */
    addr = HYPHY20G_HSCI_ENET_10G_RXXG_REG_CFG_1;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, passErrFrm,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_ERRORS_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_ERRORS_OFF);
    val = mHYPHY20G_SET_FIELD4(val, passCtlFrm,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_CTRL_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_CTRL_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;
    
} /* hyPhy20gHsciMacGsup43RxCfg */


/*******************************************************************************
**
**  hyPhy20gHsci10GeLanSetFrameSizes
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function sets the minimum and maximum frame sizes for
**                  the TXXG and RXXG block.  Both the Rx and Tx channels must
**                  be disabled before this function can be called.
**
**                  The default value of max/min frame length registers are
**                  IEEE 802.3 conformance, This function optionally configures
**                  the different frame length restrictions if desired.
**
**                  Set Maximum Frame Size to 9618 bytes for jumbo frames;
**                  Set Maximum Frame Size to 9622 bytes for VLAN_DOUBLE_TAG
**                  jumbo frames.
**
**                  Used for 10GE MAC Terminated mode and G.Sup43 Clause 7.3 and 
**                  Clause 6.2 Terminating application through MAC modes.
**
**                  This function must be called after
**                  hyPhy20gHsciInit but prior to hyPhy20gHsciDataPathEnable
**                  (enable operation).
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  txMaxFrmSize - Maximum transmit frame size.
**                                 The frame length in octets is measured from
**                                 the first byte of the destination address
**                                 to the last byte of the FCS field.
**                  txMinFrmSize - Minimum transmit frame size.
**                                 The frame length in octets is measured from
**                                 the first byte of the destination address
**                                 to the last byte of the payload, excluding
**                                 the 4-byte FCS field.
**                  rxMaxFrmSize - Maximum receive frame size.
**                                 The frame length in octets does not include
**                                 the length of any prepended frame delineation
**                                 header or preamble/SFD bytes.
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeLanSetFrameSizes(struct file *fileHndl, UINT4 slice,
                                      UINT4 txMaxFrmSize, UINT4 txMinFrmSize,
                                      UINT4 rxMaxFrmSize)
{
    INT4 result;
    UINT4 addr;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        txMaxFrmSize > 0xFFFF ||
        txMinFrmSize > 0xFF ||
        rxMaxFrmSize > 0xFFFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Write max frame size to RXXG */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_RXXG_REG_MAX_FRM_LEN, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_RXXG_BIT_MAX_FRM_LEN_MAXFR_MSK,
                rxMaxFrmSize);
    if(result) return result;

    /* Write max and min frame size to TXXG */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TXXG_REG_TX_MAX_FRM_SZ, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_TX_MAX_FRM_SZ_TX_MAXFR_MSK,
                txMaxFrmSize);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TXXG_REG_TX_MIN_FRM_SZ, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_TX_MIN_FRM_SZ_TX_MINFR_MSK,
                txMinFrmSize);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeLanSetFrameSizes */


/*******************************************************************************
**
**  hyPhy20gHsci10GeLanSetStationAddr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function changes the MAC's station address.  Both the
**                  receive and transmit MAC blocks get assigned the same
**                  address.  And both the receive and transmit channels must
**                  be in a disabled state to make this change.
**
**                  Used for 10GE MAC Terminated mode and G.Sup43 Clause 7.3 and 
**                  Clause 6.2 Terminating application through MAC modes.
**
**                  This function can optionally be called after
**                  hyPhy20gHsciInit but prior to hyPhy20gHsciDataPathEnable
**                  (enable operation).
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  hiSA      - Station Address High
**                              Legal range 0 to 255
**                  midSA     - Station Address Mid
**                              Legal range 0 to 255
**                  lowSA     - Station Address Low
**                              Legal range 0 to 255
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeLanSetStationAddr(struct file *fileHndl, UINT4 slice,
                                       UINT4 hiSA, UINT4 midSA, UINT4 lowSA)
{
    INT4 result;
    UINT4 addr, msk;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        hiSA > 0xFFFF ||
        midSA > 0xFFFF ||
        lowSA > 0xFFFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Write Station Address to RXXG */
    msk = HYPHY20G_HSCI_ENET_10G_RXXG_BIT_STN_ADR_SA_MSK;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_RXXG_REG_STN_ADR_OFFSET(2), slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, hiSA);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_RXXG_REG_STN_ADR_OFFSET(1), slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, midSA);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_RXXG_REG_STN_ADR_OFFSET(0), slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, lowSA);
    if(result) return result;

    /* Write Station Address to TXXG */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TXXG_REG_STN_ADR_HI, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_STN_ADR_HI_SA_MSK, hiSA);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TXXG_REG_STN_ADR_MID, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_STN_ADR_MID_SA_MSK, midSA);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TXXG_REG_STN_ADR_LOW, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_STN_ADR_LOW_SA_MSK, lowSA);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeLanSetStationAddr */


/*******************************************************************************
**
**  hyPhy20gHsci10GeLanSetPromiscuous
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function turns Promiscuous mode on/off.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  promiscuous - 0 - disable promiscuous mode
**                                1 - enable promiscuous mode
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeLanSetPromiscuous(struct file *fileHndl, UINT4 slice,
                                       UINT4 promiscuous)
{

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* TBD */

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeLanSetPromiscuous */


/*******************************************************************************
**
**  hyPhy20gHsci10GeLanSetPauseCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures PAUSE control frame related 
**                  settings in TXXG and RXXG blocks.  
**                  
**                  txPauseEn and rxPauseEn enable or disable PAUSE control
**                  feature in egress and ingress respectively. 
**                  
**                  rxForwardPause and rxRespOversized control how RXXG handle
**                  received PAUSE control frames.
**
**                  txPauseTime and txPauseInterval control 'PAUSETimerField'
**                  value in the pause frames generated by TXXG, and the 
**                  frequency of pause frame generation.
**                  
**                  txPauseQuantum and txPauseQuantumEn control the pause quanta 
**                  value, which is used by TXXG to decrease local pause timer
**                  that is loaded from ingress pause frames.
**
**                  Used for 10GE MAC Terminated mode and G.Sup43 Clause 7.3 and 
**                  Clause 6.2 Terminating application through MAC modes. 
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  txPauseEn - Enable/disable the transmisstion of PAUSE control
**                              0 - disable
**                              1 - enable
**                  rxPauseEn - Enable/disable response to PAUSE request frames
**                              received from line side
**                              0 - disable
**                              1 - enable
**                  rxForwardPause
**                            - determines whether RXXG processes or forwards 
**                              PAUSE Control frames;
**                              0 - process PAUSE control frames
**                              1 - pass PAUSE control frames to
**                                  downstream (CPB)
**                  rxRespOversized
**                            - determines whether RXXG process oversized PAUSE
**                              Control frames;
**                              0 - ONLY process 64 byte pause frames
**                              1 - also process oversized PAUSE Control frames
**                                  (>=64 byte)
**                  hostPause - allow host CPU to force TXXG to send PAUSE 
**                              control frames;
**                              0 - discontinue transmitting host-requested
**                                  PAUSE control frames
**                              1 - transmit PAUSE control frames with 
**                                  pause time in frame payload
**                  txPauseTime 
**                            - 16-bit 'PAUSETimerField' of PAUSE frames
**                              generated by TXXG in the HSCI slice
**                  txPauseInterval
**                            - 16-bit Tx PAUSE timer interval, a value of
**                              0 results in PAUSE frames being generated
**                              most frequently and 0xFFFF least frequently
**                  txPauseQuantum
**                            - determines the number of pause "cycle" that  
**                              TXXG should pause:
**                              0x00 - 1   cycles
**                              0x01 - 2   cycles
**                              ......
**                              0xFF - 256 cycles
**                  txPauseQuantumEn
**                            - determines the length of 1 pause "cycle":
**                              0 - sysclk*2 cycles
**                              1 - 33 sysclk*2 cycles 
**                  
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeLanSetPauseCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 txPauseEn, UINT4 rxPauseEn,
                                    UINT4 rxForwardPause, UINT4 rxRespOversized,
                                    UINT4 hostPause, UINT4 txPauseTime,
                                    UINT4 txPauseInterval, UINT4 txPauseQuantum,
                                    UINT4 txPauseQuantumEn)
{
    INT4 result;
    UINT4 addr;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        txPauseEn > 1 || 
        rxPauseEn > 1 ||
        rxForwardPause > 1  || 
        rxRespOversized > 1 ||
        hostPause > 1       ||
        txPauseQuantumEn > 1 ||
        txPauseQuantum > 0xFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /*
     * Configure RXXG block PAUSE control
     */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_RXXG_REG_CFG_1, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_ROCF_OFF,
                rxRespOversized);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PARF_OFF,
                rxForwardPause);
    if(result) return result;

    /*
     * Configure TXXG block PAUSE control
     */
    /* Write pause time */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TXXG_REG_PAUSE_TMR, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_PAUSE_TMR_PAUSE_TIME_MSK,
                txPauseTime);
    if(result) return result;

    /* Write pause interval */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TXXG_REG_PAUSE_TMR_INTRVL, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_PAUSE_TMR_INTRVL_PAUSE_IVAL_MSK,
                txPauseInterval);
    if(result) return result;

    /* PAUSE Quantum Config */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TXXG_REG_PAUSE_QNTM_VAL_CFG, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_PAUSE_QNTM_VAL_CFG_FC_PAUSE_QNTM_MSK,
                txPauseQuantum);
    if(result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_PAUSE_QNTM_VAL_CFG_FC_PHY_PACE_EN_OFF,
                txPauseQuantumEn);
    if(result) return result;

    /*
     * Write PAUSE enable bits
     */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TXXG_REG_CFG_1, slice);
    /* Enable/disable responding to Rx'ed PAUSE control frames */
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_FCRX_OFF,
                rxPauseEn);
    if(result) return result;

    /* Enable/disable transmit of PAUSE control frames */
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_FCTX_OFF,
                txPauseEn);
    if(result) return result;

    /* Put MAC into/out of generate-PAUSE-frames mode */
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_HOSTPAUSE_OFF, hostPause);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeLanSetPauseCfg */


/*******************************************************************************
**
**  hyPhy20gHsci10GeLanSetHostPause
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function controls the TXXG Host Pause bit.
**                  While it is possible manipulate that bit with the
**                  hyPhy20gHsci10GeLanSetPauseCfg API, this routine allows
**                  the application to control the Host Pause as efficiently
**                  as possible without disabling the TX channel.
**
**                  Used for 10GE MAC Terminated mode and G.Sup43 Clause 7.3 and 
**                  Clause 6.2 Terminating application through MAC modes. 
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  hostPause - 0 - discontinue transmitting host-requested
**                                  PAUSE control frames
**                              1 - MAC proceeds to transmit PAUSE control
**                                  frames with pause time in frame payload
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeLanSetHostPause(struct file *fileHndl, UINT4 slice,
                                     UINT4 hostPause)
{
    INT4 result;
    UINT4 addr;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        hostPause > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Put MAC into/out of generate-PAUSE-frames mode */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TXXG_REG_CFG_1, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_HOSTPAUSE_OFF, hostPause);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeLanSetHostPause */

/*******************************************************************************
**
**  hyPhy20gHsci10GeSendLf
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function controls the SEND_LF bit. Bit 1 should be set
**                  to send the Local Fault (LF) ordered set when CSF condition 
**                  is asserted, and Bit 0 should be set to send normal transmit 
**                  data when CSF condition is deasserted.
**                  If a user enables FW to handle the Client Management Frame, 
**                  then the user should not manually control SEND_LF & SEND_RF 
**                  because this would interfere with the FW. But if the user 
**                  does not enable FW to handle the Client Management Frame, 
**                  then the user can control the SEND_LF register bits by  
**                  the hyPhy20gHsci10GeSendLf function.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  fault     - 0 - normal transmit data
**                              1 - Local Fault (LF) ordered set
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeSendLf(struct file *fileHndl, UINT4 slice, UINT4 fault)
{
    INT4 result;
    
    /* argument checking */
    if (slice > (HYPHY20G_HSCI_NUM_SLICE - 1) ||
        fault > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (fault)
    {
        /*SEND_LF_EN = 1 */
        result = sysHyPhy20gBitWrite(fileHndl,
            HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_TX_CFG_2,slice), 
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_SEND_LF_EN_OFF, 1);
        if (result) return result;
    }

    /* set SEND_LF according to fault value */
    result = sysHyPhy20gBitWrite(fileHndl,
        HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TOP_REG_CFG,slice), 
        HYPHY20G_HSCI_ENET_10G_TOP_BIT_CFG_SEND_LF_OFF, fault);
    if (result) return result;

    return HYPHY20G_SUCCESS;
}/*hyPhy20gHsci10GeSendLf*/

/*******************************************************************************
**
**  hyPhy20gHsci10GeSendRf
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function controls the SEND_RF bit. Bit 1 should be set
**                  to send the Remote Fault (RF) ordered set when CSF condition 
**                  is asserted, and Bit 0 should be set to send normal transmit 
**                  data when CSF condition is deasserted.
**                  If a user enables FW to handle the Client Management Frame, 
**                  then the user should not manually control SEND_LF & SEND_RF 
**                  because this would interfere with the FW. But if the user 
**                  does not enable FW to handle the Client Management Frame, 
**                  then the user can control the SEND_RF register bits by  
**                  the hyPhy20gHsci10GeSendRf function.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  fault     - 0 - normal transmit data
**                              1 - Remote Fault (RF) ordered set
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeSendRf(struct file *fileHndl, UINT4 slice, UINT4 fault)
{
    INT4 result;
    /* argument checking */
    if (slice > (HYPHY20G_HSCI_NUM_SLICE - 1) ||
        fault > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (fault)
    {
        /*SEND_RF_EN = 1 */
        result = sysHyPhy20gBitWrite(fileHndl,
            HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_TX_CFG_2,slice), 
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_SEND_RF_EN_OFF, 1);
        if (result) return result;
    }

    /* set SEND_RF according to fault value */
    result = sysHyPhy20gBitWrite(fileHndl,
        HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TOP_REG_CFG,slice), 
        HYPHY20G_HSCI_ENET_10G_TOP_BIT_CFG_SEND_RF_OFF, fault);
    if (result) return result;

    return HYPHY20G_SUCCESS;
}/* hyPhy20gHsci10GeSendRf */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/*******************************************************************************
**
**  hsciHyPhy20gHsciCreateCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Allocates and intializes memory required to operate
**                  the HSCI subsystem.
**
**                  References this memory to the device level structure
**                  pointed to with *pDevCtxt.
**
**                  Function hsciHyPhy20gHsciDestroyCtxt free this memory and
**                  set the HSCI subsystem reference to NULL.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice number
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  mode      - traffic type of the specific slice
**                              1 - 10GE MAC Terminated (including G.Sup43
**                                  Clause 7.1 and 7.2 10GE MAC terminated)
**                              2 - G.Sup43 Clause 7.3
**                              3 - FC 8G and subrate
**                              4 - High Speed CBR 16/32 bit mode (including
**                                  G.Sup43 Clause 7.1 and 7.2 10GE transparent,
**                                  STS-192 asynchronous transparent)
**                              5 - High Speed CBR 20/40 bit mode
**                              6 - G.Sup43 Clause 7.3 Terminating application
**                                  through MAC
**                              7 - G.Sup43 Clause 6.2 Terminating application
**                                  through MAC
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hsciHyPhy20gHsciCreateCtxt(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    sHYPHY20G_HSCI_CTXT  *pHsciCtxt;
    sHYPHY20G_CTXT *pDevCtxt;
    UINT4 size;

    /* argument checking */
    if ((slice > HYPHY20G_HSCI_NUM_SLICE - 1) ||
        (mode > HSCI_TRAFFIC_MAX) || mode < 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    /* If pHsciCtxt[slice] is not a NULL pointer,
       destroy it first to prevent memory leak */
    if (pDevCtxt->pHsciCtxt[slice] != NULL) {
        hsciHyPhy20gHsciDestroyCtxt(fileHndl, slice);
    }

    /* allocate memory for and associate with pDevCtxt */
    size = sizeof(sHYPHY20G_HSCI_CTXT);
    pDevCtxt->pHsciCtxt[slice] = (sHYPHY20G_HSCI_CTXT *) sysHyPhy20gMemAlloc(size);

    if(pDevCtxt->pHsciCtxt[slice] == NULL) {
        return HYPHY20G_ERR_MEM_ALLOC;
    }

    //DBG_PRINT("HSCI slice #%ld Context memory allocated:addr=0x%x size=%ld\n",
    //          slice, pDevCtxt->pHsciCtxt[slice], size);

    pHsciCtxt = pDevCtxt->pHsciCtxt[slice];
    /* initialize variables */
    pHsciCtxt->valid            = HYPHY20G_VALID_SIGN;
    /* store the the operating mode to HSCI context */
    pHsciCtxt->traffic          = mode;

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsciCreateCtxt */

/*******************************************************************************
**
**  hsciHyPhy20gHsciDestroyCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Reset context of the HSCI subsystem.
**
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice number
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hsciHyPhy20gHsciDestroyCtxt(struct file *fileHndl, UINT4 slice)
{
    sHYPHY20G_HSCI_CTXT  *pHsciCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    pHsciCtxt = pDevCtxt->pHsciCtxt[slice];

    if(pHsciCtxt == NULL){
    	return HYPHY20G_SUCCESS;
    }

    sysHyPhy20gMemFree(pDevCtxt->pHsciCtxt[slice]);

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsciDestroyCtxt */


/*******************************************************************************
**
**  hsciHyPhy20gHsciCtxtGetMode
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function retrieves the configuration mode from 
**                  context of the HSCI slice
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
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
INT4 hsciHyPhy20gHsciCtxtGetMode(struct file *fileHndl, UINT4 slice,
                                 UINT4 *pMode)
{
    sHYPHY20G_HSCI_CTXT  *pHsciCtxt;
    sHYPHY20G_CTXT *pDevCtxt;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    HYPHY20G_ASSERT((pMode != NULL) , HYPHY20G_ERR_NULL_PTR_PASSED,
                    "pMode is NULL.");
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL) && (pDevCtxt->pHsciCtxt[slice] != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "pDevCtxt or (pDevCtxt->pHsciCtxt + %ld) is NULL.", slice);

    pHsciCtxt = pDevCtxt->pHsciCtxt[slice];
    *pMode = pHsciCtxt->traffic;

    HYPHY20G_ASSERT((*pMode <= HSCI_TRAFFIC_MAX), HYPHY20G_ERR_INVALID_MODE,
                    "*pMode is %ld.", *pMode);

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsciCtxtGetMode */

/*******************************************************************************
**
**  hsciHyPhy20gHsci10GeBlockInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Initializes the 10GE block at the block level.
**                  If the 'enable' is 1, configure 10GE block at Top level.
**
**                  This function should be only used for
**                  10GE MAC Terminated, G.Sup43 Clause 7.3 Terminating
**                  application bypass MAC, G.Sup43 Clause 7.3 and 6.2
**                  Terminating application through MAC modes.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hsciHyPhy20gHsci10GeBlockInit(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 mode;
    UINT4 macLp, cfgMode, eclassRst, mstatxRst, txxgRst, rxxgRst;
    UINT4 gsup43Rst;
    UINT4 gsup43Lp;
    UINT4 c62Mode;
    UINT4 ptpntpRst;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &mode);
    if(result) return result;

    /*
     * initializes mode-specific variables
     */
    switch(mode)
    {
        case HSCI_10GE_MAC_TEMINATION:
            macLp       = 0;
            gsup43Lp    = 0;
            cfgMode     = 0x0;
            ptpntpRst   = 0;
            eclassRst   = 0;
            mstatxRst   = 0;
            gsup43Rst   = 0;
            txxgRst     = 0;
            rxxgRst     = 0;
            c62Mode     = 0;
            break;
        case HSCI_GSUP43_CLAUSE73:
            macLp       = 1;
            gsup43Lp    = 0;
            cfgMode     = 0x1;
            ptpntpRst   = 1;
            eclassRst   = 1;
            gsup43Rst   = 0;
            mstatxRst   = 1;
            txxgRst     = 1;
            rxxgRst     = 1;
            c62Mode     = 0;
            break;
        case HSCI_MAC_GSUP43_CLAUSE73:
            macLp       = 0;
            gsup43Lp    = 0;
            cfgMode     = 0x0;
            ptpntpRst   = 0;
            eclassRst   = 0;
            mstatxRst   = 0;
            gsup43Rst   = 0;
            txxgRst     = 0;
            rxxgRst     = 0;
            c62Mode     = 0;
            break;
        case HSCI_MAC_GSUP43_CLAUSE62:
            macLp       = 0;
            gsup43Lp    = 0;
            cfgMode     = 0x0;
            ptpntpRst   = 0;
            eclassRst   = 0;
            gsup43Rst   = 0;
            mstatxRst   = 0;
            txxgRst     = 0;
            rxxgRst     = 0;
            c62Mode     = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /*
     * Configure ENET_10G top level
     */
    /* configuration for mode, enable and other top level settings */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TOP_REG_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;
	
    result = hyPhy20gVarFieldWrite(&val,
                    HYPHY20G_HSCI_ENET_10G_TOP_BIT_CFG_GSUP43_C62_MODE_MSK,
                    c62Mode);
    if(result) return result;

    if (mode != HSCI_GSUP43_CLAUSE73) {
        result = hyPhy20gVarFieldWrite(&val,
                    HYPHY20G_HSCI_ENET_10G_TOP_BIT_CFG_IPG_RED_ON_THRESH_MSK,
                    0x2);
        if(result) return result;
        result = hyPhy20gVarFieldWrite(&val,
                    HYPHY20G_HSCI_ENET_10G_TOP_BIT_CFG_IPG_RED_OFF_THRESH_MSK,
                    0x1);
        if(result) return result;
        result = hyPhy20gVarFieldWrite(&val,
                    HYPHY20G_HSCI_ENET_10G_TOP_BIT_CFG_PAUSE_ON_THRESH_MSK,
                    0x2);
        if(result) return result;
        result = hyPhy20gVarFieldWrite(&val,
                    HYPHY20G_HSCI_ENET_10G_TOP_BIT_CFG_PAUSE_OFF_THRESH_MSK,
                    0x1);
        if(result) return result;
    }

    result = hyPhy20gVarFieldWrite(&val,
                    HYPHY20G_HSCI_ENET_10G_TOP_BIT_CFG_MAC_LOWPWR_MSK,
                    macLp);
    if(result) return result;

    /* remove low-power because the HSCI GSUP43 block also acts as 10GE PCS. */
    result = hyPhy20gVarFieldWrite(&val,
                    HYPHY20G_HSCI_ENET_10G_TOP_BIT_CFG_GSUP43_LOWPWR_MSK,
                    gsup43Lp);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&val,
                    HYPHY20G_HSCI_ENET_10G_TOP_BIT_CFG_ENABLE_MSK,
                    HYPHY20G_ENABLE);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&val,
                    HYPHY20G_HSCI_ENET_10G_TOP_BIT_CFG_MODE_MSK,
                    cfgMode);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* bring selected sub-blocks out of reset */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TOP_REG_SW_RESETS, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, ptpntpRst,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_PTP_NTP_RST_MSK,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_PTP_NTP_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, eclassRst,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_TX_ECLASS_RST_MSK,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_TX_ECLASS_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, eclassRst,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_RX_ECLASS_RST_MSK,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_RX_ECLASS_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, mstatxRst,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_MSTATX_RST_MSK,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_MSTATX_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, txxgRst,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_TXXG2_RST_MSK,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_TXXG2_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, gsup43Rst,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_GSUP43_RST_MSK,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_GSUP43_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, rxxgRst,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_RXXG2_RST_MSK,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_RXXG2_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_ENET_10G_RST_MSK,
            HYPHY20G_HSCI_ENET_10G_TOP_BIT_SW_RESETS_ENET_10G_RST_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsci10GeBlockInit */


/*******************************************************************************
**
**  hsciHyPhy20gHsci10GeDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure data path in 10GE block for 10GE MAC Terminated
**                  traffic.
**
**                  This function must be called after
**                  hsciHyPhy20gHsci10GeBlockInit function has done.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hsciHyPhy20gHsci10GeDataPathCfg(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 mode;
    UINT4 addr, val;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &mode);
    if(result) return result;
    /* confirm mode before processing */
    if (mode != HSCI_10GE_MAC_TEMINATION) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /*
     * configure RXXG block
     */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_RXXG_REG_CFG_1, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_MIFG_EN_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_MIFG_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_ERRORS_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_ERRORS_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_CTRL_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_CTRL_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_RX_CONTIG_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_RX_CONTIG_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_RXXG_REG_CUT_THRU_TH_SEL, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CUT_THRU_TH_SEL_CUT_THRU_THRES_SEL_MSK,
                0x1);
    if(result) return result;

    /*
     * configure TXXG block
     */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TXXG_REG_CFG_1, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_IPG_REDUC_EN_OFF, 1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_ALIGN_32BIT_OFF, 1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_FCTX_OFF, 1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_FCRX_OFF, 1);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsci10GeDataPathCfg */


/*******************************************************************************
**
**  hsciHyPhy20gHsciGsup43DataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes data path in HSCI GSUP43 block
**                  for 10GE MAC Terminated, G.Sup43 Clause 7.3 Terminating
**                  application bypass MAC modes.
**
**                  This function must be called after
**                  hsciHyPhy20gHsci10GeBlockInit function has done.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hsciHyPhy20gHsciGsup43DataPathCfg(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 mode;
    UINT4 addr;
    UINT4 gsup43En, allDscrd, minIPG;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &mode);
    if(result) return result;

    switch(mode)
    {
        case HSCI_10GE_MAC_TEMINATION:
            gsup43En    = 0;    /* Disable G.Sup43 Clause 7.3 mode and work
                                   as a standard 64B/66B PCS decoder.*/
            allDscrd    = 1;
            minIPG      = 1;
            break;
        case HSCI_GSUP43_CLAUSE73:
            gsup43En    = 1;    /* Enable G.Sup43 Clause 7.3 mode */
            allDscrd    = 0;
            minIPG      = 0;
            break;
        default:
            return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /*
     * configure HSCI GSUP43_C73 block top level
     */
    /* Remove HSCI GSUP43_C73 block software reset */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_CFG_SW_RST_OFF, 0);
    if(result) return result;

    /*
     * configure HSCI GSUP43_C73 Rx block
     */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_RX_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_CFG_RX_SW_RST_OFF,
                1);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_RX_GSUP43_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_GSUP43_C73_EN_OFF,
                gsup43En);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_OS_ALL_DSCRD_OFF,
                allDscrd);
    if(result) return result;

    if (mode == HSCI_GSUP43_CLAUSE73) {
        result = hyPhy20gRegFieldWrite(fileHndl, addr,
                    HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_MIN_OS_RPT_MSK,
                    0x3);
        if(result) return result;
    }

    /*
     * configure HSCI GSUP43_C73 Tx block
     */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_TX_CFG_1, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_1_TX_SW_RST_OFF,
                1);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_TX_CFG_2, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_TX_GSUP43_C73_EN_OFF,
                gsup43En);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_FORCE_MIN_IPG_OFF,
                minIPG);
    if(result) return result;

    if (mode == HSCI_GSUP43_CLAUSE73) {
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_UNIDIR_EN_OFF,
                    0);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsciGsup43DataPathCfg */


/*******************************************************************************
**
**  hsciHyPhy20gHsciFcDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure data path in FC_CBR block for FC 8G traffic mode.
**
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
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
INT4 hsciHyPhy20gHsciFcDataPathCfg(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 addr, val, mode;
    UINT4 cfgMode, olSeqHi, olSeqLow, pattDisp;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &mode);
    if(result) return result;
    /* confirm mode before processing */
    if (mode != HSCI_FC_8G) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* 0x00: for FC traffic */
    cfgMode     = 0x00;
    /* FC OLS sequence (K28.5 D21.1 D10.4 D21.2) */
    olSeqHi     = 0x3E;
    olSeqLow    = 0xAA954AA5;
    pattDisp    = 0;    /* not used for FC mode, set to 0 */

    /*
     * configure FC_CBR Top Level
     */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_CFG, slice);

    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_FC_CBR_BIT_CFG_SOFT_RST_OFF, 1);
    if(result) return result;

    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_HSCI_FC_CBR_BIT_CFG_MODE_MSK, cfgMode);
    if(result) return result;

    /*
     * configure FC_CBR Rx Path
     */
    result = hsciHyPhy20gHsciFcCbrUsrPattCfg(fileHndl, slice, HYPHY20G_RX_DIRECTION,
                olSeqHi, olSeqLow, 0);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_RX_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_LCV_ERR_INS_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_LCV_ERR_INS_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_PN11_INV_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_PN11_INV_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0, (0x0001 << 3), 3);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_RX_DLO_EN, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_ASD_DLOLB_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_ASD_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_RX_LOSS_SYNC_DLOLB_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_RX_LOSS_SYNC_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_RX_DRL_DLOLB_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_RX_DRL_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x7,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_RX_DLOLB_INT_PER_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_RX_DLOLB_INT_PER_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /*
     * configure FC_CBR Tx Path
     */
    result = hsciHyPhy20gHsciFcCbrUsrPattCfg(fileHndl, slice, HYPHY20G_TX_DIRECTION,
                olSeqHi, olSeqLow, pattDisp);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_TX_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_FC_CONT_ALIGN_MODE_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_FC_CONT_ALIGN_MODE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_LCV_ERR_INS_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_LCV_ERR_INS_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_PN11_INV_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_PN11_INV_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0, (0x0001 << 3), 3);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_TX_DLO_EN, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_CLK_ADAPT_FIFO_UNDR_DLOLB_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_CLK_ADAPT_FIFO_UNDR_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_LCV_DLOLB_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_LCV_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_LOSS_SYNC_DLOLB_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_LOSS_SYNC_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_DRL_DLOLB_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_DRL_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x7,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_DLOLB_INT_PER_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_DLOLB_INT_PER_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* set insert and delete interval */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_INS_DEL_INTRVL, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 0x1000,
            HYPHY20G_HSCI_FC_CBR_BIT_INS_DEL_INTRVL_INS_INTERVAL_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_INS_DEL_INTRVL_INS_INTERVAL_OFF);

    val = mHYPHY20G_SET_FIELD4(val, 0x1000,
            HYPHY20G_HSCI_FC_CBR_BIT_INS_DEL_INTRVL_DEL_INTERVAL_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_INS_DEL_INTRVL_DEL_INTERVAL_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_OS_CTL, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* treat unrecognized ordered sets as fill-words */
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_OS_CTL_UNKWN_OS_FW_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_OS_CTL_UNKWN_OS_FW_EN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsciFcDataPathCfg */


/*******************************************************************************
**
**  hsciHyPhy20gHsciCbrDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure data path in FC_CBR block for High-Speed CBR
**                  traffic mode, inculding High Speed CBR 16/32 bit mode
**                  (sup43 Clause 7.1 and 7.2 10GE transparent, STS-192
**                  asynchronous transparent) and High Speed CBR 20/40 bit mode.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hsciHyPhy20gHsciCbrDataPathCfg(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 mode;
    UINT4 addr, val, mdCfgVal;
    UINT4 cfgMode;
    const UINT4 DLOLB_INT_PER = 0x7;
    UINT4 TX_NOS_PN11_SEL=1;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &mode);
    if(result) return result;
    /* confirm mode before processing */
    if (mode != HSCI_HS_CBR_32BIT_MODE && 
        mode != HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_EN &&
        mode != HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_DIS) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    cfgMode = 0x2;
    switch(mode)
    {
        case HSCI_HS_CBR_32BIT_MODE:
            mdCfgVal         = 0;
            TX_NOS_PN11_SEL = 1;
            break;
        case HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_EN:
        case HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_DIS:
            mdCfgVal         = 1;
            TX_NOS_PN11_SEL = 0;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /*
     * configure FC_CBR Top Level
     */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_FC_CBR_BIT_CFG_SOFT_RST_OFF, 1);
    if(result) return result;

    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_HSCI_FC_CBR_BIT_CFG_MODE_MSK, cfgMode);
    if(result) return result;

    /*
     * configure FC_CBR Rx Slice
     */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_RX_CFG, slice);;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_FC_CONT_ALIGN_MODE_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_FC_CONT_ALIGN_MODE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, mdCfgVal,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_LCV_ERR_INS_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_LCV_ERR_INS_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_PN11_INV_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_PN11_INV_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_FORCE_NOS_PN11_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_FORCE_NOS_PN11_OFF);
    val = mHYPHY20G_SET_FIELD4(val, mdCfgVal, (0x0001 << 3), 3);
    val = mHYPHY20G_SET_FIELD4(val, mdCfgVal,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_ENABLE_BYTE_ALIGN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_ENABLE_BYTE_ALIGN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_FORCE_REALIGN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_FORCE_REALIGN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_RX_DLO_EN, slice);;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_ASD_DLOLB_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_ASD_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, mdCfgVal,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_RX_LOSS_SYNC_DLOLB_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_RX_LOSS_SYNC_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, mdCfgVal,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_RX_DRL_DLOLB_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_RX_DRL_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, DLOLB_INT_PER,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_RX_DLOLB_INT_PER_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_RX_DLOLB_INT_PER_OFF);
    val = mHYPHY20G_SET_FIELD4(val, TX_NOS_PN11_SEL,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_NOS_PN11_SEL_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_NOS_PN11_SEL_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /*
     * configure FC_CBR Tx Slice
     */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_TX_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_FC_CONT_ALIGN_MODE_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_FC_CONT_ALIGN_MODE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_FC_RD_INV_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_FC_RD_INV_EN_OFF);
    if(mode == HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_DIS)
    {
        val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_RD_INV_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_RD_INV_EN_OFF);
    }
    else
    {
        val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_RD_INV_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_RD_INV_EN_OFF);
    }
    val = mHYPHY20G_SET_FIELD4(val, mdCfgVal,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_LCV_ERR_INS_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_LCV_ERR_INS_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_PN11_INV_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_PN11_INV_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1, (0x0001 << 3), 3);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_FC_CBR_REG_TX_DLO_EN, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_CLK_ADAPT_FIFO_UNDR_DLOLB_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_CLK_ADAPT_FIFO_UNDR_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, mdCfgVal,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_LOSS_SYNC_DLOLB_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_LOSS_SYNC_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, mdCfgVal,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_DRL_DLOLB_EN_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_DRL_DLOLB_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, DLOLB_INT_PER,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_DLOLB_INT_PER_MSK,
            HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_DLOLB_INT_PER_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsciCbrDataPathCfg */


/*******************************************************************************
**
**  hsciHyPhy20gHsciMacGsup43DataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures data path in 10GE block for
**                  G.Sup43 Clause 7.3 and 6.2 Terminating application
**                  through MAC.
**
**                  This function configures related register bits
**                  at ENET_10G top level, ENET_10G GSUP43, RXXG and TXXG 
**                  blocks.
**
**                  In the G.Sup43 Clause 7.3 Terminating mode through MAC
**                  application, this function configures RXXG and TXXG blocks
**                  for Ethernet Payload (DA, SA, Length/Type, DATA, FCS)
**                  by default.
**                  In order to transfer Extended Ethernet Payload (with
**                  Preamble), user can call hyPhy20gHsciMacGsup43C73ModeCfg
**                  API to change the default settings.
**
**                  In the G.Sup43 Clause 6.2 Terminating mode through MAC
**                  application, this function configures RXXG and TXXG blocks
**                  for Ethernet Payload (DA, SA, Length/Type, DATA, FCS),
**                  which is the only payload type supported in this mode.
**
**                  This function must be called after
**                  hsciHyPhy20gHsci10GeBlockInit function has done.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hsciHyPhy20gHsciMacGsup43DataPathCfg(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 mode;
    UINT4 addr;
    UINT4 msk;
    UINT4 regData;
    UINT4 txHdrSize;
    UINT4 spreVal;
    UINT4 txFcsAppend;
    UINT4 rxPrmbProc;
    UINT4 txPrmb;
    const UINT4 RX_OS_ALL_DSCRD = 1;
    const UINT4 PAUSE_TAG = 0x8;
    const UINT4 PAUSE_PRMB = 1; /* Same value as STD_PAUSE in TXXG */

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &mode);
    if(result) return result;
    /* confirm mode before processing */
    if (mode != HSCI_MAC_GSUP43_CLAUSE73 &&
        mode != HSCI_MAC_GSUP43_CLAUSE62) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    switch(mode)
    {
        case HSCI_MAC_GSUP43_CLAUSE73:
            txHdrSize   = 4;
            spreVal     = 0;    /* prepend preamble in egress */    
            txFcsAppend = 0;            
            rxPrmbProc  = 2;            
            break;
        case HSCI_MAC_GSUP43_CLAUSE62:
            txHdrSize   = 4;
            spreVal     = 1;    /* don't prepend preamble in egress */
            txFcsAppend = 0;
            rxPrmbProc  = 4;            
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* inversion of SPRE bit in TXXG */
    txPrmb = (spreVal == 0) ? 1 : 0; 

    /*
     * configure MAC G.Sup43 related register bits at ENET_10G top level
     */
    addr = HYPHY20G_HSCI_ENET_10G_TOP_REG_MAC_GSUP43_CFG;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TOP_BIT_MAC_GSUP43_CFG_PAUSE_TAG_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, PAUSE_TAG);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TOP_BIT_MAC_GSUP43_CFG_PAUSE_PRMB_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, PAUSE_PRMB);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TOP_BIT_MAC_GSUP43_CFG_TXXG_FCS_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, txFcsAppend);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TOP_BIT_MAC_GSUP43_CFG_TXXG_PRMB_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, txPrmb);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TOP_BIT_MAC_GSUP43_CFG_MAC_GSUP43_MODE_EN_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, 0);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;


    /*
     * configure HSCI GSUP43_C73 block
     */
    /* Remove HSCI GSUP43_C73 block software reset */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_CFG_SW_RST_OFF, 0);
    if(result) return result;

    /* configure HSCI GSUP43_C73 Rx block */
    if (mode == HSCI_MAC_GSUP43_CLAUSE73) {           
        addr = HYPHY20G_HSCI_ENET_10G_GSUP43_REG_RX_CFG;
        addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_CFG_RX_SW_RST_OFF,
                    1);
        if(result) return result;

        addr = HYPHY20G_HSCI_ENET_10G_GSUP43_REG_RX_GSUP43_CFG;
        addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_OS_ALL_DSCRD_OFF,
                    RX_OS_ALL_DSCRD);
        if(result) return result;
    }
    
    /* configure HSCI GSUP43_C73 Tx block */
    addr = HYPHY20G_HSCI_ENET_10G_GSUP43_REG_TX_CFG_1;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_1_TX_SW_RST_OFF,
                1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_1_TX_ERR_DROP_OFF,
                0);
    if(result) return result;

    addr = HYPHY20G_HSCI_ENET_10G_GSUP43_REG_TX_CFG_2;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_TX_GSUP43_C73_EN_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, 0);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_FORCE_MIN_IPG_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, 0);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_UNIDIR_EN_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, 1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    /*
     * configure RXXG block
     */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_RXXG_REG_CFG_1, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    regData = mHYPHY20G_SET_FIELD4(regData, 1,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_MIFG_EN_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_MIFG_EN_OFF);
    regData = mHYPHY20G_SET_FIELD4(regData, 0,  
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_FLCHK_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_FLCHK_OFF);
    regData = mHYPHY20G_SET_FIELD4(regData, 1,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_CTRL_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PASS_CTRL_OFF);
    regData = mHYPHY20G_SET_FIELD4(regData, 0,  
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_RX_CONTIG_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_RX_CONTIG_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    addr = HYPHY20G_HSCI_ENET_10G_RXXG_REG_CUT_THRU_TH_SEL;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk  = HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CUT_THRU_TH_SEL_CUT_THRU_THRES_SEL_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x1);
    if(result) return result;

    result = hsciHyPhy20gHsciRxxgPrmbCfg(fileHndl, slice, rxPrmbProc, 0);
    if(result) return result;

    /*
     * configure TXXG block
     */
    addr = HYPHY20G_HSCI_ENET_10G_TXXG_REG_CFG_1;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_IPG_REDUC_EN_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, 1);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_ALIGN_32BIT_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, 1);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_CRCEN_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, txFcsAppend);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_FCTX_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, 1);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_FCRX_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, 1);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_PADEN_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, txFcsAppend);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_1_SPRE_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, spreVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    addr = HYPHY20G_HSCI_ENET_10G_TXXG_REG_CFG_2;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    /* the Length/Type field in packets from CPB channel must be updated */
    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_2_LENGTH_IN_HDR_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, 1);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_TXXG_BIT_CFG_2_HDRSIZE_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, txHdrSize);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsciMacGsup43DataPathCfg */



/*******************************************************************************
**
**  hsciHyPhy20gHsciFcCbrUsrPattCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configures and updates user pattern and running disparity
**                  for FC_CBR block.
**
**
**  INPUTS:         fileHndl   - device handle
**                  slice      - HSCI slice processing resource
**                               slice number 0 - 1
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
INT4 hsciHyPhy20gHsciFcCbrUsrPattCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 dir, UINT4 olSeqHi, UINT4 olSeqLow,
                                     UINT4 pattDisp)
{
    INT4 result;
    UINT4 traffic;
    UINT4 addr, bitoffset, addr1, addr2, hiMsk, lowMsk, pattMsk;
    UINT4 upd_msk, upd_off;
    UINT4 softRstVal;

    /* argument checking */
    if (slice > 1 || dir > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic < HSCI_FC_8G || 
        ((traffic > HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_EN) && 
         (traffic != HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_DIS))) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    pattMsk = 0;
    switch (dir) {
        case HYPHY20G_TX_DIRECTION:
            addr1   = HYPHY20G_HSCI_FC_CBR_REG_TX_USR_PAT_LSB;
            addr2   = HYPHY20G_HSCI_FC_CBR_REG_TX_CFG;
            lowMsk  = HYPHY20G_HSCI_FC_CBR_BIT_TX_USR_PAT_LSB_TX_USR_PATT_MSK;
            hiMsk   = HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_USR_PATT_MSK;
            pattMsk = HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_USR_PATT_DISP_MSK;
            upd_msk = HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_USR_PATT_UPDATE_MSK;
            upd_off = HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_USR_PATT_UPDATE_OFF;
            break;
        case HYPHY20G_RX_DIRECTION:
            addr1   = HYPHY20G_HSCI_FC_CBR_REG_RX_USR_PAT_LSB;
            addr2   = HYPHY20G_HSCI_FC_CBR_REG_RX_CFG;
            lowMsk  = HYPHY20G_HSCI_FC_CBR_BIT_RX_USR_PAT_LSB_RX_USR_PATT_MSK;
            hiMsk   = HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_USR_PATT_MSK;
            upd_msk = HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_USR_PATT_UPDATE_MSK;
            upd_off = HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_USR_PATT_UPDATE_OFF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr1, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, lowMsk, olSeqLow);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr2, slice);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, hiMsk, olSeqHi);
    if(result) return result;

    if (((traffic == HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_EN) ||
        (traffic == HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_DIS))&& dir == HYPHY20G_TX_DIRECTION) {
        result = hyPhy20gRegFieldWrite(fileHndl, addr, pattMsk, pattDisp);
        if(result) return result;
    }

    /* update pattern via USR_PATT_UPDATE bit */
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr2, slice);
    bitoffset = upd_off;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitoffset, 1);
    if (result) return result;

    /* check the SOFT_RST bit: if set to 0, poll, otherwise don't poll */
    addr = HYPHY20G_HSCI_FC_CBR_REG_CFG;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = hyPhy20gRegFieldRead(fileHndl, addr,
                                  HYPHY20G_HSCI_FC_CBR_BIT_CFG_SOFT_RST_MSK,
                                  &softRstVal);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr2, slice);
    if (softRstVal == 0) {
        /* poll the update bit which self clears to 0
           when the new pattern has been loaded */
        result = sysHyPhy20gPollBit(fileHndl, addr, upd_msk, 0,
                                    HYPHY20G_DELAY_20USEC);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsciFcCbrUsrPattCfg */

/*******************************************************************************
**
**  hsciHyPhy20gHsciGsup43RxCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure RX GSUP43 block in HSCI subsystem.
**
**                  Used for G.Sup43 Clause 7.3 Terminating application
**                  bypass MAC operating mode (mode '2' in hyPhy20gHsciInit).
**
**                  This function can optionally be called after
**                  hyPhy20gHsciInit but prior to hyPhy20gHsciDataPathEnable.
**
**  INPUTS:         fileHndl   - device handle
**                  slice      - HSCI slice processing resource
**                               0 - HSCI slice 0
**                               1 - HSCI slice 1
**                  losInv      - configures the assertive state of the
**                               LOS input signal
**                               0 - LOS is expected to be asserted when the
**                                   corresponding input signal is logic 1
**                               1 - LOS is expected to be asserted when the
**                                   corresponding input signal is logic 0
**                               Default setting 0
**                  dscrdZone  - sets the fill level of the downstream packet
**                               buffer at which ordered set discard mechanism
**                               is enabled.
**                               b'00 - b'11
**                               Default setting b'10
**                  sigEn      - enables the discard mechanism for signal
**                               ordered sets
**                               0 - Signal ordered sets are not considered in
**                                   the discard rate adaptation mechanism
**                               1 - Signal ordered sets are considered in the
**                                   discard rate adaptation mechanism
**                               Default setting 0
**                  seqEn      - enables the discard mechanism for
**                               sequence ordered sets
**                               0 - Sequence ordered sets are not considered
**                                   in the discard rate adaptation mechanism
**                               1 - Sequence ordered sets are considered
**                                   in the discard rate adaptation mechanism
**                               Default setting 1
**                  allDscrd   - enables the discard of all ordered sets
**                               0 - specific ordered sets in the received
**                                   data path are discarded
**                               1 - All ordered sets in the received data path
**                                   are discarded
**                               Default setting 0
**                  osPrtDis   - disables the function of RX_MIN_OS_RPT[3:0]
**                               when the downstream packet buffer is full
**                               0 - RX_MIN_OS_RPT is always regarded
**                               1 - RX_MIN_OS_RPT is ignored when the
**                                   downstream packer buffer is full
**                               Default setting 0
**                  minOsPrt   - sets the minimum number of consecutive
**                               identical ordered sets on the received
**                               data path that must be detected before they
**                               are eligible for discard
**                               0 - ordered set may be discarded, based on the
**                                   fill-level of the downstream buffer (CPB)
**                               N - the previous N-1 received ordered sets must
**                                   be identical to the current ordered set
**                                   before the current ordered set is considered
**                                   eligible for the discard.
**                               Default setting b'0011
**                  pktType    - identifies the received data as an 10GE data
**                               frame as opposed to an ordered set
**                               Default setting b'11111101
**                  osType     - identifies the received data as an 10GE ordered
**                               set as opposed to a data packet
**                               Default setting b'11111110
**
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hsciHyPhy20gHsciGsup43RxCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 losInv, UINT4 dscrdZone, UINT4 sigEn,
                                 UINT4 seqEn, UINT4 allDscrd, UINT4 osPrtDis,
                                 UINT4 minOsPrt,UINT4 pktType,UINT4 osType)
{
    INT4 result;
    UINT4 addr, val;

    /* argument checking */
    if (slice > 1 ||
        losInv > 1 ||
        dscrdZone > 0x3 ||
        sigEn > 1 ||
        seqEn > 1 ||
        allDscrd > 1 ||
        osPrtDis > 1 ||
        minOsPrt > 0xF ||
        pktType > 0xFF ||
        osType > 0xFF) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_RX_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, losInv,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_CFG_LOS_INV_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_CFG_LOS_INV_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_RX_GSUP43_CFG, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;


    val = mHYPHY20G_SET_FIELD4(val, dscrdZone,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_DSCRD_ZONE_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_DSCRD_ZONE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, sigEn,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_SIG_OS_DSCRD_EN_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_SIG_OS_DSCRD_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, seqEn,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_SEQ_OS_DSCRD_EN_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_SEQ_OS_DSCRD_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, allDscrd,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_OS_ALL_DSCRD_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_OS_ALL_DSCRD_OFF);
    val = mHYPHY20G_SET_FIELD4(val, osPrtDis,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_FULL_OS_RPT_DIS_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_FULL_OS_RPT_DIS_OFF);
    val = mHYPHY20G_SET_FIELD4(val, minOsPrt,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_MIN_OS_RPT_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_MIN_OS_RPT_OFF);
    val = mHYPHY20G_SET_FIELD4(val, pktType,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_PKT_TYPE_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_PKT_TYPE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, osType,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_OS_TYPE_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_RX_GSUP43_CFG_RX_OS_TYPE_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsciGsup43RxCfg */

/*******************************************************************************
**
**  hsciHyPhy20gHsciGsup43TxCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure Tx GSUP43 block in HSCI subsystem.
**
**                  Used for G.Sup43 Clause 7.3 Terminating application
**                  bypass MAC operating mode (mode '2' in hyPhy20gHsciInit).
**
**                  This function can optionally be called after
**                  hyPhy20gHsciInit but prior to hyPhy20gHsciDataPathEnable.
**
**  INPUTS:         fileHndl   - device handle
**                  slice      - HSCI slice processing resource
**                               0 - HSCI slice 0
**                               1 - HSCI slice 1
**                  errDrop    - when incoming packet has an UPI TYPE value
**                               that does not match either TX_PKT_TYPE or
**                               TX_OS_TYPE:
**                               0 - incoming data packet treated as a
**                                   data packet
**                               1 - incoming packet will be dropped
**                               Default setting 1
**                  pktType    - identifies the transmit data as a 10GE
**                               data frame, as opposed to an ordered set.
**                               this value should be configured to match
**                               the UPI value of the GFP-F frame
**                               Default setting b'11111101
**                  osType     - identifies the transmit data as 10GE ordered
**                               set, as opposed to a data packet.
**                               this value should be configured to match
**                               the UPI value of the GFP-F frame
**                               Default setting b'11111110
**                  forceMinIpg- forces the minimum IPG value to 0 bytes
**                               when the downstream packet buffer is full
**                               0 - average minimum IPG is set to 12 bytes
**                                   or 8 bytes
**                               1 - IPG is set to 0 bytes and IPG insertion
**                                   must be controlled by the upstream source
**                               Default setting 0
**                  unidirEn   - enable/disable the unidirectional enable
**                               parameter from IEEE 802.3
**                               0 - Unidirectional operation is disabled
**                               1 - Unidirectional operation is enabled
**                               Default setting 0
**                  sendIdleEn - enable/disable idle generation when RF ordered
**                               sets are detected in the receive PCS
**                               0 - disable idle generation
**                               1 - enable idle generation
**                               Default setting 1
**                  fifoHmark  - identifies the number of 16-byte stored in
**                               the internal FIFO which transfers from
**                               the system interface
**                               Default setting b'00110
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hsciHyPhy20gHsciGsup43TxCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 errDrop, UINT4 pktType, UINT4 osType,
                                 UINT4 forceMinIpg, UINT4 unidirEn,
                                 UINT4 sendIdleEn, UINT4 fifoHmark)
{
    INT4 result;
    UINT4 addr, val;

    /* argument checking */
    if (slice > 1 ||
        errDrop > 1 ||
        pktType > 0xFF ||
        osType > 0xFF ||
        forceMinIpg > 1 ||
        unidirEn > 1 ||
        sendIdleEn > 1 ||
        fifoHmark > 0x1F) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_TX_CFG_1, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, errDrop,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_1_TX_ERR_DROP_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_1_TX_ERR_DROP_OFF);
    val = mHYPHY20G_SET_FIELD4(val, pktType,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_1_TX_PKT_TYPE_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_1_TX_PKT_TYPE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, osType,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_1_TX_OS_TYPE_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_1_TX_OS_TYPE_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_TX_CFG_2, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, forceMinIpg,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_FORCE_MIN_IPG_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_FORCE_MIN_IPG_OFF);
    val = mHYPHY20G_SET_FIELD4(val, unidirEn,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_UNIDIR_EN_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_UNIDIR_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, sendIdleEn,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_SEND_IDLE_EN_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_SEND_IDLE_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, fifoHmark,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_FIFO_HMARK_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_FIFO_HMARK_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsciGsup43TxCfg */

/*******************************************************************************
**
**  hsciHyPhy20gHsciRxFcDlolbIntPer
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function calculates the RX_DLOLB error event integration
**                period and applies it to an HSCI FC_CBR slice.
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
**                 slice        - HSCI slice processing resource
**                                slice number 0 - 1
**                 period       - Integration period in mseconds
**                                must be 1 or greater.
**                                maximum value is a function of the
**                                resolution of the intergration period register
**                                fields, the client rate and the bus width
**                                used between the SERDES and HSCI slice.
**                 frequency    - CBR Client frequency in units of kbits/s.
**                                minimum value 8000000 (8 Gbits/s)
**                                maximum value 11320000 (11.32 Gbits/s)
**                 busWidth     - bus width used between SERDES and HSCI
**                                (using same encoding as BUS_WIDTH field in
**                                 FC_CBR CFG register)
**                                4 - 32 bit (8b10b encoded clients)
**                                5 - 40 bit (non 8b10b encoded clients)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hsciHyPhy20gHsciRxFcDlolbIntPer(struct file *fileHndl, UINT4 slice,
                                     UINT4 period, UINT4 frequency,
                                     UINT4 busWidth)
{

    /* variable declaration */
    UINT4 addr, msk;
    UINT4 busWidthInBits, i, num, div, perLim, dlolbIntPer, dlolbClkDiv;
    INT4 result;

    /* argument checking */
    if((slice > 1)||(period == 0)||(period > 0xFFFFFFFF)||
        (frequency < 8000000)||(frequency > 11320000)||
        (busWidth > 5)||(busWidth < 4)){
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
        case 4:
            busWidthInBits = 32;
            break;
        case 5:
            busWidthInBits = 40;
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
    addr = HYPHY20G_HSCI_FC_CBR_REG_RX_DLO_CLK_CFG;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_CLK_CFG_RX_DLOLB_CLK_DIV_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, dlolbClkDiv);
    if(result) return result;

    addr = HYPHY20G_HSCI_FC_CBR_REG_RX_DLO_EN;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_HSCI_FC_CBR_BIT_RX_DLO_EN_RX_DLOLB_INT_PER_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, dlolbIntPer);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsciRxFcDlolbIntPer */

/*******************************************************************************
**
**  hsciHyPhy20gHsciTxFcDlolbIntPer
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function calculates the TX_DLOLB error event integration
**                period and applies it to an HSCI FC_CBR slice.
**
**                This function configures TX_DLOLB_INT_PER and
**                TX_DLOLB_CLK_DIV fields.
**
**                This function performs integer multiplication and division,
**                it assumes 32 bit variables are being used.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - HSCI slice processing resource
**                                slice number 0 - 1
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
INT4 hsciHyPhy20gHsciTxFcDlolbIntPer(struct file *fileHndl, UINT4 slice,
                                     UINT4 period)
{
    /* variable declaration */
    UINT4 addr, msk;
    UINT4 i, num, div, perLim, dlolbIntPer, dlolbClkDiv, frequency;
    INT4 result;

    /* argument checking */
    if((slice > 1)||(period == 0)||(period > 102873)){
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
    addr = HYPHY20G_HSCI_FC_CBR_REG_TX_DLO_CLK_CFG;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_CLK_CFG_TX_DLOLB_CLK_DIV_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, dlolbClkDiv);
    if(result) return result;

    addr = HYPHY20G_HSCI_FC_CBR_REG_TX_DLO_EN;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_HSCI_FC_CBR_BIT_TX_DLO_EN_TX_DLOLB_INT_PER_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, dlolbIntPer);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsciTxFcDlolbIntPer */


/*******************************************************************************
**
**  hsciHyPhy20gHsciRxxgPrmbCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures a specific way to process preamble
**                  in the RXXG block.
**                  
**                  When the prmbProc == 0:
**                    RXXG block will strip out Preamble/SFD;
**                    RXXG block accepts 0-11 bytes of non-0xD5 byte values
**                    followed by SFD byte;
**                  When the prmbProc == 1:
**                    RXXG block will strip out Preamble/SFD;
**                    RXXG block accepts any number of non-0xD5 byte values
**                    followed by SFD byte;
**                  When the prmbProc == 2:
**                    RXXG block will strip out Preamble/SFD;
**                    RXXG block accepts 0-11 bytes of 0x55 byte values
**                    followed by SFD byte;
**                  When the prmbProc == 3:
**                    RXXG block will strip out Preamble/SFD;
**                    RXXG block accepts any number of 0x55 byte values 
**                    followed by SFD byte;
**                  When the prmbProc == 4:
**                    RXXG block passes custom header to downstream;
**                    RXXG block expects all packets to have a custom header 
**                    of fixed size specified by 'hdrSize' parameter;
**                    Here hdrSize == 0 is a special case, and it means RXXG 
**                    block expects no preamble/SFD. 
**                  When the prmbProc == 5:
**                    RXXG block passes custom header to downstream;
**                    RXXG block scans custom header (in the range of byte 1  
**                    to 'hdrSize') for the standard preamble sequence (number
**                    of 0x55 bytes followed by a 0xD5 byte);
**                    If this sequence is found, the DA bytes are counted 
**                    from the byte after the 0xD5 byte; 
**                    If this sequence is not found, the DA bytes are counted 
**                    from the byte after the custom header (header size 
**                    specified by 'hdrSize' parameter); 
**                  
**                  Note: 
**                    The prmbProc == 0 ~ 3 are standard IEEE 802.3 application;
**                    The prmbProc == 4 is custom preamble application;
**                    The prmbProc == 5 supports custom preamble and is 
**                    typically used in EOS application; If the 'hdrSize' > 7 
**                    in this scenario, RXXG supports packets mixed of standard
**                    preamble and custom preamble.  
**
**                  This function is only used for 10GE MAC Terminated,
**                  G.Sup43 Clause 7.3 and 6.2 Terminating application 
**                  through MAC modes.
**
**                  Note:
**                    HDR_AND_PRMBL_EN bit can only be changed when RXXG block
**                    is disabled (RXEN is b'0).
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  prmbProc  - determines how RXXG processes preamble; 
**                              refer to API description for details; 
**                              Legal range 0 to 5
**                  hdrSize   - specifies custom header size (in bytes);
**                              When prmbProc == 0 ~ 3, set to 0;
**                              When prmbProc == 4, Legal range 0 to 0xFF;
**                              When prmbProc == 5, Legal range 1 to 0xFF.
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hsciHyPhy20gHsciRxxgPrmbCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 prmbProc, UINT4 hdrSize)
{
    INT4 result;
    UINT4 addr;
    UINT4 val;
    UINT4 noPrmb;
    UINT4 purePrmb;
    UINT4 longPrmb;
    UINT4 hdrPrmblEn;
    
    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        prmbProc > 5 || hdrSize > 0xFF ||
        (prmbProc == 5 && hdrSize < 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    /*
     * initializes preamble config related variables
     */
    switch(prmbProc)
    {
        case 0:
            noPrmb      = 0;
            purePrmb    = 0;
            longPrmb    = 0;
            hdrPrmblEn  = 0;
            hdrSize     = 0;
            break;
        case 1:
            noPrmb      = 0;
            purePrmb    = 0;
            longPrmb    = 1;
            hdrPrmblEn  = 0;
            hdrSize     = 0;
            break;
        case 2:
            noPrmb      = 0;
            purePrmb    = 1;
            longPrmb    = 0;
            hdrPrmblEn  = 0;
            hdrSize     = 0;
            break;
        case 3:
            noPrmb      = 0;
            purePrmb    = 1;
            longPrmb    = 1;
            hdrPrmblEn  = 0;
            hdrSize     = 0;
            break;
        case 4:
            noPrmb      = 1;
            purePrmb    = 0;
            longPrmb    = 0;
            hdrPrmblEn  = 0;
            break;
        case 5:
            noPrmb      = 1;
            purePrmb    = 0;
            longPrmb    = 0;
            hdrPrmblEn  = 1;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /*
     * Configure preamble related bits in ENET_10G RXXG block
     */
    addr = HYPHY20G_HSCI_ENET_10G_RXXG_REG_CFG_1;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, noPrmb,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_NO_PRMB_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_NO_PRMB_OFF);
    val = mHYPHY20G_SET_FIELD4(val, purePrmb,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PUREP_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_PUREP_OFF);
    val = mHYPHY20G_SET_FIELD4(val, longPrmb,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_LONGP_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_1_LONGP_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;
    
    addr = HYPHY20G_HSCI_ENET_10G_RXXG_REG_CFG_2;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, hdrPrmblEn,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_2_HDR_AND_PRMBL_EN_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_2_HDR_AND_PRMBL_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, hdrSize,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_2_HDRSIZE_MSK,
            HYPHY20G_HSCI_ENET_10G_RXXG_BIT_CFG_2_HDRSIZE_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gHsciRxxgPrmbCfg */


/*******************************************************************************
**
**  hsciHyPhy20gTxElcassPauseTagCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures a tag in Tx ECLASS to filter the 
**                  PAUSE frames generated from the TXXG block.
**
**                  This function is only used for G.Sup43 Clause 7.3 and 6.2
**                  Terminating application through MAC modes.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hsciHyPhy20gTxElcassPauseTagCfg(struct file *fileHndl, UINT4 slice)
{
    INT4 result;
    UINT4 addr;
    UINT4 val;
    UINT4 n;
    UINT4 mode;    
    UINT4 FILTER_16BIT_START_BYTE_0	        = 0x0D; 
    UINT4 FILTER_16BIT_START_BYTE_1	        = 0x0F;
    UINT4 FILTER_16BIT_DATA_0	            = 0x8808;
    UINT4 FILTER_16BIT_DATA_CMP_EN_0	    = 0xFFFF;
    UINT4 FILTER_16BIT_DATA_1	            = 0x0001;
    UINT4 FILTER_16BIT_DATA_CMP_EN_1	    = 0xFFFF;
    UINT4 FILTER_16BIT_OUT_FINAL_AND_EN	    = 0xC0000000; /* filter16 bit#31 AND 
                                                             filter16 bit#32 */
    UINT4 FILTER_OR_OUT_FINAL_AND_EN	    = 0x0000;
    UINT4 NOT_FILTER_16BIT_OUT_FINAL_AND_EN = 0x00000000;
    UINT4 NOT_FILTER_OR_OUT_FINAL_AND_EN	= 0x0000;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &mode);
    if(result) return result;
    /* confirm mode before processing */
    if (mode < HSCI_MAC_GSUP43_CLAUSE73) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    if (mode == HSCI_MAC_GSUP43_CLAUSE73) {
        /* As the TMAC inserts the Preamble and SFD before the TX_ECLASS, add 
           8 bytes to the start value. */
        FILTER_16BIT_START_BYTE_0 += 8;
        FILTER_16BIT_START_BYTE_1 += 8;
    }
    
    /* FILTER_16BIT CONFIGURATION with instances n 30-31 */
    n    = 30;
    addr = HYPHY20G_HSCI_ENET_10G_TX_ECLASS_REG_FLT_16_STR_BYT_LOC_OFFSET(n);
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gWrite(fileHndl, addr, FILTER_16BIT_START_BYTE_0);
    if(result) return result;

    addr = HYPHY20G_HSCI_ENET_10G_TX_ECLASS_REG_FLT_16_STR_BYT_LOC_OFFSET(++n);
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gWrite(fileHndl, addr, FILTER_16BIT_START_BYTE_1);
    if(result) return result;

    /* FILTER_16BIT CONFIGURATION with instances n 30-31 */
    n    = 30;
    addr = HYPHY20G_HSCI_ENET_10G_TX_ECLASS_REG_FLT_16_CFG_OFFSET(n);
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    val  = (FILTER_16BIT_DATA_0 << 16) + FILTER_16BIT_DATA_CMP_EN_0;
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    addr = HYPHY20G_HSCI_ENET_10G_TX_ECLASS_REG_FLT_16_CFG_OFFSET(++n);
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    val  = (FILTER_16BIT_DATA_1 << 16) + FILTER_16BIT_DATA_CMP_EN_1;
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* FILTER_16BIT_AND_EN SELECTION FOR FILTER_OUT */
    n    = 7;
    addr = HYPHY20G_HSCI_ENET_10G_TX_ECLASS_REG_FLT_OUT_FLT_16_SEL_OFFSET(n);
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gWrite(fileHndl, addr, FILTER_16BIT_OUT_FINAL_AND_EN);
    if(result) return result;

    /* FILTER_OR AND SELECTION FOR FILTER_OUT */
    addr = HYPHY20G_HSCI_ENET_10G_TX_ECLASS_REG_FLT_OUT_FLT_OR_SEL_OFFSET(n);
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gWrite(fileHndl, addr, FILTER_OR_OUT_FINAL_AND_EN);
    if(result) return result;

    /* NOT(FILTER_16BIT) AND SELECTION FOR FILTER_OUT */
    addr = HYPHY20G_HSCI_ENET_10G_TX_ECLASS_REG_FLT_OUT_FLT_16_NOT_SEL_OFFSET(n);
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gWrite(fileHndl, addr, NOT_FILTER_16BIT_OUT_FINAL_AND_EN);
    if(result) return result;

    /* NOT(FILTER_OR) AND SELECTION FOR FILTER_OUT */
    addr = HYPHY20G_HSCI_ENET_10G_TX_ECLASS_REG_FLT_OUT_FLT_OR_NOT_SEL_OFFSET(n);
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gWrite(fileHndl, addr, NOT_FILTER_OR_OUT_FINAL_AND_EN);
    if(result) return result;
    
    addr = HYPHY20G_HSCI_ENET_10G_TX_ECLASS_REG_FLT_OUT_EN;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr, 7, 1);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hsciHyPhy20gTxElcassPauseTagCfg */

/*******************************************************************************
**
**  hsciHyPhy20gHsciGetCtxtSize
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Calculate the size of HSCI context.
**
**
**  INPUTS:         fileHndl      - device handle
**
**  OUTPUTS:        None
**
**  SIDE_EFFECTS:   None
**
**  RETURNS:        The size of HSCI context
**
*******************************************************************************/
UINT4 hsciHyPhy20gHsciGetCtxtSize(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_HSCI_CTXT *pHsciCtxt;
    UINT4 slice = 0, size = 0;

    /* retrieve device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), 0,
                    "%s", "pDevCtxt is NULL.");

    /* calculate each slice context size */
    for (slice = 0; slice < HYPHY20G_HSCI_NUM_SLICE; slice++) {
        /* include ctxt header size */
        size += utilHyPhy20gGetCtxtHdrSize();

        /* calculate this slice context size */
        pHsciCtxt = pDevCtxt->pHsciCtxt[slice];
        if (pHsciCtxt != NULL) {
            size += sizeof(sHYPHY20G_HSCI_CTXT);
        }
    }

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, size);

    return size;
}/* hsciHyPhy20gHsciGetCtxtSize */

/*******************************************************************************
**
**  hsciHyPhy20gHsciExtractCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Extract the HSCI context into pre-allocated buffer.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-allocated buffer for HSCI context extraction
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
INT4 hsciHyPhy20gHsciExtractCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pExtractedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_HSCI_CTXT *pHsciCtxt;
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
    for (slice = 0; slice < HYPHY20G_HSCI_NUM_SLICE; slice++) {

        /* retrieve this slice context */
        pHsciCtxt = pDevCtxt->pHsciCtxt[slice];

        DBG_PRINT("%s, %s, %d, slice %ld offset 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, offset);
        
		/* insert ctxt buffer header */
        len = (pHsciCtxt == NULL) ? sizeof(sHYPHY20G_CTXT_HDR) : 
                (sizeof(sHYPHY20G_CTXT_HDR) + sizeof(sHYPHY20G_HSCI_CTXT));
        rtLen = utilHyPhy20gInsertCtxtHdr(((UINT1 *)pCtxtBuf + offset), len, 
                                            HYPHY_HSCI_CTXT_ID, slice);
        HYPHY20G_ASSERT((rtLen > 0), HYPHY20G_ERR_NULL_PTR_PASSED, 
            "%s", "pCtxtBuf is NULL.");  
        offset += sizeof(sHYPHY20G_CTXT_HDR);
        
        if (pHsciCtxt != NULL) {
            /* copy this slice context into the buffer */
            sysHyPhy20gMemCpy(((UINT1 *)pCtxtBuf + offset), 
                            (void *)pHsciCtxt, sizeof(sHYPHY20G_HSCI_CTXT));
            offset += sizeof(sHYPHY20G_HSCI_CTXT);
        }
    }/* end of for slice loop*/

    DBG_PRINT("%s, %s, %d, offset end 0x%lx\n", 
                __FILE__, __FUNCTION__, __LINE__, offset);

    *pExtractedSize = offset;
    
    return HYPHY20G_SUCCESS;
}/* hsciHyPhy20gHsciExtractCtxt */

/*******************************************************************************
**
**  hsciHyPhy20gHsciRestoreCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Restore the HSCI context into the driver context memory.
**
**
**  INPUTS:         fileHndl      - device handle
**                  pCtxtBuf - pre-stored HSCI context for restoration
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
INT4 hsciHyPhy20gHsciRestoreCtxt(struct file *fileHndl, 
                                void *pCtxtBuf, UINT4 *pParsedSize)
{
    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_HSCI_CTXT *pHsciCtxt;
    sHYPHY20G_CTXT_HDR *pCtxtBufHdr;
    INT4 result;
    UINT4 offset = 0, len = 0, slice = 0, i = 0;

    /* check the inputs */
    HYPHY20G_ASSERT((pCtxtBuf != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pCtxtBuf is NULL.");
    HYPHY20G_ASSERT((pParsedSize != NULL), HYPHY20G_ERR_NULL_PTR_PASSED, 
        "%s", "pParsedSize is NULL.");

    /* try to retrieve the hsci[0] header in the pre-stored context buffer */
    pCtxtBufHdr = utilHyPhy20gSearchCtxtHdr(pCtxtBuf, HYPHY_HSCI_CTXT_ID, 0);
    if (pCtxtBufHdr == NULL) {
        DBG_PRINT("%s, %s, %d, can not find HYPHY_HSCI_CTXT_ID hdr\n", 
                    __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_DEV_CTXT_CORRUPTED;
    }

    pCtxtBuf = (void *)pCtxtBufHdr;
    
    /* restore each slice context */
    for (slice = 0; slice < HYPHY20G_HSCI_NUM_SLICE; slice++) {

        DBG_PRINT("%s, %s, %d, slice %ld offset 0x%lx\n", 
                    __FILE__, __FUNCTION__, __LINE__, slice, offset);

        /* invalid data for this slice */
        if (pCtxtBufHdr->len == sizeof(sHYPHY20G_CTXT_HDR)) {
            offset += sizeof(sHYPHY20G_CTXT_HDR);
        } else if (pCtxtBufHdr->len > sizeof(sHYPHY20G_CTXT_HDR)) {
            /* create this slice context */
            result = hsciHyPhy20gHsciCreateCtxt(fileHndl, slice, 1);
            if (result) {
                DBG_PRINT("%s, %s, %d, CreateCtxt failed for slice %ld\n",
                            __FILE__, __FUNCTION__, __LINE__, slice);

                for (i = 0; i < slice; i++) {
                    hsciHyPhy20gHsciDestroyCtxt(fileHndl, i);
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
            pHsciCtxt = pDevCtxt->pHsciCtxt[slice];

            /* offset pCtxtBuf to figure out the payload position */
            offset += sizeof(sHYPHY20G_CTXT_HDR);
            /* payload len */
            len = pCtxtBufHdr->len - 
                    sizeof(sHYPHY20G_CTXT_HDR);
            
            sysHyPhy20gMemCpy((void *)(pHsciCtxt), 
                            ((UINT1 *)pCtxtBuf + offset), len);
            offset += len;
        }else {
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
}/* hsciHyPhy20gHsciRestoreCtxt */


/********************************************************************************
**
**  hsciHyPhy20gHsciAuditCtxt 
**  ___________________________________________________________________________
**
**  DESCRIPTION:    the function used to audit the running HSCI subCTXT.
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
INT4 hsciHyPhy20gHsciAuditCtxt (struct file *fileHndl)
{
    INT4 result;
    UINT4 slice, lowPwrOff, lowPwr, rstOff, rstBit;

    sHYPHY20G_CTXT *pDevCtxt;
    sHYPHY20G_HSCI_CTXT *pHsciCtxt;

    /* reference device context pointer */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_FAILURE,
                    "%s", "pDevCtxt is NULL.");    

    for (slice=0; slice<HYPHY20G_HSCI_NUM_SLICE; slice++)
    {
        pHsciCtxt=pDevCtxt->pHsciCtxt[slice];
      
        lowPwrOff = HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_ENET_HSCI_LOWPWR_0_OFF + slice;
        result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                                     lowPwrOff, &lowPwr);
        
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 


        /* the HSCI slice disabled */
        if (lowPwr == 1)
        {
            if (pHsciCtxt!=NULL)
            {
                DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
                return HYPHY20G_FAILURE;
            }
            else
                continue;
        }

        rstOff = HYPHY20G_TOP_BIT_SYS_SW_RST_2_ENET_HSCI_RST_0_OFF + slice;
        result = sysHyPhy20gBitRead(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                     rstOff, &rstBit);
        HYPHY20G_ASSERT((result == HYPHY20G_SUCCESS), HYPHY20G_FAILURE,
                        "%s", "sysHyPhy20gBitRead fail."); 
        if (rstBit == 1)
        {
            if (pHsciCtxt!=NULL)
            {
                DBG_PRINT("%s, %s, %d, %uth slice pHsciCtxt!=NULL when HSCI_RST == 1\n",
                    __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice );
                return HYPHY20G_FAILURE;
            }
            else
                continue;
        }
        
        /* the HSCI slice enabled */        
        if (pHsciCtxt==NULL)
        {
            DBG_PRINT("%s, %s, %d, %uth slice pHsciCtxt==NULL when this HSCI enabled.\n",
                __FILE__, __FUNCTION__, __LINE__, (unsigned int)slice );
            return HYPHY20G_FAILURE;
        }
        else if ((pHsciCtxt->valid!=HYPHY20G_VALID_SIGN)||(pHsciCtxt->traffic>HSCI_TRAFFIC_MAX)
                ||(pHsciCtxt->traffic<HSCI_10GE_MAC_TEMINATION))
        {
            DBG_PRINT("%s, %s, %d, HYPHY20G_FAILURE\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_FAILURE;
        }
    }
    
    return HYPHY20G_SUCCESS;
    
}/* hsciHyPhy20gHsciAuditCtxt */

/* end of file */
