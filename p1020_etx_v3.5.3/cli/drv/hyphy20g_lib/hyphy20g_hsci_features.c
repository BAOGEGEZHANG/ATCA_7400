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
**  FILE        : hyphy20g_hsci_features.c
**
**  $Date: 2012-12-03 13:59:42 +0800 (Mon, 03 Dec 2012) $
**
**  $Revision: 29406 $
**
**  DESCRIPTION :
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_hsci_features.h"

/** external functions **/
extern INT4 hsciHyPhy20gHsciFcCbrUsrPattCfg(struct file *fileHndl, UINT4 slice,
                                     UINT4 dir, UINT4 olSeqHi, UINT4 olSeqLow,
                                     UINT4 pattDisp);

/** external data **/

/** public data **/

/** private data **/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                */
/******************************************************************************/
                                     
/******************************************************************************/
/* PUBLIC FUNCTIONS                                                           */
/******************************************************************************/

/*******************************************************************************
**
**  hyPhy20gHsciGsup43C73PrbsEnable
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures PRBS31 pattern insertion feature
**                  in the HSCI Tx GSUP43 block.
**
**                  Used for G.Sup43 Clause 7.3 Terminating application 
**                  bypass MAC operating mode (mode '2' in hyPhy20gHsciInit).
**
**                  This function can be called after hyPhy20gHsciInit and
**                  and prior or after hyPhy20gHsciDataPathEnable.
**
**  INPUTS:         fileHndl   - device handle
**                  slice      - HSCI slice processing resource
**                               0 - HSCI slice 0
**                               1 - HSCI slice 1
**                  enable     - enable/disable PRBS31 sequence to replace
**                               the packet data on the transmit data stream
**                               0 - transmit normal packet data
**                               1 - transmit PRBS31 sequence
**                               Default setting 0
**
**  OUTPUTS:        None       - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciGsup43C73PrbsEnable(struct file *fileHndl, UINT4 slice,
                                     UINT4 enable)
{                                
    INT4 result;
    UINT4 addr, val;

    /* argument checking */
    if (slice > 1 || enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_GSUP43_REG_TX_CFG_2, slice);
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    val = mHYPHY20G_SET_FIELD4(val, enable,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_PRBS31_EN_MSK,
            HYPHY20G_HSCI_ENET_10G_GSUP43_BIT_TX_CFG_2_PRBS31_EN_OFF);

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciGsup43C73PrbsEnable */


