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
**  FILE        : hyphy20g_cpb_features.c
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
#include "hyphy20g_cpb_features.h"

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
**  hyPhy20gCpbQdrBistTest
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs the dynamic BIST test on the QDR PHY.
**                  Ensure QDR PHY is configured before running this test
**                  by calling hyPhy20gCpbInit with ramType > 0 and enable = 1.
**
**                  Data is written to the RAM in four 104-bit bursts.
**                  wrData[13] represents the full 416-bits of data.
**
**  INPUTS:         *fileHndl  - device handle
**                  startAddr  - start address of RAM
**                  endAddr    - end address of RAM
**                  wrPatt[13] - test pattern to write into RAM (note that this
**                               value is XOR'd with the write address location)
**
**                  The following returns the status of the test:
**
**                  *testResult - BIST test result
**                                    0 - BIST test passed
**                                    1 - BIST test failed
**                  *burstErr   - If testResult == 1, errored burst from read
**                  *addrErr    - If testResult == 1, errored read address
**                  *rdData[4]  - If testResult == 1, read data from errored
**                                burst and address
**                                    rdData[0] = QDR_TEST_ERR_RD_DATA[31:0]
**                                    ...
**                                    rdData[3] = QDR_TEST_ERR_RD_DATA[103:96]
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbQdrBistTest(struct file *fileHndl, UINT4 startAddr,
                            UINT4 endAddr, UINT4 wrPatt[13], UINT4 *testResult,
                            UINT4 *burstErr, UINT4 *addrErr, UINT4 *rdData)
{
    UINT4 addr, offset, mask, val;
    UINT4 wcimode, intVal;
    UINT4 i;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Wait for PHY_READY bit to get asserted */
    addr = HYPHY20G_QDR_PHY_TOP_REG_QDR_PHY_CFG;
    result = sysHyPhy20gPollBit(fileHndl, addr,
                             HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_PHY_READY_MSK,
                             HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_PHY_READY_MSK,
                             HYPHY20G_DELAY_20USEC);
    if(result) return result;


    /* Set DYNAMIC_TEST_MODE bit */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_CFG;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_DYNAMIC_TEST_MODE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 1);
    if(result) return result;


    /* clear interrupt by reading WCI_MODE bit of CPB Subsystem */
    addr = HYPHY20G_PGMRCLK_REG_WCIMODE_CTL_1;
    offset = HYPHY20G_PGMRCLK_BIT_WCIMODE_CTL_1_WCI_MODE_CTRL_OFF << 2;
    result = sysHyPhy20gBitRead(fileHndl, addr, offset, &wcimode);
    if(result) return result;

    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_INT;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_INT_QDR_TEST_ERR_I_MSK;
    if (wcimode == 1) {
        result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, 1);
        if(result) return result;
    } else {
        result = hyPhy20gRegFieldRead(fileHndl, addr, mask, &val);
        if(result) return result;
    }


    /* Program write data register desired pattern */
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_WR_DAT_QDR_TEST_WR_DATA_MSK;
    for (i = 0; i < 13; i++) {
        addr = HYPHY20G_CPB_CPB_REG_QDR_TST_WR_DAT_OFFSET(i);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, wrPatt[i]);
        if(result) return result;
    }


    /* Program compare mask register to compare all 104 bits of data */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_MSK_0;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_MSK_0_COMPARE_MASK_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, mask);
    if(result) return result;

    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_MSK1;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_MSK1_COMPARE_MASK_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, mask);
    if(result) return result;

    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_MSK_2;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_MSK_2_COMPARE_MASK_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, mask);
    if(result) return result;

    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_MSK_3;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_MSK_3_COMPARE_MASK_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, mask);
    if(result) return result;


    /* Program write start address and write direction */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_WR_CTL_STR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_WR_CTL_STR_WR_ADDR_INC_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, 1);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_WR_CTL_STR_WR_START_ADDR_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, startAddr);
    if(result) return result;

    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_WR_CTL_STR;
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;


    /* Program write end address */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_WR_CTL_END;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_WR_CTL_END_WR_END_ADDR_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, endAddr);
    if(result) return result;


    /* Program read start address and read direction */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_CTL_STR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_CTL_STR_RD_ADDR_INC_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, 1);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_CTL_STR_RD_START_ADDR_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, startAddr);
    if(result) return result;

    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_CTL_STR;
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;


    /* Program read end address */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_CTL_END;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_CTL_END_RD_END_ADDR_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, endAddr);
    if(result) return result;


    /* Begin writer operation */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_CFG;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_RD_GO_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 0);
    if(result) return result;

    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_WR_GO_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 0);
    if(result) return result;

    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 1);
    if(result) return result;


    /* Wait sufficient time for writer to write the entire QDR SRAM at least
       once */
    result = sysHyPhy20gMsDelay(500);
    if(result) return result;


    /* Stop writer operation */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_CFG;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_WR_GO_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 0);
    if(result) return result;


    /* Begin reader operation */
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_RD_GO_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 1);
    if(result) return result;


    /* Wait */
    result = sysHyPhy20gMsDelay(500);
    if(result) return result;


    /* Check test result */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_INT;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_INT_QDR_TEST_ERR_I_OFF;
    result = sysHyPhy20gBitRead(fileHndl, addr, offset, &intVal);
    if(result) return result;

    if (intVal == 0) {
        /* BIST test passed */
        *testResult = 0;
        *burstErr = 0;
        *addrErr = 0;
        for (i = 0; i < 4; i++) {
            *rdData = 0;
            rdData++;
        }
    } else {
        /* BIST test failed */
        *testResult = 1;

        addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_ADR;
        mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_ADR_QDR_TEST_ERR_RD_ADDR_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, addr, mask, &val);
        if(result) return result;

        *burstErr = val & 0x3;
        *addrErr = val >> 2;

        addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_DAT_0;
        mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_DAT_0_QDR_TEST_ERR_RD_DATA_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, addr, mask, rdData);
        if(result) return result;

        addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_DAT_1;
        mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_DAT_1_QDR_TEST_ERR_RD_DATA_MSK;
        rdData++;
        result = hyPhy20gRegFieldRead(fileHndl, addr, mask, rdData);
        if(result) return result;

        addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_DAT_2;
        mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_DAT_2_QDR_TEST_ERR_RD_DATA_MSK;
        rdData++;
        result = hyPhy20gRegFieldRead(fileHndl, addr, mask, rdData);
        if(result) return result;

        addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_DAT_3;
        mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_DAT_3_QDR_TEST_ERR_RD_DATA_MSK;
        rdData++;
        result = hyPhy20gRegFieldRead(fileHndl, addr, mask, rdData);
        if(result) return result;

    }


    /* Stop reader operation */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_CFG;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_RD_GO_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 0);
    if(result) return result;


    /* Clear DYNAMIC_TEST_MODE bit */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_CFG;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_DYNAMIC_TEST_MODE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbQdrBistTest */

