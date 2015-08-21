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
**  FILE        : hyphy20g_sonet_ln.c
**
**  $Date: 2013-01-09 16:53:11 +0800 (Wed, 09 Jan 2013) $
**
**  $Revision: 30241 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_sonet_ln.h"


/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnStliReset(struct file *fileHndl,
                                     UINT4 slice,
                                     UINT4 reset);
INT4 sonetLnHyPhy20gSonetLnSrliSfbaGrpReset(struct file *fileHndl,
                                            UINT4 group,
                                            UINT4 srliReset,
                                            UINT4 sfbaReset);
INT4 sonetLnHyPhy20gSonetLnSts192Mode(struct file *fileHndl,
                                      UINT4 slice,
                                      UINT4 mode);
INT4 sonetLnHyPhy20gSonetLnSfbaMode(struct file *fileHndl, UINT4 slice,
                                    UINT4 mode);
INT4 sonetLnHyPhy20gSonetLnSfbaReset(struct file *fileHndl, UINT4 slice,
                                     UINT4 reset);
INT4 sonetLnHyPhy20gSonetLnSrliReset(struct file *fileHndl, UINT4 group,
                                            UINT4 srliReset);
INT4 sonetLnHyPhy20gSonetLnCheckTseCtxt(struct file *fileHndl);
INT4 sonetLnHyPhy20gSonetLnToggleSonetClrintSliceBit(struct file *fileHndl,
                                                     UINT4 slice);
INT4 sonetLnHyPhy20gSonetLnRxOtu2SrcCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 src);
INT4 sonetLnHyPhy20gSonetLnRxHsciGfpSrcCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 src, UINT4 timingMd);
INT4 sonetLnHyPhy20gSonetLnWisDemapSrcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 sonetGrp);
INT4 sonetLnHyPhy20gSonetLnRxSonetSlcSrcCfg(struct file *fileHndl, UINT4 slice,
                                            UINT4 rxSrc, UINT4 sfpRate);
INT4 sonetLnHyPhy20gSonetLnRxOtu1SrcCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 rxSrc);
INT4 sonetLnHyPhy20gSonetLnTxXfiSfiSrcCfg(struct file *fileHndl, UINT4 intf,
                                          UINT4 block);
INT4 sonetLnHyPhy20gSonetLnTxHsciClkCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 txClkSrc);
INT4 sonetLnHyPhy20gSonetLnTxHsciBckPrssrCfg(struct file *fileHndl, UINT4 slice,
                                             UINT4 src);
INT4 sonetLnHyPhy20gSonetLnTxWisMapSrcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 txSrc);
INT4 sonetLnHyPhy20gSonetLnTxSonetSlcSrcCfg(struct file *fileHndl, UINT4 slice,
                                            UINT4 txSrc);
INT4 sonetLnHyPhy20gSonetLnTxSonetSlcClkCfg(struct file *fileHndl, UINT4 slice,
                                            UINT4 txClkSrc);
INT4 sonetLnHyPhy20gSonetLnTxOtu2Odu2SysSrcCfg(struct file *fileHndl,
                                               UINT4 slice, UINT4 block,
                                               UINT4 srcSlc);
INT4 sonetLnHyPhy20gSonetLnSts192SysClkCfg(struct file *fileHndl, UINT4 sonetGrp,
                                           UINT4 sysClk);
INT4 sonetLnHyPhy20gSonetLnTxOtu2ClkCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 txClkSrc);
INT4 sonetLnHyPhy20gSonetLnTxOtu1Odu1SysSrcCfg(struct file *fileHndl,
                                               UINT4 slice, UINT4 src);
INT4 sonetLnHyPhy20gSonetLnTxOtu1ClkCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 txClkSrc);
INT4 sonetLnHyPhy20gSonetLnSwizXfiSfiSwToHwVal(struct file *fileHndl, UINT4 swVal,
                                               UINT4 *pHwVal);
INT4 sonetLnHyPhy20gSonetLnSwizXfiSfiHwToSwVal(struct file *fileHndl,
                                               UINT4 hwVal, UINT4 *pSwVal);
INT4 sonetLnHyPhy20gSonetLnTseRxJ0DlyCfg(struct file *fileHndl, UINT4 sonetSlc,
                                         UINT4 sonetRate);
INT4 sonetLnHyPhy20gSonetLnTxSfpSrcCfg(struct file *fileHndl, UINT4 sfpPort,
                                       UINT4 src);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gSonetLnInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the SONET_LINE subsystem so that