/*******************************************************************************
**
**  hyPhy20gHsciPattForce
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function allows for 40-bit or 32-bit user-configured
**                pattern to overwrite data in the transmit or receive
**                stream for a HSCI slice operating in FC, ESCON or CBR mode.
**
**                When pattern forcing is disabled, in the receive direction,
**                user-configured pattern will be generated in response to
**                DLOLB events. In the transmit direction, user-configured
**                pattern will be generated if no data is being forwarded
**                from the CPB.
**
**                Note, changing the RX_NOS_PN11_SEL and
**                TX_NOS_PN11_SEL bits require RX_SOFT_RST or
**                TX_SOFT_RST assertion, which will disrupt the data stream.
**                So the RX_NOS_PN11_SEL and TX_NOS_PN11_SEL bits keep in b'0,
**                and should never be changed.
**                To insert NOS, the user-configured pattern
**                is used. Used for FC and high-speed CBR modes.
**
**                For non 8b10b CBR clients this will generate the pattern as 
**                configured with hyPhy20gHsciByteAlignedCbrUsrPattCfg.   This
**                provide the ability to generate PN-11 or a user 32-bit value.
**                This API will toggle the RX_NOS_PN11_SEL and TX_NOS_PN11_SEL 
**                bits with the respective *_SOFT_RST bits.  In this case a 
**                momentary hit to the data stream may occur.
**
**                This function can optionally be called after
**                hyPhy20gHsciDataPathEnable.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - HSCI slice processing resource
**                              0 - HSCI slice 0
**                              1 - HSCI slice 1
**                 dir          - data flow direction
**                              0 - transmit data flow
**                                  from the CPB to the SFP
**                              1 - receive data flow
**                                  from the SFP to the CPB
**                 mode         - data stream overwriting operation
**                              0 - do not overwrite data stream
**                              1 - overwrite data stream with 40-bit or 32-bit
**                                  user-defined pattern
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciPattForce(struct file *fileHndl, UINT4 slice, UINT4 dir,
                           UINT4 mode)
{
    INT4 result;
    UINT4 traffic;
    UINT4 regCfgAddr, regCfgOffset;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 || dir > 1 || mode > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic < HSCI_FC_8G || 
        ((traffic > HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_EN)
        && (traffic != HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_DIS))) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    switch (dir) {
        case HYPHY20G_TX_DIRECTION:
            regCfgAddr = HYPHY20G_HSCI_FC_CBR_REG_TX_CFG;
            regCfgOffset = HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_FORCE_NOS_PN11_OFF;
            break;
        case HYPHY20G_RX_DIRECTION:
            regCfgAddr = HYPHY20G_HSCI_FC_CBR_REG_RX_CFG;
            regCfgOffset = HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_FORCE_NOS_PN11_OFF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    /* enable/disable the FORCE_NOS_PN11 bit */
    regCfgAddr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(regCfgAddr, slice);
    result = sysHyPhy20gBitWrite(fileHndl, regCfgAddr, regCfgOffset, mode);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciPattForce */



/*******************************************************************************
**
**  hyPhy20gHsciByteAlignedCbrUsrPattCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function allows for a PN11 sequences or 32-bit
**                user-defined pattern to overwrite data in the transmit or
**                receive stream for a HSCI slice operating in
**                the byte-aligned CBR modes.
**
**                In the byte-aligned CBR modes, the running
**                disparity is not applicable.
**
**                The function hyPhy20gHsciPattForce can be used to dynamically
**                have the ordered set (PN11 sequences or 32-bit
**                user-defined pattern) overwrite the user data stream.
**
**                This function may be called while pattern overwriting is
**                forced to dynamically change the user-defined pattern that is
**                overwriting the data stream. However, switching ordered set
**                between PN11 sequences and 32-bit user-defined pattern will
**                disrupt the user data stream.
**
**                This function must be called after hyPhy20gHsciInit.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - HSCI slice processing resource
**                                0 - HSCI slice 0
**                                1 - HSCI slice 1
**                 dir          - data flow direction
**                                0 - transmit data flow
**                                    from the CPB to the SFP
**                                1 - receive data flow
**                                    from the SFP to the CPB
**                 mode         - data stream overwriting operation
**                                0 - overwrite data stream with 32-bit
**                                    user-defined pattern as specified
**                                    in userChar and userCharDisp arguments
**                                1 - overwrite data stream with PN11
**                 userChar     - Used for mode == 0
**                                Four user defined 10 bit characters
**                                each 10 bit character must be positive or
**                                negative disparity.
**                                Default value:
**                                userChar = 0xAA954AA5
**
**  OUTPUTS:        None.
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
INT4 hyPhy20gHsciByteAlignedCbrUsrPattCfg(struct file *fileHndl, UINT4 slice,
                                          UINT4 dir, UINT4 mode, UINT4 userChar)
{
    INT4 result;
    UINT4 traffic;
    UINT4 regCfgAddr, selOffset, rstOffset;
    UINT4 selBitVal, rstBitVal;
    UINT4 olSeqHi, olSeqLow;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        dir > 1 ||
        mode > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic != HSCI_HS_CBR_32BIT_MODE) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    switch (dir) {
        case HYPHY20G_TX_DIRECTION:
            regCfgAddr = HYPHY20G_HSCI_FC_CBR_REG_TX_CFG;
            selOffset = HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_NOS_PN11_SEL_OFF;
            rstOffset = HYPHY20G_HSCI_FC_CBR_BIT_TX_CFG_TX_SOFT_RST_OFF;
            break;
        case HYPHY20G_RX_DIRECTION:
            regCfgAddr = HYPHY20G_HSCI_FC_CBR_REG_RX_CFG;
            selOffset = HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_NOS_PN11_SEL_OFF;
            rstOffset = HYPHY20G_HSCI_FC_CBR_BIT_RX_CFG_RX_SOFT_RST_OFF;
            break;
        default:
            return HYPHY20G_ERR_INVALID_ARG;
    }

    regCfgAddr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(regCfgAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regCfgAddr, selOffset, &selBitVal);
    if(result) return result;
    result = sysHyPhy20gBitRead(fileHndl, regCfgAddr, selOffset, &rstBitVal);
    if(result) return result;

    /* select PN-11 or user defined pattern */
    if (mode != selBitVal) {
        /* The TX_NOS_PN11_SEL and RX_NOS_PN11_SEL fields must be configured
           when the soft reset (SOFT_RST or TX_SOFT_RST/RX_SOFT_RST)
           is asserted. */
        if (!rstBitVal) {
            result = sysHyPhy20gBitWrite(fileHndl, regCfgAddr, rstOffset, 1);
            if(result) return result;
        }

        result = sysHyPhy20gBitWrite(fileHndl, regCfgAddr, selOffset, mode);
        if(result) return result;

        if (!rstBitVal) {
            result = sysHyPhy20gBitWrite(fileHndl, regCfgAddr, rstOffset, 0);
            if(result) return result;
        }
    }

    if (!mode) {
        /* write 32-bit user-defined pattern */
        olSeqHi     = 0x3E;
        olSeqLow    = userChar;
        result = hsciHyPhy20gHsciFcCbrUsrPattCfg(fileHndl, slice, dir,
                 								 olSeqHi, olSeqLow, 0);
        if(result) return result;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHsciByteAlignedCbrUsrPattCfg */


/*******************************************************************************
**
**  hyPhy20gHsci8b10bCbrUsrPattCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the 4 user defined 10 bit
**                characters to overwrite data in the transmit or receive
**                stream for a HSCI slice operating in 8b10b CBR mode.
**
**                As 4 user defined 10 bit characters are specified, the
**                pattern must start with negative running disparity, and end
**                with negative running disparity. This function internally
**                set the negative running disparity for each 10-bit character.
**
**                The function hyPhy20gHsciPattForce can be used to dynamically
**                have the user defined pattern overwrite the user data stream.
**
**                This function may be called while pattern overwriting is
**                forced to dynamically change the ordered set that is
**                overwriting the data stream.
**
**                This function must be called after hyPhy20gHsciInit.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - HSCI slice processing resource
**                                0 - HSCI slice 0
**                                1 - HSCI slice 1
**                 dir          - data flow direction
**                                0 - transmit data flow
**                                    from the CPB to the SFP
**                                1 - receive data flow
**                                    from the SFP to the CPB
**                 userChar[4]  - Four user defined 10 bit characters
**                                each 10 bit character must be positive or
**                                negative disparity.
**                                Default value Escon OLS Sequence:
**                                userChar[3] = 0x0FA (K28.5)
**                                userChar[2] = 0x2A9 (D21.1)
**                                userChar[1] = 0x152 (D10.4)
**                                userChar[0] = 0x2A5 (D21.2) 
**                 userCharDisp - Used for mode == 0
**                                Disparity type for each user defined character
**                                set to 0 for negative disparity
**                                set to 1 for positive disparity
**                                bit3 - disparity for userChar[3]
**                                bit2 - disparity for userChar[2]
**                                bit1 - disparity for userChar[1]
**                                bit0 - disparity for userChar[0]
**                                Default value: 0xC
**
**  OUTPUTS:        None.
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
INT4 hyPhy20gHsci8b10bCbrUsrPattCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 dir, UINT4 userChar[4],
                                    UINT4 userCharDisp)
{
    INT4 result;
    UINT4 traffic;
    UINT4 olSeqHi, olSeqLow;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 || dir > 1 || userCharDisp > 0xF) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if ((traffic != HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_EN) && 
        (traffic != HSCI_HS_CBR_40BIT_MODE_TX_RD_INV_DIS)) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* select user defined pattern */
    olSeqHi     = ((userChar[3] & 0x03FC) >> 2);

    olSeqLow    = ((userChar[3] & 0x0003) << 30) | 
                  ((userChar[2] & 0x03FF) << 20) |
                  ((userChar[1] & 0x03FF) << 10) |
                  (userChar[0] & 0x03FF);
    result = hsciHyPhy20gHsciFcCbrUsrPattCfg(fileHndl, slice, dir,
             								 olSeqHi, olSeqLow, userCharDisp);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHsci8b10bCbrUsrPattCfg */


/*******************************************************************************
**
**  hyPhy20gHsciFcUsrPattCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the 4 user defined 10 bit
**                characters to overwrite data in the transmit or receive
**                stream for a HSCI slice operating in FC mode.
**
**                Running disparity setting is not applicable
**                in ESCON and FC mode.
**
**                The function hyPhy20gHsciPattForce can be used to dynamically
**                have the user defined pattern overwrite the user data stream.
**
**                This function may be called while pattern overwriting is
**                forced to dynamically change the ordered set that is
**                overwriting the data stream.
**
**                This function must be called after hyPhy20gHsciInit.
**
**  INPUTS:       *fileHndl     - base address
**                 slice        - HSCI slice processing resource
**                                0 - HSCI slice 0
**                                1 - HSCI slice 1
**                 dir          - data flow direction
**                                0 - transmit data flow
**                                    from the CPB to the SFP
**                                1 - receive data flow
**                                    from the SFP to the CPB
**                 userChar[4]  - Four user defined 10 bit characters
**                                each 10 bit character must be positive or
**                                negative disparity.
**                                Default value FC OLS Sequence:
**                                userChar[3] = 0x0FA (K28.5)
**                                userChar[2] = 0x2A9 (D21.1)
**                                userChar[1] = 0x152 (D10.4)
**                                userChar[0] = 0x2A5 (D21.2)
**                                The FC NOS Sequence:
**                                userChar[3] = 0x0FA (K28.5)
**                                userChar[2] = 0x2A5 (D21.2)
**                                userChar[1] = 0x14A (D31.5)
**                                userChar[0] = 0x295 (D05.2)
**
**  OUTPUTS:        None.
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
INT4 hyPhy20gHsciFcUsrPattCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
                              UINT4 userChar[4])
{
    INT4 result;
    UINT4 traffic;
    UINT4 olSeqHi, olSeqLow;
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

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if (traffic != HSCI_FC_8G) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* select user defined pattern */
    olSeqHi     = ((userChar[3] & 0x03FC) >> 2);

    olSeqLow    = ((userChar[3] & 0x0003) << 30) | 
                  ((userChar[2] & 0x03FF) << 20) |
                  ((userChar[1] & 0x03FF) << 10) |
                  (userChar[0] & 0x03FF);
    /* userCharDisp is not used for FC mode, set to 0 */
    result = hsciHyPhy20gHsciFcCbrUsrPattCfg(fileHndl, slice, dir,
                                             olSeqHi, olSeqLow, 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gHsciFcUsrPattCfg */

/*******************************************************************************
**
**  hyPhy20gHsciMacGsup43BpEn
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function enables or disables the backup back-pressure 
**                method of MAC_GSUP43 block in ENET_COM subsystem. This 
**                API functionally serves for HSCI applications.
**
**                For the G.Sup43 Clause 7.3 and Clause 6.2 Terminating 
**                application through MAC modes (mode '6' and '7' in  
**                hyPhy20gHsciInit), the device integrated rate limiting  
**                mechanism and the overflow indicated by the corresponding 
**                FIFO_OVRUN_I bit is not supposed to happen.
**                
**                When the overflow happened, manually setting the
**                GSUP43_x_GFP_TX_BKPRESSURE_EN bit to b'1 will allow Rx glue 
**                logic in ENET_COM subsystem unconditionally 
**                back pressure the upstream block (HSCI GSUP43 Tx block). 
**
**                When the GSUP43_x_GFP_TX_BKPRESSURE_EN bit set to b'0, the 
**                back-pressure is determined by the rate limiting mechanism.
**                
**                Used for the G.Sup43 Clause 7.3 and Clause 6.2 Terminating 
**                application through MAC modes (mode '6' and '7' in
**                hyPhy20gHsciInit).
**
**                Note, don't enable this backup back-pressure unless 
**                overflow happened.
**
**  INPUTS:       *fileHndl   - base address
**                  slice     - ENET_COM MAC_GSUP43 slice
**                              0 - ENET_COM MAC_GSUP43 slice associated with
**                                  HSCI instance 0
**                              1 - ENET_COM MAC_GSUP43 slice associated with
**                                  HSCI instance 1
**                 enable     - enable or disable the backup back-pressure
**                              function;
**                              0 - disable
**                              1 - enable
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_INVALID_MODE
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gHsciMacGsup43BpEn(struct file *fileHndl, UINT4 slice,
                               UINT4 enable)
{
    INT4 result;
    UINT4 traffic;
    UINT4 addr;
    UINT4 offset;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        enable > 1) {
        return HYPHY20G_ERR_INVALID_ARG;
    }

    /* retrieve slice mode from context */
    result = hsciHyPhy20gHsciCtxtGetMode(fileHndl, slice, &traffic);
    if(result) return result;
    /* confirm mode before processing */
    if ((traffic != HSCI_MAC_GSUP43_CLAUSE73) && 
        (traffic != HSCI_MAC_GSUP43_CLAUSE62)) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    if (slice == 0) {           
        addr   = HYPHY20G_ENET_COM_TOP_REG_GSUP43_0_CFG;
        offset = HYPHY20G_ENET_COM_TOP_BIT_GSUP43_0_CFG_GSUP43_0_GFP_TX_BKPRESSURE_EN_OFF;
    } else {
        addr   = HYPHY20G_ENET_COM_TOP_REG_GSUP43_1_CFG;
        offset = HYPHY20G_ENET_COM_TOP_BIT_GSUP43_1_CFG_GSUP43_1_GFP_TX_BKPRESSURE_EN_OFF;
    }    
		
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, enable);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsciMacGsup43BpEn */

/*******************************************************************************
**
**  hyPhy20gHsci10GeTestPattInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function initializes one of the two HSCI slices at
**                  the device level and set it to 10GE PCS test pattern mode,
**                  which parallels to the major operating modes of
**                  hyPhy20gHsciInit.
**
**                  Since the test patterns should be used for off-line testing,
**                  this function will reset the HSCI slice and then configure
**                  it to a separate mode for 10GE PCS pattern testing.
**
**                  This function optionally disables one HSCI slice.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  enbl      - enable or disable a HSCI slice
**                              0 - disable a HSCI subsystem;
**                              1 - enable a HSCI subsystem and initialize it
**                                  to 10GE PCS pattern testing mode;
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeTestPattInit(struct file *fileHndl, UINT4 slice,
                                  UINT4 enbl)
{
    INT4 result;
    UINT4 lowpwrOff;
    UINT4 rstOff;
    UINT4 HSCI_TOP_REG_CFG_ADDR;
    UINT4 ENET_10G_TOP_REG_CFG_ADDR;
    UINT4 ENET_10G_TOP_REG_SW_RESETS_ADDR;
    UINT4 HSCI_TOP_REG_CFG_VAL;
    UINT4 ENET_10G_TOP_REG_CFG_VAL;
    UINT4 ENET_10G_TOP_REG_SW_RESETS_VAL;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        enbl > HYPHY20G_ENABLE) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

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

    if (enbl == HYPHY20G_DISABLE){           /* disable operations */
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

    /* configure HSCI level and ENET_10G top level */
    HSCI_TOP_REG_CFG_ADDR           = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_TOP_REG_CFG, slice);
    ENET_10G_TOP_REG_CFG_ADDR       = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TOP_REG_CFG, slice);
    ENET_10G_TOP_REG_SW_RESETS_ADDR = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(HYPHY20G_HSCI_ENET_10G_TOP_REG_SW_RESETS, slice);
    HSCI_TOP_REG_CFG_VAL            = 0x00000D80;
    ENET_10G_TOP_REG_CFG_VAL        = 0x30041546;
    ENET_10G_TOP_REG_SW_RESETS_VAL  = 0x00003DD4;

    result = sysHyPhy20gWrite(fileHndl, HSCI_TOP_REG_CFG_ADDR, HSCI_TOP_REG_CFG_VAL);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, ENET_10G_TOP_REG_CFG_ADDR, ENET_10G_TOP_REG_CFG_VAL);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, ENET_10G_TOP_REG_SW_RESETS_ADDR, ENET_10G_TOP_REG_SW_RESETS_VAL);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeTestPattInit */


/*******************************************************************************
**
**  hyPhy20gHsci10GeR64b66bPattCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function either disables pattern reception or selects
**                  a expected pattern while receiving for the R64B66B block in
**                  one of the two HSCI slices.
**
**                  Note: this function must be called after
**                        hyPhy20gHsci10GeTestPattInit.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  mode      - operating mode of R64B66B block
**                              0 - disable R64B66B block
**                              1 - enable R64B66B block to look for one of
**                                  the three local fault patterns, types 0x2d,
**                                  0x4b, and 0x55;
**                              2 - enable R64B66B block to look for
**                                  all zeroes pattern;
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeR64b66bPattCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 mode)
{
    INT4 result;
    UINT4 addr;
    UINT4 msk;
    UINT4 JITTER_PATTERN_SLCT_VAL;
    UINT4 JITTER_TEST_ENABLE_VAL;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        mode > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_HSCI_ENET_10G_R64B66B_REG_CFG;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    if (mode) {
        JITTER_TEST_ENABLE_VAL  = 1;

        JITTER_PATTERN_SLCT_VAL = mode - 1;
        msk  = HYPHY20G_HSCI_ENET_10G_R64B66B_BIT_CFG_JITTER_PATTERN_SLCT_MSK;
        result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, JITTER_PATTERN_SLCT_VAL);
        if(result) return result;
    } else {
        JITTER_TEST_ENABLE_VAL  = 0;
    }
    
    msk  = HYPHY20G_HSCI_ENET_10G_R64B66B_BIT_CFG_JITTER_TEST_ENABLE_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, JITTER_TEST_ENABLE_VAL);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeR64b66bPattCfg */

