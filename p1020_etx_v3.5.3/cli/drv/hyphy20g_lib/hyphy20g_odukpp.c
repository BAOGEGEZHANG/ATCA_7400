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
**  $Date: 2010-03-29 10:56:27 -0700 (Mon, 29 Mar 2010) $
**
**  $Revision: 7223 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_odukpp.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/

extern INT4 hyPhy20gOdukppOhRamCreateCtxt(struct file *fileHndl);
INT4 odukppHyPhy20gJatInit(struct file *fileHndl, UINT4 slice, UINT4 mode);
INT4 odukppHyPhy20gOdukppOdu1Init(struct file *fileHndl, UINT4 slice,
                                  UINT4 mode);
INT4 odukppHyPhy20gOdukppOdu1LineDataPathCfg(struct file *fileHndl, UINT4 slice,
			                                 UINT4 source);
INT4 odukppHyPhy20gOdukppOdu1SysDataPathCfg(struct file *fileHndl, UINT4 slice,
						                    UINT4 source);
INT4 odukppHyPhy20gOdukppOdu2Init(struct file *fileHndl, UINT4 slice,
                                  UINT4 mode);
INT4 odukppHyPhy20gOdukppOdu2DataPathCfg(struct file *fileHndl, UINT4 slice,
						                 UINT4 sysIf, UINT4 odukType);
INT4 odukppHyPhy20gSetTxODU12Sel(struct file *fileHndl, UINT4 slice,
			                     UINT4 odtuTsNum);

/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/
/*******************************************************************************
**
**  hyPhy20gOdukppDeviceInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the ODUKPP ODU2 framers and ODU1
**                  framers to be in low power - power savings mode or out
**                  of low power mode and ready for individual configuration.
**
**                  This function impacts all slices.  It is the caller's
**                  responsibility to ensure that this function is not
**                  called with enable = 0 while a slice is intended to be
**                  processing user data.
**
**                  This function must be called prior to configuring the
**                  operating mode of individual ODU2 or ODU1 framers.
**
**  INPUTS:         *fileHndl  - base address
**                  enable     - Operation Mode of ODUKPP ODU2 Slices 0 and 1
**                               and ODU1 Slices 0 to 7
**                               0 - In low power modes
**                               1 - Out of low power mode and reset to
**                                   default values
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppDeviceInit(struct file *fileHndl, UINT4 enable)
{
    INT4 result;
    UINT4 regAddr, msk, off;
    UINT4 zoneHiMsk;
    UINT4 zoneLowMsk;
    UINT4 zoneVal;
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
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_2;
    off = HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_ODUKPP_LOWPWR_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, off, 1);
    if(result) return result;

    /* reset at top level */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_2;
    off = HYPHY20G_TOP_BIT_SYS_SW_RST_2_ODUKPP_RST_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, off, 1);
    if(result) return result;

    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_2;
    off = HYPHY20G_TOP_BIT_SYS_SW_RST_2_ODUKPP_RST_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, off, 0);
    if(result) return result;

    /* terminate function if disabling subsystem */
    if(enable == 0){
        return HYPHY20G_SUCCESS;
    }

    /* remove lowpwr if subsystem to be enabled */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_2;
    off = HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_ODUKPP_LOWPWR_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, off, 0);
    if(result) return result;

    /* Set BYPASS_MODE_EN = 1 and Unconditionally set ODUKPP_EN to 1 to provide
       connectivity through framers */
    /* General Configuration Register at 0x4980000 */
    regAddr = HYPHY20G_ODUKPP_TOP_REG_CFG;
    msk = HYPHY20G_ODUKPP_TOP_BIT_CFG_ODUKPP_EN_MSK;
    off = HYPHY20G_ODUKPP_TOP_BIT_CFG_BYPASS_MODE_EN_OFF;

    result = sysHyPhy20gBitWrite(fileHndl, regAddr, off, 0);
    if (result) return result;

    result =  hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 0x3FF);
    if (result) return result;

    /* Configure ZONE_HIGH/LOW_THRESH */
    regAddr = HYPHY20G_ODUKPP_TOP_REG_CBRB_FIFO_MON_CFG_1;
    zoneHiMsk = HYPHY20G_ODUKPP_TOP_BIT_CBRB_FIFO_MON_CFG_1_ZONE_HIGH_THRESH_MSK;
    zoneLowMsk = HYPHY20G_ODUKPP_TOP_BIT_CBRB_FIFO_MON_CFG_1_ZONE_LOW_THRESH_MSK;

    result = sysHyPhy20gRead(fileHndl, regAddr, &zoneVal);
    if (result) return result;

    /* Value for ZONE_HIGH_THRESH */
    result = hyPhy20gVarFieldWrite(&zoneVal, zoneHiMsk, 2);
    if (result) return result;

    /* Value for ZONE_LOW_THRESH */
    result = hyPhy20gVarFieldWrite(&zoneVal, zoneLowMsk, 0);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, zoneVal);
    if (result) return result;

    /* Set ZONE_PERIOD for OTU1/OTU2 framers to default value = 0xFE010
       This value corresponds to one multiframe */
    regAddr = HYPHY20G_ODUKPP_TOP_REG_CBRB_FIFO_MON_CFG_2;
    msk = HYPHY20G_ODUKPP_TOP_BIT_CBRB_FIFO_MON_CFG_2_OTU2_ZONE_CHK_PERIOD_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 0xFE010);
    if (result) return result;

    regAddr = HYPHY20G_ODUKPP_TOP_REG_CBRB_FIFO_MON_CFG_3;
    msk = HYPHY20G_ODUKPP_TOP_BIT_CBRB_FIFO_MON_CFG_3_OTU1_ZONE_CHK_PERIOD_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, regAddr, msk, 0xFE010);
    if (result) return result;
    
    /* init registers for odukppHyPhy20gSetTxODU12Sel */
    result = sysHyPhy20gWrite(fileHndl, 0x49800b0,  0x00000000);
    if (result) return result;                                
    result = sysHyPhy20gWrite(fileHndl, 0x49800b4,  0x00000010);
    if (result) return result;                                
    result = sysHyPhy20gWrite(fileHndl, 0x49800b8,  0x00000020);
    if (result) return result;                                
    result = sysHyPhy20gWrite(fileHndl, 0x49800bc,  0x00000030);
    if (result) return result;                                
    result = sysHyPhy20gWrite(fileHndl, 0x49800c0,  0x00000040);
    if (result) return result;                                
    result = sysHyPhy20gWrite(fileHndl, 0x49800c4,  0x00000050);
    if (result) return result;                                
    result = sysHyPhy20gWrite(fileHndl, 0x49800c8,  0x00000060);
    if (result) return result;                                
    result = sysHyPhy20gWrite(fileHndl, 0x49800cc,  0x00000070);
    if (result) return result;

    /* Create context to store ODUKPP OH RAM resource */
    result = hyPhy20gOdukppOhRamCreateCtxt(fileHndl);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gOdukppDeviceInit */


/*******************************************************************************
**
**  hyPhy20gOdukppOdu1DataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures line and system side interfaces for
**                  an ODUKPP ODU1 Framer Slice and internal configuration of
**                  the ODUKPP ODU1 Framer.  Optionally it may disable the
**                  framer slice.
**
**                  When enabling for SONET system side payload, the associated
**                  ODUKPP JAT will be initalized.
**
**                  System side payload can only be directed to the OPI CBRB
**                  FIFO when the line side payload is associated with an
**                  OTN subsystem ODTU12 Mx Timeslot.
**
**                  When the lineside interface is an ODTU12 Mx Timeslot, the
**                  timeslot to contain the ODU1 signal may not be associated
**                  with any other ODU1 signal or any other OTN subsystem OTU1
**                  Framer.  The OTU1 Framer configuration will take
**                  presedence.  The Timeslot for the ODTU12 Mx may not be
**                  associated with any OTU1 framer in the OTN subsystem
**                  bit field OTU1_FRAMER_DATA_SELn.
**
**                  When the system side interface is to be sourced from an
**                  OPI port, the slice instance must have the same offset as
**                  the OPI port specified as source. Slice instance 0 to 7 maps
**                  to OPI Port 0 to 7.  This ensures that OTU1 Framer Tx FIFO
**                  centering logic is appropriately mapped to the pacer circuit
**                  that pulls data from CBRB FIFO.
**
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - ODU1 Framer instance:
**                               0 to 7 ODU1 Framer 1 to 8
**                  mode       - Operational mode of ODU1 Framer:
**                               0 - Disabled, Framer locally held in reset
**                               1 - Enabled, Framer out of reset and
**                                   configured with inputs provided by
**                                   lineIf and sysIf
**                  lineIf     - Resource connected to ODU1 Framer Line I/F:
**                               For mode set to 1:
**                               0 to 7 OPI Port 0 to 7 (CBRB FIFO 10 to 17)
**                               08 to 11 ODTU12 MX 0 Timeslot 0 to 3
**                               12 to 15 ODTU12 MX 1 Timeslot 0 to 3
**                               For mode set to 0:
**                               Set to any value in range 0 to 15
**                  sysIf      - Resource connected to ODU1 Framer System I/F:
**                               For mode set to 1:
**                               0 to 7 OPI Port 0 to 7 (CBRB FIFO 10 to 17)
**                               Note: only value for lineIf setting 8 to 15
**                               08 to 11 VCAT Interface 0 Ports 0 to 3
**                               12 to 15 VCAT Interface 1 Ports 0 to 3
**                               16 SONET slice (selected via SONET_LINE)
**                               For mode set to 0:
**                               Set to any value in range 0 to 16
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdu1DataPathCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 mode, UINT4 lineIf, UINT4 sysIf)
{
    INT4 result;

    if (slice > 7 || mode > 1 || lineIf > 15 || sysIf > 16) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Initialize ODUKPPODU1 */
    result = odukppHyPhy20gOdukppOdu1Init(fileHndl, slice, mode);
    if (result) return result;

    if (mode) {
        /* Configure Line side data path */
        result = odukppHyPhy20gOdukppOdu1LineDataPathCfg(fileHndl, slice, lineIf);
        if (result) return result;

        /* Configure System side data path */
        result = odukppHyPhy20gOdukppOdu1SysDataPathCfg(fileHndl, slice, sysIf);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;


} /* hyPhy20gOdukppOdu1DataPathCfg */



