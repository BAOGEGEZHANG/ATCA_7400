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
**  MODULE      : HyPHY 20G ENET_COM Configuration Code
**
**  FILE        : hyphy20g_util_ecom.c
**
**  $Date: 2012-12-03 13:59:42 +0800 (Mon, 03 Dec 2012) $
**
**  $Revision: 29406 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_util_ecom.h"

/** external functions **/

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
INT4 utilHyPhy20gEmpeiInit(struct file *fileHndl);

INT4 utilHyPhy20gEnetComGsup43TxCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 pktType, UINT4 osType);
                                    
INT4 utilHyPhy20gEnetComGsup43RxCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 pktType, UINT4 osType);                                   
                                   
/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/
/*******************************************************************************
**
**  hyPhy20gEnetComDeviceInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the ENET_COM subsystem
**                  to be in low power - power savings mode or out
**                  of low power mode and ready for individual configuration.
**
**                  This function initializes inline buffer thresholds in 
**                  the EMPEI sub block.
**
**                  This function needs to be called only once during device
**                  initialization, if any of LSCI or HSCI slices is intented 
**                  to be using. This function must be called prior to calling 
**                  either hyPhy20gHsciInit or hyPhy20gLsciInit
**                  to enable a resource for a desired mode.
**
**                  It is the caller's responsibility to ensure this function
**                  is not called with enable = 0 while the ENET_COM subsystem
**                  is intended to be processing user data.
**
**  INPUTS:         fileHndl   - device handle
**                  enable     - Operation Mode of ENET_COM subsystem
**                               0 - In low power modes
**                               1 - Out of low power mode and reset to
**                                   default values
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEnetComDeviceInit(struct file *fileHndl, UINT4 enable)
{
    INT4 result;

    /* argument checking */
    if (enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* set lowpwr bit to 1 */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                             HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_ENET_COM_LOWPWR_OFF,
                             1);
    if(result) return result;

    /* reset at top level */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_2_ENET_COM_RST_OFF,
                                 1);
    if(result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_SW_RST_2,
                                 HYPHY20G_TOP_BIT_SYS_SW_RST_2_ENET_COM_RST_OFF,
                                 0);
    if(result) return result;

    /* terminate function if disabling subsystem */
    if(enable == 0){
        return HYPHY20G_SUCCESS;
    }

    /* remove lowpwr if subsystem to be enabled */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_TOP_REG_SYS_LOW_PWR_2,
                             HYPHY20G_TOP_BIT_SYS_LOW_PWR_2_ENET_COM_LOWPWR_OFF,
                             0);
    if(result) return result;

    /* remove device level EMPEI_RST */
    result = sysHyPhy20gBitWrite(fileHndl, HYPHY20G_ENET_COM_TOP_REG_SW_RST,
                            HYPHY20G_ENET_COM_TOP_BIT_SW_RST_EMPEI_RST_OFF, 0);
    if(result) return result;    

    /* Init EMPEI block */
    result = utilHyPhy20gEmpeiInit(fileHndl);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gEnetComDeviceInit */


