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
**  FILE        : hyphy20g_essi.c
**
**  $Date: 2011-01-30 17:12:19 +0800 (Sun, 30 Jan 2011) $
**
**  $Revision: 10693 $
**
**  DESCRIPTION :
**
**
**  NOTES       :
**
*******************************************************************************/

/** include files **/
#include "hyphy20g_api_wrapper.h"
#include "hyphy20g_essi.h"

/** external functions **/

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
**  hyPhy20gEssiSliceInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the top level device reset &
**                  low power bits for a particular ESSI Slice.  This function
**                  should be called before enabling individual links using
**                  hyPhy20gEssiLinkInit
**
**  INPUTS:         *fileHndl  - base address
**                  Slice      - ESSI slice to configure : 0 - 3
**                  enable     - 0 - disable subsystem
**                             - 1 - enable subsystem
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiSliceInit(struct file *fileHndl, UINT4 slice, UINT4 enable)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }


    /* Configure top level Reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_ESSI_RST_0_OFF + slice;

    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Set top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_ESSI_LOWPWR_0_OFF + slice;

    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;

    /* Configure slice level reset & low power bits */
    if (enable) {
        regAddr = HYPHY20G_ESSI_ESSI_REG_RST_TSB;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFF);
        if (result) return result;

        regAddr = HYPHY20G_ESSI_ESSI_REG_LW_PWR_LNK_1_8;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFF);
        if (result) return result;

        /* Set top level low power bit to '0' */
        regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
        offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_ESSI_LOWPWR_0_OFF + slice;

        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;
    }


    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiSliceInit */


/*******************************************************************************
**
**  hyPhy20gEssiLinkInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function enables or disables the ESSI resources
**                  associated with a specified link.
**
**                  The subsystems and blocks associated with this function are:
**                  - ESSI (RSDM, TTTP, BSLC)
**
**                  Note: This function controls BSLC_RST[3:0] bits and will
**                        only set a respective BSLC_RST[n] bit to 1 if all
**                        associated TSSI or RSDM blocks have their _RST
**                        bits set to 1.
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - ESSI slice to configure : 0 - 3
**                  link       - Essi link within the slice : 0 - 7
**                  enable     - 0 - disable subsystem
**                             - 1 - enable subsystem
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiLinkInit(struct file *fileHndl, UINT4 slice, UINT4 link,
                          UINT4 enable)
{
    INT4 result;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 mask;
    UINT4 tssiRst, rsdmRst;
    UINT4 bslcCfg;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(link > 7)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_ESSI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_ESSI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Configure block level low power bits */
    regAddr = HYPHY20G_ESSI_ESSI_REG_LW_PWR_LNK_1_8;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, link, 0);
        if (result) return result;
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, link, 1);
        if (result) return result;
    }

    /* Configure block level reset bits */
    regAddr = HYPHY20G_ESSI_ESSI_REG_RST_TSB;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    /* TSSI Reset */
    offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_TSSI_RST_OFF + link;
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
    }

    /* RSDM Reset */
    offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_RSDM_RST_OFF + link;
    if (enable) {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
        if (result) return result;
    } else {
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
    }

    /* BSLC Reset */
    if (enable) {
        result = sysHyPhy20gRead(fileHndl, regAddr, &bslcCfg);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&bslcCfg,
            HYPHY20G_ESSI_ESSI_BIT_RST_TSB_BSLC_RST_MSK,
            0x0);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, bslcCfg);
        if (result) return result;
    } else {
        /* Potentially set BSLC_RST to '1' */
        result = sysHyPhy20gRead(fileHndl, regAddr, &bslcCfg);
        if (result) return result;

        mask = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_TSSI_RST_MSK;
        offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_TSSI_RST_OFF;
        tssiRst = mHYPHY20G_GET_FIELD4(bslcCfg, mask, offset);

        mask = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_RSDM_RST_MSK;
        offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_RSDM_RST_OFF;
        rsdmRst = mHYPHY20G_GET_FIELD4(bslcCfg, mask, offset);

        if ((tssiRst == 0xFF) && (rsdmRst == 0xFF)) {
            result = hyPhy20gVarFieldWrite(&bslcCfg,
                HYPHY20G_ESSI_ESSI_BIT_RST_TSB_BSLC_RST_MSK,
                0xF);
            if (result) return result;
            result = sysHyPhy20gWrite(fileHndl, regAddr, bslcCfg);
            if (result) return result;
        }
    }

    /* Configure Input / Output mode settings for the slice */
    regAddr = HYPHY20G_ESSI_ESSI_REG_IN_OUT_MD;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFFFF);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiLinkInit */


/*******************************************************************************
**
**  hyPhy20gEssiFifoCtr
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function centers the TSSI FIFO for an ESSI link.
**
**                  In order to center the TSSSI FIFO, both the
**                  ABSOLUTE_CENTER and FIFO_CENTER bits of the TSSI register
**                  need to be set to 1.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - ESSI Slice Instance
**                                0 to 3
**                  link        - ESSI Link within Slice Instance
**                                0 to 7
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiFifoCtr(struct file *fileHndl, UINT4 slice, UINT4 link)
{
    INT4 result;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 tssiRst;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(link > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_ESSI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_ESSI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check TSSI reset bit */
    regAddr = HYPHY20G_ESSI_ESSI_REG_RST_TSB;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_TSSI_RST_OFF + link;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &tssiRst);
    if (result) return result;
    if (tssiRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Configure ABSOLUTE_CENTER and FIFO_CENTER bits */
    regAddr = HYPHY20G_ESSI_TSSI_REG_CFG;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_TSSI_TSB_SLICE_REG_OFFSET(regAddr, link);

    /* set ABSOLUTE_CENTER to 1*/
    offset = HYPHY20G_ESSI_TSSI_BIT_CFG_ABSOLUTE_CENTER_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;

    /* set FIFO_CENTER to 1*/
    /* First make sure FIFO_CENTER is '0' */
    mask = HYPHY20G_ESSI_TSSI_BIT_CFG_FIFO_CENTER_MSK;
    result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                        HYPHY20G_DELAY_1250USEC);
    if (result) return result;

    /* Set FIFO_CENTER to '1' */
    offset = HYPHY20G_ESSI_TSSI_BIT_CFG_FIFO_CENTER_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
    if (result) return result;

    /* Make sure FIFO_CENTER returns to '0' */
    mask = HYPHY20G_ESSI_TSSI_BIT_CFG_FIFO_CENTER_MSK;
    result = sysHyPhy20gPollBit(fileHndl, regAddr, mask, 0,
                                                        HYPHY20G_DELAY_1250USEC);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiFifoCtr */

/*******************************************************************************
**
**  hyPhy20gEssiScrmblrCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the scrambler and/or descrambler of
**                  the respective Transmit ESSI link and/or receive ESSI link.
**                  The scrambler/descrambler can be configured to operate in
**                  one of four modes, or be  disabled.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - ESSI Slice Instance
**                                0 to 3
**                  link        - ESSI Link within Slice Instance
**                                0 to 7
**                  dir         - Which direction to configure
**                              - 0 - Configure both scrambler and descrambler
**                              - 1 - Configure only the scrambler
**                              - 2 - Configure only the descrambler
**                  poly        - Which scrambling / descrambling polynomial to
**                                use
**                              - 0 - The standard SONET/SDH scrambling
**                                    polynomial x^7 + x^6 + 1 is used
**                              - 1 - The scrambling polynomial x^9 + x^4 + 1 is
**                                    used
**                  mode        - Scrambling / descrambling mode to use
**                              - 0 - Scrambling is disabled
**                              - 1 - The scrambler resets itself to all ones on
**                                    the most significant bit of the first byte
**                                    following the last J0/Z0 byte. The A1, A2,
**                                    and J0/Z0 bytes are not scrambled.
**                              - 2 - Same as mode '1' except that J0/Z0 bytes
**                                    are scrambled.
**                              - 3 - The scrambler resets itself to all ones on
**                                    the most significant bit of the byte
**                                    following the third A2 byte. Scrambling
**                                    then continues across all bytes until the
**                                    third-to-last A1 byte in the next frame.
**                              - 4 - The scrambler resets itself on the most
**                                    significant bit of first J0 byte. All
**                                    bytes are scrambled except the A1 and A2
**                                    bytes.
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiScrmblrCfg(struct file *fileHndl, UINT4 slice, UINT4 link,
							UINT4 dir, UINT4 poly, UINT4 mode)
{
    INT4 result;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 tssiRst, rsdmRst;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 startColVal = 0;
    UINT4 startByteVal = 0;
    UINT4 startTsVal = 0;
    UINT4 startMskColVal = 0;
    UINT4 startMskByteVal = 0;
    UINT4 startMskTsVal = 0;
    UINT4 endMskColVal = 0;
    UINT4 endMskByteVal = 0;
    UINT4 endMskTsVal = 0;
    UINT4 scrmblCfg1 = 0;
    UINT4 scrmblCfg2 = 0;
    UINT4 scrmblCfg3 = 0;
    UINT4 tssiCfg = 0;
    UINT4 descrmblCfg1 = 0;
    UINT4 descrmblCfg2 = 0;
    UINT4 descrmblCfg3 = 0;
    UINT4 rsdmCfg = 0;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);


    /* argument checking */
    if ((slice > 3)||(link > 7)||(dir > 2)||(poly > 1)||(mode > 4)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_ESSI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_ESSI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check TSSI reset bit */
    if (dir < 2) {
        regAddr = HYPHY20G_ESSI_ESSI_REG_RST_TSB;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_TSSI_RST_OFF + link;
        result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &tssiRst);
        if (result) return result;
        if (tssiRst) {
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
    }

    /* Check RSDM reset bit */
    if (dir != 1) {
        regAddr = HYPHY20G_ESSI_ESSI_REG_RST_TSB;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_RSDM_RST_OFF + link;
        result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &rsdmRst);
        if (result) return result;
        if (rsdmRst) {
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }
    }

    /* Configure the scrambler */
    if (dir < 2) {
        switch (mode) {
            case 1:
                startColVal = 0x4;
                startByteVal = 0x1;
                startMskColVal = 0x1;
                startMskByteVal = 0x1;
                endMskColVal = 0x3;
                endMskByteVal = 0xC;
                break;
            case 2:
                startColVal = 0x4;
                startByteVal = 0x1;
                startMskColVal = 0x1;
                startMskByteVal = 0x1;
                endMskColVal = 0x2;
                endMskByteVal = 0xC;
                break;
            case 3:
                startColVal = 0x2;
                startByteVal = 0x4;
                startMskColVal = 0x1;
                startMskByteVal = 0xA;
                endMskColVal = 0x2;
                endMskByteVal = 0x3;
                break;
            case 4:
                startColVal = 0x3;
                startByteVal = 0x1;
                startMskColVal = 0x1;
                startMskByteVal = 0x1;
                endMskColVal = 0x2;
                endMskByteVal = 0xC;
                break;
        }

        /* Configure SCR_START_COL and SCR_START_BYTE */
        regAddr = HYPHY20G_ESSI_TSSI_REG_CHAR_INS_AND_SCR_CFG_1;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_ESSI_TSSI_TSB_SLICE_REG_OFFSET(regAddr, link);

        result = sysHyPhy20gRead(fileHndl, regAddr, &scrmblCfg1);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&scrmblCfg1,
            HYPHY20G_ESSI_TSSI_BIT_CHAR_INS_AND_SCR_CFG_1_SCR_START_COL_MSK,
            startColVal);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&scrmblCfg1,
            HYPHY20G_ESSI_TSSI_BIT_CHAR_INS_AND_SCR_CFG_1_SCR_START_BYTE_MSK,
            startByteVal);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, scrmblCfg1);
        if (result) return result;

        /* Configure SCR_START_MASK_COL and SCR_START_MASK_BYTE */
        regAddr = HYPHY20G_ESSI_TSSI_REG_SCR_CFG_2;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_ESSI_TSSI_TSB_SLICE_REG_OFFSET(regAddr, link);

        result = sysHyPhy20gRead(fileHndl, regAddr, &scrmblCfg2);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&scrmblCfg2,
            HYPHY20G_ESSI_TSSI_BIT_SCR_CFG_2_SCR_START_MASK_COL_MSK,
            startMskColVal);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&scrmblCfg2,
            HYPHY20G_ESSI_TSSI_BIT_SCR_CFG_2_SCR_START_MASK_BYTE_MSK,
            startMskByteVal);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, scrmblCfg2);
        if (result) return result;

        /* Configure SCR_END_MASK_COL and SCR_END_MASK_BYTE */
        regAddr = HYPHY20G_ESSI_TSSI_REG_SCR_CFG_3;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_ESSI_TSSI_TSB_SLICE_REG_OFFSET(regAddr, link);

        result = sysHyPhy20gRead(fileHndl, regAddr, &scrmblCfg3);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&scrmblCfg3,
            HYPHY20G_ESSI_TSSI_BIT_SCR_CFG_3_SCR_END_MASK_COL_MSK,
            endMskColVal);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&scrmblCfg3,
            HYPHY20G_ESSI_TSSI_BIT_SCR_CFG_3_SCR_END_MASK_BYTE_MSK,
            endMskByteVal);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, scrmblCfg3);
        if (result) return result;

        /* Configure SCR_9_POLY and SCR_EN bits */
        regAddr = HYPHY20G_ESSI_TSSI_REG_CFG;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_ESSI_TSSI_TSB_SLICE_REG_OFFSET(regAddr, link);

        result = sysHyPhy20gRead(fileHndl, regAddr, &tssiCfg);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&tssiCfg,
            HYPHY20G_ESSI_TSSI_BIT_CFG_SCR_9_POLY_MSK,
            poly);
        if (result) return result;

        if (mode) {
            result = hyPhy20gVarFieldWrite(&tssiCfg,
                                    HYPHY20G_ESSI_TSSI_BIT_CFG_SCR_EN_MSK, 1);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&tssiCfg,
                                    HYPHY20G_ESSI_TSSI_BIT_CFG_SCR_EN_MSK, 0);
            if (result) return result;
        }

        result = sysHyPhy20gWrite(fileHndl, regAddr, tssiCfg);
        if (result) return result;
    }

    /* Configure the descrambler */
    if (dir != 1) {
        switch (mode) {
            case 1:
                startColVal = 0x4;
                startTsVal = 0x1;
                startMskColVal = 0x1;
                startMskTsVal = 0x1;
                endMskColVal = 0x3;
                endMskTsVal = 0xC;
                break;
            case 2:
                startColVal = 0x4;
                startTsVal = 0x1;
                startMskColVal = 0x1;
                startMskTsVal = 0x1;
                endMskColVal = 0x2;
                endMskTsVal = 0xC;
                break;
            case 3:
                startColVal = 0x2;
                startTsVal = 0x4;
                startMskColVal = 0x1;
                startMskTsVal = 0xA;
                endMskColVal = 0x2;
                endMskTsVal = 0x3;
                break;
            case 4:
                startColVal = 0x3;
                startTsVal = 0x1;
                startMskColVal = 0x1;
                startMskTsVal = 0x1;
                endMskColVal = 0x2;
                endMskTsVal = 0xC;
                break;
        }

        /* Configure START_DESC_COL and START_DESC_TS */
        regAddr = HYPHY20G_ESSI_RSDM_REG_DSCR_CFG_1;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);

        result = sysHyPhy20gRead(fileHndl, regAddr, &descrmblCfg1);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&descrmblCfg1,
            HYPHY20G_ESSI_RSDM_BIT_DSCR_CFG_1_START_DESCR_COL_MSK,
            startColVal);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&descrmblCfg1,
            HYPHY20G_ESSI_RSDM_BIT_DSCR_CFG_1_START_DESCR_TS_MSK,
            startTsVal);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, descrmblCfg1);
        if (result) return result;

        /* Configure START_MASK_COL and START_MASK_TS */
        regAddr = HYPHY20G_ESSI_RSDM_REG_DSCR_CFG_2;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);

        result = sysHyPhy20gRead(fileHndl, regAddr, &descrmblCfg2);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&descrmblCfg2,
            HYPHY20G_ESSI_RSDM_BIT_DSCR_CFG_2_START_MASK_COL_MSK,
            startMskColVal);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&descrmblCfg2,
            HYPHY20G_ESSI_RSDM_BIT_DSCR_CFG_2_START_MASK_TS_MSK,
            startMskTsVal);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, descrmblCfg2);
        if (result) return result;

        /* Configure END_MASK_COL and END_MASK_TS */
        regAddr = HYPHY20G_ESSI_RSDM_REG_DSCR_CFG_3;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);

        result = sysHyPhy20gRead(fileHndl, regAddr, &descrmblCfg3);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&descrmblCfg3,
            HYPHY20G_ESSI_RSDM_BIT_DSCR_CFG_3_END_MASK_COL_MSK,
            endMskColVal);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&descrmblCfg3,
            HYPHY20G_ESSI_RSDM_BIT_DSCR_CFG_3_END_MASK_TS_MSK,
            endMskTsVal);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, descrmblCfg3);
        if (result) return result;

        /* Configure DESCR_9_POLY and zDESCR_EN bits */
        regAddr = HYPHY20G_ESSI_RSDM_REG_CFG;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);

        result = sysHyPhy20gRead(fileHndl, regAddr, &rsdmCfg);
        if (result) return result;

        result = hyPhy20gVarFieldWrite(&rsdmCfg,
            HYPHY20G_ESSI_RSDM_BIT_CFG_DESCR_9_POLY_MSK,
            poly);
        if (result) return result;

        if (mode) {
            result = hyPhy20gVarFieldWrite(&rsdmCfg,
                                    HYPHY20G_ESSI_RSDM_BIT_CFG_DESCR_EN_MSK, 1);
            if (result) return result;
        } else {
            result = hyPhy20gVarFieldWrite(&rsdmCfg,
                                    HYPHY20G_ESSI_RSDM_BIT_CFG_DESCR_EN_MSK, 0);
            if (result) return result;
        }

        result = sysHyPhy20gWrite(fileHndl, regAddr, rsdmCfg);
        if (result) return result;

    }
    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiScrmblrCfg */

/*******************************************************************************
**
**  hyPhy20gEssiB1FrmCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the B1 Framing Algorithm for a
**                  particular link within an ESSI slice.
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - ESSI Slice Instance
**                                0 to 3
**                  link        - ESSI Link within Slice Instance
**                                0 to 7
**                  b1Mode      - Specifies RSDM B1 Framing Algorithm
**                               0 - two properly spaced A1/A2 framing patterns
**                                   are required to achieve the FRAMED state
**                               1 - three properly spaced framing patterns
**                                   followed by four valid B1s with correct
**                                   A1/A2 framing pattern must be found before
**                                   the RSDM will declare frame alignment.
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiB1FrmCfg(struct file *fileHndl, UINT4 slice, UINT4 link,
						  UINT4 b1Mode)
{
    INT4 result;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 rsdmRst;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(link > 7)||(b1Mode > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_ESSI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_ESSI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check RSDM reset bit */
    regAddr = HYPHY20G_ESSI_ESSI_REG_RST_TSB;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_RSDM_RST_OFF + link;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &rsdmRst);
    if (result) return result;
    if (rsdmRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* COnfigure B1_FRAME_EN */
    regAddr = HYPHY20G_ESSI_RSDM_REG_CFG;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);

    offset = HYPHY20G_ESSI_RSDM_BIT_CFG_B1_FRAME_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, b1Mode);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiB1FrmCfg */

