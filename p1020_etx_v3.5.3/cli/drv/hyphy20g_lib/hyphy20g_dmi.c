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
**  FILE        : hyphy20g_dmi.c
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
#include "hyphy20g_dmi.h"

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
**  hyPhy20gDmiInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function resets & initializes one or more DMI blocks
**                 using the device top level reset & low power DMI bits.
**
**                 Each port within a DMI block should then be initialized
**                 using hyPhy20gDmiPortInit.
**
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - DMI block instance
**                               - 0 to 4 - DMI slice 0 - 4
**                               - 5 - All 5 DMI slices
**                 enable        - Controls initialization of the DMI block
**                               - 0 - Block Disabled - low power mode
**                               - 1 - Block Enabled
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gDmiInit(struct file *fileHndl, UINT4 slice, UINT4 enable)
{
  /* variable declaration */
    UINT4 regAddr;
    UINT4 mask, offset;
    UINT4 i;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 5)||(enable > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Apply resets */
    if (slice == 5) {
        /* toggle top level reset for all 5 DMI slices */
        regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
        mask = 0x1F << HYPHY20G_TOP_BIT_SYS_SW_RST_1_DMI_RST_0_OFF;
        result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x1F);
        if (result) return result;
        if (enable) {
            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x0);
            if (result) return result;
            /* Configure block level reset for all 5 DMI slices */
            for (i=0; i<slice; i++) {
                regAddr = HYPHY20G_DMI_DMI_REG_SFT_RST;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
                if (result) return result;
                    
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
                if (result) return result;
            }
        }

        /* Apply Low Power settings */
        regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
        mask = 0x1F << HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_DMI_LOWPWR_0_OFF;
        if (enable) {
            /* remove low power bit for all 5 DMI slices */
            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x0);
            if (result) return result;
        } else {
            /* set low power bit for all 5 DMI slices */
            result = hyPhy20gRegFieldWrite(fileHndl, regAddr, mask, 0x1F);
            if (result) return result;
        }
    } else {
        /* configure top level reset for 1 DMI slice */
        regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
        offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_DMI_RST_0_OFF + slice;
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
        if (result) return result;
        if (enable) {
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
            if (result) return result;
            /* configure block level reset for 1 DMI slice */
            regAddr = HYPHY20G_DMI_DMI_REG_SFT_RST;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
            if (result) return result;
                
            result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
            if (result) return result;
        }

        /* Apply Low Power settings */
        regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
        offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_DMI_LOWPWR_0_OFF + slice;
        if (enable) {
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 0);
            if (result) return result;
        } else {
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, offset, 1);
            if (result) return result;
        }
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gDmiInit */

/*******************************************************************************
**
**  hyPhy20gDmiPortInit
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function resets & initializes one or more 2.5 G DMI
**                 processing resources within a DMI block and
**                 configures the major data path within the DMI processing
**                 resource(s) to either bypass the DMI function or be processed
**                 by the DMI function.
**
**                 When configured for the 'DMI Bypassed' mode, HyPHY 20G is
**                 relying on all J0 signals being received across ESSI
**                 interface to be aligned to the local J0 of the TSE in order
**                 to properly switch STS-N signals.  It is recommended to use
**                 'DMI Enabled' mode.
**
**                 'DMI Enabled' mode enables the DMI processing of the DMI
**                 block.  This will ensure that all J0 signals from the ESSI
**                 interface are realigned to the J0 reference of the TSE.  This
**                 mode will also configure the datapath for manual payload
**                 configuration, with all timeslots set to master mode.
**
**
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - DMI block instance
**                               - 0 to 4 - DMI slice 0 - 4
**                 port        - 2.5 G DMI processing port
**                               - 0 to 3 - port 0 - 3
**                               - 4 - All 4 ports
**                 mode        - Controls initialization of the DMI block
**                               - 0 - Disabled - low power mode
**                               - 1 - DMI Enabled in bypass mode
**                               - 2 - DMI Enabled
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gDmiPortInit(struct file *fileHndl, UINT4 slice, UINT4 port,
                         UINT4 mode)
{
  /* variable declaration */
    UINT4 rstCfg;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;
    UINT4 i, j;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 4)||(port > 4)||(mode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_DMI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_DMI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Apply resets */
    regAddr = HYPHY20G_DMI_DMI_REG_SFT_RST;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    if (port == 4) {
    /* reset all 4 ports within the slice */
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
        if (result) return result;
        result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0000);
        if (result) return result;

    } else {
        /* reset 1 port within the slice */
        result = sysHyPhy20gRead(fileHndl, regAddr, &rstCfg);
        if (result) return result;

        mask = 0x1<<(HYPHY20G_DMI_DMI_BIT_SFT_RST_DTOHP_RST_OFF + port);
        result = hyPhy20gVarFieldWrite(&rstCfg, mask, 1);
        if (result) return result;

        mask = 0x1<<(HYPHY20G_DMI_DMI_BIT_SFT_RST_ATOHP_RST_OFF + port);
        result = hyPhy20gVarFieldWrite(&rstCfg, mask, 1);
        if (result) return result;

        mask = 0x1<<(HYPHY20G_DMI_DMI_BIT_SFT_RST_STFA_RST_OFF + port);
        result = hyPhy20gVarFieldWrite(&rstCfg, mask, 1);
        if (result) return result;

        mask = 0x1<<(HYPHY20G_DMI_DMI_BIT_SFT_RST_SHPI_RST_OFF + port);
        result = hyPhy20gVarFieldWrite(&rstCfg, mask, 1);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, rstCfg);
        if (result) return result;

        result = sysHyPhy20gRead(fileHndl, regAddr, &rstCfg);
        if (result) return result;

        mask = 0x1<<(HYPHY20G_DMI_DMI_BIT_SFT_RST_DTOHP_RST_OFF + port);
        result = hyPhy20gVarFieldWrite(&rstCfg, mask, 0);
        if (result) return result;

        mask = 0x1<<(HYPHY20G_DMI_DMI_BIT_SFT_RST_ATOHP_RST_OFF + port);
        result = hyPhy20gVarFieldWrite(&rstCfg, mask, 0);
        if (result) return result;

        mask = 0x1<<(HYPHY20G_DMI_DMI_BIT_SFT_RST_STFA_RST_OFF + port);
        result = hyPhy20gVarFieldWrite(&rstCfg, mask, 0);
        if (result) return result;

        mask = 0x1<<(HYPHY20G_DMI_DMI_BIT_SFT_RST_SHPI_RST_OFF + port);
        result = hyPhy20gVarFieldWrite(&rstCfg, mask, 0);
        if (result) return result;

        result = sysHyPhy20gWrite(fileHndl, regAddr, rstCfg);
        if (result) return result;
    }

    switch (mode) {
      case 0:
          /* DMI slice disabled.  Set Low Power bit high*/
            regAddr = HYPHY20G_DMI_DMI_REG_LW_PWR_LNK_1_4;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            if (port == 4) {
            /* disable all 4 ports within the slice */
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0xF);
                if (result) return result;
            } else {
                /* disable 1 port within the slice */
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, port, 1);
                if (result) return result;
            }
            break;
      case 1:
        /* DMI slice Enabled with bypass.  Set Low Power bit low
           and bypass bit high */
            if (port == 4) {
                /* remove lowpwr on all 4 ports within the slice */
                regAddr = HYPHY20G_DMI_DMI_REG_LW_PWR_LNK_1_4;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
                if (result) return result;

                /* set bypass bit on all 4 ports within the slice */
                regAddr = HYPHY20G_DMI_DMI_REG_LNK_BYP;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0xF);
                if (result) return result;
            } else {
                /* remove lowpwr on 1 port within the slice */
                regAddr = HYPHY20G_DMI_DMI_REG_LW_PWR_LNK_1_4;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, port, 0);
                if (result) return result;

                /* set bypass bit on 1 port within the slice */
                regAddr = HYPHY20G_DMI_DMI_REG_LNK_BYP;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, port, 1);
                if (result) return result;
            }
            break;
      case 2:
        /* DMI slice Enabled. Set Low Power bit low and bypass bit low */
            if (port == 4) {
                /* remove lowpwr on all 4 ports within the slice */
                regAddr = HYPHY20G_DMI_DMI_REG_LW_PWR_LNK_1_4;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
                if (result) return result;

                /* disable bypass bit on all 4 ports within the slice */
                regAddr = HYPHY20G_DMI_DMI_REG_LNK_BYP;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
                if (result) return result;

                /* Configure manual payload detection for all 4 ports */
                for (j=0; j<12; j++) {
                    regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
                    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                    regAddr = regAddr + 4*j;
                    result =  sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
                    if (result) return result;
                }
                /* Set all timeslots to master for all 4 ports */
                for (i=0; i<3; i++) {
                    for (j=0; j<4; j++) {
                        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_1;
                        regAddr = regAddr + 4*i;
                        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                        regAddr = HYPHY20G_DMI_SHPI_TSB_SLICE_REG_OFFSET(regAddr, j);
                        result =  sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
                        if (result) return result;
                    }
                }

            } else {
                /* remove lowpwr on 1 port within the slice */
                regAddr = HYPHY20G_DMI_DMI_REG_LW_PWR_LNK_1_4;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, port, 0);
                if (result) return result;

                /* disable bypass bit on 1 port within the slice */
                regAddr = HYPHY20G_DMI_DMI_REG_LNK_BYP;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, port, 0);
                if (result) return result;

                /* Configure manual payload detection for 1 port */
                for (j=0; j<3; j++) {
                    regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
                    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                    regAddr = regAddr + 0xC*port;
                    regAddr = regAddr + 4*j;
                    result =  sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
                    if (result) return result;
                }
                /* Set all timeslots to master for 1 port */
                for (i=0; i<3; i++) {
                    regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_1;
                    regAddr = regAddr + 4*i;
                    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                    regAddr = HYPHY20G_DMI_SHPI_TSB_SLICE_REG_OFFSET(regAddr, port);
                    result =  sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
                    if (result) return result;
                }

            }
            break;
        }
    return HYPHY20G_SUCCESS;
} /* hyPhy20gDmiPortInit */

