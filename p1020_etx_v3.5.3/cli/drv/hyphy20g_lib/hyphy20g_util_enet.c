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
**  FILE        : hyphy20g_util_enet.c
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
#include "hyphy20g_util_enet.h"

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/


/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/
/*******************************************************************************
**
**  utilHyPhy20gMstatxGetRxStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the MSTATX Rx Status.
**
**                  It is recommended to have the device level WCI_MODE bit
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts that share registers with the
**                  interrupts accessed by this function.
**
**  INPUTS:        *fileHndl    - device handle
**                 slice        - MSTATX instance in:
**                                0  - HSCI 0
**                                1  - HSCI 1
**                                2  - LSCI 0
**                                3  - LSCI 1
**                                4  - LSCI 2
**                                ......
**                                17 - LSCI 15
**                                18 - LSCI_SGMII (mgmt port)
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
INT4 utilHyPhy20gMstatxGetRxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits0,
                                   UINT4 *statusBits1)
{
    INT4 result;
    UINT4 addr1, regData1;
    UINT4 addr2, regData2;
    UINT4 addr3, regData3;

    /* argument checking */
    if (slice > 18 || intClear > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice < 2){
        /* HSCI case */
        addr1 = HYPHY20G_HSCI_ENET_10G_MSTATX_REG_INT_1;
        addr2 = HYPHY20G_HSCI_ENET_10G_MSTATX_REG_INT_2;
        addr3 = HYPHY20G_HSCI_ENET_10G_MSTATX_REG_INT_3;
        addr1 = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr1, slice);
        addr2 = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr2, slice);
        addr3 = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr3, slice);
    } else if (slice > 1 && slice < 18) {
        /* LSCI case */
        addr1 = HYPHY20G_LSCI_ENET_FEGE_MSTATX_REG_INT_1;
        addr2 = HYPHY20G_LSCI_ENET_FEGE_MSTATX_REG_INT_2;
        addr3 = HYPHY20G_LSCI_ENET_FEGE_MSTATX_REG_INT_3;
        addr1 = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr1, slice-2);
        addr2 = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr2, slice-2);
        addr3 = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr3, slice-2);
    } else {
        /* SGMII case */
        addr1 = HYPHY20G_SGMII_ENET_FEGE_MSTATX_REG_INT_1;
        addr2 = HYPHY20G_SGMII_ENET_FEGE_MSTATX_REG_INT_2;
        addr3 = HYPHY20G_SGMII_ENET_FEGE_MSTATX_REG_INT_3;
    }

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr1, &regData1);
    if(result) return result;

    result = sysHyPhy20gRead(fileHndl, addr2, &regData2);
    if(result) return result;

    result = sysHyPhy20gRead(fileHndl, addr3, &regData3);
    if(result) return result;

    *statusBits0 = (regData2 << 16) + regData1;
    *statusBits1 = regData3;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr1, 0xFFFFFFFF);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, addr2, 0xFFFFFFFF);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, addr3, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gMstatxGetRxStatus */


/*******************************************************************************
**
**  utilHyPhy20gMstatxGetTxStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the MSTATX Tx Status.
**
**                  It is recommended to have the device level WCI_MODE bit
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts that share registers with the
**                  interrupts accessed by this function.
**
**  INPUTS:        *fileHndl    - device handle
**                 slice        - MSTATX instance in:
**                                0  - HSCI 0
**                                1  - HSCI 1
**                                2  - LSCI 0
**                                3  - LSCI 1
**                                4  - LSCI 2
**                                ......
**                                17 - LSCI 15
**                                18 - LSCI_SGMII (mgmt port)
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
INT4 utilHyPhy20gMstatxGetTxStatus(struct file *fileHndl, UINT4 slice,
                                   UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr4, regData4;
    UINT4 addr5, regData5;

    /* argument checking */
    if (slice > 18 || intClear > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice < 2){
        /* HSCI case */
        addr4 = HYPHY20G_HSCI_ENET_10G_MSTATX_REG_INT_4;
        addr5 = HYPHY20G_HSCI_ENET_10G_MSTATX_REG_INT_5;
        addr4 = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr4, slice);
        addr5 = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr5, slice);
    } else if (slice > 1 && slice < 18) {
        /* LSCI case */
        addr4 = HYPHY20G_LSCI_ENET_FEGE_MSTATX_REG_INT_4;
        addr5 = HYPHY20G_LSCI_ENET_FEGE_MSTATX_REG_INT_5;
        addr4 = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr4, slice-2);
        addr5 = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr5, slice-2);
    } else {
        /* SGMII case */
        addr4 = HYPHY20G_SGMII_ENET_FEGE_MSTATX_REG_INT_4;
        addr5 = HYPHY20G_SGMII_ENET_FEGE_MSTATX_REG_INT_5;
    }

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr4, &regData4);
    if(result) return result;

    result = sysHyPhy20gRead(fileHndl, addr5, &regData5);
    if(result) return result;

    *statusBits = (regData5 << 16) + regData4;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr4, 0xFFFFFFFF);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, addr5, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gMstatxGetTxStatus */