/*******************************************************************************
**
**  hyPhy20gOdukppOdu2DataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures line and system side interfaces for
**                  an ODUKPP ODU2 Framer Slice and internal configuration of
**                  the ODUKPP ODU2 Framer.  Optionally it may disable the
**                  framer slice.
**
**                  When enabling for SONET system side payload, the associated
**                  ODUKPP JAT will be initalized.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - ODU2 Framer instance:
**                               0 to 1 ODU2 Framer 0 to 1
**                  mode       - Operational mode of ODU2 Framer:
**                               0 - Disabled, Framer locally held in reset
**                               1 - Enabled, Framer out of reset and
**                                   configured with inputs provided by
**                                   lineIf and sysIf
**                  sysIf      - Resource connected to ODU2 Framer System I/F:
**                               For mode set to 1:
**                               0 to 1 VCAT I/F 0 and 1
**                               2 SONET STS-192 data (via SONET_LINE)
**                               3 HSCI 10GE data (via SONET_LINE)
**                               For mode set to 0:
**                               Set to any value in range 0 to 3
**                 odukType    - ODUk type being processed by ODU2 Framer:
**                               For mode set to 1:
**                               0 - ODU2
**                               1 - ODU2e
**                               2 - ODU1e
**                               3 - ODU2f
**                               4 - ODU1f
**                               For mode set to 0:
**                               Set to any value in range 0 to 4
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdu2DataPathCfg(struct file *fileHndl, UINT4 slice,
                                   UINT4 mode, UINT4 sysIf, UINT4 odukType)
{
    INT4 result;

    if (slice > 1 || mode > 1 || sysIf > 3 || odukType > 4)
    {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Initialize ODUKPP_ODU2 */
    result = odukppHyPhy20gOdukppOdu2Init(fileHndl, slice, mode);
    if (result) return result;

    if (mode) {
        /* Configure System side data path */
        if ((sysIf == 3) && (odukType != 0)) {
            /* Set sysIf depending on odukType */
            sysIf = ((odukType % 2) == 0)?(sysIf + 1):sysIf;
        }
        DBG_PRINT("%s, %s, %d, odukType = %u, sysIf = %u\n",__FILE__, __FUNCTION__, __LINE__,
                  (unsigned int)odukType, (unsigned int)sysIf);
        result = odukppHyPhy20gOdukppOdu2DataPathCfg(fileHndl, slice, sysIf,
                                                     odukType);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;


} /* hyPhy20gOdukppOdu2DataPathCfg */



/*******************************************************************************
**
**  hyPhy20gOdukppOdu2SetLoopback
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
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdu2SetLoopback(struct file *fileHndl, UINT4 slice,
                                   UINT4 loopType, UINT4 enable)
{
    /* variable declaration */
    INT4 result;
    UINT4 rxAddr, txAddr;
    UINT4 rxOffset, txOffset;

    /* argument checking */
    if (slice > 1 || loopType > 3 || enable > 1 ||
        (loopType == 0) || (loopType == 2)) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    txAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(
                           HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_GLBL_CTL, slice);
    rxAddr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(
                           HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_GLBL_CTL, slice);

    txOffset =
        HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_TX_TERMINAL_LOOPBACK_OFF;
    rxOffset =
        HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_TERMINAL_LOOPBACK_OFF;

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

} /* hyPhy20gOdukppOdu2SetLoopback */


/*******************************************************************************
**
**  hyPhy20gOdukppOdu1SetLoopback
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
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppOdu1SetLoopback(struct file *fileHndl, UINT4 slice,
                                   UINT4 loopType, UINT4 enable)
{
    INT4 result;
    UINT4 rxAddr, txAddr;
    UINT4 rxOffset, txOffset;

    /* argument checking */
    if (slice > 7 || loopType > 3 || enable > 1 ||
        (loopType == 0) || (loopType == 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* variable declaration */
    txAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                           HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_GLBL_CTL, slice);
    rxAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(
                           HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_GLBL_CTL, slice);

    txOffset =
        HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_TX_TERMINAL_LOOPBACK_OFF;
    rxOffset =
        HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_TERMINAL_LOOPBACK_OFF;

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

} /* hyPhy20gOdukppOdu1SetLoopback */

/*******************************************************************************
**
**  hyPhy20gOdukppCbrbFifoMonCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the CBRB FIFO Montioring settings.
**
**                  CBRB FIFO Monitoring is used at ODUk packetized data stream
**                  startup to center CBRB FIFO for the data stream flowing
**                  from CBRB to ODUKPP subsystem.
**
**                  In addition to this startup centering the reading from
**                  CBRB is controlled by processing packet size variation
**                  indications from the PMM Monitor associated with the
**                  CBRB port towards ODUKPP subsystem.  Data will be read
**                  from CBRB at the ODUk stream rate.
**
**                  Packet size monitoring is the primary CBRB to ODUKPP
**                  timing method, the CBRB FIFO Monitoring feature provides
**                  the course adjustment required at startup and would not
**                  be expected to occur during normal operation provided
**                  upstream data source provides a continuous ODUk stream.
**
**                  When line side interface is configured for ODTU12 MUX,
**                  this function must be called with zoneChkEnbl == 0 to
**                  disable CBRB FIFO monitoring in ODUKPP.
**
**  INPUTS:         *fileHndl       - base address
**                  slice           - OTU framer instance
**                                    0 to 9, where:
**                                    0 to 1 maps to ODU2 framer 0 to 1
**                                    2 to 9 maps to ODU1 framer 0 to 7
**                  zoneChkEnbl     - zone check enable bit to control ODUKPP's
**                                    monitoring of the FIFO levels in CBRB
**                                    0 - disable
**                                    1 - enable
**
**
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
INT4 hyPhy20gOdukppCbrbFifoMonCfg(struct file *fileHndl, UINT4 slice,
                                  UINT4 zoneChkEnbl)
{
    INT4 result;
    UINT4 cfg1Addr;
    UINT4 zoneChkEnOff;

    /* argument checking */
    if (slice > 9 || zoneChkEnbl > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine the offset of ZONE_CHK_EN for correct slice */
    zoneChkEnOff = HYPHY20G_ODUKPP_TOP_BIT_CBRB_FIFO_MON_CFG_1_ZONE_CHK_EN_OFF
                    + slice;

    /* Enable monitoring */
    cfg1Addr = HYPHY20G_ODUKPP_TOP_REG_CBRB_FIFO_MON_CFG_1;
    result = sysHyPhy20gBitWrite(fileHndl, cfg1Addr, zoneChkEnOff, zoneChkEnbl);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppCbrbFifoMonCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppCbrbFifoZoneChkPrdCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the zone check period setting in
**                  which the ODUKPP subsystem checks the CBRB FIFO level.
**                  Zone check period determines whether it should increase or
**                  decrease the read rate.
**
**                  Prior to calling this function, hyPhy20gOdukppCbrbFifoMonCfg
**                  must be called to enable ZONE_CHK_EN for zone check period
**                  configuration to be valid.
**
**
**  INPUTS:         *fileHndl       - base address
**                  frmType         - OTU framer type
**                                    0 - OTU1 framers
**                                    1 - OTU2 framers
**                  zoneChkPeriod   - zone check period to determine and update
**                                    the read rate. The period is in terms of
**                                    OTU2_NOM_CLK/4 cycles (83.5MHz) for ODU1
**                                    and OTU2_NOM_CLK cycles(334MHz) for ODU2.
**                                    The valid value can be the number of
**                                    clocks - 1.
**                                    Legal range: 0 to 0x3FFFFFFF
**                                    Default setting: 0xFE010 for OTU1 and OTU2
**                                                     framers
**                                    Note: required setting TBD
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppCbrbFifoZoneChkPrdCfg(struct file *fileHndl, UINT4 frmType,
                                         UINT4 zoneChkPeriod)
{
    /* variable declaration */
    UINT4 addr, msk;
    INT4 result;

    /* argument checking */
    if (frmType > 1 || zoneChkPeriod > 0x3FFFFFFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine registers and mask */
    if (frmType == 1) {
        /* OTU2 framers */
        addr = HYPHY20G_ODUKPP_TOP_REG_CBRB_FIFO_MON_CFG_2;
        msk = HYPHY20G_ODUKPP_TOP_BIT_CBRB_FIFO_MON_CFG_2_OTU2_ZONE_CHK_PERIOD_MSK;
    } else {
        /* OTU1 framers */
        addr = HYPHY20G_ODUKPP_TOP_REG_CBRB_FIFO_MON_CFG_3;
        msk = HYPHY20G_ODUKPP_TOP_BIT_CBRB_FIFO_MON_CFG_3_OTU1_ZONE_CHK_PERIOD_MSK;
    }

    /* Set *_ZONE_CHK_PERIOD */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, zoneChkPeriod);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppCbrbFifoZoneChkPrdCfg */

/*******************************************************************************
**
**  hyPhy20gOdukppOdu1DataAlign
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs alignment on a group of ODU1 links.
**
**                  This is required to deskew links that are members of a VCAT
**                  group. It will ensure that skew generated prior to the OPI
**                  PMM timestamp insertion on the ODUk will be minimized. If
**                  this operation is not performed, skew of 1.5 useconds may be
**                  introduced.
**
**                  As part of skew removal, this function performs software
**                  reset and VCAT ODUk data stream enabling for a user
**                  specified set of ODU1 links associated with a VCAT slice.
**
**
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - 0 - VCAT 0
**                               1 - VCAT 1
**                  alignMsk   - mask of VCAT ODU1 slices to deskew within
**                               VCAT slice
**                               - bit 0: VCAT ODU1 0
**                               - bit 1: VCAT ODU1 1
**                               - bit 2: VCAT ODU1 2
**                               - bit 3: VCAT ODU1 3
**                                 Refer to DESCRIPTION
**                  odukPpSlc0 - ODUKPP subsystem slice associated with
**                               VCAT ODU1 0
**                               Legal range 0 to 7 for ODUKPP OTU1 Framer 0 - 7
**                  odukPpSlc1 - ODUKPP subsystem slice associated with
**                               VCAT ODU1 1
**                               Legal range 0 to 7 for ODUKPP OTU1 Framer 0 - 7
**                  odukPpSlc2 - ODUKPP subsystem slice associated with
**                               VCAT ODU1 2
**                               Legal range 0 to 7 for ODUKPP OTU1 Framer 0 - 7
**                  odukPpSlc3 - ODUKPP subsystem slice associated with
**                               VCAT ODU1 3
**                               Legal range 0 to 7 for ODUKPP OTU1 Framer 0 - 7
**
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
INT4 hyPhy20gOdukppOdu1DataAlign(struct file *fileHndl, UINT4 slice,
                                 UINT4 alignMsk, UINT4 odukPpSlc0,
                                 UINT4 odukPpSlc1, UINT4 odukPpSlc2,
                                 UINT4 odukPpSlc3)
{
    /* variable declaration */
    INT4 result;
    UINT4 addr, addr0, addr1, addr2, addr3;
    UINT4 off, val;
    UINT4 enblMsk, msMsk;
    UINT4 otu1CfgAddr;

    /* argument checking */
    if (slice > 1 || alignMsk > 0xF || odukPpSlc0 > 7 || odukPpSlc1 > 7 ||
        odukPpSlc2 > 7 || odukPpSlc3 > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Unconditionally set SLICE_ENABLE and MASTER_SLAVE to 0 for user
       specified links */
    otu1CfgAddr = HYPHY20G_VCAT_LCAS_TVCPO_REG_OTU1_MD_SLICES_CFG;
    enblMsk = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_SLICE_ENABLE_MSK;
    msMsk = HYPHY20G_VCAT_LCAS_TVCPO_BIT_OTU1_MD_SLICES_CFG_MASTER_SLAVE_MSK;

    otu1CfgAddr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(otu1CfgAddr,
                                                                slice);

    /* Read the current setting in SLICE_ENABLE */
    result = sysHyPhy20gRead(fileHndl, otu1CfgAddr, &val);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&val, enblMsk, ((~alignMsk) & val));
    if(result) return result;
    DBG_PRINT("%s, %s, %d, val = 0x%lx\n",__FILE__, __FUNCTION__, __LINE__, val);

    result = hyPhy20gVarFieldWrite(&val, msMsk, (~alignMsk) & val);
    if(result) return result;
    DBG_PRINT("%s, %s, %d, val = 0x%lx\n",__FILE__, __FUNCTION__, __LINE__, val);

    /* Write to OTU1_MODE_SLICES_CFG_REG */
    result = sysHyPhy20gWrite(fileHndl, otu1CfgAddr, val);
    if (result) return result;

    /* Reset OTU1 framers for affected slice */
    off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_TX_FIFO_RESET_OFF;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_GLBL_CTL;
    addr0 = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, odukPpSlc0);

    if ( (alignMsk & (1 << 0)) == 1 ) { /* VCAT ODU1 0 */
        result = sysHyPhy20gBitWrite(fileHndl, addr0, off, 1);
        if (result) return result;
    }

    if ( (alignMsk & (1 << 1)) == 0x02 ) { /* VCAT ODU1 1 */
        addr1 = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, odukPpSlc1);
        result = sysHyPhy20gBitWrite(fileHndl, addr1, off, 1);
        if (result) return result;
    }

    if ( (alignMsk & (1 << 2)) == 0x04) { /* VCAT ODU1 2 */
        addr2 = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, odukPpSlc2);
        result = sysHyPhy20gBitWrite(fileHndl, addr2, off, 1);
        if (result) return result;
    }

    if ( (alignMsk & (1 << 3)) == 0x08) { /* VCAT ODU1 3 */
        addr3 = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, odukPpSlc3);
        result = sysHyPhy20gBitWrite(fileHndl, addr3, off, 1);
        if (result) return result;
    }

    /* Allow at least 1.5uS delay to settle down FIFO_DEPTH */
    result = sysHyPhy20gUsDelay(2);
    if (result) return result;

    /* Set SLICE_ENABLE and MASTER_SLAVE to 1 for affected links */
    /* Read the current setting in SLICE_ENABLE */
    result = sysHyPhy20gRead(fileHndl, otu1CfgAddr, &val);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&val, enblMsk, (alignMsk | val));
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&val, msMsk, (alignMsk | val));
    if(result) return result;

    /* Write to OTU1_MODE_SLICES_CFG_REG */
    result = sysHyPhy20gWrite(fileHndl, otu1CfgAddr, val);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppOdu1DataAlign */