/*******************************************************************************
**
**  hyPhy20gDmiPldFilterModeCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:  This function configures the PCD_CONFIG and PLD_SELECT
**                block fields for a 2.5 G DMI processing resources within a DMI
**                block.  These fields are used for payload detection and
**                filtering when automatic payload configuration is enabled.
**
**                For a group of four DMI ports working to process an
**                STS-192 signal using automatic payload detection, this
**                function should be called for all four ports.
**
**                Following this procedure, functions
**                hyPhy20gDmiSts192TslotCfg, hyPhy20gDmiSts48TslotCfg
**                hyPhy20gDmiSts12TslotCfg, and hyPhy20gDmiSts3TslotCfg
**                can be called to configure specific signals for automatic
**                payload configuration.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - DMI block instance
**                               - 0 to 4 - DMI slice 0 - 4
**                 port        - 2.5 G DMI processing port
**                               - 0 to 3 - port 0 - 3
**                 pcdCfg      - Controls behavior of the payload configuration
**                               detection state machine and the justification
**                               counter
**                               0 - Algorithm 1
**                               1 - Algorithm 2 with 3xEQ_PTR
**                               2 - Algorithm 2 with 3xOFFSET_IN_RANGE
**                 pldSel      - Controls payload filtering options
**                               0 - Filtering Disabled
**                               1 - PMC Payload Filtering Mode
**                               2 - ITU Payload Filtering Mode
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gDmiPldFilterModeCfg(struct file *fileHndl, UINT4 slice,
                                 UINT4 port, UINT4 pcdCfg, UINT4 pldSel)
{
  INT4 result;
  UINT4 regAddr;
  UINT4 topLvlRst;
  UINT4 topLvlPwr;
  UINT4 sliceLvlRst;
  UINT4 sliceLvlLwPwr;
  UINT4 sftRst;
  UINT4 offset;
  UINT4 mask;
  UINT4 pcdCfgData = 0;
  UINT4 pldSelData = 0;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 4)||(port > 3)||(pcdCfg > 2)||(pldSel > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_DMI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_DMI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bit(s) */
    regAddr = HYPHY20G_DMI_DMI_REG_SFT_RST;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &sliceLvlRst);
    if (result) return result;

    mask = 0x1111 << port;
    offset = port;
    sftRst = mHYPHY20G_GET_FIELD4(sliceLvlRst, mask, offset);
    if (sftRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level low power bit */
    regAddr = HYPHY20G_DMI_DMI_REG_LW_PWR_LNK_1_4;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr, port, &sliceLvlLwPwr);
    if (result) return result;
    if (sliceLvlLwPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Configure PCD_CONFIG field */
    regAddr = HYPHY20G_DMI_SHPI_REG_CFG;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
    mask = HYPHY20G_DMI_SHPI_BIT_CFG_PCD_CONFIG_MSK;
    if (pcdCfg) {
        result = hyPhy20gVarFieldWrite(&pcdCfgData, mask, pcdCfg+1);
        if (result) return result;
    } else {
        result = hyPhy20gVarFieldWrite(&pcdCfgData, mask, pcdCfg);
        if (result) return result;
    }
    result = sysHyPhy20gWrite(fileHndl, regAddr, pcdCfgData);
    if (result) return result;

    /* Configure PLD_SELECT fields */
    regAddr = HYPHY20G_DMI_DMI_REG_PYLD_FLT_SEL;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);

    result = sysHyPhy20gRead(fileHndl, regAddr, &pldSelData);
    if (result) return result;

    mask = 0x0003 << 2*port;
    result = hyPhy20gVarFieldWrite(&pldSelData, mask, pldSel);
    if (result) return result;

    result = sysHyPhy20gWrite(fileHndl, regAddr, pldSelData);
    if (result) return result;

    return HYPHY20G_SUCCESS;


} /* hyPhy20gDmiPldFilterModeCfg */