/*******************************************************************************
**
**  hyPhy20gEmpeiFeGeDaSaSwap
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables swapping of the 
**                  destination address and source address fields prior to the 
**                  Tx Inline Buffer.
**
**                  FCS field must be updated when swapping of the destination 
**                  address and source address fields.
**
**                  Used for MAC Terminated (FE/GE, SGMII FE/GE) mode only.
**
**                  This function may be required in some OAM loopback 
**                  conditions.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - EMPEI slice associated with the LSCI
**                              slice number 0 - 15
**                  enSwap    - enable/disable destination address and source 
**                              address swapping
**                              0 - don't swap
**                              1 - swap
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEmpeiFeGeDaSaSwap(struct file *fileHndl, UINT4 slice, UINT4 enSwap)
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
    if (slice > HYPHY20G_LSCI_NUM_SLICE - 1 || enSwap > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_ENET_COM_EMPEI_REG_FEGE_SLC_N_BUF_CFG_OFFSET(slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;
    /* update or don't update the FCS field */            
    val = mHYPHY20G_SET_FIELD4(val, enSwap,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_BUF_CFG_FCS_UPDATE_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_BUF_CFG_FCS_UPDATE_OFF);
    /* enable or disable swapping of the DA and SA fields 
       prior to the Tx Inline Buffer */            
    val = mHYPHY20G_SET_FIELD4(val, enSwap,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_BUF_CFG_DA_SA_SWAP_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_BUF_CFG_DA_SA_SWAP_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gEmpeiFeGeDaSaSwap */



/*******************************************************************************
**
**  hyPhy20gEmpei10GeDaSaSwap
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables swapping of the 
**                  destination address and source address fields prior to the 
**                  Tx Inline Buffer.
**
**                  FCS field must be updated when swapping of the destination 
**                  address and source address fields.
**
**                  Used for 10GE MAC Terminated mode only.
**
**                  This function may be required in some OAM loopback 
**                  conditions.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - EMPEI slice associated with the HSCI
**                              slice number 0 - 1
**                  enSwap    - enable/disable destination address and source 
**                              address swapping
**                              0 - don't swap
**                              1 - swap
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEmpei10GeDaSaSwap(struct file *fileHndl, UINT4 slice, UINT4 enSwap)
{
    INT4 result;
    UINT4 addr, val;

    /* argument checking */
    if (slice > 1 || enSwap > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_BUF_CFG_OFFSET(slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    /* update or don't update the FCS field */            
    val = mHYPHY20G_SET_FIELD4(val, enSwap,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_BUF_CFG_FCS_UPDATE_10GE_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_BUF_CFG_FCS_UPDATE_10GE_OFF);
    /* enable or disable swapping of the DA and SA fields 
       prior to the Tx Inline Buffer */            
    val = mHYPHY20G_SET_FIELD4(val, enSwap,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_BUF_CFG_DA_SA_SWAP_10GE_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_BUF_CFG_DA_SA_SWAP_10GE_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gEmpei10GeDaSaSwap */




/*******************************************************************************
**
**  hyPhy20gEmpei10GePreambleEn
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures whether or not packets are
**                  expected to consist of Preamble and HEC/SFD (customize 
**                  preamble sizes). These settings in ENET_COM:EMPEI 
**                  10GE Slice only determine how the device searches the SFD 
**                  byte (0xD5) and how the device delimits Destination Address.
**                  That said, the setting itself will not change data stream.
**
**                  This function must be used in custom preamble application.
**                  
**                  Used for 10GE MAC Terminated mode, G.Sup43 Clause 7.3 and 
**                  Clause 6.2 Terminating application through MAC modes (mode 
**                  '1', '6' and '7' in hyPhy20gHsciInit).
**                  
**                  When enable == 0:
**                  No preamble and SFD in the packet. Device assumes the first  
**                  byte is DA (destination address).     
**                  
**                  When enable == 1 and hdrSize <= 7:
**                  Use for Preamble + SFD <= 8 bytes. Device will search 
**                  for the SFD byte (i.e. 0xD5). If no SFD is found in the 8th 
**                  bytes, device assumes DA starts at 9th byte.
**                  
**                  When enable == 1 and hdrSize > 7:
**                  The HEC/SFD byte would be the TX_PREPEND_HDR + 1 byte.  
**                  DA would be the TX_PREPEND_HDR + 2 byte. If SFD is found 
**                  in [1, x_PREPEND_HDR_10GE) bytes, device assumes DA starts
**                  after the SFD byte.
**                  
**                  Note, packets from the management slice can either
**                  have no preamble and SFD, or have 7 bytes of preamble 
**                  and 1 byte of SFD.
**                  
**  INPUTS:         fileHndl  - device handle
**                  slice     - EMPEI slice associated with the HSCI
**                              slice number 0 - 1
**                  dir       - data flow direction
**                              0 - RX (packets from the Functional 
**                                  FEGE RX interface)
**                              1 - TX (packets from the FEGE CPB interface)
**                  enable    - prepend or not prepend preamble
**                              0 - custom preamble is not enabled
**                              1 - custom preamble is enabled
**                  hdrSize   - length of prepended header (custom Preamble 
**                              and SFD)
**                              Legal range 0 to 0xFF
**                              Note: when enable == 0, set this value to 0.
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEmpei10GePreambleEn(struct file *fileHndl, UINT4 slice, UINT4 dir,
                                 UINT4 enable, UINT4 hdrSize)
{
    INT4 result;
    UINT4 addr, enOff, hdrMsk;

    /* argument checking */
    if (slice > 1 || dir > 1 || 
        enable > 1 || hdrSize > 0xFF) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_BUF_CFG_OFFSET(slice);
    if (dir) {
        /* Tx direction */
        enOff = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_BUF_CFG_TX_PREAMBLE_EN_10GE_OFF;
        hdrMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_BUF_CFG_TX_PREPEND_HDR_10GE_MSK;
    } else {
        /* Rx direction */
        enOff = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_BUF_CFG_RX_PREAMBLE_EN_10GE_OFF;
        hdrMsk = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_BUF_CFG_RX_PREPEND_HDR_10GE_MSK;
    }
    
    result = sysHyPhy20gBitWrite(fileHndl, addr, enOff, enable);
    if(result) return result;

    if (enable) {
        result = hyPhy20gRegFieldWrite(fileHndl, addr, hdrMsk, hdrSize);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gEmpei10GePreambleEn */


/*******************************************************************************
**
**  hyPhy20gEnetComGsup43DsmCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs MAC_GSUP43 DSM configuration.  
**                  It may be called after hyPhy20gHsciInit and prior to 
**                  enabling HSCI data path with hyPhy20gHsciDataPathEnable.
**
**                  Delta Sigma Modulator numerator and denominator values are 
**                  used for rate limiting in the MAC_GSUP43 TX path, which
**                  refers to the data path that packets are transferred from
**                  the HSCI TXXG block, and to the GFP-F Tx subsystem.
**                  
**  INPUTS:         fileHndl  - device handle
**                  slice     - ENET_COM MAC_GSUP43 slice
**                              0 - ENET_COM MAC_GSUP43 slice associated with
**                                  HSCI instance 0
**                              1 - ENET_COM MAC_GSUP43 slice associated with
**                                  HSCI instance 1
**                  dsmNum    - MAC_GSUP43 TX path Delta Sigma Modulator 
**                              numerator value, 19 bits, legal range:
**                                  0 to 0xFFFFF
**                  dsmDenom  - MAC_GSUP43 TX path Delta Sigma Modulator 
**                              denominator value, 19 bits, legal range: 
**                                  0 to 0xFFFFF
**                  updateMsk - Mask of DSM related configuration field
**                              to updated.  Set the corresponding bit
**                              to 1 to update a field, set to 0 to
**                              maintain value of configured field:
**                              bit 0 - update dsmNum field
**                              bit 1 - update dsmDenom field
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEnetComGsup43DsmCfg(struct file *fileHndl, UINT4 slice, 
                                 UINT4 dsmNum, UINT4 dsmDenom, UINT4 updateMsk)
{
    /* variable declaration */
    INT4 result;
    UINT4 dsmNumAddr;
    UINT4 dsmDenomAddr;
    UINT4 dsmNumMsk;
    UINT4 dsmDenomMsk;
    
    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        dsmNum > 0xFFFFF || 
        dsmDenom > 0xFFFF || 
        updateMsk > 0x3) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (!slice) {
        dsmNumAddr   = HYPHY20G_ENET_COM_TOP_REG_GSUP43_0_DSM_NUMER;
        dsmDenomAddr = HYPHY20G_ENET_COM_TOP_REG_GSUP43_0_DSM_DENOM;
        dsmNumMsk    = HYPHY20G_ENET_COM_TOP_BIT_GSUP43_0_DSM_NUMER_GSUP43_0_DSM_NUMER_MSK;
        dsmDenomMsk  = HYPHY20G_ENET_COM_TOP_BIT_GSUP43_0_DSM_DENOM_GSUP43_0_DSM_DENOM_MSK;
    } else {
        dsmNumAddr   = HYPHY20G_ENET_COM_TOP_REG_GSUP43_1_DSM_NUMER;
        dsmDenomAddr = HYPHY20G_ENET_COM_TOP_REG_GSUP43_1_DSM_DENOM;
        dsmNumMsk    = HYPHY20G_ENET_COM_TOP_BIT_GSUP43_1_DSM_NUMER_GSUP43_1_DSM_NUMER_MSK;
        dsmDenomMsk  = HYPHY20G_ENET_COM_TOP_BIT_GSUP43_1_DSM_DENOM_GSUP43_1_DSM_DENOM_MSK;
     }

    /* configure the DSM_DENOM field if specified */
    if(updateMsk & 0x2){
        result = hyPhy20gRegFieldWrite (fileHndl, dsmDenomAddr, dsmDenomMsk,
                                        dsmDenom);
        if(result) return result;
    }

    /* configure the DSM_NUMER field if specified */
    if(updateMsk & 0x1){
        result = hyPhy20gRegFieldWrite (fileHndl, dsmNumAddr, dsmNumMsk,
                                        dsmNum);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gEnetComGsup43DsmCfg */

/*******************************************************************************
**
**  hyPhy20gEnetComGsup43C73ModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures 10GE data frame TYPE and 10GE 
**                  ordered set TYPE in both Tx and Rx GSUP43 blocks of the 
**                  ENET_COM subsystem.           
**
**                  Only used for G.Sup43 Clause 7.3 Terminating application
**                  through MAC operating mode (mode '6' in hyPhy20gHsciInit).
**
**                  This function can optionally be called after
**                  hyPhy20gHsciInit but prior to hyPhy20gHsciDataPathEnable 
**                  to modify the basic G.Sup43 Clause 7.3 mode configuration.
**              
**                  Default settings provided in argument descriptions are
**                  relative to the device state after hyPhy20gHsciInit has
**                  been called.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - ENET_COM MAC_GSUP43 slice
**                              0 - ENET_COM MAC_GSUP43 slice associated with
**                                  HSCI instance 0
**                              1 - ENET_COM MAC_GSUP43 slice associated with
**                                  HSCI instance 1
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
INT4 hyPhy20gEnetComGsup43C73ModeCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 txPktType, UINT4 txOsType,
                                     UINT4 rxPktType, UINT4 rxOsType)
{
    INT4 result;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    result = utilHyPhy20gEnetComGsup43TxCfg(fileHndl, slice, txPktType, 
                                            txOsType);
    if(result) return result;

    result = utilHyPhy20gEnetComGsup43RxCfg(fileHndl, slice, rxPktType, 
                                            rxOsType);

    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gEnetComGsup43C73ModeCfg */



/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/*******************************************************************************
**
**  hyPhy20gEnetComHsciInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes a EMPEI slice associated
**                  with the HSCI subsystem.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - EMPEI slice associated with the HSCI
**                              0 - EMPEI slice associated with
**                                  HSCI instance 0
**                              1 - EMPEI slice associated with
**                                  HSCI instance 1
**                  mode      - traffic type of the specific slice
**                              1 - 10GE MAC Terminated (including G.Sup43
**                                  Clause 7.1 and 7.2 10GE MAC terminated)
**                              2 - Gsup43 Clause 7.3
**                              3 - FC 8G and subrate
**                              4 - High Speed CBR 16/32 bit mode (including
**                                  Gsup43 Clause 7.1 and 7.2 10GE transparent,
**                                  STS-192 asynchronous transparent)
**                              5 - High Speed CBR 20/40 bit mode
**                                  Enable disparity inverter on the transmit path
**                              6 - Gsup43 Clause 7.3 Terminating application
**                                  through MAC 
**                              7 - Gsup43 Clause 6.2 Terminating application
**                                  through MAC 
**                              8 - High Speed CBR 20/40 bit mode
**                                  Disable disparity inverter on the transmit path
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEnetComHsciInit(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    INT4 result;
    UINT4 addr, offset;
    UINT4 byPassSel, empeiLp, eclassRst, pathSel, sliceRst;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        mode < HSCI_10GE_MAC_TEMINATION     ||
        mode > HSCI_TRAFFIC_MAX) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /*
     * Enable Operations
     */
     
    /* remove SW_RESET */
    addr = HYPHY20G_ENET_COM_EMPEI_REG_CFG;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_ENET_COM_EMPEI_BIT_CFG_SW_RESET_OFF, 0);
    if(result) return result;

    /*
     * Configure ENET_COM based on traffic mode
     */
    switch(mode)
    {
        case HSCI_10GE_MAC_TEMINATION:
        case HSCI_MAC_GSUP43_CLAUSE73:
        case HSCI_MAC_GSUP43_CLAUSE62:
            byPassSel   = 0;    /* set to 0 to enable EMPEI path in ENET_COM */
            empeiLp     = 0;    /* disable EMPEI low-power */
            eclassRst   = 0;
            pathSel     = 0;    /* set to 0 to enable 32-bit bypass mode */
            sliceRst    = 0;
            break;
        case HSCI_GSUP43_CLAUSE73:
            byPassSel   = 1;    /* set to 1 to enable bypass path in ENET_COM */
            empeiLp     = 1;    /* enable EMPEI low-power */
            eclassRst   = 1;    /* assert reset for ECLASS in this slice */
            pathSel     = 0;    /* set to 0 to enable 32-bit bypass mode */
            sliceRst    = 1;
            break;
        case HSCI_FC_8G:
            byPassSel   = 1;    /* set to 1 to enable bypass path in ENET_COM */
            empeiLp     = 0;    /* disable EMPEI low-power */
            eclassRst   = 1;    /* assert reset for ECLASS in this slice */
            pathSel     = 1;    /* set to 1 to enable 40-bit bypass mode */
            sliceRst    = 1;
            break;
        case HSCI_HS_CBR_32BIT_MODE:
            byPassSel   = 1;    /* set to 1 to enable bypass path in ENET_COM */
            empeiLp     = 0;    /* disable EMPEI low-power */
            eclassRst   = 1;    /* assert reset for ECLASS in this slice */
            pathSel     = 0;    /* set to 0 to enable 32-bit bypass mode */
            sliceRst    = 1;
            break;
        case HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_EN:
        case HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_DIS:
            byPassSel   = 1;    /* set to 1 to enable bypass path in ENET_COM */
            empeiLp     = 0;    /* disable EMPEI low-power */
            eclassRst   = 1;    /* assert reset for ECLASS in this slice */
            pathSel     = 1;    /* set to 1 to enable 40-bit bypass mode */
            sliceRst    = 1;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_ENET_COM_TOP_REG_CFG;
    offset = HYPHY20G_ENET_COM_TOP_BIT_CFG_TENGE_BYPASS_SEL_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, byPassSel);
    if(result) return result;

    /* Disable EMPEI low-power for this slice */
    addr = HYPHY20G_ENET_COM_TOP_REG_LOW_PWR_CFG;
    offset = HYPHY20G_ENET_COM_TOP_BIT_LOW_PWR_CFG_TENGE_EMPEI_LP_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, empeiLp);
    if(result) return result;

    /* set TENGE_ECLASS_RST bit */
    addr = HYPHY20G_ENET_COM_TOP_REG_SW_RST;
    offset = HYPHY20G_ENET_COM_TOP_BIT_SW_RST_TENGE_ECLASS_RST_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, eclassRst);
    if(result) return result;

    /* Set Client Interface Path */
    addr = HYPHY20G_ENET_COM_TOP_REG_CLNT_IF_PATH_CFG;
    offset = HYPHY20G_ENET_COM_TOP_BIT_CLNT_IF_PATH_CFG_HS_10B_PATH_SEL_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, pathSel);
    if(result) return result;

    /* Release SLICE_RESET */
    addr = HYPHY20G_ENET_COM_EMPEI_REG_10GE_SLC_N_BUF_CFG_OFFSET(slice);
    /* in-line data has lower priority than hi-priority MGMT and hi-priority 
       MIPS insertion queues (only applicable for 
       MAC terminated Ethernet traffic) */
    offset = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_BUF_CFG_TX_BUFF_LOW_PRI_10GE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 1);
    if(result) return result;

    offset = HYPHY20G_ENET_COM_EMPEI_BIT_10GE_SLC_N_BUF_CFG_SLICE_RESET_10GE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, sliceRst);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gEnetComHsciInit */