/*******************************************************************************
**
**  hyPhy20gOdukppPacerModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the pacer data request mode for
**                  an ODUKPP ODU1 or ODU2 slice.  Pacer data request mode is
**                  required for ODUKPP ODU1 or ODU2 slices processing ODU1 or
**                  ODU2 rate signals.  It is not required for ODU2e/1e/2f/1f
**                  rate signals.
**
**                  The pacer data request mode is set up as a function of the
**                  OPI PMM output signal type.
**
**                  If OPI PMM output signal type is formatted for usage by
**                  source C of ACB, the request generation towards receive OPI
**                  will pull any available data from CBRB. If OPI PMM signal
**                  type is formatted for usage by source B of ACB, the request
**                  generation towards receive OPI will correspond to ODU1/ODU2
**                  rate.
**
**                  This function can be called any time after
**                  hyPhy20gOdukppOdu1DataPathCfg or
**                  hyPhy20gOdukppOdu2DataPathCfg.
**
**                  The requirement for this API is obsolete.  Originally sync
**                  and async client demapping used a different timing setup at
**                  the client ACB (sync demap used source C and async demap
**                  used source B + C timing mode).  Both sync and async client
**                  demapping of ODU1 and ODU2 signals (not including
**                  overclocked ODU2e/1e/2f/1f) require the use of source B + C
**                  mode timing.
**
**                  The pacer is set up as part of hyPhy20gOdukppOdu1DataPathCfg
**                  and hyPhy20gOdukppOdu2DataPathCfg for the pacer to use
**                  source B format PMM data for the pacer.
**
**                  There is no requirement to call this API as part of datapath
**                  setup.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - ODUk Framer instance:
**                                0 to 7 ODU1 Framer 1 to 8
**                                8 to 9 ODU2 Framer 1 to 2
**                  sourceType  - PMM Output Signal Format Type
**                                0 - Reserved
**                                1 - Source B of ACB
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gOdukppPacerModeCfg(struct file *fileHndl, UINT4 slice,
                                UINT4 sourceType)
{
    /* Variable declaration */
    UINT4 denRegAddr, numRegAddr;
    UINT4 denMsk, numMsk;
    UINT4 rxReqDen;
    UINT4 rxReqNum;
    INT4 result;

    /* argument checking */
    if (slice > 9 || sourceType > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Determine registers and masks */
    if (slice < 8) { /* ODU1 rate */
        denRegAddr = HYPHY20G_ODUKPP_TOP_REG_OTU1_RX_REQ_CTL_DENM_OFFSET(slice);
        numRegAddr = HYPHY20G_ODUKPP_TOP_REG_OTU1_RX_REQ_CTL_NUMR_OFFSET(slice);

        denMsk = HYPHY20G_ODUKPP_TOP_BIT_OTU1_RX_REQ_CTL_DENM_OTU1_DENOMINATOR_MSK;
        numMsk = HYPHY20G_ODUKPP_TOP_BIT_OTU1_RX_REQ_CTL_NUMR_OTU1_NUMERATOR_MSK;
    } else { /* ODU2 rate */
        denRegAddr = HYPHY20G_ODUKPP_TOP_REG_OTU2_RX_REQ_CTL_DENM_OFFSET(slice - 8);
        numRegAddr = HYPHY20G_ODUKPP_TOP_REG_OTU2_RX_REQ_CTL_NUMR_OFFSET(slice - 8);

        denMsk = HYPHY20G_ODUKPP_TOP_BIT_OTU2_RX_REQ_CTL_DENM_OTU2_DENOMINATOR_MSK;
        numMsk = HYPHY20G_ODUKPP_TOP_BIT_OTU2_RX_REQ_CTL_NUMR_OTU2_NUMERATOR_MSK;
    }

    DBG_PRINT("%s, %s, %d, denRegAddr = 0x%lx\n", __FILE__, __FUNCTION__, __LINE__, denRegAddr);
    DBG_PRINT("%s, %s, %d, numRegAddr = 0x%lx\n", __FILE__, __FUNCTION__, __LINE__, numRegAddr);

    if (sourceType == 0) { /* source C of ACB */
        rxReqNum = 3;
        rxReqDen = 1;
    } else { /* source B of ACB */
        if (slice < 8) { /* ODU1 rate */
            rxReqNum = 18881;
            rxReqDen = 20230;
        } else { /* ODU2 rate */
            rxReqNum = 239;
            rxReqDen = 255;
        }
    }

    /* Write to OTUn_NUMERATOR */
    result = hyPhy20gRegFieldWrite(fileHndl, numRegAddr, numMsk, rxReqNum);
    if (result) return result;

    /* Write to OTUn_DENOMINATOR */
    result = hyPhy20gRegFieldWrite(fileHndl, denRegAddr, denMsk, rxReqDen);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gOdukppPacerModeCfg */



/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/


/*******************************************************************************
**
**  odukppHyPhy20gJatInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    None.
**
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - JAT 622 instance:
**                              0 to 9, where:
**                                  0 to 1 maps to ODU2 framer 0 to 1
**                                  2 to 9 maps to ODU1 framer 0 to 7
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
INT4 odukppHyPhy20gJatInit(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    UINT4 addr;
    UINT4 swRstOff;
    UINT4 msk;
    INT4 result;

    /* argument checking */
    if (slice > 9 || mode > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_ODUKPP_TOP_REG_CFG;
    swRstOff = HYPHY20G_ODUKPP_TOP_BIT_CFG_SW_RST_JAT_OFF + slice;

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
    addr = HYPHY20G_ODUKPP_JAT_JAT_TSB_SLICE_REG_OFFSET(
                                    HYPHY20G_ODUKPP_JAT_JAT_REG_PLL_CFG, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                              HYPHY20G_ODUKPP_JAT_JAT_BIT_PLL_CFG_PWRDN_OFF, 0);
    if (result) return result;

    /* Poll the DLL_RUN bit to ensure it is set to 1 */
    addr = HYPHY20G_ODUKPP_JAT_JAT_REG_DLL_STAT;
    addr = HYPHY20G_ODUKPP_JAT_JAT_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_JAT_JAT_BIT_DLL_STAT_DLL_RUN_MSK;
    result = sysHyPhy20gPollBit(fileHndl, addr, msk, 1, 5000);
    if (result) return result;

    return HYPHY20G_SUCCESS;

} /* odukppHyPhy20gJatInit */


/*******************************************************************************
**
**  odukppHyPhy20gOdukppOdu1Init
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables an ODUKPP framer slice.
**                  When enabling it performs internal framer configuration
**                  independent of system and line side interface resource
**                  selection
**
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTU1 Framer instance:
**                               0 to 7 OTU1 Framer 1 to 8
**                  mode       - Operation Mode of OTU1 Framer slice
**                               0 - Disabled
**                               1 - Enabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 odukppHyPhy20gOdukppOdu1Init(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    INT4 result;
    UINT4 addr, val, msk;
    UINT4 swRstOff, off;
    UINT4 otuFrmType;
    UINT4 txClkCfgOff, rxClkCfgOff;
    UINT4 txSysClkCfg = 0, rxSysClkCfg = 0;
    UINT4 fasOff;

    /* argument checking */
    if (slice > 7 || mode > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_ODUKPP_TOP_REG_CFG;
    swRstOff = HYPHY20G_ODUKPP_TOP_BIT_CFG_SW_RST_OTU1_FRAMER_OFF + slice;
    txClkCfgOff = HYPHY20G_ODUKPP_TOP_BIT_CLK_CFG_OTU1_TX_SYS_CLK_CFG_OFF + slice;
    rxClkCfgOff = HYPHY20G_ODUKPP_TOP_BIT_CLK_CFG_OTU1_RX_SYS_CLK_CFG_OFF + slice;

    /* disable operation */
    if (mode == 0) {
        /* hold OTU1_FRAMER block in reset */
        result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 1);
        if (result) return result;
        return HYPHY20G_SUCCESS;
    }

    /*
     * enable operation
     */
    /* ensure OTU1_FRAMER block is reset */
    result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 1);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 0);
    if (result) return result;

    /*
     * configure an OTU1 Framer Slice
     */
    switch(mode)
    {
        case 0: /* disable operation to use SONET/SDH frames */
            txSysClkCfg = 1;
            rxSysClkCfg = 1;
            otuFrmType  = 0;
            break;

        case 1:
            txSysClkCfg = 0;
            rxSysClkCfg = 0;
            otuFrmType  = 0;
            break;

        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* configure system side clock source so that OTU1 framer registers may
       be written */
    /* Set the OTU1_TX_SYS_CLK_CFG[7:0] */
    addr = HYPHY20G_ODUKPP_TOP_REG_CLK_CFG;
    result = sysHyPhy20gBitWrite(fileHndl, addr, txClkCfgOff, txSysClkCfg);
    if (result) return result;

    /* Set the OTU1_RX_SYS_CLK_CFG[7:0] */
    result = sysHyPhy20gBitWrite(fileHndl, addr, rxClkCfgOff, rxSysClkCfg);
    if (result) return result;

    /*
     * configure the Tx OTU1 framer
     */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_GLBL_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_OTU_FRAME_TYPE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, otuFrmType);
    if (result) return result;

    /*
     * configure the Rx OTU1 framer
     */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_GLBL_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_OTUK_FRAME_TYPE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, otuFrmType);
    if (result) return result;

    /* apply non-default settings to block */
    /* configure RX_FIFO_HIGH_THRESHOLD_VALUE to 0x190 (400d) */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OPU_FIFO_HI_TH;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OPU_FIFO_HI_TH_RX_FIFO_HIGH_THRESHOLD_VALUE_MSK;

    val = 0x190;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure RX_FIFO_LOW_THRESHOLD_VALUE to 0xc8 (200d) */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OPU_FIFO_LOW_TH;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OPU_FIFO_LOW_TH_RX_FIFO_LOW_THRESHOLD_VALUE_MSK;
    val = 0xc8;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure TX_ERR_SYNC_MODE to 1 */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_CNT_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_CNT_CTL_TX_ERR_SYNC_MODE_MSK;

    val = 0x1;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure RX_ERR_SYNC_MODE to 1 */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_GLBL_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
    val = 0x1;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* set OTU1_OVERWRITE_FAS[n] to 1 */
    addr = HYPHY20G_ODUKPP_TOP_REG_RX_OVERWRITE_FAS_MFAS;
    fasOff = HYPHY20G_ODUKPP_TOP_BIT_RX_OVERWRITE_FAS_MFAS_OTU1_OVERWRITE_FAS_OFF;

    msk = 1 << slice << fasOff;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x1);
    if (result) return result;

    /* set N_LOF_LOF_COUNT to 61 (0x3D) to ensure LOF entered 3ms after OOF */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_FAU_CTL_1;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_FAU_CTL_1_N_LOF_LOF_COUNT_MSK;
    val = 0x3D;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* set LOF_N_LOF_COUNT to 61 (0x3D) to ensure LOF entered 3ms after OOF */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_FAU_CTL_2;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_FAU_CTL_2_LOF_N_LOF_COUNT_MSK;
    val = 0x3D;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* set N_LOM_LOM_COUNT to 61 (0x3D) to ensure LOM entered 3ms after OOM */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_FA_MFAU_CTL_1;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_FA_MFAU_CTL_1_N_LOM_LOM_COUNT_MSK;
    val = 0x3D;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* set AUTO_OTUK_AIS_IAE to 0 */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_AUTO_AIS_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_OTUK_AIS_IAE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0);
    if (result) return result;

    /* set AUTO_ODUK_AIS_FIFO_ERROR to 1 */
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_ODUK_AIS_FIFO_ERROR_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if (result) return result;
        
    /* set MIN_AUTO_AIS_INSERT_MULTIFRAME_COUNT to 1 */
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_AUTO_AIS_CTL_MIN_AUTO_AIS_INSERT_MULTIFRAME_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if (result) return result;
    
    /* set TX_IAE_MULTIFRAME_COUNT to 16 to comply with G.798 */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_TCMI_AUTO_BDI_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_IAE_MULTIFRAME_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x10);
    if (result) return result;
    
    /* set RX_IAE_MULTIFRAME_COUNT to 16 to comply with G.798 */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_IAE_MULTIFRAME_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x10);
    if (result) return result;
          
    return HYPHY20G_SUCCESS;

} /* odukppHyPhy20gOdukppOdu1Init */