/*******************************************************************************
**
**  hyPhy20gDmiPcdPaisDetCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:    This function configures PAIS detector block associated with 
**                  the SHPI inside DMI subsystem.
**                  
**                  When PAIS Detector is enabled and the incoming H1 and H2 
**                  pointers are found to be in AIS, the PCD output of the SHPI
**                  on the time slot interpreted as in AIS will be
**                  forced to MASTER, irrespective of its previous state.
**
**                  The PAIS Detector processes the payload configuration after 
**                  automatic payload detection but prior to automatic payload 
**                  configuration filtering. 
**
**                  Payload Detection Algorithm selection is performed with
**                  hyPhy20gDmiPldFilterModeCfg.
**                  
**                  This API should be called following 
**                  hyPhy20gDmiPldFilterModeCfg to ensure the payload
**                  configuration detection state machine and the justification
**                  counter have been set up properly.
**
**
**  INPUTS:         *fileHndl   - base address
**                  slice       - DMI block instance
**                                0 to 4
**                  port        - 2.5 G DMI processing port
**                                0 to 3 
**                  pAisDetEnbl - processing mode for PAIS Detector block
**                                0 - disable PAIS detector
**                                1 - enable PAIS detector
**                  aisFCnt     - number of consecutive frames in which H1 
**                                and H2 pointers indicate a AIS pattern (0xFF)
**                                after which the payload configuration on a 
**                                time slot is forced to a MASTER state
**                                Legal range: 0x1 - 0x7 
**                                Default value = 0x3
**                  normFCnt    - number of consecutive frames in which H1 and 
**                                H2 pointers indicate a non-AIS pattern 
**                                (pattern other than 0xFF) before the force on 
**                                the payload configuration of a time slot is 
**                                released from the MASTER state 
**                                Legal range: 0x3 - 0xF
**                                Default value = 0xB
**                                Note: When Payload Detection Algorithm-1 is in
**                                      use, normFCnt must be at least 0xB.
**                                      When Payload Detection Algorithm-2 is in
**                                      use, normFCnt must be at least 0x3.
**                
**
**  OUTPUTS:        None.
**
**  RETURNS:        HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gDmiPcdPaisDetCfg(struct file *fileHndl, UINT4 slice, UINT4 port,
                              UINT4 pAisDetEnbl, UINT4 aisFCnt, UINT4 normFCnt)
{    
    /* Variable declaration */
    UINT4 addr, msk;
    UINT4 aisCntMsk = 0, normCntMsk = 0;
    UINT4 regData; 
    INT4 result;
    
    /* Argument checking */
    if (slice > 4 || port > 3 || pAisDetEnbl > 1 || (aisFCnt > 7 || aisFCnt < 1) 
        || normFCnt > 0xF )  {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
    
    /* Check to confirm user supplied normFCnt is valid for the current 
       PCD configuration */
    addr = HYPHY20G_DMI_SHPI_REG_CFG; 
    addr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(addr, slice);
    addr = HYPHY20G_DMI_SHPI_TSB_SLICE_REG_OFFSET(addr, port);
    msk = HYPHY20G_DMI_SHPI_BIT_CFG_PCD_CONFIG_MSK;
    
    result = hyPhy20gRegFieldRead(fileHndl, addr, msk, &regData);
    if (result) return result;
    
    DBG_PRINT("%s, %s, %d, Algorithm is %u\n",__FILE__, __FUNCTION__, __LINE__, (unsigned int)regData);
    
    /* When Algorithm 1 is in use, NORM_FCNT must be at least 0xB 
       When Algorithm 2 is in use, NORM_FCNT must be at least 0x3 */    
    if ((regData == 0x0 && normFCnt < 0xB) || 
        ((regData == 0x2 || regData == 0x3) && normFCnt < 0x3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);
        return HYPHY20G_ERR_INVALID_ARG;
    }
        
    /* PAIS DETECTOR CONFIGURATION register */ 
    addr = HYPHY20G_DMI_DMI_REG_PAIS_DET_CFG;
    addr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(addr, slice);
   
    /* Determine mask values for each slice */ 
    switch (port) {
        case 0:
            msk = HYPHY20G_DMI_DMI_BIT_PAIS_DET_CFG_PCD_PAIS_DET_EN_0_MSK;
            aisCntMsk = HYPHY20G_DMI_DMI_BIT_PAIS_DET_CFG_PCD_TO_AIS_FCNT_0_MSK;
            normCntMsk = HYPHY20G_DMI_DMI_BIT_PAIS_DET_CFG_PCD_TO_NORM_FCNT_0_MSK;
            break;
        case 1:
            msk = HYPHY20G_DMI_DMI_BIT_PAIS_DET_CFG_PCD_PAIS_DET_EN_1_MSK;
            aisCntMsk = HYPHY20G_DMI_DMI_BIT_PAIS_DET_CFG_PCD_TO_AIS_FCNT_1_MSK;
            normCntMsk = HYPHY20G_DMI_DMI_BIT_PAIS_DET_CFG_PCD_TO_NORM_FCNT_1_MSK;
            break;
        case 2: 
            msk = HYPHY20G_DMI_DMI_BIT_PAIS_DET_CFG_PCD_PAIS_DET_EN_2_MSK;
            aisCntMsk = HYPHY20G_DMI_DMI_BIT_PAIS_DET_CFG_PCD_TO_AIS_FCNT_2_MSK;
            normCntMsk = HYPHY20G_DMI_DMI_BIT_PAIS_DET_CFG_PCD_TO_NORM_FCNT_2_MSK;
            break;
        case 3:
            msk = HYPHY20G_DMI_DMI_BIT_PAIS_DET_CFG_PCD_PAIS_DET_EN_3_MSK;
            aisCntMsk = HYPHY20G_DMI_DMI_BIT_PAIS_DET_CFG_PCD_TO_AIS_FCNT_3_MSK;
            normCntMsk = HYPHY20G_DMI_DMI_BIT_PAIS_DET_CFG_PCD_TO_NORM_FCNT_3_MSK;
            break;
    }
    
    result = sysHyPhy20gRead(fileHndl, addr, &regData);
    if (result) return result;
        
    /* Set PCD_PAIS_DET_EN_* */  
    result = hyPhy20gVarFieldWrite(&regData, msk, pAisDetEnbl);
    if (result) return result;
    
    /* Set PCD_TO_AIS_FCNT */
    result = hyPhy20gVarFieldWrite(&regData, aisCntMsk, aisFCnt);
    if (result) return result;
    
    /* Set PCD_TO_NORM_FCNT */
    result = hyPhy20gVarFieldWrite(&regData, normCntMsk, normFCnt);
    if (result) return result;
    
    /* Write to PAIS DETECTOR CONFIGURATION register */
    result = sysHyPhy20gWrite(fileHndl, addr, regData);
    if (result) return result;
            
    return HYPHY20G_SUCCESS; 
} /* hyPhy20gDmiPcdPaisDetCfg */ 