/*******************************************************************************
**
**  hyPhy20gEnetComLsciInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes a EMPEI slice associated
**                  with the LSCI subsystem.
**
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - EMPEI slice associated with the LSCI
**                              slice number 0 - 15
**                  mode      - Operation mode
**                              0 - Disable
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
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEnetComLsciInit(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    INT4 result;
    UINT4 addr, offset;
    UINT4 byPassSel = 0, empeiLp = 0, eclassRst = 0, pathSel = 0, sliceRst = 0;
    UINT4 devId, revId, status;

    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);


    /* argument checking */
    if (slice > HYPHY20G_LSCI_NUM_SLICE -1 ||
        mode > LSCI_TRAFFIC_MAX) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (mode == HYPHY20G_DISABLE )
    {
       addr = HYPHY20G_ENET_COM_EMPEI_REG_FEGE_SLC_N_BUF_CFG_OFFSET(slice);
       offset = HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_BUF_CFG_SLICE_RESET_OFF;
       result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 1);
       if(result) return result;  	
       
       /* Initially set FEGE_ECLASS block to reset for the slice */
       addr = HYPHY20G_ENET_COM_TOP_REG_SW_RST;
       offset = HYPHY20G_ENET_COM_TOP_BIT_SW_RST_FEGE_ECLASS_RST_OFF + slice;
       result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 1);
       if(result) return result;       
       
       /* Disable EMPEI low-power for this slice */
       addr = HYPHY20G_ENET_COM_TOP_REG_LOW_PWR_CFG;
       offset = HYPHY20G_ENET_COM_TOP_BIT_LOW_PWR_CFG_FEGE_EMPEI_LP_OFF + slice;
       result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 1);
       if(result) return result; 
       
       addr = HYPHY20G_ENET_COM_TOP_REG_CFG;
       offset = HYPHY20G_ENET_COM_TOP_BIT_CFG_FEGE_BYPASS_SEL_OFF + slice;
       result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 1);
       if(result) return result;
       
       return HYPHY20G_SUCCESS;
    }
    /*
     * Configure ENET_COM based on traffic mode
     */
    switch(mode)
    {
        case LSCI_MAC_TEMINATION:
            byPassSel   = 0;
            empeiLp     = 0;
            eclassRst   = 0;
            pathSel     = 0;
            sliceRst    = 0;
            break;
        case LSCI_GE_GFPT_MAPPING:
        case LSCI_8B10B_TX_RD_INV_EN_CBR:
        case LSCI_8B10B_TX_RD_INV_DIS_CBR:
            byPassSel   = 1;
            empeiLp     = 1;
            eclassRst   = 1;
            pathSel     = 1;
            sliceRst    = 1;
            break;
        case LSCI_FC_4G:
        case LSCI_ESCON:
            /* LS CBR Mode 8 Bit or 16 Bit */
            byPassSel   = 1;
            empeiLp     = 0;
            eclassRst   = 1;
            pathSel     = 1;
            sliceRst    = 1;
            break;
        case LSCI_BYTEALIGED_CBR_STS:
        case LSCI_BYTEALIGED_CBR:
            /* LS CBR Mode 8 Bit or 16 Bit */
            byPassSel   = 1;
            empeiLp     = 1;
            eclassRst   = 1;
            pathSel     = 0;
            sliceRst    = 1;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /*
     * Enable Operations
     */
    /* Remove SW_RESET */
    addr = HYPHY20G_ENET_COM_EMPEI_REG_CFG;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_ENET_COM_EMPEI_BIT_CFG_SW_RESET_OFF, 0);
    if(result) return result;

    addr = HYPHY20G_ENET_COM_TOP_REG_CFG;
    offset = HYPHY20G_ENET_COM_TOP_BIT_CFG_FEGE_BYPASS_SEL_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, byPassSel);
    if(result) return result;

    /* Disable EMPEI low-power for this slice */
    addr = HYPHY20G_ENET_COM_TOP_REG_LOW_PWR_CFG;
    offset = HYPHY20G_ENET_COM_TOP_BIT_LOW_PWR_CFG_FEGE_EMPEI_LP_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, empeiLp);
    if(result) return result;

    /* Initially set FEGE_ECLASS block to reset for the slice */
    addr = HYPHY20G_ENET_COM_TOP_REG_SW_RST;
    offset = HYPHY20G_ENET_COM_TOP_BIT_SW_RST_FEGE_ECLASS_RST_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, eclassRst);
    if(result) return result;

    /* Set Client Interface Path */
    addr = HYPHY20G_ENET_COM_TOP_REG_CLNT_IF_PATH_CFG;
    offset = HYPHY20G_ENET_COM_TOP_BIT_CLNT_IF_PATH_CFG_LS_10B_PATH_SEL_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, pathSel);
    if(result) return result;

    addr = HYPHY20G_ENET_COM_EMPEI_REG_FEGE_SLC_N_BUF_CFG_OFFSET(slice);
    /* in-line data has lower priority than hi-priority MGMT and hi-priority 
       MIPS insertion queues (only applicable for 
       MAC terminated Ethernet traffic) */
    offset = HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_BUF_CFG_TX_BUFF_LOW_PRI_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 1);
    if(result) return result;
    
    /* Release SLICE_RESET */
    offset = HYPHY20G_ENET_COM_EMPEI_BIT_FEGE_SLC_N_BUF_CFG_SLICE_RESET_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, sliceRst);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gEnetComLsciInit */