/*******************************************************************************
**
**  hyPhy20gCpbQdrBistWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs the write portion of the dynamic BIST
**                  test on the QDR PHY.  Ensure QDR PHY is configured before
**                  running this test by calling hyPhy20gCpbInit with
**                  ramType > 0 and enable = 1.
**
**                  Data is written to the RAM in four 104-bit bursts.
**                  wrData[13] represents the full 416-bits of data.
**
**  INPUTS:         *fileHndl  - device handle
**                  startAddr  - start address of RAM
**                  endAddr    - end address of RAM
**                  wrPatt[13] - test pattern to write into RAM (note that this
**                               value is XOR'd with the write address location)
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbQdrBistWrite(struct file *fileHndl, UINT4 startAddr,
                             UINT4 endAddr, UINT4 wrPatt[13])
{
    UINT4 addr, offset, mask, val;
    UINT4 i;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Wait for PHY_READY bit to get asserted */
    addr = HYPHY20G_QDR_PHY_TOP_REG_QDR_PHY_CFG;
    result = sysHyPhy20gPollBit(fileHndl, addr,
                             HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_PHY_READY_MSK,
                             HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_PHY_READY_MSK,
                             HYPHY20G_DELAY_20USEC);
    if(result) return result;


    /* Set DYNAMIC_TEST_MODE bit */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_CFG;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_DYNAMIC_TEST_MODE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 1);
    if(result) return result;


    /* Program write data register desired pattern */
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_WR_DAT_QDR_TEST_WR_DATA_MSK;
    for (i = 0; i < 13; i++) {
        addr = HYPHY20G_CPB_CPB_REG_QDR_TST_WR_DAT_OFFSET(i);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, wrPatt[i]);
        if(result) return result;
    }


    /* Program write start address and write direction */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_WR_CTL_STR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_WR_CTL_STR_WR_ADDR_INC_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, 1);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_WR_CTL_STR_WR_START_ADDR_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, startAddr);
    if(result) return result;

    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_WR_CTL_STR;
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;


    /* Program write end address */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_WR_CTL_END;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_WR_CTL_END_WR_END_ADDR_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, endAddr);
    if(result) return result;


    /* Begin writer operation */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_CFG;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_WR_GO_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 1);
    if(result) return result;


    /* Wait sufficient time for writer to write the entire QDR SRAM at least
       once */
    result = sysHyPhy20gMsDelay(500);
    if(result) return result;


    /* Stop writer operation */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_CFG;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_WR_GO_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 0);
    if(result) return result;


    /* Clear DYNAMIC_TEST_MODE bit */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_CFG;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_DYNAMIC_TEST_MODE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbQdrBistWrite */

/*******************************************************************************
**
**  hyPhy20gCpbQdrBistReadTest
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function performs the read portion of the dynamic BIST
**                  test on the QDR PHY.  Ensure QDR PHY is configured before
**                  running this test by calling hyPhy20gCpbInit with
**                  ramType > 0 and enable = 1.  Test pattern should be
**                  written into RAM using hyPhy20gCpbQdrBistWrite before
**                  calling this function.
**
**  INPUTS:         *fileHndl  - device handle
**                  startAddr  - start address of RAM
**                  endAddr    - end address of RAM
**                  wrPatt[13] - test pattern that was written into RAM
**                               using hyPhy20gCpbQdrBistWrite
**
**                  The following returns the status of the test:
**
**                  *testResult - BIST test result
**                                    0 - BIST test passed
**                                    1 - BIST test failed
**                  *burstErr   - If testResult == 1, errored burst from read
**                  *addrErr    - If testResult == 1, errored read address
**                  *rdData[4]  - If testResult == 1, read data from errored
**                                burst and address
**                                    rdData[0] = QDR_TEST_ERR_RD_DATA[31:0]
**                                    ...
**                                    rdData[3] = QDR_TEST_ERR_RD_DATA[103:96]
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbQdrBistReadTest(struct file *fileHndl, UINT4 startAddr,
                                UINT4 endAddr, UINT4 wrPatt[13],
                                UINT4 *testResult, UINT4 *burstErr,
                                UINT4 *addrErr, UINT4 *rdData)
{
    UINT4 addr, offset, mask, val;
    UINT4 wcimode, intVal;
    UINT4 i;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* Wait for PHY_READY bit to get asserted */
    addr = HYPHY20G_QDR_PHY_TOP_REG_QDR_PHY_CFG;
    result = sysHyPhy20gPollBit(fileHndl, addr,
                             HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_PHY_READY_MSK,
                             HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_PHY_READY_MSK,
                             HYPHY20G_DELAY_20USEC);
    if(result) return result;


    /* Set DYNAMIC_TEST_MODE bit */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_CFG;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_DYNAMIC_TEST_MODE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 1);
    if(result) return result;


    /* clear interrupt by reading WCI_MODE bit of CPB subsystem */
    addr = HYPHY20G_PGMRCLK_REG_WCIMODE_CTL_1;
    offset = HYPHY20G_PGMRCLK_BIT_WCIMODE_CTL_1_WCI_MODE_CTRL_OFF << 2;
    result = sysHyPhy20gBitRead(fileHndl, addr, offset, &wcimode);
    if(result) return result;

    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_INT;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_INT_QDR_TEST_ERR_I_MSK;
    if (wcimode == 1) {
        result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, 1);
        if(result) return result;
    } else {
        result = hyPhy20gRegFieldRead(fileHndl, addr, mask, &val);
        if(result) return result;
    }


    /* Program write data register desired pattern */
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_WR_DAT_QDR_TEST_WR_DATA_MSK;
    for (i = 0; i < 13; i++) {
        addr = HYPHY20G_CPB_CPB_REG_QDR_TST_WR_DAT_OFFSET(i);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, wrPatt[i]);
        if(result) return result;
    }


    /* Program compare mask register to compare all 104 bits of data */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_MSK_0;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_MSK_0_COMPARE_MASK_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, mask);
    if(result) return result;

    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_MSK1;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_MSK1_COMPARE_MASK_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, mask);
    if(result) return result;

    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_MSK_2;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_MSK_2_COMPARE_MASK_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, mask);
    if(result) return result;

    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_MSK_3;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_MSK_3_COMPARE_MASK_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, mask);
    if(result) return result;


    /* Program read start address and read direction */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_CTL_STR;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_CTL_STR_RD_ADDR_INC_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, 1);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_CTL_STR_RD_START_ADDR_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, startAddr);
    if(result) return result;

    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_CTL_STR;
    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;


    /* Program read end address */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_CTL_END;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_CTL_END_RD_END_ADDR_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, endAddr);
    if(result) return result;


    /* Begin reader operation */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_CFG;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_RD_GO_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 1);
    if(result) return result;


    /* Wait */
    result = sysHyPhy20gMsDelay(500);
    if(result) return result;


    /* Check test result */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_INT;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_INT_QDR_TEST_ERR_I_OFF;
    result = sysHyPhy20gBitRead(fileHndl, addr, offset, &intVal);
    if(result) return result;

    if (intVal == 0) {
        /* BIST test passed */
        *testResult = 0;
        *burstErr = 0;
        *addrErr = 0;
        for (i = 0; i < 4; i++) {
            *rdData = 0;
            rdData++;
        }
    } else {
        /* BIST test failed */
        *testResult = 1;

        addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_ADR;
        mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_ADR_QDR_TEST_ERR_RD_ADDR_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, addr, mask, &val);
        if(result) return result;

        *burstErr = val & 0x3;
        *addrErr = val >> 2;

        addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_DAT_0;
        mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_DAT_0_QDR_TEST_ERR_RD_DATA_MSK;
        result = hyPhy20gRegFieldRead(fileHndl, addr, mask, rdData);
        if(result) return result;

        addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_DAT_1;
        mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_DAT_1_QDR_TEST_ERR_RD_DATA_MSK;
        rdData++;
        result = hyPhy20gRegFieldRead(fileHndl, addr, mask, rdData);
        if(result) return result;

        addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_DAT_2;
        mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_DAT_2_QDR_TEST_ERR_RD_DATA_MSK;
        rdData++;
        result = hyPhy20gRegFieldRead(fileHndl, addr, mask, rdData);
        if(result) return result;

        addr = HYPHY20G_CPB_CPB_REG_QDR_TST_RD_DAT_3;
        mask = HYPHY20G_CPB_CPB_BIT_QDR_TST_RD_DAT_3_QDR_TEST_ERR_RD_DATA_MSK;
        rdData++;
        result = hyPhy20gRegFieldRead(fileHndl, addr, mask, rdData);
        if(result) return result;

    }


    /* Stop reader operation */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_CFG;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_RD_GO_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 0);
    if(result) return result;


    /* Clear DYNAMIC_TEST_MODE bit */
    addr = HYPHY20G_CPB_CPB_REG_QDR_TST_CFG;
    offset = HYPHY20G_CPB_CPB_BIT_QDR_TST_CFG_DYNAMIC_TEST_MODE_OFF;
    result = sysHyPhy20gBitWrite(fileHndl, addr, offset, 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbQdrBistReadTest */

/*******************************************************************************
**
**  hyPhy20gCpbQdrDebugPortWrite
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function writes data into the QDR RAM using the
**                  QDR Debug Port.  Ensure QDR PHY is configured before
**                  running this test by calling hyPhy20gCpbInit with
**                  ramType > 0 and enable = 1.
**
**                  Data is written to the RAM in four 104-bit bursts (96-bit
**                  data plus 8-bit ECC).  wrData[12] represents the 384-bit
**                  data to be written to RAM.
**
**                      Burst 0:  ECC[7:0] & QDR_DEBUG_WR_DATA[383:288]
**                      Burst 1:  ECC[7:0] & QDR_DEBUG_WR_DATA[287:192]
**                      Burst 2:  ECC[7:0] & QDR_DEBUG_WR_DATA[191:96]
**                      Burst 3:  ECC[7:0] & QDR_DEBUG_WR_DATA[95:0]
**
**  INPUTS:         *fileHndl  - device handle
**                  wrAddr     - QDR RAM write address
**                  wrPatt[12] - test pattern to write to RAM
**                                   wrPatt[0] = QDR_DEBUG_WR_DATA[31:0]
**                                   ...
**                                   wrPatt[11] = QDR_DEBUG_WR_DATA[383:352]
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbQdrDebugPortWrite(struct file *fileHndl, UINT4 wrAddr,
                                  UINT4 wrPatt[12])
{
    UINT4 addr, mask, val;
    UINT4 i;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
	
    /* Wait for PHY_READY bit to get asserted */
    addr = HYPHY20G_QDR_PHY_TOP_REG_QDR_PHY_CFG;
    result = sysHyPhy20gPollBit(fileHndl, addr,
                             HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_PHY_READY_MSK,
                             HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_PHY_READY_MSK,
                             HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Enable debug port */
    addr = HYPHY20G_CPB_CPB_REG_QDR_RAM_DBG_CTL;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_CTL_QDR_DEBUG_EN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, 1);
    if(result) return result;

    /* Write data to register */
    mask = HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_WR_DAT_QDR_DEBUG_WR_DATA_MSK;
    for (i = 0; i < 12; i++) {
        addr = HYPHY20G_CPB_CPB_REG_QDR_RAM_DBG_WR_DAT_OFFSET(i);
        result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, wrPatt[i]);
        if(result) return result;
    }

    /* Write control register */
    addr = HYPHY20G_CPB_CPB_REG_QDR_RAM_DBG_CTL;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_CTL_QDR_DEBUG_RWB_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, 0);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_CTL_QDR_DEBUG_ADDR_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, wrAddr);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_CTL_QDR_DEBUG_ACCESS_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, 1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* Poll for QDR_DEBUG_ACCESS to clear */
    addr = HYPHY20G_CPB_CPB_REG_QDR_RAM_DBG_CTL;
    result = sysHyPhy20gPollBit(fileHndl, addr,
                      HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_CTL_QDR_DEBUG_ACCESS_MSK,
                      0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Disable debug port */
    addr = HYPHY20G_CPB_CPB_REG_QDR_RAM_DBG_CTL;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_CTL_QDR_DEBUG_EN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbQdrDebugPortWrite */

/*******************************************************************************
**
**  hyPhy20gCpbQdrDebugPortRead
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function reads data from the QDR RAM using the
**                  QDR Debug Port.  Ensure QDR PHY is configured before
**                  running this test by calling hyPhy20gCpbInit with
**                  ramType > 0 and enable = 1.
**
**                  Data read back is protected by ECC and a maximum of one
**                  bit error may be corrected.
**
**  INPUTS:         *fileHndl   - device handle
**                  rdAddr      - QDR RAM read address
**                  *rdData[12] - data read from RAM
**                                   rdPatt[0] = QDR_DEBUG_RD_DATA[31:0]
**                                   ...
**                                   rdPatt[11] = QDR_DEBUG_RD_DATA[383:352]
**
**  OUTPUTS:        None
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbQdrDebugPortRead(struct file *fileHndl, UINT4 rdAddr,
                                 UINT4 *rdData)
{
    UINT4 addr, mask, val;
    UINT4 i;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);
	
    /* Wait for PHY_READY bit to get asserted */
    addr = HYPHY20G_QDR_PHY_TOP_REG_QDR_PHY_CFG;
    result = sysHyPhy20gPollBit(fileHndl, addr,
                             HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_PHY_READY_MSK,
                             HYPHY20G_QDR_PHY_TOP_BIT_QDR_PHY_CFG_PHY_READY_MSK,
                             HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Enable debug port */
    addr = HYPHY20G_CPB_CPB_REG_QDR_RAM_DBG_CTL;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_CTL_QDR_DEBUG_EN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, 1);
    if(result) return result;

    /* Write control register */
    addr = HYPHY20G_CPB_CPB_REG_QDR_RAM_DBG_CTL;
    result = sysHyPhy20gRead(fileHndl, addr, &val);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_CTL_QDR_DEBUG_RWB_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, 1);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_CTL_QDR_DEBUG_ADDR_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, rdAddr);
    if(result) return result;

    mask = HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_CTL_QDR_DEBUG_ACCESS_MSK;
    result = hyPhy20gVarFieldWrite(&val, mask, 1);
    if(result) return result;

    result = sysHyPhy20gWrite(fileHndl, addr, val);
    if(result) return result;

    /* Poll for QDR_DEBUG_ACCESS to clear */
    addr = HYPHY20G_CPB_CPB_REG_QDR_RAM_DBG_CTL;
    result = sysHyPhy20gPollBit(fileHndl, addr,
                      HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_CTL_QDR_DEBUG_ACCESS_MSK,
                      0, HYPHY20G_DELAY_20USEC);
    if(result) return result;

    /* Read data from registers */
    mask = HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_RD_DAT_QDR_DEBUG_RD_DATA_MSK;
    for (i = 0; i < 12; i++) {
        addr = HYPHY20G_CPB_CPB_REG_QDR_RAM_DBG_RD_DAT_OFFSET(i);
        result = hyPhy20gRegFieldRead(fileHndl, addr, mask, rdData);
        if(result) return result;
        rdData++;
    }

    /* Disable debug port */
    addr = HYPHY20G_CPB_CPB_REG_QDR_RAM_DBG_CTL;
    mask = HYPHY20G_CPB_CPB_BIT_QDR_RAM_DBG_CTL_QDR_DEBUG_EN_MSK;
    result = hyPhy20gRegFieldWrite(fileHndl, addr, mask, 0);
    if(result) return result;

    return HYPHY20G_SUCCESS;

} /* hyPhy20gCpbQdrDebugPortRead */