/*******************************************************************************
**
**  hyPhy20gDmiSrcCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function selects the data source for one or more 2.5G
**                 DMI processing resources within one or more DMI blocks
**                 between the ESSI working and protect links associated with
**                 the DMI processing resource(s).
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - DMI block instance
**                               - 0 to 4 - DMI slice 0 - 4
**                               - 5 - All 5 DMI slices
**                 port        - 2.5 G DMI processing port
**                               - 0 to 3 - port 0 - 3
**                               - 4 - All 4 ports
**                 src         - DMI block data source:
**                               0 - Protect
**                               1 - Working
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gDmiSrcCfg(struct file *fileHndl, UINT4 slice, UINT4 port,
                       UINT4 src)
{
  /* variable declaration */
    UINT4 regAddr;
    UINT4 i;
    UINT4 mask;
    UINT4 offset;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 sliceLvlRst;
    UINT4 sftRst;
    UINT4 sliceLvlLwPwr;
    UINT4 dmiRst;
    UINT4 dmiPwr;
    INT4 result;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 5)||(port > 4)||(src > 1)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Preconfig Checking */
    if (slice ==5) {
        /* Check top level reset bit for all 5 slices */
        regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
        result = sysHyPhy20gRead(fileHndl, regAddr, &topLvlRst);
        if (result) return result;

        mask = 0x1F << HYPHY20G_TOP_BIT_SYS_SW_RST_1_DMI_RST_0_OFF;
        offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_DMI_RST_0_OFF;
        dmiRst = mHYPHY20G_GET_FIELD4(topLvlRst, mask, offset);
        if (dmiRst) {
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }

        /* Check top level low power bit for all 5 slices */
        regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
        result = sysHyPhy20gRead(fileHndl, regAddr, &topLvlPwr);
        if (result) return result;

        mask = 0x1F << HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_DMI_LOWPWR_0_OFF;
        offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_DMI_LOWPWR_0_OFF;
        dmiPwr = mHYPHY20G_GET_FIELD4(topLvlPwr, mask, offset);
        if (dmiPwr) {
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }

        /* Check slice level reset bit(s) */
        for (i=0; i<slice; i++) {
            /* Check slice level reset bits for all 5 slices */
            regAddr = HYPHY20G_DMI_DMI_REG_SFT_RST;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gRead(fileHndl, regAddr, &sliceLvlRst);
            if (result) return result;
            if (port == 4) {
                /* Check slice level reset bits for all 4 ports */
                if (sliceLvlRst) {
                    return HYPHY20G_ERR_INVALID_PRECONFIG;
                }
            } else {
                /* Check slice level reset bits for 1 port */
                mask = 0x1111 << port;
                offset = port;
                sftRst = mHYPHY20G_GET_FIELD4(sliceLvlRst, mask, offset);
                if (sftRst) {
                    return HYPHY20G_ERR_INVALID_PRECONFIG;
                }
            }
            /* Check slice level low power bits for all 5 slices */
            regAddr = HYPHY20G_DMI_DMI_REG_LW_PWR_LNK_1_4;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, i);
            result = sysHyPhy20gRead(fileHndl, regAddr, &sliceLvlLwPwr);
            if (result) return result;
            if (port == 4) {
                /* Check slice level reset bits for all 4 ports */
                if (sliceLvlLwPwr) {
                    return HYPHY20G_ERR_INVALID_PRECONFIG;
                }
            } else {
                /* Check slice level reset bits for 1 port */
                result = sysHyPhy20gBitRead(fileHndl, regAddr, port, &sliceLvlLwPwr);
                if (result) return result;
                if (sliceLvlLwPwr) {
                    return HYPHY20G_ERR_INVALID_PRECONFIG;
                }
            }

        }
    } else {
        /* Check top level reset bit for 1 slice */
        regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
        offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_DMI_RST_0_OFF + slice;
        result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
        if (result) return result;
        if (topLvlRst) {
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }

        /* Check top level low power bit */
        regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
        offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_DMI_LOWPWR_0_OFF + slice;
        result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
        if (result) return result;
        if (topLvlPwr) {
            return HYPHY20G_ERR_INVALID_PRECONFIG;
        }

        /* Check slice level reset bit(s) */
        regAddr = HYPHY20G_DMI_DMI_REG_SFT_RST;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gRead(fileHndl, regAddr, &sliceLvlRst);
        if (result) return result;
        if (port == 4) {
             /* Check slice level reset bits for all 4 ports */
            if (sliceLvlRst) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
        } else {
            /* Check slice level reset bits for 1 port */
            mask = 0x1111 << port;
            offset = port;
            sftRst = mHYPHY20G_GET_FIELD4(sliceLvlRst, mask, offset);
            if (sftRst) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
        }
        /* Check slice level low power bits for 1 slice */
        regAddr = HYPHY20G_DMI_DMI_REG_LW_PWR_LNK_1_4;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        result = sysHyPhy20gRead(fileHndl, regAddr, &sliceLvlLwPwr);
        if (result) return result;
        if (port == 4) {
            /* Check slice level reset bits for all 4 ports */
            if (sliceLvlLwPwr) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
        } else {
            /* Check slice level reset bits for 1 port */
            result = sysHyPhy20gBitRead(fileHndl, regAddr, port, &sliceLvlLwPwr);
            if (result) return result;
            if (sliceLvlLwPwr) {
                return HYPHY20G_ERR_INVALID_PRECONFIG;
            }
        }
    }

    /* Source Configuration */
    if (slice == 5) {
        /* Configure all 5 DMI slices */
        for (i=0; i<slice; i++) {
            regAddr = HYPHY20G_DMI_DMI_REG_WK_PROT;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, i);
            if (port == 4) {
            /* configure all 4 ports within each slice */
                if (src) {
                    /* Configure for working */
                    result = sysHyPhy20gWrite(fileHndl, regAddr, 0xF);
                    if (result) return result;
                } else {
                    /* Configure for protect */
                    result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
                    if (result) return result;
                }
            } else {
                /* configure 1 port within each slice */
                result = sysHyPhy20gBitWrite(fileHndl, regAddr, port, src);
                if (result) return result;
             }
        }
    } else {
        /* configure 1 DMI slice */
        regAddr = HYPHY20G_DMI_DMI_REG_WK_PROT;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        if (port == 4) {
        /* configure all 4 ports within the slice */
            if (src) {
                /* Configure for working */
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0xF);
                if (result) return result;
            } else {
                /* Configure for protect */
                result = sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
                if (result) return result;
            }
        } else {
            /* configure 1 port within the slice */
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, port, src);
            if (result) return result;
        }
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gDmiSrcCfg */