/*******************************************************************************
**
**  odukppHyPhy20gOdukppOdu1LineDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the bidirectional line side data
**                  path for an ODUKPP ODU1 Framer Slice.
**
**                  The line side data sources are:
**                     OPI CBRB FIFO 10 to 17
**                     ODTU12 Multiplexer 0 or 1 timeslot 0 to 3
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTU1 Framer instance:
**                               0 to 7 - OTU1 Framer 0 to 7
**                  source     - Date source for ODU1 Framer
**                               0 to 7 OPI Port 0 to 7 (CBRB FIFO 10 to 17)
**                               08 to 11 ODTU12 MX 0 Timeslot 0 to 3
**                               12 to 15 ODTU12 MX 1 Timeslot 0 to 3
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 odukppHyPhy20gOdukppOdu1LineDataPathCfg(struct file *fileHndl, UINT4 slice,
			                                 UINT4 source)
{
    INT4 result;
    UINT4 rxAddr, txAddr, val, msk, addr;
    UINT4 txOduSelMsk = 0, rxOduSelMsk;
    UINT4 txLnSinkSelMsk;
    UINT4 txOdu12SelMsk;
    UINT4 txOduSelVal = 0, dir;
    UINT4 index;
    UINT4 txOdu12Addr;
    UINT4 indexTxOdu12;


    /* argument checking */
    if (slice > 7 || source > 15) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Set mask for LINE_SINK_SEL */
    txLnSinkSelMsk = HYPHY20G_ODUKPP_TOP_BIT_TX_LN_SIDE_OTU1_MUX_SEL_TX_LINE_SINK_SEL_MSK;

    /* Set register addresses for each slice */
    index = (source > 7) ? slice:source;
    indexTxOdu12 = (source > 7) ? source-8:source;
    txOdu12Addr = HYPHY20G_ODUKPP_TOP_REG_TX_LN_SIDE_OTU1_MUX_SEL_OFFSET(indexTxOdu12);
    txAddr = HYPHY20G_ODUKPP_TOP_REG_TX_LN_SIDE_OTU1_MUX_SEL_OFFSET(index);
    rxAddr = HYPHY20G_ODUKPP_TOP_REG_RX_LN_SIDE_OTU1_MUX_SEL_OFFSET(index);

    /* RX_LINE_OTU1_SEL mask */
    rxOduSelMsk = HYPHY20G_ODUKPP_TOP_BIT_RX_LN_SIDE_OTU1_MUX_SEL_RX_LINE_OTU1_SEL_MSK;
    txOduSelMsk = HYPHY20G_ODUKPP_TOP_BIT_TX_LN_SIDE_OTU1_MUX_SEL_TX_LINE_ODU_SEL_MSK;
    txOdu12SelMsk = HYPHY20G_ODUKPP_TOP_BIT_TX_LN_SIDE_OTU1_MUX_SEL_TX_LINE_ODU12_SEL_MSK;

    /* Set masks and data source values */
    if (source < 8) {
        /* OPI Port 0 to 7 (CBRB FIFO 10 to 17) */
        dir = 0;
    } else {
        /* ODTU12 MX 0 or 1: Timeslot 0 to 3 */
        dir = 0x8;
    }

    /* Set the RX_LINE_SIDE_OTU1_MUX_SEL */
    result = hyPhy20gRegFieldWrite(fileHndl, rxAddr, rxOduSelMsk, source);
    if (result) return result;
    
    /* Determine sink for each TX ODU1 Framer */
    if (dir == 0) {
        val = ((source < 8) ? 0:1);
        result = hyPhy20gRegFieldWrite(fileHndl, txAddr, txLnSinkSelMsk, val);
        if (result) return result;
    }

    /* Set TX_LINE_ODU_SEL */
    txOduSelVal = dir | slice;
    result = hyPhy20gRegFieldWrite(fileHndl, txAddr, txOduSelMsk, txOduSelVal);
    if (result) return result;

    if (dir == 0x8) {
        result = odukppHyPhy20gSetTxODU12Sel(fileHndl, slice, source-8);
        if (result) return result;

        val = ((source < 8) ? 0:1);
        result = hyPhy20gRegFieldWrite(fileHndl, txAddr, txLnSinkSelMsk, val);
        if (result) return result;
    }

    /* Disable TX/RX Scrambling */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_GLBL_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_TX_SCRAMBLE_ENABLE_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0);
    if (result) return result;

    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_GLBL_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_SCRAMBLE_ENABLE_MSK;

    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0);
    if (result) return result;


    return HYPHY20G_SUCCESS;

} /* odukppHyPhy20gOdukppOdu1LineDataPathCfg */