/*******************************************************************************
**
**  hyPhy20gEnetComGsup43Init
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the MAC_GSUP43 block in the 
**                  ENET_COM subsystem, depending on the specified operating
**                  mode. 
**
**                  The MAC_GSUP43 block includes GSUP43 block, DSM block and 
**                  glue logic and it physically locates on the ENET_COM 
**                  subsystem. It is designed to support two data path:  
**                    1) G.Sup43 Clause 7.3 Terminating application through MAC;
**                    2) G.Sup43 Clause 6.2 Terminating application through MAC.
**
**                  For the G.Sup43 Clause 7.3 Terminating mode through MAC 
**                  application, the MAC_GSUP43 block connects to HSCI MAC 
**                  block at the system side serving as PCS layer, and 
**                  it also connects to GFP subsystem at the line side.
**
**                  For the G.Sup43 Clause 6.2 Terminating mode through MAC 
**                  application, the MAC_GSUP43 block similarly inter-connects 
**                  the HSCI MAC block and GFP subsystem, however the  
**                  Tx GSUP43 slice in ENET_COM is bypassed.
**
**                  This function should be called by hyPhy20gHsciInit API only.
**
**                  Alternatively, this function can be called to disable the
**                  GSUP43 block in the ENET_COM subsystem.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - ENET_COM MAC_GSUP43 slice
**                              0 - ENET_COM MAC_GSUP43 slice associated with
**                                  HSCI instance 0
**                              1 - ENET_COM MAC_GSUP43 slice associated with
**                                  HSCI instance 1
**                  mode      - operating mode of the ENET_COM MAC_GSUP43 block
**                              0 - disable the GSUP43 slice in ENET_COM
**                              1 - enable the MAC_GSUP43 block in G.Sup43 
**                                  Clause 7.3 Terminating application
**                                  through MAC
**                              2 - enable the MAC_GSUP43 block in G.Sup43 
**                                  Clause 6.2 Terminating application
**                                  through MAC
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEnetComGsup43Init(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
    INT4 result;
    UINT4 addr;
    UINT4 regData;
    UINT4 lowpwrAddr;
    UINT4 rstAddr;   
    UINT4 lowpwrOff; 
    UINT4 rstOff;
    UINT4 gsup43CfgAddr; 
    UINT4 c62MdMsk;
    UINT4 c62MdVal;
    UINT4 threshMsk;
    UINT4 rxEnblVal;
    
    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        mode > 2) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    lowpwrAddr  = HYPHY20G_ENET_COM_TOP_REG_LOW_PWR_CFG;
    rstAddr     = HYPHY20G_ENET_COM_TOP_REG_SW_RST;

    /* set lowpwr bit to 1 */
    lowpwrOff = HYPHY20G_ENET_COM_TOP_BIT_LOW_PWR_CFG_GSUP43_LP_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, lowpwrAddr, lowpwrOff, 1);
    if(result) return result;

    /* reset GSUP43 slice at ENET_COM top level */
    rstOff = HYPHY20G_ENET_COM_TOP_BIT_SW_RST_GSUP43_0_RST_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, rstAddr, rstOff, 1);
    if(result) return result;

    /* disable operations */
    if (mode == HYPHY20G_DISABLE){           
        return HYPHY20G_SUCCESS;
    }

    /*
     * enable operations
     */
    /* power up the GSUP43 slice in ENET_COM subsystem */
    lowpwrOff = HYPHY20G_ENET_COM_TOP_BIT_LOW_PWR_CFG_GSUP43_LP_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, lowpwrAddr, lowpwrOff, 0);
    if(result) return result;

    /* remove GSUP43 slice reset in ENET_COM subsystem */
    rstOff = HYPHY20G_ENET_COM_TOP_BIT_SW_RST_GSUP43_0_RST_OFF + slice;
    result = sysHyPhy20gBitWrite(fileHndl, rstAddr, rstOff, 0);
    if(result) return result;
	
    /*
     * configure Gsup43 settings at the ENET_COM subsystem top level
     */
    if (slice == 0) {           
        gsup43CfgAddr = HYPHY20G_ENET_COM_TOP_REG_GSUP43_0_CFG;
        c62MdMsk      = HYPHY20G_ENET_COM_TOP_BIT_GSUP43_0_CFG_GSUP43_0_C62_MODE_MSK;
        threshMsk     = HYPHY20G_ENET_COM_TOP_BIT_GSUP43_0_CFG_GSUP43_0_OS_DISCRD_THRESH_MSK;
    } else {
        gsup43CfgAddr = HYPHY20G_ENET_COM_TOP_REG_GSUP43_1_CFG;
        c62MdMsk      = HYPHY20G_ENET_COM_TOP_BIT_GSUP43_1_CFG_GSUP43_1_C62_MODE_MSK;
        threshMsk     = HYPHY20G_ENET_COM_TOP_BIT_GSUP43_1_CFG_GSUP43_1_OS_DISCRD_THRESH_MSK;
    }    

    /* set GSUP43_x_C62_MODE to b'1 for mode '2' */
    c62MdVal = (mode == 1) ? 0 : 1;

    result = sysHyPhy20gRead(fileHndl, gsup43CfgAddr, &regData);
    if(result) return result;

	result = hyPhy20gVarFieldWrite(&regData, c62MdMsk, c62MdVal);
	if(result) return result;

	result = hyPhy20gVarFieldWrite(&regData, threshMsk, 0x50);
	if(result) return result;
		
    result = sysHyPhy20gWrite(fileHndl, gsup43CfgAddr, regData);
    if(result) return result;

    /*
     * configure Rx GSUP43 slice in ENET_COM
     */
    addr = HYPHY20G_ENET_COM_GSUP43_REG_RX_CFG;
    addr = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_ENET_COM_GSUP43_BIT_RX_CFG_RX_SW_RST_OFF,
                1);
    if(result) return result;

    rxEnblVal = (mode == 1) ? 1 : 0;
    addr = HYPHY20G_ENET_COM_GSUP43_REG_RX_GSUP43_CFG;
    addr = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_ENET_COM_GSUP43_BIT_RX_GSUP43_CFG_RX_GSUP43_C73_EN_OFF,
                rxEnblVal);
    if(result) return result;
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_ENET_COM_GSUP43_BIT_RX_GSUP43_CFG_RX_FULL_OS_RPT_DIS_OFF,
                0);
    if(result) return result;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, 
                HYPHY20G_ENET_COM_GSUP43_BIT_RX_GSUP43_CFG_RX_MIN_OS_RPT_MSK, 
                0x3);
    if(result) return result;

    /*
     * if it is in clause 7.3 mode, configure Tx GSUP43 slice in ENET_COM
     */
    if (mode == 1) {           
        addr = HYPHY20G_ENET_COM_GSUP43_REG_TX_CFG_1;
        addr = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(addr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_ENET_COM_GSUP43_BIT_TX_CFG_1_TX_SW_RST_OFF,
                    1);
        if(result) return result;

        addr = HYPHY20G_ENET_COM_GSUP43_REG_TX_CFG_2;
        addr = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(addr, slice);
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_ENET_COM_GSUP43_BIT_TX_CFG_2_TX_GSUP43_C73_EN_OFF,
                    1);
        if(result) return result;

        /* GSUP43_C73 TX block insert the IPG itself */
        result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_ENET_COM_GSUP43_BIT_TX_CFG_2_FORCE_MIN_IPG_OFF,
                    0);
        if(result) return result;

        result = sysHyPhy20gBitWrite(fileHndl, addr,
                    HYPHY20G_ENET_COM_GSUP43_BIT_TX_CFG_2_UNIDIR_EN_OFF,
                    1);
        if(result) return result;
    }

    /* enable GSUP43 slice in ENET_COM */
    addr = HYPHY20G_ENET_COM_GSUP43_REG_CFG;
    addr = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, addr,
                HYPHY20G_ENET_COM_GSUP43_BIT_CFG_SW_RST_OFF, 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gEnetComGsup43Init */


