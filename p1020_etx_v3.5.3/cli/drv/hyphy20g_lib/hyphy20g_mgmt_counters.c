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
**  MODULE      : HyPHY 20G Configuration Code
**
**  FILE        :  hyphy20g_mgmt_counters.c
**
**  $Date: 2010-07-17 05:36:03 +0800 (星期六, 17 七月 2010) $
**
**  $Revision: 8768 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_mgmt_counters.h"

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/


/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gMgmtPortFeGeL1rppGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts the 40-bit line-code violation counter 
**                 for one of the L1RPP block of the MGMT port.
**
**                 This function is used for MAC Terminated (FE/GE, SGMII FE/GE)
**                 modes.
**
**  INPUTS:       *fileHndl     - base address
**                 accessMode   - 1 - locally trigger counter accumulation
**                                0 - do not locally trigger counter
**                                    accumulation
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
INT4 hyPhy20gMgmtPortFeGeL1rppGetCounts(struct file *fileHndl, UINT4 accessMode, 
                                        UINT4 *pCount0, UINT4 *pCount1, 
                                        UINT4 *pCount2)
{
    UINT4 updateReg;
    UINT4 updateMsk;
    UINT4 cntLowReg;
    UINT4 cntLowMsk;
    UINT4 cntMidReg;
    UINT4 cntMidMsk;
    UINT4 cntHigReg;
    UINT4 cntHigMsk;
    INT4 result;

    /* argument checking */
    if (accessMode > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }
    
    if (pCount0 == NULL || pCount1 == NULL || pCount2 == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_NULL_PTR_PASSED;
    }
    
    /* determine registers and masks associated with counter */
    cntLowReg = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_PMON_CNT_0;
    cntLowMsk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_PMON_CNT_0_PMON_CNTR_MSK;
    cntMidReg = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_PMON_CNT_1;
    cntMidMsk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_PMON_CNT_1_PMON_CNTR_MSK;
    cntHigReg = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_PMON_CNT_2;
    cntHigMsk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_PMON_CNT_2_PMON_CNTR_MSK;

    updateReg = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_PMON_UDA;
    updateMsk = HYPHY20G_SGMII_ENET_FEGE_L1RPP_BIT_PMON_UDA_PMON_UPDATE_MSK;

    /* update of the counter values if required */
    if (accessMode) {
        result = hyPhy20gRegFieldWrite(fileHndl, updateReg, updateMsk, 1);
        if (result) return result;
    }

    /* Poll PMON_UPDATE bit to ensure cleared for updating sucessfully */
    result = sysHyPhy20gPollBit(fileHndl, updateReg, updateMsk, 0, 10000);
    if (result) return result;

    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, cntLowReg, cntLowMsk, pCount0);
    if (result) return result;

    result = hyPhy20gRegFieldRead(fileHndl, cntMidReg, cntMidMsk, pCount1);
    if (result) return result;

    result = hyPhy20gRegFieldRead(fileHndl, cntHigReg, cntHigMsk, pCount2);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gMgmtPortFeGeL1rppGetCounts */


/*******************************************************************************
**
**  hyPhy20gMgmtPortEthStatGetCounts
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function extracts one of the MGMT port
**                 MSTATX Ethernet Statistics 40-bits counters.
**
**                 Refer to hyPhy20gMgmtPortEthStatTrigCounts for details of 
**                 operation.
**
**                 MSTATX provides 65 ethernet statistics counters which are
**                 the same between HSCI and LSCI/LSCI_SGMII slices, with
**                 the exception of #11 RX_ALIGNMENT_ERROR counter, which is
**                 not applicable.
**
**                 This function is used for MAC Terminated (FE/GE, SGMII FE/GE)
**                 modes.
**
**  INPUTS:       *fileHndl     - base address
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
**                 accessMd     - MSTATX counters' access mode:
**                                0  - counters remain in unchanged value
**                                1  - counters update to accumulated value
**                                2  - counters update to cleared value
**                                Note: 'accessMd' affects all Tx or Rx 
**                                counters at the same time (Tx or Rx depends
**                                on the 'counter' parameter).
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
INT4 hyPhy20gMgmtPortEthStatGetCounts(struct file *fileHndl, UINT4 counter, 
                                      UINT4 accessMd, UINT4 *pCount0,
                                      UINT4 *pCount1, UINT4 *pCount2)
{
    INT4 result;
    UINT4 ctrlMsk;

    /* argument checking */
    if (counter > 64 || accessMd > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    switch (accessMd) {
        case 0:
            ctrlMsk = 0;
            break;
        case 1:
            ctrlMsk = 0x1;
            break;
        case 2:
            ctrlMsk = 0x2;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }    
    
    /* Extract Counter */
    result = utilHyPhy20gEthStatGetCounts(fileHndl, 18, ctrlMsk, counter,
                                          pCount0, pCount1, pCount2);
    if (result) return result;
        
    return HYPHY20G_SUCCESS;
} /* hyPhy20gMgmtPortEthStatGetCounts */


/*******************************************************************************
**
**  hyPhy20gMgmtPortEthStatVarCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function selects a user-defined RX Variable or
**                 TX Variable counters in the MGMT port
**                 MSTATX Ethernet Statistics 40-bits counters.
**
**                 The user-defined RX Variable is the number 27 counter
**                 of the Ethernet Statistics 40-bits counters.
**                 The user-defined TX Variable is the number 58 counter
**                 of the Ethernet Statistics 40-bits counters.
**
**                 This function is used for
**                 MAC Terminated (FE/GE, SGMII FE/GE) modes.
**
**
**  INPUTS:       *fileHndl     - base address
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
INT4 hyPhy20gMgmtPortEthStatVarCfg(struct file *fileHndl, UINT4 dir, 
                                   UINT4 index)
{
    INT4 result;

    /* argument checking */
    if (dir > 1 || index > 0x11 ||
        (dir == 0 && index > 0xC)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = utilHyPhy20gEthStatVarCfg(fileHndl, 18, dir, index);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gMgmtPortEthStatVarCfg */




/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