/*******************************************************************************
**
**  hyPhy20gCpbTrigDropCnt
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function triggers the transfer of CPB Drop counters for
**                 all channels to holding registers.  Once transferred counters
**                 for each channel must be accessed with 
**                 hyPhy20gCpbGetChnlDropCnt prior to calling 
**                 hyPhy20gCpbTrigDropCnt again, otherwise drop event counts
**                 may be lost.
**
**  INPUTS:       *fileHndl     - base address
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_POLL_TIMEOUT
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbTrigDropCnt(struct file *fileHndl)
{
    UINT4 addrReg, lclkMsk, lclkDoneMsk;
    INT4 result;
    
    /* determine registers and masks associated with counter */
    addrReg  = HYPHY20G_CPB_CPB_REG_CNT_UDA;
    lclkMsk  = HYPHY20G_CPB_CPB_BIT_CNT_UDA_LCLK_MSK;    
    lclkDoneMsk = HYPHY20G_CPB_CPB_BIT_CNT_UDA_LCLK_DONE_MSK;
    
    /* Trigger Access */
    result = hyPhy20gRegFieldWrite(fileHndl, addrReg, lclkMsk, 0);
    if (result) return result;
    result = hyPhy20gRegFieldWrite(fileHndl, addrReg, lclkMsk, 1);
    if (result) return result;
    
    /* Poll LCLK_DONE bit to ensure channel counters can be read */
    result = sysHyPhy20gPollBit(fileHndl, addrReg, lclkDoneMsk, lclkDoneMsk, 1);
    if (result) return result;    

    return HYPHY20G_SUCCESS;
} /* hyPhy20gCpbTrigDropCnt */