/*******************************************************************************
**
**  hyPhy20gDmiSts192TslotCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures the four 2.5G processing
**                 instances of a DMI slice to combine and process an
**                 STS-192 signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - DMI block instance
**                               0 to 4
**                 mode        - mode of incoming signnal
**                               0 - STS-192c signal - manual payload detection
**                               1 - 4 x STS-48c.  Use hyPhy20gDmiSts48TslotCfg
**                                   to further configure the payload format
**                                   of the STS-48 components of the STS-192
**                                   client signal - manual payload detection
**                               2 - 192 x STS1 - manual payload detection
**                               3 - STS-192 - automatic payload detection
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
INT4 hyPhy20gDmiSts192TslotCfg(struct file *fileHndl, UINT4 slice, UINT4 mode)
{
  /* variable declaration */
  INT4 result;
    UINT4 slice0PayLdCfg1 = 0;
    UINT4 slice0PayLdCfg2 = 0;
    UINT4 slice0PayLdCfg3 = 0;
    UINT4 slice123PayLdCfg1 = 0;
    UINT4 slice123PayLdCfg2 = 0;
    UINT4 slice123PayLdCfg3 = 0;
    UINT4 regAddr;
    UINT4 autoDetect = 0;
    UINT4 i, j;
    UINT4 offset;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 sliceLvlRst;
    UINT4 sliceLvlLwPwr;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 4)||(mode > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* Preconfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_DMI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_DMI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bits */
    regAddr = HYPHY20G_DMI_DMI_REG_SFT_RST;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &sliceLvlRst);
    if (result) return result;
    if (sliceLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level low power bits */
    regAddr = HYPHY20G_DMI_DMI_REG_LW_PWR_LNK_1_4;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &sliceLvlLwPwr);
    if (result) return result;
    if (sliceLvlLwPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Create Payload settings */
    switch (mode) {
      case 0:
            slice0PayLdCfg1 = 0x1;
          slice0PayLdCfg2 = 0x0;
          slice0PayLdCfg3 = 0x0;

          slice123PayLdCfg1 = 0x0;
          slice123PayLdCfg2 = 0x0;
          slice123PayLdCfg3 = 0x0;

          autoDetect = 0;
          break;
      case 1:
            slice0PayLdCfg1 = 0x1;
          slice0PayLdCfg2 = 0x0;
          slice0PayLdCfg3 = 0x0;

          slice123PayLdCfg1 = 0x1;
          slice123PayLdCfg2 = 0x0;
          slice123PayLdCfg3 = 0x0;

          autoDetect = 0;
          break;
      case 2:
            slice0PayLdCfg1 = 0xFFFF;
          slice0PayLdCfg2 = 0xFFFF;
          slice0PayLdCfg3 = 0xFFFF;

          slice123PayLdCfg1 = 0xFFFF;
          slice123PayLdCfg2 = 0xFFFF;
          slice123PayLdCfg3 = 0xFFFF;

          autoDetect = 0;
          break;
      case 3:
          autoDetect = 1;
          break;
    }

    if (autoDetect) {
        /* Auto Config Mode */
        /* Configure autoDetect field for all 4 ports */
        for (j=0; j<12; j++) {
            regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            regAddr = regAddr + 4*j;
            if (j==0) {
                result =  sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFE);
                if (result) return result;
            } else {
                result =  sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
                if (result) return result;
            }
        }
    } else {
        /* Manual Config Mode */
        /* Configure autoDetect field for all 4 ports */
        for (j=0; j<12; j++) {
            regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            regAddr = regAddr + 4*j;
            result =  sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
            if (result) return result;
        }

        /*Configure DMI Slice Payload Manual Settings for all 4 ports */
        for (i = 0; i<HYPHY20G_DMI_MAX_PORTS; i++) {
            if (i==0) {
            /* First port config */
                regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_1;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gWrite(fileHndl, regAddr, slice0PayLdCfg1);
                if (result) return result;

                regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_2;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gWrite(fileHndl, regAddr, slice0PayLdCfg2);
                if (result) return result;

                regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS3_STS_1_NO_3;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gWrite(fileHndl, regAddr, slice0PayLdCfg3);
                if (result) return result;

            } else {
            /* Remaining 3 ports config */
                regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_1;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gWrite(fileHndl, regAddr, slice123PayLdCfg1);
                if (result) return result;

                regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_2;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gWrite(fileHndl, regAddr, slice123PayLdCfg2);
                if (result) return result;

                regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS3_STS_1_NO_3;
                regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
                regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, i);
                result = sysHyPhy20gWrite(fileHndl, regAddr, slice123PayLdCfg3);
                if (result) return result;
            }
        }

    }

  return HYPHY20G_SUCCESS;
} /* hyPhy20gDmiSts192TslotCfg */


/*******************************************************************************
**
**  hyPhy20gDmiSts48TslotCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures one of the four 2.5G processing
**                 instances of a DMI slice to process an STS-48-rate signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - DMI block instance
**                               - 0 to 4
**                 port        - 2.5 G DMI processing port
**                               - 0 to 3
**                 mode        - mode of incoming signnal
**                               0 - STS-48c signal - Manual payload detection
**                               1 - 4 x STS-12c - Manual payload detection.
**                                   Use hyPhy20gDmiSts12TslotCfg
**                                   to further configure the payload format
**                                   of the STS-12 components of the STS-48
**                                   client signal.
**                               2 - 48 x STS1 - Manual payload detection
**                               3 - STS-48 - Automatic payload detection
**
**  NOTE:          When this function is called, it will automatically set the
**                 1st STS-1 of the next STS-48 slot to manual master mode.
**
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gDmiSts48TslotCfg(struct file *fileHndl, UINT4 slice, UINT4 port,
                                    UINT4 mode)
{
  /* variable declaration */
  INT4 result;
    UINT4 payLdCfg1 = 0;
    UINT4 payLdCfg2 = 0;
    UINT4 payLdCfg3 = 0;
    UINT4 autoDetect = 0;
    UINT4 j;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 sliceLvlRst;
    UINT4 sftRst;
    UINT4 sliceLvlLwPwr;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 4)||(port > 3)||(mode > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_DMI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_DMI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bit(s) */
    regAddr = HYPHY20G_DMI_DMI_REG_SFT_RST;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &sliceLvlRst);
    if (result) return result;

    mask = 0x1111 << port;
    offset = port;
    sftRst = mHYPHY20G_GET_FIELD4(sliceLvlRst, mask, offset);
    if (sftRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level low power bit */
    regAddr = HYPHY20G_DMI_DMI_REG_LW_PWR_LNK_1_4;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr, port, &sliceLvlLwPwr);
    if (result) return result;
    if (sliceLvlLwPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Create Payload settings */
    switch (mode) {
      case 0:
            payLdCfg1 = 0x1;
          payLdCfg2 = 0x0;
          payLdCfg3 = 0x0;
          break;
      case 1:
            payLdCfg1 = 0x1111;
          payLdCfg2 = 0x0;
          payLdCfg3 = 0x0;
          break;
      case 2:
            payLdCfg1 = 0xFFFF;
          payLdCfg2 = 0xFFFF;
          payLdCfg3 = 0xFFFF;
          break;
      case 3:
          autoDetect = 1;
          break;
    }

    if (autoDetect) {
        /* Auto Config Mode */
        /* Configure autoDetect field for 1 port */
        for (j=0; j<3; j++) {
            regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            regAddr = regAddr + 0xC*port;
            regAddr = regAddr + 4*j;
            if (j==0) {
                result =  sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFE);
                if (result) return result;
            } else {
                result =  sysHyPhy20gWrite(fileHndl, regAddr, 0xFFFF);
                if (result) return result;
            }
        }
    } else {
        /* Manual Config Mode */
        /* Configure autoDetect field for 1 port */
        for (j=0; j<3; j++) {
            regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            regAddr = regAddr + 0xC*port;
            regAddr = regAddr + 4*j;
            result =  sysHyPhy20gWrite(fileHndl, regAddr, 0x0);
            if (result) return result;
        }

        /*Configure DMI Slice Payload Manual Settings for 1 port */
        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_1;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
        result = sysHyPhy20gWrite(fileHndl, regAddr, payLdCfg1);
        if (result) return result;

        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_2;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
        result = sysHyPhy20gWrite(fileHndl, regAddr, payLdCfg2);
        if (result) return result;

        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS3_STS_1_NO_3;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
        result = sysHyPhy20gWrite(fileHndl, regAddr, payLdCfg3);
        if (result) return result;
    }

    /* Configure First timeslot of next DMI port for manual master mode */
    if (port < 3) {
        /* Set to Manual Mode */
        regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = regAddr + 0xC*(port+1);
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0, 0);
        if (result) return result;

        /* Set to Master Mode */
        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_1;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port+1);
        result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0, 1);
        if (result) return result;
    }

  return HYPHY20G_SUCCESS;
} /* hyPhy20gDmiSts48TslotCfg */