/*******************************************************************************
**
**  odukppHyPhy20gOdukppOdu1SysDataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the bidirectional system side data
**                  path for an ODUKPP ODU1 Framer Slice.  Will configure the
**                  JAT_622E for CBR2G5 SONET system side payloads.
**
**                  The system side data sources are are:
**                     OPI CBRB FIFO 10 to 17
**                     VCAT Interface 0 and 1 port 0 to 3
**                     CBR2G5 from SONET subsystem
**
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTU1 Framer instance:
**                               0 to 7 - OTU1 Framer 0 to 7
**                  source     - Operation Mode of OTU1 Framer slice
**                               0 to 7 OPI Port 0 to 7 (CBRB FIFO 10 to 17)
**                               Note: only value for lineIf setting 8 to 15
**                               08 to 11 VCAT Interface 0 Ports 0 to 3
**                               12 to 15 VCAT Interface 1 Ports 0 to 3
**                               16 SONET slice (selected via SONET_LINE)
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 odukppHyPhy20gOdukppOdu1SysDataPathCfg(struct file *fileHndl, UINT4 slice,
						                    UINT4 source)
{
    /* variable declaration */
    INT4 result;
    UINT4 addr;
    UINT4 dataSelOff, txClkCfgOff, rxClkCfgOff;
    UINT4 dataSel = 0, txClkCfg = 0, txStrMode = 0, txMapping = 0;
    UINT4 rxClkCfg = 0, rxStrMode = 0, rxMapping = 0;
    UINT4 otuBlank = 0, oduBlank = 0, opuBlank = 0;
    UINT4 jatEn = 0;
    UINT4 insFas = 0, insMfas = 0;
    UINT4 txSysMxSelReg, txSysMxSelMsk;
    UINT4 txSysMxSel = 0;
    UINT4 rxSysMxSelReg;
    UINT4 rxSysMxSelOff1, rxSysMxSelOff0;
    UINT4 msk, off, val;
    UINT4 dequeModeReg, dequeModeOff, dequeModeVal = 0;
    UINT4 fracEnOff;

    /* argument checking */
    if (slice > 7 || source > 16 || (source < 8 && slice != source)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    dataSelOff  = HYPHY20G_ODUKPP_TOP_BIT_SYS_SIDE_DAT_SEL_OTU1_SYS_DATA_SEL_OFF
                  + slice;
    txClkCfgOff = HYPHY20G_ODUKPP_TOP_BIT_CLK_CFG_OTU1_TX_SYS_CLK_CFG_OFF +
                    slice;
    rxClkCfgOff = HYPHY20G_ODUKPP_TOP_BIT_CLK_CFG_OTU1_RX_SYS_CLK_CFG_OFF +
                    slice;
    txSysMxSelReg = HYPHY20G_ODUKPP_TOP_REG_TX_SYS_SIDE_OTU1_MUX_SEL_OFFSET(slice);
    txSysMxSelMsk = HYPHY20G_ODUKPP_TOP_BIT_TX_SYS_SIDE_OTU1_MUX_SEL_TX_SYS_OTU1_SEL_MSK;
    rxSysMxSelReg = HYPHY20G_ODUKPP_TOP_REG_RX_SYS_SIDE_OTU1_MUX_SEL;

    rxSysMxSelOff1 = HYPHY20G_ODUKPP_TOP_BIT_RX_SYS_SIDE_OTU1_MUX_SEL_RX_SYS_OTU2_1_SEL_OFF;
    rxSysMxSelOff0 = HYPHY20G_ODUKPP_TOP_BIT_RX_SYS_SIDE_OTU1_MUX_SEL_RX_SYS_OTU2_0_SEL_OFF;

    /* Determine data source */
    if (source < 16) {
        /* OPI Port 0 to 7 (CBRB FIFO 10 to 17) and
           VCAT Interface #0/1 Ports 0 to 3*/
        dataSel = 0;
        txClkCfg = 0;
        txStrMode = 1;
        txMapping = 0;
        rxClkCfg = 0;
        rxStrMode = 1;
        rxMapping = 0;

        otuBlank = oduBlank = opuBlank = 0;

        txSysMxSel = (source <= 7) ? (source + 8):(source - 8);

        jatEn = 0;

        dequeModeVal = 1;
    } else if (source == 16) {
        /* SONET slice (selected via SONET_LINE) */
        dataSel = 1;
        txClkCfg = 1;
        txStrMode = 0;
        txMapping = 0;
        rxClkCfg = 1;
        rxStrMode = 0;
        rxMapping = 1;

        txSysMxSel = 0;

        otuBlank = oduBlank = opuBlank = 0;

        jatEn = 1;

        dequeModeVal = 0;
    }

    insFas = insMfas = 1;

    /* Set OTU1_RX_SYS_DEQUE_MODE_SEL */
    dequeModeReg = HYPHY20G_ODUKPP_TOP_REG_RX_REQ_MD_CTL;
    dequeModeOff = HYPHY20G_ODUKPP_TOP_BIT_RX_REQ_MD_CTL_OTU1_RX_SYS_DEQUE_MODE_SEL_OFF
                    + slice;
    fracEnOff = HYPHY20G_ODUKPP_TOP_BIT_RX_REQ_MD_CTL_OTU1_RX_FRAC_DIV_EN_OFF +
                slice;

    result = sysHyPhy20gBitWrite(fileHndl, dequeModeReg, dequeModeOff,
                                 dequeModeVal);
    if (result) return result;

    /* Set OTU1_RX_FRAC_DIV_EN to 0 for corresponding slice if in ODTU12 mode */
    if (source < 8) {
        result = sysHyPhy20gBitWrite(fileHndl, dequeModeReg, fracEnOff, 0);
        if (result) return result;
    }

    /* Set RX_SYS_OTU2_*_SEL */
    if (source > 7) { /* For VCAT Interface #0/1 */
        off = (source > 7 && source < 12) ? rxSysMxSelOff0:rxSysMxSelOff1;
        off = off + ((source % 4) * 3);
        msk = 0x7 << off;
        result = hyPhy20gRegFieldWrite(fileHndl, rxSysMxSelReg, msk, slice);
        if (result) return result;
    }

    /* Set TX_SYS_OTU1_SEL */
    result = hyPhy20gRegFieldWrite(fileHndl, txSysMxSelReg, txSysMxSelMsk,
                                   txSysMxSel);
    if (result) return result;

    /* Set OTU1_SYS_DATA_SEL */
    addr = HYPHY20G_ODUKPP_TOP_REG_SYS_SIDE_DAT_SEL;

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    msk = 0x1 << dataSelOff;
    result = hyPhy20gVarFieldWrite(&val, msk, dataSel);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;


    /* Set the OTU1_TX_SYS_CLK_CFG[n] */
    addr = HYPHY20G_ODUKPP_TOP_REG_CLK_CFG;
    result = sysHyPhy20gBitWrite(fileHndl, addr, txClkCfgOff, txClkCfg);
    if (result) return result;

    /* Set the OTU1_RX_SYS_CLK_CFG[n] */
    result = sysHyPhy20gBitWrite(fileHndl, addr, rxClkCfgOff, rxClkCfg);
    if (result) return result;

    /* Set the TX_STREAMING_MODE */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_GLBL_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_TX_STREAMING_MODE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, txStrMode);
    if (result) return result;

    /* Set the TX_ASYNC_MAPPING */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OPU_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_CTL_TX_ASYNC_MAPPING_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, txMapping);
    if (result) return result;

    /* Configure Blanking */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_FA_OTU_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_FA_OTU_CTL_OTU_BLANKING_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, otuBlank);
    if (result) return result;

    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_ODU_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_ODU_CTL_ODU_BLANKING_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, oduBlank);
    if (result) return result;

    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_OPU_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_OPU_CTL_OPU_BLANKING_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, opuBlank);
    if (result) return result;

    /* Configure Tx Insert FAS/MFAS */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_FA_OTU_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_FA_OTU_CTL_INSERT_FAS_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, insFas);
    if (result) return result;

    off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_FA_OTU_CTL_INSERT_MFAS_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, insMfas);
    if (result) return result;

    /* Set the RX_STREAMING_MODE */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_GLBL_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_STREAMING_MODE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, rxStrMode);
    if (result) return result;

    /* Set the RX_ASYNC_MAPPING */
    addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_OPU_CTL;
    addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_OPU_CTL_RX_ASYNC_MAPPING_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, rxMapping);
    if (result) return result;

    /* additional setting if souce is CBR2G5 from SONET subsystem */
    if (source == 16) {
        /* Reset the Rx FIFO */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_RX_OTN_GLBL_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_RX_OTN_GLBL_CTL_RX_FIFO_RESET_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, off, 1);
        if (result) return result;

        /* Reset the Tx FIFO */
        addr = HYPHY20G_ODUKPP_OTU1_FRM_REG_TX_OTN_GLBL_CTL;
        addr = HYPHY20G_ODUKPP_OTU1_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        off = HYPHY20G_ODUKPP_OTU1_FRM_BIT_TX_OTN_GLBL_CTL_TX_FIFO_RESET_OFF;
        result = sysHyPhy20gBitWrite(fileHndl, addr, off, 1);
        if (result) return result;
    }

    /* Configure JAT_622E */
/* removed operation for emulation platform as JAT block not emulated */
#ifndef HWACCESSMODE_USB
    /* for SONET traffic */
    result = odukppHyPhy20gJatInit(fileHndl, slice + 2, jatEn);
    if (result) return result;
#endif

    return HYPHY20G_SUCCESS;

} /* odukppHyPhy20gOdukppOdu1SysDataPathCfg */