/*******************************************************************************
**
**  utilHyPhy20gEmpeiInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes the EMPEI subsystem in ENET_COM.
**
**
**
**  INPUTS:         fileHndl  - device handle
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gEmpeiInit(struct file *fileHndl)
{
    INT4 result;
    UINT4 addr, offset, val;

    addr = HYPHY20G_ENET_COM_EMPEI_REG_INLN_BUF_THS;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;
    val = mHYPHY20G_SET_FIELD4(val, 0x1,
            HYPHY20G_ENET_COM_EMPEI_BIT_INLN_BUF_THS_TX_INLINE_BUFF_FULL_THRSH_10GE_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_INLN_BUF_THS_TX_INLINE_BUFF_FULL_THRSH_10GE_OFF);
    val = mHYPHY20G_SET_FIELD4(val, 0x1,
            HYPHY20G_ENET_COM_EMPEI_BIT_INLN_BUF_THS_TX_INLINE_BUFF_FULL_THRSH_FEGE_MSK,
            HYPHY20G_ENET_COM_EMPEI_BIT_INLN_BUF_THS_TX_INLINE_BUFF_FULL_THRSH_FEGE_OFF);
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* Take FEGE_MGMT_EMPEI_LP out of low power. This bit must be set to b'0
       to enable mgmt datapath */
    addr = HYPHY20G_ENET_COM_TOP_REG_LOW_PWR_CFG;
    offset = HYPHY20G_ENET_COM_TOP_BIT_LOW_PWR_CFG_FEGE_MGMT_EMPEI_LP_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gEmpeiInit */