/*******************************************************************************
**
**  hyPhy20gDmiSts24TslotCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures a portion of one of the four
**                 2.5G processing instances of a DMI slice to process an
**                 STS-24-rate signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - DMI block instance
**                               - 0 to 4
**                 port        - 2.5 G DMI processing port
**                               - 0 to 3
**                 sts24Num    - STS24 number within this STS-48 port
**                             - valid values 0-1
**                 mode        - mode of incoming signnal
**                               0 - STS-24c signal - Manual payload Detection
**                               1 - 2 x STS-12c - Manual payload detection.
**                                   Use hyPhy20gDmiSts12TslotCfg
**                                   to further configure the payload format
**                                   of the STS-12 components of the STS-24
**                                   client signal.
**                               2 - 24 x STS1 - Manual payload detection.
**                               3 - STS-24 - Automatic Payload Detection.
**
**  NOTE:          When this function is called, it will automatically set the
**                 1st STS-1 of the next STS-24 slot to manual master mode.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gDmiSts24TslotCfg(struct file *fileHndl, UINT4 slice, UINT4 port,
                                        UINT4 sts24Num, UINT4 mode)
{
  /* variable declaration */
  INT4 result;
    UINT4 payLdCfg1, payLdCfg2, payLdCfg3;
    UINT4 payLdCfg1Mask = 0;
    UINT4 payLdCfg2Mask = 0;
    UINT4 payLdCfg3Mask = 0;
    UINT4 autoCfgMask1 = 0;
    UINT4 autoCfgMask23 = 0;
    UINT4 autoDetect = 0;
    UINT4 autoCfg;
    UINT4 j;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 sliceLvlRst;
    UINT4 sftRst;
    UINT4 sliceLvlLwPwr;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 4)||(port > 3)||(sts24Num > 1)||(mode > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_DMI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_DMI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bit(s) */
    regAddr = HYPHY20G_DMI_DMI_REG_SFT_RST;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &sliceLvlRst);
    if (result) return result;

    mask = 0x1111 << port;
    offset = port;
    sftRst = mHYPHY20G_GET_FIELD4(sliceLvlRst, mask, offset);
    if (sftRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level low power bit */
    regAddr = HYPHY20G_DMI_DMI_REG_LW_PWR_LNK_1_4;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr, port, &sliceLvlLwPwr);
    if (result) return result;
    if (sliceLvlLwPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Create Payload settings */
    switch (mode) {
      case 0:
          payLdCfg1Mask = 0x01;
          payLdCfg2Mask = 0x00;
          payLdCfg3Mask = 0x00;
          break;
      case 1:
          payLdCfg1Mask = 0x11;
          payLdCfg2Mask = 0x00;
          payLdCfg3Mask = 0x00;
          break;
      case 2:
          payLdCfg1Mask = 0xFF;
          payLdCfg2Mask = 0xFF;
          payLdCfg3Mask = 0xFF;
          break;
      case 3:
          autoCfgMask1 = 0xFE;
          autoCfgMask23 = 0xFF;
          autoDetect = 1;
          break;
    }

    mask = 0xFF << 2*sts24Num;

    if (autoDetect) {
        /* Auto Config Mode */
        /* Configure autoDetect field for 1/2 of 1 port */
        for (j=0; j<3; j++) {
            regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            regAddr = regAddr + 0xC*port;
            regAddr = regAddr + 4*j;
            result = sysHyPhy20gRead(fileHndl, regAddr, &autoCfg);
            if (result) return result;
            if (j==0) {
                result = hyPhy20gVarFieldWrite(&autoCfg, mask, autoCfgMask1);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&autoCfg, mask, autoCfgMask23);
                if (result) return result;
            }
            result = sysHyPhy20gWrite(fileHndl, regAddr, autoCfg);
            if (result) return result;
        }
    } else {
        /* Manual Config Mode */
        /* Configure autoDetect field for 1/2 of 1 port */
        for (j=0; j<3; j++) {
            regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            regAddr = regAddr + 0xC*port;
            regAddr = regAddr + 4*j;
            result = sysHyPhy20gRead(fileHndl, regAddr, &autoCfg);
            if (result) return result;
            result = hyPhy20gVarFieldWrite(&autoCfg, mask, 0x0);
            if (result) return result;
            result = sysHyPhy20gWrite(fileHndl, regAddr, autoCfg);
            if (result) return result;
        }

        /*Configure DMI Slice Payload Manual Settings for 1/2 of 1 port */
        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_1;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
        result = sysHyPhy20gRead(fileHndl, regAddr, &payLdCfg1);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&payLdCfg1, mask, payLdCfg1Mask);
        if (result) return result;
        result = sysHyPhy20gWrite(fileHndl, regAddr, payLdCfg1);
        if (result) return result;

        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_2;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
        result = sysHyPhy20gRead(fileHndl, regAddr, &payLdCfg2);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&payLdCfg2, mask, payLdCfg2Mask);
        if (result) return result;
        result = sysHyPhy20gWrite(fileHndl, regAddr, payLdCfg2);
        if (result) return result;

        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS3_STS_1_NO_3;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
        result = sysHyPhy20gRead(fileHndl, regAddr, &payLdCfg3);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&payLdCfg3, mask, payLdCfg3Mask);
        if (result) return result;
        result = sysHyPhy20gWrite(fileHndl, regAddr, payLdCfg3);
        if (result) return result;
    }

    /* Configure First timeslot of next STS-24 for manual master mode */
    if ((port < 3)||(sts24Num < 1)) {
        /* Set to Manual Mode */
        regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        if (sts24Num == 1) {
            regAddr = regAddr + 0xC*(port+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0, 0);
            if (result) return result;
        } else {
            regAddr = regAddr + 0xC*port;
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 8*(sts24Num+1), 0);
            if (result) return result;
        }

        /* Set to Master Mode */
        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_1;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        if (sts24Num == 1) {
            regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0, 1);
            if (result) return result;
        } else {
            regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 8*(sts24Num+1), 1);
            if (result) return result;
        }
    }

  return HYPHY20G_SUCCESS;
} /* hyPhy20gDmiSts24TslotCfg */