/*******************************************************************************
**
**  utilHyPhy20gFcCbrGetStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the FC_CBR block Status.
**
**                  This function is used for FC and CBR applications only.
**
**  INPUTS:        *fileHndl    - device handle
**                 slice       - FC_CBR instance in:
**                               0  - HSCI 0
**                               1  - HSCI 1
**                               2  - LSCI 0
**                               3  - LSCI 1
**                               4  - LSCI 2
**                               ......
**                               17 - LSCI 15
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
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gFcCbrGetStatus(struct file *fileHndl, UINT4 slice,
                                UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, regData;

    /* argument checking */
    if (slice > 17) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice < 2){
        /* HSCI case */
        addr = HYPHY20G_HSCI_FC_CBR_REG_INT_VAL;
        addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    } else {
        /* LSCI case */
        addr = HYPHY20G_LSCI_FC_CBR_REG_INT_VAL;
        addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice-2);
    }

    /* retrieve status */
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;

    *statusBits = regData;

    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gFcCbrGetStatus */

/*******************************************************************************
**
**  utilHyPhy20gEthStatGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts one of the HSCI/LSCI/LSCI_SGMII
**                 MSTATX Ethernet Statistics 40-bits counters.
**
**                 MSTATX provides 65 ethernet statistics counters which are
**                 the same between HSCI and LSCI/LSCI_SGMII slices, with
**                 the exception of #11 RX_ALIGNMENT_ERROR counter, which is
**                 not applicable.
**
**                 This function is used for 10GE MAC Terminated mode and
**                 MAC Terminated (FE/GE, SGMII FE/GE) mode.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - MSTATX instance in:
**                                0  - HSCI 0
**                                1  - HSCI 1
**                                2  - LSCI 0
**                                3  - LSCI 1
**                                4  - LSCI 2
**                                ......
**                                17 - LSCI 15
**                                18 - LSCI_SGMII (mgmt port)
**                 ctrlMsk      - Mask of counter values related control bits
**                                to updated.  Set the corresponding bit
**                                to 1 to assert a control bit:
**                                bit 0 - set POST bit to b'1 to update all
**                                        TX or RX statistic counters
**                                bit 1 - set CLEAR bit to b'1 to update and 
**                                        clear all TX or RX statistic counters
**                                bit 2 - set LOAD bit to b'1, to load value
**                                        0xFFFFFFFFF0 to all TX or RX
**                                        statistic counters
**                                Only one bit may be set in this argument.
**                 counter      - Counter to be accessed:
**                                0  RX_FRAMES_OK
**                                1  RX_OCTETS_OK
**                                2  RX_FRAMES
**                                3  RX_OCTETS
**                                4  RX_UNICAST_FRAMES_OK
**                                5  RX_BROADCAST_FRAMES_OK
**                                6  RX_MULTICAST_FRAMES_OK
**                                7  RX_TAGGED_FRAMES_OK
**                                8  RX_MAC_PAUSE_CONTROL_FRAMES_OK
**                                9  RX_MAC_CONTROL_FRAMES_OK
**                                10 RX_FCS_ERROR
**                                11 RESERVED
**                                12 RX_SYMBOL_ERROR
**                                13 RX_LAYER1_ERROR
**                                14 RX_INRANGE_LENGTH_ERROR
**                                15 RX_LONG_LENGTH_ERROR
**                                16 RX_LONG_LENGTH_CRC_ERROR
**                                17 RX_SHORT_LENGTH_ERROR
**                                18 RX_SHORT_LENGTH_CRC_ERROR
**                                19 RX_FRAMES_64_OCTETS
**                                20 RX_FRAMES_65_TO_127_OCTETS
**                                21 RX_FRAMES_128_TO_255_OCTETS
**                                22 RX_FRAMES_256_TO_511_OCTETS
**                                23 RX_FRAMES_512_TO_1023_OCTETS
**                                24 RX_FRAMES_1024_TO_1518_OCTETS
**                                25 RX_FRAMES_1519_TO_MAXSIZE
**                                26 RX_STATION_ADDRESS_FILTERED
**                                27 RX_VARIABLE
**                                28 RX_FRAMES_1519_TO_1522_OCTETS
**                                29 RX_FRAMES_1523_TO_1548_OCTETS
**                                30 RX_FRAMES_1549_TO_2000_OCTETS
**                                31 RX_FRAMES_2001_TO_2500_OCTETS
**                                32 RX_FRAMES_2501_TO_MAXSIZE
**                                33 TX_FRAMES_OK
**                                34 TX_OCTETS_OK
**                                35 TX_OCTETS
**                                36 TX_TAGGED_FRAMES_OK
**                                37 TX_MAC_PAUSE_CONTROL_FRAMES_OK
**                                38 TX_FCS_ERROR
**                                39 TX_SHORT_LENGTH_ERROR
**                                40 TX_LONG_LENGTH_ERROR
**                                41 TX_SYSTEM_ERROR
**                                42 TX_MAC_ERROR
**                                43 TX_FRAMES_1523_TO_MAXSIZE
**                                   (TX_TAGGED_JUMBO_FRAMES_OK)
**                                44 TX_MAC_CONTROLFRAMES_OK
**                                45 TX_UNICAST_FRAMES_OK
**                                46 TX_BROADCAST_FRAMES_OK
**                                47 TX_MULTICAST_FRAMES_OK
**                                48 TX_UNICAST_FRAMES_ATTEMPTED
**                                49 TX_BROADCAST_FRAMES_ATTEMPTED
**                                50 TX_MULTICAST_FRAMES_ATTEMPTED
**                                51 TX_FRAMES_64_OCTETS
**                                52 TX_FRAMES_65_TO_127_OCTETS
**                                53 TX_FRAMES_128_TO_255_OCTETS
**                                54 TX_FRAMES_256_TO_511_OCTETS
**                                55 TX_FRAMES_512_TO_1023_OCTETS
**                                56 TX_FRAMES_1024_TO_1518_OCTETS
**                                57 TX_FRAMES_1519_TO_MAXSIZE
**                                58 TX_VARIABLE
**                                59 RX_FRAMES_1523_TO_MAXSIZE
**                                   (RX_TAGGED_JUMBO_FRAMES_OK)
**                                60 TX_FRAMES_1519_TO_1522_OCTETS
**                                61 TX_FRAMES_1523_TO_1548_OCTETS
**                                62 TX_FRAMES_1549_TO_2000_OCTETS
**                                63 TX_FRAMES_2001_TO_2500_OCTETS
**                                64 TX_FRAMES_2501_TO_MAXSIZE
**                *pCount0      - Pointer to storage for counter's bit 15:0
**                *pCount1      - Pointer to storage for counter's bit 31:16
**                *pCount2      - Pointer to storage for counter's bit 39:32
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gEthStatGetCounts(struct file *fileHndl, UINT4 slice, 
                                  UINT4 ctrlMsk, UINT4 counter, UINT4 *pCount0,
                                  UINT4 *pCount1, UINT4 *pCount2)
{
    UINT4 val;
    UINT4 ctrlReg   = 0;
    UINT4 clearMsk  = 0;
    UINT4 updateMsk = 0;
    UINT4 loadMsk   = 0;
    UINT4 cntLowReg = 0;
    UINT4 cntLowMsk = 0;
    UINT4 cntMidReg = 0;
    UINT4 cntMidMsk = 0;
    UINT4 cntHigReg = 0;
    UINT4 cntHigMsk = 0;
    INT4 result;

    /* argument checking */
    if (slice > 18 || counter > 64 || 
        (ctrlMsk != 0x0 && ctrlMsk != 0x1 && 
         ctrlMsk != 0x2 && ctrlMsk != 0x4)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (pCount0 == NULL || 
        pCount1 == NULL ||
        pCount2 == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_NULL_PTR_PASSED;
    }

    /* determine registers and masks associated with counter */
    if (slice < 2){
        /* HSCI case */
        ctrlReg   = HYPHY20G_HSCI_ENET_10G_MSTATX_REG_CTL;
        if (counter < 33) {
            /* RX */
            loadMsk   = HYPHY20G_HSCI_ENET_10G_MSTATX_BIT_CTL_RX_LOAD_MSK;
            clearMsk  = HYPHY20G_HSCI_ENET_10G_MSTATX_BIT_CTL_RX_SNAP_CLEAR_MSK;
            updateMsk = HYPHY20G_HSCI_ENET_10G_MSTATX_BIT_CTL_RX_SNAP_POST_MSK;
        } else {
            /* TX */
            loadMsk   = HYPHY20G_HSCI_ENET_10G_MSTATX_BIT_CTL_TX_LOAD_MSK;
            clearMsk  = HYPHY20G_HSCI_ENET_10G_MSTATX_BIT_CTL_TX_SNAP_CLEAR_MSK;
            updateMsk = HYPHY20G_HSCI_ENET_10G_MSTATX_BIT_CTL_TX_SNAP_POST_MSK;
        }
        ctrlReg   = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(ctrlReg, slice);
        cntLowReg = HYPHY20G_HSCI_ENET_10G_MSTATX_REG_STATS_CNT_LOW_OFFSET(counter);
        cntLowReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cntLowReg, slice);
        cntLowMsk = HYPHY20G_HSCI_ENET_10G_MSTATX_BIT_STATS_CNT_LOW_COUNTL_MSK;
        cntMidReg = HYPHY20G_HSCI_ENET_10G_MSTATX_REG_STATS_CNT_MID_OFFSET(counter);
        cntMidReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cntMidReg, slice);
        cntMidMsk = HYPHY20G_HSCI_ENET_10G_MSTATX_BIT_STATS_CNT_MID_COUNTM_MSK;
        cntHigReg = HYPHY20G_HSCI_ENET_10G_MSTATX_REG_STATS_CNT_HI_OFFSET(counter);
        cntHigReg = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cntHigReg, slice);
        cntHigMsk = HYPHY20G_HSCI_ENET_10G_MSTATX_BIT_STATS_CNT_HI_COUNTH_MSK;
    } else if (slice > 1 && slice < 18) {
        /* LSCI case */
        ctrlReg   = HYPHY20G_LSCI_ENET_FEGE_MSTATX_REG_CTL;
        if (counter < 33) {
            /* RX */
            loadMsk   = HYPHY20G_LSCI_ENET_FEGE_MSTATX_BIT_CTL_RX_LOAD_MSK;
            clearMsk  = HYPHY20G_LSCI_ENET_FEGE_MSTATX_BIT_CTL_RX_SNAP_CLEAR_MSK;
            updateMsk = HYPHY20G_LSCI_ENET_FEGE_MSTATX_BIT_CTL_RX_SNAP_POST_MSK;
        } else {
            /* TX */
            loadMsk   = HYPHY20G_LSCI_ENET_FEGE_MSTATX_BIT_CTL_TX_LOAD_MSK;
            clearMsk  = HYPHY20G_LSCI_ENET_FEGE_MSTATX_BIT_CTL_TX_SNAP_CLEAR_MSK;
            updateMsk = HYPHY20G_LSCI_ENET_FEGE_MSTATX_BIT_CTL_TX_SNAP_POST_MSK;
        }
        ctrlReg   = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(ctrlReg, slice-2);
        cntLowReg = HYPHY20G_LSCI_ENET_FEGE_MSTATX_REG_STATS_CNT_LOW_OFFSET(counter);
        cntLowReg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(cntLowReg, slice-2);
        cntLowMsk = HYPHY20G_LSCI_ENET_FEGE_MSTATX_BIT_STATS_CNT_LOW_COUNTL_MSK;
        cntMidReg = HYPHY20G_LSCI_ENET_FEGE_MSTATX_REG_STATS_CNT_MID_OFFSET(counter);
        cntMidReg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(cntMidReg, slice-2);
        cntMidMsk = HYPHY20G_LSCI_ENET_FEGE_MSTATX_BIT_STATS_CNT_MID_COUNTM_MSK;
        cntHigReg = HYPHY20G_LSCI_ENET_FEGE_MSTATX_REG_STATS_CNT_HI_OFFSET(counter);
        cntHigReg = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(cntHigReg, slice-2);
        cntHigMsk = HYPHY20G_LSCI_ENET_FEGE_MSTATX_BIT_STATS_CNT_HI_COUNTH_MSK;
    } else {
        /* SGMII case */
        ctrlReg   = HYPHY20G_SGMII_ENET_FEGE_MSTATX_REG_CTL;
        if (counter < 33) {
            /* RX */
            loadMsk   = HYPHY20G_SGMII_ENET_FEGE_MSTATX_BIT_CTL_RX_LOAD_MSK;
            clearMsk  = HYPHY20G_SGMII_ENET_FEGE_MSTATX_BIT_CTL_RX_SNAP_CLEAR_MSK;
            updateMsk = HYPHY20G_SGMII_ENET_FEGE_MSTATX_BIT_CTL_RX_SNAP_POST_MSK;
        } else {
            /* TX */
            loadMsk   = HYPHY20G_SGMII_ENET_FEGE_MSTATX_BIT_CTL_TX_LOAD_MSK;
            clearMsk  = HYPHY20G_SGMII_ENET_FEGE_MSTATX_BIT_CTL_TX_SNAP_CLEAR_MSK;
            updateMsk = HYPHY20G_SGMII_ENET_FEGE_MSTATX_BIT_CTL_TX_SNAP_POST_MSK;
        }
        cntLowReg = HYPHY20G_SGMII_ENET_FEGE_MSTATX_REG_STATS_CNT_LOW_OFFSET(counter);
        cntLowMsk = HYPHY20G_SGMII_ENET_FEGE_MSTATX_BIT_STATS_CNT_LOW_COUNTL_MSK;
        cntMidReg = HYPHY20G_SGMII_ENET_FEGE_MSTATX_REG_STATS_CNT_MID_OFFSET(counter);
        cntMidMsk = HYPHY20G_SGMII_ENET_FEGE_MSTATX_BIT_STATS_CNT_MID_COUNTM_MSK;
        cntHigReg = HYPHY20G_SGMII_ENET_FEGE_MSTATX_REG_STATS_CNT_HI_OFFSET(counter);
        cntHigMsk = HYPHY20G_SGMII_ENET_FEGE_MSTATX_BIT_STATS_CNT_HI_COUNTH_MSK;
    }

    /* configure the LOAD bit if specified */
    if(ctrlMsk & 0x4){
        result = hyPhy20gRegFieldWrite(fileHndl, ctrlReg, loadMsk, 1);
        if(result) return result;
    }

    /* Poll LOAD bit to ensure cleared for loading sucessfully */
    result = sysHyPhy20gPollBit(fileHndl, ctrlReg, loadMsk, 0, 10000);
    if (result) return result;

    /* configure the POST bit if specified */
    if(ctrlMsk & 0x1){
        result = hyPhy20gRegFieldWrite(fileHndl, ctrlReg, updateMsk, 1);
        if(result) return result;
    }
    
    /* Poll POST bit to ensure cleared for updating sucessfully */
    result = sysHyPhy20gPollBit(fileHndl, ctrlReg, updateMsk, 0, 10000);
    if (result) return result;
        
    /* configure the CLEAR bit if specified */
    if(ctrlMsk & 0x2){
        result = hyPhy20gRegFieldWrite(fileHndl, ctrlReg, clearMsk, 1);
        if(result) return result;
    }

    /* Poll CLEAR bit to ensure cleared for clearing sucessfully */
    result = sysHyPhy20gPollBit(fileHndl, ctrlReg, clearMsk, 0, 10000);
    if (result) return result;        

    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, cntLowReg, cntLowMsk, &val);
    if (result) return result;
    *pCount0 = val;

    result = hyPhy20gRegFieldRead(fileHndl, cntMidReg, cntMidMsk, &val);
    if (result) return result;
    *pCount1 = val;

    result = hyPhy20gRegFieldRead(fileHndl, cntHigReg, cntHigMsk, &val);
    if (result) return result;
    *pCount2 = val;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gEthStatGetCounts */