/*******************************************************************************
**
**  odukppHyPhy20gOdukppOdu2DataPathCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the bidirectional system side data
**                  path for an ODUKPP ODU2 Framer Slice.  Will configure the
**                  JAT_622E for CBR10G SONET system side payloads
**
**                  The system side data sources are:
**                     VCAT I/F 0 and 1
**                     CBR10G STS-192 from SONET subsystem (via SONET_LINE)
**                     CBR10G 10GE LAN from HSCI subsystem (via SONET_LINE)
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTU2 Framer instance:
**                               0 - OTU2 Framer 0
**                               1 - OTU2 Framer 1
**                  sysIf     - Operation Mode of OTU2 Framer slice
**                              0 - 1 VCAT I/F 0 - 1
**                              2 - CBR10G STS-192 from SONET subsystem
**                              3 - MAC Terminated 10GE LAN from HSCI subsystem
**                                  G.Sup43 7.1 Compliant - (ODU2e rate signals)
**                              4 - MAC Terminated 10GE LAN from HSCI subsystem
**                                  G.Sup43 7.2 Compliant - (ODU1e rate signals)
**                  odukType  - ODUk type being processed by ODU2 Framer:
**                              0 - ODU2
**                              1 - ODU2e
**                              2 - ODU1e
**                              3 - ODU2f
**                              4 - ODU1f
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 odukppHyPhy20gOdukppOdu2DataPathCfg(struct file *fileHndl, UINT4 slice,
						                 UINT4 sysIf, UINT4 odukType)
{
    /* variable declaration */
    sHYPHY20G_CTXT  *pDevCtxt;    
    INT4 result;
    UINT4 addr, val, msk;
    UINT4 dataSelMsk, vcatDataModeOff, txClkCfgOff, rxClkCfgOff;
    UINT4 dataSel, txClkCfg, txStrMode, txMapping, txStuffEn;
    UINT4 vcatCh, off;
    UINT4 rxClkCfg, rxStrMode, rxMapping, rxStuffEn;
    UINT4 jatEn;
    UINT4 otuBlank, oduBlank, opuBlank;
    UINT4 insFas, insMfas;
    UINT4 txFracDivNumReg = 0, txFracDivDenReg = 0;
    UINT4 rxReqCtrlNumReg, rxReqCtrlDenReg;
    UINT4 rxFracDivCtrlReg = 0, txFracDivNumMsk = 0;
    UINT4 txFracDivDenMsk = 0, rxFracDivNumMsk = 0, rxFracDivDenMsk = 0;
    UINT4 txFracDivNumVal = 1, txFracDivDenVal = 1, rxFracDivNumVal = 1;
    UINT4 rxFracDivDenVal = 1;
    UINT4 rxReqCtrlNumMsk = 0, rxReqCtrlDenMsk = 0;
    UINT4 rxReqNumVal = 0xEF, rxReqDenVal = 0xFF;
    UINT4 dequeModeReg, dequeModeOff, dequeMode = 0;

    /* argument checking */
    if (slice > 1 || sysIf > 4 || odukType > 4) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    pDevCtxt = sysHyPhy20gGetDeviceCtxt(fileHndl);
    /* confirm device context pointer is valid before processing */
    HYPHY20G_ASSERT((pDevCtxt != NULL), HYPHY20G_ERR_NULL_PTR_PASSED,
                    "%s", "pDevCtxt is NULL.");

    txClkCfgOff = HYPHY20G_ODUKPP_TOP_BIT_CLK_CFG_OTU2_TX_SYS_CLK_CFG_OFF + slice;
    rxClkCfgOff = HYPHY20G_ODUKPP_TOP_BIT_CLK_CFG_OTU2_RX_SYS_CLK_CFG_OFF + slice;

    dequeModeReg = HYPHY20G_ODUKPP_TOP_REG_RX_REQ_MD_CTL;
    dequeModeOff = HYPHY20G_ODUKPP_TOP_BIT_RX_REQ_MD_CTL_OTU2_RX_SYS_DEQUE_MODE_SEL_OFF + slice;

    if (slice == 0) {
        dataSelMsk = HYPHY20G_ODUKPP_TOP_BIT_SYS_SIDE_DAT_SEL_OTU2_SYS_DATA_SEL0_MSK;
        vcatDataModeOff = HYPHY20G_ODUKPP_TOP_BIT_SYS_SIDE_DAT_SEL_VCAT_DATA_MODE_SEL0_OFF;

        rxReqCtrlNumReg = HYPHY20G_ODUKPP_TOP_REG_OTU2_RX_REQ_CTL_NUMR_OFFSET(slice);
        rxReqCtrlDenReg = HYPHY20G_ODUKPP_TOP_REG_OTU2_RX_REQ_CTL_DENM_OFFSET(slice);
        txFracDivNumReg = HYPHY20G_ODUKPP_TOP_REG_TX_10GE_FRCTNL_DIVIDER_NUMR_0;
        txFracDivDenReg = HYPHY20G_ODUKPP_TOP_REG_TX_10GE_FRCTNL_DIVIDER_DENM_0;
        rxFracDivCtrlReg = HYPHY20G_ODUKPP_TOP_REG_RX_10GE_FRCTNL_DIVIDER_CTL_0;

        txFracDivNumMsk = HYPHY20G_ODUKPP_TOP_BIT_TX_10GE_FRCTNL_DIVIDER_NUMR_0_TX_GE_FRAC_NUMER0_MSK;
        txFracDivDenMsk = HYPHY20G_ODUKPP_TOP_BIT_TX_10GE_FRCTNL_DIVIDER_DENM_0_TX_GE_FRAC_DENOM0_MSK;
        rxFracDivNumMsk = HYPHY20G_ODUKPP_TOP_BIT_RX_10GE_FRCTNL_DIVIDER_CTL_0_RX_GE_FRAC_NUMER0_MSK;
        rxFracDivDenMsk = HYPHY20G_ODUKPP_TOP_BIT_RX_10GE_FRCTNL_DIVIDER_CTL_0_RX_GE_FRAC_DENOM0_MSK;
    } else {
        dataSelMsk = HYPHY20G_ODUKPP_TOP_BIT_SYS_SIDE_DAT_SEL_OTU2_SYS_DATA_SEL1_MSK;
        vcatDataModeOff = HYPHY20G_ODUKPP_TOP_BIT_SYS_SIDE_DAT_SEL_VCAT_DATA_MODE_SEL1_OFF;

        rxReqCtrlNumReg = HYPHY20G_ODUKPP_TOP_REG_OTU2_RX_REQ_CTL_NUMR_OFFSET(slice);
        rxReqCtrlDenReg = HYPHY20G_ODUKPP_TOP_REG_OTU2_RX_REQ_CTL_DENM_OFFSET(slice);
        txFracDivNumReg = HYPHY20G_ODUKPP_TOP_REG_TX_10GE_FRCTNL_DIVIDER_NUMR_1;
        txFracDivDenReg = HYPHY20G_ODUKPP_TOP_REG_TX_10GE_FRCTNL_DIVIDER_DENM_1;
        rxFracDivCtrlReg = HYPHY20G_ODUKPP_TOP_REG_RX_10GE_FRCTNL_DIVIDER_CTL_1;

        txFracDivNumMsk = HYPHY20G_ODUKPP_TOP_BIT_TX_10GE_FRCTNL_DIVIDER_NUMR_1_TX_GE_FRAC_NUMER1_MSK;
        txFracDivDenMsk = HYPHY20G_ODUKPP_TOP_BIT_TX_10GE_FRCTNL_DIVIDER_DENM_1_TX_GE_FRAC_DENOM1_MSK;
        rxFracDivNumMsk = HYPHY20G_ODUKPP_TOP_BIT_RX_10GE_FRCTNL_DIVIDER_CTL_1_RX_GE_FRAC_NUMER1_MSK;
        rxFracDivDenMsk = HYPHY20G_ODUKPP_TOP_BIT_RX_10GE_FRCTNL_DIVIDER_CTL_1_RX_GE_FRAC_DENOM1_MSK;
    }

    /* Determine which VCAT interface corresponds to which OTU2 Framer
       instance */
    vcatCh = (slice == 0) ? 1:2;

    /* Determine OTU2_NUMERATOR (rxReqNumVal) and OTU2_DENOMINATOR settings
       (rxReqDenVal) for each payloadType depending on the system interface */
    switch(sysIf)
    {
        case 0:
        case 1: /* VCAT I/F #0/1 */
            switch (pDevCtxt->gData.hyphy20gCbrbPacketSizeMode) {
                 case 0: /* standard */
                 case 2: /* custom fabric */
                    /* standard and custome fabric case */
                    if (odukType == 0) { /* ODU2 rate */
                        rxReqNumVal = 239;
                        rxReqDenVal = 255;
                    } else { /* ODU2e/2f/1e/1f rate */
                        rxReqNumVal = 3;
                        rxReqDenVal = 1;
                    }
                    break;
                case 1: /* standard predictor compensated */
                    switch (odukType) {
                        case 0: /* 0 - ODU2  */
                            rxReqNumVal = 239;
                            rxReqDenVal = 255;
                            break;
                        case 1: /* 1 - ODU2e */
                            rxReqNumVal = 553;
                            rxReqDenVal = 570;
                            break;
                        case 2: /* 2 - ODU1e */
                            rxReqNumVal = 237;
                            rxReqDenVal = 245;
                            break;
                        case 3: /* 3 - ODU2f */
                            rxReqNumVal = 6399;
                            rxReqDenVal = 6460;
                            break;
                        case 4: /* 4 - ODU1f */
                            rxReqNumVal = 3555;
                            rxReqDenVal = 3604;
                            break;
                        default:
                            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
                    }
                    break;
                default:
                    DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
            }
            dequeMode = 1;
            break;
        case 2: /* CBR10G STS-192 from SONET */
            if (odukType == 0) { /* ODU2 rate */
                rxReqNumVal = 239;
                rxReqDenVal = 255;
            }
            break;
        case 3: /* CBR10G LAN from HSCI subsystem G.Sup43 7.1 Compliant */
            if (odukType == 1) { /* ODU2e rate */
                rxReqNumVal = 1643;
                rxReqDenVal = 1692;
            }
            break;
        case 4: /* CBR10G LAN from HSCI subsystem G.Sup43 7.2 Compliant */
            if (odukType == 2) { /* ODU1e rate */
                rxReqNumVal = 1375;
                rxReqDenVal = 1422;
            }
            break;
        default:
             DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    switch(sysIf)
    {
        /* VCAT I/F #0 */
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
        /* VCAT I/F #1 */
        case 1:
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
        /* CBR10G STS-192 from SONET subsystem */
        case 2:
            dataSel     = 1;
            txClkCfg    = 1;
            txStrMode   = 0;
            txMapping   = 0;
            rxClkCfg    = 1;
            rxStrMode   = 0;
            rxMapping   = 1;

            otuBlank = oduBlank = opuBlank = 0;

            txStuffEn = rxStuffEn = 1;

            jatEn       = 1;
            break;
        /* CBR10G 10GE LAN from HSCI subsystem G.Sub43 7.1 for ODU2e rate */
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

            /* Tx G.Sup43 clause 7.1: 2715625/2820096 */
            txFracDivNumVal = 2715625;
            txFracDivDenVal = 2820096;

            /* Rx G.Sup43 clause 7.1: 237/239 */
            rxFracDivNumVal = 237;
            rxFracDivDenVal = 239;

            jatEn       = 0;
            break;
        /* CBR10G from HSCI subsystem G.Sup43 7.2 for ODU1e rate */
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

            /* Tx G.Sup43 clause 7.2: 2715625/2820096 */
            txFracDivNumVal = 2715625;
            txFracDivDenVal = 2820096;

            /* Rx G.Sup43 clause 7.2: 238/239 */
            rxFracDivNumVal = 238;
            rxFracDivDenVal = 239;

            jatEn       = 0;
            break;

        default:
             DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
             return HYPHY20G_ERR_INVALID_ARG;
    }

    insFas = insMfas = 1;

    /* Set OTU2_RX_SYS_DEQUE_MODE_SEL */
    result = sysHyPhy20gBitWrite(fileHndl, dequeModeReg, dequeModeOff,
                                 dequeMode);
    if (result) return result;

    /* Set OTU2_SYS_DATA_SEL[n] */
    addr = HYPHY20G_ODUKPP_TOP_REG_SYS_SIDE_DAT_SEL;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, dataSelMsk, dataSel);
    DBG_PRINT("%s, %s, %d, addr = 0x%x, OTU2_SYS_DATA_SEL = %u\n",__FILE__, __FUNCTION__, __LINE__,
              (unsigned int)addr, (unsigned int)dataSel);
    if (result) return result;

    /* Set VCAT Sytstem Side Data Select */
    addr = HYPHY20G_ODUKPP_TOP_REG_SYS_SIDE_DAT_SEL;
    result = sysHyPhy20gBitWrite(fileHndl, addr, vcatDataModeOff, vcatCh);
    if (result) return result;

    /* Set the OTU2_TX_SYS_CLK_CFG[n] */
    addr = HYPHY20G_ODUKPP_TOP_REG_CLK_CFG;
    result = sysHyPhy20gBitWrite(fileHndl, addr, txClkCfgOff, txClkCfg);
    DBG_PRINT("%s, %s, %d, addr = 0x%x, OTU2_TX_SYS_CLK_CFG = %u\n",__FILE__, __FUNCTION__, __LINE__,
              (unsigned int)addr, (unsigned int)txClkCfg);
    if (result) return result;

    /* Set the OTU2_RX_SYS_CLK_CFG[n] */
    result = sysHyPhy20gBitWrite(fileHndl, addr, rxClkCfgOff, rxClkCfg);
    DBG_PRINT("%s, %s, %d, addr = 0x%x, OTU2_RX_SYS_CLK_CFG = %u\n",__FILE__, __FUNCTION__, __LINE__,
              (unsigned int)addr, (unsigned int)rxClkCfg);
    if (result) return result;

    /* Set the TX_STREAMING_MODE */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_GLBL_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_TX_SCRAMBLE_ENABLE_MSK;

    result = hyPhy20gVarFieldWrite(&val, msk, 0x0);
    if (result) return result;

    msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_TX_STREAMING_MODE_MSK;

    result = hyPhy20gVarFieldWrite(&val, msk, txStrMode);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* Set the TX_ASYNC_MAPPING */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OPU_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_CTL_TX_ASYNC_MAPPING_OFF;

    result = sysHyPhy20gBitWrite(fileHndl, addr, off, txMapping);
    if (result) return result;

    /* Configure Blanking */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_FA_OTU_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_FA_OTU_CTL_OTU_BLANKING_OFF;

    result = sysHyPhy20gBitWrite(fileHndl, addr, off, otuBlank);
    if (result) return result;

    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_CTL_ODU_BLANKING_OFF;

    result = sysHyPhy20gBitWrite(fileHndl, addr, off, oduBlank);
    if (result) return result;

    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OPU_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_CTL_OPU_BLANKING_OFF;

    result = sysHyPhy20gBitWrite(fileHndl, addr, off, opuBlank);
    if (result) return result;

    /* Configure Tx Insert FAS/MFAS */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_FA_OTU_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_FA_OTU_CTL_INSERT_FAS_OFF;

    result = sysHyPhy20gBitWrite(fileHndl, addr, off, insFas);
    if (result) return result;

    off = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_FA_OTU_CTL_INSERT_MFAS_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, insMfas);
    if (result) return result;

    /* Set the TX_STUFF_ENABLE */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OPU_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_CTL_TX_STUFF_ENABLE_OFF;

    result = sysHyPhy20gBitWrite(fileHndl, addr, off, txStuffEn);
    if (result) return result;

    /* RX_OTN_GLOBAL_CONTROL */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if (result) return result;

    /* Set RX_SCRAMBLE_ENABLE = 0 */
    msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_SCRAMBLE_ENABLE_MSK;

    result = hyPhy20gVarFieldWrite(&val, msk, 0x0);
    if (result) return result;

    /* Set the RX_STREAMING_MODE */
    msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_STREAMING_MODE_MSK;

    result = hyPhy20gVarFieldWrite(&val, msk, rxStrMode);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if (result) return result;

    /* Set the RX_ASYNC_MAPPING */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OPU_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);

    off = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OPU_CTL_RX_ASYNC_MAPPING_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, off, rxMapping);
    if (result) return result;

    /* Set the RX_STUFF_ENABLE */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OPU_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OPU_CTL_RX_STUFF_ENABLE_OFF;

    result = sysHyPhy20gBitWrite(fileHndl, addr, off, rxStuffEn);
    if (result) return result;

    /* Set OTU2_RX_REQUEST_CTRL_DENOMINATOR: OTU2_DENOMINATOR [31:0] and
           OTU2_RX_REQUEST_CTRL_NUMERATOR: OTU2_NUMERATOR [31:0] */
    rxReqCtrlDenMsk = HYPHY20G_ODUKPP_TOP_BIT_OTU2_RX_REQ_CTL_DENM_OTU2_DENOMINATOR_MSK;
    rxReqCtrlNumMsk = HYPHY20G_ODUKPP_TOP_BIT_OTU2_RX_REQ_CTL_NUMR_OTU2_NUMERATOR_MSK;

    /* Denominator control */
    result = sysHyPhy20gRead(fileHndl, rxReqCtrlDenReg, &val);
    if(result) return result;

    DBG_PRINT("%s, %s, %d, OTU2_DENOMINATOR = %u\n",__FILE__, __FUNCTION__, __LINE__,
              (unsigned int)val);
    result = hyPhy20gVarFieldWrite (&val, rxReqCtrlDenMsk, rxReqDenVal);
    if(result) return result;

    DBG_PRINT("%s, %s, %d, OTU2_DENOMINATOR = %u\n",__FILE__, __FUNCTION__, __LINE__,
              (unsigned int)val);
    result = sysHyPhy20gWrite(fileHndl, rxReqCtrlDenReg, val);
    if(result) return result;

    /* Numerator control */
    result = sysHyPhy20gRead(fileHndl, rxReqCtrlNumReg, &val);
    if(result) return result;

    DBG_PRINT("%s, %s, %d, OTU2_NUMERATOR = %u\n",__FILE__, __FUNCTION__, __LINE__,
              (unsigned int)val);
    result = hyPhy20gVarFieldWrite (&val, rxReqCtrlNumMsk, rxReqNumVal);
    if(result) return result;

    DBG_PRINT("%s, %s, %d, OTU2_NUMERATOR = %u\n",__FILE__, __FUNCTION__, __LINE__,
              (unsigned int)val);
    result = sysHyPhy20gWrite(fileHndl, rxReqCtrlNumReg, val);
    if(result) return result;

    /* System side: configure TX 10GE Fractional Divider Numerator n */
    result = hyPhy20gRegFieldWrite(fileHndl, txFracDivNumReg, txFracDivNumMsk,
                                   txFracDivNumVal);
    if (result) return result;

    /* configure TX 10GE Fractional Divider Denominator n */
    result = hyPhy20gRegFieldWrite(fileHndl, txFracDivDenReg, txFracDivDenMsk,
                                   txFracDivDenVal);
    if(result) return result;

    /* System side: configure RX 10GE Fractional Divider Control n */
    result = sysHyPhy20gRead(fileHndl, rxFracDivCtrlReg, &val);
    if(result) return result;

    /* Configure denominator */
    result = hyPhy20gVarFieldWrite (&val, rxFracDivDenMsk, rxFracDivDenVal);
    if(result) return result;

    /* Configure numerator */
    result = hyPhy20gVarFieldWrite (&val, rxFracDivNumMsk, rxFracDivNumVal);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, rxFracDivCtrlReg, val);
    if(result) return result;

    /* additional setting if source is CBR10G from SONET/HSCI subsystem */
    if (sysIf == 2 || sysIf == 3 || sysIf == 4) {
        /* Reset Rx FIFO */
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        off = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_FIFO_RESET_OFF;

        result = sysHyPhy20gBitWrite(fileHndl, addr, off, 1);
        if (result) return result;

        /* Reset Tx FIFO */
        addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_GLBL_CTL;
        addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
        off = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_TX_FIFO_RESET_OFF;

        result = sysHyPhy20gBitWrite(fileHndl, addr, off, 1);
        if (result) return result;
    }

    /* removed operation for emulation platform as JAT block not emulated */