/*******************************************************************************
**
**  hyPhy20gDmiSts12TslotCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures a portion of one of the four
**                 2.5G processing instances of a DMI slice to process an
**                 STS-12-rate signal.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - DMI block instance
**                               - 0 to 4
**                 port        - 2.5 G DMI processing port
**                               - 0 to 3
**                 sts12Num    - STS12 number within this STS-48 port
**                             - valid values 0-3
**                 mode        - mode of incoming signnal
**                               0 - STS-12c signal - Manual payload Detection
**                               1 - 4 x STS-3c - Manual payload detection.
**                                   Use hyPhy20gDmiSts3TslotCfg
**                                   to further configure the payload format
**                                   of the STS-3 components of the STS-12
**                                   client signal.
**                               2 - 12 x STS1 - Manual payload detection.
**                               3 - STS-12 - Automatic Payload Detection.
**
**  NOTE:          When this function is called, it will automatically set the
**                 1st STS-1 of the next STS-12 slot to manual master mode.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gDmiSts12TslotCfg(struct file *fileHndl, UINT4 slice, UINT4 port,
                                        UINT4 sts12Num, UINT4 mode)
{
  /* variable declaration */
  INT4 result;
    UINT4 payLdCfg1, payLdCfg2, payLdCfg3;
    UINT4 payLdCfg1Mask = 0;
    UINT4 payLdCfg2Mask = 0;
    UINT4 payLdCfg3Mask = 0;
    UINT4 autoCfgMask1 = 0;
    UINT4 autoCfgMask23 = 0;
    UINT4 autoDetect = 0;
    UINT4 autoCfg;
    UINT4 j;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 sliceLvlRst;
    UINT4 sftRst;
    UINT4 sliceLvlLwPwr;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 4)||(port > 3)||(sts12Num > 3)||(mode > 3)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_DMI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_DMI_LOWPWR_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bit(s) */
    regAddr = HYPHY20G_DMI_DMI_REG_SFT_RST;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &sliceLvlRst);
    if (result) return result;

    mask = 0x1111 << port;
    offset = port;
    sftRst = mHYPHY20G_GET_FIELD4(sliceLvlRst, mask, offset);
    if (sftRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level low power bit */
    regAddr = HYPHY20G_DMI_DMI_REG_LW_PWR_LNK_1_4;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr, port, &sliceLvlLwPwr);
    if (result) return result;
    if (sliceLvlLwPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Create Payload settings */
    switch (mode) {
      case 0:
            payLdCfg1Mask = 0x1;
          payLdCfg2Mask = 0x0;
          payLdCfg3Mask = 0x0;
          break;
      case 1:
            payLdCfg1Mask = 0xF;
          payLdCfg2Mask = 0x0;
          payLdCfg3Mask = 0x0;
          break;
      case 2:
            payLdCfg1Mask = 0xF;
          payLdCfg2Mask = 0xF;
          payLdCfg3Mask = 0xF;
          break;
      case 3:
          autoCfgMask1 = 0xE;
          autoCfgMask23 = 0xF;
          autoDetect = 1;
          break;
    }

    mask = 0xF << 4*sts12Num;

    if (autoDetect) {
        /* Auto Config Mode */
        /* Configure autoDetect field for 1/4 of 1 port */
        for (j=0; j<3; j++) {
            regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            regAddr = regAddr + 0xC*port;
            regAddr = regAddr + 4*j;
            result = sysHyPhy20gRead(fileHndl, regAddr, &autoCfg);
            if (result) return result;
            if (j==0) {
                result = hyPhy20gVarFieldWrite(&autoCfg, mask, autoCfgMask1);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&autoCfg, mask, autoCfgMask23);
                if (result) return result;
            }
            result = sysHyPhy20gWrite(fileHndl, regAddr, autoCfg);
            if (result) return result;
        }
    } else {
        /* Manual Config Mode */
        /* Configure autoDetect field for 1/4 of 1 port */
        for (j=0; j<3; j++) {
            regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            regAddr = regAddr + 0xC*port;
            regAddr = regAddr + 4*j;
            result = sysHyPhy20gRead(fileHndl, regAddr, &autoCfg);
            if (result) return result;
            result = hyPhy20gVarFieldWrite(&autoCfg, mask, 0x0);
            if (result) return result;
            result = sysHyPhy20gWrite(fileHndl, regAddr, autoCfg);
            if (result) return result;
        }

        /*Configure DMI Slice Payload Manual Settings for 1/4 of 1 port */
        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_1;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
        result = sysHyPhy20gRead(fileHndl, regAddr, &payLdCfg1);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&payLdCfg1, mask, payLdCfg1Mask);
        if (result) return result;
        result = sysHyPhy20gWrite(fileHndl, regAddr, payLdCfg1);
        if (result) return result;

        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_2;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
        result = sysHyPhy20gRead(fileHndl, regAddr, &payLdCfg2);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&payLdCfg2, mask, payLdCfg2Mask);
        if (result) return result;
        result = sysHyPhy20gWrite(fileHndl, regAddr, payLdCfg2);
        if (result) return result;

        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS3_STS_1_NO_3;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
        result = sysHyPhy20gRead(fileHndl, regAddr, &payLdCfg3);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&payLdCfg3, mask, payLdCfg3Mask);
        if (result) return result;
        result = sysHyPhy20gWrite(fileHndl, regAddr, payLdCfg3);
        if (result) return result;
    }

    /* Configure First timeslot of next STS-12 for manual master mode */
    if ((port < 3)||(sts12Num < 3)) {
        /* Set to Manual Mode */
        regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        if (sts12Num == 3) {
            regAddr = regAddr + 0xC*(port+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0, 0);
            if (result) return result;
        } else {
            regAddr = regAddr + 0xC*port;
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 4*(sts12Num+1), 0);
            if (result) return result;
        }

        /* Set to Master Mode */
        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_1;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        if (sts12Num == 3) {
            regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0, 1);
            if (result) return result;
        } else {
            regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 4*(sts12Num+1), 1);
            if (result) return result;
        }
    }

  return HYPHY20G_SUCCESS;
} /* hyPhy20gDmiSts12TslotCfg */