**                  it may be further configured to transfer user data.
**
**                  The block is reset to default values each time the function
**                  is called.
**
**  INPUTS:         *fileHndl  - base address
**                  enable     - 0 - SONET_LINE subsystem disabled
**                               1 - SONET_LINE subsystem enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetLnInit(struct file *fileHndl, UINT4 enable)
{
    /* variable declaration */
    INT4 result;

    /* argument checking */
    if(enable > 1){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* unconditionally set SONLI_RST bit to 1 */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_1,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONLI_RST_OFF,
                                 1);
    if(result) return result;

    if (enable == 0) {
        return HYPHY20G_SUCCESS;
    }

    /* if enabling set SONLI_RST bit to 0  */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_1,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_1_SONLI_RST_OFF,
                                 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetLnInit */

/*******************************************************************************
**
**  hyPhy20gSonetLnXfiSfiDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function maps an XFI or SFI interface to SONET, OTN
**                  or HSCI subsystems.
**
**                  It is the responsibility of calling software to ensure
**                  an internal block is not misassigned to more than one
**                  line side data source.
**
**                  When 'block' specifies one of the SONET resources,
**                  hyPhy20gTseInit must be called prior to calling this
**                  function as this function configures TSE frame pulse
**                  offsets.  Also, the CLRINT_SLICE bits of the SONET slices
**                  within the SONET group will be toggled.
**
**                  When 'block' specifies one of the SONET resources and
**                  line-timing mode is selected, hyPhy20gSonetLnLineTimeSrcCfg
**                  should be called prior to this function to configure the
**                  line timing source.
**
**  INPUTS:         *fileHndl  - base address
**                  intf       - interface
**                                0 -  XFI #1
**                                1 -  XFI #2
**                                2 -  SFI #1
**                                3 -  SFI #2
**                  block      - Subsystem resource to process data stream
**                                0 - HSCI slice 0
**                                1 - HSCI slice 1
**                                2 - SONET group 0 (slice 0 to 3)
**                                3 - SONET group 1 (slice 4 to 7)
**                                4 - SONET group 2 (slice 8 to 11)
**                                5 - SONET group 3 (slice 12 to 15)
**                                6 - OTN OTU2 Framer slice 0
**                                7 - OTN OTU2 Framer slice 1
**                  timingMd   - For blocks 2 to 5, specifies timing mode
**                                0 - internal timing (use local reference)
**                                1 - line timing source 0
**                                2 - line timing source 1
**                               Note: hyPhy20gSonetLnLineTimeSrcCfg
**                               configures the line timing source.
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gSonetLnXfiSfiDataPathCfg(struct file *fileHndl, UINT4 intf,
                                      UINT4 block, UINT4 timingMd)
{
    UINT4 sonetGrp, sonetGrpSlc;
    UINT4 otu2Slc;
    INT4 result;

    /* Check input arguments */
    if ((intf > 3) || (block > 7) || (timingMd > 2) ||
        (block == 0 && timingMd == 2) || (block == 1 && timingMd == 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check TSE context */
    if (block >= 2 && block <= 5) {
        result = sonetLnHyPhy20gSonetLnCheckTseCtxt(fileHndl);
        if (result) return result;
    }

    sonetGrp = block-2;
    sonetGrpSlc = (block-2) * 4;
    otu2Slc = block-6;

    if (block <= 1) {
        /* HSCI 0 or 1 */

        /* Select HSCI/GFP rx source */
        result = sonetLnHyPhy20gSonetLnRxHsciGfpSrcCfg(fileHndl, block, intf, 0);
        if (result) return result;

        /* Configure HSCI tx clk */
        result = sonetLnHyPhy20gSonetLnTxHsciClkCfg(fileHndl, block, intf);
        if (result) return result;

        /* Configure HSCI back pressure source */
        result = sonetLnHyPhy20gSonetLnTxHsciBckPrssrCfg(fileHndl, block, 5);
        if (result) return result;

    } else if (block >= 2 && block <= 5) {
        /* SONET group 0 to 3 */

        /* Bring STLI out of reset */
        result = sonetLnHyPhy20gSonetLnStliReset(fileHndl, sonetGrp, 0);
        if (result) return result;

        /* Bring SRLI out of reset, put SFBA into reset */
        result = sonetLnHyPhy20gSonetLnSrliSfbaGrpReset(fileHndl, sonetGrp,
                                                        0, 1);
        if (result) return result;

        /* Set STS-192 mode for SRLI/STLI pair */
        result = sonetLnHyPhy20gSonetLnSts192Mode(fileHndl, sonetGrp, 1);
        if (result) return result;

        /* Select SONET slice rx source */
        result = sonetLnHyPhy20gSonetLnRxSonetSlcSrcCfg(fileHndl, sonetGrpSlc,
                                                        intf, 0);
        if (result) return result;

        /* Configure SONET slice tx clock */
        result = sonetLnHyPhy20gSonetLnTxSonetSlcClkCfg(fileHndl, sonetGrpSlc,
                                                        intf);
        if (result) return result;

        /* Configure STS-192 system clock */
        result = sonetLnHyPhy20gSonetLnSts192SysClkCfg(fileHndl, sonetGrp,
                                                       timingMd);
        if (result) return result;

        /* configure TSE RX_J0DLY for the set of four 2.5G ports
           associated with the STS-192 signal */
        result = sonetLnHyPhy20gSonetLnTseRxJ0DlyCfg(fileHndl, sonetGrpSlc, 3);
        if (result) return result;

    } else {
        /* OTU2 framer 0 and 1 */

        /* Select OTU2 rx source */
        result = sonetLnHyPhy20gSonetLnRxOtu2SrcCfg(fileHndl, otu2Slc, intf);
        if (result) return result;

        /* Configure OTU2 tx clock */
        result = sonetLnHyPhy20gSonetLnTxOtu2ClkCfg(fileHndl, otu2Slc, intf);
        if (result) return result;
    }

    /* Configure XFI and SFI transmit source */
    result = sonetLnHyPhy20gSonetLnTxXfiSfiSrcCfg(fileHndl, intf, block);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetLnXfiSfiDataPathCfg */

/*******************************************************************************
**
**  hyPhy20gSonetLnXfiTxMulticastCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function multicasts the transmit XFI #1 data stream
**                  to XFI #2 for 1+1 protection.  The main use of this
**                  function is to multicast OTU2 traffic.
**
**                  Both XFI SERDES must be programmed to the same rate or be
**                  controlled by the same PMM for proper phase alignment
**                  between the working and protect data streams.
**
**  INPUTS:         *fileHndl  - base address
**                  block      - Subsystem resource to multicast
**                                0 - HSCI slice 0
**                                1 - HSCI slice 1
**                                2 - SONET group 0 (slice 0 to 3)
**                                3 - SONET group 1 (slice 4 to 7)
**                                4 - SONET group 2 (slice 8 to 11)
**                                5 - SONET group 3 (slice 12 to 15)
**                                6 - OTN OTU2 Framer slice 0
**                                7 - OTN OTU2 Framer slice 1
**                                8 - disable multicast transmission
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetLnXfiTxMulticastCfg(struct file *fileHndl, UINT4 block)
{
    UINT4 addr;
    UINT4 tdatSelMsk;
    UINT4 tdatSrc;
    UINT4 txSelVal;
    UINT4 txSel0Msk;
    UINT4 txSel1Msk;
    INT4 result;

    /* Check input arguments */
    if (block > 8) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    tdatSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_TX_SEL_XFI_TDATA1_SEL_MSK;
    txSel0Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_TX_SEL_TX_XFI_SEL0_MSK;
    txSel1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_TX_SEL_TX_XFI_SEL1_MSK;

    if (block <= 1) {
        /* HSCI 0 or 1 */
        txSelVal = block + 2;
    } else if (block >= 2 && block <= 5) {
        /* SONET group 0 to 3 */
        txSelVal = (block-2) + 4;
    } else {
        /* OTU2 framer 0 and 1 */
        txSelVal = block-6;
    }
    
    tdatSrc = (block == 8) ? 0 : 1;
    addr = HYPHY20G_SONET_LINE_TOP_REG_SFI_TX_SEL;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, tdatSelMsk, tdatSrc);
    if (result) return result;

    if (block != 8) {
        result = hyPhy20gRegFieldWrite(fileHndl, addr, txSel0Msk, 0);
        if (result) return result;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, txSel1Msk, txSelVal);
        if (result) return result;
    }    

    if (block == 6) {
        result = sonetLnHyPhy20gSonetLnTxOtu2ClkCfg(fileHndl, 0, 1);
        if (result) return result;
        result = sonetLnHyPhy20gSonetLnTxOtu2ClkCfg(fileHndl, 1, 0);
        if (result) return result;
    }  
    if (block == 7) {
        result = sonetLnHyPhy20gSonetLnTxOtu2ClkCfg(fileHndl, 0, 0);
        if (result) return result;
        result = sonetLnHyPhy20gSonetLnTxOtu2ClkCfg(fileHndl, 1, 1);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetLnXfiTxMulticastCfg */

/*******************************************************************************
**
**  hyPhy20gSonetLnOtu2Odu2SysDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function maps the system side interface of an OTN
**                  OTU2 Framer or ODUKPP ODU2 Framer to SONET subsystem
**                  (for STS-192 processing) or the HSCI subsystem (for 10GE
**                  LAN processing).
**
**                  It will map an OTN OTU2 framer slice to an upstream
**                  processing block for payloads which are not processed by the
**                  ODTU12 subsystem or OPI subsystem.
**
**                  It will map an ODUKPP ODU2 framer slice to an upstream
**                  processing block for payloads which are not processed by the
**                  VCAT subsystem.
**
**                  It is the responsibility of calling software to ensure
**                  an internal block is not misassigned to more than one
**                  line side data source.
**
**                  Before mapping the system side interface of an OTU2/ODU2
**                  framer to an upstream HSCI block, the framer TX
**                  clock must be configured. If the framer TX clock
**                  source changes, system side interface mapping of an OTU2 or
**                  ODU2 framer to an upstream HSCI block must be performed.
**
**                  When 'block' specifies one of the SONET resources,
**                  hyPhy20gTseInit must be called prior to calling this
**                  function as this function configures TSE frame pulse
**                  offsets.  Also, the CLRINT_SLICE bits of the SONET slices
**                  within the SONET group will be toggled.
**
**
**  INPUTS:         *fileHndl  - base address
**                  sysIntf    - OTN OTU2 Framer System Side Interface
**                                0 -  OTN OTU2 Framer slice 0
**                                1 -  OTN OTU2 Framer slice 1
**                                2 -  ODUKPP ODU2 Framer slice 0
**                                3 -  ODUKPP ODU2 Framer slice 1
**                  block      - Subsystem resource to process data stream
**                                0 - HSCI slice 0
**                                1 - HSCI slice 1
**                                2 - SONET group 0 (slice 0 to 3)
**                                3 - SONET group 1 (slice 4 to 7)
**                                4 - SONET group 2 (slice 8 to 11)
**                                5 - SONET group 3 (slice 12 to 15)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gSonetLnOtu2Odu2SysDataPathCfg(struct file *fileHndl, UINT4 sysIntf,
                                           UINT4 block)
{
    UINT4 sonetGrp, sonetGrpSlc, otu2Odu2Slc;
    UINT4 txOtu2LnClkSel;
    INT4 result;

    /* Check input arguments */
    if ((sysIntf > 3) || (block > 5)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check TSE context */
    if (block >= 2 && block <= 5) {
        result = sonetLnHyPhy20gSonetLnCheckTseCtxt(fileHndl);
        if (result) return result;
    }

    sonetGrp = block-2;
    sonetGrpSlc = (block-2)*4;
    otu2Odu2Slc = sysIntf+4;

    if (block <= 1) {
        /* HSCI 0 to 1 */

        /* Configure rx */
        result = sonetLnHyPhy20gSonetLnRxHsciGfpSrcCfg(fileHndl, block,
                                                       otu2Odu2Slc, 0);
        if (result) return result;

        /* Configure tx */
        result = sonetLnHyPhy20gSonetLnTxOtu2Odu2SysSrcCfg(fileHndl, sysIntf, 1,
                                                           block);
        if (result) return result;

        /* Configure HSCI TX clock */
        switch (sysIntf) {
            case 0: /* 0 -  OTN OTU2 Framer slice 0 */
            case 1: /* 1 -  OTN OTU2 Framer slice 1 */
                txOtu2LnClkSel = 4; /* 4      - OTU2 */
                break;
            case 2: /* 2 -  ODUKPP ODU2 Framer slice 0 */
            case 3: /* 3 -  ODUKPP ODU2 Framer slice 1 */
                txOtu2LnClkSel = 5; /* 5      - ODUKPP ODU2 */
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }

        /* Configure HSCI tx clk */
        result = sonetLnHyPhy20gSonetLnTxHsciClkCfg(fileHndl, block,
                                                    txOtu2LnClkSel);
        if (result) return result;

        /* Configure backpressure source for HSCI */
        result = sonetLnHyPhy20gSonetLnTxHsciBckPrssrCfg(fileHndl, block,
                                                         sysIntf);
        if (result) return result;

    } else {
        /* SONET group 0 to 3 */
        /* Bring STLI out of reset */
        result = sonetLnHyPhy20gSonetLnStliReset(fileHndl, sonetGrp, 0);
        if (result) return result;

        /* Bring SRLI out of reset, place SFBA into reset */
        result = sonetLnHyPhy20gSonetLnSrliSfbaGrpReset(fileHndl, sonetGrp,
                                                        0, 1);
        if (result) return result;

        /* Set STS-192 mode for SRLI/STLI pair */
        result = sonetLnHyPhy20gSonetLnSts192Mode(fileHndl, sonetGrp, 1);
        if (result) return result;

        /* Configure rx */
        result = sonetLnHyPhy20gSonetLnRxSonetSlcSrcCfg(fileHndl, sonetGrpSlc,
                                                        otu2Odu2Slc, 0);
        if (result) return result;

        /* Configure tx */
        result = sonetLnHyPhy20gSonetLnTxOtu2Odu2SysSrcCfg(fileHndl, sysIntf,
                                                           0, sonetGrp);
        if (result) return result;

        result = sonetLnHyPhy20gSonetLnTxSonetSlcClkCfg(fileHndl, sonetGrpSlc,
                                                        4);
        if (result) return result;

        /* Configure STS-192 system clock */
        result = sonetLnHyPhy20gSonetLnSts192SysClkCfg(fileHndl, sonetGrp, 0);
        if (result) return result;

        /* configure TSE RX_J0DLY for the set of four 2.5G ports
           associated with the STS-192 signal */
        result = sonetLnHyPhy20gSonetLnTseRxJ0DlyCfg(fileHndl, sonetGrpSlc, 3);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetLnOtu2Odu2SysDataPathCfg */

/*******************************************************************************
**
**  hyPhy20gSonetLnSonetSysDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function maps the system side interface of a group of
**                  four SONET slices that are processing an STS-192 signal to
**                  an upstream HSCI block for termination of 10GE WAN payloads
**                  or to the GFP subsystem for POS mode of operation.
**
**                  This function configures registers in SONET_LINE and SONET
**                  subsystems.
**
**                  hyPhy20gSonetInit must be called before this function in
**                  order for configuration of SONET subsystem to take effect.
**
**                  It is the responsibility of calling software to ensure
**                  an internal block is not misassigned to more than one
**                  line side data source
**
**  INPUTS:         *fileHndl  - base address
**                  sysIntf    - SONET Framer System Side Interface
**                                0 - SONET group 0 (slice 0 to 3)
**                                1 - SONET group 1 (slice 4 to 7)
**                                2 - SONET group 2 (slice 8 to 11)
**                                3 - SONET group 3 (slice 12 to 15)
**                  block      - Subsystem resource to process data stream
**                                0 - HSCI/GFP slice 0
**                                1 - HSCI/GFP slice 1
**                                2 - TSE subsystem
**                  gfpEn      - For block set to 0 or 1, specifies if the
**                               associated subsystem is an HSCI or GFP
**                               slice
**                                0 - HSCI slice processes data stream
**                                1 - GFP slice processes data stream
**                  timingMd   - For blocks 0 to 1, specifies timing mode
**                                0 - internal timing (use local reference)
**                                1 - line timing
**                               Note: hyPhy20gSonetLnLineTimeSrcCfg
**                               configures the line timing source.  Line timing
**                               clocks are one-to-one mapped to an HSCI block.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetLnSonetSysDataPathCfg(struct file *fileHndl, UINT4 sysIntf,
                                        UINT4 block, UINT4 gfpEn,
                                        UINT4 timingMd)
{
    UINT4 addr, msk;
    UINT4 sonetGrpSlc;
    UINT4 txSrcSelVal;
    UINT4 clkSrc;
    UINT4 i;
    INT4 result;

    /* Check input arguments */
    if ((sysIntf > 3) || (block > 2) || (gfpEn > 1) || (timingMd > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    sonetGrpSlc = sysIntf * 4;

    if (block <= 1) {
        /* HSCI 0 or 1 */

        /* Configure rx */
        result = sonetLnHyPhy20gSonetLnRxHsciGfpSrcCfg(fileHndl, block,
                                                       sysIntf+8, timingMd);
        if (result) return result;

        result = sonetLnHyPhy20gSonetLnWisDemapSrcCfg(fileHndl, block, sysIntf);
        if (result) return result;

        /* Configure tx */
        clkSrc = (timingMd == 0) ? 7 : 6;
        result = sonetLnHyPhy20gSonetLnTxHsciClkCfg(fileHndl, block, clkSrc);
        if (result) return result;

        result = sonetLnHyPhy20gSonetLnTxHsciBckPrssrCfg(fileHndl, block, 4);
        if (result) return result;

        result = sonetLnHyPhy20gSonetLnTxWisMapSrcCfg(fileHndl, block, gfpEn);
        if (result) return result;
		
        /* According to the prep 215232
        result = sonetLnHyPhy20gSonetLnTxSonetSlcClkCfg(fileHndl, sonetGrpSlc,
                                                        4);
        if (result) return result;
        */

        result = sonetLnHyPhy20gSonetLnTxSonetSlcSrcCfg(fileHndl, sysIntf,
                                                        block);
        if (result) return result;

        txSrcSelVal = 1;

    } else {
        /* TSE */
        txSrcSelVal = 0;
    }

    /* configure TX_SOURCE_SEL to select between HSCI and TSE interfaces
       for each slice associated with the STS-192 */
    for (i=(sysIntf*4); i < (4 + (sysIntf*4)); i++) {
        addr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_CTL;
        addr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(addr, i);
        msk = HYPHY20G_SONET_TOP_BIT_SDH_PROC_SLC_CTL_TX_SOURCE_SEL_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, txSrcSelVal);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetLnSonetSysDataPathCfg */

/*******************************************************************************
**
**  hyPhy20gSonetLnXfiMonDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function maps an XFI or SFI interface to the ingress
**                  path of the SONET, OTN or HSCI subsystems.
**
**                  This function is required when a subsystem is required to
**                  monitor the received signal in parallel to the internal
**                  transfer of the signal.  For example OC-192 to OTU2
**                  application takes an OC-192 signal from an XFI or SFI
**                  interface and transparently maps it to ODU2 through the
**                  following data path:
**
**                  XFI/SFI <-> HSCI (FC/CBR) <-> CPB <-> GFP/VCAT (OTNMA/D) <->
**                  CBRB <-> OTU2 Framer <-> XFI/SFI
**
**                  As the OC-192 is asynchronously transported there is no
**                  processing of the OC-192 overhead columns.  This function
**                  allows the XFI/SFI port associated with the OC-192 signal
**                  to be processed by a SONET slice group so that overhead
**                  can be monitored.
**
**                  This function does not set up any egress data path
**                  associated with the interface or processing block.
**
**                  It is the responsibility of calling software to ensure
**                  an internal block is not misassigned to more than one
**                  line side data source
**
**                  When 'block' specifies one of the SONET resources,
**                  hyPhy20gTseInit must be called prior to calling this
**                  function as this function configures TSE frame pulse
**                  offsets.  Also, the CLRINT_SLICE bits of the SONET slices
**                  within the SONET group will be toggled.
**
**
**  INPUTS:         *fileHndl  - base address
**                  intf       - interface
**                                0 -  XFI #1
**                                1 -  XFI #2
**                                2 -  SFI #1
**                                3 -  SFI #2
**                  block      - Subsystem resource to monitor data stream
**                                0 - HSCI slice 0
**                                1 - HSCI slice 1
**                                2 - SONET group 0 (slice 0 to 3)
**                                3 - SONET group 1 (slice 4 to 7)
**                                4 - SONET group 2 (slice 8 to 11)
**                                5 - SONET group 3 (slice 12 to 15)
**                                6 - OTN OTU2 Framer slice 0
**                                7 - OTN OTU2 Framer slice 1
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gSonetLnXfiMonDataPathCfg(struct file *fileHndl, UINT4 intf,
                                      UINT4 block)
{
    UINT4 sonetGrp, sonetGrpSlc;
    UINT4 otu2Slc;
    INT4 result;

    /* Check input arguments */
    if ((intf > 3) || (block > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check TSE context */
    if (block >= 2 && block <= 5) {
        result = sonetLnHyPhy20gSonetLnCheckTseCtxt(fileHndl);
        if (result) return result;
    }

    sonetGrp = block-2;
    sonetGrpSlc = (block-2) * 4;

    if (block <= 1) {
        /* HSCI 0 or 1 */

        /* Select HSCI/GFP rx source */
        result = sonetLnHyPhy20gSonetLnRxHsciGfpSrcCfg(fileHndl, block, intf, 0);
        if (result) return result;

    } else if (block >= 2 && block <= 5) {
        /* SONET group 0 to 3 */

        /* Bring STLI out of reset */
        result = sonetLnHyPhy20gSonetLnStliReset(fileHndl, sonetGrp, 0);
        if (result) return result;

        /* Bring SRLI out of reset, put SFBA into reset */
        result = sonetLnHyPhy20gSonetLnSrliSfbaGrpReset(fileHndl, sonetGrp,
                                                        0, 1);
        if (result) return result;

        /* Set STS-192 mode for SRLI/STLI pair */
        result = sonetLnHyPhy20gSonetLnSts192Mode(fileHndl, sonetGrp, 1);
        if (result) return result;

        /* Select SONET slice rx source */
        result = sonetLnHyPhy20gSonetLnRxSonetSlcSrcCfg(fileHndl, sonetGrpSlc,
                                                        intf, 0);
        if (result) return result;

        /* configure TSE RX_J0DLY for the set of four 2.5G ports
           associated with the STS-192 signal */
        result = sonetLnHyPhy20gSonetLnTseRxJ0DlyCfg(fileHndl, sonetGrpSlc, 3);
        if (result) return result;

    } else {
        /* OTU2 framer 0 and 1 */
        otu2Slc = block-6;

        /* Select OTU2 rx source */
        result = sonetLnHyPhy20gSonetLnRxOtu2SrcCfg(fileHndl, otu2Slc, intf);
        if (result) return result;

    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetLnXfiMonDataPathCfg */

/*******************************************************************************
**
**  hyPhy20gSonetLnSfpDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function maps an SFP interface to SONET, OTN
**                  or LSCI subsystems.
**
**                  Any SFP port can be mapped to any OTU1 FRAMER or SONET
**                  slice.  SFP ports can only be 1:1 mapped to a companion
**                  LSCI slice.
**
**                  It is the responsibility of calling software to ensure
**                  an internal block is not misassigned to more than one
**                  line side data source.
**
**                  When 'block' specifies one of the SONET resources,
**                  hyPhy20gTseInit must be called prior to calling this
**                  function as this function configures TSE frame pulse
**                  offsets.  Also, the CLRINT_SLICE bit of the SONET slice
**                  will be toggled.
**
**  INPUTS:         *fileHndl  - base address
**                  intf       - interface
**                                0 to 15 - SFP ports 0 to 15
**                  block      - Subsystem resource to process data stream
**                                 0 to 15 - SONET slice 0 to 15
**                                16 to 23 - OTU1 Framer slice 0 to 7
**                                24 - LSCI slice with the same index as intf
**                                     parameter
**                  sonetRate  - SONET line rate (if block is greater than 15,
**                                                use 0)
**                                  0 - STS-48
**                                  1 - STS-12
**                                  2 - STS-3
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gSonetLnSfpDataPathCfg(struct file *fileHndl, UINT4 intf,
                                   UINT4 block, UINT4 sonetRate)
{
    UINT4 devId, revId, status;
    UINT4 reg, msk, val;
    UINT4 sonetGrp, otu1Slc, sfbaMode;
    UINT4 sfbaOc3Oc12RegState, sliceInOc3Oc12State, sfbaOc3Oc12State;
    UINT4 src, i;
    INT4 result;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Check input arguments */
    if ((intf > 15) || (block > 24) || (sonetRate > 2) ||
        (block > 15 && sonetRate != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check TSE context */
    if (block >= 0 && block <= 15) {
        result = sonetLnHyPhy20gSonetLnCheckTseCtxt(fileHndl);
        if (result) return result;
    }

    sonetGrp = block/4;
    otu1Slc = block-16;

    if (block >= 0 && block <= 15) {
        /* SONET Slices */

        /* Configure SRLI/STLI/SFBA based on STS-48/12/3 */
        result = sonetLnHyPhy20gSonetLnSts192Mode(fileHndl, sonetGrp, 0);
        if (result) return result;
        if (sonetRate == 0) {
            /* STS-48 */
            result = sonetLnHyPhy20gSonetLnStliReset(fileHndl, sonetGrp, 0);
            if (result) return result;

            result = sonetLnHyPhy20gSonetLnSrliReset(fileHndl, sonetGrp, 0);
            if (result) return result;

            result = sonetLnHyPhy20gSonetLnSfbaReset(fileHndl, block, 1);
            if (result) return result;

        } else {
            /* STS-12 or STS-3 */
            result = sonetLnHyPhy20gSonetLnStliReset(fileHndl, sonetGrp, 0);
            if (result) return result;

            result = sonetLnHyPhy20gSonetLnSfbaReset(fileHndl, block, 0);
            if (result) return result;

            /* Reset SRLI only if all other SFBAs are active */
            reg = HYPHY20G_SONET_LINE_TOP_REG_SRLI_SFBA_SW_RST;
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SRLI_SFBA_SW_RST_SFBA_RST_MSK;
            result = hyPhy20gRegFieldRead(fileHndl, reg, msk, &val);
            if (result) return result;

            sfbaOc3Oc12RegState = ~val;
            sliceInOc3Oc12State = 1;
            for (i = sonetGrp*4; i < (sonetGrp*4) + 4; i++) {
                sfbaOc3Oc12State = (sfbaOc3Oc12RegState >> i) & 0x1;
                if (!sfbaOc3Oc12State) {
                    sliceInOc3Oc12State = 0;
                }
            }
            if (sliceInOc3Oc12State) {
                result = sonetLnHyPhy20gSonetLnSrliReset(fileHndl, sonetGrp, 1);
                if (result) return result;
            }

            /* Configure SFBA mode */
            sfbaMode = (sonetRate == 1) ? 1 : 0;
            result = sonetLnHyPhy20gSonetLnSfbaMode(fileHndl, block, sfbaMode);
            if (result) return result;
        }

        /* configure rx */
        src = intf + 8;
        result = sonetLnHyPhy20gSonetLnRxSonetSlcSrcCfg(fileHndl, block, src,
                                                        sonetRate);
        if (result) return result;

        /* configure TSE RX_J0DLY */
        result = sonetLnHyPhy20gSonetLnTseRxJ0DlyCfg(fileHndl, block, sonetRate);
        if (result) return result;

        /* configure tx */
        src = intf + 5;
        result = sonetLnHyPhy20gSonetLnTxSonetSlcClkCfg(fileHndl, block, src);
        if (result) return result;

    } else if (block >= 16 && block <= 23) {
        /* OTU1 Framers */

        /* Configure rx */
        result = sonetLnHyPhy20gSonetLnRxOtu1SrcCfg(fileHndl, otu1Slc, intf);
        if (result) return result;

        /* Configure tx */
        result = sonetLnHyPhy20gSonetLnTxOtu1ClkCfg(fileHndl, otu1Slc, intf);
        if (result) return result;

    } else {
        /* LSCI - supports 1:1 mapping only */
        /* nothing to do here */
    }

    /* Configure SFP tx source */
    result = sonetLnHyPhy20gSonetLnTxSfpSrcCfg(fileHndl, intf, block);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetLnSfpDataPathCfg */

/*******************************************************************************
**
**  hyPhy20gSonetLnSfpMonDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function maps an SFP interface to the ingerss path of
**                  the SONET, OTN or LSCI subsystems.
**
**                  Any SFP port can be mapped to any OTU1 FRAMER or SONET
**                  slice.  SFP ports can only be 1:1 mapped to a companion
**                  LSCI slice.
**
**                  This function is required to allow a subsystem to monitor
**                  the data flow for a signal that would otherwise be passed
**                  through HyPHY 20G as a transparent client.  An example would
**                  be an SFP port with an STS-48 signal that is asynchronously
**                  mapped to OTU1 in HyPHY 20G and transported on another
**                  SFP port.  In this application, the STS-48 signal is
**                  transferred through FC/CBR block in order to be mapped to
**                  OTU1 (via CPB and GFP (OTNMA/D) to  CBRB to OTU1 Framer).
**                  This function allow a SONET slice block to receive the
**                  STS-48 signal so that transport overhead can be monitored.
**
**                  It is the responsibility of calling software to ensure
**                  an internal block is not misassigned to more than one
**                  line side data source
**
**                  When 'block' specifies one of the SONET resources,
**                  hyPhy20gTseInit must be called prior to calling this
**                  function as this function configures TSE frame pulse
**                  offsets.  Also, the CLRINT_SLICE bit of the SONET slice
**                  will be toggled.
**
**
**  INPUTS:         *fileHndl  - base address
**                  intf       - interface
**                                0 to 15 - SFP ports 0 to 15
**                  block      - Subsystem resource to process data stream
**                                 0 to 15 - SONET slice 0 to 15
**                                16 to 23 - OTU1 Framer slice 0 to 7
**                                24 - LSCI slice with the same index as intf
**                                     parameter
**                  sonetRate  - SONET line rate (if block is greater than 15,
**                                                use 0)
**                                  0 - STS-48
**                                  1 - STS-12
**                                  2 - STS-3
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gSonetLnSfpMonDataPathCfg(struct file *fileHndl, UINT4 intf,
                                      UINT4 block, UINT4 sonetRate)
{
    UINT4 devId, revId, status;
    UINT4 reg, msk, val;
    UINT4 sonetGrp, otu1Slc, sfbaMode;
    UINT4 sfbaOc3Oc12RegState, sliceInOc3Oc12State, sfbaOc3Oc12State;
    UINT4 src, i;
    INT4 result;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Check input arguments */
    if ((intf > 15) || (block > 24) || (sonetRate > 2) ||
        (block > 15 && sonetRate != 0)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check TSE context */
    if (block >= 0 && block <= 15) {
        result = sonetLnHyPhy20gSonetLnCheckTseCtxt(fileHndl);
        if (result) return result;
    }

    sonetGrp = block/4;
    otu1Slc = block-16;

    if (block >= 0 && block <= 15) {
        /* SONET Slices */

        /* Configure SRLI/STLI/SFBA based on STS-48/12/3 */
        result = sonetLnHyPhy20gSonetLnSts192Mode(fileHndl, sonetGrp, 0);
        if (result) return result;
        if (sonetRate == 0) {
            /* STS-48 */
            result = sonetLnHyPhy20gSonetLnStliReset(fileHndl, sonetGrp, 0);
            if (result) return result;

            result = sonetLnHyPhy20gSonetLnSrliReset(fileHndl, sonetGrp, 0);
            if (result) return result;

            result = sonetLnHyPhy20gSonetLnSfbaReset(fileHndl, block, 1);
            if (result) return result;

        } else {
            /* STS-12 or STS-3 */
            result = sonetLnHyPhy20gSonetLnStliReset(fileHndl, sonetGrp, 0);
            if (result) return result;

            result = sonetLnHyPhy20gSonetLnSfbaReset(fileHndl, block, 0);
            if (result) return result;

            /* Reset SRLI only if all other SFBAs are active */
            reg = HYPHY20G_SONET_LINE_TOP_REG_SRLI_SFBA_SW_RST;
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SRLI_SFBA_SW_RST_SFBA_RST_MSK;
            result = hyPhy20gRegFieldRead(fileHndl, reg, msk, &val);
            if (result) return result;

            sfbaOc3Oc12RegState = ~val;
            sliceInOc3Oc12State = 1;
            for (i = sonetGrp*4; i < (sonetGrp*4) + 4; i++) {
                sfbaOc3Oc12State = (sfbaOc3Oc12RegState >> i) & 0x1;
                if (!sfbaOc3Oc12State) {
                    sliceInOc3Oc12State = 0;
                }
            }
            if (sliceInOc3Oc12State) {
                result = sonetLnHyPhy20gSonetLnSrliReset(fileHndl, sonetGrp, 1);
                if (result) return result;
            }

            /* Configure SFBA mode */
            sfbaMode = (sonetRate == 1) ? 1 : 0;
            result = sonetLnHyPhy20gSonetLnSfbaMode(fileHndl, block, sfbaMode);
            if (result) return result;
        }

        /* configure rx */
        src = intf + 8;
        result = sonetLnHyPhy20gSonetLnRxSonetSlcSrcCfg(fileHndl, block, src,
                                                        sonetRate);
        if (result) return result;

        /* configure TSE RX_J0DLY */
        result = sonetLnHyPhy20gSonetLnTseRxJ0DlyCfg(fileHndl, block, sonetRate);
        if (result) return result;

    } else if (block >= 16 && block <= 23) {
        /* OTU Framers */

        /* Configure rx */
        result = sonetLnHyPhy20gSonetLnRxOtu1SrcCfg(fileHndl, otu1Slc, intf);
        if (result) return result;

    } else {
        /* LSCI - supports 1:1 mapping only */
        /* nothing to do here */
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetLnSfpMonDataPathCfg */

/*******************************************************************************
**
**  hyPhy20gSonetLnOtu1Odu1SysDataPathCfgEx
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function maps the system side interface of an OTN
**                  OTU1 Framer slice or an ODUKPP ODU1 Framer slice to an
**                  upstream SONET block for STS-48 payloads.
**
**                  It is the responsibility of calling software to ensure
**                  an internal block is not misassigned to more than one
**                  line side data source
**
**                  When 'block' specifies one of the SONET resources,
**                  hyPhy20gTseInit must be called prior to calling this
**                  function as this function configures TSE frame pulse
**                  offsets.  Also, the CLRINT_SLICE bit of the SONET slice
**                  will be toggled.
**
**
**  INPUTS:         *fileHndl  - base address
**                  sysIntf    - OTN OTU1 Framer System Side Interface
**                                0 to 7  - OTN OTU1 Framer slice 0 to 7
**                                8 to 15 - OTN ODU1 Framer slice 0 to 7
**                  block      - Subsystem resource to process data stream
**                                 0 to 15 - SONET slice  0 to 15
**
**                  txClkSrc  - Transmit clock source of SONET slice
**                                0 to 1  - XFI 1 to 2
**                                2 to 3  - SFI 1 to 2
**                                4       - CLK311
**                                5 to 20 - SFP 0 to 15
**                  srliReset   - SRLI reset mode
**                                  0 - SRLI core operates normally
**                                  1 - SRLI core logic held in reset
**                  sfbaReset   - SFBA group reset mode
**                                  0 - SFBA core operates normally
**                                  1 - SFBA core logic held in reset
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gSonetLnOtu1Odu1SysDataPathCfgEx(struct file *fileHndl, UINT4 sysIntf,
                   UINT4 block,UINT4 txClkSrc,UINT4 srliReset, UINT4 sfbaReset)
{
    UINT4 devId, revId, status;
    UINT4 sonetGrp, src;
    INT4 result;

    if ((txClkSrc > 20) || ((txClkSrc <= 3) &&
        (block != 0 && block != 4 && block != 8 && block != 12))) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if( srliReset>1 || sfbaReset >1 )
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Check input arguments */
    if ((sysIntf > 15) || (block > 15)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check TSE context */
    result = sonetLnHyPhy20gSonetLnCheckTseCtxt(fileHndl);
    if (result) return result;

    sonetGrp = block/4;

    /* Configure SRLI/STLI/SFBA for STS-48 rate */
    result = sonetLnHyPhy20gSonetLnSts192Mode(fileHndl, sonetGrp, 0);
    if (result) return result;

    result = sonetLnHyPhy20gSonetLnStliReset(fileHndl, sonetGrp, 0);
    if (result) return result;

    result = sonetLnHyPhy20gSonetLnSrliSfbaGrpReset(fileHndl, sonetGrp, srliReset, sfbaReset);
    if (result) return result;

    /* Configure rx */
    src = sysIntf+24;
    result = sonetLnHyPhy20gSonetLnRxSonetSlcSrcCfg(fileHndl, block, src, 0);
    if (result) return result;

    result = sonetLnHyPhy20gSonetLnTseRxJ0DlyCfg(fileHndl, block, 0);
    if (result) return result;

    /* Configure tx */
    result = sonetLnHyPhy20gSonetLnTxSonetSlcClkCfg(fileHndl, block, txClkSrc);
    if (result) return result;

    result = sonetLnHyPhy20gSonetLnTxOtu1Odu1SysSrcCfg(fileHndl, sysIntf, block);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetLnOtu1SysDataPathCfg */

/*******************************************************************************
**
**  hyPhy20gSonetLnOtu1Odu1SysDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    
**                  This function maps the system side interface of an OTN
**                  OTU1 Framer slice or an ODUKPP ODU1 Framer slice to an
**                  upstream SONET block for STS-48 payloads.
**
**                  It is the responsibility of calling software to ensure
**                  an internal block is not misassigned to more than one
**                  line side data source
**
**                  When 'block' specifies one of the SONET resources,
**                  hyPhy20gTseInit must be called prior to calling this
**                  function as this function configures TSE frame pulse
**                  offsets.  Also, the CLRINT_SLICE bit of the SONET slice
**                  will be toggled.
**
**
**  INPUTS:         *fileHndl  - base address
**                  sysIntf    - OTN OTU1 Framer System Side Interface
**                                0 to 7  - OTN OTU1 Framer slice 0 to 7
**                                8 to 15 - OTN ODU1 Framer slice 0 to 7
**                  block      - Subsystem resource to process data stream
**                                 0 to 15 - SONET slice  0 to 15
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/         
INT4 hyPhy20gSonetLnOtu1Odu1SysDataPathCfg(struct file *fileHndl, UINT4 sysIntf,
                                           UINT4 block)
{
    return hyPhy20gSonetLnOtu1Odu1SysDataPathCfgEx(fileHndl,sysIntf,block,4,0,1);
}

/*******************************************************************************
**
**  hyPhy20gSonetLnXfiSrcCSel
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the XFI ACB block Source C input.
**
**                  For cases where payload is synchronously demapped from
**                  OTNMD to transfer data to an HSCI slice, the OPI VCAT
**                  interface should be used as the source C input.  In this
**                  case no dithered packets will be generated in HSCI and
**                  all rate information related to the client will be captured
**                  in the CBRB data stream.  The ACB CNT_N and CNT_D fields will
**                  scale the rate indications from the ODU2/2e/2f/1e/1f
**                  packetized ODUk timing data to the client rate.
**
**                  For cases where payload is asynchronously demapped from
**                  OTNMD the HSCI PMM must be used for sources C and the OPI
**                  VCAT interface PMM used for source B input.  In this case
**                  dithered packets will be present in both CBRB and CPB
**                  subsystems that provide timing data for the client.
**
**
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - XFI Instance
**                                    0 - XFI #0
**                                    1 - XFI #1
**                  source          - Packet Monitor block providing data to
**                                    source C of XFI ACB
**                                    0 - OPI OTU2 Framer 0 Interface
**                                        OPI PMM Address 8
**                                    1 - OPI OTU2 Framer 1 Interface
**                                        OPI PMM Address 9
**                                    2 - HSCI Slice 0
**                                    3 - HSCI Slice 1
**                                    4 - ENET_COM
**                                    5 - OPI VCAT Interface 0
**                                        OPI PMM Address 10
**                                    6 - OPI VCAT Interface 1
**                                        OPI PMM Address 14
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
INT4 hyPhy20gSonetLnXfiSrcCSel(struct file *fileHndl, UINT4 slice, UINT4 source)
{
    /* variable declaration */
    UINT4 clkCtlReg = HYPHY20G_SONET_LINE_TOP_REG_XFI_CLK_CTL;
    UINT4 ckcltcMsk =
                    HYPHY20G_SONET_LINE_TOP_BIT_XFI_CLK_CTL_XFI_CKCTLC_SEL0_MSK;
    INT4 result;

    /* argument checking */
    if((slice > 1)||(source > 6)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if(slice){
        ckcltcMsk = HYPHY20G_SONET_LINE_TOP_BIT_XFI_CLK_CTL_XFI_CKCTLC_SEL1_MSK;
    }

    /* configure the XFI_CKCTLC_SELn bit for XFI port bit with the specified
       source */
    result = hyPhy20gRegFieldWrite (fileHndl, clkCtlReg, ckcltcMsk, source);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetLnXfiSrcCSel */

/*******************************************************************************
**
**  hyPhy20gSonetLnXfiSrcBSel
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function selects the XFI ACB block Source B input.
**
**                  Source B is only used in addition to an XFI ACB that has
**                  a valid Source C input.
**
**                  The usage of source B and source C also applies to CBR
**                  clients that are:
**                  - GFP demapped in GFP-T FA mode or GFP-F
**                     mode with an 8 bit dthered packet CBR stream
**                  - asynchronously demapped
**                    STS-192 from ODU2
**
**                  It is invalid to use an OPI VCAT Inteface PMM for both
**                  a source B and source C input simultaneously.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - XFI Instance
**                                    0 - XFI #0
**                                    1 - XFI #1
**                  source          - Packet Monitor block providing data to
**                                    source B of XFI ACB
**                                    0 - OPI VCAT 0 Interface
**                                        OPI PMM Address 10
**                                    1 - OPI VCAT 1 Interface
**                                        OPI PMM Address 14
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
INT4 hyPhy20gSonetLnXfiSrcBSel(struct file *fileHndl, UINT4 slice, UINT4 source)
{
    /* variable declaration */
    UINT4 clkCtlReg = HYPHY20G_SONET_LINE_TOP_REG_XFI_CLK_CTL;
    UINT4 ckcltbMsk =
                    HYPHY20G_SONET_LINE_TOP_BIT_XFI_CLK_CTL_XFI_CKCTLB_SEL0_MSK;
    INT4 result;

    /* argument checking */
    if((slice > 1)||(source > 1)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if(slice){
        ckcltbMsk = HYPHY20G_SONET_LINE_TOP_BIT_XFI_CLK_CTL_XFI_CKCTLB_SEL1_MSK;
    }

    /* configure the XFI_CKCTLB_SELn bit for XFI port bit with the specified
       source */
    result = hyPhy20gRegFieldWrite (fileHndl, clkCtlReg, ckcltbMsk, source);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetLnXfiSrcBSel */

/*******************************************************************************
**
**  hyPhy20gSonetLnSfpSrcCSel
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the SFP ACB block Source C input.
**
**                  The SFP ACB Source C input may come from any of the PMM
**                  blocks associated with the 8 OTU1 framers, the ENET_COM
**                  block or to the LSCI PMM that has the same slice index
**                  as the SFP ACB being configured.
**
**                  It may also come from the PMM blocks in the OPI subsystem
**                  associated with the VCAT interface.  This is only valid
**                  for STS-48 synchronously demapped from ODU1.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - SFP ACB slice
**                                    0 to 15
**                  source          - Packet Monitor block providing data to
**                                    source C of SFP ACB
**                                    0 to 7 - OPI OTU1 Framer PMM 0 to 7
**                                             OPI PMM Address 0 to 7
**                                    8 - LSCI PMM for the LSCI with the same
**                                        address index as SFP 'slice' number
**                                    9 - ENET_COM PMM
**                                    10 to 17 - OPI VCAT Interface 0/1 PMM
**                                               OPI PMM Address 10 to 17
**
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetLnSfpSrcCSel(struct file *fileHndl, UINT4 slice, UINT4 source)
{
    /* variable declaration */
    UINT4 devId, revId, status;
    UINT4 ckctlcCtrReg;
    UINT4 ckctlcSelOff;
    UINT4 ckctlcMsk;
    UINT4 addr;
    UINT4 msk;
    UINT4 txMuxSelVal;
    INT4 result;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 15)||(source > 17)){
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure SERDES TX_CKCTL_MUX_SEL field */
    if (source !=8) {
        /* OPI or ENET_COM --> TX_CKCTL_MUX_SEL must be configured to 0 */
        txMuxSelVal = 0;
    } else {
        /* LSCI --> TX_CKCTL_MUX_SEL must be configured to 1 */
        txMuxSelVal = 1;
    }
    addr = HYPHY20G_SERDES_SFP_TOP_REG_CFG_1;
    addr = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_SERDES_SFP_TOP_BIT_CFG_1_TX_CKCTL_MUX_SEL_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, txMuxSelVal);
    if (result) return result;

    if(slice > 9){
        ckctlcCtrReg = HYPHY20G_SONET_LINE_TOP_REG_SFP_CLK_CTL_1;
        ckctlcSelOff = 5*(slice - 10);
        ckctlcMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_CLK_CTL_1_SFP_CKCTLC_SEL10_MSK;
        ckctlcMsk = ckctlcMsk << ckctlcSelOff;
    } else if (slice < 4) {
        ckctlcCtrReg = HYPHY20G_SONET_LINE_TOP_REG_XFI_CLK_CTL;
        ckctlcSelOff = (5*slice);
        ckctlcMsk = HYPHY20G_SONET_LINE_TOP_BIT_XFI_CLK_CTL_SFP_CKCTLC_SEL0_MSK;
        ckctlcMsk = ckctlcMsk << ckctlcSelOff;
    } else {
        ckctlcCtrReg = HYPHY20G_SONET_LINE_TOP_REG_SFP_CLK_CTL_0;
        ckctlcSelOff = 5*(slice - 4);
        ckctlcMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_CLK_CTL_0_SFP_CKCTLC_SEL4_MSK;
        ckctlcMsk = ckctlcMsk << ckctlcSelOff;
    }

    /* configure the CKCTLC_SEL_n bit for SFP port bit with the specified
       source */
    result = hyPhy20gRegFieldWrite (fileHndl, ckctlcCtrReg, ckctlcMsk, source);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetLnSfpSrcCSel */

/*******************************************************************************
**
**  hyPhy20gSonetLnSfpSrcBSel
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the SFP ACB block Source B input.
**
**                  The SFP ACB Source B input may come from any of the PMM
**                  blocks associated with the OPI VCAT interface.
**
**  INPUTS:         fileHndl        - device handle
**                  slice           - SFP ACB slice
**                                    0 to 15
**                  source          - Packet Monitor block providing data to
**                                    source B of SFP ACB
**                                    0 to 7 - OPI VCAT Interface PMM 0 to 7
**                                             OPI PMM Address 10 to 17
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetLnSfpSrcBSel(struct file *fileHndl, UINT4 slice, UINT4 source)
{
    /* variable declaration */
    UINT4 devId, revId, status;
    UINT4 ckctlbCtrReg = HYPHY20G_SONET_LINE_TOP_REG_SFP_CKCTLB_0;
    UINT4 ckctlbMsk;
    INT4 result;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if((slice > 15)||(source > 7)){
        return HYPHY20G_ERR_INVALID_ARG;
    }

    ckctlbMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_CKCTLB_0_SFP_CKCTLB_SEL0_MSK <<
                (3*slice);

    if(slice > 9){
        ckctlbCtrReg = HYPHY20G_SONET_LINE_TOP_REG_SFP_CKCTLB_1;
        ckctlbMsk =
                 HYPHY20G_SONET_LINE_TOP_BIT_SFP_CKCTLB_0_SFP_CKCTLB_SEL0_MSK <<
                 (3*(slice-10));
    }

    /* configure the CKCTLB_SEL_n bit for SFP port bit with the specified
       source */
    result = hyPhy20gRegFieldWrite (fileHndl, ckctlbCtrReg, ckctlbMsk, source);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gSonetLnSfpSrcBSel */

/*******************************************************************************
**
**  hyPhy20gSonetLnLineTimeSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures which STS-192 receive clock will
**                  be used as the line timing source for one of the two
**                  line timing clocks that can be selected for the transmit
**                  STS-192 signal.
**
**                  Selection of transmit timing source used by a set of
**                  SONET slices processing an STS-192 is configured with
**                  hyPhy20gSonetLnXfiSfiDataPathCfg.
**
**  INPUTS:         *fileHndl  - base address
**                  lineTmClk  - Line timing clock available for transmit
**                               STS-192 signals from SONET subsystem.
**                               Legal range 0 to 1
**                  sonetGrp   - SONET receive clock feeding line timing clock
**                                0 - SONET group 0 (slice 0 to 3)
**                                1 - SONET group 1 (slice 4 to 7)
**                                2 - SONET group 2 (slice 8 to 11)
**                                3 - SONET group 3 (slice 12 to 15)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gSonetLnLineTimeSrcCfg(struct file *fileHndl, UINT4 lineTmClk,
                                   UINT4 sonetGrp)
{
    UINT4 reg, msk;
    INT4 result;

    /* argument checking */
    if (lineTmClk > 1 || sonetGrp > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* System clock SYSCLK_OC192[n] */
    msk = (lineTmClk == 0) ?
          HYPHY20G_SONET_LINE_TOP_BIT_GEFC_TX_SEL_CLK_OC192_SEL0_MSK :
          HYPHY20G_SONET_LINE_TOP_BIT_GEFC_TX_SEL_CLK_OC192_SEL1_MSK;
    reg = HYPHY20G_SONET_LINE_TOP_REG_GEFC_TX_SEL;

    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, sonetGrp);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gSonetLnLineTimeSrcCfg */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnStliReset
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the reset bit of the STLI.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - STLI slice
**                                  0 - STLI for SONET group 0 (slice 0 to 3)
**                                  1 - STLI for SONET group 1 (slice 4 to 7)
**                                  2 - STLI for SONET group 2 (slice 8 to 11)
**                                  3 - STLI for SONET group 3 (slice 12 to 15)
**                  reset       - reset mode
**                                  0 - STLI core operates normally
**                                  1 - STLI core logic held in reset
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnStliReset(struct file *fileHndl, UINT4 slice,
                                     UINT4 reset)
{
    UINT4 bitLoc;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* Check input arguments */
    if ((slice > 3) || (reset > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    bitLoc = HYPHY20G_SONET_LINE_TOP_BIT_TX_SW_RST_STLI_RST_OFF + slice;

    result = sysHyPhy20gBitWrite (fileHndl,
                                  HYPHY20G_SONET_LINE_TOP_REG_TX_SW_RST, bitLoc,
                                  reset);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* sonetLnHyPhy20gSonetLnStliReset */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnSrliSfbaGrpReset
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the reset bit of the SRLI and
**                  SFBAs based on the SONET group specified.
**
**  INPUTS:         *fileHndl   - base address
**                  group       - SRLI/SFBA group
**                                  0 - SRLI/SFBA for SONET group 0
**                                      (slice 0 to 3)
**                                  1 - SRLI/SFBA for SONET group 1
**                                      (slice 4 to 7)
**                                  2 - SRLI/SFBA for SONET group 2
**                                      (slice 8 to 11)
**                                  3 - SRLI/SFBA for SONET group 3
**                                      (slice 12 to 15)
**                  srliReset   - SRLI reset mode
**                                  0 - SRLI core operates normally
**                                  1 - SRLI core logic held in reset
**                  sfbaReset   - SFBA group reset mode
**                                  0 - SFBA core operates normally
**                                  1 - SFBA core logic held in reset
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnSrliSfbaGrpReset(struct file *fileHndl, UINT4 group,
                                            UINT4 srliReset, UINT4 sfbaReset)
{
    UINT4 addr;
    UINT4 val;
    UINT4 mask;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* Check input arguments */
    if ((group > 3) || (srliReset > 1) || (sfbaReset > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Read register */
    addr = HYPHY20G_SONET_LINE_TOP_REG_SRLI_SFBA_SW_RST;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    /* Construct mask */
    mask = (0x1 << group);
    mask |= ((0xF << (group*4)) <<
            HYPHY20G_SONET_LINE_TOP_BIT_SRLI_SFBA_SW_RST_SFBA_RST_OFF);

    /* Clear bits */
    val &= ~mask;

    /* Configure reset bits */
    if (srliReset == 1) {
        // Place SRLI in reset
        val |= (0x1 << group);
    }
    if (sfbaReset == 1) {
        // Place SFBA in reset
        val |= ((0xF << (group*4)) <<
               HYPHY20G_SONET_LINE_TOP_BIT_SRLI_SFBA_SW_RST_SFBA_RST_OFF);
    }
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* sonetLnHyPhy20gSonetLnSrliSfbaGrpReset */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnSrliReset
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the reset bit of the SRLI
**                  based on the SONET group specified.
**
**  INPUTS:         *fileHndl   - base address
**                  group       - SRLI group
**                                  0 - SRLI for SONET group 0 (slice 0 to 3)
**                                  1 - SRLI for SONET group 1 (slice 4 to 7)
**                                  2 - SRLI for SONET group 2 (slice 8 to 11)
**                                  3 - SRLI for SONET group 3 (slice 12 to 15)
**                  srliReset   - reset mode
**                                  0 - SRLI core operates normally
**                                  1 - SRLI core logic held in reset
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnSrliReset(struct file *fileHndl, UINT4 group,
                                            UINT4 srliReset)
{
    UINT4 addr;
    UINT4 val;
    UINT4 mask;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* Check input arguments */
    if ((group > 3) || (srliReset > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Read register */
    addr = HYPHY20G_SONET_LINE_TOP_REG_SRLI_SFBA_SW_RST;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    /* Construct mask */
    mask = 0x1 <<
            (HYPHY20G_SONET_LINE_TOP_BIT_SRLI_SFBA_SW_RST_SRLI_RST_OFF + group);

    /* Clear bits */
    val &= ~mask;

    /* Configure reset bits */
    if (srliReset == 1) {
        // Place SRLI in reset
        val |= (0x1 <<
           (HYPHY20G_SONET_LINE_TOP_BIT_SRLI_SFBA_SW_RST_SRLI_RST_OFF + group));
    }

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* sonetLnHyPhy20gSonetLnSrliReset */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnSts192Mode
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function sets the SRLI/STLI pair in either
**                  STS-48 or STS-192 mode.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - SRLI/STLI slice
**                                  0 - SRLI/STLI for SONET group 0
**                                      (slice 0 to 3)
**                                  1 - SRLI/STLI for SONET group 1
**                                      (slice 4 to 7)
**                                  2 - SRLI/STLI for SONET group 2
**                                      (slice 8 to 11)
**                                  3 - SRLI/STLI for SONET group 3
**                                      (slice 12 to 15)
**                  mode        - SRLI/STLI mode
**                                  0 - 4 x STS-48/STS-12/STS-3 mode
**                                  1 - STS-192 mode
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnSts192Mode(struct file *fileHndl, UINT4 slice,
                                      UINT4 mode)
{
    UINT4 bitLoc;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* Check input arguments */
    if ((slice > 3) || (mode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    bitLoc = HYPHY20G_SONET_LINE_TOP_BIT_STS192_MD_SEL_STS192_OFF + slice;

    result = sysHyPhy20gBitWrite (fileHndl,
                                  HYPHY20G_SONET_LINE_TOP_REG_STS192_MD_SEL,
                                  bitLoc, mode);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* sonetLnHyPhy20gSonetLnSts192Mode */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnSfbaReset
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the reset bit of the SFBA.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - SFBA slice 0 to 15
**                  reset       - reset mode
**                                  0 - SFBA core operates normally
**                                  1 - SFBA core held in reset
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnSfbaReset(struct file *fileHndl, UINT4 slice,
                                     UINT4 reset)
{
    UINT4 addr;
    UINT4 val;
    UINT4 mask;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* Check input arguments */
    if ((slice > 15) || (reset > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Read register */
    addr = HYPHY20G_SONET_LINE_TOP_REG_SRLI_SFBA_SW_RST;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    /* Construct mask */
    mask = 0x1 <<
            (HYPHY20G_SONET_LINE_TOP_BIT_SRLI_SFBA_SW_RST_SFBA_RST_OFF + slice);

    /* Clear bit */
    val &= ~mask;

    /* Configure reset bit */
    if (reset == 1) {
        // Place SFBA in reset
        val |= 0x1 <<
            (HYPHY20G_SONET_LINE_TOP_BIT_SRLI_SFBA_SW_RST_SFBA_RST_OFF + slice);
    }
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* sonetLnHyPhy20gSonetLnSfbaReset */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnSfbaMode
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the SFBA to be in either
**                  STS-12 or STS-3 mode.
**
**                  If an SFBA's mode is changed, the CLRINT_SLICE bit of the
**                  corresponding SONET slice must be toggled.  Toggling of
**                  CLRINT_SLICE can be done with the function
**                  sonetLnHyPhy20gSonetLnToggleSonetClrintSliceBit.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - SFBA slice 0 to 15
**                  mode        - SFBA mode
**                                  0 - STS-3 mode
**                                  1 - STS-12 mode
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnSfbaMode(struct file *fileHndl, UINT4 slice,
                                    UINT4 mode)
{
    UINT4 bitLoc;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* Check input arguments */
    if ((slice > 15) || (mode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    bitLoc = HYPHY20G_SONET_LINE_TOP_BIT_SFBA_MD_SEL_RX_RX_SFBA_MODE_SEL_OFF
             + slice;

    result = sysHyPhy20gBitWrite (fileHndl,
                                  HYPHY20G_SONET_LINE_TOP_REG_SFBA_MD_SEL_RX,
                                  bitLoc, mode);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* sonetLnHyPhy20gSonetLnSfbaMode */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnCheckTseCtxt
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function checks for TSE initialization status.  The
**                  status of the TSE context is used to determine if the
**                  TSE subsystem has been initialized.
**
**  INPUTS:         *fileHndl   - base address
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnCheckTseCtxt(struct file *fileHndl)
{
    sHYPHY20G_CTXT *pDevCtxt;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* Get device context */
    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* Confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL),
                    HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt or pDevCtxt->pCpbCtxt is NULL.");

    /* Check for initialization of TSE context */
    if (pDevCtxt->pTseCtxt == NULL){
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    return HYPHY20G_SUCCESS;

} /* sonetLnHyPhy20gSonetLnCheckTseCtxt */

/*******************************************************************************
**
**  utilHyPhy20gSonetLnGetSignalRate
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function checks the configured signal rate for the
**                  specified SONET slice within the SONET LINE.
**
**  INPUTS:         *fileHndl - base address
**                  slice     - sonet slice (0 to 15)
**                  *rate     - returns the configured rate of the specified
**                              sonet slice within the sonet_line subsystem
**                                0 - STS-192
**                                1 - STS-48
**                                2 - STS-12
**                                3 - STS-3
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gSonetLnGetSignalRate(struct file *fileHndl, UINT4 slice,
                                      UINT4 *rate)
{
    UINT4 addr;
    UINT4 val;
    UINT4 bitVal;
    UINT4 sonetGrp;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* check arguments */
    if (slice > 15) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* read RX_OUTPUT_MUX_SEL field to determine if the slice rate is STS-192/48
       or STS-12/3 */
    addr = HYPHY20G_SONET_LINE_TOP_REG_STS_MD_SEL_RX;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    bitVal = (val >>
             (HYPHY20G_SONET_LINE_TOP_BIT_STS_MD_SEL_RX_RX_OUTPUT_MUX_SEL_OFF +
              slice)) & 0x1;

    if (bitVal == 0) {
        /* slice is configured for STS-12/3 */

        /* read RX_SFBA_MODE_SEL field to determine if the slice rate is
           STS-12 or STS-3 */
        addr = HYPHY20G_SONET_LINE_TOP_REG_SFBA_MD_SEL_RX;
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if(result) return result;

        bitVal = (val >>
                (HYPHY20G_SONET_LINE_TOP_BIT_SFBA_MD_SEL_RX_RX_SFBA_MODE_SEL_OFF
                 + slice)) & 0x1;

        if (bitVal == 0) {
            /* STS-3 */
            *rate = 3;
        } else {
            /* STS-12 */
            *rate = 2;
        }

    } else {
        /* slice is configured for STS-192/48 */
        /* read STS192 field to determine if the slice rate is STS-192 or
           STS-48 */
        addr = HYPHY20G_SONET_LINE_TOP_REG_STS192_MD_SEL;
        result = sysHyPhy20gRead(fileHndl, addr, &val);
        if(result) return result;

        sonetGrp = slice/4;
        bitVal = (val >>
                 (HYPHY20G_SONET_LINE_TOP_BIT_STS192_MD_SEL_STS192_OFF +
                  sonetGrp)) & 0x1;

        if (bitVal == 1) {
            /* STS-192 */
            *rate = 0;
        } else {
            /* STS-48 */
            *rate = 1;
        }
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gSonetLnGetSignalRate */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnToggleSonetClrintSliceBit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function toggles the CLRINT_SLICE bit in the SONET
**                  subsystem for the specified slice.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - sonet slice 0 to 15
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnToggleSonetClrintSliceBit(struct file *fileHndl,
                                                     UINT4 slice)
{
    UINT4 regAddr;
    UINT4 msk;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* Check input arguments */
    if (slice > 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    regAddr = HYPHY20G_SONET_TOP_REG_SDH_PROC_SLC_CFG1;
    regAddr = HYPHY20G_SONET_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    msk = HYPHY20G_SONET_TOP_BIT_SDH_PROC_SLC_CFG1_CLRINT_SLICE_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 1);
    if (result) return result;

    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 0);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* sonetLnHyPhy20gSonetLnToggleSonetClrintSliceBit */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnRxOtu2SrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the input data stream for the
**                  downstream OTU2 receive framers.
**
**  INPUTS:         *fileHndl - base address
**                  slice     - OTU2 framer
**                              Legal range 0 to 1
**                  src       - Source of OTU2 receive framer
**                               0 -  XFI #1
**                               1 -  XFI #2
**                               2 -  SFI #1
**                               3 -  SFI #2
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnRxOtu2SrcCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 src)
{
    UINT4 reg, msk, val;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* RX_OTU2_LINE_SEL */

    /* argument checking */
    if ((slice > 1) || (src > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    msk = (slice == 0) ?
          HYPHY20G_SONET_LINE_TOP_BIT_OTU2_RX_SEL_RX_OTU2_LINE_SEL0_MSK :
          HYPHY20G_SONET_LINE_TOP_BIT_OTU2_RX_SEL_RX_OTU2_LINE_SEL1_MSK;

    result = sonetLnHyPhy20gSonetLnSwizXfiSfiSwToHwVal(fileHndl, src, &val);
    if (result) return result;

    reg = HYPHY20G_SONET_LINE_TOP_REG_OTU2_RX_SEL;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnRxOtu2SrcCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnRxHsciGfpSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the input data stream for the
**                  downstream HSCI/GFP receive slice.
**
**  INPUTS:         *fileHndl - base address
**                  slice     - HSCI/GFP slice
**                              Legal range 0 to 1
**                  src       - Receive data source of HSCI/GFP slice
**                                0 to 1  - XFI 1 to 2
**                                2 to 3  - SFI 1 to 2
**                                4 to 5  - OTU2 Framer 0 to 1
**                                6 to 7  - ODUKPP ODU2 Framer 0 to 1
**                                8 to 11 - SONET group 0 to 3
**                  timingMd  - Timing mode
**                                0 - Timing from receive data source
**                                1 - Line timing mode
**                              Note: hyPhy20gSonetLnLineTimeSrcCfg
**                              configures the line timing source.
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnRxHsciGfpSrcCfg(struct file *fileHndl, UINT4 slice,
                                           UINT4 src, UINT4 timingMd)
{
    UINT4 reg, val;
    UINT4 dataMsk, dataVal;
    UINT4 otu2odu2DataMsk, otu2odu2DataVal;
    UINT4 clkMsk, clkVal;
    UINT4 otu2odu2ClkMsk, otu2odu2ClkVal;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((slice > 1) || (src > 11)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* select RX_GEFC_OTU2_ODU2_DATA_SELn, RX_GEFC_DATA_SELn, RX_GEFC_CLK_SELn
       and RX_GEFC_OTU2_ODU2_CLK_SELn fields based on HSCI/GFP slice */

    /* HSCI/GFP 0 */
    dataMsk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_RX_SEL_RX_GEFC_DATA_SEL0_MSK;
    otu2odu2DataMsk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_RX_SEL_RX_GEFC_OTU2_ODU2_DATA_SEL0_MSK;
    clkMsk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_RX_SEL_RX_GEFC_CLK_SEL0_MSK;
    otu2odu2ClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_RX_SEL_RX_GEFC_OTU2_ODU2_CLK_SEL0_MSK;

    if (slice == 1) {
        /* HSCI/GFP 1 */
        dataMsk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_RX_SEL_RX_GEFC_DATA_SEL1_MSK;
        otu2odu2DataMsk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_RX_SEL_RX_GEFC_OTU2_ODU2_DATA_SEL1_MSK;
        clkMsk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_RX_SEL_RX_GEFC_CLK_SEL1_MSK;
        otu2odu2ClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_RX_SEL_RX_GEFC_OTU2_ODU2_CLK_SEL1_MSK;
    }

    /* Data and clock source selection */
    if (src <= 3) {
        /* XFI, SFI */
        result = sonetLnHyPhy20gSonetLnSwizXfiSfiSwToHwVal(fileHndl, src, &val);
        if (result) return result;

        dataVal = clkVal = val;
        otu2odu2DataVal = otu2odu2ClkVal = 0;  /* don't care */

    } else if (src >= 4 && src <= 7) {
        /* OTU2, ODU2 */
        dataVal = clkVal = (src == 4 || src == 6) ? 4 : 5;
        otu2odu2DataVal = otu2odu2ClkVal = (src <= 5) ? 0 : 1;

    } else {
        /* SONET */
        dataVal = 0x6;
        clkVal = 0x7;
        otu2odu2DataVal = otu2odu2ClkVal = 0;  /* don't care */
    }

    if (timingMd == 1) {
        /* Line timing mode */
        clkVal = 0x6;
    }

    /* configure rx clock and data settings */
    reg = HYPHY20G_SONET_LINE_TOP_REG_GEFC_RX_SEL;
    result = sysHyPhy20gRead(fileHndl, reg, &val);

    result = hyPhy20gVarFieldWrite(&val, dataMsk, dataVal);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&val, otu2odu2DataMsk, otu2odu2DataVal);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&val, clkMsk, clkVal);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&val, otu2odu2ClkMsk, otu2odu2ClkVal);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, reg, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnRxHsciGfpSrcCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnWisDemapSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the SONET data source feeding the
**                  WIS demapper (SONET -> WIS -> HSCI/GFP).
**
**  INPUTS:         *fileHndl - base address
**                  slice     - WIS demapper slice
**                              Legal range 0 to 1
**                  sonetGrp  - SONET group feeding WIS demapper
**                               0 - SONET group 0 (slice 0 to 3)
**                               1 - SONET group 1 (slice 4 to 7)
**                               2 - SONET group 2 (slice 8 to 11)
**                               3 - SONET group 3 (slice 12 to 15)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnWisDemapSrcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 sonetGrp)
{
    UINT4 reg, msk;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((slice > 1) || (sonetGrp > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    msk = (slice == 0) ?
          HYPHY20G_SONET_LINE_TOP_BIT_GEFC_RX_SEL_RX_GEFC_SONET_SEL0_MSK :
          HYPHY20G_SONET_LINE_TOP_BIT_GEFC_RX_SEL_RX_GEFC_SONET_SEL1_MSK;

    reg = HYPHY20G_SONET_LINE_TOP_REG_GEFC_RX_SEL;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, sonetGrp);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnWisDemapSrcCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnRxSonetSlcSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the receive clock and data source for
**                  a SONET slice or group.
**
**                  This function toggles
**
**  INPUTS:         *fileHndl - base address
**                  slice     - SONET slice
**                              For rxSrc 0-7:
**                                - legal values are 0, 4, 8 and 12
**                              For rxSrc 8 to 39:
**                                - legal range is 0 to 15
**                  rxSrc     - Receive source for SONET slice
**                                0 to 1   - XFI 1 to 2
**                                2 to 3   - SFI 1 to 2
**                                4 to 5   - OTU2 Framer 0 to 1
**                                6 to 7   - ODUKPP ODU2 Framer 0 to 1
**                                8 to 23  - SFP 0 to 15
**                                24 to 31 - OTU1 Framer 0 to 7
**                                32 to 39 - ODUKPP ODU1 Framer 0 to 7
**                  sfpRate   - For rxSrc 8 to 23, specifies SONET slice
**                              data rate
**                                0 - STS-48
**                                1 - STS-12
**                                2 - STS-3
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnRxSonetSlcSrcCfg(struct file *fileHndl, UINT4 slice,
                                            UINT4 rxSrc, UINT4 sfpRate)
{
    UINT4 reg, msk, val;
    UINT4 rxOdu1Reg, sfpOtu1Reg;
    UINT4 rxSfiSfpMsk, rxSfiXfiOtu2Msk, rxSfpMsk;
    UINT4 rxOtu1SfpMsk, rxOtu1Msk, rxOtu1Odu1Msk, rxOdu1Msk;
    UINT4 rxSfiSfpSel, rxSfiXfiOtu2Sel, rxOtu1SfpSel, rxOtu1Odu1Sel;
    UINT4 rxSfpSel, rxOtu1Sel, rxOdu1Sel;
    UINT4 sliceEnd, rxOpMuxSel;
    UINT4 i;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /*
      RX_SFI_SFP_SEL
      RX_SFI_XFI_OTU2_SEL
      RX_SFP_SELn[3:0]
      RX_OTU1_SFP_SELn
      RX_OTU1_SELn[2:0]
      RX_OTU1_ODU1_SELn
      RX_ODU1_SELn[2:0]
      RX_OUTPUT_MUX_SEL
    */

    /* argument checking */
    if ((rxSrc > 39) ||
        ((rxSrc >= 8 && rxSrc <= 39) && (slice > 15 || sfpRate > 2)) ||
        ((rxSrc <= 7) &&
         (slice != 0 && slice != 4 && slice != 8 && slice != 12))) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine field masks and register offsets */
    rxOdu1Reg = (slice < 8) ? HYPHY20G_SONET_LINE_TOP_REG_RX_ODU1_CTRL1:
                HYPHY20G_SONET_LINE_TOP_REG_RX_ODU1_CTRL2;

    if (slice < 4) {
        rxSfiSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_RX_CTL_1_RX_SFI_SFP_SEL1_MSK;
        rxSfiXfiOtu2Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_RX_CTL_1_RX_SFI_XFI_OTU2_SEL1_MSK;

        sfpOtu1Reg = HYPHY20G_SONET_LINE_TOP_REG_SFP_OTU1_RX_CTL_1;
    } else if (slice < 8) {
        rxSfiSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_RX_CTL_1_RX_SFI_SFP_SEL2_MSK;
        rxSfiXfiOtu2Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_RX_CTL_1_RX_SFI_XFI_OTU2_SEL2_MSK;

        sfpOtu1Reg = HYPHY20G_SONET_LINE_TOP_REG_SFP_OTU1_RX_CTL_2;
    } else if (slice < 12) {
        rxSfiSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_RX_CTL_1_RX_SFI_SFP_SEL3_MSK;
        rxSfiXfiOtu2Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_RX_CTL_1_RX_SFI_XFI_OTU2_SEL3_MSK;

        sfpOtu1Reg = HYPHY20G_SONET_LINE_TOP_REG_SFP_OTU1_RX_CTL_3;
    } else {
        rxSfiSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_RX_CTL_1_RX_SFI_SFP_SEL4_MSK;
        rxSfiXfiOtu2Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_RX_CTL_1_RX_SFI_XFI_OTU2_SEL4_MSK;

        sfpOtu1Reg = HYPHY20G_SONET_LINE_TOP_REG_SFP_OTU1_RX_CTL_4;
    }

    switch (slice) {
        case 0:
            /* SONET slice 0 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_1_RX_SFP_SEL1_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_1_RX_OTU1_SFP_SEL1_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_1_RX_OTU1_SEL1_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_OTU1_ODU1_SEL1_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_ODU1_SEL1_MSK;
            break;
        case 1:
            /* SONET slice 1 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_1_RX_SFP_SEL2_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_1_RX_OTU1_SFP_SEL2_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_1_RX_OTU1_SEL2_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_OTU1_ODU1_SEL2_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_ODU1_SEL2_MSK;
            break;
        case 2:
            /* SONET slice 2 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_1_RX_SFP_SEL3_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_1_RX_OTU1_SFP_SEL3_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_1_RX_OTU1_SEL3_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_OTU1_ODU1_SEL3_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_ODU1_SEL3_MSK;
            break;
        case 3:
            /* SONET slice 3 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_1_RX_SFP_SEL4_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_1_RX_OTU1_SFP_SEL4_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_1_RX_OTU1_SEL4_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_OTU1_ODU1_SEL4_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_ODU1_SEL4_MSK;
            break;
        case 4:
            /* SONET slice 4 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_2_RX_SFP_SEL5_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_2_RX_OTU1_SFP_SEL5_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_2_RX_OTU1_SEL5_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_OTU1_ODU1_SEL5_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_ODU1_SEL5_MSK;
            break;
        case 5:
            /* SONET slice 5 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_2_RX_SFP_SEL6_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_2_RX_OTU1_SFP_SEL6_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_2_RX_OTU1_SEL6_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_OTU1_ODU1_SEL6_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_ODU1_SEL6_MSK;
            break;
        case 6:
            /* SONET slice 6 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_2_RX_SFP_SEL7_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_2_RX_OTU1_SFP_SEL7_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_2_RX_OTU1_SEL7_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_OTU1_ODU1_SEL7_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_ODU1_SEL7_MSK;
            break;
        case 7:
            /* SONET slice 7 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_2_RX_SFP_SEL8_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_2_RX_OTU1_SFP_SEL8_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_2_RX_OTU1_SEL8_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_OTU1_ODU1_SEL8_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL1_RX_ODU1_SEL8_MSK;
            break;
        case 8:
            /* SONET slice 8 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_3_RX_SFP_SEL9_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_3_RX_OTU1_SFP_SEL9_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_3_RX_OTU1_SEL9_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_OTU1_ODU1_SEL9_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_ODU1_SEL9_MSK;
            break;
        case 9:
            /* SONET slice 9 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_3_RX_SFP_SEL10_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_3_RX_OTU1_SFP_SEL10_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_3_RX_OTU1_SEL10_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_OTU1_ODU1_SEL10_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_ODU1_SEL10_MSK;
            break;
        case 10:
            /* SONET slice 10 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_3_RX_SFP_SEL11_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_3_RX_OTU1_SFP_SEL11_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_3_RX_OTU1_SEL11_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_OTU1_ODU1_SEL11_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_ODU1_SEL11_MSK;
            break;
        case 11:
            /* SONET slice 11 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_3_RX_SFP_SEL12_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_3_RX_OTU1_SFP_SEL12_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_3_RX_OTU1_SEL12_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_OTU1_ODU1_SEL12_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_ODU1_SEL12_MSK;
            break;
        case 12:
            /* SONET slice 12 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_4_RX_SFP_SEL13_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_4_RX_OTU1_SFP_SEL13_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_4_RX_OTU1_SEL13_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_OTU1_ODU1_SEL13_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_ODU1_SEL13_MSK;
            break;
        case 13:
            /* SONET slice 13 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_4_RX_SFP_SEL14_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_4_RX_OTU1_SFP_SEL14_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_4_RX_OTU1_SEL14_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_OTU1_ODU1_SEL14_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_ODU1_SEL14_MSK;
            break;
        case 14:
            /* SONET slice 14 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_4_RX_SFP_SEL15_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_4_RX_OTU1_SFP_SEL15_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_4_RX_OTU1_SEL15_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_OTU1_ODU1_SEL15_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_ODU1_SEL15_MSK;
            break;
        case 15:
            /* SONET slice 15 */
            rxSfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_4_RX_SFP_SEL16_MSK;
            rxOtu1SfpMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_4_RX_OTU1_SFP_SEL16_MSK;
            rxOtu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_RX_CTL_4_RX_OTU1_SEL16_MSK;
            rxOtu1Odu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_OTU1_ODU1_SEL16_MSK;
            rxOdu1Msk = HYPHY20G_SONET_LINE_TOP_BIT_RX_ODU1_CTRL2_RX_ODU1_SEL16_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    rxSfiSfpSel     = 0;  /* SFI, XFI, OTU2 or ODU2 */
    rxSfiXfiOtu2Sel = 0;  /* SFI 1 */
    rxOtu1SfpSel    = 0;  /* SFP */
    rxOtu1Odu1Sel   = 0;  /* OTU1 */
    rxSfpSel        = 0;
    rxOtu1Sel       = 0;
    rxOdu1Sel       = 0;

    /* determine register settings */
    if (rxSrc <= 7) {
        /* XFI, SFI, OTU2 or ODU2 */

        /* RX_SFI_XFI_OTU2_SEL */
        if (rxSrc < 4) {
            result = sonetLnHyPhy20gSonetLnSwizXfiSfiSwToHwVal(fileHndl, rxSrc,
                                                              &rxSfiXfiOtu2Sel);
            if (result) return result;
        } else {
            rxSfiXfiOtu2Sel = rxSrc;
        }

        /* RX_OUTPUT_MUX_SEL */
        sliceEnd = slice + 3;
        rxOpMuxSel = 1;

    } else if (rxSrc >= 8 && rxSrc <= 23) {
        /* SFP */

        /* RX_SFI_SFP_SEL */
        rxSfiSfpSel = 1;
        /* RX_SFP_SEL */
        rxSfpSel = rxSrc - 8;
        /* RX_OTU1_SFP_SEL */
        rxOtu1SfpSel = 0;

        /* RX_OUTPUT_MUX_SEL */
        sliceEnd = slice;
        rxOpMuxSel = (sfpRate == 0) ? 1 : 0;

    } else if (rxSrc >= 24 && rxSrc <= 31) {
        /* OTU1 */

        /* RX_SFI_SFP_SEL */
        rxSfiSfpSel = 1;
        /* RX_OTU1_SFP_SEL */
        rxOtu1SfpSel = 1;
        /* RX_OTU1_SEL */
        rxOtu1Sel = rxSrc - 24;

        /* RX_OUTPUT_MUX_SEL */
        sliceEnd = slice;
        rxOpMuxSel = 1;

    } else {
        /* ODU1 */

        /* RX_SFI_SFP_SEL */
        rxSfiSfpSel = 1;
        /* RX_OTU1_SFP_SEL */
        rxOtu1SfpSel = 1;
        /* RX_OTU1_ODU1_SEL */
        rxOtu1Odu1Sel = 1;
        /* RX_ODU1_SEL */
        rxOdu1Sel = rxSrc - 32;

        /* RX_OUTPUT_MUX_SEL */
        sliceEnd = slice;
        rxOpMuxSel = 1;
    }

    /* configuration sequence */

    /* configure RX_SFI_SFP_SEL */
    reg = HYPHY20G_SONET_LINE_TOP_REG_SFI_XFI_OTU2_RX_CTL_1;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, rxSfiSfpMsk, rxSfiSfpSel);
    if (result) return result;

    if (rxSrc <= 7) {
        /* configure RX_SFI_XFI_OTU2_SEL */
        reg = HYPHY20G_SONET_LINE_TOP_REG_SFI_XFI_OTU2_RX_CTL_1;
        result = hyPhy20gRegFieldWrite(fileHndl, reg, rxSfiXfiOtu2Msk,
                                       rxSfiXfiOtu2Sel);
        if (result) return result;
    } else {
        /* configure RX_OTU1_SFP_SEL */
        reg = sfpOtu1Reg;
        result = hyPhy20gRegFieldWrite(fileHndl, reg, rxOtu1SfpMsk,
                                       rxOtu1SfpSel);
        if (result) return result;

        if (rxSrc <= 23) {
            /* configure RX_SFP_SEL */
            reg = sfpOtu1Reg;
            result = hyPhy20gRegFieldWrite(fileHndl, reg, rxSfpMsk, rxSfpSel);
            if (result) return result;
        }
    }

    if (rxSrc >= 24) {
        /* configure RX_OTU1_ODU1_SEL */
        reg = rxOdu1Reg;
        result = hyPhy20gRegFieldWrite(fileHndl, reg, rxOtu1Odu1Msk,
                                       rxOtu1Odu1Sel);
        if (result) return result;

        if (rxSrc <= 31) {
            /* configure RX_OTU1_SEL */
            reg = sfpOtu1Reg;
            result = hyPhy20gRegFieldWrite(fileHndl, reg, rxOtu1Msk,
                                           rxOtu1Sel);
            if (result) return result;
        } else {
            /* configure RX_ODU1_SEL */
            reg = rxOdu1Reg;
            result = hyPhy20gRegFieldWrite(fileHndl, reg, rxOdu1Msk, rxOdu1Sel);
            if (result) return result;
        }
    }

    /* configure RX_OUTPUT_MUX_SEL */
    reg = HYPHY20G_SONET_LINE_TOP_REG_STS_MD_SEL_RX;
    msk = HYPHY20G_SONET_LINE_TOP_BIT_STS_MD_SEL_RX_RX_OUTPUT_MUX_SEL_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, reg, msk, &val);
    if (result) return result;

    for (i = slice; i <= sliceEnd; i++) {
        if (rxOpMuxSel == 1) {
            /* set RX_OUTPUT_MUX_SEL to 1 */
            val |= (0x1 << i);
        } else {
            /* clear RX_OUTPUT_MUX_SEL to 0 */
            val &= ~(0x1 << i);
        }
    }

    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, val);
    if (result) return result;

    for (i = slice; i <= sliceEnd; i++) {
        /* toggle CLRINT_SLICE of SONET slices due to change in
           RX_OUTPUT_MUX_SEL */
        result = sonetLnHyPhy20gSonetLnToggleSonetClrintSliceBit(fileHndl, i);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnRxSonetSlcSrcCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnRxOtu1SrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the receive clock and data source for
**                  the OTU1 slice.
**
**  INPUTS:         *fileHndl - base address
**                  slice     - OTU1 framer
**                              Legal range 0 to 7
**                  rxSrc     - Receive source of OTU1 framer
**                                0 to 15 - SFP 0 to 15
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnRxOtu1SrcCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 rxSrc)
{
    UINT4 reg, msk;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /*
      RX_OTU1_LINE_SELn
    */

    /* argument checking */
    if ((slice > 7) || (rxSrc > 15)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (slice) {
        case 0:
            /* OTU 0 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_RX_SEL_RX_OTU1_LINE_SEL0_MSK;
            break;
        case 1:
            /* OTU 1 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_RX_SEL_RX_OTU1_LINE_SEL1_MSK;
            break;
        case 2:
            /* OTU 2 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_RX_SEL_RX_OTU1_LINE_SEL2_MSK;
            break;
        case 3:
            /* OTU 3 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_RX_SEL_RX_OTU1_LINE_SEL3_MSK;
            break;
        case 4:
            /* OTU 4 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_RX_SEL_RX_OTU1_LINE_SEL4_MSK;
            break;
        case 5:
            /* OTU 5 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_RX_SEL_RX_OTU1_LINE_SEL5_MSK;
            break;
        case 6:
            /* OTU 6 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_RX_SEL_RX_OTU1_LINE_SEL6_MSK;
            break;
        case 7:
            /* OTU 7 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_RX_SEL_RX_OTU1_LINE_SEL7_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    reg = HYPHY20G_SONET_LINE_TOP_REG_OTU1_RX_SEL;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, rxSrc);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnRxOtu1SrcCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnTxXfiSfiSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the transmit data source for the
**                  XFI and SFI interfaces.
**
**  INPUTS:         *fileHndl - base address
**                  intf       - interface
**                                0 -  XFI #1
**                                1 -  XFI #2
**                                2 -  SFI #1
**                                3 -  SFI #2
**                  block      - Subsystem resource to process data stream
**                                0 - HSCI slice 0
**                                1 - HSCI slice 1
**                                2 - SONET group 0 (slice 0 to 3)
**                                3 - SONET group 1 (slice 4 to 7)
**                                4 - SONET group 2 (slice 8 to 11)
**                                5 - SONET group 3 (slice 12 to 15)
**                                6 - OTN OTU2 Framer slice 0
**                                7 - OTN OTU2 Framer slice 1
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnTxXfiSfiSrcCfg(struct file *fileHndl, UINT4 intf,
                                          UINT4 block)
{
    UINT4 addr;
    UINT4 txSelVal, txSelMsk;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* Check input arguments */
    if ((intf > 3) || (block > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Interface selection */
    switch (intf) {
        case 0:
            /* XFI #1 */
            txSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_TX_SEL_TX_XFI_SEL0_MSK;
            break;
        case 1:
            /* XFI #2 */
            txSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_TX_SEL_TX_XFI_SEL1_MSK;
            break;
        case 2:
            /* SFI #1 */
            txSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_TX_SEL_TX_SFI_SEL0_MSK;
            break;
        case 3:
            /* SFI #2 */
            txSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_TX_SEL_TX_SFI_SEL1_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if (block <= 1) {
        /* HSCI 0 or 1 */
        txSelVal = block + 2;
    } else if (block >= 2 && block <= 5) {
        /* SONET group 0 to 3 */
        txSelVal = (block-2) + 4;
    } else {
        /* OTU2 framer 0 and 1 */
        txSelVal = block-6;
    }

    addr = HYPHY20G_SONET_LINE_TOP_REG_SFI_TX_SEL;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, txSelMsk, txSelVal);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* sonetLnHyPhy20gSonetLnTxXfiSfiSrcCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnTxHsciClkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the transmit clock source for the
**                  HSCI slice.
**
**  INPUTS:         *fileHndl - base address
**                  slice     - HSCI slice
**                              Legal range 0 to 1
**                  txClkSrc  - Transmit clock source of HSCI slice
**                                0 to 1 - XFI 1 to 2
**                                2 to 3 - SFI 1 to 2
**                                4      - OTU2
**                                5      - ODUKPP ODU2
**                                6      - SYSCLK_OC192
**                                7      - CLK311
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnTxHsciClkCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 txClkSrc)
{
    UINT4 reg, msk;
    UINT4 clkSrc;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /*
      TX_GEFC_CLK_SELn
    */

    /* argument checking */
    if (slice > 1 || txClkSrc > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (slice) {
        case 0:
            /* HSCI 0 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_TX_SEL_TX_GEFC_CLK_SEL0_MSK;
            break;
        case 1:
            /* HSCI 1 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_TX_SEL_TX_GEFC_CLK_SEL1_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    if (txClkSrc <= 3) {
        result = sonetLnHyPhy20gSonetLnSwizXfiSfiSwToHwVal(fileHndl, txClkSrc,
                                                           &clkSrc);
        if (result) return result;
    } else {
        clkSrc = txClkSrc;
    }

    reg = HYPHY20G_SONET_LINE_TOP_REG_GEFC_TX_SEL;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, clkSrc);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnTxHsciClkCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnTxHsciBckPrssrCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the back pressure source for the
**                  transmit HSCI slice.
**
**  INPUTS:         *fileHndl - base address
**                  slice     - HSCI slice
**                              Legal range 0 to 1
**                  src       - Back pressure source of HSCI slice
**                                0 to 1 - OTU2 Framer 0 to 1
**                                2 to 3 - ODUKPP ODU2 Framer 0 to 1
**                                4      - WIS Mapper
**                                5      - None (XFI/SFI)
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnTxHsciBckPrssrCfg(struct file *fileHndl, UINT4 slice,
                                             UINT4 src)
{
    UINT4 reg, val;
    UINT4 sonetSelMsk, otu2Odu2SelMsk;
    UINT4 sonetSel, otu2Odu2Sel;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /*
      TX_GEFC_SONET_SELn
      TX_GEFC_OTU2_ODU2_SELn
    */

    /* argument checking */
    if (slice > 1 || src > 5) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (slice) {
        case 0:
            /* HSCI 0 */
            sonetSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_TX_SEL_TX_GEFC_SONET_SEL0_MSK;
            otu2Odu2SelMsk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_TX_SEL_TX_GEFC_OTU2_ODU2_SEL0_MSK;
            break;
        case 1:
            /* HSCI 1 */
            sonetSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_TX_SEL_TX_GEFC_SONET_SEL1_MSK;
            otu2Odu2SelMsk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_TX_SEL_TX_GEFC_OTU2_ODU2_SEL1_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* register settings */
    switch (src) {
        case 0:
            /* OTU2 Framer 0 */
            sonetSel = 0;
            otu2Odu2Sel = 0;
            break;
        case 1:
            /* OTU2 Framer 1 */
            sonetSel = 1;
            otu2Odu2Sel = 0;
            break;
        case 2:
            /* ODU2 Framer 0 */
            sonetSel = 0;
            otu2Odu2Sel = 1;
            break;
        case 3:
            /* ODU2 Framer 1 */
            sonetSel = 1;
            otu2Odu2Sel = 1;
            break;
        case 4:
            /* WIS Mapper */
            sonetSel = 3;
            otu2Odu2Sel = 0;  /* don't care */
            break;
        case 5:
            /* None (XFI, SFI) */
            sonetSel = 2;
            otu2Odu2Sel = 0;  /* don't care */
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    reg = HYPHY20G_SONET_LINE_TOP_REG_GEFC_TX_SEL;
    result = sysHyPhy20gRead(fileHndl, reg, &val);
    if (result) return result;

    /* configure TX_GEFC_SONET_SEL */
    result = hyPhy20gVarFieldWrite(&val, sonetSelMsk, sonetSel);
    if (result) return result;

    if (src != 4 && src != 5) {
        /* configure TX_GEFC_OTU2_ODU2_SEL */
        result = hyPhy20gVarFieldWrite(&val, otu2Odu2SelMsk, otu2Odu2Sel);
        if (result) return result;
    }

    result = sysHyPhy20gWrite(fileHndl, reg, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnTxHsciBckPrssrCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnTxWisMapSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the transmit source (GFP or HSCI)
**                  for the WIS mapper in the direction of
**                  HSCI/GFP -> WIS -> SONET.
**
**                  sonetLnHyPhy20gSonetLnTxSonetSlcSrcCfg should be called
**                  after this function to select the SONET source for the
**                  WIS Mapper.
**
**  INPUTS:         *fileHndl - base address
**                  slice     - WIS mapper slice
**                              Legal range 0 to 1
**                  txSrc     - Transmit source for WIS Mapper
**                                0 - HSCI
**                                1 - GFP
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnTxWisMapSrcCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 txSrc)
{
    UINT4 reg, msk;
    UINT4 src;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /*
      TX_POS_GEFCB_n
    */

    /* argument checking */
    if (slice > 1 || txSrc > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (slice) {
        case 0:
            /* WIS Mapper 0 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_TX_SEL_TX_POS_GEFCB_0_MSK;
            break;
        case 1:
            /* WIS Mapper 1 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_TX_SEL_TX_POS_GEFCB_1_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    src = (~txSrc) & 0x1;
    reg = HYPHY20G_SONET_LINE_TOP_REG_GEFC_TX_SEL;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, src);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnTxWisMapSrcCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnTxSonetSlcSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects a WIS mapper as the transmit data
**                  source for the SONET group.
**
**                  sonetLnHyPhy20gSonetLnTxWisMapSrcCfg should be called
**                  prior to this function to select the transmit source for
**                  the WIS Mapper.
**
**  INPUTS:         *fileHndl - base address
**                  slice     - SONET group
**                              Legal range 0 to 3
**                  txSrc     - Transmit source
**                                0 to 1   - WIS Mapper 1 to 2
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnTxSonetSlcSrcCfg(struct file *fileHndl, UINT4 slice,
                                            UINT4 txSrc)
{
    UINT4 reg, msk;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /*
      TX_SONET_SELn
    */

    /* argument checking */
    if (slice > 3 || txSrc > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (slice) {
        case 0:
            /* SONET group 0 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_TX_SEL_TX_SONET_SEL0_MSK;
            break;
        case 1:
            /* SONET group 1 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_TX_SEL_TX_SONET_SEL1_MSK;
            break;
        case 2:
            /* SONET group 2 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_TX_SEL_TX_SONET_SEL2_MSK;
            break;
        case 3:
            /* SONET group 3 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_TX_SEL_TX_SONET_SEL3_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    reg = HYPHY20G_SONET_LINE_TOP_REG_TX_SEL;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, txSrc);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnTxSonetSlcSrcCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnTxSonetSlcClkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the transmit clock source for the
**                  SONET slice.
**
**  INPUTS:         *fileHndl - base address
**                  slice     - SONET slice
**                              For txClkSrc 0 to 3:
**                                - legal values are 0, 4, 8 and 12
**                              For other txClkSrc values (4 to 20)
**                                - legal range is 0 to 15
**                  txClkSrc  - Transmit clock source of SONET slice
**                                0 to 1  - XFI 1 to 2
**                                2 to 3  - SFI 1 to 2
**                                4       - CLK311
**                                5 to 20 - SFP 0 to 15
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnTxSonetSlcClkCfg(struct file *fileHndl, UINT4 slice,
                                            UINT4 txClkSrc)
{
    UINT4 reg;
    UINT4 sfpClkSelReg;
    UINT4 sfiSfpClkMsk, sfiXfiOtu2ClkMsk, sfpClkMsk;
    UINT4 sfiSfpClkSel, sfiXfiOtu2ClkSel, sfpClkSel;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /*
      TX_SFI_XFI_OTU2_CLK_SEL
      TX_SFI_SFP_CLK_SEL
      TX_SFP_CLK_SEL
    */

    /* argument checking */
    if ((txClkSrc > 20) || ((txClkSrc <= 3) &&
        (slice != 0 && slice != 4 && slice != 8 && slice != 12))) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine field masks and register offsets */
    if (slice < 4) {
        sfiSfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_CLK_CTL_TX_1_TX_SFI_SFP_CLK_SEL1_MSK;
        sfiXfiOtu2ClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_CLK_CTL_TX_1_TX_SFI_XFI_OTU2_CLK_SEL1_MSK;

        sfpClkSelReg = HYPHY20G_SONET_LINE_TOP_REG_SFP_OTU1_CLK_CTL_TX_1;
    } else if (slice < 8) {
        sfiSfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_CLK_CTL_TX_1_TX_SFI_SFP_CLK_SEL2_MSK;
        sfiXfiOtu2ClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_CLK_CTL_TX_1_TX_SFI_XFI_OTU2_CLK_SEL2_MSK;

        sfpClkSelReg = HYPHY20G_SONET_LINE_TOP_REG_SFP_OTU1_CLK_CTL_TX_2;
    } else if (slice < 12) {
        sfiSfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_CLK_CTL_TX_1_TX_SFI_SFP_CLK_SEL3_MSK;
        sfiXfiOtu2ClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_CLK_CTL_TX_1_TX_SFI_XFI_OTU2_CLK_SEL3_MSK;

        sfpClkSelReg = HYPHY20G_SONET_LINE_TOP_REG_SFP_OTU1_CLK_CTL_TX_3;
    } else {
        sfiSfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_CLK_CTL_TX_1_TX_SFI_SFP_CLK_SEL4_MSK;
        sfiXfiOtu2ClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFI_XFI_OTU2_CLK_CTL_TX_1_TX_SFI_XFI_OTU2_CLK_SEL4_MSK;

        sfpClkSelReg = HYPHY20G_SONET_LINE_TOP_REG_SFP_OTU1_CLK_CTL_TX_4;
    }

    switch (slice) {
        case 0:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_1_TX_SFP_CLK_SEL1_MSK;
            break;
        case 1:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_1_TX_SFP_CLK_SEL2_MSK;
            break;
        case 2:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_1_TX_SFP_CLK_SEL3_MSK;
            break;
        case 3:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_1_TX_SFP_CLK_SEL4_MSK;
            break;
        case 4:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_2_TX_SFP_CLK_SEL5_MSK;
            break;
        case 5:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_2_TX_SFP_CLK_SEL6_MSK;
            break;
        case 6:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_2_TX_SFP_CLK_SEL7_MSK;
            break;
        case 7:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_2_TX_SFP_CLK_SEL8_MSK;
            break;
        case 8:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_3_TX_SFP_CLK_SEL9_MSK;
            break;
        case 9:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_3_TX_SFP_CLK_SEL10_MSK;
            break;
        case 10:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_3_TX_SFP_CLK_SEL11_MSK;
            break;
        case 11:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_3_TX_SFP_CLK_SEL12_MSK;
            break;
        case 12:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_4_TX_SFP_CLK_SEL13_MSK;
            break;
        case 13:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_4_TX_SFP_CLK_SEL14_MSK;
            break;
        case 14:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_4_TX_SFP_CLK_SEL15_MSK;
            break;
        case 15:
            sfpClkMsk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_OTU1_CLK_CTL_TX_4_TX_SFP_CLK_SEL16_MSK;
            break;

        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    sfiSfpClkSel = (txClkSrc <= 4) ? 0 : 1;
    reg = HYPHY20G_SONET_LINE_TOP_REG_SFI_XFI_OTU2_CLK_CTL_TX_1;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, sfiSfpClkMsk, sfiSfpClkSel);
    if (result) return result;

    if (txClkSrc <= 4) {
        if (txClkSrc <= 3) {
            /* XFI or SFI */
            result = sonetLnHyPhy20gSonetLnSwizXfiSfiSwToHwVal(fileHndl,
                                                  txClkSrc, &sfiXfiOtu2ClkSel);
            if (result) return result;
        } else {
            /* CLK311 */
            sfiXfiOtu2ClkSel = txClkSrc;
        }

        reg = HYPHY20G_SONET_LINE_TOP_REG_SFI_XFI_OTU2_CLK_CTL_TX_1;
        result = hyPhy20gRegFieldWrite(fileHndl, reg, sfiXfiOtu2ClkMsk,
                                       sfiXfiOtu2ClkSel);
        if (result) return result;

    } else {
        /* SFP 0 to 15 */
        sfpClkSel = txClkSrc - 5;
        reg = sfpClkSelReg;
        result = hyPhy20gRegFieldWrite(fileHndl, reg, sfpClkMsk, sfpClkSel);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnTxSonetSlcClkCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnSts192SysClkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the system clock for an STS-192.
**
**  INPUTS:         *fileHndl - base address
**                  sonetGrp  - SONET group
**                              Legal range 0 to 3
**                  sysClk    - System clock selection
**                                0 - CLK311
**                                1 - Line timing source 0
**                                2 - Line timing source 1
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnSts192SysClkCfg(struct file *fileHndl, UINT4 sonetGrp,
                                           UINT4 sysClk)
{
    UINT4 reg, msk, val;
    UINT4 sysClkVal, sonetGrpMsk;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument check */
    if (sonetGrp > 3 || sysClk > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    reg = HYPHY20G_SONET_LINE_TOP_REG_GEFC_TX_SEL;
    msk = HYPHY20G_SONET_LINE_TOP_BIT_GEFC_TX_SEL_SYSCLK_OC192_SEL_MSK;

    switch (sysClk) {
        case 0:
            /* CLK311 */
            sysClkVal = 0;
            break;
        case 1:
            /* SYSCLK_OC192[0] */
            sysClkVal = 2;
            break;
        case 2:
            /* SYSCLK_OC192[1] */
            sysClkVal = 3;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    result = hyPhy20gRegFieldRead(fileHndl, reg, msk, &val);
    if (result) return result;

    sonetGrp = 2 * sonetGrp;
    sonetGrpMsk = 0x3 << sonetGrp;
    val &= (~sonetGrpMsk);
    val |= (sysClkVal << sonetGrp);

    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnSts192SysClkCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnTxOtu2Odu2SysSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the transmit OTU2/ODU2 source
**                  from the system side SONET or HSCI blocks.
**
**  INPUTS:         *fileHndl - base address
**                  slice     - 0 to 1 - OTU2 Framer 0 to 1
**                              2 to 3 - ODUKPP ODU2 Framer 0 to 1
**                  block     - Source block for selected slice
**                                0 - SONET
**                                1 - HSCI
**                  srcSlc    - Source slice from selected block
**                              For block == 0:  0 to 3 - SONET group 0 to 3
**                              For block == 1:  0 to 1 - HSCI 0 to 1
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnTxOtu2Odu2SysSrcCfg(struct file *fileHndl,
                                               UINT4 slice, UINT4 block,
                                               UINT4 srcSlc)
{
    UINT4 reg, msk;
    UINT4 tdataReg, sysReg;
    UINT4 tdataSelMsk, sysSelMsk;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /*
      TX_OTU2_SYS_SELn
      TX_ODU2_SYS_SELn
      TX_OTU2_TDATA_SELn
      TX_ODU2_TDATA_SELn
    */

    /* argument checking */
    if ((slice > 3) || (block > 1) ||
        (block == 0 && srcSlc > 3) || (block == 1 && srcSlc > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (slice) {
        case 0:
            /* OTU2 Framer 0 */
            tdataReg = HYPHY20G_SONET_LINE_TOP_REG_OTU2_TDATA_SEL;
            sysReg = HYPHY20G_SONET_LINE_TOP_REG_OTU2_TX_SEL;

            tdataSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_OTU2_TDATA_SEL_TX_OTU2_TDATA_SEL0_MSK;
            sysSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_OTU2_TX_SEL_TX_OTU2_SYS_SEL0_MSK;
            break;
        case 1:
            /* OTU2 Framer 1 */
            tdataReg = HYPHY20G_SONET_LINE_TOP_REG_OTU2_TDATA_SEL;
            sysReg = HYPHY20G_SONET_LINE_TOP_REG_OTU2_TX_SEL;

            tdataSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_OTU2_TDATA_SEL_TX_OTU2_TDATA_SEL1_MSK;
            sysSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_OTU2_TX_SEL_TX_OTU2_SYS_SEL1_MSK;
            break;
        case 2:
            /* ODUKPP ODU2 Framer 0 */
            tdataReg = HYPHY20G_SONET_LINE_TOP_REG_OTU2_TDATA_SEL;
            sysReg = HYPHY20G_SONET_LINE_TOP_REG_OTU2_TX_SEL;

            tdataSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_OTU2_TDATA_SEL_TX_ODU2_TDATA_SEL0_MSK;
            sysSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_OTU2_TX_SEL_TX_ODU2_SYS_SEL0_MSK;
            break;
        case 3:
            /* ODUKPP ODU2 Framer 1 */
            tdataReg = HYPHY20G_SONET_LINE_TOP_REG_OTU2_TDATA_SEL;
            sysReg = HYPHY20G_SONET_LINE_TOP_REG_OTU2_TX_SEL;

            tdataSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_OTU2_TDATA_SEL_TX_ODU2_TDATA_SEL1_MSK;
            sysSelMsk = HYPHY20G_SONET_LINE_TOP_BIT_OTU2_TX_SEL_TX_ODU2_SYS_SEL1_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    reg = (block == 0) ? tdataReg : sysReg;
    msk = (block == 0) ? tdataSelMsk : sysSelMsk;

    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, srcSlc);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnTxOtu2Odu2SysSrcCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnTxOtu2ClkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the transmit clock source for an
**                  OTU2 framer.
**
**  INPUTS:         *fileHndl - base address
**                  slice     - OTU2 framer
**                              Legal range 0 to 1
**                  txClkSrc  - Transmit clock source of OTU2 framer
**                                0 to 1 - XFI 1 to 2
**                                2 to 3 - SFI 1 to 2
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnTxOtu2ClkCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 txClkSrc)
{
    UINT4 reg, msk, val;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /*
      TX_OTU2_LINE_CLK_SEL
    */

    /* argument checking */
    if (slice > 1 || txClkSrc > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = sonetLnHyPhy20gSonetLnSwizXfiSfiSwToHwVal(fileHndl, txClkSrc, &val);
    if (result) return result;

    msk = (slice == 0) ?
         HYPHY20G_SONET_LINE_TOP_BIT_OTU2_TX_CLK_SEL_TX_OTU2_LINE_CLK_SEL0_MSK :
         HYPHY20G_SONET_LINE_TOP_BIT_OTU2_TX_CLK_SEL_TX_OTU2_LINE_CLK_SEL1_MSK;

    reg = HYPHY20G_SONET_LINE_TOP_REG_OTU2_TX_CLK_SEL;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnTxOtu2ClkCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnTxOtu1Odu1SysSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the transmit OTU1/ODU1 source from
**                  the system side SONET blocks.
**
**  INPUTS:         *fileHndl - base address
**                  slice     - 0 to 7  - OTU1 Framer 0 to 7
**                              8 to 15 - ODUKPP ODU1 Framer 0 to 7
**                  src       - Source SONET slice
**                                0 to 15 - SONET slice 0 to 15
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnTxOtu1Odu1SysSrcCfg(struct file *fileHndl,
                                               UINT4 slice, UINT4 src)
{
    UINT4 reg, msk;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /*
      TX_OTU1_TDATA_SELn
      TX_ODU1_TDATA_SELn
    */

    /* argument checking */
    if (slice > 15 || src > 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (slice) {
        case 0:
            /* OTU1 Framer 0 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TDATA_SEL_1_TX_OTU1_TDATA_SEL0_MSK;
            break;
        case 1:
            /* OTU1 Framer 1 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TDATA_SEL_1_TX_OTU1_TDATA_SEL1_MSK;
            break;
        case 2:
            /* OTU1 Framer 2 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TDATA_SEL_1_TX_OTU1_TDATA_SEL2_MSK;
            break;
        case 3:
            /* OTU1 Framer 3 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TDATA_SEL_1_TX_OTU1_TDATA_SEL3_MSK;
            break;
        case 4:
            /* OTU1 Framer 4 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TDATA_SEL_1_TX_OTU1_TDATA_SEL4_MSK;
            break;
        case 5:
            /* OTU1 Framer 5 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TDATA_SEL_1_TX_OTU1_TDATA_SEL5_MSK;
            break;
        case 6:
            /* OTU1 Framer 6 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TDATA_SEL_1_TX_OTU1_TDATA_SEL6_MSK;
            break;
        case 7:
            /* OTU1 Framer 7 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TDATA_SEL_1_TX_OTU1_TDATA_SEL7_MSK;
            break;
        case 8:
            /* ODU1 Framer 0 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_ODU1_TDATA_SEL_1_TX_ODU1_TDATA_SEL0_MSK;
            break;
        case 9:
            /* ODU1 Framer 1 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_ODU1_TDATA_SEL_1_TX_ODU1_TDATA_SEL1_MSK;
            break;
        case 10:
            /* ODU1 Framer 2 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_ODU1_TDATA_SEL_1_TX_ODU1_TDATA_SEL2_MSK;
            break;
        case 11:
            /* ODU1 Framer 3 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_ODU1_TDATA_SEL_1_TX_ODU1_TDATA_SEL3_MSK;
            break;
        case 12:
            /* ODU1 Framer 4 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_ODU1_TDATA_SEL_1_TX_ODU1_TDATA_SEL4_MSK;
            break;
        case 13:
            /* ODU1 Framer 5 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_ODU1_TDATA_SEL_1_TX_ODU1_TDATA_SEL5_MSK;
            break;
        case 14:
            /* ODU1 Framer 6 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_ODU1_TDATA_SEL_1_TX_ODU1_TDATA_SEL6_MSK;
            break;
        case 15:
            /* ODU1 Framer 7 */
            msk = HYPHY20G_SONET_LINE_TOP_BIT_ODU1_TDATA_SEL_1_TX_ODU1_TDATA_SEL7_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    reg = (slice <= 7) ? HYPHY20G_SONET_LINE_TOP_REG_OTU1_TDATA_SEL_1 :
                         HYPHY20G_SONET_LINE_TOP_REG_ODU1_TDATA_SEL_1;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, src);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnTxOtu1Odu1SysSrcCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnTxOtu1ClkCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the transmit clock source for an
**                  OTU1 framer.
**
**  INPUTS:         *fileHndl - base address
**                  slice     - OTU1 framer
**                              Legal range 0 to 7
**                  txClkSrc  - Transmit clock source of OTU1 framer
**                                0 to 15 - SFP 0 to 15
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnTxOtu1ClkCfg(struct file *fileHndl, UINT4 slice,
                                        UINT4 txClkSrc)
{
    UINT4 reg, msk;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /*
      TX_OTU1_LINE_SEL
    */

    /* argument checking */
    if (slice > 7 || txClkSrc > 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (slice) {
        case 0:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TX_CLK_SEL_TX_OTU1_LINE_SEL0_MSK;
            break;
        case 1:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TX_CLK_SEL_TX_OTU1_LINE_SEL1_MSK;
            break;
        case 2:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TX_CLK_SEL_TX_OTU1_LINE_SEL2_MSK;
            break;
        case 3:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TX_CLK_SEL_TX_OTU1_LINE_SEL3_MSK;
            break;
        case 4:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TX_CLK_SEL_TX_OTU1_LINE_SEL4_MSK;
            break;
        case 5:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TX_CLK_SEL_TX_OTU1_LINE_SEL5_MSK;
            break;
        case 6:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TX_CLK_SEL_TX_OTU1_LINE_SEL6_MSK;
            break;
        case 7:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_OTU1_TX_CLK_SEL_TX_OTU1_LINE_SEL7_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    reg = HYPHY20G_SONET_LINE_TOP_REG_OTU1_TX_CLK_SEL;
    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, txClkSrc);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnTxOtu1ClkCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnSwizXfiSfiSwToHwVal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function maps the SW XFI/SFI input values to HW
**                  XFI/SFI values.
**
**  INPUTS:         *fileHndl - base address
**                  swVal     - SW XFI/SFI value
**                               0 -  XFI #1
**                               1 -  XFI #2
**                               2 -  SFI #1
**                               3 -  SFI #2
**                  *pHwVal   - Pointer to storage for HW value
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnSwizXfiSfiSwToHwVal(struct file *fileHndl,
                                               UINT4 swVal, UINT4 *pHwVal)
{
    UINT4 val;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if (swVal > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (swVal) {
        case 0:
            /* XFI 1 */
            val = 2;
            break;
        case 1:
            /* XFI 2 */
            val = 3;
            break;
        case 2:
            /* SFI 1 */
            val = 0;
            break;
        case 3:
            /* SFI 2 */
            val = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    *pHwVal = val;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnSwizXfiSfiSwToHwVal */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnSwizXfiSfiHwToSwVal
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function maps the SW XFI/SFI input values to HW
**                  XFI/SFI values.
**
**  INPUTS:         *fileHndl - base address
**                  hwVal     - HW XFI/SFI value
**                               0 -  SFI #1
**                               1 -  SFI #2
**                               2 -  XFI #1
**                               3 -  XFI #2
**                  *pSwVal   - Pointer to storage for HW value
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnSwizXfiSfiHwToSwVal(struct file *fileHndl,
                                               UINT4 hwVal, UINT4 *pSwVal)
{
    UINT4 val;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if (hwVal > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (hwVal) {
        case 0:
            /* SFI 1 */
            val = 2;
            break;
        case 1:
            /* SFI 2 */
            val = 3;
            break;
        case 2:
            /* XFI 1 */
            val = 0;
            break;
        case 3:
            /* XFI 2 */
            val = 1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    *pSwVal = val;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnSwizXfiSfiHwToSwVal */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnTseRxJ0DlyCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures TSE RX_J0DLY for the set of
**                  2.5G ports associated with the STS-192 signal.
**
**  INPUTS:         *fileHndl - base address
**                  sonetSlc  - SONET slice
**                              For STS-3/12/48 sonet rates:
**                                - legal range is 0 to 15
**                              For STS-192:
**                                - legal values are 0, 4, 8, and 12
**                  sonetRate - SONET rate
**                                0 - STS-48
**                                1 - STS-12
**                                2 - STS-3
**                                3 - STS-192
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnTseRxJ0DlyCfg(struct file *fileHndl, UINT4 sonetSlc,
                                         UINT4 sonetRate)
{
    UINT4 addr, msk;
    UINT4 idxStart, idxEnd;
    UINT4 rxJ0Dly = 0;
    UINT4 i;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if ((sonetRate > 3) || ((sonetRate <= 2) && (sonetSlc > 15)) ||
        (((sonetRate == 3) && (sonetSlc != 0 && sonetSlc != 4 && sonetSlc != 8 &&
          sonetSlc != 12)))) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* RX_J0DLY value */
    switch (sonetRate) {
        case 0:
            /* STS-48 */
            rxJ0Dly = 1533;
            break;
        case 1:
            /* STS-12 */
            rxJ0Dly = 1501;
            break;
        case 2:
            /* STS-12 */
            rxJ0Dly = 1458;
            break;
        case 3:
            /* STS-192 */
            rxJ0Dly = 1533;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    idxStart = sonetSlc;
    idxEnd = (sonetRate == 3) ? (sonetSlc + 3) : sonetSlc;

    for (i = idxStart; i <= idxEnd; i++){
        addr = HYPHY20G_TSE_TOP_REG_J0_DLY_CFG_FOR_RX_SONET_SLC_OFFSET(i);
        msk = HYPHY20G_TSE_TOP_BIT_J0_DLY_CFG_FOR_RX_SONET_SLC_RX_J0DLY_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, rxJ0Dly);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnTseRxJ0DlyCfg */

/*******************************************************************************
**
**  sonetLnHyPhy20gSonetLnTxSfpSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function selects the SFP transmit source.
**
**  INPUTS:         *fileHndl - base address
**                  sfpPort   - SFP port
**                                0 to 15 - SFP ports 0 to 15
**                  src       - Transmit source for SFP port
**                                 0 to 15 - SONET slice 0 to 15
**                                16 to 23 - OTU1 Framer slice 0 to 7
**                                24 - LSCI slice with the same index as intf
**                                     parameter
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 sonetLnHyPhy20gSonetLnTxSfpSrcCfg(struct file *fileHndl, UINT4 sfpPort,
                                       UINT4 src)
{
    UINT4 reg, msk, val;
    UINT4 muxSel;
    INT4 result;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);

    /* argument checking */
    if (sfpPort > 15 || src > 24) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* select register */
    if (sfpPort >= 0 && sfpPort <= 5) {
        reg = HYPHY20G_SONET_LINE_TOP_REG_SFP_TX_SEL_0;
    } else if (sfpPort >= 6 && sfpPort <= 11) {
        reg = HYPHY20G_SONET_LINE_TOP_REG_SFP_TX_SEL_1;
    } else {
        reg = HYPHY20G_SONET_LINE_TOP_REG_SFP_TX_SEL_2;
    }

    switch (sfpPort) {
        case 0:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_0_TX_SFP_SEL0_MSK;
            break;
        case 1:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_0_TX_SFP_SEL1_MSK;
            break;
        case 2:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_0_TX_SFP_SEL2_MSK;
            break;
        case 3:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_0_TX_SFP_SEL3_MSK;
            break;
        case 4:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_0_TX_SFP_SEL4_MSK;
            break;
        case 5:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_0_TX_SFP_SEL5_MSK;
            break;
        case 6:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_1_TX_SFP_SEL6_MSK;
            break;
        case 7:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_1_TX_SFP_SEL7_MSK;
            break;
        case 8:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_1_TX_SFP_SEL8_MSK;
            break;
        case 9:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_1_TX_SFP_SEL9_MSK;
            break;
        case 10:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_1_TX_SFP_SEL10_MSK;
            break;
        case 11:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_1_TX_SFP_SEL11_MSK;
            break;
        case 12:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_2_TX_SFP_SEL12_MSK;
            break;
        case 13:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_2_TX_SFP_SEL13_MSK;
            break;
        case 14:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_2_TX_SFP_SEL14_MSK;
            break;
        case 15:
            msk = HYPHY20G_SONET_LINE_TOP_BIT_SFP_TX_SEL_2_TX_SFP_SEL15_MSK;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n", __FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }

    result = hyPhy20gRegFieldWrite(fileHndl, reg, msk, src);
    if (result) return result;

    /* Select LSCI or SONET_LINE data/clock source */
    muxSel = (src == 24) ? 1 : 0;
    reg = HYPHY20G_SERDES_SFP_TOP_REG_CFG_1;
    reg = HYPHY20G_SERDES_SFP_MTSB_SLICE_REG_OFFSET(reg, sfpPort);

    result = sysHyPhy20gRead(fileHndl, reg, &val);
    if (result) return result;

    /* Configure TX_DATA_MUX_SEL */
    msk = HYPHY20G_SERDES_SFP_TOP_BIT_CFG_1_TX_DATA_MUX_SEL_MSK;
    result = hyPhy20gVarFieldWrite(&val, msk, muxSel);

    result = sysHyPhy20gWrite(fileHndl, reg, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* sonetLnHyPhy20gSonetLnTxSfpSrcCfg */

/* end of file */