/*******************************************************************************
**
**  utilHyPhy20gEthStatVarCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function selects a user-defined RX Variable or
**                 TX Variable counters in the HSCI/LSCI/LSCI_SGMII
**                 MSTATX Ethernet Statistics 40-bits counters.
**
**                 The user-defined RX Variable is the number 27 counter
**                 of the Ethernet Statistics 40-bits counters.
**                 The user-defined TX Variable is the number 58 counter
**                 of the Ethernet Statistics 40-bits counters.
**
**                 This function is used for 10GE MAC Terminated mode and
**                 MAC Terminated (FE/GE, SGMII FE/GE) mode.
**
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - MSTATX instance in:
**                                0  - HSCI 0
**                                1  - HSCI 1
**                                2  - LSCI 0
**                                3  - LSCI 1
**                                4  - LSCI 2
**                                ......
**                                17 - LSCI 15
**                                18 - LSCI_SGMII (mgmt port)
**                 dir          - 0 - TX Variable counter
**                                1 - RX Variable counter
**                 index        - RX Variable counter Programmable options
**                                     TX                  RX
**                                0x0  Octet OK            Octet OK
**                                0x1  Octet Error         Octet Error
**                                0x2  Unicast             Unicast
**                                0x3  Multicast           Multicast
**                                0x4  Broadcast           Broadcast
**                                0x5  Tagged              Tagged
**                                0x6  Control             Control
**                                0x7  PAUSE               PAUSE
**                                0x8  FCS                 FCS
**                                0x9  Long                In Range
**                                0xA  Short               Long
**                                0xB  transmit MAC Error  Jabber
**                                0xC  System Error        Short
**                                0xD  Reserved            Runt
**                                0xE  Reserved            Alignment
**                                0xF  Reserved            Filtered
**                                0x10 Reserved            Symbol
**                                0x11 Reserved            Layer1 Error
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gEthStatVarCfg(struct file *fileHndl, UINT4 slice,
                               UINT4 dir, UINT4 index)
{
    UINT4 addr = 0, msk = 0;
    INT4 result;

    /* argument checking */
    if (slice > 18 || dir > 1 || index > 0x11 ||
        (dir == 0 && index > 0xC)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice < 2) {
        /* HSCI case */
        addr = HYPHY20G_HSCI_ENET_10G_MSTATX_REG_VAR_SEL;
        addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
        if (dir) {
            msk  = HYPHY20G_HSCI_ENET_10G_MSTATX_BIT_VAR_SEL_TX_INDEX_MSK;
        } else {
            msk  = HYPHY20G_HSCI_ENET_10G_MSTATX_BIT_VAR_SEL_RX_INDEX_MSK;
        }
    } else if (slice > 1 && slice < 18) {
        /* LSCI case */
        addr = HYPHY20G_LSCI_ENET_FEGE_MSTATX_REG_VAR_SEL;
        addr = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(addr, slice-2);
        if (dir) {
            msk  = HYPHY20G_LSCI_ENET_FEGE_MSTATX_BIT_VAR_SEL_TX_INDEX_MSK;
        } else {
            msk  = HYPHY20G_LSCI_ENET_FEGE_MSTATX_BIT_VAR_SEL_RX_INDEX_MSK;
        }
    } else {
        /* SGMII case */
        addr = HYPHY20G_SGMII_ENET_FEGE_MSTATX_REG_VAR_SEL;
        if (dir) {
            msk  = HYPHY20G_SGMII_ENET_FEGE_MSTATX_BIT_VAR_SEL_TX_INDEX_MSK;
        } else {
            msk  = HYPHY20G_SGMII_ENET_FEGE_MSTATX_BIT_VAR_SEL_RX_INDEX_MSK;
        }
    }

    /* select a user-defined RX Variable or TX Variable counters */
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, index);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* utilHyPhy20gEthStatVarCfg */

