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
**  FILE        : hyphy20g_mgmt_status.c
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
#include "hyphy20g_mgmt_status.h"

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/


/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/
/*******************************************************************************
**
**  hyPhy20gMgmtPortTopLevelGetStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the SGMII_LSCI Top Level Status.
**
**                  This function is used for all LSCI applications.
**                  The bit 1 and 2 are only applicable for MAC Terminated
**                  (FE/GE, SGMII FE/GE) modes when PTP feature in use.
**
**  INPUTS:        *fileHndl    - device handle
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31:3 - RESERVED
**                                  Bit    2 - TX_TSTMPR_TOD_SIGN_OVRFL_V
**                                  Bit    1 - RX_TSTMPR_TOD_SIGN_OVRFL_V
**                                  Bit    0 - LOS_INT_V
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortTopLevelGetStatus(struct file *fileHndl, UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, msk;
    UINT4 losStat = 0;
    UINT4 ovrflStat = 0;

    /* retrieve status */
    addr = HYPHY20G_SGMII_TOP_REG_LOS_INT_STAT;
    msk  = HYPHY20G_SGMII_TOP_BIT_LOS_INT_STAT_LOS_INT_V_MSK;

    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &losStat);
    if (result) return result;

    addr = HYPHY20G_SGMII_ENET_FEGE_TOP_REG_INT_VAL;

    result = sysHyPhy20gRead(fileHndl, addr, &ovrflStat);
    if (result) return result;

    *statusBits = ((ovrflStat & 0x60) >> 4) + losStat;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortTopLevelGetStatus */



/*******************************************************************************
**
**  hyPhy20gMgmtPortFegeGetStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the SGMII_LSCI ENET_FEGE Status.
**
**                  This function is used for MAC Terminated (FE/GE,
**                  SGMII FE/GE) application.
**
**                  It is recommended to have the device level WCI_MODE bit
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts within the registers accessed by
**                  this function.
**
**  INPUTS:        *fileHndl    - device handle
**                  intClear    - 1 - all *INT_I interrupt bits accessed by this
**                               function will be cleared by writing them to '1'
**                              - 0 - all *INT_I interrupt bits accessed by this
**                               function will retain their values.
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31:7 - RESERVED
**                                  Bit    6 - TX_TSTMPR_TOD_SIGN_OVRFL_I
**                                  Bit    5 - RX_TSTMPR_TOD_SIGN_OVRFL_I
**                                  Bit    4 - OVERFLOW_I
**                                  Bit    3 - RX_COMMA_DEL_CHG_I
**                                  Bit    2 - TX_TSTMP_FIFO_I
**                                  Bit    1 - TX_SOF_I
**                                  Bit    0 - RX_SOF_I
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortFegeGetStatus(struct file *fileHndl, UINT4 intClear,
                                   UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, regData;

    /* argument checking */
    if (intClear > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve status */
    addr = HYPHY20G_SGMII_ENET_FEGE_TOP_REG_INT;

    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if(result) return result;
    *statusBits = regData;

    /* clear status bits if requested */
    if(intClear == 1) {
        result = sysHyPhy20gWrite(fileHndl, addr, 0xFFFFFFFF);
        if (result) return result;
    }

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortFegeGetStatus */


/*******************************************************************************
**
**  hyPhy20gMgmtPortL1rppGetStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the SGMII_LSCI L1RPP block Status.
**
**                  This function is used for MAC Terminated (FE/GE,
**                  SGMII FE/GE) application.
**
**                  The bit 7 AUTO_NEG_LP_ABLTY_UPD_V is a new status bit in 
**                  Rev B device, and it is only available when MANUAL_ACK_EN
**                  bit set (mode '6' and '7' of hyPhy20gMgmPortFeGeModeCfg).
**
**  INPUTS:        *fileHndl    - device handle
**                 *statusBits  - Pointer to storage for status bits
**                                  Bit 31:24- RESERVED
**                                  Bit 23:20- BIT_ALIGN[3:0]
**                                  Bit 19   - L1RPP_STATUS
**                                  Bit 18   - L1RPP_STATUS
**                                  Bit 17   - SDET
**                                  Bit 16   - ENCDET
**                                  Bit 15   - LOS_V
**                                  Bit 14   - SDET_IN_V
**                                  Bit 13   - ALIGN_CHANGE_V
**                                  Bit 12   - LINK_MON_V
**                                  Bit 11   - FEF_STAT_V
**                                  Bit 10   - CARRIER_STAT_V
**                                  Bit 7    - AUTO_NEG_LP_ABLTY_UPD_V
**                                  Bit 4    - LINK_OK_V
**                                  Bit 0    - SYNC_OK_V
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortL1rppGetStatus(struct file *fileHndl, UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr;
    UINT4 intStat = 0;
    UINT4 l1rppStat = 0;
    UINT4 bitAlignStat = 0;

    /* retrieve status */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_INT_STAT;
    result = sysHyPhy20gRead(fileHndl, addr, &intStat);
    if(result) return result;

    /* retrieve status */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_CFG_AND_STAT;
    result = sysHyPhy20gRead(fileHndl, addr, &l1rppStat);
    if(result) return result;

    /* Read BIT ALIGNMENTstatus */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_BIT_ALGN;
    result = sysHyPhy20gRead(fileHndl, addr, &bitAlignStat);
    if(result) return result;

    *statusBits = ((bitAlignStat & 0xF) << 20) +
                  ((l1rppStat & 0xF000) << 4) + (intStat & 0xFFFF);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortL1rppGetStatus */


/*******************************************************************************
**
**  hyPhy20gMgmtPortL1rppGetMiiANStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the SGMII_LSCI L1RPP MII
**                  AUTO-NEGOTIATION Status.
**
**                  This function is used for MAC Terminated (FE/GE,
**                  SGMII FE/GE) application.
**
**  INPUTS:        *fileHndl    - device handle
**                 *statusBits0 - Pointer to storage for status bits
**                                  Bit 31-19: RESERVED
**                                  Bit 18   : RX_PAGE
**                                  Bit 17   : RESOLVE_PRIORITY
**                                  Bit 16   : LNK_NEXT_PAGE
**                                  Bit 15   : LNK_ACK
**                                  Bit 14-13: LNK_REM_FAULT[1:0]
**                                  Bit 9    : LNK_ASM_DIR
**                                  Bit 8    : LNK_PAUSE
**                                  Bit 7    : LNK_HALF_DUPLEX
**                                  Bit 6    : LNK_FULL_DUPLEX
**                                  Bit 5    : AN_COMPLETE
**                                  Bit 4    : REM_FLT_IND
**                                  Bit 3    : UNUSED
**                                  Bit 2    : LNK_STATE
**                                  Bit 1    : UNUSED
**                                  Bit 0    : UNUSED
**                 *statusBits1 - Pointer to storage for status bits
**                                  Bit 31    : LNK_NP_NEXT_PAGE
**                                  Bit 20    : LNK_NP_ACK
**                                  Bit 29    : LNK_NP_MSG_PAGE
**                                  Bit 28    : LNK_NP_ACK2
**                                  Bit 27    : LNK_NP_TOGGLE
**                                  Bit 26-16 : LNK_NP_CODE_FIELD[10:0]
**                                  Bit 15    : LOC_NP_NEXT_PAGE
**                                  Bit 13    : LOC_NP_MSG_PAGE
**                                  Bit 12    : LOC_NP_ACK2
**                                  Bit 11    : LOC_NP_TOGGLE
**                                  Bit 10-0  : LOC_NP_CODE_FIELD[10:0]
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortL1rppGetMiiANStatus(struct file *fileHndl, 
                                         UINT4 *statusBits0, UINT4 *statusBits1)
{
    INT4 result;
    UINT4 addr;
    UINT4 miiStat    = 0; /* MII STATUS */
    UINT4 anLinkStat = 0; /* MII AUTO-NEGOTIATION LINK
                             PARTNER BASE PAGE ABILITY */
    UINT4 anExpStat  = 0; /* MII AUTO-NEGOTIATION EXPANSION */
    UINT4 anTxNpStat = 0; /* MII AUTO-NEGOTIATION NEXT PAGE TRANSMIT */
    UINT4 anRxNpStat = 0; /* MII AUTO-NEGOTIATION LINK PARTNER RCVD NEXT PAGE */

    /* retrieve status */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_MII_STAT;
    result = sysHyPhy20gRead(fileHndl, addr, &miiStat);
    if(result) return result;

    /* retrieve status */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_MII_AUTO_NEG_LNK_PRT_BAS_PG_ABL;
    result = sysHyPhy20gRead(fileHndl, addr, &anLinkStat);
    if(result) return result;

    /* retrieve status */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_MII_AUTO_NEG_EXP;
    result = sysHyPhy20gRead(fileHndl, addr, &anExpStat);
    if(result) return result;

    *statusBits0 = ((anExpStat & 0x3) << 17) +
                   ((anLinkStat & 0xFFFF) << 6) + (miiStat & 0x3F);

    /* retrieve status */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_MII_AUTO_NEG_NXT_PG_TX;
    result = sysHyPhy20gRead(fileHndl, addr, &anTxNpStat);
    if(result) return result;

    /* retrieve status */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1RPP_REG_MII_AUTO_NEG_LNK_PRT_RCVD_NXT_PG;
    result = sysHyPhy20gRead(fileHndl, addr, &anRxNpStat);
    if(result) return result;

    *statusBits1 = ((anRxNpStat & 0xFFFF) << 16) + (anTxNpStat & 0xFFFF);

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortL1rppGetMiiANStatus */


/*******************************************************************************
**
**  hyPhy20gMgmtPortFeGeMacGetStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the SGMII_LSCI ENET_FEGE 
**                  sub-blocks' Status, including RMAC, TMAC and L1TPP.
**
**                  This function is used for MAC Terminated
**                  (FE/GE, SGMII FE/GE) application only.
**
**  INPUTS:        *fileHndl   - device handle
**                 *statusBits - Pointer to storage for status bits
**                                  Bit 31-5 : RESERVED
**                                  Bit 4    : INVLD_FRAME
**                                  Bit 3    : L1TPP_STATUS
**                                  Bit 2    : TMAC_STATUS
**                                  Bit 1    : GIGA_STATUS
**                                  Bit 0    : RMAC_STATUS
**
**  OUTPUTS:        None        - None
**
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gMgmtPortFeGeMacGetStatus(struct file *fileHndl, UINT4 *statusBits)
{
    INT4 result;
    UINT4 addr, msk;
    UINT4 rmacStat = 0;
    UINT4 gigaStat = 0;
    UINT4 tmacStat = 0;
    UINT4 l1tppStat = 0;
    UINT4 invalidFrmStat = 0;

    /* retrieve status */
    addr = HYPHY20G_SGMII_ENET_FEGE_RMAC_REG_CFG_1;
    msk  = HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_CFG_1_RMAC_STATUS_MSK;

    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &rmacStat);
    if (result) return result;

    /* retrieve status */
    addr = HYPHY20G_SGMII_ENET_FEGE_RMAC_REG_LNK_AND_STATS_CFG;
    msk  = HYPHY20G_SGMII_ENET_FEGE_RMAC_BIT_LNK_AND_STATS_CFG_GIGA_STATUS_MSK;

    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &gigaStat);
    if (result) return result;

    /* retrieve TMAC status */
    addr = HYPHY20G_SGMII_ENET_FEGE_RMAC_REG_LNK_AND_STATS_CFG;
    msk  = HYPHY20G_SGMII_ENET_FEGE_TMAC_BIT_CFG_1_TMAC_STATUS_MSK;

    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &tmacStat);
    if (result) return result;

    /* retrieve L1TPP status */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1TPP_REG_CFG;
    msk  = HYPHY20G_SGMII_ENET_FEGE_L1TPP_BIT_CFG_L1TPP_STATUS_MSK;

    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &l1tppStat);
    if (result) return result;

    /* retrieve Invalid frame status */
    addr = HYPHY20G_SGMII_ENET_FEGE_L1TPP_REG_CTL;
    msk  = HYPHY20G_SGMII_ENET_FEGE_L1TPP_BIT_CTL_INVLD_FRAME_MSK;

    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &invalidFrmStat);
    if (result) return result;

    *statusBits = (invalidFrmStat << 4) + (l1tppStat << 3) +
                  (tmacStat << 2) + (gigaStat << 1) + rmacStat;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortFeGeMacGetStatus */


/*******************************************************************************
**
**  hyPhy20gMgmtPortMstatxGetRxStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the MSTATX (in SGMII_LSCI) Rx Status.
**
**                  It is recommended to have the device level WCI_MODE bit
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts within the registers accessed by
**                  this function.
**
**                  This function is used for MAC Terminated
**                  (FE/GE, SGMII FE/GE) application only.
**
**  INPUTS:        *fileHndl    - device handle
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
INT4 hyPhy20gMgmtPortMstatxGetRxStatus(struct file *fileHndl, UINT4 intClear, 
                                       UINT4 *statusBits0, UINT4 *statusBits1)
{
    INT4 result;

    /* argument checking */
    if (intClear > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = utilHyPhy20gMstatxGetRxStatus(fileHndl, 18, intClear,
                                           statusBits0, statusBits1);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortMstatxGetRxStatus */

/*******************************************************************************
**
**  hyPhy20gMgmtPortMstatxGetTxStatus
**  ___________________________________________________________________________
**
**
**  DESCRIPTION:    This function checks the MSTATX (in SGMII_LSCI) Tx Status.
**
**                  It is recommended to have the device level WCI_MODE bit
**                  set to '1' if using this function to prevent unintentional
**                  clearing of interrupts within the registers accessed by
**                  this function.
**
**                  This function is used for MAC Terminated
**                  (FE/GE, SGMII FE/GE) application only.
**
**  INPUTS:        *fileHndl    - device handle
**                 slice        - MSTATX instance in:
**                                0  - LSCI 0
**                                1  - LSCI 1
**                                2  - LSCI 2
**                                ......
**                                15 - LSCI 15
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
INT4 hyPhy20gMgmtPortMstatxGetTxStatus(struct file *fileHndl, 
                                       UINT4 intClear, UINT4 *statusBits)
{
    INT4 result;

    /* argument checking */
    if (intClear > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    result = utilHyPhy20gMstatxGetTxStatus(fileHndl, 18, intClear,
                                           statusBits);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gMgmtPortMstatxGetTxStatus */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/* end of file */