/*******************************************************************************
**
**  utilHyPhy20gEnetComGsup43TxCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure Tx GSUP43 block in ENET_COM subsystem.
**
**                  Used for G.Sup43 Clause 7.3 Terminating application
**                  through MAC operating mode (mode '6' in hyPhy20gHsciInit).
**
**                  This function can optionally be called after
**                  hyPhy20gHsciInit but prior to hyPhy20gHsciDataPathEnable.
**
**  INPUTS:         fileHndl   - device handle
**                  slice     - ENET_COM MAC_GSUP43 slice
**                              0 - ENET_COM MAC_GSUP43 slice associated with
**                                  HSCI instance 0
**                              1 - ENET_COM MAC_GSUP43 slice associated with
**                                  HSCI instance 1
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
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gEnetComGsup43TxCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 pktType, UINT4 osType)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 mask;

    /* argument checking */
    if (slice > 1 || pktType > 0xFF || osType > 0xFF) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_ENET_COM_GSUP43_REG_TX_CFG_1;
    addr = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    mask = HYPHY20G_ENET_COM_GSUP43_BIT_TX_CFG_1_TX_PKT_TYPE_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, pktType);
    if (result) return result;
        
    mask = HYPHY20G_ENET_COM_GSUP43_BIT_TX_CFG_1_TX_OS_TYPE_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, osType);
    if (result) return result;
        
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gEnetComGsup43TxCfg */

