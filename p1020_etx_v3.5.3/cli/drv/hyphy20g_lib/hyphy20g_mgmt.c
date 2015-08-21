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
**  MODULE      : HyPHY 20G SGMII Subsystem Configuration Code
**
**  FILE        : hyphy20g_mgmt.c
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
#include "hyphy20g_mgmt.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/


/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 mgmtHyPhy20gFeGeBlockInit(struct file *fileHndl, UINT4 enable);


/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/
/*******************************************************************************
**
**  hyPhy20gMgmtPortInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the LSCI_SGMII subsystem (MGMT
**                  port) to MAC Terminated operating mode.
**
**                  Alternatively, this function can be called to disable the
**                  LSCI_SGMII subsystem.
**
**  INPUTS:         fileHndl  - device handle
**                  enable    - enable or disable the MGMT port
**                              0 - disable
**                              1 - enable
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortInit(struct file *fileHndl, UINT4 enable)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 feGeLpwr, feGeRst, lsciRst;
    UINT4 sgmiiLpwr;
	
    /* argument checking */
    if (enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (enable) {
        /* enable settings */
        sgmiiLpwr      = 0;
        feGeLpwr       = 0;
        feGeRst        = 0;
        lsciRst        = 0;
    } else {
        /* disable settings */
        sgmiiLpwr      = 1;
        feGeLpwr       = 1;
        feGeRst        = 1;
        lsciRst        = 1;
    }

    /* set lowpwr bit to 1 for SGMII */
    addr = HYPHY20G_TOP_REG_SYS_LOW_PWR_2;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
             HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_ENET_LSCI_LOWPWR_16_OFF, sgmiiLpwr);
    if(result) return result;


    /* configure LSCI_SGMII Top level register */
    addr = HYPHY20G_SGMII_TOP_REG_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, feGeLpwr,
            HYPHY20G_SGMII_TOP_BIT_CFG_ENET_FEGE_LOWPWR_MSK,
            HYPHY20G_SGMII_TOP_BIT_CFG_ENET_FEGE_LOWPWR_OFF);
    val = mHYPHY20G_SET_FIELD4(val, feGeRst,
            HYPHY20G_SGMII_TOP_BIT_CFG_ENET_FEGE_RST_MSK,
            HYPHY20G_SGMII_TOP_BIT_CFG_ENET_FEGE_RST_OFF);
    val = mHYPHY20G_SET_FIELD4(val, lsciRst,
            HYPHY20G_SGMII_TOP_BIT_CFG_LSCI_RST_MSK,
            HYPHY20G_SGMII_TOP_BIT_CFG_LSCI_RST_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    if (!enable) {           /* disable operations */
        return HYPHY20G_SUCCESS;
    }

    result = mgmtHyPhy20gFeGeBlockInit(fileHndl, 1);
    if(result) return result;

    /* max frame size to 9618 bytes for jumbo frames */
    result = hyPhy20gMgmtPortFeGeSetFrameSizes(fileHndl, 0x2592,
                                               0x00, 0x2592, 0x00);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortInit */


/*******************************************************************************
**
**  hyPhy20gMgmtPortDataPathEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables the Rx and Tx channel
**                  data flow separately in the management port.
**
**                  This function must called to enable traffic after the
**                  management port is initialized and mode specific
**                  configuration has done.
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
**                  dir       - data flow direction
**                              0 - enable/disable the transmit data flow
**                                  (from the ENET_COM to the SFP)
**                              1 - enable/disable the receive data flow
**                                  (from the SFP to the ENET_COM)
**                  enable    - 0 - disable
**                              1 - enable
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortDataPathEnable(struct file *fileHndl, UINT4 dir,
                                    UINT4 enable)
{
    INT4 result;
    UINT4 macRegAddr;
    UINT4 macOff;

    /* argument checking */
    if (dir > 1 || enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (dir == HYPHY20G_TX_DIRECTION) {
        macRegAddr = HYPHY20G_SGMII_ENET_FEGE_TMAC_REG_CFG_1;
        macOff = HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_TMAC_ENBL_OFF;

    } else {
        macRegAddr = HYPHY20G_SGMII_ENET_FEGE_RMAC_REG_CFG_1;
        macOff = HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_RMAC_ENBL_OFF;
    }

    result = sysHyPhy20gBitWrite(fileHndl, macRegAddr, macOff, enable);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gMgmtPortDataPathEnable */


/*******************************************************************************
**
**  hyPhy20gMgmPortFeGeModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the ENET_FEGE block in the MGMT
**                  port for MAC Terminated traffic operation mode.
**
**                  When LSCI slice is configured in MAC Terminated
**                  (FE/GE, SGMII FE/GE) mode, this function
**                  must be called after hyPhy20gMgmtPortInit is executed
**                  but prior to hyPhy20gMgmtPortDataPathEnable (enable
**                  operation).
**
**                  Along with the major LSCI_SGMII block operating mode, this
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
**                  hyPhy20gMgmtPortGeAnBasePageCfg function prior to calling
**                  hyPhy20gMgmtPortFeGeModeCfg.
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
**                    hyPhy20gMgmtPortSgmiiAnBpAbilityCompare API to compare
**                    PHY's Control Information with MAC's setting.
**
**  INPUTS:         fileHndl  - device handle
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
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gMgmPortFeGeModeCfg(struct file *fileHndl, UINT4 mode)
{
    INT4 result;
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

    /* argument checking */
    if (mode > LSCI_SGMII_FE_MAC_TEMINATION_MANACK) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /*
     * unconditionally disable L1RPP and L1TPP block prior to configuration
     */
    /* disable L1RPP block */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_CFG_AND_STAT;
    bitOff = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_L1RPP_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitOff, 0);
    if(result) return result;

    /* disable L1TPP block */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1TPP_REG_CFG;
    bitOff = HYPHY20G_SGMII_ENET_FEGE_L1TPP_BIT_CFG_L1TPP_EN_OFF;
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
    addr = HYPHY20G_SGMII_TOP_REG_CFG;
    bitOff = HYPHY20G_SGMII_TOP_BIT_CFG_TX_10B_SWAP_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitOff, txSwapVal);
    if(result) return result;

    /*
     * configure ENET_FEGE Top Level register
     */
    /* configure the mode in which this FEGE Ethernet Channel is to operate */
    addr = HYPHY20G_SGMII_ENET_FEGE_TOP_REG_MD_CFG;
    msk  = HYPHY20G_SGMII_ENET_FEGE_TOP_BIT_MD_CFG_FEGE_MODE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, fegeMdVal);
    if(result) return result;

    /*
     * configure L1RPP block
     */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_CFG_AND_STAT;
    msk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_MODE_MSK;
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
        addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_DBG_LNK_TMR_OVERRIDE;
        msk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_DBG_LNK_TMR_OVERRIDE_LINK_TIMER_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x30E);
        if(result) return result;
    }
    
    /* configure LINK TIME DURATION for the FE mode */
    if (mode == LSCI_FE_MAC_TEMINATION) {
        addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_LNK_TM_DUR;
        msk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_LNK_TM_DUR_LINK_TIME_DUR_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, linkTimeVal);
        if(result) return result;
    }

    /* enable/disable the Auto-Negotiation feature */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_MII_CTL;
    bitOff = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_CTL_AN_ENABLE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitOff, anEnblVal);
    if(result) return result;

    if (anEnblVal) {
        /* configure MII AUTO-NEGOTIATION ADVERTISEMENT register (base page) */
        addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_MII_AUTO_NEG_AD;
        regData = 0x0000;

        if (mode == LSCI_SGMII_GE_MAC_TEMINATION        ||
            mode == LSCI_SGMII_FE_MAC_TEMINATION        ||
            mode == LSCI_SGMII_GE_MAC_TEMINATION_MANACK ||
            mode == LSCI_SGMII_FE_MAC_TEMINATION_MANACK) {

            msk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_BIT0_MSK;
            result = hyPhy20gVarFieldWrite(&regData, msk, 1);
            if(result) return result;

        } else if (mode == LSCI_GE_MAC_TEMINATION ||
                   mode == LSCI_GE_MAC_TEMINATION_AN_NEXTPG) {

            msk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_NEXT_PAGE_MSK;
            result = hyPhy20gVarFieldWrite(&regData, msk, locNextPgVal);
            if(result) return result;

            msk = HYPHY20G_LSCI_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_HALF_DUPLEX_MSK;
            result = hyPhy20gVarFieldWrite(&regData, msk, 0);
            if(result) return result;

            msk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_FULL_DUPLEX_MSK;
            result = hyPhy20gVarFieldWrite(&regData, msk, 1);
            if(result) return result;
        }

        DBG_PRINT("%s, %s, %d, value 0x%x to write to base page register.\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)regData);
        result = sysHyPhy20gWrite(fileHndl, addr, regData);
        if(result) return result;

        /* load the base page setting to the core */
        addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_CFG_AND_STAT;
        msk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_AN_ABLTY_ADV_UPD_MSK;
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
    addr = HYPHY20G_SGMII_ENET_FEGE_L1TPP_REG_CFG;
    msk  = HYPHY20G_SGMII_ENET_FEGE_L1TPP_BIT_CFG_MODE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, l1tppMdVal);
    if(result) return result;

    /* enable L1TPP block */
    bitOff = HYPHY20G_SGMII_ENET_FEGE_L1TPP_BIT_CFG_L1TPP_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitOff, 1);
    if(result) return result;

    /* enable L1RPP block */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_CFG_AND_STAT;
    bitOff = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_L1RPP_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, bitOff, 1);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmPortFeGeModeCfg */



/*******************************************************************************
**
**  hyPhy20gMgmtPortGeAnNextPageCfg
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
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortGeAnNextPageCfg(struct file *fileHndl, UINT4 nextPg,
                                     UINT4 msgType, UINT4 ack2, UINT4 msgCode)
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

    /* argument checking */
    if (nextPg > 1  ||
        msgType > 1 ||
        ack2 > 1    ||
        msgCode > 0x07FF) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    stateAddr      = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_DBG_SM_STAT;
    stateMsk       = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_DBG_SM_STAT_AN_STATE_MSK;
    completeAddr   = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_MII_STAT;
    completeOff    = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_STAT_AN_COMPLETE_OFF;

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

    nextPgAddr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_MII_AUTO_NEG_NXT_PG_TX;
    nextPgMsk  = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_NXT_PG_TX_LOC_NP_NEXT_PAGE_MSK;
    msgTypeMsk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_NXT_PG_TX_LOC_NP_MSG_PAGE_MSK;
    ack2Msk    = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_NXT_PG_TX_LOC_NP_ACK2_MSK;
    msgCodeMsk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_NXT_PG_TX_LOC_NP_CODE_FIELD_MSK;

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

} /* hyPhy20gMgmtPortGeAnNextPageCfg */


/*******************************************************************************
**
**  hyPhy20gMgmtPortSgmiiAnBpAbilityCompare
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
**                  mode '6' and '7' of hyPhy20gMgmtPortFeGeModeCfg API).
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
**                     hyPhy20gMgmtPortSgmiiAnBpAbilityCompare API implemented
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
**                     hyPhy20gMgmtPortFeGeModeCfg API can be used to
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
**                    1) ENET_FEGE block is configured in mode '6' or '7' of
**                       by hyPhy20gMgmtPortFeGeModeCfg API, and
**                    2) AUTO_NEG_LP_ABLTY_UPD_I bit is asserting.
**
**                  It is user's responsibility to re-configure either MAC
**                  or PHY if their ability mis-match.
**
**
**  INPUTS:         fileHndl  - device handle
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
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortSgmiiAnBpAbilityCompare(struct file *fileHndl,
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

    /* argument checking */
    if (mmCode == NULL) {
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* Retrieve ENET_FEGE mode and check pre-conditions */
    addr = HYPHY20G_SGMII_ENET_FEGE_TOP_REG_MD_CFG;
    feGeMdMsk = HYPHY20G_SGMII_ENET_FEGE_TOP_BIT_MD_CFG_FEGE_MODE_MSK;
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
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_MII_AUTO_NEG_LNK_PRT_BAS_PG_ABL;
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    lnkStatMsk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_LINK_STATUS_MSK;
    rsvd0Msk   = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_RSVD_0_MSK;
    dplxMdMsk  = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_DUPLEX_MODE_MSK;
    speedMsk   = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_SPEED_MSK;
    rsvd1Msk   = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_LNK_PRT_BAS_PG_RSVD_1_MSK;

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
    ackMsk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_MII_AUTO_NEG_AD_ACK_MSK;
    updOff = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_AN_ABLTY_ADV_UPD_OFF;
    updMsk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_AN_ABLTY_ADV_UPD_MSK;

    /* write ACKNOWLEDGE, bit-14 to b'1 in AUTO-NEGOTIATION
       ADVERTISEMENT register (base page) */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_MII_AUTO_NEG_AD;
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&regData, ackMsk, 1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if(result) return result;

    /* load the new base page value to the L1RPP core */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_CFG_AND_STAT;
    result = sysHyPhy20gBitWrite(fileHndl, addr, updOff, 1);
    if(result) return result;

    /* Wait for AN_ABLTY_ADV_UPD bit to self-cleared */
    result = sysHyPhy20gPollBit(fileHndl, addr, updMsk,
                                0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortSgmiiAnBpAbilityCompare */


/*******************************************************************************
**
**  hyPhy20gMgmtPortFeGeTmacCfg1
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure TMAC block Configuration 1 register within an
**                  SGMII_LSCI's ENET_FEGE slice.
**
**                  Refer to register 0x4906000 for further details
**                  on the functionalities configured with this function.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gMgmtPortInit but prior to
**                  hyPhy20gMgmtPortDataPathEnable
**
**  INPUTS:         fileHndl  - device handle
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
**                  minFrmCkEn
**                            - enable/disable checking the minimum frame length
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  truncEn   - enable/disable truncating the MAC frame
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  maxFrmCkEn
**                            - enable/disable checking the maximum frame length
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  ibufHthrEn
**                            - enable/disable outgoing flow control of
**                              IBUF_HTHR request
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  xonEn     - enable/disable sending an XON PAUSE
**                              control frame
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  rmacPauseEn
**                            - enable/disable pause request upon the
**                              completion of the current packet transmission
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  ebufErrEn - enable/disable checking EBUF_ERR from the
**                              upstream and passes it along with packet data
**                              to downstream.
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortFeGeTmacCfg1(struct file *fileHndl,
                                  UINT4 crcCkEn, UINT4 padAddEn,
                                  UINT4 crcAddEn, UINT4 minFrmCkEn,
                                  UINT4 truncEn, UINT4 maxFrmCkEn,
                                  UINT4 ibufHthrEn, UINT4 xonEn,
                                  UINT4 rmacPauseEn, UINT4 ebufErrEn)
{
    INT4 result;
    UINT4 addr, val;

    /* argument checking */
    if (crcCkEn > 1     ||
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

    addr = HYPHY20G_SGMII_ENET_FEGE_TMAC_REG_CFG_1;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, crcCkEn,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_CRC_CK_MSK,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_CRC_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, padAddEn,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_PAD_ADD_EN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_PAD_ADD_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, crcAddEn,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_CRC_ADD_EN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_CRC_ADD_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, minFrmCkEn,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_MIN_FRM_CK_MSK,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_MIN_FRM_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, truncEn,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_TRUNC_EN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_TRUNC_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, maxFrmCkEn,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_MAX_FRM_CK_MSK,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_MAX_FRM_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, ibufHthrEn,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_IBUF_HTHR_EN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_IBUF_HTHR_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, xonEn,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_XON_EN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_XON_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, rmacPauseEn,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_RMAC_PAUSE_EN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_RMAC_PAUSE_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, ebufErrEn,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_EBUF_ERR_EN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_EBUF_ERR_EN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortFeGeTmacCfg1 */

/*******************************************************************************
**
**  hyPhy20gMgmtPortFeGeTmacCfg2
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure TMAC block Configuration 2 register within an
**                  SGMII_LSCI's ENET_FEGE slice.
**
**                  Refer to register 0x4906004 for further details
**                  on the functionalities configured with this function.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gMgmtPortInit but prior to
**                  hyPhy20gMgmtPortDataPathEnable function
**
**  INPUTS:         fileHndl  - device handle
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
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortFeGeTmacCfg2(struct file *fileHndl,
                                  UINT4 sfdEn, UINT4 pambLen)
{
    INT4 result;
    UINT4 addr, val;

    /* argument checking */
    if (sfdEn > 1       ||
        pambLen > 15) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_SGMII_ENET_FEGE_TMAC_REG_CFG_2;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, sfdEn,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_2_SFD_EN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_2_SFD_EN_OFF);
    val = mHYPHY20G_SET_FIELD4(val, pambLen,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_2_PAMB_LEN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_2_PAMB_LEN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortFeGeTmacCfg2 */

/*******************************************************************************
**
**  hyPhy20gMgmtPortFeGeTxIpgCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure the length in octet of the Inter-packet Gap.
**                  The configuration is applied to the TMAC block.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gMgmtPortInit function.
**
**
**  INPUTS:         fileHndl  - device handle
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
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortFeGeTxIpgCfg(struct file *fileHndl, UINT4 txIPG1,
                                  UINT4 txIPG2)
{
    INT4 result;
    UINT4 addr, val;

    /* argument checking */
    if (txIPG1 > 15 || txIPG2 > 15) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_SGMII_ENET_FEGE_TMAC_REG_INTR_PAC_GAP_CFG_2;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, txIPG1,
        HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_INTR_PAC_GAP_CFG_2_FIPG1_LEN_MSK,
        HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_INTR_PAC_GAP_CFG_2_FIPG1_LEN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, txIPG2,
        HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_INTR_PAC_GAP_CFG_2_FIPG2_LEN_MSK,
        HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_INTR_PAC_GAP_CFG_2_FIPG2_LEN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortFeGeTxIpgCfg */

/*******************************************************************************
**
**  hyPhy20gMgmtPortFeGeRmacCfg1
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure RMAC block Configuration 1 register within an
**                  SGMII_LSCI's ENET_FEGE slice.
**
**                  Refer to register 0x4904100 for further details
**                  on the functionalities configured with this function.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gMgmtPortInit but prior to
**                  hyPhy20gMgmtPortDataPathEnable
**
**
**  INPUTS:         fileHndl  - device handle
**                  preambleCkEn
**                            - enable/disable preamble check
**                              0 - disable
**                              1 - enable
**                              Default setting 0
**                  tagAwareEn
**                            - enable/disable Tagging Extension Aware
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  inrangeDisEn
**                            - enable/disable in-range length error discard
**                              0 - disable
**                              1 - enable
**                              (if inrangeCk is non-zero)
**                              Default setting 1
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
**                  maxFrmTrnEn
**                            - enable/disable maximum frame length truncate
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  maxFrmDisEn
**                            - enable/disable maximum frame length discard
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  maxFrmCkEn
**                            - enable/disable maximum frame length check
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  minFrmDisEn
**                            - enable/disable minimum frame length discard
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  minFrmCkEn
**                            - enable/disable minimum frame length check
**                              0 - disable
**                              1 - enable
**                              Default setting 1
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
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortFeGeRmacCfg1(struct file *fileHndl,
                                  UINT4 preambleCkEn, UINT4 tagAwareEn,
                                  UINT4 inrangeDisEn, UINT4 inrangeCk,
                                  UINT4 maxFrmTrnEn, UINT4 maxFrmDisEn,
                                  UINT4 maxFrmCkEn, UINT4 minFrmDisEn,
                                  UINT4 minFrmCkEn, UINT4 crcDisEn,
                                  UINT4 crcCkEn)
{
    INT4 result;
    UINT4 addr, val;

    /* argument checking */
    if (preambleCkEn > 1    ||
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

    addr = HYPHY20G_SGMII_ENET_FEGE_RMAC_REG_CFG_1;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, preambleCkEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_PAMB_CK_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_PAMB_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, tagAwareEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_TAG_AWARE_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_TAG_AWARE_OFF);

    val = mHYPHY20G_SET_FIELD4(val, inrangeDisEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_INRANGE_DIS_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_INRANGE_DIS_OFF);

    val = mHYPHY20G_SET_FIELD4(val, inrangeCk,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_INRANGE_CK_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_INRANGE_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, maxFrmTrnEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_TRN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_TRN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, maxFrmDisEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_DIS_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_DIS_OFF);

    val = mHYPHY20G_SET_FIELD4(val, maxFrmCkEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_CK_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, minFrmDisEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_MIN_FRM_DIS_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_MIN_FRM_DIS_OFF);

    val = mHYPHY20G_SET_FIELD4(val, minFrmCkEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_MIN_FRM_CK_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_MIN_FRM_CK_OFF);

    val = mHYPHY20G_SET_FIELD4(val, crcDisEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_CRC_DIS_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_CRC_DIS_OFF);

    val = mHYPHY20G_SET_FIELD4(val, crcCkEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_CRC_CK_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_CRC_CK_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortFeGeRmacCfg1 */

/*******************************************************************************
**
**  hyPhy20gMgmtPortFeGeRmacCfg2
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure RMAC block Configuration 2 register within an
**                  SGMII_LSCI's ENET_FEGE slice.
**
**                  Refer to register 0x4904104 for further details
**                  on the functionalities configured with this function.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gMgmtPortInit but prior to
**                  hyPhy20gMgmtPortDataPathEnable
**
**  INPUTS:         fileHndl  - device handle
**                  interPktL1En
**                          0 - disable interpacket Layer 1 error report
**                          1 - enable interpacket Layer 1 error report
**                              Default setting 0
**                  padStripEn
**                          0 - don't pad
**                          1 - pad strip non-errored packets
**                              Note: rxCRCStrip must also be set when pad
**                                    stripping is enabled
**                              Default setting 0
**                  crcStripEn
**                          0 - don't strip received CRC from forwarded frame
**                          1 - strip rxed CRC from forwarded frame
**                              Default setting 0. set to 1 for proper operation.
**                  carrMonEn - determine if monitor the L1RPP_ER input
**                              0 - monitors for false carrier and
**                                  errored packet
**                              1 - in addition to false carrier and errored
**                                  packet, monitors for L1RPP_ER input
**                              Default setting 0
**                  pauseFrmEn- enable/disable Pause Control frame reception
**                              0 - disable
**                              1 - enable
**                              Default setting 1
**                  pauseFrmDropEn
**                          0 - drop PAUSE frames
**                          1 - forward PAUSE frames
**                              Default setting 1
**                  ctlFrmDropEn
**                          0 - drop control frames (except PAUSE frames)
**                          1 - forward control frames (except PAUSE frames)
**                              Default setting 0
**                  desAddrDropEn
**                          0 - forward only if destination address field
**                              contains a Unicast address and the destination
**                              destination field matches the station address
**                          1 - forward all MAC frames containing a Unicast
**                              address
**                              Default setting 0
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortFeGeRmacCfg2(struct file *fileHndl,
                                  UINT4 interPktL1En, 
                                  UINT4 padStripEn, UINT4 crcStripEn,
                                  UINT4 carrMonEn, UINT4 pauseFrmEn,
                                  UINT4 pauseFrmDropEn, UINT4 ctlFrmDropEn,
                                  UINT4 desAddrDropEn)
{
    INT4 result;
    UINT4 addr, val;

    /* argument checking */
    if (interPktL1En > 1    ||
        padStripEn > 1      ||
        crcStripEn > 1      ||
        carrMonEn > 1       ||
        pauseFrmEn > 1      ||
        pauseFrmDropEn > 1  ||
        ctlFrmDropEn > 1    ||
        desAddrDropEn > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_SGMII_ENET_FEGE_RMAC_REG_CFG_2;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, interPktL1En,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_INTERPKT_L1_EN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_INTERPKT_L1_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, padStripEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_PAD_STRIP_EN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_PAD_STRIP_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, crcStripEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_CRC_STRIP_EN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_CRC_STRIP_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, carrMonEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_CARRIER_EN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_CARRIER_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, pauseFrmEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_INPAUSE_EN_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_INPAUSE_EN_OFF);

    val = mHYPHY20G_SET_FIELD4(val, pauseFrmDropEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_PAUSEFRM_DROP_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_PAUSEFRM_DROP_OFF);

    val = mHYPHY20G_SET_FIELD4(val, ctlFrmDropEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_CTRLFRM_DROP_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_CTRLFRM_DROP_OFF);

    val = mHYPHY20G_SET_FIELD4(val, desAddrDropEn,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_DA_DROP_MSK,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_DA_DROP_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortFeGeRmacCfg2 */

/*******************************************************************************
**
**  hyPhy20gMgmtPortFeGeSetStationAddr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Changes the channel station MAC address in the RMAC
**                  and TMAC blocks. This function requires that traffic on the
**                  channel to be stopped.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function can optionally be called after
**                  hyPhy20gMgmtPortInit but prior to
**                  hyPhy20gMgmtPortDataPathEnable
**
**  INPUTS:         fileHndl  - device handle
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
INT4 hyPhy20gMgmtPortFeGeSetStationAddr(struct file *fileHndl,
                                        UINT4 hiSA, UINT4 midSA, UINT4 lowSA)
{
    INT4 result;
    UINT4 addr;

    /* argument checking */
    if (hiSA > 0xFFFF ||
        midSA > 0xFFFF ||
        lowSA > 0xFFFF) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Write Station Address to RMAC */
    addr = HYPHY20G_SGMII_ENET_FEGE_RMAC_REG_STN_ADR_HI_WD_CFG;
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_STN_ADR_HI_WD_CFG_STA_MSK, hiSA);
    if(result) return result;

    addr = HYPHY20G_SGMII_ENET_FEGE_RMAC_REG_STN_ADR_MID_WD_CFG;
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_STN_ADR_MID_WD_CFG_STA_MSK, midSA);
    if(result) return result;

    addr = HYPHY20G_SGMII_ENET_FEGE_RMAC_REG_STN_ADR_LOW_WD_CFG;
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_STN_ADR_LOW_WD_CFG_STA_MSK, lowSA);
    if(result) return result;

    /* Write Station Address to TMAC */
    addr = HYPHY20G_SGMII_ENET_FEGE_TMAC_REG_STN_ADR_HI_WD_CFG;
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_STN_ADR_HI_WD_CFG_STA_MSK, hiSA);
    if(result) return result;

    addr = HYPHY20G_SGMII_ENET_FEGE_TMAC_REG_STN_ADR_MID_WD_CFG;
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_STN_ADR_MID_WD_CFG_STA_MSK, midSA);
    if(result) return result;

    addr = HYPHY20G_SGMII_ENET_FEGE_TMAC_REG_STN_ADR_LOW_WD_CFG;
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_STN_ADR_LOW_WD_CFG_STA_MSK, lowSA);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortFeGeSetStationAddr */

/*******************************************************************************
**
**  hyPhy20gMgmtPortFeGeSetFrameSizes
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
**                  hyPhy20gMgmtPortInit but prior to
**                  hyPhy20gMgmtPortDataPathEnable
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
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortFeGeSetFrameSizes(struct file *fileHndl,
                                       UINT4 txMaxFrameSize,
                                       UINT4 txMinFrameSize,
                                       UINT4 rxMaxFrameSize,
                                       UINT4 rxMinFrameSize)
{
    INT4 result;
    UINT4 addr;

    /* argument checking */
    if (rxMaxFrameSize > HYPHY20G_SGMII_ENET_FEGE_RXMAXFRMSIZE_UPPERLT ||
        txMinFrameSize + 4 > txMaxFrameSize ||
        txMaxFrameSize > HYPHY20G_SGMII_ENET_FEGE_TXMAXFRMSIZE_UPPERLT - 1) {
		return HYPHY20G_ERR_INVALID_ARG;
    }

    if (txMinFrameSize < HYPHY20G_SGMII_ENET_FEGE_TXMINFRMSIZE_LOWERLT) {
        DBG_PRINT("%s, %s, %d, strongly recommend that txMinFrameSize set larger than 0x16. Otherwise It may bring unspecified effects.\n",__FILE__, __FUNCTION__, __LINE__);
	}

    /* Write max/min frame size to RMAC */
    addr = HYPHY20G_SGMII_ENET_FEGE_RMAC_REG_MAX_FRM_LEN_CFG;
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_MAX_FRM_LEN_CFG_MAX_FRM_LEN_MSK,
                rxMaxFrameSize);
    if(result) return result;

    addr = HYPHY20G_SGMII_ENET_FEGE_RMAC_REG_CFG_2;
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_MIN_FRM_LEN_MSK,
                rxMinFrameSize);
    if(result) return result;

    /* Write max and min frame size to TMAC */
    addr = HYPHY20G_SGMII_ENET_FEGE_TMAC_REG_MAX_FRM_LEN_CFG;
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_STN_ADR_LOW_WD_CFG_STA_MSK,
                txMaxFrameSize);
    if(result) return result;

    addr = HYPHY20G_SGMII_ENET_FEGE_TMAC_REG_MIN_FRM_LEN_CFG;
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_MAX_FRM_LEN_CFG_MAX_FRM_LEN_MSK,
                txMinFrameSize);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortFeGeSetFrameSizes */


/*******************************************************************************
**
**  hyPhy20gMgmtQueueInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the EMPEI settings related to the
**                  management port.  These settings impact the data associated
**                  with all EMPEI slices for LSCI and HSCI subsystems.
**
**                  This function configures the VLAN TPID value , that is
**                  to be inserted into the packets received from any of the
**                  Management Extraction Queues for transfer to the Tx
**                  management port.
**
**                  The VLAN TPID is inserted in the 13th and 14th bytes of
**                  the Ethernet frame.
**
**                  Standard VLAN ID value is 0x8100, and other values can be
**                  configured.
**
**                  This function provides configuration of FCS_REMOVE and
**                  VLAN_REMOVE.  These fields control removal of the FCS and the
**                  four byte VLAN TAG received from the Rx management port.
**
**                  For normal operation it is assumed that the Rx mamagement port
**                  is configured to strip the received FCS. Set CRC_STRIP_EN
**                   = 1 in the RMAC.
**                  For normal operation, set FCS_REMOVE = 0 and VLAN_REMOVE = 1
**                  These fields may be varied for debug testing.
**
**  INPUTS:         fileHndl  - device handle
**                  vlanTpid  - VLAN ID to be inserted into the packets from
**                              the Management Extraction Queue.
**                              legal range 0 - 0xFFFF
**                  fcsStrip  - Control of FCS_REMOVE bit (see Description)
**                              0 - Do not remove FCS
**                              1 - Remove FCS
**                              Should be set to 0 for proper operation.
**                  vlanStrip - Control of VLAN_REMOVE bit (see Description)
**                              0 - Do not remove VLAN TAG
**                              1 - Remove VLAN TAG
**                              Should be set to 1 for proper operation.
**
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtQueueInit(struct file *fileHndl, UINT4 vlanTpid,
                           UINT4 fcsStrip, UINT4 vlanStrip)
{
    UINT4 regAddr = HYPHY20G_ENET_COM_EMPEI_REG_MGMT_ETH_FRM_CFG;
    UINT4 val;
    UINT4 tpidMsk = HYPHY20G_ENET_COM_EMPEI_BIT_MGMT_ETH_FRM_CFG_VLAN_TPID_MSK;
    UINT4 fcsRemMsk = HYPHY20G_ENET_COM_EMPEI_BIT_MGMT_ETH_FRM_CFG_FCS_REMOVE_MSK;
    UINT4 vlanRemMsk = HYPHY20G_ENET_COM_EMPEI_BIT_MGMT_ETH_FRM_CFG_VLAN_REMOVE_MSK;
    INT4 result;

    /* argument checking */
    if ((vlanTpid > 0xFFFF) || (fcsStrip > 1) || (vlanStrip > 1)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = sysHyPhy20gRead(fileHndl, regAddr, &val);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, tpidMsk, vlanTpid);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, fcsRemMsk, fcsStrip);
    if(result) return result;

    result = hyPhy20gVarFieldWrite (&val, vlanRemMsk, vlanStrip);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gMgmtQueueInit */