/*******************************************************************************
**
**  hyPhy20gDmiSts3TslotCfg
**  ___________________________________________________________________________
**
**  DESCRIPTION:   This function configures a portion of one of the four
**                 2.5G processing instances of a DMI slice to process an
**                 STS-3-rate.
**
**  INPUTS:        *fileHndl   - base address
**                 slice       - DMI block instance
**                               - 0 to 4
**                 port        - 2.5 G DMI processing port
**                               - 0 to 3
**                 sts12Num    - STS12 channel within this 2.5G DMI port
**                             - valid values 0-3
**                 sts3Num     - STS3 channel within sts12Num
**                             - valid values 0-3
**                 mode        - 0 - 3 x STS-1 signal - Manual payload detection
**                             - 1 - STS-3c signal - Manual payload detection.
**                             - 2 - STS-3 - Automatic payload detection.
**
**  NOTE:          When this function is called, it will automatically set the
**                 1st STS-1 of the next STS-3 slot to manual master mode.
**
**  OUTPUTS:       None.
**
**  RETURNS:        HYPHY20G_ERR_INCOMPATIBLE_DEVICE
**                  HYPHY20G_ERR_INVALID_ARG
**                  HYPHY20G_ERR_DEV_ACCESS
**                  HYPHY20G_ERR_INVALID_PRECONFIG
**                  HYPHY20G_SUCCESS
**
*******************************************************************************/
INT4 hyPhy20gDmiSts3TslotCfg(struct file *fileHndl, UINT4 slice, UINT4 port,
                             UINT4 sts12Num, UINT4 sts3Num, UINT4 mode)
{
  /* variable declaration */
  INT4 result;
    UINT4 payLdCfg1Mask = 0;
    UINT4 payLdCfg2Mask = 0;
    UINT4 payLdCfg3Mask = 0;
    UINT4 autoCfgMask1 = 0;
    UINT4 autoCfgMask23 = 0;
    UINT4 payLdcfg1, payLdcfg2, payLdcfg3;
    UINT4 autoCfg;
    UINT4 autoDetect = 0;
    UINT4 j;
    UINT4 regAddr;
    UINT4 mask;
    UINT4 offset;
    UINT4 sts3Off;
    UINT4 topLvlRst;
    UINT4 topLvlPwr;
    UINT4 sliceLvlRst;
    UINT4 sftRst;
    UINT4 sliceLvlLwPwr;
    UINT4 devId, revId, status;
	
    /* Return error if API called against META device */
    result = hyPhy20gCheckRealRev(fileHndl, &devId, &revId, &status);
    if(result) return result;

    if (devId == HYPHY20G_META20G_DEVICE)
        return(HYPHY20G_ERR_INCOMPATIBLE_DEVICE);

    /* argument checking */
    if ((slice > 4)||(port > 3)||(sts12Num > 3)||(sts3Num > 3)||(mode > 2)) {
        DBG_PRINT("%s, %s, %d, HYPHY20G_ERR_INVALID_ARG\n",__FILE__, __FUNCTION__, __LINE__);return HYPHY20G_ERR_INVALID_ARG;
    }

    /* PreConfig Checking */
    /* Check top level reset bit */
    regAddr = HYPHY20G_TOP_REG_SYS_SW_RST_1;
    offset = HYPHY20G_TOP_BIT_SYS_SW_RST_1_DMI_RST_0_OFF + slice;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlRst);
    if (result) return result;
    if (topLvlRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check top level low power bit */
    regAddr = HYPHY20G_TOP_REG_SYS_LOW_PWR_1;
    offset = HYPHY20G_TOP_BIT_SYS_LOW_PWR_1_DMI_LOWPWR_0_OFF;
    result = sysHyPhy20gBitRead(fileHndl, regAddr, offset, &topLvlPwr);
    if (result) return result;
    if (topLvlPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level reset bit(s) */
    regAddr = HYPHY20G_DMI_DMI_REG_SFT_RST;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gRead(fileHndl, regAddr, &sliceLvlRst);
    if (result) return result;

    mask = 0x1111 << port;
    offset = port;
    sftRst = mHYPHY20G_GET_FIELD4(sliceLvlRst, mask, offset);
    if (sftRst) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Check slice level low power bit */
    regAddr = HYPHY20G_DMI_DMI_REG_LW_PWR_LNK_1_4;
    regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
    result = sysHyPhy20gBitRead(fileHndl, regAddr, port, &sliceLvlLwPwr);
    if (result) return result;
    if (sliceLvlLwPwr) {
        return HYPHY20G_ERR_INVALID_PRECONFIG;
    }

    /* Create Payload settings */
    switch (mode) {
      case 0:
          payLdCfg1Mask = 0x1;
          payLdCfg2Mask = 0x1;
          payLdCfg3Mask = 0x1;
          break;
      case 1:
          payLdCfg1Mask = 0x1;
          payLdCfg2Mask = 0x0;
          payLdCfg3Mask = 0x0;
          break;
      case 2:
          autoCfgMask1 = 0x0;
          autoCfgMask23 = 0x1;
          autoDetect = 1;
          break;
    }

    sts3Off = 4*sts12Num + sts3Num;
    mask = 0x1 << sts3Off;

    if (autoDetect) {
        /* Auto Config Mode */
        /* Configure autoDetect field for 1/16 of 1 port */
        for (j=0; j<3; j++) {
            regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            regAddr = regAddr + 0xC*port;
            regAddr = regAddr + 4*j;
            result = sysHyPhy20gRead(fileHndl, regAddr, &autoCfg);
            if (result) return result;
            if (j==0) {
                result = hyPhy20gVarFieldWrite(&autoCfg, mask, autoCfgMask1);
                if (result) return result;
            } else {
                result = hyPhy20gVarFieldWrite(&autoCfg, mask, autoCfgMask23);
                if (result) return result;
            }
            result = sysHyPhy20gWrite(fileHndl, regAddr, autoCfg);
            if (result) return result;
        }
    } else {
        /* Manual Config Mode */
        /* Configure autoDetect field for 1/16 of 1 port */
        for (j=0; j<3; j++) {
            regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
            regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
            regAddr = regAddr + 0xC*port;
            regAddr = regAddr + 4*j;
            result = sysHyPhy20gRead(fileHndl, regAddr, &autoCfg);
            if (result) return result;
            result = hyPhy20gVarFieldWrite(&autoCfg, mask, 0x0);
            if (result) return result;
            result = sysHyPhy20gWrite(fileHndl, regAddr, autoCfg);
            if (result) return result;
        }

        /*Configure DMI Slice Payload Manual Settings for 1/16 of 1 port */
        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_1;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
        result = sysHyPhy20gRead(fileHndl, regAddr, &payLdcfg1);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&payLdcfg1, mask, payLdCfg1Mask);
        if (result) return result;
        result = sysHyPhy20gWrite(fileHndl, regAddr, payLdcfg1);
        if (result) return result;

        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_2;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
        result = sysHyPhy20gRead(fileHndl, regAddr, &payLdcfg2);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&payLdcfg2, mask, payLdCfg2Mask);
        if (result) return result;
        result = sysHyPhy20gWrite(fileHndl, regAddr, payLdcfg2);
        if (result) return result;

        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS3_STS_1_NO_3;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
        result = sysHyPhy20gRead(fileHndl, regAddr, &payLdcfg3);
        if (result) return result;
        result = hyPhy20gVarFieldWrite(&payLdcfg3, mask, payLdCfg3Mask);
        if (result) return result;
        result = sysHyPhy20gWrite(fileHndl, regAddr, payLdcfg3);
        if (result) return result;
    }

    /* Configure First timeslot of next STS-3 for manual master mode */
    if ((port < 3)||(sts3Off < 15)) {
        /* Set to Manual Mode */
        regAddr = HYPHY20G_DMI_DMI_REG_AC_LINK0_STS3_1;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        if (sts3Off == 15) {
            regAddr = regAddr + 0xC*(port+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0, 0);
            if (result) return result;
        } else {
            regAddr = regAddr + 0xC*(port);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, sts3Num+1, 0);
            if (result) return result;
        }

        /* Set to Master Mode */
        regAddr = HYPHY20G_DMI_STFA_REG_PYLD_CFG_FOR_STS_3_STS_1_NO_1;
        regAddr = HYPHY20G_DMI_MTSB_SLICE_REG_OFFSET(regAddr, slice);
        if (sts3Off == 15) {
            regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port+1);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, 0, 1);
            if (result) return result;
        } else {
            regAddr = HYPHY20G_DMI_STFA_TSB_SLICE_REG_OFFSET(regAddr, port);
            result = sysHyPhy20gBitWrite(fileHndl, regAddr, sts3Num+1, 1);
            if (result) return result;
        }
    }

    return HYPHY20G_SUCCESS;
} /* hyPhy20gDmiSts3TslotCfg */


/******************************************************************************/
/* PRIVATE FUNCTIONS                                                          */
/******************************************************************************/

