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
**  FILE        : hyphy20g_hsci_status.c
**
**  $Date: 2011-04-29 06:36:46 +0800 (Fri, 29 Apr 2011) $
**
**  $Revision: 11881 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_hsci_status.h"

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/


/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/


/*******************************************************************************
**
**  hyPhy20gHsciTopLevelGetStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the HSCI Top Level Status.
**
**                  This function is used for all HSCI applications.
**
**  INPUTS:        *fileHndl    - device handle
**                  slice       - 0 - HSCI slice 0
**                                1 - HSCI slice 1
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31:1 - RESERVED
**                                  Bit 0    - LOS_INT_V
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciTopLevelGetStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, msk, losStat;

    /* argument checking */
    if (slice > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Read LOS status */
    addr = HYPHY20G_HSCI_TOP_REG_LOS_INT_VAL;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk = HYPHY20G_HSCI_TOP_BIT_LOS_INT_VAL_LOS_INT_V_MSK;

    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &losStat);
    if (result) return result;
    *statusBits = losStat;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciTopLevelGetStatus */


/*******************************************************************************
**
**  hyPhy20gHsci10GeGetStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the HSCI ENET_10G Status.
**
**                  This function is used for 10GE MAC Terminated application
**                  only.
**
**  INPUTS:        *fileHndl    - device handle
**                  slice       - 0 - HSCI slice 0
**                                1 - HSCI slice 1
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31:4 - RESERVED
**                                  Bit 3    - TXSTAT_32
**                                  Bit 2    - DIAG_HOSTPAUSEDONE
**                                  Bit 1    - TXACTIVE
**                                  Bit 0    - PAUSED
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeGetStatus(struct file *fileHndl, UINT4 slice,
                               UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, stat = 0, diagStat = 0;

    /* argument checking */
    if (slice > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    addr = HYPHY20G_HSCI_ENET_10G_TXXG_REG_STAT;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &stat);
    if(result) return result;

    /* retrieve status */
    addr = HYPHY20G_HSCI_ENET_10G_TXXG_REG_DIAG_STAT;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &diagStat);
    if(result) return result;

    *statusBits = ((diagStat & 0x3) << 2) + (stat & 0x3);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeGetStatus */


/*******************************************************************************
**
**  hyPhy20gHsci10GeGetTxxgStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the HSCI ENET_10G TXXG Status.
**
**                  It is recommended to have the device level WCI_MODE bit
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts within the registers accessed by
**                  this function.
**
**                  This function is used for 10GE MAC Terminated application
**                  only.
**
**  INPUTS:        *fileHndl    - device handle
**                  slice       - 0 - HSCI slice 0
**                                1 - HSCI slice 1
**                  intClear    - 1 - all *INT_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this
**                               function will retain their values.
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31:5 - RESERVED
**                                  Bit 4    - FIFO_ERR_I
**                                  Bit 3    - FIFO_UDR_I
**                                  Bit 2    - MAX_LERR_I
**                                  Bit 1    - MIN_LERR_I
**                                  Bit 0    - XFER_I
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeGetTxxgStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, regData;

    /* argument checking */
    if (slice > 1 || intClear > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    addr = HYPHY20G_HSCI_ENET_10G_TXXG_REG_INT;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;
    *statusBits = regData >> 11;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeGetTxxgStatus */


/*******************************************************************************
**
**  hyPhy20gHsci10GeGetRxxgStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the HSCI ENET_10G RXXG Status.
**
**                  It is recommended to have the device level WCI_MODE bit
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts within the registers accessed by
**                  this function.
**
**                  This function is used for 10GE MAC Terminated application
**                  only.
**
**  INPUTS:        *fileHndl    - device handle
**                  slice       - 0 - HSCI slice 0
**                                1 - HSCI slice 1
**                  intClear    - 1 - all *INT_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this
**                               function will retain their values.
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31:11- RESERVED
**                                  Bit 10   - MIN_LERR_I
**                                  Bit  9   - MAX_LERR_I
**                                  Bit  8   - ODD_ALIGN_I
**                                  Bit  7   - LINE_ERR_I
**                                  Bit  6   - CONTIG_ERR_I
**                                  Bit  5   - RX_OVR_I
**                                  Bit  4   - ADR_FILTER_I
**                                  Bit  3   - ERR_DETECT_I
**                                  Bit  2   - XFER_I
**                                  Bit  1   - IFG_ERR_I
**                                  Bit  0   - PRMB_ERR_I
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeGetRxxgStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, regData;

    /* argument checking */
    if (slice > 1 || intClear > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    addr = HYPHY20G_HSCI_ENET_10G_RXXG_REG_INT;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    *statusBits = regData >> 5;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeGetRxxgStatus */

/*******************************************************************************
**
**  hyPhy20gHsciMstatxGetRxStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the MSTATX Rx Status.
**
**                  It is recommended to have the device level WCI_MODE bit
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts within the registers accessed by
**                  this function.
**
**                  This function is used for 10GE MAC Terminated application
**                  only.
**
**  INPUTS:        *fileHndl    - device handle
**                  slice       - MSTATX instance in:
**                                0  - HSCI 0
**                                1  - HSCI 1
**                  intClear    - 1 - all *INT_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this
**                               function will retain their values.
**                 *statusBits0 - Pointer to storage for status bits
**                                  Bit 31:28- RESERVED
**                                  Bit 27   - RX_VARIABLE_I
**                                  Bit 26   - RX_STATION_ADDRESS_FILTERED_I
**                                  Bit 25   - RX_FRAMES_1519_TO_MAX_SIZE_I
**                                  Bit 24   - RX_FRAMES_1024_TO_1518_OCTETS_I
**                                  Bit 23   - RX_FRAMES_512_TO_1023_OCTETS_I
**                                  Bit 22   - RX_FRAMES_256_TO_511_OCTETS_I
**                                  Bit 21   - RX_FRAMES_128_TO_255_OCTETS_I
**                                  Bit 20   - RX_FRAMES_65_TO_127_OCTETS_I
**                                  Bit 19   - RX_FRAMES_64_OCTETS_I
**                                  Bit 18   - RX_SHORT_LENGTH_CRC_ERROR_I
**                                  Bit 17   - RX_SHORT_LENGTH_ERROR_I
**                                  Bit 16   - RX_LONG_LENGTH_CRC_ERROR_I
**                                  Bit 15   - RX_LONG_LENGTH_ERROR_I
**                                  Bit 14   - RX_INRANGE_LENGTH_ERROR_I
**                                  Bit 13   - RX_FRAMES_LOST_DUE_TO_INTERNAL_MAC_ERROR_I
**                                  Bit 12   - RX_SYMBOL_ERROR_I
**                                  Bit 11   - RESERVED
**                                  Bit 10   - RX_FCS_ERROR_I
**                                  Bit 9    - RX_MAC_CONTROL_FRAMES_OK_I
**                                  Bit 8    - RX_MAC_PAUSE_CONTROL_FRAMES_OK_I
**                                  Bit 7    - RX_TAGGED_FRAMES_OK_I
**                                  Bit 6    - RX_MULTICAST_FRAMES_OK_I
**                                  Bit 5    - RX_BROADCAST_FRAMES_OK_I
**                                  Bit 4    - RX_UNICAST_FRAMES_OK_I
**                                  Bit 3    - RX_OCTETS_I
**                                  Bit 2    - RX_FRAMES_I
**                                  Bit 1    - RX_OCTETS_OK_I
**                                  Bit 0    - RX_FRAMES_OK_I
**                 *statusBits1 - Pointer to storage for status bits
**                                  Bit 31:5 - RESERVED
**                                  Bit 4    - RX_FRAMES_2501_TO_MAX_SIZE_I
**                                  Bit 3    - RX_FRAMES_2001_TO_2500_OCTETS_I
**                                  Bit 2    - RX_FRAMES_1549_TO_2000_OCTETS_I
**                                  Bit 1    - RX_FRAMES_1523_TO_1548_OCTETS_I
**                                  Bit 0    - RX_FRAMES_1519_TO_1522_OCTETS_I
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciMstatxGetRxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits0,
                                   UINT4 *statusBits1)
{
    INT4 result;

    /* argument checking */
    if (slice > 1 || intClear > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = utilHyPhy20gMstatxGetRxStatus(fileHndl, slice, intClear,
                                           statusBits0, statusBits1);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciMstatxGetRxStatus */

/*******************************************************************************
**
**  hyPhy20gHsciMstatxGetTxStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the MSTATX Tx Status.
**
**                  It is recommended to have the device level WCI_MODE bit
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts within the registers accessed by
**                  this function.
**
**                  This function is used for 10GE MAC Terminated application
**                  only.
**
**  INPUTS:        *fileHndl    - device handle
**                  slice       - MSTATX instance in:
**                                0  - HSCI 0
**                                1  - HSCI 1
**                  intClear    - 1 - all *INT_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this
**                               function will retain their values.
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31   - TX_FRAMES_2501_TO_MAX_SIZE_I
**                                  Bit 30   - TX_FRAMES_2001_TO_2500_OCTETS_I
**                                  Bit 29   - TX_FRAMES_1549_TO_2000_OCTETS_I
**                                  Bit 28   - TX_FRAMES_1523_TO_1548_OCTETS_I
**                                  Bit 27   - TX_FRAMES_1519_TO_1522_OCTETS_I
**                                  Bit 26   - RX_FRAMES_1523_TO_MAX_SIZE_I
**                                  Bit 25   - TX_VARIABLE_I
**                                  Bit 24   - TX_FRAMES_1519_TO_MAX_SIZE_I
**                                  Bit 23   - TX_FRAMES_1024_TO_1518_OCTETS_I
**                                  Bit 22   - TX_FRAMES_512_TO_1023_OCTETS_I
**                                  Bit 21   - TX_FRAMES_256_TO_511_OCTETS_I
**                                  Bit 20   - TX_FRAMES_128_TO_255_OCTETS_I
**                                  Bit 19   - TX_FRAMES_65_TO_127_OCTETS_I
**                                  Bit 18   - TX_FRAMES_64_OCTETS_I
**                                  Bit 17   - TX_MULTICAST_FRAMES_ATTEMPTED_I
**                                  Bit 16   - TX_BROADCAST_FRAMES_ATTEMPTED_I
**                                  Bit 15   - TX_UNICAST_FRAMES_ATTEMPTED_I
**                                  Bit 14   - TX_MULTICAST_FRAMES_OK_I
**                                  Bit 13   - TX_BROADCAST_FRAMES_OK_I
**                                  Bit 12   - TX_UNICAST_FRAMES_OK_I
**                                  Bit 11   - TX_SQET_TEST_ERROR_I
**                                  Bit 10   - TX_FRAMES_1523_TO_MAX_SIZE_I
**                                  Bit 9    - TX_MAC_ERROR_I
**                                  Bit 8    - TX_SYSTEM_ERROR_I
**                                  Bit 7    - TX_LONG_LENGTH_ERROR_I
**                                  Bit 6    - TX_SHORT_LENGTH_ERROR_I
**                                  Bit 5    - TX_FCS_ERROR_I
**                                  Bit 4    - TX_MAC_PAUSE_CONTROL_FRAMES_OK_I
**                                  Bit 3    - TX_TAGGED_FRAMES_OK_I
**                                  Bit 2    - TX_OCTETS_I
**                                  Bit 1    - TX_OCTETS_OK_I
**                                  Bit 0    - TX_FRAMES_OK_I
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciMstatxGetTxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;

    /* argument checking */
    if (slice > 1 || intClear > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = utilHyPhy20gMstatxGetTxStatus(fileHndl, slice, intClear,
                                           statusBits);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciMstatxGetTxStatus */


/*******************************************************************************
**
**  hyPhy20gHsciGsup43GetRxStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the HSCI Gsup43 block Status.
**
**                  This function is used for both 10GE MAC Terminated
**                  and GSUP43 CLAUSE 7.3 modes.
**
**                  Note, the ALL_ONES_OR_ZEROS_V bit is only available in 
**                  Rev B device. 
**
**  INPUTS:        *fileHndl    - device handle
**                  slice       - 0 - HSCI slice 0
**                                1 - HSCI slice 1
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31   - RF_FAULT             
**                                  Bit 30   - LF_FAULT             
**                                  Bit 29:8 - RESERVED
**                                  Bit 7    - ALL_ONES_OR_ZEROS_V  
**                                  Bit 4    - RX_FAULT_V           
**                                  Bit 3    - LINK_FAIL_V          
**                                  Bit 2    - HI_BER_V             
**                                  Bit 1    - LOS_SYNC_V           
**                                  Bit 0    - LOS_V                
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciGsup43GetRxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, regData;

    /* argument checking */
    if (slice > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    addr = HYPHY20G_HSCI_ENET_10G_GSUP43_REG_RX_STAT;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;
    *statusBits = regData;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciGsup43GetRxStatus */

/*******************************************************************************
**
**  hyPhy20gHsciGsup43GetTxStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the HSCI Gsup43 block Status.
**
**                  This function is used for both 10GE MAC Terminated
**                  and GSUP43 CLAUSE 7.3 modes.
**
**  INPUTS:        *fileHndl    - device handle
**                  slice       - 0 - HSCI slice 0
**                                1 - HSCI slice 1
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31:3 - RESERVED
**                                  Bit  2   - SEND_IDLE_V
**                                  Bit  1   - SEND_LF_V
**                                  Bit  0   - SEND_RF_V
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciGsup43GetTxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, regData;

    /* argument checking */
    if (slice > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    addr = HYPHY20G_HSCI_ENET_10G_GSUP43_REG_TX_STAT;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;
    *statusBits = (regData >> 2) & 0x7;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciGsup43GetTxStatus */

/*******************************************************************************
**
**  hyPhy20gHsciFcCbrGetStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the HSCI FC_CBR block Status.
**
**                  This function is used for FC 8G and high speed CBR
**                  applications.
**
**  INPUTS:        *fileHndl    - device handle
**                  slice       - 0 - HSCI slice 0
**                                1 - HSCI slice 1
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31:21 - RESERVED
**                                  Bit 20    - TX_DISP_INVERT_V
**                                  Bit 19    - TX_DLOLB_V
**                                  Bit 18    - RESERVED
**                                  Bit 17    - TX_SYNC_V
**                                  Bit 16    - RESERVED
**                                  Bit 15    - RESERVED
**                                  Bit 14    - UNUSED
**                                  Bit 13    - UNUSED
**                                  Bit 12:8  - RESERVED
**                                  Bit 7     - UNUSED
**                                  Bit 6     - RESERVED
**                                  Bit 5     - RX_XDET_V
**                                  Bit 4     - RESERVED
**                                  Bit 3     - RX_DLOLB_V
**                                  Bit 2     - RX_ASD_V
**                                  Bit 1     - RX_SYNC_V
**                                  Bit 0     - RX_LCV_V
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciFcCbrGetStatus(struct file *fileHndl, UINT4 slice,
                                UINT4 *statusBits)
{
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = utilHyPhy20gFcCbrGetStatus(fileHndl, slice, statusBits);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciFcCbrGetStatus */

/*******************************************************************************
**
**  hyPhy20gHsciPmmGetStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the HSCI PMM block Status.
**
**                  This function is used for GE GFP-T (Transparent GE),
**                  FC 4G and subrates, as well as low speed CBR applications.
**
**                  It is recommended to have the device level WCI_MODE bit
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts within the registers accessed by
**                  this function.
**
**  INPUTS:        *fileHndl   - device handle
**                  slice      - HSCI 0 to 1
**                  intClear   - 1 - all *_I interrupt bits accessed by this
**                                   function will be cleared by writing them
**                                   to '1'
**                             - 0 - all *_I interrupt bits accessed by this
**                                   function will retain their values if 
**                                   WCIMODE is used.
**                 *statusBits - Pointer to storage for status bits
**                                  Bit 31-30: MKR_ECC_ERR_INT_I[1:0]
**                                  Bit 29-28: MON_ECC_ERR_INT_I[1:0]
**                                  Bit 27   : TVB_FIFO_OVERFLOW_I
**                                  Bit 26   : TVB_FIFO_UNDERFLOW_I
**                                  Bit 25-9 : RESERVED
**                                  Bit 8    : RESERVED
**                                  Bit 7-6  : RESERVED
**                                  Bit 5    : MON_EOP_ERR_INT_I
**                                  Bit 4    : MON_SOP_ERR_INT_I
**                                  Bit 3    : MON_DP_OVERFLOW_INT_I
**                                  Bit 2    : MKR_DSM_UNDERFLOW_INT_I
**                                  Bit 1    : MKR_DSM_OVERFLOW_INT_I
**                                  Bit 0    : MKR_DP_OVERFLOW_INT_I
**
**  OUTPUTS:        None       - None
**
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciPmmGetStatus(struct file *fileHndl, UINT4 slice,
                              UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr;
    UINT4 pmmStat;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > 1 || intClear > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    addr = HYPHY20G_HSCI_TOP_REG_PAC_MKR_MON_AND_LOS_INT;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &pmmStat);
    if(result) return result;

    *statusBits = pmmStat;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciPmmGetStatus */



/*******************************************************************************
**
**  hyPhy20gEnetComGsup43GetTopStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the ENET_COM GSUP43 top level status
**                  for both slice 0 and 1. The status register physically  
**                  locates on ENET_COM subsystem, but it functionally relates 
**                  to G.Sup43 Terminating application in HSCI.
**
**                  This function is used for G.Sup43 Clause 7.3 and Clause 6.2
**                  G.Sup43 Clause 7.3 Terminating application through MAC
**                  modes. 
**
**  INPUTS:        *fileHndl    - device handle
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31:12 - RESERVED
**                                  Bit 11    - GSUP43_1_TX_FIFO_OVERFLOW_I 
**                                  Bit 10    - GSUP43_0_TX_FIFO_OVERFLOW_I 
**                                  Bit 9:6   - RESERVED
**                                  Bit 5     - GSUP43_1_RX_FIFO_UNDERFLOW_I
**                                  Bit 4     - GSUP43_0_RX_FIFO_UNDERFLOW_I
**                                  Bit 3     - GSUP43_1_RX_FIFO_OVERFLOW_I 
**                                  Bit 2     - GSUP43_0_RX_FIFO_OVERFLOW_I 
**                                  Bit 1     - PTP_DCSU_TSTMPR_CPTR_EVNT_I 
**                                  Bit 0     - IRIGI_IDLE_I 
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEnetComGsup43GetTopStatus(struct file *fileHndl, UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, regData;

    /* retrieve status */
    addr = HYPHY20G_ENET_COM_TOP_REG_INT_INDICATION2;
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;
    *statusBits = regData;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gEnetComGsup43GetTopStatus */


/*******************************************************************************
**
**  hyPhy20gEnetComGsup43GetRxStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the ENET_COM GSUP43 Rx slice status.
**                  The status register physically locates on ENET_COM 
**                  subsystem, but it functionally relates to G.Sup43 
**                  Terminating application in HSCI.
**
**                  This function is used for G.Sup43 Clause 7.3 and Clause 6.2
**                  G.Sup43 Clause 7.3 Terminating application through MAC
**                  modes. 
**
**  INPUTS:        *fileHndl    - device handle
**                  slice       - ENET_COM MAC_GSUP43 slice
**                                0 - ENET_COM MAC_GSUP43 slice associated with
**                                    HSCI instance 0
**                                1 - ENET_COM MAC_GSUP43 slice associated with
**                                    HSCI instance 1
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31   - RF_FAULT             
**                                  Bit 30   - LF_FAULT             
**                                  Bit 29:8 - RESERVED
**                                  Bit 7    - ALL_ONES_OR_ZEROS_V  
**                                  Bit 4    - RX_FAULT_V           
**                                  Bit 3    - LINK_FAIL_V          
**                                  Bit 2    - HI_BER_V             
**                                  Bit 1    - LOS_SYNC_V           
**                                  Bit 0    - LOS_V                
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEnetComGsup43GetRxStatus(struct file *fileHndl, UINT4 slice,
                                      UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, regData;

    /* argument checking */
    if (slice > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    addr = HYPHY20G_ENET_COM_GSUP43_REG_RX_STAT;
    addr = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;
    *statusBits = regData;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gEnetComGsup43GetRxStatus */

/*******************************************************************************
**
**  hyPhy20gEnetComGsup43GetTxStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the ENET_COM GSUP43 Tx slice status.
**                  The status register physically locates on ENET_COM 
**                  subsystem, but it functionally relates to G.Sup43 
**                  Terminating application in HSCI.
**
**                  This function is used for G.Sup43 Clause 7.3 and Clause 6.2
**                  G.Sup43 Clause 7.3 Terminating application through MAC
**                  modes. 
**
**  INPUTS:        *fileHndl    - device handle
**                  slice       - ENET_COM MAC_GSUP43 slice
**                                0 - ENET_COM MAC_GSUP43 slice associated with
**                                    HSCI instance 0
**                                1 - ENET_COM MAC_GSUP43 slice associated with
**                                    HSCI instance 1
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31:3 - RESERVED
**                                  Bit  2   - SEND_IDLE_V
**                                  Bit  1   - SEND_LF_V
**                                  Bit  0   - SEND_RF_V
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEnetComGsup43GetTxStatus(struct file *fileHndl, UINT4 slice,
                                      UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, regData;

    /* argument checking */
    if (slice > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    addr = HYPHY20G_ENET_COM_GSUP43_REG_TX_STAT;
    addr = HYPHY20G_ENET_COM_GSUP43_TSB_SLICE_REG_OFFSET(addr, slice);

    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;
    *statusBits = (regData >> 2) & 0x7;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gEnetComGsup43GetTxStatus */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