/*******************************************************************************
**
**  utilHyPhy20gEnetComGsup43RxCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    Configure RX GSUP43 block in ENET_COM subsystem.
**
**                  Used for G.Sup43 Clause 7.3 Terminating application
**                  through MAC operating mode (mode '6' in hyPhy20gHsciInit).
**
**                  This function can optionally be called after
**                  hyPhy20gHsciInit but prior to hyPhy20gHsciDataPathEnable.
**
**  INPUTS:         fileHndl   - device handle
**                  slice     - ENET_COM MAC_GSUP43 slice
**                              0 - ENET_COM MAC_GSUP43 slice associated with
**                                  HSCI instance 0
**                              1 - ENET_COM MAC_GSUP43 slice associated with
**                                  HSCI instance 1
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
INT4 utilHyPhy20gEnetComGsup43RxCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 pktType, UINT4 osType)
{
    INT4 result;
    UINT4 addr, val;
    UINT4 mask;

    /* argument checking */
    if (slice > 1 || pktType > 0xFF || osType > 0xFF) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_ENET_COM_GSUP43_REG_RX_GSUP43_CFG;
    addr = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    mask = HYPHY20G_ENET_COM_GSUP43_BIT_RX_GSUP43_CFG_RX_PKT_TYPE_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, pktType);
    if (result) return result;
        
    mask = HYPHY20G_ENET_COM_GSUP43_BIT_RX_GSUP43_CFG_RX_OS_TYPE_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, osType);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gEnetComGsup43RxCfg */


/* end of file */