/*******************************************************************************
**
**  hyPhy20gEssiDcBalMonCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the DC Balance monitor within the
**                  RSDM block for a particular ESSI link
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - ESSI Slice Instance
**                                0 to 3
**                  link        - ESSI Link within Slice Instance
**                                0 to 7
**                  thresh      - Specifies the DC balance count threshold for
**                                forcing the RSDM out of frame
**                              - 0 - Recommended value of 0x3E8 (1000) is used
**                              - 1 - 4080 - For using custom values.
**                  decayPer    - Specifies the number of ICLK clock ticks
**                                between decays - must be a multiple of 2
**                              - 0 - Recommended value of 0x1AE (430) is used
**                              - 2 - 2046 - For using custom values
**                  decayFac    - Fraction that the DC balance count is reduced
**                                when a decay occurs.
**                              - 0 - Decay factor of 1
**                              - 1 - Decay factor of 1/2
**                              - 2 - Decay factor of 1/4
**                              - 3 - Decay factor of 1/8 - RECOMMENDED VALUE
**                  enable      - 1 - DC Balance Monitor enabled
**                              - 0 - DC Balance Monitor disabled
**
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiDcBalMonCfg(struct file *fileHndl, UINT4 slice, UINT4 link,
                             UINT4 thresh, UINT4 decayPer, UINT4 decayFac,
                             UINT4 enable)
{
    INT4 result;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 rsdmRst;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 dcBalCfg = 0;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(link > 7)||(thresh > 4080)||(decayPer > 2046)||(decayFac > 3)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    if (decayPer%2) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }


    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_ESSI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_ESSI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check RSDM reset bit */
    regAddr = HYPHY20G_ESSI_ESSI_REG_RST_TSB;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_RSDM_RST_OFF + link;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &rsdmRst);
    if (result) return result;
    if (rsdmRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Configure DC_BAL_THRESH */
    regAddr = HYPHY20G_ESSI_RSDM_REG_DC_BAL_CFG_2;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);

    if (thresh) {
        result = sysHyPhy20gWrite(fileHndl, regAddr, thresh);
        if (result) return result;
    } else {
        result = sysHyPhy20gWrite(fileHndl, regAddr,
                                    HYPHY20G_ESSI_RECOMMENDED_DC_BAL_THRESH);
        if (result) return result;
    }

    /* Configure remaining DC Balance Parameters */
    regAddr = HYPHY20G_ESSI_RSDM_REG_DC_BAL_CFG_1;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);

    if (decayPer) {
        result = hyPhy20gVarFieldWrite(&dcBalCfg,
                HYPHY20G_ESSI_RSDM_BIT_DC_BAL_CFG_1_DECAY_PER_MSK, decayPer);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&dcBalCfg,
                HYPHY20G_ESSI_RSDM_BIT_DC_BAL_CFG_1_DECAY_PER_MSK,
                                    HYPHY20G_ESSI_RECOMMENDED_DECAY_PERIOD);
        if (result) return result;
    }

    result = hyPhy20gVarFieldWrite(&dcBalCfg,
                HYPHY20G_ESSI_RSDM_BIT_DC_BAL_CFG_1_DECAY_FAC_MSK, decayFac);
    if (result) return result;

    result = hyPhy20gVarFieldWrite(&dcBalCfg,
            HYPHY20G_ESSI_RSDM_BIT_DC_BAL_CFG_1_DC_BAL_EN_MSK, enable);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, dcBalCfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiDcBalMonCfg */

/*******************************************************************************
**
**  hyPhy20gEssiCidMonCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the CID (consecutive identical
**                  digits) monitor within the RSDM block for a particular
**                  ESSI link
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - ESSI Slice Instance
**                                0 to 3
**                  link        - ESSI Link within Slice Instance
**                                0 to 7
**                  thresh      - Specifies the CID count threshold for
**                                forcing the RSDM out of frame
**                              - 0 - Recommended value of 0xA (10) is used
**                              - 1 - 63 - For using custom values.
**                  enable      - 1 - CID Monitor enabled
**                              - 0 - CID Monitor disabled
**
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiCidMonCfg(struct file *fileHndl, UINT4 slice, UINT4 link,
                                  UINT4 thresh, UINT4 enable)
{
    INT4 result;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 rsdmRst;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 cidCfg = 0;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(link > 7)||(thresh > 63)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_ESSI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_ESSI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check RSDM reset bit */
    regAddr = HYPHY20G_ESSI_ESSI_REG_RST_TSB;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_RSDM_RST_OFF + link;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &rsdmRst);
    if (result) return result;
    if (rsdmRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Set CID_EN to '0' before changing CID_THRESH */
    regAddr = HYPHY20G_ESSI_RSDM_REG_TRNS_AND_CID_DETECTOR_CFG;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);

    offset = HYPHY20G_ESSI_RSDM_BIT_TRNS_AND_CID_DETECTOR_CFG_CID_EN_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
    if (result) return result;

    /* Configure CID parameters */
    result = sysHyPhy20gRead(fileHndl, regAddr, &cidCfg);
    if (result) return result;

    if (thresh) {
        result = hyPhy20gVarFieldWrite(&cidCfg,
                HYPHY20G_ESSI_RSDM_BIT_TRNS_AND_CID_DETECTOR_CFG_CID_THRESH_MSK,
                                                                        thresh);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&cidCfg,
                HYPHY20G_ESSI_RSDM_BIT_TRNS_AND_CID_DETECTOR_CFG_CID_THRESH_MSK,
                                        HYPHY20G_ESSI_RECOMMENDED_CID_THRESH);
        if (result) return result;
    }

    result = hyPhy20gVarFieldWrite(&cidCfg,
                HYPHY20G_ESSI_RSDM_BIT_TRNS_AND_CID_DETECTOR_CFG_CID_EN_MSK,
                                                                        enable);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, cidCfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiCidMonCfg */

/*******************************************************************************
**
**  hyPhy20gEssiTransitionDetCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the transition detector within the
**                  RSDM block for a particular ESSI link
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - ESSI Slice Instance
**                                0 to 3
**                  link        - ESSI Link within Slice Instance
**                                0 to 7
**                  thresh      - Specifies the transition threshold for
**                                forcing the RSDM out of frame
**                              - 0 - Recommended value of 0x4 (512) is used
**                              - 1 - 7 - For using custom values. (Threshold
**                                will be equal to 128 * thresh
**                  enable      - 1 - transition detector enabled
**                              - 0 - transition detector disabled
**
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiTransitionDetCfg(struct file *fileHndl, UINT4 slice, UINT4 link,
                                  UINT4 thresh, UINT4 enable)
{
    INT4 result;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 rsdmRst;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 tranCfg = 0;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(link > 7)||(thresh > 7)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_ESSI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_ESSI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check RSDM reset bit */
    regAddr = HYPHY20G_ESSI_ESSI_REG_RST_TSB;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_RSDM_RST_OFF + link;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &rsdmRst);
    if (result) return result;
    if (rsdmRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Configure TRAN_THRESH  */
    regAddr = HYPHY20G_ESSI_RSDM_REG_TRNS_AND_CID_DETECTOR_CFG;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);

    result = sysHyPhy20gRead(fileHndl, regAddr, &tranCfg);
    if (result) return result;

    if ((thresh)&&(enable)) {
        /* Enabled to a custom value */
        result = hyPhy20gVarFieldWrite(&tranCfg,
               HYPHY20G_ESSI_RSDM_BIT_TRNS_AND_CID_DETECTOR_CFG_TRAN_THRESH_MSK,
                                                                        thresh);
        if (result) return result;
    } else {
        if (enable) {
            /* Enabled to the recommended value */
            result = hyPhy20gVarFieldWrite(&tranCfg,
                HYPHY20G_ESSI_RSDM_BIT_TRNS_AND_CID_DETECTOR_CFG_TRAN_THRESH_MSK,
                                        HYPHY20G_ESSI_RECOMMENDED_TRAN_THRESH);
            if (result) return result;
        }  else {
            /* Disabled */
            result = hyPhy20gVarFieldWrite(&tranCfg,
                HYPHY20G_ESSI_RSDM_BIT_TRNS_AND_CID_DETECTOR_CFG_TRAN_THRESH_MSK,
                                        0x0);
            if (result) return result;
        }
    }

    result = sysHyPhy20gWrite(fileHndl, regAddr, tranCfg);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} //hyPhy20gEssiTransitionDetCfg