/*******************************************************************************
**
**  hyPhy20gCpbGetChnlDropCnt
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function retrieves the CPB channel drop count for one
**                 channel.  In order for counters to be transferred to per
**                 channel holding registers hyPhy20gCpbTrigDropCnt must be
**                 called.
**
**  INPUTS:       *fileHndl     - base address
**                 chnl         - CPB Channel Drop Count
**                                0 to 15 - LSCI write port 0 to 15
**                                16 to 17 - HSCI write port 0 to 1
**                                64 to 127 - GFP A write port 0 to 63
**                                128 to 191 - GFP B write port 0 to 63
**                *pCnt         - CPB drop count retrieved for chnl
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_NULL_PTR_PASSED
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gCpbGetChnlDropCnt(struct file *fileHndl, UINT4 chnl, UINT4 *pCnt)
{
    UINT4 addrReg, cntMsk;
    UINT4 valReg, valMsk;
    UINT4 val;
    INT4 result;
    
    /* argument checking */
    if ((chnl > 17 && chnl < 64) || chnl > 191) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    if (pCnt == NULL) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_NULL_PTR_PASSED\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_NULL_PTR_PASSED;
    }
    
    /* determine registers and masks associated with counter */
    addrReg = HYPHY20G_CPB_CPB_REG_CNT_UDA;
    cntMsk  = HYPHY20G_CPB_CPB_BIT_CNT_UDA_CNTR_RADDR_MSK;
    valReg  = HYPHY20G_CPB_CPB_REG_CNT_VAL;
    valMsk  = HYPHY20G_CPB_CPB_BIT_CNT_VAL_CNTR_DOUT_MSK;
    
    /* set CNTR_RADDR to specify the channel whose count will be reported */
    result = hyPhy20gRegFieldWrite(fileHndl, addrReg, cntMsk, chnl);
    if (result) return result;
    
    /* Wait at least 20ns */
    result = sysHyPhy20gUsDelay(1);
    if(result) return result;    
    
    /* Retrieve Counter Value */
    result = hyPhy20gRegFieldRead(fileHndl, valReg, valMsk, &val);
    if (result) return result;
    *pCnt = val;

    return HYPHY20G_SUCCESS;
} /* hyPhy20gCpbGetChnlDropCnt */

/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

/* end of file */