/*******************************************************************************
**
**  hyPhy20gFeGeMgmtChnlQueueInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function Initializes management insertion
**                  and extraction queues in a EMPEI FEGE slice.
**
**                  Management extraction queue stores packets extracted from
**                  the FEGE port and to be sent to Tx management port.
**
**                  Management high priority and low priority insertion queues
**                  store packets received from Rx management port to be
**                  inserted to a FEGE port.
**
**                  This function configures the High and Low Priority VLAN TCI
**                  values associated with the EMPEI FEGE slice. These are
**                  used to identify which VLAN TCI values are transferred to
**                  the high and low priority insertion queues and which VLAN
**                  TCI value is to be appended to data transferred out of
**                  the extraction queue.
**
**                  It configures which of these VLAN TCI values are to be
**                  inserted into bytes 15 and 16 of packets transferred from
**                  a management extraction queue.
**
**                  It also configures whether a new FCS is appended to packets
**                  transferred out of the extraction queue.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - EMPEI slice associated with the LSCI
**                              slice number 0 - 15
**                  fcsAppend - append or don't append FCS to data from
**                              extraction queue
**                              0 - don't append FCS if the management link's
**                                  Tx MAC appends the FCS to the frame
**                              1 - append FCS if management link's Tx MAC
**                                  expects the frame to have a valid FCS
**                  hiTci     - user defined TCI value. Used to:
**                              - identify data to be transferred to low
**                                priority insertion queue.
**                              - append to data transferred from extraction
**                                queue (based on extQueTci setting)
**                              legal range: lowTci[15:12] = 0000 and
**                                           0x000 < lowTci[11:0] < 0xFFF
**                  lowTci    - user defined TCI value. Used to:
**                              - identify data to be transferred to low
**                                priority insertion queue.
**                              - append to data transferred from extraction
**                                queue (based on extQueTci setting)
**                              legal range: lowTci[15:12] = 0000 and
**                                           0x000 < lowTci[11:0] < 0xFFF
**                  extQueTci - Specifies which VLAN TCI value is appended to
**                              packets transferred from extraction queue to
**                              Tx management port
**                              0 - use lowTci value
**                              1 - use hiTci value
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gFeGeMgmtChnlQueueInit(struct file *fileHndl, UINT4 slice,
                                   UINT4 fcsAppend, UINT4 hiTci, UINT4 lowTci,
                                   UINT4 extQueTci)
{
    UINT4 extQCfgAddr, insQCfg2Addr;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 15 || fcsAppend > 1 || hiTci > 0xFFFF || lowTci > 0xFFFF ||
        extQueTci > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    extQCfgAddr = HYPHY20G_ENET_COM_EMPEI_REG_FEGE_SLC_N_MGMT_EXT_QUE_CFG_OFFSET(slice);
    insQCfg2Addr = HYPHY20G_ENET_COM_EMPEI_REG_FEGE_SLC_N_MGMT_INS_QUE_CFG_2_OFFSET(slice);

    /* init extraction queue */
    result = sysHyPhy20gRead(fileHndl, extQCfgAddr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_MGMT_EXT_QUE_CFG_VLAN_INSERT_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_MGMT_EXT_QUE_CFG_VLAN_INSERT_OFF);
    val = mHYPHY20G_SET_FIELD4(val, extQueTci,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_MGMT_EXT_QUE_CFG_VLAN_PRIORITY_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_MGMT_EXT_QUE_CFG_VLAN_PRIORITY_OFF);
    val = mHYPHY20G_SET_FIELD4(val, fcsAppend,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_MGMT_EXT_QUE_CFG_FCS_APPEND_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_MGMT_EXT_QUE_CFG_FCS_APPEND_OFF);

    result = sysHyPhy20gWrite(fileHndl, extQCfgAddr, val);
    if(result) return result;

    /* init high/low priority TCI value used by both
       extraction and insertion queue */
    result = sysHyPhy20gRead(fileHndl, insQCfg2Addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, hiTci,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_MGMT_INS_QUE_CFG_2_VLAN_HI_TCI_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_MGMT_INS_QUE_CFG_2_VLAN_HI_TCI_OFF);

    val = mHYPHY20G_SET_FIELD4(val, lowTci,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_MGMT_INS_QUE_CFG_2_VLAN_LOW_TCI_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_MGMT_INS_QUE_CFG_2_VLAN_LOW_TCI_OFF);

    result = sysHyPhy20gWrite(fileHndl, insQCfg2Addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gFeGeMgmtChnlQueueInit */


/*******************************************************************************
**
**  hyPhy20gFeGeMgmtChnlExtQueuePktEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures if a type of packets is extracted
**                  from functional FEGE data stream (these packets will be sent
**                  to management port through extraction queue).
**
**                  LACP, Marker or packets matching a specified VLAN TAG may
**                  be extracted.
**
**                  This function makes call to FW command that performs the
**                  required configuration of LSCI:ECLASS and EMPEI.
**
**                  This function can be called to enable packet extraction
**                  following hyPhy20gFeGeMgmtEmpeiChnlQueueInit.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - ECLASS/EMPEI slice associated with the LSCI
**                              slice number 0 - 15
**                  pktType   - Management Packet Type to be configured:
**                                  0 - LACP
**                                  1 - Marker
**                                  2 - VLAN tagged
**                  vlanTpid  - VLAN TPID value for pktType = 2
**                                  Legal range:
**                                  0x0 to 0xFFFF
**                              Field not processed when enable = 0 or
**                              pktType = 0 or pktType = 1
**                  vlanTci   - VLAN TCI value for pktType = 2
**                                  Legal range:
**                                  0x0 to 0xFFFF
**                              Field not processed when enable = 0 or
**                              pktType = 0 or pktType = 1
**                  enable    - LED Blinking Control
**                                  0: Disable LED Blinking Feature for port
**                                  1: Enable LED Blinking Feature for port
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gFeGeMgmtChnlExtQueuePktEnable(struct file *fileHndl, UINT4 slice,
                                           UINT4 pktType, UINT4 vlanTpid,
                                           UINT4 vlanTci, UINT4 enable)
{
    INT4 result;

    /* argument checking */
    if (slice > 15 || pktType > 0x2 || vlanTpid > 0xFFFF ||
        vlanTpid > 0xFFFF || enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = hyPhy20gHostMsgCmdCfgEnetMpFilterSetEnetMpFilterSetup(fileHndl, 1,
                                                                   slice,
                                                                   pktType,
                                                                   vlanTpid,
                                                                   vlanTci,
                                                                   enable);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gFeGeMgmtChnlExtQueuePktEnable */

/*******************************************************************************
**
**  hyPhy20gFeGeMgmtChnlInsQueueEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enable or disable packets insertion into the
**                  high or low priority insertion queues.
**
**                  The high and low priority insertion queues transfer data
**                  from the Rx management port to the transmit FEGE slice
**                  data stream.
**
**                  When enabled packets matching the tciHi or tciLow values
**                  configured in hyPhy20gFeGeMgmtEmpeiChnlQueueInit are
**                  transferred into the transmit FEGE slice data stream
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - EMPEI slice associated with the LSCI
**                              slice number 0 - 15
**                  insQueue  - insertion queue to be enabled or disabled:
**                              0 - low priority insertion queue
**                              1 - high priority insertion queue
**                  enable    - control of enabling queue insertion
**                              0 - disabled - no packets inserted into
**                                  queue specified by insQueue
**                              1 - enable - packets inserted into queue
**                                  specified by insQueue
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gFeGeMgmtChnlInsQueueEnable(struct file *fileHndl, UINT4 slice,
                                        UINT4 insQueue, UINT4 enable)
{
    UINT4 insQCfg1Addr, msk;
    INT4 result;

    /* argument checking */
    if (slice > 15 || insQueue > 1 || enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    insQCfg1Addr = HYPHY20G_ENET_COM_EMPEI_REG_FEGE_SLC_N_MGMT_INS_QUE_CFG_1_OFFSET(slice);

    switch (insQueue) {
        case 0: /* Low Priority Insertion Queue */
            msk = HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_MGMT_INS_QUE_CFG_1_VLAN_LOW_MATCH_MSK;
            break;
        case 1: /* High Priority Insertion Queue */
            msk = HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_MGMT_INS_QUE_CFG_1_VLAN_HI_MATCH_MSK;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* enable/disable insertion queue */
    result = hyPhy20gRegFieldWrite(fileHndl, insQCfg1Addr, msk, enable);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gFeGeMgmtChnlInsQueueEnable */


/*******************************************************************************
**
**  hyPhy20g10GeMgmtChnlQueueInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function Initializes management insertion
**                  and extraction queues in a EMPEI 10GE slice.
**
**                  Management extraction queue stores packets extracted from
**                  the 10GE port and to be sent to Tx management port.
**
**                  Management high priority and low priority insertion queues
**                  store packets received from Rx management port to be
**                  inserted to a 10GE port.
**
**                  This function configures the High and Low Priority VLAN TCI
**                  values associated with the EMPEI 10GE slice. These are
**                  used to identify which VLAN TCI values are transferred to
**                  the high and low priority insertion queues and which VLAN
**                  TCI value is to be appended to data transferred out of
**                  the extraction queue.
**
**                  It configures which of these VLAN TCI values are to be
**                  inserted into bytes 15 and 16 of packets transferred from
**                  a management extraction queue.
**
**                  It also configures whether a new FCS is appended to packets
**                  transferred out of the extraction queue.
**
**                  It also configures whether 8 bytes custom preamble,
**                  7 bytes of Preamble(0x55) and 1 byte of SFD(0xD5), are
**                  prepended to packets transferred from Rx management port
**                  to both High and Low priority insertion queues.
**
**                  Similarly, it configures whether 8 bytes custom preamble
**                  are stripped from from packets transferred from extraction
**                  queue to Tx management port.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - EMPEI slice associated with the HSCI
**                              slice number 0 - 1
**                  fcsAppend - append or don't append FCS to data from
**                              extraction queue
**                              0 - don't append FCS if the management link's
**                                  Tx MAC appends the FCS to the frame
**                              1 - append FCS if management link's Tx MAC
**                                  expects the frame to have a valid FCS
**                  hiTci     - user defined TCI value. Used to:
**                              - identify data to be transferred to low
**                                priority insertion queue.
**                              - append to data transferred from extraction
**                                queue (based on extQueTci setting)
**                              legal range: lowTci[15:12] = 0000 and
**                                           0x000 < lowTci[11:0] < 0xFFF
**                  lowTci    - user defined TCI value. Used to:
**                              - identify data to be transferred to low
**                                priority insertion queue.
**                              - append to data transferred from extraction
**                                queue (based on extQueTci setting)
**                              legal range: lowTci[15:12] = 0000 and
**                                           0x000 < lowTci[11:0] < 0xFFF
**                  extQueTci - Specifies which VLAN TCI value is appended to
**                              packets transferred from extraction queue to
**                              Tx management port
**                              0 - use lowTci value
**                              1 - use hiTci value
**                  prmbAdd   - add or don't add 8 bytes custom preamble to
**                              packets transferred from Rx management port to
**                              both High and Low priority insertion queues.
**                              0 - don't prepend custom preamble
**                              1 - prepend 8 bytes custom preamble
**                  prmbStrip - strip or don't strip 8 bytes custom preamble
**                              from packets transferred from extraction queue
**                              to Tx management port
**                              0 - don't strip custom preamble
**                              1 - strip 8 bytes custom preamble
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20g10GeMgmtChnlQueueInit(struct file *fileHndl, UINT4 slice,
                                   UINT4 fcsAppend, UINT4 hiTci, UINT4 lowTci,
                                   UINT4 extQueTci, UINT4 prmbAdd,
                                   UINT4 prmbStrip)
{
    INT4 result;
    UINT4 extQCfgAddr, insQCfg2Addr;
    UINT4 prmbAddAddr, prmbAddOff;
    UINT4 val;

    /* argument checking */
    if (slice > 1 || fcsAppend > 1 || hiTci > 0xFFFF || lowTci > 0xFFFF ||
        extQueTci > 1 || prmbAdd > 1 || prmbStrip > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    extQCfgAddr = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_MGMT_EXT_QUE_CFG_OFFSET(slice);
    insQCfg2Addr = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_MGMT_INS_QUE_CFG_2_OFFSET(slice);

    /* init extraction queue */
    result = sysHyPhy20gRead(fileHndl, extQCfgAddr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, prmbStrip,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_EXT_QUE_CFG_MGMT_EQ_PREAMBLE_STRIP_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_EXT_QUE_CFG_MGMT_EQ_PREAMBLE_STRIP_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 1,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_EXT_QUE_CFG_VLAN_INSERT_10GE_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_EXT_QUE_CFG_VLAN_INSERT_10GE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, extQueTci,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_EXT_QUE_CFG_VLAN_PRIORITY_10GE_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_EXT_QUE_CFG_VLAN_PRIORITY_10GE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, fcsAppend,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_EXT_QUE_CFG_FCS_APPEND_10GE_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_EXT_QUE_CFG_FCS_APPEND_10GE_OFF);

    result = sysHyPhy20gWrite(fileHndl, extQCfgAddr, val);
    if(result) return result;

    /* init high/low priority TCI value used by both
       extraction and insertion queue */
    result = sysHyPhy20gRead(fileHndl, insQCfg2Addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, hiTci,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_INS_QUE_CFG_2_VLAN_HI_TCI_10GE_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_INS_QUE_CFG_2_VLAN_HI_TCI_10GE_OFF);

    val = mHYPHY20G_SET_FIELD4(val, lowTci,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_INS_QUE_CFG_2_VLAN_LOW_TCI_10GE_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_INS_QUE_CFG_2_VLAN_LOW_TCI_10GE_OFF);

    result = sysHyPhy20gWrite(fileHndl, insQCfg2Addr, val);
    if(result) return result;

    /* init preamble prepend behavior of both High and Low priority
       insertion queues. */
    prmbAddAddr = HYPHY20G_ENET_COM_EMPEI_REG_MGMT_IQ_ADD_PREAMBLE_10GE;
    prmbAddOff  = HYPHY20G_ENET_COM_EMPEI_BIT_MGMT_IQ_ADD_PREAMBLE_10GE_MGMT_IQ_PREAMBLE_ADD_10GE_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, prmbAddAddr, prmbAddOff, prmbAdd);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20g10GeMgmtChnlQueueInit */

/*******************************************************************************
**
**  hyPhy20g10GeMgmtChnlExtQueuePktEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures if a type of packets is extracted
**                  from functional 10GE data stream (these packets will be sent
**                  to management port through extraction queue).
**
**                  LACP, Marker or packets matching a specified VLAN TAG may
**                  be extracted.
**
**                  This function makes call to FW command that performs the
**                  required configuration of HSCI:ECLASS and EMPEI.
**
**                  This function can be called to enable packet extraction
**                  following hyPhy20g10GeMgmtEmpeiChnlQueueInit.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - ECLASS/EMPEI slice associated with the HSCI
**                              slice number 0 - 1
**                  pktType   - Management Packet Type to be configured:
**                                  0 - LACP
**                                  1 - Marker
**                                  2 - VLAN tagged
**                  vlanTpid  - VLAN TPID value for pktType = 2
**                                  Legal range:
**                                  0x0 to 0xFFFF
**                              Field not processed when enable = 0 or
**                              pktType = 0 or pktType = 1
**                  vlanTci   - VLAN TCI value for pktType = 2
**                                  Legal range:
**                                  0x0 to 0xFFFF
**                              Field not processed when enable = 0 or
**                              pktType = 0 or pktType = 1
**                  enable    - LED Blinking Control
**                                  0: Disable LED Blinking Feature for port
**                                  1: Enable LED Blinking Feature for port
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_MEM_ALLOC
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_STATUS_CHECK_MISMATCH
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_OUTBOUND_MAILBOX_TIMEOUT
**                  HYPHY20G_ERR_RSP_CFG
**                  HYPHY20G_ERR_RSP_NACK
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20g10GeMgmtChnlExtQueuePktEnable(struct file *fileHndl, UINT4 slice,
                                           UINT4 pktType, UINT4 vlanTpid,
                                           UINT4 vlanTci, UINT4 enable)
{
    INT4 result;

    /* argument checking */
    if (slice > 1 || pktType > 0x2 || vlanTpid > 0xFFFF || vlanTpid > 0xFFFF ||
        enable > 1 ) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = hyPhy20gHostMsgCmdCfgEnetMpFilterSetEnetMpFilterSetup(fileHndl, 0,
                                                                   slice,
                                                                   pktType,
                                                                   vlanTpid,
                                                                   vlanTci,
                                                                   enable);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20g10GeMgmtChnlExtQueuePktEnable */

/*******************************************************************************
**
**  hyPhy20g10GeMgmtChnlInsQueueEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enable or disable packets insertion into the
**                  high or low priority insertion queues.
**
**                  The high and low priority insertion queues transfer data
**                  from the Rx management port to the transmit 10GE slice
**                  data stream.
**
**                  When enabled packets matching the tciHi or tciLow values
**                  configured in hyPhy20gFeGeMgmtEmpeiChnlQueueInit are
**                  transferred into the transmit FEGE slice data stream
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - EMPEI slice associated with the HSCI
**                              slice number 0 - 1
**                  insQueue  - insertion queue to be enabled or disabled:
**                              0 - low priority insertion queue
**                              1 - high priority insertion queue
**                  enable    - control of enabling queue insertion
**                              0 - disabled - no packets inserted into
**                                  queue specified by insQueue
**                              1 - enable - packets inserted into queue
**                                  specified by insQueue
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20g10GeMgmtChnlInsQueueEnable(struct file *fileHndl, UINT4 slice,
                                        UINT4 insQueue, UINT4 enable)
{
    UINT4 insQCfg1Addr, msk;
    INT4 result;

    /* argument checking */
    if (slice > 1 || insQueue > 1 || enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    insQCfg1Addr = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_MGMT_INS_QUE_CFG_1_OFFSET(slice);

    switch (insQueue) {
        case 0: /* Low Priority Insertion Queue */
            msk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_INS_QUE_CFG_1_VLAN_LOW_MATCH_10GE_MSK;
            break;
        case 1: /* High Priority Insertion Queue */
            msk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_MGMT_INS_QUE_CFG_1_VLAN_HI_MATCH_10GE_MSK;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* enable/disable insertion queue */
    result = hyPhy20gRegFieldWrite(fileHndl, insQCfg1Addr, msk, enable);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20g10GeMgmtChnlInsQueueEnable */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/*******************************************************************************
**
**  mgmtHyPhy20gFeGeBlockInit
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
**                  enable    - 1 - enable the FEGE block
**                              0 - disable the FEGE block
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 mgmtHyPhy20gFeGeBlockInit(struct file *fileHndl, UINT4 enable)
{
    INT4 result;
    UINT4 addr, val;

    /* argument checking */
    if (enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* put the ENET_FEGE block in reset */
    addr = HYPHY20G_SGMII_TOP_REG_CFG;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_SGMII_TOP_BIT_CFG_ENET_FEGE_RST_OFF, 1);
    if(result) return result;

    if (enable == HYPHY20G_DISABLE) {           /* disable operations */
        return HYPHY20G_SUCCESS;
    }

    /*
     * enable operations
     */
    /* remove the ENET_FEGE block */
    addr = HYPHY20G_SGMII_TOP_REG_CFG;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_SGMII_TOP_BIT_CFG_ENET_FEGE_RST_OFF, 0);
    if(result) return result;

    /* remove sub-block reset */
    addr = HYPHY20G_SGMII_ENET_FEGE_TOP_REG_SW_RST;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* remove the reset to the useful sub-blocks for other modes */
    val = mHYPHY20G_SET_FIELD4(val, 0x500, (0x07FF << 0), 0);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /*
     * configure ENET_FEGE Top Level register
     */
    addr = HYPHY20G_SGMII_ENET_FEGE_TOP_REG_MD_CFG;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 0x3,
            HYPHY20G_SGMII_ENET_FEGE_TOP_BIT_MD_CFG_IPG_RED_ON_THRESH_MSK,
            HYPHY20G_SGMII_ENET_FEGE_TOP_BIT_MD_CFG_IPG_RED_ON_THRESH_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x1,
            HYPHY20G_SGMII_ENET_FEGE_TOP_BIT_MD_CFG_IPG_RED_OFF_THRESH_MSK,
            HYPHY20G_SGMII_ENET_FEGE_TOP_BIT_MD_CFG_IPG_RED_OFF_THRESH_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x2,
            HYPHY20G_SGMII_ENET_FEGE_TOP_BIT_MD_CFG_PAUSE_ON_THRESH_MSK,
            HYPHY20G_SGMII_ENET_FEGE_TOP_BIT_MD_CFG_PAUSE_ON_THRESH_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x1,
            HYPHY20G_SGMII_ENET_FEGE_TOP_BIT_MD_CFG_PAUSE_OFF_THRESH_MSK,
            HYPHY20G_SGMII_ENET_FEGE_TOP_BIT_MD_CFG_PAUSE_OFF_THRESH_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_SGMII_ENET_FEGE_TOP_BIT_MD_CFG_UNIDIRECTIONAL_MODE_MSK,
            HYPHY20G_SGMII_ENET_FEGE_TOP_BIT_MD_CFG_UNIDIRECTIONAL_MODE_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /*
     * configure L1RPP block
     */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_CFG_AND_STAT;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_AUTO_SENSE_MSK,
            HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_AUTO_SENSE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0,
            HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_L1RPP_EN_MSK,
            HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_CFG_AND_STAT_L1RPP_EN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /*
     * configure RMAC block
     */
    addr = HYPHY20G_SGMII_ENET_FEGE_RMAC_REG_CFG_1;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_INRANGE_DIS_OFF,
                1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_MAX_FRM_TRN_OFF,
                1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_TRANSPARENT_OFF,
                0);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_ETHER_MODE_OFF,
                1);
    if(result) return result;
    result = hyPhy20gRegFieldWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_INRANGE_CK_MSK, 0x3);
    if(result) return result;

    addr = HYPHY20G_SGMII_ENET_FEGE_RMAC_REG_CFG_2;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_INPAUSE_EN_OFF,
                1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_2_PAUSEFRM_DROP_OFF,
                1);
    if(result) return result;

    /*
     * configure TMAC block
     */
    addr = HYPHY20G_SGMII_ENET_FEGE_TMAC_REG_CFG_1;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_IBUF_HTHR_EN_OFF,
                1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_RMAC_PAUSE_EN_OFF,
                1);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_EBUF_ERR_EN_OFF,
                1);
    if(result) return result;

    /* don't enable tx path at this time */
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_TMAC_ENBL_OFF,
                0);
    if(result) return result;

    /*
     * configure L1TPP block
     */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1TPP_REG_CFG;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_SGMII_ENET_FEGE_L1TPP_BIT_CFG_RD_ALIGN_EN_OFF,
                1);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* mgmtHyPhy20gFeGeBlockInit */


/* end of file */