#ifndef HWACCESSMODE_USB
    /* for SONET  traffic: Configure JAT_622E */
    result = odukppHyPhy20gJatInit(fileHndl, slice, jatEn);
    if (result) return result;
#endif

    return HYPHY20G_SUCCESS;

} /* odukppHyPhy20gOdukppOdu2DataPathCfg */

/*******************************************************************************
**
**  odukppHyPhy20gOdukppOdu2Init
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the major operational mode of an
**                  ODUKPP ODU2 Framer resource.
**
**                  The major operational modes are:
**                     ODU2 Line Side Frames
**                     Disabled
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTU2 Framer instance:
**                               0 - OTU2 Framer 0
**                               1 - OTU2 Framer 1
**                  mode       - Operation Mode of ODU2 Framer slice
**                               0 - ODU2 Line Side Frame
**                               1 - Disabled
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 odukppHyPhy20gOdukppOdu2Init(struct file *fileHndl, UINT4 slice,
                                  UINT4 mode)
{
    /* variable declaration */
    INT4 result;
    UINT4 addr = 0, val, off;
    UINT4 swRstOff = 0, msk;
    UINT4 otuFrmType = 0;
    UINT4 fasOff;

    /* argument checking */
    if (slice > 1 || mode > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    swRstOff = HYPHY20G_ODUKPP_TOP_BIT_CFG_SW_RST_OTU2_FRAMER_OFF + slice;
    addr = HYPHY20G_ODUKPP_TOP_REG_CFG;

    /* disable operation */
    if (mode == 0) {
        /* hold OTU2_FRAMER block in reset */
        result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 1);
        if (result) return result;
        return HYPHY20G_SUCCESS;
    }

    if (mode == 1) {
        /* ODU2 Line Side Frame */
        otuFrmType = 0;
    }

    /* ensure OTU2_FRAMER block has been reset */
    result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 1);
    if (result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr, swRstOff, 0);
    if (result) return result;

    /*
     * configure the Tx OTU2 framer
     */

    /* configure OTU_FRAME_TYPE */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_GLBL_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_GLBL_CTL_OTU_FRAME_TYPE_OFF;

    result = sysHyPhy20gBitWrite(fileHndl, addr, off, otuFrmType);
    if(result) return result;

    /*
     * configure the Rx OTU2 framer
     */

    /* configure OTUK_FRAME_TYPE */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    off = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_OTUK_FRAME_TYPE_OFF;

    result = sysHyPhy20gBitWrite(fileHndl, addr, off, otuFrmType);
    if(result) return result;

    /* apply non-default settings to block */
    /* configure RX_FIFO_HIGH_THRESHOLD_VALUE to 0x5e (94d) */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OPU_FIFO_HI_TH;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk =
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OPU_FIFO_HI_TH_RX_FIFO_HIGH_THRESHOLD_VALUE_MSK;
    val = 0x5e;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure RX_FIFO_LOW_THRESHOLD_VALUE to 0x28 (40d) */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_OPU_FIFO_LOW_TH;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk =
HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_OPU_FIFO_LOW_TH_RX_FIFO_LOW_THRESHOLD_VALUE_MSK;
    val = 0x28;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure TX_ERR_SYNC_MODE to 1 */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_CNT_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_CNT_CTL_TX_ERR_SYNC_MODE_MSK;
    val = 0x1;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure TX_FIFO_HIGH_THRESHOLD_VALUE to 0x3c (60d) */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_OPU_FIFO_HI_TH;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk =
HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_OPU_FIFO_HI_TH_TX_FIFO_HIGH_THRESHOLD_VALUE_MSK;
    val = 0x3c;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* configure RX_ERR_SYNC_MODE to 1 */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_GLBL_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_GLBL_CTL_RX_ERR_SYNC_MODE_MSK;
    val = 0x1;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, val);
    if (result) return result;

    /* set OTU2_OVERWRITE_FAS[n] to 1 */
    addr = HYPHY20G_ODUKPP_TOP_REG_RX_OVERWRITE_FAS_MFAS;
    fasOff = HYPHY20G_ODUKPP_TOP_BIT_RX_OVERWRITE_FAS_MFAS_OTU2_OVERWRITE_FAS_OFF;

    msk = 1 << slice << fasOff;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x1);
    if (result) return result;

    /* set AUTO_OTUK_AIS_IAE to 0 */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_AUTO_AIS_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_OTUK_AIS_IAE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0);
    if (result) return result;

    /* set AUTO_ODUK_AIS_FIFO_ERROR to 1 */
    msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_AUTO_AIS_CTL_AUTO_ODUK_AIS_FIFO_ERROR_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if (result) return result;

    /* set MIN_AUTO_AIS_INSERT_MULTIFRAME_COUNT to 1 */
    msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_AUTO_AIS_CTL_MIN_AUTO_AIS_INSERT_MULTIFRAME_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 1);
    if (result) return result;
        
    /* set TX_IAE_MULTIFRAME_COUNT to 16 to comply with G.798 */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_TX_OTN_ODU_TCMI_AUTO_BDI_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_TX_OTN_ODU_TCMI_AUTO_BDI_CTL_TX_IAE_MULTIFRAME_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x10);
    if (result) return result;
        
    /* set RX_IAE_MULTIFRAME_COUNT to 16 to comply with G.798 */
    addr = HYPHY20G_ODUKPP_OTU2_FRM_REG_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL;
    addr = HYPHY20G_ODUKPP_OTU2_FRM_TSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_ODUKPP_OTU2_FRM_BIT_RX_OTN_ODU_TCMI_AUTO_BDI_INS_CTL_RX_IAE_MULTIFRAME_COUNT_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, 0x10);
    if (result) return result;
            
    return HYPHY20G_SUCCESS;

} /* odukppHyPhy20gOdukppOdu2Init */


/*******************************************************************************
**
**  odukppHyPhy20gSetTxODU12Sel
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures TX_LINE_ODU12_SEL.
**
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - OTU1 Framer instance:
**                               0 to 7 - OTU1 Framer 0 to 7
**                  odtuTsNum  - Date source for ODU1 Framer
**                               0  to 3 ODTU12 MX 0 Timeslot 0 to 3
**                               4  to 7 ODTU12 MX 1 Timeslot 0 to 3
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 odukppHyPhy20gSetTxODU12Sel(struct file *fileHndl, UINT4 slice,
			                     UINT4 odtuTsNum)
{
    INT4 result;
    UINT4 addr;
    UINT4 txLnSinkSelMsk;
    UINT4 txOdu12SelMsk;
    UINT4 frmNumIndex;
    UINT4 i;
    UINT4 tsNum;
    UINT4 sinkSel[8]; /* index is OTU frm number number */
    UINT4 odtuTsNumList[8]; /* index is OTU frm number number, value is TS number */

    /* argument checking */
    if (slice > 7 || odtuTsNum > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* LINE_SINK_SEL, TX_LINE_ODU12_SEL mask */
    txLnSinkSelMsk = HYPHY20G_ODUKPP_TOP_BIT_TX_LN_SIDE_OTU1_MUX_SEL_TX_LINE_SINK_SEL_MSK;
    txOdu12SelMsk = HYPHY20G_ODUKPP_TOP_BIT_TX_LN_SIDE_OTU1_MUX_SEL_TX_LINE_ODU12_SEL_MSK;

    for (i = 0; i < 8; i++) {
        sinkSel[i]   = 0xFFFFFFFF;
        odtuTsNumList[i] = 0xFFFFFFFF;
    }

    /* 
     * 1. get sink_sel and TS number basing on OTU1 framer number
     */
    for (frmNumIndex = 0; frmNumIndex < 8; frmNumIndex++) {
        addr = HYPHY20G_ODUKPP_TOP_REG_TX_LN_SIDE_OTU1_MUX_SEL_OFFSET(frmNumIndex);
        result = hyPhy20gRegFieldRead(fileHndl, addr, txLnSinkSelMsk,
                                      &sinkSel[frmNumIndex]);
        if (result) return result;

        if (sinkSel[frmNumIndex] == 1) {
            UINT4 match = 0;
            UINT4 frmNum;
            /* go thru each TS */
            for (tsNum = 0; tsNum < 8; tsNum++) {                
                addr = HYPHY20G_ODUKPP_TOP_REG_TX_LN_SIDE_OTU1_MUX_SEL_OFFSET(tsNum);
                result = hyPhy20gRegFieldRead(fileHndl, addr, txOdu12SelMsk,
                                              &frmNum);
                if (result) return result;

                if (frmNum == frmNumIndex) {
                    match = 1;
                    break;
                }
            }

            if (match) {
                odtuTsNumList[frmNumIndex] = tsNum;
            } else {
                odtuTsNumList[frmNumIndex] = 0xFFFFFFFF;
                DBG_PRINT("%s, %s, %d ERROR!\n",__FILE__, __FUNCTION__, __LINE__);
            }
        } else {
            odtuTsNumList[frmNumIndex] = 0xFFFFFFFF;
        }
    } /* end for */

    sinkSel[slice]       = 1;
    odtuTsNumList[slice] = odtuTsNum;

    DBG_PRINT("%s, %s, %d\n",__FILE__, __FUNCTION__, __LINE__);
    for (i = 0; i < 8; i++) {
        DBG_PRINT("sinkSel[%d]=0x%lx\t", (UINT2)i, sinkSel[i]);
    }
    DBG_PRINT("\n");
    for (i = 0; i < 8; i++) {
        DBG_PRINT("odtuTsNumList[%d]=0x%lx\t", (UINT2)i, odtuTsNumList[i]);
    }
    DBG_PRINT("\n");
    
    /* 
     * 2. spread out unused TS number to unused OTU1 framer
     */
    for (frmNumIndex = 0; frmNumIndex < 8; frmNumIndex++) {
        if (odtuTsNumList[frmNumIndex] == 0xFFFFFFFF) {
            UINT4 freeTsNum = 0xFFFFFFFF;
            UINT4 num = 0;
            
            /* get a unused TS number */
            for (num = 0; num < 8; num++) {
                if ((num != odtuTsNumList[0]) &&
                    (num != odtuTsNumList[1]) &&
                    (num != odtuTsNumList[2]) &&
                    (num != odtuTsNumList[3]) &&
                    (num != odtuTsNumList[4]) &&
                    (num != odtuTsNumList[5]) &&
                    (num != odtuTsNumList[6]) &&
                    (num != odtuTsNumList[7])) {
                    freeTsNum = num;
                    break;
                }
            }
            DBG_PRINT("%s, %s, %d frmNumIndex=%d freeTsNum=%d\n",__FILE__, __FUNCTION__, __LINE__, (UINT2)frmNumIndex, (UINT2)freeTsNum);
            
            /* check exception */        
            if (freeTsNum == 0xFFFFFFFF) 
                return HYPHY20G_ERR_INVALID_PRECONFIG;

            addr = HYPHY20G_ODUKPP_TOP_REG_TX_LN_SIDE_OTU1_MUX_SEL_OFFSET(freeTsNum);
            result = hyPhy20gRegFieldWrite(fileHndl, addr, txOdu12SelMsk, frmNumIndex);
            if (result) return result;

            odtuTsNumList[frmNumIndex] = freeTsNum;
        }
    }

    for (i = 0; i < 8; i++) {
        DBG_PRINT("sinkSel[%d]=0x%lx\t", (UINT2)i, sinkSel[i]);
    }
    DBG_PRINT("\n");
    for (i = 0; i < 8; i++) {
        DBG_PRINT("odtuTsNumList[%d]=0x%lx\t", (UINT2)i, odtuTsNumList[i]);
    }
    DBG_PRINT("\n");

    /* 
     * 3. Write slice to the timeslot
     */
    addr = HYPHY20G_ODUKPP_TOP_REG_TX_LN_SIDE_OTU1_MUX_SEL_OFFSET(odtuTsNum);
    result = hyPhy20gRegFieldWrite(fileHndl, addr, txOdu12SelMsk, slice);
    if (result) return result;
    
    return HYPHY20G_SUCCESS;
    
} /* odukppHyPhy20gSetTxODU12Sel */


/* end of file */