/*******************************************************************************
**
**  hyPhy20gHsci10GeT64b66bPattCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures a T64B66B block in one of the  
**                  following test pattern modes:
**                  - pseudo random test pattern that takes constant LF
**                    ordered set as the input of scrambler;
**                  - pseudo random test pattern that takes all
**                    zeros as the input of scrambler;
**                  - square wave test pattern;
**
**                  Note: this function must be called after
**                        hyPhy20gHsci10GeTestPattInit.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  mode      - test pattern mode of T64B66B block
**                              0 - pseudo random test pattern that takes
**                                  constant LF ordered set as the input
**                                  of scrambler;
**                              1 - pseudo random test pattern that takes all
**                                  zeros as the input of scrambler;
**                              2 - square wave test pattern;
**                  swaveLen  - select the number of consecutive 0 and 1
**                              that constitute the square wave when mode == 2;
**                              set this parameter to 0 when mode == 0 or 1;
**                              b'000 - 4 consecutive 0 and 1
**                              ...
**                              b'111 - 11 consecutive 0 and 1
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeT64b66bPattCfg(struct file *fileHndl, UINT4 slice,
                                    UINT4 mode, UINT4 swaveLen)
{
    INT4 result;
    UINT4 REG_CFG_1;
    UINT4 msk;
    UINT4 regData;
    UINT4 JDAT_PAT_SEL_VAL;
    UINT4 JTST_PAT_SEL_VAL;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        mode > 2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    if (mode == 0) {
        JDAT_PAT_SEL_VAL  = 0;
        JTST_PAT_SEL_VAL  = 0;

    } else if (mode == 1) {
        JDAT_PAT_SEL_VAL  = 1;
        JTST_PAT_SEL_VAL  = 0;

    } else {
        JDAT_PAT_SEL_VAL  = 1;
        JTST_PAT_SEL_VAL  = 1;
    }

    REG_CFG_1 = HYPHY20G_HSCI_ENET_10G_T64B66B_REG_CFG_1;
    REG_CFG_1 = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(REG_CFG_1, slice);
    result = sysHyPhy20gRead(fileHndl, REG_CFG_1, &regData);
    if(result) return result;

    if (mode == 2) {
        msk = HYPHY20G_HSCI_ENET_10G_T64B66B_BIT_CFG_1_SWAVE_LEN_MSK;
        result = hyPhy20gVarFieldWrite(&regData, msk, swaveLen);
        if(result) return result;
    }

    msk = HYPHY20G_HSCI_ENET_10G_T64B66B_BIT_CFG_1_JDAT_PAT_SEL_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, JDAT_PAT_SEL_VAL);
    if(result) return result;

    msk = HYPHY20G_HSCI_ENET_10G_T64B66B_BIT_CFG_1_JTST_PAT_SEL_MSK;
    result = hyPhy20gVarFieldWrite(&regData, msk, JTST_PAT_SEL_VAL);
    if(result) return result;

    result = hyPhy20gVarFieldWrite(&regData, (0x0001 << 5), 1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, REG_CFG_1, regData);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeT64b66bPattCfg */