/*******************************************************************************
**
**  hyPhy20gEssiSlicingModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures the BSLC common slicing mode used
**                  by all 8 links within an ESSI slice. It provides the
**                  ability to configure transmit and receive paths
**                  independently
**
**  INPUTS:         *fileHndl  - base address
**                  slice      - ESSI Slice Instance
**                               0 to 3
**                  dir        - Interface Direction to apply slicing mode to
**                               0 - Both Transmit and Receive
**                               1 - Transmit Only
**                               2 - Receive Only
**                  mode       - Slicing Mode
**                               0 - Byte Mode, 1 Link, 8 Bits Per Link, Slicing
**                                   Disabled.
**                               1 - Nibble Mode, 2 Links, 4 Bits Per Link.
**                               2 - Di-bit Mode, 4 Links, 2 Bits Per Link.
**                               3 - Bit Mode, 8 Links, 1 Bit Per Link.
**
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiSlicingModeCfg(struct file *fileHndl, UINT4 slice, UINT4 dir,
				                UINT4 mode)
{
    INT4 result;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 essiRst;
    UINT4 bslcRst;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;
    UINT4 slicingCfg = 0;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 3)||(dir > 2)||(mode > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_ESSI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_ESSI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check BSLC reset bit */
    regAddr = HYPHY20G_ESSI_ESSI_REG_RST_TSB;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &essiRst);
    if (result) return result;
    offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_BSLC_RST_OFF;
    mask = 0xF << offset;
    bslcRst = mHYPHY20G_GET_FIELD4(essiRst, mask, offset);
    if (bslcRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    regAddr = HYPHY20G_ESSI_ESSI_REG_SLM;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    switch (dir) {
        case 0:
            /* Configure both Tx & Rx */
            mask = HYPHY20G_ESSI_ESSI_BIT_SLM_SLICINGMODE_TX_MSK;
            result = hyPhy20gVarFieldWrite(&slicingCfg, mask, mode);
            if (result) return result;

            mask = HYPHY20G_ESSI_ESSI_BIT_SLM_SLICINGMODE_RX_MSK;
            result = hyPhy20gVarFieldWrite(&slicingCfg, mask, mode);
            if (result) return result;

            result = sysHyPhy20gWrite(fileHndl, regAddr, slicingCfg);
            if (result) return result;
            break;
        case 1:
            /* Configure only Tx */
            result = sysHyPhy20gRead(fileHndl, regAddr, &slicingCfg);
            if (result) return result;

            mask = HYPHY20G_ESSI_ESSI_BIT_SLM_SLICINGMODE_TX_MSK;
            result = hyPhy20gVarFieldWrite(&slicingCfg, mask, mode);
            if (result) return result;

            result = sysHyPhy20gWrite(fileHndl, regAddr, slicingCfg);
            if (result) return result;
            break;
        case 2:
            /* Configure only Rx */
            result = sysHyPhy20gRead(fileHndl, regAddr, &slicingCfg);
            if (result) return result;

            mask = HYPHY20G_ESSI_ESSI_BIT_SLM_SLICINGMODE_RX_MSK;
            result = hyPhy20gVarFieldWrite(&slicingCfg, mask, mode);
            if (result) return result;

            result = sysHyPhy20gWrite(fileHndl, regAddr, slicingCfg);
            if (result) return result;
            break;
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiSlicingModeCfg */


/*******************************************************************************
**
**  hyPhy20gEssiReAlign
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs frame re-alignment on the RSDM of
**                  a particular ESSI link (also known as the smart framing
**                  algorithm).  It adjusts the frame pulse delay registers in
**                  order to achieve frame synchronization for the ESSI links.
**
**                  Synchronization is achieved through an externally controlled
**                  multi-frame synchronization signal (J0FP)
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - ESSI Slice Instance
**                                0 to 3
**                  link        - ESSI Link within Slice Instance
**                                0 to 7
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiReAlign(struct file *fileHndl, UINT4 slice, UINT4 link)
{

    INT4 result;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 rsdmRst;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 diffJ0 = 0;
    UINT4 essiLink = 0;
    UINT4 newFpDly = 0;
    UINT4 essiJ0Dly = 0;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);


    /* argument checking */
    if ((slice > 3)||(link > 7)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_ESSI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_ESSI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check RSDM reset bit */
    regAddr = HYPHY20G_ESSI_ESSI_REG_RST_TSB;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_RSDM_RST_OFF + link;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &rsdmRst);
    if (result) return result;
    if (rsdmRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* 1. Wait for the RSDM to frame to the receive stream */
    regAddr = HYPHY20G_ESSI_RSDM_REG_STAT;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);

    result = sysHyPhy20gPollBit(fileHndl, regAddr,
                              HYPHY20G_ESSI_RSDM_BIT_STAT_OOF_ESSI_V_MSK, 0,
                              HYPHY20G_DELAY_1250USEC);
    if (result) return result;

    /* 2. Trigger a transfer of the J0 difference counter */
    regAddr = HYPHY20G_ESSI_RSDM_REG_J0_DIFF;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);

    result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
    if (result) return result;

    /* 3. Wait for the transfer to complete */
    regAddr = HYPHY20G_ESSI_RSDM_REG_STAT;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);

    result = sysHyPhy20gPollBit(fileHndl, regAddr,
                              HYPHY20G_ESSI_RSDM_BIT_STAT_TIP_DIFF_J0_MSK, 0,
                              HYPHY20G_DELAY_1250USEC);
    if (result) return result;

    /* 4. Read the DIFF_J0 value  */
    regAddr = HYPHY20G_ESSI_RSDM_REG_J0_DIFF;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, link);

    result = sysHyPhy20gRead(fileHndl, regAddr, &diffJ0);
    if (result) return result;

    /* If DIFF_J0 > 50 and DIFF_J0 < 57, procedure is complete */
    if ((diffJ0 > 50) && (diffJ0 < 57)) return HYPHY20G_SUCCESS;

    /* 5. Read ESSI_J0DLY from TSE */
    essiLink = (8*slice) + link;
    regAddr = HYPHY20G_TSE_TOP_REG_J0_DLY_CFG_FOR_ESSI_OFFSET(essiLink);

    result = sysHyPhy20gRead(fileHndl, regAddr, &essiJ0Dly);
    if (result) return result;

    /* 6. Calculate the new DFPDLY for the middle of the FIFO range */
    newFpDly = essiJ0Dly + 0x97DF - diffJ0 + 0x34;
    /* Wrap if greater than the counter size */
    if(newFpDly >= 0x97DF) {
        newFpDly = newFpDly - 0x97DF;
    }

    /* 7. Write ESSI_J0DLY with the newFpDly value */
    result = sysHyPhy20gWrite(fileHndl, regAddr, newFpDly);
    if (result) return result;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gEssiReAlign */