/*******************************************************************************
**
**  utilHyPhy20gFcCbrTxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts one of the FC_CBR block transmit
**                 counters.
**
**                 This function is used for FC 4G or lower rate, ESCON, FC 8G,
**                 Low Speed CBR (both byte-aligned and 8b10b encoded),
**                 High Speed CBR (both byte-aligned and 8b10b encoded) modes.
**
**                 The counter "07 - TX_LCV_CNT" is only applicable for the
**                 8b10b encoded traffics.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - FC_CBR instance in:
**                                0  - HSCI 0
**                                1  - HSCI 1
**                                2  - LSCI 0
**                                3  - LSCI 1
**                                4  - LSCI 2
**                                ......
**                                17 - LSCI 15
**                 update       - 1 - update of the counter values
**                              - 0 - do not update of the counter values
**                 counter      - Counter to be accessed:
**                                00 - TX_WRD_CNT
**                                01 - TX_BAD_CRC
**                                02 - TX_TOT_PKTS
**                                03 - FW_INS_CNT
**                                04 - PS_INS_CNT
**                                05 - FW_DEL_CNT
**                                06 - PS_DEL_CNT
**                                07 - TX_LCV_CNT
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gFcCbrTxGetCounts(struct file *fileHndl, UINT4 slice,
                                  UINT4 update, UINT4 counter, UINT4 *pCount)
{
    UINT4 updateReg;
    UINT4 updateMsk;
    UINT4 cntReg;
    UINT4 cntMsk;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 17 || update > 1 || counter > 7) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice < 2){
        /* HSCI case */
        switch(counter)
        {
            case 0: /* TX_WRD_CNT */
                cntReg = HYPHY20G_HSCI_FC_CBR_REG_TX_WD_CNT;
                cntMsk = HYPHY20G_HSCI_FC_CBR_BIT_TX_WD_CNT_TX_WRD_CNT_MSK;
                break;
            case 1: /* TX_BAD_CRC */
                cntReg = HYPHY20G_HSCI_FC_CBR_REG_TX_FRAMES_BAD_CRC;
                cntMsk = HYPHY20G_HSCI_FC_CBR_BIT_TX_FRAMES_BAD_CRC_TX_BAD_CRC_MSK;
                break;
            case 2: /* TX_TOT_PKTS */
                cntReg = HYPHY20G_HSCI_FC_CBR_REG_TX_TOTAL_PKT;
                cntMsk = HYPHY20G_HSCI_FC_CBR_BIT_TX_TOTAL_PKT_TX_TOT_PKTS_MSK;
                break;
            case 3: /* FW_INS_CNT */
                cntReg = HYPHY20G_HSCI_FC_CBR_REG_FILL_WD_INS_CNT;
                cntMsk = HYPHY20G_HSCI_FC_CBR_BIT_FILL_WD_INS_CNT_FW_INS_CNT_MSK;
                break;
            case 4: /* PS_INS_CNT */
                cntReg = HYPHY20G_HSCI_FC_CBR_REG_PRMT_SEQ_INS_CNT;
                cntMsk = HYPHY20G_HSCI_FC_CBR_BIT_PRMT_SEQ_INS_CNT_PS_INS_CNT_MSK;
                break;
            case 5: /* FW_DEL_CNT */
                cntReg = HYPHY20G_HSCI_FC_CBR_REG_FILL_WD_DEL_CNT;
                cntMsk = HYPHY20G_HSCI_FC_CBR_BIT_FILL_WD_DEL_CNT_FW_DEL_CNT_MSK;
                break;
            case 6: /* PS_DEL_CNT */
                cntReg = HYPHY20G_HSCI_FC_CBR_REG_PRMT_SEQ_DEL_CNT;
                cntMsk = HYPHY20G_HSCI_FC_CBR_BIT_PRMT_SEQ_DEL_CNT_PS_DEL_CNT_MSK;
                break;
            case 7: /* TX_LCV_CNT */
                cntReg = HYPHY20G_HSCI_FC_CBR_REG_LCV_CNT;
                cntMsk = HYPHY20G_HSCI_FC_CBR_BIT_LCV_CNT_TX_LCV_CNT_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        cntReg      = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cntReg, slice);
        updateReg   = HYPHY20G_HSCI_FC_CBR_REG_CNT_UDA;
        updateReg   = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(updateReg, slice);
        updateMsk   = HYPHY20G_HSCI_FC_CBR_BIT_CNT_UDA_CNTR_UPDATE_MSK;
    } else {
        /* LSCI case */
        switch(counter)
        {
            case 0: /* TX_WRD_CNT */
                cntReg = HYPHY20G_LSCI_FC_CBR_REG_TX_WD_CNT;
                cntMsk = HYPHY20G_LSCI_FC_CBR_BIT_TX_WD_CNT_TX_WRD_CNT_MSK;
                break;
            case 1: /* TX_BAD_CRC */
                cntReg = HYPHY20G_LSCI_FC_CBR_REG_TX_FRAMES_BAD_CRC;
                cntMsk = HYPHY20G_LSCI_FC_CBR_BIT_TX_FRAMES_BAD_CRC_TX_BAD_CRC_MSK;
                break;
            case 2: /* TX_TOT_PKTS */
                cntReg = HYPHY20G_LSCI_FC_CBR_REG_TX_TOTAL_PKT;
                cntMsk = HYPHY20G_LSCI_FC_CBR_BIT_TX_TOTAL_PKT_TX_TOT_PKTS_MSK;
                break;
            case 3: /* FW_INS_CNT */
                cntReg = HYPHY20G_LSCI_FC_CBR_REG_FILL_WD_INS_CNT;
                cntMsk = HYPHY20G_LSCI_FC_CBR_BIT_FILL_WD_INS_CNT_FW_INS_CNT_MSK;
                break;
            case 4: /* PS_INS_CNT */
                cntReg = HYPHY20G_LSCI_FC_CBR_REG_PRMT_SEQ_INS_CNT;
                cntMsk = HYPHY20G_LSCI_FC_CBR_BIT_PRMT_SEQ_INS_CNT_PS_INS_CNT_MSK;
                break;
            case 5: /* FW_DEL_CNT */
                cntReg = HYPHY20G_LSCI_FC_CBR_REG_FILL_WD_DEL_CNT;
                cntMsk = HYPHY20G_LSCI_FC_CBR_BIT_FILL_WD_DEL_CNT_FW_DEL_CNT_MSK;
                break;
            case 6: /* PS_DEL_CNT */
                cntReg = HYPHY20G_LSCI_FC_CBR_REG_PRMT_SEQ_DEL_CNT;
                cntMsk = HYPHY20G_LSCI_FC_CBR_BIT_PRMT_SEQ_DEL_CNT_PS_DEL_CNT_MSK;
                break;
            case 7: /* TX_LCV_CNT */
                cntReg = HYPHY20G_LSCI_FC_CBR_REG_LCV_CNT;
                cntMsk = HYPHY20G_LSCI_FC_CBR_BIT_LCV_CNT_TX_LCV_CNT_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        cntReg      = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(cntReg, slice-2);
        updateReg   = HYPHY20G_LSCI_FC_CBR_REG_CNT_UDA;
        updateReg   = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(updateReg, slice-2);
        updateMsk   = HYPHY20G_LSCI_FC_CBR_BIT_CNT_UDA_CNTR_UPDATE_MSK;
    }

    /* update of the counter values if required */
    if (update) {
        result = hyPhy20gRegFieldWrite(fileHndl, updateReg, updateMsk, 1);
        if (result) return result;
    }

    /* Poll CNTR_UPDATE bit to ensure cleared for updating sucessfully */
    result = sysHyPhy20gPollBit(fileHndl, updateReg, updateMsk, 0, 10000);
    if (result) return result;

    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, cntReg, cntMsk, &val);
    if (result) return result;
    *pCount = val;

    sysHyPhy20gUsDelay(5);
    
    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gFcCbrTxGetCounts */