/*******************************************************************************
**
**  hyPhy20gHsci10GeT64b66bSetSeed
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function sets seed A or seed B to a user programmable
**                  value. In the T64B66B block, seed A and seed B are used by
**                  pseudo random test pattern, which can be selected by
**                  hyPhy20gHsci10GeT64b66bPattCfg (mode == 0 or 1).
**
**                  Seed A and seed B are programmable only when jitter pattern
**                  is disabled by hyPhy20gHsci10GeT64b66bPattEnbl.
**
**                  Note: this function must be called after
**                        hyPhy20gHsci10GeTestPattInit.
**
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  seedSel   - select Seed A or B
**                              0 - Seed A
**                              1 - Seed B
**                  seedVal0  - JITTER SEED Value Bit 15:0
**                              legal value 0 - 0xFFFF
**                  seedVal1  - JITTER SEED Value Bit 31:16
**                              legal value 0 - 0xFFFF
**                  seedVal2  - JITTER SEED Value Bit 47:32
**                              legal value 0 - 0xFFFF
**                  seedVal3  - JITTER SEED Value Bit 57:48
**                              legal value 0 - 0x3FF
**
**  OUTPUTS:        None      - None
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**
*******************************************************************************/
INT4 hyPhy20gHsci10GeT64b66bSetSeed(struct file *fileHndl, UINT4 slice,
                                    UINT4 seedSel, UINT4 seedVal0,
                                    UINT4 seedVal1, UINT4 seedVal2,
                                    UINT4 seedVal3)
{
    INT4 result;
    UINT4 msk;
    UINT4 JITT_PAT_EN_VAL;
    UINT4 REG_CFG_1;
    UINT4 seedAddr3;
    UINT4 seedAddr2;
    UINT4 seedAddr1;
    UINT4 seedAddr0;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        seedSel > 1 ||
        seedVal3 > 0x3FF ||
        seedVal2 > 0xFFFF ||
        seedVal1 > 0xFFFF ||
        seedVal0 > 0xFFFF) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    REG_CFG_1 = HYPHY20G_HSCI_ENET_10G_T64B66B_REG_CFG_1;
    REG_CFG_1 = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(REG_CFG_1, slice);
    msk  = HYPHY20G_HSCI_ENET_10G_T64B66B_BIT_CFG_1_JITT_PAT_EN_MSK;
    result = hyPhy20gRegFieldRead(fileHndl, REG_CFG_1, msk, &JITT_PAT_EN_VAL);
    if(result) return result;

    if (JITT_PAT_EN_VAL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_PRECONFIG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    if (seedSel) {
        seedAddr3 = HYPHY20G_HSCI_ENET_10G_T64B66B_REG_JTR_TST_SED_B_3;
        seedAddr2 = HYPHY20G_HSCI_ENET_10G_T64B66B_REG_JTR_TST_SED_B_2;
        seedAddr1 = HYPHY20G_HSCI_ENET_10G_T64B66B_REG_JTR_TST_SED_B_1;
        seedAddr0 = HYPHY20G_HSCI_ENET_10G_T64B66B_REG_JTR_TST_SED_B_0;
    } else {
        seedAddr3 = HYPHY20G_HSCI_ENET_10G_T64B66B_REG_JTR_TST_SED_A_3;
        seedAddr2 = HYPHY20G_HSCI_ENET_10G_T64B66B_REG_JTR_TST_SED_A_2;
        seedAddr1 = HYPHY20G_HSCI_ENET_10G_T64B66B_REG_JTR_TST_SED_A_1;
        seedAddr0 = HYPHY20G_HSCI_ENET_10G_T64B66B_REG_JTR_TST_SED_A_0;
    }
    seedAddr3 = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(seedAddr3, slice);
    seedAddr2 = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(seedAddr2, slice);
    seedAddr1 = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(seedAddr1, slice);
    seedAddr0 = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(seedAddr0, slice);

    result = sysHyPhy20gWrite(fileHndl, seedAddr3, seedVal3);
    if(result) return result;
    result = sysHyPhy20gWrite(fileHndl, seedAddr2, seedVal2);
    if(result) return result;
    result = sysHyPhy20gWrite(fileHndl, seedAddr1, seedVal1);
    if(result) return result;
    result = sysHyPhy20gWrite(fileHndl, seedAddr0, seedVal0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeT64b66bSetSeed */

/*******************************************************************************
**
**  hyPhy20gHsci10GeT64b66bPattEnbl
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables 10GE PCS pattern
**                  transmission in the T64B66B block of one of the
**                  two HSCI slices.
**
**                  Note: this function must be called after
**                        hyPhy20gHsci10GeTestPattInit.
**
**  INPUTS:         fileHndl  - device handle
**                  slice     - HSCI slice processing resource
**                              0 - HSCI instance 0
**                              1 - HSCI instance 1
**                  enbl      - enable or disable 10GE PCS pattern transmission
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
INT4 hyPhy20gHsci10GeT64b66bPattEnbl(struct file *fileHndl, UINT4 slice,
                                     UINT4 enbl)
{
    INT4 result;
    UINT4 addr;
    UINT4 msk;

    /* argument checking */
    if (slice > HYPHY20G_HSCI_NUM_SLICE - 1 ||
        enbl > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }

    addr = HYPHY20G_HSCI_ENET_10G_T64B66B_REG_CFG_1;
    addr = HYPHY20G_HSCI_MTSB_SLICE_REG_OFFSET(addr, slice);
    msk  = HYPHY20G_HSCI_ENET_10G_T64B66B_BIT_CFG_1_JITT_PAT_EN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, msk, enbl);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gHsci10GeT64b66bPattEnbl */
/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