/*******************************************************************************
**
**  hyPhy20gEssiSetSts192EssiJ0Delays
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function need only be called for sets of four ESSI 
**                  links processing an STS-192 signal bound for DMI 
**                  processing.  It is not required for non STS-192 signals 
**                  or when the associated DMI is bypassed.
**
**                  This function configures the ESSI_J0DLY setting for the 
**                  the set of four ESSI links transferring an STS-192 signal. 
**                  The ESSI_J0DLY fields are set to ensure the J0 is present
**                  in the FIFO for each link.                         
**                  
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - DMI slice instance
**                                0 to 4
**                  workProtSel - Set of ESSI links associated with the DMI 
**                                slice
**                                0 - working
**                                1 - protect
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gEssiSetSts192EssiJ0Delays(struct file *fileHndl, UINT4 slice, 
                                        UINT4 workProtSel)
{
    INT4 result;
    UINT4 regAddr;
    UINT4 offset;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 rsdmRst;
    UINT4 linkOff = 0;
    UINT4 diffJ0 = 0;
    UINT4 newFpDly = 0;
    UINT4 essiJ0Dly = 0;
    UINT4 essiLink = 0;
    UINT4 sliceOff = 0;  
    UINT4 total = 0;
    UINT4 val = 0; 
    UINT4 avgJ0Dly = 0;
    UINT4 essiSlice;
    UINT4 i;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if (slice > 4 || workProtSel > 1) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Calculate slice offset based on the data source of the link */   
    if (slice == 4) { 
        sliceOff = 28;
        linkOff = 4;
    } else { 
        sliceOff = (workProtSel == 1)? (slice*4 + 16):(slice*4);  
        linkOff = (sliceOff % 8 == 0)? 0:4;
    }
    
    /* Calculate ESSI slice offset based on DMI slice */
    switch (slice) {
        case 0:
        case 1:
            essiSlice = (workProtSel == 1)? 2:0;
            break;
        case 2:
        case 3:
            essiSlice = (workProtSel == 1)? 3:1;
            break;
        default:
            DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
            return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_ESSI_RST_0_OFF + essiSlice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_ESSI_LOWPWR_0_OFF + essiSlice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check RSDM reset bit */
    regAddr = HYPHY20G_ESSI_ESSI_REG_RST_TSB;
    regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, essiSlice);
    offset = HYPHY20G_ESSI_ESSI_BIT_RST_TSB_RSDM_RST_OFF + linkOff;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &rsdmRst);
    if (result) return result;
    if (rsdmRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Initialize index for ESSI link */  
    essiLink = sliceOff;
    
    for (i = linkOff; i < linkOff + 4; i++) {
        DBG_PRINT("%s, %s, %d, i = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)i);
        /* 1. Wait for the RSDM to frame to the receive stream */
        regAddr = HYPHY20G_ESSI_RSDM_REG_STAT;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, essiSlice);
        regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, i);

        result = sysHyPhy20gPollBit(fileHndl, regAddr,
                                    HYPHY20G_ESSI_RSDM_BIT_STAT_OOF_ESSI_V_MSK, 
                                    0, HYPHY20G_DELAY_1250USEC);
        if (result) return result;

        /* 2. Trigger a transfer of the J0 difference counter */
        regAddr = HYPHY20G_ESSI_RSDM_REG_J0_DIFF;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, essiSlice);
        regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, i);

        result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
        if (result) return result;

        /* 3. Wait for the transfer to complete */
        regAddr = HYPHY20G_ESSI_RSDM_REG_STAT;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, essiSlice);
        regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, i);

        result = sysHyPhy20gPollBit(fileHndl, regAddr,
                                    HYPHY20G_ESSI_RSDM_BIT_STAT_TIP_DIFF_J0_MSK, 
                                    0, HYPHY20G_DELAY_1250USEC);
        if (result) return result;

        /* 4. Read the DIFF_J0 value  */
        regAddr = HYPHY20G_ESSI_RSDM_REG_J0_DIFF;
        regAddr = HYPHY20G_ESSI_MTSB_SLICE_REG_OFFSET(regAddr, essiSlice);
        regAddr = HYPHY20G_ESSI_RSDM_TSB_SLICE_REG_OFFSET(regAddr, i);

        result = sysHyPhy20gRead(fileHndl, regAddr, &diffJ0);
        if (result) return result;

        /* 5. Read ESSI_J0DLY from TSE */ 
        DBG_PRINT("%s, %s, %d, Pre essiLink = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)essiLink);
        regAddr = HYPHY20G_TSE_TOP_REG_J0_DLY_CFG_FOR_ESSI_OFFSET(essiLink);
        result = sysHyPhy20gRead(fileHndl, regAddr, &essiJ0Dly);
        if (result) return result;

        /* increment essiLink for the next essi link */
        essiLink++;
        
        DBG_PRINT("%s, %s, %d, Post essiLink = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)essiLink);
        DBG_PRINT("%s, %s, %d, linkOff(i) = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)i);
        
        /* 6. Calculate the new DFPDLY for the middle of the FIFO range */
        newFpDly = essiJ0Dly + 0x97DF - diffJ0 + 0x34;
        /* Wrap if greater than the counter size */
        if(newFpDly >= 0x97DF) {
            newFpDly = newFpDly - 0x97DF;
        }

        /* 7. Write ESSI_J0DLY with the newFpDly value */
        result = sysHyPhy20gWrite(fileHndl, regAddr, newFpDly);
        if (result) return result;   
        DBG_PRINT("%s, %s, %d, newFpDly = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)newFpDly);
            
        /* 8. Read J0DLY from all 4 links and average them to produce a 
              consistent value across all 4 links of STS192c */
        result = sysHyPhy20gRead(fileHndl, regAddr, &val);
        if (result) return result;
            
        total += val; 
        DBG_PRINT("%s, %s, %d, total = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)total);
    }
    
    /* 9. Write the averaged out value to all 4 links */
    avgJ0Dly = total / 4;
    for (i = sliceOff; i < sliceOff + 4; i++) {
        regAddr = HYPHY20G_TSE_TOP_REG_J0_DLY_CFG_FOR_ESSI_OFFSET(i); 
        DBG_PRINT("%s, %s, %d, sliceOff = %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)i);
        
        result = sysHyPhy20gWrite(fileHndl, regAddr, avgJ0Dly);
        if (result) return result; 
        DBG_PRINT("%s, %s, %d, avgJ0Dly = 0x%lx\n",__FILE__, __FUNCTION__, __LINE__, avgJ0Dly);
        
    }
    
    return HYPHY20G_SUCCESS;     
                                            
} /* hyPhy20gEssiSetSts192EssiJ0Delays */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/
/* end of file */