/*******************************************************************************
**
**  utilHyPhy20gFcCbrRxGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts one of the FC_CBR block receive
**                 counters.
**
**                 This function is used for FC 4G or lower rate, ESCON, FC 8G,
**                 Low Speed CBR (both byte-aligned and 8b10b encoded),
**                 High Speed CBR (both byte-aligned and 8b10b encoded) modes.
**
**                 The counter "03 - RX_LCV_CNT" is only applicable for the
**                 8b10b encoded traffics.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - FC_CBR instance in:
**                                0  - HSCI 0
**                                1  - HSCI 1
**                                2  - LSCI 0
**                                3  - LSCI 1
**                                4  - LSCI 2
**                                ......
**                                17 - LSCI 15
**                 update       - 1 - update of the counter values
**                              - 0 - do not update of the counter values
**                 counter      - Counter to be accessed:
**                                00 - RX_WRD_CNT
**                                01 - RX_BAD_CRC
**                                02 - RX_TOT_PKTS
**                                03 - RX_LCV_CNT
**
**                *pCount       - Pointer to storage for counter data
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 utilHyPhy20gFcCbrRxGetCounts(struct file *fileHndl, UINT4 slice,
                                  UINT4 update, UINT4 counter, UINT4 *pCount)
{
    UINT4 updateReg;
    UINT4 updateMsk;
    UINT4 cntReg;
    UINT4 cntMsk;
    UINT4 val;
    INT4 result;

    /* argument checking */
    if (slice > 17 || update > 1 || counter > 3) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* determine registers and masks associated with counter */
    if (slice < 2){
        /* HSCI case */
        switch(counter)
        {
            case 0: /* RX_WRD_CNT */
                cntReg = HYPHY20G_HSCI_FC_CBR_REG_RX_WD_CNT;
                cntMsk = HYPHY20G_HSCI_FC_CBR_BIT_RX_WD_CNT_RX_WRD_CNT_MSK;
                break;
            case 1: /* RX_BAD_CRC */
                cntReg = HYPHY20G_HSCI_FC_CBR_REG_RX_FRAMES_BAD_CRC;
                cntMsk = HYPHY20G_HSCI_FC_CBR_BIT_RX_FRAMES_BAD_CRC_RX_BAD_CRC_MSK;
                break;
            case 2: /* RX_TOT_PKTS */
                cntReg = HYPHY20G_HSCI_FC_CBR_REG_RX_TOTAL_PKT;
                cntMsk = HYPHY20G_HSCI_FC_CBR_BIT_RX_TOTAL_PKT_RX_TOT_PKTS_MSK;
                break;
            case 3: /* RX_LCV_CNT */
                cntReg = HYPHY20G_HSCI_FC_CBR_REG_LCV_CNT;
                cntMsk = HYPHY20G_HSCI_FC_CBR_BIT_LCV_CNT_RX_LCV_CNT_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        cntReg      = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(cntReg, slice);
        updateReg   = HYPHY20G_HSCI_FC_CBR_REG_CNT_UDA;
        updateReg   = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(updateReg, slice);
        updateMsk   = HYPHY20G_HSCI_FC_CBR_BIT_CNT_UDA_CNTR_UPDATE_MSK;
    } else {
        /* LSCI case */
        switch(counter)
        {
            case 0: /* RX_WRD_CNT */
                cntReg = HYPHY20G_LSCI_FC_CBR_REG_RX_WD_CNT;
                cntMsk = HYPHY20G_LSCI_FC_CBR_BIT_RX_WD_CNT_RX_WRD_CNT_MSK;
                break;
            case 1: /* RX_BAD_CRC */
                cntReg = HYPHY20G_LSCI_FC_CBR_REG_RX_FRAMES_BAD_CRC;
                cntMsk = HYPHY20G_LSCI_FC_CBR_BIT_RX_FRAMES_BAD_CRC_RX_BAD_CRC_MSK;
                break;
            case 2: /* RX_TOT_PKTS */
                cntReg = HYPHY20G_LSCI_FC_CBR_REG_RX_TOTAL_PKT;
                cntMsk = HYPHY20G_LSCI_FC_CBR_BIT_RX_TOTAL_PKT_RX_TOT_PKTS_MSK;
                break;
            case 3: /* RX_LCV_CNT */
                cntReg = HYPHY20G_LSCI_FC_CBR_REG_LCV_CNT;
                cntMsk = HYPHY20G_LSCI_FC_CBR_BIT_LCV_CNT_RX_LCV_CNT_MSK;
                break;
            default:
                return HYPHY20G_ERR_INVALID_ARG;
        }
        cntReg      = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(cntReg, slice-2);
        updateReg   = HYPHY20G_LSCI_FC_CBR_REG_CNT_UDA;
        updateReg   = HYPHY20G_LSCI_MTSB_SLICE_REG_OFFSET(updateReg, slice-2);
        updateMsk   = HYPHY20G_LSCI_FC_CBR_BIT_CNT_UDA_CNTR_UPDATE_MSK;
    }

    /* update of the counter values if required */
    if (update) {
        result = hyPhy20gRegFieldWrite(fileHndl, updateReg, updateMsk, 1);
        if (result) return result;
    }

    /* Poll CNTR_UPDATE bit to ensure cleared for updating sucessfully */
    result = sysHyPhy20gPollBit(fileHndl, updateReg, updateMsk, 0, 10000);
    if (result) return result;

    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, cntReg, cntMsk, &val);
    if (result) return result;
    *pCount = val;

    sysHyPhy20gUsDelay(5);
    
    return HYPHY20G_SUCCESS;

} /* utilHyPhy20gFcCbrRxGetCounts */


/* end of file */
